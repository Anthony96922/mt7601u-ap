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
	chlist.c

	Abstract:

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/

#ifndef __CHLIST_H__
#define __CHLIST_H__

#include "rtmp_type.h"
#include "rtmp_def.h"


typedef struct _CH_DESC {
	unsigned char FirstChannel;
	unsigned char NumOfCh;
	unsigned char ChannelProp;
}CH_DESC, *PCH_DESC;

typedef struct _COUNTRY_REGION_CH_DESC {
	unsigned char RegionIndex;
	PCH_DESC pChDesc;
}COUNTRY_REGION_CH_DESC, *PCOUNTRY_REGION_CH_DESC;

#ifdef EXT_BUILD_CHANNEL_LIST
#define ODOR			0
#define IDOR			1
#define BOTH			2

typedef struct _CH_DESP {
	unsigned char FirstChannel;
	unsigned char NumOfCh;
	CHAR MaxTxPwr;			/* dBm */
	unsigned char Geography;			/* 0:out door, 1:in door, 2:both */
	bool DfsReq;			/* Dfs require, 0: No, 1: yes. */
} CH_DESP, *PCH_DESP;

typedef struct _CH_REGION {
	unsigned char CountReg[3];
	unsigned char DfsType;			/* 0: CE, 1: FCC, 2: JAP, 3:JAP_W53, JAP_W56 */
	CH_DESP *pChDesp;
} CH_REGION, *PCH_REGION;

extern CH_REGION ChRegion[];
#endif /* EXT_BUILD_CHANNEL_LIST */

#ifdef SINGLE_SKU_V2
#define	SINGLE_SKU_TABLE_LENGTH		(SINGLE_SKU_TABLE_CCK_LENGTH+SINGLE_SKU_TABLE_OFDM_LENGTH+(SINGLE_SKU_TABLE_HT_LENGTH*2))

#define SINGLE_SKU_TABLE_CCK_LENGTH	4
#define SINGLE_SKU_TABLE_OFDM_LENGTH	8
#define SINGLE_SKU_TABLE_HT_LENGTH	16

typedef struct _CH_POWER_{
	struct _CH_POWER_ *pNext;
	unsigned char		StartChannel;
	unsigned char		num;
	unsigned char		*Channel;
	unsigned char		PwrCCK[SINGLE_SKU_TABLE_CCK_LENGTH];
	unsigned char		PwrOFDM[SINGLE_SKU_TABLE_OFDM_LENGTH];
	unsigned char		PwrHT20[SINGLE_SKU_TABLE_HT_LENGTH];
	unsigned char		PwrHT40[SINGLE_SKU_TABLE_HT_LENGTH];
}CH_POWER;
#endif /* SINGLE_SKU_V2 */

typedef struct _CH_FREQ_MAP_{
	unsigned short		channel;
	unsigned short		freqKHz;
}CH_FREQ_MAP;

extern CH_FREQ_MAP CH_HZ_ID_MAP[];
extern int CH_HZ_ID_MAP_NUM;


#define     MAP_CHANNEL_ID_TO_KHZ(_ch, _khz)                 \
			RTMP_MapChannelID2KHZ(_ch, (unsigned int *)&(_khz))
#define     MAP_KHZ_TO_CHANNEL_ID(_khz, _ch)                 \
			RTMP_MapKHZ2ChannelID(_khz, (INT *)&(_ch))

/* Check if it is Japan W53(ch52,56,60,64) channel. */
#define JapanChannelCheck(_ch)  ((_ch == 52) || (_ch == 56) || (_ch == 60) || (_ch == 64))


#ifdef EXT_BUILD_CHANNEL_LIST
VOID BuildChannelListEx(
	IN PRTMP_ADAPTER pAd);

VOID BuildBeaconChList(
	IN PRTMP_ADAPTER pAd,
	OUT unsigned char * pBuf,
	OUT	unsigned long * pBufLen);
#endif /* EXT_BUILD_CHANNEL_LIST */

#ifdef DOT11_N_SUPPORT
VOID N_ChannelCheck(RTMP_ADAPTER *pAd);
unsigned char N_SetCenCh(RTMP_ADAPTER *pAd, unsigned char channel);
bool N_ChannelGroupCheck(RTMP_ADAPTER *pAd, unsigned char channel);

#endif /* DOT11_N_SUPPORT */

unsigned char GetCountryMaxTxPwr(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char channel);

VOID RTMP_MapChannelID2KHZ(
	IN unsigned char Ch,
	OUT unsigned int *pFreq);

VOID RTMP_MapKHZ2ChannelID(
	IN unsigned long Freq,
	OUT INT *pCh);

unsigned char GetChannel_5GHZ(
	IN PCH_DESC pChDesc, 
	IN unsigned char index);

unsigned char GetChannel_2GHZ(
	IN PCH_DESC pChDesc, 
	IN unsigned char index);

unsigned char GetChannelFlag(
	IN PCH_DESC pChDesc, 
	IN unsigned char index);

unsigned short TotalChNum(
	IN PCH_DESC pChDesc);
	
#endif /* __CHLIST_H__ */

