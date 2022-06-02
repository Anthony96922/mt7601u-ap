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
    ap_apcli.h

    Abstract:
    Support AP-Client function.

    Revision History:
    Who               When            What
    --------------    ----------      ----------------------------------------------
    Shiang, Fonchi    02-13-2007      created
*/

#ifndef _AP_APCLI_H_
#define _AP_APCLI_H_

#ifdef APCLI_SUPPORT

#include "rtmp.h"
  
#define AUTH_TIMEOUT	300         /* unit: msec */
#define ASSOC_TIMEOUT	300         /* unit: msec */
/*#define JOIN_TIMEOUT	2000        // unit: msec // not used in Ap-client mode, remove it */
#define PROBE_TIMEOUT	1000        /* unit: msec */
  
#define APCLI_ROOT_BSSID_GET(pAd, wcid) ((pAd)->MacTab.Content[(wcid)].Addr)

/* sanity check for apidx */
#define APCLI_MR_APIDX_SANITY_CHECK(idx) \
{ \
	if ((idx) >= MAX_APCLI_NUM) \
	{ \
		(idx) = 0; \
		DBGPRINT(RT_DEBUG_ERROR, ("%s> Error! apcli-idx > MAX_APCLI_NUM!\n", __FUNCTION__)); \
	} \
}

typedef struct _APCLI_MLME_JOIN_REQ_STRUCT {
	unsigned char	Bssid[MAC_ADDR_LEN];
	unsigned char	SsidLen;
	unsigned char	Ssid[MAX_LEN_OF_SSID];
} APCLI_MLME_JOIN_REQ_STRUCT;

typedef struct _STA_CTRL_JOIN_REQ_STRUCT {
	unsigned short	Status;
} APCLI_CTRL_MSG_STRUCT, *PSTA_CTRL_MSG_STRUCT;

bool isValidApCliIf(
	SHORT ifIndex);

/* */
/* Private routines in apcli_ctrl.c */
/* */
VOID ApCliCtrlStateMachineInit(
	IN PRTMP_ADAPTER pAd,
	IN STATE_MACHINE *Sm,
	OUT STATE_MACHINE_FUNC Trans[]);

/* */
/* Private routines in apcli_sync.c */
/* */
VOID ApCliSyncStateMachineInit(
    IN PRTMP_ADAPTER pAd, 
    IN STATE_MACHINE *Sm, 
    OUT STATE_MACHINE_FUNC Trans[]);

/* */
/* Private routines in apcli_auth.c */
/* */
VOID ApCliAuthStateMachineInit(
    IN PRTMP_ADAPTER pAd, 
    IN STATE_MACHINE *Sm, 
    OUT STATE_MACHINE_FUNC Trans[]);

/* */
/* Private routines in apcli_assoc.c */
/* */
VOID ApCliAssocStateMachineInit(
    IN PRTMP_ADAPTER pAd, 
    IN STATE_MACHINE *Sm, 
    OUT STATE_MACHINE_FUNC Trans[]);

MAC_TABLE_ENTRY *ApCliTableLookUpByWcid(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char wcid,
	IN unsigned char * pAddrs);


bool ApCliAllowToSendPacket(
	IN RTMP_ADAPTER *pAd,
	IN PNDIS_PACKET pPacket,
	OUT unsigned char		*pWcid);
	
bool 	ApCliValidateRSNIE(
	IN		PRTMP_ADAPTER	pAd, 
	IN 		PEID_STRUCT    	pEid_ptr,
	IN		unsigned short			eid_len,
	IN		unsigned short			idx);


VOID ApCli_Remove(
	IN PRTMP_ADAPTER 	pAd);

VOID RT28xx_ApCli_Close(
	IN PRTMP_ADAPTER 	pAd);



INT ApCliIfLookUp(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pAddr);


INT ApCli_VirtualIF_PacketSend(
	IN PNDIS_PACKET		skb_p, 
	IN PNET_DEV			dev_p);

INT ApCli_VirtualIF_Ioctl(
	IN PNET_DEV				dev_p,
	IN OUT VOID 			*rq_p,
	IN INT cmd);

	
VOID ApCliMgtMacHeaderInit(
    IN	PRTMP_ADAPTER	pAd, 
    IN OUT PHEADER_802_11 pHdr80211, 
    IN unsigned char SubType, 
    IN unsigned char ToDs, 
    IN unsigned char * pDA, 
    IN unsigned char * pBssid,
    IN unsigned short ifIndex);

#ifdef DOT11_N_SUPPORT
bool ApCliCheckHt(
	IN		PRTMP_ADAPTER 		pAd,
	IN		unsigned short 				IfIndex,
	IN OUT	HT_CAPABILITY_IE 	*pHtCapability,
	IN OUT	ADD_HT_INFO_IE 		*pAddHtInfo);
#endif /* DOT11_N_SUPPORT */

bool ApCliLinkUp(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char ifIndex);

VOID ApCliLinkDown(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char ifIndex);

VOID ApCliIfUp(
	IN PRTMP_ADAPTER pAd);

VOID ApCliIfDown(
	IN PRTMP_ADAPTER pAd);

VOID ApCliIfMonitor(
	IN PRTMP_ADAPTER pAd);

bool ApCliMsgTypeSubst(
	IN PRTMP_ADAPTER  pAd,
	IN PFRAME_802_11 pFrame, 
	OUT INT *Machine, 
	OUT INT *MsgType);

bool preCheckMsgTypeSubset(
	IN PRTMP_ADAPTER  pAd,
	IN PFRAME_802_11 pFrame, 
	OUT INT *Machine, 
	OUT INT *MsgType);

bool ApCliPeerAssocRspSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *pMsg, 
    IN unsigned long MsgLen, 
    OUT unsigned char * pAddr2, 
    OUT unsigned short *pCapabilityInfo, 
    OUT unsigned short *pStatus, 
    OUT unsigned short *pAid, 
    OUT unsigned char SupRate[], 
    OUT unsigned char *pSupRateLen,
    OUT unsigned char ExtRate[], 
    OUT unsigned char *pExtRateLen,
    OUT HT_CAPABILITY_IE *pHtCapability,
    OUT ADD_HT_INFO_IE *pAddHtInfo,	/* AP might use this additional ht info IE */
    OUT unsigned char *pHtCapabilityLen,
    OUT unsigned char *pAddHtInfoLen,
    OUT unsigned char *pNewExtChannelOffset,
    OUT PEDCA_PARM pEdcaParm,
    OUT unsigned char *pCkipFlag);

VOID	ApCliPeerPairMsg1Action(
	IN PRTMP_ADAPTER    pAd, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN MLME_QUEUE_ELEM  *Elem);

VOID	ApCliPeerPairMsg3Action(
	IN PRTMP_ADAPTER    pAd, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN MLME_QUEUE_ELEM  *Elem);

VOID	ApCliPeerGroupMsg1Action(
	IN PRTMP_ADAPTER    pAd, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN MLME_QUEUE_ELEM  *Elem);

bool ApCliCheckRSNIE(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned char *          pData,
	IN  unsigned char           DataLen,
	IN  MAC_TABLE_ENTRY *pEntry,
	OUT	unsigned char			*Offset);

bool ApCliParseKeyData(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned char *          pKeyData,
	IN  unsigned char           KeyDataLen,
	IN  MAC_TABLE_ENTRY *pEntry,
	IN	unsigned char			IfIdx,
	IN	unsigned char			bPairewise);

bool  ApCliHandleRxBroadcastFrame(
	IN  PRTMP_ADAPTER   pAd,
	IN	RX_BLK			*pRxBlk,
	IN  MAC_TABLE_ENTRY *pEntry,
	IN	unsigned char			FromWhichBSSID);

VOID APCliInstallPairwiseKey(
	IN  PRTMP_ADAPTER   pAd,
	IN  MAC_TABLE_ENTRY *pEntry);

bool APCliInstallSharedKey(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned char *          pKey,
	IN  unsigned char           KeyLen,
	IN	unsigned char			DefaultKeyIdx,
	IN  MAC_TABLE_ENTRY *pEntry);

VOID ApCliUpdateMlmeRate(
	IN PRTMP_ADAPTER	pAd);

VOID APCli_Init(
	IN	PRTMP_ADAPTER				pAd,
	IN	RTMP_OS_NETDEV_OP_HOOK		*pNetDevOps);

bool ApCli_Open(
	IN	PRTMP_ADAPTER		pAd,
	IN	PNET_DEV			dev_p);

bool ApCli_Close(
	IN	PRTMP_ADAPTER	pAd,
	IN	PNET_DEV		dev_p);

bool ApCliWaitProbRsp(
	IN PRTMP_ADAPTER pAd,
	IN unsigned short ifIndex);

#endif /* APCLI_SUPPORT */

#endif /* _AP_APCLI_H_ */

