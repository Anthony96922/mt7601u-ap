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
    BigInteger

    
    Revision History:
    Who         When            What
    --------    ----------      ------------------------------------------
    Eddy        2009/01/12      Create
***************************************************************************/

#ifndef __CRYPT_BIGINTEGER_H__
#define __CRYPT_BIGINTEGER_H__

#include "rt_config.h"


/* BigInteger definition & structure */
#define SLIDING_WINDOW 16
typedef struct _BIG_INTEGER_STRUC
{
	char Name[10];
	unsigned int *pIntegerArray;
	unsigned int AllocSize;
	unsigned int ArrayLength;
	unsigned int IntegerLength;
	int  Signed;
} BIG_INTEGER, *PBIG_INTEGER;


/* BigInteger operations */
void BigInteger_Print (
    IN PBIG_INTEGER pBI);
    
void BigInteger_Init (
    INOUT PBIG_INTEGER *pBI);

void BigInteger_Free_AllocSize (
    IN PBIG_INTEGER *pBI);

void BigInteger_Free (
    IN PBIG_INTEGER *pBI);

void BigInteger_AllocSize (
    IN PBIG_INTEGER *pBI,
    IN int Length);

void BigInteger_ClearHighBits (
    IN PBIG_INTEGER pBI);

void BigInteger_BI2Bin (
    IN PBIG_INTEGER pBI, 
    OUT unsigned char *pValue,
    OUT unsigned int *Length);

void BigInteger_Bin2BI (
    IN unsigned char *pValue,
    IN unsigned int Length,
    OUT PBIG_INTEGER *pBI);

void BigInteger_BitsOfBI (
    IN PBIG_INTEGER pBI,
    OUT unsigned int *Bits_Of_P);

int BigInteger_GetBitValue (
    IN PBIG_INTEGER pBI,
    IN unsigned int Index);

unsigned char BigInteger_GetByteValue (
    IN PBIG_INTEGER pBI,
    IN unsigned int Index);

void BigInteger_Copy (
    IN PBIG_INTEGER pBI_Copied,
    OUT PBIG_INTEGER *pBI_Result);

int BigInteger_UnsignedCompare (
    IN PBIG_INTEGER pFirstOperand,
    IN PBIG_INTEGER pSecondOperand);

void BigInteger_Add (
    IN PBIG_INTEGER pFirstOperand,
    IN PBIG_INTEGER pSecondOperand, 
    OUT PBIG_INTEGER *pBI_Result);

void BigInteger_Sub (
    IN PBIG_INTEGER pFirstOperand, 
    IN PBIG_INTEGER pSecondOperand, 
    OUT PBIG_INTEGER *pBI_Result);

void BigInteger_Mul (
    IN PBIG_INTEGER pFirstOperand, 
    IN PBIG_INTEGER pSecondOperand, 
    OUT PBIG_INTEGER *pBI_Result);

void BigInteger_Square (
    IN PBIG_INTEGER pBI, 
    OUT PBIG_INTEGER *pBI_Result);
    
void BigInteger_Div (
    IN PBIG_INTEGER pFirstOperand, 
    IN PBIG_INTEGER pSecondOperand, 
    OUT PBIG_INTEGER *pBI_Result,
    OUT PBIG_INTEGER *pBI_Remainder);

void BigInteger_Montgomery_Reduction (
    IN PBIG_INTEGER pBI_A,
    IN PBIG_INTEGER pBI_P,
    IN PBIG_INTEGER pBI_R,
    OUT PBIG_INTEGER *pBI_Result);

void BigInteger_Montgomery_ExpMod (
    IN PBIG_INTEGER pBI_G,
    IN PBIG_INTEGER pBI_E,
    IN PBIG_INTEGER pBI_P,
    OUT PBIG_INTEGER *pBI_Result);


#endif /* __CRYPT_BIGINTEGER_H__ */

