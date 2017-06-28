/* sha.c
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


#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#include <wolfssl/wolfcrypt/settings.h>

#if !defined(NO_SHA)

#include <wolfssl/wolfcrypt/sha.h>
#include <wolfssl/wolfcrypt/error-crypt.h>

/* fips wrapper calls, user can call direct */
#ifdef HAVE_FIPS
	int wc_InitSha(Sha* sha)
	{
	    return InitSha_fips(sha);
	}
    int wc_InitSha_ex(Sha* sha, void* heap, int devId)
    {
        (void)heap;
        (void)devId;
        return InitSha_fips(sha);
    }

	int wc_ShaUpdate(Sha* sha, const byte* data, word32 len)
	{
	    return ShaUpdate_fips(sha, data, len);
	}

	int wc_ShaFinal(Sha* sha, byte* out)
	{
	    return ShaFinal_fips(sha,out);
    }
    void wc_ShaFree(Sha* sha)
    {
        (void)sha;
        /* Not supported in FIPS */
    }

#else /* else build without fips */


#if defined(WOLFSSL_TI_HASH)
    /* #include <wolfcrypt/src/port/ti/ti-hash.c> included by wc_port.c */

#else

#include <wolfssl/wolfcrypt/logging.h>
#ifdef NO_INLINE
    #include <wolfssl/wolfcrypt/misc.h>
#else
    #define WOLFSSL_MISC_INCLUDED
    #include <wolfcrypt/src/misc.c>
#endif


/* Hardware Acceleration */
#if defined(WOLFSSL_PIC32MZ_HASH)
    #define USE_SHA_SOFTWARE_IMPL
    #define wc_InitSha   wc_InitSha_sw
    #define wc_ShaUpdate wc_ShaUpdate_sw
    #define wc_ShaFinal  wc_ShaFinal_sw

#elif defined(STM32F2_HASH) || defined(STM32F4_HASH)

    /*
     * STM32F2/F4 hardware SHA1 support through the standard peripheral
     * library. (See note in README).
     */

    static int InitSha(Sha* sha)
    {
        /* STM32 struct notes:
         * sha->buffer  = first 4 bytes used to hold partial block if needed
         * sha->buffLen = num bytes currently stored in sha->buffer
         * sha->loLen   = num bytes that have been written to STM32 FIFO
         */
        XMEMSET(sha->buffer, 0, SHA_REG_SIZE);
        sha->buffLen = 0;
        sha->loLen = 0;

        /* initialize HASH peripheral */
        HASH_DeInit();

        /* configure algo used, algo mode, datatype */
        HASH->CR &= ~ (HASH_CR_ALGO | HASH_CR_DATATYPE | HASH_CR_MODE);
        HASH->CR |= (HASH_AlgoSelection_SHA1 | HASH_AlgoMode_HASH
                     | HASH_DataType_8b);

        /* reset HASH processor */
        HASH->CR |= HASH_CR_INIT;

        return 0;
    }

    int wc_ShaUpdate(Sha* sha, const byte* data, word32 len)
    {
        word32 i = 0;
        word32 fill = 0;
        word32 diff = 0;

        /* if saved partial block is available */
        if (sha->buffLen) {
            fill = 4 - sha->buffLen;

            /* if enough data to fill, fill and push to FIFO */
            if (fill <= len) {
                XMEMCPY((byte*)sha->buffer + sha->buffLen, data, fill);
                HASH_DataIn(*(uint32_t*)sha->buffer);

                data += fill;
                len -= fill;
                sha->loLen += 4;
                sha->buffLen = 0;
            } else {
                /* append partial to existing stored block */
                XMEMCPY((byte*)sha->buffer + sha->buffLen, data, len);
                sha->buffLen += len;
                return 0;
            }
        }

        /* write input block in the IN FIFO */
        for(i = 0; i < len; i += 4)
        {
            diff = len - i;
            if ( diff < 4) {
                /* store incomplete last block, not yet in FIFO */
                XMEMSET(sha->buffer, 0, SHA_REG_SIZE);
                XMEMCPY((byte*)sha->buffer, data, diff);
                sha->buffLen = diff;
            } else {
                HASH_DataIn(*(uint32_t*)data);
                data+=4;
            }
        }

        /* keep track of total data length thus far */
        sha->loLen += (len - sha->buffLen);

        return 0;
    }

    int wc_ShaFinal(Sha* sha, byte* hash)
    {
        __IO uint16_t nbvalidbitsdata = 0;

        /* finish reading any trailing bytes into FIFO */
        if (sha->buffLen) {
            HASH_DataIn(*(uint32_t*)sha->buffer);
            sha->loLen += sha->buffLen;
        }

        /* calculate number of valid bits in last word of input data */
        nbvalidbitsdata = 8 * (sha->loLen % SHA_REG_SIZE);

        /* configure number of valid bits in last word of the data */
        HASH_SetLastWordValidBitsNbr(nbvalidbitsdata);

        /* start HASH processor */
        HASH_StartDigest();

        /* wait until Busy flag == RESET */
        while (HASH_GetFlagStatus(HASH_FLAG_BUSY) != RESET) {}

        /* read message digest */
        sha->digest[0] = HASH->HR[0];
        sha->digest[1] = HASH->HR[1];
        sha->digest[2] = HASH->HR[2];
        sha->digest[3] = HASH->HR[3];
        sha->digest[4] = HASH->HR[4];

        ByteReverseWords(sha->digest, sha->digest, SHA_DIGEST_SIZE);

        XMEMCPY(hash, sha->digest, SHA_DIGEST_SIZE);

        return wc_InitSha(sha);  /* reset state */
    }


#elif defined(FREESCALE_LTC_SHA)

    #include "fsl_ltc.h"
    static int InitSha(Sha* sha)
    {
        LTC_HASH_Init(LTC_BASE, &sha->ctx, kLTC_Sha1, NULL, 0);
        return 0;
    }

    int wc_ShaUpdate(Sha* sha, const byte* data, word32 len)
    {
        LTC_HASH_Update(&sha->ctx, data, len);
        return 0;
    }

    int wc_ShaFinal(Sha* sha, byte* hash)
    {
        uint32_t hashlen = SHA_DIGEST_SIZE;
        LTC_HASH_Finish(&sha->ctx, hash, &hashlen);
        return wc_InitSha(sha);  /* reset state */
    }


#elif defined(FREESCALE_MMCAU_SHA)

    #include "fsl_mmcau.h"
    #define USE_SHA_SOFTWARE_IMPL /* Only for API's, actual transform is here */
    #define XSHATRANSFORM   ShaTransform

    static int InitSha(Sha* sha)
    {
        int ret = 0;
        ret = wolfSSL_CryptHwMutexLock();
        if(ret != 0) {
            return ret;
        }
        MMCAU_SHA1_InitializeOutput((uint32_t*)sha->digest);
        wolfSSL_CryptHwMutexUnLock();

        sha->buffLen = 0;
        sha->loLen   = 0;
        sha->hiLen   = 0;

        return ret;
    }

    static int ShaTransform(Sha* sha, byte* data)
    {
        int ret = wolfSSL_CryptHwMutexLock();
        if(ret == 0) {
            MMCAU_SHA1_HashN(data, 1, (uint32_t*)sha->digest);
            wolfSSL_CryptHwMutexUnLock();
        }
        return ret;
    }

#else

    /* Software implementation */
    #define USE_SHA_SOFTWARE_IMPL

    static int InitSha(Sha* sha)
    {
        int ret = 0;

        sha->digest[0] = 0x67452301L;
        sha->digest[1] = 0xEFCDAB89L;
        sha->digest[2] = 0x98BADCFEL;
        sha->digest[3] = 0x10325476L;
        sha->digest[4] = 0xC3D2E1F0L;

        sha->buffLen = 0;
        sha->loLen   = 0;
        sha->hiLen   = 0;

        return ret;
    }

#endif /* End Hardware Acceleration */


/* Software implementation */
#ifdef USE_SHA_SOFTWARE_IMPL

/* Check if custom Sha transform is used */
#ifndef XSHATRANSFORM
    #define XSHATRANSFORM   ShaTransform

    #define blk0(i) (W[i] = sha->buffer[i])
    #define blk1(i) (W[(i)&15] = \
        rotlFixed(W[((i)+13)&15]^W[((i)+8)&15]^W[((i)+2)&15]^W[(i)&15],1))

    #define f1(x,y,z) ((z)^((x) &((y)^(z))))
    #define f2(x,y,z) ((x)^(y)^(z))
    #define f3(x,y,z) (((x)&(y))|((z)&((x)|(y))))
    #define f4(x,y,z) ((x)^(y)^(z))

    /* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
    #define R0(v,w,x,y,z,i) (z)+= f1((w),(x),(y)) + blk0((i)) + 0x5A827999+ \
        rotlFixed((v),5); (w) = rotlFixed((w),30);
    #define R1(v,w,x,y,z,i) (z)+= f1((w),(x),(y)) + blk1((i)) + 0x5A827999+ \
        rotlFixed((v),5); (w) = rotlFixed((w),30);
    #define R2(v,w,x,y,z,i) (z)+= f2((w),(x),(y)) + blk1((i)) + 0x6ED9EBA1+ \
        rotlFixed((v),5); (w) = rotlFixed((w),30);
    #define R3(v,w,x,y,z,i) (z)+= f3((w),(x),(y)) + blk1((i)) + 0x8F1BBCDC+ \
        rotlFixed((v),5); (w) = rotlFixed((w),30);
    #define R4(v,w,x,y,z,i) (z)+= f4((w),(x),(y)) + blk1((i)) + 0xCA62C1D6+ \
        rotlFixed((v),5); (w) = rotlFixed((w),30);

    static void ShaTransform(Sha* sha, byte* data)
    {
        word32 W[SHA_BLOCK_SIZE / sizeof(word32)];

        /* Copy context->state[] to working vars */
        word32 a = sha->digest[0];
        word32 b = sha->digest[1];
        word32 c = sha->digest[2];
        word32 d = sha->digest[3];
        word32 e = sha->digest[4];

    #ifdef USE_SLOW_SHA
        word32 t, i;

        for (i = 0; i < 16; i++) {
            R0(a, b, c, d, e, i);
            t = e; e = d; d = c; c = b; b = a; a = t;
        }

        for (; i < 20; i++) {
            R1(a, b, c, d, e, i);
            t = e; e = d; d = c; c = b; b = a; a = t;
        }

        for (; i < 40; i++) {
            R2(a, b, c, d, e, i);
            t = e; e = d; d = c; c = b; b = a; a = t;
        }

        for (; i < 60; i++) {
            R3(a, b, c, d, e, i);
            t = e; e = d; d = c; c = b; b = a; a = t;
        }

        for (; i < 80; i++) {
            R4(a, b, c, d, e, i);
            t = e; e = d; d = c; c = b; b = a; a = t;
        }
    #else
        /* nearly 1 K bigger in code size but 25% faster  */
        /* 4 rounds of 20 operations each. Loop unrolled. */
        R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
        R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
        R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
        R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);

        R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);

        R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
        R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
        R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
        R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
        R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);

        R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
        R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
        R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
        R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
        R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);

        R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
        R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
        R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
        R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
        R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);
    #endif

        /* Add the working vars back into digest state[] */
        sha->digest[0] += a;
        sha->digest[1] += b;
        sha->digest[2] += c;
        sha->digest[3] += d;
        sha->digest[4] += e;

        (void)data; /* Not used */
    }
#endif /* !USE_CUSTOM_SHA_TRANSFORM */


static INLINE void AddLength(Sha* sha, word32 len)
{
    word32 tmp = sha->loLen;
    if ( (sha->loLen += len) < tmp)
        sha->hiLen++;                       /* carry low to high */
}

int wc_InitSha_ex(Sha* sha, void* heap, int devId)
{
    int ret = 0;

    if (sha == NULL)
        return BAD_FUNC_ARG;

    sha->heap = heap;

    ret = InitSha(sha);
    if (ret != 0)
        return ret;

#if defined(WOLFSSL_ASYNC_CRYPT) && defined(WC_ASYNC_ENABLE_SHA)
    ret = wolfAsync_DevCtxInit(&sha->asyncDev, WOLFSSL_ASYNC_MARKER_SHA,
                                                            sha->heap, devId);
#else
    (void)devId;
#endif /* WOLFSSL_ASYNC_CRYPT */

    return ret;
}

int wc_ShaUpdate(Sha* sha, const byte* data, word32 len)
{
    /* do block size increments */
    byte* local = (byte*)sha->buffer;

#if defined(WOLFSSL_ASYNC_CRYPT) && defined(WC_ASYNC_ENABLE_SHA)
    if (sha->asyncDev.marker == WOLFSSL_ASYNC_MARKER_SHA) {
    #if defined(HAVE_INTEL_QA)
        return IntelQaSymSha(&sha->asyncDev, NULL, data, len);
    #endif
    }
#endif /* WOLFSSL_ASYNC_CRYPT */

    /* check that internal buffLen is valid */
    if (sha->buffLen >= SHA_BLOCK_SIZE)
        return BUFFER_E;

    while (len) {
        word32 add = min(len, SHA_BLOCK_SIZE - sha->buffLen);
        XMEMCPY(&local[sha->buffLen], data, add);

        sha->buffLen += add;
        data         += add;
        len          -= add;

        if (sha->buffLen == SHA_BLOCK_SIZE) {
#if defined(LITTLE_ENDIAN_ORDER) && !defined(FREESCALE_MMCAU_SHA)
            ByteReverseWords(sha->buffer, sha->buffer, SHA_BLOCK_SIZE);
#endif
            XSHATRANSFORM(sha, local);
            AddLength(sha, SHA_BLOCK_SIZE);
            sha->buffLen = 0;
        }
    }

    return 0;
}

int wc_ShaFinal(Sha* sha, byte* hash)
{
    byte* local = (byte*)sha->buffer;

#if defined(WOLFSSL_ASYNC_CRYPT) && defined(WC_ASYNC_ENABLE_SHA)
    if (sha->asyncDev.marker == WOLFSSL_ASYNC_MARKER_SHA) {
    #if defined(HAVE_INTEL_QA)
        return IntelQaSymSha(&sha->asyncDev, hash, NULL, SHA_DIGEST_SIZE);
    #endif
    }
#endif /* WOLFSSL_ASYNC_CRYPT */

    AddLength(sha, sha->buffLen);  /* before adding pads */

    local[sha->buffLen++] = 0x80;  /* add 1 */

    /* pad with zeros */
    if (sha->buffLen > SHA_PAD_SIZE) {
        XMEMSET(&local[sha->buffLen], 0, SHA_BLOCK_SIZE - sha->buffLen);
        sha->buffLen += SHA_BLOCK_SIZE - sha->buffLen;

#if defined(LITTLE_ENDIAN_ORDER) && !defined(FREESCALE_MMCAU_SHA)
        ByteReverseWords(sha->buffer, sha->buffer, SHA_BLOCK_SIZE);
#endif
        XSHATRANSFORM(sha, local);
        sha->buffLen = 0;
    }
    XMEMSET(&local[sha->buffLen], 0, SHA_PAD_SIZE - sha->buffLen);

    /* put lengths in bits */
    sha->hiLen = (sha->loLen >> (8*sizeof(sha->loLen) - 3)) +
    (sha->hiLen << 3);
    sha->loLen = sha->loLen << 3;

    /* store lengths */
#if defined(LITTLE_ENDIAN_ORDER) && !defined(FREESCALE_MMCAU_SHA)
    ByteReverseWords(sha->buffer, sha->buffer, SHA_BLOCK_SIZE);
#endif
    /* ! length ordering dependent on digest endian type ! */
    XMEMCPY(&local[SHA_PAD_SIZE], &sha->hiLen, sizeof(word32));
    XMEMCPY(&local[SHA_PAD_SIZE + sizeof(word32)], &sha->loLen, sizeof(word32));

#ifdef FREESCALE_MMCAU_SHA
    /* Kinetis requires only these bytes reversed */
    ByteReverseWords(&sha->buffer[SHA_PAD_SIZE/sizeof(word32)],
                     &sha->buffer[SHA_PAD_SIZE/sizeof(word32)],
                     2 * sizeof(word32));
#endif

    XSHATRANSFORM(sha, local);
#ifdef LITTLE_ENDIAN_ORDER
    ByteReverseWords(sha->digest, sha->digest, SHA_DIGEST_SIZE);
#endif
    XMEMCPY(hash, sha->digest, SHA_DIGEST_SIZE);

    return InitSha(sha); /* reset state */
}

#endif /* USE_SHA_SOFTWARE_IMPL */


int wc_InitSha(Sha* sha)
{
    return wc_InitSha_ex(sha, NULL, INVALID_DEVID);
}

void wc_ShaFree(Sha* sha)
{
    if (sha == NULL)
        return;

#if defined(WOLFSSL_ASYNC_CRYPT) && defined(WC_ASYNC_ENABLE_SHA)
    wolfAsync_DevCtxFree(&sha->asyncDev, WOLFSSL_ASYNC_MARKER_SHA);
#endif /* WOLFSSL_ASYNC_CRYPT */
}

#endif /* !WOLFSSL_TI_HASH */
#endif /* HAVE_FIPS */

#ifndef WOLFSSL_TI_HASH
int wc_ShaGetHash(Sha* sha, byte* hash)
{
    int ret;
    Sha tmpSha;

    if (sha == NULL || hash == NULL)
        return BAD_FUNC_ARG;

    ret = wc_ShaCopy(sha, &tmpSha);
    if (ret == 0) {
        ret = wc_ShaFinal(&tmpSha, hash);
    }
    return ret;
}

int wc_ShaCopy(Sha* src, Sha* dst)
{
    int ret = 0;

    if (src == NULL || dst == NULL)
        return BAD_FUNC_ARG;

    XMEMCPY(dst, src, sizeof(Sha));

#ifdef WOLFSSL_ASYNC_CRYPT
    ret = wolfAsync_DevCopy(&src->asyncDev, &dst->asyncDev);
#endif

    return ret;
}
#endif /* !WOLFSSL_TI_HASH */

#endif /* !NO_SHA */
