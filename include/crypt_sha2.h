/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************/

/****************************************************************************
    Module Name:
    SHA2

    Abstract:
    FIPS 180-2: Secure Hash Standard (SHS)
    
    Revision History:
    Who         When            What
    --------    ----------      ------------------------------------------
    Eddy        2008/11/24      Create SHA1
    Eddy        2008/07/23      Create SHA256
***************************************************************************/

#ifndef __CRYPT_SHA2_H__
#define __CRYPT_SHA2_H__


/* Algorithm options */
#define SHA1_SUPPORT
#define SHA256_SUPPORT

#ifdef SHA1_SUPPORT
#define SHA1_BLOCK_SIZE    64	/* 512 bits = 64 bytes */
#define SHA1_DIGEST_SIZE   20	/* 160 bits = 20 bytes */
typedef struct _SHA1_CTX_STRUC {
	unsigned int HashValue[5];	/* 5 = (SHA1_DIGEST_SIZE / 32) */
	unsigned long long MessageLen;	/* total size */
	unsigned char Block[SHA1_BLOCK_SIZE];
	unsigned int BlockLen;
} SHA1_CTX_STRUC, *PSHA1_CTX_STRUC;

VOID RT_SHA1_Init(
	IN SHA1_CTX_STRUC * pSHA_CTX);
VOID RT_SHA1_Hash(
	IN SHA1_CTX_STRUC * pSHA_CTX);
VOID RT_SHA1_Append(
	IN SHA1_CTX_STRUC * pSHA_CTX,
	IN const unsigned char Message[],
	IN unsigned int MessageLen);
VOID RT_SHA1_End(
	IN SHA1_CTX_STRUC * pSHA_CTX,
	OUT unsigned char DigestMessage[]);
VOID RT_SHA1(
	IN const unsigned char Message[],
	IN unsigned int MessageLen,
	OUT unsigned char DigestMessage[]);
#endif /* SHA1_SUPPORT */

#ifdef SHA256_SUPPORT
#define SHA256_BLOCK_SIZE   64	/* 512 bits = 64 bytes */
#define SHA256_DIGEST_SIZE  32	/* 256 bits = 32 bytes */
typedef struct _SHA256_CTX_STRUC {
	unsigned int HashValue[8];	/* 8 = (SHA256_DIGEST_SIZE / 32) */
	unsigned long long MessageLen;	/* total size */
	unsigned char Block[SHA256_BLOCK_SIZE];
	unsigned int BlockLen;
} SHA256_CTX_STRUC, *PSHA256_CTX_STRUC;

VOID RT_SHA256_Init(
	IN SHA256_CTX_STRUC * pSHA_CTX);
VOID RT_SHA256_Hash(
	IN SHA256_CTX_STRUC * pSHA_CTX);
VOID RT_SHA256_Append(
	IN SHA256_CTX_STRUC * pSHA_CTX,
	IN const unsigned char Message[],
	IN unsigned int MessageLen);
VOID RT_SHA256_End(
	IN SHA256_CTX_STRUC * pSHA_CTX,
	OUT unsigned char DigestMessage[]);
VOID RT_SHA256(
	IN const unsigned char Message[],
	IN unsigned int MessageLen,
	OUT unsigned char DigestMessage[]);
#endif /* SHA256_SUPPORT */


#endif /* __CRYPT_SHA2_H__ */
