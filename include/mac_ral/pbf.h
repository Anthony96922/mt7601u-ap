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
	pbf.h

	Abstract:
	Ralink Wireless Chip MAC related definition & structures

	Revision History:
	Who			When		  What
	--------	----------	  ----------------------------------------------
*/

#ifndef __PBF_H__
#define __PBF_H__


#ifdef RLT_MAC
#include "mac_ral/nmac/ral_nmac_pbf.h"
#endif /* RLT_MAC */

#ifdef RTMP_MAC
#include "mac_ral/omac/ral_omac_pbf.h"
#endif /* RTMP_MAC */


/* ================================================================================= */
/* Register format  for PBF                                                                                                                                                     */
/* ================================================================================= */


#define WPDMA_GLO_CFG 	0x208
#ifdef RT_BIG_ENDIAN
typedef	union _WPDMA_GLO_CFG_STRUC	{
	struct	{
		unsigned int rx_2b_offset:1;
		unsigned int clk_gate_dis:1;
		unsigned int rsv:14;
		unsigned int HDR_SEG_LEN:8;
		unsigned int BigEndian:1;
		unsigned int EnTXWriteBackDDONE:1;
		unsigned int WPDMABurstSIZE:2;
		unsigned int RxDMABusy:1;
		unsigned int EnableRxDMA:1;
		unsigned int TxDMABusy:1;
		unsigned int EnableTxDMA:1;
	}	field;
	unsigned int word;
}WPDMA_GLO_CFG_STRUC, *PWPDMA_GLO_CFG_STRUC;
#else
typedef	union _WPDMA_GLO_CFG_STRUC	{
	struct {
		unsigned int EnableTxDMA:1;
		unsigned int TxDMABusy:1;
		unsigned int EnableRxDMA:1;
		unsigned int RxDMABusy:1;
		unsigned int WPDMABurstSIZE:2;
		unsigned int EnTXWriteBackDDONE:1;
		unsigned int BigEndian:1;
		unsigned int HDR_SEG_LEN:8;
		unsigned int rsv:14;
		unsigned int clk_gate_dis:1;
		unsigned int rx_2b_offset:1;
	} field;
	unsigned int word;
} WPDMA_GLO_CFG_STRUC, *PWPDMA_GLO_CFG_STRUC;
#endif


#define PBF_CTRL			0x0410
#define MCU_INT_STA		0x0414
#define MCU_INT_ENA	0x0418
#define TXRXQ_PCNT		0x0438
#define PBF_DBG			0x043c


#endif /* __PBF_H__ */

