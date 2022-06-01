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
    AES

    Abstract:
    RFC 3394: Advanced Encryption Standard (AES) Key Wrap Algorithm    
    RFC 3601: Counter with CBC-MAC (CCM)
    RFC 4493: The AES-CMAC Algorithm
    FIPS PUBS 197: ADVANCED ENCRYPTION STANDARD (AES)
    NIST 800-38A: Recommendation for Block Cipher Modes of Operation
    NIST 800-38C: The CCM Mode for Authentication and Confidentiality 
    
    Revision History:
    Who         When            What
    --------    ----------      ------------------------------------------
    Eddy        2009/05/19      Create AES-Key Wrap
    Eddy        2009/04/20      Create AES-CMAC, AES-CCM    
    Eddy        2009/01/19      Create AES-128, AES-192, AES-256, AES-CBC
***************************************************************************/

#ifndef __CRYPT_AES_H__
#define __CRYPT_AES_H__

#include "rt_config.h"


/* AES definition & structure */
#define AES_STATE_ROWS 4     /* Block size: 4*4*8 = 128 bits */
#define AES_STATE_COLUMNS 4
#define AES_BLOCK_SIZES AES_STATE_ROWS*AES_STATE_COLUMNS
#define AES_KEY_ROWS 4
#define AES_KEY_COLUMNS 8    /*Key length: 4*{4,6,8}*8 = 128, 192, 256 bits */
#define AES_KEY128_LENGTH 16
#define AES_KEY192_LENGTH 24
#define AES_KEY256_LENGTH 32
#define AES_CBC_IV_LENGTH 16

typedef struct {
    unsigned char State[AES_STATE_ROWS][AES_STATE_COLUMNS];
    unsigned char KeyWordExpansion[AES_KEY_ROWS][AES_KEY_ROWS*((AES_KEY256_LENGTH >> 2) + 6 + 1)];
} AES_CTX_STRUC, *PAES_CTX_STRUC;


/* AES operations */
VOID RT_AES_KeyExpansion (
    IN unsigned char Key[],
    IN unsigned int KeyLength,
    INOUT AES_CTX_STRUC *paes_ctx);

VOID RT_AES_Encrypt (
    IN unsigned char PlainBlock[],
    IN unsigned int PlainBlockSize,
    IN unsigned char Key[],
    IN unsigned int KeyLength,
    OUT unsigned char CipherBlock[],
    INOUT unsigned int *CipherBlockSize);

VOID RT_AES_Decrypt (
    IN unsigned char CipherBlock[],
    IN unsigned int CipherBlockSize,
    IN unsigned char Key[],
    IN unsigned int KeyLength,
    OUT unsigned char PlainBlock[],
    INOUT unsigned int *PlainBlockSize);

/* AES Counter with CBC-MAC operations */
VOID AES_CCM_MAC (
    IN unsigned char Payload[],
    IN unsigned int  PayloadLength,
    IN unsigned char Key[],
    IN unsigned int  KeyLength,
    IN unsigned char Nonce[],
    IN unsigned int  NonceLength,
    IN unsigned char AAD[],
    IN unsigned int  AADLength,
    IN unsigned int  MACLength,
    OUT unsigned char MACText[]);

INT AES_CCM_Encrypt (
    IN unsigned char PlainText[],
    IN unsigned int  PlainTextLength,
    IN unsigned char Key[],
    IN unsigned int  KeyLength,
    IN unsigned char Nonce[],
    IN unsigned int  NonceLength,
    IN unsigned char AAD[],
    IN unsigned int  AADLength,
    IN unsigned int  MACLength,
    OUT unsigned char CipherText[],
    INOUT unsigned int *CipherTextLength);

INT AES_CCM_Decrypt (
    IN unsigned char CipherText[],
    IN unsigned int  CipherTextLength,
    IN unsigned char Key[],
    IN unsigned int  KeyLength,
    IN unsigned char Nonce[],
    IN unsigned int  NonceLength,
    IN unsigned char AAD[],
    IN unsigned int  AADLength,
    IN unsigned int  MACLength,
    OUT unsigned char PlainText[],
    INOUT unsigned int *PlainTextLength);

/* AES-CMAC operations */
VOID AES_CMAC_GenerateSubKey (
    IN unsigned char Key[],
    IN unsigned int KeyLength,
    OUT unsigned char SubKey1[],
    OUT unsigned char SubKey2[]);

VOID AES_CMAC (
    IN unsigned char PlainText[],
    IN unsigned int PlainTextLength,
    IN unsigned char Key[],
    IN unsigned int KeyLength,
    OUT unsigned char MACText[],
    INOUT unsigned int *MACTextLength);



/* AES-CBC operations */
VOID AES_CBC_Encrypt (
    IN unsigned char PlainText[],
    IN unsigned int PlainTextLength,
    IN unsigned char Key[],
    IN unsigned int KeyLength,
    IN unsigned char IV[],
    IN unsigned int IVLength,
    OUT unsigned char CipherText[],
    INOUT unsigned int *CipherTextLength);

VOID AES_CBC_Decrypt (
    IN unsigned char CipherText[],
    IN unsigned int CipherTextLength,
    IN unsigned char Key[],
    IN unsigned int KeyLength,
    IN unsigned char IV[],
    IN unsigned int IVLength,
    OUT unsigned char PlainText[],
    INOUT unsigned int *PlainTextLength);

/* AES key wrap operations */
INT AES_Key_Wrap (
    IN unsigned char PlainText[],
    IN unsigned int  PlainTextLength,
    IN unsigned char Key[],
    IN unsigned int  KeyLength,
    OUT unsigned char CipherText[],
    OUT unsigned int *CipherTextLength);
        
INT AES_Key_Unwrap (
    IN unsigned char CipherText[],
    IN unsigned int  CipherTextLength,
    IN unsigned char Key[],
    IN unsigned int  KeyLength,
    OUT unsigned char PlainText [],
    OUT unsigned int *PlainTextLength);


#endif /* __CRYPT_AES_H__ */

