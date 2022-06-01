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
	unsigned int		PHYMODE:3;
	unsigned int		iTxBF:1;
	unsigned int		eTxBF:1;
	unsigned int		STBC:1;
	unsigned int		ShortGI:1;
	unsigned int		BW:2;			/* channel bandwidth 20/40/80 MHz */
	unsigned int		MCS:7;
	
	unsigned int		rsv0:2;
	unsigned int		Sounding:1;
	unsigned int		NDPSndBW:1;	/* NDP sounding BW */
	unsigned int		NDPSndRate:2;	/* 0 : MCS0, 1: MCS8, 2: MCS16, 3: reserved */
	unsigned int		txop:2;

	unsigned int		MpduDensity:3;
	unsigned int		AMPDU:1;
	unsigned int		TS:1;
	unsigned int		CFACK:1;
	unsigned int		MIMOps:1;	/* the remote peer is in dynamic MIMO-PS mode */
	unsigned int		FRAG:1;		/* 1 to inform TKIP engine this is a fragment. */
	
	/* Word 1 */
	unsigned int		Rsv1:2;
	unsigned int		MPDUtotalByteCnt:14;
	unsigned int		wcid:8;
	unsigned int		BAWinSize:6;
	unsigned int		NSEQ:1;
	unsigned int		ACK:1;

	/* Word 2 */
	unsigned int		IV;
	/* Word 3 */
	unsigned int		EIV;

	/* Word 4 */
	unsigned int		TxPktId:8;
	unsigned int		Rsv4:4;
	unsigned int		TxPwrAdj:4;
	unsigned int		TxStreamMode:8;
	unsigned int		TxEAPId:8;
}	TXWI_NMAC, *PTXWI_NMAC;
#else
typedef	struct GNU_PACKED _TXWI_NMAC {
	/* Word	0 */
	/* ex: 00 03 00 40 means txop = 3, PHYMODE = 1 */
	unsigned int		FRAG:1;		/* 1 to inform TKIP engine this is a fragment. */
	unsigned int		MIMOps:1;	/* the remote peer is in dynamic MIMO-PS mode */
	unsigned int		CFACK:1;
	unsigned int		TS:1;
	unsigned int		AMPDU:1;
	unsigned int		MpduDensity:3;

	unsigned int		txop:2;
	unsigned int		NDPSndRate:2; /* 0 : MCS0, 1: MCS8, 2: MCS16, 3: reserved */
	unsigned int		NDPSndBW:1; /* NDP sounding BW */
	unsigned int		Sounding:1;
	unsigned int		rsv0:2;
	
	unsigned int		MCS:7;
	unsigned int		BW:2;		/*channel bandwidth 20/40/80 MHz */
	unsigned int		ShortGI:1;
	unsigned int		STBC:1;
	unsigned int		eTxBF:1;
	unsigned int		iTxBF:1;
	unsigned int		PHYMODE:3;  

	/* Word1 */
	/* ex:  1c ff 38 00 means ACK=0, BAWinSize=7, MPDUtotalByteCnt = 0x38 */
	unsigned int		ACK:1;
	unsigned int		NSEQ:1;
	unsigned int		BAWinSize:6;
	unsigned int		wcid:8;
	unsigned int		MPDUtotalByteCnt:14;
	unsigned int		Rsv1:2;
	
	/*Word2 */
	unsigned int		IV;
	
	/*Word3 */
	unsigned int		EIV;

	/* Word 4 */
	unsigned int		TxEAPId:8;
	unsigned int		TxStreamMode:8;
	unsigned int		TxPwrAdj:4;
	unsigned int		Rsv4:4;	
	unsigned int		TxPktId:8;
}	TXWI_NMAC, *PTXWI_NMAC;
#endif /* RT_BIG_ENDIAN */
#else
#ifdef RT_BIG_ENDIAN
typedef struct GNU_PACKED _TXWI_NMAC {
	/* Word 0 */
	unsigned int		PHYMODE:2;
	unsigned int		Rsv1:3;	
	unsigned int		STBC:2;
	unsigned int		ShortGI:1;
	unsigned int		BW:1;
	unsigned int		MCS:7;
	
	unsigned int		TXLUT:1;
	unsigned int		TXRPT:1;
	unsigned int		Autofallback:1;	/* TX rate auto fallback disable */
	unsigned int		CWMIN:3;
	unsigned int		txop:2;

	unsigned int		MpduDensity:3;
	unsigned int		AMPDU:1;
	unsigned int		TS:1;
	unsigned int		CFACK:1;
	unsigned int		MIMOps:1;	/* the remote peer is in dynamic MIMO-PS mode */
	unsigned int		FRAG:1;		/* 1 to inform TKIP engine this is a fragment. */
	
	/* Word 1 */
	unsigned int		TxPktId:4;
	unsigned int		MPDUtotalByteCnt:12;
	unsigned int		wcid:8;
	unsigned int		BAWinSize:6;
	unsigned int		NSEQ:1;
	unsigned int		ACK:1;

	/* Word 2 */
	unsigned int		IV;
	/* Word 3 */
	unsigned int		EIV;

	/* Word 4 */
	unsigned int		Rsv3:9;
	unsigned int		PIFS_REV:1;
	unsigned int		Rsv2:1;
	unsigned int		CCP:1;		/* Channel Check Packet */
	unsigned int		TxPwrAdj:4;
	unsigned int		TxStreamMode:8;
	unsigned int		TxEAPId:8;
}	TXWI_NMAC, *PTXWI_NMAC;
#else
typedef	struct GNU_PACKED _TXWI_NMAC {
	/* Word	0 */
	/* ex: 00 03 00 40 means txop = 3, PHYMODE = 1 */
	unsigned int		FRAG:1;		/* 1 to inform TKIP engine this is a fragment. */
	unsigned int		MIMOps:1;	/* the remote peer is in dynamic MIMO-PS mode */
	unsigned int		CFACK:1;
	unsigned int		TS:1;
	unsigned int		AMPDU:1;
	unsigned int		MpduDensity:3;

	unsigned int		txop:2;
	unsigned int		CWMIN:3;
	unsigned int		Autofallback:1;	/* TX rate auto fallback disable */
	unsigned int		TXRPT:1;
	unsigned int		TXLUT:1;

	unsigned int		MCS:7;
	unsigned int		BW:1;
	unsigned int		ShortGI:1;
	unsigned int		STBC:2;
	unsigned int		Rsv1:3;	
	unsigned int		PHYMODE:2;  

	/* Word1 */
	/* ex:  1c ff 38 00 means ACK=0, BAWinSize=7, MPDUtotalByteCnt = 0x38 */
	unsigned int		ACK:1;
	unsigned int		NSEQ:1;
	unsigned int		BAWinSize:6;
	unsigned int		wcid:8;
	unsigned int		MPDUtotalByteCnt:12;
	unsigned int		TxPktId:4;
	
	/*Word2 */
	unsigned int		IV;
	
	/*Word3 */
	unsigned int		EIV;

	/* Word 4 */
	unsigned int		TxEAPId:8;
	unsigned int		TxStreamMode:8;
	unsigned int		TxPwrAdj:4;
	unsigned int		CCP:1;		/* Channel Check Packet */
	unsigned int		Rsv2:1;
	unsigned int		PIFS_REV:1;
	unsigned int		Rsv3:9;
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

