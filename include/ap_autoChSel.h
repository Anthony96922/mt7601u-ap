/****************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ****************************************************************************

    Abstract:


 */

#include "ap_autoChSel_cmm.h"

#ifndef __AUTOCHSELECT_H__
#define __AUTOCHSELECT_H__

#define AP_AUTO_CH_SEL(__P, __O)	APAutoSelectChannel((__P), (__O))

unsigned long AutoChBssSearchWithSSID(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * Bssid,
	IN unsigned char * pSsid,
	IN unsigned char SsidLen,
	IN unsigned char Channel);

VOID APAutoChannelInit(
	IN PRTMP_ADAPTER pAd);

VOID UpdateChannelInfo(
	IN PRTMP_ADAPTER pAd,
	IN int ch,
	IN ChannelSel_Alg Alg);

unsigned long AutoChBssInsertEntry(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pBssid,
	IN CHAR Ssid[],
	IN unsigned char SsidLen, 
	IN unsigned char ChannelNo,
	IN unsigned char ExtChOffset,
	IN CHAR Rssi);

VOID AutoChBssTableInit(
	IN PRTMP_ADAPTER pAd);

VOID ChannelInfoInit(
	IN PRTMP_ADAPTER pAd);

VOID AutoChBssTableDestroy(
	IN PRTMP_ADAPTER pAd);

VOID ChannelInfoDestroy(
	IN PRTMP_ADAPTER pAd);

VOID CheckPhyModeIsABand(
	IN PRTMP_ADAPTER pAd);

unsigned char SelectBestChannel(
	IN PRTMP_ADAPTER pAd,
	IN ChannelSel_Alg Alg);

unsigned char APAutoSelectChannel(
	IN PRTMP_ADAPTER pAd,
	IN ChannelSel_Alg Alg);

#endif /* __AUTOCHSELECT_H__ */

