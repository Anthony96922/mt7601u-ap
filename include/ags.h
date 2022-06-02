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

	Abstract:

	All AGS (Adaptive Group Switching) Related Structure & Definition

***************************************************************************/ 
    
#ifndef __AGS_H__
#define __AGS_H__


extern unsigned char AGS1x1HTRateTable[];
extern unsigned char AGS2x2HTRateTable[];
extern unsigned char AGS3x3HTRateTable[];
#ifdef DOT11_VHT_AC
extern unsigned char Ags1x1VhtRateTable[];
extern unsigned char Ags2x2VhtRateTable[];
#endif /* DOT11_VHT_AC */

#define AGS_TX_QUALITY_WORST_BOUND       8
#define AGS_QUICK_RA_TIME_INTERVAL        50	/* 50ms */

/* The size, in bytes, of an AGS entry in the rate switch table */
#define SIZE_OF_AGS_RATE_TABLE_ENTRY	9

typedef struct _RTMP_RA_AGS_TB {
	unsigned char	ItemNo;

	unsigned char	STBC:1;
	unsigned char	ShortGI:1;
	unsigned char	BW:2;
	unsigned char	Mode:3;
	unsigned char	Rsv1:1;

	unsigned char Nss:2; // NSS_XXX (VHT only)
	unsigned char rsv2:6; // Reserved
	
	unsigned char	CurrMCS;
	unsigned char	TrainUp;
	unsigned char	TrainDown;
	unsigned char	downMcs;
	unsigned char	upMcs3;
	unsigned char	upMcs2;
	unsigned char	upMcs1;
}RTMP_RA_AGS_TB;


/* AGS control */
typedef struct _AGS_CONTROL {
	unsigned char MCSGroup; /* The MCS group under testing */
	unsigned char lastRateIdx;
} AGS_CONTROL,*PAGS_CONTROL;


/* The statistics information for AGS */
typedef struct _AGS_STATISTICS_INFO {
	CHAR	RSSI;
	unsigned long	TxErrorRatio;
	unsigned long	AccuTxTotalCnt;
	unsigned long	TxTotalCnt;
	unsigned long	TxSuccess;
	unsigned long	TxRetransmit;
	unsigned long	TxFailCount;
} AGS_STATISTICS_INFO, *PAGS_STATISTICS_INFO;


/* Support AGS (Adaptive Group Switching) */
#define SUPPORT_AGS(__pAd)		((__pAd)->rateAlg == RATE_ALG_AGS)

#ifdef DOT11_VHT_AC
#define AGS_IS_USING(__pAd, __pRateTable)	\
    (SUPPORT_AGS(__pAd) && \
     ((__pRateTable == AGS1x1HTRateTable) ||\
      (__pRateTable == AGS2x2HTRateTable) ||\
      (__pRateTable == AGS3x3HTRateTable) ||\
      (__pRateTable == Ags1x1VhtRateTable) ||\
      (__pRateTable == Ags2x2VhtRateTable))) 
#else
#define AGS_IS_USING(__pAd, __pRateTable)	\
    (SUPPORT_AGS(__pAd) && \
     ((__pRateTable == AGS1x1HTRateTable) || \
      (__pRateTable == AGS2x2HTRateTable) || \
      (__pRateTable == AGS3x3HTRateTable))) 
#endif /* DOT11_VHT_AC */

#endif /* __AGS_H__ */

