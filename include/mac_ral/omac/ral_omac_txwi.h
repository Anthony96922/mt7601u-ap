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
	ral_omac_txwi.h

	Abstract:
	Ralink Wireless Chip MAC related definition & structures

	Revision History:
	Who			When		  What
	--------	----------	  ----------------------------------------------
*/

#ifndef __RAL_OMAC_TXWI_H__
#define __RAL_OMAC_TXWI_H__


#include "rtmp_type.h"


#ifdef RT_BIG_ENDIAN
typedef	struct GNU_PACKED _TXWI_OMAC {
	/* Word 0 */
	unsigned int		PHYMODE:2;
	unsigned int		iTxBF:1; /* iTxBF enable */
	unsigned int		Sounding:1; /* Sounding enable */
	unsigned int		eTxBF:1; /* eTxBF enable */
	unsigned int		STBC:2;	/*channel bandwidth 20MHz or 40 MHz */
	unsigned int		ShortGI:1;
	unsigned int		BW:1;	/*channel bandwidth 20MHz or 40 MHz */
	unsigned int		MCS:7;
	
	unsigned int		rsv:1;
	unsigned int		TXRPT:1;
	unsigned int		Autofallback:1; /* TX rate auto fallback disable */
	unsigned int		NDPSndBW:1; /* NDP sounding BW */
	unsigned int		NDPSndRate:2; /* 0 : MCS0, 1: MCS8, 2: MCS16, 3: reserved */
	unsigned int		txop:2;
	unsigned int		MpduDensity:3;
	unsigned int		AMPDU:1;
	
	unsigned int		TS:1;
	unsigned int		CFACK:1;
	unsigned int		MIMOps:1;	/* the remote peer is in dynamic MIMO-PS mode */
	unsigned int		FRAG:1;		/* 1 to inform TKIP engine this is a fragment. */
	/* Word 1 */
	unsigned int		PacketId:4;
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
	/* For Expert Antenna */
	unsigned int		Reserved:11;
	unsigned int		CCP:1;
	unsigned int		TxPwrAdj:4;
	unsigned int		TxStreamMode:8;
	unsigned int		EncodedAntID:8;
}	TXWI_OMAC, *PTXWI_OMAC;
#else
typedef	struct GNU_PACKED _TXWI_OMAC {
	/* Word	0 */
	/* ex: 00 03 00 40 means txop = 3, PHYMODE = 1 */
	unsigned int		FRAG:1;		/* 1 to inform TKIP engine this is a fragment. */
	unsigned int		MIMOps:1;	/* the remote peer is in dynamic MIMO-PS mode */
	unsigned int		CFACK:1;
	unsigned int		TS:1;
		
	unsigned int		AMPDU:1;
	unsigned int		MpduDensity:3;
	unsigned int		txop:2;	/*FOR "THIS" frame. 0:HT TXOP rule , 1:PIFS TX ,2:Backoff, 3:sifs only when previous frame exchange is successful. */
	unsigned int		NDPSndRate:2; /* 0 : MCS0, 1: MCS8, 2: MCS16, 3: reserved */
	unsigned int		NDPSndBW:1; /* NDP sounding BW */
	unsigned int		Autofallback:1; /* TX rate auto fallback disable */
	unsigned int		TXRPT:1;
	unsigned int		rsv:1;
	
	unsigned int		MCS:7;
	unsigned int		BW:1;	/*channel bandwidth 20MHz or 40 MHz */
	unsigned int		ShortGI:1;
	unsigned int		STBC:2;	/* 1: STBC support MCS =0-7,   2,3 : RESERVE */
	unsigned int		eTxBF:1; /* eTxBF enable */
	unsigned int		Sounding:1; /* Sounding enable */
	unsigned int		iTxBF:1; /* iTxBF enable */
	unsigned int		PHYMODE:2;  
	/* Word1 */
	/* ex:  1c ff 38 00 means ACK=0, BAWinSize=7, MPDUtotalByteCnt = 0x38 */
	unsigned int		ACK:1;
	unsigned int		NSEQ:1;
	unsigned int		BAWinSize:6;
	unsigned int		wcid:8;
	unsigned int		MPDUtotalByteCnt:12;
	unsigned int		PacketId:4;
	/*Word2 */
	unsigned int		IV;
	/*Word3 */
	unsigned int		EIV;

}	TXWI_OMAC, *PTXWI_OMAC;
#endif


#define TxWIMPDUByteCnt	TXWI_O.MPDUtotalByteCnt
#define TxWIWirelessCliID	TXWI_O.wcid
#define TxWIFRAG			TXWI_O.FRAG
#define TxWICFACK			TXWI_O.CFACK
#define TxWITS				TXWI_O.TS
#define TxWIAMPDU			TXWI_O.AMPDU
#define TxWIACK				TXWI_O.ACK
#define TxWITXOP			TXWI_O.txop
#define TxWINSEQ			TXWI_O.NSEQ
#define TxWIBAWinSize		TXWI_O.BAWinSize
#define TxWIShortGI			TXWI_O.ShortGI
#define TxWISTBC			TXWI_O.STBC
#define TxWIBW				TXWI_O.BW
#define TxWIMCS				TXWI_O.MCS
#define TxWIPHYMODE		TXWI_O.PHYMODE
#define TxWIMIMOps			TXWI_O.MIMOps
#define TxWIMpduDensity		TXWI_O.MpduDensity

#endif /* __RAL_OMAC_TXWI_H__ */

