/*
 *  Convert PEM to DER
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

#include "polarssl/error.h"
#include "polarssl/base64.h"

#define DFL_FILENAME            "file.pem"
#define DFL_OUTPUT_FILENAME     "file.der"

#if !defined(POLARSSL_BASE64_C) || !defined(POLARSSL_FS_IO)
int main( int argc, char *argv[] )
{
    ((void) argc);
    ((void) argv);

    printf("POLARSSL_BASE64_C and/or POLARSSL_FS_IO not defined.\n");
    return( 0 );
}
#else
/*
 * global options
 */
struct options
{
    char *filename;             /* filename of the input file             */
    char *output_file;          /* where to store the output              */
} opt;

int convert_pem_to_der( const unsigned char *input, size_t ilen,
                        unsigned char *output, size_t *olen )
{
    int ret;
    const unsigned char *s1, *s2, *end = input + ilen;
    size_t len = 0;

    s1 = (unsigned char *) strstr( (char *) input, "-----BEGIN" );
    if( s1 == NULL )
        return( -1 );

    s2 = (unsigned char *) strstr( (char *) input, "-----END" );
    if( s2 == NULL )
        return( -1 );

    s1 += 10;
    while( s1 < end && *s1 != '-' )
        s1++;
    while( s1 < end && *s1 == '-' )
        s1++;
    if( *s1 == '\r' ) s1++;
    if( *s1 == '\n' ) s1++;

    if( s2 <= s1 || s2 > end )
        return( -1 );

    ret = base64_decode( NULL, &len, (const unsigned char *) s1, s2 - s1 );
    if( ret == POLARSSL_ERR_BASE64_INVALID_CHARACTER )
        return( ret );

    if( len > *olen )
        return( -1 );

    if( ( ret = base64_decode( output, &len, (const unsigned char *) s1,
                               s2 - s1 ) ) != 0 )
    {
        return( ret );
    }

    *olen = len;

    return( 0 );
}

/*
 * Load all data from a file into a given buffer.
 */
static int load_file( const char *path, unsigned char **buf, size_t *n )
{
    FILE *f;
    long size;

    if( ( f = fopen( path, "rb" ) ) == NULL )
        return( -1 );

    fseek( f, 0, SEEK_END );
    if( ( size = ftell( f ) ) == -1 )
    {
        fclose( f );
        return( -1 );
    }
    fseek( f, 0, SEEK_SET );

    *n = (size_t) size;

    if( *n + 1 == 0 ||
        ( *buf = (unsigned char *) malloc( *n + 1 ) ) == NULL )
    {
        fclose( f );
        return( -1 );
    }

    if( fread( *buf, 1, *n, f ) != *n )
    {
        fclose( f );
        free( *buf );
        return( -1 );
    }

    fclose( f );

    (*buf)[*n] = '\0';

    return( 0 );
}

/*
 * Write buffer to a file
 */
static int write_file( const char *path, unsigned char *buf, size_t n )
{
    FILE *f;

    if( ( f = fopen( path, "wb" ) ) == NULL )
        return( -1 );

    if( fwrite( buf, 1, n, f ) != n )
    {
        fclose( f );
        return( -1 );
    }

    fclose( f );
    return( 0 );
}

#define USAGE \
    "\n usage: pem2der param=<>...\n"                   \
    "\n acceptable parameters:\n"                       \
    "    filename=%%s         default: file.pem\n"      \
    "    output_file=%%s      default: file.der\n"      \
    "\n"

int main( int argc, char *argv[] )
{
    int ret = 0;
    unsigned char *pem_buffer = NULL;
    unsigned char der_buffer[4096];
    char buf[1024];
    size_t pem_size, der_size = sizeof(der_buffer);
    int i, j, n;
    char *p, *q;

    /*
     * Set to sane values
     */
    memset( buf, 0, sizeof(buf) );
    memset( der_buffer, 0, sizeof(der_buffer) );

    if( argc == 0 )
    {
    usage:
        printf( USAGE );
        goto exit;
    }

    opt.filename            = DFL_FILENAME;
    opt.output_file         = DFL_OUTPUT_FILENAME;

    for( i = 1; i < argc; i++ )
    {

        p = argv[i];
        if( ( q = strchr( p, '=' ) ) == NULL )
            goto usage;
        *q++ = '\0';

        n = strlen( p );
        for( j = 0; j < n; j++ )
        {
            if( argv[i][j] >= 'A' && argv[i][j] <= 'Z' )
                argv[i][j] |= 0x20;
        }

        if( strcmp( p, "filename" ) == 0 )
            opt.filename = q;
        else if( strcmp( p, "output_file" ) == 0 )
            opt.output_file = q;
        else
            goto usage;
    }

    /*
     * 1.1. Load the PEM file
     */
    printf( "\n  . Loading the PEM file ..." );
    fflush( stdout );

    ret = load_file( opt.filename, &pem_buffer, &pem_size );

    if( ret != 0 )
    {
#ifdef POLARSSL_ERROR_C
        error_strerror( ret, buf, 1024 );
#endif
        printf( " failed\n  !  load_file returned %d - %s\n\n", ret, buf );
        goto exit;
    }

    printf( " ok\n" );

    /*
     * 1.2. Convert from PEM to DER
     */
    printf( "  . Converting from PEM to DER ..." );
    fflush( stdout );

    if( ( ret = convert_pem_to_der( pem_buffer, pem_size, der_buffer, &der_size ) ) != 0 )
    {
#ifdef POLARSSL_ERROR_C
        error_strerror( ret, buf, 1024 );
#endif
        printf( " failed\n  !  convert_pem_to_der %d - %s\n\n", ret, buf );
        goto exit;
    }

    printf( " ok\n" );

    /*
     * 1.3. Write the DER file
     */
    printf( "  . Writing the DER file ..." );
    fflush( stdout );

    ret = write_file( opt.output_file, der_buffer, der_size );

    if( ret != 0 )
    {
#ifdef POLARSSL_ERROR_C
        error_strerror( ret, buf, 1024 );
#endif
        printf( " failed\n  !  write_file returned %d - %s\n\n", ret, buf );
        goto exit;
    }

    printf( " ok\n" );

exit:
    free( pem_buffer );

#if defined(_WIN32)
    printf( "  + Press Enter to exit this program.\n" );
    fflush( stdout ); getchar();
#endif

    return( ret );
}
#endif /* POLARSSL_BASE64_C && POLARSSL_FS_IO */
