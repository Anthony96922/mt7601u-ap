/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    rtmp_type.h

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Name        Date            Modification logs
    Paul Lin    1-2-2004
*/

#ifndef __RTMP_TYPE_H__
#define __RTMP_TYPE_H__



#ifndef GNU_PACKED
#define GNU_PACKED  __attribute__ ((packed))
#endif /* GNU_PACKED */


#ifdef LINUX
/* Put platform dependent declaration here */
/* For example, linux type definition */
#endif /* LINUX */

/* modified for fixing compile warning on Sigma 8634 platform */

typedef signed char CHAR;

typedef signed short SHORT;
typedef signed int INT;
typedef signed long LONG;
typedef signed long long LONGLONG;

#ifdef LINUX
typedef void VOID;
#endif /* LINUX */

typedef unsigned int NDIS_MEDIA_STATE;

typedef union _LARGE_INTEGER {
	struct {
#ifdef RT_BIG_ENDIAN
		int HighPart;
		unsigned int LowPart;
#else
		unsigned int LowPart;
		int HighPart;
#endif
	} u;
	long long QuadPart;
} LARGE_INTEGER;


/* Register set pair for initialzation register set definition */
typedef struct _RTMP_REG_PAIR {
	unsigned int Register;
	unsigned int Value;
} RTMP_REG_PAIR, *PRTMP_REG_PAIR;

typedef struct _REG_PAIR {
	unsigned char Register;
	unsigned char Value;
} REG_PAIR, *PREG_PAIR;

typedef struct _REG_PAIR_CHANNEL {
	unsigned char Register;
	unsigned char FirstChannel;
	unsigned char LastChannel;
	unsigned char Value;
} REG_PAIR_CHANNEL, *PREG_PAIR_CHANNEL;

typedef struct _REG_PAIR_BW {
	unsigned char Register;
	unsigned char BW;
	unsigned char Value;
} REG_PAIR_BW, *PREG_PAIR_BW;


typedef struct _REG_PAIR_PHY{
	unsigned char reg;
	unsigned char s_ch;
	unsigned char e_ch;
	unsigned char phy;	/* RF_MODE_XXX */
	unsigned char bw;	/* RF_BW_XX */
	unsigned char val;
}REG_PAIR_PHY;


/* Register set pair for initialzation register set definition */
typedef struct _RTMP_RF_REGS {
	unsigned char Channel;
	unsigned int R1;
	unsigned int R2;
	unsigned int R3;
	unsigned int R4;
} RTMP_RF_REGS, *PRTMP_RF_REGS;

typedef struct _FREQUENCY_ITEM {
	unsigned char Channel;
	unsigned char N;
	unsigned char R;
	unsigned char K;
} FREQUENCY_ITEM, *PFREQUENCY_ITEM;

typedef int NTSTATUS;

#define STATUS_SUCCESS			0x00
#define STATUS_UNSUCCESSFUL 		0x01

typedef struct _QUEUE_ENTRY {
	struct _QUEUE_ENTRY *Next;
} QUEUE_ENTRY, *PQUEUE_ENTRY;

/* Queue structure */
typedef struct _QUEUE_HEADER {
	PQUEUE_ENTRY Head;
	PQUEUE_ENTRY Tail;
	unsigned long Number;
} QUEUE_HEADER, *PQUEUE_HEADER;

typedef struct _BANK_RF_REG_PAIR {
	unsigned char Bank;
	unsigned char Register;
	unsigned char Value;
} BANK_RF_REG_PAIR, *PBANK_RF_REG_PAIR;

typedef struct _R_M_W_REG{
	unsigned int Register;
	unsigned int ClearBitMask;
	unsigned int Value;
} R_M_W_REG, *PR_M_W_REG;

typedef struct _RF_R_M_W_REG{
	unsigned char Bank;
	unsigned char Register;
	unsigned char ClearBitMask;
	unsigned char Value;
} RF_R_M_W_REG, *PRF_R_M_W_REG;

#endif /* __RTMP_TYPE_H__ */
