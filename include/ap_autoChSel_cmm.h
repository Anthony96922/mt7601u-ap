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


#ifndef __AUTOCHSELECT_CMM_H__
#define __AUTOCHSELECT_CMM_H__

#define RSSI_TO_DBM_OFFSET 120 /* RSSI-115 = dBm */


typedef struct {
	unsigned long dirtyness[MAX_NUM_OF_CHANNELS+1];
	unsigned long ApCnt[MAX_NUM_OF_CHANNELS+1];
	unsigned int FalseCCA[MAX_NUM_OF_CHANNELS+1];
    bool SkipList[MAX_NUM_OF_CHANNELS+1];
#ifdef AP_QLOAD_SUPPORT
	unsigned int chanbusytime[MAX_NUM_OF_CHANNELS+1]; /* QLOAD ALARM */
#endif /* AP_QLOAD_SUPPORT */
        bool IsABand;
} CHANNELINFO, *PCHANNELINFO;

typedef struct {
	unsigned char Bssid[MAC_ADDR_LEN];
	unsigned char SsidLen;
	CHAR Ssid[MAX_LEN_OF_SSID];
	unsigned char Channel;
	unsigned char ExtChOffset;
	CHAR Rssi;
} BSSENTRY, *PBSSENTRY;

typedef struct {
	unsigned char BssNr;
	BSSENTRY BssEntry[MAX_LEN_OF_BSS_TABLE];	
} BSSINFO, *PBSSINFO;

typedef enum ChannelSelAlg
{
	ChannelAlgRandom, /*use by Dfs */
	ChannelAlgApCnt,
	ChannelAlgCCA
} ChannelSel_Alg;

#endif /* __AUTOCHSELECT_CMM_H__ */

