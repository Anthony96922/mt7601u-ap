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
	rtmp_mac.h

	Abstract:
	Ralink Wireless Chip MAC related definition & structures

	Revision History:
	Who			When		  What
	--------	----------	  ----------------------------------------------
*/

#ifndef __RTMP_MAC_H__
#define __RTMP_MAC_H__


#ifdef RLT_MAC
#include "mac_ral/nmac/ral_nmac.h"
#endif /* RLT_MAC */

#ifdef RTMP_MAC
#include "mac_ral/omac/ral_omac.h"
#endif /* RTMP_MAC */


/*
	TX / RX ring descriptor format

	TX:
		PCI/RBUS_Descriptor + TXINFO + TXWI + 802.11

	Rx:
		PCI/RBUS/USB_Descripotr + (PCI/RBUS RXFCE_INFO) + (PCI/RBUS  RXINFO) + RXWI + 802.11 + (USB RXINFO)
	
*/

/* the first 24-byte in TXD is called TXINFO and will be DMAed to MAC block through TXFIFO. */
/* MAC block use this TXINFO to control the transmission behavior of this frame. */
#define FIFO_MGMT	0
#define FIFO_HCCA	1
#define FIFO_EDCA	2
#define FIFO_EDCA2	3

typedef	union GNU_PACKED _TXWI_STRUC {
#ifdef RLT_MAC
	struct _TXWI_NMAC TXWI_N;
#endif /* RLT_MAC */
#ifdef RTMP_MAC
	struct _TXWI_OMAC TXWI_O;
#endif /* RTMP_MAC */
	unsigned int word;
}TXWI_STRUC;


#define TXINFO_SIZE			4
typedef union GNU_PACKED _TXINFO_STRUC{
#ifdef RTMP_MAC
	struct _TXINFO_OMAC txinfo_omac;
#endif /* RTMP_MAC */
#ifdef RLT_MAC
	struct _TXINFO_NMAC_PKT txinfo_nmac_pkt;
	struct _TXINFO_NMAC_CMD txinfo_nmac_cmd;
#endif /* RLT_MAC */
	unsigned int word;
}TXINFO_STRUC;


/*
	RXWI wireless information format, in PBF. invisible in driver.
*/
typedef	union GNU_PACKED _RXWI_STRUC {
#ifdef RLT_MAC
#ifdef MT7601
	struct _RXWI_OMAC RXWI_O;
#else
	struct _RXWI_NMAC RXWI_N;
#endif /* MT7601 */
#endif /* RLT_MAC */
#ifdef RTMP_MAC
	struct _RXWI_OMAC RXWI_O;
#endif /* RTMP_MAC */
}RXWI_STRUC;


#define RXINFO_SIZE			4
#ifdef RT_BIG_ENDIAN
typedef	struct GNU_PACKED _RXINFO_STRUC {
	unsigned int		ip_sum_err:1;		/* IP checksum error */
	unsigned int		tcp_sum_err:1;	/* TCP checksum error */
	unsigned int		ip_sum_bypass:1;		/* IP checksum bypass(hw does not do checksum) */
	unsigned int		tcp_sum_bypass:1;	/* TCP/UDP checksum bypass(hw does not do checksum) */
#ifdef HDR_TRANS_SUPPORT
	unsigned int		rsv:5;
	unsigned int		pkt_80211:1;
#else
	unsigned int		rsv:6;
#endif /* HDR_TRANS_SUPPORT */
	unsigned int		pn_len:3;
	unsigned int		wapi_kidx:1;
	unsigned int		BssIdx3:1;
	unsigned int		Decrypted:1;
	unsigned int		AMPDU:1;
	unsigned int		L2PAD:1;
	unsigned int		RSSI:1;
	unsigned int		HTC:1;
	unsigned int		AMSDU:1;		/* rx with 802.3 header, not 802.11 header. obsolete. */
	unsigned int		CipherErr:2;        /* 0: decryption okay, 1:ICV error, 2:MIC error, 3:KEY not valid */
	unsigned int		Crc:1;			/* 1: CRC error */
	unsigned int		MyBss:1;		/* 1: this frame belongs to the same BSSID */
	unsigned int		Bcast:1;			/* 1: this is a broadcast frame */
	unsigned int		Mcast:1;			/* 1: this is a multicast frame */
	unsigned int		U2M:1;			/* 1: this RX frame is unicast to me */
	unsigned int		FRAG:1;
	unsigned int		NULLDATA:1;
	unsigned int		DATA:1;
	unsigned int		BA:1;
}	RXINFO_STRUC, *PRXINFO_STRUC;
#else
typedef	struct GNU_PACKED _RXINFO_STRUC {
	unsigned int		BA:1;
	unsigned int		DATA:1;
	unsigned int		NULLDATA:1;
	unsigned int		FRAG:1;
	unsigned int		U2M:1;
	unsigned int		Mcast:1;
	unsigned int		Bcast:1;
	unsigned int		MyBss:1;
	unsigned int		Crc:1;
	unsigned int		CipherErr:2;
	unsigned int		AMSDU:1;
	unsigned int		HTC:1;
	unsigned int		RSSI:1;
	unsigned int		L2PAD:1;
	unsigned int		AMPDU:1;
	unsigned int		Decrypted:1;
	unsigned int		BssIdx3:1;
	unsigned int		wapi_kidx:1;
	unsigned int		pn_len:3;
#ifdef HDR_TRANS_SUPPORT
	unsigned int		pkt_80211:1;
	unsigned int		rsv:5;
#else
	unsigned int		rsv:6;
#endif /* HDR_TRANS_SUPPORT */
	unsigned int		tcp_sum_bypass:1;
	unsigned int		ip_sum_bypass:1;
	unsigned int		tcp_sum_err:1;
	unsigned int		ip_sum_err:1;
}RXINFO_STRUC, *PRXINFO_STRUC;
#endif

#define TSO_SIZE		0


/* ================================================================================= */
/* Register format */
/* ================================================================================= */


/*
	SCH/DMA registers - base address 0x0200
*/
#define WMM_AIFSN_CFG   0x0214
#ifdef RT_BIG_ENDIAN
typedef	union _AIFSN_CSR_STRUC{
	struct {
	    unsigned int   Aifsn7:4;       /* for AC_VO */
	    unsigned int   Aifsn6:4;       /* for AC_VI */
	    unsigned int   Aifsn5:4;       /* for AC_BK */
	    unsigned int   Aifsn4:4;       /* for AC_BE */
	    unsigned int   Aifsn3:4;       /* for AC_VO */
	    unsigned int   Aifsn2:4;       /* for AC_VI */
	    unsigned int   Aifsn1:4;       /* for AC_BK */
	    unsigned int   Aifsn0:4;       /* for AC_BE */
	}field;
	unsigned int word;
} AIFSN_CSR_STRUC;
#else
typedef union _AIFSN_CSR_STRUC {
	struct {
	    unsigned int   Aifsn0:4;
	    unsigned int   Aifsn1:4;
	    unsigned int   Aifsn2:4;
	    unsigned int   Aifsn3:4;
	    unsigned int   Aifsn4:4;
	    unsigned int   Aifsn5:4;
	    unsigned int   Aifsn6:4;
	    unsigned int   Aifsn7:4;
	} field;
	unsigned int word;
} AIFSN_CSR_STRUC;
#endif

/* CWMIN_CSR: CWmin for each EDCA AC */
#define WMM_CWMIN_CFG   0x0218
#ifdef RT_BIG_ENDIAN
typedef	union _CWMIN_CSR_STRUC	{
	struct {
		unsigned int   Cwmin7:4;       /* for AC_VO */
		unsigned int   Cwmin6:4;       /* for AC_VI */
		unsigned int   Cwmin5:4;       /* for AC_BK */
		unsigned int   Cwmin4:4;       /* for AC_BE */
		unsigned int   Cwmin3:4;       /* for AC_VO */
		unsigned int   Cwmin2:4;       /* for AC_VI */
		unsigned int   Cwmin1:4;       /* for AC_BK */
		unsigned int   Cwmin0:4;       /* for AC_BE */
	} field;
	unsigned int word;
} CWMIN_CSR_STRUC;
#else
typedef	union _CWMIN_CSR_STRUC	{
	struct {
	    unsigned int   Cwmin0:4;
	    unsigned int   Cwmin1:4;
	    unsigned int   Cwmin2:4;
	    unsigned int   Cwmin3:4;
	    unsigned int   Cwmin4:4;
	    unsigned int   Cwmin5:4;
	    unsigned int   Cwmin6:4;
	    unsigned int   Cwmin7:4;
	} field;
	unsigned int word;
} CWMIN_CSR_STRUC;
#endif


/* CWMAX_CSR: CWmin for each EDCA AC */
#define WMM_CWMAX_CFG   0x021c
#ifdef RT_BIG_ENDIAN
typedef	union _CWMAX_CSR_STRUC {
	struct {
		unsigned int   Cwmax7:4;       /* for AC_VO */
		unsigned int   Cwmax6:4;       /* for AC_VI */
		unsigned int   Cwmax5:4;       /* for AC_BK */
		unsigned int   Cwmax4:4;       /* for AC_BE */
		unsigned int   Cwmax3:4;       /* for AC_VO */
		unsigned int   Cwmax2:4;       /* for AC_VI */
		unsigned int   Cwmax1:4;       /* for AC_BK */
		unsigned int   Cwmax0:4;       /* for AC_BE */
	} field;
	unsigned int word;
} CWMAX_CSR_STRUC;
#else
typedef	union _CWMAX_CSR_STRUC {
	struct {
	    unsigned int   Cwmax0:4;
	    unsigned int   Cwmax1:4;
	    unsigned int   Cwmax2:4;
	    unsigned int   Cwmax3:4;
	    unsigned int   Cwmax4:4;
	    unsigned int   Cwmax5:4;
	    unsigned int   Cwmax6:4;
	    unsigned int   Cwmax7:4;
	} field;
	unsigned int word;
}	CWMAX_CSR_STRUC;
#endif


/* AC_TXOP_CSR0: AC_BK/AC_BE TXOP register */
#define WMM_TXOP0_CFG    0x0220
#ifdef RT_BIG_ENDIAN
typedef	union _AC_TXOP_CSR0_STRUC {
	struct {
	    unsigned short  Ac1Txop; /* for AC_BE, in unit of 32us */
	    unsigned short  Ac0Txop; /* for AC_BK, in unit of 32us */
	} field;
	unsigned int word;
} AC_TXOP_CSR0_STRUC;
#else
typedef	union _AC_TXOP_CSR0_STRUC {
	struct {
	    unsigned short  Ac0Txop;
	    unsigned short  Ac1Txop;
	} field;
	unsigned int word;
} AC_TXOP_CSR0_STRUC;
#endif


/* AC_TXOP_CSR1: AC_VO/AC_VI TXOP register */
#define WMM_TXOP1_CFG    0x0224
#ifdef RT_BIG_ENDIAN
typedef	union _AC_TXOP_CSR1_STRUC {
	struct {
	    unsigned short  Ac3Txop; /* for AC_VO, in unit of 32us */
	    unsigned short  Ac2Txop; /* for AC_VI, in unit of 32us */
	} field;
	unsigned int word;
} AC_TXOP_CSR1_STRUC;
#else
typedef	union _AC_TXOP_CSR1_STRUC {
	struct {
	    unsigned short Ac2Txop;
	    unsigned short Ac3Txop;
	} field;
	unsigned int word;
} AC_TXOP_CSR1_STRUC;
#endif


#define WMM_TXOP2_CFG 0x0228
#define WMM_TXOP3_CFG 0x022c

#define WMM_CTRL	0x0230



/*================================================================================= */
/* MAC  registers                                                                                                                                                                 */
/*================================================================================= */
/*  4.1 MAC SYSTEM  configuration registers (offset:0x1000) */
#define MAC_CSR0            0x1000
#ifdef RT_BIG_ENDIAN
typedef	union _ASIC_VER_ID_STRUC {
	struct {
	    unsigned short  ASICVer;        /* version */
	    unsigned short  ASICRev;        /* reversion */
	} field;
	unsigned int word;
} ASIC_VER_ID_STRUC;
#else
typedef	union _ASIC_VER_ID_STRUC {
	struct {
	    unsigned short  ASICRev;
	    unsigned short  ASICVer;
	} field;
	unsigned int word;
} ASIC_VER_ID_STRUC;
#endif

#define MAC_SYS_CTRL            0x1004
#define MAC_ADDR_DW0		0x1008
#define MAC_ADDR_DW1		0x100c

/* MAC_CSR2: STA MAC register 0 */
#ifdef RT_BIG_ENDIAN
typedef	union _MAC_DW0_STRUC {
	struct {
		unsigned char Byte3;		/* MAC address byte 3 */
		unsigned char Byte2;		/* MAC address byte 2 */
		unsigned char Byte1;		/* MAC address byte 1 */
		unsigned char Byte0;		/* MAC address byte 0 */
	} field;
	unsigned int word;
} MAC_DW0_STRUC;
#else
typedef	union _MAC_DW0_STRUC {
	struct {
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
	} field;
	unsigned int word;
} MAC_DW0_STRUC;
#endif


/* MAC_CSR3: STA MAC register 1 */
#ifdef RT_BIG_ENDIAN
typedef	union _MAC_DW1_STRUC {
	struct {
		unsigned char		Rsvd1;
		unsigned char		U2MeMask;
		unsigned char		Byte5;		/* MAC address byte 5 */
		unsigned char		Byte4;		/* MAC address byte 4 */
	} field;
	unsigned int word;
} MAC_DW1_STRUC;
#else
typedef	union _MAC_DW1_STRUC {
	struct {
		unsigned char Byte4;
		unsigned char Byte5;
		unsigned char U2MeMask;
		unsigned char Rsvd1;
	} field;
	unsigned int word;
}	MAC_DW1_STRUC;
#endif

#define MAC_BSSID_DW0		0x1010
#define MAC_BSSID_DW1		0x1014
/* MAC_CSR5: BSSID register 1 */
#ifdef RT_BIG_ENDIAN
typedef	union	_MAC_BSSID_DW1_STRUC {
	struct {
		unsigned int NMBssMode3:1;
		unsigned int NMBssMode2:1;
		unsigned int NMBssMode:1;
		unsigned int MBssBcnNum:3;
		unsigned int MBssMode:2; /* 0: one BSSID, 10: 4 BSSID,  01: 2 BSSID , 11: 8BSSID */
		unsigned int Byte5:8;		 /* BSSID byte 5 */
		unsigned int Byte4:8;		 /* BSSID byte 4 */
	} field;
	unsigned int word;
} MAC_BSSID_DW1_STRUC;
#else
typedef	union	_MAC_BSSID_DW1_STRUC {
	struct {
		unsigned int Byte4:8;
		unsigned int Byte5:8;
		unsigned int MBssMode:2;
		unsigned int MBssBcnNum:3;
		unsigned int NMBssMode:1;
		unsigned int NMBssMode2:1;		
		unsigned int NMBssMode3:1;
	} field;
	unsigned int word;
} MAC_BSSID_DW1_STRUC;
#endif

/* rt2860b max 16k bytes. bit12:13 Maximum PSDU length (power factor) 0:2^13, 1:2^14, 2:2^15, 3:2^16 */
#define MAX_LEN_CFG              0x1018


/* BBP_CSR_CFG: BBP serial control register */
#define BBP_CSR_CFG            	0x101c
#ifdef RT_BIG_ENDIAN
typedef	union _BBP_CSR_CFG_STRUC {
	struct {
		unsigned int		:12;		
		unsigned int		BBP_RW_MODE:1;	/* 0: use serial mode  1:parallel */
		unsigned int		BBP_PAR_DUR:1;		/* 0: 4 MAC clock cycles  1: 8 MAC clock cycles */
		unsigned int		Busy:1;				/* 1: ASIC is busy execute BBP programming. */
		unsigned int		fRead:1;				/* 0: Write BBP, 1:	Read BBP */
		unsigned int		RegNum:8;			/* Selected BBP register */
		unsigned int		Value:8;				/* Register value to program into BBP */
	} field;
	unsigned int word;
} BBP_CSR_CFG_STRUC;
#else
typedef	union _BBP_CSR_CFG_STRUC {
	struct {
		unsigned int		Value:8;
		unsigned int		RegNum:8;
		unsigned int		fRead:1;
		unsigned int		Busy:1;
		unsigned int		BBP_PAR_DUR:1;
		unsigned int		BBP_RW_MODE:1;
		unsigned int		:12;
	} field;
	unsigned int word;
} BBP_CSR_CFG_STRUC;
#endif


/* RF_CSR_CFG: RF control register */
#define RF_CSR_CFG0            		0x1020
#ifdef RT_BIG_ENDIAN
typedef	union _RF_CSR_CFG0_STRUC {
	struct {
		unsigned int Busy:1;		    /* 0: idle 1: 8busy */
		unsigned int Sel:1;			/* 0:RF_LE0 activate  1:RF_LE1 activate */
		unsigned int StandbyMode:1;	/* 0: high when stand by 1:	low when standby */
		unsigned int bitwidth:5;		/* Selected BBP register */
		unsigned int RegIdAndContent:24;	/* Register value to program into BBP */
	} field;
	unsigned int word;
} RF_CSR_CFG0_STRUC;
#else
typedef	union _RF_CSR_CFG0_STRUC {
	struct {
		unsigned int RegIdAndContent:24;
		unsigned int bitwidth:5;
		unsigned int StandbyMode:1;
		unsigned int Sel:1;
		unsigned int Busy:1;
	} field;
	unsigned int word;
} RF_CSR_CFG0_STRUC;
#endif


#define RF_CSR_CFG1           		0x1024		 
#ifdef RT_BIG_ENDIAN
typedef	union _RF_CSR_CFG1_STRUC {
	struct {
		unsigned int rsv:7;	/* 0: idle 1: 8busy */
		unsigned int RFGap:5;	/* Gap between BB_CONTROL_RF and RF_LE. 0: 3 system clock cycle (37.5usec) 1: 5 system clock cycle (62.5usec) */
		unsigned int RegIdAndContent:24;	/* Register value to program into BBP */
	} field;
	unsigned int word;
} RF_CSR_CFG1_STRUC;
#else
typedef	union _RF_CSR_CFG1_STRUC {
	struct {
		unsigned int RegIdAndContent:24;
		unsigned int RFGap:5;
		unsigned int rsv:7;
	} field;
	unsigned int word;
} RF_CSR_CFG1_STRUC;
#endif


#define RF_CSR_CFG2           		0x1028
#ifdef RT_BIG_ENDIAN
typedef	union _RF_CSR_CFG2_STRUC {
	struct {
		unsigned int rsv:8;		    /* 0: idle 1: 8busy */
		unsigned int RegIdAndContent:24; /* Register value to program into BBP */
	} field;
	unsigned int word;
}	RF_CSR_CFG2_STRUC;
#else
typedef	union _RF_CSR_CFG2_STRUC {
	struct {
		unsigned int RegIdAndContent:24;
		unsigned int rsv:8;
	} field;
	unsigned int word;
} RF_CSR_CFG2_STRUC;
#endif


#define LED_CFG           		0x102c 
#ifdef RT_BIG_ENDIAN
typedef	union _LED_CFG_STRUC {
	struct {
		unsigned int		:1;			 
		unsigned int		LedPolar:1;			/* Led Polarity.  0: active low1: active high */
		unsigned int		YLedMode:2;			/* yellow Led Mode */
		unsigned int		GLedMode:2;			/* green Led Mode */
		unsigned int		RLedMode:2;			/* red Led Mode    0: off1: blinking upon TX2: periodic slow blinking3: always on */
		unsigned int		rsv:2;		    
		unsigned int		SlowBlinkPeriod:6;			/* slow blinking period. unit:1ms */
		unsigned int		OffPeriod:8;			/* blinking off period unit 1ms */
		unsigned int		OnPeriod:8;			/* blinking on period unit 1ms */
	} field;
	unsigned int word;
} LED_CFG_STRUC;
#else
typedef	union _LED_CFG_STRUC {
	struct {
		unsigned int		OnPeriod:8;
		unsigned int		OffPeriod:8;
		unsigned int		SlowBlinkPeriod:6;
		unsigned int		rsv:2;
		unsigned int		RLedMode:2;
		unsigned int		GLedMode:2;
		unsigned int		YLedMode:2;	
		unsigned int		LedPolar:1;
		unsigned int		:1;
	} field;
	unsigned int word;
} LED_CFG_STRUC;
#endif


#define AMPDU_MAX_LEN_20M1S	0x1030
#define AMPDU_MAX_LEN_20M2S	0x1034
#define AMPDU_MAX_LEN_40M1S	0x1038
#define AMPDU_MAX_LEN_40M2S	0x103c
#define AMPDU_MAX_LEN			0x1040


/* The number of the Tx chains */
#define NUM_OF_TX_CHAIN		4

#define TX_CHAIN_ADDR0_L	0x1044		/* Stream mode MAC address registers */
#define TX_CHAIN_ADDR0_H	0x1048
#define TX_CHAIN_ADDR1_L	0x104C
#define TX_CHAIN_ADDR1_H	0x1050
#define TX_CHAIN_ADDR2_L	0x1054
#define TX_CHAIN_ADDR2_H	0x1058
#define TX_CHAIN_ADDR3_L	0x105C
#define TX_CHAIN_ADDR3_H	0x1060

#ifdef RT_BIG_ENDIAN
typedef union _TX_CHAIN_ADDR0_L_STRUC {
	struct {
		unsigned char TxChainAddr0L_Byte3; /* Destination MAC address of Tx chain0 (byte 3) */
		unsigned char TxChainAddr0L_Byte2; /* Destination MAC address of Tx chain0 (byte 2) */
		unsigned char TxChainAddr0L_Byte1; /* Destination MAC address of Tx chain0 (byte 1) */
		unsigned char TxChainAddr0L_Byte0; /* Destination MAC address of Tx chain0 (byte 0) */
	} field;
	unsigned int word;
} TX_CHAIN_ADDR0_L_STRUC;
#else
typedef union _TX_CHAIN_ADDR0_L_STRUC {
	struct {
		unsigned char TxChainAddr0L_Byte0;
		unsigned char TxChainAddr0L_Byte1;
		unsigned char TxChainAddr0L_Byte2;
		unsigned char TxChainAddr0L_Byte3;
	} field;
	unsigned int word;
} TX_CHAIN_ADDR0_L_STRUC;
#endif


#ifdef RT_BIG_ENDIAN
typedef union _TX_CHAIN_ADDR0_H_STRUC {
	struct {
		unsigned short	Reserved:12; /* Reserved */
		unsigned short	TxChainSel0:4; /* Selection value of Tx chain0 */
		unsigned char	TxChainAddr0H_Byte5; /* Destination MAC address of Tx chain0 (byte 5) */
		unsigned char	TxChainAddr0H_Byte4; /* Destination MAC address of Tx chain0 (byte 4) */
	} field;
	unsigned int word;
} TX_CHAIN_ADDR0_H_STRUC;
#else
typedef union _TX_CHAIN_ADDR0_H_STRUC {
	struct {
		unsigned char	TxChainAddr0H_Byte4; /* Destination MAC address of Tx chain0 (byte 4) */
		unsigned char	TxChainAddr0H_Byte5; /* Destination MAC address of Tx chain0 (byte 5) */
		unsigned short	TxChainSel0:4; /* Selection value of Tx chain0 */
		unsigned short	Reserved:12; /* Reserved */
	} field;
	unsigned int word;
} TX_CHAIN_ADDR0_H_STRUC;
#endif


#ifdef BIG_ENDIAN
typedef union _TX_CHAIN_ADDR1_L_STRUC {
	struct {
		unsigned char	TxChainAddr1L_Byte3; /* Destination MAC address of Tx chain1 (byte 3) */
		unsigned char	TxChainAddr1L_Byte2; /* Destination MAC address of Tx chain1 (byte 2) */
		unsigned char	TxChainAddr1L_Byte1; /* Destination MAC address of Tx chain1 (byte 1) */
		unsigned char	TxChainAddr1L_Byte0; /* Destination MAC address of Tx chain1 (byte 0) */
	} field;
	unsigned int word;
} TX_CHAIN_ADDR1_L_STRUC, *PTX_CHAIN_ADDR1_L_STRUC;
#else
typedef union _TX_CHAIN_ADDR1_L_STRUC {
	struct {
		unsigned char	TxChainAddr1L_Byte0;
		unsigned char	TxChainAddr1L_Byte1;
		unsigned char	TxChainAddr1L_Byte2;
		unsigned char	TxChainAddr1L_Byte3;
	} field;
	unsigned int word;
} TX_CHAIN_ADDR1_L_STRUC, *PTX_CHAIN_ADDR1_L_STRUC;
#endif


#ifdef RT_BIG_ENDIAN
typedef union _TX_CHAIN_ADDR1_H_STRUC {
	struct {
		unsigned short	Reserved:12; /* Reserved */
		unsigned short	TxChainSel0:4; /* Selection value of Tx chain0 */
		unsigned char	TxChainAddr1H_Byte5; /* Destination MAC address of Tx chain1 (byte 5) */
		unsigned char	TxChainAddr1H_Byte4; /* Destination MAC address of Tx chain1 (byte 4) */
	} field;
	unsigned int word;
} TX_CHAIN_ADDR1_H_STRUC ;
#else
typedef union _TX_CHAIN_ADDR1_H_STRUC {
	struct {
		unsigned char	TxChainAddr1H_Byte4; 
		unsigned char	TxChainAddr1H_Byte5; 
		unsigned short	TxChainSel0:4; 
		unsigned short	Reserved:12; 
	} field;
	unsigned int word;
} TX_CHAIN_ADDR1_H_STRUC ;
#endif


#ifdef RT_BIG_ENDIAN
typedef union _TX_CHAIN_ADDR2_L_STRUC {
	struct {
		unsigned char	TxChainAddr2L_Byte3; /* Destination MAC address of Tx chain2 (byte 3) */
		unsigned char	TxChainAddr2L_Byte2; /* Destination MAC address of Tx chain2 (byte 2) */
		unsigned char	TxChainAddr2L_Byte1; /* Destination MAC address of Tx chain2 (byte 1) */
		unsigned char	TxChainAddr2L_Byte0; /* Destination MAC address of Tx chain2 (byte 0) */
	} field;
	unsigned int word;
} TX_CHAIN_ADDR2_L_STRUC;
#else
typedef union _TX_CHAIN_ADDR2_L_STRUC {
	struct {
		unsigned char	TxChainAddr2L_Byte0;
		unsigned char	TxChainAddr2L_Byte1;
		unsigned char	TxChainAddr2L_Byte2;
		unsigned char	TxChainAddr2L_Byte3;
	} field;
	unsigned int word;
} TX_CHAIN_ADDR2_L_STRUC;
#endif


#ifdef RT_BIG_ENDIAN
typedef union _TX_CHAIN_ADDR2_H_STRUC {
	struct {
		unsigned short	Reserved:12; /* Reserved */
		unsigned short	TxChainSel0:4; /* Selection value of Tx chain0 */
		unsigned char	TxChainAddr2H_Byte5; /* Destination MAC address of Tx chain2 (byte 5) */
		unsigned char	TxChainAddr2H_Byte4; /* Destination MAC address of Tx chain2 (byte 4) */
	} field;
	unsigned int word;
} TX_CHAIN_ADDR2_H_STRUC;
#else
typedef union _TX_CHAIN_ADDR2_H_STRUC {
	struct {
		unsigned char	TxChainAddr2H_Byte4;
		unsigned char	TxChainAddr2H_Byte5;
		unsigned short	TxChainSel0:4;
		unsigned short	Reserved:12;
	} field;
	unsigned int word;
} TX_CHAIN_ADDR2_H_STRUC;
#endif


#ifdef RT_BIG_ENDIAN
typedef union _TX_CHAIN_ADDR3_L_STRUC {
	struct {
		unsigned char	TxChainAddr3L_Byte3; /* Destination MAC address of Tx chain3 (byte 3) */
		unsigned char	TxChainAddr3L_Byte2; /* Destination MAC address of Tx chain3 (byte 2) */
		unsigned char	TxChainAddr3L_Byte1; /* Destination MAC address of Tx chain3 (byte 1) */
		unsigned char	TxChainAddr3L_Byte0; /* Destination MAC address of Tx chain3 (byte 0) */
	} field;
	unsigned int word;
} TX_CHAIN_ADDR3_L_STRUC, *PTX_CHAIN_ADDR3_L_STRUC;
#else
typedef union _TX_CHAIN_ADDR3_L_STRUC {
	struct {
		unsigned char	TxChainAddr3L_Byte0;
		unsigned char	TxChainAddr3L_Byte1;
		unsigned char	TxChainAddr3L_Byte2;
		unsigned char	TxChainAddr3L_Byte3;
	} field;
	unsigned int word;
}	TX_CHAIN_ADDR3_L_STRUC, *PTX_CHAIN_ADDR3_L_STRUC;
#endif


#ifdef RT_BIG_ENDIAN
typedef union _TX_CHAIN_ADDR3_H_STRUC {
	struct {
		unsigned short	Reserved:12; /* Reserved */
		unsigned short	TxChainSel0:4; /* Selection value of Tx chain0 */
		unsigned char	TxChainAddr3H_Byte5; /* Destination MAC address of Tx chain3 (byte 5) */
		unsigned char	TxChainAddr3H_Byte4; /* Destination MAC address of Tx chain3 (byte 4) */
	} field;
	unsigned int word;
} TX_CHAIN_ADDR3_H_STRUC, *PTX_CHAIN_ADDR3_H_STRUC;
#else
typedef union _TX_CHAIN_ADDR3_H_STRUC {
	struct {
		unsigned char	TxChainAddr3H_Byte4;
		unsigned char	TxChainAddr3H_Byte5;
		unsigned short	TxChainSel0:4;
		unsigned short	Reserved:12;
	} field;
	unsigned int word;
} TX_CHAIN_ADDR3_H_STRUC;
#endif



/*  4.2 MAC TIMING  configuration registers (offset:0x1100) */
#define XIFS_TIME_CFG             0x1100
#ifdef RT_BIG_ENDIAN
typedef	union _IFS_SLOT_CFG_STRUC {
	struct {
	    unsigned int  rsv:2;         
	    unsigned int  BBRxendEnable:1;        /*  reference RXEND signal to begin XIFS defer */
	    unsigned int  EIFS:9;        /*  unit 1us */
	    unsigned int  OfdmXifsTime:4;        /*OFDM SIFS. unit 1us. Applied after OFDM RX when MAC doesn't reference BBP signal BBRXEND */
	    unsigned int  OfdmSifsTime:8;        /*  unit 1us. Applied after OFDM RX/TX */
	    unsigned int  CckmSifsTime:8;        /*  unit 1us. Applied after CCK RX/TX */
	} field;
	unsigned int word;
} IFS_SLOT_CFG_STRUC;
#else
typedef	union _IFS_SLOT_CFG_STRUC {
	struct {
	    unsigned int  CckmSifsTime:8;
	    unsigned int  OfdmSifsTime:8;
	    unsigned int  OfdmXifsTime:4;
	    unsigned int  EIFS:9;
	    unsigned int  BBRxendEnable:1;
	    unsigned int  rsv:2;         
	} field;
	unsigned int word;
} IFS_SLOT_CFG_STRUC;
#endif

#define BKOFF_SLOT_CFG		0x1104
#define NAV_TIME_CFG		0x1108
#define CH_TIME_CFG			0x110C
#define PBF_LIFE_TIMER		0x1110	/*TX/RX MPDU timestamp timer (free run)Unit: 1us */


/* BCN_TIME_CFG : Synchronization control register */
#define BCN_TIME_CFG             0x1114
#ifdef RT_BIG_ENDIAN
typedef	union _BCN_TIME_CFG_STRUC {
	struct {
		unsigned int TxTimestampCompensate:8;
		unsigned int :3;
		unsigned int bBeaconGen:1;		/* Enable beacon generator */
		unsigned int bTBTTEnable:1;
		unsigned int TsfSyncMode:2;		/* Enable TSF sync, 00: disable, 01: infra mode, 10: ad-hoc mode */
		unsigned int bTsfTicking:1;		/* Enable TSF auto counting */
		unsigned int BeaconInterval:16;  /* in unit of 1/16 TU */
	} field;
	unsigned int word;
} BCN_TIME_CFG_STRUC;
#else
typedef union _BCN_TIME_CFG_STRUC {
	struct {
		unsigned int BeaconInterval:16;
		unsigned int bTsfTicking:1;
		unsigned int TsfSyncMode:2;
		unsigned int bTBTTEnable:1;
		unsigned int bBeaconGen:1;
		unsigned int :3;
		unsigned int TxTimestampCompensate:8;
	} field;
	unsigned int word;
} BCN_TIME_CFG_STRUC;
#endif


#define TBTT_SYNC_CFG		0x1118
#define TSF_TIMER_DW0		0x111c	/* Local TSF timer lsb 32 bits. Read-only */
#define TSF_TIMER_DW1		0x1120	/* msb 32 bits. Read-only. */
#define TBTT_TIMER			0x1124	/* TImer remains till next TBTT. Read-only */
#define INT_TIMER_CFG		0x1128
#define INT_TIMER_EN		0x112c	/* GP-timer and pre-tbtt Int enable */
#define CH_IDLE_STA			0x1130	/* channel idle time */
#define CH_BUSY_STA			0x1134	/* channle busy time */
#define CH_BUSY_STA_SEC	0x1138	/* channel busy time for secondary channel */


/*  4.2 MAC POWER  configuration registers (offset:0x1200) */
#define MAC_STATUS_CFG		0x1200
#define PWR_PIN_CFG		0x1204


/* AUTO_WAKEUP_CFG: Manual power control / status register */
#define AUTO_WAKEUP_CFG	0x1208
#ifdef RT_BIG_ENDIAN
typedef	union _AUTO_WAKEUP_STRUC {
	struct {
		unsigned int :16;
		unsigned int EnableAutoWakeup:1;	/* 0:sleep, 1:awake */
		unsigned int NumofSleepingTbtt:7;          /* ForceWake has high privilege than PutToSleep when both set */
		unsigned int AutoLeadTime:8;
	} field;
	unsigned int word;
} AUTO_WAKEUP_STRUC;
#else
typedef	union _AUTO_WAKEUP_STRUC {
	struct {
		unsigned int AutoLeadTime:8;
		unsigned int NumofSleepingTbtt:7;
		unsigned int EnableAutoWakeup:1;
		unsigned int :16;
	} field;
	unsigned int word;
} AUTO_WAKEUP_STRUC;
#endif


/*  4.3 MAC TX  configuration registers (offset:0x1300) */
#define EDCA_AC0_CFG	0x1300
#define EDCA_AC1_CFG	0x1304
#define EDCA_AC2_CFG	0x1308
#define EDCA_AC3_CFG	0x130c
#ifdef RT_BIG_ENDIAN
typedef	union _EDCA_AC_CFG_STRUC {
	struct {
	    unsigned int :12;
	    unsigned int Cwmax:4;	/* unit power of 2 */
	    unsigned int Cwmin:4;
	    unsigned int Aifsn:4;	/* # of slot time */
	    unsigned int AcTxop:8;	/*  in unit of 32us */
	} field;
	unsigned int word;
} EDCA_AC_CFG_STRUC;
#else
typedef	union _EDCA_AC_CFG_STRUC {
	struct {
	    unsigned int AcTxop:8;
	    unsigned int Aifsn:4;
	    unsigned int Cwmin:4;
	    unsigned int Cwmax:4;
	    unsigned int :12;
	} field;
	unsigned int word;
} EDCA_AC_CFG_STRUC;
#endif

#define EDCA_TID_AC_MAP	0x1310


/* Default Tx power */
#define DEFAULT_TX_POWER	0x6

#define TX_PWR_CFG_0		0x1314
#define TX_PWR_CFG_0_EXT	0x1390
#define TX_PWR_CFG_1		0x1318
#define TX_PWR_CFG_1_EXT	0x1394
#define TX_PWR_CFG_2		0x131C
#define TX_PWR_CFG_2_EXT	0x1398
#define TX_PWR_CFG_3		0x1320
#define TX_PWR_CFG_3_EXT	0x139C
#define TX_PWR_CFG_4		0x1324
#define TX_PWR_CFG_4_EXT	0x13A0
#define TX_PWR_CFG_5		0x1384
#define TX_PWR_CFG_6		0x1388
#define TX_PWR_CFG_7		0x13D4
#define TX_PWR_CFG_8		0x13D8
#define TX_PWR_CFG_9		0x13DC

#ifdef RT_BIG_ENDIAN
typedef	union _TX_PWR_CFG_STRUC {
	struct {
	    unsigned int Byte3:8;
	    unsigned int Byte2:8;
	    unsigned int Byte1:8;
	    unsigned int Byte0:8;
	} field;
	unsigned int word;
} TX_PWR_CFG_STRUC;
#else
typedef	union _TX_PWR_CFG_STRUC {
	struct {
	    unsigned int Byte0:8;
	    unsigned int Byte1:8;
	    unsigned int Byte2:8;
	    unsigned int Byte3:8;
	} field;
	unsigned int word;
} TX_PWR_CFG_STRUC;
#endif


#define TX_PIN_CFG		0x1328		 
#define TX_BAND_CFG	0x132c	/* 0x1 use upper 20MHz. 0 juse lower 20MHz */
#define TX_SW_CFG0		0x1330
#define TX_SW_CFG1		0x1334
#define TX_SW_CFG2		0x1338


#define TXOP_THRES_CFG	0x133c
#ifdef RT_BIG_ENDIAN
typedef union _TXOP_THRESHOLD_CFG_STRUC {
	struct {
		unsigned int	TXOP_REM_THRES:8; /* Remaining TXOP threshold (unit: 32us) */
		unsigned int	CF_END_THRES:8; /* CF-END threshold (unit: 32us) */
		unsigned int	RDG_IN_THRES:8; /* Rx RDG threshold (unit: 32us) */
		unsigned int	RDG_OUT_THRES:8; /* Tx RDG threshold (unit: 32us) */
	} field;
	unsigned int word;
} TXOP_THRESHOLD_CFG_STRUC;
#else
typedef union _TXOP_THRESHOLD_CFG_STRUC {
	struct {
		unsigned int	RDG_OUT_THRES:8;
		unsigned int	RDG_IN_THRES:8;
		unsigned int	CF_END_THRES:8;
		unsigned int	TXOP_REM_THRES:8;
	} field;
	unsigned int word;
} TXOP_THRESHOLD_CFG_STRUC;
#endif

#define TXOP_CTRL_CFG 0x1340


#define TX_RTS_CFG 0x1344
#ifdef RT_BIG_ENDIAN
typedef	union _TX_RTS_CFG_STRUC {
	struct {
	    unsigned int rsv:7;     
	    unsigned int RtsFbkEn:1;    /* enable rts rate fallback */
	    unsigned int RtsThres:16;    /* unit:byte */
	    unsigned int AutoRtsRetryLimit:8;
	} field;
	unsigned int word;
} TX_RTS_CFG_STRUC;
#else
typedef	union _TX_RTS_CFG_STRUC	 {
	struct {
	    unsigned int AutoRtsRetryLimit:8;
	    unsigned int RtsThres:16;
	    unsigned int RtsFbkEn:1;
	    unsigned int rsv:7;
	} field;
	unsigned int word;
} TX_RTS_CFG_STRUC;
#endif


#define TX_TXBF_CFG_0 0x138c
#define TX_TXBF_CFG_1 0x13A4
#define TX_TXBF_CFG_2 0x13A8
#define TX_TXBF_CFG_3 0x13AC
typedef	union _TX_TXBF_CFG_0_STRUC {
	struct {
#ifdef RT_BIG_ENDIAN
	    unsigned int       EtxbfFbkRate:16;     
	    unsigned int       EtxbfFbkEn:1;
	    unsigned int       EtxbfFbkSeqEn:1;
	    unsigned int       EtxbfFbkCoef:2;
	    unsigned int       EtxbfFbkCode:2;
	    unsigned int       EtxbfFbkNg:2;
	    unsigned int       CsdBypass:1;
	    unsigned int       EtxbfForce:1;
	    unsigned int       EtxbfEnable:1;
	    unsigned int       AutoTxbfEn:3;
	    unsigned int       ItxbfForce:1;
	    unsigned int       ItxbfEn:1;
#else
	    unsigned int       ItxbfEn:1;
	    unsigned int       ItxbfForce:1;
	    unsigned int       AutoTxbfEn:3;
	    unsigned int       EtxbfEnable:1;
	    unsigned int       EtxbfForce:1;
	    unsigned int       CsdBypass:1;
	    unsigned int       EtxbfFbkNg:2;
	    unsigned int       EtxbfFbkCode:2;
	    unsigned int       EtxbfFbkCoef:2;
	    unsigned int       EtxbfFbkSeqEn:1;
	    unsigned int       EtxbfFbkEn:1;
	    unsigned int       EtxbfFbkRate:16;     
#endif
	} field;
	unsigned int word;
} TX_TXBF_CFG_0_STRUC;


#define TX_TIMEOUT_CFG	0x1348
#ifdef RT_BIG_ENDIAN
typedef	union _TX_TIMEOUT_CFG_STRUC {
	struct {
	    unsigned int rsv2:8;     
	    unsigned int TxopTimeout:8;	/*TXOP timeout value for TXOP truncation.  It is recommended that (SLOT_TIME) > (TX_OP_TIMEOUT) > (RX_ACK_TIMEOUT) */
	    unsigned int RxAckTimeout:8;	/* unit:slot. Used for TX precedure */
	    unsigned int MpduLifeTime:4;    /*  expiration time = 2^(9+MPDU LIFE TIME)  us */
	    unsigned int rsv:4;     
	} field;
	unsigned int word;
} TX_TIMEOUT_CFG_STRUC;
#else
typedef	union _TX_TIMEOUT_CFG_STRUC {
	struct {
	    unsigned int rsv:4;     
	    unsigned int MpduLifeTime:4;
	    unsigned int RxAckTimeout:8;
	    unsigned int TxopTimeout:8;
	    unsigned int rsv2:8;
	} field;
	unsigned int word;
} TX_TIMEOUT_CFG_STRUC;
#endif


#define TX_RTY_CFG	0x134c
#ifdef RT_BIG_ENDIAN
typedef	union _TX_RTY_CFG_STRUC {
	struct {
	    unsigned int rsv:1;     
	    unsigned int TxautoFBEnable:1;    /* Tx retry PHY rate auto fallback enable */
	    unsigned int AggRtyMode:1;	/* Aggregate MPDU retry mode.  0:expired by retry limit, 1: expired by mpdu life timer */
	    unsigned int NonAggRtyMode:1;	/* Non-Aggregate MPDU retry mode.  0:expired by retry limit, 1: expired by mpdu life timer */
	    unsigned int LongRtyThre:12;	/* Long retry threshoold */
	    unsigned int LongRtyLimit:8;	/*long retry limit */
	    unsigned int ShortRtyLimit:8;	/*  short retry limit */
	} field;
	unsigned int word;
} TX_RTY_CFG_STRUC;
#else
typedef	union _TX_RTY_CFG_STRUC {
	struct {
	    unsigned int ShortRtyLimit:8;
	    unsigned int LongRtyLimit:8;
	    unsigned int LongRtyThre:12;
	    unsigned int NonAggRtyMode:1;
	    unsigned int AggRtyMode:1;
	    unsigned int TxautoFBEnable:1;
	    unsigned int rsv:1;
	} field;
	unsigned int word;
} TX_RTY_CFG_STRUC;
#endif


#define TX_LINK_CFG	0x1350
#ifdef RT_BIG_ENDIAN
typedef	union _TX_LINK_CFG_STRUC {
	struct {
	    unsigned int       RemotMFS:8;	/*remote MCS feedback sequence number */
	    unsigned int       RemotMFB:8;    /*  remote MCS feedback */
	    unsigned int       rsv:3;	/* */
	    unsigned int       TxCFAckEn:1;	/*   Piggyback CF-ACK enable */
	    unsigned int       TxRDGEn:1;	/* RDG TX enable */
	    unsigned int       TxMRQEn:1;	/*  MCS request TX enable */
	    unsigned int       RemoteUMFSEnable:1;	/*  remote unsolicit  MFB enable.  0: not apply remote remote unsolicit (MFS=7) */
	    unsigned int       MFBEnable:1;	/*  TX apply remote MFB 1:enable */
	    unsigned int       RemoteMFBLifeTime:8;	/*remote MFB life time. unit : 32us */
	} field;
	unsigned int word;
} TX_LINK_CFG_STRUC;
#else
typedef	union _TX_LINK_CFG_STRUC {
	struct {
	    unsigned int       RemoteMFBLifeTime:8;
	    unsigned int       MFBEnable:1;
	    unsigned int       RemoteUMFSEnable:1;
	    unsigned int       TxMRQEn:1;
	    unsigned int       TxRDGEn:1;
	    unsigned int       TxCFAckEn:1;
	    unsigned int       rsv:3;
	    unsigned int       RemotMFB:8;
	    unsigned int       RemotMFS:8;
	} field;
	unsigned int word;
} TX_LINK_CFG_STRUC;
#endif


#define HT_FBK_CFG0	0x1354
#ifdef RT_BIG_ENDIAN
typedef	union _HT_FBK_CFG0_STRUC {
	struct {
	    unsigned int HTMCS7FBK:4;
	    unsigned int HTMCS6FBK:4;
	    unsigned int HTMCS5FBK:4;
	    unsigned int HTMCS4FBK:4;
	    unsigned int HTMCS3FBK:4;
	    unsigned int HTMCS2FBK:4;
	    unsigned int HTMCS1FBK:4;
	    unsigned int HTMCS0FBK:4;
	} field;
	unsigned int word;
} HT_FBK_CFG0_STRUC;
#else
typedef	union _HT_FBK_CFG0_STRUC {
	struct {
	    unsigned int HTMCS0FBK:4;
	    unsigned int HTMCS1FBK:4;
	    unsigned int HTMCS2FBK:4;
	    unsigned int HTMCS3FBK:4;
	    unsigned int HTMCS4FBK:4;
	    unsigned int HTMCS5FBK:4;
	    unsigned int HTMCS6FBK:4;
	    unsigned int HTMCS7FBK:4;
	} field;
	unsigned int word;
} HT_FBK_CFG0_STRUC;
#endif


#define HT_FBK_CFG1	0x1358
#ifdef RT_BIG_ENDIAN
typedef	union _HT_FBK_CFG1_STRUC {
	struct {
	    unsigned int       HTMCS15FBK:4;
	    unsigned int       HTMCS14FBK:4;
	    unsigned int       HTMCS13FBK:4;
	    unsigned int       HTMCS12FBK:4;
	    unsigned int       HTMCS11FBK:4;
	    unsigned int       HTMCS10FBK:4;
	    unsigned int       HTMCS9FBK:4;
	    unsigned int       HTMCS8FBK:4;
	} field;
	unsigned int word;
} HT_FBK_CFG1_STRUC;
#else
typedef	union _HT_FBK_CFG1_STRUC {
	struct {
	    unsigned int       HTMCS8FBK:4;
	    unsigned int       HTMCS9FBK:4;
	    unsigned int       HTMCS10FBK:4;
	    unsigned int       HTMCS11FBK:4;
	    unsigned int       HTMCS12FBK:4;
	    unsigned int       HTMCS13FBK:4;
	    unsigned int       HTMCS14FBK:4;
	    unsigned int       HTMCS15FBK:4;
	} field;
	unsigned int word;
} HT_FBK_CFG1_STRUC;
#endif


#define LG_FBK_CFG0	0x135c
#ifdef RT_BIG_ENDIAN
typedef	union _LG_FBK_CFG0_STRUC {
	struct {
	    unsigned int       OFDMMCS7FBK:4;
	    unsigned int       OFDMMCS6FBK:4;
	    unsigned int       OFDMMCS5FBK:4
	    unsigned int       OFDMMCS4FBK:4;
	    unsigned int       OFDMMCS3FBK:4;
	    unsigned int       OFDMMCS2FBK:4;
	    unsigned int       OFDMMCS1FBK:4;
	    unsigned int       OFDMMCS0FBK:4;
	} field;
	unsigned int word;
} LG_FBK_CFG0_STRUC;
#else
typedef	union _LG_FBK_CFG0_STRUC {
	struct {
	    unsigned int       OFDMMCS0FBK:4;
	    unsigned int       OFDMMCS1FBK:4;
	    unsigned int       OFDMMCS2FBK:4;
	    unsigned int       OFDMMCS3FBK:4;
	    unsigned int       OFDMMCS4FBK:4;
	    unsigned int       OFDMMCS5FBK:4;
	    unsigned int       OFDMMCS6FBK:4;
	    unsigned int       OFDMMCS7FBK:4;
	} field;
	unsigned int word;
} LG_FBK_CFG0_STRUC;
#endif


#define LG_FBK_CFG1		0x1360
#ifdef RT_BIG_ENDIAN
typedef	union _LG_FBK_CFG1_STRUC {
	struct {
	    unsigned int       rsv:16;	 
	    unsigned int       CCKMCS3FBK:4;
	    unsigned int       CCKMCS2FBK:4;
	    unsigned int       CCKMCS1FBK:4;
	    unsigned int       CCKMCS0FBK:4;
	} field;
	unsigned int word;
}	LG_FBK_CFG1_STRUC;
#else
typedef	union _LG_FBK_CFG1_STRUC {
	struct {
	    unsigned int       CCKMCS0FBK:4;
	    unsigned int       CCKMCS1FBK:4;
	    unsigned int       CCKMCS2FBK:4;
	    unsigned int       CCKMCS3FBK:4;
	    unsigned int       rsv:16;
	}	field;
	unsigned int word;
}	LG_FBK_CFG1_STRUC;
#endif


/*======================================================= */
/*                                     Protection Paramater                                                         */
/*======================================================= */
#define ASIC_SHORTNAV	1
#define ASIC_LONGNAV	2
#define ASIC_RTS		1
#define ASIC_CTS		2

#define CCK_PROT_CFG	0x1364	/* CCK Protection */
#define OFDM_PROT_CFG	0x1368	/* OFDM Protection */
#define MM20_PROT_CFG	0x136C	/* MM20 Protection */
#define MM40_PROT_CFG	0x1370	/* MM40 Protection */
#define GF20_PROT_CFG	0x1374	/* GF20 Protection */
#define GF40_PROT_CFG	0x1378	/* GR40 Protection */
#ifdef RT_BIG_ENDIAN
typedef	union _PROT_CFG_STRUC {
	struct {
	    unsigned int       rsv:5;	 
	    unsigned int       RTSThEn:1;	/*RTS threshold enable on CCK TX */
	    unsigned int       TxopAllowGF40:1;	/*CCK TXOP allowance.0:disallow. */
	    unsigned int       TxopAllowGF20:1;	/*CCK TXOP allowance.0:disallow. */
	    unsigned int       TxopAllowMM40:1;	/*CCK TXOP allowance.0:disallow. */
	    unsigned int       TxopAllowMM20:1;	/*CCK TXOP allowance. 0:disallow. */
	    unsigned int       TxopAllowOfdm:1;	/*CCK TXOP allowance.0:disallow. */
	    unsigned int       TxopAllowCck:1;	/*CCK TXOP allowance.0:disallow. */
	    unsigned int       ProtectNav:2;	/*TXOP protection type for CCK TX. 0:None, 1:ShortNAVprotect,  2:LongNAVProtect, 3:rsv */
	    unsigned int       ProtectCtrl:2;	/*Protection control frame type for CCK TX. 1:RTS/CTS, 2:CTS-to-self, 0:None, 3:rsv */
	    unsigned int       ProtectRate:16;	/*Protection control frame rate for CCK TX(RTS/CTS/CFEnd). */
	} field;
	unsigned int word;
} PROT_CFG_STRUC;
#else
typedef	union _PROT_CFG_STRUC {
	struct {
	    unsigned int       ProtectRate:16;
	    unsigned int       ProtectCtrl:2;
	    unsigned int       ProtectNav:2;
	    unsigned int       TxopAllowCck:1;
	    unsigned int       TxopAllowOfdm:1;
	    unsigned int       TxopAllowMM20:1;
	    unsigned int       TxopAllowMM40:1;
	    unsigned int       TxopAllowGF20:1;
	    unsigned int       TxopAllowGF40:1;
	    unsigned int       RTSThEn:1;
	    unsigned int       rsv:5;	 
	} field;
	unsigned int word;
} PROT_CFG_STRUC;
#endif


#define EXP_CTS_TIME	0x137C
#define EXP_ACK_TIME	0x1380


#define HT_FBK_TO_LEGACY	0x1384


#ifdef DOT11N_SS3_SUPPORT
#define TX_FBK_CFG_3S_0	0x13c4
#ifdef RT_BIG_ENDIAN
typedef	union _TX_FBK_CFG_3S_0_STRUC {
	struct {
		unsigned int       rsv0:3;
		unsigned int       HTMCS19FBK:5;
		unsigned int       rsv1:3;
		unsigned int       HTMCS18FBK:5;
		unsigned int       rsv2:3;
		unsigned int       HTMCS17FBK:5;
		unsigned int       rsv3:3;
		unsigned int       HTMCS16FBK:5;
	} field;
	unsigned int word;
} TX_FBK_CFG_3S_0_STRUC;
#else
typedef	union _TX_FBK_CFG_3S_0_STRUC {
	struct {
		unsigned int       HTMCS16FBK:5;
		unsigned int       rsv3:3;
		unsigned int       HTMCS17FBK:5;
		unsigned int       rsv2:3;
		unsigned int       HTMCS18FBK:5;
		unsigned int       rsv1:3;
		unsigned int       HTMCS19FBK:5;
		unsigned int       rsv0:4;
	} field;
	unsigned int word;
} TX_FBK_CFG_3S_0_STRUC;
#endif

#define TX_FBK_CFG_3S_1	0x13c8
#ifdef RT_BIG_ENDIAN
typedef	union _TX_FBK_CFG_3S_1_STRUC {
	struct {
		unsigned int       rsv0:3;
		unsigned int       HTMCS23FBK:5;
		unsigned int       rsv1:3;
		unsigned int       HTMCS22FBK:5;
		unsigned int       rsv2:3;
		unsigned int       HTMCS21FBK:5;
		unsigned int       rsv3:3;
		unsigned int       HTMCS20FBK:5;
	} field;
	unsigned int word;
} TX_FBK_CFG_3S_1_STRUC;
#else
typedef	union _TX_FBK_CFG_3S_1_STRUC {
	struct {
		unsigned int       HTMCS20FBK:5;
		unsigned int       rsv3:3;
		unsigned int       HTMCS21FBK:5;
		unsigned int       rsv2:3;
		unsigned int       HTMCS22FBK:5;
		unsigned int       rsv1:3;
		unsigned int       HTMCS23FBK:5;
		unsigned int       rsv0:3;
	} field;
	unsigned int word;
} TX_FBK_CFG_3S_1_STRUC;
#endif
#endif /* DOT11N_SS3_SUPPORT */

#define TX_AC_RTY_LIMIT		0x13cc
#define TX_AC_FBK_SPEED	0x13d0



/*  4.4 MAC RX configuration registers (offset:0x1400) */

/* RX_FILTR_CFG:  /RX configuration register */
#define RX_FILTR_CFG	0x1400
#ifdef RT_BIG_ENDIAN
typedef	union _RX_FILTR_CFG_STRUC {
	struct {
		unsigned int rsv:15;		
		unsigned int DropRsvCntlType:1;
        	unsigned int DropBAR:1;
		unsigned int DropBA:1;	
		unsigned int DropPsPoll:1;		/* Drop Ps-Poll */
		unsigned int DropRts:1;		/* Drop Ps-Poll */
		unsigned int DropCts:1;		/* Drop Ps-Poll */
		unsigned int DropAck:1;		/* Drop Ps-Poll */
		unsigned int DropCFEnd:1;		/* Drop Ps-Poll */
		unsigned int DropCFEndAck:1;		/* Drop Ps-Poll */
		unsigned int DropDuplicate:1;		/* Drop duplicate frame */
		unsigned int DropBcast:1;		/* Drop broadcast frames */
		unsigned int DropMcast:1;		/* Drop multicast frames */
		unsigned int DropVerErr:1;	    /* Drop version error frame */
		unsigned int DropNotMyBSSID:1;			/* Drop fram ToDs bit is true */
		unsigned int DropNotToMe:1;		/* Drop not to me unicast frame */
		unsigned int DropPhyErr:1;		/* Drop physical error */
		unsigned int DropCRCErr:1;		/* Drop CRC error */
	} field;
	unsigned int word;
} RX_FILTR_CFG_STRUC;
#else
typedef	union _RX_FILTR_CFG_STRUC {
	struct {
		unsigned int DropCRCErr:1;
		unsigned int DropPhyErr:1;
		unsigned int DropNotToMe:1;
		unsigned int DropNotMyBSSID:1;
		unsigned int DropVerErr:1;
		unsigned int DropMcast:1;
		unsigned int DropBcast:1;
		unsigned int DropDuplicate:1;
		unsigned int DropCFEndAck:1;
		unsigned int DropCFEnd:1;
		unsigned int DropAck:1;
		unsigned int DropCts:1;
		unsigned int DropRts:1;
		unsigned int DropPsPoll:1;
		unsigned int DropBA:1;
        	unsigned int  DropBAR:1;
		unsigned int DropRsvCntlType:1;
		unsigned int rsv:15;
	} field;
	unsigned int word;
}	RX_FILTR_CFG_STRUC;
#endif


/* AUTO_RSP_CFG: Auto-Responder */
#define AUTO_RSP_CFG	0x1404
#ifdef RT_BIG_ENDIAN
typedef union _AUTO_RSP_CFG_STRUC {
	struct {
		unsigned int        :24;    
		unsigned int       AckCtsPsmBit:1;   /* Power bit value in conrtrol frame */
		unsigned int       DualCTSEn:1;   /* Power bit value in conrtrol frame */
		unsigned int       rsv:1;   /* Power bit value in conrtrol frame */
		unsigned int       AutoResponderPreamble:1;    /* 0:long, 1:short preamble */
		unsigned int       CTS40MRef:1;  /* Response CTS 40MHz duplicate mode */
		unsigned int       CTS40MMode:1;  /* Response CTS 40MHz duplicate mode */
		unsigned int       BACAckPolicyEnable:1;    /* 0:long, 1:short preamble */
		unsigned int       AutoResponderEnable:1;
	} field;
	unsigned int word;
} AUTO_RSP_CFG_STRUC;
#else
typedef union _AUTO_RSP_CFG_STRUC {
	struct {
		unsigned int       AutoResponderEnable:1;
		unsigned int       BACAckPolicyEnable:1;
		unsigned int       CTS40MMode:1;
		unsigned int       CTS40MRef:1;
		unsigned int       AutoResponderPreamble:1;
		unsigned int       rsv:1;
		unsigned int       DualCTSEn:1;
		unsigned int       AckCtsPsmBit:1;
		unsigned int        :24;    
	} field;
	unsigned int   word;
} AUTO_RSP_CFG_STRUC;
#endif


#define LEGACY_BASIC_RATE	0x1408
#define HT_BASIC_RATE		0x140c
#define HT_CTRL_CFG			0x1410
#define SIFS_COST_CFG		0x1414
#define RX_PARSER_CFG		0x1418	/*Set NAV for all received frames */


/*  4.5 MAC Security configuration (offset:0x1500) */
#define TX_SEC_CNT0		0x1500
#define RX_SEC_CNT0		0x1504
#define CCMP_FC_MUTE	0x1508


/*  4.6 HCCA/PSMP (offset:0x1600) */
#define TXOP_HLDR_ADDR0		0x1600		 
#define TXOP_HLDR_ADDR1		0x1604		 
#define TXOP_HLDR_ET			0x1608		 
#define QOS_CFPOLL_RA_DW0		0x160c
#define QOS_CFPOLL_A1_DW1		0x1610
#define QOS_CFPOLL_QC			0x1614


/*  4.7 MAC Statistis registers (offset:0x1700) */
/* RX_STA_CNT0_STRUC: RX PLCP error count & RX CRC error count */
#define RX_STA_CNT0		0x1700
#ifdef RT_BIG_ENDIAN
typedef	union _RX_STA_CNT0_STRUC {
	struct {
	    unsigned short  PhyErr;
	    unsigned short  CrcErr;
	} field;
	unsigned int word;
} RX_STA_CNT0_STRUC;
#else
typedef	union _RX_STA_CNT0_STRUC {
	struct {
	    unsigned short  CrcErr;
	    unsigned short  PhyErr;
	} field;
	unsigned int word;
} RX_STA_CNT0_STRUC;
#endif


/* RX_STA_CNT1_STRUC: RX False CCA count & RX LONG frame count */
#define RX_STA_CNT1		0x1704
#ifdef RT_BIG_ENDIAN
typedef	union _RX_STA_CNT1_STRUC {
	struct {
	    unsigned short  PlcpErr;
	    unsigned short  FalseCca;
	} field;
	unsigned int word;
} RX_STA_CNT1_STRUC;
#else
typedef	union _RX_STA_CNT1_STRUC {
	struct {
	    unsigned short  FalseCca;
	    unsigned short  PlcpErr;
	} field;
	unsigned int word;
} RX_STA_CNT1_STRUC;
#endif


/* RX_STA_CNT2_STRUC: */
#define RX_STA_CNT2		0x1708
#ifdef RT_BIG_ENDIAN
typedef	union _RX_STA_CNT2_STRUC {
	struct {
	    unsigned short  RxFifoOverflowCount;
	    unsigned short  RxDupliCount;
	} field;
	unsigned int word;
} RX_STA_CNT2_STRUC;
#else
typedef	union _RX_STA_CNT2_STRUC {
	struct {
	    unsigned short  RxDupliCount;
	    unsigned short  RxFifoOverflowCount;
	} field;
	unsigned int word;
} RX_STA_CNT2_STRUC;
#endif


/* STA_CSR3: TX Beacon count */
#define TX_STA_CNT0		0x170C
#ifdef RT_BIG_ENDIAN
typedef	union _TX_STA_CNT0_STRUC {
	struct {
	    unsigned short  TxBeaconCount;
	    unsigned short  TxFailCount;
	} field;
	unsigned int word;
} TX_STA_CNT0_STRUC;
#else
typedef	union _TX_STA_CNT0_STRUC {
	struct {
	    unsigned short  TxFailCount;
	    unsigned short  TxBeaconCount;
	} field;
	unsigned int word;
} TX_STA_CNT0_STRUC;
#endif



/* TX_STA_CNT1: TX tx count */
#define TX_STA_CNT1		0x1710
#ifdef RT_BIG_ENDIAN
typedef	union _TX_STA_CNT1_STRUC {
	struct {
	    unsigned short  TxRetransmit;
	    unsigned short  TxSuccess;
	} field;
	unsigned int word;
} TX_STA_CNT1_STRUC;
#else
typedef	union _TX_STA_CNT1_STRUC {
	struct {
	    unsigned short  TxSuccess;
	    unsigned short  TxRetransmit;
	} field;
	unsigned int word;
} TX_STA_CNT1_STRUC;
#endif


/* TX_STA_CNT2: TX tx count */
#define TX_STA_CNT2		0x1714
#ifdef RT_BIG_ENDIAN
typedef	union _TX_STA_CNT2_STRUC {
	struct {
	    unsigned short  TxUnderFlowCount;
	    unsigned short  TxZeroLenCount;
	} field;
	unsigned int word;
} TX_STA_CNT2_STRUC;
#else
typedef	union _TX_STA_CNT2_STRUC {
	struct {
	    unsigned short  TxZeroLenCount;
	    unsigned short  TxUnderFlowCount;
	} field;
	unsigned int word;
} TX_STA_CNT2_STRUC;
#endif


/* TX_STA_FIFO_STRUC: TX Result for specific PID status fifo register */
#define TX_STA_FIFO		0x1718
#ifdef RT_BIG_ENDIAN
typedef	union _TX_STA_FIFO_STRUC {
	struct {
		unsigned int		Reserve:2;
		unsigned int		iTxBF:1; /* iTxBF enable */
		unsigned int		Sounding:1; /* Sounding enable */
		unsigned int		eTxBF:1; /* eTxBF enable */
		unsigned int		SuccessRate:11;	/*include MCS, mode ,shortGI, BW settingSame format as TXWI Word 0 Bit 31-16. */
		unsigned int		wcid:8;		/*wireless client index */
		unsigned int       	TxAckRequired:1;    /* ack required */
		unsigned int       	TxAggre:1;    /* Tx is aggregated */
		unsigned int       	TxSuccess:1;   /* Tx success. whether success or not */
		unsigned int       	PidType:4;
		unsigned int       	bValid:1;   /* 1:This register contains a valid TX result */
	} field;
	unsigned int word;
} TX_STA_FIFO_STRUC;
#else
typedef	union _TX_STA_FIFO_STRUC {
	struct {
		unsigned int       	bValid:1;
		unsigned int       	PidType:4;
		unsigned int       	TxSuccess:1;
		unsigned int       	TxAggre:1;
		unsigned int       	TxAckRequired:1;
		unsigned int		wcid:8;
		unsigned int		SuccessRate:11;
		unsigned int		eTxBF:1;
		unsigned int		Sounding:1;
		unsigned int		iTxBF:1;
		unsigned int		Reserve:2;
	} field;
	unsigned int word;
} TX_STA_FIFO_STRUC;
#endif


/* 
	Debug counters
*/
#define TX_AGG_CNT		0x171c
#ifdef RT_BIG_ENDIAN
typedef	union _TX_NAG_AGG_CNT_STRUC {
	struct {
	    unsigned short  AggTxCount;
	    unsigned short  NonAggTxCount;
	} field;
	unsigned int word;
} TX_NAG_AGG_CNT_STRUC;
#else
typedef	union _TX_NAG_AGG_CNT_STRUC {
	struct {
	    unsigned short  NonAggTxCount;
	    unsigned short  AggTxCount;
	} field;
	unsigned int word;
} TX_NAG_AGG_CNT_STRUC;
#endif


#define TX_AGG_CNT0	0x1720
#ifdef RT_BIG_ENDIAN
typedef	union _TX_AGG_CNT0_STRUC {
	struct {
	    unsigned short  AggSize2Count;
	    unsigned short  AggSize1Count;
	} field;
	unsigned int word;
} TX_AGG_CNT0_STRUC;
#else
typedef	union _TX_AGG_CNT0_STRUC {
	struct {
	    unsigned short  AggSize1Count;
	    unsigned short  AggSize2Count;
	} field;
	unsigned int word;
} TX_AGG_CNT0_STRUC;
#endif


#define TX_AGG_CNT1	0x1724
#ifdef RT_BIG_ENDIAN
typedef	union _TX_AGG_CNT1_STRUC {
	struct {
	    unsigned short  AggSize4Count;
	    unsigned short  AggSize3Count;
	} field;
	unsigned int word;
} TX_AGG_CNT1_STRUC;
#else
typedef	union _TX_AGG_CNT1_STRUC {
	struct {
	    unsigned short  AggSize3Count;
	    unsigned short  AggSize4Count;
	} field;
	unsigned int word;
} TX_AGG_CNT1_STRUC;
#endif


#define TX_AGG_CNT2	0x1728
#ifdef RT_BIG_ENDIAN
typedef	union _TX_AGG_CNT2_STRUC {
	struct {
	    unsigned short  AggSize6Count;
	    unsigned short  AggSize5Count;
	} field;
	unsigned int word;
} TX_AGG_CNT2_STRUC;
#else
typedef	union _TX_AGG_CNT2_STRUC {
	struct {
	    unsigned short  AggSize5Count;
	    unsigned short  AggSize6Count;
	} field;
	unsigned int word;
} TX_AGG_CNT2_STRUC;
#endif


#define TX_AGG_CNT3	0x172c
#ifdef RT_BIG_ENDIAN
typedef	union _TX_AGG_CNT3_STRUC {
	struct {
	    unsigned short  AggSize8Count;
	    unsigned short  AggSize7Count;
	} field;
	unsigned int word;
} TX_AGG_CNT3_STRUC;
#else
typedef	union _TX_AGG_CNT3_STRUC {
	struct {
	    unsigned short  AggSize7Count;
	    unsigned short  AggSize8Count;
	} field;
	unsigned int word;
} TX_AGG_CNT3_STRUC;
#endif


#define TX_AGG_CNT4	0x1730
#ifdef RT_BIG_ENDIAN
typedef	union _TX_AGG_CNT4_STRUC {
	struct {
	    unsigned short  AggSize10Count;
	    unsigned short  AggSize9Count;
	} field;
	unsigned int word;
} TX_AGG_CNT4_STRUC;
#else
typedef	union _TX_AGG_CNT4_STRUC {
	struct {
	    unsigned short  AggSize9Count;
	    unsigned short  AggSize10Count;
	} field;
	unsigned int word;
} TX_AGG_CNT4_STRUC;
#endif


#define TX_AGG_CNT5	0x1734
#ifdef RT_BIG_ENDIAN
typedef	union _TX_AGG_CNT5_STRUC {
	struct {
	    unsigned short  AggSize12Count;
	    unsigned short  AggSize11Count;
	} field;
	unsigned int word;
} TX_AGG_CNT5_STRUC;
#else
typedef	union _TX_AGG_CNT5_STRUC {
	struct {
	    unsigned short  AggSize11Count;
	    unsigned short  AggSize12Count;
	} field;
	unsigned int word;
} TX_AGG_CNT5_STRUC;
#endif


#define TX_AGG_CNT6		0x1738
#ifdef RT_BIG_ENDIAN
typedef	union _TX_AGG_CNT6_STRUC {
	struct {
	    unsigned short  AggSize14Count;
	    unsigned short  AggSize13Count;
	} field;
	unsigned int word;
} TX_AGG_CNT6_STRUC;
#else
typedef	union _TX_AGG_CNT6_STRUC {
	struct {
	    unsigned short  AggSize13Count;
	    unsigned short  AggSize14Count;
	} field;
	unsigned int word;
} TX_AGG_CNT6_STRUC;
#endif


#define TX_AGG_CNT7		0x173c
#ifdef RT_BIG_ENDIAN
typedef	union _TX_AGG_CNT7_STRUC {
	struct {
	    unsigned short  AggSize16Count;
	    unsigned short  AggSize15Count;
	} field;
	unsigned int word;
} TX_AGG_CNT7_STRUC;
#else
typedef	union _TX_AGG_CNT7_STRUC {
	struct {
	    unsigned short  AggSize15Count;
	    unsigned short  AggSize16Count;
	} field;
	unsigned int word;
} TX_AGG_CNT7_STRUC;
#endif


#define TX_AGG_CNT8	0x174c	/* AGG_SIZE  = 17,18 */
#define TX_AGG_CNT9	0x1750	/* AGG_SIZE  = 19,20 */
#define TX_AGG_CNT10	0x1754	/* AGG_SIZE  = 21,22 */
#define TX_AGG_CNT11	0x1758	/* AGG_SIZE  = 23,24 */
#define TX_AGG_CNT12	0x175c	/* AGG_SIZE  = 25,26 */
#define TX_AGG_CNT13	0x1760	/* AGG_SIZE  = 27,28 */
#define TX_AGG_CNT14	0x1764	/* AGG_SIZE  = 29,30 */
#define TX_AGG_CNT15	0x1768	/* AGG_SIZE  = 31,32 */
#ifdef RT_BIG_ENDIAN
typedef	union _TX_AGG_CNT_STRUC {
	struct {
	    unsigned short  AggCnt_y;	/* the count of aggregation size = x + 1 */
	    unsigned short  AggCnt_x;	/* the count of aggregation size = x */
	} field;
	unsigned int word;
} TX_AGG_CNT_STRUC;
#else
typedef	union _TX_AGG_CNT_STRUC {
	struct {
	    unsigned short  AggCnt_x;
	    unsigned short  AggCnt_y;
	} field;
	unsigned int word;
} TX_AGG_CNT_STRUC;
#endif

typedef	union _TX_AGG_CNTN_STRUC {
	struct {
#ifdef RT_BIG_ENDIAN
	    unsigned short  AggSizeHighCount;
	    unsigned short  AggSizeLowCount;
#else
	    unsigned short  AggSizeLowCount;
	    unsigned short  AggSizeHighCount;
#endif
	} field;
	unsigned int word;
} TX_AGG_CNTN_STRUC;


#define MPDU_DENSITY_CNT		0x1740
#ifdef RT_BIG_ENDIAN
typedef	union _MPDU_DEN_CNT_STRUC {
	struct {
	    unsigned short  RXZeroDelCount;	/*RX zero length delimiter count */
	    unsigned short  TXZeroDelCount;	/*TX zero length delimiter count */
	} field;
	unsigned int word;
} MPDU_DEN_CNT_STRUC;
#else
typedef	union _MPDU_DEN_CNT_STRUC {
	struct {
	    unsigned short  TXZeroDelCount;
	    unsigned short  RXZeroDelCount;
	} field;
	unsigned int word;
} MPDU_DEN_CNT_STRUC;
#endif


#ifdef FIFO_EXT_SUPPORT
/* TX_STA_FIFO_EXT_STRUC: TX retry cnt for specific frame */
#define TX_STA_FIFO_EXT		0x1798		/* Only work after RT53xx */
#ifdef RT_BIG_ENDIAN
typedef	union _TX_STA_FIFO_EXT_STRUC {
	struct {
		unsigned int		Reserve:24;
		unsigned int		txRtyCnt:8;   /* frame Tx retry cnt */
	} field;
	unsigned int word;
} TX_STA_FIFO_EXT_STRUC;
#else
typedef	union _TX_STA_FIFO_EXT_STRUC {
	struct {
		unsigned int		txRtyCnt:8;
		unsigned int		Reserve:24;
	} field;
	unsigned int word;
} TX_STA_FIFO_EXT_STRUC;
#endif



#define WCID_TX_CNT_0	0x176c
#define WCID_TX_CNT_1	0x1770
#define WCID_TX_CNT_2	0x1774
#define WCID_TX_CNT_3	0x1778
#define WCID_TX_CNT_4	0x177c
#define WCID_TX_CNT_5	0x1780
#define WCID_TX_CNT_6	0x1784
#define WCID_TX_CNT_7	0x1788
#ifdef RT_BIG_ENDIAN
typedef	union _WCID_TX_CNT_STRUC {
	struct {
		unsigned int		reTryCnt:16;
		unsigned int		succCnt:16;
	} field;
	unsigned int word;
} WCID_TX_CNT_STRUC;
#else
typedef	union _WCID_TX_CNT_STRUC {
	struct {
		unsigned int		succCnt:16;
		unsigned int		reTryCnt:16;
	} field;
	unsigned int word;
} WCID_TX_CNT_STRUC;
#endif


#define WCID_MAPPING_0	0x178c
#define WCID_MAPPING_1	0x1790
#ifdef RT_BIG_ENDIAN
typedef	union _WCID_MAPPING_STRUC {
	struct {
		unsigned int		wcid3:8;
		unsigned int		wcid2:8;
		unsigned int		wcid1:8;
		unsigned int		wcid0:8;
	} field;
	unsigned int word;
} WCID_MAPPING_STRUC;
#else
typedef	union _WCID_MAPPING_STRUC {
	struct {
		unsigned int		wcid0:8;
		unsigned int		wcid1:8;
		unsigned int		wcid2:8;
		unsigned int		wcid3:8;
	} field;
	unsigned int word;
} WCID_MAPPINGT_STRUC;
#endif
#endif /* FIFO_EXT_SUPPORT */



/* Security key table memory, base address = 0x1000 */
#define MAC_WCID_BASE		0x1800 /*8-bytes(use only 6-bytes) * 256 entry = */
#define HW_WCID_ENTRY_SIZE   8


#ifdef RT_BIG_ENDIAN
typedef	union _SHAREDKEY_MODE_STRUC {
	struct {
		unsigned int       Bss1Key3CipherAlg:4;
		unsigned int       Bss1Key2CipherAlg:4;
		unsigned int       Bss1Key1CipherAlg:4;
		unsigned int       Bss1Key0CipherAlg:4;
		unsigned int       Bss0Key3CipherAlg:4;
		unsigned int       Bss0Key2CipherAlg:4;
		unsigned int       Bss0Key1CipherAlg:4;
		unsigned int       Bss0Key0CipherAlg:4;
	} field;
	unsigned int word;
} SHAREDKEY_MODE_STRUC;
#else
typedef	union _SHAREDKEY_MODE_STRUC {
	struct {
		unsigned int       Bss0Key0CipherAlg:4;
		unsigned int       Bss0Key1CipherAlg:4;
		unsigned int       Bss0Key2CipherAlg:4;
		unsigned int       Bss0Key3CipherAlg:4;
		unsigned int       Bss1Key0CipherAlg:4;
		unsigned int       Bss1Key1CipherAlg:4;
		unsigned int       Bss1Key2CipherAlg:4;
		unsigned int       Bss1Key3CipherAlg:4;
	} field;
	unsigned int word;
} SHAREDKEY_MODE_STRUC;
#endif


/* 64-entry for pairwise key table, 8-byte per entry  */
typedef struct _HW_WCID_ENTRY {
    unsigned char   Address[6];
    unsigned char   Rsv[2];
} HW_WCID_ENTRY;


/* ================================================================================= */
/* WCID  format */
/* ================================================================================= */
/*7.1	WCID  ENTRY  format  : 8bytes */
typedef	struct _WCID_ENTRY_STRUC {
	unsigned char		RXBABitmap7;    /* bit0 for TID8, bit7 for TID 15 */
	unsigned char		RXBABitmap0;    /* bit0 for TID0, bit7 for TID 7 */
	unsigned char		MAC[6];	/* 0 for shared key table.  1 for pairwise key table */
} WCID_ENTRY_STRUC;


/*8.1.1	SECURITY  KEY  format  : 8DW */
/* 32-byte per entry, total 16-entry for shared key table, 64-entry for pairwise key table */
typedef struct _HW_KEY_ENTRY {
    unsigned char   Key[16];
    unsigned char   TxMic[8];
    unsigned char   RxMic[8];
} HW_KEY_ENTRY;


/*8.1.2	IV/EIV  format  : 2DW */

/* RX attribute entry format  : 1DW */
#ifdef RT_BIG_ENDIAN
typedef	union _WCID_ATTRIBUTE_STRUC {
	struct {
		unsigned int		WAPIKeyIdx:8;
		unsigned int		WAPI_rsv:8;
		unsigned int		WAPI_MCBC:1;
		unsigned int		rsv:3;
		unsigned int		BSSIdxExt:1;
		unsigned int		PairKeyModeExt:1;
		unsigned int		RXWIUDF:3;
		unsigned int		BSSIdx:3; /*multipleBSS index for the WCID */
		unsigned int		PairKeyMode:3;
		unsigned int		KeyTab:1;	/* 0 for shared key table.  1 for pairwise key table */
	} field;
	unsigned int word;
} WCID_ATTRIBUTE_STRUC;
#else
typedef	union _WCID_ATTRIBUTE_STRUC {
	struct {
		unsigned int		KeyTab:1;	/* 0 for shared key table.  1 for pairwise key table */
		unsigned int		PairKeyMode:3;
		unsigned int		BSSIdx:3; 		/*multipleBSS index for the WCID */
		unsigned int		RXWIUDF:3;
		unsigned int		PairKeyModeExt:1;
		unsigned int		BSSIdxExt:1;
		unsigned int		rsv:3;
		unsigned int		WAPI_MCBC:1;
		unsigned int		WAPI_rsv:8;
		unsigned int		WAPIKeyIdx:8;
	} field;
	unsigned int word;
} WCID_ATTRIBUTE_STRUC;
#endif


/* ================================================================================= */
/* HOST-MCU communication data structure */
/* ================================================================================= */

/* H2M_MAILBOX_CSR: Host-to-MCU Mailbox */
#ifdef RT_BIG_ENDIAN
typedef union _H2M_MAILBOX_STRUC {
    struct {
        unsigned int       Owner:8;
        unsigned int       CmdToken:8;    /* 0xff tells MCU not to report CmdDoneInt after excuting the command */
        unsigned int       HighByte:8;
        unsigned int       LowByte:8;
    } field;
    unsigned int word;
} H2M_MAILBOX_STRUC;
#else
typedef union _H2M_MAILBOX_STRUC {
    struct {
        unsigned int       LowByte:8;
        unsigned int       HighByte:8;
        unsigned int       CmdToken:8;
        unsigned int       Owner:8;
    } field;
    unsigned int word;
} H2M_MAILBOX_STRUC;
#endif


/* M2H_CMD_DONE_CSR: MCU-to-Host command complete indication */
#ifdef RT_BIG_ENDIAN
typedef union _M2H_CMD_DONE_STRUC {
    struct {
        unsigned int       CmdToken3;
        unsigned int       CmdToken2;
        unsigned int       CmdToken1;
        unsigned int       CmdToken0;
    } field;
    unsigned int word;
} M2H_CMD_DONE_STRUC;
#else
typedef union _M2H_CMD_DONE_STRUC {
    struct {
        unsigned int       CmdToken0;
        unsigned int       CmdToken1;
        unsigned int       CmdToken2;
        unsigned int       CmdToken3;
    } field;
    unsigned int word;
} M2H_CMD_DONE_STRUC;
#endif


/* HOST_CMD_CSR: For HOST to interrupt embedded processor */
#ifdef RT_BIG_ENDIAN
typedef	union _HOST_CMD_CSR_STRUC {
	struct {
	    unsigned int   Rsv:24;
	    unsigned int   HostCommand:8;
	} field;
	unsigned int word;
} HOST_CMD_CSR_STRUC;
#else
typedef	union _HOST_CMD_CSR_STRUC {
	struct {
	    unsigned int   HostCommand:8;
	    unsigned int   Rsv:24;
	} field;
	unsigned int word;
} HOST_CMD_CSR_STRUC;
#endif


// TODO: shiang-6590, Need to check following definitions are useful or not!!!
/* AIFSN_CSR: AIFSN for each EDCA AC */


/* E2PROM_CSR: EEPROM control register */
#ifdef RT_BIG_ENDIAN
typedef	union _E2PROM_CSR_STRUC {
	struct {
		unsigned int		Rsvd:25;
		unsigned int       LoadStatus:1;   /* 1:loading, 0:done */
		unsigned int		Type:1;			/* 1: 93C46, 0:93C66 */
		unsigned int		EepromDO:1;
		unsigned int		EepromDI:1;
		unsigned int		EepromCS:1;
		unsigned int		EepromSK:1;
		unsigned int		Reload:1;		/* Reload EEPROM content, write one to reload, self-cleared. */
	} field;
	unsigned int word;
} E2PROM_CSR_STRUC;
#else
typedef	union _E2PROM_CSR_STRUC {
	struct {
		unsigned int		Reload:1;
		unsigned int		EepromSK:1;
		unsigned int		EepromCS:1;
		unsigned int		EepromDI:1;
		unsigned int		EepromDO:1;
		unsigned int		Type:1;
		unsigned int       LoadStatus:1;
		unsigned int		Rsvd:25;
	} field;
	unsigned int word;
} E2PROM_CSR_STRUC;
#endif


/* QOS_CSR0: TXOP holder address0 register */
#ifdef RT_BIG_ENDIAN
typedef	union _QOS_CSR0_STRUC {
	struct {
		unsigned char		Byte3;		/* MAC address byte 3 */
		unsigned char		Byte2;		/* MAC address byte 2 */
		unsigned char		Byte1;		/* MAC address byte 1 */
		unsigned char		Byte0;		/* MAC address byte 0 */
	} field;
	unsigned int word;
} QOS_CSR0_STRUC;
#else
typedef	union _QOS_CSR0_STRUC {
	struct {
		unsigned char		Byte0;
		unsigned char		Byte1;
		unsigned char		Byte2;
		unsigned char		Byte3;
	} field;
	unsigned int word;
} QOS_CSR0_STRUC;
#endif


/* QOS_CSR1: TXOP holder address1 register */
#ifdef RT_BIG_ENDIAN
typedef	union _QOS_CSR1_STRUC {
	struct {
		unsigned char		Rsvd1;
		unsigned char		Rsvd0;
		unsigned char		Byte5;		/* MAC address byte 5 */
		unsigned char		Byte4;		/* MAC address byte 4 */
	} field;
	unsigned int word;
} QOS_CSR1_STRUC;
#else
typedef	union _QOS_CSR1_STRUC {
	struct {
		unsigned char		Byte4;		/* MAC address byte 4 */
		unsigned char		Byte5;		/* MAC address byte 5 */
		unsigned char		Rsvd0;
		unsigned char		Rsvd1;
	} field;
	unsigned int word;
} QOS_CSR1_STRUC;
#endif

// TODO: shiang-6590, check upper definitions are useful or not!



/* Other on-chip shared memory space, base = 0x2000 */

/* CIS space - base address = 0x2000 */
#define HW_CIS_BASE             0x2000

/* Carrier-sense CTS frame base address. It's where mac stores carrier-sense frame for carrier-sense function. */
#define HW_CS_CTS_BASE			0x7700
/* DFS CTS frame base address. It's where mac stores CTS frame for DFS. */
#define HW_DFS_CTS_BASE			0x7780
#define HW_CTS_FRAME_SIZE		0x80

/* 2004-11-08 john - since NULL frame won't be that long (256 byte). We steal 16 tail bytes */
/* to save debugging settings */
#define HW_DEBUG_SETTING_BASE   0x77f0  /* 0x77f0~0x77ff total 16 bytes */
#define HW_DEBUG_SETTING_BASE2  0x7770  /* 0x77f0~0x77ff total 16 bytes */

#ifdef WOW_SUPPORT
/* WOW - NullFrame buffer */
#define HW_NULL2_BASE			0x7780
#define GPIO_HOLDTIME_OFFSET    0x7020 /* Target is 0x7023 */
#endif /* WOW_SUPPORT */

/* 
	On-chip BEACON frame space -
   	1. HW_BEACON_OFFSET/64B must be 0;
   	2. BCN_OFFSETx(0~) must also be changed in MACRegTable(common/rtmp_init.c)
 */
#define HW_BEACON_OFFSET		0x0200 


/* 	In order to support maximum 8 MBSS and its maximum length is 512 for each beacon
	Three section discontinue memory segments will be used.
	1. The original region for BCN 0~3
	2. Extract memory from FCE table for BCN 4~5
	3. Extract memory from Pair-wise key table for BCN 6~7
  		It occupied those memory of wcid 238~253 for BCN 6 
		and wcid 222~237 for BCN 7  	*/
/*#define HW_BEACON_MAX_COUNT     8 */
#define HW_BEACON_MAX_SIZE(__pAd)      ((__pAd)->chipCap.BcnMaxHwSize)
#define HW_BEACON_BASE0(__pAd)         ((__pAd)->chipCap.BcnBase[0])
/*#define HW_BEACON_BASE1         0x7A00 */
/*#define HW_BEACON_BASE2         0x7C00 */
/*#define HW_BEACON_BASE3         0x7E00 */
/*#define HW_BEACON_BASE4         0x7200 */
/*#define HW_BEACON_BASE5         0x7400 */
/*#define HW_BEACON_BASE6         0x5DC0 */
/*#define HW_BEACON_BASE7         0x5BC0 */


/* Higher 8KB shared memory */
#define HW_BEACON_BASE0_REDIRECTION	0x4000
#define HW_BEACON_BASE1_REDIRECTION	0x4200
#define HW_BEACON_BASE2_REDIRECTION	0x4400
#define HW_BEACON_BASE3_REDIRECTION	0x4600
#define HW_BEACON_BASE4_REDIRECTION	0x4800
#define HW_BEACON_BASE5_REDIRECTION	0x4A00
#define HW_BEACON_BASE6_REDIRECTION	0x4C00
#define HW_BEACON_BASE7_REDIRECTION	0x4E00


/* HOST-MCU shared memory - base address = 0x2100 */
#define HOST_CMD_CSR		0x404
#define H2M_MAILBOX_CSR         0x7010
#define H2M_MAILBOX_CID         0x7014
#define H2M_MAILBOX_STATUS      0x701c
#define H2M_INT_SRC             0x7024
#define H2M_BBP_AGENT           0x7028
#define M2H_CMD_DONE_CSR        0x000c
#define MCU_TXOP_ARRAY_BASE     0x000c   /* TODO: to be provided by Albert */
#define MCU_TXOP_ENTRY_SIZE     32       /* TODO: to be provided by Albert */
#define MAX_NUM_OF_TXOP_ENTRY   16       /* TODO: must be same with 8051 firmware */
#define MCU_MBOX_VERSION        0x01     /* TODO: to be confirmed by Albert */
#define MCU_MBOX_VERSION_OFFSET 5        /* TODO: to be provided by Albert */


/* Host DMA registers - base address 0x200 .  TX0-3=EDCAQid0-3, TX4=HCCA, TX5=MGMT, */
/*  DMA RING DESCRIPTOR */
#define E2PROM_CSR          0x0004
#define IO_CNTL_CSR         0x77d0



/* ================================================================ */
/* Tx /	Rx / Mgmt ring descriptor definition */
/* ================================================================ */

/* the following PID values are used to mark outgoing frame type in TXD->PID so that */
/* proper TX statistics can be collected based on these categories */
/* b3-2 of PID field - */
#define PID_MGMT			0x05
#define PID_BEACON			0x0c
#define PID_DATA_NORMALUCAST	 	0x02
#define PID_DATA_AMPDU	 	0x04
#define PID_DATA_NO_ACK    	0x08
#define PID_DATA_NOT_NORM_ACK	 	0x03
/* value domain of pTxD->HostQId (4-bit: 0~15) */
#define QID_AC_BK               1   /* meet ACI definition in 802.11e */
#define QID_AC_BE               0   /* meet ACI definition in 802.11e */
#define QID_AC_VI               2
#define QID_AC_VO               3
#define QID_HCCA                4
#define NUM_OF_TX_RING          5
#define QID_CTRL	            9
#define QID_MGMT                13
#define QID_RX                  14
#define QID_OTHER               15





#define RTMP_MAC_SHR_MSEL_PROTECT_LOCK(__pAd, __IrqFlags)	__IrqFlags = __IrqFlags;
#define RTMP_MAC_SHR_MSEL_PROTECT_UNLOCK(__pAd, __IrqFlags) __IrqFlags = __IrqFlags;


#ifdef RTMP_MAC_USB
#ifdef DFS_SUPPORT
#define BBPR127TABLE_OWNERID			0x4CA0 
#define BBPR127TABLE_OFFSET				0x4D00
#endif /* DFS_SUPPORT */
#endif /* RTMP_MAC_USB */


struct _RTMP_ADAPTER;

INT get_pkt_phymode_by_rxwi(RXWI_STRUC *rxwi);
INT get_pkt_rssi_by_rxwi(struct _RTMP_ADAPTER *pAd, RXWI_STRUC *rxwi, INT size, CHAR *rssi);
INT get_pkt_snr_by_rxwi(struct _RTMP_ADAPTER *pAd, RXWI_STRUC *rxwi, INT size, unsigned char *snr);

INT rtmp_mac_set_band(struct _RTMP_ADAPTER *pAd, int  band);
INT rtmp_mac_set_ctrlch(struct _RTMP_ADAPTER *pAd, INT extch);
INT rtmp_mac_set_mmps(struct  _RTMP_ADAPTER *pAd, INT ReduceCorePower);

#endif /* __RTMP_MAC_H__ */

