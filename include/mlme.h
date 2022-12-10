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
	mlme.h

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	John Chang	2003-08-28		Created
	John Chang  2004-09-06      modified for RT2600
	
*/
#ifndef __MLME_H__
#define __MLME_H__

#include "rtmp_dot11.h"

#include "wpa_cmm.h"


/* maximum supported capability information - */
/* ESS, IBSS, Privacy, Short Preamble, Spectrum mgmt, Short Slot */
#define SUPPORTED_CAPABILITY_INFO   0x0533

#define END_OF_ARGS                 -1
#define LFSR_MASK                   0x80000057
#define MLME_TASK_EXEC_INTV         100/*200*/       /* */
#ifdef RT3290
#define LEAD_TIME                   7
#else
#define LEAD_TIME                   5
#endif /* RT3290 */

#define MLME_TASK_EXEC_MULTIPLE       10  /*5*/       /* MLME_TASK_EXEC_MULTIPLE * MLME_TASK_EXEC_INTV = 1 sec */
#define REORDER_EXEC_INTV         	100       /* 0.1 sec */
#ifdef RTMP_MAC_USB
#endif /* RTMP_MAC_USB */
/*#define TBTT_PRELOAD_TIME         384        // usec. LomgPreamble + 24-byte at 1Mbps */

/* The definition of Radar detection duration region */
#define CE		0
#define FCC		1
#define JAP		2
#define JAP_W53	3
#define JAP_W56	4
#define MAX_RD_REGION 5

#define BEACON_LOST_TIME            4 * OS_HZ    /* 2048 msec = 2 sec */

#define DLS_TIMEOUT                 1200      /* unit: msec */
#define AUTH_TIMEOUT                300       /* unit: msec */
#define ASSOC_TIMEOUT               300       /* unit: msec */
#define JOIN_TIMEOUT                2000        /* unit: msec */
#define SHORT_CHANNEL_TIME          90        /* unit: msec */
#define MIN_CHANNEL_TIME            110        /* unit: msec, for dual band scan */
#define MAX_CHANNEL_TIME            140       /* unit: msec, for single band scan */
#define	FAST_ACTIVE_SCAN_TIME	    30 		  /* Active scan waiting for probe response time */
#define CW_MIN_IN_BITS              4         /* actual CwMin = 2^CW_MIN_IN_BITS - 1 */
#define AUTO_CHANNEL_SEL_TIMEOUT		400		/* uint: msec */
#define LINK_DOWN_TIMEOUT           20000      /* unit: msec */
#define AUTO_WAKEUP_TIMEOUT			70			/*unit: msec */

#ifdef CONFIG_AP_SUPPORT 
#define CW_MAX_IN_BITS              6        /* actual CwMax = 2^CW_MAX_IN_BITS - 1 */
#endif /* CONFIG_AP_SUPPORT */


#ifdef CONFIG_APSTA_MIXED_SUPPORT
extern unsigned int CW_MAX_IN_BITS;
#endif /* CONFIG_APSTA_MIXED_SUPPORT */

/* Note: RSSI_TO_DBM_OFFSET has been changed to variable for new RF (2004-0720). */
/* SHould not refer to this constant anymore */
/*#define RSSI_TO_DBM_OFFSET          120 // for RT2530 RSSI-115 = dBm */
#define RSSI_FOR_MID_TX_POWER       -55  /* -55 db is considered mid-distance */
#define RSSI_FOR_LOW_TX_POWER       -45  /* -45 db is considered very short distance and */
                                        /* eligible to use a lower TX power */
#define RSSI_FOR_LOWEST_TX_POWER    -30
/*#define MID_TX_POWER_DELTA          0   // 0 db from full TX power upon mid-distance to AP */
#define LOW_TX_POWER_DELTA          6    /* -3 db from full TX power upon very short distance. 1 grade is 0.5 db */
#define LOWEST_TX_POWER_DELTA       16   /* -8 db from full TX power upon shortest distance. 1 grade is 0.5 db */

#define RSSI_TRIGGERED_UPON_BELOW_THRESHOLD     0
#define RSSI_TRIGGERED_UPON_EXCCEED_THRESHOLD   1
#define RSSI_THRESHOLD_FOR_ROAMING              25
#define RSSI_DELTA                              5

/* Channel Quality Indication */
#define CQI_IS_GOOD(cqi)            ((cqi) >= 50)
/*#define CQI_IS_FAIR(cqi)          (((cqi) >= 20) && ((cqi) < 50)) */
#define CQI_IS_POOR(cqi)            (cqi < 50)  /*(((cqi) >= 5) && ((cqi) < 20)) */
#define CQI_IS_BAD(cqi)             (cqi < 5)
#define CQI_IS_DEAD(cqi)            (cqi == 0)

/* weighting factor to calculate Channel quality, total should be 100% */
#define RSSI_WEIGHTING                   50
#define TX_WEIGHTING                     30
#define RX_WEIGHTING                     20


#define BSS_NOT_FOUND                    0xFFFFFFFF

#ifdef CONFIG_AP_SUPPORT
#define MAX_LEN_OF_MLME_QUEUE            20 /*10 */
#endif /* CONFIG_AP_SUPPORT */


enum SCAN_MODE{
	/* Active scan, send probe request, and wait beacon and probe response */
	SCAN_ACTIVE = 0x00,			/* all channels */
	SCAN_CISCO_ACTIVE = 0x1,	/* single channel only */
	FAST_SCAN_ACTIVE = 0x2,	
#ifdef WSC_INCLUDED
	SCAN_WSC_ACTIVE = 0x3,
#endif /* WSC_INCLUDED */
#ifdef DOT11N_DRAFT3
	SCAN_2040_BSS_COEXIST = 0x4,
#endif /* DOT11N_DRAFT3 */
	SCAN_ACTIVE_MAX,
	
	/* Passive scan, no probe request, only wait beacon and probe response */
	SCAN_PASSIVE = 0x80,		/* all channels */
	SCAN_CISCO_PASSIVE = 0x81,	/* single channel only */
	SCAN_CISCO_NOISE = 0x82,	/* single channel only, for noise histogram collection */
	SCAN_CISCO_CHANNEL_LOAD = 0x83,	/* single channel only, for channel load collection */
	SCAN_PASSIVE_MAX,
};

#define SCAN_MASK	0x80
#define SCAN_MODE_ACT(_x)	(((_x) & SCAN_MASK) == 0)
#define SCAN_MODE_PSV(_x)	(((_x) & SCAN_MASK) == SCAN_MASK)
#define SCAN_MODE_VALID(_x)	((SCAN_MODE_ACT(_x) && ((_x) < SCAN_ACTIVE_MAX)) ||\
								(SCAN_MODE_PSV(_x) && ((_x) < SCAN_PASSIVE_MAX)))

/*#define BSS_TABLE_EMPTY(x)             ((x).BssNr == 0) */
#define MAC_ADDR_IS_GROUP(Addr)       (((Addr[0]) & 0x01))
#define MAC_ADDR_HASH(Addr)            (Addr[0] ^ Addr[1] ^ Addr[2] ^ Addr[3] ^ Addr[4] ^ Addr[5])
#define MAC_ADDR_HASH_INDEX(Addr)      (MAC_ADDR_HASH(Addr) & (HASH_TABLE_SIZE - 1))
#define TID_MAC_HASH(Addr,TID)            (TID^Addr[0] ^ Addr[1] ^ Addr[2] ^ Addr[3] ^ Addr[4] ^ Addr[5])
#define TID_MAC_HASH_INDEX(Addr,TID)      (TID_MAC_HASH(Addr,TID) & (HASH_TABLE_SIZE - 1))


/* bit definition of the 2-byte pBEACON->Capability field */
#define CAP_IS_ESS_ON(x)                 (((x) & 0x0001) != 0)
#define CAP_IS_IBSS_ON(x)                (((x) & 0x0002) != 0)
#define CAP_IS_CF_POLLABLE_ON(x)         (((x) & 0x0004) != 0)
#define CAP_IS_CF_POLL_REQ_ON(x)         (((x) & 0x0008) != 0)
#define CAP_IS_PRIVACY_ON(x)             (((x) & 0x0010) != 0)
#define CAP_IS_SHORT_PREAMBLE_ON(x)      (((x) & 0x0020) != 0)
#define CAP_IS_PBCC_ON(x)                (((x) & 0x0040) != 0)
#define CAP_IS_AGILITY_ON(x)             (((x) & 0x0080) != 0)
#define CAP_IS_SPECTRUM_MGMT(x)          (((x) & 0x0100) != 0)  /* 802.11e d9 */
#define CAP_IS_QOS(x)                    (((x) & 0x0200) != 0)  /* 802.11e d9 */
#define CAP_IS_SHORT_SLOT(x)             (((x) & 0x0400) != 0)
#define CAP_IS_APSD(x)                   (((x) & 0x0800) != 0)  /* 802.11e d9 */
#define CAP_IS_IMMED_BA(x)               (((x) & 0x1000) != 0)  /* 802.11e d9 */
#define CAP_IS_DSSS_OFDM(x)              (((x) & 0x2000) != 0)
#define CAP_IS_DELAY_BA(x)               (((x) & 0x4000) != 0)  /* 802.11e d9 */

#define CAP_GENERATE(ess,ibss,priv,s_pre,s_slot,spectrum)  (((ess) ? 0x0001 : 0x0000) | ((ibss) ? 0x0002 : 0x0000) | ((priv) ? 0x0010 : 0x0000) | ((s_pre) ? 0x0020 : 0x0000) | ((s_slot) ? 0x0400 : 0x0000) | ((spectrum) ? 0x0100 : 0x0000))

/*#define STA_QOS_CAPABILITY               0 // 1-byte. see 802.11e d9.0 for bit definition */

#define ERP_IS_NON_ERP_PRESENT(x)        (((x) & 0x01) != 0)    /* 802.11g */
#define ERP_IS_USE_PROTECTION(x)         (((x) & 0x02) != 0)    /* 802.11g */
#define ERP_IS_USE_BARKER_PREAMBLE(x)    (((x) & 0x04) != 0)    /* 802.11g */

#define DRS_TX_QUALITY_WORST_BOUND       8/* 3  // just test by gary */
#define DRS_PENALTY                      8

#define BA_NOTUSE 	2
/*BA Policy subfiled value in ADDBA frame */
#define IMMED_BA 	1
#define DELAY_BA	0

/* BA Initiator subfield in DELBA frame */
#define ORIGINATOR	1
#define RECIPIENT	0

/* ADDBA Status Code */
#define ADDBA_RESULTCODE_SUCCESS					0
#define ADDBA_RESULTCODE_REFUSED					37
#define ADDBA_RESULTCODE_INVALID_PARAMETERS			38

/* DELBA Reason Code */
#define DELBA_REASONCODE_QSTA_LEAVING				36
#define DELBA_REASONCODE_END_BA						37
#define DELBA_REASONCODE_UNKNOWN_BA					38
#define DELBA_REASONCODE_TIMEOUT					39

/* reset all OneSecTx counters */
#ifdef FIFO_EXT_SUPPORT
#define RESET_ONE_SEC_TX_CNT(__pEntry) \
if (((__pEntry)) != NULL) \
{ \
	(__pEntry)->OneSecTxRetryOkCount = 0; \
	(__pEntry)->OneSecTxFailCount = 0; \
	(__pEntry)->OneSecTxNoRetryOkCount = 0; \
	(__pEntry)->OneSecRxLGICount = 0; \
	(__pEntry)->OneSecRxSGICount = 0; \
	(__pEntry)->fifoTxSucCnt = 0;\
	(__pEntry)->fifoTxRtyCnt = 0;\
}
#else
#define RESET_ONE_SEC_TX_CNT(__pEntry) \
if (((__pEntry)) != NULL) \
{ \
	(__pEntry)->OneSecTxRetryOkCount = 0; \
	(__pEntry)->OneSecTxFailCount = 0; \
	(__pEntry)->OneSecTxNoRetryOkCount = 0; \
	(__pEntry)->OneSecRxLGICount = 0; \
	(__pEntry)->OneSecRxSGICount = 0; \
}
#endif /* FIFO_EXT_SUPPORT */


/*
	802.11 frame formats
*/
/*  HT Capability INFO field in HT Cap IE . */
typedef struct GNU_PACKED{
#ifdef RT_BIG_ENDIAN
	unsigned short	LSIGTxopProSup:1;
	unsigned short	Forty_Mhz_Intolerant:1;
	unsigned short	PSMP:1;
	unsigned short	CCKmodein40:1;
	unsigned short	AMsduSize:1;
	unsigned short	DelayedBA:1;
	unsigned short	RxSTBC:2;
	unsigned short	TxSTBC:1;
	unsigned short	ShortGIfor40:1;
	unsigned short	ShortGIfor20:1;
	unsigned short	GF:1;
	unsigned short	MimoPs:2;
	unsigned short	ChannelWidth:1;
	unsigned short	AdvCoding:1;
#else
	unsigned short	AdvCoding:1;
	unsigned short	ChannelWidth:1;
	unsigned short	MimoPs:2;		/* mimo power safe */
	unsigned short	GF:1;			/* green field */
	unsigned short	ShortGIfor20:1;
	unsigned short	ShortGIfor40:1;	/* for40MHz */
	unsigned short	TxSTBC:1;		/* 0:not supported,  1:if supported */
	unsigned short	RxSTBC:2;
	unsigned short	DelayedBA:1;
	unsigned short	AMsduSize:1;	/* only support as zero */
	unsigned short	CCKmodein40:1;
	unsigned short	PSMP:1;
	unsigned short	Forty_Mhz_Intolerant:1;
	unsigned short	LSIGTxopProSup:1;
#endif /* RT_BIG_ENDIAN */
} HT_CAP_INFO, *PHT_CAP_INFO;


/*  HT Capability INFO field in HT Cap IE . */
typedef struct GNU_PACKED _HT_CAP_PARM{
#ifdef RT_BIG_ENDIAN
	unsigned char	rsv:3;/*momi power safe */
	unsigned char	MpduDensity:3;
	unsigned char	MaxRAmpduFactor:2;
#else
	unsigned char	MaxRAmpduFactor:2;
	unsigned char	MpduDensity:3;
	unsigned char	rsv:3;/*momi power safe */
#endif /* RT_BIG_ENDIAN */
} HT_CAP_PARM, *PHT_CAP_PARM;


typedef struct GNU_PACKED _HT_MCS_SET_TX_SUBFIELD{
#ifdef RT_BIG_ENDIAN
	unsigned char	TxMCSSetDefined:1; 
	unsigned char	TxRxNotEqual:1;
	unsigned char	TxMaxStream:2;
	unsigned char	TxUnqualModulation:1;
	unsigned char	rsv:3;
#else
	unsigned char	rsv:3;
	unsigned char	TxUnqualModulation:1;
	unsigned char	TxMaxStream:2;
	unsigned char	TxRxNotEqual:1;
	unsigned char	TxMCSSetDefined:1;
#endif /* RT_BIG_ENDIAN */
}HT_MCS_SET_TX_SUBFIELD, *PHT_MCS_SET_TX_SUBFIELD;


/*  HT Capability INFO field in HT Cap IE . */
typedef struct GNU_PACKED _HT_MCS_SET{
	unsigned char	MCSSet[10];
	unsigned char	SupRate[2];  /* unit : 1Mbps */
#ifdef RT_BIG_ENDIAN
	unsigned char	rsv:3;
	unsigned char	MpduDensity:1;
	unsigned char	TxStream:2;
	unsigned char	TxRxNotEqual:1;
	unsigned char	TxMCSSetDefined:1; 
#else
	unsigned char	TxMCSSetDefined:1; 
	unsigned char	TxRxNotEqual:1;
	unsigned char	TxStream:2;
	unsigned char	MpduDensity:1;
	unsigned char	rsv:3;
#endif /* RT_BIG_ENDIAN */
	unsigned char	rsv3[3];  
} HT_MCS_SET, *PHT_MCS_SET;

/*  HT Capability INFO field in HT Cap IE . */
typedef struct GNU_PACKED _EXT_HT_CAP_INFO{
#ifdef RT_BIG_ENDIAN 
	unsigned short	rsv2:4;
	unsigned short	RDGSupport:1;	/*reverse Direction Grant  support */
	unsigned short	PlusHTC:1;	/*+HTC control field support */
	unsigned short	MCSFeedback:2;	/*0:no MCS feedback, 2:unsolicited MCS feedback, 3:Full MCS feedback,  1:rsv. */
	unsigned short	rsv:5;/*momi power safe */
	unsigned short	TranTime:2;
	unsigned short	Pco:1;
#else
	unsigned short	Pco:1;
	unsigned short	TranTime:2;
	unsigned short	rsv:5;/*momi power safe */
	unsigned short	MCSFeedback:2;	/*0:no MCS feedback, 2:unsolicited MCS feedback, 3:Full MCS feedback,  1:rsv. */
	unsigned short	PlusHTC:1;	/*+HTC control field support */
	unsigned short	RDGSupport:1;	/*reverse Direction Grant  support */
	unsigned short	rsv2:4;
#endif /* RT_BIG_ENDIAN */
} EXT_HT_CAP_INFO, *PEXT_HT_CAP_INFO;

/* HT Explicit Beamforming Feedback Capable */
#define HT_ExBF_FB_CAP_NONE			0
#define HT_ExBF_FB_CAP_DELAYED		1
#define HT_ExBF_FB_CAP_IMMEDIATE		2
#define HT_ExBF_FB_CAP_BOTH			3

/* HT Beamforming field in HT Cap IE */
typedef struct GNU_PACKED _HT_BF_CAP{
#ifdef RT_BIG_ENDIAN
	unsigned long	rsv:3;
	unsigned long	ChanEstimation:2;
	unsigned long	CSIRowBFSup:2;
	unsigned long	ComSteerBFAntSup:2;
	unsigned long	NoComSteerBFAntSup:2;
	unsigned long	CSIBFAntSup:2;
	unsigned long	MinGrouping:2;
	unsigned long	ExpComBF:2;	
	unsigned long	ExpNoComBF:2;
	unsigned long	ExpCSIFbk:2;
	unsigned long	ExpComSteerCapable:1;
	unsigned long	ExpNoComSteerCapable:1;
	unsigned long	ExpCSICapable:1;
	unsigned long	Calibration:2;
	unsigned long	ImpTxBFCapable:1;
	unsigned long	TxNDPCapable:1;	
	unsigned long	RxNDPCapable:1;	
	unsigned long	TxSoundCapable:1;
	unsigned long	RxSoundCapable:1;
	unsigned long	TxBFRecCapable:1;
#else
	unsigned long	TxBFRecCapable:1;
	unsigned long	RxSoundCapable:1;
	unsigned long	TxSoundCapable:1;
	unsigned long	RxNDPCapable:1;	
	unsigned long	TxNDPCapable:1;	
	unsigned long	ImpTxBFCapable:1;	
	unsigned long	Calibration:2;
	unsigned long	ExpCSICapable:1;
	unsigned long	ExpNoComSteerCapable:1;
	unsigned long	ExpComSteerCapable:1;
	unsigned long	ExpCSIFbk:2;	
	unsigned long	ExpNoComBF:2;	
	unsigned long	ExpComBF:2;	
	unsigned long	MinGrouping:2;
	unsigned long	CSIBFAntSup:2;
	unsigned long	NoComSteerBFAntSup:2;
	unsigned long	ComSteerBFAntSup:2;
	unsigned long	CSIRowBFSup:2;
	unsigned long	ChanEstimation:2;
	unsigned long	rsv:3;
#endif /* RT_BIG_ENDIAN */
} HT_BF_CAP, *PHT_BF_CAP;

/*  HT antenna selection field in HT Cap IE . */
typedef struct GNU_PACKED _HT_AS_CAP{
#ifdef RT_BIG_ENDIAN
	unsigned char	rsv:1;
	unsigned char	TxSoundPPDU:1;
	unsigned char	RxASel:1;
	unsigned char	AntIndFbk:1;
	unsigned char	ExpCSIFbk:1;
	unsigned char	AntIndFbkTxASEL:1;
	unsigned char	ExpCSIFbkTxASEL:1;
	unsigned char	AntSelect:1; 
#else
	unsigned char	AntSelect:1; 
	unsigned char	ExpCSIFbkTxASEL:1;
	unsigned char	AntIndFbkTxASEL:1;
	unsigned char	ExpCSIFbk:1;
	unsigned char	AntIndFbk:1;
	unsigned char	RxASel:1;
	unsigned char	TxSoundPPDU:1;
	unsigned char	rsv:1;
#endif /* RT_BIG_ENDIAN */
} HT_AS_CAP, *PHT_AS_CAP;

/* Draft 1.0 set IE length 26, but is extensible.. */
#define SIZE_HT_CAP_IE		26
/* The structure for HT Capability IE. */
typedef struct GNU_PACKED _HT_CAPABILITY_IE{
	HT_CAP_INFO		HtCapInfo;
	HT_CAP_PARM		HtCapParm;
/*	HT_MCS_SET		HtMCSSet; */
	unsigned char			MCSSet[16];
	EXT_HT_CAP_INFO	ExtHtCapInfo;
	HT_BF_CAP		TxBFCap;	/* beamforming cap. rt2860c not support beamforming. */
	HT_AS_CAP		ASCap;	/*antenna selection. */
} HT_CAPABILITY_IE, *PHT_CAPABILITY_IE;


/* 802.11n draft3 related structure definitions. */
/* 7.3.2.60 */
#define dot11OBSSScanPassiveDwell							20	/* in TU. min amount of time that the STA continously scans each channel when performing an active OBSS scan. */
#define dot11OBSSScanActiveDwell							10	/* in TU.min amount of time that the STA continously scans each channel when performing an passive OBSS scan. */
#define dot11BSSWidthTriggerScanInterval					300  /* in sec. max interval between scan operations to be performed to detect BSS channel width trigger events. */
#define dot11OBSSScanPassiveTotalPerChannel					200	/* in TU. min total amount of time that the STA scans each channel when performing a passive OBSS scan. */
#define dot11OBSSScanActiveTotalPerChannel					20	/*in TU. min total amount of time that the STA scans each channel when performing a active OBSS scan */
#define dot11BSSWidthChannelTransactionDelayFactor			5	/* min ratio between the delay time in performing a switch from 20MHz BSS to 20/40 BSS operation and the maximum */
																/*	interval between overlapping BSS scan operations. */
#define dot11BSSScanActivityThreshold						25	/* in %%, max total time that a STA may be active on the medium during a period of */
																/*	(dot11BSSWidthChannelTransactionDelayFactor * dot11BSSWidthTriggerScanInterval) seconds without */
																/*	being obligated to perform OBSS Scan operations. default is 25(== 0.25%) */

typedef struct GNU_PACKED _OVERLAP_BSS_SCAN_IE{
	unsigned short		ScanPassiveDwell;
	unsigned short		ScanActiveDwell;
	unsigned short		TriggerScanInt;				/* Trigger scan interval */
	unsigned short		PassiveTalPerChannel;		/* passive total per channel */
	unsigned short		ActiveTalPerChannel;		/* active total per channel */
	unsigned short		DelayFactor;				/* BSS width channel transition delay factor */
	unsigned short		ScanActThre;				/* Scan Activity threshold */
}OVERLAP_BSS_SCAN_IE, *POVERLAP_BSS_SCAN_IE;


/*  7.3.2.56. 20/40 Coexistence element used in  Element ID = 72 = IE_2040_BSS_COEXIST */
typedef union GNU_PACKED _BSS_2040_COEXIST_IE{
 struct GNU_PACKED {
 #ifdef RT_BIG_ENDIAN
	unsigned char	rsv:3;
 	unsigned char	ObssScanExempGrant:1;
	unsigned char	ObssScanExempReq:1;
	unsigned char	BSS20WidthReq:1;
	unsigned char	Intolerant40:1;	
	unsigned char	InfoReq:1; 
 #else
	unsigned char	InfoReq:1;
	unsigned char	Intolerant40:1;			/* Inter-BSS. set 1 when prohibits a receiving BSS from operating as a 20/40 Mhz BSS. */
	unsigned char	BSS20WidthReq:1;		/* Intra-BSS set 1 when prohibits a receiving AP from operating its BSS as a 20/40MHz BSS. */
	unsigned char	ObssScanExempReq:1;
	unsigned char	ObssScanExempGrant:1;
	unsigned char	rsv:3;
#endif /* RT_BIG_ENDIAN */
    } field;
 unsigned char   word;
} BSS_2040_COEXIST_IE, *PBSS_2040_COEXIST_IE;


typedef struct  _TRIGGER_EVENTA{
	bool			bValid;	 
	unsigned char	BSSID[6];	
	unsigned char	RegClass;	/* Regulatory Class */
	unsigned short	Channel;
} TRIGGER_EVENTA, *PTRIGGER_EVENTA;


/* 20/40 trigger event table */
/* If one Event A delete or created, or if Event B is detected or not detected, STA should send 2040BSSCoexistence to AP. */
#define MAX_TRIGGER_EVENT		64
typedef struct  _TRIGGER_EVENT_TAB{
	unsigned char	EventANo;	
	TRIGGER_EVENTA	EventA[MAX_TRIGGER_EVENT];	
	unsigned long			EventBCountDown;	/* Count down counter for Event B. */
} TRIGGER_EVENT_TAB, *PTRIGGER_EVENT_TAB;


/* 7.3.27 20/40 Bss Coexistence Mgmt capability used in extended capabilities information IE( ID = 127 = IE_EXT_CAPABILITY). */
/*	This is the first octet and was defined in 802.11n D3.03 and 802.11yD9.0 */
typedef struct GNU_PACKED _EXT_CAP_INFO_ELEMENT{
#ifdef RT_BIG_ENDIAN
	// TODO: shiang-6590, check the data structure format if this IE
	unsigned int	rsv7:1;
	unsigned int	TDLSChSwitchSupport:1; /* bit30: TDLS Channel Switching */
	unsigned int	TDLSPeerPSMSupport:1; /* bit29: TDLS Peer PSM Support */
	unsigned int	UAPSDBufSTASupport:1; /* bit28: Peer U-APSD Buffer STA Support */
	unsigned int	rsv6:1;
	unsigned int	DMSSupport:1;
	unsigned int	rsv5:6;
	unsigned int	BssTransitionManmt:1;
	unsigned int	rsv4:1;
	unsigned int	WNMSleepSupport:1;/*bit 17*/
	unsigned int	TFSSupport:1;/*bit 16*/
	unsigned int	rsv3:4;
	unsigned int	FMSSupport:1;/*bit 11*/
	unsigned int	rsv2:8;
	unsigned int	ExtendChannelSwitch:1;
	unsigned int	rsv:1;
	unsigned int	BssCoexistMgmtSupport:1;
#else
	unsigned int	BssCoexistMgmtSupport:1;
	unsigned int	rsv:1;
	unsigned int	ExtendChannelSwitch:1;
	unsigned int	rsv2:8;
	unsigned int	FMSSupport:1;/*bit 11*/
	unsigned char	proxy_arp:1;
	unsigned int	rsv3:3;
	unsigned int	TFSSupport:1;/*bit 16*/
	unsigned int	WNMSleepSupport:1;/*bit 17*/
	unsigned int	rsv4:1;
	unsigned int	BssTransitionManmt:1;
	unsigned int	rsv5:6;
	unsigned int	DMSSupport:1;
	unsigned int	rsv6:1;
	unsigned int	UAPSDBufSTASupport:1; /* bit28: Peer U-APSD Buffer STA Support */
	unsigned int	TDLSPeerPSMSupport:1; /* bit29: TDLS Peer PSM Support */
	unsigned int	TDLSChSwitchSupport:1; /* bit30: TDLS Channel Switching */
	unsigned int	rsv7:1;
	unsigned int	rsv8:8;
	unsigned int	rsv9:8;
	unsigned char	utf8_ssid:1;
#endif /* RT_BIG_ENDIAN */

}EXT_CAP_INFO_ELEMENT, *PEXT_CAP_INFO_ELEMENT;


/* 802.11n 7.3.2.61 */
typedef struct GNU_PACKED _BSS_2040_COEXIST_ELEMENT{
	unsigned char					ElementID;		/* ID = IE_2040_BSS_COEXIST = 72 */
	unsigned char					Len;
	BSS_2040_COEXIST_IE		BssCoexistIe;
}BSS_2040_COEXIST_ELEMENT, *PBSS_2040_COEXIST_ELEMENT;


/*802.11n 7.3.2.59 */
typedef struct GNU_PACKED _BSS_2040_INTOLERANT_CH_REPORT{
	unsigned char				ElementID;		/* ID = IE_2040_BSS_INTOLERANT_REPORT = 73 */
	unsigned char				Len;	
	unsigned char				RegulatoryClass;
	unsigned char				ChList[0];
}BSS_2040_INTOLERANT_CH_REPORT, *PBSS_2040_INTOLERANT_CH_REPORT;


/* The structure for channel switch annoucement IE. This is in 802.11n D3.03 */
typedef struct GNU_PACKED _CHA_SWITCH_ANNOUNCE_IE{
	unsigned char			SwitchMode;	/*channel switch mode */
	unsigned char			NewChannel;	/* */
	unsigned char			SwitchCount;	/* */
} CHA_SWITCH_ANNOUNCE_IE, *PCHA_SWITCH_ANNOUNCE_IE;


/* The structure for channel switch annoucement IE. This is in 802.11n D3.03 */
typedef struct GNU_PACKED _SEC_CHA_OFFSET_IE{
	unsigned char			SecondaryChannelOffset;	 /* 1: Secondary above, 3: Secondary below, 0: no Secondary */
} SEC_CHA_OFFSET_IE, *PSEC_CHA_OFFSET_IE;


/* This structure is extracted from struct RT_HT_CAPABILITY and RT_VHT_CAP */
typedef struct _RT_PHY_INFO{
	bool		bHtEnable;	 /* If we should use ht rate. */
	bool		bPreNHt;	 /* If we should use ht rate. */
	/*Substract from HT Capability IE */
	unsigned char		MCSSet[16];
#ifdef DOT11_VHT_AC
	bool 	bVhtEnable;
	unsigned char 		vht_bw;
	VHT_MCS_SET vht_mcs_set;
#endif /* DOT11_VHT_AC */
} RT_PHY_INFO;


#ifdef DOT11_VHT_AC
typedef struct _RT_VHT_CAP{
	unsigned int vht_bw:2;
	unsigned int vht_txstbc:1;
	unsigned int vht_rxstbc:3;
	unsigned int sgi_80m:1;
	unsigned int vht_htc:1;

	unsigned int vht_mcs_ss1:2;
	unsigned int vht_mcs_ss2:2;
	unsigned int vht_rx_rate:2;
	unsigned int vht_tx_rate:2;

	unsigned int rsv:16;
}RT_VHT_CAP;
#endif /* DOT11_VHT_AC */


/*
	This structure substracts ralink supports from all 802.11n-related features.
	Features not listed here but contained in 802.11n spec are not supported in rt2860
*/
typedef struct {
#ifdef RT_BIG_ENDIAN
	unsigned short	rsv:5;
	unsigned short	AmsduSize:1;	/* Max receiving A-MSDU size */
	unsigned short	AmsduEnable:1;	/* Enable to transmit A-MSDU. Suggest disable. We should use A-MPDU to gain best benifit of 802.11n */
	unsigned short	RxSTBC:2;
	unsigned short	TxSTBC:1;
	unsigned short	ShortGIfor40:1;	/*for40MHz */
	unsigned short	ShortGIfor20:1;
	unsigned short	GF:1;	/*green field */
	unsigned short	MimoPs:2;/*mimo power safe MMPS_ */
	unsigned short	ChannelWidth:1;
#else
	unsigned short	ChannelWidth:1;
	unsigned short	MimoPs:2;/*mimo power safe MMPS_ */
	unsigned short	GF:1;	/*green field */
	unsigned short	ShortGIfor20:1;
	unsigned short	ShortGIfor40:1;	/*for 40MHz */
	unsigned short	TxSTBC:1;	/* 0:not supported,  1:if supported */
	unsigned short	RxSTBC:2;	/* 2 bits */
	unsigned short	AmsduEnable:1;	/* Enable to transmit A-MSDU. Suggest disable. We should use A-MPDU to gain best benifit of 802.11n */
	unsigned short	AmsduSize:1;	/* Max receiving A-MSDU size */
	unsigned short	rsv:5;
#endif

	/*Substract from Addiont HT INFO IE */
#ifdef RT_BIG_ENDIAN
	unsigned char	RecomWidth:1;
	unsigned char	ExtChanOffset:2;	/* Please not the difference with following 	unsigned char	NewExtChannelOffset; from 802.11n */
	unsigned char	MpduDensity:3;	
	unsigned char	MaxRAmpduFactor:2;
#else
	unsigned char	MaxRAmpduFactor:2;
	unsigned char	MpduDensity:3;
	unsigned char	ExtChanOffset:2;	/* Please not the difference with following 	unsigned char	NewExtChannelOffset; from 802.11n */
	unsigned char	RecomWidth:1;
#endif

#ifdef RT_BIG_ENDIAN
	unsigned short	rsv2:11;
	unsigned short	OBSS_NonHTExist:1;
	unsigned short	rsv3:1;
	unsigned short	NonGfPresent:1;
	unsigned short	OperaionMode:2;
#else
	unsigned short	OperaionMode:2;
	unsigned short	NonGfPresent:1;
	unsigned short	rsv3:1;
	unsigned short	OBSS_NonHTExist:1;
	unsigned short	rsv2:11;
#endif
	
	/* New Extension Channel Offset IE */
	unsigned char	NewExtChannelOffset;	
	/* Extension Capability IE = 127 */
	unsigned char	BSSCoexist2040;	
} RT_HT_CAPABILITY, *PRT_HT_CAPABILITY;


/*   field in Addtional HT Information IE . */
typedef struct GNU_PACKED _ADD_HTINFO{
#ifdef RT_BIG_ENDIAN
	unsigned char	SerInterGranu:3;
	unsigned char	S_PSMPSup:1;
	unsigned char	RifsMode:1; 
	unsigned char	RecomWidth:1;	
	unsigned char	ExtChanOffset:2;
#else
	unsigned char	ExtChanOffset:2;
	unsigned char	RecomWidth:1;
	unsigned char	RifsMode:1; 
	unsigned char	S_PSMPSup:1;	 /*Indicate support for scheduled PSMP */
	unsigned char	SerInterGranu:3;	 /*service interval granularity */
#endif
} ADD_HTINFO, *PADD_HTINFO;


typedef struct GNU_PACKED _ADD_HTINFO2{
#ifdef RT_BIG_ENDIAN
	unsigned short	rsv2:11; 
	unsigned short	OBSS_NonHTExist:1;
	unsigned short	rsv:1;
	unsigned short	NonGfPresent:1;
	unsigned short	OperaionMode:2;	
#else
	unsigned short	OperaionMode:2;
	unsigned short	NonGfPresent:1;
	unsigned short	rsv:1; 
	unsigned short	OBSS_NonHTExist:1;
	unsigned short	rsv2:11; 
#endif
} ADD_HTINFO2, *PADD_HTINFO2;


/* TODO: Need sync with spec about the definition of StbcMcs. In Draft 3.03, it's reserved. */
typedef struct GNU_PACKED _ADD_HTINFO3{
#ifdef RT_BIG_ENDIAN
	unsigned short	rsv:4;
	unsigned short	PcoPhase:1;
	unsigned short	PcoActive:1;
	unsigned short	LsigTxopProt:1;
	unsigned short	STBCBeacon:1;
	unsigned short	DualCTSProtect:1;
	unsigned short	DualBeacon:1;
	unsigned short	StbcMcs:6;
#else
	unsigned short	StbcMcs:6;
	unsigned short	DualBeacon:1;
	unsigned short	DualCTSProtect:1; 
	unsigned short	STBCBeacon:1;
	unsigned short	LsigTxopProt:1;	/* L-SIG TXOP protection full support */
	unsigned short	PcoActive:1; 
	unsigned short	PcoPhase:1; 
	unsigned short	rsv:4; 
#endif /* RT_BIG_ENDIAN */
} ADD_HTINFO3, *PADD_HTINFO3;

#define SIZE_ADD_HT_INFO_IE		22
typedef struct  GNU_PACKED _ADD_HT_INFO_IE{
	unsigned char				ControlChan;
	ADD_HTINFO			AddHtInfo;
	ADD_HTINFO2			AddHtInfo2;	 
	ADD_HTINFO3			AddHtInfo3;	 
	unsigned char				MCSSet[16];		/* Basic MCS set */
} ADD_HT_INFO_IE, *PADD_HT_INFO_IE;

typedef struct  GNU_PACKED _NEW_EXT_CHAN_IE{
	unsigned char				NewExtChanOffset;
} NEW_EXT_CHAN_IE, *PNEW_EXT_CHAN_IE;

typedef struct GNU_PACKED _FRAME_802_11 {
    HEADER_802_11   Hdr;
    unsigned char            Octet[1];
} FRAME_802_11, *PFRAME_802_11;

/* QoSNull embedding of management action. When HT Control MA field set to 1. */
typedef struct GNU_PACKED _MA_BODY {
    unsigned char            Category;
    unsigned char            Action;
    unsigned char            Octet[1];
} MA_BODY, *PMA_BODY;

typedef	struct GNU_PACKED _HEADER_802_3	{
    unsigned char           DAAddr1[MAC_ADDR_LEN];
    unsigned char           SAAddr2[MAC_ADDR_LEN];
    unsigned char           Octet[2];
} HEADER_802_3, *PHEADER_802_3;


/* Block ACK related format */
/* 2-byte BA Parameter  field  in 	DELBA frames to terminate an already set up bA */
typedef struct GNU_PACKED _DELBA_PARM{
#ifdef RT_BIG_ENDIAN
    unsigned short      TID:4;	/* value of TC os TS */
    unsigned short      Initiator:1;	/* 1: originator    0:recipient */
    unsigned short      Rsv:11;	/* always set to 0 */
#else
    unsigned short      Rsv:11;	/* always set to 0 */
    unsigned short      Initiator:1;	/* 1: originator    0:recipient */
    unsigned short      TID:4;	/* value of TC os TS */
#endif /* RT_BIG_ENDIAN */
} DELBA_PARM, *PDELBA_PARM;

/* 2-byte BA Parameter Set field  in ADDBA frames to signal parm for setting up a BA */
typedef struct GNU_PACKED _BA_PARM{
#ifdef RT_BIG_ENDIAN
    unsigned short      BufSize:10;	/* number of buffe of size 2304 octetsr */
    unsigned short      TID:4;	/* value of TC os TS */
    unsigned short      BAPolicy:1;	/* 1: immediately BA    0:delayed BA */
    unsigned short      AMSDUSupported:1;	/* 0: not permitted		1: permitted */
#else
    unsigned short      AMSDUSupported:1;	/* 0: not permitted		1: permitted */
    unsigned short      BAPolicy:1;	/* 1: immediately BA    0:delayed BA */
    unsigned short      TID:4;	/* value of TC os TS */
    unsigned short      BufSize:10;	/* number of buffe of size 2304 octetsr */
#endif /* RT_BIG_ENDIAN */
} BA_PARM, *PBA_PARM;

/* 2-byte BA Starting Seq CONTROL field */
typedef union GNU_PACKED _BASEQ_CONTROL{
    struct GNU_PACKED {
#ifdef RT_BIG_ENDIAN
    unsigned short      StartSeq:12;   /* sequence number of the 1st MSDU for which this BAR is sent */
	unsigned short      FragNum:4;	/* always set to 0 */
#else
    unsigned short      FragNum:4;	/* always set to 0 */
	unsigned short      StartSeq:12;   /* sequence number of the 1st MSDU for which this BAR is sent */
#endif /* RT_BIG_ENDIAN */
    }   field;
    unsigned short           word;
} BASEQ_CONTROL, *PBASEQ_CONTROL;

/*BAControl and BARControl are the same */
/* 2-byte BA CONTROL field in BA frame */
typedef struct GNU_PACKED _BA_CONTROL{
#ifdef RT_BIG_ENDIAN
    unsigned short      TID:4;
    unsigned short      Rsv:9;
    unsigned short      Compressed:1;
    unsigned short      MTID:1;		/*EWC V1.24 */
    unsigned short      ACKPolicy:1; /* only related to N-Delayed BA. But not support in RT2860b. 0:NormalACK  1:No ACK */
#else
    unsigned short      ACKPolicy:1; /* only related to N-Delayed BA. But not support in RT2860b. 0:NormalACK  1:No ACK */
    unsigned short      MTID:1;		/*EWC V1.24 */
    unsigned short      Compressed:1;
    unsigned short      Rsv:9;
    unsigned short      TID:4;
#endif /* RT_BIG_ENDIAN */
} BA_CONTROL, *PBA_CONTROL;

/* 2-byte BAR CONTROL field in BAR frame */
typedef struct GNU_PACKED _BAR_CONTROL{
#ifdef RT_BIG_ENDIAN
    unsigned short      TID:4;
    unsigned short      Rsv1:9;
    unsigned short      Compressed:1;
    unsigned short      MTID:1;		/*if this bit1, use  FRAME_MTBA_REQ,  if 0, use FRAME_BA_REQ */
    unsigned short      ACKPolicy:1;
#else
    unsigned short      ACKPolicy:1; /* 0:normal ack,  1:no ack. */
    unsigned short      MTID:1;		/*if this bit1, use  FRAME_MTBA_REQ,  if 0, use FRAME_BA_REQ */
    unsigned short      Compressed:1;
    unsigned short      Rsv1:9;
    unsigned short      TID:4;
#endif /* !RT_BIG_ENDIAN */
} BAR_CONTROL, *PBAR_CONTROL;

/* BARControl in MTBAR frame */
typedef struct GNU_PACKED _MTBAR_CONTROL{
#ifdef RT_BIG_ENDIAN
    unsigned short      NumTID:4;
    unsigned short      Rsv1:9;
    unsigned short      Compressed:1;
    unsigned short      MTID:1;		
    unsigned short      ACKPolicy:1;
#else
    unsigned short      ACKPolicy:1;
    unsigned short      MTID:1;		
    unsigned short      Compressed:1;
    unsigned short      Rsv1:9;
    unsigned short      NumTID:4;
#endif /* RT_BIG_ENDIAN */
} MTBAR_CONTROL, *PMTBAR_CONTROL;

typedef struct GNU_PACKED _PER_TID_INFO{
#ifdef RT_BIG_ENDIAN
    unsigned short      TID:4;
    unsigned short      Rsv1:12;
#else
    unsigned short      Rsv1:12;
    unsigned short      TID:4;
#endif /* RT_BIG_ENDIAN */
} PER_TID_INFO, *PPER_TID_INFO;

typedef struct {
	PER_TID_INFO      PerTID;
	BASEQ_CONTROL 	 BAStartingSeq;
} EACH_TID, *PEACH_TID;


/* BAREQ AND MTBAREQ have the same subtype BAR, 802.11n BAR use compressed bitmap. */
typedef struct GNU_PACKED _FRAME_BA_REQ {
	FRAME_CONTROL   FC;
	unsigned short          Duration;
	unsigned char           Addr1[MAC_ADDR_LEN];
	unsigned char           Addr2[MAC_ADDR_LEN];
	BAR_CONTROL  BARControl;
	BASEQ_CONTROL 	 BAStartingSeq;
}   FRAME_BA_REQ, *PFRAME_BA_REQ;

typedef struct GNU_PACKED _FRAME_MTBA_REQ {
	FRAME_CONTROL   FC;
	unsigned short          Duration;
	unsigned char           Addr1[MAC_ADDR_LEN];
	unsigned char           Addr2[MAC_ADDR_LEN];
	MTBAR_CONTROL  MTBARControl;
	PER_TID_INFO	PerTIDInfo;
	BASEQ_CONTROL 	 BAStartingSeq;
}   FRAME_MTBA_REQ, *PFRAME_MTBA_REQ;

/* Compressed format is mandantory in HT STA */
typedef struct GNU_PACKED _FRAME_MTBA {
	FRAME_CONTROL   FC;
	unsigned short          Duration;
	unsigned char           Addr1[MAC_ADDR_LEN];
	unsigned char           Addr2[MAC_ADDR_LEN];
	BA_CONTROL  BAControl;
	BASEQ_CONTROL 	 BAStartingSeq;
	unsigned char		BitMap[8];
}   FRAME_MTBA, *PFRAME_MTBA;

typedef struct GNU_PACKED _FRAME_PSMP_ACTION {
	HEADER_802_11   Hdr;
	unsigned char	Category;
	unsigned char	Action;
	unsigned char	Psmp;	/* 7.3.1.25 */
}   FRAME_PSMP_ACTION, *PFRAME_PSMP_ACTION;

typedef struct GNU_PACKED _FRAME_ACTION_HDR {
	HEADER_802_11   Hdr;
	unsigned char	Category;
	unsigned char	Action;
}   FRAME_ACTION_HDR, *PFRAME_ACTION_HDR;

/*Action Frame */
/*Action Frame  Category:Spectrum,  Action:Channel Switch. 7.3.2.20 */
typedef struct GNU_PACKED _CHAN_SWITCH_ANNOUNCE {
	unsigned char					ElementID;	/* ID = IE_CHANNEL_SWITCH_ANNOUNCEMENT = 37 */
	unsigned char					Len;
	CHA_SWITCH_ANNOUNCE_IE	CSAnnounceIe;
}   CHAN_SWITCH_ANNOUNCE, *PCHAN_SWITCH_ANNOUNCE;


/*802.11n : 7.3.2.20a */
typedef struct GNU_PACKED _SECOND_CHAN_OFFSET {
	unsigned char				ElementID;		/* ID = IE_SECONDARY_CH_OFFSET = 62 */
	unsigned char				Len;
	SEC_CHA_OFFSET_IE	SecChOffsetIe;
}   SECOND_CHAN_OFFSET, *PSECOND_CHAN_OFFSET;


typedef struct GNU_PACKED _FRAME_SPETRUM_CS {
	HEADER_802_11   Hdr;
	unsigned char	Category;
	unsigned char	Action;
	CHAN_SWITCH_ANNOUNCE	CSAnnounce;
	SECOND_CHAN_OFFSET		SecondChannel;
}   FRAME_SPETRUM_CS, *PFRAME_SPETRUM_CS;


typedef struct GNU_PACKED _FRAME_ADDBA_REQ {
	HEADER_802_11   Hdr;
	unsigned char	Category;
	unsigned char	Action;
	unsigned char	Token;	/* 1 */
	BA_PARM		BaParm;	      /*  2 - 10 */
	unsigned short		TimeOutValue;	/* 0 - 0 */
	BASEQ_CONTROL	BaStartSeq; /* 0-0 */
}   FRAME_ADDBA_REQ, *PFRAME_ADDBA_REQ;

typedef struct GNU_PACKED _FRAME_ADDBA_RSP {
	HEADER_802_11   Hdr;
	unsigned char	Category;
	unsigned char	Action;
	unsigned char	Token;
	unsigned short	StatusCode;
	BA_PARM		BaParm; /*0 - 2 */
	unsigned short		TimeOutValue;
}   FRAME_ADDBA_RSP, *PFRAME_ADDBA_RSP;

typedef struct GNU_PACKED _FRAME_DELBA_REQ {
	HEADER_802_11   Hdr;
	unsigned char	Category;
	unsigned char	Action;
	DELBA_PARM		DelbaParm;
	unsigned short	ReasonCode;
}   FRAME_DELBA_REQ, *PFRAME_DELBA_REQ;


/*7.2.1.7 */
typedef struct GNU_PACKED _FRAME_BAR {
	FRAME_CONTROL   FC;
	unsigned short          Duration;
	unsigned char           Addr1[MAC_ADDR_LEN];
	unsigned char           Addr2[MAC_ADDR_LEN];
	BAR_CONTROL		BarControl;
	BASEQ_CONTROL	StartingSeq;
}   FRAME_BAR, *PFRAME_BAR;

/*7.2.1.7 */
typedef struct GNU_PACKED _FRAME_BA {
	FRAME_CONTROL   FC;
	unsigned short          Duration;
	unsigned char           Addr1[MAC_ADDR_LEN];
	unsigned char           Addr2[MAC_ADDR_LEN];
	BAR_CONTROL		BarControl;
	BASEQ_CONTROL	StartingSeq;
	unsigned char		bitmask[8];
}   FRAME_BA, *PFRAME_BA;


/* Radio Measuement Request Frame Format */
typedef struct GNU_PACKED _FRAME_RM_REQ_ACTION {
	HEADER_802_11   Hdr;
	unsigned char	Category;
	unsigned char	Action;
	unsigned char	Token;
	unsigned short	Repetition;
	unsigned char   data[0];
}   FRAME_RM_REQ_ACTION, *PFRAME_RM_REQ_ACTION;

typedef struct GNU_PACKED _HT_EXT_CHANNEL_SWITCH_ANNOUNCEMENT_IE{
	unsigned char		ID;
	unsigned char		Length;
	unsigned char		ChannelSwitchMode;
	unsigned char		NewRegClass;
	unsigned char		NewChannelNum;
	unsigned char		ChannelSwitchCount;
} HT_EXT_CHANNEL_SWITCH_ANNOUNCEMENT_IE, *PHT_EXT_CHANNEL_SWITCH_ANNOUNCEMENT_IE;


/* */
/* _Limit must be the 2**n - 1 */
/* _SEQ1 , _SEQ2 must be within 0 ~ _Limit */
/* */
#define SEQ_STEPONE(_SEQ1, _SEQ2, _Limit)	((_SEQ1 == ((_SEQ2+1) & _Limit)))
#define SEQ_SMALLER(_SEQ1, _SEQ2, _Limit)	(((_SEQ1-_SEQ2) & ((_Limit+1)>>1)))
#define SEQ_LARGER(_SEQ1, _SEQ2, _Limit)	((_SEQ1 != _SEQ2) && !(((_SEQ1-_SEQ2) & ((_Limit+1)>>1))))
#define SEQ_WITHIN_WIN(_SEQ1, _SEQ2, _WIN, _Limit) (SEQ_LARGER(_SEQ1, _SEQ2, _Limit) &&  \
												SEQ_SMALLER(_SEQ1, ((_SEQ2+_WIN+1)&_Limit), _Limit))

/* */
/* Contention-free parameter (without ID and Length) */
/* */
typedef struct GNU_PACKED _CF_PARM{
    bool     bValid;         /* 1: variable contains valid value */
    unsigned char       CfpCount;
    unsigned char       CfpPeriod;
    unsigned short      CfpMaxDuration;
    unsigned short      CfpDurRemaining;
} CF_PARM, *PCF_PARM;

typedef	struct _CIPHER_SUITE {
	NDIS_802_11_ENCRYPTION_STATUS	PairCipher;		/* Unicast cipher 1, this one has more secured cipher suite */
	NDIS_802_11_ENCRYPTION_STATUS	PairCipherAux;	/* Unicast cipher 2 if AP announce two unicast cipher suite */
	NDIS_802_11_ENCRYPTION_STATUS	GroupCipher;	/* Group cipher */
	unsigned short							RsnCapability;	/* RSN capability from beacon */
	bool							bMixMode;		/* Indicate Pair & Group cipher might be different */
}	CIPHER_SUITE, *PCIPHER_SUITE;
	

/* EDCA configuration from AP's BEACON/ProbeRsp */
typedef struct {
    bool     bValid;         /* 1: variable contains valid value */
    bool     bAdd;         /* 1: variable contains valid value */
    bool     bQAck;
    bool     bQueueRequest;
    bool     bTxopRequest;
    bool     bAPSDCapable;
/*  bool     bMoreDataAck; */
    unsigned char       EdcaUpdateCount;
    unsigned char       Aifsn[4];       /* 0:AC_BK, 1:AC_BE, 2:AC_VI, 3:AC_VO */
    unsigned char       Cwmin[4];
    unsigned char       Cwmax[4];
    unsigned short      Txop[4];      /* in unit of 32-us */
    bool     bACM[4];      /* 1: Admission Control of AC_BK is mandattory */
} EDCA_PARM, *PEDCA_PARM;

/* QBSS LOAD information from QAP's BEACON/ProbeRsp */
typedef struct {
    bool     bValid;                     /* 1: variable contains valid value */
    unsigned short      StaNum;
    unsigned char       ChannelUtilization;
    unsigned short      RemainingAdmissionControl;  /* in unit of 32-us */
} QBSS_LOAD_PARM, *PQBSS_LOAD_PARM;

/* QBSS Info field in QSTA's assoc req */
typedef struct GNU_PACKED _QBSS_STA_INFO_PARM{
#ifdef RT_BIG_ENDIAN
	unsigned char		Rsv2:1;
	unsigned char		MaxSPLength:2;
	unsigned char		Rsv1:1;
	unsigned char		UAPSD_AC_BE:1;
	unsigned char		UAPSD_AC_BK:1;
	unsigned char		UAPSD_AC_VI:1;
	unsigned char		UAPSD_AC_VO:1;
#else
    unsigned char		UAPSD_AC_VO:1;
	unsigned char		UAPSD_AC_VI:1;
	unsigned char		UAPSD_AC_BK:1;
	unsigned char		UAPSD_AC_BE:1;
	unsigned char		Rsv1:1;
	unsigned char		MaxSPLength:2;
	unsigned char		Rsv2:1;
#endif /* RT_BIG_ENDIAN */
} QBSS_STA_INFO_PARM, *PQBSS_STA_INFO_PARM;

typedef struct {
	QBSS_STA_INFO_PARM	QosInfo;
	unsigned char	Rsv;
	unsigned char	Q_AC_BE[4];
	unsigned char	Q_AC_BK[4];
	unsigned char	Q_AC_VI[4];
	unsigned char	Q_AC_VO[4];
} QBSS_STA_EDCA_PARM, *PQBSS_STA_EDCA_PARM;

/* QBSS Info field in QAP's Beacon/ProbeRsp */
typedef struct GNU_PACKED _QBSS_AP_INFO_PARM{
#ifdef RT_BIG_ENDIAN
	unsigned char		UAPSD:1;
	unsigned char		Rsv:3;
    unsigned char		ParamSetCount:4;
#else
    unsigned char		ParamSetCount:4;
	unsigned char		Rsv:3;
	unsigned char		UAPSD:1;
#endif /* RT_BIG_ENDIAN */
} QBSS_AP_INFO_PARM, *PQBSS_AP_INFO_PARM;

/* QOS Capability reported in QAP's BEACON/ProbeRsp */
/* QOS Capability sent out in QSTA's AssociateReq/ReAssociateReq */
typedef struct {
    bool     bValid;                     /* 1: variable contains valid value */
    bool     bQAck;
    bool     bQueueRequest;
    bool     bTxopRequest;
/*  bool     bMoreDataAck; */
    unsigned char       EdcaUpdateCount;
} QOS_CAPABILITY_PARM, *PQOS_CAPABILITY_PARM;



typedef struct {
    unsigned char   Bssid[MAC_ADDR_LEN];
    unsigned char   Channel;
	unsigned char   CentralChannel;	/*Store the wide-band central channel for 40MHz.  .used in 40MHz AP. Or this is the same as Channel. */
    unsigned char   BssType;
    unsigned short  AtimWin;
    unsigned short  BeaconPeriod;

    unsigned char   SupRate[MAX_LEN_OF_SUPPORTED_RATES];
    unsigned char   SupRateLen;
    unsigned char   ExtRate[MAX_LEN_OF_SUPPORTED_RATES];
    unsigned char   ExtRateLen;
	HT_CAPABILITY_IE HtCapability;
	unsigned char			HtCapabilityLen;
	ADD_HT_INFO_IE AddHtInfo;	/* AP might use this additional ht info IE */
	unsigned char			AddHtInfoLen;
	EXT_CAP_INFO_ELEMENT ExtCapInfo;	/* this is the extened capibility IE appreed in MGMT frames. Doesn't need to update once set in Init. */
	unsigned char			NewExtChanOffset;
	CHAR    Rssi;

#ifdef DOT11_VHT_AC
	unsigned char vht_cap_len;
	unsigned char vht_op_len;
	VHT_CAP_IE vht_cap_ie;
	VHT_OP_IE vht_op_ie;
#endif /* DOT11_VHT_AC */


	CHAR	MinSNR;	
    unsigned char   Privacy;			/* Indicate security function ON/OFF. Don't mess up with auth mode. */
	unsigned char	Hidden;

    unsigned short  DtimPeriod;
    unsigned short  CapabilityInfo;

    unsigned short  CfpCount;
    unsigned short  CfpPeriod;
    unsigned short  CfpMaxDuration;
    unsigned short  CfpDurRemaining;
    unsigned char   SsidLen;
    CHAR    Ssid[MAX_LEN_OF_SSID];
    
	unsigned char	SameRxTimeCount;
	unsigned long   LastBeaconRxTimeA; /* OS's timestamp */
    unsigned long   LastBeaconRxTime; /* OS's timestamp */

	bool	bSES;

	/* New for WPA2 */
	CIPHER_SUITE					WPA;			/* AP announced WPA cipher suite */
	CIPHER_SUITE					WPA2;			/* AP announced WPA2 cipher suite */
#ifdef WAPI_SUPPORT
	CIPHER_SUITE					WAPI;			/* AP announced WAPI cipher suite */
#endif /* WAPI_SUPPORT */

	/* New for microsoft WPA support */
	NDIS_802_11_FIXED_IEs	FixIEs;
	NDIS_802_11_AUTHENTICATION_MODE	AuthModeAux;	/* Addition mode for WPA2 / WPA capable AP */
	NDIS_802_11_AUTHENTICATION_MODE	AuthMode;	
	NDIS_802_11_WEP_STATUS	WepStatus;				/* Unicast Encryption Algorithm extract from VAR_IE */
	unsigned short					VarIELen;				/* Length of next VIE include EID & Length */
	unsigned char					VarIEs[MAX_VIE_LEN];
	unsigned short					VarIeFromProbeRspLen;
	unsigned char					*pVarIeFromProbRsp;

	/* CCX Ckip information */
    unsigned char   CkipFlag;

	/* CCX 2 TSF */
	unsigned char	PTSF[4];		/* Parent TSF */
	unsigned char	TTSF[8];		/* Target TSF */

    /* 802.11e d9, and WMM */
	EDCA_PARM           EdcaParm;
	QOS_CAPABILITY_PARM QosCapability;
	QBSS_LOAD_PARM      QbssLoad;

#ifdef WSC_INCLUDED
    unsigned char		WpsAP;		/* 0x00: not support WPS, 0x01: support normal WPS, 0x02: support Ralink auto WPS, 0x04: support WAC AP */
	unsigned short		WscDPIDFromWpsAP;
#endif /* WSC_INCLUDED */



	unsigned char   MacAddr[MAC_ADDR_LEN];
	unsigned long ClientStatusFlags;
} BSS_ENTRY, *PBSS_ENTRY;

typedef struct {
    unsigned char           BssNr;
    unsigned char           BssOverlapNr;
    BSS_ENTRY       BssEntry[MAX_LEN_OF_BSS_TABLE];
} BSS_TABLE, *PBSS_TABLE;


typedef struct _MLME_QUEUE_ELEM {
	unsigned char             Msg[MGMT_DMA_BUFFER_SIZE];	/* move here to fix alignment issue for ARM CPU */
    unsigned long             Machine;
    unsigned long             MsgType;
    unsigned long             MsgLen;
    LARGE_INTEGER     TimeStamp;
    unsigned char             Rssi0;
    unsigned char             Rssi1;
    unsigned char             Rssi2;
    unsigned char             AntSel;
    unsigned char             Signal;
    unsigned char             Channel;
    unsigned char             Wcid;
    bool           Occupied;
	unsigned char			  OpMode;
	unsigned long             Priv;
} MLME_QUEUE_ELEM, *PMLME_QUEUE_ELEM;

typedef struct _MLME_QUEUE {
    unsigned long             Num;
    unsigned long             Head;
    unsigned long             Tail;
    NDIS_SPIN_LOCK   Lock;
    MLME_QUEUE_ELEM  Entry[MAX_LEN_OF_MLME_QUEUE];
} MLME_QUEUE, *PMLME_QUEUE;

typedef VOID (*STATE_MACHINE_FUNC)(VOID *pAd, MLME_QUEUE_ELEM *Elem);

typedef struct _STATE_MACHINE {
    unsigned long				Base;
    unsigned long				NrState;
    unsigned long				NrMsg;
    unsigned long				CurrState;
    STATE_MACHINE_FUNC	*TransFunc;
} STATE_MACHINE, *PSTATE_MACHINE;

#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT
typedef VOID (*APCLI_STATE_MACHINE_FUNC)(VOID *pAd, MLME_QUEUE_ELEM *Elem, unsigned long * pCurrState, unsigned short ifIndex);

typedef struct _STA_STATE_MACHINE {
	unsigned long                           Base;
	unsigned long                           NrState;
	unsigned long                           NrMsg;
	unsigned long                           CurrState;
	APCLI_STATE_MACHINE_FUNC          *TransFunc;
} APCLI_STATE_MACHINE, *PSTA_STATE_MACHINE;
#endif /* APCLI_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

/* MLME AUX data structure that hold temporarliy settings during a connection attempt. */
/* Once this attemp succeeds, all settings will be copy to pAd->StaActive. */
/* A connection attempt (user set OID, roaming, CCX fast roaming,..) consists of */
/* several steps (JOIN, AUTH, ASSOC or REASSOC) and may fail at any step. We purposely */
/* separate this under-trial settings away from pAd->StaActive so that once */
/* this new attempt failed, driver can auto-recover back to the active settings. */
typedef struct _MLME_AUX {
	unsigned char               BssType;
	unsigned char               Ssid[MAX_LEN_OF_SSID];
	unsigned char               SsidLen;
	unsigned char               Bssid[MAC_ADDR_LEN];
	unsigned char			AutoReconnectSsid[MAX_LEN_OF_SSID];
	unsigned char			AutoReconnectSsidLen;
	unsigned short			Alg;
	unsigned char			ScanType;
	unsigned char			Channel;
	unsigned char               CentralChannel;
	unsigned short              Aid;
	unsigned short              CapabilityInfo;
	unsigned short              BeaconPeriod;
	unsigned short              CfpMaxDuration;
	unsigned short              CfpPeriod;
	unsigned short              AtimWin;
    
	/* Copy supported rate from desired AP's beacon. We are trying to match */
	/* AP's supported and extended rate settings. */
	unsigned char		        SupRate[MAX_LEN_OF_SUPPORTED_RATES];
	unsigned char		        ExtRate[MAX_LEN_OF_SUPPORTED_RATES];
	unsigned char		        SupRateLen;
	unsigned char		        ExtRateLen;
	HT_CAPABILITY_IE		HtCapability;
	unsigned char		        	HtCapabilityLen;
	ADD_HT_INFO_IE		AddHtInfo;	/* AP might use this additional ht info IE */
	EXT_CAP_INFO_ELEMENT ExtCapInfo; /* this is the extened capibility IE appreed in MGMT frames. Doesn't need to update once set in Init. */
	unsigned char			NewExtChannelOffset;
	/*RT_HT_CAPABILITY	SupportedHtPhy; */

#ifdef DOT11_VHT_AC
	unsigned char vht_cap_len;
	unsigned char vht_op_len;
	VHT_CAP_IE vht_cap;
	VHT_OP_IE vht_op;
#endif /* DOT11_VHT_AC */

    /* new for QOS */
    QOS_CAPABILITY_PARM APQosCapability;    /* QOS capability of the current associated AP */
    EDCA_PARM           APEdcaParm;         /* EDCA parameters of the current associated AP */
    QBSS_LOAD_PARM      APQbssLoad;         /* QBSS load of the current associated AP */

    /* new to keep Ralink specific feature */
    unsigned long               APRalinkIe;
    
    BSS_TABLE           SsidBssTab;     /* AP list for the same SSID */
    BSS_TABLE           RoamTab;        /* AP list eligible for roaming */
    unsigned long               BssIdx;
    unsigned long               RoamIdx;

	bool				CurrReqIsFromNdis;

    RALINK_TIMER_STRUCT BeaconTimer, ScanTimer, APScanTimer;
    RALINK_TIMER_STRUCT AuthTimer;
    RALINK_TIMER_STRUCT AssocTimer, ReassocTimer, DisassocTimer;
#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT
	unsigned short              VarIELen;           /* Length of next VIE include EID & Length */
    unsigned char               VarIEs[MAX_VIE_LEN];
    LONG				Rssi; /* Record the rssi value when receive Probe Rsp. */
	RALINK_TIMER_STRUCT ProbeTimer, ApCliAssocTimer, ApCliAuthTimer;
#endif /* APCLI_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */


} MLME_AUX, *PMLME_AUX;

typedef struct _MLME_ADDBA_REQ_STRUCT{
	unsigned char   Wcid;	/* */
	unsigned char   pAddr[MAC_ADDR_LEN];
	unsigned char   BaBufSize;
	unsigned short	TimeOutValue;
	unsigned char   TID;
	unsigned char   Token;
	unsigned short	BaStartSeq;
} MLME_ADDBA_REQ_STRUCT, *PMLME_ADDBA_REQ_STRUCT;


typedef struct _MLME_DELBA_REQ_STRUCT{
	unsigned char   Wcid;	/* */
	unsigned char     Addr[MAC_ADDR_LEN];
	unsigned char   TID;
	unsigned char	Initiator;
} MLME_DELBA_REQ_STRUCT, *PMLME_DELBA_REQ_STRUCT;

/* assoc struct is equal to reassoc */
typedef struct _MLME_ASSOC_REQ_STRUCT{
    unsigned char     Addr[MAC_ADDR_LEN];
    unsigned short    CapabilityInfo;
    unsigned short    ListenIntv;
    unsigned long     Timeout;
} MLME_ASSOC_REQ_STRUCT, *PMLME_ASSOC_REQ_STRUCT, MLME_REASSOC_REQ_STRUCT, *PMLME_REASSOC_REQ_STRUCT;

typedef struct _MLME_DISASSOC_REQ_STRUCT{
    unsigned char     Addr[MAC_ADDR_LEN];
    unsigned short    Reason;
} MLME_DISASSOC_REQ_STRUCT, *PMLME_DISASSOC_REQ_STRUCT;

typedef struct _MLME_AUTH_REQ_STRUCT {
    unsigned char        Addr[MAC_ADDR_LEN];
    unsigned short       Alg;
    unsigned long        Timeout;
} MLME_AUTH_REQ_STRUCT, *PMLME_AUTH_REQ_STRUCT;

typedef struct _MLME_DEAUTH_REQ_STRUCT {
    unsigned char        Addr[MAC_ADDR_LEN];
    unsigned short       Reason;
} MLME_DEAUTH_REQ_STRUCT, *PMLME_DEAUTH_REQ_STRUCT;

typedef struct {
    unsigned long      BssIdx;
} MLME_JOIN_REQ_STRUCT;

typedef struct _MLME_SCAN_REQ_STRUCT {
    unsigned char      Bssid[MAC_ADDR_LEN];
    unsigned char      BssType;
    unsigned char      ScanType;
    unsigned char      SsidLen;
    CHAR       Ssid[MAX_LEN_OF_SSID];
} MLME_SCAN_REQ_STRUCT, *PMLME_SCAN_REQ_STRUCT;

typedef struct _MLME_START_REQ_STRUCT {
    CHAR        Ssid[MAX_LEN_OF_SSID];
    unsigned char       SsidLen;
} MLME_START_REQ_STRUCT, *PMLME_START_REQ_STRUCT;


typedef struct GNU_PACKED _EID_STRUCT{
    unsigned char   Eid;
    unsigned char   Len;
    unsigned char   Octet[1];
} EID_STRUCT,*PEID_STRUCT, BEACON_EID_STRUCT, *PBEACON_EID_STRUCT;


/* ========================== AP mlme.h =============================== */
#define TBTT_PRELOAD_TIME       384        /* usec. LomgPreamble + 24-byte at 1Mbps */
#define DEFAULT_DTIM_PERIOD     1

/* weighting factor to calculate Channel quality, total should be 100% */
/*#define RSSI_WEIGHTING                   0 */
/*#define TX_WEIGHTING                     40 */
/*#define RX_WEIGHTING                     60 */

#define MAC_TABLE_AGEOUT_TIME			300			/* unit: sec */
#define MAC_TABLE_MIN_AGEOUT_TIME		60			/* unit: sec */
#define MAC_TABLE_ASSOC_TIMEOUT			5			/* unit: sec */
#define MAC_TABLE_FULL(Tab)				((Tab).size == MAX_LEN_OF_MAC_TABLE)

/* AP shall drop the sta if contine Tx fail count reach it. */
#define MAC_ENTRY_LIFE_CHECK_CNT		1024			/* packet cnt. */

/* Value domain of pMacEntry->Sst */
typedef enum _Sst {
    SST_NOT_AUTH,   /* 0: equivalent to IEEE 802.11/1999 state 1 */
    SST_AUTH,       /* 1: equivalent to IEEE 802.11/1999 state 2 */
    SST_ASSOC       /* 2: equivalent to IEEE 802.11/1999 state 3 */
} SST;

/* value domain of pMacEntry->AuthState */
typedef enum _AuthState {
    AS_NOT_AUTH,
    AS_AUTH_OPEN,       /* STA has been authenticated using OPEN SYSTEM */
    AS_AUTH_KEY,        /* STA has been authenticated using SHARED KEY */
    AS_AUTHENTICATING   /* STA is waiting for AUTH seq#3 using SHARED KEY */
} AUTH_STATE;


typedef struct _IE_lists {
	unsigned char Addr2[MAC_ADDR_LEN];
	unsigned char ApAddr[MAC_ADDR_LEN];
	unsigned short CapabilityInfo;
	unsigned short ListenInterval;
	unsigned char SsidLen;
	unsigned char Ssid[MAX_LEN_OF_SSID];
	unsigned char SupportedRatesLen;
	unsigned char SupportedRates[MAX_LEN_OF_SUPPORTED_RATES];
	unsigned char RSN_IE[MAX_LEN_OF_RSNIE];
	unsigned char RSNIE_Len;
	bool bWmmCapable;
#ifdef WSC_AP_SUPPORT
	bool bWscCapable;
#endif /* WSC_AP_SUPPORT */
	unsigned long RalinkIe;
	EXT_CAP_INFO_ELEMENT ExtCapInfo;
	unsigned char ht_cap_len;
	HT_CAPABILITY_IE HTCapability;
#ifdef DOT11_VHT_AC
	VHT_CAP_IE vht_cap;
	VHT_OP_IE vht_op;
	unsigned char vht_cap_len;
	unsigned char vht_op_len;
#endif /* DOT11_VHT_AC */
}IE_LISTS;


typedef struct _bcn_ie_list {
	unsigned char Addr2[MAC_ADDR_LEN]; 
	unsigned char Bssid[MAC_ADDR_LEN];
	CHAR Ssid[MAX_LEN_OF_SSID];
	unsigned char SsidLen;
	unsigned char BssType;
	unsigned short BeaconPeriod;
	unsigned char Channel;
	unsigned char NewChannel;
	unsigned short AtimWin;
	unsigned short CapabilityInfo;
	unsigned char Erp;
	unsigned char DtimCount;
	unsigned char DtimPeriod;
	unsigned char BcastFlag;
	unsigned char MessageToMe;
	unsigned char SupRate[MAX_LEN_OF_SUPPORTED_RATES];
	unsigned char SupRateLen;
	unsigned char ExtRate[MAX_LEN_OF_SUPPORTED_RATES];
	unsigned char ExtRateLen;
	unsigned char CkipFlag;
	unsigned char AironetCellPowerLimit;
	LARGE_INTEGER TimeStamp;
	CF_PARM CfParm;
	EDCA_PARM EdcaParm;
	QBSS_LOAD_PARM QbssLoad;
	QOS_CAPABILITY_PARM QosCapability;
	unsigned long RalinkIe;
	EXT_CAP_INFO_ELEMENT ExtCapInfo;
	unsigned char HtCapabilityLen;
	unsigned char PreNHtCapabilityLen;
	HT_CAPABILITY_IE HtCapability;
	unsigned char AddHtInfoLen;
	ADD_HT_INFO_IE AddHtInfo;
	unsigned char NewExtChannelOffset;
#ifdef DOT11_VHT_AC
	VHT_CAP_IE vht_cap_ie;
	VHT_OP_IE vht_op_ie;
	unsigned char vht_cap_len;
	unsigned char vht_op_len;
#endif /* DOT11_VHT_AC */
}BCN_IE_LIST;

#endif	/* MLME_H__ */

