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
    ap.h

    Abstract:
    Miniport generic portion header file

    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    Paul Lin    08-01-2002    created
    James Tan   09-06-2002    modified (Revise NTCRegTable)
    John Chang  12-22-2004    modified for RT2561/2661. merge with STA driver
*/
#ifndef __AP_H__
#define __AP_H__




/* ============================================================= */
/*      Common definition */
/* ============================================================= */
#define MBSS_VLAN_INFO_GET(												\
	__pAd, __VLAN_VID, __VLAN_Priority, __FromWhichBSSID) 				\
{																		\
	if ((__FromWhichBSSID < __pAd->ApCfg.BssidNum) &&					\
		(__FromWhichBSSID < HW_BEACON_MAX_NUM) &&						\
		(__pAd->ApCfg.MBSSID[__FromWhichBSSID].VLAN_VID != 0))			\
	{																	\
		__VLAN_VID = __pAd->ApCfg.MBSSID[__FromWhichBSSID].VLAN_VID;	\
		__VLAN_Priority = __pAd->ApCfg.MBSSID[__FromWhichBSSID].VLAN_Priority; \
	}																	\
}

/* ============================================================= */
/*      Function Prototypes */
/* ============================================================= */

/* ap_data.c */

bool APBridgeToWirelessSta(
    IN  PRTMP_ADAPTER   pAd,
    IN  unsigned char *          pHeader,
    IN  unsigned int            HdrLen,
    IN  unsigned char *          pData,
    IN  unsigned int            DataLen,
    IN  unsigned long           fromwdsidx);

void RTMP_BASetup(
	IN PRTMP_ADAPTER pAd,
	IN PMAC_TABLE_ENTRY pMacEntry,
	IN unsigned char UserPriority);

void	APSendPackets(
	IN	NDIS_HANDLE		MiniportAdapterContext,
	IN	PPNDIS_PACKET	ppPacketArray,
	IN	unsigned int			NumberOfPackets);

NDIS_STATUS APSendPacket(
    IN  PRTMP_ADAPTER   pAd,
    IN  PNDIS_PACKET    pPacket);

NDIS_STATUS APInsertPsQueue(
	IN PRTMP_ADAPTER pAd,
	IN PNDIS_PACKET pPacket,
	IN MAC_TABLE_ENTRY *pMacEntry,
	IN unsigned char QueIdx);

NDIS_STATUS APHardTransmit(
	IN	PRTMP_ADAPTER	pAd,
	IN	TX_BLK			*pTxBlk,
	IN	unsigned char			QueIdx);

void APRxEAPOLFrameIndicate(
	IN	PRTMP_ADAPTER	pAd,
	IN	MAC_TABLE_ENTRY	*pEntry,
	IN	RX_BLK			*pRxBlk,
	IN	unsigned char			FromWhichBSSID);

NDIS_STATUS APCheckRxError(
	IN RTMP_ADAPTER *pAd,
	IN RXINFO_STRUC *pRxInfo,
	IN unsigned char Wcid);

bool APCheckClass2Class3Error(
    IN  PRTMP_ADAPTER   pAd,
	IN unsigned long Wcid, 
	IN  PHEADER_802_11  pHeader);

void APHandleRxPsPoll(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char *			pAddr,
	IN	unsigned short			Aid,
    IN	bool			isActive);

void    RTMPDescriptorEndianChange(
    IN  unsigned char *          pData,
    IN  unsigned long           DescriptorType);
    
void    RTMPFrameEndianChange(
    IN  PRTMP_ADAPTER   pAd,
    IN  unsigned char *          pData,
    IN  unsigned long           Dir,
    IN  bool         FromRxDoneInt);

/* ap_assoc.c */

void APAssocStateMachineInit(
    IN  PRTMP_ADAPTER   pAd, 
    IN  STATE_MACHINE *S, 
    OUT STATE_MACHINE_FUNC Trans[]);


void MbssKickOutStas(
	IN PRTMP_ADAPTER pAd,
	IN INT apidx,
	IN unsigned short Reason);

void APMlmeKickOutSta(
    IN PRTMP_ADAPTER pAd, 
	IN unsigned char * pStaAddr, 
	IN unsigned char Wcid,
	IN unsigned short Reason);



void  APCls3errAction(
    IN  PRTMP_ADAPTER   pAd, 
	IN 	unsigned long Wcid,
    IN	PHEADER_802_11	pHeader);

/*
void	RTMPAddClientSec(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char	BssIdx,
	IN unsigned char		 KeyIdx,
	IN unsigned char		 CipherAlg,
	IN unsigned char *		 pKey,
	IN unsigned char *		 pTxMic,
	IN unsigned char *		 pRxMic,
	IN MAC_TABLE_ENTRY *pEntry);
*/

/* ap_auth.c */

void APAuthStateMachineInit(
    IN PRTMP_ADAPTER pAd, 
    IN STATE_MACHINE *Sm, 
    OUT STATE_MACHINE_FUNC Trans[]);

void APCls2errAction(
    IN PRTMP_ADAPTER pAd, 
	IN 	unsigned long Wcid,
    IN	PHEADER_802_11	pHeader);

/* ap_connect.c */

#ifdef CONFIG_AP_SUPPORT
bool BeaconTransmitRequired(
	IN PRTMP_ADAPTER	pAd,
	IN INT				apidx,
	IN MULTISSID_STRUCT *pMbss);
#endif /* CONFIG_AP_SUPPORT */

void APMakeBssBeacon(
    IN  PRTMP_ADAPTER   pAd,
	IN	INT				apidx);

void  APUpdateBeaconFrame(
    IN  PRTMP_ADAPTER   pAd,
	IN	INT				apidx);

void APMakeAllBssBeacon(
    IN  PRTMP_ADAPTER   pAd);

void  APUpdateAllBeaconFrame(
    IN  PRTMP_ADAPTER   pAd);


/* ap_sync.c */

void APSyncStateMachineInit(
    IN PRTMP_ADAPTER pAd,
    IN STATE_MACHINE *Sm,
    OUT STATE_MACHINE_FUNC Trans[]);

void APScanTimeout(
	IN void * SystemSpecific1,
	IN void * FunctionContext,
	IN void * SystemSpecific2,
	IN void * SystemSpecific3);

void APInvalidStateWhenScan(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem);

void APScanTimeoutAction(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem);

void APPeerProbeReqAction(
    IN  PRTMP_ADAPTER pAd, 
    IN  MLME_QUEUE_ELEM *Elem);

void APPeerBeaconAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem);

void APMlmeScanReqAction(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem);

void APPeerBeaconAtScanAction(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem);

void APScanCnclAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);

void ApSiteSurvey(
	IN	PRTMP_ADAPTER  		pAd,
	IN	PNDIS_802_11_SSID	pSsid,
	IN	unsigned char				ScanType,
	IN	bool				ChannelSel);

void SupportRate(
	IN unsigned char * SupRate,
	IN unsigned char SupRateLen,
	IN unsigned char * ExtRate,
	IN unsigned char ExtRateLen,
	OUT unsigned char * *Rates,
	OUT unsigned char * RatesLen,
	OUT unsigned char * pMaxSupportRate);


bool ApScanRunning(
	IN PRTMP_ADAPTER pAd);
	
#ifdef DOT11N_DRAFT3
void APOverlappingBSSScan(
	IN RTMP_ADAPTER *pAd);

INT GetBssCoexEffectedChRange(
	IN RTMP_ADAPTER *pAd,
	IN BSS_COEX_CH_RANGE *pCoexChRange);

#endif /* DOT11N_DRAFT3 */

/* ap_wpa.c */
void WpaStateMachineInit(
    IN  PRTMP_ADAPTER   pAd, 
    IN  STATE_MACHINE *Sm, 
    OUT STATE_MACHINE_FUNC Trans[]);

/* ap_mlme.c */
void APMlmePeriodicExec(
    IN  PRTMP_ADAPTER   pAd);

bool APMsgTypeSubst(
    IN PRTMP_ADAPTER pAd,
    IN PFRAME_802_11 pFrame, 
    OUT INT *Machine, 
    OUT INT *MsgType);

void APQuickResponeForRateUpExec(
    IN void * SystemSpecific1, 
    IN void * FunctionContext, 
    IN void * SystemSpecific2, 
    IN void * SystemSpecific3);

#ifdef RTMP_MAC_USB
void BeaconUpdateExec(
    IN void * SystemSpecific1, 
    IN void * FunctionContext, 
    IN void * SystemSpecific2, 
    IN void * SystemSpecific3);
#endif /* RTMP_MAC_USB */

void RTMPSetPiggyBack(
	IN PRTMP_ADAPTER	pAd,
	IN bool			bPiggyBack);

void APAsicEvaluateRxAnt(
	IN PRTMP_ADAPTER	pAd);

void APAsicRxAntEvalTimeout(
	IN PRTMP_ADAPTER	pAd);

/* ap.c */
NDIS_STATUS APInitialize(
    IN  PRTMP_ADAPTER   pAd);

void APShutdown(
    IN PRTMP_ADAPTER    pAd);

void APStartUp(
    IN  PRTMP_ADAPTER   pAd);

void APStop(
    IN  PRTMP_ADAPTER   pAd);

void APCleanupPsQueue(
    IN  PRTMP_ADAPTER   pAd,
    IN  PQUEUE_HEADER   pQueue);


void MacTableMaintenance(
    IN PRTMP_ADAPTER pAd);

unsigned int MacTableAssocStaNumGet(
	IN PRTMP_ADAPTER pAd);

MAC_TABLE_ENTRY *APSsPsInquiry(
    IN  PRTMP_ADAPTER   pAd, 
    IN  unsigned char *          pAddr, 
    OUT SST             *Sst, 
    OUT unsigned short          *Aid,
    OUT unsigned char           *PsMode,
    OUT unsigned char           *Rate); 

bool APPsIndicate(
    IN  PRTMP_ADAPTER   pAd, 
    IN  unsigned char *          pAddr, 
	IN unsigned long Wcid, 
    IN  unsigned char           Psm);

#ifdef SYSTEM_LOG_SUPPORT
void ApLogEvent(
    IN PRTMP_ADAPTER    pAd,
    IN unsigned char *           pAddr,
    IN unsigned short           Event);
#else
#define ApLogEvent(_pAd, _pAddr, _Event)
#endif /* SYSTEM_LOG_SUPPORT */

#ifdef DOT11_N_SUPPORT
void APUpdateOperationMode(
    IN PRTMP_ADAPTER pAd);
#endif /* DOT11_N_SUPPORT */

void APUpdateCapabilityAndErpIe(
	IN PRTMP_ADAPTER pAd);

bool ApCheckAccessControlList(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char *        pAddr,
	IN unsigned char         Apidx);

void ApUpdateAccessControlList(
    IN PRTMP_ADAPTER pAd,
    IN unsigned char         Apidx);

void ApEnqueueNullFrame(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char *        pAddr,
	IN unsigned char         TxRate,
	IN unsigned char         PID,
	IN unsigned char         apidx,
    IN bool       bQosNull,
    IN bool       bEOSP,
    IN unsigned char         OldUP);

/* ap_sanity.c */


bool PeerAssocReqCmmSanity(
    IN PRTMP_ADAPTER pAd, 
	IN bool isRessoc,
    IN void *Msg, 
    IN INT MsgLen,
    IN IE_LISTS *ie_lists);


bool PeerDisassocReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN void *Msg, 
    IN unsigned long MsgLen, 
    OUT unsigned char * pAddr2, 
    OUT	unsigned short	*SeqNum,
    OUT unsigned short *Reason);

bool PeerDeauthReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN void *Msg, 
    IN unsigned long MsgLen, 
    OUT unsigned char * pAddr2, 
   	OUT	unsigned short	*SeqNum,    
    OUT unsigned short *Reason);

bool APPeerAuthSanity(
    IN PRTMP_ADAPTER pAd, 
    IN void *Msg, 
    IN unsigned long MsgLen, 
	OUT unsigned char * pAddr1, 
    OUT unsigned char * pAddr2, 
    OUT unsigned short *Alg, 
    OUT unsigned short *Seq, 
    OUT unsigned short *Status, 
    OUT char *ChlgText
	);


#ifdef DOT1X_SUPPORT
/* ap_cfg.h */
INT	Set_OwnIPAddr_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

INT	Set_EAPIfName_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

INT	Set_PreAuthIfName_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

/* Define in ap.c */
bool DOT1X_InternalCmdAction(
    IN  PRTMP_ADAPTER	pAd,
    IN  MAC_TABLE_ENTRY *pEntry,
    IN	unsigned char			cmd);

bool DOT1X_EapTriggerAction(
    IN  PRTMP_ADAPTER	pAd,
    IN  MAC_TABLE_ENTRY *pEntry);
#endif /* DOT1X_SUPPORT */
#endif  /* __AP_H__ */

void AP_E2PROM_IOCTL_PostCtrl(
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq,
	IN	char *					msg);

void IAPP_L2_UpdatePostCtrl(
	IN PRTMP_ADAPTER	pAd,
    IN unsigned char *mac_p,
    IN INT  bssid);
