/* md5.h
 *
 * Copyright (C) 2006-2016 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */


#ifndef WOLF_CRYPT_MD5_H
#define WOLF_CRYPT_MD5_H

#include <wolfssl/wolfcrypt/types.h>

#ifndef NO_MD5

#ifdef HAVE_FIPS
    #define wc_InitMd5   InitMd5
    #define wc_Md5Update Md5Update
    #define wc_Md5Final  Md5Final
    #define wc_Md5Hash   Md5Hash
#endif

#ifdef __cplusplus
    extern "C" {
#endif

/* in bytes */
enum {
#if defined(STM32F2_HASH) || defined(STM32F4_HASH)
    MD5_REG_SIZE    =  4,      /* STM32 register size, bytes */
#endif
    MD5             =  0,      /* hash type unique */
    MD5_BLOCK_SIZE  = 64,
    MD5_DIGEST_SIZE = 16,
    MD5_PAD_SIZE    = 56
};

#if defined(WOLFSSL_PIC32MZ_HASH)
    #include "port/pic32/pic32mz-crypt.h"
#endif
#ifdef WOLFSSL_ASYNC_CRYPT
    #include <wolfssl/wolfcrypt/async.h>
#endif

#ifdef WOLFSSL_TI_HASH
    #include "wolfssl/wolfcrypt/port/ti/ti-hash.h"
#else

/* MD5 digest */
typedef struct Md5 {
    word32  buffLen;   /* in bytes          */
    word32  loLen;     /* length in bytes   */
    word32  hiLen;     /* length in bytes   */
    word32  buffer[MD5_BLOCK_SIZE  / sizeof(word32)];
#if !defined(WOLFSSL_PIC32MZ_HASH)
    word32  digest[MD5_DIGEST_SIZE / sizeof(word32)];
#else
    word32  digest[PIC32_HASH_SIZE / sizeof(word32)];
    pic32mz_desc desc; /* Crypt Engine descriptor */
#endif
    void*   heap;
#ifdef WOLFSSL_ASYNC_CRYPT
    WC_ASYNC_DEV asyncDev;
#endif /* WOLFSSL_ASYNC_CRYPT */
} Md5;

#endif /* WOLFSSL_TI_HASH */

WOLFSSL_API int wc_InitMd5(Md5*);
WOLFSSL_API int wc_InitMd5_ex(Md5*, void*, int);
WOLFSSL_API int wc_Md5Update(Md5*, const byte*, word32);
WOLFSSL_API int wc_Md5Final(Md5*, byte*);
WOLFSSL_API void wc_Md5Free(Md5*);

WOLFSSL_API int  wc_Md5GetHash(Md5*, byte*);
WOLFSSL_API int  wc_Md5Copy(Md5*, Md5*);


#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* NO_MD5 */
#endif /* WOLF_CRYPT_MD5_H */
