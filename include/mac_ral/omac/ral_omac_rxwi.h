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
	unsigned int		tid:4;
	unsigned int		MPDUtotalByteCnt:12;
	unsigned int		UDF:3;
	unsigned int		bss_idx:3;
	unsigned int		key_idx:2;
	unsigned int		wcid:8;
	
	/* Word 1 */
	unsigned int		phy_mode:2;              /* 1: this RX frame is unicast to me */
	unsigned int		iTxBF:1; /* iTxBF enable */
	unsigned int		Sounding:1; /* Sounding enable */
	unsigned int		eTxBF:1; /* eTxBF enable */
	unsigned int		stbc:2;
	unsigned int		sgi:1;
	unsigned int		bw:1;
	unsigned int		mcs:7;
	unsigned int		SEQUENCE:12;
	unsigned int		FRAG:4;
	
	/* Word 2 */
	unsigned int		rsv1:8;
	unsigned int		RSSI2:8;
	unsigned int		RSSI1:8;
	unsigned int		RSSI0:8;
	
	/* Word 3 */
	unsigned int		FOFFSET:8;
	unsigned int		SNR2:8;
	unsigned int		SNR1:8;
	unsigned int		SNR0:8;
	
	unsigned int		rsv3;

#if defined(RT5592) || defined(MT7601)
	/* Word 5 */
	/* For Exper Antenna */
	unsigned int		rsv4:24;
	unsigned int		EANT_ID:8;
#endif /* RT5592 */
}	RXWI_OMAC, *PRXWI_OMAC;
#else
typedef	struct GNU_PACKED _RXWI_OMAC{
	/* Word	0 */
	unsigned int		wcid:8;
	unsigned int		key_idx:2;
	unsigned int		bss_idx:3;
	unsigned int		UDF:3;
	unsigned int		MPDUtotalByteCnt:12;
	unsigned int		tid:4;

	/* Word	1 */
	unsigned int		FRAG:4;
	unsigned int		SEQUENCE:12;
	unsigned int		mcs:7;
	unsigned int		bw:1;
	unsigned int		sgi:1;
	unsigned int		stbc:2;
	unsigned int		eTxBF:1; /* eTxBF enable */
	unsigned int		Sounding:1; /* Sounding enable */
	unsigned int		iTxBF:1; /* iTxBF enable */
	unsigned int		phy_mode:2;              /* 1: this RX frame is unicast to me */

	/*Word2 */
	unsigned int		RSSI0:8;
	unsigned int		RSSI1:8;
	unsigned int		RSSI2:8;
	unsigned int		rsv1:8;

	/*Word3 */
	unsigned int		SNR0:8;
	unsigned int		SNR1:8;
	unsigned int		SNR2:8;
	unsigned int		FOFFSET:8;

	unsigned int		rsv3;

#if defined(RT5592) || defined(MT7601)
	/* Word 5 */
	/* For Exper Antenna */
	unsigned int      EANT_ID:8;
	unsigned int      rsv4:24;
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

