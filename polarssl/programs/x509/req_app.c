/*
 *  Certificate request reading application
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

#include "polarssl/x509_csr.h"

#if !defined(POLARSSL_BIGNUM_C) || !defined(POLARSSL_RSA_C) ||  \
    !defined(POLARSSL_X509_CSR_PARSE_C) || !defined(POLARSSL_FS_IO)
int main( int argc, char *argv[] )
{
    ((void) argc);
    ((void) argv);

    printf("POLARSSL_BIGNUM_C and/or POLARSSL_RSA_C and/or "
           "POLARSSL_X509_CSR_PARSE_C and/or POLARSSL_FS_IO not defined.\n");
    return( 0 );
}
#else

#define DFL_FILENAME            "cert.req"
#define DFL_DEBUG_LEVEL         0

/*
 * global options
 */
struct options
{
    const char *filename;       /* filename of the certificate request  */
} opt;

#define USAGE \
    "\n usage: req_app param=<>...\n"                   \
    "\n acceptable parameters:\n"                       \
    "    filename=%%s         default: cert.req\n"      \
    "\n"

int main( int argc, char *argv[] )
{
    int ret = 0;
    unsigned char buf[100000];
    x509_csr csr;
    int i, j, n;
    char *p, *q;

    /*
     * Set to sane values
     */
    x509_csr_init( &csr );

    if( argc == 0 )
    {
    usage:
        printf( USAGE );
        goto exit;
    }

    opt.filename            = DFL_FILENAME;

    for( i = 1; i < argc; i++ )
    {
        n = strlen( argv[i] );

        for( j = 0; j < n; j++ )
        {
            if( argv[i][j] >= 'A' && argv[i][j] <= 'Z' )
                argv[i][j] |= 0x20;
        }

        p = argv[i];
        if( ( q = strchr( p, '=' ) ) == NULL )
            goto usage;
        *q++ = '\0';

        if( strcmp( p, "filename" ) == 0 )
            opt.filename = q;
        else
            goto usage;
    }

    /*
     * 1.1. Load the CSR
     */
    printf( "\n  . Loading the CSR ..." );
    fflush( stdout );

    ret = x509_csr_parse_file( &csr, opt.filename );

    if( ret != 0 )
    {
        printf( " failed\n  !  x509_csr_parse_file returned %d\n\n", ret );
        x509_csr_free( &csr );
        goto exit;
    }

    printf( " ok\n" );

    /*
     * 1.2 Print the CSR
     */
    printf( "  . CSR information    ...\n" );
    ret = x509_csr_info( (char *) buf, sizeof( buf ) - 1, "      ", &csr );
    if( ret == -1 )
    {
        printf( " failed\n  !  x509_csr_info returned %d\n\n", ret );
        x509_csr_free( &csr );
        goto exit;
    }

    printf( "%s\n", buf );

exit:
    x509_csr_free( &csr );

#if defined(_WIN32)
    printf( "  + Press Enter to exit this program.\n" );
    fflush( stdout ); getchar();
#endif

    return( ret );
}
#endif /* POLARSSL_BIGNUM_C && POLARSSL_RSA_C && POLARSSL_X509_CSR_PARSE_C &&
          POLARSSL_FS_IO */
