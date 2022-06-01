/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    radar.h

    Abstract:
     CS/DFS common functions.

    Revision History:
    Who       When            What
    --------  ----------      ----------------------------------------------
*/
#ifndef __RADAR_H__
#define __RADAR_H__

/* RESTRICTION_BAND_1: 5600MHz ~ 5650MHz */
#define RESTRICTION_BAND_1(_pAd)												\
	_pAd->CommonCfg.RegTransmitSetting.field.BW == BW_40 ? 						\
	((_pAd->CommonCfg.Channel >= 116) && (_pAd->CommonCfg.Channel <= 128)) :	\
	((_pAd->CommonCfg.Channel >= 120) && (_pAd->CommonCfg.Channel <= 128))

/* 802.11H */
typedef struct _DOT11_H {
	/* 802.11H and DFS related params */
	unsigned char CSCount;		/*Channel switch counter */
	unsigned char CSPeriod; 	/*Channel switch period (beacon count) */
	unsigned short RDCount; 	/*Radar detection counter, if RDCount >  ChMovingTime, start to send beacons*/
	unsigned char RDMode;		/*Radar Detection mode */
	unsigned short ChMovingTime;
	bool bDFSIndoor;
	unsigned long InServiceMonitorCount;	/* unit: sec */
} DOT11_H, *PDOT11_H;

bool RadarChannelCheck(
	IN PRTMP_ADAPTER	pAd,
	IN unsigned char			Ch);

unsigned long JapRadarType(
	IN PRTMP_ADAPTER pAd);

#ifdef CONFIG_AP_SUPPORT
VOID ChannelSwitchingCountDownProc(
	IN PRTMP_ADAPTER	pAd);
#endif /* CONFIG_AP_SUPPORT */

VOID RadarDetectPeriodic(
	IN PRTMP_ADAPTER	pAd);

INT	Set_CSPeriod_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	char *			arg);

INT Set_ChMovingTime_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN char * arg);

INT Set_BlockChReset_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN char * arg);

#if defined(DFS_SUPPORT) || defined(CARRIER_DETECTION_SUPPORT)
INT	Set_RadarShow_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

VOID CckMrcStatusCtrl(
	IN PRTMP_ADAPTER pAd);

VOID RadarGLRTCompensate(
	IN PRTMP_ADAPTER pAd);

#endif /*defined(DFS_SUPPORT) || defined(CARRIER_DETECTION_SUPPORT)*/

#endif /* __RADAR_H__ */
