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
	ral_omac_usb.h
 
    Abstract:
 
    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
 */

#ifndef __RAL_OMAC_USB_H__
#define __RAL_OMAC_USB_H__
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




#define USB_DMA_CFG 0x0238
#ifdef RT_BIG_ENDIAN
typedef	union _USB_DMA_CFG_STRUC {
	struct {
		unsigned int TxBusy:1;   	/*USB DMA TX FSM busy. debug only */
		unsigned int RxBusy:1;        /*USB DMA RX FSM busy. debug only */
		unsigned int EpoutValid:3;        /*OUT endpoint data valid. debug only */
		unsigned int rsv3:1;		
		unsigned int UDMA_RX_WL_DROP:1; /* Drop current WL RX packets in UDMA */
		unsigned int rsv2:1;
		unsigned int TxBulkEn:1;        /*Enable USB DMA Tx */
		unsigned int RxBulkEn:1;        /*Enable USB DMA Rx */
		unsigned int RxBulkAggEn:1;        /*Enable Rx Bulk Aggregation */
		unsigned int TxopHalt:1;        /*Halt TXOP count down when TX buffer is full. */
		unsigned int TxClear:1;        /*Clear USB DMA TX path */
		unsigned int rsv1:2;        
		unsigned int phyclear:1;        		/*phy watch dog enable. write 1 */
		unsigned int RxBulkAggLmt:8;        /*Rx Bulk Aggregation Limit  in unit of 1024 bytes */
		unsigned int RxBulkAggTOut:8;        /*Rx Bulk Aggregation TimeOut  in unit of 33ns */
	} field;
	unsigned int word;
} USB_DMA_CFG_STRUC, *PUSB_DMA_CFG_STRUC;
#else
typedef	union _USB_DMA_CFG_STRUC {
	struct {
		unsigned int RxBulkAggTOut:8;        /*Rx Bulk Aggregation TimeOut  in unit of 33ns */
		unsigned int RxBulkAggLmt:8;        /*Rx Bulk Aggregation Limit  in unit of 256 bytes */
		unsigned int phyclear:1;        		/*phy watch dog enable. write 1 */
		unsigned int rsv1:2;
		unsigned int TxClear:1;        /*Clear USB DMA TX path */
		unsigned int TxopHalt:1;        /*Halt TXOP count down when TX buffer is full. */
		unsigned int RxBulkAggEn:1;        /*Enable Rx Bulk Aggregation */
		unsigned int RxBulkEn:1;        /*Enable USB DMA Rx */
		unsigned int TxBulkEn:1;        /*Enable USB DMA Tx */
		unsigned int rsv2:1;
		unsigned int UDMA_RX_WL_DROP:1; /* Drop current WL RX packets in UDMA */
		unsigned int rsv3:1;		
		unsigned int EpoutValid:3;        /*OUT endpoint data valid */
		unsigned int RxBusy:1;        /*USB DMA RX FSM busy */
		unsigned int TxBusy:1;   	/*USB DMA TX FSM busy */
	} field;
	unsigned int word;
} USB_DMA_CFG_STRUC, *PUSB_DMA_CFG_STRUC;
#endif

#endif /*__RAL_OMAC_USB_H__ */

