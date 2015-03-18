/****************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ****************************************************************************

    Module Name:
	rt6590.h

    Abstract:

    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
 */

#ifndef __RT6590_H__
#define __RT6590_H__

struct _RTMP_ADAPTER;

#define NIC6590_PCIe_DEVICE_ID 0x6590
#define MAX_RF_ID	127
#define MAC_RF_BANK 7

/*
	rsv: Reserved
	tcp: packet type, tcp : 1, udp:0
	tups: TCP/UDP header start offset (in unit of DWORD)
	ips: IP start offset (in unit of byte), base address of ips is the beginning of TXWI
	mss: Max Segment size (in unit of byte)
*/
#ifdef RT_BIG_ENDIAN
typedef struct _TSO_INFO_{
	UINT32 mss:16;
	UINT32 ips:8;
	UINT32 tups:6;	
	UINT32 tcp:1;	
	UINT32 rsv:1;
}TSO_INFO;
#else
typedef struct _TSO_INFO_{
	UINT32 rsv:1;
	UINT32 tcp:1;
	UINT32 tups:6;
	UINT32 ips:8;
	UINT32 mss:16;
}TSO_INFO;
#endif /* RT_BIG_ENDIAN */


/* 
 * Frequency plan item  for RT85592 
 * N: R9[4], R8[7:0]
 * K: R7[7], R9[3:0]
 * mod: R9[7:5], R11[3:2] (eg. mod=8 => 0x0, mod=10 => 0x2)
 * R: R11[1:0] (eg. R=1 => 0x0, R=3 => 0x2)
 */
typedef struct _RT8592_FREQ_ITEM {
	UINT8 Channel;
	UINT16 N;
	UINT8 K;
	UINT8 mod;
	UINT8 R;
} RT8592_FREQ_ITEM;

#define RT6590_RF_VER	"MT7650_WiFi_RF_Register_20120516.xls"
//#define RT6590_BBP_VER	"TC6008_BBP_CR_20120518.xls"
#define RT6590_BBP_VER	"TC6008_BBP_CR_20120522.xls"

/* 
	Rdiv: R24[1:0]  (2-bits)
	PLL_N: R29[7:0], R30[0]  (9-bits)
	PLL_K(Nominator): R31[4:0] (5-bits)
	Non-Sigma: !SDM R31[7:5] (3-bits)
	Den: (Denomina - 8) R32[4:0]  (5-bits)
	Loop Filter Config: R33, R34  
	Pll_idiv: frac comp R35[6:0]  (7-bits)
	
	5G only
		Pll_LDO: R16 [6:4] = <010>
*/
typedef struct _RT6590_FREQ_ITEM {
	UINT8 Channel;
	UINT8 Band;
	UINT16 PLL_N;
	UINT8 PLL_K;
	UINT8 Rdiv;
	UINT8 NonSigma;
	UINT8 Den;
	UINT8 LFC_R33;
	UINT8 LFC_R34;
	UINT32 Pll_idiv;
	UINT8 Pll_LDO; // 5G only
} RT6590_FREQ_ITEM;

#define RF_G_BAND 		0x01
#define RF_A_BAND 		0x02
#define RF_A_BAND_LB	0x04
#define RF_A_BAND_MB	0x08
#define RF_A_BAND_HB	0x10
typedef struct _RT6590_RF_SWITCH_ITEM {
	UCHAR Bank;
	UCHAR Register;
	UCHAR Band; /* G_Band, A_Band_LB, A_Band_MB, A_Band_HB */
	UCHAR BW;
	UCHAR Value;
} RT6590_RF_SWITCH_ITEM, *PRT6590_RF_SWITCH_ITEM;

typedef struct _RT6590_DCOC_Table {
	UCHAR Band; /* G_Band, A_Band_LB, A_Band_MB, A_Band_HB */
	RTMP_REG_PAIR RegDate;
} RT6590_DOCO_Table, *PRT6590_DOCO_Table;

VOID RT6590_Init(struct _RTMP_ADAPTER *pAd);
INT RT6590_ReadChannelPwr(struct _RTMP_ADAPTER *pAd);

#ifdef RT8592
VOID RT85592_Init(struct _RTMP_ADAPTER *pAd);
INT RT85592_ReadChannelPwr(struct _RTMP_ADAPTER *pAd);
VOID RT85592ReadTxPwrPerRate(struct _RTMP_ADAPTER *pAd);
#endif /* RT8592 */
VOID RT6590ReadTxPwrPerRate(struct _RTMP_ADAPTER *pAd);

VOID dump_bw_info(struct _RTMP_ADAPTER *pAd);

#endif /* __RT6590_H__ */

