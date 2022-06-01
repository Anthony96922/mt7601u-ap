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
    rtmp.h

    Abstract:
    Miniport generic portion header file

    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    Paul Lin    2002-08-01    created
    James Tan   2002-09-06    modified (Revise NTCRegTable)
    John Chang  2004-09-06    modified for RT2600     
*/
#ifndef __RTMP_H__
#define __RTMP_H__

#include "link_list.h"
#include "spectrum_def.h"

#include "rtmp_dot11.h"
#include "wpa_cmm.h"

#ifdef CONFIG_AP_SUPPORT
#include "ap_autoChSel_cmm.h"
#endif /* CONFIG_AP_SUPPORT */

#include "wsc.h"
#ifdef MAT_SUPPORT
#include "mat.h"
#endif /* MAT_SUPPORT */


#ifdef WAPI_SUPPORT
#include "wapi_def.h"
#endif /* WAPI_SUPPORT */





#ifdef CLIENT_WDS
#include "client_wds_cmm.h"
#endif /* CLIENT_WDS */




#ifdef WFD_SUPPORT
#include "wfd_cmm.h"
#endif /* WFD_SUPPORT */

#include "drs_extr.h"

struct _RTMP_RA_LEGACY_TB;

typedef struct _RTMP_ADAPTER RTMP_ADAPTER;
typedef struct _RTMP_ADAPTER *PRTMP_ADAPTER;

typedef struct _RTMP_CHIP_OP_ RTMP_CHIP_OP;
typedef struct _RTMP_CHIP_CAP_ RTMP_CHIP_CAP;

typedef struct _UAPSD_INFO {
	bool bAPSDCapable;
} UAPSD_INFO;

#include "rtmp_mcu.h"

#include "rtmp_M51.h"

#ifdef CONFIG_ANDES_SUPPORT
#include "rtmp_and.h"
#endif

#include "rtmp_chip.h"

#include "radar.h"

#ifdef CARRIER_DETECTION_SUPPORT
#include "cs.h"
#endif /* CARRIER_DETECTION_SUPPORT */

#ifdef DFS_SUPPORT
#include "dfs.h"
#endif /* DFS_SUPPORT */

#ifdef LED_CONTROL_SUPPORT
#include "rt_led.h"
#endif /* LED_CONTROL_SUPPORT */


#ifdef RALINK_ATE
#include "rt_ate.h"
#endif /* RALINK_ATE */


/*#define DBG		1 */

bool RtmpPktPmBitCheck(
    IN  PRTMP_ADAPTER       pAd);

/*#define DBG_DIAGNOSE		1 */


/*+++Used for merge MiniportMMRequest() and MiniportDataMMRequest() into one function */
#define MAX_DATAMM_RETRY	3
#define MGMT_USE_QUEUE_FLAG	0x80
/*---Used for merge MiniportMMRequest() and MiniportDataMMRequest() into one function */
/* The number of channels for per-channel Tx power offset */


#define	MAXSEQ		(0xFFF)

#ifdef DOT11N_SS3_SUPPORT
#define MAX_MCS_SET 24		/* From MCS 0 ~ MCS 23 */
#else
#define MAX_MCS_SET 16		/* From MCS 0 ~ MCS 15 */
#endif /* DOT11N_SS3_SUPPORT */


#define MAX_TXPOWER_ARRAY_SIZE	5

extern unsigned char CISCO_OUI[];
extern unsigned char BaSizeArray[4];

extern unsigned char BROADCAST_ADDR[MAC_ADDR_LEN];
extern unsigned char ZERO_MAC_ADDR[MAC_ADDR_LEN];
extern unsigned long BIT32[32];
extern char *CipherName[];
extern unsigned char RxwiMCSToOfdmRate[12];
extern unsigned char SNAP_802_1H[6];
extern unsigned char SNAP_BRIDGE_TUNNEL[6];
extern unsigned char EAPOL[2];
extern unsigned char IPX[2];
extern unsigned char TPID[];
extern unsigned char APPLE_TALK[2];
extern unsigned char OfdmRateToRxwiMCS[];
extern unsigned char MapUserPriorityToAccessCategory[8];

extern unsigned char RateIdToMbps[];
extern unsigned short RateIdTo500Kbps[];

extern unsigned char CipherSuiteWpaNoneTkip[];
extern unsigned char CipherSuiteWpaNoneTkipLen;

extern unsigned char CipherSuiteWpaNoneAes[];
extern unsigned char CipherSuiteWpaNoneAesLen;

extern unsigned char SsidIe;
extern unsigned char SupRateIe;
extern unsigned char ExtRateIe;

#ifdef DOT11_N_SUPPORT
extern unsigned char HtCapIe;
extern unsigned char AddHtInfoIe;
extern unsigned char NewExtChanIe;
extern unsigned char BssCoexistIe;
extern unsigned char ExtHtCapIe;
#endif /* DOT11_N_SUPPORT */
extern unsigned char ExtCapIe;

extern unsigned char ErpIe;
extern unsigned char DsIe;
extern unsigned char TimIe;
extern unsigned char WpaIe;
extern unsigned char Wpa2Ie;
extern unsigned char IbssIe;
extern unsigned char WapiIe;

extern unsigned char WPA_OUI[];
extern unsigned char RSN_OUI[];
extern unsigned char WAPI_OUI[];
extern unsigned char WME_INFO_ELEM[];
extern unsigned char WME_PARM_ELEM[];
extern unsigned char RALINK_OUI[];
extern unsigned char PowerConstraintIE[];

typedef union _CAPTURE_MODE_PACKET_BUFFER {
	struct 
	{
		unsigned int       BYTE0:8;     
		unsigned int       BYTE1:8;
		unsigned int       BYTE2:8;
		unsigned int       BYTE3:8;
	} field;
	unsigned int                   Value;
}CAPTURE_MODE_PACKET_BUFFER, *PCAPTURE_MODE_PACKET_BUFFER;

typedef struct _RSSI_SAMPLE {
	char LastRssi0;		/* last received RSSI */
	char LastRssi1;		/* last received RSSI */
	char LastRssi2;		/* last received RSSI */
	char AvgRssi0;
	char AvgRssi1;
	char AvgRssi2;
	short AvgRssi0X8;
	short AvgRssi1X8;
	short AvgRssi2X8;
	char LastSnr0;
	char LastSnr1;
	char LastSnr2;
	char AvgSnr0;
	char AvgSnr1;
	char AvgSnr2;
	short AvgSnr0X8;
	short AvgSnr1X8;
	short AvgSnr2X8;
	char LastNoiseLevel0;
	char LastNoiseLevel1;
	char LastNoiseLevel2;
} RSSI_SAMPLE;

/* */
/*  Queue structure and macros */
/* */
#define InitializeQueueHeader(QueueHeader)              \
{                                                       \
	(QueueHeader)->Head = (QueueHeader)->Tail = NULL;   \
	(QueueHeader)->Number = 0;                          \
}

#define RemoveHeadQueue(QueueHeader)                \
(QueueHeader)->Head;                                \
{                                                   \
	PQUEUE_ENTRY pNext;                             \
	if ((QueueHeader)->Head != NULL)				\
	{												\
		pNext = (QueueHeader)->Head->Next;          \
		(QueueHeader)->Head->Next = NULL;		\
		(QueueHeader)->Head = pNext;                \
		if (pNext == NULL)                          \
			(QueueHeader)->Tail = NULL;             \
		(QueueHeader)->Number--;                    \
	}												\
}

#define InsertHeadQueue(QueueHeader, QueueEntry)            \
{                                                           \
		((PQUEUE_ENTRY)QueueEntry)->Next = (QueueHeader)->Head; \
		(QueueHeader)->Head = (PQUEUE_ENTRY)(QueueEntry);       \
		if ((QueueHeader)->Tail == NULL)                        \
			(QueueHeader)->Tail = (PQUEUE_ENTRY)(QueueEntry);   \
		(QueueHeader)->Number++;                                \
}

#define InsertTailQueue(QueueHeader, QueueEntry)				\
{                                                               \
	((PQUEUE_ENTRY)QueueEntry)->Next = NULL;                    \
	if ((QueueHeader)->Tail)                                    \
		(QueueHeader)->Tail->Next = (PQUEUE_ENTRY)(QueueEntry); \
	else                                                        \
		(QueueHeader)->Head = (PQUEUE_ENTRY)(QueueEntry);       \
	(QueueHeader)->Tail = (PQUEUE_ENTRY)(QueueEntry);           \
	(QueueHeader)->Number++;                                    \
}

#define InsertTailQueueAc(pAd, pEntry, QueueHeader, QueueEntry)			\
{																		\
	((PQUEUE_ENTRY)QueueEntry)->Next = NULL;							\
	if ((QueueHeader)->Tail)											\
		(QueueHeader)->Tail->Next = (PQUEUE_ENTRY)(QueueEntry);			\
	else																\
		(QueueHeader)->Head = (PQUEUE_ENTRY)(QueueEntry);				\
	(QueueHeader)->Tail = (PQUEUE_ENTRY)(QueueEntry);					\
	(QueueHeader)->Number++;											\
}
void DisplayTxAgg (RTMP_ADAPTER *pAd);


/* */
/*  Macros for flag and ref count operations */
/* */
#define RTMP_SET_FLAG(_M, _F)       ((_M)->Flags |= (_F))
#define RTMP_CLEAR_FLAG(_M, _F)     ((_M)->Flags &= ~(_F))
#define RTMP_CLEAR_FLAGS(_M)        ((_M)->Flags = 0)
#define RTMP_TEST_FLAG(_M, _F)      (((_M)->Flags & (_F)) != 0)
#define RTMP_TEST_FLAGS(_M, _F)     (((_M)->Flags & (_F)) == (_F))
/* Macro for power save flag. */
#define RTMP_SET_PSFLAG(_M, _F)       ((_M)->PSFlags |= (_F))
#define RTMP_CLEAR_PSFLAG(_M, _F)     ((_M)->PSFlags &= ~(_F))
#define RTMP_CLEAR_PSFLAGS(_M)        ((_M)->PSFlags = 0)
#define RTMP_TEST_PSFLAG(_M, _F)      (((_M)->PSFlags & (_F)) != 0)
#define RTMP_TEST_PSFLAGS(_M, _F)     (((_M)->PSFlags & (_F)) == (_F))

#define OPSTATUS_SET_FLAG(_pAd, _F)     ((_pAd)->CommonCfg.OpStatusFlags |= (_F))
#define OPSTATUS_CLEAR_FLAG(_pAd, _F)   ((_pAd)->CommonCfg.OpStatusFlags &= ~(_F))
#define OPSTATUS_TEST_FLAG(_pAd, _F)    (((_pAd)->CommonCfg.OpStatusFlags & (_F)) != 0)

#define CLIENT_STATUS_SET_FLAG(_pEntry,_F)      ((_pEntry)->ClientStatusFlags |= (_F))
#define CLIENT_STATUS_CLEAR_FLAG(_pEntry,_F)    ((_pEntry)->ClientStatusFlags &= ~(_F))
#define CLIENT_STATUS_TEST_FLAG(_pEntry,_F)     (((_pEntry)->ClientStatusFlags & (_F)) != 0)

#define RX_FILTER_SET_FLAG(_pAd, _F)    ((_pAd)->CommonCfg.PacketFilter |= (_F))
#define RX_FILTER_CLEAR_FLAG(_pAd, _F)  ((_pAd)->CommonCfg.PacketFilter &= ~(_F))
#define RX_FILTER_TEST_FLAG(_pAd, _F)   (((_pAd)->CommonCfg.PacketFilter & (_F)) != 0)

#define RTMP_SET_MORE_FLAG(_M, _F)       ((_M)->MoreFlags |= (_F))
#define RTMP_TEST_MORE_FLAG(_M, _F)      (((_M)->MoreFlags & (_F)) != 0)
#define RTMP_CLEAR_MORE_FLAG(_M, _F)     ((_M)->MoreFlags &= ~(_F))

#define SET_ASIC_CAP(_pAd, _caps)		((_pAd)->chipCap.asic_caps |= (_caps))
#define IS_ASIC_CAP(_pAd, _caps)			(((_pAd)->chipCap.asic_caps & (_caps)) != 0)
#define CLR_ASIC_CAP(_pAd, _caps)		((_pAd)->chipCap.asic_caps &= ~(_caps))



#define CKIP_KP_ON(_p)				((((_p)->StaCfg.CkipFlag) & 0x10) && ((_p)->StaCfg.bCkipCmicOn == TRUE))
#define CKIP_CMIC_ON(_p)			((((_p)->StaCfg.CkipFlag) & 0x08) && ((_p)->StaCfg.bCkipCmicOn == TRUE))

#define INC_RING_INDEX(_idx, _RingSize)    \
{                                          \
    (_idx) = (_idx+1) % (_RingSize);       \
}

#ifdef USB_BULK_BUF_ALIGMENT
#define CUR_WRITE_IDX_INC(_idx, _RingSize)    \
{                                          \
    (_idx) = (_idx+1) % (_RingSize);       \
}
#endif /* USB_BULK_BUF_ALIGMENT */

#ifdef DOT11_N_SUPPORT
/* StaActive.SupportedHtPhy.MCSSet is copied from AP beacon.  Don't need to update here. */
#define COPY_HTSETTINGS_FROM_MLME_AUX_TO_ACTIVE_CFG(_pAd)                                 \
{                                                                                       \
	_pAd->StaActive.SupportedHtPhy.ChannelWidth = _pAd->MlmeAux.HtCapability.HtCapInfo.ChannelWidth;      \
	_pAd->StaActive.SupportedHtPhy.MimoPs = _pAd->MlmeAux.HtCapability.HtCapInfo.MimoPs;      \
	_pAd->StaActive.SupportedHtPhy.GF = _pAd->MlmeAux.HtCapability.HtCapInfo.GF;      \
	_pAd->StaActive.SupportedHtPhy.ShortGIfor20 = _pAd->MlmeAux.HtCapability.HtCapInfo.ShortGIfor20;      \
	_pAd->StaActive.SupportedHtPhy.ShortGIfor40 = _pAd->MlmeAux.HtCapability.HtCapInfo.ShortGIfor40;      \
	_pAd->StaActive.SupportedHtPhy.TxSTBC = _pAd->MlmeAux.HtCapability.HtCapInfo.TxSTBC;      \
	_pAd->StaActive.SupportedHtPhy.RxSTBC = _pAd->MlmeAux.HtCapability.HtCapInfo.RxSTBC;      \
	_pAd->StaActive.SupportedHtPhy.ExtChanOffset = _pAd->MlmeAux.AddHtInfo.AddHtInfo.ExtChanOffset;      \
	_pAd->StaActive.SupportedHtPhy.RecomWidth = _pAd->MlmeAux.AddHtInfo.AddHtInfo.RecomWidth;      \
	_pAd->StaActive.SupportedHtPhy.OperaionMode = _pAd->MlmeAux.AddHtInfo.AddHtInfo2.OperaionMode;      \
	_pAd->StaActive.SupportedHtPhy.NonGfPresent = _pAd->MlmeAux.AddHtInfo.AddHtInfo2.NonGfPresent;      \
	NdisMoveMemory((_pAd)->MacTab.Content[BSSID_WCID].HTCapability.MCSSet, (_pAd)->StaActive.SupportedPhyInfo.MCSSet, sizeof(unsigned char) * 16);\
}

#define COPY_AP_HTSETTINGS_FROM_BEACON(_pAd, _pHtCapability)                                 \
{                                                                                       \
	_pAd->MacTab.Content[BSSID_WCID].AMsduSize = (unsigned char)(_pHtCapability->HtCapInfo.AMsduSize);	\
	_pAd->MacTab.Content[BSSID_WCID].MmpsMode= (unsigned char)(_pHtCapability->HtCapInfo.MimoPs);	\
	_pAd->MacTab.Content[BSSID_WCID].MaxRAmpduFactor = (unsigned char)(_pHtCapability->HtCapParm.MaxRAmpduFactor);	\
}
#endif /* DOT11_N_SUPPORT */

#ifdef DOT11_VHT_AC
#define COPY_VHT_FROM_MLME_AUX_TO_ACTIVE_CFG(_pAd)                                 \
{                                                                                       \
}
#endif /* DOT11_VHT_AC */


/*
	Common fragment list structure -  Identical to the scatter gather frag list structure
*/
#define NIC_MAX_PHYS_BUF_COUNT              8

typedef struct _RTMP_SCATTER_GATHER_ELEMENT {
	void * Address;
	unsigned long Length;
	unsigned long * Reserved;
} RTMP_SCATTER_GATHER_ELEMENT, *PRTMP_SCATTER_GATHER_ELEMENT;

typedef struct _RTMP_SCATTER_GATHER_LIST {
	unsigned long NumberOfElements;
	unsigned long * Reserved;
	RTMP_SCATTER_GATHER_ELEMENT Elements[NIC_MAX_PHYS_BUF_COUNT];
} RTMP_SCATTER_GATHER_LIST, *PRTMP_SCATTER_GATHER_LIST;


/*
	Some utility macros
*/
#ifndef min
#define min(_a, _b)     (((_a) < (_b)) ? (_a) : (_b))
#endif

#ifndef max
#define max(_a, _b)     (((_a) > (_b)) ? (_a) : (_b))
#endif

#define GET_LNA_GAIN(_pAd)	((_pAd->LatchRfRegs.Channel <= 14) ? (_pAd->BLNAGain) : ((_pAd->LatchRfRegs.Channel <= 64) ? (_pAd->ALNAGain0) : ((_pAd->LatchRfRegs.Channel <= 128) ? (_pAd->ALNAGain1) : (_pAd->ALNAGain2))))

#define INC_COUNTER64(Val)          (Val.QuadPart++)

#define INFRA_ON(_p)                (OPSTATUS_TEST_FLAG(_p, fOP_STATUS_INFRA_ON))
#define ADHOC_ON(_p)                (OPSTATUS_TEST_FLAG(_p, fOP_STATUS_ADHOC_ON))
#define MONITOR_ON(_p)              (((_p)->StaCfg.BssType) == BSS_MONITOR)
#define IDLE_ON(_p)                 (!INFRA_ON(_p) && !ADHOC_ON(_p))

/* Check LEAP & CCKM flags */
#define LEAP_ON(_p)                 (((_p)->StaCfg.LeapAuthMode) == CISCO_AuthModeLEAP)
#define LEAP_CCKM_ON(_p)            ((((_p)->StaCfg.LeapAuthMode) == CISCO_AuthModeLEAP) && ((_p)->StaCfg.LeapAuthInfo.CCKM == TRUE))

/* if orginal Ethernet frame contains no LLC/SNAP, then an extra LLC/SNAP encap is required */
#define EXTRA_LLCSNAP_ENCAP_FROM_PKT_START(_pBufVA, _pExtraLlcSnapEncap)		\
{																\
	if (((*(_pBufVA + 12) << 8) + *(_pBufVA + 13)) > 1500)		\
	{															\
		_pExtraLlcSnapEncap = SNAP_802_1H;						\
		if (NdisEqualMemory(IPX, _pBufVA + 12, 2) || 			\
			NdisEqualMemory(APPLE_TALK, _pBufVA + 12, 2))		\
		{														\
			_pExtraLlcSnapEncap = SNAP_BRIDGE_TUNNEL;			\
		}														\
	}															\
	else														\
	{															\
		_pExtraLlcSnapEncap = NULL;								\
	}															\
}

/* New Define for new Tx Path. */
#define EXTRA_LLCSNAP_ENCAP_FROM_PKT_OFFSET(_pBufVA, _pExtraLlcSnapEncap)	\
{																\
	if (((*(_pBufVA) << 8) + *(_pBufVA + 1)) > 1500)			\
	{															\
		_pExtraLlcSnapEncap = SNAP_802_1H;						\
		if (NdisEqualMemory(IPX, _pBufVA, 2) || 				\
			NdisEqualMemory(APPLE_TALK, _pBufVA, 2))			\
		{														\
			_pExtraLlcSnapEncap = SNAP_BRIDGE_TUNNEL;			\
		}														\
	}															\
	else														\
	{															\
		_pExtraLlcSnapEncap = NULL;								\
	}															\
}

#define MAKE_802_3_HEADER(_p, _pMac1, _pMac2, _pType)                   \
{                                                                       \
    NdisMoveMemory(_p, _pMac1, MAC_ADDR_LEN);                           \
    NdisMoveMemory((_p + MAC_ADDR_LEN), _pMac2, MAC_ADDR_LEN);          \
    NdisMoveMemory((_p + MAC_ADDR_LEN * 2), _pType, LENGTH_802_3_TYPE); \
}

/*
	if pData has no LLC/SNAP (neither RFC1042 nor Bridge tunnel),
		keep it that way.
	else if the received frame is LLC/SNAP-encaped IPX or APPLETALK,
		preserve the LLC/SNAP field
	else remove the LLC/SNAP field from the result Ethernet frame

	Patch for WHQL only, which did not turn on Netbios but use IPX within its payload
	Note:
		_pData & _DataSize may be altered (remove 8-byte LLC/SNAP) by this MACRO
		_pRemovedLLCSNAP: pointer to removed LLC/SNAP; NULL is not removed
*/
#define CONVERT_TO_802_3(_p8023hdr, _pDA, _pSA, _pData, _DataSize, _pRemovedLLCSNAP)      \
{                                                                       \
    char LLC_Len[2];                                                    \
                                                                        \
    _pRemovedLLCSNAP = NULL;                                            \
    if (NdisEqualMemory(SNAP_802_1H, _pData, 6)  ||                     \
        NdisEqualMemory(SNAP_BRIDGE_TUNNEL, _pData, 6))                 \
    {                                                                   \
        unsigned char * pProto = _pData + 6;                                     \
                                                                        \
        if ((NdisEqualMemory(IPX, pProto, 2) || NdisEqualMemory(APPLE_TALK, pProto, 2)) &&  \
            NdisEqualMemory(SNAP_802_1H, _pData, 6))                    \
        {                                                               \
            LLC_Len[0] = (unsigned char)(_DataSize >> 8);                       \
            LLC_Len[1] = (unsigned char)(_DataSize & (256 - 1));                \
            MAKE_802_3_HEADER(_p8023hdr, _pDA, _pSA, LLC_Len);          \
        }                                                               \
        else                                                            \
        {                                                               \
            MAKE_802_3_HEADER(_p8023hdr, _pDA, _pSA, pProto);           \
            _pRemovedLLCSNAP = _pData;                                  \
            _DataSize -= LENGTH_802_1_H;                                \
            _pData += LENGTH_802_1_H;                                   \
        }                                                               \
    }                                                                   \
    else                                                                \
    {                                                                   \
        LLC_Len[0] = (unsigned char)(_DataSize >> 8);                           \
        LLC_Len[1] = (unsigned char)(_DataSize & (256 - 1));                    \
        MAKE_802_3_HEADER(_p8023hdr, _pDA, _pSA, LLC_Len);              \
    }                                                                   \
}

/*
	Enqueue this frame to MLME engine
	We need to enqueue the whole frame because MLME need to pass data type
	information from 802.11 header
*/
#ifdef RTMP_MAC_USB
#define REPORT_MGMT_FRAME_TO_MLME(_pAd, Wcid, _pFrame, _FrameSize, _Rssi0, _Rssi1, _Rssi2, _MinSNR, _AntSel, _OpMode)        \
{                                                                                       \
    unsigned int High32TSF=0, Low32TSF=0;                                                          \
    MlmeEnqueueForRecv(_pAd, Wcid, High32TSF, Low32TSF, (unsigned char)_Rssi0, (unsigned char)_Rssi1,(unsigned char)_Rssi2 ,_AntSel, _FrameSize, _pFrame, (unsigned char)_MinSNR, _OpMode);   \
}
#endif /* RTMP_MAC_USB */

#define MAC_ADDR_EQUAL(pAddr1,pAddr2)           RTMPEqualMemory((void *)(pAddr1), (void *)(pAddr2), MAC_ADDR_LEN)
#define SSID_EQUAL(ssid1, len1, ssid2, len2)    ((len1==len2) && (RTMPEqualMemory(ssid1, ssid2, len1)))



/* */
/*  Data buffer for DMA operation, the buffer must be contiguous physical memory */
/*  Both DMA to / from CPU use the same structure. */
/* */
typedef struct _RTMP_DMABUF {
	unsigned long AllocSize;
	void * AllocVa;		/* TxBuf virtual address */
	NDIS_PHYSICAL_ADDRESS AllocPa;	/* TxBuf physical address */
} RTMP_DMABUF, *PRTMP_DMABUF;

/* */
/* Control block (Descriptor) for all ring descriptor DMA operation, buffer must be */
/* contiguous physical memory. NDIS_PACKET stored the binding Rx packet descriptor */
/* which won't be released, driver has to wait until upper layer return the packet */
/* before giveing up this rx ring descriptor to ASIC. NDIS_BUFFER is assocaited pair */
/* to describe the packet buffer. For Tx, NDIS_PACKET stored the tx packet descriptor */
/* which driver should ACK upper layer when the tx is physically done or failed. */
/* */
typedef struct _RTMP_DMACB {
	unsigned long AllocSize;	/* Control block size */
	void * AllocVa;		/* Control block virtual address */
	NDIS_PHYSICAL_ADDRESS AllocPa;	/* Control block physical address */
	PNDIS_PACKET pNdisPacket;
	PNDIS_PACKET pNextNdisPacket;

	RTMP_DMABUF DmaBuf;	/* Associated DMA buffer structure */
#ifdef CACHE_LINE_32B
	RXD_STRUC LastBDInfo;
#endif /* CACHE_LINE_32B */
} RTMP_DMACB, *PRTMP_DMACB;

typedef struct _RTMP_TX_RING {
	RTMP_DMACB Cell[TX_RING_SIZE];
	unsigned int TxCpuIdx;
	unsigned int TxDmaIdx;
	unsigned int TxSwFreeIdx;	/* software next free tx index */
} RTMP_TX_RING, *PRTMP_TX_RING;

typedef struct _RTMP_RX_RING {
	RTMP_DMACB Cell[RX_RING_SIZE];
	unsigned int RxCpuIdx;
	unsigned int RxDmaIdx;
	int RxSwReadIdx;	/* software next read index */
} RTMP_RX_RING, *PRTMP_RX_RING;

typedef struct _RTMP_MGMT_RING {
	RTMP_DMACB Cell[MGMT_RING_SIZE];
	unsigned int TxCpuIdx;
	unsigned int TxDmaIdx;
	unsigned int TxSwFreeIdx;	/* software next free tx index */
} RTMP_MGMT_RING, *PRTMP_MGMT_RING;

typedef struct _RTMP_CTRL_RING {
	RTMP_DMACB Cell[16];
	unsigned int TxCpuIdx;
	unsigned int TxDmaIdx;
	unsigned int TxSwFreeIdx;	/* software next free tx index */
} RTMP_CTRL_RING, *PRTMP_CTRL_RING;

/* */
/*  Statistic counter structure */
/* */
typedef struct _COUNTER_802_3 {
	/* General Stats */
	unsigned long GoodTransmits;
	unsigned long GoodReceives;
	unsigned long TxErrors;
	unsigned long RxErrors;
	unsigned long RxNoBuffer;

	/* Ethernet Stats */
	unsigned long RcvAlignmentErrors;
	unsigned long OneCollision;
	unsigned long MoreCollisions;

} COUNTER_802_3, *PCOUNTER_802_3;

typedef struct _COUNTER_802_11 {
	unsigned long Length;
/*	LARGE_INTEGER   LastTransmittedFragmentCount; */
	LARGE_INTEGER TransmittedFragmentCount;
	LARGE_INTEGER MulticastTransmittedFrameCount;
	LARGE_INTEGER FailedCount;
	LARGE_INTEGER RetryCount;
	LARGE_INTEGER MultipleRetryCount;
	LARGE_INTEGER RTSSuccessCount;
	LARGE_INTEGER RTSFailureCount;
	LARGE_INTEGER ACKFailureCount;
	LARGE_INTEGER FrameDuplicateCount;
	LARGE_INTEGER ReceivedFragmentCount;
	LARGE_INTEGER MulticastReceivedFrameCount;
	LARGE_INTEGER FCSErrorCount;
	LARGE_INTEGER TransmittedFrameCount;
	LARGE_INTEGER WEPUndecryptableCount;
	LARGE_INTEGER TransmitCountFrmOs;
} COUNTER_802_11, *PCOUNTER_802_11;


#ifdef RT3290
typedef struct _ANT_DIVERSITY
{
	bool		RateUp;
	unsigned long		TrainCounter;
	unsigned long		AntennaDiversityState;	// 0->Stable state 1->training state
	unsigned long		AntennaDiversityPER[2];  // 0 ->main 1->aux
	unsigned long		AntennaDiversityTxPacketCount[2];  // 0 ->main 1->aux	
	unsigned long		AntennaDiversityRxPacketCount[2];
	char		Rssi[2];
	unsigned long		AntennaDiversityCount;
	unsigned long		AntennaDiversityTrigger;	
}ANT_DIVERSITY, *PANT_DIVERSITY;
#endif /* RT3290 */

typedef struct _COUNTER_RALINK {
	unsigned int OneSecStart;	/* for one sec count clear use */
	unsigned int OneSecBeaconSentCnt;
	unsigned int OneSecFalseCCACnt;	/* CCA error count, for debug purpose, might move to global counter */
	unsigned int OneSecRxFcsErrCnt;	/* CRC error */
	unsigned int OneSecRxOkCnt;	/* RX without error */
	unsigned int OneSecTxFailCount;
	unsigned int OneSecTxNoRetryOkCount;
	unsigned int OneSecTxRetryOkCount;
	unsigned int OneSecRxOkDataCnt;	/* unicast-to-me DATA frame count */
	unsigned int OneSecTransmittedByteCount;	/* both successful and failure, used to calculate TX throughput */

	unsigned long OneSecOsTxCount[NUM_OF_TX_RING];
	unsigned long OneSecDmaDoneCount[NUM_OF_TX_RING];
	unsigned int OneSecTxDoneCount;
	unsigned long OneSecRxCount;
	unsigned int OneSecReceivedByteCount;
	unsigned int OneSecTxAggregationCount;
	unsigned int OneSecRxAggregationCount;
	unsigned int OneSecEnd;	/* for one sec count clear use */

#ifdef MICROWAVE_OVEN_SUPPORT
	unsigned short FalseCCACnt_100MS[MLME_TASK_EXEC_MULTIPLE]; /* one handred millisecond false CCA Count */
	unsigned short PLCPErrCnt_100MS[MLME_TASK_EXEC_MULTIPLE]; /* one handred millisecond PLCP Error Count */
#endif /* MICROWAVE_OVEN_SUPPORT */

	unsigned long TransmittedByteCount;	/* both successful and failure, used to calculate TX throughput */
	unsigned long ReceivedByteCount;	/* both CRC okay and CRC error, used to calculate RX throughput */
#ifdef RT3290
	// TODO: shiang, check the purpose of following parameter
	unsigned long OneSecRxOkCnt2; /* RX without error */
#endif /* RT3290 */
	unsigned long BadCQIAutoRecoveryCount;
	unsigned long PoorCQIRoamingCount;
	unsigned long MgmtRingFullCount;
	unsigned long RxCountSinceLastNULL;
	unsigned long RxCount;
	unsigned long KickTxCount;
	LARGE_INTEGER RealFcsErrCount;
	unsigned long PendingNdisPacketCount;
	unsigned long FalseCCACnt;                    /* CCA error count */

	unsigned int LastOneSecTotalTxCount;	/* OneSecTxNoRetryOkCount + OneSecTxRetryOkCount + OneSecTxFailCount */
	unsigned int LastOneSecRxOkDataCnt;	/* OneSecRxOkDataCnt */
	unsigned long DuplicateRcv;
	unsigned long TxAggCount;
	unsigned long TxNonAggCount;
	unsigned long TxAgg1MPDUCount;
	unsigned long TxAgg2MPDUCount;
	unsigned long TxAgg3MPDUCount;
	unsigned long TxAgg4MPDUCount;
	unsigned long TxAgg5MPDUCount;
	unsigned long TxAgg6MPDUCount;
	unsigned long TxAgg7MPDUCount;
	unsigned long TxAgg8MPDUCount;
	unsigned long TxAgg9MPDUCount;
	unsigned long TxAgg10MPDUCount;
	unsigned long TxAgg11MPDUCount;
	unsigned long TxAgg12MPDUCount;
	unsigned long TxAgg13MPDUCount;
	unsigned long TxAgg14MPDUCount;
	unsigned long TxAgg15MPDUCount;
	unsigned long TxAgg16MPDUCount;

	LARGE_INTEGER TransmittedOctetsInAMSDU;
	LARGE_INTEGER TransmittedAMSDUCount;
	LARGE_INTEGER ReceivedOctesInAMSDUCount;
	LARGE_INTEGER ReceivedAMSDUCount;
	LARGE_INTEGER TransmittedAMPDUCount;
	LARGE_INTEGER TransmittedMPDUsInAMPDUCount;
	LARGE_INTEGER TransmittedOctetsInAMPDUCount;
	LARGE_INTEGER MPDUInReceivedAMPDUCount;
} COUNTER_RALINK, *PCOUNTER_RALINK;

typedef struct _COUNTER_DRS {
	/* to record the each TX rate's quality. 0 is best, the bigger the worse. */
	unsigned short TxQuality[MAX_TX_RATE_INDEX+1];
	unsigned char PER[MAX_TX_RATE_INDEX+1];
	unsigned char TxRateUpPenalty;	/* extra # of second penalty due to last unstable condition */
	unsigned long CurrTxRateStableTime;	/* # of second in current TX rate */
	/*bool         fNoisyEnvironment; */
	bool fLastSecAccordingRSSI;
	unsigned char LastSecTxRateChangeAction;	/* 0: no change, 1:rate UP, 2:rate down */
	unsigned char LastTimeTxRateChangeAction;	/*Keep last time value of LastSecTxRateChangeAction */
	unsigned long LastTxOkCount;
} COUNTER_DRS, *PCOUNTER_DRS;


#ifdef DOT11_N_SUPPORT
#ifdef TXBF_SUPPORT
typedef
    struct {
	unsigned long TxSuccessCount;
	unsigned long TxRetryCount;
	unsigned long TxFailCount;
	unsigned long ETxSuccessCount;
	unsigned long ETxRetryCount;
	unsigned long ETxFailCount;
	unsigned long ITxSuccessCount;
	unsigned long ITxRetryCount;
	unsigned long ITxFailCount;
} COUNTER_TXBF;
#endif /* TXBF_SUPPORT */
#endif /* DOT11_N_SUPPORT */


#ifdef STREAM_MODE_SUPPORT
typedef struct _STREAM_MODE_ENTRY_{
#define STREAM_MODE_STATIC		1
	unsigned short flag;
	unsigned char macAddr[MAC_ADDR_LEN];
}STREAM_MODE_ENTRY;
#endif /* STREAM_MODE_SUPPORT */

/* for Microwave oven */
#ifdef MICROWAVE_OVEN_SUPPORT
typedef struct _MO_CFG_STRUCT {
	bool		bEnable;
	unsigned char  		nPeriod_Cnt; 	/* measurement period 100ms, mitigate the interference period 900 ms */
	unsigned short 		nFalseCCACnt;	
	unsigned short		nFalseCCATh;	/* default is 100 */
#ifdef MT7601
	unsigned int		Stored_BBP_R65;
	unsigned char		Stored_RF_B5_R6;
	unsigned char		Stored_RF_B5_R7;
#endif /* MT7601 */
} MO_CFG_STRUCT, *PMO_CFG_STRUCT;
#endif /* MICROWAVE_OVEN_SUPPORT */

/***************************************************************************
  *	security key related data structure
  **************************************************************************/

/* structure to define WPA Group Key Rekey Interval */
typedef struct GNU_PACKED _RT_802_11_WPA_REKEY {
	unsigned long ReKeyMethod;	/* mechanism for rekeying: 0:disable, 1: time-based, 2: packet-based */
	unsigned long ReKeyInterval;	/* time-based: seconds, packet-based: kilo-packets */
} RT_WPA_REKEY,*PRT_WPA_REKEY, RT_802_11_WPA_REKEY, *PRT_802_11_WPA_REKEY;


#ifdef RTMP_MAC_USB
/***************************************************************************
  *	RTUSB I/O related data structure
  **************************************************************************/

/* for USB interface, avoid in interrupt when write key */
typedef struct RT_ADD_PAIRWISE_KEY_ENTRY {
	unsigned char MacAddr[6];
	unsigned short MacTabMatchWCID;	/* ASIC */
	CIPHER_KEY CipherKey;
} RT_ADD_PAIRWISE_KEY_ENTRY,*PRT_ADD_PAIRWISE_KEY_ENTRY;


/* Cipher suite type for mixed mode group cipher, P802.11i-2004 */
typedef enum _RT_802_11_CIPHER_SUITE_TYPE {
	Cipher_Type_NONE,
	Cipher_Type_WEP40,
	Cipher_Type_TKIP,
	Cipher_Type_RSVD,
	Cipher_Type_CCMP,
	Cipher_Type_WEP104
} RT_802_11_CIPHER_SUITE_TYPE, *PRT_802_11_CIPHER_SUITE_TYPE;
#endif /* RTMP_MAC_USB */

typedef struct {
	unsigned char Addr[MAC_ADDR_LEN];
	unsigned char ErrorCode[2];	/*00 01-Invalid authentication type */
	/*00 02-Authentication timeout */
	/*00 03-Challenge from AP failed */
	/*00 04-Challenge to AP failed */
	bool Reported;
} ROGUEAP_ENTRY, *PROGUEAP_ENTRY;

typedef struct {
	unsigned char RogueApNr;
	ROGUEAP_ENTRY RogueApEntry[MAX_LEN_OF_BSS_TABLE];
} ROGUEAP_TABLE, *PROGUEAP_TABLE;

/*
  *	Fragment Frame structure
  */
typedef struct _FRAGMENT_FRAME {
	PNDIS_PACKET pFragPacket;
	unsigned long RxSize;
	unsigned short Sequence;
	unsigned short LastFrag;
	unsigned long Flags;		/* Some extra frame information. bit 0: LLC presented */
} FRAGMENT_FRAME, *PFRAGMENT_FRAME;


/* */
/* Tkip Key structure which RC4 key & MIC calculation */
/* */
typedef struct _TKIP_KEY_INFO {
	unsigned int nBytesInM;		/* # bytes in M for MICKEY */
	unsigned long IV16;
	unsigned long IV32;
	unsigned long K0;		/* for MICKEY Low */
	unsigned long K1;		/* for MICKEY Hig */
	unsigned long L;		/* Current state for MICKEY */
	unsigned long R;		/* Current state for MICKEY */
	unsigned long M;		/* Message accumulator for MICKEY */
	unsigned char RC4KEY[16];
	unsigned char MIC[8];
} TKIP_KEY_INFO, *PTKIP_KEY_INFO;


/* */
/* Private / Misc data, counters for driver internal use */
/* */
typedef struct __PRIVATE_STRUC {
	unsigned int SystemResetCnt;	/* System reset counter */
	unsigned int TxRingFullCnt;	/* Tx ring full occurrance number */
	unsigned int PhyRxErrCnt;	/* PHY Rx error count, for debug purpose, might move to global counter */
	/* Variables for WEP encryption / decryption in rtmp_wep.c */
	/* Tkip stuff */
	TKIP_KEY_INFO Tx;
	TKIP_KEY_INFO Rx;
} PRIVATE_STRUC, *PPRIVATE_STRUC;


/***************************************************************************
  *	Channel and BBP related data structures
  **************************************************************************/
/* structure to tune BBP R66 (BBP TUNING) */
typedef struct _BBP_R66_TUNING {
	bool bEnable;
	unsigned short FalseCcaLowerThreshold;	/* default 100 */
	unsigned short FalseCcaUpperThreshold;	/* default 512 */
	unsigned char R66Delta;
	unsigned char R66CurrentValue;
	bool R66LowerUpperSelect;	/*Before LinkUp, Used LowerBound or UpperBound as R66 value. */
} BBP_R66_TUNING, *PBBP_R66_TUNING;


#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
#define EFFECTED_CH_SECONDARY 0x1
#define EFFECTED_CH_PRIMARY	0x2
#define EFFECTED_CH_LEGACY		0x4
#endif /* DOT11N_DRAFT3 */
#endif /* DOT11_N_SUPPORT */

/* structure to store channel TX power */
typedef struct _CHANNEL_TX_POWER {
	unsigned short RemainingTimeForUse;	/*unit: sec */
	unsigned char Channel;
#ifdef DOT11N_DRAFT3
	bool bEffectedChannel;	/* For BW 40 operating in 2.4GHz , the "effected channel" is the channel that is covered in 40Mhz. */
#endif /* DOT11N_DRAFT3 */
	char Power;
	char Power2;
#ifdef DOT11N_SS3_SUPPORT
	char Power3;
#endif /* DOT11N_SS3_SUPPORT */
	unsigned char MaxTxPwr;
	unsigned char DfsReq;
	unsigned char RegulatoryDomain;

/*
	Channel property:
 
	CHANNEL_DISABLED: The channel is disabled.
	CHANNEL_PASSIVE_SCAN: Only passive scanning is allowed.
	CHANNEL_NO_IBSS: IBSS is not allowed.
	CHANNEL_RADAR: Radar detection is required.
	CHANNEL_NO_FAT_ABOVE: Extension channel above this channel is not allowed.
	CHANNEL_NO_FAT_BELOW: Extension channel below this channel is not allowed.
	CHANNEL_40M_CAP: 40 BW channel group
 */
#define CHANNEL_DEFAULT_PROP	0x00
#define CHANNEL_DISABLED		0x01	/* no use */
#define CHANNEL_PASSIVE_SCAN	0x02
#define CHANNEL_NO_IBSS			0x04
#define CHANNEL_RADAR			0x08
#define CHANNEL_NO_FAT_ABOVE	0x10
#define CHANNEL_NO_FAT_BELOW	0x20
#define CHANNEL_40M_CAP			0x40

	unsigned char Flags;

} CHANNEL_TX_POWER, *PCHANNEL_TX_POWER;

/* Channel list subset */
typedef struct _CHANNEL_LIST_SUB {
	unsigned char	Channel;
	unsigned char	IdxMap; /* Index mapping to original channel list */
} CHANNEL_LIST_SUB, *PCHANNEL_LIST_SUB;


typedef struct _SOFT_RX_ANT_DIVERSITY_STRUCT {
	unsigned char EvaluatePeriod;	/* 0:not evalute status, 1: evaluate status, 2: switching status */
	unsigned char EvaluateStableCnt;
	unsigned char Pair1PrimaryRxAnt;	/* 0:Ant-E1, 1:Ant-E2 */
	unsigned char Pair1SecondaryRxAnt;	/* 0:Ant-E1, 1:Ant-E2 */
	short Pair1LastAvgRssi;	/* */
	short Pair2LastAvgRssi;	/* */
	unsigned long RcvPktNumWhenEvaluate;
	bool FirstPktArrivedWhenEvaluate;
#ifdef CONFIG_AP_SUPPORT
	long Pair1AvgRssiGroup1[2];
	long Pair1AvgRssiGroup2[2];
	unsigned long RcvPktNum[2];
#endif /* CONFIG_AP_SUPPORT */
} SOFT_RX_ANT_DIVERSITY, *PSOFT_RX_ANT_DIVERSITY;

typedef enum _ABGBAND_STATE_ {
	UNKNOWN_BAND,
	BG_BAND,
	A_BAND,
} ABGBAND_STATE;

/***************************************************************************
  *	structure for MLME state machine
  **************************************************************************/
typedef struct _MLME_STRUCT {
	STATE_MACHINE_FUNC ActFunc[ACT_FUNC_SIZE];
	/* Action */
	STATE_MACHINE ActMachine;

#ifdef WSC_INCLUDED
	STATE_MACHINE WscMachine;
	STATE_MACHINE_FUNC WscFunc[WSC_FUNC_SIZE];

#endif /* WSC_INCLUDED */

#ifdef QOS_DLS_SUPPORT
	STATE_MACHINE DlsMachine;
	STATE_MACHINE_FUNC DlsFunc[DLS_FUNC_SIZE];
#endif /* QOS_DLS_SUPPORT */


#ifdef CONFIG_AP_SUPPORT
	/* AP state machines */
	STATE_MACHINE ApAssocMachine;
	STATE_MACHINE ApAuthMachine;
	STATE_MACHINE ApSyncMachine;
	STATE_MACHINE_FUNC ApAssocFunc[AP_ASSOC_FUNC_SIZE];
/*	STATE_MACHINE_FUNC		ApDlsFunc[DLS_FUNC_SIZE]; */
	STATE_MACHINE_FUNC ApAuthFunc[AP_AUTH_FUNC_SIZE];
	STATE_MACHINE_FUNC ApSyncFunc[AP_SYNC_FUNC_SIZE];
#ifdef APCLI_SUPPORT
	STATE_MACHINE ApCliAuthMachine;
	STATE_MACHINE ApCliAssocMachine;
	STATE_MACHINE ApCliCtrlMachine;
	STATE_MACHINE ApCliSyncMachine;
	STATE_MACHINE ApCliWpaPskMachine;

	STATE_MACHINE_FUNC ApCliAuthFunc[APCLI_AUTH_FUNC_SIZE];
	STATE_MACHINE_FUNC ApCliAssocFunc[APCLI_ASSOC_FUNC_SIZE];
	STATE_MACHINE_FUNC ApCliCtrlFunc[APCLI_CTRL_FUNC_SIZE];
	STATE_MACHINE_FUNC ApCliSyncFunc[APCLI_SYNC_FUNC_SIZE];
#endif /* APCLI_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

	/* common WPA state machine */
	STATE_MACHINE WpaMachine;
	STATE_MACHINE_FUNC WpaFunc[WPA_FUNC_SIZE];


	unsigned long ChannelQuality;	/* 0..100, Channel Quality Indication for Roaming */
	unsigned long Now32;		/* latch the value of NdisGetSystemUpTime() */
	unsigned long LastSendNULLpsmTime;

	bool bRunning;
	NDIS_SPIN_LOCK TaskLock;
	MLME_QUEUE Queue;

	unsigned int ShiftReg;

	RALINK_TIMER_STRUCT PeriodicTimer;
	RALINK_TIMER_STRUCT APSDPeriodicTimer;
	RALINK_TIMER_STRUCT LinkDownTimer;
	RALINK_TIMER_STRUCT LinkUpTimer;
	unsigned long PeriodicRound;
	unsigned long GPIORound;
	unsigned long OneSecPeriodicRound;

	unsigned char RealRxPath;
	bool bLowThroughput;
	bool bEnableAutoAntennaCheck;
	RALINK_TIMER_STRUCT RxAntEvalTimer;


#ifdef RTMP_MAC_USB
	RALINK_TIMER_STRUCT AutoWakeupTimer;
	bool AutoWakeupTimerRunning;
#endif /* RTMP_MAC_USB */


#ifdef CONFIG_MULTI_CHANNEL
	RALINK_TIMER_STRUCT MCCTimer;

	RALINK_TIMER_STRUCT ConcurrentP2PConnectTimer;
	unsigned int HCCAToEDCATimerValue;
	unsigned int EDCAToHCCATimerValue;
	bool ConcurrentP2PConnectTimerRunning;
	unsigned int StaStayTick;
	unsigned int P2pStayTick;
#endif /* CONFIG_MULTI_CHANNEL */

} MLME_STRUCT, *PMLME_STRUCT;

#ifdef DOT11_N_SUPPORT
/***************************************************************************
  *	802.11 N related data structures
  **************************************************************************/
struct reordering_mpdu {
	struct reordering_mpdu *next;
	PNDIS_PACKET pPacket;	/* coverted to 802.3 frame */
	int Sequence;		/* sequence number of MPDU */
	bool bAMSDU;
	unsigned char					OpMode;
};

struct reordering_list {
	struct reordering_mpdu *next;
	int qlen;
};

struct reordering_mpdu_pool {
	void * mem;
	NDIS_SPIN_LOCK lock;
	struct reordering_list freelist;
};

typedef enum _REC_BLOCKACK_STATUS {
	Recipient_NONE = 0,
	Recipient_USED,
	Recipient_HandleRes,
	Recipient_Accept
} REC_BLOCKACK_STATUS, *PREC_BLOCKACK_STATUS;

typedef enum _ORI_BLOCKACK_STATUS {
	Originator_NONE = 0,
	Originator_USED,
	Originator_WaitRes,
	Originator_Done
} ORI_BLOCKACK_STATUS, *PORI_BLOCKACK_STATUS;

typedef struct _BA_ORI_ENTRY {
	unsigned char Wcid;
	unsigned char TID;
	unsigned char BAWinSize;
	unsigned char Token;
/* Sequence is to fill every outgoing QoS DATA frame's sequence field in 802.11 header. */
	unsigned short Sequence;
	unsigned short TimeOutValue;
	ORI_BLOCKACK_STATUS ORI_BA_Status;
	RALINK_TIMER_STRUCT ORIBATimer;
	void * pAdapter;
} BA_ORI_ENTRY, *PBA_ORI_ENTRY;

typedef struct _BA_REC_ENTRY {
	unsigned char Wcid;
	unsigned char TID;
	unsigned char BAWinSize;	/* 7.3.1.14. each buffer is capable of holding a max AMSDU or MSDU. */
	/*unsigned char NumOfRxPkt; */
	/*unsigned char    Curindidx; // the head in the RX reordering buffer */
	unsigned short LastIndSeq;
/*	unsigned short		LastIndSeqAtTimer; */
	unsigned short TimeOutValue;
	RALINK_TIMER_STRUCT RECBATimer;
	unsigned long LastIndSeqAtTimer;
	unsigned long nDropPacket;
	unsigned long rcvSeq;
	REC_BLOCKACK_STATUS REC_BA_Status;
/*	unsigned char	RxBufIdxUsed; */
	/* corresponding virtual address for RX reordering packet storage. */
	/*RTMP_REORDERDMABUF MAP_RXBuf[MAX_RX_REORDERBUF]; */
	NDIS_SPIN_LOCK RxReRingLock;	/* Rx Ring spinlock */
/*	struct _BA_REC_ENTRY *pNext; */
	void * pAdapter;
	struct reordering_list list;
} BA_REC_ENTRY, *PBA_REC_ENTRY;


typedef struct {
	unsigned long numAsRecipient;	/* I am recipient of numAsRecipient clients. These client are in the BARecEntry[] */
	unsigned long numAsOriginator;	/* I am originator of   numAsOriginator clients. These clients are in the BAOriEntry[] */
	unsigned long numDoneOriginator;	/* count Done Originator sessions */
	BA_ORI_ENTRY BAOriEntry[MAX_LEN_OF_BA_ORI_TABLE];
	BA_REC_ENTRY BARecEntry[MAX_LEN_OF_BA_REC_TABLE];
} BA_TABLE, *PBA_TABLE;

/*For QureyBATableOID use; */
typedef struct GNU_PACKED _OID_BA_REC_ENTRY {
	unsigned char MACAddr[MAC_ADDR_LEN];
	unsigned char BaBitmap;		/* if (BaBitmap&(1<<TID)), this session with{MACAddr, TID}exists, so read BufSize[TID] for BufferSize */
	unsigned char rsv;
	unsigned char BufSize[8];
	REC_BLOCKACK_STATUS REC_BA_Status[8];
} OID_BA_REC_ENTRY, *POID_BA_REC_ENTRY;

/*For QureyBATableOID use; */
typedef struct GNU_PACKED _OID_BA_ORI_ENTRY {
	unsigned char MACAddr[MAC_ADDR_LEN];
	unsigned char BaBitmap;		/* if (BaBitmap&(1<<TID)), this session with{MACAddr, TID}exists, so read BufSize[TID] for BufferSize, read ORI_BA_Status[TID] for status */
	unsigned char rsv;
	unsigned char BufSize[8];
	ORI_BLOCKACK_STATUS ORI_BA_Status[8];
} OID_BA_ORI_ENTRY, *POID_BA_ORI_ENTRY;

typedef struct _QUERYBA_TABLE {
	OID_BA_ORI_ENTRY BAOriEntry[32];
	OID_BA_REC_ENTRY BARecEntry[32];
	unsigned char OriNum;		/* Number of below BAOriEntry */
	unsigned char RecNum;		/* Number of below BARecEntry */
} QUERYBA_TABLE, *PQUERYBA_TABLE;

typedef union _BACAP_STRUC {
#ifdef RT_BIG_ENDIAN
	struct {
		unsigned int:4;
		unsigned int b2040CoexistScanSup:1;	/*As Sta, support do 2040 coexistence scan for AP. As Ap, support monitor trigger event to check if can use BW 40MHz. */
		unsigned int bHtAdhoc:1;	/* adhoc can use ht rate. */
		unsigned int MMPSmode:2;	/* MIMO power save more, 0:static, 1:dynamic, 2:rsv, 3:mimo enable */
		unsigned int AmsduSize:1;	/* 0:3839, 1:7935 bytes. unsigned int  MSDUSizeToBytes[]        = { 3839, 7935}; */
		unsigned int AmsduEnable:1;	/*Enable AMSDU transmisstion */
		unsigned int MpduDensity:3;
		unsigned int Policy:2;	/* 0: DELAY_BA 1:IMMED_BA  (//BA Policy subfiled value in ADDBA frame)   2:BA-not use */
		unsigned int AutoBA:1;	/* automatically BA */
		unsigned int TxBAWinLimit:8;
		unsigned int RxBAWinLimit:8;
	} field;
#else
	struct {
		unsigned int RxBAWinLimit:8;
		unsigned int TxBAWinLimit:8;
		unsigned int AutoBA:1;	/* automatically BA */
		unsigned int Policy:2;	/* 0: DELAY_BA 1:IMMED_BA  (//BA Policy subfiled value in ADDBA frame)   2:BA-not use */
		unsigned int MpduDensity:3;
		unsigned int AmsduEnable:1;	/*Enable AMSDU transmisstion */
		unsigned int AmsduSize:1;	/* 0:3839, 1:7935 bytes. unsigned int  MSDUSizeToBytes[]        = { 3839, 7935}; */
		unsigned int MMPSmode:2;	/* MIMO power save more, 0:static, 1:dynamic, 2:rsv, 3:mimo enable */
		unsigned int bHtAdhoc:1;	/* adhoc can use ht rate. */
		unsigned int b2040CoexistScanSup:1;	/*As Sta, support do 2040 coexistence scan for AP. As Ap, support monitor trigger event to check if can use BW 40MHz. */
		 unsigned int:4;
	} field;
#endif
	unsigned int word;
} BACAP_STRUC, *PBACAP_STRUC;

typedef struct {
	bool IsRecipient;
	unsigned char MACAddr[MAC_ADDR_LEN];
	unsigned char TID;
	unsigned char nMSDU;
	unsigned short TimeOut;
	bool bAllTid;	/* If True, delete all TID for BA sessions with this MACaddr. */
} OID_ADD_BA_ENTRY, *POID_ADD_BA_ENTRY;

#ifdef DOT11N_DRAFT3
typedef enum _BSS2040COEXIST_FLAG {
	BSS_2040_COEXIST_DISABLE = 0,
	BSS_2040_COEXIST_TIMER_FIRED = 1,
	BSS_2040_COEXIST_INFO_SYNC = 2,
	BSS_2040_COEXIST_INFO_NOTIFY = 4,
} BSS2040COEXIST_FLAG;

typedef struct _BssCoexChRange_ {
	unsigned char primaryCh;
	unsigned char secondaryCh;
	unsigned char effectChStart;
	unsigned char effectChEnd;
} BSS_COEX_CH_RANGE;
#endif /* DOT11N_DRAFT3 */

#define IS_HT_STA(_pMacEntry)	\
	(_pMacEntry->MaxHTPhyMode.field.MODE >= MODE_HTMIX)

#define IS_HT_RATE(_pMacEntry)	\
	(_pMacEntry->HTPhyMode.field.MODE >= MODE_HTMIX)

#define PEER_IS_HT_RATE(_pMacEntry)	\
	(_pMacEntry->HTPhyMode.field.MODE >= MODE_HTMIX)

#endif /* DOT11_N_SUPPORT */

/*This structure is for all 802.11n card InterOptibilityTest action. Reset all Num every n second.  (Details see MLMEPeriodic) */
typedef struct _IOT_STRUC {
	bool bRTSLongProtOn;
} IOT_STRUC, *PIOT_STRUC;

/* This is the registry setting for 802.11n transmit setting.  Used in advanced page. */
typedef union _REG_TRANSMIT_SETTING {
#ifdef RT_BIG_ENDIAN
	struct {
		unsigned int rsv:13;
		unsigned int EXTCHA:2;
		unsigned int HTMODE:1;
		unsigned int TRANSNO:2;
		unsigned int STBC:1;	/*SPACE */
		unsigned int ShortGI:1;
		unsigned int BW:1;	/*channel bandwidth 20MHz or 40 MHz */
		unsigned int TxBF:1;	/* 3*3 */
		unsigned int ITxBfEn:1;
		unsigned int rsv0:9;
		/*unsigned int  MCS:7;                 // MCS */
		/*unsigned int  PhyMode:4; */
	} field;
#else
	struct {
		/*unsigned int  PhyMode:4; */
		/*unsigned int  MCS:7;                 // MCS */
		unsigned int rsv0:9;
		unsigned int ITxBfEn:1;
		unsigned int TxBF:1;
		unsigned int BW:1;	/*channel bandwidth 20MHz or 40 MHz */
		unsigned int ShortGI:1;
		unsigned int STBC:1;	/*SPACE */
		unsigned int TRANSNO:2;
		unsigned int HTMODE:1;
		unsigned int EXTCHA:2;
		unsigned int rsv:13;
	} field;
#endif
	unsigned int word;
} REG_TRANSMIT_SETTING, *PREG_TRANSMIT_SETTING;


typedef union _DESIRED_TRANSMIT_SETTING {
#ifdef RT_BIG_ENDIAN
	struct {
		unsigned short rsv:3;
		unsigned short FixedTxMode:2;	/* If MCS isn't AUTO, fix rate in CCK, OFDM or HT mode. */
		unsigned short PhyMode:4;
		unsigned short MCS:7;	/* MCS */
	} field;
#else
	struct {
		unsigned short MCS:7;	/* MCS */
		unsigned short PhyMode:4;
		unsigned short FixedTxMode:2;	/* If MCS isn't AUTO, fix rate in CCK, OFDM or HT mode. */
		unsigned short rsv:3;
	} field;
#endif
	unsigned short word;
} DESIRED_TRANSMIT_SETTING, *PDESIRED_TRANSMIT_SETTING;


struct hw_setting{
	unsigned char prim_ch;
	unsigned char cent_ch;
	unsigned char bbp_bw;
	unsigned char rf_band;
};


struct wifi_dev{
	NDIS_802_11_AUTHENTICATION_MODE AuthMode;
	NDIS_802_11_WEP_STATUS WepStatus;
	NDIS_802_11_WEP_STATUS GroupKeyWepStatus;
	WPA_MIX_PAIR_CIPHER WpaMixPairCipher;
	
	RT_PHY_INFO DesiredHtPhyInfo;
	DESIRED_TRANSMIT_SETTING DesiredTransmitSetting;	/* Desired transmit setting. this is for reading registry setting only. not useful. */
	bool bAutoTxRateSwitch;

	bool bWmmCapable;	/* 0:disable WMM, 1:enable WMM */
	
	HTTRANSMIT_SETTING HTPhyMode, MaxHTPhyMode, MinHTPhyMode;
};


#ifdef RTMP_MAC_USB
/***************************************************************************
  *	USB-based chip Beacon related data structures
  **************************************************************************/
#define BEACON_BITMAP_MASK		0xff
typedef struct _BEACON_SYNC_STRUCT_ {
	unsigned char BeaconBuf[HW_BEACON_MAX_NUM][HW_BEACON_OFFSET];
	unsigned char *BeaconTxWI[HW_BEACON_MAX_NUM];
	unsigned long TimIELocationInBeacon[HW_BEACON_MAX_NUM];
	unsigned long CapabilityInfoLocationInBeacon[HW_BEACON_MAX_NUM];
	bool EnableBeacon;	/* trigger to enable beacon transmission. */
	unsigned char BeaconBitMap;	/* NOTE: If the MAX_MBSSID_NUM is larger than 8, this parameter need to change. */
	unsigned char DtimBitOn;	/* NOTE: If the MAX_MBSSID_NUM is larger than 8, this parameter need to change. */
} BEACON_SYNC_STRUCT;
#endif /* RTMP_MAC_USB */

/***************************************************************************
  *	Multiple SSID related data structures
  **************************************************************************/
#define WLAN_MAX_NUM_OF_TIM			((MAX_LEN_OF_MAC_TABLE >> 3) + 1)	/* /8 + 1 */
#define WLAN_CT_TIM_BCMC_OFFSET		0	/* unit: 32B */

/* clear bcmc TIM bit */
#define WLAN_MR_TIM_BCMC_CLEAR(apidx) \
	pAd->ApCfg.MBSSID[apidx].TimBitmaps[WLAN_CT_TIM_BCMC_OFFSET] &= ~NUM_BIT8[0];

/* set bcmc TIM bit */
#define WLAN_MR_TIM_BCMC_SET(apidx) \
	pAd->ApCfg.MBSSID[apidx].TimBitmaps[WLAN_CT_TIM_BCMC_OFFSET] |= NUM_BIT8[0];

/* clear a station PS TIM bit */
#define WLAN_MR_TIM_BIT_CLEAR(ad_p, apidx, wcid) \
	{	unsigned char tim_offset = wcid >> 3; \
		unsigned char bit_offset = wcid & 0x7; \
		ad_p->ApCfg.MBSSID[apidx].TimBitmaps[tim_offset] &= (~NUM_BIT8[bit_offset]); }

/* set a station PS TIM bit */
#define WLAN_MR_TIM_BIT_SET(ad_p, apidx, wcid) \
	{	unsigned char tim_offset = wcid >> 3; \
		unsigned char bit_offset = wcid & 0x7; \
		ad_p->ApCfg.MBSSID[apidx].TimBitmaps[tim_offset] |= NUM_BIT8[bit_offset]; }


#ifdef CONFIG_AP_SUPPORT
typedef struct _MULTISSID_STRUCT {

	struct wifi_dev wdev;

#ifdef HOSTAPD_SUPPORT
	bool Hostapd;
	bool HostapdWPS;
#endif

	unsigned char Bssid[MAC_ADDR_LEN];
	unsigned char SsidLen;
	char Ssid[MAX_LEN_OF_SSID];
	unsigned short CapabilityInfo;

	unsigned char MaxStaNum;	/* Limit the STA connection number per BSS */
	unsigned char StaCount;

	PNET_DEV MSSIDDev;

	NDIS_802_11_AUTHENTICATION_MODE AuthMode;
	NDIS_802_11_WEP_STATUS WepStatus;
	NDIS_802_11_WEP_STATUS GroupKeyWepStatus;
	WPA_MIX_PAIR_CIPHER WpaMixPairCipher;

	HTTRANSMIT_SETTING HTPhyMode, MaxHTPhyMode, MinHTPhyMode;	/* For transmit phy setting in TXWI. */
	RT_PHY_INFO DesiredHtPhyInfo;
	DESIRED_TRANSMIT_SETTING DesiredTransmitSetting;	/* Desired transmit setting. this is for reading registry setting only. not useful. */
	bool bAutoTxRateSwitch;

	/*MBSS_STATISTICS MbssStat;*/
	unsigned long TxCount;
	unsigned long RxCount;
	unsigned long ReceivedByteCount;
	unsigned long TransmittedByteCount;
	unsigned long RxErrorCount;
	unsigned long RxDropCount;

	unsigned long TxErrorCount;
	unsigned long TxDropCount;
	unsigned long ucPktsTx;
	unsigned long ucPktsRx;
	unsigned long mcPktsTx;
	unsigned long mcPktsRx;
	unsigned long bcPktsTx;
	unsigned long bcPktsRx;

	/*CIPHER_KEY SharedKey[SHARE_KEY_NUM]; // ref pAd->SharedKey[BSS][4] */
	unsigned char DefaultKeyId;

	unsigned char TxRate; /* RATE_1, RATE_2, RATE_5_5, RATE_11, ... */
	unsigned char DesiredRates[MAX_LEN_OF_SUPPORTED_RATES];	/* OID_802_11_DESIRED_RATES */
	unsigned char DesiredRatesIndex;
	unsigned char MaxTxRate; /* RATE_1, RATE_2, RATE_5_5, RATE_11 */

/*	unsigned long TimBitmap;      // bit0 for broadcast, 1 for AID1, 2 for AID2, ...so on */
/*    unsigned long TimBitmap2;     // b0 for AID32, b1 for AID33, ... and so on */
	unsigned char TimBitmaps[WLAN_MAX_NUM_OF_TIM];

	/* WPA */
	unsigned char GMK[32];
	unsigned char PMK[32];
	unsigned char GTK[32];
#ifdef DOT1X_SUPPORT
	bool IEEE8021X;
	bool PreAuth;
#endif /* DOT1X_SUPPORT */
	unsigned char GNonce[32];
	unsigned char PortSecured;
	NDIS_802_11_PRIVACY_FILTER PrivacyFilter;
	unsigned char BANClass3Data;
	unsigned long IsolateInterStaTraffic;

	unsigned char RSNIE_Len[2];
	unsigned char RSN_IE[2][MAX_LEN_OF_RSNIE];

	/* for Group Rekey */
	RT_WPA_REKEY WPAREKEY;
	unsigned long REKEYCOUNTER;
	RALINK_TIMER_STRUCT REKEYTimer;
	unsigned char REKEYTimerRunning;
	unsigned char RekeyCountDown;

#ifdef WAPI_SUPPORT
	unsigned char WAPIPassPhrase[64];	/* WAPI PSK pass phrase */
	unsigned int WAPIPassPhraseLen;	/* the length of WAPI PSK pass phrase */
	unsigned int WapiPskType;	/* 0 - Hex, 1 - ASCII */
	unsigned char WAPI_BK[16];	/* WAPI base key */

	unsigned char NMK[LEN_WAPI_NMK];
	unsigned char key_announce_flag[LEN_WAPI_TSC];
	bool sw_wpi_encrypt;	/* WPI data encrypt by SW */
#endif /* WAPI_SUPPORT */

	unsigned char TimIELocationInBeacon;
	unsigned char CapabilityInfoLocationInBeacon;

	/* For PMK Cache using */
	unsigned long PMKCachePeriod;	/* unit : jiffies */
	NDIS_AP_802_11_PMKID PMKIDCache;

	/* outgoing BEACON frame buffer and corresponding TXWI */
	/* TXWI_STRUC *BeaconTxWI; */
	char BeaconBuf[MAX_BEACON_SIZE];	/* NOTE: BeaconBuf should be 4-byte aligned */

	bool bHideSsid;
	unsigned short StationKeepAliveTime;	/* unit: second */

	/* VLAN related */
	bool bVLAN_Tag;
	unsigned short VLAN_VID;
	unsigned short VLAN_Priority;

	RT_802_11_ACL AccessControlList;

	/* EDCA Qos */
	bool bWmmCapable;	/* 0:disable WMM, 1:enable WMM */
	bool bDLSCapable;	/* 0:disable DLS, 1:enable DLS */

	/*
	   Why need the parameter: 2009/09/22

	   1. iwpriv ra0 set WmmCapable=0
	   2. iwpriv ra0 set WirelessMode=9
	   3. iwpriv ra0 set WirelessMode=0
	   4. iwpriv ra0 set SSID=SampleAP

	   After the 4 commands, WMM is still enabled.
	   So we need the parameter to recover WMM Capable flag.

	   No the problem in station mode.
	 */
	bool bWmmCapableOrg;	/* origin Wmm Capable in non-11n mode */

#ifdef QOS_DLS_SUPPORT
	unsigned char DlsPTK[64];	/* Due to windows dirver count on meetinghouse to handle 4-way shake */
#endif /* QOS_DLS_SUPPORT */

#ifdef DOT1X_SUPPORT
	/* For 802.1x daemon setting per BSS */
	unsigned char radius_srv_num;
	RADIUS_SRV_INFO radius_srv_info[MAX_RADIUS_SRV_NUM];
	unsigned char NasId[IFNAMSIZ];
	unsigned char NasIdLen;
#endif /* DOT1X_SUPPORT */

#ifdef RTL865X_SOC
	unsigned int mylinkid;
#endif

#ifdef CONFIG_AP_SUPPORT
	WSC_LV_INFO WscIEBeacon;
	WSC_LV_INFO WscIEProbeResp;
#ifdef WSC_AP_SUPPORT
	WSC_CTRL WscControl;
	WSC_SECURITY_MODE WscSecurityMode;
#endif /* WSC_AP_SUPPORT */

#endif /* CONFIG_AP_SUPPORT */

#ifdef IDS_SUPPORT
	unsigned int RcvdConflictSsidCount;
	unsigned int RcvdSpoofedAssocRespCount;
	unsigned int RcvdSpoofedReassocRespCount;
	unsigned int RcvdSpoofedProbeRespCount;
	unsigned int RcvdSpoofedBeaconCount;
	unsigned int RcvdSpoofedDisassocCount;
	unsigned int RcvdSpoofedAuthCount;
	unsigned int RcvdSpoofedDeauthCount;
	unsigned int RcvdSpoofedUnknownMgmtCount;
	unsigned int RcvdReplayAttackCount;

	char RssiOfRcvdConflictSsid;
	char RssiOfRcvdSpoofedAssocResp;
	char RssiOfRcvdSpoofedReassocResp;
	char RssiOfRcvdSpoofedProbeResp;
	char RssiOfRcvdSpoofedBeacon;
	char RssiOfRcvdSpoofedDisassoc;
	char RssiOfRcvdSpoofedAuth;
	char RssiOfRcvdSpoofedDeauth;
	char RssiOfRcvdSpoofedUnknownMgmt;
	char RssiOfRcvdReplayAttack;
#endif /* IDS_SUPPORT */

	/* used in if beacon send or stop */
	bool bBcnSntReq;
	unsigned char BcnBufIdx;





	unsigned char PhyMode;


	/* UAPSD information: such as enable or disable, do not remove */
	UAPSD_INFO UapsdInfo;

} MULTISSID_STRUCT, *PMULTISSID_STRUCT;
#endif /* CONFIG_AP_SUPPORT */


/* configuration common to OPMODE_AP as well as OPMODE_STA */
typedef struct _COMMON_CONFIG {
	bool bCountryFlag;
	unsigned char CountryCode[3];
#ifdef EXT_BUILD_CHANNEL_LIST
	unsigned char Geography;
	unsigned char DfsType;
	unsigned char * pChDesp;
#endif				/* EXT_BUILD_CHANNEL_LIST */
	unsigned char CountryRegion;	/* Enum of country region, 0:FCC, 1:IC, 2:ETSI, 3:SPAIN, 4:France, 5:MKK, 6:MKK1, 7:Israel */
	unsigned char CountryRegionForABand;	/* Enum of country region for A band */
	unsigned char PhyMode;
	unsigned char cfg_wmode;
	unsigned char SavedPhyMode;
	unsigned short Dsifs;		/* in units of usec */
	unsigned long PacketFilter;	/* Packet filter for receiving */
	unsigned char RegulatoryClass[MAX_NUM_OF_REGULATORY_CLASS];

	char Ssid[MAX_LEN_OF_SSID];	/* NOT NULL-terminated */
	unsigned char SsidLen;		/* the actual ssid length in used */
	unsigned char LastSsidLen;	/* the actual ssid length in used */
	char LastSsid[MAX_LEN_OF_SSID];	/* NOT NULL-terminated */
	unsigned char LastBssid[MAC_ADDR_LEN];

	unsigned char Bssid[MAC_ADDR_LEN];
	unsigned short BeaconPeriod;
	unsigned char Channel;
	unsigned char CentralChannel;	/* Central Channel when using 40MHz is indicating. not real channel. */

	unsigned char SupRate[MAX_LEN_OF_SUPPORTED_RATES];
	unsigned char SupRateLen;
	unsigned char ExtRate[MAX_LEN_OF_SUPPORTED_RATES];
	unsigned char ExtRateLen;
	unsigned char DesireRate[MAX_LEN_OF_SUPPORTED_RATES];	/* OID_802_11_DESIRED_RATES */
	unsigned char MaxDesiredRate;
	unsigned char ExpectedACKRate[MAX_LEN_OF_SUPPORTED_RATES];

	unsigned long BasicRateBitmap;	/* backup basic ratebitmap */
	unsigned long BasicRateBitmapOld;	/* backup basic ratebitmap */

	bool bInServicePeriod;


	bool bAPSDAC_BE;
	bool bAPSDAC_BK;
	bool bAPSDAC_VI;
	bool bAPSDAC_VO;


	/* because TSPEC can modify the APSD flag, we need to keep the APSD flag
	   requested in association stage from the station;
	   we need to recover the APSD flag after the TSPEC is deleted. */
	bool bACMAPSDBackup[4];	/* for delivery-enabled & trigger-enabled both */
	bool bACMAPSDTr[4];	/* no use */
	unsigned char MaxSPLength;

	bool bNeedSendTriggerFrame;
	bool bAPSDForcePowerSave;	/* Force power save mode, should only use in APSD-STAUT */
	unsigned long TriggerTimerCount;
	unsigned char BBPCurrentBW;	/* BW_10, BW_20, BW_40, BW_80 */
	REG_TRANSMIT_SETTING RegTransmitSetting;	/*registry transmit setting. this is for reading registry setting only. not useful. */
	unsigned char TxRate;		/* Same value to fill in TXD. TxRate is 6-bit */
	unsigned char MaxTxRate;	/* RATE_1, RATE_2, RATE_5_5, RATE_11 */
	unsigned char TxRateIndex;	/* Tx rate index in RateSwitchTable */
	unsigned char MinTxRate;	/* RATE_1, RATE_2, RATE_5_5, RATE_11 */
	unsigned char RtsRate;		/* RATE_xxx */
	HTTRANSMIT_SETTING MlmeTransmit;	/* MGMT frame PHY rate setting when operatin at Ht rate. */
	unsigned char MlmeRate;		/* RATE_xxx, used to send MLME frames */
	unsigned char BasicMlmeRate;	/* Default Rate for sending MLME frames */

	unsigned short RtsThreshold;	/* in unit of BYTE */
	unsigned short FragmentThreshold;	/* in unit of BYTE */

	unsigned char TxPower;		/* in unit of mW */
	unsigned long TxPowerPercentage;	/* 0~100 % */
	unsigned long TxPowerDefault;	/* keep for TxPowerPercentage */
	unsigned char PwrConstraint;

#ifdef DOT11_N_SUPPORT
	BACAP_STRUC BACapability;	/*   NO USE = 0XFF  ;  IMMED_BA =1  ;  DELAY_BA=0 */
	BACAP_STRUC REGBACapability;	/*   NO USE = 0XFF  ;  IMMED_BA =1  ;  DELAY_BA=0 */
#endif				/* DOT11_N_SUPPORT */

#ifdef DOT11_VHT_AC
	bool force_vht;
	unsigned char vht_bw;
	unsigned char vht_cent_ch;
	unsigned char vht_cent_ch2;
#endif /* DOT11_VHT_AC */

	IOT_STRUC IOTestParm;	/* 802.11n InterOpbility Test Parameter; */
	unsigned long TxPreamble;	/* Rt802_11PreambleLong, Rt802_11PreambleShort, Rt802_11PreambleAuto */
	bool bUseZeroToDisableFragment;	/* Microsoft use 0 as disable */
	unsigned long UseBGProtection;	/* 0: auto, 1: always use, 2: always not use */
	bool bUseShortSlotTime;	/* 0: disable, 1 - use short slot (9us) */
	bool bEnableTxBurst;	/* 1: enble TX PACKET BURST (when BA is established or AP is not a legacy WMM AP), 0: disable TX PACKET BURST */
	bool bAggregationCapable;	/* 1: enable TX aggregation when the peer supports it */
	bool bPiggyBackCapable;	/* 1: enable TX piggy-back according MAC's version */
	bool bIEEE80211H;	/* 1: enable IEEE802.11h spec. */
	unsigned char RDDurRegion; /* Region of radar detection */
	unsigned long DisableOLBCDetect;	/* 0: enable OLBC detect; 1 disable OLBC detect */

#ifdef DOT11_N_SUPPORT
	bool bRdg;
#endif				/* DOT11_N_SUPPORT */
	bool bWmmCapable;	/* 0:disable WMM, 1:enable WMM */
	QOS_CAPABILITY_PARM APQosCapability;	/* QOS capability of the current associated AP */
	EDCA_PARM APEdcaParm;	/* EDCA parameters of the current associated AP */
	QBSS_LOAD_PARM APQbssLoad;	/* QBSS load of the current associated AP */
	unsigned char AckPolicy[4];	/* ACK policy of the specified AC. see ACK_xxx */
	/* a bitmap of bool flags. each bit represent an operation status of a particular */
	/* bool control, either ON or OFF. These flags should always be accessed via */
	/* OPSTATUS_TEST_FLAG(), OPSTATUS_SET_FLAG(), OP_STATUS_CLEAR_FLAG() macros. */
	/* see fOP_STATUS_xxx in RTMP_DEF.C for detail bit definition */
	unsigned long OpStatusFlags;

	bool NdisRadioStateOff;	/*For HCT 12.0, set this flag to TRUE instead of called MlmeRadioOff. */

#ifdef DFS_SUPPORT
	/* IEEE802.11H--DFS. */
	RADAR_DETECT_STRUCT RadarDetect;
#endif /* DFS_SUPPORT */
#ifdef CARRIER_DETECTION_SUPPORT
	CARRIER_DETECTION_STRUCT CarrierDetect;
#endif /* CARRIER_DETECTION_SUPPORT */

#ifdef DOT11_N_SUPPORT
	/* HT */
	RT_HT_CAPABILITY DesiredHtPhy;
	HT_CAPABILITY_IE HtCapability;
	ADD_HT_INFO_IE AddHTInfo;	/* Useful as AP. */
	/*This IE is used with channel switch announcement element when changing to a new 40MHz. */
	/*This IE is included in channel switch ammouncement frames 7.4.1.5, beacons, probe Rsp. */
	NEW_EXT_CHAN_IE NewExtChanOffset;	/*7.3.2.20A, 1 if extension channel is above the control channel, 3 if below, 0 if not present */

	EXT_CAP_INFO_ELEMENT ExtCapIE;	/* this is the extened capibility IE appreed in MGMT frames. Doesn't need to update once set in Init. */

#ifdef DOT11N_DRAFT3
	bool bBssCoexEnable;
	/* 
	   Following two paramters now only used for the initial scan operation. the AP only do 
	   bandwidth fallback when BssCoexApCnt > BssCoexApCntThr
	   By default, the "BssCoexApCntThr" is set as 0 in "UserCfgInit()".
	 */
	unsigned char BssCoexApCntThr;
	unsigned char BssCoexApCnt;

	unsigned char Bss2040CoexistFlag;	/* bit 0: bBssCoexistTimerRunning, bit 1: NeedSyncAddHtInfo. */
	RALINK_TIMER_STRUCT Bss2040CoexistTimer;
	unsigned char					Bss2040NeedFallBack; 	/* 1: Need Fall back to 20MHz */

	/*This IE is used for 20/40 BSS Coexistence. */
	BSS_2040_COEXIST_IE BSS2040CoexistInfo;

	unsigned short Dot11OBssScanPassiveDwell;	/* Unit : TU. 5~1000 */
	unsigned short Dot11OBssScanActiveDwell;	/* Unit : TU. 10~1000 */
	unsigned short Dot11BssWidthTriggerScanInt;	/* Unit : Second */
	unsigned short Dot11OBssScanPassiveTotalPerChannel;	/* Unit : TU. 200~10000 */
	unsigned short Dot11OBssScanActiveTotalPerChannel;	/* Unit : TU. 20~10000 */
	unsigned short Dot11BssWidthChanTranDelayFactor;
	unsigned short Dot11OBssScanActivityThre;	/* Unit : percentage */

	unsigned long Dot11BssWidthChanTranDelay;	/* multiple of (Dot11BssWidthTriggerScanInt * Dot11BssWidthChanTranDelayFactor) */
	unsigned long CountDownCtr;	/* CountDown Counter from (Dot11BssWidthTriggerScanInt * Dot11BssWidthChanTranDelayFactor) */

	BSS_2040_COEXIST_IE LastBSSCoexist2040;
	BSS_2040_COEXIST_IE BSSCoexist2040;
	TRIGGER_EVENT_TAB TriggerEventTab;
	unsigned char ChannelListIdx;

	bool bOverlapScanning;
	bool bBssCoexNotify;
#endif /* DOT11N_DRAFT3 */

	bool bHTProtect;
	bool bMIMOPSEnable;
	bool bBADecline;
	bool bDisableReordering;
	bool bForty_Mhz_Intolerant;
	bool bExtChannelSwitchAnnouncement;
	bool bRcvBSSWidthTriggerEvents;
	unsigned long LastRcvBSSWidthTriggerEventsTime;

	unsigned char TxBASize;

	bool bRalinkBurstMode;
	unsigned int RestoreBurstMode;
#endif /* DOT11_N_SUPPORT */

#ifdef DOT11_VHT_AC
	unsigned int cfg_vht;
	VHT_CAP_INFO vht_info;
#endif /* DOT11_VHT_AC */

#ifdef SYSTEM_LOG_SUPPORT
	/* Enable wireless event */
	bool bWirelessEvent;
#endif /* SYSTEM_LOG_SUPPORT */

	bool bWiFiTest;	/* Enable this parameter for WiFi test */

	/* Tx & Rx Stream number selection */
	unsigned char TxStream;
	unsigned char RxStream;

	/* transmit phy mode, trasmit rate for Multicast. */
#ifdef MCAST_RATE_SPECIFIC
	unsigned char McastTransmitMcs;
	unsigned char McastTransmitPhyMode;
#endif /* MCAST_RATE_SPECIFIC */

	bool bHardwareRadio;	/* Hardware controlled Radio enabled */

#ifdef RTMP_MAC_USB
	bool bMultipleIRP;	/* Multiple Bulk IN flag */
	unsigned char NumOfBulkInIRP;	/* if bMultipleIRP == TRUE, NumOfBulkInIRP will be 4 otherwise be 1 */
	RT_HT_CAPABILITY SupportedHtPhy;
	unsigned long MaxPktOneTxBulk;
	unsigned char TxBulkFactor;
	unsigned char RxBulkFactor;

	bool IsUpdateBeacon;
	BEACON_SYNC_STRUCT *pBeaconSync;
	RALINK_TIMER_STRUCT BeaconUpdateTimer;
	unsigned int BeaconAdjust;
	unsigned int BeaconFactor;
	unsigned int BeaconRemain;
#endif				/* RTMP_MAC_USB */

#ifdef WSC_INCLUDED
	/* WSC hardware push button function 0811 */
	unsigned char WscHdrPshBtnCheckCount;
#endif				/* WSC_INCLUDED */


	NDIS_SPIN_LOCK MeasureReqTabLock;
	PMEASURE_REQ_TAB pMeasureReqTab;

	NDIS_SPIN_LOCK TpcReqTabLock;
	PTPC_REQ_TAB pTpcReqTab;

	/* transmit phy mode, trasmit rate for Multicast. */
#ifdef MCAST_RATE_SPECIFIC
	HTTRANSMIT_SETTING MCastPhyMode;
#endif /* MCAST_RATE_SPECIFIC */

#ifdef SINGLE_SKU
	unsigned short DefineMaxTxPwr;
	bool bSKUMode;
	unsigned short AntGain;
	unsigned short BandedgeDelta;
	unsigned short ModuleTxpower;
#endif /* SINGLE_SKU */

#ifdef WAPI_SUPPORT
	COMMON_WAPI_INFO comm_wapi_info;

	/* rekey related parameter */
	/* USK update parameter */
	unsigned char wapi_usk_rekey_method;	/* 0:disable , 1:time, 2:packet */
	unsigned int wapi_usk_rekey_threshold;	/* the rekey threshold */

	/* MSK update parameter */
	unsigned char wapi_msk_rekey_method;	/* 0:disable , 1:time, 2:packet */
	unsigned int wapi_msk_rekey_threshold;	/* the rekey threshold */

	unsigned int wapi_msk_rekey_cnt;
	RALINK_TIMER_STRUCT WapiMskRekeyTimer;
	unsigned char WapiMskRekeyTimerRunning;
#endif /* WAPI_SUPPORT */


	bool HT_DisallowTKIP;	/* Restrict the encryption type in 11n HT mode */

	bool HT_Disable;	/* 1: disable HT function; 0: enable HT function */


#ifdef PRE_ANT_SWITCH
	bool PreAntSwitch;	/* Preamble Antenna Switch */
	short PreAntSwitchRSSI;	/* Preamble Antenna Switch RSSI threshold */
	short PreAntSwitchTimeout; /* Preamble Antenna Switch timeout in seconds */
#endif /* PRE_ANT_SWITCH */

#ifdef CFO_TRACK
	short	CFOTrack;	/* CFO Tracking. 0=>use default, 1=>track, 2-7=> track 8-n times, 8=>done tracking */
#endif /* CFO_TRACK */

#ifdef NEW_RATE_ADAPT_SUPPORT
	unsigned short	lowTrafficThrd;		/* Threshold for reverting to default MCS when traffic is low */
	unsigned char	TrainUpRule;		/* QuickDRS train up criterion: 0=>Throughput, 1=>PER, 2=> Throughput & PER */
	short	TrainUpRuleRSSI;	/* If TrainUpRule=2 then use Hybrid rule when RSSI < TrainUpRuleRSSI */
	unsigned short	TrainUpLowThrd;		/* QuickDRS Hybrid train up low threshold */
	unsigned short	TrainUpHighThrd;	/* QuickDRS Hybrid train up high threshold */
#endif /* NEW_RATE_ADAPT_SUPPORT */

#ifdef STREAM_MODE_SUPPORT
#define		STREAM_MODE_STA_NUM		4

	unsigned char	StreamMode; /* 0=disabled, 1=enable for 1SS, 2=enable for 2SS, 3=enable for 1,2SS */
	unsigned char	StreamModeMac[STREAM_MODE_STA_NUM][MAC_ADDR_LEN];
	unsigned short	StreamModeMCS;	/* Bit map for enabling Stream Mode based on MCS */
#endif /* STREAM_MODE_SUPPORT */

#ifdef DOT11_N_SUPPORT
#ifdef TXBF_SUPPORT
	unsigned long ITxBfTimeout;
	unsigned long ETxBfTimeout;
	unsigned long	ETxBfEnCond;		/* Enable sending of sounding and beamforming */
	bool	ETxBfNoncompress;	/* Force non-compressed Sounding Response */
	bool	ETxBfIncapable;		/* Report Incapable of BF in TX BF Capabilities */
#endif /* TXBF_SUPPORT */
#endif /* DOT11_N_SUPPORT */

#ifdef DBG_CTRL_SUPPORT
	unsigned long DebugFlags;	/* Temporary debug flags */
#endif /* DBG_CTRL_SUPPORT */


#ifdef WSC_INCLUDED
	bool WscPBCOverlap;
	WSC_STA_PBC_PROBE_INFO WscStaPbcProbeInfo;
#endif				/* WSC_INCLUDED */


#ifdef MICROWAVE_OVEN_SUPPORT
	MO_CFG_STRUCT MO_Cfg;	/* data structure for mitigating microwave interference */
#endif /* MICROWAVE_OVEN_SUPPORT */

	
} COMMON_CONFIG, *PCOMMON_CONFIG;

#ifdef DBG_CTRL_SUPPORT
/* DebugFlag definitions */
#define DBF_NO_BF_AWARE_RA		0x0001	/* Revert to older Rate Adaptation that is not BF aware */
#define DBF_SHOW_BF_STATS		0x0002	/* Display BF statistics in AP "iwpriv stat" display */
#define DBF_NO_TXBF_3SS			0x0004	/* Disable TXBF for MCS > 20 */
#define DBF_UNUSED0008			0x0008	/* Unused */
#define DBF_DBQ_RA_LOG			0x0010	/* Log RA information in DBQ */
#define DBF_INIT_MCS_MARGIN		0x0020	/* Use 6 dB margin when selecting initial MCS */
#define DBF_INIT_MCS_DIS1		0x0040	/* Disable highest MCSs when selecting initial MCS */
#define DBF_FORCE_QUICK_DRS		0x0080	/* Force Quick DRS even if rate didn't change */
#define DBF_FORCE_SGI			0x0100	/* Force Short GI */
#define DBF_DBQ_NO_BCN			0x0200	/* Disable logging of RX Beacon frames */
#define DBF_LOG_VCO_CAL			0x0400	/* Log VCO cal */
#define DBF_DISABLE_CAL			0x0800	/* Disable Divider Calibration at channel change */
#ifdef INCLUDE_DEBUG_QUEUE
#define DBF_DBQ_TXFIFO			0x1000	/* Enable logging of TX information from FIFO */
#define DBF_DBQ_TXFRAME			0x2000	/* Enable logging of Frames queued for TX */
#define DBF_DBQ_RXWI_FULL		0x4000	/* Enable logging of full RXWI */
#define DBF_DBQ_RXWI			0x8000	/* Enable logging of partial RXWI */
#endif /* INCLUDE_DEBUG_QUEUE */

#define DBF_SHOW_RA_LOG			0x010000	/* Display concise Rate Adaptation information */
#define DBF_SHOW_ZERO_RA_LOG	0x020000	/* Include RA Log entries when TxCount is 0 */
#define DBF_FORCE_20MHZ			0x040000	/* Force 20 MHz TX */
#define DBF_FORCE_40MHZ 		0x080000	/* Force 40 MHz Tx */
#define DBF_DISABLE_CCK			0x100000	/* Disable CCK */
#define DBF_UNUSED200000		0x200000	/* Unused */
#define DBF_ENABLE_HT_DUP		0x400000	/* Allow HT Duplicate mode in TX rate table */
#define DBF_ENABLE_CCK_5G		0x800000	/* Enable CCK rates in 5G band */
#define DBF_UNUSED0100000		0x0100000	/* Unused */
#define DBF_ENABLE_20MHZ_MCS8	0x02000000	/* Substitute 20MHz MCS8 for 40MHz MCS8 */
#define DBF_DISABLE_20MHZ_MCS0	0x04000000	/* Disable substitution of 20MHz MCS0 for 40MHz MCS32 */
#define DBF_DISABLE_20MHZ_MCS1	0x08000000	/* Disable substitution of 20MHz MCS1 for 40MHz MCS0 */
#endif /* DBG_CTRL_SUPPORT */



#ifdef CONFIG_AP_SUPPORT
/***************************************************************************
  *	AP related data structures
  **************************************************************************/
/* AUTH-RSP State Machine Aux data structure */
typedef struct _AP_MLME_AUX {
	unsigned char Addr[MAC_ADDR_LEN];
	unsigned short Alg;
	char Challenge[CIPHER_TEXT_LEN];
} AP_MLME_AUX, *PAP_MLME_AUX;

typedef enum _MAC_ENTRY_OP_MODE_ {
	ENTRY_OP_MODE_ERROR = 0x00,
	ENTRY_OP_MODE_CLI = 0x01,	/* Sta mode, set this TRUE after Linkup,too. */
	ENTRY_OP_MODE_WDS = 0x02,	/* This is WDS Entry. only for AP mode. */
	ENTRY_OP_MODE_APCLI = 0x04,	/* This is a AP-Client entry, only for AP mode which enable AP-Client functions. */
	ENTRY_OP_MODE_MESH = 0x08,	/* Peer conect with us via mesh. */
	ENTRY_OP_MODE_DLS = 0x10,	/* This is DLS Entry. only for STA mode. */
	ENTRY_OP_MODE_MAX = 0x20
} MAC_ENTRY_OP_MODE;
#endif /* CONFIG_AP_SUPPORT */

/* Values of LastSecTxRateChangeAction */
#define RATE_NO_CHANGE	0		/* No change in rate */
#define RATE_UP			1		/* Trying higher rate or same rate with different BF */
#define RATE_DOWN		2		/* Trying lower rate */


typedef struct _MAC_TABLE_ENTRY {
	/*
	   0:Invalid,
	   Bit 0: AsCli, Bit 1: AsWds, Bit 2: AsAPCLI,
	   Bit 3: AsMesh, Bit 4: AsDls, Bit 5: AsTDls
	 */
	unsigned int EntryType;

	bool isCached;
	bool bIAmBadAtheros;	/* Flag if this is Atheros chip that has IOT problem.  We need to turn on RTS/CTS protection. */

	/* WPA/WPA2 4-way database */
	unsigned char EnqueueEapolStartTimerRunning;	/* Enqueue EAPoL-Start for triggering EAP SM */
	RALINK_TIMER_STRUCT EnqueueStartForPSKTimer;	/* A timer which enqueue EAPoL-Start for triggering PSK SM */

	/*jan for wpa */
	/* record which entry revoke MIC Failure , if it leaves the BSS itself, AP won't update aMICFailTime MIB */
	unsigned char CMTimerRunning;
	unsigned char apidx;		/* MBSS number */
	unsigned char RSNIE_Len;
	unsigned char RSN_IE[MAX_LEN_OF_RSNIE];
	unsigned char ANonce[LEN_KEY_DESC_NONCE];
	unsigned char SNonce[LEN_KEY_DESC_NONCE];
	unsigned char R_Counter[LEN_KEY_DESC_REPLAY];
	unsigned char PTK[64];
	unsigned char ReTryCounter;
	RALINK_TIMER_STRUCT RetryTimer;
#ifdef TXBF_SUPPORT
	RALINK_TIMER_STRUCT eTxBfProbeTimer;
#endif /* TXBF_SUPPORT */
	NDIS_802_11_AUTHENTICATION_MODE AuthMode;	/* This should match to whatever microsoft defined */
	NDIS_802_11_WEP_STATUS WepStatus;
	NDIS_802_11_WEP_STATUS GroupKeyWepStatus;
	unsigned char WpaState;
	unsigned char GTKState;
	unsigned short PortSecured;
	NDIS_802_11_PRIVACY_FILTER PrivacyFilter;	/* PrivacyFilter enum for 802.1X */
	CIPHER_KEY PairwiseKey;
	void * pAd;
	int PMKID_CacheIdx;
	unsigned char PMKID[LEN_PMKID];
	unsigned char NegotiatedAKM[LEN_OUI_SUITE];	/* It indicate the negotiated AKM suite */

#ifdef WAPI_SUPPORT
	unsigned char usk_id;		/* unicast key index for WPI */
#endif				/* WAPI_SUPPORT */

	unsigned char Addr[MAC_ADDR_LEN];
	unsigned char			HdrAddr1[MAC_ADDR_LEN];
	unsigned char			HdrAddr2[MAC_ADDR_LEN];
	unsigned char			HdrAddr3[MAC_ADDR_LEN];
	unsigned char PsMode;
	unsigned char FlgPsModeIsWakeForAWhile; /* wake up for a while until a condition */
	unsigned char VirtualTimeout; /* peer power save virtual timeout */
	SST Sst;
	AUTH_STATE AuthState;	/* for SHARED KEY authentication state machine used only */
	bool IsReassocSta;	/* Indicate whether this is a reassociation procedure */
	unsigned short Aid;
	unsigned short CapabilityInfo;
	unsigned char LastRssi;
	unsigned long NoDataIdleCount;
	unsigned short StationKeepAliveCount;	/* unit: second */
	unsigned long PsQIdleCount;
	QUEUE_HEADER PsQueue;

	unsigned int StaConnectTime;	/* the live time of this station since associated with AP */
	unsigned int StaIdleTimeout;	/* idle timeout per entry */

#ifdef UAPSD_SUPPORT
	/* these UAPSD states are used on the fly */
	/* 0:AC_BK, 1:AC_BE, 2:AC_VI, 3:AC_VO */
	bool bAPSDCapablePerAC[4];	/* for trigger-enabled */
	bool bAPSDDeliverEnabledPerAC[4];	/* for delivery-enabled */


	unsigned char MaxSPLength;

	bool bAPSDAllAC;	/* 1: all AC are delivery-enabled U-APSD */

	QUEUE_HEADER UAPSDQueue[WMM_NUM_OF_AC];	/* queue for each U-APSD */
	unsigned short UAPSDQIdleCount;	/* U-APSD queue timeout */

	PQUEUE_ENTRY pUAPSDEOSPFrame;	/* the last U-APSD frame */
	unsigned short UAPSDTxNum;	/* total U-APSD frame number */
	bool bAPSDFlagEOSPOK;	/* 1: EOSP frame is tx by ASIC */
	bool bAPSDFlagSPStart;	/* 1: SP is started */

	/* need to use unsigned long, because time parameters in OS is defined as
	   unsigned long */
	unsigned long UAPSDTimeStampLast;	/* unit: 1000000/OS_HZ */
	bool bAPSDFlagSpRoughUse;	/* 1: use rough SP (default: accurate) */

	/* we will set the flag when PS-poll frame is received and
	   clear it when statistics handle.
	   if the flag is set when PS-poll frame is received then calling
	   statistics handler to clear it. */
	bool bAPSDFlagLegacySent;	/* 1: Legacy PS sent but
					   yet statistics handle */

#ifdef RTMP_MAC_USB
	unsigned int UAPSDTagOffset[WMM_NUM_OF_AC];
#endif /* RTMP_MAC_USB */
#endif /* UAPSD_SUPPORT */

#ifdef DOT11_N_SUPPORT
	bool bSendBAR;
	unsigned short NoBADataCountDown;

	unsigned int CachedBuf[16];	/* unsigned int (4 bytes) for alignment */

#ifdef TXBF_SUPPORT
	COUNTER_TXBF TxBFCounters;		/* TxBF Statistics */
	unsigned int LastETxCount;		/* Used to compute %BF statistics */
	unsigned int LastITxCount;
	unsigned int LastTxCount;
#endif /* TXBF_SUPPORT */
#endif /* DOT11_N_SUPPORT */

#ifdef STREAM_MODE_SUPPORT
	unsigned int StreamModeMACReg;	/* MAC reg used to control stream mode for this client. 0=>No stream mode */
#endif // STREAM_MODE_SUPPORT //

	unsigned int FIFOCount;
	unsigned int DebugFIFOCount;
	unsigned int DebugTxCount;
	bool bDlsInit;

/*==================================================== */
/*WDS entry needs these */
/* If ValidAsWDS==TRUE, MatchWDSTabIdx is the index in WdsTab.MacTab */
	unsigned int MatchWDSTabIdx;
	unsigned char MaxSupportedRate;
	unsigned char CurrTxRate;
	unsigned char CurrTxRateIndex;
	unsigned char lastRateIdx;
	unsigned char *pTable;	/* Pointer to this entry's Tx Rate Table */

#ifdef NEW_RATE_ADAPT_SUPPORT
	unsigned char lowTrafficCount;
	unsigned char fewPktsCnt;
	bool perThrdAdj;
	unsigned char mcsGroup;/*the mcs group to be tried */
#endif /* NEW_RATE_ADAPT_SUPPORT */
	enum RATE_ADAPT_ALG rateAlg;

#ifdef MFB_SUPPORT
	unsigned char lastLegalMfb;	/*last legal mfb which is used to set rate */
	bool isMfbChanged;	/*purpose: true when mfb has changed but the new mfb is not adopted for Tx */
	struct _RTMP_RA_LEGACY_TB *LegalMfbRS;
	bool fLastChangeAccordingMfb;
	NDIS_SPIN_LOCK fLastChangeAccordingMfbLock;
/*Tx MRQ */
	bool toTxMrq;
	unsigned char msiToTx, mrqCnt;	/*mrqCnt is used to count down the inverted-BF mrq to be sent */
/*Rx mfb */
	unsigned char pendingMfsi;
/*Tx MFB */
	bool toTxMfb;
	unsigned char	mfbToTx;
	unsigned char	mfb0, mfb1;
#endif	/* MFB_SUPPORT */
#ifdef TXBF_SUPPORT
	unsigned char			TxSndgType;
	NDIS_SPIN_LOCK	TxSndgLock;

/* ETxBF */
	unsigned char		bfState;
	unsigned char		sndgMcs;
	unsigned char		sndgBW;
	int			sndg0Snr0, sndg0Snr1, sndg0Snr2;
	unsigned char		sndg0Mcs;
#ifdef ETXBF_EN_COND3_SUPPORT
	unsigned char		bestMethod;
	unsigned char		sndgRateIdx;
	unsigned char		bf0Mcs, sndg0RateIdx, bf0RateIdx;
	unsigned char		sndg1Mcs, bf1Mcs, sndg1RateIdx, bf1RateIdx;
	int			sndg1Snr0, sndg1Snr1, sndg1Snr2;
#endif /* ETXBF_EN_COND3_SUPPORT */
	unsigned char		noSndgCnt;
	unsigned char		eTxBfEnCond;
	unsigned char		noSndgCntThrd, ndpSndgStreams;
	unsigned char		iTxBfEn;

	bool		phyETxBf;			/* True=>Set ETxBF bit in PHY rate */
	bool		phyITxBf;			/* True=>Set ITxBF bit in PHY rate */
	unsigned char		lastNonBfRate;		/* Last good non-BF rate */
	bool		lastRatePhyTxBf;	/* For Quick Check. True if last rate was BF */
	unsigned short      BfTxQuality[MAX_TX_RATE_INDEX + 1];	/* Beamformed TX Quality */
#endif /* TXBF_SUPPORT */

	/* to record the each TX rate's quality. 0 is best, the bigger the worse. */
	unsigned short      TxQuality[MAX_TX_RATE_INDEX + 1];
	unsigned int		OneSecTxNoRetryOkCount;
	unsigned int      OneSecTxRetryOkCount;
	unsigned int      OneSecTxFailCount;
	unsigned int      OneSecRxLGICount;		/* unicast-to-me Long GI count */
	unsigned int      OneSecRxSGICount;      	/* unicast-to-me Short GI count */

#ifdef FIFO_EXT_SUPPORT
	unsigned int		fifoTxSucCnt;
	unsigned int		fifoTxRtyCnt;
#endif /* FIFO_EXT_SUPPORT */


	bool fLastSecAccordingRSSI;
	unsigned char LastSecTxRateChangeAction;	/* 0: no change, 1:rate UP, 2:rate down */
	char LastTimeTxRateChangeAction;	/*Keep last time value of LastSecTxRateChangeAction */
	unsigned long LastTxOkCount; /* TxSuccess count in last Rate Adaptation interval */
	unsigned char LastTxPER;	/* Tx PER in last Rate Adaptation interval */
	unsigned char PER[MAX_TX_RATE_INDEX + 1];

	unsigned int ContinueTxFailCnt;
	unsigned int CurrTxRateStableTime;	/* # of second in current TX rate */
	unsigned char TxRateUpPenalty;	/* extra # of second penalty due to last unstable condition */
#ifdef WDS_SUPPORT
	bool LockEntryTx;	/* TRUE = block to WDS Entry traffic, FALSE = not. */
#endif				/* WDS_SUPPORT */
	unsigned long TimeStamp_toTxRing;

/*==================================================== */

#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT
	unsigned int MatchAPCLITabIdx;	/* indicate the index in ApCfg.ApCliTab. */
#endif				/* APCLI_SUPPORT */
#endif				/* CONFIG_AP_SUPPORT */



	/*
		A bitmap of bool flags. each bit represent an operation status of a particular
		bool control, either ON or OFF. These flags should always be accessed via
		CLIENT_STATUS_TEST_FLAG(), CLIENT_STATUS_SET_FLAG(), CLIENT_STATUS_CLEAR_FLAG() macros.
		see fOP_STATUS_xxx in RTMP_DEF.C for detail bit definition. fCLIENT_STATUS_AMSDU_INUSED
	*/
	unsigned long ClientStatusFlags;

	HTTRANSMIT_SETTING HTPhyMode, MaxHTPhyMode, MinHTPhyMode;	/* For transmit phy setting in TXWI. */

#ifdef DOT11_N_SUPPORT
	/* HT EWC MIMO-N used parameters */
	unsigned short RXBAbitmap;	/* fill to on-chip  RXWI_BA_BITMASK in 8.1.3RX attribute entry format */
	unsigned short TXBAbitmap;	/* This bitmap as originator, only keep in software used to mark AMPDU bit in TXWI */
	unsigned short TXAutoBAbitmap;
	unsigned short BADeclineBitmap;
	unsigned short BARecWcidArray[NUM_OF_TID];	/* The mapping wcid of recipient session. if RXBAbitmap bit is masked */
	unsigned short BAOriWcidArray[NUM_OF_TID];	/* The mapping wcid of originator session. if TXBAbitmap bit is masked */
	unsigned short BAOriSequence[NUM_OF_TID];	/* The mapping wcid of originator session. if TXBAbitmap bit is masked */

	/* 802.11n features. */
	unsigned char MpduDensity;
	unsigned char MaxRAmpduFactor;
	unsigned char AMsduSize;
	unsigned char MmpsMode;		/* MIMO power save more. */

	HT_CAPABILITY_IE HTCapability;

#ifdef DOT11N_DRAFT3
	unsigned char BSS2040CoexistenceMgmtSupport;
	bool bForty_Mhz_Intolerant;
#endif /* DOT11N_DRAFT3 */

#ifdef DOT11_VHT_AC
	VHT_CAP_IE vht_cap_ie;
#endif /* DOT11_VHT_AC */

#endif /* DOT11_N_SUPPORT */


	bool bAutoTxRateSwitch;

	unsigned char RateLen;
	struct _MAC_TABLE_ENTRY *pNext;
	unsigned short TxSeq[NUM_OF_TID];
	unsigned short NonQosDataSeq;

	RSSI_SAMPLE RssiSample;
	unsigned int LastRxRate;
	short freqOffset;		/* Last RXWI FOFFSET */
	short freqOffsetValid;	/* Set when freqOffset field has been updated */


	bool bWscCapable;
	unsigned char Receive_EapolStart_EapRspId;

	unsigned int TXMCSExpected[MAX_MCS_SET];
	unsigned int TXMCSSuccessful[MAX_MCS_SET];
	unsigned int TXMCSFailed[MAX_MCS_SET];
	unsigned int TXMCSAutoFallBack[MAX_MCS_SET][MAX_MCS_SET];


#ifdef WAPI_SUPPORT
	bool WapiUskRekeyTimerRunning;
	RALINK_TIMER_STRUCT WapiUskRekeyTimer;
	unsigned int wapi_usk_rekey_cnt;
#endif				/* WAPI_SUPPORT */


	unsigned long AssocDeadLine;




	unsigned long ChannelQuality;	/* 0..100, Channel Quality Indication for Roaming */


#ifdef CONFIG_AP_SUPPORT
	MULTISSID_STRUCT *pMbss;
#endif				/* CONFIG_AP_SUPPORT */
#ifdef VENDOR_FEATURE1_SUPPORT
	/* total 128B, use unsigned int to avoid alignment problem */
	unsigned int HeaderBuf[32];	/* (total 128B) TempBuffer for TX_INFO + TX_WI + 802.11 Header + padding + AMSDU SubHeader + LLC/SNAP */

	unsigned char HdrPadLen;	/* recording Header Padding Length; */
	unsigned char MpduHeaderLen;
	unsigned short Protocol;
#endif /* VENDOR_FEATURE1_SUPPORT */

#ifdef AGS_SUPPORT
	AGS_CONTROL AGSCtrl;	/* AGS control */
#endif /* AGS_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
	LARGE_INTEGER TxPackets;
	LARGE_INTEGER RxPackets;
	unsigned long TxBytes;
	unsigned long RxBytes;
#endif /* CONFIG_AP_SUPPORT */
#ifdef WFD_SUPPORT
	bool bWfdClient;
#endif /* WFD_SUPPORT */

unsigned char	SupportRateMode; /* 1: CCK 2:OFDM 4: HT, 8:VHT */
bool SupportCCKMCS[MAX_LEN_OF_CCK_RATES];
bool SupportOFDMMCS[MAX_LEN_OF_OFDM_RATES];
bool SupportHTMCS[MAX_LEN_OF_HT_RATES];


#ifdef DOT11_VHT_AC
bool SupportVHTMCS[MAX_LEN_OF_VHT_RATES];
#endif /* DOT11_VHT_AC */
#ifdef CONFIG_MULTI_CHANNEL
	bool bDoRateTune;
#endif /* CONFIG_MULTI_CHANNEL */
} MAC_TABLE_ENTRY, *PMAC_TABLE_ENTRY;


typedef struct _MAC_TABLE {
	MAC_TABLE_ENTRY *Hash[HASH_TABLE_SIZE];
	MAC_TABLE_ENTRY Content[MAX_LEN_OF_MAC_TABLE];
	unsigned short Size;
	QUEUE_HEADER McastPsQueue;
	unsigned long PsQIdleCount;
	bool fAnyStationInPsm;
	bool fAnyStationBadAtheros;	/* Check if any Station is atheros 802.11n Chip.  We need to use RTS/CTS with Atheros 802,.11n chip. */
	bool fAnyTxOPForceDisable;	/* Check if it is necessary to disable BE TxOP */
	bool fAllStationAsRalink;	/* Check if all stations are ralink-chipset */
#ifdef DOT11_N_SUPPORT
	bool fAnyStationIsLegacy;	/* Check if I use legacy rate to transmit to my BSS Station/ */
	bool fAnyStationNonGF;	/* Check if any Station can't support GF. */
	bool fAnyStation20Only;	/* Check if any Station can't support GF. */
	bool fAnyStationMIMOPSDynamic;	/* Check if any Station is MIMO Dynamic */
	bool fAnyBASession;	/* Check if there is BA session.  Force turn on RTS/CTS */
	bool fAnyStaFortyIntolerant;	/* Check if still has any station set the Intolerant bit on! */
	bool fAllStationGainGoodMCS; /* Check if all stations more than MCS threshold */

#ifdef CONFIG_AP_SUPPORT
	bool fAnyStationIsHT;	/* Check if there is 11n STA.  Force turn off AP MIMO PS */
#endif				/* CONFIG_AP_SUPPORT */
#endif				/* DOT11_N_SUPPORT */

	unsigned short			MsduLifeTime; /* life time for PS packet */

#ifdef WAPI_SUPPORT
	bool fAnyWapiStation;
#endif				/* WAPI_SUPPORT */
} MAC_TABLE, *PMAC_TABLE;


/***************************************************************************
  *	AP WDS related data structures
  **************************************************************************/
//#ifdef WDS_SUPPORT
typedef struct _WDS_COUNTER {
	LARGE_INTEGER ReceivedFragmentCount;
	LARGE_INTEGER TransmittedFragmentCount;
	unsigned long ReceivedByteCount;
	unsigned long TransmittedByteCount;
	unsigned long RxErrors;
	unsigned long TxErrors;
	LARGE_INTEGER MulticastReceivedFrameCount;
	unsigned long OneCollision;
	unsigned long MoreCollisions;
	unsigned long RxNoBuffer;
	unsigned long RcvAlignmentErrors;
} WDS_COUNTER, *PWDS_COUNTER;

typedef struct _WDS_ENTRY {
	bool Valid;
	unsigned char Addr[MAC_ADDR_LEN];
	unsigned long NoDataIdleCount;
	struct _WDS_ENTRY *pNext;
} WDS_ENTRY, *PWDS_ENTRY;

typedef struct _WDS_TABLE_ENTRY {
	unsigned short Size;
	unsigned char WdsAddr[MAC_ADDR_LEN];
	WDS_ENTRY *Hash[HASH_TABLE_SIZE];
	WDS_ENTRY Content[MAX_LEN_OF_MAC_TABLE];
	unsigned char MaxSupportedRate;
	unsigned char CurrTxRate;
	unsigned short TxQuality[MAX_LEN_OF_SUPPORTED_RATES];
	unsigned short OneSecTxOkCount;
	unsigned short OneSecTxRetryOkCount;
	unsigned short OneSecTxFailCount;
	unsigned long CurrTxRateStableTime;	/* # of second in current TX rate */
	unsigned char TxRateUpPenalty;	/* extra # of second penalty due to last unstable condition */
} WDS_TABLE_ENTRY, *PWDS_TABLE_ENTRY;

typedef struct _RT_802_11_WDS_ENTRY {
	struct wifi_dev wdev;
	PNET_DEV dev;
	unsigned char Valid;
	unsigned char PhyMode;
	unsigned char PeerWdsAddr[MAC_ADDR_LEN];
	unsigned char MacTabMatchWCID;	/* ASIC */
	NDIS_802_11_WEP_STATUS WepStatus;
	unsigned char KeyIdx;
	CIPHER_KEY WdsKey;
	HTTRANSMIT_SETTING HTPhyMode, MaxHTPhyMode, MinHTPhyMode;
	RT_PHY_INFO DesiredHtPhyInfo;
	bool bAutoTxRateSwitch;
	DESIRED_TRANSMIT_SETTING DesiredTransmitSetting;	/* Desired transmit setting. */
	WDS_COUNTER WdsCounter;

#ifdef WDS_VLAN_SUPPORT
	/* VLAN */
    unsigned short				VLAN_VID;
    unsigned short				VLAN_Priority;
#endif /* WDS_VLAN_SUPPORT */
} RT_802_11_WDS_ENTRY, *PRT_802_11_WDS_ENTRY;

typedef struct _WDS_TABLE {
	unsigned char Mode;
	unsigned long Size;
	RT_802_11_WDS_ENTRY WdsEntry[MAX_WDS_ENTRY];
} WDS_TABLE, *PWDS_TABLE;
//#endif /* WDS_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
/***************************************************************************
  *	AP APCLI related data structures
  **************************************************************************/
typedef struct _APCLI_STRUCT {
	struct wifi_dev wdev;
	
	PNET_DEV dev;
#ifdef RTL865X_SOC
	unsigned int mylinkid;
#endif
	bool Enable;		/* Set it as 1 if the apcli interface was configured to "1"  or by iwpriv cmd "ApCliEnable" */
	bool Valid;		/* Set it as 1 if the apcli interface associated success to remote AP. */
	unsigned char MacTabWCID;	/*WCID value, which point to the entry of ASIC Mac table. */
	unsigned char SsidLen;
	char Ssid[MAX_LEN_OF_SSID];

	unsigned char CfgSsidLen;
	char CfgSsid[MAX_LEN_OF_SSID];
	unsigned char CfgApCliBssid[ETH_LENGTH_OF_ADDRESS];
	unsigned char CurrentAddress[ETH_LENGTH_OF_ADDRESS];

	unsigned long ApCliRcvBeaconTime;
	unsigned long ApCliLinkUpTime;
	unsigned short ApCliBeaconPeriod;

	unsigned long CtrlCurrState;
	unsigned long SyncCurrState;
	unsigned long AuthCurrState;
	unsigned long AssocCurrState;
	unsigned long WpaPskCurrState;

	unsigned short AuthReqCnt;
	unsigned short AssocReqCnt;

	unsigned long ClientStatusFlags;
	unsigned char MpduDensity;

	NDIS_802_11_AUTHENTICATION_MODE AuthMode;	/* This should match to whatever microsoft defined */
	NDIS_802_11_WEP_STATUS WepStatus;

	/* Add to support different cipher suite for WPA2/WPA mode */
	NDIS_802_11_ENCRYPTION_STATUS GroupCipher;	/* Multicast cipher suite */
	NDIS_802_11_ENCRYPTION_STATUS PairCipher;	/* Unicast cipher suite */
	bool bMixCipher;	/* Indicate current Pair & Group use different cipher suites */
	unsigned short RsnCapability;

	unsigned char PSK[100];		/* reserve PSK key material */
	unsigned char PSKLen;
	unsigned char PMK[32];		/* WPA PSK mode PMK */
	/*unsigned char       PTK[64];                // WPA PSK mode PTK */
	unsigned char GTK[32];		/* GTK from authenticator */

	/*CIPHER_KEY            PairwiseKey; */
	CIPHER_KEY SharedKey[SHARE_KEY_NUM];
	unsigned char DefaultKeyId;

	/* WPA 802.1x port control, WPA_802_1X_PORT_SECURED, WPA_802_1X_PORT_NOT_SECURED */
	/*unsigned char       PortSecured; */

	/* store RSN_IE built by driver */
	unsigned char RSN_IE[MAX_LEN_OF_RSNIE];	/* The content saved here should be convert to little-endian format. */
	unsigned char RSNIE_Len;

	/* For WPA countermeasures */
	unsigned long LastMicErrorTime;	/* record last MIC error time */
	/*unsigned long       MicErrCnt;          // Should be 0, 1, 2, then reset to zero (after disassoiciation). */
	bool bBlockAssoc;	/* Block associate attempt for 60 seconds after counter measure occurred. */

	/* For WPA-PSK supplicant state */
	/*WPA_STATE     WpaState;           // Default is SS_NOTUSE */
	/*unsigned char         ReplayCounter[8]; */
	/*unsigned char         ANonce[32];         // ANonce for WPA-PSK from authenticator */
	unsigned char SNonce[32];	/* SNonce for WPA-PSK */
	unsigned char GNonce[32];	/* GNonce for WPA-PSK from authenticator */

#ifdef WSC_AP_SUPPORT
	WSC_CTRL WscControl;
#endif /* WSC_AP_SUPPORT */

	HTTRANSMIT_SETTING HTPhyMode, MaxHTPhyMode, MinHTPhyMode;
	RT_PHY_INFO DesiredHtPhyInfo;
	bool bAutoTxRateSwitch;
	DESIRED_TRANSMIT_SETTING DesiredTransmitSetting;	/* Desired transmit setting. */
	unsigned char RxMcsSet[16];
#ifdef APCLI_WPA_SUPPLICANT_SUPPORT
    	bool             IEEE8021X;
  	bool             IEEE8021x_required_keys;
    	CIPHER_KEY	        DesireSharedKey[4];	// Record user desired WEP keys	
    	unsigned char               DesireSharedKeyId;
    	unsigned char               WpaSupplicantUP;
	unsigned char				WpaSupplicantScanCount;
	bool				bRSN_IE_FromWpaSupplicant;
	bool				bLostAp;
	unsigned char				*pWpsProbeReqIe;
	unsigned int				WpsProbeReqIeLen;
	unsigned char				*pWpaAssocIe;
	unsigned int				WpaAssocIeLen;
	bool			    bScanReqIsFromWebUI;
	BSSID_INFO	SavedPMK[PMKID_NO];
	unsigned int		SavedPMKNum;			// Saved PMKID number
	bool		bConfigChanged;
	NDIS_802_11_ASSOCIATION_INFORMATION     AssocInfo;
	unsigned short       ReqVarIELen;                // Length of next VIE include EID & Length
	unsigned char       ReqVarIEs[MAX_VIE_LEN];		// The content saved here should be little-endian format.
	unsigned short       ResVarIELen;                // Length of next VIE include EID & Length
	unsigned char       ResVarIEs[MAX_VIE_LEN];
	unsigned char       LastSsidLen;               // the actual ssid length in used
	char        LastSsid[MAX_LEN_OF_SSID]; // NOT NULL-terminated
	unsigned char		LastBssid[MAC_ADDR_LEN];
#endif/*APCLI_WPA_SUPPLICANT_SUPPORT*/


    PSPOLL_FRAME PsPollFrame;
    HEADER_802_11 NullFrame;

	UAPSD_INFO	UapsdInfo;
} APCLI_STRUCT, *PAPCLI_STRUCT;

typedef struct _AP_ADMIN_CONFIG {
	unsigned short CapabilityInfo;
	/* Multiple SSID */
	unsigned char BssidNum;
	unsigned char MacMask;
	MULTISSID_STRUCT MBSSID[HW_BEACON_MAX_NUM];
	unsigned long IsolateInterStaTrafficBTNBSSID;

#ifdef APCLI_SUPPORT
	unsigned char ApCliInfRunned;	/* Number of  ApClient interface which was running. value from 0 to MAX_APCLI_INTERFACE */
	bool FlgApCliIsUapsdInfoUpdated;
	APCLI_STRUCT ApCliTab[MAX_APCLI_NUM];	/*AP-client */
#endif				/* APCLI_SUPPORT */

	/* for wpa */
	RALINK_TIMER_STRUCT CounterMeasureTimer;

	unsigned char CMTimerRunning;
	unsigned char BANClass3Data;
	LARGE_INTEGER aMICFailTime;
	LARGE_INTEGER PrevaMICFailTime;
	unsigned long MICFailureCounter;

	RSSI_SAMPLE RssiSample;
	unsigned long NumOfAvgRssiSample;

	bool bAutoChannelAtBootup;	/* 0: disable, 1: enable */
	ChannelSel_Alg AutoChannelAlg;	/* Alg for selecting Channel */
	bool bAvoidDfsChannel;	/* 0: disable, 1: enable */
	bool bIsolateInterStaTraffic;
	bool bHideSsid;

	/* temporary latch for Auto channel selection */
	unsigned long ApCnt;		/* max RSSI during Auto Channel Selection period */
	unsigned char AutoChannel_Channel;	/* channel number during Auto Channel Selection */
	unsigned char current_channel_index;	/* current index of channel list */
	unsigned char AutoChannelSkipListNum;	/* number of rejected channel list */
	unsigned char AutoChannelSkipList[10];
	unsigned char DtimCount;	/* 0.. DtimPeriod-1 */
	unsigned char DtimPeriod;	/* default = 3 */
	unsigned char ErpIeContent;
	unsigned long LastOLBCDetectTime;
	unsigned long LastNoneHTOLBCDetectTime;
	unsigned long LastScanTime;	/* Record last scan time for issue BSSID_SCAN_LIST */

	unsigned char LastSNR0;		/* last received BEACON's SNR */
	unsigned char LastSNR1;		/* last received BEACON's SNR for 2nd  antenna */
#ifdef DOT11N_SS3_SUPPORT
	unsigned char LastSNR2;		/* last received BEACON's SNR for 2nd  antenna */
#endif				/* DOT11N_SS3_SUPPORT */

#ifdef DOT1X_SUPPORT
	/* dot1x related parameter */
	unsigned int own_ip_addr;
	unsigned int retry_interval;
	unsigned int session_timeout_interval;
	unsigned int quiet_interval;
	unsigned char EAPifname[HW_BEACON_MAX_NUM][IFNAMSIZ];	/* indicate as the binding interface for EAP negotiation. */
	unsigned char EAPifname_len[HW_BEACON_MAX_NUM];
	unsigned char PreAuthifname[HW_BEACON_MAX_NUM][IFNAMSIZ];	/* indicate as the binding interface for WPA2 Pre-authentication. */
	unsigned char PreAuthifname_len[HW_BEACON_MAX_NUM];
#endif				/* DOT1X_SUPPORT */

	/* EDCA parameters to be announced to its local BSS */
	EDCA_PARM BssEdcaParm;

	RALINK_TIMER_STRUCT ApQuickResponeForRateUpTimer;
	bool ApQuickResponeForRateUpTimerRunning;

#ifdef IDS_SUPPORT
	/* intrusion detection parameter */
	bool IdsEnable;
	unsigned int AuthFloodThreshold;	/* Authentication frame flood threshold */
	unsigned int AssocReqFloodThreshold;	/* Association request frame flood threshold */
	unsigned int ReassocReqFloodThreshold;	/* Re-association request frame flood threshold */
	unsigned int ProbeReqFloodThreshold;	/* Probe request frame flood threshold */
	unsigned int DisassocFloodThreshold;	/* Disassociation frame flood threshold */
	unsigned int DeauthFloodThreshold;	/* Deauthentication frame flood threshold */
	unsigned int EapReqFloodThreshold;	/* EAP request frame flood threshold */

	unsigned int RcvdAuthCount;
	unsigned int RcvdAssocReqCount;
	unsigned int RcvdReassocReqCount;
	unsigned int RcvdProbeReqCount;
	unsigned int RcvdDisassocCount;
	unsigned int RcvdDeauthCount;
	unsigned int RcvdEapReqCount;

	RALINK_TIMER_STRUCT IDSTimer;
	bool IDSTimerRunning;
#endif				/* IDS_SUPPORT */

	/* Indicate the maximum idle timeout */
	unsigned int StaIdleTimeout;

	unsigned long EntryLifeCheck;

#ifdef IGMP_SNOOP_SUPPORT
	bool IgmpSnoopEnable;	/* 0: disable, 1: enable. */
#endif				/* IGMP_SNOOP_SUPPORT */


#ifdef CLIENT_WDS
	NDIS_SPIN_LOCK CliWdsTabLock;
	PCLIWDS_PROXY_ENTRY pCliWdsEntryPool;
	LIST_HEADER CliWdsEntryFreeList;
	LIST_HEADER CliWdsProxyTab[CLIWDS_HASH_TAB_SIZE];
#endif /* CLIENT_WDS */

#ifdef DOT11_N_SUPPORT
#ifdef GREENAP_SUPPORT
	unsigned char GreenAPLevel;
	bool bGreenAPEnable;
	bool bGreenAPActive;
#endif /* GREENAP_SUPPORT */

	unsigned long	MAX_PSDU_LEN;	/* Maximum PSDU length */
#endif /* DOT11_N_SUPPORT */

	unsigned char	EntryClientCount;
} AP_ADMIN_CONFIG, *PAP_ADMIN_CONFIG;

#ifdef IGMP_SNOOP_SUPPORT
typedef enum _IGMP_GROUP_TYPE {
	MODE_IS_INCLUDE = 1,
	MODE_IS_EXCLUDE,
	CHANGE_TO_INCLUDE_MODE,
	CHANGE_TO_EXCLUDE_MODE,
	ALLOW_NEW_SOURCES,
	BLOCK_OLD_SOURCES
} IgmpGroupType;

typedef enum _MULTICAST_FILTER_ENTRY_TYPE {
	MCAT_FILTER_STATIC = 0,
	MCAT_FILTER_DYNAMIC,
} MulticastFilterEntryType;

typedef struct _MEMBER_ENTRY {
	struct _MEMBER_ENTRY *pNext;
	unsigned char Addr[MAC_ADDR_LEN];
/*	unsigned short Aid; */
} MEMBER_ENTRY, *PMEMBER_ENTRY;

typedef struct _MULTICAST_FILTER_TABLE_ENTRY {
	bool Valid;
	MulticastFilterEntryType type;	/* 0: static, 1: dynamic. */
	unsigned int lastTime;
	PNET_DEV net_dev;
	unsigned char Addr[MAC_ADDR_LEN];
	LIST_HEADER MemberList;
	struct _MULTICAST_FILTER_TABLE_ENTRY *pNext;
} MULTICAST_FILTER_TABLE_ENTRY, *PMULTICAST_FILTER_TABLE_ENTRY;

typedef struct _MULTICAST_FILTER_TABLE {
	unsigned char Size;
	PMULTICAST_FILTER_TABLE_ENTRY
	  Hash[MAX_LEN_OF_MULTICAST_FILTER_HASH_TABLE];
	MULTICAST_FILTER_TABLE_ENTRY Content[MAX_LEN_OF_MULTICAST_FILTER_TABLE];
	NDIS_SPIN_LOCK MulticastFilterTabLock;
	NDIS_SPIN_LOCK FreeMemberPoolTabLock;
	MEMBER_ENTRY freeMemberPool[FREE_MEMBER_POOL_SIZE];
	LIST_HEADER freeEntryList;
} MULTICAST_FILTER_TABLE, *PMULTICAST_FILTER_TABLE;
#endif /* IGMP_SNOOP_SUPPORT */


#ifdef DOT11_N_SUPPORT
#ifdef GREENAP_SUPPORT
typedef enum _RT_GREEN_AP_LEVEL {
	GREENAP_11BGN_STAS = 0,
	GREENAP_ONLY_11BG_STAS,
	GREENAP_WITHOUT_ANY_STAS_CONNECT
} RT_GREEN_AP_LEVEL;
#endif /* DOT11_N_SUPPORT */
#endif /* GREENAP_SUPPORT */

/* ----------- end of AP ---------------------------- */
#endif /* CONFIG_AP_SUPPORT */

#ifdef BLOCK_NET_IF
typedef struct _BLOCK_QUEUE_ENTRY {
	bool SwTxQueueBlockFlag;
	LIST_HEADER NetIfList;
} BLOCK_QUEUE_ENTRY, *PBLOCK_QUEUE_ENTRY;
#endif /* BLOCK_NET_IF */


struct wificonf {
	bool bShortGI;
	bool bGreenField;
};

typedef struct _RTMP_DEV_INFO_ {
	unsigned char chipName[16];
	RTMP_INF_TYPE infType;
} RTMP_DEV_INFO;

#ifdef DBG_DIAGNOSE
#define DIAGNOSE_TIME	10	/* 10 sec */
typedef struct _RtmpDiagStrcut_ {	/* Diagnosis Related element */
	unsigned char inited;
	unsigned char qIdx;
	unsigned char ArrayStartIdx;
	unsigned char ArrayCurIdx;
	/* Tx Related Count */
	unsigned short TxDataCnt[DIAGNOSE_TIME];
	unsigned short TxFailCnt[DIAGNOSE_TIME];
/*	unsigned short TxDescCnt[DIAGNOSE_TIME][16];		// TxDesc queue length in scale of 0~14, >=15 */
	unsigned short TxDescCnt[DIAGNOSE_TIME][24];	/* 3*3    // TxDesc queue length in scale of 0~14, >=15 */
/*	unsigned short TxMcsCnt[DIAGNOSE_TIME][16];			// TxDate MCS Count in range from 0 to 15, step in 1. */
	unsigned short TxMcsCnt[DIAGNOSE_TIME][MAX_MCS_SET];	/* 3*3 */
	unsigned short TxSWQueCnt[DIAGNOSE_TIME][9];	/* TxSwQueue length in scale of 0, 1, 2, 3, 4, 5, 6, 7, >=8 */

	unsigned short TxAggCnt[DIAGNOSE_TIME];
	unsigned short TxNonAggCnt[DIAGNOSE_TIME];
/*	unsigned short TxAMPDUCnt[DIAGNOSE_TIME][16];		// 10 sec, TxDMA APMDU Aggregation count in range from 0 to 15, in setp of 1. */
	unsigned short TxAMPDUCnt[DIAGNOSE_TIME][MAX_MCS_SET];	/* 3*3 // 10 sec, TxDMA APMDU Aggregation count in range from 0 to 15, in setp of 1. */
	unsigned short TxRalinkCnt[DIAGNOSE_TIME];	/* TxRalink Aggregation Count in 1 sec scale. */
	unsigned short TxAMSDUCnt[DIAGNOSE_TIME];	/* TxAMSUD Aggregation Count in 1 sec scale. */

	/* Rx Related Count */
	unsigned short RxDataCnt[DIAGNOSE_TIME];	/* Rx Total Data count. */
	unsigned short RxCrcErrCnt[DIAGNOSE_TIME];
/*	unsigned short			RxMcsCnt[DIAGNOSE_TIME][16];		// Rx MCS Count in range from 0 to 15, step in 1. */
	unsigned short RxMcsCnt[DIAGNOSE_TIME][MAX_MCS_SET];	/* 3*3 */
} RtmpDiagStruct;
#endif /* DBG_DIAGNOSE */

#if defined(RTMP_INTERNAL_TX_ALC) || defined(RTMP_TEMPERATURE_COMPENSATION) 
/*
	The number of channels for per-channel Tx power offset
*/
#define NUM_OF_CH_FOR_PER_CH_TX_PWR_OFFSET	14

/* The Tx power control using the internal ALC */
typedef struct _TX_POWER_CONTROL {
	bool bInternalTxALC; /* Internal Tx ALC */
	bool bExtendedTssiMode; /* The extended TSSI mode (each channel has different Tx power if needed) */
	char PerChTxPwrOffset[NUM_OF_CH_FOR_PER_CH_TX_PWR_OFFSET + 1]; /* Per-channel Tx power offset */
	char idxTxPowerTable; /* The index of the Tx power table for ant0 */
	char idxTxPowerTable2; /* The index of the Tx power table for ant1 */
	char RF_TX_ALC; /* 3390: RF R12[4:0]: Tx0 ALC, 3352: RF R47[4:0]: Tx0 ALC, 5390: RF R49[5:0]: Tx0 ALC */
	char MAC_PowerDelta; /* Tx power control over MAC 0x1314~0x1324 */
	char MAC_PowerDelta2; /* Tx power control for Tx1 */
	char TotalDeltaPower2; /* Tx power control for Tx1 */
#ifdef RTMP_TEMPERATURE_COMPENSATION
	int LookupTable[IEEE80211_BAND_NUMS][33];
	int RefTemp[IEEE80211_BAND_NUMS];
	unsigned char TssiGain[IEEE80211_BAND_NUMS];
	/* Index offset, -7....25. */
	int LookupTableIndex;
#endif /* RTMP_TEMPERATURE_COMPENSATION */

} TX_POWER_CONTROL, *PTX_POWER_CONTROL;
#endif /* RTMP_INTERNAL_TX_ALC || RTMP_TEMPERATURE_COMPENSATION */

/* */
/* The entry of transmit power control over MAC */
/* */
typedef struct _TX_POWER_CONTROL_OVER_MAC_ENTRY {
	unsigned short MACRegisterOffset;	/* MAC register offset */
	unsigned long RegisterValue;	/* Register value */
} TX_POWER_CONTROL_OVER_MAC_ENTRY, *PTX_POWER_CONTROL_OVER_MAC_ENTRY;

/* */
/* The maximum registers of transmit power control */
/* */
#define MAX_TX_PWR_CONTROL_OVER_MAC_REGISTERS 5



/* */
/* The configuration of the transmit power control over MAC */
/* */
typedef struct _CONFIGURATION_OF_TX_POWER_CONTROL_OVER_MAC {
	unsigned char NumOfEntries;	/* Number of entries */
	TX_POWER_CONTROL_OVER_MAC_ENTRY TxPwrCtrlOverMAC[MAX_TX_PWR_CONTROL_OVER_MAC_REGISTERS];
} CONFIGURATION_OF_TX_POWER_CONTROL_OVER_MAC, *PCONFIGURATION_OF_TX_POWER_CONTROL_OVER_MAC;

/* */
/* The extension of the transmit power control over MAC */
/* */
typedef struct _TX_POWER_CONTROL_EXT_OVER_MAC {
	struct {
		unsigned long TxPwrCfg0;	/* MAC 0x1314 */
		unsigned long TxPwrCfg0Ext;	/* MAC 0x1390 */
		unsigned long TxPwrCfg1;	/* MAC 0x1318 */
		unsigned long TxPwrCfg1Ext;	/* MAC 0x1394 */
		unsigned long TxPwrCfg2;	/* MAC 0x131C */
		unsigned long TxPwrCfg2Ext;	/* MAC 0x1398 */
		unsigned long TxPwrCfg3;	/* MAC 0x1320 */
		unsigned long TxPwrCfg3Ext;	/* MAC 0x139C */
		unsigned long TxPwrCfg4;	/* MAC 0x1324 */
		unsigned long TxPwrCfg4Ext;	/* MAC 0x13A0 */
		unsigned long TxPwrCfg5;	/* MAC 0x1384 */
		unsigned long TxPwrCfg6;	/* MAC 0x1388 */
		unsigned long TxPwrCfg7;	/* MAC 0x13D4 */
		unsigned long TxPwrCfg8;	/* MAC 0x13D8 */
		unsigned long TxPwrCfg9;	/* MAC 0x13DC */
	} BW20Over2Dot4G;

	struct {
		unsigned long TxPwrCfg0;	/* MAC 0x1314 */
		unsigned long TxPwrCfg0Ext;	/* MAC 0x1390 */
		unsigned long TxPwrCfg1;	/* MAC 0x1318 */
		unsigned long TxPwrCfg1Ext;	/* MAC 0x1394 */
		unsigned long TxPwrCfg2;	/* MAC 0x131C */
		unsigned long TxPwrCfg2Ext;	/* MAC 0x1398 */
		unsigned long TxPwrCfg3;	/* MAC 0x1320 */
		unsigned long TxPwrCfg3Ext;	/* MAC 0x139C */
		unsigned long TxPwrCfg4;	/* MAC 0x1324 */
		unsigned long TxPwrCfg4Ext;	/* MAC 0x13A0 */
		unsigned long TxPwrCfg5;	/* MAC 0x1384 */
		unsigned long TxPwrCfg6;	/* MAC 0x1388 */
		unsigned long TxPwrCfg7;	/* MAC 0x13D4 */
		unsigned long TxPwrCfg8;	/* MAC 0x13D8 */
		unsigned long TxPwrCfg9;	/* MAC 0x13DC */
	} BW40Over2Dot4G;

	struct {
		unsigned long TxPwrCfg0;	/* MAC 0x1314 */
		unsigned long TxPwrCfg0Ext;	/* MAC 0x1390 */
		unsigned long TxPwrCfg1;	/* MAC 0x1318 */
		unsigned long TxPwrCfg1Ext;	/* MAC 0x1394 */
		unsigned long TxPwrCfg2;	/* MAC 0x131C */
		unsigned long TxPwrCfg2Ext;	/* MAC 0x1398 */
		unsigned long TxPwrCfg3;	/* MAC 0x1320 */
		unsigned long TxPwrCfg3Ext;	/* MAC 0x139C */
		unsigned long TxPwrCfg4;	/* MAC 0x1324 */
		unsigned long TxPwrCfg4Ext;	/* MAC 0x13A0 */
		unsigned long TxPwrCfg5;	/* MAC 0x1384 */
		unsigned long TxPwrCfg6;	/* MAC 0x1388 */
		unsigned long TxPwrCfg7;	/* MAC 0x13D4 */
		unsigned long TxPwrCfg8;	/* MAC 0x13D8 */
		unsigned long TxPwrCfg9;	/* MAC 0x13DC */
	} BW20Over5G;

	struct {
		unsigned long TxPwrCfg0;	/* MAC 0x1314 */
		unsigned long TxPwrCfg0Ext;	/* MAC 0x1390 */
		unsigned long TxPwrCfg1;	/* MAC 0x1318 */
		unsigned long TxPwrCfg1Ext;	/* MAC 0x1394 */
		unsigned long TxPwrCfg2;	/* MAC 0x131C */
		unsigned long TxPwrCfg2Ext;	/* MAC 0x1398 */
		unsigned long TxPwrCfg3;	/* MAC 0x1320 */
		unsigned long TxPwrCfg3Ext;	/* MAC 0x139C */
		unsigned long TxPwrCfg4;	/* MAC 0x1324 */
		unsigned long TxPwrCfg4Ext;	/* MAC 0x13A0 */
		unsigned long TxPwrCfg5;	/* MAC 0x1384 */
		unsigned long TxPwrCfg6;	/* MAC 0x1388 */
		unsigned long TxPwrCfg7;	/* MAC 0x13D4 */
		unsigned long TxPwrCfg8;	/* MAC 0x13D8 */
		unsigned long TxPwrCfg9;	/* MAC 0x13DC */
	} BW40Over5G;
} TX_POWER_CONTROL_EXT_OVER_MAC, *PTX_POWER_CONTROL_EXT_OVER_MAC;

/* For Wake on Wireless LAN */
#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
typedef struct _WOW_CFG_STRUCT {
	bool			bEnable;		/* Enable WOW function*/
	bool			bWOWFirmware;	/* Enable WOW function, trigger to reload WOW-support firmware */
	bool			bInBand;		/* use in-band signal to wakeup system */
	unsigned char			nSelectedGPIO;	/* Side band signal to wake up system */
	unsigned char			nDelay;			/* Delay number is multiple of 3 secs, and it used to postpone the WOW function */
	unsigned char           nHoldTime;      /* GPIO puls hold time, unit: 10ms */
} WOW_CFG_STRUCT, *PWOW_CFG_STRUCT;
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */

#ifdef NEW_WOW_SUPPORT
typedef enum {
	WOW_PKT_TO_HOST,
	WOW_PKT_TO_ANDES
} WOW_PKT_FLOW_T;

typedef enum {
	WOW_WAKEUP_BY_PCIE,
	WOW_WAKEUP_BY_USB,
	WOW_WAKEUP_BY_GPIO
} WOW_WAKEUP_METHOD_T; 

typedef enum {
	WOW_ENABLE = 1,
	WOW_TRAFFIC = 3,
	WOW_WAKEUP = 4
} WOW_FEATURE_T;

typedef enum {
	WOW_MASK_CFG = 1,
	WOW_SEC_CFG,
	WOW_INFRA_CFG,
	WOW_P2P_CFG,
} WOW_CONFIG_T;

enum {
	WOW_MAGIC_PKT,
	WOW_BITMAP,
	WOW_IPV4_TCP_SYNC,
	WOW_IPV6_TCP_SYNC
};

typedef struct NEW_WOW_MASK_CFG_STRUCT {
	unsigned int 	Config_Type;
	unsigned int 	Function_Enable;
	unsigned int 	Detect_Mask;
	unsigned int 	Event_Mask;
} NEW_WOW_MASK_CFG_STRUCT, PNEW_WOW_MASK_CFG_STRUCT;

typedef struct NEW_WOW_SEC_CFG_STRUCT {
	unsigned int 	Config_Type;
	unsigned int 	WPA_Ver;
	unsigned char 	PTK[64];
	unsigned char 	R_COUNTER[8];
	unsigned char 	Key_Id;
	unsigned char 	Cipher_Alg;
	unsigned char 	WCID;
	unsigned char 	Group_Cipher;
} NEW_WOW_SEC_CFG_STRUCT, PNEW_WOW_SEC_CFG_STRUCT;

typedef struct NEW_WOW_INFRA_CFG_STRUCT {
	unsigned int 	Config_Type;
	unsigned char 	STA_MAC[6];
	unsigned char 	AP_MAC[6];
	unsigned int 	AP_Status;
} NEW_WOW_INFRA_CFG_STRUCT, PNEW_WOW_INFRA_CFG_STRUCT;

typedef struct _NEW_WOW_P2P_CFG_STRUCT {
	unsigned int 	Config_Type;
	unsigned char 	GO_MAC[6];
	unsigned char 	CLI_MAC[6];
	unsigned int 	P2P_Status;
} NEW_WOW_P2P_CFG_STRUCT, *PNEW_WOW_P2P_CFG_STRUCT;

typedef struct _NEW_WOW_PARAM_STRUCT {
	unsigned int 	Parameter;
	unsigned int 	Value;
} NEW_WOW_PARAM_STRUCT, *PNEW_WOW_PARAM_STRUCT;
#endif /* NEW_WOW_SUPPORT */

/*
	Packet drop reason code
*/
typedef enum{
	PKT_ATE_ON = 1 << 8,
	PKT_RADAR_ON = 2 << 8,
	PKT_RRM_QUIET = 3 << 8,
	PKT_TX_STOP = 4 <<8,
	PKT_TX_JAM = 5 << 8,

	PKT_NETDEV_DOWN = 6 < 8,
	PKT_NETDEV_NO_MATCH = 7 << 8,
	PKT_NOT_ALLOW_SEND = 8 << 8,

	PKT_INVALID_DST = 9<< 8,
	PKT_INVALID_SRC = 10 << 8,
	PKT_INVALID_PKT_DATA = 11 << 8,
	PKT_INVALID_PKT_LEN = 12 << 8,
	PKT_INVALID_ETH_TYPE = 13 << 8,
	PKT_INVALID_TXBLK_INFO = 14 << 8,
	PKT_INVALID_SW_ENCRYPT = 15 << 8,
	PKT_INVALID_PKT_TYPE = 16 << 8,
	PKT_INVALID_PKT_MIC = 17 << 8,
	
	PKT_PORT_NOT_SECURE = 18 << 8,
	PKT_TSPEC_NO_MATCH  = 19 << 8,
	PKT_NO_ASSOCED_STA = 20 << 8,
	PKT_INVALID_MAC_ENTRY = 21 << 8,
	
	PKT_TX_QUE_FULL = 22 << 8,
	PKT_TX_QUE_ADJUST = 23<<8,
		
	PKT_PS_QUE_TIMEOUT = 24 <<8,
	PKT_PS_QUE_CLEAN = 25 << 8,
	PKT_MCAST_PS_QUE_FULL = 26 << 8,
	PKT_UCAST_PS_QUE_FULL = 27 << 8,
	
	PKT_RX_EAPOL_SANITY_FAIL = 28 <<8,
	PKT_RX_NOT_TO_KERNEL = 29 << 8,
	PKT_RX_MESH_SIG_FAIL = 30 << 8,
	PKT_APCLI_FAIL = 31 << 8,
	PKT_ZERO_DATA = 32 <<8,
	PKT_SW_DECRYPT_FAIL = 33 << 8,
	PKT_TX_SW_ENC_FAIL = 34 << 8,

	PKT_ACM_FAIL = 35 << 8,
	PKT_IGMP_GRP_FAIL = 36 << 8,
	PKT_MGMT_FAIL = 37 << 8,
	PKT_AMPDU_OUT_ORDER = 38 << 8,
	PKT_UAPSD_EOSP = 39 << 8,
	PKT_UAPSD_Q_FULL = 40 << 8,

	PKT_DRO_REASON_MAX = 41,
}PKT_DROP_REASON;

/* Packet drop Direction code */
typedef enum{
	PKT_TX = 0,
	PKT_RX = 1 << 31,
}PKT_DROP_DIECTION;




typedef struct _BBP_RESET_CTL
{
#define BBP_RECORD_NUM	47
	REG_PAIR BBPRegDB[BBP_RECORD_NUM];
	bool	AsicCheckEn;
} BBP_RESET_CTL, *PBBP_RESET_CTL;


/* */
/*  The miniport adapter structure */
/* */
struct _RTMP_ADAPTER {
	void * OS_Cookie;	/* save specific structure relative to OS */
	PNET_DEV net_dev;
	unsigned long VirtualIfCnt;

	RTMP_CHIP_OP chipOps;
	RTMP_CHIP_CAP chipCap;


#ifdef HOSTAPD_SUPPORT
	unsigned int IoctlIF;
#endif /* HOSTAPD_SUPPORT */
#ifdef INF_PPA_SUPPORT
	unsigned int g_if_id;
	bool PPAEnable;
	PPA_DIRECTPATH_CB *pDirectpathCb;
#endif /* INF_PPA_SUPPORT */


	NDIS_SPIN_LOCK irq_lock;

	/*======Cmd Thread in PCI/RBUS/USB */
	CmdQ CmdQ;
	NDIS_SPIN_LOCK CmdQLock;	/* CmdQLock spinlock */
	RTMP_OS_TASK cmdQTask;

#ifdef RTMP_MAC_USB
/*****************************************************************************************/
/*      USB related parameters                                                           */
/*****************************************************************************************/
/*	struct usb_config_descriptor		*config; */
	void *config;

	unsigned int NumberOfPipes;
	unsigned short BulkOutMaxPacketSize;
	unsigned short BulkInMaxPacketSize;
	unsigned char BulkOutEpAddr[6];
	unsigned char BulkInEpAddr[2]; 

	/*======Control Flags */
	unsigned long BulkFlags;
	bool bUsbTxBulkAggre;	/* Flags for bulk out data priority */

	/*======Cmd Thread */
/*	CmdQ					CmdQ; */
/*	NDIS_SPIN_LOCK			CmdQLock;				// CmdQLock spinlock */
/*	RTMP_OS_TASK			cmdQTask; */

	/*======Semaphores (event) */
	RTMP_OS_SEM UsbVendorReq_semaphore;
	RTMP_OS_SEM reg_atomic;
	RTMP_OS_SEM hw_atomic;
	void * UsbVendorReqBuf;
	bool VendorResetFlag;
/*	wait_queue_head_t	 *wait; */
	void *wait;

	/* lock for ATE */
#ifdef RALINK_ATE
	NDIS_SPIN_LOCK GenericLock;	/* ATE Tx/Rx generic spinlock */
#endif /* RALINK_ATE */

#endif /* RTMP_MAC_USB */

/*****************************************************************************************/
/*      RBUS related parameters                                                           								  */
/*****************************************************************************************/

/*****************************************************************************************/
/*      Both PCI/USB related parameters                                                  							  */
/*****************************************************************************************/
	/*RTMP_DEV_INFO                 chipInfo; */
	RTMP_INF_TYPE infType;

/*****************************************************************************************/
/*      Driver Mgmt related parameters                                                  							  */
/*****************************************************************************************/
	RTMP_OS_TASK mlmeTask;
#ifdef RTMP_TIMER_TASK_SUPPORT
	/* If you want use timer task to handle the timer related jobs, enable this. */
	RTMP_TIMER_TASK_QUEUE TimerQ;
	NDIS_SPIN_LOCK TimerQLock;
	RTMP_OS_TASK timerTask;
#endif /* RTMP_TIMER_TASK_SUPPORT */

/*****************************************************************************************/
/*      Tx related parameters                                                           */
/*****************************************************************************************/
	bool DeQueueRunning[NUM_OF_TX_RING];	/* for ensuring RTUSBDeQueuePacket get call once */
	NDIS_SPIN_LOCK DeQueueLock[NUM_OF_TX_RING];

#ifdef RTMP_MAC_USB
	/* Data related context and AC specified, 4 AC supported */
	NDIS_SPIN_LOCK BulkOutLock[6];	/* BulkOut spinlock for 4 ACs */
	NDIS_SPIN_LOCK MLMEBulkOutLock;	/* MLME BulkOut lock */

	HT_TX_CONTEXT TxContext[NUM_OF_TX_RING];
	NDIS_SPIN_LOCK TxContextQueueLock[NUM_OF_TX_RING];	/* TxContextQueue spinlock */

	/* 4 sets of Bulk Out index and pending flag */
	/*
	   array size of NextBulkOutIndex must be larger than or equal to 5;
	   Or BulkOutPending[0] will be overwrited in NICInitTransmit().
	 */
	unsigned char NextBulkOutIndex[NUM_OF_TX_RING];	/* only used for 4 EDCA bulkout pipe */

	bool BulkOutPending[6];	/* used for total 6 bulkout pipe */
	unsigned char bulkResetPipeid;
	bool MgmtBulkPending;
	unsigned long bulkResetReq[6];
#ifdef INF_AMAZON_SE
	unsigned long BulkOutDataSizeCount[NUM_OF_TX_RING];
	bool BulkOutDataFlag[NUM_OF_TX_RING];
	unsigned long BulkOutDataSizeLimit[NUM_OF_TX_RING];
	unsigned char RunningQueueNoCount;
	unsigned char LastRunningQueueNo;
#endif /* #ifdef INF_AMAZON_SE */


#endif /* RTMP_MAC_USB */

	/* resource for software backlog queues */
	QUEUE_HEADER TxSwQueue[NUM_OF_TX_RING];	/* 4 AC + 1 HCCA */
	NDIS_SPIN_LOCK TxSwQueueLock[NUM_OF_TX_RING];	/* TxSwQueue spinlock */

	/* Maximum allowed tx software Queue length */
	unsigned int					TxSwQMaxLen;

	RTMP_DMABUF MgmtDescRing;	/* Shared memory for MGMT descriptors */
	RTMP_MGMT_RING MgmtRing;
	NDIS_SPIN_LOCK MgmtRingLock;	/* Prio Ring spinlock */


	unsigned char LastMCUCmd;

/*****************************************************************************************/
/*      Rx related parameters                                                           */
/*****************************************************************************************/


#ifdef RTMP_MAC_USB
	RX_CONTEXT RxContext[RX_RING_SIZE];	/* 1 for redundant multiple IRP bulk in. */
	NDIS_SPIN_LOCK BulkInLock;	/* BulkIn spinlock for 4 ACs */
	NDIS_SPIN_LOCK CmdRspLock;
	unsigned char PendingRx;	/* The Maximum pending Rx value should be       RX_RING_SIZE. */
	unsigned char NextRxBulkInIndex;	/* Indicate the current RxContext Index which hold by Host controller. */
	unsigned char NextRxBulkInReadIndex;	/* Indicate the current RxContext Index which driver can read & process it. */
	unsigned long NextRxBulkInPosition;	/* Want to contatenate 2 URB buffer while 1st is bulkin failed URB. This Position is 1st URB TransferLength. */
	unsigned long TransferBufferLength;	/* current length of the packet buffer */
	unsigned long ReadPosition;	/* current read position in a packet buffer */

	CMD_RSP_CONTEXT CmdRspEventContext;
#endif /* RTMP_MAC_USB */

/*****************************************************************************************/
/*      ASIC related parameters                                                          */
/*****************************************************************************************/
	unsigned int MACVersion;	/* MAC version. Record rt2860C(0x28600100) or rt2860D (0x28600101).. */

	/* --------------------------- */
	/* E2PROM */
	/* --------------------------- */
	unsigned long EepromVersion;	/* byte 0: version, byte 1: revision, byte 2~3: unused */
	unsigned long FirmwareVersion;	/* byte 0: Minor version, byte 1: Major version, otherwise unused. */
	unsigned short EEPROMDefaultValue[NUM_EEPROM_BBP_PARMS];
#ifdef TXBF_SUPPORT
	unsigned short EEPROMITxBFCalParams[6];
#endif /* TXBF_SUPPORT */
	unsigned char EEPROMAddressNum;	/* 93c46=6  93c66=8 */
	bool EepromAccess;
	unsigned char EFuseTag;

	/* --------------------------- */
	/* BBP Control */
	/* --------------------------- */
	unsigned char BbpWriteLatch[MAX_BBP_ID + 1];	/* record last BBP register value written via BBP_IO_WRITE/BBP_IO_WRITE_VY_REG_ID */
	char BbpRssiToDbmDelta;	/* change from unsigned char to char for high power */
	BBP_R66_TUNING BbpTuning;

	/* ---------------------------- */
	/* RFIC control */
	/* ---------------------------- */
	unsigned char RfIcType;		/* RFIC_xxx */
	unsigned long RfFreqOffset;	/* Frequency offset for channel switching */


	RTMP_RF_REGS LatchRfRegs;	/* latch th latest RF programming value since RF IC doesn't support READ */

	EEPROM_ANTENNA_STRUC Antenna;	/* Since ANtenna definition is different for a & g. We need to save it for future reference. */
	EEPROM_NIC_CONFIG2_STRUC NicConfig2;
#if defined(BT_COEXISTENCE_SUPPORT) || defined(RT3290)
	EEPROM_NIC_CONFIG3_STRUC NicConfig3;
#endif /* defined(BT_COEXISTENCE_SUPPORT) || defined(RT3290) */

	/* This soft Rx Antenna Diversity mechanism is used only when user set */
	/* RX Antenna = DIVERSITY ON */
	SOFT_RX_ANT_DIVERSITY RxAnt;

	CHANNEL_TX_POWER TxPower[MAX_NUM_OF_CHANNELS];	/* Store Tx power value for all channels. */
	CHANNEL_TX_POWER ChannelList[MAX_NUM_OF_CHANNELS];	/* list all supported channels for site survey */



	unsigned char ChannelListNum;	/* number of channel in ChannelList[] */
	unsigned char Bbp94;
	bool BbpForCCK;
	unsigned long Tx20MPwrCfgABand[MAX_TXPOWER_ARRAY_SIZE];
	unsigned long Tx20MPwrCfgGBand[MAX_TXPOWER_ARRAY_SIZE];
	unsigned long Tx40MPwrCfgABand[MAX_TXPOWER_ARRAY_SIZE];
	unsigned long Tx40MPwrCfgGBand[MAX_TXPOWER_ARRAY_SIZE];
#ifdef DOT11_VHT_AC
	unsigned long Tx80MPwrCfgABand[MAX_TXPOWER_ARRAY_SIZE]; // Per-rate Tx power control for VHT BW80 (5GHz only)
#endif /* DOT11_VHT_AC */


#ifdef MT7601
	unsigned int TxCCKPwrCfg;
#endif /* MT7601 */

	bool bAutoTxAgcA;	/* Enable driver auto Tx Agc control */
	unsigned char TssiRefA;		/* Store Tssi reference value as 25 temperature. */
	unsigned char TssiPlusBoundaryA[5];	/* Tssi boundary for increase Tx power to compensate. */
	unsigned char TssiMinusBoundaryA[5];	/* Tssi boundary for decrease Tx power to compensate. */
	unsigned char TxAgcStepA;	/* Store Tx TSSI delta increment / decrement value */
	char TxAgcCompensateA;	/* Store the compensation (TxAgcStep * (idx-1)) */

	bool bAutoTxAgcG;	/* Enable driver auto Tx Agc control */
	unsigned char TssiRefG;		/* Store Tssi reference value as 25 temperature. */
	unsigned char TssiPlusBoundaryG[5];	/* Tssi boundary for increase Tx power to compensate. */
	unsigned char TssiMinusBoundaryG[5];	/* Tssi boundary for decrease Tx power to compensate. */
	unsigned char TxAgcStepG;	/* Store Tx TSSI delta increment / decrement value */
	char TxAgcCompensateG;	/* Store the compensation (TxAgcStep * (idx-1)) */
#if defined(RTMP_INTERNAL_TX_ALC) || defined(RTMP_TEMPERATURE_COMPENSATION) 
	TX_POWER_CONTROL TxPowerCtrl;	/* The Tx power control using the internal ALC */
#endif /* RTMP_INTERNAL_TX_ALC || RTMP_TEMPERATURE_COMPENSATION */


	signed char BGRssiOffset[3]; /* Store B/G RSSI #0/1/2 Offset value on EEPROM 0x46h */
	signed char ARssiOffset[3]; /* Store A RSSI 0/1/2 Offset value on EEPROM 0x4Ah */

	char BLNAGain;		/* Store B/G external LNA#0 value on EEPROM 0x44h */
	char ALNAGain0;		/* Store A external LNA#0 value for ch36~64 */
	char ALNAGain1;		/* Store A external LNA#1 value for ch100~128 */
	char ALNAGain2;		/* Store A external LNA#2 value for ch132~165 */


#ifdef LED_CONTROL_SUPPORT
	/* LED control */
	LED_CONTROL LedCntl;
#endif /* LED_CONTROL_SUPPORT */

	/* ---------------------------- */
	/* MAC control */
	/* ---------------------------- */

#ifdef RT8592
// TODO: shiang-6590, temporary get from windows and need to revise it!!
	/* IQ Calibration */
	unsigned char IQGainTx[3][4];
	unsigned char IQPhaseTx[3][4];
	unsigned short IQControl;
#endif /* RT8592 */

#if defined(RT3290) || defined(RT65xx) || defined(MT7601)

	WLAN_FUN_CTRL_STRUC WlanFunCtrl;
#endif /* defined(RT3290) || defined(RT65xx) || defined(MT7601) */

#ifdef RT3290
	// TODO: shiang, check about the purpose of this parameter
	CMB_CTRL_STRUC	CmbCtrl;
	WLAN_FUN_INFO_STRUC WlanFunInfo;
	BT_FUN_CTRL_STRUC BtFunCtrl;
	WLAN_BT_COEX_SETTING WlanBTCoexInfo;
	bool RateUp;
	unsigned long AntennaDiversityState;/* 0->Stable state 1->training state */
	unsigned long AntennaDiversityPER[2];  // 0 ->main 1->aux
	unsigned long AntennaDiversityTxPacketCount[2];  // 0 ->main 1->aux	
	unsigned long AntennaDiversityRxPacketCount[2];
	unsigned long AntennaDiversityTrigger;
	unsigned long AntennaDiversityCount;
	unsigned long TrainCounter;
	ANT_DIVERSITY AntennaDiversityInfo;
#endif /* RT3290 */

	struct hw_setting hw_cfg;

/*****************************************************************************************/
/*      802.11 related parameters                                                        */
/*****************************************************************************************/
	/* outgoing BEACON frame buffer and corresponding TXD */
	TXWI_STRUC BeaconTxWI;
	unsigned char * BeaconBuf;
	unsigned short BeaconOffset[HW_BEACON_MAX_NUM];

	/* pre-build PS-POLL and NULL frame upon link up. for efficiency purpose. */
	HEADER_802_11 NullFrame;

#ifdef RTMP_MAC_USB
	TX_CONTEXT NullContext[2];
	TX_CONTEXT PsPollContext;
#endif /* RTMP_MAC_USB */


#ifdef UAPSD_SUPPORT
	NDIS_SPIN_LOCK UAPSDEOSPLock;	/* EOSP frame access lock use */
	bool bAPSDFlagSPSuspend;	/* 1: SP is suspended; 0: SP is not */
#endif /* UAPSD_SUPPORT */

/*=========AP=========== */
#ifdef CONFIG_AP_SUPPORT
	/* ----------------------------------------------- */
	/* AP specific configuration & operation status */
	/* used only when pAd->OpMode == OPMODE_AP */
	/* ----------------------------------------------- */
	AP_ADMIN_CONFIG ApCfg;	/* user configuration when in AP mode */
	AP_MLME_AUX ApMlmeAux;
#ifdef APCLI_SUPPORT
		MLME_AUX				ApCliMlmeAux;			/* temporary settings used during MLME state machine */
#endif /* APCLI_SUPPORT */

#ifdef WDS_SUPPORT
	WDS_TABLE WdsTab;	/* WDS table when working as an AP */
	NDIS_SPIN_LOCK WdsTabLock;
#endif /* WDS_SUPPORT */

#ifdef MBSS_SUPPORT
	bool FlgMbssInit;
#endif /* MBSS_SUPPORT */

#ifdef WDS_SUPPORT
	bool flg_wds_init;
#endif /* WDS_SUPPORT */

#ifdef APCLI_SUPPORT
	bool flg_apcli_init;
#endif /* APCLI_SUPPORT */

/*#ifdef AUTO_CH_SELECT_ENHANCE */
	PBSSINFO pBssInfoTab;
	PCHANNELINFO pChannelInfo;
/*#endif // AUTO_CH_SELECT_ENHANCE */


#endif /* CONFIG_AP_SUPPORT */

	WDS_TABLE MulTestTab;
/*=======STA=========== */

/*=======Common=========== */
	/* OP mode: either AP or STA */
	unsigned char OpMode;		/* OPMODE_STA, OPMODE_AP */

	enum RATE_ADAPT_ALG rateAlg;		/* Rate adaptation algorithm */
	
	NDIS_MEDIA_STATE IndicateMediaState;	/* Base on Indication state, default is NdisMediaStateDisConnected */
#ifdef CONFIG_MULTI_CHANNEL
	bool		Multi_Channel_Enable;
#endif /* CONFIG_MULTI_CHANNEL */


#ifdef PROFILE_STORE
	RTMP_OS_TASK 	WriteDatTask;
	bool			bWriteDat;
#endif /* PROFILE_STORE */

#ifdef CREDENTIAL_STORE	
	STA_CONNECT_INFO StaCtIf;
#endif /* CREDENTIAL_STORE */

#ifdef WSC_INCLUDED
	RTMP_OS_TASK wscTask;
	unsigned char WriteWscCfgToDatFile;
	bool WriteWscCfgToAr9DatFile;
	NDIS_SPIN_LOCK WscElmeLock;
	MLME_QUEUE_ELEM *pWscElme;

	/* WSC hardware push button function 0811 */
	bool WscHdrPshBtnFlag;	/* 1: support, read from EEPROM */
#ifdef CONFIG_AP_SUPPORT
	bool bWscDriverAutoUpdateCfg;
#endif /* CONFIG_AP_SUPPORT */
#endif /* WSC_INCLUDED */


	/* MAT related parameters */
#ifdef MAT_SUPPORT
	MAT_STRUCT MatCfg;
#endif /* MAT_SUPPORT */


	/* 
		Frequency setting for rate adaptation 
			@ra_interval: 		for baseline time interval
			@ra_fast_interval:	for quick response time interval
	*/
	unsigned int			ra_interval;
	unsigned int			ra_fast_interval;

	/* configuration: read from Registry & E2PROM */
	bool bLocalAdminMAC;	/* Use user changed MAC */
	unsigned char PermanentAddress[MAC_ADDR_LEN];	/* Factory default MAC address */
	unsigned char CurrentAddress[MAC_ADDR_LEN];	/* User changed MAC address */

	/* ------------------------------------------------------ */
	/* common configuration to both OPMODE_STA and OPMODE_AP */
	/* ------------------------------------------------------ */
	COMMON_CONFIG CommonCfg;
	MLME_STRUCT Mlme;

	/* AP needs those vaiables for site survey feature. */
	MLME_AUX MlmeAux;	/* temporary settings used during MLME state machine */
#if defined(AP_SCAN_SUPPORT) || defined(CONFIG_STA_SUPPORT)
	BSS_TABLE ScanTab;	/* store the latest SCAN result */
#endif /* defined(AP_SCAN_SUPPORT) || defined(CONFIG_STA_SUPPORT) */

	/*About MacTab, the sta driver will use #0 and #1 for multicast and AP. */
	MAC_TABLE MacTab;	/* ASIC on-chip WCID entry table.  At TX, ASIC always use key according to this on-chip table. */
	NDIS_SPIN_LOCK MacTabLock;

#ifdef DOT11_N_SUPPORT
	BA_TABLE BATable;
	NDIS_SPIN_LOCK BATabLock;
	RALINK_TIMER_STRUCT RECBATimer;
#endif /* DOT11_N_SUPPORT */

	/* DOT11_H */
	DOT11_H Dot11_H;

	/* encryption/decryption KEY tables */
	CIPHER_KEY SharedKey[HW_BEACON_MAX_NUM + MAX_P2P_NUM][4];	/* STA always use SharedKey[BSS0][0..3] */

	/* RX re-assembly buffer for fragmentation */
	FRAGMENT_FRAME FragFrame;	/* Frame storage for fragment frame */

	/* various Counters */
	COUNTER_802_3 Counters8023;	/* 802.3 counters */
	COUNTER_802_11 WlanCounters;	/* 802.11 MIB counters */
	COUNTER_RALINK RalinkCounters;	/* Ralink propriety counters */
	/* COUNTER_DRS DrsCounters;	*/ /* counters for Dynamic TX Rate Switching */
	PRIVATE_STRUC PrivateInfo;	/* Private information & counters */

	/* flags, see fRTMP_ADAPTER_xxx flags */
	unsigned long Flags;		/* Represent current device status */
	unsigned long PSFlags;		/* Power Save operation flag. */
	unsigned long MoreFlags;	/* Represent specific requirement */

	/* current TX sequence # */
	unsigned short Sequence;

	/* Control disconnect / connect event generation */
	/*+++Didn't used anymore */
	unsigned long LinkDownTime;
	/*--- */
	unsigned long LastRxRate;
	unsigned long LastTxRate;
	/*+++Used only for Station */
	bool bConfigChanged;	/* Config Change flag for the same SSID setting */
	/*--- */

	unsigned long ExtraInfo;	/* Extra information for displaying status */
	unsigned long SystemErrorBitmap;	/* b0: E2PROM version error */

	/*+++Didn't used anymore */
	unsigned long MacIcVersion;	/* MAC/BBP serial interface issue solved after ver.D */
	/*--- */

#ifdef SYSTEM_LOG_SUPPORT
	/* --------------------------- */
	/* System event log */
	/* --------------------------- */
	RT_802_11_EVENT_TABLE EventTab;
#endif /* SYSTEM_LOG_SUPPORT */

	bool HTCEnable;

	/*****************************************************************************************/
	/*      Statistic related parameters                                                     */
	/*****************************************************************************************/
#ifdef RTMP_MAC_USB
	unsigned long BulkOutDataOneSecCount;
	unsigned long BulkInDataOneSecCount;
	unsigned long BulkLastOneSecCount;	/* BulkOutDataOneSecCount + BulkInDataOneSecCount */
	unsigned long watchDogRxCnt;
	unsigned long watchDogRxOverFlowCnt;
	unsigned long watchDogTxPendingCnt[NUM_OF_TX_RING];
#endif /* RTMP_MAC_USB */

	bool bUpdateBcnCntDone;

	unsigned long macwd;
	/* ---------------------------- */
	/* DEBUG paramerts */
	/* ---------------------------- */

	/* ---------------------------- */
	/* rt2860c emulation-use Parameters */
	/* ---------------------------- */
	/*unsigned long         rtsaccu[30]; */
	/*unsigned long         ctsaccu[30]; */
	/*unsigned long         cfendaccu[30]; */
	/*unsigned long         bacontent[16]; */
	/*unsigned long         rxint[RX_RING_SIZE+1]; */
	/*unsigned char         rcvba[60]; */
	bool bLinkAdapt;
	bool bForcePrintTX;
	bool bForcePrintRX;
	/*bool               bDisablescanning;               //defined in RT2870 USB */
	bool bStaFifoTest;
	bool bProtectionTest;
	bool bHCCATest;
	bool bGenOneHCCA;
	bool bBroadComHT;
	/*+++Following add from RT2870 USB. */
	unsigned long BulkOutReq;
	unsigned long BulkOutComplete;
	unsigned long BulkOutCompleteOther;
	unsigned long BulkOutCompleteCancel;	/* seems not use now? */
	unsigned long BulkInReq;
	unsigned long BulkInComplete;
	unsigned long BulkInCompleteFail;
	/*--- */

	struct wificonf WIFItestbed;

	unsigned char		TssiGain;
#ifdef RALINK_ATE
	ATE_INFO ate;
#ifdef RTMP_MAC_USB
	bool ContinBulkOut;	/*ATE bulk out control */
	bool ContinBulkIn;	/*ATE bulk in control */
	RTMP_OS_ATOMIC BulkOutRemained;
	RTMP_OS_ATOMIC BulkInRemained;
#endif /* RTMP_MAC_USB */
#endif /* RALINK_ATE */

#ifdef DOT11_N_SUPPORT
	struct reordering_mpdu_pool mpdu_blk_pool;
#endif /* DOT11_N_SUPPORT */

	/* statistics count */

	void *iw_stats;
	void *stats;

#ifdef BLOCK_NET_IF
	BLOCK_QUEUE_ENTRY blockQueueTab[NUM_OF_TX_RING];
#endif /* BLOCK_NET_IF */

#ifdef CONFIG_AP_SUPPORT
#ifdef IGMP_SNOOP_SUPPORT
	PMULTICAST_FILTER_TABLE pMulticastFilterTable;
	unsigned char IgmpGroupTxRate;
#endif /* IGMP_SNOOP_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */


#ifdef MULTIPLE_CARD_SUPPORT
	int MC_RowID;
	char MC_FileName[256];
#endif /* MULTIPLE_CARD_SUPPORT */

	unsigned long TbttTickCount;	/* beacon timestamp work-around */
#ifdef PCI_MSI_SUPPORT
	bool HaveMsi;
#endif /* PCI_MSI_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
	RALINK_TIMER_STRUCT PeriodicTimer;

#ifdef AP_QLOAD_SUPPORT
	unsigned char FlgQloadEnable;	/* 1: any BSS WMM is enabled */
	unsigned long QloadUpTimeLast;	/* last up time */
	unsigned char QloadChanUtil;	/* last QBSS Load, unit: us */
	unsigned int QloadChanUtilTotal;	/* current QBSS Load Total */
	unsigned char QloadChanUtilBeaconCnt;	/* 1~100, default: 50 */
	unsigned char QloadChanUtilBeaconInt;	/* 1~100, default: 50 */
	unsigned int QloadLatestChannelBusyTimePri;
	unsigned int QloadLatestChannelBusyTimeSec;

	/*
	   ex: For 100ms beacon interval,
	   if the busy time in last TBTT is smaller than 5ms, QloadBusyCount[0] ++;
	   if the busy time in last TBTT is between 5 and 10ms, QloadBusyCount[1] ++;
	   ......
	   if the busy time in last TBTT is larger than 95ms, QloadBusyCount[19] ++;

	   Command: "iwpriv ra0 qload show".
	 */

/* provide busy time statistics for every TBTT */
#define QLOAD_FUNC_BUSY_TIME_STATS

/* provide busy time alarm mechanism */
/* use the function to avoid to locate in some noise environments */
#define QLOAD_FUNC_BUSY_TIME_ALARM

#ifdef QLOAD_FUNC_BUSY_TIME_STATS
#define QLOAD_BUSY_INTERVALS	20	/* partition TBTT to QLOAD_BUSY_INTERVALS */
	/* for primary channel & secondary channel */
	unsigned int QloadBusyCountPri[QLOAD_BUSY_INTERVALS];
	unsigned int QloadBusyCountSec[QLOAD_BUSY_INTERVALS];
#endif /* QLOAD_FUNC_BUSY_TIME_STATS */

#ifdef QLOAD_FUNC_BUSY_TIME_ALARM
#define QLOAD_DOES_ALARM_OCCUR(pAd)	(pAd->FlgQloadAlarmIsSuspended == TRUE)
#define QLOAD_ALARM_EVER_OCCUR(pAd) (pAd->QloadAlarmNumber > 0)
	bool FlgQloadAlarmIsSuspended;	/* 1: suspend */

	unsigned char QloadAlarmBusyTimeThreshold;	/* unit: 1/100 */
	unsigned char QloadAlarmBusyNumThreshold;	/* unit: 1 */
	unsigned char QloadAlarmBusyNum;
	unsigned char QloadAlarmDuration;	/* unit: TBTT */

	unsigned int QloadAlarmNumber;	/* total alarm times */
	bool FlgQloadAlarm;	/* 1: alarm occurs */

	/* speed up use */
	unsigned int QloadTimePeriodLast;
	unsigned int QloadBusyTimeThreshold;
#else

#define QLOAD_DOES_ALARM_OCCUR(pAd)	0
#endif /* QLOAD_FUNC_BUSY_TIME_ALARM */

#endif /* AP_QLOAD_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

	/* for detect_wmm_traffic() BE TXOP use */
	unsigned long OneSecondnonBEpackets;	/* record non BE packets per second */
	unsigned char is_on;

	/* for detect_wmm_traffic() BE/BK TXOP use */
#define TIME_BASE			(1000000/OS_HZ)
#define TIME_ONE_SECOND		(1000000/TIME_BASE)
	unsigned char flg_be_adjust;
	unsigned long be_adjust_last_time;


#ifdef WSC_INCLUDED
	/* for multiple card */
	unsigned char *pHmacData;
#endif /* WSC_INCLUDED */

#ifdef IKANOS_VX_1X0
	struct IKANOS_TX_INFO IkanosTxInfo;
	struct IKANOS_TX_INFO IkanosRxInfo[HW_BEACON_MAX_NUM + MAX_WDS_ENTRY +
					   MAX_APCLI_NUM + MAX_MESH_NUM];
#endif /* IKANOS_VX_1X0 */


#ifdef DBG_DIAGNOSE
	RtmpDiagStruct DiagStruct;
#endif /* DBG_DIAGNOSE */


	unsigned char FlgCtsEnabled;
	unsigned char PM_FlgSuspend;

#ifdef RTMP_EFUSE_SUPPORT
	bool bUseEfuse;
	bool bEEPROMFile;
	bool bFroceEEPROMBuffer;
	bool bCalFreeIC;
	unsigned char EEPROMImage[1024];
#endif /* RTMP_EFUSE_SUPPORT */


	EXT_CAP_INFO_ELEMENT ExtCapInfo;


#ifdef VENDOR_FEATURE1_SUPPORT
	unsigned char FifoUpdateDone, FifoUpdateRx;
#endif /* VENDOR_FEATURE1_SUPPORT */

	unsigned char RFICType;

#ifdef LINUX
#ifdef RT_CFG80211_SUPPORT
	void *pCfgDev;
	void *pCfg80211_CB;

	bool FlgCfg80211Scanning;
	bool FlgCfg80211Connecting;
	unsigned char Cfg80211_Alpha2[2];
#endif /* RT_CFG80211_SUPPORT */
#endif /* LINUX */

#ifdef OS_ABL_SUPPORT
#ifdef MAT_SUPPORT
	/* used in OS_ABL */
	bool (*MATPktRxNeedConvert) (RTMP_ADAPTER *pAd, PNET_DEV net_dev);

	unsigned char * (*MATEngineRxHandle)(RTMP_ADAPTER *pAd, PNDIS_PACKET pPkt, unsigned int infIdx);
#endif /* MAT_SUPPORT */
#endif /* OS_ABL_SUPPORT */

	unsigned int ContinueMemAllocFailCount;

	struct {
		int IeLen;
		unsigned char *pIe;
	} ProbeRespIE[MAX_LEN_OF_BSS_TABLE];

	/* purpose: We free all kernel resources when module is removed */
	LIST_HEADER RscTimerMemList;	/* resource timers memory */
	LIST_HEADER RscTaskMemList;	/* resource tasks memory */
	LIST_HEADER RscLockMemList;	/* resource locks memory */
	LIST_HEADER RscTaskletMemList;	/* resource tasklets memory */
	LIST_HEADER RscSemMemList;	/* resource semaphore memory */
	LIST_HEADER RscAtomicMemList;	/* resource atomic memory */

	/* purpose: Cancel all timers when module is removed */
	LIST_HEADER RscTimerCreateList;	/* timers list */

#ifdef OS_ABL_SUPPORT
#endif /* OS_ABL_SUPPORT */





#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	WOW_CFG_STRUCT WOW_Cfg; /* data structure for wake on wireless */
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */

#ifdef WLAN_SKB_RECYCLE
    struct sk_buff_head rx0_recycle;
#endif /* WLAN_SKB_RECYCLE */

#ifdef FPGA_MODE
	int tx_kick_cnt;
	int phy_rates;
	int data_phy;
	unsigned char data_bw;
	unsigned char data_mcs;
	unsigned char data_gi;
	unsigned char data_basize;
	bool fpga_on;
#endif /* FPGA_MODE */
#ifdef WFA_VHT_PF
	bool force_amsdu;
#endif /* WFA_VHT_PF */

	unsigned char bloopBackTest;
	bool bHwTxLookupRate;
	TXWI_STRUC NullTxWI;
	bool TestMulMac;

	struct MCU_CTRL MCUCtrl;

#ifdef CONFIG_MULTI_CHANNEL
	unsigned short NullBufOffset[2];
	char NullFrBuf[100];
	unsigned int NullFrLen;
	unsigned int MultiChannelFlowCtl;
	RTMP_OS_TASK MultiChannelTask;
	unsigned char MultiChannelAction;
#endif /* CONFIG_MULTI_CHANNEL */

#ifdef SINGLE_SKU_V2
	LIST_HEADER SingleSkuPwrList;
	unsigned char DefaultTargetPwr;
	char SingleSkuRatePwrDiff[18];
#endif /* SINGLE_SKU_V2 */

};

#if defined(RTMP_INTERNAL_TX_ALC) || defined(RTMP_TEMPERATURE_COMPENSATION) 
/* The offset of the Tx power tuning entry (zero-based array) */
#define TX_POWER_TUNING_ENTRY_OFFSET			30

/* The lower-bound of the Tx power tuning entry */
#define LOWERBOUND_TX_POWER_TUNING_ENTRY		-30

/* The upper-bound of the Tx power tuning entry in G band */
#define UPPERBOUND_TX_POWER_TUNING_ENTRY(__pAd)		((__pAd)->chipCap.TxAlcTxPowerUpperBound_2G)

#ifdef A_BAND_SUPPORT
/* The upper-bound of the Tx power tuning entry in A band */
#define UPPERBOUND_TX_POWER_TUNING_ENTRY_5G(__pAd)		((__pAd)->chipCap.TxAlcTxPowerUpperBound_5G)
#endif /* A_BAND_SUPPORT */

/* Temperature compensation lookup table */

#define TEMPERATURE_COMPENSATION_LOOKUP_TABLE_OFFSET	7

/* The lower/upper power delta index for the TSSI rate table */

#define LOWER_POWER_DELTA_INDEX		0
#define UPPER_POWER_DELTA_INDEX		24

/* The offset of the TSSI rate table */

#define TSSI_RATIO_TABLE_OFFSET	12


/* Get the power delta bound */

#define GET_TSSI_RATE_TABLE_INDEX(x) (((x) > UPPER_POWER_DELTA_INDEX) ? (UPPER_POWER_DELTA_INDEX) : (((x) < LOWER_POWER_DELTA_INDEX) ? (LOWER_POWER_DELTA_INDEX) : ((x))))

/* 802.11b CCK TSSI information */

typedef union _CCK_TSSI_INFO
{
#ifdef RT_BIG_ENDIAN
	struct
	{
		unsigned char	Reserved:1;
		unsigned char	ShortPreamble:1;
		unsigned char	Rate:2;
		unsigned char	Tx40MSel:2;
		unsigned char	TxType:2;
	} field;
#else
	struct
	{
		unsigned char	TxType:2;
		unsigned char	Tx40MSel:2;
		unsigned char	Rate:2;
		unsigned char	ShortPreamble:1;
		unsigned char	Reserved:1;
	} field;
#endif /* RT_BIG_ENDIAN */

	unsigned char	value;
} CCK_TSSI_INFO, *PCCK_TSSI_INFO;


/* 802.11a/g OFDM TSSI information */

typedef union _OFDM_TSSI_INFO
{
#ifdef RT_BIG_ENDIAN
	struct
	{
		unsigned char	Rate:4;
		unsigned char	Tx40MSel:2;
		unsigned char	TxType:2;
	} field;
#else
	struct
	{
		unsigned char	TxType:2;
		unsigned char	Tx40MSel:2;
		unsigned char	Rate:4;
	} field;
#endif /* RT_BIG_ENDIAN */

	unsigned char	value;
} OFDM_TSSI_INFO, *POFDM_TSSI_INFO;


/* 802.11n HT TSSI information */

typedef struct _HT_TSSI_INFO {
	union {
#ifdef RT_BIG_ENDIAN
		struct {
			unsigned char SGI:1;
			unsigned char STBC:2;
			unsigned char Aggregation:1;
			unsigned char Tx40MSel:2;
			unsigned char TxType:2;
		} field;
#else	
		struct {
			unsigned char TxType:2;
			unsigned char Tx40MSel:2;
			unsigned char Aggregation:1;
			unsigned char STBC:2;
			unsigned char SGI:1;
		} field;
#endif /* RT_BIG_ENDIAN */

		unsigned char value;
	} PartA;

	union {
#ifdef RT_BIG_ENDIAN
		struct {
			unsigned char BW:1;
			unsigned char MCS:7;
		} field;
#else	
		struct {
			unsigned char MCS:7;
			unsigned char BW:1;
		} field;
#endif /* RT_BIG_ENDIAN */
		unsigned char	value;
	} PartB;
} HT_TSSI_INFO, *PHT_TSSI_INFO;

typedef struct _TSSI_INFO_{
	unsigned char tssi_info_0;
	union {
		CCK_TSSI_INFO cck_tssi_info;
		OFDM_TSSI_INFO ofdm_tssi_info;
		HT_TSSI_INFO ht_tssi_info_1;
		unsigned char byte;
	}tssi_info_1;
	HT_TSSI_INFO ht_tssi_info_2;
}TSSI_INFO;

#endif /* RTMP_INTERNAL_TX_ALC || RTMP_TEMPERATURE_COMPENSATION */


/***************************************************************************
  *	Rx Path software control block related data structures
  **************************************************************************/
typedef struct _RX_BLK_
{
	unsigned char hw_rx_info[RXD_SIZE]; /* include "RXD_STRUC RxD" and "RXINFO_STRUC rx_info " */
	RXINFO_STRUC *pRxInfo;
#ifdef RLT_MAC
	RXFCE_INFO *pRxFceInfo;
#endif /* RLT_MAC */
	RXWI_STRUC *pRxWI;
	PHEADER_802_11 pHeader;
	PNDIS_PACKET pRxPacket;
	unsigned char *pData;
	unsigned short DataSize;
	unsigned short Flags;
	unsigned char UserPriority;	/* for calculate TKIP MIC using */
	unsigned char OpMode;	/* 0:OPMODE_STA 1:OPMODE_AP */
	unsigned char wcid;		/* copy of pRxWI->RxWIWirelessCliID */
	unsigned char mcs;
	unsigned char U2M;
#ifdef HDR_TRANS_SUPPORT
	bool	bHdrRxTrans;	/* this packet's header is translated to 802.3 by HW  */
	bool bHdrVlanTaged;	/* VLAN tag is added to this header */
	unsigned char *pTransData;
	unsigned short TransDataSize;
#endif /* HDR_TRANS_SUPPORT */
} RX_BLK;


#define RX_BLK_SET_FLAG(_pRxBlk, _flag)		(_pRxBlk->Flags |= _flag)
#define RX_BLK_TEST_FLAG(_pRxBlk, _flag)		(_pRxBlk->Flags & _flag)
#define RX_BLK_CLEAR_FLAG(_pRxBlk, _flag)	(_pRxBlk->Flags &= ~(_flag))


#define fRX_WDS			0x0001
#define fRX_AMSDU		0x0002
#define fRX_ARALINK		0x0004
#define fRX_HTC			0x0008
#define fRX_PAD			0x0010
#define fRX_AMPDU		0x0020
#define fRX_QOS			0x0040
#define fRX_INFRA		0x0080
#define fRX_EAP			0x0100
#define fRX_MESH		0x0200
#define fRX_APCLI		0x0400
#define fRX_DLS			0x0800
#define fRX_WPI			0x1000
#define fRX_P2PGO		0x2000
#define fRX_P2PCLI		0x4000

#define LENGTH_AMSDU_SUBFRAMEHEAD	14
#define LENGTH_ARALINK_SUBFRAMEHEAD	14
#define LENGTH_ARALINK_HEADER_FIELD	 2


/***************************************************************************
  *	Tx Path software control block related data structures
  **************************************************************************/
#define TX_UNKOWN_FRAME		0x00
#define TX_MCAST_FRAME			0x01
#define TX_LEGACY_FRAME		0x02
#define TX_AMPDU_FRAME		0x04
#define TX_AMSDU_FRAME		0x08
#define TX_RALINK_FRAME		0x10
#define TX_FRAG_FRAME			0x20


/*	Currently the sizeof(TX_BLK) is 148 bytes. */
typedef struct _TX_BLK_
{
	unsigned char				QueIdx;
	unsigned char				TxFrameType;				/* Indicate the Transmission type of the all frames in one batch */
	unsigned char				TotalFrameNum;				/* Total frame number want to send-out in one batch */
	unsigned short				TotalFragNum;				/* Total frame fragments required in one batch */
	unsigned short				TotalFrameLen;				/* Total length of all frames want to send-out in one batch */

	QUEUE_HEADER		TxPacketList;
	MAC_TABLE_ENTRY	*pMacEntry;					/* NULL: packet with 802.11 RA field is multicast/broadcast address */
	HTTRANSMIT_SETTING	*pTransmit;
	
	/* Following structure used for the characteristics of a specific packet. */
	PNDIS_PACKET		pPacket;
	unsigned char *				pSrcBufHeader;				/* Reference to the head of sk_buff->data */
	unsigned char *				pSrcBufData;				/* Reference to the sk_buff->data, will changed depends on hanlding progresss */
	unsigned int				SrcBufLen;					/* Length of packet payload which not including Layer 2 header */

	unsigned char *				pExtraLlcSnapEncap;			/* NULL means no extra LLC/SNAP is required */
#ifndef VENDOR_FEATURE1_SUPPORT
	/*
		Note: Can not insert any other new parameters
		between pExtraLlcSnapEncap & HeaderBuf; Or
		the start address of HeaderBuf will not be aligned by 4.

		But we can not change HeaderBuf[128] to HeaderBuf[32] because
		many codes use HeaderBuf[index].
	*/
	unsigned char				HeaderBuf[128];				/* TempBuffer for TX_INFO + TX_WI + TSO_INFO + 802.11 Header + padding + AMSDU SubHeader + LLC/SNAP */
#else
	unsigned int				HeaderBuffer[32];			/* total 128B, use unsigned int to avoid alignment problem */
	unsigned char				*HeaderBuf;
#endif /* VENDOR_FEATURE1_SUPPORT */
	unsigned char				MpduHeaderLen;				/* 802.11 header length NOT including the padding */
	unsigned char				HdrPadLen;					/* recording Header Padding Length; */
	unsigned char				apidx;						/* The interface associated to this packet */
	unsigned char				Wcid;						/* The MAC entry associated to this packet */
	unsigned char				UserPriority;				/* priority class of packet */
	unsigned char				FrameGap;					/* what kind of IFS this packet use */
	unsigned char				MpduReqNum;					/* number of fragments of this frame */
	unsigned char				TxRate;						/* TODO: Obsoleted? Should change to MCS? */
	unsigned char				CipherAlg;					/* cipher alogrithm */
	PCIPHER_KEY			pKey;
	unsigned char				KeyIdx;						/* Indicate the transmit key index */

#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT
	unsigned int				ApCliIfidx;
	PAPCLI_STRUCT		pApCliEntry;
#endif /* APCLI_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

	unsigned int				Flags;						/*See following definitions for detail. */

	/*YOU SHOULD NOT TOUCH IT! Following parameters are used for hardware-depended layer. */
	unsigned long				Priv;						/* Hardware specific value saved in here. */

#ifdef CONFIG_AP_SUPPORT
	MULTISSID_STRUCT *pMbss;
#endif /* CONFIG_AP_SUPPORT */

#ifdef TXBF_SUPPORT
	unsigned char				TxSndgPkt; /* 1: sounding 2: NDP sounding */
	unsigned char				TxNDPSndgBW;
	unsigned char				TxNDPSndgMcs;
#endif /* TXBF_SUPPORT */

#ifdef TX_PKT_SG
	PACKET_INFO pkt_info;
#endif /* TX_PKT_SG */
	unsigned char				OpMode;

#ifdef HDR_TRANS_SUPPORT
	bool				NeedTrans;	/* indicate the packet needs to do hw header translate */
#endif /* HDR_TRANS_SUPPORT */
} TX_BLK, *PTX_BLK;


#define fTX_bRtsRequired			0x0001	/* Indicate if need send RTS frame for protection. Not used in RT2860/RT2870. */
#define fTX_bAckRequired			0x0002	/* the packet need ack response */
#define fTX_bPiggyBack			0x0004	/* Legacy device use Piggback or not */
#define fTX_bHTRate				0x0008	/* allow to use HT rate */
#define fTX_bForceNonQoS		0x0010	/* force to transmit frame without WMM-QoS in HT mode */
#define fTX_bAllowFrag			0x0020	/* allow to fragment the packet, A-MPDU, A-MSDU, A-Ralink is not allowed to fragment */
#define fTX_bMoreData			0x0040	/* there are more data packets in PowerSave Queue */
#define fTX_bWMM				0x0080	/* QOS Data */
#define fTX_bClearEAPFrame		0x0100

#define	fTX_bSwEncrypt			0x0400	/* this packet need to be encrypted by software before TX */
#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT
#define fTX_bApCliPacket			0x0200
#endif /* APCLI_SUPPORT */

#ifdef WDS_SUPPORT
#define fTX_bWDSEntry			0x1000	/* Used when WDS_SUPPORT */
#endif /* WDS_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

#ifdef UAPSD_SUPPORT
#define	fTX_bWMM_UAPSD_EOSP	0x0800	/* Used when UAPSD_SUPPORT */
#endif /* UAPSD_SUPPORT */



#ifdef WAPI_SUPPORT
#define	fTX_bWPIDataFrame		0x8000	/* indicate this packet is an WPI data frame, it need to be encrypted by software */
#endif /* WAPI_SUPPORT */

#ifdef CLIENT_WDS
#define fTX_bClientWDSFrame		0x10000
#endif /* CLIENT_WDS */


#define TX_BLK_SET_FLAG(_pTxBlk, _flag)		(_pTxBlk->Flags |= _flag)
#define TX_BLK_TEST_FLAG(_pTxBlk, _flag)	(((_pTxBlk->Flags & _flag) == _flag) ? 1 : 0)
#define TX_BLK_CLEAR_FLAG(_pTxBlk, _flag)	(_pTxBlk->Flags &= ~(_flag))
	



#ifdef RT_BIG_ENDIAN
/***************************************************************************
  *	Endian conversion related functions
  **************************************************************************/
/*
	========================================================================

	Routine Description:
		Endian conversion of Tx/Rx descriptor .

	Arguments:
		pAd 	Pointer to our adapter
		pData			Pointer to Tx/Rx descriptor
		DescriptorType	Direction of the frame

	Return Value:
		None

	Note:
		Call this function when read or update descriptor
	========================================================================
*/
static inline void	RTMPWIEndianChange(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char *			pData,
	IN	unsigned long			DescriptorType)
{
	int size;
	int i;
	unsigned char TXWISize = pAd->chipCap.TXWISize;
	unsigned char RXWISize = pAd->chipCap.RXWISize;
	
	size = ((DescriptorType == TYPE_TXWI) ? TXWISize : RXWISize);
	
	if(DescriptorType == TYPE_TXWI)
	{
		*((unsigned int *)(pData)) = SWAP32(*((unsigned int *)(pData)));		/* Byte 0~3 */
		*((unsigned int *)(pData + 4)) = SWAP32(*((unsigned int *)(pData+4)));	/* Byte 4~7 */
	} 
	else
	{
		for(i=0; i < size/4 ; i++)
			*(((unsigned int *)pData) +i) = SWAP32(*(((unsigned int *)pData)+i));
	}
}




/*
	========================================================================

	Routine Description:
		Endian conversion of Tx/Rx descriptor .

	Arguments:
		pAd 	Pointer to our adapter
		pData			Pointer to Tx/Rx descriptor
		DescriptorType	Direction of the frame

	Return Value:
		None

	Note:
		Call this function when read or update descriptor
	========================================================================
*/

#ifdef RTMP_MAC_USB
static inline void RTMPDescriptorEndianChange(unsigned char *pData, unsigned long DescType)
{	
	*((unsigned int *)(pData)) = SWAP32(*((unsigned int *)(pData)));
}
#endif /* RTMP_MAC_USB */
/*
	========================================================================

	Routine Description:
		Endian conversion of all kinds of 802.11 frames .

	Arguments:
		pAd 	Pointer to our adapter
		pData			Pointer to the 802.11 frame structure
		Dir 			Direction of the frame
		FromRxDoneInt	Caller is from RxDone interrupt

	Return Value:
		None

	Note:
		Call this function when read or update buffer data
	========================================================================
*/
static inline void	RTMPFrameEndianChange(
	IN	PRTMP_ADAPTER	pAd, 
	IN	unsigned char *			pData, 
	IN	unsigned long			Dir,
	IN	bool 		FromRxDoneInt)
{
	PHEADER_802_11 pFrame;
	unsigned char *	pMacHdr;

	/* swab 16 bit fields - Frame Control field */
	if(Dir == DIR_READ)
	{
		*(unsigned short *)pData = SWAP16(*(unsigned short *)pData);
	}

	pFrame = (PHEADER_802_11) pData;
	pMacHdr = (unsigned char *) pFrame;

	/* swab 16 bit fields - Duration/ID field */
	*(unsigned short *)(pMacHdr + 2) = SWAP16(*(unsigned short *)(pMacHdr + 2));

	if (pFrame->FC.Type != BTYPE_CNTL)
	{
		/* swab 16 bit fields - Sequence Control field */
		*(unsigned short *)(pMacHdr + 22) = SWAP16(*(unsigned short *)(pMacHdr + 22));
	}

	if(pFrame->FC.Type == BTYPE_MGMT)
	{
		switch(pFrame->FC.SubType)
		{
			case SUBTYPE_ASSOC_REQ:
			case SUBTYPE_REASSOC_REQ:
				/* swab 16 bit fields - CapabilityInfo field */
				pMacHdr += sizeof(HEADER_802_11);
				*(unsigned short *)pMacHdr = SWAP16(*(unsigned short *)pMacHdr);

				/* swab 16 bit fields - Listen Interval field */
				pMacHdr += 2;
				*(unsigned short *)pMacHdr = SWAP16(*(unsigned short *)pMacHdr);
				break;

			case SUBTYPE_ASSOC_RSP:
			case SUBTYPE_REASSOC_RSP:
				/* swab 16 bit fields - CapabilityInfo field */
				pMacHdr += sizeof(HEADER_802_11);
				*(unsigned short *)pMacHdr = SWAP16(*(unsigned short *)pMacHdr);

				/* swab 16 bit fields - Status Code field */
				pMacHdr += 2;
				*(unsigned short *)pMacHdr = SWAP16(*(unsigned short *)pMacHdr);

				/* swab 16 bit fields - AID field */
				pMacHdr += 2;
				*(unsigned short *)pMacHdr = SWAP16(*(unsigned short *)pMacHdr);
				break;

			case SUBTYPE_AUTH:
				 /* When the WEP bit is on, don't do the conversion here.
					This is only shared WEP can hit this condition. 
					For AP, it shall do conversion after decryption. 
					For STA, it shall do conversion before encryption. */
				if (pFrame->FC.Wep == 1)
					break;
				else
				{
					/* swab 16 bit fields - Auth Alg No. field */
					pMacHdr += sizeof(HEADER_802_11);
					*(unsigned short *)pMacHdr = SWAP16(*(unsigned short *)pMacHdr);

					/* swab 16 bit fields - Auth Seq No. field */
					pMacHdr += 2;
					*(unsigned short *)pMacHdr = SWAP16(*(unsigned short *)pMacHdr);

					/* swab 16 bit fields - Status Code field */
					pMacHdr += 2;
					*(unsigned short *)pMacHdr = SWAP16(*(unsigned short *)pMacHdr);
				}
				break;

			case SUBTYPE_BEACON:
			case SUBTYPE_PROBE_RSP:
				/* swab 16 bit fields - BeaconInterval field */
				pMacHdr += (sizeof(HEADER_802_11) + TIMESTAMP_LEN);
				*(unsigned short *)pMacHdr = SWAP16(*(unsigned short *)pMacHdr);

				/* swab 16 bit fields - CapabilityInfo field */
				pMacHdr += sizeof(unsigned short);
				*(unsigned short *)pMacHdr = SWAP16(*(unsigned short *)pMacHdr);
				break;

			case SUBTYPE_DEAUTH:
			case SUBTYPE_DISASSOC:
				/* If the PMF is negotiated, those frames shall be encrypted */
				if(!FromRxDoneInt && pFrame->FC.Wep == 1)
					break;
				else
				{
					/* swab 16 bit fields - Reason code field */
					pMacHdr += sizeof(HEADER_802_11);
					*(unsigned short *)pMacHdr = SWAP16(*(unsigned short *)pMacHdr);
				}
				break;
		}
	}
	else if( pFrame->FC.Type == BTYPE_DATA )
	{
	}
	else if(pFrame->FC.Type == BTYPE_CNTL)
	{
		switch(pFrame->FC.SubType)
		{
			case SUBTYPE_BLOCK_ACK_REQ:
				{
					PFRAME_BA_REQ pBAReq = (PFRAME_BA_REQ)pFrame;
					*(unsigned short *)(&pBAReq->BARControl) = SWAP16(*(unsigned short *)(&pBAReq->BARControl));
					pBAReq->BAStartingSeq.word = SWAP16(pBAReq->BAStartingSeq.word);
				}
				break;
			case SUBTYPE_BLOCK_ACK:
				/* For Block Ack packet, the HT_CONTROL field is in the same offset with Addr3 */
				*(unsigned int *)(&pFrame->Addr3[0]) = SWAP32(*(unsigned int *)(&pFrame->Addr3[0]));
				break;

			case SUBTYPE_ACK:
				/*For ACK packet, the HT_CONTROL field is in the same offset with Addr2 */
				*(unsigned int *)(&pFrame->Addr2[0])=	SWAP32(*(unsigned int *)(&pFrame->Addr2[0]));
				break;
		}
	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR,("Invalid Frame Type!!!\n"));
	}

	/* swab 16 bit fields - Frame Control */
	if(Dir == DIR_WRITE)
	{
		*(unsigned short *)pData = SWAP16(*(unsigned short *)pData);
	}
}
#endif /* RT_BIG_ENDIAN */


/***************************************************************************
  *	Other static inline function definitions
  **************************************************************************/
static inline void ConvertMulticastIP2MAC(
	IN unsigned char * pIpAddr,
	IN unsigned char * *ppMacAddr, 
	IN unsigned short ProtoType)
{
	if (pIpAddr == NULL)
		return;

	if (ppMacAddr == NULL || *ppMacAddr == NULL)
		return;

	switch (ProtoType)
	{
		case ETH_P_IPV6:
/*			memset(*ppMacAddr, 0, ETH_LENGTH_OF_ADDRESS); */
			*(*ppMacAddr) = 0x33;
			*(*ppMacAddr + 1) = 0x33;
			*(*ppMacAddr + 2) = pIpAddr[12];
			*(*ppMacAddr + 3) = pIpAddr[13];
			*(*ppMacAddr + 4) = pIpAddr[14];
			*(*ppMacAddr + 5) = pIpAddr[15];
			break;

		case ETH_P_IP:
		default:
/*			memset(*ppMacAddr, 0, ETH_LENGTH_OF_ADDRESS); */
			*(*ppMacAddr) = 0x01;
			*(*ppMacAddr + 1) = 0x00;
			*(*ppMacAddr + 2) = 0x5e;
			*(*ppMacAddr + 3) = pIpAddr[1] & 0x7f;
			*(*ppMacAddr + 4) = pIpAddr[2];
			*(*ppMacAddr + 5) = pIpAddr[3];
			break;
	}

	return;
}


char *get_phymode_str(int phy_mode);
char *get_bw_str(int bandwidth);


bool RTMPCheckForHang(
	IN  NDIS_HANDLE MiniportAdapterContext);

/*
	Private routines in rtmp_init.c
*/
unsigned int RTMPAllocTxRxRingMemory(
	IN  PRTMP_ADAPTER   pAd);

#ifdef RESOURCE_PRE_ALLOC
unsigned int RTMPInitTxRxRingMemory(
	IN RTMP_ADAPTER *pAd);
#endif /* RESOURCE_PRE_ALLOC */

unsigned int	RTMPReadParametersHook(
	IN	PRTMP_ADAPTER pAd);

unsigned int	RTMPSetProfileParameters(
	IN RTMP_ADAPTER *pAd,
	IN char *		pBuffer);

int RTMPGetKeyParameter(
    IN char * key,
    OUT char * dest,
    IN int destsize,
    IN char * buffer,
    IN bool bTrimSpace);

#ifdef WSC_INCLUDED
void rtmp_read_wsc_user_parms_from_file(
	IN	PRTMP_ADAPTER pAd, 
	IN 	char *tmpbuf, 
	IN 	char *buffer);
#endif/*WSC_INCLUDED*/

void rtmp_read_multest_from_file(
	IN PRTMP_ADAPTER pAd,
	IN char * tmpbuf,
	IN char * buffer);

#ifdef SINGLE_SKU_V2
unsigned int	RTMPSetSingleSKUParameters(
	IN RTMP_ADAPTER *pAd);

void InitSkuRateDiffTable(
	IN PRTMP_ADAPTER 	pAd );

unsigned char GetSkuChannelBasePwr(
	IN PRTMP_ADAPTER 	pAd,
	IN unsigned char 			channel);

unsigned char GetSkuRatePwr(
	IN PRTMP_ADAPTER 	pAd,
	IN char 				phymode,
	IN unsigned char 			channel,
	IN unsigned char			bw);

void UpdateSkuRatePwr(
	IN PRTMP_ADAPTER 	pAd,
	IN unsigned char 			channel,
	IN unsigned char			bw,
	IN char				base_pwr);
#endif /* SINGLE_SKU_V2 */

void AP_WDS_KeyNameMakeUp(
	IN	char	*pKey,
	IN	unsigned int	KeyMaxSize,
	IN	int	KeyId);

void AsicUpdateMulTestRxWCIDTable(
	IN PRTMP_ADAPTER pAd);

MAC_TABLE_ENTRY *MulTestTableLookup(
    IN  PRTMP_ADAPTER pAd, 
    IN  unsigned char * pAddr,
	IN bool bResetIdelCount);

void AsicUpdateMulTestEncryption(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char wcid);

MAC_TABLE_ENTRY *MacTableInsertMulTestEntry(
	IN  PRTMP_ADAPTER   pAd, 
	IN  unsigned char * pAddr,
	unsigned int WdsTabIdx);



#ifdef RLT_RF
unsigned int rlt_rf_write(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char bank,
	IN unsigned char regID,
	IN unsigned char value);

unsigned int rlt_rf_read(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char bank,
	IN unsigned char regID,
	IN unsigned char *pValue);
#endif /* RLT_RF */

void NICReadEEPROMParameters(
	IN  PRTMP_ADAPTER       pAd,
	IN	char *				mac_addr);

void NICInitAsicFromEEPROM(
	IN  PRTMP_ADAPTER       pAd);

unsigned int NICInitializeAdapter(
	IN  PRTMP_ADAPTER   pAd,
	IN  bool    bHardReset);

unsigned int NICInitializeAsic(
	IN  PRTMP_ADAPTER   pAd,
	IN  bool		bHardReset);


void RTMPRingCleanUp(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned char           RingType);

void UserCfgExit(	
	IN  RTMP_ADAPTER *pAd);

void UserCfgInit(
	IN  PRTMP_ADAPTER   pAd);

unsigned int NICLoadFirmware(
	IN  PRTMP_ADAPTER   pAd);

void NICEraseFirmware(
	IN PRTMP_ADAPTER pAd);

void NICUpdateFifoStaCounters(
	IN PRTMP_ADAPTER pAd);

void NICUpdateRawCounters(
	IN  PRTMP_ADAPTER   pAd);

#ifdef MICROWAVE_OVEN_SUPPORT
void NICUpdateRxStatusCnt1(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned char   Idx);

unsigned int NICSumFalseCCACnt(
IN  PRTMP_ADAPTER   pAd);

unsigned int NICSumPLCPErrCnt(
IN  PRTMP_ADAPTER   pAd);
#endif /* MICROWAVE_OVEN_SUPPORT */

#ifdef FIFO_EXT_SUPPORT
bool NicGetMacFifoTxCnt(
	IN RTMP_ADAPTER *pAd,
	IN MAC_TABLE_ENTRY *pEntry);

void AsicFifoExtSet(
	IN RTMP_ADAPTER *pAd);

void AsicFifoExtEntryClean(
	IN RTMP_ADAPTER * pAd, 
	IN MAC_TABLE_ENTRY *pEntry);
#endif /* FIFO_EXT_SUPPORT */

void NicResetRawCounters(RTMP_ADAPTER *pAd);

void NicGetTxRawCounters(
	IN RTMP_ADAPTER *pAd,
	IN TX_STA_CNT0_STRUC *pStaTxCnt0,
	IN TX_STA_CNT1_STRUC *pStaTxCnt1);
	
void RTMPZeroMemory(
	IN  void *   pSrc,
	IN  unsigned long   Length);

unsigned long RTMPCompareMemory(
	IN  void *   pSrc1,
	IN  void *   pSrc2,
	IN  unsigned long   Length);

void RTMPMoveMemory(
	OUT void *   pDest,
	IN  void *   pSrc,
	IN  unsigned long   Length);

void AtoH(
	char *	src,
	unsigned char * dest,
	int		destlen);

unsigned char BtoH(
	char ch);

void	RTMP_TimerListAdd(
	IN	PRTMP_ADAPTER			pAd,
	IN	void					*pRsc);

void	RTMP_TimerListRelease(
	IN	PRTMP_ADAPTER			pAd);

void	RTMP_AllTimerListRelease(
	IN	PRTMP_ADAPTER			pAd);

void RTMPInitTimer(
	IN PRTMP_ADAPTER	pAd,
	IN PRALINK_TIMER_STRUCT	pTimer,
	IN void *		pTimerFunc,
	IN void *		pData,
	IN bool		Repeat);

void RTMPSetTimer(
	IN PRALINK_TIMER_STRUCT	pTimer,
	IN unsigned long		Value);


void RTMPModTimer(
	IN PRALINK_TIMER_STRUCT	pTimer,
	IN unsigned long		Value);

void RTMPCancelTimer(
	IN PRALINK_TIMER_STRUCT	pTimer,
	OUT bool		*pCancelled);

void	RTMPReleaseTimer(
	IN PRALINK_TIMER_STRUCT	pTimer,
	OUT bool		*pCancelled);

void RTMPEnableRxTx(
	IN PRTMP_ADAPTER	pAd);

void AntCfgInit(
	IN PRTMP_ADAPTER	pAd);

/* */
/* prototype in action.c */
/* */
void ActionStateMachineInit(
    IN	PRTMP_ADAPTER	pAd, 
    IN  STATE_MACHINE *S, 
    OUT STATE_MACHINE_FUNC Trans[]);

void MlmeADDBAAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem);

void MlmeDELBAAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem);

void MlmeDLSAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem);

void MlmeInvalidAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem);

void MlmeQOSAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem);

#ifdef DOT11_N_SUPPORT
void PeerAddBAReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);

void PeerAddBARspAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);

void PeerDelBAAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);

void PeerBAAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem);
#endif /* DOT11_N_SUPPORT */

void SendPSMPAction(
	IN PRTMP_ADAPTER	pAd,
	IN unsigned char			Wcid,
	IN unsigned char			Psmp);
				   
#ifdef CONFIG_AP_SUPPORT				   
void SendBeaconRequest(
	IN PRTMP_ADAPTER		pAd,
	IN unsigned char				Wcid);
#endif /* CONFIG_AP_SUPPORT */

void PeerRMAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);

void PeerPublicAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);


#ifdef CONFIG_AP_SUPPORT
void ApPublicAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);
#endif /* CONFIG_AP_SUPPORT */

void PeerBSSTranAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);

#ifdef DOT11_N_SUPPORT
void PeerHTAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);
#endif /* DOT11_N_SUPPORT */

void PeerQOSAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem);

#ifdef QOS_DLS_SUPPORT
void PeerDLSAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem);
#endif /* QOS_DLS_SUPPORT */


#ifdef DOT11_N_SUPPORT
void RECBATimerTimeout(
    IN void * SystemSpecific1, 
    IN void * FunctionContext, 
    IN void * SystemSpecific2, 
    IN void * SystemSpecific3);

void ORIBATimerTimeout(
	IN	PRTMP_ADAPTER	pAd);

void SendRefreshBAR(
	IN	PRTMP_ADAPTER	pAd,
	IN	MAC_TABLE_ENTRY	*pEntry);

#ifdef DOT11N_DRAFT3
void RTMP_11N_D3_TimerInit(
	IN PRTMP_ADAPTER pAd);

void SendBSS2040CoexistMgmtAction(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char	Wcid,
	IN	unsigned char	apidx,
	IN	unsigned char	InfoReq);

void SendNotifyBWActionFrame(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char  Wcid,
	IN unsigned char apidx);
	
bool ChannelSwitchSanityCheck(
	IN	PRTMP_ADAPTER	pAd,
	IN    unsigned char  Wcid,
	IN    unsigned char  NewChannel,
	IN    unsigned char  Secondary);

void ChannelSwitchAction(
	IN	PRTMP_ADAPTER	pAd,
	IN    unsigned char  Wcid,
	IN    unsigned char  Channel,
	IN    unsigned char  Secondary);

unsigned long BuildIntolerantChannelRep(
	IN	PRTMP_ADAPTER	pAd,
	IN    unsigned char *  pDest); 

void Update2040CoexistFrameAndNotify(
	IN	PRTMP_ADAPTER	pAd,
	IN    unsigned char  Wcid,
	IN	bool	bAddIntolerantCha);
	
void Send2040CoexistAction(
	IN	PRTMP_ADAPTER	pAd,
	IN    unsigned char  Wcid,
	IN	bool	bAddIntolerantCha);

void UpdateBssScanParm(
	IN PRTMP_ADAPTER pAd,
	IN OVERLAP_BSS_SCAN_IE APBssScan);
#endif /* DOT11N_DRAFT3 */

void AsicEnableRalinkBurstMode(
	IN PRTMP_ADAPTER pAd);

void AsicDisableRalinkBurstMode(
	IN PRTMP_ADAPTER pAd);
#endif /* DOT11_N_SUPPORT */
	
void ActHeaderInit(
    IN	PRTMP_ADAPTER	pAd, 
    IN OUT PHEADER_802_11 pHdr80211, 
    IN unsigned char * Addr1, 
    IN unsigned char * Addr2,
    IN unsigned char * Addr3);

void BarHeaderInit(
	IN	PRTMP_ADAPTER	pAd, 
	IN OUT PFRAME_BAR pCntlBar, 
	IN unsigned char * pDA,
	IN unsigned char * pSA);

void InsertActField(
	IN PRTMP_ADAPTER pAd,
	OUT unsigned char * pFrameBuf,
	OUT unsigned long * pFrameLen,
	IN unsigned char Category,
	IN unsigned char ActCode);

bool QosBADataParse(
	IN PRTMP_ADAPTER	pAd, 
	IN bool bAMSDU,
	IN unsigned char * p8023Header,
	IN unsigned char	WCID,
	IN unsigned char	TID,
	IN unsigned short Sequence,
	IN unsigned char DataOffset, 
	IN unsigned short Datasize,
	IN unsigned int   CurRxIndex);

#ifdef DOT11_N_SUPPORT
bool CntlEnqueueForRecv(
    IN	PRTMP_ADAPTER	pAd, 
	IN unsigned long Wcid, 
    IN unsigned long MsgLen, 
	IN PFRAME_BA_REQ pMsg);

void BaAutoManSwitch(
	IN	PRTMP_ADAPTER	pAd);
#endif /* DOT11_N_SUPPORT */

void HTIOTCheck(
	IN	PRTMP_ADAPTER	pAd,
	IN    unsigned char     BatRecIdx);

/* */
/* Private routines in rtmp_data.c */
/* */
bool RTMPHandleTxRingDmaDoneInterrupt(
	IN PRTMP_ADAPTER pAd,
	IN unsigned int int_reg);

void RTMPHandleMgmtRingDmaDoneInterrupt(
	IN PRTMP_ADAPTER pAd);

void RTMPHandleTBTTInterrupt(
	IN PRTMP_ADAPTER pAd);

void RTMPHandlePreTBTTInterrupt(
	IN PRTMP_ADAPTER pAd);

void RTMPHandleTwakeupInterrupt(
	IN PRTMP_ADAPTER pAd);

void RTMPHandleRxCoherentInterrupt(
	IN PRTMP_ADAPTER pAd);

#ifdef CONFIG_AP_SUPPORT
void RTMPHandleMcuInterrupt(
	IN PRTMP_ADAPTER pAd);
#endif /* CONFIG_AP_SUPPORT */


unsigned int STASendPacket(
	IN  PRTMP_ADAPTER   pAd,
	IN  PNDIS_PACKET    pPacket);

void STASendPackets(
	IN  NDIS_HANDLE     MiniportAdapterContext,
	IN  PPNDIS_PACKET   ppPacketArray,
	IN  unsigned int            NumberOfPackets);

void RTMPDeQueuePacket(
	IN RTMP_ADAPTER *pAd,
   	IN bool bIntContext,
	IN unsigned char QueIdx,
	IN int Max_Tx_Packets);

unsigned int	RTMPHardTransmit(
	IN PRTMP_ADAPTER	pAd,
	IN PNDIS_PACKET		pPacket,
	IN  unsigned char			QueIdx,
	OUT	unsigned long *pFreeTXDLeft);

unsigned int	STAHardTransmit(
	IN PRTMP_ADAPTER	pAd,
	IN TX_BLK			*pTxBlk,
	IN  unsigned char			QueIdx);

void STARxEAPOLFrameIndicate(
	IN	PRTMP_ADAPTER	pAd,
	IN	MAC_TABLE_ENTRY	*pEntry,
	IN	RX_BLK			*pRxBlk,
	IN	unsigned char			FromWhichBSSID);

unsigned int RTMPFreeTXDRequest(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned char           RingType,
	IN  unsigned char           NumberRequired,
	IN 	unsigned char *          FreeNumberIs);

unsigned int MlmeHardTransmit(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned char	QueIdx,
	IN  PNDIS_PACKET    pPacket,
	IN	bool			FlgDataQForce,
	IN	bool			FlgIsLocked);

unsigned int MlmeHardTransmitMgmtRing(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned char	QueIdx,
	IN  PNDIS_PACKET    pPacket);


unsigned short RTMPCalcDuration(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char Rate,
	IN unsigned long Size);

void RTMPWriteTxWI(
	IN RTMP_ADAPTER *pAd,
	IN TXWI_STRUC *pTxWI,
	IN bool FRAG,
	IN bool CFACK,
	IN bool InsTimestamp,
	IN bool AMPDU,
	IN bool Ack,
	IN bool NSeq, /* HW new a sequence. */
	IN unsigned char BASize,
	IN unsigned char WCID,
	IN unsigned long Length,
	IN unsigned char PID,
	IN unsigned char TID,
	IN unsigned char TxRate,
	IN unsigned char Txopmode,
	IN bool CfAck,
	IN HTTRANSMIT_SETTING *pTransmit);


void RTMPWriteTxWI_Data(
	IN RTMP_ADAPTER *pAd,
	INOUT TXWI_STRUC *pTxWI,
	IN TX_BLK *pTxBlk);

	
void RTMPWriteTxWI_Cache(
	IN RTMP_ADAPTER *pAd,
	INOUT TXWI_STRUC *pTxWI,
	IN TX_BLK *pTxBlk);

void RTMPSuspendMsduTransmission(
	IN RTMP_ADAPTER *pAd);

void RTMPResumeMsduTransmission(
	IN RTMP_ADAPTER *pAd);

unsigned int MiniportMMRequest(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char QueIdx,
	IN unsigned char *pData,
	IN unsigned int Length);

void RTMPSendNullFrame(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char TxRate,
	IN bool bQosNull,
	IN unsigned short PwrMgmt);

#ifdef CONFIG_MULTI_CHANNEL
void RTMPP2PSendNullFrame(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned char           TxRate,
	IN	bool			bQosNull,
	IN  unsigned short			PwrMgmt);
#endif /*CONFIG_MULTI_CHANNEL*/




bool RTMPFreeTXDUponTxDmaDone(
	IN PRTMP_ADAPTER    pAd, 
	IN unsigned char            QueIdx);

bool RTMPCheckEtherType(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PNDIS_PACKET	pPacket,
	IN	PMAC_TABLE_ENTRY pMacEntry,
	IN	unsigned char			OpMode,
	OUT unsigned char * pUserPriority,
	OUT unsigned char * pQueIdx);


void RTMPCckBbpTuning(
	IN	PRTMP_ADAPTER	pAd, 
	IN	unsigned int			TxRate);
/* */
/* MLME routines */
/* */

/* Asic/RF/BBP related functions */
void AsicGetTxPowerOffset(
	IN PRTMP_ADAPTER 			pAd,
	IN unsigned long *TxPwr);

void AsicGetAutoAgcOffsetForExternalTxAlc(
	IN PRTMP_ADAPTER 		pAd,
	IN char * 				pDeltaPwr,
	IN char * 				pTotalDeltaPwr,
	IN char * 				pAgcCompensate,
	IN char * 				pDeltaPowerByBbpR1);
	
#ifdef RTMP_TEMPERATURE_COMPENSATION
void AsicGetAutoAgcOffsetForTemperatureSensor(
	IN PRTMP_ADAPTER 		pAd,
	IN char *				pDeltaPwr,
	IN char *				pTotalDeltaPwr,
	IN char *				pAgcCompensate,
	IN char * 				pDeltaPowerByBbpR1);
#endif /* RTMP_TEMPERATURE_COMPENSATION */

#ifdef SINGLE_SKU
void GetSingleSkuDeltaPower(
	IN 		PRTMP_ADAPTER 	pAd,
	IN 		char * 			pTotalDeltaPower,
	INOUT	unsigned long *pSingleSKUTotalDeltaPwr,
	INOUT  	unsigned char *              	pSingleSKUBbpR1Offset);
#endif /* SINGLE_SKU*/

void AsicPercentageDeltaPower(
	IN 		PRTMP_ADAPTER 		pAd,
	IN		char				Rssi,
	INOUT	char *				pDeltaPwr,
	INOUT	char *				pDeltaPowerByBbpR1);

void AsicCompensatePowerViaBBP(
	IN 		PRTMP_ADAPTER 		pAd,
	INOUT	char *				pTotalDeltaPower);

void AsicAdjustTxPower(
	IN PRTMP_ADAPTER pAd);

void 	AsicUpdateProtect(
	IN		PRTMP_ADAPTER	pAd,
	IN 		unsigned short			OperaionMode,
	IN 		unsigned char			SetMask,
	IN		bool			bDisableBGProtect,
	IN		bool			bNonGFExist);

void AsicBBPAdjust(
	IN RTMP_ADAPTER *pAd);

void AsicSwitchChannel(
	IN  PRTMP_ADAPTER   pAd, 
	IN	unsigned char			Channel,
	IN	bool			bScan);

int AsicSetChannel(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char ch,
	IN unsigned char bw,
	IN unsigned char ext_ch,
	IN bool bScan);

void AsicLockChannel(
	IN PRTMP_ADAPTER pAd, 
	IN unsigned char Channel) ;

void AsicAntennaSelect(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned char           Channel);

void AsicResetBBPAgent(
	IN PRTMP_ADAPTER pAd);


void AsicSetBssid(
	IN  PRTMP_ADAPTER   pAd, 
	IN  unsigned char * pBssid);

void AsicSetMcastWC(
	IN PRTMP_ADAPTER pAd);


void AsicDelWcidTab(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char	Wcid);

#ifdef DOT11_N_SUPPORT
void AsicEnableRDG(
	IN PRTMP_ADAPTER pAd);

void AsicDisableRDG(
	IN PRTMP_ADAPTER pAd);
#endif /* DOT11_N_SUPPORT */

void AsicDisableSync(
	IN  PRTMP_ADAPTER   pAd);

void AsicEnableBssSync(
	IN  PRTMP_ADAPTER   pAd);

void AsicEnableIbssSync(
	IN  PRTMP_ADAPTER   pAd);

void AsicSetEdcaParm(
	IN PRTMP_ADAPTER pAd,
	IN PEDCA_PARM    pEdcaParm);

void AsicSetSlotTime(
	IN PRTMP_ADAPTER pAd,
	IN bool bUseShortSlotTime);

void AsicAddSharedKeyEntry(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char         BssIndex,
	IN unsigned char         KeyIdx,
	IN PCIPHER_KEY	 pCipherKey);

void AsicRemoveSharedKeyEntry(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char         BssIndex,
	IN unsigned char         KeyIdx);

void AsicUpdateWCIDIVEIV(
	IN PRTMP_ADAPTER pAd,
	IN unsigned short		WCID,
	IN unsigned long        uIV,
	IN unsigned long        uEIV);

void AsicUpdateRxWCIDTable(
	IN PRTMP_ADAPTER pAd,
	IN unsigned short		WCID,
	IN unsigned char *        pAddr);

void	AsicUpdateWcidAttributeEntry(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char			BssIdx,
	IN 	unsigned char		 	KeyIdx,
	IN 	unsigned char		 	CipherAlg,
	IN	unsigned char				Wcid,
	IN	unsigned char				KeyTabFlag);

void AsicAddPairwiseKeyEntry(
	IN PRTMP_ADAPTER 	pAd,
	IN unsigned char			WCID,
	IN PCIPHER_KEY		pCipherKey);

void AsicRemovePairwiseKeyEntry(
	IN PRTMP_ADAPTER  pAd,
	IN unsigned char		 Wcid);

bool AsicSendCommandToMcu(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char         Command,
	IN unsigned char         Token,
	IN unsigned char         Arg0,
	IN unsigned char         Arg1,
	IN bool in_atomic);

bool AsicSendCommandToMcuBBP(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char		 Command,
	IN unsigned char		 Token,
	IN unsigned char		 Arg0,
	IN unsigned char		 Arg1,
	IN bool		FlgIsNeedLocked);



#ifdef WAPI_SUPPORT
void AsicUpdateWAPIPN(
	IN PRTMP_ADAPTER pAd,
	IN unsigned short		 WCID,
	IN unsigned long         pn_low,
	IN unsigned long         pn_high);
#endif /* WAPI_SUPPORT */

#ifdef VCORECAL_SUPPORT
void AsicVCORecalibration(
	IN PRTMP_ADAPTER pAd);
#endif /* VCORECAL_SUPPORT */

#ifdef STREAM_MODE_SUPPORT
unsigned int StreamModeRegVal(
	IN RTMP_ADAPTER *pAd);

void AsicSetStreamMode(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char * pMacAddr,
	IN int chainIdx,
	IN bool bEnabled);

void RtmpStreamModeInit(
	IN RTMP_ADAPTER *pAd);

/*
	Update the Tx chain address
	Parameters
		pAd: The adapter data structure
		pMacAddress: The MAC address of the peer STA

	Return Value:
		None
*/
void AsicUpdateTxChainAddress(
	IN PRTMP_ADAPTER pAd, 
	IN unsigned char * pMacAddress);

int Set_StreamMode_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  char *         arg);

int Set_StreamModeMac_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  char *          arg);

int Set_StreamModeMCS_Proc(
    IN  PRTMP_ADAPTER   pAd,
    IN  char *         arg);
#endif /* STREAM_MODE_SUPPORT */

#ifdef WOW_SUPPORT
#ifdef RTMP_MAC_USB
/* For WOW, 8051 MUC send full frame */
void AsicWOWSendNullFrame(
    IN PRTMP_ADAPTER pAd,
    IN unsigned char TxRate,
    IN bool bQosNull);
 
void AsicLoadWOWFirmware(
    IN PRTMP_ADAPTER pAd,
    IN bool WOW);
#endif /* RTMP_MAC_USB */
#endif /* WOW_SUPPORT */

void MacAddrRandomBssid(
	IN  PRTMP_ADAPTER   pAd, 
	OUT unsigned char * pAddr);

void MgtMacHeaderInit(
	IN  PRTMP_ADAPTER     pAd, 
	IN OUT PHEADER_802_11 pHdr80211, 
	IN unsigned char SubType, 
	IN unsigned char ToDs, 
	IN unsigned char * pDA, 
	IN unsigned char * pBssid);

void MlmeRadioOff(
	IN PRTMP_ADAPTER pAd);

void MlmeRadioOn(
	IN PRTMP_ADAPTER pAd);


void BssTableInit(
	IN BSS_TABLE *Tab);

#ifdef DOT11_N_SUPPORT
void BATableInit(
	IN PRTMP_ADAPTER pAd,
    IN BA_TABLE *Tab);

void BATableExit(	
	IN RTMP_ADAPTER *pAd);
#endif /* DOT11_N_SUPPORT */

unsigned long BssTableSearch(
	IN BSS_TABLE *Tab, 
	IN unsigned char * pBssid,
	IN unsigned char Channel);

unsigned long BssSsidTableSearch(
	IN BSS_TABLE *Tab, 
	IN unsigned char *    pBssid,
	IN unsigned char *    pSsid,
	IN unsigned char     SsidLen,
	IN unsigned char     Channel);

unsigned long BssTableSearchWithSSID(
	IN BSS_TABLE *Tab, 
	IN unsigned char *    Bssid,
	IN unsigned char *    pSsid,
	IN unsigned char     SsidLen,
	IN unsigned char     Channel);

unsigned long BssSsidTableSearchBySSID(
	IN BSS_TABLE *Tab,
	IN unsigned char *	 pSsid,
	IN unsigned char	 SsidLen);

void BssTableDeleteEntry(
	IN OUT  PBSS_TABLE pTab, 
	IN      unsigned char * pBssid,
	IN      unsigned char Channel);

unsigned long BssTableSetEntry(
	IN PRTMP_ADAPTER pAd, 
	OUT BSS_TABLE *Tab, 
	IN BCN_IE_LIST *ie_list,
	IN char Rssi,
	IN unsigned short LengthVIE,	
	IN PNDIS_802_11_VARIABLE_IEs pVIE);


#ifdef DOT11_N_SUPPORT
void BATableInsertEntry(
    IN	PRTMP_ADAPTER	pAd, 
	IN unsigned short Aid,    
    IN unsigned short		TimeOutValue,
	IN unsigned short		StartingSeq,
    IN unsigned char TID, 
	IN unsigned char BAWinSize, 
	IN unsigned char OriginatorStatus, 
    IN bool IsRecipient);

#ifdef DOT11N_DRAFT3
void Bss2040CoexistTimeOut(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);


void  TriEventInit(
	IN	PRTMP_ADAPTER	pAd);

int TriEventTableSetEntry(
	IN	PRTMP_ADAPTER	pAd, 
	OUT TRIGGER_EVENT_TAB *Tab, 
	IN unsigned char * pBssid, 
	IN HT_CAPABILITY_IE *pHtCapability,
	IN unsigned char			HtCapabilityLen,
	IN unsigned char			RegClass,
	IN unsigned char ChannelNo);

#endif /* DOT11N_DRAFT3 */
#endif /* DOT11_N_SUPPORT */

void BssTableSsidSort(
	IN  PRTMP_ADAPTER   pAd, 
	OUT BSS_TABLE *OutTab, 
	IN  char Ssid[], 
	IN  unsigned char SsidLen);

void  BssTableSortByRssi(
	IN OUT BSS_TABLE *OutTab);

void BssCipherParse(
	IN OUT  PBSS_ENTRY  pBss);

unsigned int  MlmeQueueInit(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE *Queue);

void  MlmeQueueDestroy(
	IN MLME_QUEUE *Queue);

bool MlmeEnqueue(
	IN PRTMP_ADAPTER pAd, 
	IN unsigned long Machine, 
	IN unsigned long MsgType, 
	IN unsigned long MsgLen, 
	IN void *Msg,
	IN unsigned long Priv);

bool MlmeEnqueueForRecv(
	IN  PRTMP_ADAPTER   pAd, 
	IN unsigned long Wcid, 
	IN unsigned long TimeStampHigh, 
	IN unsigned long TimeStampLow, 
	IN unsigned char Rssi0, 
	IN unsigned char Rssi1, 
	IN unsigned char Rssi2, 
	IN unsigned char AntSel, 
	IN unsigned long MsgLen, 
	IN void * Msg,
	IN unsigned char Signal,
	IN unsigned char OpMode);

#ifdef WSC_INCLUDED
bool MlmeEnqueueForWsc(
	IN PRTMP_ADAPTER	pAd,
	IN unsigned long eventID,
	IN long senderID,
	IN unsigned long Machine,
	IN unsigned long MsgType,
	IN unsigned long MsgLen,
	IN void *Msg);
#endif /* WSC_INCLUDED */

bool MlmeDequeue(
	IN MLME_QUEUE *Queue, 
	OUT MLME_QUEUE_ELEM **Elem);

void    MlmeRestartStateMachine(
	IN  PRTMP_ADAPTER   pAd);

bool  MlmeQueueEmpty(
	IN MLME_QUEUE *Queue);

bool  MlmeQueueFull(
	IN MLME_QUEUE *Queue,
	IN unsigned char SendId);

bool  MsgTypeSubst(
	IN PRTMP_ADAPTER pAd, 
	IN PFRAME_802_11 pFrame, 
	OUT int *Machine, 
	OUT int *MsgType);

void StateMachineInit(
	IN STATE_MACHINE *Sm, 
	IN STATE_MACHINE_FUNC Trans[], 
	IN unsigned long StNr, 
	IN unsigned long MsgNr, 
	IN STATE_MACHINE_FUNC DefFunc, 
	IN unsigned long InitState, 
	IN unsigned long Base);

void StateMachineSetAction(
	IN STATE_MACHINE *S, 
	IN unsigned long St, 
	unsigned long Msg, 
	IN STATE_MACHINE_FUNC F);

void StateMachinePerformAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN STATE_MACHINE *S, 
	IN MLME_QUEUE_ELEM *Elem,
	IN unsigned long CurrState);

void Drop(
	IN  PRTMP_ADAPTER   pAd, 
	IN MLME_QUEUE_ELEM *Elem);

void AssocStateMachineInit(
	IN  PRTMP_ADAPTER   pAd, 
	IN  STATE_MACHINE *Sm, 
	OUT STATE_MACHINE_FUNC Trans[]);

void ReassocTimeout(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

void AssocTimeout(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

void DisassocTimeout(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

/*---------------------------------------------- */
void MlmeDisassocReqAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void MlmeAssocReqAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void MlmeReassocReqAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void MlmeDisassocReqAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void PeerAssocRspAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void PeerReassocRspAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void PeerDisassocAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void DisassocTimeoutAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void AssocTimeoutAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void  ReassocTimeoutAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void  Cls3errAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  unsigned char * pAddr);

void  InvalidStateWhenAssoc(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void  InvalidStateWhenReassoc(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void InvalidStateWhenDisassociate(
	IN  PRTMP_ADAPTER pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

#ifdef RTMP_MAC_USB
void MlmeCntlConfirm(
	IN PRTMP_ADAPTER pAd, 
	IN unsigned long MsgType, 
	IN unsigned short Msg);
#endif /* RTMP_MAC_USB */

void  ComposePsPoll(
	IN  PRTMP_ADAPTER   pAd);

void  ComposeNullFrame(
	IN  PRTMP_ADAPTER pAd);

void  AssocPostProc(
	IN  PRTMP_ADAPTER pAd, 
	IN  unsigned char * pAddr2, 
	IN  unsigned short CapabilityInfo, 
	IN  unsigned short Aid, 
	IN  unsigned char SupRate[], 
	IN  unsigned char SupRateLen,
	IN  unsigned char ExtRate[],
	IN  unsigned char ExtRateLen,
	IN PEDCA_PARM pEdcaParm,
	IN IE_LISTS *ie_list,
	IN HT_CAPABILITY_IE *pHtCapability,
	IN  unsigned char HtCapabilityLen,
	IN ADD_HT_INFO_IE *pAddHtInfo);

void AuthStateMachineInit(
	IN  PRTMP_ADAPTER   pAd, 
	IN PSTATE_MACHINE sm, 
	OUT STATE_MACHINE_FUNC Trans[]);

void AuthTimeout(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

void MlmeAuthReqAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void PeerAuthRspAtSeq2Action(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void PeerAuthRspAtSeq4Action(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void AuthTimeoutAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void Cls2errAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  unsigned char * pAddr);

void MlmeDeauthReqAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void InvalidStateWhenAuth(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

/*============================================= */

void AuthRspStateMachineInit(
	IN  PRTMP_ADAPTER   pAd, 
	IN  PSTATE_MACHINE Sm, 
	IN  STATE_MACHINE_FUNC Trans[]);

void PeerDeauthAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);

void PeerAuthSimpleRspGenAndSend(
	IN  PRTMP_ADAPTER   pAd, 
	IN  PHEADER_802_11  pHdr80211, 
	IN  unsigned short Alg, 
	IN  unsigned short Seq, 
	IN  unsigned short Reason, 
	IN  unsigned short Status);

/* */
/* Private routines in dls.c */
/* */
#ifdef CONFIG_AP_SUPPORT
#ifdef QOS_DLS_SUPPORT
void APDLSStateMachineInit(
    IN	PRTMP_ADAPTER		pAd, 
    IN  STATE_MACHINE		*S, 
    OUT STATE_MACHINE_FUNC	Trans[]);

void APPeerDlsReqAction(
    IN PRTMP_ADAPTER	pAd, 
    IN MLME_QUEUE_ELEM	*Elem);

void APPeerDlsRspAction(
    IN PRTMP_ADAPTER	pAd, 
    IN MLME_QUEUE_ELEM	*Elem);

void APPeerDlsTearDownAction(
    IN PRTMP_ADAPTER	pAd, 
    IN MLME_QUEUE_ELEM	*Elem);
#endif /* QOS_DLS_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */


#ifdef QOS_DLS_SUPPORT
bool PeerDlsReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN void *Msg, 
    IN unsigned long MsgLen,
    OUT unsigned char * pDA,
    OUT unsigned char * pSA,
    OUT unsigned short *pCapabilityInfo, 
    OUT unsigned short *pDlsTimeout,
    OUT unsigned char *pRatesLen,
    OUT unsigned char Rates[],
    OUT unsigned char *pHtCapabilityLen,
    OUT HT_CAPABILITY_IE *pHtCapability);

bool PeerDlsRspSanity(
    IN PRTMP_ADAPTER pAd, 
    IN void *Msg, 
    IN unsigned long MsgLen,
    OUT unsigned char * pDA,
    OUT unsigned char * pSA,
    OUT unsigned short *pCapabilityInfo, 
    OUT unsigned short *pStatus,
    OUT unsigned char *pRatesLen,
    OUT unsigned char Rates[],
    OUT unsigned char *pHtCapabilityLen,
    OUT HT_CAPABILITY_IE *pHtCapability);

bool PeerDlsTearDownSanity(
    IN PRTMP_ADAPTER pAd, 
    IN void *Msg, 
    IN unsigned long MsgLen,
    OUT unsigned char * pDA,
    OUT unsigned char * pSA,
    OUT unsigned short *pReason);
#endif /* QOS_DLS_SUPPORT */

bool PeerProbeReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN void *Msg, 
    IN unsigned long MsgLen, 
    OUT unsigned char * pAddr2,
    OUT char Ssid[], 
    OUT unsigned char *SsidLen,
    OUT bool *bRequestRssi);

/*======================================== */

void SyncStateMachineInit(
	IN  PRTMP_ADAPTER   pAd, 
	IN  STATE_MACHINE *Sm, 
	OUT STATE_MACHINE_FUNC Trans[]);

void BeaconTimeout(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

void ScanTimeout(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

void MlmeScanReqAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void InvalidStateWhenScan(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void InvalidStateWhenJoin(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void InvalidStateWhenStart(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void PeerBeacon(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void EnqueueProbeRequest(
	IN PRTMP_ADAPTER pAd);

bool ScanRunning(
		IN PRTMP_ADAPTER pAd);
/*========================================= */

void MlmeCntlInit(
	IN  PRTMP_ADAPTER   pAd, 
	IN  STATE_MACHINE *S, 
	OUT STATE_MACHINE_FUNC Trans[]);

void MlmeCntlMachinePerformAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  STATE_MACHINE *S, 
	IN  MLME_QUEUE_ELEM *Elem);

void CntlIdleProc(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void CntlOidScanProc(
	IN  PRTMP_ADAPTER pAd,
	IN  MLME_QUEUE_ELEM *Elem);

void CntlOidSsidProc(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM * Elem);

void CntlOidRTBssidProc(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM * Elem);

void CntlMlmeRoamingProc(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM * Elem);

void CntlWaitDisassocProc(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void CntlWaitJoinProc(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void CntlWaitReassocProc(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void CntlWaitStartProc(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void CntlWaitAuthProc(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void CntlWaitAuthProc2(
	IN  PRTMP_ADAPTER pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void CntlWaitAssocProc(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

#ifdef QOS_DLS_SUPPORT
void CntlOidDLSSetupProc(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem);
#endif /* QOS_DLS_SUPPORT */


void LinkUp(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned char BssType);

void LinkDown(
	IN  PRTMP_ADAPTER   pAd,
	IN  bool         IsReqFromAP);

void IterateOnBssTab(
	IN  PRTMP_ADAPTER   pAd);

void IterateOnBssTab2(
	IN  PRTMP_ADAPTER   pAd);;

void JoinParmFill(
	IN  PRTMP_ADAPTER   pAd, 
	IN  OUT MLME_JOIN_REQ_STRUCT *JoinReq, 
	IN  unsigned long BssIdx);

void AssocParmFill(
	IN  PRTMP_ADAPTER   pAd, 
	IN OUT MLME_ASSOC_REQ_STRUCT *AssocReq, 
	IN  unsigned char * pAddr, 
	IN  unsigned short CapabilityInfo, 
	IN  unsigned long Timeout, 
	IN  unsigned short ListenIntv);

void ScanParmFill(
	IN  PRTMP_ADAPTER   pAd, 
	IN  OUT MLME_SCAN_REQ_STRUCT *ScanReq, 
	IN  char Ssid[], 
	IN  unsigned char SsidLen, 
	IN  unsigned char BssType, 
	IN  unsigned char ScanType); 

void DisassocParmFill(
	IN  PRTMP_ADAPTER   pAd, 
	IN  OUT MLME_DISASSOC_REQ_STRUCT *DisassocReq, 
	IN  unsigned char * pAddr, 
	IN  unsigned short Reason);

void StartParmFill(
	IN  PRTMP_ADAPTER   pAd, 
	IN  OUT MLME_START_REQ_STRUCT *StartReq, 
	IN  char Ssid[], 
	IN  unsigned char SsidLen);

void AuthParmFill(
	IN  PRTMP_ADAPTER   pAd, 
	IN  OUT MLME_AUTH_REQ_STRUCT *AuthReq, 
	IN  unsigned char * pAddr, 
	IN  unsigned short Alg);

void EnqueuePsPoll(
	IN  PRTMP_ADAPTER   pAd);

void EnqueueBeaconFrame(
	IN  PRTMP_ADAPTER   pAd); 

void MlmeJoinReqAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void MlmeScanReqAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void MlmeStartReqAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void MlmeForceJoinReqAction(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem);

void MlmeForceScanReqAction(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem);

void ScanTimeoutAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void BeaconTimeoutAtJoinAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void PeerBeaconAtScanAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void PeerBeaconAtJoinAction(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void PeerBeacon(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void PeerProbeReqAction(
	IN  PRTMP_ADAPTER pAd, 
	IN  MLME_QUEUE_ELEM *Elem);

void ScanNextChannel(
	IN  PRTMP_ADAPTER   pAd,
	IN	unsigned char	OpMode);


unsigned long MakeIbssBeacon(
	IN  PRTMP_ADAPTER   pAd);


bool MlmeScanReqSanity(
	IN  PRTMP_ADAPTER   pAd, 
	IN  void *Msg, 
	IN  unsigned long MsgLen, 
	OUT unsigned char *BssType, 
	OUT char ssid[], 
	OUT unsigned char *SsidLen, 
	OUT unsigned char *ScanType);


bool PeerBeaconAndProbeRspSanity_Old(
	IN  PRTMP_ADAPTER   pAd, 
	IN  void *Msg, 
	IN  unsigned long MsgLen, 
	IN  unsigned char MsgChannel,
	OUT unsigned char * pAddr2, 
	OUT unsigned char * pBssid, 
	OUT char Ssid[], 
	OUT unsigned char *pSsidLen, 
	OUT unsigned char *pBssType, 
	OUT unsigned short *pBeaconPeriod, 
	OUT unsigned char *pChannel, 
	OUT unsigned char *pNewChannel, 
	OUT LARGE_INTEGER *pTimestamp, 
	OUT CF_PARM *pCfParm, 
	OUT unsigned short *pAtimWin, 
	OUT unsigned short *pCapabilityInfo, 
	OUT unsigned char *pErp,
	OUT unsigned char *pDtimCount, 
	OUT unsigned char *pDtimPeriod, 
	OUT unsigned char *pBcastFlag, 
	OUT unsigned char *pMessageToMe, 
	OUT unsigned char SupRate[],
	OUT unsigned char *pSupRateLen,
	OUT unsigned char ExtRate[],
	OUT unsigned char *pExtRateLen,
	OUT	unsigned char *pCkipFlag,
	OUT	unsigned char *pAironetCellPowerLimit,
	OUT PEDCA_PARM       pEdcaParm,
	OUT PQBSS_LOAD_PARM  pQbssLoad,
	OUT PQOS_CAPABILITY_PARM pQosCapability,
	OUT unsigned long *pRalinkIe,
	OUT unsigned char		 *pHtCapabilityLen,
	OUT HT_CAPABILITY_IE *pHtCapability,
	OUT EXT_CAP_INFO_ELEMENT *pExtCapInfo,
	OUT unsigned char		 *AddHtInfoLen,
	OUT ADD_HT_INFO_IE *AddHtInfo,
	OUT unsigned char *NewExtChannel,
	OUT unsigned short *LengthVIE,
	OUT PNDIS_802_11_VARIABLE_IEs pVIE);


bool PeerBeaconAndProbeRspSanity(
	IN PRTMP_ADAPTER pAd,
	IN void *Msg, 
	IN unsigned long MsgLen, 
	IN unsigned char  MsgChannel,
	OUT BCN_IE_LIST *ie_list,
	OUT unsigned short *LengthVIE,	
	OUT PNDIS_802_11_VARIABLE_IEs pVIE);


#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
bool PeerBeaconAndProbeRspSanity2(
	IN PRTMP_ADAPTER pAd, 
	IN void *Msg, 
	IN unsigned long MsgLen, 
	IN OVERLAP_BSS_SCAN_IE *BssScan,
	OUT unsigned char 	*RegClass);
#endif /* DOT11N_DRAFT3 */
#endif /* DOT11_N_SUPPORT */

bool PeerAddBAReqActionSanity(
    IN PRTMP_ADAPTER pAd, 
    IN void *pMsg, 
    IN unsigned long MsgLen,
	OUT unsigned char * pAddr2);

bool PeerAddBARspActionSanity(
    IN PRTMP_ADAPTER pAd, 
    IN void *pMsg, 
    IN unsigned long MsgLen);

bool PeerDelBAActionSanity(
    IN PRTMP_ADAPTER pAd, 
    IN unsigned char Wcid, 
    IN void *pMsg, 
    IN unsigned long MsgLen);

bool MlmeAssocReqSanity(
	IN  PRTMP_ADAPTER   pAd, 
	IN  void *Msg, 
	IN  unsigned long MsgLen, 
	OUT unsigned char * pApAddr, 
	OUT unsigned short *CapabilityInfo, 
	OUT unsigned long *Timeout, 
	OUT unsigned short *ListenIntv);

bool MlmeAuthReqSanity(
	IN  PRTMP_ADAPTER   pAd, 
	IN  void *Msg, 
	IN  unsigned long MsgLen, 
	OUT unsigned char * pAddr, 
	OUT unsigned long *Timeout, 
	OUT unsigned short *Alg);

bool MlmeStartReqSanity(
	IN  PRTMP_ADAPTER   pAd, 
	IN  void *Msg, 
	IN  unsigned long MsgLen, 
	OUT char Ssid[], 
	OUT unsigned char *Ssidlen);

bool PeerAuthSanity(
	IN  PRTMP_ADAPTER   pAd, 
	IN  void *Msg, 
	IN  unsigned long MsgLen, 
	OUT unsigned char * pAddr, 
	OUT unsigned short *Alg, 
	OUT unsigned short *Seq, 
	OUT unsigned short *Status, 
	OUT char ChlgText[]);

bool PeerAssocRspSanity(
	IN  PRTMP_ADAPTER   pAd, 
    IN void *pMsg, 
	IN  unsigned long MsgLen, 
	OUT unsigned char * pAddr2, 
	OUT unsigned short *pCapabilityInfo, 
	OUT unsigned short *pStatus, 
	OUT unsigned short *pAid, 
	OUT unsigned char SupRate[], 
	OUT unsigned char *pSupRateLen,
	OUT unsigned char ExtRate[],
	OUT unsigned char *pExtRateLen,
    OUT HT_CAPABILITY_IE		*pHtCapability,
    OUT ADD_HT_INFO_IE		*pAddHtInfo,	/* AP might use this additional ht info IE */
    OUT unsigned char			*pHtCapabilityLen,
    OUT unsigned char			*pAddHtInfoLen,
    OUT unsigned char			*pNewExtChannelOffset,
	OUT PEDCA_PARM pEdcaParm,
	OUT EXT_CAP_INFO_ELEMENT *pExtCapInfo,
	OUT unsigned char *pCkipFlag,
	OUT IE_LISTS *ie_list);

bool PeerDisassocSanity(
	IN  PRTMP_ADAPTER   pAd, 
	IN  void *Msg, 
	IN  unsigned long MsgLen, 
	OUT unsigned char * pAddr2, 
	OUT unsigned short *Reason);

bool PeerDeauthSanity(
	IN  PRTMP_ADAPTER   pAd, 
	IN  void *Msg, 
	IN  unsigned long MsgLen, 
	OUT unsigned char * pAddr1,
	OUT unsigned char * pAddr2, 
	OUT unsigned char * pAddr3, 
	OUT unsigned short *Reason);

bool GetTimBit(
	IN  char *Ptr, 
	IN  unsigned short Aid, 
	OUT unsigned char *TimLen, 
	OUT unsigned char *BcastFlag, 
	OUT unsigned char *DtimCount, 
	OUT unsigned char *DtimPeriod, 
	OUT unsigned char *MessageToMe);

unsigned char ChannelSanity(
	IN PRTMP_ADAPTER pAd, 
	IN unsigned char channel);

NDIS_802_11_NETWORK_TYPE NetworkTypeInUseSanity(
	IN PBSS_ENTRY pBss);

bool MlmeDelBAReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN void *Msg, 
    IN unsigned long MsgLen);

bool MlmeAddBAReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN void *Msg, 
    IN unsigned long MsgLen, 
    OUT unsigned char * pAddr2);

unsigned long MakeOutgoingFrame(
	OUT unsigned char *Buffer, 
	OUT unsigned long *Length, ...);

unsigned char RandomByte(
	IN  PRTMP_ADAPTER   pAd);

unsigned char RandomByte2(
	IN  PRTMP_ADAPTER   pAd);

void AsicUpdateAutoFallBackTable(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char *			pTxRate);



void  MlmePeriodicExec(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

void LinkDownExec(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

void LinkUpExec(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

void STAMlmePeriodicExec(
	PRTMP_ADAPTER pAd);

void MlmeAutoScan(
	IN PRTMP_ADAPTER pAd);

void MlmeAutoReconnectLastSSID(
	IN PRTMP_ADAPTER pAd);

bool MlmeValidateSSID(
	IN unsigned char * pSsid,
	IN unsigned char  SsidLen); 

void MlmeCheckForRoaming(
	IN PRTMP_ADAPTER pAd,
	IN unsigned long    Now32);

bool MlmeCheckForFastRoaming(
	IN  PRTMP_ADAPTER   pAd);

#ifdef TXBF_SUPPORT
bool MlmeTxBfAllowed(
	IN PRTMP_ADAPTER 		pAd,
	IN PMAC_TABLE_ENTRY		pEntry,
	IN struct _RTMP_RA_LEGACY_TB *pTxRate);
#endif /* TXBF_SUPPORT */

#ifdef AGS_SUPPORT
int Show_AGS_Proc(
    IN  PRTMP_ADAPTER	pAd, 
    IN  char *			arg);


#ifdef CONFIG_AP_SUPPORT
void ApMlmeDynamicTxRateSwitchingAGS(
	IN RTMP_ADAPTER *pAd,
	IN int idx);

void ApQuickResponeForRateUpExecAGS(
	IN RTMP_ADAPTER *pAd,
	IN int idx);

#endif /* CONFIG_AP_SUPPORT */
#endif /* AGS_SUPPORT */

void MlmeCalculateChannelQuality(
	IN PRTMP_ADAPTER pAd,
	IN PMAC_TABLE_ENTRY pMacEntry,
	IN unsigned long Now);

void MlmeCheckPsmChange(
	IN PRTMP_ADAPTER pAd,
	IN unsigned long    Now32);

void MlmeSetPsmBit(
	IN PRTMP_ADAPTER pAd, 
	IN unsigned short psm);

void MlmeSetTxPreamble(
	IN PRTMP_ADAPTER pAd, 
	IN unsigned short TxPreamble);

void UpdateBasicRateBitmap(
	IN	PRTMP_ADAPTER	pAd);

void MlmeUpdateTxRates(
	IN PRTMP_ADAPTER 	pAd,
	IN 	bool		 	bLinkUp,
	IN	unsigned char			apidx);

#ifdef DOT11_N_SUPPORT
void MlmeUpdateHtTxRates(
	IN PRTMP_ADAPTER 		pAd,
	IN	unsigned char				apidx);
#endif /* DOT11_N_SUPPORT */

void    RTMPCheckRates(
	IN      PRTMP_ADAPTER   pAd,
	IN OUT  unsigned char           SupRate[],
	IN OUT  unsigned char           *SupRateLen);


bool RTMPCheckHt(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char Wcid,
	INOUT HT_CAPABILITY_IE *pHtCapability,
	INOUT ADD_HT_INFO_IE *pAddHtInfo);

#ifdef DOT11_VHT_AC
bool RTMPCheckVht(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char Wcid,
	IN VHT_CAP_IE *vht_cap,
	IN VHT_OP_IE *vht_op);
#endif /* DOT11_VHT_AC */

void RTMPUpdateMlmeRate(
	IN PRTMP_ADAPTER	pAd);

char RTMPMaxRssi(
	IN PRTMP_ADAPTER	pAd,
	IN char				Rssi0,
	IN char				Rssi1,
	IN char				Rssi2);

char RTMPAvgRssi(
        IN PRTMP_ADAPTER	pAd,
        IN RSSI_SAMPLE		*pRssi);


char RTMPMinSnr(
	IN PRTMP_ADAPTER	pAd,
	IN char				Snr0,
	IN char				Snr1);

void AsicSetRxAnt(
	IN PRTMP_ADAPTER	pAd,
	IN unsigned char			Ant);

#ifdef RTMP_EFUSE_SUPPORT
int set_eFuseGetFreeBlockCount_Proc(  
   	IN	PRTMP_ADAPTER	pAd,
	IN	char *			arg);

int set_eFusedump_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	char *			arg);

int set_eFuseLoadFromBin_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	char *			arg);

void eFusePhysicalReadRegisters( 
	IN	PRTMP_ADAPTER	pAd, 
	IN	unsigned short Offset, 
	IN	unsigned short Length, 
	OUT	unsigned short* pData);

int RtmpEfuseSupportCheck(
	IN RTMP_ADAPTER *pAd);

#ifdef RALINK_ATE
int set_eFuseBufferModeWriteBack_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	char *			arg);
#endif /* RALINK_ATE */
#endif /* RTMP_EFUSE_SUPPORT */





void AsicEvaluateRxAnt(
	IN PRTMP_ADAPTER	pAd);

void AsicRxAntEvalTimeout(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

void APSDPeriodicExec(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

bool RTMPCheckEntryEnableAutoRateSwitch(
	IN PRTMP_ADAPTER    pAd,
	IN PMAC_TABLE_ENTRY	pEntry);

unsigned char RTMPStaFixedTxMode(
	IN PRTMP_ADAPTER    pAd,
	IN PMAC_TABLE_ENTRY	pEntry);

void RTMPUpdateLegacyTxSetting(
		unsigned char				fixed_tx_mode,
		PMAC_TABLE_ENTRY	pEntry);

bool RTMPAutoRateSwitchCheck(
	IN PRTMP_ADAPTER    pAd);

unsigned int MlmeInit(
	IN  PRTMP_ADAPTER   pAd);



#ifdef RTMP_TEMPERATURE_COMPENSATION
void InitLookupTable(
	IN PRTMP_ADAPTER pAd);
#endif /* RTMP_TEMPERATURE_COMPENSATION */

void MlmeHandler(
	IN  PRTMP_ADAPTER   pAd);

void MlmeHalt(
	IN  PRTMP_ADAPTER   pAd);

void MlmeResetRalinkCounters(
	IN  PRTMP_ADAPTER   pAd);

void BuildChannelList(
	IN PRTMP_ADAPTER pAd);

unsigned char FirstChannel(
	IN  PRTMP_ADAPTER   pAd);

unsigned char NextChannel(
	IN  PRTMP_ADAPTER   pAd, 
	IN  unsigned char channel);

void ChangeToCellPowerLimit(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char         AironetCellPowerLimit);

/* */
/* Prototypes of function definition in cmm_tkip.c */
/* */
void    RTMPInitMICEngine(
	IN  PRTMP_ADAPTER   pAd,    
	IN  unsigned char *          pKey,
	IN  unsigned char *          pDA,
	IN  unsigned char *          pSA,
	IN  unsigned char           UserPriority,
	IN  unsigned char *          pMICKey);

bool RTMPTkipCompareMICValue(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned char *          pSrc,
	IN  unsigned char *          pDA,
	IN  unsigned char *          pSA,
	IN  unsigned char *          pMICKey,
	IN	unsigned char			UserPriority,
	IN  unsigned int            Len);

void    RTMPCalculateMICValue(
	IN  PRTMP_ADAPTER   pAd,
	IN  PNDIS_PACKET    pPacket,
	IN  unsigned char *          pEncap,
	IN  PCIPHER_KEY     pKey,
	IN	unsigned char			apidx);

void    RTMPTkipAppendByte( 
	IN  PTKIP_KEY_INFO  pTkip,  
	IN  unsigned char           uChar);

void    RTMPTkipAppend( 
	IN  PTKIP_KEY_INFO  pTkip,  
	IN  unsigned char *          pSrc,
	IN  unsigned int            nBytes);

void    RTMPTkipGetMIC( 
	IN  PTKIP_KEY_INFO  pTkip);

/* */
/* Prototypes of function definition in cmm_cfg.c */
/* */
int RT_CfgSetCountryRegion(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg,
	IN int				band);

int RT_CfgSetWirelessMode(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

unsigned char cfgmode_2_wmode(unsigned char cfg_mode);
unsigned char wmode_2_cfgmode(unsigned char wmode);
unsigned char *wmode_2_str(unsigned char wmode);

#ifdef CONFIG_AP_SUPPORT
#ifdef MBSS_SUPPORT
int RT_CfgSetMbssWirelessMode(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);
#endif /* MBSS_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

int RT_CfgSetShortSlot(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	RT_CfgSetWepKey(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			keyString,
	IN	CIPHER_KEY		*pSharedKey,
	IN	int				keyIdx);

int RT_CfgSetWPAPSKKey(
	IN RTMP_ADAPTER	*pAd, 
	IN char *		keyString,
	IN int			keyStringLen,
	IN unsigned char		*pHashStr,
	IN int			hashStrLen,
	OUT unsigned char *		pPMKBuf);

int	RT_CfgSetFixedTxPhyMode(
	IN	char *			arg);

int	RT_CfgSetMacAddress(
	IN 	PRTMP_ADAPTER 	pAd,
	IN	char *			arg);

int	RT_CfgSetTxMCSProc(
	IN	char *			arg,
	OUT	bool			*pAutoRate);

int	RT_CfgSetAutoFallBack(
	IN 	PRTMP_ADAPTER 	pAd,
	IN	char *			arg);

#ifdef WSC_INCLUDED
int	RT_CfgSetWscPinCode(
	IN RTMP_ADAPTER *pAd,
	IN char *		pPinCodeStr,
	OUT PWSC_CTRL   pWscControl);
#endif /* WSC_INCLUDED */

int	Set_Antenna_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);



#ifdef MICROWAVE_OVEN_SUPPORT
int Set_MO_FalseCCATh_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);
#endif /* MICROWAVE_OVEN_SUPPORT */

/* */
/* Prototypes of function definition in cmm_info.c */
/* */
unsigned int RTMPWPARemoveKeyProc(
	IN  PRTMP_ADAPTER   pAd,
	IN  void *           pBuf);

void    RTMPWPARemoveAllKeys(
	IN  PRTMP_ADAPTER   pAd);

bool RTMPCheckStrPrintAble(
    IN  char *pInPutStr, 
    IN  unsigned char strLen);
    
void    RTMPSetPhyMode(
	IN  PRTMP_ADAPTER   pAd,
	IN  unsigned long phymode);

void	RTMPUpdateHTIE(
	IN	RT_HT_CAPABILITY	*pRtHt,
	IN		unsigned char				*pMcsSet,
	OUT		HT_CAPABILITY_IE *pHtCapability,
	OUT		ADD_HT_INFO_IE		*pAddHtInfo);

void	RTMPAddWcidAttributeEntry(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char			BssIdx,
	IN 	unsigned char		 	KeyIdx,
	IN 	unsigned char		 	CipherAlg,
	IN 	MAC_TABLE_ENTRY *pEntry);

char * GetEncryptType(
	char enc);

char * GetAuthMode(
	char auth);

#ifdef DOT11_N_SUPPORT
void	RTMPSetHT(
	IN	PRTMP_ADAPTER	pAd,
	IN	OID_SET_HT_PHYMODE *pHTPhyMode);

void	RTMPSetIndividualHT(
	IN	PRTMP_ADAPTER		pAd,
	IN	unsigned char				apidx);

unsigned char get_cent_ch_by_htinfo(
	RTMP_ADAPTER *pAd,
	ADD_HT_INFO_IE *ht_op,
	HT_CAPABILITY_IE *ht_cap);

int get_ht_cent_ch(RTMP_ADAPTER *pAd, unsigned char *rf_bw, unsigned char *ext_ch);
int ht_mode_adjust(RTMP_ADAPTER *pAd, MAC_TABLE_ENTRY *pEntry, HT_CAPABILITY_IE *peer, RT_HT_CAPABILITY *my);
int set_ht_fixed_mcs(RTMP_ADAPTER *pAd, MAC_TABLE_ENTRY *pEntry, unsigned char fixed_mcs, unsigned char mcs_bound);
int get_ht_max_mcs(RTMP_ADAPTER *pAd, unsigned char *desire_mcs, unsigned char *cap_mcs);
#endif /* DOT11_N_SUPPORT */

void RTMPDisableDesiredHtInfo(
	IN	PRTMP_ADAPTER	pAd);

#ifdef SYSTEM_LOG_SUPPORT
void RtmpDrvSendWirelessEvent(
	IN	void			*pAdSrc,
	IN	unsigned short			Event_flag,
	IN	unsigned char * 			pAddr,
	IN  unsigned char			BssIdx,
	IN	char			Rssi);
#else
#define RtmpDrvSendWirelessEvent(_pAd, _Event_flag, _pAddr, _BssIdx, _Rssi)
#endif /* SYSTEM_LOG_SUPPORT */
	
char    ConvertToRssi(
	IN PRTMP_ADAPTER  pAd,
	IN char				Rssi,
	IN unsigned char    RssiNumber,
	IN unsigned char AntSel,
	IN unsigned char BW);

char    ConvertToSnr(
	IN PRTMP_ADAPTER  pAd,
	IN unsigned char				Snr);

#ifdef DOT11N_DRAFT3
void BuildEffectedChannelList(
	IN PRTMP_ADAPTER pAd);


void DeleteEffectedChannelList(
	IN PRTMP_ADAPTER pAd);

void CntlChannelWidth(
	IN PRTMP_ADAPTER	pAd,
	IN unsigned char			PrimaryChannel,
	IN unsigned char			CentralChannel,	
	IN unsigned char			ChannelWidth,
	IN unsigned char			SecondaryChannelOffset);

#endif /* DOT11N_DRAFT3 */


void APAsicEvaluateRxAnt(
	IN PRTMP_ADAPTER	pAd);


void APAsicRxAntEvalTimeout(
	IN PRTMP_ADAPTER	pAd);


/* */
/* function prototype in ap_wpa.c */
/* */
void RTMPGetTxTscFromAsic(
	IN  PRTMP_ADAPTER   pAd,
	IN	unsigned char			apidx,
	OUT	unsigned char *			pTxTsc);

MAC_TABLE_ENTRY *PACInquiry(
	IN  PRTMP_ADAPTER   pAd, 
	IN  unsigned long           Wcid);

unsigned int	APValidateRSNIE(
	IN PRTMP_ADAPTER    pAd,
	IN PMAC_TABLE_ENTRY pEntry,
	IN unsigned char *			pRsnIe,
	IN unsigned char			rsnie_len);

void HandleCounterMeasure(
	IN PRTMP_ADAPTER pAd, 
	IN MAC_TABLE_ENTRY  *pEntry);

void WPAStart4WayHS(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MAC_TABLE_ENTRY *pEntry,
	IN	unsigned long			TimeInterval);

void WPAStart2WayGroupHS(
	IN  PRTMP_ADAPTER   pAd, 
	IN  MAC_TABLE_ENTRY *pEntry);

void PeerPairMsg1Action(
	IN PRTMP_ADAPTER pAd, 
	IN MAC_TABLE_ENTRY  *pEntry,
	IN MLME_QUEUE_ELEM *Elem);

void PeerPairMsg2Action(
	IN PRTMP_ADAPTER pAd, 
	IN MAC_TABLE_ENTRY  *pEntry,
	IN MLME_QUEUE_ELEM *Elem);

void PeerPairMsg3Action(
	IN PRTMP_ADAPTER pAd, 
	IN MAC_TABLE_ENTRY  *pEntry,
	IN MLME_QUEUE_ELEM *Elem);

void PeerPairMsg4Action(
	IN PRTMP_ADAPTER pAd, 
	IN MAC_TABLE_ENTRY  *pEntry,
	IN MLME_QUEUE_ELEM *Elem);

void PeerGroupMsg1Action(
	IN  PRTMP_ADAPTER    pAd, 
	IN  PMAC_TABLE_ENTRY pEntry,
    IN  MLME_QUEUE_ELEM  *Elem);

void PeerGroupMsg2Action(
	IN  PRTMP_ADAPTER    pAd, 
	IN  PMAC_TABLE_ENTRY pEntry,
	IN  void             *Msg,
	IN  unsigned int             MsgLen);

void CMTimerExec(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

void WPARetryExec(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

#ifdef TXBF_SUPPORT
void eTxBfProbeTimerExec(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2,
	IN void * SystemSpecific3);
#endif /* TXBF_SUPPORT */

void EnqueueStartForPSKExec(
    IN void * SystemSpecific1, 
    IN void * FunctionContext, 
    IN void * SystemSpecific2, 
    IN void * SystemSpecific3); 

void RTMPHandleSTAKey(
    IN PRTMP_ADAPTER    pAdapter, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN MLME_QUEUE_ELEM  *Elem);

void MlmeDeAuthAction(
	IN  PRTMP_ADAPTER    pAd, 
	IN  PMAC_TABLE_ENTRY pEntry,
	IN  unsigned short           Reason,
	IN  bool          bDataFrameFirst);


void GREKEYPeriodicExec(
	IN  void *   SystemSpecific1, 
	IN  void *   FunctionContext, 
	IN  void *   SystemSpecific2, 
	IN  void *   SystemSpecific3);

void AES_128_CMAC(
	IN	unsigned char *	key,
	IN	unsigned char *	input,
	IN	int		len,
	OUT	unsigned char *	mac);

#ifdef DOT1X_SUPPORT
void    WpaSend(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  unsigned char *          pPacket,
    IN  unsigned long           Len);

void RTMPAddPMKIDCache(
	IN  PRTMP_ADAPTER   		pAd,
	IN	int						apidx,
	IN	unsigned char *				pAddr,
	IN	unsigned char					*PMKID,
	IN	unsigned char					*PMK);

int RTMPSearchPMKIDCache(
	IN  PRTMP_ADAPTER   pAd,
	IN	int				apidx,
	IN	unsigned char *		pAddr);

void RTMPDeletePMKIDCache(
	IN  PRTMP_ADAPTER   pAd,
	IN	int				apidx,
	IN  int				idx);

void RTMPMaintainPMKIDCache(
	IN  PRTMP_ADAPTER   pAd);
#else
#define RTMPMaintainPMKIDCache(_pAd)
#endif /* DOT1X_SUPPORT */

#ifdef RESOURCE_PRE_ALLOC
void RTMPResetTxRxRingMemory(
	IN  RTMP_ADAPTER   *pAd);
#endif /* RESOURCE_PRE_ALLOC */

void RTMPFreeTxRxRingMemory(
    IN  PRTMP_ADAPTER   pAd);

bool RTMP_FillTxBlkInfo(
	IN RTMP_ADAPTER *pAd,
	IN TX_BLK *pTxBlk);

 void announce_802_3_packet(
	IN	void			*pAdSrc,
	IN	PNDIS_PACKET	pPacket,
	IN	unsigned char			OpMode);

#ifdef DOT11_N_SUPPORT
unsigned int BA_Reorder_AMSDU_Annnounce(
	IN	PRTMP_ADAPTER	pAd, 	
	IN	PNDIS_PACKET	pPacket,
	IN	unsigned char			OpMode);
#endif /* DOT11_N_SUPPORT */

PNET_DEV get_netdev_from_bssid(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char			FromWhichBSSID);


#ifdef DOT11_N_SUPPORT
void ba_flush_reordering_timeout_mpdus(
	IN PRTMP_ADAPTER	pAd, 
	IN PBA_REC_ENTRY	pBAEntry,
	IN unsigned long			Now32);


void BAOriSessionSetUp(
			IN PRTMP_ADAPTER    pAd, 
			IN MAC_TABLE_ENTRY	*pEntry,
			IN unsigned char			TID,
			IN unsigned short			TimeOut,
			IN unsigned long			DelayTime,
			IN bool		isForced);

void BASessionTearDownALL(
	IN OUT	PRTMP_ADAPTER pAd, 
	IN		unsigned char Wcid);

void BAOriSessionTearDown(
	IN OUT	PRTMP_ADAPTER	pAd, 
	IN		unsigned char			Wcid,
	IN		unsigned char			TID,
	IN		bool			bPassive,
	IN		bool			bForceSend);

void BARecSessionTearDown(
	IN OUT	PRTMP_ADAPTER	pAd, 
	IN		unsigned char			Wcid,
	IN		unsigned char			TID,
	IN		bool			bPassive);
#endif /* DOT11_N_SUPPORT */

bool ba_reordering_resource_init(PRTMP_ADAPTER pAd, int num);
void ba_reordering_resource_release(PRTMP_ADAPTER pAd);

int ComputeChecksum(
	IN unsigned int PIN);

unsigned int GenerateWpsPinCode(
	IN	PRTMP_ADAPTER	pAd,
    IN  bool         bFromApcli,
	IN	unsigned char	apidx);

#ifdef WSC_INCLUDED
int	Set_WscGenPinCode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int Set_WscVendorPinCode_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  char *          arg);

#ifdef WSC_AP_SUPPORT
void RTMPIoctlSetWSCOOB(IN PRTMP_ADAPTER pAd);
#endif

#ifdef WSC_STA_SUPPORT
void CntlWscIterate(
	IN PRTMP_ADAPTER pAd);

unsigned short WscGetAuthTypeFromStr(
    IN  char *          arg);

unsigned short WscGetEncrypTypeFromStr(
    IN  char *          arg);
#endif /* WSC_STA_SUPPORT */
/* */
/* prototype in wsc.c */
/* */
bool	WscMsgTypeSubst(
	IN	unsigned char	EAPType,
	IN	unsigned char	EAPCode,
	OUT	int	    *MsgType);

void    WscStateMachineInit(
	IN	PRTMP_ADAPTER		pAd, 
	IN	STATE_MACHINE		*S, 
	OUT STATE_MACHINE_FUNC Trans[]);


void    WscEAPOLStartAction(
    IN  PRTMP_ADAPTER    pAd, 
    IN  MLME_QUEUE_ELEM  *Elem);

void    WscEAPAction(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	MLME_QUEUE_ELEM *Elem);

void    WscEapEnrolleeAction(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	MLME_QUEUE_ELEM	*Elem,
	IN  unsigned char	        MsgType,
	IN  MAC_TABLE_ENTRY *pEntry,
	IN  PWSC_CTRL       pWscControl);

#ifdef CONFIG_AP_SUPPORT
void    WscEapApProxyAction(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	MLME_QUEUE_ELEM	*Elem,
	IN  unsigned char	        MsgType,
	IN  MAC_TABLE_ENTRY *pEntry,
	IN  PWSC_CTRL       pWscControl);
#endif /* CONFIG_AP_SUPPORT */

void    WscEapRegistrarAction(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	MLME_QUEUE_ELEM	*Elem,
	IN  unsigned char	        MsgType,
	IN  MAC_TABLE_ENTRY *pEntry,
	IN  PWSC_CTRL       pWscControl);
	
void    WscEAPOLTimeOutAction(
    IN  void * SystemSpecific1, 
    IN  void * FunctionContext, 
    IN  void * SystemSpecific2, 
    IN  void * SystemSpecific3);

void    Wsc2MinsTimeOutAction(
    IN  void * SystemSpecific1, 
    IN  void * FunctionContext, 
    IN  void * SystemSpecific2, 
    IN  void * SystemSpecific3);

unsigned char	WscRxMsgType(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PMLME_QUEUE_ELEM	pElem);

void	WscInitRegistrarPair(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	IN  unsigned char				apidx);

void	WscSendEapReqId(
	IN	PRTMP_ADAPTER		pAd,
	IN	PMAC_TABLE_ENTRY	pEntry,
	IN  unsigned char				CurOpMode);

void    WscSendEapolStart(
	IN	PRTMP_ADAPTER	pAd,
	IN  unsigned char *          pBssid,
	IN  unsigned char			CurOpMode);

void	WscSendEapRspId(
	IN	PRTMP_ADAPTER		pAd,
	IN  PMAC_TABLE_ENTRY    pEntry,
	IN  PWSC_CTRL           pWscControl);

void	WscMacHeaderInit(
	IN	PRTMP_ADAPTER	pAd, 
	IN OUT	PHEADER_802_11	Hdr, 
	IN	unsigned char * 			pAddr1,
	IN  unsigned char *          pBSSID,
	IN  bool         bFromApCli);

void	WscSendMessage(
	IN	PRTMP_ADAPTER		pAdapter, 
	IN  unsigned char               OpCode,
	IN  unsigned char *				pData,
	IN  int					Len,
	IN  PWSC_CTRL           pWscControl,
    IN  unsigned char               OpMode,	/* 0: AP Mode, 1: AP Client Mode, 2: STA Mode */
    IN  unsigned char               EapType);

void	WscSendEapReqAck(
	IN	PRTMP_ADAPTER		pAdapter, 
	IN	PMAC_TABLE_ENTRY	pEntry);
	
void	WscSendEapReqDone(
	IN	PRTMP_ADAPTER		pAdapter, 
	IN	PMLME_QUEUE_ELEM	pElem);

void	WscSendEapFail(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	IN  bool				bSendDeAuth);

void WscM2DTimeOutAction(
    IN  void * SystemSpecific1, 
    IN  void * FunctionContext, 
    IN  void * SystemSpecific2, 
    IN  void * SystemSpecific3);

void WscUPnPMsgTimeOutAction(
	IN  void * SystemSpecific1, 
    IN  void * FunctionContext, 
    IN  void * SystemSpecific2, 
    IN  void * SystemSpecific3);

int WscSendUPnPConfReqMsg(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char apIdx,
	IN unsigned char * ssidStr,
	IN unsigned char * macAddr,
	IN int	  Status,
	IN unsigned int   eventID,
	IN unsigned char  CurOpMode);

	
int WscSendUPnPMessage(
	IN	PRTMP_ADAPTER		pAd,
	IN	unsigned char				apIdx,
	IN	unsigned short				msgType,
	IN	unsigned short				msgSubType,
	IN	unsigned char *				pData,
	IN	int					dataLen,
	IN	unsigned int				eventID,
	IN	unsigned int				toIPAddr,
	IN	unsigned char *				pMACAddr,
	IN  unsigned char				CurOpMode);

void WscUPnPErrHandle(
	IN PRTMP_ADAPTER 	pAd,
	IN PWSC_CTRL		pWscControl,
	IN unsigned int 			eventID);

void    WscBuildBeaconIE(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	unsigned char b_configured,
	IN	bool b_selRegistrar,
	IN	unsigned short devPwdId,
	IN	unsigned short selRegCfgMethods,
	IN  unsigned char apidx,
	IN  unsigned char *pAuthorizedMACs,
	IN  unsigned char  	AuthorizedMACsLen,
	IN  unsigned char	CurOpMode);

void    WscBuildProbeRespIE(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	unsigned char respType,
	IN	unsigned char scState,
	IN	bool b_selRegistrar,
	IN	unsigned short devPwdId,
	IN	unsigned short selRegCfgMethods,
	IN  unsigned char apidx,
	IN  unsigned char *pAuthorizedMACs,
	IN  int   AuthorizedMACsLen,
	IN  unsigned char	CurOpMode);


#ifdef CONFIG_AP_SUPPORT
void WscBuildAssocRespIE(
	IN	PRTMP_ADAPTER	pAd,
	IN  unsigned char 			ApIdx,
	IN  unsigned char			Reason,
	OUT	unsigned char *			pOutBuf,
	OUT	unsigned char *			pIeLen);

void	WscSelectedRegistrar(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char *	RegInfo,
	IN	unsigned int	length,
	IN  unsigned char 	apidx);

void    WscInformFromWPA(
    IN  PMAC_TABLE_ENTRY    pEntry);
#endif /* CONFIG_AP_SUPPORT */


void    WscProfileRetryTimeout(
	IN  void * SystemSpecific1,
	IN  void * FunctionContext,
	IN  void * SystemSpecific2,
	IN  void * SystemSpecific3);

void    WscPBCTimeOutAction(
    IN  void * SystemSpecific1, 
    IN  void * FunctionContext, 
    IN  void * SystemSpecific2, 
    IN  void * SystemSpecific3);

void    WscScanTimeOutAction(
    IN  void * SystemSpecific1,
    IN  void * FunctionContext,
    IN  void * SystemSpecific2,
    IN  void * SystemSpecific3);


int WscGenerateUUID(
	RTMP_ADAPTER	*pAd, 
	unsigned char 			*uuidHexStr, 
	unsigned char 			*uuidAscStr, 
	int 			apIdx,
	bool			bUseCurrentTime);
	
void WscStop(
	IN	PRTMP_ADAPTER	pAd,
#ifdef CONFIG_AP_SUPPORT
    IN  bool         bFromApcli,
#endif /* CONFIG_AP_SUPPORT */
	IN  PWSC_CTRL       pWscControl);

void WscInit(
	IN	PRTMP_ADAPTER	pAd,
    IN  bool         bFromApcli,	
	IN  unsigned char       	BssIndex);

bool	ValidateChecksum(
	IN unsigned int PIN);

unsigned int	WscRandomGen4digitPinCode(
	IN	PRTMP_ADAPTER	pAd);

unsigned int WscRandomGeneratePinCode(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char	apidx);

int BuildMessageM1(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	OUT	void *pbuf);

int BuildMessageM2(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	OUT	void *pbuf);

int BuildMessageM2D(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	OUT	void *pbuf);

int BuildMessageM3(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	OUT	void *pbuf);

int BuildMessageM4(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	OUT	void *pbuf);

int BuildMessageM5(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	OUT	void *pbuf);

int BuildMessageM6(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	OUT	void *pbuf);

int BuildMessageM7(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	OUT	void *pbuf);

int BuildMessageM8(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	OUT	void *pbuf);

int BuildMessageDONE(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	OUT	void *pbuf);

int BuildMessageACK(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	OUT	void *pbuf);

int BuildMessageNACK(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	OUT	void *pbuf);

int ProcessMessageM1(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	IN	void *precv,
	IN	int Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM2(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL		pWscControl,
	IN	void *precv,
	IN	int Length,
	IN  unsigned char			apidx,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM2D(
	IN	PRTMP_ADAPTER		pAdapter,
	IN	void *precv,
	IN	int Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM3(
	IN	PRTMP_ADAPTER		pAdapter,
	IN	void *precv,
	IN	int Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM4(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	IN	void *precv,
	IN	int Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM5(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	IN	void *precv,
	IN	int Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM6(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	IN	void *precv,
	IN	int Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM7(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL           pWscControl,
	IN	void *precv,
	IN	int Length,
	OUT	PWSC_REG_DATA pReg);

int ProcessMessageM8(
	IN	PRTMP_ADAPTER		pAdapter,
	IN	void *precv,
	IN	int Length,
	IN  PWSC_CTRL       pWscControl);

unsigned short  WscGetAuthType(
    IN  NDIS_802_11_AUTHENTICATION_MODE authType);

unsigned short  WscGetEncryType(
    IN  NDIS_802_11_WEP_STATUS encryType);

unsigned int WscThreadInit(
	IN RTMP_ADAPTER *pAd);

bool WscThreadExit(
	IN RTMP_ADAPTER *pAd);

int     AppendWSCTLV(
    IN  unsigned short index, 
    OUT unsigned char * obuf, 
    IN  unsigned char * ibuf, 
    IN  unsigned short varlen);

void    WscGetRegDataPIN(
    IN  PRTMP_ADAPTER   pAd,
    IN  unsigned int            PinCode,
    IN  PWSC_CTRL       pWscControl);

void    WscPushPBCAction(
	IN	PRTMP_ADAPTER	pAd,
	IN  PWSC_CTRL   	pWscControl);

void    WscScanExec(
	IN	PRTMP_ADAPTER	pAd,
	IN  PWSC_CTRL   	pWscControl);

bool WscPBCExec(
	IN	PRTMP_ADAPTER	pAd,
	IN  bool			bFromM2,
	IN  PWSC_CTRL       pWscControl);

void    WscPBCBssTableSort(
	IN	PRTMP_ADAPTER	pAd,
	IN  PWSC_CTRL       pWscControl);

void	WscGenRandomKey(
	IN  	PRTMP_ADAPTER	pAd,
	IN  	PWSC_CTRL       pWscControl,
	INOUT	unsigned char *			pKey,
	INOUT	unsigned short *			pKeyLen);

void	WscCreateProfileFromCfg(
	IN	PRTMP_ADAPTER		pAd,
	IN  unsigned char               OpMode,         /* 0: AP Mode, 1: AP Client Mode, 2: STA Mode */
	IN  PWSC_CTRL           pWscControl,
	OUT PWSC_PROFILE        pWscProfile); 

void    WscWriteConfToPortCfg(
    IN  PRTMP_ADAPTER   pAd,
    IN  PWSC_CTRL       pWscControl,
    IN  PWSC_CREDENTIAL pCredential,
    IN  bool         bEnrollee);

#ifdef APCLI_SUPPORT
void    WscWriteConfToApCliCfg(
    IN  PRTMP_ADAPTER   pAd,
    IN  PWSC_CTRL       pWscControl,
    IN  PWSC_CREDENTIAL pCredential,
    IN  bool         bEnrollee);
#endif /* APCLI_SUPPORT */

void   WpsSmProcess(
    IN PRTMP_ADAPTER        pAd,
    IN MLME_QUEUE_ELEM 	   *Elem);

void WscPBCSessionOverlapCheck(
	IN	PRTMP_ADAPTER	pAd);

void WscPBC_DPID_FromSTA(
	IN	PRTMP_ADAPTER		pAd,
	IN	unsigned char *				pMacAddr);

#ifdef CONFIG_AP_SUPPORT
int	WscGetConfWithoutTrigger(
	IN	PRTMP_ADAPTER	pAd,
	IN  PWSC_CTRL       pWscControl,
	IN  bool         bFromUPnP);

bool	WscReadProfileFromUfdFile(
	IN	PRTMP_ADAPTER		pAd,
	IN  unsigned char               ApIdx,
	IN  char *				pUfdFileName);

bool	WscWriteProfileToUfdFile(
	IN	PRTMP_ADAPTER		pAd,
	IN  unsigned char               ApIdx,
	IN  char *				pUfdFileName);
#endif /* CONFIG_AP_SUPPORT */

void WscCheckWpsIeFromWpsAP(
    IN  PRTMP_ADAPTER 	pAd, 
    IN  PEID_STRUCT		pEid,
    OUT unsigned short *			pDPIDFromAP);


/* WSC hardware push button function 0811 */
void WSC_HDR_BTN_Init(
	IN	PRTMP_ADAPTER	pAd);

void WSC_HDR_BTN_Stop(
	IN	PRTMP_ADAPTER	pAd);
 
void WSC_HDR_BTN_CheckHandler(
	IN	PRTMP_ADAPTER	pAd);
#ifdef WSC_LED_SUPPORT
bool WscSupportWPSLEDMode(
	IN PRTMP_ADAPTER pAdapter);

bool WscSupportWPSLEDMode10(
	IN PRTMP_ADAPTER pAdapter);

bool WscAPHasSecuritySetting(
	IN PRTMP_ADAPTER pAdapter,
	IN PWSC_CTRL     pWscControl);

void WscLEDTimer(
	IN void *	SystemSpecific1, 
	IN void *	FunctionContext, 
	IN void *	SystemSpecific2, 
	IN void *	SystemSpecific3);

void WscSkipTurnOffLEDTimer(
	IN void *	SystemSpecific1, 
	IN void *	FunctionContext, 
	IN void *	SystemSpecific2, 
	IN void *	SystemSpecific3);
#endif /* WSC_LED_SUPPORT */



#ifdef CONFIG_AP_SUPPORT
void WscUpdatePortCfgTimeout(
	IN void * SystemSpecific1,
	IN void * FunctionContext,
	IN void * SystemSpecific2,
	IN void * SystemSpecific3);
#endif /* CONFIG_AP_SUPPORT */

void	WscCheckPeerDPID(
	IN  PRTMP_ADAPTER	pAd,
	IN  PFRAME_802_11 	Fr,
	IN  unsigned char *			eid_data,
	IN  int				eid_len);

void	WscClearPeerList(
	IN  PLIST_HEADER	pWscEnList);

PWSC_PEER_ENTRY	WscFindPeerEntry(
	IN  PLIST_HEADER	pWscEnList,
	IN	unsigned char *			pMacAddr);

void	WscDelListEntryByMAC(
	PLIST_HEADER		pWscEnList,
	IN  unsigned char *			pMacAddr);;

void	WscInsertPeerEntryByMAC(
	IN  PLIST_HEADER	pWscEnList,
	IN	unsigned char *			pMacAddr);

#ifdef CONFIG_AP_SUPPORT
int WscApShowPeerList(
	IN  PRTMP_ADAPTER	pAd,
	IN	char *			arg);
#endif /* CONFIG_AP_SUPPORT */


void	WscMaintainPeerList(
	IN  PRTMP_ADAPTER	pAd,
	IN  PWSC_CTRL		pWpsCtrl);

void	WscAssignEntryMAC(
	IN  PRTMP_ADAPTER	pAd,
	IN  PWSC_CTRL		pWpsCtrl);

#ifdef WSC_V2_SUPPORT
#ifdef CONFIG_AP_SUPPORT
void 	WscOnOff(
	IN  PRTMP_ADAPTER	pAd,
	IN  int				ApIdx,
	IN  bool			bOff);

void	WscAddEntryToAclList(
	IN  PRTMP_ADAPTER	pAd,
	IN	int				ApIdx,
	IN  unsigned char *			pMacAddr);

void WscSetupLockTimeout(
	IN void * SystemSpecific1,
	IN void * FunctionContext,
	IN void * SystemSpecific2,
	IN void * SystemSpecific3);

void	WscCheckPinAttackCount(
	IN  PRTMP_ADAPTER	pAd,
	IN  PWSC_CTRL		pWscControl);
#endif /* CONFIG_AP_SUPPORT */

bool	WscGenV2Msg(
	IN  PWSC_CTRL		pWpsCtrl,
	IN  bool			bSelRegistrar,
	IN	unsigned char *			pAuthorizedMACs,
	IN  int   			AuthorizedMACsLen,
	OUT	unsigned char			**pOutBuf,
	OUT	int				*pOutBufLen);

bool	WscParseV2SubItem(
	IN	unsigned char			SubID,
	IN	unsigned char *			pData,
	IN	unsigned short			DataLen,
	OUT	unsigned char *			pOutBuf,
	OUT	unsigned char *			pOutBufLen);

void	WscSendEapFragAck(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL			pWscControl,
	IN	PMAC_TABLE_ENTRY	pEntry);

void	WscSendEapFragData(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  PWSC_CTRL			pWscControl,
	IN	PMAC_TABLE_ENTRY	pEntry);
#endif /* WSC_V2_SUPPORT */

bool WscGetDataFromPeerByTag(
    IN  PRTMP_ADAPTER 	pAd, 
    IN  unsigned char *			pIeData,
    IN  int				IeDataLen,
    IN  unsigned short			WscTag,
    OUT unsigned char *			pWscBuf,
    OUT unsigned short *			pWscBufLen);

#endif /* WSC_INCLUDED */




bool rtstrmactohex(
	IN char * s1,
	IN char * s2);

bool rtstrcasecmp(
	IN char * s1,
	IN char * s2);

char * rtstrstruncasecmp(
	IN char * s1,
	IN char * s2);

char * rtstrstr(
	IN char * s1,
	IN const char * s2);

char * rstrtok(
	IN char * s,
	IN const char * ct);

int rtinet_aton(
	char *cp,
	unsigned int *addr);

/*//////// common ioctl functions ////////*/
int Set_DriverVersion_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int Set_CountryRegion_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int Set_CountryRegionABand_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int Set_WirelessMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int Set_MBSS_WirelessMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int Set_Channel_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);
int	Set_ShortSlot_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	char *			arg);

int	Set_TxPower_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	char *			arg);

int Set_BGProtection_Proc(
	IN  PRTMP_ADAPTER		pAd, 
	IN  char *			arg);

int Set_TxPreamble_Proc(
	IN  PRTMP_ADAPTER		pAd,
	IN  char *			arg);

int Set_RTSThreshold_Proc(
	IN  PRTMP_ADAPTER		pAd,
	IN  char *			arg);

int Set_FragThreshold_Proc(
	IN  PRTMP_ADAPTER		pAd,
	IN  char *			arg);

int Set_TxBurst_Proc(
	IN  PRTMP_ADAPTER		pAd,
	IN  char *			arg);


#ifdef AGGREGATION_SUPPORT
int	Set_PktAggregate_Proc(
	IN  PRTMP_ADAPTER		pAd,
	IN  char *			arg);
#endif /* AGGREGATION_SUPPORT */

#ifdef INF_PPA_SUPPORT
int	Set_INF_AMAZON_SE_PPA_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	unsigned char *			arg);

int ifx_ra_start_xmit (
	IN	struct net_device *rx_dev, 
	IN	struct net_device *tx_dev,
	IN	struct sk_buff *skb,
	IN	int len);
#endif /* INF_PPA_SUPPORT */

int	Set_IEEE80211H_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

#ifdef EXT_BUILD_CHANNEL_LIST
int Set_ExtCountryCode_Proc(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  char *          arg);

int Set_ExtDfsType_Proc(
    IN      PRTMP_ADAPTER   pAd,
    IN      char *         arg);

int Set_ChannelListAdd_Proc(
    IN      PRTMP_ADAPTER   pAd,
    IN      char *         arg);

int Set_ChannelListShow_Proc(
    IN      PRTMP_ADAPTER	pAd,
    IN      char *			arg);
int Set_ChannelListDel_Proc(
    IN      PRTMP_ADAPTER	pAd,
    IN      char *			arg);
#endif /* EXT_BUILD_CHANNEL_LIST */

#ifdef DBG
int	Set_Debug_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_DebugFunc_Proc(
	IN RTMP_ADAPTER *pAd, 
	IN char * arg);
#endif

#ifdef TXBF_SUPPORT
int	Set_ReadITxBf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_ReadETxBf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_WriteITxBf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_WriteETxBf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_StatITxBf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_StatETxBf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_TxBfTag_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int Set_ITxBfTimeout_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  char *          arg);

int Set_ETxBfTimeout_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  char *          arg);

int	Set_InvTxBfTag_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_ITxBfCal_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_ITxBfDivCal_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_ITxBfLnaCal_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	char *			arg);

int	Set_ETxBfEnCond_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int Set_ETxBfCodebook_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  char *          arg);

int Set_ETxBfCoefficient_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  char *          arg);

int Set_ETxBfGrouping_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  char *          arg);

int Set_ETxBfNoncompress_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  char *          arg);

int Set_ETxBfIncapable_Proc(
    IN  PRTMP_ADAPTER   pAd,
    IN  char *          arg);

int	Set_NoSndgCntThrd_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_NdpSndgStreams_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_Trigger_Sounding_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_ITxBfEn_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

#endif /* TXBF_SUPPORT */

int Set_RateAdaptInterval(
	IN RTMP_ADAPTER *pAd,
	IN char * arg);


#ifdef PRE_ANT_SWITCH
int Set_PreAntSwitch_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  char *         arg);

int Set_PreAntSwitchRSSI_Proc(
    IN  PRTMP_ADAPTER   pAd,
    IN  char *         arg);

int Set_PreAntSwitchTimeout_Proc(
    IN  PRTMP_ADAPTER   pAd,
    IN  char *         arg);

#endif /* PRE_ANT_SWITCH */


#ifdef CFO_TRACK
int Set_CFOTrack_Proc(
    IN  PRTMP_ADAPTER   pAd,
    IN  char *         arg);

#ifdef CFO_TRACK
#ifdef CONFIG_AP_SUPPORT
int rtmp_cfo_track(RTMP_ADAPTER *pAd, MAC_TABLE_ENTRY *pEntry, int lastClient);
#endif /* CONFIG_AP_SUPPORT */
#endif /* CFO_TRACK */

#endif // CFO_TRACK //

#ifdef DBG_CTRL_SUPPORT
int Set_DebugFlags_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  char *          arg);

#ifdef INCLUDE_DEBUG_QUEUE
int Set_DebugQueue_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  char *         arg);

void dbQueueEnqueue(
	IN unsigned char type,
	IN unsigned char *data);

void dbQueueEnqueueTxFrame(
	IN unsigned char *pTxWI,
	IN unsigned char *pHeader_802_11);

void dbQueueEnqueueRxFrame(
	IN unsigned char *pRxWI,
	IN unsigned char *pHeader_802_11,
	IN unsigned long flags);
#endif /* INCLUDE_DEBUG_QUEUE */
#endif /* DBG_CTRL_SUPPORT */

int	Show_DescInfo_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Show_MacTable_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int show_devinfo_proc(RTMP_ADAPTER *pAd, char * arg);


int	Set_ResetStatCounter_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

#ifdef DOT11_N_SUPPORT
int	Set_BASetup_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_BADecline_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_BAOriTearDown_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_BARecTearDown_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtBw_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtMcs_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtGi_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtOpMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtStbc_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtHtc_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtExtcha_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtMpduDensity_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtBaWinSize_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtRdg_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtLinkAdapt_Proc(																																																																																																																																																																																																																																									
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtAmsdu_Proc(																																																																																																																																																																																																																																																																																																																			
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);
	
int	Set_HtAutoBa_Proc(																																																																																																																																																																																																																																																																																																																			
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);
					
int	Set_HtProtect_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	char *			arg);

int	Set_HtMimoPs_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	char *			arg);

#ifdef DOT11N_DRAFT3
int Set_HT_BssCoex_Proc(
	IN	PRTMP_ADAPTER		pAd,
	IN	char *				pParam);

int Set_HT_BssCoexApCntThr_Proc(
	IN	PRTMP_ADAPTER		pAd,
	IN	char *				pParam);
#endif /* DOT11N_DRAFT3 */


#ifdef CONFIG_AP_SUPPORT
int	Set_HtTxStream_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtRxStream_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);
#ifdef DOT11_N_SUPPORT
#ifdef GREENAP_SUPPORT
int	Set_GreenAP_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);
#endif /* GREENAP_SUPPORT */
#endif /* DOT11_N_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

int	Set_ForceShortGI_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	char *			arg);

int	Set_ForceGF_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	char *			arg);

int	SetCommonHT(RTMP_ADAPTER *pAd);

int	Set_SendPSMPAction_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

void convert_reordering_packet_to_preAMSDU_or_802_3_packet(
	IN	PRTMP_ADAPTER	pAd, 
	IN	RX_BLK			*pRxBlk,
	IN  unsigned char			FromWhichBSSID);

int	Set_HtMIMOPSmode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);


int	Set_HtTxBASize_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_HtDisallowTKIP_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

int	Set_BurstMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);
#endif /* DOT11_N_SUPPORT */


#ifdef DOT11_VHT_AC
int	Set_VhtBw_Proc(
	IN RTMP_ADAPTER *pAd, 
	IN char * arg);

int	Set_VhtStbc_Proc(
	IN RTMP_ADAPTER *pAd, 
	IN char * arg);
#endif /* DOT11_VHT_AC */


#ifdef APCLI_SUPPORT
int RTMPIoctlConnStatus(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  char *          arg);

#endif /*APCLI_SUPPORT*/





#ifdef CONFIG_AP_SUPPORT
void detect_wmm_traffic(
	IN RTMP_ADAPTER	*pAd, 
	IN unsigned char up,
	IN unsigned char bOutput);

void dynamic_tune_be_tx_op(
	IN RTMP_ADAPTER *pAd,
	IN unsigned long nonBEpackets);
#endif /* CONFIG_AP_SUPPORT */


#ifdef DOT11_N_SUPPORT
void Handle_BSS_Width_Trigger_Events(RTMP_ADAPTER *pAd);

void build_ext_channel_switch_ie(
	IN RTMP_ADAPTER *pAd,
	IN HT_EXT_CHANNEL_SWITCH_ANNOUNCEMENT_IE *pIE);

void assoc_ht_info_debugshow(
	IN RTMP_ADAPTER *pAd,
	IN MAC_TABLE_ENTRY *pEntry,
	IN unsigned char ht_cap_len,
	IN HT_CAPABILITY_IE *pHTCapability);
#endif /* DOT11_N_SUPPORT */

bool APRxDoneInterruptHandle(RTMP_ADAPTER *pAd);
bool STARxDoneInterruptHandle(RTMP_ADAPTER *pAd, bool argc);
bool RxDoneInterruptHandle(RTMP_ADAPTER *pAd);

NTSTATUS StopDmaRx(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char Level);
NTSTATUS StopDmaTx(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char Level);

#ifdef DOT11_N_SUPPORT
/* AMPDU packet indication */
void Indicate_AMPDU_Packet(
	IN	PRTMP_ADAPTER	pAd,
	IN	RX_BLK			*pRxBlk,
	IN	unsigned char			FromWhichBSSID);

#ifdef HDR_TRANS_SUPPORT
void Indicate_AMPDU_Packet_Hdr_Trns(
	IN	PRTMP_ADAPTER	pAd,
	IN	RX_BLK			*pRxBlk,
	IN	unsigned char			FromWhichBSSID);
#endif /* HDR_TRANS_SUPPORT */

/* AMSDU packet indication */
void Indicate_AMSDU_Packet(
	IN	PRTMP_ADAPTER	pAd,
	IN	RX_BLK			*pRxBlk,
	IN	unsigned char			FromWhichBSSID);

void BaReOrderingBufferMaintain(
    IN PRTMP_ADAPTER pAd);
#endif /* DOT11_N_SUPPORT */

/* Normal legacy Rx packet indication */
void Indicate_Legacy_Packet(
	IN	PRTMP_ADAPTER	pAd,
	IN	RX_BLK			*pRxBlk,
	IN	unsigned char			FromWhichBSSID);

#ifdef HDR_TRANS_SUPPORT
void Indicate_Legacy_Packet_Hdr_Trns(
	IN	PRTMP_ADAPTER	pAd,
	IN	RX_BLK			*pRxBlk,
	IN	unsigned char			FromWhichBSSID);
#endif /* HDR_TRANS_SUPPORT */

void Indicate_EAPOL_Packet(
	IN	PRTMP_ADAPTER	pAd,
	IN	RX_BLK			*pRxBlk,
	IN	unsigned char			FromWhichBSSID);

unsigned int deaggregate_AMSDU_announce(
	IN	PRTMP_ADAPTER	pAd,
	PNDIS_PACKET		pPacket,
	IN	unsigned char *			pData,
	IN	unsigned long			DataSize,
	IN	unsigned char			OpMode);

#ifdef TXBF_SUPPORT
bool clientSupportsETxBF(RTMP_ADAPTER *pAd, HT_BF_CAP *pTxBFCap);
void setETxBFCap(RTMP_ADAPTER *pAd, HT_BF_CAP *pTxBFCap);

#ifdef ETXBF_EN_COND3_SUPPORT
void txSndgSameMcs(RTMP_ADAPTER *pAd, MAC_TABLE_ENTRY * pEnt, unsigned char smoothMfb);
void txSndgOtherGroup(RTMP_ADAPTER *pAd, MAC_TABLE_ENTRY *pEntry);
void txMrqInvTxBF(RTMP_ADAPTER *pAd, MAC_TABLE_ENTRY *pEntry);
void chooseBestMethod(RTMP_ADAPTER *pAd, MAC_TABLE_ENTRY *pEntry, unsigned char mfb);
void rxBestSndg(RTMP_ADAPTER *pAd, MAC_TABLE_ENTRY *pEntry);
#endif /* ETXBF_EN_COND3_SUPPORT */

void handleBfFb(RTMP_ADAPTER *pAd, RX_BLK *pRxBlk);

void TxBFInit(
	IN PRTMP_ADAPTER 	pAd,
	IN MAC_TABLE_ENTRY	*pEntry,
	IN bool			supportsETxBF);

void eTxBFProbing(
 	IN PRTMP_ADAPTER 	pAd,
	IN MAC_TABLE_ENTRY	*pEntry);

void Trigger_Sounding_Packet(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char			SndgType,
	IN	unsigned char			SndgBW,
	IN	unsigned char			SndgMcs,
	IN  MAC_TABLE_ENTRY *pEntry);

void rtmp_asic_set_bf(
	IN RTMP_ADAPTER *pAd);

bool rtmp_chk_itxbf_calibration(
	IN RTMP_ADAPTER *pAd);

#endif /* TXBF_SUPPORT */

bool CmdRspEventCallbackHandle(PRTMP_ADAPTER pAd, unsigned char * pRspBuffer);	

#ifdef CONFIG_AP_SUPPORT
/* remove LLC and get 802_3 Header */
#define  RTMP_AP_802_11_REMOVE_LLC_AND_CONVERT_TO_802_3(_pRxBlk, _pHeader802_3)	\
{																						\
	unsigned char *_pRemovedLLCSNAP = NULL, *_pDA, *_pSA;                                 						\
																				\
																				\
	if (RX_BLK_TEST_FLAG(_pRxBlk, fRX_WDS) || RX_BLK_TEST_FLAG(_pRxBlk, fRX_MESH)) 		\
	{                                                                           											\
		_pDA = _pRxBlk->pHeader->Addr3;                                         							\
		_pSA = (unsigned char *)_pRxBlk->pHeader + sizeof(HEADER_802_11);                					\
	}                                                                           											\
	else if (RX_BLK_TEST_FLAG(_pRxBlk, fRX_APCLI))										\
	{																					\
		_pDA = _pRxBlk->pHeader->Addr1; 													\
		_pSA = _pRxBlk->pHeader->Addr3;													\
	}																					\
	else                                                                        										\
	{                                                                           											\
		_pDA = _pRxBlk->pHeader->Addr3;                                         							\
		_pSA = _pRxBlk->pHeader->Addr2;                                         							\
	}                                                                           											\
																				\
	CONVERT_TO_802_3(_pHeader802_3, _pDA, _pSA, _pRxBlk->pData, 						\
		_pRxBlk->DataSize, _pRemovedLLCSNAP);                                   						\
}
#endif /* CONFIG_AP_SUPPORT */



bool APFowardWirelessStaToWirelessSta(
	IN	PRTMP_ADAPTER	pAd,
	IN	PNDIS_PACKET	pPacket,
	IN	unsigned long			FromWhichBSSID);

void Announce_or_Forward_802_3_Packet(
	IN	PRTMP_ADAPTER	pAd,
	IN	PNDIS_PACKET	pPacket,
	IN	unsigned char			FromWhichBSSID);

void Sta_Announce_or_Forward_802_3_Packet(
	IN	PRTMP_ADAPTER	pAd,
	IN	PNDIS_PACKET	pPacket,
	IN	unsigned char			FromWhichBSSID);

#ifdef CONFIG_AP_SUPPORT
#define AP_ANNOUNCE_OR_FORWARD_802_3_PACKET(_pAd, _pPacket, _FromWhichBSS)\
			Announce_or_Forward_802_3_Packet(_pAd, _pPacket, _FromWhichBSS);
#endif /* CONFIG_AP_SUPPORT */





/* Normal, AMPDU or AMSDU */
void CmmRxnonRalinkFrameIndicate(
	IN RTMP_ADAPTER *pAd,
	IN RX_BLK *pRxBlk,
	IN unsigned char FromWhichBSSID);

#ifdef HDR_TRANS_SUPPORT
void CmmRxnonRalinkFrameIndicate_Hdr_Trns(
	IN RTMP_ADAPTER *pAd,
	IN RX_BLK *pRxBlk,
	IN unsigned char FromWhichBSSID);
#endif /* HDR_TRANS_SUPPORT */

void CmmRxRalinkFrameIndicate(
	IN RTMP_ADAPTER *pAd,
	IN MAC_TABLE_ENTRY *pEntry,
	IN RX_BLK *pRxBlk,
	IN unsigned char FromWhichBSSID);

void Update_Rssi_Sample(
	IN RTMP_ADAPTER *pAd,
	IN RSSI_SAMPLE *pRssi,
	IN RXWI_STRUC *pRxWI);

PNDIS_PACKET GetPacketFromRxRing(
	IN RTMP_ADAPTER *pAd,
	OUT RX_BLK *pRxBlk,
	OUT bool	 *pbReschedule,
	INOUT unsigned int *pRxPending);

PNDIS_PACKET RTMPDeFragmentDataFrame(
	IN RTMP_ADAPTER *pAd,
	IN RX_BLK *pRxBlk);

/*////////////////////////////////////*/

#if defined (AP_SCAN_SUPPORT) || defined (CONFIG_STA_SUPPORT)
void RTMPIoctlGetSiteSurvey(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	RTMP_IOCTL_INPUT_STRUCT *wrq);
#endif

#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT
int Set_ApCli_AuthMode_Proc(
    IN PRTMP_ADAPTER pAd, 
    IN char * arg);

int Set_ApCli_EncrypType_Proc(
    IN PRTMP_ADAPTER pAd, 
    IN char * arg);
#endif /* APCLI_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */


#ifdef MAT_SUPPORT

void getIPv6MacTbInfo(MAT_STRUCT *, char *, unsigned long);

void getIPMacTbInfo(
	IN MAT_STRUCT *pMatCfg, 
	IN char *pOutBuf,
	IN unsigned long BufLen);

unsigned int MATEngineInit(
	IN RTMP_ADAPTER *pAd);

unsigned int MATEngineExit(
	IN RTMP_ADAPTER *pAd);

unsigned char * MATEngineRxHandle(
	IN PRTMP_ADAPTER	pAd,
	IN PNDIS_PACKET		pPkt,
	IN unsigned int				infIdx);


unsigned char * MATEngineTxHandle(
	IN PRTMP_ADAPTER	pAd,
	IN PNDIS_PACKET     pPkt,
	IN unsigned int				infIdx,
	IN unsigned char                  OpMode);

bool MATPktRxNeedConvert(
	IN PRTMP_ADAPTER	pAd, 
	IN PNET_DEV			net_dev);

#endif /* MAT_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
typedef struct CountryCodeToCountryRegion {
	unsigned short		CountryNum;
	unsigned char		IsoName[3];
	/*unsigned char		CountryName[40]; */
	char *		pCountryName;
	bool		SupportABand;
	/*unsigned long		RegDomainNum11A; */
	unsigned char		RegDomainNum11A;
	bool  	SupportGBand;
	/*unsigned long		RegDomainNum11G; */
	unsigned char		RegDomainNum11G;
} COUNTRY_CODE_TO_COUNTRY_REGION;
#endif /* CONFIG_AP_SUPPORT */

#ifdef SNMP_SUPPORT
/*for snmp */
typedef struct _DefaultKeyIdxValue
{
	unsigned char	KeyIdx;
	unsigned char	Value[16];
} DefaultKeyIdxValue, *PDefaultKeyIdxValue;
#endif



int	Set_FixedTxMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

#ifdef CONFIG_APSTA_MIXED_SUPPORT
int	Set_OpMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);
#endif /* CONFIG_APSTA_MIXED_SUPPORT */

int Set_LongRetryLimit_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	char *			arg);

int Set_ShortRetryLimit_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	char *			arg);

int Set_AutoFallBack_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	char *			arg);


void RT28XXDMADisable(
	IN RTMP_ADAPTER 		*pAd);

void RT28XXDMAEnable(
	IN RTMP_ADAPTER 		*pAd);
	
void RT28xx_UpdateBeaconToAsic(
	IN RTMP_ADAPTER * pAd, 
	IN int apidx,
	IN unsigned long BeaconLen,
	IN unsigned long UpdatePos);

void CfgInitHook(PRTMP_ADAPTER pAd);


unsigned int RtmpNetTaskInit(
	IN RTMP_ADAPTER *pAd);

void RtmpNetTaskExit(
	IN PRTMP_ADAPTER pAd);

unsigned int RtmpMgmtTaskInit(
	IN RTMP_ADAPTER *pAd);
	
void RtmpMgmtTaskExit(
	IN RTMP_ADAPTER *pAd);

void tbtt_tasklet(unsigned long data);


	
	


void AsicTurnOffRFClk(
	IN PRTMP_ADAPTER    pAd, 
	IN	unsigned char           Channel);



#ifdef RTMP_TIMER_TASK_SUPPORT
int RtmpTimerQThread(
	IN unsigned long Context);

RTMP_TIMER_TASK_ENTRY *RtmpTimerQInsert(
	IN RTMP_ADAPTER *pAd, 
	IN RALINK_TIMER_STRUCT *pTimer);

bool RtmpTimerQRemove(
	IN RTMP_ADAPTER *pAd, 
	IN RALINK_TIMER_STRUCT *pTimer);

void RtmpTimerQExit(
	IN RTMP_ADAPTER *pAd);

void RtmpTimerQInit(
	IN RTMP_ADAPTER *pAd);
#endif /* RTMP_TIMER_TASK_SUPPORT */


#ifdef RTMP_MAC_USB

NTSTATUS RTUSBMultiRead(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned short			Offset,
	OUT	unsigned char *			pData,
	IN	unsigned short			length);

NTSTATUS RTUSBMultiWrite(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned short			Offset,
	IN	unsigned char *			pData,
	IN	unsigned short			length,
	IN	bool			bWriteHigh);

NTSTATUS RTUSBMultiWrite_nBytes(
        IN      PRTMP_ADAPTER   pAd,
        IN      unsigned short                  Offset,
        IN      unsigned char *                  pData,
        IN      unsigned short                  length,
        IN      unsigned short                  batchLen);

NTSTATUS RTUSBMultiWrite_OneByte(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned short			Offset,
	IN	unsigned char *			pData);

NTSTATUS RTUSBReadBBPRegister(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char			Id,
	IN	unsigned char *			pValue);

NTSTATUS RTUSBWriteBBPRegister(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char			Id,
	IN	unsigned char			Value);

NTSTATUS RTUSBWriteRFRegister(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned int			Value);
	
NTSTATUS RTUSB_VendorRequest(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned int			TransferFlags,
	IN	unsigned char			ReservedBits,
	IN	unsigned char			Request,
	IN	unsigned short			Value,
	IN	unsigned short			Index,
	IN	void *			TransferBuffer,
	IN	unsigned int			TransferBufferLength);

NTSTATUS RTUSBReadEEPROM(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned short			Offset,
	OUT	unsigned char *			pData,
	IN	unsigned short			length);

NTSTATUS RTUSBWriteEEPROM(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned short			Offset,
	IN	unsigned char *			pData,
	IN	unsigned short			length);

void RTUSBPutToSleep(
	IN	PRTMP_ADAPTER	pAd);

NTSTATUS RTUSBWakeUp(
	IN	PRTMP_ADAPTER	pAd);

unsigned int	RTUSBEnqueueCmdFromNdis(
	IN	PRTMP_ADAPTER	pAd,
	IN	NDIS_OID		Oid,
	IN	bool			SetInformation,
	IN	void *			pInformationBuffer,
	IN	unsigned int			InformationBufferLength);

void RTUSBDequeueCmd(
	IN	PCmdQ		cmdq,
	OUT	PCmdQElmt	*pcmdqelmt);

int RTUSBCmdThread(
	IN unsigned long Context);

void RTUSBBssBeaconExit(
	IN RTMP_ADAPTER *pAd);

void RTUSBBssBeaconStop(
	IN RTMP_ADAPTER *pAd);

void RTUSBBssBeaconStart(
	IN RTMP_ADAPTER * pAd);

void RTUSBBssBeaconInit(
	IN RTMP_ADAPTER *pAd);

void RTUSBWatchDog(
	IN RTMP_ADAPTER *pAd);
	
NTSTATUS RTUSBWriteMACRegister(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned short			Offset,
	IN	unsigned int			Value,
	IN	bool			bWriteHigh);

NTSTATUS RTUSBReadMACRegister(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned short			Offset,
	OUT	unsigned int *pValue);

NTSTATUS RTUSBSingleWrite(
	IN 	RTMP_ADAPTER 	*pAd,
	IN	unsigned short			Offset,
	IN	unsigned short			Value,
	IN	bool			bWriteHigh);

NTSTATUS RTUSBFirmwareWrite(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char *		pFwImage,
	IN unsigned long		FwLen);

NTSTATUS	RTUSBVenderReset(
	IN	PRTMP_ADAPTER	pAd);

unsigned int RTUSBSetHardWareRegister(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	void *			pBuf);

unsigned int RTUSBQueryHardWareRegister(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	void *			pBuf);

/*void CMDHandler( */
/*    IN PRTMP_ADAPTER pAd); */

unsigned int	RTUSBWriteHWMACAddress(
	IN RTMP_ADAPTER *pAd);

void MlmeSetPsm(
	IN RTMP_ADAPTER *pAd, 
	IN unsigned short psm);

unsigned int RTMPWPAAddKeyProc(
	IN RTMP_ADAPTER *pAd,
	IN void *pBuf);

void AsicRxAntEvalAction(
	IN RTMP_ADAPTER *pAd);

void append_pkt(
	IN RTMP_ADAPTER *pAd, 
	IN unsigned char *pHeader802_3,
	IN unsigned int HdrLen,
	IN unsigned char *pData,
	IN unsigned long DataSize,
	OUT PNDIS_PACKET *ppPacket);


void RTUSBMlmeHardTransmit(
	IN RTMP_ADAPTER *pAd,
	IN MGMT_STRUC *pMgmt);

int MlmeThread(unsigned long Context);


/*
	Function Prototype in rtusb_data.c
*/
unsigned int	RTUSBFreeDescRequest(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char BulkOutPipeId,
	IN unsigned int req_cnt);


bool	RTUSBNeedQueueBackForAgg(
	IN RTMP_ADAPTER *pAd, 
	IN unsigned char		BulkOutPipeId);


/* Function Prototype in cmm_data_usb.c */
unsigned short RtmpUSB_WriteSubTxResource(
	IN	PRTMP_ADAPTER	pAd,
	IN	TX_BLK			*pTxBlk,
	IN	bool			bIsLast,
	OUT	unsigned short			*FreeNumber);

unsigned short RtmpUSB_WriteSingleTxResource(
	IN	PRTMP_ADAPTER	pAd,
	IN	TX_BLK			*pTxBlk,
	IN	bool			bIsLast,
	OUT	unsigned short			*FreeNumber);

unsigned short	RtmpUSB_WriteFragTxResource(
	IN	PRTMP_ADAPTER	pAd,
	IN	TX_BLK			*pTxBlk,
	IN	unsigned char			fragNum,
	OUT	unsigned short			*FreeNumber);
	
unsigned short RtmpUSB_WriteMultiTxResource(
	IN	PRTMP_ADAPTER	pAd,
	IN	TX_BLK			*pTxBlk,
	IN	unsigned char			frameNum,
	OUT	unsigned short			*FreeNumber);

void RtmpUSB_FinalWriteTxResource(
	IN	PRTMP_ADAPTER	pAd,
	IN	TX_BLK			*pTxBlk,
	IN	unsigned short			totalMPDUSize,
	IN	unsigned short			TxIdx);

void RtmpUSBDataLastTxIdx(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char			QueIdx,
	IN	unsigned short			TxIdx);

void RtmpUSBDataKickOut(
	IN	PRTMP_ADAPTER	pAd,
	IN	TX_BLK			*pTxBlk,
	IN	unsigned char			QueIdx);

int RtmpUSBMgmtKickOut(
	IN RTMP_ADAPTER 	*pAd, 
	IN unsigned char 			QueIdx,
	IN PNDIS_PACKET		pPacket,
	IN unsigned char *			pSrcBufVA,
	IN unsigned int 			SrcBufLen);

void RtmpUSBNullFrameKickOut(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char		QueIdx,
	IN unsigned char		*pNullFrame,
	IN unsigned int		frameLen);
	
void RtmpUsbStaAsicForceWakeupTimeout(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

void RT28xxUsbStaAsicForceWakeup(
	IN PRTMP_ADAPTER pAd,
	IN bool       bFromTx);

void RT28xxUsbStaAsicSleepThenAutoWakeup(
	IN PRTMP_ADAPTER pAd, 
	IN unsigned short TbttNumToNextWakeUp);

void RT28xxUsbMlmeRadioOn(
	IN PRTMP_ADAPTER pAd);

void RT28xxUsbMlmeRadioOFF(
	IN PRTMP_ADAPTER pAd);
void RT28xxUsbAsicRadioOff(
	IN PRTMP_ADAPTER pAd);

void RT28xxUsbAsicRadioOn(
	IN PRTMP_ADAPTER pAd);

bool AsicCheckCommandOk(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char		 Command);

void RT28xxUsbAsicWOWEnable(
	IN PRTMP_ADAPTER pAd);

void RT28xxUsbAsicWOWDisable(
	IN PRTMP_ADAPTER pAd);

#endif /* RTMP_MAC_USB */

#ifdef NEW_WOW_SUPPORT
void RT28xxAndesWOWEnable(
    IN PRTMP_ADAPTER pAd);
void RT28xxAndesWOWDisable(
    IN PRTMP_ADAPTER pAd);
#endif /* NEW_WOW_SUPPORT */

#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
void RT28xxAsicWOWEnable(
    IN PRTMP_ADAPTER pAd);

void RT28xxAsicWOWDisable(
    IN PRTMP_ADAPTER pAd);
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */

unsigned int RTMPCheckRxError(
	IN RTMP_ADAPTER *pAd, 
	IN PHEADER_802_11 pHeader,
	IN RXWI_STRUC *pRxWI,
	IN RXINFO_STRUC *pRxInfo);


/*////////////////////////////////////*/

#ifdef AP_QLOAD_SUPPORT
void QBSS_LoadInit(
 	IN		RTMP_ADAPTER	*pAd);

void QBSS_LoadAlarmReset(
 	IN		RTMP_ADAPTER	*pAd);

void QBSS_LoadAlarmResume(
 	IN		RTMP_ADAPTER	*pAd);

unsigned int QBSS_LoadBusyTimeGet(
 	IN		RTMP_ADAPTER	*pAd);

bool QBSS_LoadIsAlarmIssued(
 	IN		RTMP_ADAPTER	*pAd);

bool QBSS_LoadIsBusyTimeAccepted(
 	IN		RTMP_ADAPTER	*pAd,
	IN		unsigned int			BusyTime);

unsigned int QBSS_LoadElementAppend(
 	IN		RTMP_ADAPTER	*pAd,
	OUT		unsigned char			*buf_p);

unsigned int QBSS_LoadElementParse(
 	IN		RTMP_ADAPTER	*pAd,
	IN		unsigned char			*pElement,
	OUT		unsigned short			*pStationCount,
	OUT		unsigned char			*pChanUtil,
	OUT		unsigned short			*pAvalAdmCap);

void QBSS_LoadUpdate(
 	IN		RTMP_ADAPTER	*pAd,
	IN		unsigned long			UpTime);

void QBSS_LoadStatusClear(
 	IN		RTMP_ADAPTER	*pAd);

int	Show_QoSLoad_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	char *			arg);
#endif /* AP_QLOAD_SUPPORT */

/*///////////////////////////////////*/
int RTMPShowCfgValue(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			pName,
	IN	char *			pBuf,
	IN	unsigned int			MaxLen);

char * RTMPGetRalinkAuthModeStr(
    IN  NDIS_802_11_AUTHENTICATION_MODE authMode);

char * RTMPGetRalinkEncryModeStr(
    IN  unsigned short encryMode);
/*//////////////////////////////////*/



void ReSyncBeaconTime(RTMP_ADAPTER *pAd);
void RTMPSetAGCInitValue(RTMP_ADAPTER *pAd, unsigned char BandWidth);

#ifdef TXBF_SUPPORT
void handleHtcField(
	IN	PRTMP_ADAPTER	pAd,
	IN	RX_BLK			*pRxBlk);
#endif /* TXBF_SUPPORT */

#ifdef MFB_SUPPORT
void MFB_PerPareMRQ(
	IN	PRTMP_ADAPTER	pAd,
	OUT	void* pBuf,
	IN	PMAC_TABLE_ENTRY pEntry);

void MFB_PerPareMFB(
	IN	PRTMP_ADAPTER	pAd,
	OUT	void* pBuf,
	IN	PMAC_TABLE_ENTRY pEntry);
#endif /* MFB_SUPPORT */

#define VIRTUAL_IF_INC(__pAd) ((__pAd)->VirtualIfCnt++)
#define VIRTUAL_IF_DEC(__pAd) ((__pAd)->VirtualIfCnt--)
#define VIRTUAL_IF_NUM(__pAd) ((__pAd)->VirtualIfCnt)



#ifdef RTMP_USB_SUPPORT
/*
 * Function Prototype in rtusb_bulk.c
 */
 
#ifdef INF_AMAZON_SE
void SoftwareFlowControl(
	IN PRTMP_ADAPTER pAd) ;
#endif /* INF_AMAZON_SE */


void	RTUSBInitTxDesc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PTX_CONTEXT		pTxContext,
	IN	unsigned char			BulkOutPipeId,
	IN	usb_complete_t	Func);

void	RTUSBInitHTTxDesc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PHT_TX_CONTEXT	pTxContext,
	IN	unsigned char			BulkOutPipeId,
	IN	unsigned long			BulkOutSize,
	IN	usb_complete_t	Func);

void	RTUSBInitRxDesc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PRX_CONTEXT		pRxContext);

void RTUSBCleanUpDataBulkOutQueue(
	IN	PRTMP_ADAPTER	pAd);

void RTUSBCancelPendingBulkOutIRP(
	IN	PRTMP_ADAPTER	pAd);

void RTUSBBulkOutDataPacket(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char			BulkOutPipeId,
	IN	unsigned char			Index);

void RTUSBBulkOutNullFrame(
	IN	PRTMP_ADAPTER	pAd);

void RTUSBBulkOutRTSFrame(
	IN	PRTMP_ADAPTER	pAd);

void RTUSBCancelPendingBulkInIRP(
	IN	PRTMP_ADAPTER	pAd);

void RTUSBCancelPendingIRPs(
	IN	PRTMP_ADAPTER	pAd);

void RTUSBBulkOutMLMEPacket(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char			Index);

void RTUSBBulkOutPsPoll(
	IN	PRTMP_ADAPTER	pAd);

void RTUSBCleanUpMLMEBulkOutQueue(
	IN	PRTMP_ADAPTER	pAd);

void RTUSBKickBulkOut(
	IN	PRTMP_ADAPTER pAd);

void	RTUSBBulkReceive(
	IN	PRTMP_ADAPTER	pAd);

void RTUSBBulkCmdRspEventReceive(
	IN PRTMP_ADAPTER pAd);

void DoBulkIn(
	IN RTMP_ADAPTER *pAd);

void RTUSBInitRxDesc(
	IN	PRTMP_ADAPTER	pAd,
	IN  PRX_CONTEXT		pRxContext);

void RTUSBBulkRxHandle(
	IN unsigned long data);	
#endif /* RTMP_USB_SUPPORT */


#ifdef SOFT_ENCRYPT
bool RTMPExpandPacketForSwEncrypt(
	IN  PRTMP_ADAPTER   pAd,
	IN	PTX_BLK			pTxBlk);

void RTMPUpdateSwCacheCipherInfo(	
	IN  PRTMP_ADAPTER   pAd,
	IN	PTX_BLK			pTxBlk,
	IN	unsigned char *			pHdr);
#endif /* SOFT_ENCRYPT */


/*
	OS Related funciton prototype definitions.
	TODO: Maybe we need to move these function prototypes to other proper place.
*/

void RTInitializeCmdQ(
	IN	PCmdQ	cmdq);

int RTPCICmdThread(
	IN unsigned long Context);

void CMDHandler(
    IN PRTMP_ADAPTER pAd);

void RTThreadDequeueCmd(
	IN	PCmdQ		cmdq,
	OUT	PCmdQElmt	*pcmdqelmt);

unsigned int RTEnqueueInternalCmd(
	IN PRTMP_ADAPTER	pAd,
	IN NDIS_OID			Oid,
	IN void *			pInformationBuffer,
	IN unsigned int			InformationBufferLength);

#ifdef HOSTAPD_SUPPORT
void ieee80211_notify_michael_failure(
	IN	PRTMP_ADAPTER    pAd,
	IN	PHEADER_802_11   pHeader,
	IN	unsigned int            keyix,
	IN	int              report);

const char* ether_sprintf(const unsigned char *mac);
#endif/*HOSTAPD_SUPPORT*/

#ifdef VENDOR_FEATURE3_SUPPORT
void RTMP_IO_WRITE32(
	PRTMP_ADAPTER pAd,
	unsigned int Offset,
	unsigned int Value);

void RTMP_BBP_IO_READ8_BY_REG_ID(
	PRTMP_ADAPTER pAd,
	unsigned int Offset,
	unsigned char *pValue);

void RTMP_BBP_IO_READ8(
	PRTMP_ADAPTER pAd,
	unsigned char Offset,
	unsigned char *pValue,
	bool FlgValidMCR);

void RTMP_BBP_IO_WRITE8_BY_REG_ID(
	PRTMP_ADAPTER pAd,
	unsigned int Offset,
	unsigned char Value);

void RTMP_BBP_IO_WRITE8(
	PRTMP_ADAPTER pAd,
	unsigned char Offset,
	unsigned char Value,
	bool FlgValidMCR);
#endif /* VENDOR_FEATURE3_SUPPORT */


int AsicGetMacVersion(
	IN RTMP_ADAPTER *pAd);

int WaitForAsicReady(
	IN RTMP_ADAPTER *pAd);

bool CHAN_PropertyCheck(
	IN PRTMP_ADAPTER	pAd,
	IN unsigned int			ChanNum,
	IN unsigned char			Property);


void  getRate(
    IN HTTRANSMIT_SETTING HTSetting, 
    OUT unsigned long* fLastTxRxRate);


#ifdef APCLI_SUPPORT
#ifdef APCLI_WPA_SUPPLICANT_SUPPORT
void    ApcliSendAssocIEsToWpaSupplicant( 
    IN  PRTMP_ADAPTER pAd,
    IN unsigned int ifIndex);

int	    ApcliWpaCheckEapCode(
	IN  PRTMP_ADAPTER   		pAd,
	IN  unsigned char *				pFrame,
	IN  unsigned short				FrameLen,
	IN  unsigned short				OffSet);

void    ApcliWpaSendEapolStart(
	IN	PRTMP_ADAPTER	pAd,
	IN  unsigned char *          pBssid,
	IN  PMAC_TABLE_ENTRY pMacEntry,
	IN	PAPCLI_STRUCT pApCliEntry);


void	ApCliRTMPSendNullFrame(
	IN	PRTMP_ADAPTER	pAd,
	IN	unsigned char			TxRate,
	IN	bool 		bQosNull,
	IN PMAC_TABLE_ENTRY pMacEntry);

#endif/*APCLI_WPA_SUPPLICANT_SUPPORT*/
#endif/*APCLI_SUPPORT*/


void RTMP_IndicateMediaState(	
	IN	PRTMP_ADAPTER		pAd,
	IN  NDIS_MEDIA_STATE	media_state);

#if defined(RT3350) || defined(RT33xx)
void RTMP_TxEvmCalibration(
	IN PRTMP_ADAPTER pAd);
#endif /* defined(RT3350) || defined(RT33xx) */

int RTMPSetInformation(
    IN RTMP_ADAPTER *pAd,
    IN OUT RTMP_IOCTL_INPUT_STRUCT *rq,
    IN int cmd);

int RTMPQueryInformation(
    IN RTMP_ADAPTER *pAd,
    INOUT RTMP_IOCTL_INPUT_STRUCT *rq,
    IN int cmd);

void RTMPIoctlShow(
	IN RTMP_ADAPTER *pAd,
	IN RTMP_IOCTL_INPUT_STRUCT *rq,
	IN unsigned int subcmd,
	IN void *pData,
	IN unsigned long Data);

int RTMP_COM_IoctlHandle(
	IN void *pAdSrc,
	IN RTMP_IOCTL_INPUT_STRUCT *wrq,
	IN int cmd,
	IN unsigned short subcmd,
	IN void *pData,
	IN unsigned long Data);

#ifdef CONFIG_AP_SUPPORT
int RTMP_AP_IoctlPrepare(
	IN RTMP_ADAPTER *pAd,
	IN void *pCB);
#endif /* CONFIG_AP_SUPPORT */


int Set_VcoPeriod_Proc(
	IN RTMP_ADAPTER *pAd,
	IN char * arg);

int	Set_RateAlg_Proc(
	IN RTMP_ADAPTER *pAd,
	IN char * arg);

#ifdef SINGLE_SKU
int Set_ModuleTxpower_Proc(
	IN RTMP_ADAPTER *pAd, 
	IN char * arg);
#endif /* SINGLE_SKU */

void RtmpEnqueueNullFrame(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char *        pAddr,
	IN unsigned char         TxRate,
	IN unsigned char         PID,
	IN unsigned char         apidx,
    IN bool       bQosNull,
    IN bool       bEOSP,
    IN unsigned char         OldUP);

void RtmpCleanupPsQueue(
	IN  PRTMP_ADAPTER   pAd,
	IN  PQUEUE_HEADER   pQueue);

#ifdef CONFIG_MULTI_CHANNEL
void RtmpPrepareHwNullFrame(
	IN PRTMP_ADAPTER pAd,
	IN PMAC_TABLE_ENTRY pEntry,
	IN bool bQosNull,
	IN bool bEOSP,
	IN unsigned char OldUP,
	IN unsigned char OpMode,
	IN unsigned char PwrMgmt,
	IN bool bWaitACK,
	IN char Index);

void RTMPHwSendNullFrame(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char TxRate,
	IN bool bQosNull,
	IN unsigned short PwrMgmt,
	IN char Index);

void RtmpEnqueueLastNullFrame(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pAddr,
	IN unsigned char TxRate,
	IN unsigned char PID,
	IN unsigned char apidx,
    IN bool bQosNull,
    IN bool bEOSP,
    IN unsigned char OldUP,
    IN unsigned char PwrMgmt,
	IN unsigned char OpMode);

void RtmpPrepareHwNullFrame(
	IN PRTMP_ADAPTER pAd,
	IN PMAC_TABLE_ENTRY pEntry,
	IN bool bQosNull,
	IN bool bEOSP,
	IN unsigned char OldUP,
	IN unsigned char OpMode,
	IN unsigned char PwrMgmt,
	IN bool bWaitACK,
	IN char Index);

void MCC_ChangeAction(
	IN void * SystemSpecific1, 
	IN void * FunctionContext, 
	IN void * SystemSpecific2, 
	IN void * SystemSpecific3);

void ConcurrentP2PConnectTimeout(
    IN void * SystemSpecific1, 
    IN void * FunctionContext, 
    IN void * SystemSpecific2, 
    IN void * SystemSpecific3);

unsigned int MultiChannelThreadInit(
	IN  PRTMP_ADAPTER pAd);

bool MultiChannelThreadExit(
	IN  PRTMP_ADAPTER pAd);

void MultiChannelTimerStop(
	IN  PRTMP_ADAPTER pAd);

void MultiChannelTimerStart(
	IN  PRTMP_ADAPTER pAd,
	IN MAC_TABLE_ENTRY  *pEntry);

void MultiChannelSwitchToRa(
	IN  PRTMP_ADAPTER pAd);

void MultiChannelSwitchToP2P(
	IN  PRTMP_ADAPTER pAd);
#endif /* CONFIG_MULTI_CHANNEL */


unsigned char dot11_2_ra_rate(unsigned char MaxSupportedRateIn500Kbps);
unsigned char dot11_max_sup_rate(int SupRateLen, unsigned char *SupRate, int ExtRateLen, unsigned char *ExtRate);

void set_entry_phy_cfg(RTMP_ADAPTER *pAd, MAC_TABLE_ENTRY *pEntry);
void MacTableReset(RTMP_ADAPTER *pAd);
MAC_TABLE_ENTRY *MacTableLookup(RTMP_ADAPTER *pAd, unsigned char *pAddr);
bool MacTableDeleteEntry(RTMP_ADAPTER *pAd, unsigned short wcid, unsigned char *pAddr);
MAC_TABLE_ENTRY *MacTableInsertEntry(
    IN RTMP_ADAPTER *pAd,
    IN unsigned char *pAddr,
	IN unsigned char apidx,
	IN unsigned char OpMode,
	IN bool CleanAll);



void dumpTxWI(RTMP_ADAPTER *pAd, TXWI_STRUC *pTxWI);
void dump_rxwi(RTMP_ADAPTER *pAd, RXWI_STRUC *pRxWI);
void dump_txinfo(RTMP_ADAPTER *pAd, TXINFO_STRUC *pTxInfo);
void dump_rxinfo(RTMP_ADAPTER *pAd, RXINFO_STRUC *pRxInfo);
#if defined(RT65xx) || defined(MT7601)
void dumpRxFCEInfo(RTMP_ADAPTER *pAd, RXFCE_INFO *pRxFceInfo);
#endif /* defined(RT65xx) || defined(MT7601) */



#ifdef FPGA_MODE
int set_tx_kickcnt(RTMP_ADAPTER *pAd, char * arg);
int set_data_phy_mode(RTMP_ADAPTER *pAd, char * arg);
int set_data_bw(RTMP_ADAPTER *pAd, char * arg);
int set_data_mcs(RTMP_ADAPTER *pAd, char * arg);
int set_data_gi(RTMP_ADAPTER *pAd, char * arg);
int set_data_basize(RTMP_ADAPTER *pAd, char * arg);
int set_fpga_mode(RTMP_ADAPTER *pAd, char * arg);
#endif /* FPGA_MODE */

#ifdef WFA_VHT_PF
int set_force_amsdu(RTMP_ADAPTER *pAd, char * arg);
#endif /* WFA_VHT_PF */



#ifdef RLT_RF
int set_rf(RTMP_ADAPTER *pAd, char * arg);
#endif /* RLT_RF */

bool CmdRspEventHandle(RTMP_ADAPTER *pAd);



#endif  /* __RTMP_H__ */

