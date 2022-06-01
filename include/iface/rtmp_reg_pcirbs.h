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
	rtmp_reg_pcirbus.h
 
    Abstract:
 
    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
*/

#ifndef __RTMP_REG_PCIRBUS_H__
#define __RTMP_REG_PCIRBUS_H__


// TODO: shiang, for RT3290, make sure following definition is correct to put as here

#define ASIC_VERSION	0x0000

#define CMB_CTRL		0x20
#ifdef RT_BIG_ENDIAN
typedef union _CMB_CTRL_STRUC{
	struct{
		unsigned int       	LDO0_EN:1;
		unsigned int       	LDO3_EN:1;
		unsigned int       	LDO_BGSEL:2;
		unsigned int       	LDO_CORE_LEVEL:4;
		unsigned int       	PLL_LD:1;
		unsigned int       	XTAL_RDY:1;
		unsigned int       	Rsv:2;
		unsigned int		LDO25_FRC_ON:1;//4      
		unsigned int		LDO25_LARGEA:1;
		unsigned int		LDO25_LEVEL:2;
		unsigned int		AUX_OPT_Bit15_Two_AntennaMode:1;
		unsigned int		AUX_OPT_Bit14_TRSW1_as_GPIO:1;
		unsigned int		AUX_OPT_Bit13_GPIO7_as_GPIO:1;
		unsigned int		AUX_OPT_Bit12_TRSW0_as_WLAN_ANT_SEL:1;
		unsigned int		AUX_OPT_Bit11_Rsv:1;
		unsigned int		AUX_OPT_Bit10_NotSwap_WL_LED_ACT_RDY:1;
		unsigned int		AUX_OPT_Bit9_GPIO3_as_GPIO:1;
		unsigned int		AUX_OPT_Bit8_AuxPower_Exists:1;
		unsigned int		AUX_OPT_Bit7_KeepInterfaceClk:1;
		unsigned int		AUX_OPT_Bit6_KeepXtal_On:1;
		unsigned int		AUX_OPT_Bit5_RemovePCIePhyClk_BTOff:1;
		unsigned int		AUX_OPT_Bit4_RemovePCIePhyClk_WLANOff:1;
		unsigned int		AUX_OPT_Bit3_PLLOn_L1:1;
		unsigned int		AUX_OPT_Bit2_PCIeCoreClkOn_L1:1;
		unsigned int		AUX_OPT_Bit1_PCIePhyClkOn_L1:1;	
		unsigned int		AUX_OPT_Bit0_InterfaceClk_40Mhz:1;
	}field;
	unsigned int word;
}CMB_CTRL_STRUC, *PCMB_CTRL_STRUC;
#else
typedef union _CMB_CTRL_STRUC{
	struct{
		unsigned int		AUX_OPT_Bit0_InterfaceClk_40Mhz:1;
		unsigned int		AUX_OPT_Bit1_PCIePhyClkOn_L1:1;	
		unsigned int		AUX_OPT_Bit2_PCIeCoreClkOn_L1:1;
		unsigned int		AUX_OPT_Bit3_PLLOn_L1:1;
		unsigned int		AUX_OPT_Bit4_RemovePCIePhyClk_WLANOff:1;
		unsigned int		AUX_OPT_Bit5_RemovePCIePhyClk_BTOff:1;
		unsigned int		AUX_OPT_Bit6_KeepXtal_On:1;
		unsigned int		AUX_OPT_Bit7_KeepInterfaceClk:1;
		unsigned int		AUX_OPT_Bit8_AuxPower_Exists:1;
		unsigned int		AUX_OPT_Bit9_GPIO3_as_GPIO:1;
		unsigned int		AUX_OPT_Bit10_NotSwap_WL_LED_ACT_RDY:1;	
		unsigned int		AUX_OPT_Bit11_Rsv:1;
		unsigned int		AUX_OPT_Bit12_TRSW0_as_WLAN_ANT_SEL:1;
		unsigned int		AUX_OPT_Bit13_GPIO7_as_GPIO:1;
		unsigned int		AUX_OPT_Bit14_TRSW1_as_GPIO:1;
		unsigned int		AUX_OPT_Bit15_Two_AntennaMode:1;
		unsigned int		LDO25_LEVEL:2;
		unsigned int		LDO25_LARGEA:1;
		unsigned int		LDO25_FRC_ON:1;//4      
		unsigned int       	Rsv:2;
		unsigned int       	XTAL_RDY:1;
		unsigned int       	PLL_LD:1;
		unsigned int       	LDO_CORE_LEVEL:4;
		unsigned int       	LDO_BGSEL:2;
		unsigned int       	LDO3_EN:1;
		unsigned int       	LDO0_EN:1;
	}field;
	unsigned int word;
}CMB_CTRL_STRUC, *PCMB_CTRL_STRUC;
#endif


#define OSCCTL				0x38
#ifdef RT_BIG_ENDIAN
typedef	union _OSCCTL_STRUC{
	struct{
		unsigned int       	ROSC_EN:1;
		unsigned int       	CAL_REQ:1;
		unsigned int       	CLK_32K_VLD:1;
		unsigned int		CAL_ACK:1;
		unsigned int		CAL_CNT:12;
		unsigned int		Rsv:3;
		unsigned int		REF_CYCLE:13;
	}field;
	unsigned int word;
}OSCCTL_STRUC, *POSCCTL_STRUC;
#else
typedef union _OSCCTL_STRUC{
	struct{
		unsigned int		REF_CYCLE:13;
		unsigned int		Rsv:3;
		unsigned int		CAL_CNT:12;
		unsigned int		CAL_ACK:1;
		unsigned int       	CLK_32K_VLD:1;
		unsigned int       	CAL_REQ:1;
		unsigned int       	ROSC_EN:1;
	}field;
	unsigned int word;
}OSCCTL_STRUC, *POSCCTL_STRUC;
#endif


#define COEXCFG0			0x40
#ifdef RT_BIG_ENDIAN
typedef union _COEXCFG0_STRUC{
	struct{
		unsigned int       	COEX_CFG1:8;
		unsigned int       	COEX_CFG0:8;		
		unsigned int       	FIX_WL_RF_LNA:2;
		unsigned int		FIX_BT_H_PA:3;
		unsigned int		FIX_BT_L_PA:3;
		unsigned int		FIX_WL_TX_PWR:2;
		unsigned int		Rsv:3;
		unsigned int		FIX_WL_ANT_EN:1;
		unsigned int		FIX_WL_DI_ANT:1;
		unsigned int		COEX_ENT:1;
	}field;
	unsigned int word;
}COEXCFG0_STRUC, *PCOEXCFG0_STRUC;
#else
typedef union _COEXCFG0_STRUC{
	struct{
		unsigned int		COEX_ENT:1;
		unsigned int		FIX_WL_DI_ANT:1;
		unsigned int		FIX_WL_ANT_EN:1;
		unsigned int		Rsv:3;
		unsigned int		FIX_WL_TX_PWR:2;
		unsigned int		FIX_BT_L_PA:3;
		unsigned int		FIX_BT_H_PA:3;
		unsigned int       	FIX_WL_RF_LNA:2;

		unsigned int       	COEX_CFG0:8;
		unsigned int       	COEX_CFG1:8;
	}field;
	unsigned int word;
}COEXCFG0_STRUC, *PCOEXCFG0_STRUC;
#endif


#define COEXCFG1			0x44
#ifdef RT_BIG_ENDIAN
typedef union _COEXCFG1_STRUC{
	struct{
		unsigned int       	Rsv:8;
		unsigned int		DIS_WL_RF_DELY:8;
		unsigned int		DIS_WL_PA_DELY:8;
		unsigned int		DIS_WL_TR_DELY:8;
	}field;
	unsigned int word;
}COEXCFG1_STRUC, *PCOEXCFG1_STRUC;
#else
typedef union _COEXCFG1_STRUC{
	struct{
		unsigned int		DIS_WL_TR_DELY:8;
		unsigned int		DIS_WL_PA_DELY:8;
		unsigned int		DIS_WL_RF_DELY:8;
		unsigned int       	Rsv:8;
	}field;
	unsigned int word;
}COEXCFG1_STRUC, *PCOEXCFG1_STRUC;
#endif


#define COEXCFG2			0x48
#ifdef RT_BIG_ENDIAN
typedef union _COEXCFG2_STRUC{
	struct{
		unsigned int		BT_COEX_CFG1_Bit31_Rsv:1;
		unsigned int		BT_COEX_CFG1_Bit30_Rsv:1;	
		unsigned int		BT_COEX_CFG1_Bit29_HaltHighPriorityTx_wl_bcn_busy:1;
		unsigned int		BT_COEX_CFG1_Bit28_HaltHighPriorityTx_wl_rx_busy:1;
		unsigned int		BT_COEX_CFG1_Bit27_HaltHighPriorityTx_wl_busy:1;
		unsigned int		BT_COEX_CFG1_Bit26_HaltLowPriorityTx_wl_bcn_busy:1;
		unsigned int		BT_COEX_CFG1_Bit25_HaltLowPriorityTx_wl_rx_busy:1;
		unsigned int		BT_COEX_CFG1_Bit24_HaltLowPriorityTx_wl_busy:1;		
		
		unsigned int		BT_COEX_CFG0_Bit23_Rsv:1;
		unsigned int		BT_COEX_CFG0_Bit22_Rsv:1;	
		unsigned int		BT_COEX_CFG0_Bit21_HaltHighPriorityTx_wl_bcn_busy:1;
		unsigned int		BT_COEX_CFG0_Bit20_HaltHighPriorityTx_wl_rx_busy:1;
		unsigned int		BT_COEX_CFG0_Bit19_HaltHighPriorityTx_wl_busy:1;
		unsigned int		BT_COEX_CFG0_Bit18_HaltLowPriorityTx_wl_bcn_busy:1;
		unsigned int		BT_COEX_CFG0_Bit17_HaltLowPriorityTx_wl_rx_busy:1;
		unsigned int		BT_COEX_CFG0_Bit16_HaltLowPriorityTx_wl_busy:1;
		
		unsigned int		WL_COEX_CFG1_Bit15_LowerTxPwr_bt_high_priority:1;
		unsigned int		WL_COEX_CFG1_Bit14_Enable_Tx_free_timer:1;
		unsigned int		WL_COEX_CFG1_Bit13_Disable_TxAgg_bi_high_priority:1;
		unsigned int		WL_COEX_CFG1_Bit12_Disable_bt_rx_req_h:1;
		unsigned int		WL_COEX_CFG1_Bit11_HaltTx_bt_tx_req_l:1;
		unsigned int		WL_COEX_CFG1_Bit10_HaltTx_bt_tx_req_h:1;
		unsigned int		WL_COEX_CFG1_Bit9_HaltTx_bt_rx_req_h:1;	
		unsigned int		WL_COEX_CFG1_Bit8_HaltTx_bt_rx_busy:1;		
		
		unsigned int		WL_COEX_CFG0_Bit7_LowerTxPwr_bt_high_priority:1;
		unsigned int		WL_COEX_CFG0_Bit6_Enable_Tx_free_timer:1;
		unsigned int		WL_COEX_CFG0_Bit5_Disable_TxAgg_bi_high_priority:1;
		unsigned int		WL_COEX_CFG0_Bit4_Disable_bt_rx_req_h:1;
		unsigned int		WL_COEX_CFG0_Bit3_HaltTx_bt_tx_req_l:1;
		unsigned int		WL_COEX_CFG0_Bit2_HaltTx_bt_tx_req_h:1;
		unsigned int		WL_COEX_CFG0_Bit1_HaltTx_bt_rx_req_h:1;	
		unsigned int		WL_COEX_CFG0_Bit0_HaltTx_bt_rx_busy:1;			
	}field;
	unsigned int word;
}COEXCFG2_STRUC, *PCOEXCFG2_STRUC;
#else
typedef union _COEXCFG2_STRUC{
	struct{
		unsigned int		WL_COEX_CFG0_Bit0_HaltTx_bt_rx_busy:1;	
		unsigned int		WL_COEX_CFG0_Bit1_HaltTx_bt_rx_req_h:1;	
		unsigned int		WL_COEX_CFG0_Bit2_HaltTx_bt_tx_req_h:1;
		unsigned int		WL_COEX_CFG0_Bit3_HaltTx_bt_tx_req_l:1;
		unsigned int		WL_COEX_CFG0_Bit4_Disable_bt_rx_req_h:1;
		unsigned int		WL_COEX_CFG0_Bit5_Disable_TxAgg_bi_high_priority:1;
		unsigned int		WL_COEX_CFG0_Bit6_Enable_Tx_free_timer:1;		
		unsigned int		WL_COEX_CFG0_Bit7_LowerTxPwr_bt_high_priority:1;

		unsigned int		WL_COEX_CFG1_Bit8_HaltTx_bt_rx_busy:1;
		unsigned int		WL_COEX_CFG1_Bit9_HaltTx_bt_rx_req_h:1;	
		unsigned int		WL_COEX_CFG1_Bit10_HaltTx_bt_tx_req_h:1;
		unsigned int		WL_COEX_CFG1_Bit11_HaltTx_bt_tx_req_l:1;
		unsigned int		WL_COEX_CFG1_Bit12_Disable_bt_rx_req_h:1;
		unsigned int		WL_COEX_CFG1_Bit13_Disable_TxAgg_bi_high_priority:1;
		unsigned int		WL_COEX_CFG1_Bit14_Enable_Tx_free_timer:1;		
		unsigned int		WL_COEX_CFG1_Bit15_LowerTxPwr_bt_high_priority:1;
		
		unsigned int		BT_COEX_CFG0_Bit16_HaltLowPriorityTx_wl_busy:1;
		unsigned int		BT_COEX_CFG0_Bit17_HaltLowPriorityTx_wl_rx_busy:1;
		unsigned int		BT_COEX_CFG0_Bit18_HaltLowPriorityTx_wl_bcn_busy:1;
		unsigned int		BT_COEX_CFG0_Bit19_HaltHighPriorityTx_wl_busy:1;
		unsigned int		BT_COEX_CFG0_Bit20_HaltHighPriorityTx_wl_rx_busy:1;
		unsigned int		BT_COEX_CFG0_Bit21_HaltHighPriorityTx_wl_bcn_busy:1;
		unsigned int		BT_COEX_CFG0_Bit22_Rsv:1;	
		unsigned int		BT_COEX_CFG0_Bit23_Rsv:1;
		
		unsigned int		BT_COEX_CFG1_Bit24_HaltLowPriorityTx_wl_busy:1;
		unsigned int		BT_COEX_CFG1_Bit25_HaltLowPriorityTx_wl_rx_busy:1;
		unsigned int		BT_COEX_CFG1_Bit26_HaltLowPriorityTx_wl_bcn_busy:1;
		unsigned int		BT_COEX_CFG1_Bit27_HaltHighPriorityTx_wl_busy:1;
		unsigned int		BT_COEX_CFG1_Bit28_HaltHighPriorityTx_wl_rx_busy:1;
		unsigned int		BT_COEX_CFG1_Bit29_HaltHighPriorityTx_wl_bcn_busy:1;
		unsigned int		BT_COEX_CFG1_Bit30_Rsv:1;			
		unsigned int		BT_COEX_CFG1_Bit31_Rsv:1;
	}field;
	unsigned int word;
}COEXCFG2_STRUC, *PCOEXCFG2_STRUC;
#endif


#define PLL_CTRL		0x50
#ifdef RT_BIG_ENDIAN
typedef	union	_PLL_CTRL_STRUC	{
	struct	{
		unsigned long		VBGBK_EN:1;
		unsigned long      	LOCK_DETECT_WINDOW_CTRL:3;
		unsigned long		PFD_DELAY_CTRL:2;
		unsigned long		CP_CURRENT_CTRL:2;
		unsigned long		LPF_C2_CTRL:2;
		unsigned long		LPF_C1_CTRL:2;
		unsigned long		LPF_R1:1;
		unsigned long		VCO_FIXED_CURRENT_CONTROL:3;
		unsigned long		RESERVED_INPUT2:8;
		unsigned long		RESERVED_INPUT1:8;
	}	field;
	unsigned long			word;
}	PLL_CTRL_STRUC, *PPLL_CTRL_STRUC;
#else
typedef	union	_PLL_CTRL_STRUC	{
	struct	{
		unsigned long		RESERVED_INPUT1:8;
		unsigned long		RESERVED_INPUT2:8;
		unsigned long		VCO_FIXED_CURRENT_CONTROL:3;
		unsigned long		LPF_R1:1;
		unsigned long		LPF_C1_CTRL:2;
		unsigned long		LPF_C2_CTRL:2;
		unsigned long		CP_CURRENT_CTRL:2;
		unsigned long		PFD_DELAY_CTRL:2;
		unsigned long      	LOCK_DETECT_WINDOW_CTRL:3;
		unsigned long		VBGBK_EN:1;
	}	field;
	unsigned long			word;
} PLL_CTRL_STRUC, *PPLL_CTRL_STRUC;
#endif /* RT_BIG_ENDIAN */


#define WPDMA_RST_IDX 	0x20c
#ifdef RT_BIG_ENDIAN
typedef	union _WPDMA_RST_IDX_STRUC {
	struct	{
		unsigned int       	:14;
		unsigned int       	RST_DRX_IDX1:1;
		unsigned int       	RST_DRX_IDX0:1;
		unsigned int       	rsv:6;
		unsigned int       	RST_DTX_IDX9:1;
		unsigned int       	RST_DTX_IDX8:1;
		unsigned int       	RST_DTX_IDX7:1;
		unsigned int       	RST_DTX_IDX6:1;
		unsigned int       	RST_DTX_IDX5:1;
		unsigned int       	RST_DTX_IDX4:1;
		unsigned int		RST_DTX_IDX3:1;
		unsigned int		RST_DTX_IDX2:1;
		unsigned int		RST_DTX_IDX1:1;
		unsigned int		RST_DTX_IDX0:1;
	}	field;
	unsigned int			word;
}WPDMA_RST_IDX_STRUC, *PWPDMA_RST_IDX_STRUC;
#else
typedef	union _WPDMA_RST_IDX_STRUC {
	struct	{
		unsigned int		RST_DTX_IDX0:1;
		unsigned int		RST_DTX_IDX1:1;
		unsigned int		RST_DTX_IDX2:1;
		unsigned int		RST_DTX_IDX3:1;
		unsigned int       	RST_DTX_IDX4:1;
		unsigned int       	RST_DTX_IDX5:1;
		unsigned int       	RST_DTX_IDX6:1;
		unsigned int       	RST_DTX_IDX7:1;		
		unsigned int       	RST_DTX_IDX8:1;
		unsigned int       	RST_DTX_IDX9:1;
		unsigned int       	rsv:6;
		unsigned int       	RST_DRX_IDX0:1;
		unsigned int       	RST_DRX_IDX1:1;
		unsigned int       	:14;
	}	field;
	unsigned int			word;
} WPDMA_RST_IDX_STRUC, *PWPDMA_RST_IDX_STRUC;
#endif
#define DELAY_INT_CFG  0x0210
#ifdef RT_BIG_ENDIAN
typedef	union _DELAY_INT_CFG_STRUC {
	struct	{
		unsigned int       	TXDLY_INT_EN:1;
		unsigned int       	TXMAX_PINT:7;
		unsigned int       	TXMAX_PTIME:8;
		unsigned int       	RXDLY_INT_EN:1;
		unsigned int       	RXMAX_PINT:7;
		unsigned int		RXMAX_PTIME:8;
	}	field;
	unsigned int			word;
}DELAY_INT_CFG_STRUC, *PDELAY_INT_CFG_STRUC;
#else
typedef	union _DELAY_INT_CFG_STRUC {
	struct	{
		unsigned int		RXMAX_PTIME:8;
		unsigned int       	RXMAX_PINT:7;
		unsigned int       	RXDLY_INT_EN:1;
		unsigned int       	TXMAX_PTIME:8;
		unsigned int       	TXMAX_PINT:7;
		unsigned int       	TXDLY_INT_EN:1;
	}	field;
	unsigned int			word;
} DELAY_INT_CFG_STRUC, *PDELAY_INT_CFG_STRUC;
#endif

#endif /* __RTMP_REG_PCIRBUS_H__ */
