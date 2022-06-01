/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	ral_nmac.h

	Abstract:
	Ralink Wireless Chip RAL MAC related definition & structures

	Revision History:
	Who			When		  What
	--------	----------	  ----------------------------------------------
*/

#ifndef __RAL_NMAC_H__
#define __RAL_NMAC_H__


#include "rtmp_type.h"

#ifdef MT7601
#include "mac_ral/omac/ral_omac_rxwi.h"
#else
#include "mac_ral/nmac/ral_nmac_rxwi.h"
#endif /* MT7601 */

#include "mac_ral/nmac/ral_nmac_txwi.h"

enum INFO_TYPE {
	NORMAL_PACKET,
	CMD_PACKET,
};

enum D_PORT {
	WLAN_PORT,
	CPU_RX_PORT,
	CPU_TX_PORT,
	HOST_PORT,
	VIRTUAL_CPU_RX_PORT,
	VIRTUAL_CPU_TX_PORT,
	DISCARD,
};

#ifdef RT_BIG_ENDIAN
typedef struct GNU_PACKED _TXINFO_NMAC_PKT{
	unsigned int info_type:2;
	unsigned int d_port:3;
	unsigned int QSEL:2;
	unsigned int wiv:1;
	unsigned int rsv1:2;
	unsigned int cso:1;
	unsigned int tso:1;
	unsigned int pkt_80211:1;
	unsigned int rsv0:1;
	unsigned int tx_burst:1;
	unsigned int next_vld:1;
	unsigned int pkt_len:16;
}TXINFO_NMAC_PKT;
#else
typedef struct GNU_PACKED _TXINFO_NMAC_PKT {
	unsigned int pkt_len:16;
	unsigned int next_vld:1;
	unsigned int tx_burst:1;
	unsigned int rsv0:1;
	unsigned int pkt_80211:1;
	unsigned int tso:1;
	unsigned int cso:1;
	unsigned int rsv1:2;
	unsigned int wiv:1;
	unsigned int QSEL:2;
	unsigned int d_port:3;
	unsigned int info_type:2;
}TXINFO_NMAC_PKT;
#endif /* RT_BIG_ENDIAN */

#define TxInfoWIV			txinfo_nmac_pkt.wiv
#define TxInfoQSEL			txinfo_nmac_pkt.QSEL
#define TxInfoPktLen			txinfo_nmac_pkt.pkt_len
#define TxInfoSwLstRnd		txinfo_nmac_pkt.rsv0
#define TxInfoUDMATxburst	txinfo_nmac_pkt.tx_burst
#define TxInfoUDMANextVld	txinfo_nmac_pkt.next_vld
#define TxInfoPKT_80211	txinfo_nmac_pkt.pkt_80211
#define TxInfoCSO			txinfo_nmac_pkt.cso
#define TxInfoTSO			txinfo_nmac_pkt.tso


#ifdef RT_BIG_ENDIAN
typedef struct GNU_PACKED _TXINFO_NMAC_CMD{
	unsigned int info_type:2;
	unsigned int d_port:3;
	unsigned int cmd_type:7;
	unsigned int cmd_seq:4;
	unsigned int pkt_len:16;
}TXINFO_NMAC_CMD;
#else
typedef struct GNU_PACKED _TXINFO_NMAC_CMD{
	unsigned int pkt_len:16;
	unsigned int cmd_seq:4;
	unsigned int cmd_type:7;
	unsigned int d_port:3;
	unsigned int info_type:2;
}TXINFO_NMAC_CMD;
#endif /* RT_BIG_ENDIAN */


typedef union GNU_PACKED _TXINFO_NMAC{
	struct _TXINFO_NMAC_PKT txinfo_pkt;
	struct _TXINFO_NMAC_CMD txinfo_cmd;
}TXINFO_NMAC;


/*
	Rx Memory layout:

	1. Rx Descriptor
		Rx Descriptor(12 Bytes) + RX_FCE_Info(4 Bytes)
	2. Rx Buffer
		RxInfo(4 Bytes) + RXWI() + 802.11 packet
*/


#ifdef RT_BIG_ENDIAN
typedef struct GNU_PACKED _RXFCE_INFO{
	unsigned int info_type:2;
	unsigned int s_port:3;
	unsigned int qsel:2;
	unsigned int pcie_intr:1;

	unsigned int mac_len:3;
	unsigned int l3l4_done:1;
	unsigned int pkt_80211:1;
	unsigned int ip_err:1;
	unsigned int tcp_err:1;
	unsigned int udp_err:1;
	
	unsigned int rsv:2;
	unsigned int pkt_len:14;
}RXFCE_INFO;
#else
typedef struct GNU_PACKED _RXFCE_INFO{
	unsigned int pkt_len:14;
	unsigned int rsv:2;

	unsigned int udp_err:1;
	unsigned int tcp_err:1;
	unsigned int ip_err:1;
	unsigned int pkt_80211:1;
	unsigned int l3l4_done:1;
	unsigned int mac_len:3;

	unsigned int pcie_intr:1;
	unsigned int qsel:2;
	unsigned int s_port:3;
	unsigned int info_type:2;
}RXFCE_INFO;
#endif /* RT_BIG_ENDIAN */

#ifdef RT_BIG_ENDIAN
typedef struct GNU_PACKED _RXFCE_INFO_CMD{
	unsigned int info_type:2;
	unsigned int d_port:3;
	unsigned int qsel:2;
	unsigned int pcie_intr:1;
	unsigned int evt_type:4;
	unsigned int cmd_seq:4;
	unsigned int self_gen:1;
	unsigned int rsv:1;
	unsigned int pkt_len:14;
}RXFCE_INFO_CMD;
#else
typedef struct GNU_PACKED _RXFCE_INFO_CMD{
	unsigned int pkt_len:14;
	unsigned int rsv:1;
	unsigned int self_gen:1;
	unsigned int cmd_seq:4;
	unsigned int evt_type:4;
	unsigned int pcie_intr:1;
	unsigned int qsel:2;
	unsigned int d_port:3;
	unsigned int info_type:2;
}RXFCE_INFO_CMD;
#endif


#ifdef RT_BIG_ENDIAN
typedef struct GNU_PACKED _RXINFO_NMAC{
	unsigned int ic_err:1;
	unsigned int tc_err:1;
	unsigned int ichk_bps:1;
	unsigned int tchk_bps:1;
	unsigned int rsv0:6;
	unsigned int pn_len:3;
	unsigned int wapi_kid:1;
	unsigned int bssid_idx3:1;
	unsigned int dec:1;
	unsigned int ampdu:1;
	unsigned int l2pad:1;
	unsigned int rssi:1;
	unsigned int htc:1;
	unsigned int amsdu:1;
	unsigned int mic_err:1;
	unsigned int icv_err:1;
	unsigned int crc_err:1;
	unsigned int mybss:1;
	unsigned int bc:1;
	unsigned int mc:1;
	unsigned int u2me:1;
	unsigned int frag:1;
	unsigned int null:1;
	unsigned int data:1;
	unsigned int ba:1;
}RXINFO_NMAC;
#else
typedef struct GNU_PACKED _RXINFO_NMAC{
	unsigned int ba:1;
	unsigned int data:1;
	unsigned int null:1;
	unsigned int frag:1;
	unsigned int u2me:1;
	unsigned int mcast:1;
	unsigned int bcast:1;
	unsigned int mybss:1;
	unsigned int crc_err:1;
	unsigned int icv_err:1;	
	unsigned int mic_err:1;
	unsigned int amsdu:1;
	unsigned int htc:1;
	unsigned int rssi:1;
	unsigned int l2pad:1;
	unsigned int ampdu:1;
	unsigned int dec:1;
	unsigned int bssid_idx3:1;
	unsigned int wapi_kid:1;
	unsigned int pn_len:3;
	unsigned int rsv0:6;
	unsigned int tchk_bps:1;
	unsigned int ichk_bps:1;
	unsigned int tc_err:1;
	unsigned int ic_err:1;
}RXINFO_NMAC;
#endif /* RT_BIG_ENDIAN */


/* ================================================================================= */
/* Register format */
/* ================================================================================= */


#define WLAN_FUN_CTRL		0x80
#ifdef RT_BIG_ENDIAN
typedef union _WLAN_FUN_CTRL_STRUC{
	struct{
		unsigned int GPIO0_OUT_OE_N:8;
		unsigned int GPIO0_OUT:8;
		unsigned int GPIO0_IN:8;
		unsigned int WLAN_ACC_BT:1;
		unsigned int INV_TR_SW0:1;
		unsigned int FRC_WL_ANT_SET:1;
		unsigned int PCIE_APP0_CLK_REQ:1;
		unsigned int WLAN_RESET:1;
		unsigned int WLAN_RESET_RF:1;
		unsigned int WLAN_CLK_EN:1;
		unsigned int WLAN_EN:1;
	}field;
	unsigned int word;
}WLAN_FUN_CTRL_STRUC, *PWLAN_FUN_CTRL_STRUC;
#else
typedef union _WLAN_FUN_CTRL_STRUC{
	struct{
		unsigned int WLAN_EN:1;
		unsigned int WLAN_CLK_EN:1;
		unsigned int WLAN_RESET_RF:1;
		unsigned int WLAN_RESET:1;
		unsigned int PCIE_APP0_CLK_REQ:1;
		unsigned int FRC_WL_ANT_SET:1;
		unsigned int INV_TR_SW0:1;
		unsigned int WLAN_ACC_BT:1;
		unsigned int GPIO0_IN:8;
		unsigned int GPIO0_OUT:8;
		unsigned int GPIO0_OUT_OE_N:8;
	}field;
	unsigned int word;
}WLAN_FUN_CTRL_STRUC, *PWLAN_FUN_CTRL_STRUC;
#endif


#define WLAN_FUN_INFO		0x84
#ifdef RT_BIG_ENDIAN
typedef union _WLAN_FUN_INFO_STRUC{
	struct{
		unsigned int		BT_EEP_BUSY:1; /* Read-only for WLAN Driver */
		unsigned int		Rsv1:26;		
		unsigned int		COEX_MODE:5; /* WLAN function enable */
	}field;
	unsigned int word;
}WLAN_FUN_INFO_STRUC, *PWLAN_FUN_INFO_STRUC;
#else
typedef union _WLAN_FUN_INFO_STRUC{
	struct{
		unsigned int		COEX_MODE:5; /* WLAN function enable */
		unsigned int		Rsv1:26;
		unsigned int		BT_EEP_BUSY:1; /* Read-only for WLAN Driver */
	}field;
	unsigned int word;
}WLAN_FUN_INFO_STRUC, *PWLAN_FUN_INFO_STRUC;
#endif


#define BT_FUN_CTRL		0xC0
#ifdef RT_BIG_ENDIAN
typedef union _BT_FUN_CTRL_STRUC{
	struct{
		unsigned int		GPIO1_OUT_OE_N:8;
		unsigned int		GPIO1_OUT:8;
		unsigned int		GPIO1_IN:8;
		unsigned int		BT_ACC_WLAN:1;
		unsigned int		INV_TR_SW1:1;
		unsigned int		URXD_GPIO_MODE:1;
		unsigned int		PCIE_APP1_CLK_REQ:1;
		unsigned int		BT_RESET:1;
		unsigned int		BT_RF_EN:1;
		unsigned int		BT_CLK_EN:1;
		unsigned int		BT_EN:1;
	}field;
	unsigned int word;
}BT_FUN_CTRL_STRUC, *PBT_FUN_CTRL_STRUC;
#else
typedef union _BT_FUN_CTRL_STRUC	{
	struct{
		unsigned int		BT_EN:1;
		unsigned int		BT_CLK_EN:1;
		unsigned int		BT_RF_EN:1;
		unsigned int		BT_RESET:1;
		unsigned int		PCIE_APP1_CLK_REQ:1;
		unsigned int		URXD_GPIO_MODE:1;
		unsigned int		INV_TR_SW1:1;
		unsigned int		BT_ACC_WLAN:1;
		unsigned int		GPIO1_IN:8;
		unsigned int		GPIO1_OUT:8;
		unsigned int		GPIO1_OUT_OE_N:8;
	}field;
	unsigned int word;
}BT_FUN_CTRL_STRUC, *PBT_FUN_CTRL_STRUC;
#endif


#define BT_FUN_INFO		0xC4
#ifdef RT_BIG_ENDIAN
typedef union _BT_FUN_INFO_STRUC{
	struct{
		unsigned int		WLAN_EEP_BUSY:1;
		unsigned int		BTPower1:7;	/* Peer */
		unsigned int		BTPower0:8; /* Self */
		unsigned int		AFH_END_CH:8;		
		unsigned int		AFH_START_CH:8;
	}field;
	unsigned int word;
} BT_FUN_INFO_STRUC, *PBT_FUN_INFO_STRUC;
#else
typedef	union _BT_FUN_INFO_STRUC	{
	struct{
		unsigned int		AFH_START_CH:8;
		unsigned int		AFH_END_CH:8;
		unsigned int		BTPower0:8;	/* Self */
		unsigned int		BTPower1:7;	/* Peer */
		unsigned int		WLAN_EEP_BUSY:1;			
	}field;
	unsigned int word;
}BT_FUN_INFO_STRUC, *PBT_FUN_INFO_STRUC;
#endif

// TODO: shiang, this data structure is not defined for register. may can move to other place
typedef struct _WLAN_BT_COEX_SETTING
{
	bool					ampduOff;
	bool					coexSettingRunning;
	bool					RateSelectionForceToUseRSSI;
	unsigned char					TxQualityFlag;
	unsigned long					alc;
	unsigned long					slna;
}WLAN_BT_COEX_SETTING, *PWLAN_BT_COEX_SETTING;


#define MCU_CMD_CFG 0x0234


#define TSO_CTRL	0x0250
#ifdef RT_BIG_ENDIAN
typedef union _TSO_CTRL_STRUC {
	struct {
		unsigned int rsv:13;
		unsigned int TSO_WR_LEN_EN:1;
		unsigned int TSO_SEG_EN:1;
		unsigned int TSO_EN:1;
		unsigned int RXWI_LEN:4;
		unsigned int RX_L2_FIX_LEN:4;
		unsigned int TXWI_LEN:4;
		unsigned int TX_L2_FIX_LEN:4;
	} field;
	unsigned int word;
} TSO_CTRL_STRUC;
#else
typedef union _TSO_CTRL_STRUC {
	struct {
		unsigned int TX_L2_FIX_LEN:4;
		unsigned int TXWI_LEN:4;
		unsigned int RX_L2_FIX_LEN:4;
		unsigned int RXWI_LEN:4;
		unsigned int TSO_EN:1;
		unsigned int TSO_SEG_EN:1;
		unsigned int TSO_WR_LEN_EN:1;
		unsigned int rsv:13;
	} field;
	unsigned int word;
} TSO_CTRL_STRUC;
#endif /* RT_BIG_ENDIAN */


#define TX_FBK_LIMIT	0x1398
#ifdef RT_BIG_ENDIAN
typedef union _TX_FBK_LIMIT_STRUC {
	struct {
		unsigned int rsv:13;
		unsigned int TX_RATE_LUT_EN:1;
		unsigned int TX_AMPDU_UP_CLEAR:1;
		unsigned int TX_MPDU_UP_CLEAR:1;
		unsigned int TX_AMPDU_FBK_LIMIT:8;
		unsigned int TX_MPDU_FBK_LIMIT:8;
	} field;
	unsigned int word;
} TX_FBK_LIMIT_STRUC;
#else
typedef union _TX_FBK_LIMIT_STRUC {
	struct {
		unsigned int TX_MPDU_FBK_LIMIT:8;
		unsigned int TX_AMPDU_FBK_LIMIT:8;
		unsigned int TX_MPDU_UP_CLEAR:1;
		unsigned int TX_AMPDU_UP_CLEAR:1;
		unsigned int TX_RATE_LUT_EN:1;
		unsigned int rsv:13;
	} field;
	unsigned int word;
} TX_FBK_LIMIT_STRUC;
#endif /* RT_BIG_ENDIAN */

#define TX_PROT_CFG6    0x13E0    // VHT 20 Protection
#define TX_PROT_CFG7    0x13E4    // VHT 40 Protection
#define TX_PROT_CFG8    0x13E8    // VHT 80 Protection
#define PIFS_TX_CFG     0x13EC    // PIFS setting


//----------------------------------------------------------------
// Header Translation 
//----------------------------------------------------------------

/* 
	WIFI INFO for TX translation 
	|TXINO|TXWI|WIFI INFO|802.3 MAC Header|Pyaload| 
*/

#define WIFI_INFO_SIZE		4
#ifdef RT_BIG_ENDIAN
typedef union GNU_PACKED _WIFI_INFO_STRUC {
	struct {
    	unsigned int More_Data:1;
    	unsigned int WEP:1;
    	unsigned int PS:1;
    	unsigned int RDG:1;
    	unsigned int QoS:1;
    	unsigned int EOSP:1;
    	unsigned int TID:4;
    	unsigned int Mode:2;
    	unsigned int VLAN:1;
    	unsigned int Rsv:3;
    	unsigned int BssIdx:4;
    	unsigned int Seq_Num:12;
	} field;
	unsigned int word;
} WIFI_INFO_STRUC, *PWIFI_INFO_STRUC;
#else
typedef union GNU_PACKED _WIFI_INFO_STRUC {
	struct {
    	unsigned int Seq_Num:12;
    	unsigned int BssIdx:4;
    	unsigned int Rsv:3;
    	unsigned int VLAN:1;
    	unsigned int Mode:2;
    	unsigned int TID:4;
    	unsigned int EOSP:1;
    	unsigned int QoS:1;
    	unsigned int RDG:1;
    	unsigned int PS:1;
    	unsigned int WEP:1;
    	unsigned int More_Data:1;
	} field;
	unsigned int word;
} WIFI_INFO_STRUC, *PWIFI_INFO_STRUC;
#endif /* RT_BIG_ENDIAN */

/* 
	header translation control register 
	bit0 --> TX translation enable
	bit1 --> RX translation enable 
*/
#define HEADER_TRANS_CTRL_REG	0x0260
#define HT_TX_ENABLE			0x1
#define HT_RX_ENABLE			0x2

#define HT_MAC_ADDR_DW0		0x02A4
#define HT_MAC_ADDR_DW1		0x02A8
#define HT_MAC_BSSID_DW0		0x02AC
#define HT_MAC_BSSID_DW1		0x02B0

#ifdef RT_BIG_ENDIAN
typedef union GNU_PACKED _HDR_TRANS_CTRL_STRUC {
	struct {
    	unsigned int Rsv:30;
    	unsigned int Rx_En:1;
    	unsigned int Tx_En:1;
	} field;
	unsigned int word;	
} HDR_TRANS_CTRL_STRUC, *PHDR_TRANS_CTRL_STRUC;
#else
typedef union GNU_PACKED _HDR_TRANS_CTRL_STRUC {
	struct {
    	unsigned int Tx_En:1;
    	unsigned int Rx_En:1;
    	unsigned int Rsv:30;
	} field;
	unsigned int word;
} HDR_TRANS_CTRL_STRUC, *PHDR_TRANS_CTRL_STRUC;
#endif /* RT_BIG_ENDIAN */

/* RX header translation enable by WCID */ 
#define HT_RX_WCID_EN_BASE	0x0264
#define HT_RX_WCID_OFFSET	32
#if defined(MT7601)
#define HT_RX_WCID_SIZE		(HT_RX_WCID_OFFSET * 4)	/* 128 WCIDs */
#elif defined(RT65xx)
#define HT_RX_WCID_SIZE		(HT_RX_WCID_OFFSET * 8)	/*	256 WCIDs */
#endif /* defined(RT63xx) */
#ifdef RT_BIG_ENDIAN
typedef union GNU_PACKED _HT_RX_WCID_EN_STRUC {
	struct {
    	unsigned int RX_WCID31_TRAN_EN:1;
    	unsigned int RX_WCID30_TRAN_EN:1;
    	unsigned int RX_WCID29_TRAN_EN:1;
    	unsigned int RX_WCID28_TRAN_EN:1;
    	unsigned int RX_WCID27_TRAN_EN:1;
    	unsigned int RX_WCID26_TRAN_EN:1;
    	unsigned int RX_WCID25_TRAN_EN:1;
    	unsigned int RX_WCID24_TRAN_EN:1;
    	unsigned int RX_WCID23_TRAN_EN:1;
    	unsigned int RX_WCID22_TRAN_EN:1;
    	unsigned int RX_WCID21_TRAN_EN:1;
    	unsigned int RX_WCID20_TRAN_EN:1;
    	unsigned int RX_WCID19_TRAN_EN:1;
    	unsigned int RX_WCID18_TRAN_EN:1;
    	unsigned int RX_WCID17_TRAN_EN:1;
    	unsigned int RX_WCID16_TRAN_EN:1;
    	unsigned int RX_WCID15_TRAN_EN:1;
    	unsigned int RX_WCID14_TRAN_EN:1;
    	unsigned int RX_WCID13_TRAN_EN:1;
    	unsigned int RX_WCID12_TRAN_EN:1;
    	unsigned int RX_WCID11_TRAN_EN:1;
    	unsigned int RX_WCID10_TRAN_EN:1;
    	unsigned int RX_WCID9_TRAN_EN:1;
    	unsigned int RX_WCID8_TRAN_EN:1;
    	unsigned int RX_WCID7_TRAN_EN:1;
    	unsigned int RX_WCID6_TRAN_EN:1;
    	unsigned int RX_WCID5_TRAN_EN:1;
    	unsigned int RX_WCID4_TRAN_EN:1;
    	unsigned int RX_WCID3_TRAN_EN:1;
    	unsigned int RX_WCID2_TRAN_EN:1;
    	unsigned int RX_WCID1_TRAN_EN:1;
    	unsigned int RX_WCID0_TRAN_EN:1;
	} field;
	unsigned int word;	
} HT_RX_WCID_EN_STRUC, *PHT_RX_WCID_EN_STRUC;
#else
typedef union GNU_PACKED _HT_RX_WCID_EN_STRUC {
	struct {
    	unsigned int RX_WCID0_TRAN_EN:1;
    	unsigned int RX_WCID1_TRAN_EN:1;
    	unsigned int RX_WCID2_TRAN_EN:1;
    	unsigned int RX_WCID3_TRAN_EN:1;
    	unsigned int RX_WCID4_TRAN_EN:1;
    	unsigned int RX_WCID5_TRAN_EN:1;
    	unsigned int RX_WCID6_TRAN_EN:1;
    	unsigned int RX_WCID7_TRAN_EN:1;
    	unsigned int RX_WCID8_TRAN_EN:1;
    	unsigned int RX_WCID9_TRAN_EN:1;
    	unsigned int RX_WCID10_TRAN_EN:1;
    	unsigned int RX_WCID11_TRAN_EN:1;
    	unsigned int RX_WCID12_TRAN_EN:1;
    	unsigned int RX_WCID13_TRAN_EN:1;
    	unsigned int RX_WCID14_TRAN_EN:1;
    	unsigned int RX_WCID15_TRAN_EN:1;
    	unsigned int RX_WCID16_TRAN_EN:1;
    	unsigned int RX_WCID17_TRAN_EN:1;
    	unsigned int RX_WCID18_TRAN_EN:1;
    	unsigned int RX_WCID19_TRAN_EN:1;
    	unsigned int RX_WCID20_TRAN_EN:1;
    	unsigned int RX_WCID21_TRAN_EN:1;
    	unsigned int RX_WCID22_TRAN_EN:1;
    	unsigned int RX_WCID23_TRAN_EN:1;
    	unsigned int RX_WCID24_TRAN_EN:1;
    	unsigned int RX_WCID25_TRAN_EN:1;
    	unsigned int RX_WCID26_TRAN_EN:1;
    	unsigned int RX_WCID27_TRAN_EN:1;
    	unsigned int RX_WCID28_TRAN_EN:1;
    	unsigned int RX_WCID29_TRAN_EN:1;
    	unsigned int RX_WCID30_TRAN_EN:1;
    	unsigned int RX_WCID31_TRAN_EN:1;
	} field;
	unsigned int word;
} HT_RX_WCID_EN_STRUC, *PHT_RX_WCID_EN_STRUC;
#endif /* RT_BIG_ENDIAN */

/* RX header translation - black list */
#define HT_RX_BL_BASE		0x0284
#define HT_RX_BL_OFFSET		2
#define HT_RX_BL_SIZE		8
#ifdef RT_BIG_ENDIAN
typedef union GNU_PACKED _HT_RX_BLACK_LIST_STRUC {
	struct {
    	unsigned int BLACK_ETHER_TYPE1:16;
    	unsigned int BLACK_ETHER_TYPE0:16;
	} field;
	unsigned int word;	
} HT_RX_BLACK_LIST_STRUC, *PHT_RX_BLACK_LIST_STRUC;
#else
typedef union GNU_PACKED _HT_RX_BLACK_LIST_STRUC {
	struct {
    	unsigned int BLACK_ETHER_TYPE0:16;
    	unsigned int BLACK_ETHER_TYPE1:16;
	} field;
	unsigned int word;
} HT_RX_BLACK_LIST_STRUC, *PHT_RX_BLACK_LIST_STRUC;
#endif /* RT_BIG_ENDIAN */

/* RX VLAN Mapping (TCI) */
#define HT_BSS_VLAN_BASE	0x0294
#define HT_BSS_VLAN_OFFSET	2
#define HT_BSS_VLAN_SIZE	8
#ifdef RT_BIG_ENDIAN
typedef union GNU_PACKED _HT_BSS_VLAN_STRUC {
	struct {
    	unsigned int TCI1_VID:12;
    	unsigned int TCI1_CFI:1;
    	unsigned int TCI1_PCP:3;
    	unsigned int TCI0_VID:12;
    	unsigned int TCI0_CFI:1;
    	unsigned int TCI0_PCP:3;
	} field;
	unsigned int word;	
} HT_BSS_VLAN_STRUC, *PHT_BSS_VLAN_STRUC;
#else
typedef union GNU_PACKED _HT_BSS_VLAN_STRUC {
	struct {
    	unsigned int TCI0_PCP:3;
    	unsigned int TCI0_CFI:1;
    	unsigned int TCI0_VID:12;
    	unsigned int TCI1_PCP:3;
    	unsigned int TCI1_CFI:1;
    	unsigned int TCI1_VID:12;
	} field;
	unsigned int word;
} HT_BSS_VLAN_STRUC, *PHT_BSS_VLAN_STRUC;
#endif /* RT_BIG_ENDIAN */


// TODO: shiang-6590, following definitions are dummy and not used for RT6590, shall remove/correct these!
#define GPIO_CTRL_CFG	0x0228
#define PBF_MAX_PCNT	0x0408 //actually, it's the TX_MAX_PCNT
// TODO:shiang-6590 --------------------------

#define PAIRWISE_KEY_TABLE_BASE     0x8000      /* 32-byte * 256-entry =  -byte */
#define HW_KEY_ENTRY_SIZE           0x20

#define PAIRWISE_IVEIV_TABLE_BASE     0xa000      /* 8-byte * 256-entry =  -byte */
#define MAC_IVEIV_TABLE_BASE     0xa000      /* 8-byte * 256-entry =  -byte */
#define HW_IVEIV_ENTRY_SIZE   8

#define MAC_WCID_ATTRIBUTE_BASE     0xa800      /* 4-byte * 256-entry =  -byte */
#define HW_WCID_ATTRI_SIZE   4

#define SHARED_KEY_TABLE_BASE       0xac00      /* 32-byte * 16-entry = 512-byte */
#define SHARED_KEY_MODE_BASE       0xb000      /* 32-byte * 16-entry = 512-byte */

#define HW_SHARED_KEY_MODE_SIZE   4
#define SHAREDKEYTABLE			0
#define PAIRWISEKEYTABLE			1

/* This resgiser is ONLY be supported for RT3883 or later.
   It conflicted with BCN#0 offset of previous chipset. */
#define WAPI_PN_TABLE_BASE		0xb800		
#define WAPI_PN_ENTRY_SIZE   		8

#define RF_MISC	0x0518
#ifdef RT_BIG_ENDIAN
typedef union _RF_MISC_STRUC{
	struct{
		unsigned int Rsv1:29;
		unsigned int EXT_PA_EN:1;
		unsigned int ADDAC_LDO_ADC9_EN:1;
		unsigned int ADDAC_LDO_ADC6_EN:1;
	}field;
	unsigned int word;
}RF_MISC_STRUC, *PRF_MISC_STRUC;
#else
typedef union _RF_MISC_STRUC{
	struct{
		unsigned int ADDAC_LDO_ADC6_EN:1;
		unsigned int ADDAC_LDO_ADC9_EN:1;
		unsigned int EXT_PA_EN:1;
		unsigned int Rsv1:29;
	}field;
	unsigned int word;
}RF_MISC_STRUC, *PRF_MISC_STRUC;
#endif

VOID ral_wlan_chip_onoff(
	IN struct _RTMP_ADAPTER *pAd,
	IN bool bOn,
	IN bool bResetWLAN);

#ifdef MT7601
VOID MT7601_WLAN_ChipOnOff(
	IN struct _RTMP_ADAPTER *pAd,
	IN bool bOn,
	IN bool bResetWLAN);
#endif /* MT7601 */

#define AUX_CLK_CFG		0x120C
#define BB_PA_MODE_CFG0	0x1214
#define BB_PA_MODE_CFG1	0x1218
#define RF_PA_MODE_CFG0 0x121C
#define RF_PA_MODE_CFG1	0x1220
#define TX_ALC_CFG_0	0x13B0
#define TX_ALC_CFG_1	0x13B4

#define TX0_RF_GAIN_CORR	0x13A0
#define TX0_RF_GAIN_ATTEN	0x13A8
#define TX0_BB_GAIN_ATTEN	0x13C0
#define TX_ALC_VGA3			0x13C8


#if defined(RT65xx) || defined(MT7601)
#define RESET_CTL			0x070C
#define INT_LEVEL			0x0718
#define COM_REG0			0x0730
#define COM_REG1			0x0734
#define COM_REG2			0x0738
#define COM_REG3			0x073C
#define PCIE_REMAP_BASE1	0x0740
#define PCIE_REMAP_BASE2	0x0744
#define PCIE_REMAP_BASE3	0x0748
#define PCIE_REMAP_BASE4	0x074C
#define LED_CTRL				0x0770
#define LED_TX_BLINK_0		0x0774
#define LED_TX_BLINK_1		0x0778
#define LED0_S0				0x077C
#define LED0_S1				0x0780
#define SEMAPHORE_00		0x07B0
#endif /* RT65xx */
#endif /* __RAL_NMAC_H__ */

