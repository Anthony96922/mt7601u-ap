/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering	the source code	is stricitly prohibited, unless	the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	ral_nmac_txwi.h

	Abstract:
	Ralink Wireless Chip MAC related definition & structures

	Revision History:
	Who			When		  What
	--------	----------	  ----------------------------------------------
*/

#ifndef __RAL_NMAC_TXWI_H__
#define __RAL_NMAC_TXWI_H__

#include "rtmp_type.h"



#ifdef RT65xx
#ifdef RT_BIG_ENDIAN
typedef struct GNU_PACKED _TXWI_NMAC {
	/* Word 0 */
	UINT32		PHYMODE:3;
	UINT32		iTxBF:1;
	UINT32		eTxBF:1;
	UINT32		STBC:1;
	UINT32		ShortGI:1;
	UINT32		BW:2;			/* channel bandwidth 20/40/80 MHz */
	UINT32		MCS:7;
	
	UINT32		rsv0:2;
	UINT32		Sounding:1;
	UINT32		NDPSndBW:1;	/* NDP sounding BW */
	UINT32		NDPSndRate:2;	/* 0 : MCS0, 1: MCS8, 2: MCS16, 3: reserved */
	UINT32		txop:2;

	UINT32		MpduDensity:3;
	UINT32		AMPDU:1;
	UINT32		TS:1;
	UINT32		CFACK:1;
	UINT32		MIMOps:1;	/* the remote peer is in dynamic MIMO-PS mode */
	UINT32		FRAG:1;		/* 1 to inform TKIP engine this is a fragment. */
	
	/* Word 1 */
	UINT32		Rsv1:2;
	UINT32		MPDUtotalByteCnt:14;
	UINT32		wcid:8;
	UINT32		BAWinSize:6;
	UINT32		NSEQ:1;
	UINT32		ACK:1;

	/* Word 2 */
	UINT32		IV;
	/* Word 3 */
	UINT32		EIV;

	/* Word 4 */
	UINT32		TxPktId:8;
	UINT32		Rsv4:4;
	UINT32		TxPwrAdj:4;
	UINT32		TxStreamMode:8;
	UINT32		TxEAPId:8;
}	TXWI_NMAC, *PTXWI_NMAC;
#else
typedef	struct GNU_PACKED _TXWI_NMAC {
	/* Word	0 */
	/* ex: 00 03 00 40 means txop = 3, PHYMODE = 1 */
	UINT32		FRAG:1;		/* 1 to inform TKIP engine this is a fragment. */
	UINT32		MIMOps:1;	/* the remote peer is in dynamic MIMO-PS mode */
	UINT32		CFACK:1;
	UINT32		TS:1;
	UINT32		AMPDU:1;
	UINT32		MpduDensity:3;

	UINT32		txop:2;
	UINT32		NDPSndRate:2; /* 0 : MCS0, 1: MCS8, 2: MCS16, 3: reserved */
	UINT32		NDPSndBW:1; /* NDP sounding BW */
	UINT32		Sounding:1;
	UINT32		rsv0:2;
	
	UINT32		MCS:7;
	UINT32		BW:2;		/*channel bandwidth 20/40/80 MHz */
	UINT32		ShortGI:1;
	UINT32		STBC:1;
	UINT32		eTxBF:1;
	UINT32		iTxBF:1;
	UINT32		PHYMODE:3;  

	/* Word1 */
	/* ex:  1c ff 38 00 means ACK=0, BAWinSize=7, MPDUtotalByteCnt = 0x38 */
	UINT32		ACK:1;
	UINT32		NSEQ:1;
	UINT32		BAWinSize:6;
	UINT32		wcid:8;
	UINT32		MPDUtotalByteCnt:14;
	UINT32		Rsv1:2;
	
	/*Word2 */
	UINT32		IV;
	
	/*Word3 */
	UINT32		EIV;

	/* Word 4 */
	UINT32		TxEAPId:8;
	UINT32		TxStreamMode:8;
	UINT32		TxPwrAdj:4;
	UINT32		Rsv4:4;	
	UINT32		TxPktId:8;
}	TXWI_NMAC, *PTXWI_NMAC;
#endif /* RT_BIG_ENDIAN */
#else
#ifdef RT_BIG_ENDIAN
typedef struct GNU_PACKED _TXWI_NMAC {
	/* Word 0 */
	UINT32		PHYMODE:2;
	UINT32		Rsv1:3;	
	UINT32		STBC:2;
	UINT32		ShortGI:1;
	UINT32		BW:1;
	UINT32		MCS:7;
	
	UINT32		TXLUT:1;
	UINT32		TXRPT:1;
	UINT32		Autofallback:1;	/* TX rate auto fallback disable */
	UINT32		CWMIN:3;
	UINT32		txop:2;

	UINT32		MpduDensity:3;
	UINT32		AMPDU:1;
	UINT32		TS:1;
	UINT32		CFACK:1;
	UINT32		MIMOps:1;	/* the remote peer is in dynamic MIMO-PS mode */
	UINT32		FRAG:1;		/* 1 to inform TKIP engine this is a fragment. */
	
	/* Word 1 */
	UINT32		TxPktId:4;
	UINT32		MPDUtotalByteCnt:12;
	UINT32		wcid:8;
	UINT32		BAWinSize:6;
	UINT32		NSEQ:1;
	UINT32		ACK:1;

	/* Word 2 */
	UINT32		IV;
	/* Word 3 */
	UINT32		EIV;

	/* Word 4 */
	UINT32		Rsv3:9;
	UINT32		PIFS_REV:1;
	UINT32		Rsv2:1;
	UINT32		CCP:1;		/* Channel Check Packet */
	UINT32		TxPwrAdj:4;
	UINT32		TxStreamMode:8;
	UINT32		TxEAPId:8;
}	TXWI_NMAC, *PTXWI_NMAC;
#else
typedef	struct GNU_PACKED _TXWI_NMAC {
	/* Word	0 */
	/* ex: 00 03 00 40 means txop = 3, PHYMODE = 1 */
	UINT32		FRAG:1;		/* 1 to inform TKIP engine this is a fragment. */
	UINT32		MIMOps:1;	/* the remote peer is in dynamic MIMO-PS mode */
	UINT32		CFACK:1;
	UINT32		TS:1;
	UINT32		AMPDU:1;
	UINT32		MpduDensity:3;

	UINT32		txop:2;
	UINT32		CWMIN:3;
	UINT32		Autofallback:1;	/* TX rate auto fallback disable */
	UINT32		TXRPT:1;
	UINT32		TXLUT:1;

	UINT32		MCS:7;
	UINT32		BW:1;
	UINT32		ShortGI:1;
	UINT32		STBC:2;
	UINT32		Rsv1:3;	
	UINT32		PHYMODE:2;  

	/* Word1 */
	/* ex:  1c ff 38 00 means ACK=0, BAWinSize=7, MPDUtotalByteCnt = 0x38 */
	UINT32		ACK:1;
	UINT32		NSEQ:1;
	UINT32		BAWinSize:6;
	UINT32		wcid:8;
	UINT32		MPDUtotalByteCnt:12;
	UINT32		TxPktId:4;
	
	/*Word2 */
	UINT32		IV;
	
	/*Word3 */
	UINT32		EIV;

	/* Word 4 */
	UINT32		TxEAPId:8;
	UINT32		TxStreamMode:8;
	UINT32		TxPwrAdj:4;
	UINT32		CCP:1;		/* Channel Check Packet */
	UINT32		Rsv2:1;
	UINT32		PIFS_REV:1;
	UINT32		Rsv3:9;
}	TXWI_NMAC, *PTXWI_NMAC;
#endif /* RT_BIG_ENDIAN */
#endif /* RT65xx */

#define TxWIMPDUByteCnt	TXWI_N.MPDUtotalByteCnt
#define TxWIWirelessCliID	TXWI_N.wcid
#define TxWIFRAG			TXWI_N.FRAG
#define TxWICFACK			TXWI_N.CFACK
#define TxWITS				TXWI_N.TS
#define TxWIAMPDU			TXWI_N.AMPDU
#define TxWIACK				TXWI_N.ACK
#define TxWITXOP			TXWI_N.txop
#define TxWINSEQ			TXWI_N.NSEQ
#define TxWIBAWinSize		TXWI_N.BAWinSize
#define TxWIShortGI			TXWI_N.ShortGI
#define TxWISTBC			TXWI_N.STBC
#define TxWIPacketId		TXWI_N.TxPktId
#define TxWIBW				TXWI_N.BW
#define TxWIMCS				TXWI_N.MCS
#define TxWIPHYMODE		TXWI_N.PHYMODE
#define TxWIMIMOps			TXWI_N.MIMOps
#define TxWIMpduDensity		TXWI_N.MpduDensity
#define TxWITXRPT			TXWI_N.TXRPT
#define TxWITXLUT			TXWI_N.TXLUT


#endif /* __RAL_NMAC_TXWI_H__ */

