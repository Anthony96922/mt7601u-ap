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

    Module Name:
    connect.c

    Abstract:
    Routines to deal Link UP/DOWN and build/update BEACON frame contents

    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    John Chang  08-04-2003    created for 11g soft-AP
 */

#include "rt_config.h"

unsigned char PowerConstraintIE[3] = {IE_POWER_CONSTRAINT, 1, 3};

/*
	==========================================================================
	Description:
		Used to check the necessary to send Beancon.
	return value
		0: mean no necessary.
		0: mean need to send Beacon for the service.
	==========================================================================
*/
bool BeaconTransmitRequired(
	IN RTMP_ADAPTER *pAd,
	IN INT apidx,
	IN MULTISSID_STRUCT *pMbss)
{
#ifdef WDS_SUPPORT
	unsigned char idx;
#endif /* WDS_SUPPORT */
	bool result = FALSE;

	do {
#ifdef WDS_SUPPORT
		if (pAd->WdsTab.Mode == WDS_BRIDGE_MODE)
			break;
#endif /* WDS_SUPPORT */

#ifdef CARRIER_DETECTION_SUPPORT
		if (isCarrierDetectExist(pAd) == TRUE)
			break;
#endif /* CARRIER_DETECTION_SUPPORT */


		if (apidx == MAIN_MBSSID) {
			if (pMbss->bBcnSntReq == TRUE) {
				result = TRUE;
				break;
			}
#ifdef WDS_SUPPORT
			for (idx = 0; idx < MAX_WDS_ENTRY; idx++) {
				if ((pAd->WdsTab.WdsEntry[idx].dev != NULL)
					&& (RTMP_OS_NETDEV_STATE_RUNNING(pAd->WdsTab.WdsEntry[idx].dev))) {
					result = TRUE;
					break;
				}
			}
#endif /* WDS_SUPPORT */
		} else {
			if (pMbss->bBcnSntReq == TRUE)
				result = TRUE;
		}
	}
	while (FALSE);

	return result;
}


/*
	==========================================================================
	Description:
		Pre-build a BEACON frame in the shared memory
	==========================================================================
*/
VOID APMakeBssBeacon(RTMP_ADAPTER *pAd, INT apidx)
{
	unsigned char DsLen = 1, SsidLen;
	HEADER_802_11 BcnHdr;
	LARGE_INTEGER FakeTimestamp;
	unsigned long FrameLen = 0, TmpLen = 0, TmpLen2 = 0;
	unsigned char * pBeaconFrame = (unsigned char *)pAd->ApCfg.MBSSID[apidx].BeaconBuf;
	unsigned char *ptr;
	unsigned int i;
	unsigned int longValue, reg_base;
	HTTRANSMIT_SETTING BeaconTransmit = {.word = 0}; /* MGMT frame PHY rate setting when operatin at Ht rate. */
	unsigned char PhyMode, SupRateLen;
	unsigned char TXWISize = pAd->chipCap.TXWISize;

	if(!BeaconTransmitRequired(pAd, apidx, &pAd->ApCfg.MBSSID[apidx]))
		return;

	PhyMode = pAd->ApCfg.MBSSID[apidx].PhyMode;

	if (pAd->ApCfg.MBSSID[apidx].bHideSsid)
		SsidLen = 0;
	else
		SsidLen = pAd->ApCfg.MBSSID[apidx].SsidLen;


	MgtMacHeaderInit(pAd, &BcnHdr, SUBTYPE_BEACON, 0, BROADCAST_ADDR,
				pAd->ApCfg.MBSSID[apidx].Bssid);

	/* for update framelen to TxWI later. */
	SupRateLen = pAd->CommonCfg.SupRateLen;
	if (PhyMode == WMODE_B)
		SupRateLen = 4;

	MakeOutgoingFrame(pBeaconFrame, &FrameLen,
				sizeof(HEADER_802_11), &BcnHdr,
				TIMESTAMP_LEN, &FakeTimestamp,
				2, &pAd->CommonCfg.BeaconPeriod,
				2, &pAd->ApCfg.MBSSID[apidx].CapabilityInfo,
				1, &SsidIe,
				1, &SsidLen,
				SsidLen, pAd->ApCfg.MBSSID[apidx].Ssid,
				1, &SupRateIe,
				1, &SupRateLen,
				SupRateLen, pAd->CommonCfg.SupRate,
				1, &DsIe,
				1, &DsLen,
				1, &pAd->CommonCfg.Channel,
				END_OF_ARGS);

	/* add country IE, power constraint IE */
	if (pAd->CommonCfg.bCountryFlag) {
		unsigned char *TmpFrame = NULL;
		unsigned char CountryIe = IE_COUNTRY;

		os_alloc_mem(NULL, (unsigned char **)&TmpFrame, 256);
		if (TmpFrame != NULL) {
			NdisZeroMemory(TmpFrame, 256);
			/* prepare channel information */
#ifdef EXT_BUILD_CHANNEL_LIST
			BuildBeaconChList(pAd, TmpFrame, &TmpLen2);
#else
			{
				unsigned char MaxTxPower = GetCountryMaxTxPwr(pAd, pAd->CommonCfg.Channel);
				MakeOutgoingFrame(TmpFrame + TmpLen2, &TmpLen,
							1, &pAd->ChannelList[0].Channel,
							1, &pAd->ChannelListNum,
							1, &MaxTxPower,
							END_OF_ARGS);
				TmpLen2 += TmpLen;
			}
#endif /* EXT_BUILD_CHANNEL_LIST */

			/* need to do the padding bit check, and concatenate it */
			if ((TmpLen2 % 2) == 0) {
				unsigned char TmpLen3 = TmpLen2 + 4;
				MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
				                  1, &CountryIe,
				                  1, &TmpLen3,
				                  3, pAd->CommonCfg.CountryCode,
				                  TmpLen2 + 1, TmpFrame,
			                  END_OF_ARGS);
			} else {
				unsigned char TmpLen3 = TmpLen2 + 3;
				MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
							1, &CountryIe,
							1, &TmpLen3,
							3, pAd->CommonCfg.CountryCode,
							TmpLen2, TmpFrame,
							END_OF_ARGS);
			}
			FrameLen += TmpLen;
			os_free_mem(NULL, TmpFrame);
		} else
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Allocate memory fail!!!\n", __FUNCTION__));
	}

	if (pAd->CommonCfg.ExtRateLen) {
		MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
					1, &ExtRateIe,
					1, &pAd->CommonCfg.ExtRateLen,
					pAd->CommonCfg.ExtRateLen, pAd->CommonCfg.ExtRate,
					END_OF_ARGS);
		FrameLen += TmpLen;
	}

	RTMPWriteTxWI(pAd, &pAd->BeaconTxWI, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE, 0, BSS0Mcast_WCID,
		FrameLen, PID_MGMT, 0, 0,IFS_HTTXOP, FALSE, &BeaconTransmit);

	/*
		step 6. move BEACON TXD and frame content to on-chip memory
	*/
	ptr = (unsigned char *)&pAd->BeaconTxWI;
#ifdef RT_BIG_ENDIAN
	RTMPWIEndianChange(pAd, ptr, TYPE_TXWI);
#endif

	reg_base = pAd->BeaconOffset[pAd->ApCfg.MBSSID[apidx].BcnBufIdx];
	for (i = 0; i < TXWISize; i += 4) {
		longValue = *ptr + (*(ptr + 1) << 8) + (*(ptr + 2) << 16) + (*(ptr + 3) << 24);
		RTMP_CHIP_UPDATE_BEACON(pAd, reg_base + i, longValue, 4);
		ptr += 4;
	}

	/* update BEACON frame content. start right after the TXWI field. */
	ptr = (unsigned char *)pAd->ApCfg.MBSSID[apidx].BeaconBuf;
#ifdef RT_BIG_ENDIAN
	RTMPFrameEndianChange(pAd, ptr, DIR_WRITE, FALSE);
#endif

	reg_base = pAd->BeaconOffset[pAd->ApCfg.MBSSID[apidx].BcnBufIdx] + TXWISize;
	for (i = 0; i < FrameLen; i += 4) {
		longValue =  *ptr + (*(ptr + 1) << 8) + (*(ptr + 2) << 16) + (*(ptr + 3) << 24);
		RTMP_CHIP_UPDATE_BEACON(pAd, reg_base + i, longValue, 4);
		ptr += 4;
	}

	pAd->ApCfg.MBSSID[apidx].TimIELocationInBeacon = (unsigned char)FrameLen;
	pAd->ApCfg.MBSSID[apidx].CapabilityInfoLocationInBeacon = sizeof(HEADER_802_11) + TIMESTAMP_LEN + 2;
}


/*
	==========================================================================
	Description:
		Update the BEACON frame in the shared memory. Because TIM IE is variable
		length. other IEs after TIM has to shift and total frame length may change
		for each BEACON period.
	Output:
		pAd->ApCfg.MBSSID[apidx].CapabilityInfo
		pAd->ApCfg.ErpIeContent
	==========================================================================
*/
VOID APUpdateBeaconFrame(RTMP_ADAPTER *pAd, INT apidx)
{
	unsigned char *pBeaconFrame = (unsigned char *)pAd->ApCfg.MBSSID[apidx].BeaconBuf;
	unsigned char *ptr;
	unsigned long FrameLen = pAd->ApCfg.MBSSID[apidx].TimIELocationInBeacon;
	unsigned long UpdatePos = pAd->ApCfg.MBSSID[apidx].TimIELocationInBeacon;
	unsigned char RSNIe = IE_WPA, RSNIe2 = IE_WPA2;
	unsigned char ID_1B, TimFirst, TimLast, *pTim;
	MULTISSID_STRUCT *pMbss;
	COMMON_CONFIG *pComCfg;
	unsigned char PhyMode;
#ifdef WSC_AP_SUPPORT
	bool bHasWpsIE = FALSE;
#endif /* ifdef WSC_AP_SUPPORT */
	unsigned int i;
	HTTRANSMIT_SETTING BeaconTransmit = {.word = 0}; /* MGMT frame PHY rate setting when operatin at Ht rate. */
	unsigned long TmpLen = 0;

	pMbss = &pAd->ApCfg.MBSSID[apidx];
	pComCfg = &pAd->CommonCfg;
	PhyMode = pMbss->PhyMode;

	if(!BeaconTransmitRequired(pAd, apidx, pMbss))
		return;

	/*
		step 1 - update BEACON's Capability
	*/
	ptr = pBeaconFrame + pMbss->CapabilityInfoLocationInBeacon;
	*ptr = (unsigned char)(pMbss->CapabilityInfo & 0x00ff);
	*(ptr + 1) = (unsigned char)((pMbss->CapabilityInfo & 0xff00) >> 8);

	/*
		step 2 - update TIM IE
		TODO: enlarge TIM bitmap to support up to 64 STAs
		TODO: re-measure if RT2600 TBTT interrupt happens faster than BEACON sent out time
	*/
	ptr = pBeaconFrame + pMbss->TimIELocationInBeacon;
	*ptr = IE_TIM;
	*(ptr + 2) = pAd->ApCfg.DtimCount;
	*(ptr + 3) = pAd->ApCfg.DtimPeriod;

	/* find the smallest AID (PS mode) */
	TimFirst = 0; /* record first TIM byte != 0x00 */
	TimLast = 0;  /* record last TIM byte != 0x00 */
	pTim = pMbss->TimBitmaps;

	for(ID_1B = 0; ID_1B < WLAN_MAX_NUM_OF_TIM; ID_1B++) {
		/* get the TIM indicating PS packets for 8 stations */
		unsigned char tim_1B = pTim[ID_1B];

		if (ID_1B == 0)
			tim_1B &= 0xfe; /* skip bit0 bc/mc */

		if (tim_1B == 0)
			continue; /* find next 1B */

		if (TimFirst == 0)
			TimFirst = ID_1B;

		TimLast = ID_1B;
	}

	/* fill TIM content to beacon buffer */
	if (TimFirst & 0x01)
		TimFirst--; /* find the even offset byte */

	*(ptr + 1) = 3 + (TimLast-TimFirst + 1); /* TIM IE length */
	*(ptr + 4) = TimFirst;

	for(i = TimFirst; i <= TimLast; i++)
		*(ptr + 5 + i - TimFirst) = pTim[i];

	/* bit0 means backlogged mcast/bcast */
	if (pAd->ApCfg.DtimCount == 0)
		*(ptr + 4) |= (pMbss->TimBitmaps[WLAN_CT_TIM_BCMC_OFFSET] & 0x01);

	/* adjust BEACON length according to the new TIM */
	FrameLen += (2 + *(ptr + 1));

#ifdef A_BAND_SUPPORT
	/*
		Only 802.11a APs that comply with 802.11h are required to include a
		Power Constrint Element(IE=32) in beacons and probe response frames
	*/
	if ((pComCfg->Channel > 14) && pComCfg->bIEEE80211H == TRUE))
	{
		unsigned char PwrConstraintIE = IE_POWER_CONSTRAINT;
		unsigned char PwrConstraintLen = 1;
		unsigned char PwrConstraint = pComCfg->PwrConstraint;

		/* prepare power constraint IE */
		MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
					1, &PwrConstraintIE,
					1, &PwrConstraintLen,
					1, &PwrConstraint,
					END_OF_ARGS);
		FrameLen += TmpLen;
	}
#endif /* A_BAND_SUPPORT */

	/* Update ERP */
	if (PhyMode != WMODE_B) {
		/* fill ERP IE */
		ptr = (unsigned char *)pBeaconFrame + FrameLen; /* pTxD->DataByteCnt; */
		*ptr = IE_ERP;
		*(ptr + 1) = 1;
		*(ptr + 2) = pAd->ApCfg.ErpIeContent;
		FrameLen += 3;
	}

#ifdef A_BAND_SUPPORT
	/* fill up Channel Switch Announcement Element */
	if ((pComCfg->Channel > 14)
		&& (pComCfg->bIEEE80211H == 1)
		&& (pAd->Dot11_H.RDMode == RD_SWITCHING_MODE)) {
		ptr = pBeaconFrame + FrameLen;
		*ptr = IE_CHANNEL_SWITCH_ANNOUNCEMENT;
		*(ptr + 1) = 3;
		*(ptr + 2) = 1;
		*(ptr + 3) = pComCfg->Channel;
		*(ptr + 4) = (pAd->Dot11_H.CSPeriod - pAd->Dot11_H.CSCount - 1);
		ptr += 5;
		FrameLen += 5;

#ifdef DOT11_N_SUPPORT
		/* Extended Channel Switch Announcement Element */
		if (pComCfg->bExtChannelSwitchAnnouncement) {
			HT_EXT_CHANNEL_SWITCH_ANNOUNCEMENT_IE HtExtChannelSwitchIe;
			build_ext_channel_switch_ie(pAd, &HtExtChannelSwitchIe);
			NdisMoveMemory(ptr, &HtExtChannelSwitchIe, sizeof(HT_EXT_CHANNEL_SWITCH_ANNOUNCEMENT_IE));
			ptr += sizeof(HT_EXT_CHANNEL_SWITCH_ANNOUNCEMENT_IE);
			FrameLen += sizeof(HT_EXT_CHANNEL_SWITCH_ANNOUNCEMENT_IE);
		}
#endif /* DOT11_N_SUPPORT */
	}
#endif /* A_BAND_SUPPORT */

#ifdef DOT11_N_SUPPORT
	/* step 5. Update HT. Since some fields might change in the same BSS. */
	if (WMODE_CAP_N(PhyMode) && (pMbss->DesiredHtPhyInfo.bHtEnable)) {
		unsigned char HtLen, HtLen1;
		/*unsigned char i; */

		/* add HT Capability IE */
		HtLen = sizeof(pComCfg->HtCapability);
		HtLen1 = sizeof(pComCfg->AddHTInfo);
		MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
					1, &HtCapIe,
					1, &HtLen,
					HtLen, &pComCfg->HtCapability,
					1, &AddHtInfoIe,
					1, &HtLen1,
					HtLen1, &pComCfg->AddHTInfo,
					END_OF_ARGS);
		FrameLen += TmpLen;

#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
	 	/*
			P802.11n_D3.03, 7.3.2.60 Overlapping BSS Scan Parameters IE
		*/
	 	if ((pComCfg->Channel <= 14) &&
			(pComCfg->HtCapability.HtCapInfo.ChannelWidth == 1)) {
			OVERLAP_BSS_SCAN_IE OverlapScanParam;
			unsigned char OverlapScanIE, ScanIELen;

			OverlapScanIE = IE_OVERLAPBSS_SCAN_PARM;
			ScanIELen = 14;
			OverlapScanParam.ScanPassiveDwell = cpu2le16(pComCfg->Dot11OBssScanPassiveDwell);
			OverlapScanParam.ScanActiveDwell = cpu2le16(pComCfg->Dot11OBssScanActiveDwell);
			OverlapScanParam.TriggerScanInt = cpu2le16(pComCfg->Dot11BssWidthTriggerScanInt);
			OverlapScanParam.PassiveTalPerChannel = cpu2le16(pComCfg->Dot11OBssScanPassiveTotalPerChannel);
			OverlapScanParam.ActiveTalPerChannel = cpu2le16(pComCfg->Dot11OBssScanActiveTotalPerChannel);
			OverlapScanParam.DelayFactor = cpu2le16(pComCfg->Dot11BssWidthChanTranDelayFactor);
			OverlapScanParam.ScanActThre = cpu2le16(pComCfg->Dot11OBssScanActivityThre);

			MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
						1, &OverlapScanIE,
						1, &ScanIELen,
						ScanIELen, &OverlapScanParam,
						END_OF_ARGS);
			FrameLen += TmpLen;
	 	}
#endif /* DOT11N_DRAFT3 */
#endif /* DOT11_N_SUPPORT */

		/* 7.3.2.27 Extended Capabilities IE */
		{
			unsigned long infoPos;
			unsigned char * pInfo;
			unsigned char extInfoLen;
			bool	bNeedAppendExtIE = FALSE;
			EXT_CAP_INFO_ELEMENT extCapInfo;

			extInfoLen = sizeof(EXT_CAP_INFO_ELEMENT);
			NdisZeroMemory(&extCapInfo, extInfoLen);

#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
			/* P802.11n_D1.10, HT Information Exchange Support */
			if (WMODE_CAP_N(PhyMode) && (pComCfg->Channel <= 14) &&
			   (pMbss->DesiredHtPhyInfo.bHtEnable) &&
			   (pComCfg->bBssCoexEnable == TRUE))
				extCapInfo.BssCoexistMgmtSupport = 1;
#endif /* DOT11N_DRAFT3 */
#endif /* DOT11_N_SUPPORT */

			extCapInfo.utf8_ssid = 1;

			pInfo = (unsigned char *)(&extCapInfo);
			for (infoPos = 0; infoPos < extInfoLen; infoPos++) {
				if (pInfo[infoPos] != 0) {
					bNeedAppendExtIE = TRUE;
					break;
				}
			}

			if (bNeedAppendExtIE == TRUE) {
				MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
							1, &ExtCapIe,
							1, &extInfoLen,
							extInfoLen, &extCapInfo,
							END_OF_ARGS);
				FrameLen += TmpLen;
			}
		}

#ifdef DOT11_VHT_AC
		if (WMODE_CAP_AC(PhyMode) && (pComCfg->Channel > 14)) {
			int _len = build_vht_ies(pAd, (unsigned char *)(pBeaconFrame+FrameLen), SUBTYPE_BEACON);
			FrameLen += _len;
		}
#endif /* DOT11_VHT_AC */
	}
#endif /* DOT11_N_SUPPORT */
#ifdef HOSTAPD_SUPPORT
	if (pAd->ApCfg.MBSSID[apidx].HostapdWPS && (pAd->ApCfg.MBSSID[apidx].WscIEBeacon.ValueLen))
		bHasWpsIE = TRUE;
#endif

#ifdef WSC_AP_SUPPORT
	/* add Simple Config Information Element */
	if ((pAd->ApCfg.MBSSID[apidx].WscControl.WscConfMode >= 1) && (pAd->ApCfg.MBSSID[apidx].WscIEBeacon.ValueLen))
		bHasWpsIE = TRUE;

	if (bHasWpsIE) {
		unsigned long WscTmpLen = 0;

		MakeOutgoingFrame(pBeaconFrame + FrameLen, &WscTmpLen,
					pAd->ApCfg.MBSSID[apidx].WscIEBeacon.ValueLen, pAd->ApCfg.MBSSID[apidx].WscIEBeacon.Value,
					END_OF_ARGS);
		FrameLen += WscTmpLen;
	}

	if ((pAd->ApCfg.MBSSID[apidx].WscControl.WscConfMode != WSC_DISABLE) &&
#ifdef DOT1X_SUPPORT
	    (pAd->ApCfg.MBSSID[apidx].IEEE8021X == FALSE) &&
#endif /* DOT1X_SUPPORT */
	    (pAd->ApCfg.MBSSID[apidx].WepStatus == Ndis802_11WEPEnabled))
	{
		/*
		    Non-WPS Windows XP and Vista PCs are unable to determine if a WEP enalbed network is static key based
		    or 802.1X based. If the legacy station gets an EAP-Rquest/Identity from the AP, it assume the WEP
		    network is 802.1X enabled & will prompt the user for 802.1X credentials. If the legacy station doesn't
		    receive anything after sending an EAPOL-Start, it will assume the WEP network is static key based and
		    prompt user for the WEP key. <<from "WPS and Static Key WEP Networks">>
		    A WPS enabled AP should include this IE in the beacon when the AP is hosting a static WEP key network.
		    The IE would be 7 bytes long with the Extended Capability field set to 0 (all bits zero)
		    http:msdn.microsoft.com/library/default.asp?url=/library/en-us/randz/protocol/securing_public_wi-fi_hotspots.asp
		*/
		unsigned char PROVISION_SERVICE_IE[7] = {0xDD, 0x05, 0x00, 0x50, 0xF2, 0x05, 0x00};
		MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
					7, PROVISION_SERVICE_IE,
					END_OF_ARGS);
		FrameLen += TmpLen;
	}
#endif /* WSC_AP_SUPPORT */

	if ((pMbss->AuthMode == Ndis802_11AuthModeWPA) ||
	    (pMbss->AuthMode == Ndis802_11AuthModeWPAPSK))
		RSNIe = IE_WPA;
	else if ((pMbss->AuthMode == Ndis802_11AuthModeWPA2) ||
		 (pMbss->AuthMode == Ndis802_11AuthModeWPA2PSK))
		RSNIe = IE_WPA2;
#ifdef WAPI_SUPPORT
	else if ((pMbss->AuthMode == Ndis802_11AuthModeWAICERT) ||
		 (pMbss->AuthMode == Ndis802_11AuthModeWAIPSK))
		RSNIe = IE_WAPI;
#endif /* WAPI_SUPPORT */

	/* Append RSN_IE when WPA OR WPAPSK, */
	if ((pMbss->AuthMode == Ndis802_11AuthModeWPA1WPA2) ||
	    (pMbss->AuthMode == Ndis802_11AuthModeWPA1PSKWPA2PSK)) {
		MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
					1, &RSNIe,
					1, &pMbss->RSNIE_Len[0],
					pMbss->RSNIE_Len[0], pMbss->RSN_IE[0],
					1, &RSNIe2,
					1, &pMbss->RSNIE_Len[1],
					pMbss->RSNIE_Len[1], pMbss->RSN_IE[1],
					END_OF_ARGS);
		FrameLen += TmpLen;
	} else if (pMbss->AuthMode >= Ndis802_11AuthModeWPA) {
		MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
					1, &RSNIe,
					1, &pMbss->RSNIE_Len[0],
					pMbss->RSNIE_Len[0], pMbss->RSN_IE[0],
					END_OF_ARGS);
		FrameLen += TmpLen;
	}

	/* add WMM IE here */
	if (pMbss->bWmmCapable){
		unsigned char i;
		unsigned char WmeParmIe[26] = {IE_VENDOR_SPECIFIC, 24, 0x00, 0x50, 0xf2, 0x02, 0x01, 0x01, 0, 0};
		WmeParmIe[8] = pAd->ApCfg.BssEdcaParm.EdcaUpdateCount & 0x0f;
#ifdef UAPSD_SUPPORT
		UAPSD_MR_IE_FILL(WmeParmIe[8], &pAd->ApCfg.MBSSID[apidx].UapsdInfo);
#endif /* UAPSD_SUPPORT */
		for (i = QID_AC_BE; i <= QID_AC_VO; i++)
		{
			WmeParmIe[10 + (i * 4)] = (i << 5) +						/* b5-6 is ACI */
						((unsigned char)pAd->ApCfg.BssEdcaParm.bACM[i] << 4) +		/* b4 is ACM */
						(pAd->ApCfg.BssEdcaParm.Aifsn[i] & 0x0f);		/* b0-3 is AIFSN */
			WmeParmIe[11 + (i * 4)] = (pAd->ApCfg.BssEdcaParm.Cwmax[i] << 4) +		/* b5-8 is CWMAX */
						(pAd->ApCfg.BssEdcaParm.Cwmin[i] & 0x0f);		/* b0-3 is CWMIN */
			WmeParmIe[12 + (i * 4)] = (unsigned char)(pAd->ApCfg.BssEdcaParm.Txop[i] & 0xff);	/* low byte of TXOP */
			WmeParmIe[13 + (i * 4)] = (unsigned char)(pAd->ApCfg.BssEdcaParm.Txop[i] >> 8);		/* high byte of TXOP */
		}

		MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
					26, WmeParmIe,
					END_OF_ARGS);
		FrameLen += TmpLen;
	}

#ifdef AP_QLOAD_SUPPORT
	if (pAd->FlgQloadEnable != 0)
		FrameLen += QBSS_LoadElementAppend(pAd, pBeaconFrame+FrameLen);
#endif /* AP_QLOAD_SUPPORT */

	/* add Ralink-specific IE here - Byte0.b0=1 for aggregation, Byte0.b1=1 for piggy-back */
	{
		unsigned char RalinkSpecificIe[9] = {IE_VENDOR_SPECIFIC, 7, 0x00, 0x0c, 0x43, 0x00, 0x00, 0x00, 0x00};

		if (pComCfg->bAggregationCapable)
			RalinkSpecificIe[5] |= 0x1;
		if (pComCfg->bPiggyBackCapable)
			RalinkSpecificIe[5] |= 0x2;
#ifdef DOT11_N_SUPPORT
		if (pComCfg->bRdg)
			RalinkSpecificIe[5] |= 0x4;
#endif /* DOT11_N_SUPPORT */
		MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
					9, RalinkSpecificIe,
					END_OF_ARGS);
		FrameLen += TmpLen;
	}

#ifdef DOT11_N_SUPPORT
	if (WMODE_CAP_N(PhyMode) &&
		(pMbss->DesiredHtPhyInfo.bHtEnable)) {
		unsigned char HtLen, HtLen1;
		/*unsigned char i; */
		/* add HT Capability IE */
		HtLen = sizeof(pComCfg->HtCapability);
		HtLen1 = sizeof(pComCfg->AddHTInfo);

		if (pAd->bBroadComHT == TRUE) {
			unsigned char epigram_ie_len;
			unsigned char BROADCOM_HTC[4] = {0x0, 0x90, 0x4c, 0x33};
			unsigned char BROADCOM_AHTINFO[4] = {0x0, 0x90, 0x4c, 0x34};

			epigram_ie_len = HtLen + 4;
			MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
						  1, &WpaIe,
						  1, &epigram_ie_len,
						  4, &BROADCOM_HTC[0],
						  HtLen, &pComCfg->HtCapability,
						  END_OF_ARGS);
			FrameLen += TmpLen;

			epigram_ie_len = HtLen1 + 4;
			MakeOutgoingFrame(pBeaconFrame + FrameLen, &TmpLen,
						  1, &WpaIe,
						  1, &epigram_ie_len,
						  4, &BROADCOM_AHTINFO[0],
						  HtLen1, &pComCfg->AddHTInfo,
						  END_OF_ARGS);
			FrameLen += TmpLen;
		}
	}
#endif /* DOT11_N_SUPPORT */

	/* step 6. Since FrameLen may change, update TXWI. */
#ifdef A_BAND_SUPPORT
	if (pAd->CommonCfg.Channel > 14) {
		BeaconTransmit.field.MODE = MODE_OFDM;
		BeaconTransmit.field.MCS = MCS_RATE_6;
	}
#endif /* A_BAND_SUPPORT */
	RTMPWriteTxWI(pAd, &pAd->BeaconTxWI, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE, 0, 0xff,
		FrameLen, PID_MGMT, QID_MGMT, 0, IFS_HTTXOP, FALSE, &BeaconTransmit);

	/* step 7. move BEACON TXWI and frame content to on-chip memory */
	RT28xx_UpdateBeaconToAsic(pAd, apidx, FrameLen, UpdatePos);
}


/*
    ==========================================================================
    Description:
        Pre-build All BEACON frame in the shared memory
    ==========================================================================
*/
static unsigned char GetBcnNum(
	IN PRTMP_ADAPTER pAd)
{
	int i, NumBcn;

	NumBcn = 0;
	for (i = 0; i < pAd->ApCfg.BssidNum; i++) {
		if (pAd->ApCfg.MBSSID[i].bBcnSntReq) {
			pAd->ApCfg.MBSSID[i].BcnBufIdx = NumBcn;
			NumBcn++;
		}
	}


	return NumBcn;
}

VOID APMakeAllBssBeacon(
	IN PRTMP_ADAPTER pAd)
{
	INT i, j;
	unsigned int regValue;
	unsigned char NumOfMacs;
	unsigned char NumOfBcns;
	unsigned char TXWISize = pAd->chipCap.TXWISize;

	/* before MakeBssBeacon, clear all beacon TxD's valid bit */
	/* Note: can not use MAX_MBSSID_NUM here, or
		1. when MBSS_SUPPORT is enabled;
		2. MAX_MBSSID_NUM will be 8;
		3. if HW_BEACON_OFFSET is 0x0200,
			we will overwrite other shared memory SRAM of chip */
	/* use pAd->ApCfg.BssidNum to avoid the case is best */

	/* choose the Beacon number */
	NumOfBcns = GetBcnNum(pAd);

	for (i = 0; i < HW_BEACON_MAX_COUNT(pAd); i++)
		for (j = 0; j < TXWISize; j += 4)
			RTMP_CHIP_UPDATE_BEACON(pAd, pAd->BeaconOffset[i] + j, 0, 4);

#ifdef RTMP_MAC_USB
	RTUSBBssBeaconStop(pAd);
#endif /* RTMP_MAC_USB */

	for (i = 0; i < pAd->ApCfg.BssidNum; i++)
		APMakeBssBeacon(pAd, i);

	RTMP_IO_READ32(pAd, MAC_BSSID_DW1, &regValue);
	regValue &= 0x0000FFFF;

	/*
		Note:
			1.The MAC address of Mesh and AP-Client link are different from Main BSSID.
			2.If the Mesh link is included, its MAC address shall follow the last MBSSID's MAC by increasing 1.
			3.If the AP-Client link is included, its MAC address shall follow the Mesh interface MAC by increasing 1.
	*/
	NumOfMacs = pAd->ApCfg.BssidNum + MAX_MESH_NUM + MAX_APCLI_NUM;

	/* set Multiple BSSID mode */
	if (NumOfMacs <= 1) {
		pAd->ApCfg.MacMask = ~(1 - 1);
		/*regValue |= 0x0;*/
	} else if (NumOfMacs <= 2) {
		if (pAd->CurrentAddress[5] % 2 != 0)
			DBGPRINT(RT_DEBUG_ERROR, ("The 2-BSSID mode is enabled, the BSSID byte5 MUST be the multiple of 2\n"));

		regValue |= (1 << 16);
		pAd->ApCfg.MacMask = ~(2 - 1);
	} else if (NumOfMacs <= 4) {
		if (pAd->CurrentAddress[5] % 4 != 0)
			DBGPRINT(RT_DEBUG_ERROR, ("The 4-BSSID mode is enabled, the BSSID byte5 MUST be the multiple of 4\n"));

		regValue |= (2 << 16);
		pAd->ApCfg.MacMask = ~(4 - 1);
	} else if (NumOfMacs <= 8) {
		if (pAd->CurrentAddress[5] % 8 != 0)
			DBGPRINT(RT_DEBUG_ERROR, ("The 8-BSSID mode is enabled, the BSSID byte5 MUST be the multiple of 8\n"));

		regValue |= (3 << 16);
		pAd->ApCfg.MacMask = ~(8 - 1);
	}

	/* set Multiple BSSID Beacon number */
	if (NumOfBcns > 1)
		regValue |= (((NumOfBcns - 1) & 0x7) << 18);

	/* set as 0/1 bit-21 of MAC_BSSID_DW1(offset: 0x1014)
		to disable/enable the new MAC address assignment.  */
	if (pAd->chipCap.MBSSIDMode == MBSSID_MODE1)
		regValue |= (1 << 21);

	RTMP_IO_WRITE32(pAd, MAC_BSSID_DW1, regValue);

#ifdef HDR_TRANS_SUPPORT
	/*
		point WCID MAC table to 0x1800
		This is for debug.
		But HDR_TRANS doesn't work if you remove it.
		Check after IC formal release.
	*/
	regValue |= 0x18000000;
	RTMP_IO_WRITE32(pAd, HT_MAC_BSSID_DW1, regValue);
#endif /* HDR_TRANS_SUPPORT */

#ifdef RTMP_MAC_USB
	RTUSBBssBeaconStart(pAd);
#endif /* RTMP_MAC_USB */

}


/*
    ==========================================================================
    Description:
        Pre-build All BEACON frame in the shared memory
    ==========================================================================
*/
VOID APUpdateAllBeaconFrame(
	IN PRTMP_ADAPTER pAd)
{
	INT i;
#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
	bool FlgQloadIsAlarmIssued = FALSE;
#endif /* DOT11N_DRAFT3 */
#endif /* DOT11_N_SUPPORT */

	if (pAd->ApCfg.DtimCount == 0)
		pAd->ApCfg.DtimCount = pAd->ApCfg.DtimPeriod - 1;
	else
		pAd->ApCfg.DtimCount -= 1;
#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
	/* QLOAD ALARM */
#ifdef AP_QLOAD_SUPPORT
	FlgQloadIsAlarmIssued = QBSS_LoadIsAlarmIssued(pAd);
#endif /* AP_QLOAD_SUPPORT */

	if ((pAd->ApCfg.DtimCount == 0) &&
		(((pAd->CommonCfg.Bss2040CoexistFlag & BSS_2040_COEXIST_INFO_SYNC) &&
		  (pAd->CommonCfg.bForty_Mhz_Intolerant == FALSE)) ||
		(FlgQloadIsAlarmIssued == TRUE)))
	{
		unsigned char	prevBW, prevExtChOffset;
		DBGPRINT(RT_DEBUG_TRACE, ("DTIM Period reached, BSS20WidthReq=%d, Intolerant40=%d!\n",
				pAd->CommonCfg.LastBSSCoexist2040.field.BSS20WidthReq, pAd->CommonCfg.LastBSSCoexist2040.field.Intolerant40));
		pAd->CommonCfg.Bss2040CoexistFlag &= (~BSS_2040_COEXIST_INFO_SYNC);

		prevBW = pAd->CommonCfg.AddHTInfo.AddHtInfo.RecomWidth;
		prevExtChOffset = pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset;

		if (pAd->CommonCfg.LastBSSCoexist2040.field.BSS20WidthReq ||
			pAd->CommonCfg.LastBSSCoexist2040.field.Intolerant40 ||
			(pAd->MacTab.fAnyStaFortyIntolerant == TRUE) ||
			(FlgQloadIsAlarmIssued == TRUE))
		{
			pAd->CommonCfg.AddHTInfo.AddHtInfo.RecomWidth = 0;
			pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset = 0;
		}
		else
		{
			pAd->CommonCfg.AddHTInfo.AddHtInfo.RecomWidth = pAd->CommonCfg.RegTransmitSetting.field.BW;
			pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset = pAd->CommonCfg.RegTransmitSetting.field.EXTCHA;
		}
		DBGPRINT(RT_DEBUG_TRACE,("\tNow RecomWidth=%d, ExtChanOffset=%d, prevBW=%d, prevExtOffset=%d\n",
				pAd->CommonCfg.AddHTInfo.AddHtInfo.RecomWidth,
				pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset,
				prevBW, prevExtChOffset));
		pAd->CommonCfg.Bss2040CoexistFlag |= BSS_2040_COEXIST_INFO_NOTIFY;
	}
#endif /* DOT11N_DRAFT3 */
#endif /* DOT11_N_SUPPORT */

	for(i = 0; i < pAd->ApCfg.BssidNum; i++)
		APUpdateBeaconFrame(pAd, i);
}
