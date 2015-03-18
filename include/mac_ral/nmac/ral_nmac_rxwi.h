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
	ral_nmac_rxwi.h

	Abstract:
	Ralink Wireless Chip MAC related definition & structures

	Revision History:
	Who			When		  What
	--------	----------	  ----------------------------------------------
*/

#ifndef __RAL_NMAC_RXWI_H__
#define __RAL_NMAC_RXWI_H__

#include "rtmp_type.h"

/*
	RXWI wireless information format.
*/
#ifdef RT_BIG_ENDIAN
typedef	struct GNU_PACKED _RXWI_NMAC{
	/* Word 0 */
	UINT32 eof:1;
	UINT32 rsv:1;
	UINT32 MPDUtotalByteCnt:14;
	UINT32 udf:3;
	UINT32 bss_idx:3;
	UINT32 key_idx:2;
	UINT32 wcid:8;

	/* Word 1 */
	UINT32 phy_mode:3;
	UINT32 i_txbf:1;
	UINT32 e_txbf:1;
	UINT32 stbc:1;
	UINT32 sgi:1;
	UINT32 bw:2;
	UINT32 mcs:7;
	UINT32 sn:12;
	UINT32 tid:4;

	/* Word 2 */
	UINT8 rssi[4];

	/* Word 3~6 */
	UINT8 bbp_rxinfo[16];
}	RXWI_NMAC;
#else
typedef	struct GNU_PACKED _RXWI_NMAC {
	/* Word 0 */
	UINT32 wcid:8;
	UINT32 key_idx:2;
	UINT32 bss_idx:3;
	UINT32 udf:3;
	UINT32 MPDUtotalByteCnt:14;
	UINT32 rsv:1;
	UINT32 eof:1;

	/* Word 1 */
	UINT32 tid:4;
	UINT32 sn:12;
	UINT32 mcs:7;
	UINT32 bw:2;
	UINT32 sgi:1;
	UINT32 stbc:1;
	UINT32 e_txbf:1;
	UINT32 i_txbf:1;
	UINT32 phy_mode:3;

	/* Word 2 */
	UINT8 rssi[4];

	/* Word 3~6 */
	UINT8 bbp_rxinfo[16];
}	RXWI_NMAC;
#endif /* RT_BIG_ENDIAN */


#define RxWIMPDUByteCnt	RXWI_N.MPDUtotalByteCnt
#define RxWIWirelessCliID	RXWI_N.wcid
#define RxWIKeyIndex		RXWI_N.key_idx
#define RxWIMCS				RXWI_N.mcs
#define RxWIBW				RXWI_N.bw
#define RxWIBSSID			RXWI_N.bss_idx
#define RxWISGI				RXWI_N.sgi
#define RxWIPhyMode		RXWI_N.phy_mode
#define RxWISTBC			RXWI_N.stbc
#define RxWITID				RXWI_N.tid
#define RxWIRSSI0			RXWI_N.rssi[0]
#define RxWIRSSI1			RXWI_N.rssi[1]
#define RxWIRSSI2			RXWI_N.rssi[2]
#define RxWISNR0			RXWI_N.bbp_rxinfo[0]
#define RxWISNR1			RXWI_N.bbp_rxinfo[1]
#define RxWISNR2			RXWI_N.bbp_rxinfo[2]
#define RxWIFOFFSET			RXWI_N.bbp_rxinfo[3]



#endif /* __RAL_NMAC_RXWI_H__ */

