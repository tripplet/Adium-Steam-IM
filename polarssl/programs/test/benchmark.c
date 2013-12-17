/*
 *  Benchmark demonstration program
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

#include "polarssl/timing.h"

#include "polarssl/md4.h"
#include "polarssl/md5.h"
#include "polarssl/sha1.h"
#include "polarssl/sha256.h"
#include "polarssl/sha512.h"
#include "polarssl/arc4.h"
#include "polarssl/des.h"
#include "polarssl/aes.h"
#include "polarssl/blowfish.h"
#include "polarssl/camellia.h"
#include "polarssl/gcm.h"
#include "polarssl/havege.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/rsa.h"
#include "polarssl/dhm.h"
#include "polarssl/ecdsa.h"
#include "polarssl/ecdh.h"

#if defined _MSC_VER && !defined snprintf
#define snprintf _snprintf
#endif

#define BUFSIZE         1024
#define HEADER_FORMAT   "  %-18s :  "
#define TITLE_LEN       19

#if !defined(POLARSSL_TIMING_C)
int main( int argc, char *argv[] )
{
    ((void) argc);
    ((void) argv);

    printf("POLARSSL_TIMING_C not defined.\n");
    return( 0 );
}
#else

static int myrand( void *rng_state, unsigned char *output, size_t len )
{
    size_t use_len;
    int rnd;

    if( rng_state != NULL )
        rng_state  = NULL;

    while( len > 0 )
    {
        use_len = len;
        if( use_len > sizeof(int) )
            use_len = sizeof(int);

        rnd = rand();
        memcpy( output, &rnd, use_len );
        output += use_len;
        len -= use_len;
    }

    return( 0 );
}

#define TIME_AND_TSC( TITLE, CODE )                                     \
do {                                                                    \
    unsigned long i, j, tsc;                                            \
                                                                        \
    printf( HEADER_FORMAT, TITLE );                                     \
    fflush( stdout );                                                   \
                                                                        \
    set_alarm( 1 );                                                     \
    for( i = 1; ! alarmed; i++ )                                        \
    {                                                                   \
        CODE;                                                           \
    }                                                                   \
                                                                        \
    tsc = hardclock();                                                  \
    for( j = 0; j < 1024; j++ )                                         \
    {                                                                   \
        CODE;                                                           \
    }                                                                   \
                                                                        \
    printf( "%9lu Kb/s,  %9lu cycles/byte\n", i * BUFSIZE / 1024,       \
                    ( hardclock() - tsc ) / ( j * BUFSIZE ) );          \
} while( 0 )

#define TIME_PUBLIC( TITLE, TYPE, CODE )                                \
do {                                                                    \
    unsigned long i;                                                    \
    int ret;                                                            \
                                                                        \
    printf( HEADER_FORMAT, TITLE );                                     \
    fflush( stdout );                                                   \
    set_alarm( 3 );                                                     \
                                                                        \
    ret = 0;                                                            \
    for( i = 1; ! alarmed && ! ret ; i++ )                              \
    {                                                                   \
        CODE;                                                           \
    }                                                                   \
                                                                        \
    if( ret != 0 )                                                      \
        printf( "FAILED\n" );                                           \
    else                                                                \
        printf( "%9lu " TYPE "/s\n", i / 3 );                                  \
} while( 0 )

unsigned char buf[BUFSIZE];

typedef struct {
    char md4, md5, sha1, sha256, sha512,
         arc4, des3, des, aes_cbc, aes_gcm, camellia, blowfish,
         havege, ctr_drbg,
         rsa, dhm, ecdsa, ecdh;
} todo_list;

#define OPTIONS                                                         \
    "md4, md5, sha1, sha256, sha512,\n"                                 \
    "arc4, des3, des, aes_cbc, aes_gcm, camellia, blowfish,\n"          \
    "havege, ctr_drbg,\n"                                               \
    "rsa, dhm, ecdsa, ecdh.\n"

int main( int argc, char *argv[] )
{
    int keysize, i;
    unsigned char tmp[200];
    char title[TITLE_LEN];
    todo_list todo;

    if( argc == 1 )
        memset( &todo, 1, sizeof( todo ) );
    else
    {
        memset( &todo, 0, sizeof( todo ) );

        for( i = 1; i < argc; i++ )
        {
            if( strcmp( argv[i], "md4" ) == 0 )
                todo.md4 = 1;
            else if( strcmp( argv[i], "md5" ) == 0 )
                todo.md5 = 1;
            else if( strcmp( argv[i], "sha1" ) == 0 )
                todo.sha1 = 1;
            else if( strcmp( argv[i], "sha256" ) == 0 )
                todo.sha256 = 1;
            else if( strcmp( argv[i], "sha512" ) == 0 )
                todo.sha512 = 1;
            else if( strcmp( argv[i], "arc4" ) == 0 )
                todo.arc4 = 1;
            else if( strcmp( argv[i], "des3" ) == 0 )
                todo.des3 = 1;
            else if( strcmp( argv[i], "des" ) == 0 )
                todo.des = 1;
            else if( strcmp( argv[i], "aes_cbc" ) == 0 )
                todo.aes_cbc = 1;
            else if( strcmp( argv[i], "aes_gcm" ) == 0 )
                todo.aes_gcm = 1;
            else if( strcmp( argv[i], "camellia" ) == 0 )
                todo.camellia = 1;
            else if( strcmp( argv[i], "blowfish" ) == 0 )
                todo.blowfish = 1;
            else if( strcmp( argv[i], "havege" ) == 0 )
                todo.havege = 1;
            else if( strcmp( argv[i], "ctr_drbg" ) == 0 )
                todo.ctr_drbg = 1;
            else if( strcmp( argv[i], "rsa" ) == 0 )
                todo.rsa = 1;
            else if( strcmp( argv[i], "dhm" ) == 0 )
                todo.dhm = 1;
            else if( strcmp( argv[i], "ecdsa" ) == 0 )
                todo.ecdsa = 1;
            else if( strcmp( argv[i], "ecdh" ) == 0 )
                todo.ecdh = 1;
            else
            {
                printf( "Unrecognized option: %s\n", argv[i] );
                printf( "Available options:" OPTIONS );
            }
        }
    }

    printf( "\n" );

    memset( buf, 0xAA, sizeof( buf ) );

#if defined(POLARSSL_MD4_C)
    if( todo.md4 )
        TIME_AND_TSC( "MD4", md4( buf, BUFSIZE, tmp ) );
#endif

#if defined(POLARSSL_MD5_C)
    if( todo.md5 )
        TIME_AND_TSC( "MD5", md5( buf, BUFSIZE, tmp ) );
#endif

#if defined(POLARSSL_SHA1_C)
    if( todo.sha1 )
        TIME_AND_TSC( "SHA-1", sha1( buf, BUFSIZE, tmp ) );
#endif

#if defined(POLARSSL_SHA256_C)
    if( todo.sha256 )
        TIME_AND_TSC( "SHA-256", sha256( buf, BUFSIZE, tmp, 0 ) );
#endif

#if defined(POLARSSL_SHA512_C)
    if( todo.sha512 )
        TIME_AND_TSC( "SHA-512", sha512( buf, BUFSIZE, tmp, 0 ) );
#endif

#if defined(POLARSSL_ARC4_C)
    if( todo.arc4 )
    {
        arc4_context arc4;
        arc4_setup( &arc4, tmp, 32 );
        TIME_AND_TSC( "ARC4", arc4_crypt( &arc4, BUFSIZE, buf, buf ) );
    }
#endif

#if defined(POLARSSL_DES_C) && defined(POLARSSL_CIPHER_MODE_CBC)
    if( todo.des3 )
    {
        des3_context des3;
        des3_set3key_enc( &des3, tmp );
        TIME_AND_TSC( "3DES",
                des3_crypt_cbc( &des3, DES_ENCRYPT, BUFSIZE, tmp, buf, buf ) );
    }

    if( todo.des )
    {
        des_context des;
        des_setkey_enc( &des, tmp );
        TIME_AND_TSC( "DES",
                des_crypt_cbc( &des, DES_ENCRYPT, BUFSIZE, tmp, buf, buf ) );
    }
#endif

#if defined(POLARSSL_AES_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    if( todo.aes_cbc )
    {
        aes_context aes;
        for( keysize = 128; keysize <= 256; keysize += 64 )
        {
            snprintf( title, sizeof( title ), "AES-CBC-%d", keysize );

            memset( buf, 0, sizeof( buf ) );
            memset( tmp, 0, sizeof( tmp ) );
            aes_setkey_enc( &aes, tmp, keysize );

            TIME_AND_TSC( title,
                aes_crypt_cbc( &aes, AES_ENCRYPT, BUFSIZE, tmp, buf, buf ) );
        }
    }
#endif
#if defined(POLARSSL_GCM_C)
    if( todo.aes_gcm )
    {
        gcm_context gcm;
        for( keysize = 128; keysize <= 256; keysize += 64 )
        {
            snprintf( title, sizeof( title ), "AES-GCM-%d", keysize );

            memset( buf, 0, sizeof( buf ) );
            memset( tmp, 0, sizeof( tmp ) );
            gcm_init( &gcm, POLARSSL_CIPHER_ID_AES, tmp, keysize );

            TIME_AND_TSC( title,
                    gcm_crypt_and_tag( &gcm, GCM_ENCRYPT, BUFSIZE, tmp,
                        12, NULL, 0, buf, buf, 16, tmp ) );
        }
    }
#endif
#endif

#if defined(POLARSSL_CAMELLIA_C) && defined(POLARSSL_CIPHER_MODE_CBC)
    if( todo.camellia )
    {
        camellia_context camellia;
        for( keysize = 128; keysize <= 256; keysize += 64 )
        {
            snprintf( title, sizeof( title ), "CAMELLIA-CBC-%d", keysize );

            memset( buf, 0, sizeof( buf ) );
            memset( tmp, 0, sizeof( tmp ) );
            camellia_setkey_enc( &camellia, tmp, keysize );

            TIME_AND_TSC( title,
                    camellia_crypt_cbc( &camellia, CAMELLIA_ENCRYPT,
                        BUFSIZE, tmp, buf, buf ) );
        }
    }
#endif

#if defined(POLARSSL_BLOWFISH_C) && defined(POLARSSL_CIPHER_MODE_CBC)
    if( todo.blowfish )
    {
        blowfish_context blowfish;
        for( keysize = 128; keysize <= 256; keysize += 64 )
        {
            snprintf( title, sizeof( title ), "BLOWFISH-CBC-%d", keysize );

            memset( buf, 0, sizeof( buf ) );
            memset( tmp, 0, sizeof( tmp ) );
            blowfish_setkey( &blowfish, tmp, keysize );

            TIME_AND_TSC( title,
                    blowfish_crypt_cbc( &blowfish, BLOWFISH_ENCRYPT, BUFSIZE,
                        tmp, buf, buf ) );
        }
    }
#endif

#if defined(POLARSSL_HAVEGE_C)
    if( todo.havege )
    {
        havege_state hs;
        havege_init( &hs );
        TIME_AND_TSC( "HAVEGE", havege_random( &hs, buf, BUFSIZE ) );
    }
#endif

#if defined(POLARSSL_CTR_DRBG_C)
    if( todo.ctr_drbg )
    {
        ctr_drbg_context ctr_drbg;

        if( ctr_drbg_init( &ctr_drbg, myrand, NULL, NULL, 0 ) != 0 )
            exit(1);
        TIME_AND_TSC( "CTR_DRBG (NOPR)",
                if( ctr_drbg_random( &ctr_drbg, buf, BUFSIZE ) != 0 )
                exit(1) );

        if( ctr_drbg_init( &ctr_drbg, myrand, NULL, NULL, 0 ) != 0 )
            exit(1);
        ctr_drbg_set_prediction_resistance( &ctr_drbg, CTR_DRBG_PR_ON );
        TIME_AND_TSC( "CTR_DRBG (PR)",
                if( ctr_drbg_random( &ctr_drbg, buf, BUFSIZE ) != 0 )
                exit(1) );
    }
#endif

#if defined(POLARSSL_RSA_C) && defined(POLARSSL_GENPRIME)
    if( todo.rsa )
    {
        rsa_context rsa;
        for( keysize = 1024; keysize <= 4096; keysize *= 2 )
        {
            snprintf( title, sizeof( title ), "RSA-%d", keysize );

            rsa_init( &rsa, RSA_PKCS_V15, 0 );
            rsa_gen_key( &rsa, myrand, NULL, keysize, 65537 );

            TIME_PUBLIC( title, " public",
                    buf[0] = 0;
                    ret = rsa_public( &rsa, buf, buf ) );

            TIME_PUBLIC( title, "private",
                    buf[0] = 0;
                    ret = rsa_private( &rsa, myrand, NULL, buf, buf ) );

            rsa_free( &rsa );
        }
    }
#endif

#if defined(POLARSSL_DHM_C) && defined(POLARSSL_BIGNUM_C)
    if( todo.dhm )
    {
#define DHM_SIZES 3
        int dhm_sizes[DHM_SIZES] = { 1024, 2048, 3072 };
        const char *dhm_P[DHM_SIZES] = {
            POLARSSL_DHM_RFC5114_MODP_1024_P,
            POLARSSL_DHM_RFC3526_MODP_2048_P,
            POLARSSL_DHM_RFC3526_MODP_3072_P,
        };
        const char *dhm_G[DHM_SIZES] = {
            POLARSSL_DHM_RFC5114_MODP_1024_G,
            POLARSSL_DHM_RFC3526_MODP_2048_G,
            POLARSSL_DHM_RFC3526_MODP_3072_G,
        };

        dhm_context dhm;
        size_t olen;
        for( i = 0; i < DHM_SIZES; i++ )
        {
            memset( &dhm, 0, sizeof( dhm_context ) );

            mpi_read_string( &dhm.P, 16, dhm_P[i] );
            mpi_read_string( &dhm.G, 16, dhm_G[i] );
            dhm.len = mpi_size( &dhm.P );
            dhm_make_public( &dhm, dhm.len, buf, dhm.len, myrand, NULL );
            mpi_copy( &dhm.GY, &dhm.GX );

            snprintf( title, sizeof( title ), "DHE-%d", dhm_sizes[i] );
            TIME_PUBLIC( title, "handshake",
                    olen = sizeof( buf );
                    ret |= dhm_make_public( &dhm, dhm.len, buf, dhm.len,
                                            myrand, NULL );
                    ret |= dhm_calc_secret( &dhm, buf, &olen, myrand, NULL ) );

            snprintf( title, sizeof( title ), "DH-%d", dhm_sizes[i] );
            TIME_PUBLIC( title, "handshake",
                    olen = sizeof( buf );
                    ret |= dhm_calc_secret( &dhm, buf, &olen, myrand, NULL ) );

            dhm_free( &dhm );
        }
    }
#endif

#if defined(POLARSSL_ECDSA_C)
    if( todo.ecdsa )
    {
        ecdsa_context ecdsa;
        const ecp_curve_info *curve_info;
        size_t sig_len;

        memset( buf, 0x2A, sizeof( buf ) );

        for( curve_info = ecp_curve_list();
             curve_info->grp_id != POLARSSL_ECP_DP_NONE;
             curve_info++ )
        {
            ecdsa_init( &ecdsa );

            if( ecdsa_genkey( &ecdsa, curve_info->grp_id, myrand, NULL ) != 0 )
                exit( 1 );

            snprintf( title, sizeof( title ), "ECDSA-%s",
                                              curve_info->name );
            TIME_PUBLIC( title, "sign",
                    ret = ecdsa_write_signature( &ecdsa, buf, curve_info->size,
                                                tmp, &sig_len, myrand, NULL ) );

            TIME_PUBLIC( title, "verify",
                    ret = ecdsa_read_signature( &ecdsa, buf, curve_info->size,
                                                tmp, sig_len ) );

            ecdsa_free( &ecdsa );
        }
    }
#endif

#if defined(POLARSSL_ECDH_C)
    if( todo.ecdh )
    {
        ecdh_context ecdh;
        const ecp_curve_info *curve_info;
        size_t olen;

        for( curve_info = ecp_curve_list();
             curve_info->grp_id != POLARSSL_ECP_DP_NONE;
             curve_info++ )
        {
            ecdh_init( &ecdh );

            if( ecp_use_known_dp( &ecdh.grp, curve_info->grp_id ) != 0 ||
                ecdh_make_public( &ecdh, &olen, buf, sizeof( buf),
                                  myrand, NULL ) != 0 ||
                ecp_copy( &ecdh.Qp, &ecdh.Q ) != 0 )
            {
                exit( 1 );
            }

            snprintf( title, sizeof( title ), "ECDHE-%s",
                                              curve_info->name );
            TIME_PUBLIC( title, "handshake",
                    ret |= ecdh_make_public( &ecdh, &olen, buf, sizeof( buf),
                                             myrand, NULL );
                    ret |= ecdh_calc_secret( &ecdh, &olen, buf, sizeof( buf ),
                                             myrand, NULL ) );

            snprintf( title, sizeof( title ), "ECDH-%s",
                                              curve_info->name );
            TIME_PUBLIC( title, "handshake",
                    ret |= ecdh_calc_secret( &ecdh, &olen, buf, sizeof( buf ),
                                             myrand, NULL ) );
            ecdh_free( &ecdh );
        }
    }
#endif
    printf( "\n" );

#if defined(_WIN32)
    printf( "  Press Enter to exit this program.\n" );
    fflush( stdout ); getchar();
#endif

    return( 0 );
}

#endif /* POLARSSL_TIMING_C */
