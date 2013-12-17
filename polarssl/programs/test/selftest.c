/*
 *  Self-test demonstration program
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
#include <stdio.h>

#include "polarssl/ctr_drbg.h"
#include "polarssl/dhm.h"
#include "polarssl/gcm.h"
#include "polarssl/md2.h"
#include "polarssl/md4.h"
#include "polarssl/md5.h"
#include "polarssl/sha1.h"
#include "polarssl/sha256.h"
#include "polarssl/sha512.h"
#include "polarssl/arc4.h"
#include "polarssl/des.h"
#include "polarssl/aes.h"
#include "polarssl/camellia.h"
#include "polarssl/base64.h"
#include "polarssl/bignum.h"
#include "polarssl/rsa.h"
#include "polarssl/x509.h"
#include "polarssl/xtea.h"
#include "polarssl/pbkdf2.h"
#include "polarssl/ecp.h"

#if defined(POLARSSL_MEMORY_BUFFER_ALLOC_C)
#include "polarssl/memory.h"
#endif

int main( int argc, char *argv[] )
{
    int ret = 0, v;
#if defined(POLARSSL_MEMORY_BUFFER_ALLOC_C)
    unsigned char buf[1000000];
#endif

    if( argc == 2 && strcmp( argv[1], "-quiet" ) == 0 )
        v = 0;
    else
    {
        v = 1;
        printf( "\n" );
    }

#if defined(POLARSSL_SELF_TEST)

#if defined(POLARSSL_MEMORY_BUFFER_ALLOC_C)
    memory_buffer_alloc_init( buf, sizeof(buf) );
#endif

#if defined(POLARSSL_MD2_C)
    if( ( ret = md2_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_MD4_C)
    if( ( ret = md4_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_MD5_C)
    if( ( ret = md5_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_SHA1_C)
    if( ( ret = sha1_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_SHA256_C)
    if( ( ret = sha256_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_SHA512_C)
    if( ( ret = sha512_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_ARC4_C)
    if( ( ret = arc4_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_DES_C)
    if( ( ret = des_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_AES_C)
    if( ( ret = aes_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_GCM_C)
    if( ( ret = gcm_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_BASE64_C)
    if( ( ret = base64_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_BIGNUM_C)
    if( ( ret = mpi_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_RSA_C) && defined(POLARSSL_BIGNUM_C)
    if( ( ret = rsa_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_X509_USE_C)
    if( ( ret = x509_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_XTEA_C)
    if( ( ret = xtea_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_CAMELLIA_C)
    if( ( ret = camellia_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_CTR_DRBG_C)
    if( ( ret = ctr_drbg_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_PBKDF2_C)
    if( ( ret = pbkdf2_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_ECP_C)
    if( ( ret = ecp_self_test( v ) ) != 0 )
        return( ret );
#endif

#if defined(POLARSSL_DHM_C)
    if( ( ret = dhm_self_test( v ) ) != 0 )
        return( ret );
#endif

#else
    printf( " POLARSSL_SELF_TEST not defined.\n" );
#endif

    if( v != 0 )
    {
#if defined(POLARSSL_MEMORY_BUFFER_ALLOC_C) && defined(POLARSSL_MEMORY_DEBUG)
        memory_buffer_alloc_status();
#endif

        printf( "  [ All tests passed ]\n\n" );
#if defined(_WIN32)
        printf( "  Press Enter to exit this program.\n" );
        fflush( stdout ); getchar();
#endif
    }
#if defined(POLARSSL_MEMORY_BUFFER_ALLOC_C)
    memory_buffer_alloc_free();
#endif

    return( ret );
}
