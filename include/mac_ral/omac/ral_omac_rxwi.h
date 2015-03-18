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
	ral_omac_rxwi.h

	Abstract:
	Ralink Wireless Chip MAC related definition & structures

	Revision History:
	Who			When		  What
	--------	----------	  ----------------------------------------------
*/

#ifndef __RAL_OMAC_RXWI_H__
#define __RAL_OMAC_RXWI_H__

#include "rtmp_type.h"

/*
	RXWI wireless information format, in PBF. invisible in driver.
*/
#ifdef RT_BIG_ENDIAN
typedef	struct GNU_PACKED _RXWI_OMAC{
	/* Word 0 */
	UINT32		tid:4;
	UINT32		MPDUtotalByteCnt:12;
	UINT32		UDF:3;
	UINT32		bss_idx:3;
	UINT32		key_idx:2;
	UINT32		wcid:8;
	
	/* Word 1 */
	UINT32		phy_mode:2;              /* 1: this RX frame is unicast to me */
	UINT32		iTxBF:1; /* iTxBF enable */
	UINT32		Sounding:1; /* Sounding enable */
	UINT32		eTxBF:1; /* eTxBF enable */
	UINT32		stbc:2;
	UINT32		sgi:1;
	UINT32		bw:1;
	UINT32		mcs:7;
	UINT32		SEQUENCE:12;
	UINT32		FRAG:4;
	
	/* Word 2 */
	UINT32		rsv1:8;
	UINT32		RSSI2:8;
	UINT32		RSSI1:8;
	UINT32		RSSI0:8;
	
	/* Word 3 */
	UINT32		FOFFSET:8;
	UINT32		SNR2:8;
	UINT32		SNR1:8;
	UINT32		SNR0:8;
	
	UINT32		rsv3;

#if defined(RT5592) || defined(MT7601)
	/* Word 5 */
	/* For Exper Antenna */
	UINT32		rsv4:24;
	UINT32		EANT_ID:8;
#endif /* RT5592 */
}	RXWI_OMAC, *PRXWI_OMAC;
#else
typedef	struct GNU_PACKED _RXWI_OMAC{
	/* Word	0 */
	UINT32		wcid:8;
	UINT32		key_idx:2;
	UINT32		bss_idx:3;
	UINT32		UDF:3;
	UINT32		MPDUtotalByteCnt:12;
	UINT32		tid:4;

	/* Word	1 */
	UINT32		FRAG:4;
	UINT32		SEQUENCE:12;
	UINT32		mcs:7;
	UINT32		bw:1;
	UINT32		sgi:1;
	UINT32		stbc:2;
	UINT32		eTxBF:1; /* eTxBF enable */
	UINT32		Sounding:1; /* Sounding enable */
	UINT32		iTxBF:1; /* iTxBF enable */
	UINT32		phy_mode:2;              /* 1: this RX frame is unicast to me */

	/*Word2 */
	UINT32		RSSI0:8;
	UINT32		RSSI1:8;
	UINT32		RSSI2:8;
	UINT32		rsv1:8;

	/*Word3 */
	UINT32		SNR0:8;
	UINT32		SNR1:8;
	UINT32		SNR2:8;
	UINT32		FOFFSET:8;

	UINT32		rsv3;

#if defined(RT5592) || defined(MT7601)
	/* Word 5 */
	/* For Exper Antenna */
	UINT32      EANT_ID:8;
	UINT32      rsv4:24;
#endif /* RT5592 */
}	RXWI_OMAC, *PRXWI_OMAC;
#endif


#define RxWIMPDUByteCnt	RXWI_O.MPDUtotalByteCnt
#define RxWIWirelessCliID	RXWI_O.wcid
#define RxWIKeyIndex		RXWI_O.key_idx
#define RxWIMCS				RXWI_O.mcs
#define RxWIBW				RXWI_O.bw
#define RxWISGI				RXWI_O.sgi
#define RxWIBSSID			RXWI_O.bss_idx
#define RxWIPhyMode		RXWI_O.phy_mode
#define RxWISTBC			RXWI_O.stbc
#define RxWITID				RXWI_O.tid
#define RxWIRSSI0			RXWI_O.RSSI0
#define RxWIRSSI1			RXWI_O.RSSI1
#define RxWIRSSI2			RXWI_O.RSSI2
#define RxWISNR0			RXWI_O.SNR0
#define RxWISNR1			RXWI_O.SNR1
#define RxWISNR2			RXWI_O.SNR2
#define RxWIFOFFSET			RXWI_O.FOFFSET



#endif /* __RAL_OMAC_RXWI_H__ */

