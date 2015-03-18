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
    ap_wds.h

    Abstract:
    Support WDS function.

    Revision History:
    Who       When            What
    ------    ----------      ----------------------------------------------
    Fonchi    02-13-2007      created
*/


#ifndef _AP_WDS_H_
#define _AP_WDS_H_

#define WDS_ENTRY_RETRY_INTERVAL	(100 * OS_HZ / 1000)

#ifdef WDS_VLAN_SUPPORT /* support WDS VLAN */
#define WDS_VLAN_INFO_GET(												\
	__pAd, __VLAN_VID, __VLAN_Priority, __FromWhichBSSID) 				\
{																		\
	if ((__FromWhichBSSID >= MIN_NET_DEVICE_FOR_WDS) &&					\
		(__FromWhichBSSID < (MIN_NET_DEVICE_FOR_WDS+MAX_WDS_ENTRY)) &&	\
		(__pAd->WdsTab.WdsEntry[										\
			__FromWhichBSSID - MIN_NET_DEVICE_FOR_WDS].VLAN_VID != 0))	\
	{																	\
		__VLAN_VID = __pAd->WdsTab.WdsEntry[							\
				__FromWhichBSSID - MIN_NET_DEVICE_FOR_WDS].VLAN_VID;	\
		__VLAN_Priority = __pAd->WdsTab.WdsEntry[						\
				__FromWhichBSSID - MIN_NET_DEVICE_FOR_WDS].VLAN_Priority;\
	}																	\
}
#else
#define WDS_VLAN_INFO_GET(												\
	__pAd, __VLAN_VID, __VLAN_Priority, __FromWhichBSSID)
#endif /* WDS_VLAN_SUPPORT */

static inline BOOLEAN WDS_IF_UP_CHECK(
	IN  PRTMP_ADAPTER   pAd, 
	IN  ULONG ifidx)
{
	if ((pAd->flg_wds_init != TRUE) ||
		(ifidx >= MAX_WDS_ENTRY))
		return FALSE;

/*	if(RTMP_OS_NETDEV_STATE_RUNNING(pAd->WdsTab.WdsEntry[ifidx].dev)) */
/* Patch for wds ,when dirver call apmlmeperiod => APMlmeDynamicTxRateSwitching check if wds device ready */
if ((pAd->WdsTab.WdsEntry[ifidx].dev != NULL) && (RTMP_OS_NETDEV_STATE_RUNNING(pAd->WdsTab.WdsEntry[ifidx].dev)))
		return TRUE;

	return FALSE;
}

LONG WdsEntryAlloc(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR pAddr);

VOID WdsEntryDel(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR pAddr);

MAC_TABLE_ENTRY *MacTableInsertWDSEntry(
	IN  PRTMP_ADAPTER   pAd, 
	IN  PUCHAR pAddr,
	UINT WdsTabIdx);

BOOLEAN MacTableDeleteWDSEntry(
	IN PRTMP_ADAPTER pAd,
	IN USHORT wcid,
	IN PUCHAR pAddr);


BOOLEAN ApWdsAllowToSendPacket(
	IN RTMP_ADAPTER *pAd,
	IN PNDIS_PACKET pPacket,
	OUT	UCHAR		*pWcid);
	
MAC_TABLE_ENTRY *WdsTableLookupByWcid(
    IN  PRTMP_ADAPTER   pAd, 
	IN UCHAR wcid,
	IN PUCHAR pAddr,
	IN BOOLEAN bResetIdelCount);

MAC_TABLE_ENTRY *WdsTableLookup(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PUCHAR          pAddr,
	IN BOOLEAN bResetIdelCount);

MAC_TABLE_ENTRY *FindWdsEntry(
	IN PRTMP_ADAPTER	pAd,
	IN UCHAR 			Wcid,
	IN PUCHAR			pAddr,
	IN UINT32			PhyMode);

VOID WdsTableMaintenance(
    IN PRTMP_ADAPTER    pAd);

VOID RT28xx_WDS_Close(
	IN PRTMP_ADAPTER pAd);


VOID WdsDown(
	IN PRTMP_ADAPTER pAd);

VOID AsicUpdateWdsRxWCIDTable(
	IN PRTMP_ADAPTER pAd);

VOID AsicUpdateWdsEncryption(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR wcid);

VOID WdsPeerBeaconProc(
	IN PRTMP_ADAPTER pAd,
	IN PMAC_TABLE_ENTRY pEntry,
	IN USHORT CapabilityInfo,
	IN UCHAR MaxSupportedRateIn500Kbps,
	IN UCHAR MaxSupportedRateLen,
	IN BOOLEAN bWmmCapable,
	IN ULONG ClientRalinkIe,
	IN HT_CAPABILITY_IE *pHtCapability,
	IN UCHAR HtCapabilityLen);

VOID APWdsInitialize(
	IN PRTMP_ADAPTER pAd);

INT	Show_WdsTable_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

VOID rtmp_read_wds_from_file(
			IN  PRTMP_ADAPTER pAd,
			PSTRING tmpbuf,
			PSTRING buffer);

VOID WdsPrepareWepKeyFromMainBss(
	IN  PRTMP_ADAPTER pAd);


VOID WDS_Init(
	IN	PRTMP_ADAPTER				pAd,
	IN	RTMP_OS_NETDEV_OP_HOOK		*pNetDevOps);

VOID WDS_Remove(
	IN	PRTMP_ADAPTER				pAd);

BOOLEAN WDS_StatsGet(
	IN	PRTMP_ADAPTER				pAd,
	IN	RT_CMD_STATS				*pStats);

VOID AP_WDS_KeyNameMakeUp(
	IN	STRING						*pKey,
	IN	UINT32						KeyMaxSize,
	IN	INT							KeyId);

/*
	==========================================================================
	Description:
		Check the WDS Entry is valid or not.
	==========================================================================
 */
static inline BOOLEAN ValidWdsEntry(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR WdsIndex)
{
	BOOLEAN result;
	PMAC_TABLE_ENTRY pMacEntry;

	do
	{
		if (WdsIndex >= MAX_WDS_ENTRY)
		{
			result = FALSE;
			break;
		}

		if (pAd->WdsTab.WdsEntry[WdsIndex].Valid != TRUE)
		{
			result = FALSE;
			break;
		}

		if ((pAd->WdsTab.WdsEntry[WdsIndex].MacTabMatchWCID==0)
			|| (pAd->WdsTab.WdsEntry[WdsIndex].MacTabMatchWCID >= MAX_LEN_OF_MAC_TABLE))
		{
			result = FALSE;
			break;
		}

		pMacEntry = &pAd->MacTab.Content[pAd->WdsTab.WdsEntry[WdsIndex].MacTabMatchWCID];
		if (!IS_ENTRY_WDS(pMacEntry))
		{
			result = FALSE;
			break;
		}
			
		result = TRUE;
	} while(FALSE);

	return result;
}
#endif /* _AP_WDS_H_ */

