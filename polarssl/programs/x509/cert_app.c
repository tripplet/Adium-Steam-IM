/*
 *  Certificate reading application
 *
 *  Copyright (C) 2006-2013, Brainspark B.V.
 *
 *  This file is part of PolarSSL (http://www.polarssl.org)
 *  Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "polarssl/config.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "polarssl/entropy.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/net.h"
#include "polarssl/ssl.h"
#include "polarssl/x509.h"

#if !defined(POLARSSL_BIGNUM_C) || !defined(POLARSSL_ENTROPY_C) ||  \
    !defined(POLARSSL_SSL_TLS_C) || !defined(POLARSSL_SSL_CLI_C) || \
    !defined(POLARSSL_NET_C) || !defined(POLARSSL_RSA_C) ||         \
    !defined(POLARSSL_X509_CRT_PARSE_C) || !defined(POLARSSL_FS_IO) ||  \
    !defined(POLARSSL_CTR_DRBG_C)
int main( int argc, char *argv[] )
{
    ((void) argc);
    ((void) argv);

    printf("POLARSSL_BIGNUM_C and/or POLARSSL_ENTROPY_C and/or "
           "POLARSSL_SSL_TLS_C and/or POLARSSL_SSL_CLI_C and/or "
           "POLARSSL_NET_C and/or POLARSSL_RSA_C and/or "
           "POLARSSL_X509_CRT_PARSE_C and/or POLARSSL_FS_IO and/or "
           "POLARSSL_CTR_DRBG_C not defined.\n");
    return( 0 );
}
#else

#define MODE_NONE               0
#define MODE_FILE               1
#define MODE_SSL                2

#define DFL_MODE                MODE_NONE
#define DFL_FILENAME            "cert.crt"
#define DFL_CA_FILE             ""
#define DFL_CA_PATH             ""
#define DFL_SERVER_NAME         "localhost"
#define DFL_SERVER_PORT         4433
#define DFL_DEBUG_LEVEL         0
#define DFL_PERMISSIVE          0

/*
 * global options
 */
struct options
{
    int mode;                   /* the mode to run the application in   */
    const char *filename;       /* filename of the certificate file     */
    const char *ca_file;        /* the file with the CA certificate(s)  */
    const char *ca_path;        /* the path with the CA certificate(s) reside */
    const char *server_name;    /* hostname of the server (client only) */
    int server_port;            /* port on which the ssl service runs   */
    int debug_level;            /* level of debugging                   */
    int permissive;             /* permissive parsing                   */
} opt;

static void my_debug( void *ctx, int level, const char *str )
{
    if( level < opt.debug_level )
    {
        fprintf( (FILE *) ctx, "%s", str );
        fflush(  (FILE *) ctx  );
    }
}

static int my_verify( void *data, x509_crt *crt, int depth, int *flags )
{
    char buf[1024];
    ((void) data);

    printf( "\nVerify requested for (Depth %d):\n", depth );
    x509_crt_info( buf, sizeof( buf ) - 1, "", crt );
    printf( "%s", buf );

    if( ( (*flags) & BADCERT_EXPIRED ) != 0 )
        printf( "  ! server certificate has expired\n" );

    if( ( (*flags) & BADCERT_REVOKED ) != 0 )
        printf( "  ! server certificate has been revoked\n" );

    if( ( (*flags) & BADCERT_CN_MISMATCH ) != 0 )
        printf( "  ! CN mismatch\n" );

    if( ( (*flags) & BADCERT_NOT_TRUSTED ) != 0 )
        printf( "  ! self-signed or not signed by a trusted CA\n" );

    if( ( (*flags) & BADCRL_NOT_TRUSTED ) != 0 )
        printf( "  ! CRL not trusted\n" );

    if( ( (*flags) & BADCRL_EXPIRED ) != 0 )
        printf( "  ! CRL expired\n" );

    if( ( (*flags) & BADCERT_OTHER ) != 0 )
        printf( "  ! other (unknown) flag\n" );

    if ( ( *flags ) == 0 )
        printf( "  This certificate has no flags\n" );

    return( 0 );
}

#define USAGE_IO \
    "    ca_file=%%s          The single file containing the top-level CA(s) you fully trust\n" \
    "                        default: \"\" (none)\n" \
    "    ca_path=%%s          The path containing the top-level CA(s) you fully trust\n" \
    "                        default: \"\" (none) (overrides ca_file)\n"

#define USAGE \
    "\n usage: cert_app param=<>...\n"                  \
    "\n acceptable parameters:\n"                       \
    "    mode=file|ssl       default: none\n"           \
    "    filename=%%s         default: cert.crt\n"      \
    USAGE_IO                                            \
    "    server_name=%%s      default: localhost\n"     \
    "    server_port=%%d      default: 4433\n"          \
    "    debug_level=%%d      default: 0 (disabled)\n"  \
    "    permissive=%%d       default: 0 (disabled)\n"  \
    "\n"

int main( int argc, char *argv[] )
{
    int ret = 0, server_fd;
    unsigned char buf[1024];
    entropy_context entropy;
    ctr_drbg_context ctr_drbg;
    ssl_context ssl;
    x509_crt cacert;
    x509_crt clicert;
    pk_context pkey;
    int i, j;
    int flags, verify = 0;
    char *p, *q;
    const char *pers = "cert_app";

    /*
     * Set to sane values
     */
    server_fd = 0;
    x509_crt_init( &cacert );
    x509_crt_init( &clicert );
    pk_init( &pkey );

    if( argc == 0 )
    {
    usage:
        printf( USAGE );
        goto exit;
    }

    opt.mode                = DFL_MODE;
    opt.filename            = DFL_FILENAME;
    opt.ca_file             = DFL_CA_FILE;
    opt.ca_path             = DFL_CA_PATH;
    opt.server_name         = DFL_SERVER_NAME;
    opt.server_port         = DFL_SERVER_PORT;
    opt.debug_level         = DFL_DEBUG_LEVEL;
    opt.permissive          = DFL_PERMISSIVE;

    for( i = 1; i < argc; i++ )
    {
        p = argv[i];
        if( ( q = strchr( p, '=' ) ) == NULL )
            goto usage;
        *q++ = '\0';

        for( j = 0; p + j < q; j++ )
        {
            if( argv[i][j] >= 'A' && argv[i][j] <= 'Z' )
                argv[i][j] |= 0x20;
        }

        if( strcmp( p, "mode" ) == 0 )
        {
            if( strcmp( q, "file" ) == 0 )
                opt.mode = MODE_FILE;
            else if( strcmp( q, "ssl" ) == 0 )
                opt.mode = MODE_SSL;
            else
                goto usage;
        }
        else if( strcmp( p, "filename" ) == 0 )
            opt.filename = q;
        else if( strcmp( p, "ca_file" ) == 0 )
            opt.ca_file = q;
        else if( strcmp( p, "ca_path" ) == 0 )
            opt.ca_path = q;
        else if( strcmp( p, "server_name" ) == 0 )
            opt.server_name = q;
        else if( strcmp( p, "server_port" ) == 0 )
        {
            opt.server_port = atoi( q );
            if( opt.server_port < 1 || opt.server_port > 65535 )
                goto usage;
        }
        else if( strcmp( p, "debug_level" ) == 0 )
        {
            opt.debug_level = atoi( q );
            if( opt.debug_level < 0 || opt.debug_level > 65535 )
                goto usage;
        }
        else if( strcmp( p, "permissive" ) == 0 )
        {
            opt.permissive = atoi( q );
            if( opt.permissive < 0 || opt.permissive > 1 )
                goto usage;
        }
        else
            goto usage;
    }

    /*
     * 1.1. Load the trusted CA
     */
    printf( "  . Loading the CA root certificate ..." );
    fflush( stdout );

    if( strlen( opt.ca_path ) )
    {
        ret = x509_crt_parse_path( &cacert, opt.ca_path );
        verify = 1;
    }
    else if( strlen( opt.ca_file ) )
    {
        ret = x509_crt_parse_file( &cacert, opt.ca_file );
        verify = 1;
    }

    if( ret < 0 )
    {
        printf( " failed\n  !  x509_crt_parse returned -0x%x\n\n", -ret );
        goto exit;
    }

    printf( " ok (%d skipped)\n", ret );

    if( opt.mode == MODE_FILE )
    {
        x509_crt crt;
        x509_crt *cur = &crt;
        x509_crt_init( &crt );

        /*
         * 1.1. Load the certificate(s)
         */
        printf( "\n  . Loading the certificate(s) ..." );
        fflush( stdout );

        ret = x509_crt_parse_file( &crt, opt.filename );

        if( ret < 0 )
        {
            printf( " failed\n  !  x509_crt_parse_file returned %d\n\n", ret );
            x509_crt_free( &crt );
            goto exit;
        }

        if( opt.permissive == 0 && ret > 0 )
        {
            printf( " failed\n  !  x509_crt_parse failed to parse %d certificates\n\n", ret );
            x509_crt_free( &crt );
            goto exit;
        }

        printf( " ok\n" );

        /*
         * 1.2 Print the certificate(s)
         */
        while( cur != NULL )
        {
            printf( "  . Peer certificate information    ...\n" );
            ret = x509_crt_info( (char *) buf, sizeof( buf ) - 1, "      ",
                                 cur );
            if( ret == -1 )
            {
                printf( " failed\n  !  x509_crt_info returned %d\n\n", ret );
                x509_crt_free( &crt );
                goto exit;
            }

            printf( "%s\n", buf );

            cur = cur->next;
        }

        /*
         * 1.3 Verify the certificate
         */
        if( verify )
        {
            printf( "  . Verifying X.509 certificate..." );

            if( ( ret = x509_crt_verify( &crt, &cacert, NULL, NULL, &flags,
                                         my_verify, NULL ) ) != 0 )
            {
                printf( " failed\n" );

                if( ( ret & BADCERT_EXPIRED ) != 0 )
                    printf( "  ! server certificate has expired\n" );

                if( ( ret & BADCERT_REVOKED ) != 0 )
                    printf( "  ! server certificate has been revoked\n" );

                if( ( ret & BADCERT_CN_MISMATCH ) != 0 )
                    printf( "  ! CN mismatch (expected CN=%s)\n", opt.server_name );

                if( ( ret & BADCERT_NOT_TRUSTED ) != 0 )
                    printf( "  ! self-signed or not signed by a trusted CA\n" );

                printf( "\n" );
            }
            else
                printf( " ok\n" );
        }

        x509_crt_free( &crt );
    }
    else if( opt.mode == MODE_SSL )
    {
        /*
         * 1. Initialize the RNG and the session data
         */
        printf( "\n  . Seeding the random number generator..." );
        fflush( stdout );

        entropy_init( &entropy );
        if( ( ret = ctr_drbg_init( &ctr_drbg, entropy_func, &entropy,
                                   (const unsigned char *) pers,
                                   strlen( pers ) ) ) != 0 )
        {
            printf( " failed\n  ! ctr_drbg_init returned %d\n", ret );
            goto exit;
        }

        printf( " ok\n" );

        /*
         * 2. Start the connection
         */
        printf( "  . SSL connection to tcp/%s/%-4d...", opt.server_name,
                                                        opt.server_port );
        fflush( stdout );

        if( ( ret = net_connect( &server_fd, opt.server_name,
                                             opt.server_port ) ) != 0 )
        {
            printf( " failed\n  ! net_connect returned %d\n\n", ret );
            goto exit;
        }

        /*
         * 3. Setup stuff
         */
        if( ( ret = ssl_init( &ssl ) ) != 0 )
        {
            printf( " failed\n  ! ssl_init returned %d\n\n", ret );
            goto exit;
        }

        ssl_set_endpoint( &ssl, SSL_IS_CLIENT );
        if( verify )
        {
            ssl_set_authmode( &ssl, SSL_VERIFY_REQUIRED );
            ssl_set_ca_chain( &ssl, &cacert, NULL, opt.server_name );
            ssl_set_verify( &ssl, my_verify, NULL );
        }
        else
            ssl_set_authmode( &ssl, SSL_VERIFY_NONE );

        ssl_set_rng( &ssl, ctr_drbg_random, &ctr_drbg );
        ssl_set_dbg( &ssl, my_debug, stdout );
        ssl_set_bio( &ssl, net_recv, &server_fd,
                net_send, &server_fd );

        ssl_set_own_cert( &ssl, &clicert, &pkey );

#if defined(POLARSSL_SSL_SERVER_NAME_INDICATION)
        ssl_set_hostname( &ssl, opt.server_name );
#endif

        /*
         * 4. Handshake
         */
        while( ( ret = ssl_handshake( &ssl ) ) != 0 )
        {
            if( ret != POLARSSL_ERR_NET_WANT_READ && ret != POLARSSL_ERR_NET_WANT_WRITE )
            {
                printf( " failed\n  ! ssl_handshake returned %d\n\n", ret );
                ssl_free( &ssl );
                goto exit;
            }
        }

        printf( " ok\n" );

        /*
         * 5. Print the certificate
         */
        printf( "  . Peer certificate information    ...\n" );
        ret = x509_crt_info( (char *) buf, sizeof( buf ) - 1, "      ",
                             ssl.session->peer_cert );
        if( ret == -1 )
        {
            printf( " failed\n  !  x509_crt_info returned %d\n\n", ret );
            ssl_free( &ssl );
            goto exit;
        }

        printf( "%s\n", buf );

        ssl_close_notify( &ssl );
        ssl_free( &ssl );
    }
    else
        goto usage;

exit:

    if( server_fd )
        net_close( server_fd );
    x509_crt_free( &cacert );
    x509_crt_free( &clicert );
    pk_free( &pkey );
    entropy_free( &entropy );

#if defined(_WIN32)
    printf( "  + Press Enter to exit this program.\n" );
    fflush( stdout ); getchar();
#endif

    return( ret );
}
#endif /* POLARSSL_BIGNUM_C && POLARSSL_ENTROPY_C && POLARSSL_SSL_TLS_C &&
          POLARSSL_SSL_CLI_C && POLARSSL_NET_C && POLARSSL_RSA_C &&
          POLARSSL_X509_CRT_PARSE_C && POLARSSL_FS_IO && POLARSSL_CTR_DRBG_C */
