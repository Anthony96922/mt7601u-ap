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
	ral_nmac_pbf.h

	Abstract:
	Ralink Wireless Chip MAC related definition & structures

	Revision History:
	Who			When		  What
	--------	----------	  ----------------------------------------------
*/

#ifndef __RAL_NMAC_PBF_H__
#define __RAL_NMAC_PBF_H__



/* ================================================================================= */
/* Register format  for PBF                                                                                                                                                     */
/* ================================================================================= */


/* Most are for debug. Driver doesn't touch PBF register. */
#define PBF_SYS_CTRL 	 0x0400

#ifdef RT_BIG_ENDIAN
typedef union _PBF_SYS_CTRL_STRUC {
	struct {
		UINT32 rsv5:7; /* Reserved */
		UINT32 CSR_TEST_EN:1;
		UINT32 MAC_CLKSEL:2; /* MAC clock selection */
		UINT32 PWRSV_EN:2;
		UINT32 SHRM_SEL:1; /* Shared memory access selection */
		UINT32 PBF_MSEL:2; /* Packet buffer memory access selection */
		UINT32 rsv4:5;
		UINT32 PBF_CLK_EN:1; /* PBF clock enable */
		UINT32 MAC_CLK_EN:1; /* MAC clock enable */
		UINT32 rsv3:6;
		UINT32 PBF_RESET:1; /* PBF hardware reset */
		UINT32 MAC_RESET:1; /* MAC hardware reset */
		UINT32 rsv:2;
	} field;
	UINT32 word;
} PBF_SYS_CTRL_STRUC;
#else
typedef union _PBF_SYS_CTRL_STRUC{
	struct {
                UINT32 rsv5:7; /* Reserved */
                UINT32 CSR_TEST_EN:1;
                UINT32 MAC_CLKSEL:2; /* MAC clock selection */
                UINT32 PWRSV_EN:2;
                UINT32 SHRM_SEL:1; /* Shared memory access selection */
                UINT32 PBF_MSEL:2; /* Packet buffer memory access selection */
                UINT32 rsv4:5;
                UINT32 PBF_CLK_EN:1; /* PBF clock enable */
                UINT32 MAC_CLK_EN:1; /* MAC clock enable */
                UINT32 rsv3:6;
                UINT32 PBF_RESET:1; /* PBF hardware reset */
                UINT32 MAC_RESET:1; /* MAC hardware reset */
                UINT32 rsv:2;
	}field;
	UINT32 word;
} PBF_SYS_CTRL_STRUC;
#endif


#define PBF_CFG			0x0404
#define TX_MAX_PCNT	0x0408
#define RX_MAX_PCNT	0x040c
#define PBF_CTRL			0x0410
#define RXQ_STA			0x0430
#define TXQ_STA			0x0434

#define BCN_OFFSET0		0x041c
#define BCN_OFFSET1		0x0420
#define BCN_OFFSET2		0x0424
#define BCN_OFFSET3		0x0428


#define FCE_CTRL			0x0800
#define FCE_PARAM		0x0804
#define CHECKSUM_OFFLOAD	0x0808

#ifdef RT_BIG_ENDIAN
typedef union _CSO_CTRL_STRUC {
	struct {
                UINT32 rsv:21;
                UINT32 stamp_seq_num_en:1;
                UINT32 cso_bigendian:1;
                UINT32 cso_en:1;
                UINT32 tx_ipv6_en:1;
                UINT32 tx_ipv4_cs_gen:1;
                UINT32 tx_tcp_cs_gen:1;
                UINT32 tx_udp_cs_gen:1;
                UINT32 rx_ipv6_en:1;
                UINT32 rx_ipv4_cs_en:1;
                UINT32 rx_tcp_cs_en:1;
                UINT32 rx_udp_cs_en:1;
	} field;
	UINT32 word;
} CSO_CTRL_STRUC;
#else
typedef union _CSO_CTRL_STRUC{
	struct {
                UINT32 rx_udp_cs_en:1;
                UINT32 rx_tcp_cs_en:1;
                UINT32 rx_ipv4_cs_en:1;
                UINT32 rx_ipv6_en:1;
                UINT32 tx_udp_cs_gen:1;
                UINT32 tx_tcp_cs_gen:1;
                UINT32 tx_ipv4_cs_gen:1;
                UINT32 tx_ipv6_en:1;
                UINT32 cso_en:1;
                UINT32 cso_bigendian:1;
                UINT32 stamp_seq_num_en:1;
                UINT32 rsv:21;
	}field;
	UINT32 word;
} CSO_CTRL_STRUC;
#endif

#endif /* __RAL_NMAC_PBF_H__ */

