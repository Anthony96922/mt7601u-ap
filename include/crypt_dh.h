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
    DH

    Abstract:
    RFC 2631: Diffie-Hellman Key Agreement Method
    
    Revision History:
    Who         When            What
    --------    ----------      ------------------------------------------
    Eddy        2009/01/21      Create Diffie-Hellman, Montgomery Algorithm
***************************************************************************/

#ifndef __CRYPT_DH_H__
#define __CRYPT_DH_H__

#include "rt_config.h"


/* DH operations */
void DH_PublicKey_Generate (
    IN unsigned char GValue[],
    IN unsigned int GValueLength,
    IN unsigned char PValue[],
    IN unsigned int PValueLength,
    IN unsigned char PrivateKey[],
    IN unsigned int PrivateKeyLength,
    OUT unsigned char PublicKey[],
    INOUT unsigned int *PublicKeyLength);

void DH_SecretKey_Generate (
    IN unsigned char PublicKey[],
    IN unsigned int PublicKeyLength,
    IN unsigned char PValue[],
    IN unsigned int PValueLength,
    IN unsigned char PrivateKey[],
    IN unsigned int PrivateKeyLength,
    OUT unsigned char SecretKey[],
    INOUT unsigned int *SecretKeyLength);

#define RT_DH_PublicKey_Generate(GK, GKL, PV, PVL, PriK, PriKL, PubK, PubKL) \
    DH_PublicKey_Generate((GK), (GKL), (PV), (PVL), (PriK), (PriKL), (unsigned char *) (PubK), (unsigned int *) (PubKL))

#define RT_DH_SecretKey_Generate(PubK, PubKL, PV, PVL, PriK, PriKL, SecK, SecKL) \
    DH_SecretKey_Generate((PubK), (PubKL), (PV), (PVL), (PriK), (PriKL), (unsigned char *) (SecK), (unsigned int *) (SecKL))

#define RT_DH_FREE_ALL()

    
#endif /* __CRYPT_DH_H__ */

