/**
 * \file cipher_wrap.c
 * 
 * \brief Generic cipher wrapper for PolarSSL
 *
 * \author Adriaan de Jong <dejong@fox-it.com>
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

#if defined(POLARSSL_CIPHER_C)

#include "polarssl/cipher_wrap.h"

#if defined(POLARSSL_AES_C)
#include "polarssl/aes.h"
#endif

#if defined(POLARSSL_ARC4_C)
#include "polarssl/arc4.h"
#endif

#if defined(POLARSSL_CAMELLIA_C)
#include "polarssl/camellia.h"
#endif

#if defined(POLARSSL_DES_C)
#include "polarssl/des.h"
#endif

#if defined(POLARSSL_BLOWFISH_C)
#include "polarssl/blowfish.h"
#endif

#if defined(POLARSSL_GCM_C)
#include "polarssl/gcm.h"
#endif

#if defined(POLARSSL_MEMORY_C)
#include "polarssl/memory.h"
#else
#define polarssl_malloc     malloc
#define polarssl_free       free
#endif

#include <stdlib.h>

#if defined(POLARSSL_AES_C)

static int aes_crypt_ecb_wrap( void *ctx, operation_t operation,
        const unsigned char *input, unsigned char *output )
{
    return aes_crypt_ecb( (aes_context *) ctx, operation, input, output );
}

static int aes_crypt_cbc_wrap( void *ctx, operation_t operation, size_t length,
        unsigned char *iv, const unsigned char *input, unsigned char *output )
{
    return aes_crypt_cbc( (aes_context *) ctx, operation, length, iv, input, output );
}

static int aes_crypt_cfb128_wrap( void *ctx, operation_t operation, size_t length,
        size_t *iv_off, unsigned char *iv, const unsigned char *input, unsigned char *output )
{
#if defined(POLARSSL_CIPHER_MODE_CFB)
    return aes_crypt_cfb128( (aes_context *) ctx, operation, length, iv_off, iv, input, output );
#else
    ((void) ctx);
    ((void) operation);
    ((void) length);
    ((void) iv_off);
    ((void) iv);
    ((void) input);
    ((void) output);

    return POLARSSL_ERR_CIPHER_FEATURE_UNAVAILABLE;
#endif
}

static int aes_crypt_ctr_wrap( void *ctx, size_t length,
        size_t *nc_off, unsigned char *nonce_counter, unsigned char *stream_block,
        const unsigned char *input, unsigned char *output )
{
#if defined(POLARSSL_CIPHER_MODE_CTR)
    return aes_crypt_ctr( (aes_context *) ctx, length, nc_off, nonce_counter,
                          stream_block, input, output );
#else
    ((void) ctx);
    ((void) length);
    ((void) nc_off);
    ((void) nonce_counter);
    ((void) stream_block);
    ((void) input);
    ((void) output);

    return POLARSSL_ERR_CIPHER_FEATURE_UNAVAILABLE;
#endif
}

static int aes_setkey_dec_wrap( void *ctx, const unsigned char *key, unsigned int key_length )
{
    return aes_setkey_dec( (aes_context *) ctx, key, key_length );
}

static int aes_setkey_enc_wrap( void *ctx, const unsigned char *key, unsigned int key_length )
{
    return aes_setkey_enc( (aes_context *) ctx, key, key_length );
}

static void * aes_ctx_alloc( void )
{
    return polarssl_malloc( sizeof( aes_context ) );
}

static void aes_ctx_free( void *ctx )
{
    polarssl_free( ctx );
}

const cipher_base_t aes_info = {
    POLARSSL_CIPHER_ID_AES,
    aes_crypt_ecb_wrap,
    aes_crypt_cbc_wrap,
    aes_crypt_cfb128_wrap,
    aes_crypt_ctr_wrap,
    NULL,
    aes_setkey_enc_wrap,
    aes_setkey_dec_wrap,
    aes_ctx_alloc,
    aes_ctx_free
};

const cipher_info_t aes_128_ecb_info = {
    POLARSSL_CIPHER_AES_128_ECB,
    POLARSSL_MODE_ECB,
    128,
    "AES-128-ECB",
    16,
    0,
    16,
    &aes_info
};

const cipher_info_t aes_192_ecb_info = {
    POLARSSL_CIPHER_AES_192_ECB,
    POLARSSL_MODE_ECB,
    192,
    "AES-192-ECB",
    16,
    0,
    16,
    &aes_info
};

const cipher_info_t aes_256_ecb_info = {
    POLARSSL_CIPHER_AES_256_ECB,
    POLARSSL_MODE_ECB,
    256,
    "AES-256-ECB",
    16,
    0,
    16,
    &aes_info
};

const cipher_info_t aes_128_cbc_info = {
    POLARSSL_CIPHER_AES_128_CBC,
    POLARSSL_MODE_CBC,
    128,
    "AES-128-CBC",
    16,
    0,
    16,
    &aes_info
};

const cipher_info_t aes_192_cbc_info = {
    POLARSSL_CIPHER_AES_192_CBC,
    POLARSSL_MODE_CBC,
    192,
    "AES-192-CBC",
    16,
    0,
    16,
    &aes_info
};

const cipher_info_t aes_256_cbc_info = {
    POLARSSL_CIPHER_AES_256_CBC,
    POLARSSL_MODE_CBC,
    256,
    "AES-256-CBC",
    16,
    0,
    16,
    &aes_info
};

#if defined(POLARSSL_CIPHER_MODE_CFB)
const cipher_info_t aes_128_cfb128_info = {
    POLARSSL_CIPHER_AES_128_CFB128,
    POLARSSL_MODE_CFB,
    128,
    "AES-128-CFB128",
    16,
    0,
    16,
    &aes_info
};

const cipher_info_t aes_192_cfb128_info = {
    POLARSSL_CIPHER_AES_192_CFB128,
    POLARSSL_MODE_CFB,
    192,
    "AES-192-CFB128",
    16,
    0,
    16,
    &aes_info
};

const cipher_info_t aes_256_cfb128_info = {
    POLARSSL_CIPHER_AES_256_CFB128,
    POLARSSL_MODE_CFB,
    256,
    "AES-256-CFB128",
    16,
    0,
    16,
    &aes_info
};
#endif /* POLARSSL_CIPHER_MODE_CFB */

#if defined(POLARSSL_CIPHER_MODE_CTR)
const cipher_info_t aes_128_ctr_info = {
    POLARSSL_CIPHER_AES_128_CTR,
    POLARSSL_MODE_CTR,
    128,
    "AES-128-CTR",
    16,
    0,
    16,
    &aes_info
};

const cipher_info_t aes_192_ctr_info = {
    POLARSSL_CIPHER_AES_192_CTR,
    POLARSSL_MODE_CTR,
    192,
    "AES-192-CTR",
    16,
    0,
    16,
    &aes_info
};

const cipher_info_t aes_256_ctr_info = {
    POLARSSL_CIPHER_AES_256_CTR,
    POLARSSL_MODE_CTR,
    256,
    "AES-256-CTR",
    16,
    0,
    16,
    &aes_info
};
#endif /* POLARSSL_CIPHER_MODE_CTR */

#if defined(POLARSSL_GCM_C)
static void *gcm_ctx_alloc( void )
{
    return polarssl_malloc( sizeof( gcm_context ) );
}

static void gcm_ctx_free( void *ctx )
{
    polarssl_free( ctx );
}

static int gcm_setkey_wrap( void *ctx, const unsigned char *key, unsigned int key_length )
{
    return gcm_init( (gcm_context *) ctx, key, key_length );
}

const cipher_base_t gcm_aes_info = {
    POLARSSL_CIPHER_ID_AES,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    gcm_setkey_wrap,
    gcm_setkey_wrap,
    gcm_ctx_alloc,
    gcm_ctx_free,
};

const cipher_info_t aes_128_gcm_info = {
    POLARSSL_CIPHER_AES_128_GCM,
    POLARSSL_MODE_GCM,
    128,
    "AES-128-GCM",
    12,
    1,
    16,
    &gcm_aes_info
};

const cipher_info_t aes_192_gcm_info = {
    POLARSSL_CIPHER_AES_192_GCM,
    POLARSSL_MODE_GCM,
    192,
    "AES-192-GCM",
    12,
    1,
    16,
    &gcm_aes_info
};

const cipher_info_t aes_256_gcm_info = {
    POLARSSL_CIPHER_AES_256_GCM,
    POLARSSL_MODE_GCM,
    256,
    "AES-256-GCM",
    12,
    1,
    16,
    &gcm_aes_info
};
#endif /* POLARSSL_GCM_C */

#endif

#if defined(POLARSSL_CAMELLIA_C)

static int camellia_crypt_ecb_wrap( void *ctx, operation_t operation,
        const unsigned char *input, unsigned char *output )
{
    return camellia_crypt_ecb( (camellia_context *) ctx, operation, input, output );
}

static int camellia_crypt_cbc_wrap( void *ctx, operation_t operation, size_t length,
        unsigned char *iv, const unsigned char *input, unsigned char *output )
{
    return camellia_crypt_cbc( (camellia_context *) ctx, operation, length, iv, input, output );
}

static int camellia_crypt_cfb128_wrap( void *ctx, operation_t operation, size_t length,
        size_t *iv_off, unsigned char *iv, const unsigned char *input, unsigned char *output )
{
#if defined(POLARSSL_CIPHER_MODE_CFB)
    return camellia_crypt_cfb128( (camellia_context *) ctx, operation, length, iv_off, iv, input, output );
#else
    ((void) ctx);
    ((void) operation);
    ((void) length);
    ((void) iv_off);
    ((void) iv);
    ((void) input);
    ((void) output);

    return POLARSSL_ERR_CIPHER_FEATURE_UNAVAILABLE;
#endif
}

static int camellia_crypt_ctr_wrap( void *ctx, size_t length,
        size_t *nc_off, unsigned char *nonce_counter, unsigned char *stream_block,
        const unsigned char *input, unsigned char *output )
{
#if defined(POLARSSL_CIPHER_MODE_CTR)
    return camellia_crypt_ctr( (camellia_context *) ctx, length, nc_off, nonce_counter,
                          stream_block, input, output );
#else
    ((void) ctx);
    ((void) length);
    ((void) nc_off);
    ((void) nonce_counter);
    ((void) stream_block);
    ((void) input);
    ((void) output);

    return POLARSSL_ERR_CIPHER_FEATURE_UNAVAILABLE;
#endif
}

static int camellia_setkey_dec_wrap( void *ctx, const unsigned char *key, unsigned int key_length )
{
    return camellia_setkey_dec( (camellia_context *) ctx, key, key_length );
}

static int camellia_setkey_enc_wrap( void *ctx, const unsigned char *key, unsigned int key_length )
{
    return camellia_setkey_enc( (camellia_context *) ctx, key, key_length );
}

static void * camellia_ctx_alloc( void )
{
    return polarssl_malloc( sizeof( camellia_context ) );
}

static void camellia_ctx_free( void *ctx )
{
    polarssl_free( ctx );
}

const cipher_base_t camellia_info = {
    POLARSSL_CIPHER_ID_CAMELLIA,
    camellia_crypt_ecb_wrap,
    camellia_crypt_cbc_wrap,
    camellia_crypt_cfb128_wrap,
    camellia_crypt_ctr_wrap,
    NULL,
    camellia_setkey_enc_wrap,
    camellia_setkey_dec_wrap,
    camellia_ctx_alloc,
    camellia_ctx_free
};

const cipher_info_t camellia_128_ecb_info = {
    POLARSSL_CIPHER_CAMELLIA_128_ECB,
    POLARSSL_MODE_ECB,
    128,
    "CAMELLIA-128-ECB",
    16,
    0,
    16,
    &camellia_info
};

const cipher_info_t camellia_192_ecb_info = {
    POLARSSL_CIPHER_CAMELLIA_192_ECB,
    POLARSSL_MODE_ECB,
    192,
    "CAMELLIA-192-ECB",
    16,
    0,
    16,
    &camellia_info
};

const cipher_info_t camellia_256_ecb_info = {
    POLARSSL_CIPHER_CAMELLIA_256_ECB,
    POLARSSL_MODE_ECB,
    256,
    "CAMELLIA-256-ECB",
    16,
    0,
    16,
    &camellia_info
};

const cipher_info_t camellia_128_cbc_info = {
    POLARSSL_CIPHER_CAMELLIA_128_CBC,
    POLARSSL_MODE_CBC,
    128,
    "CAMELLIA-128-CBC",
    16,
    0,
    16,
    &camellia_info
};

const cipher_info_t camellia_192_cbc_info = {
    POLARSSL_CIPHER_CAMELLIA_192_CBC,
    POLARSSL_MODE_CBC,
    192,
    "CAMELLIA-192-CBC",
    16,
    0,
    16,
    &camellia_info
};

const cipher_info_t camellia_256_cbc_info = {
    POLARSSL_CIPHER_CAMELLIA_256_CBC,
    POLARSSL_MODE_CBC,
    256,
    "CAMELLIA-256-CBC",
    16,
    0,
    16,
    &camellia_info
};

#if defined(POLARSSL_CIPHER_MODE_CFB)
const cipher_info_t camellia_128_cfb128_info = {
    POLARSSL_CIPHER_CAMELLIA_128_CFB128,
    POLARSSL_MODE_CFB,
    128,
    "CAMELLIA-128-CFB128",
    16,
    0,
    16,
    &camellia_info
};

const cipher_info_t camellia_192_cfb128_info = {
    POLARSSL_CIPHER_CAMELLIA_192_CFB128,
    POLARSSL_MODE_CFB,
    192,
    "CAMELLIA-192-CFB128",
    16,
    0,
    16,
    &camellia_info
};

const cipher_info_t camellia_256_cfb128_info = {
    POLARSSL_CIPHER_CAMELLIA_256_CFB128,
    POLARSSL_MODE_CFB,
    256,
    "CAMELLIA-256-CFB128",
    16,
    0,
    16,
    &camellia_info
};
#endif /* POLARSSL_CIPHER_MODE_CFB */

#if defined(POLARSSL_CIPHER_MODE_CTR)
const cipher_info_t camellia_128_ctr_info = {
    POLARSSL_CIPHER_CAMELLIA_128_CTR,
    POLARSSL_MODE_CTR,
    128,
    "CAMELLIA-128-CTR",
    16,
    0,
    16,
    &camellia_info
};

const cipher_info_t camellia_192_ctr_info = {
    POLARSSL_CIPHER_CAMELLIA_192_CTR,
    POLARSSL_MODE_CTR,
    192,
    "CAMELLIA-192-CTR",
    16,
    0,
    16,
    &camellia_info
};

const cipher_info_t camellia_256_ctr_info = {
    POLARSSL_CIPHER_CAMELLIA_256_CTR,
    POLARSSL_MODE_CTR,
    256,
    "CAMELLIA-256-CTR",
    16,
    0,
    16,
    &camellia_info
};
#endif /* POLARSSL_CIPHER_MODE_CTR */

#endif

#if defined(POLARSSL_DES_C)

static int des_crypt_ecb_wrap( void *ctx, operation_t operation,
        const unsigned char *input, unsigned char *output )
{
    ((void) operation);
    return des_crypt_ecb( (des_context *) ctx, input, output );
}

static int des3_crypt_ecb_wrap( void *ctx, operation_t operation,
        const unsigned char *input, unsigned char *output )
{
    ((void) operation);
    return des3_crypt_ecb( (des3_context *) ctx, input, output );
}

static int des_crypt_cbc_wrap( void *ctx, operation_t operation, size_t length,
        unsigned char *iv, const unsigned char *input, unsigned char *output )
{
    return des_crypt_cbc( (des_context *) ctx, operation, length, iv, input, output );
}

static int des3_crypt_cbc_wrap( void *ctx, operation_t operation, size_t length,
        unsigned char *iv, const unsigned char *input, unsigned char *output )
{
    return des3_crypt_cbc( (des3_context *) ctx, operation, length, iv, input, output );
}

static int des_crypt_cfb128_wrap( void *ctx, operation_t operation, size_t length,
        size_t *iv_off, unsigned char *iv, const unsigned char *input, unsigned char *output )
{
    ((void) ctx);
    ((void) operation);
    ((void) length);
    ((void) iv_off);
    ((void) iv);
    ((void) input);
    ((void) output);

    return POLARSSL_ERR_CIPHER_FEATURE_UNAVAILABLE;
}

static int des_crypt_ctr_wrap( void *ctx, size_t length,
        size_t *nc_off, unsigned char *nonce_counter, unsigned char *stream_block,
        const unsigned char *input, unsigned char *output )
{
    ((void) ctx);
    ((void) length);
    ((void) nc_off);
    ((void) nonce_counter);
    ((void) stream_block);
    ((void) input);
    ((void) output);

    return POLARSSL_ERR_CIPHER_FEATURE_UNAVAILABLE;
}

static int des_setkey_dec_wrap( void *ctx, const unsigned char *key, unsigned int key_length )
{
    ((void) key_length);

    return des_setkey_dec( (des_context *) ctx, key );
}

static int des_setkey_enc_wrap( void *ctx, const unsigned char *key, unsigned int key_length )
{
    ((void) key_length);

    return des_setkey_enc( (des_context *) ctx, key );
}

static int des3_set2key_dec_wrap( void *ctx, const unsigned char *key, unsigned int key_length )
{
    ((void) key_length);

    return des3_set2key_dec( (des3_context *) ctx, key );
}

static int des3_set2key_enc_wrap( void *ctx, const unsigned char *key, unsigned int key_length )
{
    ((void) key_length);

    return des3_set2key_enc( (des3_context *) ctx, key );
}

static int des3_set3key_dec_wrap( void *ctx, const unsigned char *key, unsigned int key_length )
{
    ((void) key_length);

    return des3_set3key_dec( (des3_context *) ctx, key );
}

static int des3_set3key_enc_wrap( void *ctx, const unsigned char *key, unsigned int key_length )
{
    ((void) key_length);

    return des3_set3key_enc( (des3_context *) ctx, key );
}

static void * des_ctx_alloc( void )
{
    return polarssl_malloc( sizeof( des_context ) );
}

static void * des3_ctx_alloc( void )
{
    return polarssl_malloc( sizeof( des3_context ) );
}

static void des_ctx_free( void *ctx )
{
    polarssl_free( ctx );
}

const cipher_base_t des_info = {
    POLARSSL_CIPHER_ID_DES,
    des_crypt_ecb_wrap,
    des_crypt_cbc_wrap,
    des_crypt_cfb128_wrap,
    des_crypt_ctr_wrap,
    NULL,
    des_setkey_enc_wrap,
    des_setkey_dec_wrap,
    des_ctx_alloc,
    des_ctx_free
};

const cipher_info_t des_ecb_info = {
    POLARSSL_CIPHER_DES_ECB,
    POLARSSL_MODE_ECB,
    POLARSSL_KEY_LENGTH_DES,
    "DES-ECB",
    8,
    0,
    8,
    &des_info
};

const cipher_info_t des_cbc_info = {
    POLARSSL_CIPHER_DES_CBC,
    POLARSSL_MODE_CBC,
    POLARSSL_KEY_LENGTH_DES,
    "DES-CBC",
    8,
    0,
    8,
    &des_info
};

const cipher_base_t des_ede_info = {
    POLARSSL_CIPHER_ID_DES,
    des3_crypt_ecb_wrap,
    des3_crypt_cbc_wrap,
    des_crypt_cfb128_wrap,
    des_crypt_ctr_wrap,
    NULL,
    des3_set2key_enc_wrap,
    des3_set2key_dec_wrap,
    des3_ctx_alloc,
    des_ctx_free
};

const cipher_info_t des_ede_ecb_info = {
    POLARSSL_CIPHER_DES_EDE_ECB,
    POLARSSL_MODE_ECB,
    POLARSSL_KEY_LENGTH_DES_EDE,
    "DES-EDE-ECB",
    8,
    0,
    8,
    &des_ede_info
};

const cipher_info_t des_ede_cbc_info = {
    POLARSSL_CIPHER_DES_EDE_CBC,
    POLARSSL_MODE_CBC,
    POLARSSL_KEY_LENGTH_DES_EDE,
    "DES-EDE-CBC",
    8,
    0,
    8,
    &des_ede_info
};

const cipher_base_t des_ede3_info = {
    POLARSSL_CIPHER_ID_DES,
    des3_crypt_ecb_wrap,
    des3_crypt_cbc_wrap,
    des_crypt_cfb128_wrap,
    des_crypt_ctr_wrap,
    NULL,
    des3_set3key_enc_wrap,
    des3_set3key_dec_wrap,
    des3_ctx_alloc,
    des_ctx_free
};

const cipher_info_t des_ede3_ecb_info = {
    POLARSSL_CIPHER_DES_EDE3_ECB,
    POLARSSL_MODE_ECB,
    POLARSSL_KEY_LENGTH_DES_EDE3,
    "DES-EDE3-ECB",
    8,
    0,
    8,
    &des_ede3_info
};
const cipher_info_t des_ede3_cbc_info = {
    POLARSSL_CIPHER_DES_EDE3_CBC,
    POLARSSL_MODE_CBC,
    POLARSSL_KEY_LENGTH_DES_EDE3,
    "DES-EDE3-CBC",
    8,
    0,
    8,
    &des_ede3_info
};
#endif

#if defined(POLARSSL_BLOWFISH_C)

static int blowfish_crypt_ecb_wrap( void *ctx, operation_t operation,
        const unsigned char *input, unsigned char *output )
{
    return blowfish_crypt_ecb( (blowfish_context *) ctx, operation, input, output );
}

static int blowfish_crypt_cbc_wrap( void *ctx, operation_t operation, size_t length,
        unsigned char *iv, const unsigned char *input, unsigned char *output )
{
    return blowfish_crypt_cbc( (blowfish_context *) ctx, operation, length, iv, input, output );
}

static int blowfish_crypt_cfb64_wrap( void *ctx, operation_t operation, size_t length,
        size_t *iv_off, unsigned char *iv, const unsigned char *input, unsigned char *output )
{
#if defined(POLARSSL_CIPHER_MODE_CFB)
    return blowfish_crypt_cfb64( (blowfish_context *) ctx, operation, length, iv_off, iv, input, output );
#else
    ((void) ctx);
    ((void) operation);
    ((void) length);
    ((void) iv_off);
    ((void) iv);
    ((void) input);
    ((void) output);

    return POLARSSL_ERR_CIPHER_FEATURE_UNAVAILABLE;
#endif
}

static int blowfish_crypt_ctr_wrap( void *ctx, size_t length,
        size_t *nc_off, unsigned char *nonce_counter, unsigned char *stream_block,
        const unsigned char *input, unsigned char *output )
{
#if defined(POLARSSL_CIPHER_MODE_CTR)
    return blowfish_crypt_ctr( (blowfish_context *) ctx, length, nc_off, nonce_counter,
                          stream_block, input, output );
#else
    ((void) ctx);
    ((void) length);
    ((void) nc_off);
    ((void) nonce_counter);
    ((void) stream_block);
    ((void) input);
    ((void) output);

    return POLARSSL_ERR_CIPHER_FEATURE_UNAVAILABLE;
#endif
}

static int blowfish_setkey_wrap( void *ctx, const unsigned char *key, unsigned int key_length )
{
    return blowfish_setkey( (blowfish_context *) ctx, key, key_length );
}

static void * blowfish_ctx_alloc( void )
{
    return polarssl_malloc( sizeof( blowfish_context ) );
}

static void blowfish_ctx_free( void *ctx )
{
    polarssl_free( ctx );
}

const cipher_base_t blowfish_info = {
    POLARSSL_CIPHER_ID_BLOWFISH,
    blowfish_crypt_ecb_wrap,
    blowfish_crypt_cbc_wrap,
    blowfish_crypt_cfb64_wrap,
    blowfish_crypt_ctr_wrap,
    NULL,
    blowfish_setkey_wrap,
    blowfish_setkey_wrap,
    blowfish_ctx_alloc,
    blowfish_ctx_free
};

const cipher_info_t blowfish_ecb_info = {
    POLARSSL_CIPHER_BLOWFISH_ECB,
    POLARSSL_MODE_ECB,
    128,
    "BLOWFISH-ECB",
    8,
    0,
    8,
    &blowfish_info
};

const cipher_info_t blowfish_cbc_info = {
    POLARSSL_CIPHER_BLOWFISH_CBC,
    POLARSSL_MODE_CBC,
    128,
    "BLOWFISH-CBC",
    8,
    0,
    8,
    &blowfish_info
};

#if defined(POLARSSL_CIPHER_MODE_CFB)
const cipher_info_t blowfish_cfb64_info = {
    POLARSSL_CIPHER_BLOWFISH_CFB64,
    POLARSSL_MODE_CFB,
    128,
    "BLOWFISH-CFB64",
    8,
    0,
    8,
    &blowfish_info
};
#endif /* POLARSSL_CIPHER_MODE_CFB */

#if defined(POLARSSL_CIPHER_MODE_CTR)
const cipher_info_t blowfish_ctr_info = {
    POLARSSL_CIPHER_BLOWFISH_CTR,
    POLARSSL_MODE_CTR,
    128,
    "BLOWFISH-CTR",
    8,
    0,
    8,
    &blowfish_info
};
#endif /* POLARSSL_CIPHER_MODE_CTR */
#endif /* POLARSSL_BLOWFISH_C */

#if defined(POLARSSL_ARC4_C)
static int arc4_crypt_stream_wrap( void *ctx, size_t length,
                                   const unsigned char *input,
                                   unsigned char *output )
{
    return( arc4_crypt( (arc4_context *) ctx, length, input, output ) );
}

static int arc4_setkey_wrap( void *ctx, const unsigned char *key,
                             unsigned int key_length )
{
    /* we get key_length in bits, arc4 expects it in bytes */
    if( key_length % 8 != 0)
        return( POLARSSL_ERR_CIPHER_BAD_INPUT_DATA );

    arc4_setup( (arc4_context *) ctx, key, key_length / 8 );
    return( 0 );
}

static void * arc4_ctx_alloc( void )
{
    return polarssl_malloc( sizeof( arc4_context ) );
}

static void arc4_ctx_free( void *ctx )
{
    polarssl_free( ctx );
}

const cipher_base_t arc4_base_info = {
    POLARSSL_CIPHER_ID_ARC4,
    NULL,
    NULL,
    NULL,
    NULL,
    arc4_crypt_stream_wrap,
    arc4_setkey_wrap,
    arc4_setkey_wrap,
    arc4_ctx_alloc,
    arc4_ctx_free
};

const cipher_info_t arc4_128_info = {
    POLARSSL_CIPHER_ARC4_128,
    POLARSSL_MODE_STREAM,
    128,
    "ARC4-128",
    0,
    0,
    1,
    &arc4_base_info
};
#endif /* POLARSSL_ARC4_C */

#if defined(POLARSSL_CIPHER_NULL_CIPHER)
static int null_crypt_stream( void *ctx, size_t length,
                              const unsigned char *input,
                              unsigned char *output )
{
    ((void) ctx);
    memmove( output, input, length );
    return( 0 );
}

static int null_setkey( void *ctx, const unsigned char *key,
                        unsigned int key_length )
{
    ((void) ctx);
    ((void) key);
    ((void) key_length);

    return( 0 );
}

static void * null_ctx_alloc( void )
{
    return (void *) 1;
}

static void null_ctx_free( void *ctx )
{
    ((void) ctx);
}

const cipher_base_t null_base_info = {
    POLARSSL_CIPHER_ID_NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    null_crypt_stream,
    null_setkey,
    null_setkey,
    null_ctx_alloc,
    null_ctx_free
};

const cipher_info_t null_cipher_info = {
    POLARSSL_CIPHER_NULL,
    POLARSSL_MODE_STREAM,
    0,
    "NULL",
    0,
    0,
    1,
    &null_base_info
};
#endif /* defined(POLARSSL_CIPHER_NULL_CIPHER) */

#endif
