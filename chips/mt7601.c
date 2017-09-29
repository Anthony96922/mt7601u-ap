/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	mt7601.c

	Abstract:
	Specific funcitons and configurations for MT7601 (RT63xx)

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/

#ifdef MT7601

#include "rt_config.h"

#include "mcu/MT7601_firmware.h"

//#define MT7601FPGA

#ifdef RTMP_EFUSE_SUPPORT
/* MT7601_USB_V0_C-20130107 */
UCHAR MT7601_EFUSE_DEFAULT_BIN[] = {
	0x01, 0x76, 0x00, 0x0c, 0x00, 0x0c, 0x43, 0x26, 0x60, 0x40, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x01, 0x02, 0x8f, 0x14, 0x01, 0x76, 0x00, 0x00, 0x4a, 0x00, 0x01, 0x00, 0x80, 0x50, 0x08, 0x00, 
	0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0xff, 0x02, 0x00, 0x20, 0x40, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x11, 0xff, 0x04, 0x28, 0xff, 0xff, 0x2c, 0x01, 0xff, 0xff, 0x99, 0x99, 
	0x8c, 0x88, 0xff, 0x07, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x20, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 
	0x03, 0x03, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};
#endif /* RTMP_EFUSE_SUPPORT */

static RTMP_REG_PAIR	MT7601_MACRegTable[] = {
	//{TSO_CTRL,		0x16058},		// must set when enable HDR_TRANS_SUPPORT
	{TSO_CTRL,		0x06050},		// must set when enable HDR_TRANS_SUPPORT
	{BCN_OFFSET0,		0x18100800}, 
	{BCN_OFFSET1,		0x38302820}, 
	{PBF_SYS_CTRL,		0x80c00},
	{PBF_CFG,		0x7F723c1f},
	{FCE_CTRL,		0x1},
	{0xA38,			0x0},			// For bi-direction RX fifo overflow issue.
	{TX0_RF_GAIN_CORR,	0x003B0005},		// 20120806 Jason Huang
	{TX0_RF_GAIN_ATTEN,	0x00006900},		// 20120806 Jason Huang
	{TX0_BB_GAIN_ATTEN,	0x00000400},		// 20120806 Jason Huang
	{TX_ALC_VGA3,		0x00060006},		// 20120806 Jason Huang
	//{TX_SW_CFG0,		0x400},			// 20120822 Gary
	{TX_SW_CFG0,		0x402},			// 20121017 Jason Huang
	{TX_SW_CFG1,		0x0},			// 20120822 Gary
	{TX_SW_CFG2,		0x0},			// 20120822 Gary

#ifdef HDR_TRANS_SUPPORT
	{HEADER_TRANS_CTRL_REG, 0x2},
#else
	{HEADER_TRANS_CTRL_REG, 0x0},
#endif /* HDR_TRANS_SUPPORT */

#ifdef CONFIG_RX_CSO_SUPPORT
	{CHECKSUM_OFFLOAD,	0x30f},
	{FCE_PARAM,		0x00256f0f},
#else
	{CHECKSUM_OFFLOAD,	0x200},
	{FCE_PARAM,		0x00254f0f},
#endif /* CONFIG_RX_CSO_SUPPORT */
};
static UCHAR MT7601_NUM_MAC_REG_PARMS = (sizeof(MT7601_MACRegTable) / sizeof(RTMP_REG_PAIR));


//1019 BBP CR
static RTMP_REG_PAIR MT7601_BBP_InitRegTb[] = {
	/* TX/RX Controls */
	{BBP_R1, 0x04},
	{BBP_R4, 0x40},
	{BBP_R20, 0x06},
	{BBP_R31, 0x08},
	/* CCK Tx Control  */
	{BBP_R178, 0xFF},
	/* AGC/Sync controls */
	//{BBP_R65, 0x2C},
	{BBP_R66, 0x14},
	{BBP_R68, 0x8B},
	{BBP_R69, 0x12},
	{BBP_R70, 0x09},
	{BBP_R73, 0x11},
	{BBP_R75, 0x60},
	{BBP_R76, 0x44},
	{BBP_R84, 0x9A},
	{BBP_R86, 0x38},
	{BBP_R91, 0x07},
	{BBP_R92, 0x02},
	/* Rx Path Controls */
	{BBP_R99, 0x50},
	{BBP_R101, 0x00},
	{BBP_R103, 0xC0},
	{BBP_R104, 0x92},
	{BBP_R105, 0x3C},
	{BBP_R106, 0x03},
	{BBP_R128, 0x12},
	/* Change RXWI content: Gain Report */
	{BBP_R142, 0x04},
	{BBP_R143, 0x37},
	/* Change RXWI content: Antenna Report */
	{BBP_R142, 0x03},
	{BBP_R143, 0x99},
	/* Calibration Index Register */
	/* CCK Receiver Control */
	{BBP_R160, 0xEB},
	{BBP_R161, 0xC4},
	{BBP_R162, 0x77},
	{BBP_R163, 0xF9},
	{BBP_R164, 0x88},
	{BBP_R165, 0x80},
	{BBP_R166, 0xFF},
	{BBP_R167, 0xE4},
	/* Added AGC controls */
	/* These AGC/GLRT registers are accessed through R195 and R196. */
	/* 0x00 */
	{BBP_R195, 0x00},
	{BBP_R196, 0x00},
	/* 0x01 */
	{BBP_R195, 0x01},
	{BBP_R196, 0x04},
	/* 0x02 */
	{BBP_R195, 0x02},
	{BBP_R196, 0x20},
	/* 0x03 */
	{BBP_R195, 0x03},
	{BBP_R196, 0x0A},
	/* 0x06 */
	{BBP_R195, 0x06},
	{BBP_R196, 0x16},
	/* 0x07 */
	{BBP_R195, 0x07},
	{BBP_R196, 0x05},
	/* 0x08 */
	{BBP_R195, 0x08},
	{BBP_R196, 0x37},
	/* 0x0A */
	{BBP_R195, 0x0A},
	{BBP_R196, 0x15},
	/* 0x0B */
	{BBP_R195, 0x0B},
	{BBP_R196, 0x17},
	/* 0x0C */
	{BBP_R195, 0x0C},
	{BBP_R196, 0x06},
	/* 0x0D */
	{BBP_R195, 0x0D},
	{BBP_R196, 0x09},
	/* 0x0E */
	{BBP_R195, 0x0E},
	{BBP_R196, 0x05},
	/* 0x0F */
	{BBP_R195, 0x0F},
	{BBP_R196, 0x09},
	/* 0x10 */
	{BBP_R195, 0x10},
	{BBP_R196, 0x20},
	/* 0x20 */
	{BBP_R195, 0x20},
	{BBP_R196, 0x17},
	/* 0x21 */
	{BBP_R195, 0x21},
	{BBP_R196, 0x06},
	/* 0x22 */
	{BBP_R195, 0x22},
	{BBP_R196, 0x09},
	/* 0x23 */
	{BBP_R195, 0x23},
	{BBP_R196, 0x17},
	/* 0x24 */
	{BBP_R195, 0x24},
	{BBP_R196, 0x06},
	/* 0x25 */
	{BBP_R195, 0x25},
	{BBP_R196, 0x09},
	/* 0x26 */
	{BBP_R195, 0x26},
	{BBP_R196, 0x17},
	/* 0x27 */
	{BBP_R195, 0x27},
	{BBP_R196, 0x06},
	/* 0x28 */
	{BBP_R195, 0x28},
	{BBP_R196, 0x09},
	/* 0x29 */
	{BBP_R195, 0x29},
	{BBP_R196, 0x05},
	/* 0x2A */
	{BBP_R195, 0x2A},
	{BBP_R196, 0x09},
	/* 0x80 */
	{BBP_R195, 0x80},
	{BBP_R196, 0x8B},
	/* 0x81 */
	{BBP_R195, 0x81},
	{BBP_R196, 0x12},
	/* 0x82 */
	{BBP_R195, 0x82},
	{BBP_R196, 0x09},
	/* 0x83 */
	{BBP_R195, 0x83},
	{BBP_R196, 0x17},
	/* 0x84 */
	{BBP_R195, 0x84},
	{BBP_R196, 0x11},
	/* 0x85 */
	{BBP_R195, 0x85},
	{BBP_R196, 0x00},
	/* 0x86 */
	{BBP_R195, 0x86},
	{BBP_R196, 0x00},
	/* 0x87 */
	{BBP_R195, 0x87},
	{BBP_R196, 0x18},
	/* 0x88 */
	{BBP_R195, 0x88},
	{BBP_R196, 0x60},
	/* 0x89 */
	{BBP_R195, 0x89},
	{BBP_R196, 0x44},
	/*     */
	{BBP_R195, 0x8A},
	{BBP_R196, 0x8B},
	{BBP_R195, 0x8B},
	{BBP_R196, 0x8B},
	{BBP_R195, 0x8C},
	{BBP_R196, 0x8B},
	{BBP_R195, 0x8D},
	{BBP_R196, 0x8B},
	/*     */
	{BBP_R195, 0x8E},
	{BBP_R196, 0x09},
	{BBP_R195, 0x8F},
	{BBP_R196, 0x09},
	{BBP_R195, 0x90},
	{BBP_R196, 0x09},
	{BBP_R195, 0x91},
	{BBP_R196, 0x09},
	/*     */
	{BBP_R195, 0x92},
	{BBP_R196, 0x11},
	{BBP_R195, 0x93},
	{BBP_R196, 0x11},
	{BBP_R195, 0x94},
	{BBP_R196, 0x11},
	{BBP_R195, 0x95},
	{BBP_R196, 0x11},
	// PPAD
	{BBP_R47, 0x80},
	{BBP_R60, 0x80},
	{BBP_R150, 0xD2},
	{BBP_R151, 0x32},
	{BBP_R152, 0x23},
	{BBP_R153, 0x41},
	{BBP_R154, 0x00},
	{BBP_R155, 0x4F},
	{BBP_R253, 0x7E},
	{BBP_R195, 0x30},
	{BBP_R196, 0x32},
	{BBP_R195, 0x31},
	{BBP_R196, 0x23},
	{BBP_R195, 0x32},
	{BBP_R196, 0x45},
	{BBP_R195, 0x35},
	{BBP_R196, 0x4A},
	{BBP_R195, 0x36},
	{BBP_R196, 0x5A},
	{BBP_R195, 0x37},
	{BBP_R196, 0x5A},
};
static UCHAR MT7601_BBP_InitRegTb_Size = (sizeof(MT7601_BBP_InitRegTb) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR MT7601_BBP_BW20RegTb[] = {
	{BBP_R69, 0x12},
	{BBP_R91, 0x07},
	/* 0x23 */
	{BBP_R195, 0x23},
	{BBP_R196, 0x17},
	/* 0x24 */
	{BBP_R195, 0x24},
	{BBP_R196, 0x06},
	/* 0x81 */
	{BBP_R195, 0x81},
	{BBP_R196, 0x12},
	/* 0x83 */
	{BBP_R195, 0x83},
	{BBP_R196, 0x17},
};
const UCHAR MT7601_BBP_BW20RegTb_Size = (sizeof(MT7601_BBP_BW20RegTb) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR MT7601_BBP_BW40RegTb[] = {
	{BBP_R69, 0x15},
	{BBP_R91, 0x04},
	/* 0x23 */
	{BBP_R195, 0x23},
	{BBP_R196, 0x12},
	/* 0x24 */
	{BBP_R195, 0x24},
	{BBP_R196, 0x08},
	/* 0x81 */
	{BBP_R195, 0x81},
	{BBP_R196, 0x15},
	/* 0x83 */
	{BBP_R195, 0x83},
	{BBP_R196, 0x16},
};
const UCHAR MT7601_BBP_BW40RegTb_Size = (sizeof(MT7601_BBP_BW40RegTb) / sizeof(RTMP_REG_PAIR));

static RTMP_REG_PAIR MT7601_BBP_CommonRegTb[] = {
	{BBP_R75, 0x60},
	{BBP_R92, 0x02},
	{BBP_R178, 0xFF},		// For CCK CH14 OBW
	/* 0x88 */
	{BBP_R195, 0x88},
	{BBP_R196, 0x60},

};
const static UCHAR MT7601_BBP_CommonRegTb_Size = (sizeof(MT7601_BBP_CommonRegTb) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR MT7601_BBP_HighTempBW20RegTb[] = {
	{BBP_R69, 0x12},
	{BBP_R91, 0x07},
	/* 0x23 */
	{BBP_R195, 0x23},
	{BBP_R196, 0x17},
	/* 0x24 */
	{BBP_R195, 0x24},
	{BBP_R196, 0x06},
	/* 0x81 */
	{BBP_R195, 0x81},
	{BBP_R196, 0x12},
	/* 0x83 */
	{BBP_R195, 0x83},
	{BBP_R196, 0x17},
};
const UCHAR MT7601_BBP_HighTempBW20RegTb_Size = (sizeof(MT7601_BBP_HighTempBW20RegTb) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR MT7601_BBP_HighTempBW40RegTb[] = {
	{BBP_R69, 0x15},
	{BBP_R91, 0x04},
	/* 0x23 */
	{BBP_R195, 0x23},
	{BBP_R196, 0x12},
	/* 0x24 */
	{BBP_R195, 0x24},
	{BBP_R196, 0x08},
	/* 0x81 */
	{BBP_R195, 0x81},
	{BBP_R196, 0x15},
	/* 0x83 */
	{BBP_R195, 0x83},
	{BBP_R196, 0x16},
};
const UCHAR MT7601_BBP_HighTempBW40RegTb_Size = (sizeof(MT7601_BBP_HighTempBW40RegTb) / sizeof(RTMP_REG_PAIR));

static RTMP_REG_PAIR MT7601_BBP_HighTempCommonRegTb[] = {
	{BBP_R75, 0x60},
	{BBP_R92, 0x02},
	{BBP_R178, 0xFF},		// For CCK CH14 OBW
	/* 0x88 */
	{BBP_R195, 0x88},
	{BBP_R196, 0x60},
};
const static UCHAR MT7601_BBP_HighTempCommonRegTb_Size = (sizeof(MT7601_BBP_HighTempCommonRegTb) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR MT7601_BBP_LowTempBW20RegTb[] = {
	{BBP_R69, 0x12},
	{BBP_R75, 0x5E},
	{BBP_R91, 0x07},
	{BBP_R92, 0x02},
	/* 0x23 */
	{BBP_R195, 0x23},
	{BBP_R196, 0x17},
	/* 0x24 */
	{BBP_R195, 0x24},
	{BBP_R196, 0x06},
	/* 0x81 */
	{BBP_R195, 0x81},
	{BBP_R196, 0x12},
	/* 0x83 */
	{BBP_R195, 0x83},
	{BBP_R196, 0x17},
	/* 0x88 */
	{BBP_R195, 0x88},
	{BBP_R196, 0x5E},
};
const UCHAR MT7601_BBP_LowTempBW20RegTb_Size = (sizeof(MT7601_BBP_LowTempBW20RegTb) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR MT7601_BBP_LowTempBW40RegTb[] = {
	{BBP_R69, 0x15},
	{BBP_R75, 0x5C},
	{BBP_R91, 0x04},
	{BBP_R92, 0x03},
	/* 0x23 */
	{BBP_R195, 0x23},
	{BBP_R196, 0x10},
	/* 0x24 */
	{BBP_R195, 0x24},
	{BBP_R196, 0x08},
	/* 0x81 */
	{BBP_R195, 0x81},
	{BBP_R196, 0x15},
	/* 0x83 */
	{BBP_R195, 0x83},
	{BBP_R196, 0x16},
	/* 0x88 */
	{BBP_R195, 0x88},
	{BBP_R196, 0x5B},
};
UCHAR MT7601_BBP_LowTempBW40RegTb_Size = (sizeof(MT7601_BBP_LowTempBW40RegTb) / sizeof(RTMP_REG_PAIR));

static RTMP_REG_PAIR MT7601_BBP_LowTempCommonRegTb[] = {
	{BBP_R178, 0xFF},		// For CCK CH14 OBW
};
const static UCHAR MT7601_BBP_LowTempCommonRegTb_Size = (sizeof(MT7601_BBP_LowTempCommonRegTb) / sizeof(RTMP_REG_PAIR));

// 20121122 RF CR
/* Bank	Register Value(Hex) */
static BANK_RF_REG_PAIR MT7601_RF_Central_RegTb[] = {
/*
	Bank 0 - For central blocks: BG, PLL, XTAL, LO, ADC/DAC
*/
	{RF_BANK0,	RF_R00, 0x02},
	{RF_BANK0,	RF_R01, 0x01},
	{RF_BANK0,	RF_R02, 0x11},
	{RF_BANK0,	RF_R03, 0xFF},
	{RF_BANK0,	RF_R04, 0x0A},
	{RF_BANK0,	RF_R05, 0x20},
	{RF_BANK0,	RF_R06, 0x00},

	/*
		BG
	*/
	{RF_BANK0,	RF_R07, 0x00},
	{RF_BANK0,	RF_R08, 0x00},
	{RF_BANK0,	RF_R09, 0x00},
	{RF_BANK0,	RF_R10, 0x00},
	{RF_BANK0,	RF_R11, 0x21},

	/*
		XO
	*/
	//{RF_BANK0,	RF_R12, 0x00},		// By EEPROM
	{RF_BANK0,	RF_R13, 0x00},		// 40MHZ xtal
	//{RF_BANK0,	RF_R13, 0x13},		// 20MHZ xtal
	{RF_BANK0,	RF_R14, 0x7C},
	{RF_BANK0,	RF_R15, 0x22},
	{RF_BANK0,	RF_R16, 0x80},


	/*
		PLL
	*/
	{RF_BANK0,	RF_R17, 0x99},
	{RF_BANK0,	RF_R18, 0x99},
	{RF_BANK0,	RF_R19, 0x09},
	{RF_BANK0,	RF_R20, 0x50},
	{RF_BANK0,	RF_R21, 0xB0},
	{RF_BANK0,	RF_R22, 0x00},
	{RF_BANK0,	RF_R23, 0xC5},
	{RF_BANK0,	RF_R24, 0xFC},
	{RF_BANK0,	RF_R25, 0x40},
	{RF_BANK0,	RF_R26, 0x4D},
	{RF_BANK0,	RF_R27, 0x02},
	{RF_BANK0,	RF_R28, 0x72},
	{RF_BANK0,	RF_R29, 0x01},
	{RF_BANK0,	RF_R30, 0x00},
	{RF_BANK0,	RF_R31, 0x00},

	/*
		Test Ports
	*/
	{RF_BANK0,	RF_R32, 0x00},
	{RF_BANK0,	RF_R33, 0x00},
	{RF_BANK0,	RF_R34, 0x23},
	{RF_BANK0,	RF_R35, 0x01}, /* Change setting to reduce spurs */
	{RF_BANK0,	RF_R36, 0x00},
	{RF_BANK0,	RF_R37, 0x00},

	/*
		ADC/DAC
	*/
	{RF_BANK0,	RF_R38, 0x00},
	{RF_BANK0,	RF_R39, 0x20},
	{RF_BANK0,	RF_R40, 0x00},
	{RF_BANK0,	RF_R41, 0xD0},
	{RF_BANK0,	RF_R42, 0x1B},
	{RF_BANK0,	RF_R43, 0x02},
	{RF_BANK0,	RF_R44, 0x00},
};
static UINT32 MT7601_RF_Central_RegTb_Size = (sizeof(MT7601_RF_Central_RegTb) / sizeof(BANK_RF_REG_PAIR));

static BANK_RF_REG_PAIR MT7601_RF_Channel_RegTb[] = {
	{RF_BANK4,	RF_R00, 0x01},
	{RF_BANK4,	RF_R01, 0x00},
	{RF_BANK4,	RF_R02, 0x00},
	{RF_BANK4,	RF_R03, 0x00},

	/*
		LDO
	*/
	{RF_BANK4,	RF_R04, 0x00},
	{RF_BANK4,	RF_R05, 0x08},
	{RF_BANK4,	RF_R06, 0x00},

	/*
		RX
	*/
	{RF_BANK4,	RF_R07, 0x5B},
	{RF_BANK4,	RF_R08, 0x52},
	{RF_BANK4,	RF_R09, 0xB6},
	{RF_BANK4,	RF_R10, 0x57},
	{RF_BANK4,	RF_R11, 0x33},
	{RF_BANK4,	RF_R12, 0x22},
	{RF_BANK4,	RF_R13, 0x3D},
	{RF_BANK4,	RF_R14, 0x3E},
	{RF_BANK4,	RF_R15, 0x13},
	{RF_BANK4,	RF_R16, 0x22},
	{RF_BANK4,	RF_R17, 0x23},
	{RF_BANK4,	RF_R18, 0x02},
	{RF_BANK4,	RF_R19, 0xA4},
	{RF_BANK4,	RF_R20, 0x01},
	{RF_BANK4,	RF_R21, 0x12},
	{RF_BANK4,	RF_R22, 0x80},
	{RF_BANK4,	RF_R23, 0xB3},
	{RF_BANK4,	RF_R24, 0x00}, /* Reserved */
	{RF_BANK4,	RF_R25, 0x00}, /* Reserved */
	{RF_BANK4,	RF_R26, 0x00}, /* Reserved */
	{RF_BANK4,	RF_R27, 0x00}, /* Reserved */

	/*
		LOGEN
	*/
	{RF_BANK4,	RF_R28, 0x18},
	{RF_BANK4,	RF_R29, 0xEE},
	{RF_BANK4,	RF_R30, 0x6B},
	{RF_BANK4,	RF_R31, 0x31},
	{RF_BANK4,	RF_R32, 0x5D},
	{RF_BANK4,	RF_R33, 0x00}, /* Reserved */

	/*
		TX
	*/
	{RF_BANK4,	RF_R34, 0x96},
	{RF_BANK4,	RF_R35, 0x55},
	{RF_BANK4,	RF_R36, 0x08},
	{RF_BANK4,	RF_R37, 0xBB},
	{RF_BANK4,	RF_R38, 0xB3},
	{RF_BANK4,	RF_R39, 0xB3},
	{RF_BANK4,	RF_R40, 0x03},
	{RF_BANK4,	RF_R41, 0x00}, /* Reserved */
	{RF_BANK4,	RF_R42, 0x00}, /* Reserved */
	{RF_BANK4,	RF_R43, 0xC5},
	{RF_BANK4,	RF_R44, 0xC5},
	{RF_BANK4,	RF_R45, 0xC5},
	{RF_BANK4,	RF_R46, 0x07},
	{RF_BANK4,	RF_R47, 0xA8},
	{RF_BANK4,	RF_R48, 0xEF},
	{RF_BANK4,	RF_R49, 0x1A},

	/*
		PA
	*/
	{RF_BANK4,	RF_R54, 0x07},
	{RF_BANK4,	RF_R55, 0xA7},
	{RF_BANK4,	RF_R56, 0xCC},
	{RF_BANK4,	RF_R57, 0x14},
	{RF_BANK4,	RF_R58, 0x07},
	{RF_BANK4,	RF_R59, 0xA8},
	{RF_BANK4,	RF_R60, 0xD7},
	{RF_BANK4,	RF_R61, 0x10},
	{RF_BANK4,	RF_R62, 0x1C},
	{RF_BANK4,	RF_R63, 0x00}, /* Reserved */
};
static UINT32 MT7601_RF_Channel_RegTb_Size = (sizeof(MT7601_RF_Channel_RegTb) / sizeof(BANK_RF_REG_PAIR));

static BANK_RF_REG_PAIR MT7601_RF_VGA_RegTb[] = {
	{RF_BANK5,	RF_R00, 0x47},
	{RF_BANK5,	RF_R01, 0x00},
	{RF_BANK5,	RF_R02, 0x00},
	{RF_BANK5,	RF_R03, 0x08},
	{RF_BANK5,	RF_R04, 0x04},
	{RF_BANK5,	RF_R05, 0x20},
	{RF_BANK5,	RF_R06, 0x3A},
	{RF_BANK5,	RF_R07, 0x3A},
	{RF_BANK5,	RF_R08, 0x00},
	{RF_BANK5,	RF_R09, 0x00},
	{RF_BANK5,	RF_R10, 0x10},
	{RF_BANK5,	RF_R11, 0x10},
	{RF_BANK5,	RF_R12, 0x10},
	{RF_BANK5,	RF_R13, 0x10},
	{RF_BANK5,	RF_R14, 0x10},
	{RF_BANK5,	RF_R15, 0x20},
	{RF_BANK5,	RF_R16, 0x22},
	{RF_BANK5,	RF_R17, 0x7C},
	{RF_BANK5,	RF_R18, 0x00},
	{RF_BANK5,	RF_R19, 0x00},
	{RF_BANK5,	RF_R20, 0x00},
	{RF_BANK5,	RF_R21, 0xF1},
	{RF_BANK5,	RF_R22, 0x11},
	{RF_BANK5,	RF_R23, 0x02},
	{RF_BANK5,	RF_R24, 0x41},
	{RF_BANK5,	RF_R25, 0x20},
	{RF_BANK5,	RF_R26, 0x00},
	{RF_BANK5,	RF_R27, 0xD7},
	{RF_BANK5,	RF_R28, 0xA2},
	{RF_BANK5,	RF_R29, 0x20},
	{RF_BANK5,	RF_R30, 0x49},
	{RF_BANK5,	RF_R31, 0x20},
	{RF_BANK5,	RF_R32, 0x04},
	{RF_BANK5,	RF_R33, 0xF1},
	{RF_BANK5,	RF_R34, 0xA1},
	{RF_BANK5,	RF_R35, 0x01},
	{RF_BANK5,	RF_R41, 0x00},
	{RF_BANK5,	RF_R42, 0x00},
	{RF_BANK5,	RF_R43, 0x00},
	{RF_BANK5,	RF_R44, 0x00},
	{RF_BANK5,	RF_R45, 0x00},
	{RF_BANK5,	RF_R46, 0x00},
	{RF_BANK5,	RF_R47, 0x00},
	{RF_BANK5,	RF_R48, 0x00},
	{RF_BANK5,	RF_R49, 0x00},
	{RF_BANK5,	RF_R50, 0x00},
	{RF_BANK5,	RF_R51, 0x00},
	{RF_BANK5,	RF_R52, 0x00},
	{RF_BANK5,	RF_R53, 0x00},
	{RF_BANK5,	RF_R54, 0x00},
	{RF_BANK5,	RF_R55, 0x00},
	{RF_BANK5,	RF_R56, 0x00},
	{RF_BANK5,	RF_R57, 0x00},
	{RF_BANK5,	RF_R58, 0x31},
	{RF_BANK5,	RF_R59, 0x31},
	{RF_BANK5,	RF_R60, 0x0A},
	{RF_BANK5,	RF_R61, 0x02},
	{RF_BANK5,	RF_R62, 0x00},
	{RF_BANK5,	RF_R63, 0x00},
};
static UINT32 MT7601_RF_VGA_RegTb_Size = (sizeof(MT7601_RF_VGA_RegTb) / sizeof(BANK_RF_REG_PAIR));

const MT7601_FREQ_ITEM MT7601_Frequency_Plan[] =
{
	/* CH,	K_R17,	K_R18,	K_R19,	N_R20 */
	{ 1,		0x99,	0x99,	0x09,	0x50},
	{ 2,		0x46,	0x44,	0x0A,	0x50},
	{ 3,		0xEC,	0xEE,	0x0A,	0x50},
	{ 4,		0x99,	0x99,	0x0B,	0x50},
	{ 5,		0x46,	0x44,	0x08,	0x51},
	{ 6,		0xEC,	0xEE,	0x08,	0x51},
	{ 7,		0x99,	0x99,	0x09,	0x51},
	{ 8,		0x46,	0x44,	0x0A,	0x51},
	{ 9,		0xEC,	0xEE,	0x0A,	0x51},
	{ 10,	0x99,	0x99,	0x0B,	0x51},
	{ 11,	0x46,	0x44,	0x08,	0x52},
	{ 12,	0xEC,	0xEE,	0x08,	0x52},
	{ 13,	0x99,	0x99,	0x09,	0x52},
	{ 14,	0x33,	0x33,	0x0B,	0x52},
};
UINT32 NUM_OF_MT7601_CHNL = (sizeof(MT7601_Frequency_Plan) / sizeof(MT7601_FREQ_ITEM));


/*
	========================================================================

	Routine Description:
		Enable Wlan function. this action will enable wlan clock so that chip can accept command. So MUST put in the 
		very beginning of Initialization. And put in the very LAST in the Halt function.

	Arguments:
		pAd		Pointer to our adapter

	Return Value:
		None

	IRQL <= DISPATCH_LEVEL

	Note:
		Before Enable RX, make sure you have enabled Interrupt.
	========================================================================
*/
VOID MT7601_WLAN_ChipOnOff(
	IN RTMP_ADAPTER *pAd,
	IN BOOLEAN bOn,
	IN BOOLEAN bResetWLAN)
{
	WLAN_FUN_CTRL_STRUC WlanFunCtrl = {.word=0};


#ifdef RTMP_MAC_USB
	int RET;
	if (IS_USB_INF(pAd)) {
		RTMP_SEM_EVENT_WAIT(&pAd->hw_atomic, RET);
		if (RET != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("reg_atomic get failed(ret=%d)\n", RET));
			return;
		}
	}
#endif /* RTMP_MAC_USB */

	RTMP_IO_READ32(pAd, WLAN_FUN_CTRL, &WlanFunCtrl.word);
	DBGPRINT(RT_DEBUG_INFO, ("==>%s(): OnOff:%d, pAd->WlanFunCtrl:0x%x, Reg-WlanFunCtrl=0x%x\n",
				__FUNCTION__, bOn, pAd->WlanFunCtrl.word, WlanFunCtrl.word));

	if (bResetWLAN == TRUE) {
		WlanFunCtrl.field.GPIO0_OUT_OE_N = 0xFF;
		WlanFunCtrl.field.FRC_WL_ANT_SET = 0;

		if (pAd->WlanFunCtrl.field.WLAN_EN) {
			/*
				Restore all HW default value and reset RF.
			*/
			WlanFunCtrl.field.WLAN_RESET = 1;
			WlanFunCtrl.field.WLAN_RESET_RF = 1;
			DBGPRINT(RT_DEBUG_TRACE, ("Reset(1) WlanFunCtrl.word = 0x%x\n", WlanFunCtrl.word));
			RTMP_IO_WRITE32(pAd, WLAN_FUN_CTRL, WlanFunCtrl.word);
			RTMPusecDelay(20);
			WlanFunCtrl.field.WLAN_RESET = 0;
			WlanFunCtrl.field.WLAN_RESET_RF = 0;
			DBGPRINT(RT_DEBUG_TRACE, ("Reset(2) WlanFunCtrl.word = 0x%x\n", WlanFunCtrl.word));
			RTMP_IO_WRITE32(pAd, WLAN_FUN_CTRL, WlanFunCtrl.word);
			RTMPusecDelay(20);
		}
	}

	if (bOn == TRUE) {
		/*
			Enable WLAN function and clock
			WLAN_FUN_CTRL[1:0] = 0x3
		*/
		ENABLE_WLAN_FUN(WlanFunCtrl);
	} else {
		/*
			Diable WLAN function and clock
			WLAN_FUN_CTRL[1:0] = 0x0
		*/
		DISABLE_WLAN_FUN(WlanFunCtrl);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("WlanFunCtrl.word = 0x%x\n", WlanFunCtrl.word));
	RTMP_IO_WRITE32(pAd, WLAN_FUN_CTRL, WlanFunCtrl.word);
	RTMPusecDelay(20);

	if (bOn) {
		RTMP_IO_READ32(pAd, MAC_CSR0, &pAd->MACVersion);
		DBGPRINT(RT_DEBUG_TRACE, ("MACVersion = 0x%08x\n", pAd->MACVersion));
	}

	if (bOn == TRUE) {
		UINT index = 0;
		CMB_CTRL_STRUC CmbCtrl;

		CmbCtrl.word = 0;

		do {
			do {
				RTMP_IO_READ32(pAd, CMB_CTRL, &CmbCtrl.word);

				/*
					Check status of PLL_LD & XTAL_RDY.
					HW issue: Must check PLL_LD&XTAL_RDY when setting EEP to disable PLL power down
				*/
				if ((CmbCtrl.field.PLL_LD == 1) && (CmbCtrl.field.XTAL_RDY == 1))
					break;

				RTMPusecDelay(20);
			} while (index++ < MAX_CHECK_COUNT);

			if (index >= MAX_CHECK_COUNT) {
				DBGPRINT(RT_DEBUG_ERROR, 
						("Lenny:[boundary]Check PLL_LD ..CMB_CTRL 0x%08x, index=%d!\n",
						CmbCtrl.word, index));
				/*
					Disable WLAN then enable WLAN again
				*/
				DISABLE_WLAN_FUN(WlanFunCtrl);
				RTMP_IO_WRITE32(pAd, WLAN_FUN_CTRL, WlanFunCtrl.word);
				RTMPusecDelay(20);

				ENABLE_WLAN_FUN(WlanFunCtrl);
				RTMP_IO_WRITE32(pAd, WLAN_FUN_CTRL, WlanFunCtrl.word);
				RTMPusecDelay(20);
			} else {
				break;
			}
		}
		while (TRUE);
	}

	pAd->WlanFunCtrl.word = WlanFunCtrl.word;
	RTMP_IO_READ32(pAd, WLAN_FUN_CTRL, &WlanFunCtrl.word);
	DBGPRINT(RT_DEBUG_INFO,
		("<== %s():  pAd->WlanFunCtrl.word = 0x%x, Reg->WlanFunCtrl=0x%x!\n",
		__FUNCTION__, pAd->WlanFunCtrl.word, WlanFunCtrl.word));


#ifdef RTMP_MAC_USB
	if (IS_USB_INF(pAd)) {
		RTMP_SEM_EVENT_UP(&pAd->hw_atomic);
	}
#endif /* RTMP_MAC_USB */

}


VOID MT7601_RXDC_CAL(RTMP_ADAPTER *pAd)
{
#define MAX_RXDCOC_RETRY_CNT	20
	UINT count;
	UCHAR RValue;
	UINT32 Mac_R1004;

	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &Mac_R1004);

	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x08);

//1012 BBP CR
	AndesBBPRandomWrite(pAd, 4,
	BBP_R158, 0x8D,
	BBP_R159, 0xFC,
	BBP_R158, 0x8C,
	BBP_R159, 0x4C);

	for (count = 0; count < MAX_RXDCOC_RETRY_CNT; count++) {
		RTMPusecDelay(300);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R158, 0x8C);
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R159, &RValue);

		if (RValue == 0x0c)
			break;
	}

	if (count == MAX_RXDCOC_RETRY_CNT)
		DBGPRINT_ERR(("MT7601_RXDC_CAL Fail!\n"));

	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x00);

	//0725 BBP CR change DCOC sequence.
	AndesBBPRandomWrite(pAd, 2,
	BBP_R158, 0x8D,
	BBP_R159, 0xE0);

	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, Mac_R1004);
}


VOID MT7601_INIT_CAL(RTMP_ADAPTER *pAd)
{
	UCHAR RfValue;
	UINT32 Mac_R1004;

	DBGPRINT(RT_DEBUG_TRACE, ("==>%s\n", __FUNCTION__));

#ifdef MT7601FPGA
	return;
#endif /*MT7601FPGA */

	MT7601_Bootup_Read_Temperature(pAd, &pAd->chipCap.CurrentTemperBbpR49);
	pAd->chipCap.CurrentTemperature = (pAd->chipCap.CurrentTemperBbpR49 - pAd->chipCap.TemperatureRef25C) * MT7601_E2_TEMPERATURE_SLOPE;
	pAd->chipCap.TemperatureDPD = pAd->chipCap.CurrentTemperature;

	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &Mac_R1004);

	// R Calibration
	AndesCalibrationOP(pAd, ANDES_CALIBRATION_R, 0);

	// VCO Cal
	rlt_rf_read(pAd, RF_BANK0, RF_R04, &RfValue);
	RfValue = RfValue | 0x80;
	rlt_rf_write(pAd, RF_BANK0, RF_R04, RfValue);
	RTMPusecDelay(2000);

	/* TXDC */
	AndesCalibrationOP(pAd, ANDES_CALIBRATION_TXDCOC, 0);

	// MT7601_RXDC_CAL
	MT7601_RXDC_CAL(pAd);

	/* Tx Filter BW */
	AndesCalibrationOP(pAd, ANDES_CALIBRATION_BW, 0x00001);

	/* Rx Filter BW */
	AndesCalibrationOP(pAd, ANDES_CALIBRATION_BW, 0x00000);

	/* TXLOFT */
	AndesCalibrationOP(pAd, ANDES_CALIBRATION_LOFT, 0);

	/* TXIQ */
	AndesCalibrationOP(pAd, ANDES_CALIBRATION_TXIQ, 0);

	/* RXIQ */
	AndesCalibrationOP(pAd, ANDES_CALIBRATION_RXIQ, 0);

#ifdef DPD_CALIBRATION_SUPPORT
	/* DPD-Calibration */
	AndesCalibrationOP(pAd, ANDES_CALIBRATION_DPD, pAd->chipCap.CurrentTemperature);
#endif /* DPD_CALIBRATION_SUPPORT */

	// MT7601_RXDC_CAL
	MT7601_RXDC_CAL(pAd);

#ifdef RTMP_INTERNAL_TX_ALC
	MT7601_TssiDcGainCalibration(pAd);
#endif /* RTMP_INTERNAL_TX_ALC */

	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, Mac_R1004);

	MT7601AsicTemperatureCompensation(pAd, TRUE);

	DBGPRINT(RT_DEBUG_TRACE, ("<==%s\n", __FUNCTION__));

}


VOID dump_bw_info(RTMP_ADAPTER *pAd)
{
}


/*
========================================================================
Routine Description:
	Initialize FCE.

Arguments:
	pAd					- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
VOID InitFce(
	PRTMP_ADAPTER pAd)
{
	L2_STUFFING_STRUC L2Stuffing = { {0} };

	DBGPRINT(RT_DEBUG_TRACE, ("%s: -->\n", __FUNCTION__));

	RTMP_IO_READ32(pAd, FCE_L2_STUFF, &L2Stuffing.word);
	L2Stuffing.field.FS_WR_MPDU_LEN_EN = 0;
	RTMP_IO_WRITE32(pAd, FCE_L2_STUFF, L2Stuffing.word);

	DBGPRINT(RT_DEBUG_TRACE, ("%s: <--\n", __FUNCTION__));
}


/*
========================================================================
Routine Description:
	Initialize specific RF registers.

Arguments:
	pAd					- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
static VOID NICInitMT7601RFRegisters(RTMP_ADAPTER *pAd)
{

	DBGPRINT(RT_DEBUG_TRACE, ("%s\n", __FUNCTION__));

#ifdef MT7601FPGA
	return;
#endif /*MT7601FPGA */

	rlt_rf_write(pAd, RF_BANK0, RF_R12, pAd->RfFreqOffset);

	AndesRFRandomWritePair(pAd, MT7601_RF_Central_RegTb, MT7601_RF_Central_RegTb_Size);

	AndesRFRandomWritePair(pAd, MT7601_RF_Channel_RegTb, MT7601_RF_Channel_RegTb_Size);

	AndesRFRandomWritePair(pAd, MT7601_RF_VGA_RegTb, MT7601_RF_VGA_RegTb_Size);

	MT7601_INIT_CAL(pAd);


}


/*
========================================================================
Routine Description:
	Initialize specific MAC registers.

Arguments:
	pAd					- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
static VOID NICInitMT7601MacRegisters(RTMP_ADAPTER *pAd)
{
	UINT32 MacReg = 0;

	DBGPRINT(RT_DEBUG_TRACE, ("%s\n", __FUNCTION__));

	/*
		Enable PBF and MAC clock
		SYS_CTRL[11:10] = 0x3
	*/
	AndesRandomWritePair(pAd, MT7601_MACRegTable, MT7601_NUM_MAC_REG_PARMS);

	/*
		Release BBP and MAC reset
		MAC_SYS_CTRL[1:0] = 0x0
	*/
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacReg);
	MacReg &= ~(0x3);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacReg);

	//InitFce(pAd);

	RTMP_IO_WRITE32(pAd, AUX_CLK_CFG, 0);
}


/*
========================================================================
Routine Description:
	Initialize specific BBP registers.

Arguments:
	pAd					- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
static VOID NICInitMT7601BbpRegisters(
	IN	PRTMP_ADAPTER pAd)
{
	DBGPRINT(RT_DEBUG_TRACE, ("%s\n", __FUNCTION__));

#ifdef MT7601FPGA
	return;
#endif /*MT7601FPGA */

	AndesBBPRandomWritePair(pAd, MT7601_BBP_InitRegTb, MT7601_BBP_InitRegTb_Size);

	return;
}


static VOID MT7601_AsicAntennaDefaultReset(
	IN struct _RTMP_ADAPTER	*pAd,
	IN EEPROM_ANTENNA_STRUC *pAntenna)
{
	DBGPRINT(RT_DEBUG_TRACE, ("%s\n", __FUNCTION__));
	pAntenna->word = 0;
	pAntenna->field.RfIcType = 0xf;
	pAntenna->field.TxPath = 1;
	pAntenna->field.RxPath = 1;
}


static VOID MT7601_ChipBBPAdjust(RTMP_ADAPTER *pAd)
{
#if defined(DOT11_N_SUPPORT) && defined(DBG)
	static char *ext_str[]={"extNone", "extAbove", "", "extBelow"};
#endif
	UCHAR rf_bw, ext_ch;

#ifdef DOT11_N_SUPPORT
	if (get_ht_cent_ch(pAd, &rf_bw, &ext_ch) == FALSE)
#endif /* DOT11_N_SUPPORT */
	{
		rf_bw = BW_20;
		ext_ch = EXTCHA_NONE;
		pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel;
	}

	rtmp_bbp_set_bw(pAd, rf_bw);

	/* TX/Rx : control channel setting */
	rtmp_mac_set_ctrlch(pAd, ext_ch);
	rtmp_bbp_set_ctrlch(pAd, ext_ch);

#ifdef DOT11_N_SUPPORT
	DBGPRINT(RT_DEBUG_TRACE, ("%s() : %s, ChannelWidth=%d, Channel=%d, ExtChanOffset=%d(%d) \n",
					__FUNCTION__, ext_str[ext_ch],
					pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth,
					pAd->CommonCfg.Channel,
					pAd->CommonCfg.RegTransmitSetting.field.EXTCHA,
					pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset));
#endif /* DOT11_N_SUPPORT */
}




VOID MT7601_ChipAGCInit(
	IN PRTMP_ADAPTER		pAd,
	IN UCHAR			BandWidth)
{
	UCHAR R66 = 0x14;
	CHAR lan_gain = GET_LNA_GAIN(pAd);
	
	if (pAd->LatchRfRegs.Channel <= 14) /* BG band */
	{	
		/* Gary was verified Amazon AP and find that RT307x has BBP_R66 invalid default value */

		R66 += 2 * (lan_gain - 8) + 0x20;
		
		rtmp_bbp_set_agc(pAd, R66, RX_CHAIN_ALL);
		
	}

}


static VOID MT7601_ChipSwitchChannel(
	struct _RTMP_ADAPTER *pAd,
	UCHAR Channel,
	BOOLEAN	 bScan)
{

	CHAR TxPwer = 0;
	CHAR CCK1MPwr, CCK11MPwr;
	UCHAR	index;
	UCHAR RFValue = 0;
	UINT32 Value = 0;
	UINT32	value;
	UINT32 ret;
#ifdef SINGLE_SKU_V2
	CHAR SkuBasePwr;
	CHAR ChannelPwrAdj;
#endif /* SINGLE_SKU_V2 */

	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	
	DBGPRINT(RT_DEBUG_TRACE, ("%s: SwitchChannel#%d(RF=%d, %dT)\n",
				__FUNCTION__, Channel, pAd->RfIcType, pAd->Antenna.field.TxPath));

#ifdef MT7601FPGA
	return;
#endif /*MT7601FPGA */

	if (Channel > 14)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("AsicSwitchChannel: Can't find the Channel#%d \n", Channel));
		return;
	}

#ifdef SINGLE_SKU_V2
	SkuBasePwr = GetSkuChannelBasePwr(pAd, Channel);

#ifdef RTMP_INTERNAL_TX_ALC
	if (pAd->TxPowerCtrl.bInternalTxALC != TRUE)
#endif /* RTMP_INTERNAL_TX_ALC */
	{
		UINT32 value;
		if (pAd->DefaultTargetPwr > SkuBasePwr)
			ChannelPwrAdj = SkuBasePwr - pAd->DefaultTargetPwr;
		else
			ChannelPwrAdj = 0;

		if (ChannelPwrAdj > 31)
			ChannelPwrAdj = 31;
		if (ChannelPwrAdj < -32)
			ChannelPwrAdj = -32;

		RTMP_IO_READ32(pAd, TX_ALC_CFG_1, &value);
		value = (value & ~0x3F) | (ChannelPwrAdj & 0x3F);
		RTMP_IO_WRITE32(pAd, TX_ALC_CFG_1, value);
		DBGPRINT(RT_DEBUG_TRACE, ("SkuBasePwr = 0x%x, DefaultTargetPwr = 0x%x, ChannelPwrAdj 0x13b4: 0x%x\n", SkuBasePwr, pAd->DefaultTargetPwr, value));
	}

#ifdef RTMP_INTERNAL_TX_ALC
	if (pAd->TxPowerCtrl.bInternalTxALC)
	{
		TxPwer = SkuBasePwr;
		pAd->TxPower[Channel - 1].Power = TxPwer;
	}
	else
#endif /* RTMP_INTERNAL_TX_ALC */
#endif /* SINGLE_SKU_V2 */
	TxPwer = pAd->TxPower[Channel - 1].Power;

#ifdef RTMP_MAC_USB
	if (IS_USB_INF(pAd)) {
		RTMP_SEM_EVENT_WAIT(&pAd->hw_atomic, ret);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("reg_atomic get failed(ret=%d)\n", ret));
			return;
		}
	}
#endif /* RTMP_MAC_USB */

	for (index = 0; index < NUM_OF_MT7601_CHNL; index++)
	{
		if (Channel == MT7601_Frequency_Plan[index].Channel)
		{		
			/* Frequeny plan setting */
			AndesRFRandomWrite(pAd, 4,
				RF_BANK0, RF_R17, MT7601_Frequency_Plan[index].K_R17,
				RF_BANK0, RF_R18, MT7601_Frequency_Plan[index].K_R18,
				RF_BANK0, RF_R19, MT7601_Frequency_Plan[index].K_R19,
				RF_BANK0, RF_R20, MT7601_Frequency_Plan[index].N_R20);
		}
	}

	RTMP_IO_READ32(pAd, TX_ALC_CFG_0, &Value);
	Value = Value & (~0x3F3F);
	Value |= (TxPwer & 0x3F);
	RTMP_IO_WRITE32(pAd, TX_ALC_CFG_0, Value);

	pAd->LatchRfRegs.Channel = Channel; /* Channel latch */

	/* BBP setting */
	if (Channel <= 14)
	{
		AndesBBPRandomWrite(pAd, 3,
			BBP_R62, (0x37 - GET_LNA_GAIN(pAd)),
			BBP_R63, (0x37 - GET_LNA_GAIN(pAd)),
			BBP_R64, (0x37 - GET_LNA_GAIN(pAd)));
		//RtmpUpdateFilterCoefficientControl(pAd, Channel);
	}

	/*
		vcocal_en (initiate VCO calibration (reset after completion)) - It should be at the end of RF configuration. 
	*/
	rlt_rf_write(pAd, RF_BANK0, RF_R04, 0x0A);
	rlt_rf_write(pAd, RF_BANK0, RF_R05, 0x20);
	rlt_rf_read(pAd, RF_BANK0, RF_R04, &RFValue);
	RFValue = RFValue | 0x80;
	rlt_rf_write(pAd, RF_BANK0, RF_R04, RFValue);
	RTMPusecDelay(2000);

	rtmp_bbp_set_bw(pAd, pAd->CommonCfg.BBPCurrentBW);

	switch (pAd->CommonCfg.BBPCurrentBW)
	{
		case BW_20:
			if (pChipCap->TemperatureMode == TEMPERATURE_MODE_HIGH)
				AndesBBPRandomWritePair(pAd, MT7601_BBP_HighTempBW20RegTb, MT7601_BBP_HighTempBW20RegTb_Size);
			else if (pChipCap->TemperatureMode == TEMPERATURE_MODE_LOW)
				AndesBBPRandomWritePair(pAd, MT7601_BBP_LowTempBW20RegTb, MT7601_BBP_LowTempBW20RegTb_Size);
			else
				AndesBBPRandomWritePair(pAd, MT7601_BBP_BW20RegTb, MT7601_BBP_BW20RegTb_Size);

			/* Tx Filter BW */
			AndesCalibrationOP(pAd, ANDES_CALIBRATION_BW, 0x10001);
			/* Rx Filter BW */
			AndesCalibrationOP(pAd, ANDES_CALIBRATION_BW, 0x10000);
			break;
		case BW_40:
			if (pChipCap->TemperatureMode == TEMPERATURE_MODE_HIGH)
				AndesBBPRandomWritePair(pAd, MT7601_BBP_HighTempBW40RegTb, MT7601_BBP_HighTempBW40RegTb_Size);
			else if (pChipCap->TemperatureMode == TEMPERATURE_MODE_LOW)
				AndesBBPRandomWritePair(pAd, MT7601_BBP_LowTempBW40RegTb, MT7601_BBP_LowTempBW40RegTb_Size);
			else
				AndesBBPRandomWritePair(pAd, MT7601_BBP_BW40RegTb, MT7601_BBP_BW40RegTb_Size);

			/* Tx Filter BW */
			AndesCalibrationOP(pAd, ANDES_CALIBRATION_BW, 0x10101);
			/* Rx Filter BW */
			AndesCalibrationOP(pAd, ANDES_CALIBRATION_BW, 0x10100);

			break;
		default:			
			break;
	}

#ifdef MICROWAVE_OVEN_SUPPORT
	/* B5.R6 and B5.R7 */
	rlt_rf_read(pAd, RF_BANK5, RF_R06, &RFValue);
	pAd->CommonCfg.MO_Cfg.Stored_RF_B5_R6 = RFValue;
	rlt_rf_read(pAd, RF_BANK5, RF_R07, &RFValue);
	pAd->CommonCfg.MO_Cfg.Stored_RF_B5_R7 = RFValue;
#endif /* MICROWAVE_OVEN_SUPPORT */

	/* CCK CH14 OBW */
	if ((pAd->CommonCfg.BBPCurrentBW == BW_20) && ( Channel == 14))
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, 0x60);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R178, 0x0);

		value = pAd->TxCCKPwrCfg;
		CCK1MPwr = value & 0x3F;
		CCK1MPwr -= 2;
		if (CCK1MPwr < -32)
			CCK1MPwr = -32;
		CCK11MPwr = (value & 0x3F00) >> 8;
		CCK11MPwr -= 2;
		if (CCK11MPwr < -32)
			CCK11MPwr = -32;

		value = (value & ~0xFFFF) | (CCK11MPwr << 8) | CCK1MPwr;

		pAd->Tx20MPwrCfgGBand[0] = value;
	}
	else
	{
		UCHAR BBPValue;
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &BBPValue);
		BBPValue &= ~(0x20);
		AndesBBPRandomWrite(pAd, 2,
			BBP_R4, BBPValue,
			BBP_R178, 0xFF);
		pAd->Tx20MPwrCfgGBand[0] = pAd->TxCCKPwrCfg;
	}
	RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0, pAd->Tx20MPwrCfgGBand[0]);


#ifdef RTMP_MAC_USB
	if (IS_USB_INF(pAd))
		RTMP_SEM_EVENT_UP(&pAd->hw_atomic);
#endif /* RTMP_MAC_USB */

#ifdef SINGLE_SKU_V2
		UpdateSkuRatePwr(pAd, Channel, pAd->CommonCfg.BBPCurrentBW, SkuBasePwr);
#endif /* SINGLE_SKU_V2 */

}


NTSTATUS MT7601DisableTxRx(
	RTMP_ADAPTER *pAd,
	UCHAR Level)
{
	UINT32 MacReg = 0;
	UINT32 MTxCycle;
	BOOLEAN bResetWLAN = FALSE;
	BOOLEAN bFree = TRUE;
	UINT8 CheckFreeTimes = 0;
	UINT32 MaxRetry;

	if (!IS_MT7601(pAd))
		return FALSE;

	DBGPRINT(RT_DEBUG_TRACE, ("----> %s\n", __FUNCTION__));

	if (Level == DOT11POWERSAVE)
		MaxRetry = 20;
	else
		MaxRetry = 2000;

	if (Level == RTMP_HALT)
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_ACTIVE);

	DBGPRINT(RT_DEBUG_INFO, ("%s Tx success = %ld\n", 
		__FUNCTION__, (ULONG)pAd->WlanCounters.TransmittedFragmentCount.u.LowPart));
	DBGPRINT(RT_DEBUG_INFO, ("%s Tx success = %ld\n", 
		__FUNCTION__, (ULONG)pAd->WlanCounters.ReceivedFragmentCount.QuadPart));

	if (StopDmaTx(pAd, Level) == STATUS_UNSUCCESSFUL)
		return STATUS_UNSUCCESSFUL;

	/*
		Check page count in TxQ,
	*/
	for (MTxCycle = 0; MTxCycle < MaxRetry; MTxCycle++)
	{
		BOOLEAN bFree = TRUE;
		RTMP_IO_READ32(pAd, 0x438, &MacReg);
		if (MacReg != 0)
			bFree = FALSE;
		RTMP_IO_READ32(pAd, 0xa30, &MacReg);
		if (MacReg & 0x000000FF)
			bFree = FALSE;
		RTMP_IO_READ32(pAd, 0xa34, &MacReg);
		if (MacReg & 0xFF00FF00)
			bFree = FALSE;
		if (bFree)
			break;
		RtmpOsMsDelay(10);
		if (MacReg == 0xFFFFFFFF)
		{
			//RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST);
			return STATUS_UNSUCCESSFUL;
		}
	}

	if (MTxCycle >= MaxRetry)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Check TxQ page count max\n"));
		RTMP_IO_READ32(pAd, 0x0a30, &MacReg);
		DBGPRINT(RT_DEBUG_TRACE, ("0x0a30 = 0x%08x\n", MacReg));

		RTMP_IO_READ32(pAd, 0x0a34, &MacReg);
		DBGPRINT(RT_DEBUG_TRACE, ("0x0a34 = 0x%08x\n", MacReg));

		RTMP_IO_READ32(pAd, 0x438, &MacReg);
		DBGPRINT(RT_DEBUG_TRACE, ("0x438 = 0x%08x\n", MacReg));
		bResetWLAN = TRUE;

		if (Level == DOT11POWERSAVE)
			return STATUS_UNSUCCESSFUL;
	}

	/*
		Check MAC Tx idle
	*/
	for (MTxCycle = 0; MTxCycle < MaxRetry; MTxCycle++)
	{
		RTMP_IO_READ32(pAd, MAC_STATUS_CFG, &MacReg);
		if (MacReg & 0x1)
			RTMPusecDelay(50);
		else
			break;

		if (MacReg == 0xFFFFFFFF)
		{
			//RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST);
			return STATUS_UNSUCCESSFUL;
		}
	}

	if (MTxCycle >= MaxRetry)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Check MAC Tx idle max(0x%08x)\n", MacReg));
		bResetWLAN = TRUE;

		if (Level == DOT11POWERSAVE)
			return STATUS_UNSUCCESSFUL;
	}
	
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST) == FALSE)
	{
		if (Level == RTMP_HALT)
		{
			/*
				Disable MAC TX/RX
			*/
			RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacReg);
			MacReg &= ~(0x0000000c);
			RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacReg);
		}
		else
		{
			/*
				Disable MAC RX
			*/
			RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacReg);
			MacReg &= ~(0x00000008);
			//MacReg &= ~(0x0000000c);
			RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacReg);
		}
	}

	/*
		Check page count in RxQ,
	*/
	for (MTxCycle = 0; MTxCycle < MaxRetry; MTxCycle++) {
		bFree = TRUE;
		RTMP_IO_READ32(pAd, 0x430, &MacReg);

		if (MacReg & (0x00FF0000))
			bFree = FALSE;

		RTMP_IO_READ32(pAd, 0xa30, &MacReg);

		if (MacReg != 0)
			bFree = FALSE;

		RTMP_IO_READ32(pAd, 0xa34, &MacReg);

		if (MacReg != 0)
			bFree = FALSE;

		if (bFree && (CheckFreeTimes > 5)) //&& (!IsInBandCmdProcessing(pAd)))
			break;

		if (bFree)
			CheckFreeTimes++;

		if (MacReg == 0xFFFFFFFF) {
			//RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST);
			return STATUS_UNSUCCESSFUL;
		}
#ifdef RTMP_MAC_USB
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_POLL_IDLE);
		RTUSBBulkCmdRspEventReceive(pAd);
		RTUSBBulkReceive(pAd);
#endif
	}

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_POLL_IDLE);

	if (MTxCycle >= MaxRetry) {
		DBGPRINT(RT_DEBUG_ERROR, ("Check RxQ page count max\n"));

		RTMP_IO_READ32(pAd, 0x0a30, &MacReg);
		DBGPRINT(RT_DEBUG_TRACE, ("0x0a30 = 0x%08x\n", MacReg));

		RTMP_IO_READ32(pAd, 0x0a34, &MacReg);
		DBGPRINT(RT_DEBUG_TRACE, ("0x0a34 = 0x%08x\n", MacReg));

		RTMP_IO_READ32(pAd, 0x0430, &MacReg);
		DBGPRINT(RT_DEBUG_TRACE, ("0x0430 = 0x%08x\n", MacReg));
		bResetWLAN = TRUE;

		if (Level == DOT11POWERSAVE)
			return STATUS_UNSUCCESSFUL;
	}

	/*
		Check MAC Rx idle
	*/
	for (MTxCycle = 0; MTxCycle < MaxRetry; MTxCycle++) {
		RTMP_IO_READ32(pAd, MAC_STATUS_CFG, &MacReg);
		if (MacReg & 0x2)
			RTMPusecDelay(50);
		else
			break;
		if (MacReg == 0xFFFFFFFF)
		{
			//RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST);
			return STATUS_UNSUCCESSFUL;
		}
	}

	if (MTxCycle >= MaxRetry) {
		DBGPRINT(RT_DEBUG_ERROR, ("Check MAC Rx idle max(0x%08x)\n", MacReg));
		bResetWLAN = TRUE;

		if (Level == DOT11POWERSAVE)
			return STATUS_UNSUCCESSFUL;
	}

	if (StopDmaRx(pAd, Level) == STATUS_UNSUCCESSFUL)
		return STATUS_UNSUCCESSFUL;

	if ((Level == RTMP_HALT) &&
		(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST) == FALSE)) {
		if (!pAd->chipCap.IsComboChip)
			NICEraseFirmware(pAd);
		/*
			Disable RF/MAC
		*/

		if ((pAd->chipCap.IsComboChip) || RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_SUSPEND)
				|| RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_CMD_RADIO_OFF))
			bResetWLAN = 0;

		MT7601_WLAN_ChipOnOff(pAd, FALSE, bResetWLAN);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("<---- %s\n", __FUNCTION__));

	return STATUS_SUCCESS;
}


#ifdef RTMP_USB_SUPPORT
VOID MT7601UsbAsicRadioOff(RTMP_ADAPTER *pAd, UCHAR Stage)
{
#ifdef RTMP_MAC_USB
	UINT32 ret;
#endif
	DBGPRINT(RT_DEBUG_TRACE, ("--> %s\n", __FUNCTION__));

	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF))
		return;

	if (Stage == SUSPEND_RADIO_OFF)
		MT7601DisableTxRx(pAd, RTMP_HALT);
	else {
		if (MT7601DisableTxRx(pAd, DOT11POWERSAVE) == STATUS_UNSUCCESSFUL) {
			DBGPRINT(RT_DEBUG_TRACE, ("Give up radio off!\n"));
			return;
		}
	}

#ifdef RTMP_MAC_USB
	if (IS_USB_INF(pAd)) {
		RTMP_SEM_EVENT_WAIT(&pAd->hw_atomic, ret);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("reg_atomic get failed(ret=%d)\n", ret));
			RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);
			return;
		}
	}
#endif /* RTMP_MAC_USB */

	RTMP_SET_PSFLAG(pAd, fRTMP_PS_MCU_SLEEP);

	if (Stage != SUSPEND_RADIO_OFF)
		PWR_SAVING_OP(pAd, RADIO_OFF, 1, 0, 0, 0, 0);

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF);

	/* Stop bulkin pipe*/
	if((!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))) {
		RTUSBCancelPendingBulkInIRP(pAd);
		//pAd->PendingRx = 0;
	}

#ifdef RTMP_MAC_USB
	if (IS_USB_INF(pAd))
		RTMP_SEM_EVENT_UP(&pAd->hw_atomic);
#endif /* RTMP_MAC_USB */

	DBGPRINT(RT_DEBUG_TRACE, ("<-- %s\n", __FUNCTION__));
}


VOID MT7601UsbAsicRadioOn(RTMP_ADAPTER *pAd, UCHAR Stage)
{
	UINT32 MACValue = 0;
	UINT32 rx_filter_flag;
	UCHAR RFValue = 0;

	DBGPRINT(RT_DEBUG_TRACE, ("==> %s\n", __FUNCTION__));

	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF))
		return;

	RTMP_CLEAR_PSFLAG(pAd, fRTMP_PS_MCU_SLEEP);

#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;

	DBGPRINT(RT_DEBUG_TRACE, ("--> %s\n", __FUNCTION__));

	if ((RTMP_Usb_AutoPM_Get_Interface(pObj->pUsb_Dev,pObj->intf)) == 1){
		DBGPRINT(RT_DEBUG_TRACE, ("%s: autopm_resume success\n", __FUNCTION__));
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
	} else if ((RTMP_Usb_AutoPM_Get_Interface(pObj->pUsb_Dev,pObj->intf)) == (-1)) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: autopm_resume fail ------\n", __FUNCTION__));
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
		return;
	} else
		DBGPRINT(RT_DEBUG_TRACE, ("%s: autopm_resume do nothing\n", __FUNCTION__));

#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */

	if (pAd->WlanFunCtrl.field.WLAN_EN == 0)
		MT7601_WLAN_ChipOnOff(pAd, TRUE, FALSE);

	/* make some traffic to invoke EvtDeviceD0Entry callback function*/
	RTUSBReadMACRegister(pAd,0x1000, &MACValue);
	//DBGPRINT(RT_DEBUG_TRACE,("A MAC query to invoke EvtDeviceD0Entry, MACValue = 0x%x\n",MACValue));

	/* enable RX of MAC block*/
#ifdef XLINK_SUPPORT
	if (pAd->StaCfg.PSPXlink)
		rx_filter_flag = PSPXLINK;
	else
#endif /* XLINK_SUPPORT */
		rx_filter_flag = STANORMAL; /* Staion not drop control frame will fail WiFi Certification.*/

	RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, rx_filter_flag);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0xc);

	/* 4. Clear idle flag*/
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF);
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);

	/* Send Bulkin IRPs after flag fRTMP_ADAPTER_IDLE_RADIO_OFF is cleared.*/
	RTUSBBulkCmdRspEventReceive(pAd);
	RTUSBBulkReceive(pAd);

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);

	rlt_rf_write(pAd, RF_BANK0, RF_R04, 0x0A);
	rlt_rf_write(pAd, RF_BANK0, RF_R05, 0x20);

	rlt_rf_read(pAd, RF_BANK0, RF_R04, &RFValue);
	RFValue = RFValue | 0x80;
	rlt_rf_write(pAd, RF_BANK0, RF_R04, RFValue);
	RTMPusecDelay(2000);


	DBGPRINT(RT_DEBUG_TRACE, ("<== %s\n", __FUNCTION__));
}
#endif /* RTMP_USB_SUPPORT */




VOID MT7601_NICInitAsicFromEEPROM(
	IN PRTMP_ADAPTER		pAd)
{
	// TODO: wait TC6008 EEPROM format
}


INT MT7601_ReadChannelPwr(RTMP_ADAPTER *pAd)
{
	UINT32 i, idx, ss_offset_g, MacReg;
	EEPROM_TX_PWR_STRUC Power;
	CHAR tx_pwr1, tx_pwr2;
	CHAR max_tx1_pwr;
	UINT16 TargetPwr = 0;
#ifdef RTMP_INTERNAL_TX_ALC
	EEPROM_NIC_CONFIG2_STRUC NicConfig2;
#endif /* RTMP_INTERNAL_TX_ALC */

	DBGPRINT(RT_DEBUG_TRACE, ("%s()--->\n", __FUNCTION__));

	RTMP_IO_READ32(pAd, TX_ALC_CFG_0, &MacReg);
	max_tx1_pwr = (MacReg >> 16) & 0x3F;

#if defined (RTMP_INTERNAL_TX_ALC) || defined (SINGLE_SKU_V2)
	RT28xx_EEPROM_READ16(pAd, EEPROM_G_TARGET_POWER, TargetPwr);
	tx_pwr1 = TargetPwr & 0xFF;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: EEPROM 0xD0 = 0x%x\n", __FUNCTION__, tx_pwr1));

	if ((tx_pwr1 == 0x0) || (tx_pwr1 > max_tx1_pwr)) {
		tx_pwr1 = 0x20;
		DBGPRINT(RT_DEBUG_ERROR, ("%s: EEPROM 0xD0 Error! Use Default Target Power = 0x%x\n", __FUNCTION__, tx_pwr1));
	}
#endif /* defined (RTMP_INTERNAL_TX_ALC) || defined (SINGLE_SKU_V2) */

#ifdef SINGLE_SKU_V2
	pAd->DefaultTargetPwr = tx_pwr1;
#endif /* SINGLE_SKU_V2 */

	/* Read Tx power value for all channels*/
	/* Value from 1 - 0x7f. Default value is 24.*/
	/* Power value : 2.4G 0x00 (0) ~ 0x1F (31)*/
	/*             : 5.5G 0xF9 (-7) ~ 0x0F (15)*/

#ifdef RTMP_INTERNAL_TX_ALC
	NicConfig2.word = pAd->EEPROMDefaultValue[EEPROM_NIC_CFG2_OFFSET];
	if (NicConfig2.field.bInternalTxALC) {

		for (i = 0; i < 7; i++) {
			idx = i * 2;

			pAd->TxPower[idx].Power = tx_pwr1;
			pAd->TxPower[idx + 1].Power = tx_pwr1;

			pAd->TxPower[idx].Channel = i * 2 +1;
			pAd->TxPower[idx + 1].Channel = i * 2 + 2;

			DBGPRINT(RT_DEBUG_TRACE, ("%s: TxPower[%d].Power = 0x%02X, TxPower[%d].Power = 0x%02X\n", 
				__FUNCTION__, 
				i * 2, 
				pAd->TxPower[i * 2].Power, 
				i * 2 + 1, 
				pAd->TxPower[i * 2 + 1].Power));
		}

		return TRUE;
	}
#endif /* RTMP_INTERNAL_TX_ALC */

	/* 0. 11b/g, ch1 - ch 14, 1SS */
	ss_offset_g = EEPROM_G_TX_PWR_OFFSET;

	for (i = 0; i < 7; i++) {
		idx = i * 2;
		RT28xx_EEPROM_READ16(pAd, ss_offset_g + idx, Power.word);

		tx_pwr1 = tx_pwr2 = DEFAULT_TX_POWER;

		if ((Power.field.Byte0 <= max_tx1_pwr) && (Power.field.Byte0 >= 0))
			tx_pwr1 = Power.field.Byte0;

		if ((Power.field.Byte1 <= max_tx1_pwr) || (Power.field.Byte1 >= 0))
			tx_pwr2 = Power.field.Byte1;

		pAd->TxPower[idx].Power = tx_pwr1;
		pAd->TxPower[idx + 1].Power = tx_pwr2;

		pAd->TxPower[idx].Channel = i * 2 +1;
		pAd->TxPower[idx + 1].Channel = i * 2 + 2;

		DBGPRINT(RT_DEBUG_TRACE, ("%s: TxPower[%d].Power = 0x%02X, TxPower[%d].Power = 0x%02X\n", 
			__FUNCTION__, 
			i * 2, 
			pAd->TxPower[i * 2].Power, 
			i * 2 + 1, 
			pAd->TxPower[i * 2 + 1].Power));

	}

	return TRUE;
}


VOID MT7601_ReadTxPwrPerRate(RTMP_ADAPTER *pAd)
{
	UINT32		data, Adata, Gdata;
	USHORT		i, value, value2;
	CHAR		value_1, value_2;
	CHAR		Apwrdelta, Gpwrdelta;
	CHAR		t1,t2;

	/* Get power delta for 20MHz and 40MHz.*/
	DBGPRINT(RT_DEBUG_TRACE, ("Txpower per Rate\n"));
	RT28xx_EEPROM_READ16(pAd, EEPROM_TXPOWER_DELTA, value2);
	Apwrdelta = 0;
	Gpwrdelta = 0;

	if ((value2 & 0xff) != 0xff) {
		if ((value2 & 0x80)) {
			/* Unit = 0.5 dBm, Max is 4 dBm */
			Gpwrdelta = (value2&0x1f);
			if (Gpwrdelta > 8)
				Gpwrdelta = 8;

			if ((value2 & 0x40) == 0)
				Gpwrdelta = -Gpwrdelta;
		}

	} if ((value2 & 0xff00) != 0xff00) {
		if ((value2 & 0x8000)) {
			Apwrdelta = ((value2&0x1f00)>>8);
			if (Apwrdelta > 8)
				Apwrdelta = 8;

			if ((value2 & 0x4000) == 0)
				Apwrdelta = - Apwrdelta;
		}
	}

#ifdef SINGLE_SKU_V2
	pAd->chipCap.Apwrdelta = Apwrdelta;
	pAd->chipCap.Gpwrdelta = Gpwrdelta;
#endif /* SINGLE_SKU_V2 */
	DBGPRINT(RT_DEBUG_TRACE, ("Gpwrdelta = %d, Apwrdelta = %d .\n", Gpwrdelta, Apwrdelta));

	/* Get Txpower per MCS for 20MHz in 2.4G.*/

	for (i = 0; i < 5; i++) {
		RT28xx_EEPROM_READ16(pAd, EEPROM_TXPOWER_BYRATE_20MHZ_2_4G + i*4, value);
		data = value;

		/* signed 6-bit */
		value_1 = (value&0x3f);
		value_2 = (value&0x3f00)>>8;

		/* signed extension */
		value_1 = (value_1 > 0x1F) ? value_1 - 0x40 : value_1;
		value_2 = (value_2 > 0x1F) ? value_2 - 0x40 : value_2;

		t1 = value_1 + Gpwrdelta;
		t2 = value_2 + Gpwrdelta;

		/* boundary check */
		if (t1 > 31)
			t1 = 31;
		if (t1 < -32)
			t1 = -32;

		if (t2 > 31)
			t2 = 31;
		if (t2 < -32)
			t2 = -32;

		Gdata = (t1 & 0x3f) + ((t2 & 0x3f)<<8);

		t1 = value_1 + Apwrdelta;
		t2 = value_2 + Apwrdelta;

		/* boundary check */
		if (t1 > 31)
			t1 = 31;
		if (t1 < -32)
			t1 = -32;

		if (t2 > 31)
			t2 = 31;
		if (t2 < -32)
			t2 = -32;

		Adata = (t1 & 0x3f)  + (( t2 & 0x3f) <<8);

		RT28xx_EEPROM_READ16(pAd, EEPROM_TXPOWER_BYRATE_20MHZ_2_4G + i*4 + 2, value);

		if (i != 4) {
			/* signed 6-bit */
			value_1 = (value&0x3f);
			value_2 = (value&0x3f00)>>8;

			/* signed extension */
			value_1 = (value_1 > 0x1F) ? value_1 - 0x40 : value_1;
			value_2 = (value_2 > 0x1F) ? value_2 - 0x40 : value_2;

			t1 = value_1 + Gpwrdelta;
			t2 = value_2 + Gpwrdelta;

			/* boundary check */
			if (t1 > 31)
				t1 = 31;
			if (t1 < -32)
				t1 = -32;

			if (t2 > 31)
				t2 = 31;
			if (t2 < -32)
				t2 = -32;

			Gdata |= (((t1 & 0x3f) << 16) + ((t2 & 0x3f) << 24));

			t1 = value_1 + Apwrdelta;
			t2 = value_2 + Apwrdelta;

			/* boundary check */
			if (t1 > 31)
				t1 = 31;
			if (t1 < -32)
				t1 = -32;

			if (t2 > 31)
				t2 = 31;
			if (t2 < -32)
				t2 = -32;

			Adata |= (((t1 & 0x3f) <<16) + ((t2 & 0x3f) << 24));
		} else {
			Gdata |= 0xFFFF0000;
			Adata |= 0xFFFF0000;
		}
		data |= (value<<16);

		/* For 20M/40M Power Delta issue */
		pAd->Tx20MPwrCfgABand[i] = data;
		pAd->Tx20MPwrCfgGBand[i] = data;
		pAd->Tx40MPwrCfgABand[i] = Adata;
		pAd->Tx40MPwrCfgGBand[i] = Gdata;

		if (data != 0xffffffff)
			RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + i*4, data);
		DBGPRINT_RAW(RT_DEBUG_TRACE, ("20MHz BW, 2.4G band-%08x,  Adata = %08x,  Gdata = %08x \n", data, Adata, Gdata));
	}

	/* Extra set MAC registers to compensate Tx power if any */
	RTMP_CHIP_ASIC_EXTRA_POWER_OVER_MAC(pAd);

	pAd->TxCCKPwrCfg = pAd->Tx20MPwrCfgGBand[0];

}


VOID MT7601_AsicExtraPowerOverMAC(
	IN	PRTMP_ADAPTER 		pAd)
{
	ULONG	ExtraPwrOverMAC = 0;
	ULONG	ExtraPwrOverTxPwrCfg7 = 0, ExtraPwrOverTxPwrCfg9 = 0;

	/* For OFDM_54 and HT_MCS_7, extra fill the corresponding register value into MAC 0x13D4 */
	RTMP_IO_READ32(pAd, 0x1318, &ExtraPwrOverMAC);
	ExtraPwrOverTxPwrCfg7 |= (ExtraPwrOverMAC & 0x0000FF00) >> 8; /* Get Tx power for OFDM 54 */
	RTMP_IO_READ32(pAd, 0x131C, &ExtraPwrOverMAC);
	ExtraPwrOverTxPwrCfg7 |= (ExtraPwrOverMAC & 0x0000FF00) << 8; /* Get Tx power for HT MCS 7 */
	RTMP_IO_WRITE32(pAd, TX_PWR_CFG_7, ExtraPwrOverTxPwrCfg7);

	/* For STBC_MCS_7, extra fill the corresponding register value into MAC 0x13DC */
	RTMP_IO_READ32(pAd, 0x1324, &ExtraPwrOverMAC);
	ExtraPwrOverTxPwrCfg9 |= (ExtraPwrOverMAC & 0x0000FF00) >> 8; /* Get Tx power for STBC MCS 7 */
	RTMP_IO_WRITE32(pAd, TX_PWR_CFG_9, ExtraPwrOverTxPwrCfg9);

	DBGPRINT(RT_DEBUG_INFO, ("Offset = 0x13D4, TxPwr = 0x%08X, Offset = 0x13DC, TxPwr = 0x%08X\n",
		(UINT)ExtraPwrOverTxPwrCfg7,
		(UINT)ExtraPwrOverTxPwrCfg9));
}


#if defined(RTMP_INTERNAL_TX_ALC) || defined(SINGLE_SKU_V2)
VOID MT7601_InitPAModeTable(
	IN PRTMP_ADAPTER			pAd)
{
	INT32 PAMode;
	UINT32 Value = 0;
	UINT16 index, offset;

	RTMP_IO_READ32(pAd, RF_PA_MODE_CFG0, &Value);
	for (index = 0, offset = 0; index < 4 ; index++, offset += 2) {
		PAMode = (Value >> offset) & 0x3;
		if (PAMode == 3)
			pAd->chipCap.PAModeCCK[index] = RF_PA_MODE3_DECODE;
		else if (PAMode == 1)
			pAd->chipCap.PAModeCCK[index] = RF_PA_MODE1_DECODE;
		else if (PAMode == 0)
			pAd->chipCap.PAModeCCK[index] = RF_PA_MODE0_DECODE;
		else if (PAMode == 2)
			pAd->chipCap.PAModeCCK[index] = RF_PA_MODE2_DECODE;
		DBGPRINT(RT_DEBUG_TRACE, ("PAModeCCK[%d] = %d\n", index, pAd->chipCap.PAModeCCK[index]));
	}

	for (index = 0, offset = 8; index < 8 ; index++, offset += 2) {
		PAMode = (Value >> offset) & 0x3;
		if (PAMode == 3)
			pAd->chipCap.PAModeOFDM[index] = RF_PA_MODE3_DECODE;
		else if (PAMode == 1)
			pAd->chipCap.PAModeOFDM[index] = RF_PA_MODE1_DECODE;
		else if (PAMode == 0)
			pAd->chipCap.PAModeOFDM[index] = RF_PA_MODE0_DECODE;
		else if (PAMode == 2)
			pAd->chipCap.PAModeOFDM[index] = RF_PA_MODE2_DECODE;
		DBGPRINT(RT_DEBUG_TRACE, ("PAModeOFDM[%d] = %d\n", index, pAd->chipCap.PAModeOFDM[index]));
	}

	RTMP_IO_READ32(pAd, RF_PA_MODE_CFG1, &Value);

	for (index = 0, offset = 0; index < 16 ; index++, offset+= 2) {
		PAMode = (Value >> offset) & 0x3;
		if (PAMode == 3)
			pAd->chipCap.PAModeHT[index] = RF_PA_MODE3_DECODE;
		else if (PAMode == 1)
			pAd->chipCap.PAModeHT[index] = RF_PA_MODE1_DECODE;
		else if (PAMode == 0)
			pAd->chipCap.PAModeHT[index] = RF_PA_MODE0_DECODE;
		else if (PAMode == 2)
			pAd->chipCap.PAModeHT[index] = RF_PA_MODE2_DECODE;
		DBGPRINT(RT_DEBUG_TRACE, ("PAModeHT[%d] = %d\n", index, pAd->chipCap.PAModeHT[index]));
	}

}
#endif /* defined(RTMP_INTERNAL_TX_ALC) || defined(SINGLE_SKU_V2) */


VOID MT7601_InitTemperatureCompensation(
	IN PRTMP_ADAPTER			pAd)
{
	UINT32 Value = 0;

	pAd->chipCap.TemperatureMode = TEMPERATURE_MODE_NORMAL;
	pAd->chipCap.CurrentTemperature = 0;
	pAd->chipCap.bPllLockProtect = FALSE;
	RT28xx_EEPROM_READ16(pAd, EEPROM_G_TARGET_POWER, Value);
	pAd->chipCap.TemperatureRef25C = (Value >> 8) & 0xFF;
	DBGPRINT(RT_DEBUG_TRACE, ("pAd->TemperatureRef25C = 0x%x\n", pAd->chipCap.TemperatureRef25C));

}


VOID MT7601_TemperatureCompensation(
	IN PRTMP_ADAPTER			pAd)
{
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;

	if (pAd->Mlme.OneSecPeriodicRound % 4 == 0) {
#ifdef RTMP_INTERNAL_TX_ALC
		if (pAd->TxPowerCtrl.bInternalTxALC == FALSE)
#endif /* RTMP_INTERNAL_TX_ALC */
			MT7601_Read_Temperature(pAd, &pChipCap->CurrentTemperBbpR49);

		MT7601AsicTemperatureCompensation(pAd, FALSE);
	}
}


VOID MT7601AsicTemperatureCompensation(
	IN PRTMP_ADAPTER			pAd,
	IN BOOLEAN				bPowerOn)
{
	INT32	CurrentTemper;
	UCHAR	RfReg;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 high_temp_cr_threshold, low_temp_cr_threshold;

	CurrentTemper = (pChipCap->CurrentTemperBbpR49 - pChipCap->TemperatureRef25C) * MT7601_E2_TEMPERATURE_SLOPE;		// 3.9 * 10
	pChipCap->CurrentTemperature = CurrentTemper;

	/* DPD Calibration */
	if (((CurrentTemper - pChipCap->TemperatureDPD) > 450) || ((CurrentTemper - pChipCap->TemperatureDPD) < -450)) {
		pChipCap->TemperatureDPD = CurrentTemper;

#ifdef DPD_CALIBRATION_SUPPORT
		/* DPD-Calibration */
		AndesCalibrationOP(pAd, ANDES_CALIBRATION_DPD, pChipCap->TemperatureDPD);
#endif /* DPD_CALIBRATION_SUPPORT */

		rlt_rf_write(pAd, RF_BANK0, RF_R04, 0x0A);
		rlt_rf_write(pAd, RF_BANK0, RF_R05, 0x20);
		rlt_rf_read(pAd, RF_BANK0, RF_R04, &RfReg);
		RfReg = RfReg | 0x80;
		rlt_rf_write(pAd, RF_BANK0, RF_R04, RfReg);
		RTMPusecDelay(2000);

		DBGPRINT(RT_DEBUG_TRACE, ("%s::ReCalibration DPD.\n", __FUNCTION__));
	}

	/* PLL Lock Protect */
	if (CurrentTemper < -50) { // ( 20 - 25 ) * 10 = -50
		if (pAd->chipCap.bPllLockProtect == FALSE) {
			pAd->chipCap.bPllLockProtect = TRUE;
			rlt_rf_write(pAd, RF_BANK4, RF_R04, 0x06);

			rlt_rf_read(pAd, RF_BANK4, RF_R10, &RfReg);
			RfReg = RfReg & ~0x30;
			rlt_rf_write(pAd, RF_BANK4, RF_R10, RfReg);

			DBGPRINT(RT_DEBUG_TRACE, ("%s::Enable PLL Lock Protect.\n", __FUNCTION__));
		}
	} else if (CurrentTemper > 50) { // ( 30 - 25 ) * 10 = 50
		if (pAd->chipCap.bPllLockProtect == TRUE) {
			pAd->chipCap.bPllLockProtect = FALSE;
			rlt_rf_write(pAd, RF_BANK4, RF_R04, 0x0);

			rlt_rf_read(pAd, RF_BANK4, RF_R10, &RfReg);
			RfReg = (RfReg & ~0x30) | 0x10;
			rlt_rf_write(pAd, RF_BANK4, RF_R10, RfReg);

			DBGPRINT(RT_DEBUG_TRACE, ("%s::Disable PLL Lock Protect.\n", __FUNCTION__));
		}
	}

	if (bPowerOn) {
		high_temp_cr_threshold = 350;
		low_temp_cr_threshold = -250;
	} else {
		high_temp_cr_threshold = 400;
		low_temp_cr_threshold = -200;
	}

	/* BBP CR for H, L, N temperature */
	if (CurrentTemper > high_temp_cr_threshold) { // (60 - 25) * 10 = 350
		if (pChipCap->TemperatureMode != TEMPERATURE_MODE_HIGH) {
			pChipCap->TemperatureMode = TEMPERATURE_MODE_HIGH;

			AndesBBPRandomWritePair(pAd, MT7601_BBP_HighTempCommonRegTb, MT7601_BBP_HighTempCommonRegTb_Size);

			if (pAd->CommonCfg.BBPCurrentBW == BW_20)
				AndesBBPRandomWritePair(pAd, MT7601_BBP_HighTempBW20RegTb, MT7601_BBP_HighTempBW20RegTb_Size);
			else if (pAd->CommonCfg.BBPCurrentBW == BW_40)
				AndesBBPRandomWritePair(pAd, MT7601_BBP_HighTempBW40RegTb, MT7601_BBP_HighTempBW40RegTb_Size);
			else
				DBGPRINT(RT_DEBUG_ERROR, ("%s::Unsupported BW(%x)\n", __FUNCTION__, pAd->CommonCfg.BBPCurrentBW));

			DBGPRINT(RT_DEBUG_TRACE, ("%s::Change to TEMPERATURE_MODE_HIGH\n", __FUNCTION__));
		}
	} else if (CurrentTemper > low_temp_cr_threshold) { // ( 0 - 25 ) * 10
		if (pChipCap->TemperatureMode != TEMPERATURE_MODE_NORMAL) {
			pChipCap->TemperatureMode = TEMPERATURE_MODE_NORMAL;

			AndesBBPRandomWritePair(pAd, MT7601_BBP_CommonRegTb, MT7601_BBP_CommonRegTb_Size);

			if (pAd->CommonCfg.BBPCurrentBW == BW_20)
				AndesBBPRandomWritePair(pAd, MT7601_BBP_BW20RegTb, MT7601_BBP_BW20RegTb_Size);
			else if (pAd->CommonCfg.BBPCurrentBW == BW_40)
				AndesBBPRandomWritePair(pAd, MT7601_BBP_BW40RegTb, MT7601_BBP_BW40RegTb_Size);
			else
				DBGPRINT(RT_DEBUG_ERROR, ("%s::Unsupported BW(%x)\n", __FUNCTION__, pAd->CommonCfg.BBPCurrentBW));

			DBGPRINT(RT_DEBUG_TRACE, ("%s::Change to TEMPERATURE_MODE_NORMAL\n", __FUNCTION__));
		}
	} else {
		if (pChipCap->TemperatureMode != TEMPERATURE_MODE_LOW) {
			pChipCap->TemperatureMode = TEMPERATURE_MODE_LOW;

			AndesBBPRandomWritePair(pAd, MT7601_BBP_LowTempCommonRegTb, MT7601_BBP_LowTempCommonRegTb_Size);

			if (pAd->CommonCfg.BBPCurrentBW == BW_20)
				AndesBBPRandomWritePair(pAd, MT7601_BBP_LowTempBW20RegTb, MT7601_BBP_LowTempBW20RegTb_Size);
			else if (pAd->CommonCfg.BBPCurrentBW == BW_40)
				AndesBBPRandomWritePair(pAd, MT7601_BBP_LowTempBW40RegTb, MT7601_BBP_LowTempBW40RegTb_Size);
			else
				DBGPRINT(RT_DEBUG_ERROR, ("%s::Unsupported BW(%x)\n", __FUNCTION__, pAd->CommonCfg.BBPCurrentBW));

			DBGPRINT(RT_DEBUG_TRACE, ("%s::Change to TEMPERATURE_MODE_LOW\n", __FUNCTION__));
		}
	}
}


#ifdef RTMP_INTERNAL_TX_ALC
INT16 lin2dBd(UINT16 linearValue)
{
	short exp;
	unsigned int mantisa;
	int app,dBd;
	// Default backoff ; to enhance leading bit searching time
	mantisa = linearValue << DEFAULT_BO;
	exp = -(DEFAULT_BO);
	// Leading bit searching
	if (mantisa < (0x8000)) {
		while (mantisa < (0x8000)) {
			mantisa = mantisa << 1; // no need saturation
			exp--;
			if (exp < -20) {
				//printf("error, input too small");
				//printf("exponent = %d\n",exp);
				return LIN2DB_ERROR_CODE;
			}
		}
	} else {
		while (mantisa > (0xFFFF)) {
			mantisa = mantisa >> 1; // no need saturation
			exp ++;
			if (exp > 20) {
				//printf("error, input too large");
				//printf("exponent = %d\n",exp);
				return LIN2DB_ERROR_CODE;
			}
		}
	}
	//printf("exp=0d%d,mantisa=0x%x\n",exp,mantisa);

	if (mantisa <= 47104) {
		app = (mantisa + (mantisa >> 3) + (mantisa >> 4) - 38400); //S(15,0)
		if (app < 0)
			app = 0;
	} else {
		app = (mantisa - (mantisa >> 3) - (mantisa >> 6) - 23040); //S(15,0)
		if (app < 0)
			app = 0;
	}

	dBd = ((15 + exp) << 15) + app; //since 2^15=1 here
	//printf("dBd1=%d\n",dBd);
	dBd = (dBd << 2) + (dBd << 1) + (dBd >> 6) + (dBd>>7);
	dBd = (dBd >> 10); //S10.5
	//printf("app=%d,dBd=%d,dBdF=%f\n",app,dBd,(double)dBd/32);
	return(dBd);
}


VOID MT7601_EnableTSSI(IN PRTMP_ADAPTER pAd)
{
	MT7601_TX_ALC_DATA *pTxALCData = &pAd->chipCap.TxALCData;

	AndesFunSetOP(pAd, 5, pTxALCData->TSSI_USE_HVGA);

}


VOID MT7601_TssiDcGainCalibration(RTMP_ADAPTER *pAd)
{
	UCHAR Rf_B5_R03, Rf_B4_R39, bbp_r47;
	INT i, count;
	UCHAR RValue;
	INT16 tssi_linear;
	INT16 tssi0_db = 0, tssi0_db_hvga = 0;
	MT7601_TX_ALC_DATA *pTxALCData = &pAd->chipCap.TxALCData;

	/* Mac Bypass */
	RTMP_IO_WRITE32(pAd, 0x50C, 0x30);
	RTMP_IO_WRITE32(pAd, 0x504, 0xC0030);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x0);

	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R58, 0x0);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R241, 0x2);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R23, 0x8);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R47, &bbp_r47);


	/* Set VGA gain */
	rlt_rf_read(pAd, RF_BANK5, RF_R03, &Rf_B5_R03);
	rlt_rf_write(pAd, RF_BANK5, RF_R03, 0x8);

	/* Mixer disable */
	rlt_rf_read(pAd, RF_BANK4, RF_R39, &Rf_B4_R39);
	rlt_rf_write(pAd, RF_BANK4, RF_R39, 0x0);

	for (i = 0; i < 4; i++) {
		if ((i == 0) || (i == 2))
			rlt_rf_write(pAd, RF_BANK4, RF_R39, 0x0); //disable PA
		else
			rlt_rf_write(pAd, RF_BANK4, RF_R39, Rf_B4_R39); // enable PA

		if (i < 2) {
 		 	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R23, 0x8);
			rlt_rf_write(pAd, RF_BANK5, RF_R03, 0x8);
		} else {
 		 	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R23, 0x2);
			rlt_rf_write(pAd, RF_BANK5, RF_R03, 0x11);
		}

		/* BBP TSSI initial and soft reset */
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22  , 0x0);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R244, 0x0);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R21, 0x1);
		RTMPusecDelay(1);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R21, 0x0);

		/* TSSI measurement */
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, 0x50);
		if ((i == 0) || (i == 2))
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, 0x40);		// enable dc
		else
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R244, 0x31);	// enable ton gen

		for (count = 0; count < 20; count++) {
			//RTMPusecDelay(100);
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R47, &RValue);

			if ((RValue & 0x10) == 0x00)
				break;
		}

		if (count == 20)
			DBGPRINT(RT_DEBUG_ERROR, ("TssiDC0 Read Fail!\n"));

		/* TSSI read */
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, 0x40);
		if (i == 0)
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, &pTxALCData->TssiDC0);
		else if (i == 2)
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, &pTxALCData->TssiDC0_HVGA);
		else
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, (PUCHAR)&tssi_linear);

		tssi_linear = tssi_linear & 0xff;
		tssi_linear = (tssi_linear & 0x80) ? tssi_linear - 0x100 : tssi_linear;

		if (i == 1)
			tssi0_db = lin2dBd(tssi_linear - pTxALCData->TssiDC0);
		else if (i == 3) {
			tssi_linear = tssi_linear - pTxALCData->TssiDC0_HVGA;
			tssi_linear = tssi_linear * 4;
			tssi0_db_hvga = lin2dBd(tssi_linear);
		}
	}

	pTxALCData->TSSI0_DB = tssi0_db;
	DBGPRINT(RT_DEBUG_TRACE, ("tssi0_db_hvga = %x\n", tssi0_db_hvga));
	DBGPRINT(RT_DEBUG_TRACE, ("tssi0_db = %x\n", tssi0_db));


	pTxALCData->TSSI_DBOFFSET_HVGA = tssi0_db_hvga - tssi0_db;
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, 0x0);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R244, 0x0);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R21, 0x1);
	RTMPusecDelay(1);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R21, 0x0);
	RTMP_IO_WRITE32(pAd, 0x504, 0x0);
	RTMP_IO_WRITE32(pAd, 0x50C, 0x0);
	rlt_rf_write(pAd, RF_BANK5, RF_R03, Rf_B5_R03);
	rlt_rf_write(pAd, RF_BANK4, RF_R39, Rf_B4_R39);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, bbp_r47);
	// -(tssi0_db*slope +tssi0_db) / 4096
	DBGPRINT(RT_DEBUG_TRACE, ("TssiDC0 = %d (0x%x)\n", pTxALCData->TssiDC0, pTxALCData->TssiDC0));
	DBGPRINT(RT_DEBUG_TRACE, ("TssiDC0_HVGA = %d (0x%x)\n", pTxALCData->TssiDC0_HVGA, pTxALCData->TssiDC0));
	DBGPRINT(RT_DEBUG_TRACE, ("TSSI_DBOFFSET_HVGA = %x\n", pTxALCData->TSSI_DBOFFSET_HVGA));

}


VOID MT7601_InitDesiredTSSITable(
	IN PRTMP_ADAPTER			pAd)
{
	UINT32 Value = 0;
#ifdef DBG
	UINT16 offset;
#endif /* DBG */
	INT32 init_offset;
	MT7601_TX_ALC_DATA *pTxALCData = &pAd->chipCap.TxALCData;


	if (pAd->TxPowerCtrl.bInternalTxALC == FALSE)
		return;

	/* TSSI Slope in EEPROM 0x6E u.2.6 */
	RT28xx_EEPROM_READ16(pAd, EEPROM_TX0_TSSI_SLOPE, Value);
	pTxALCData->TssiSlope = Value & 0xFF;

	/* TSSI Offset Channel 1 ~ 4 in EEPROF 0x6F s.3.4 */
	pTxALCData->MT7601_TSSI_OFFSET[0] = (Value & 0xFF00) >> 8;
	RT28xx_EEPROM_READ16(pAd, EEPROM_TX0_TSSI_OFFSET_GROUP1, Value);
	pTxALCData->MT7601_TSSI_OFFSET[1] = Value & 0xFF;
	pTxALCData->MT7601_TSSI_OFFSET[2] = (Value & 0xFF00) >> 8;

#ifdef DOT11_N_SUPPORT
	if(pAd->TxPower[pAd->CommonCfg.CentralChannel-1].Power <= 20)
		pTxALCData->TSSI_USE_HVGA = 1;
	else
		pTxALCData->TSSI_USE_HVGA = 0;
#else
	if(pAd->TxPower[pAd->CommonCfg.Channel-1].Power <= 20)
		pTxALCData->TSSI_USE_HVGA = 1;
	else
		pTxALCData->TSSI_USE_HVGA = 0;
#endif /* DOT11_N_SUPPORT */

	DBGPRINT(RT_DEBUG_TRACE, ("TSSI slope = 0x%x, offset[0] = 0x%x, offset[1] = 0x%x, offset[2] = 0x%x, TSSI_USE_HVGA = %x\n",
		pTxALCData->TssiSlope, pTxALCData->MT7601_TSSI_OFFSET[0], pTxALCData->MT7601_TSSI_OFFSET[1], 
		pTxALCData->MT7601_TSSI_OFFSET[2], pTxALCData->TSSI_USE_HVGA));

	RT28xx_EEPROM_READ16(pAd, EEPROM_TX0_TSSI_OFFSET, Value);
	pTxALCData->MT7601_TSSI_T0_Delta_Offset = ((CHAR)(Value & 0xFF)) * 1024;
	DBGPRINT(RT_DEBUG_TRACE, ("TSSI T0 Delta offset = %d\n", pTxALCData->MT7601_TSSI_T0_Delta_Offset));

	RTMP_IO_READ32(pAd, TX_ALC_CFG_1, &Value);
	init_offset = ( (pTxALCData->TSSI0_DB * pTxALCData->TssiSlope) + pTxALCData->MT7601_TSSI_OFFSET[1]) / 4096;
	init_offset = -(init_offset-10);

	if(init_offset < -0x20)
		init_offset = 0x20;
	else if(init_offset > 0x1f)
		init_offset = 0x1f;

	Value = (Value & ~0x3F) | (init_offset & 0x3F);
	pTxALCData->InitTxAlcCfg1 = Value;
	RTMP_IO_WRITE32(pAd, TX_ALC_CFG_1, Value);
	DBGPRINT(RT_DEBUG_TRACE, ("Init MAC 13b4: 0x%x\n", Value));

	MT7601_InitPAModeTable(pAd);
}


BOOLEAN MT7601_GetTssiCompensationParam(
	IN 		PRTMP_ADAPTER 		pAd, 
	OUT 	PCHAR 				TssiLinear0,
	OUT 	PCHAR 				TssiLinear1, 
	OUT 	PINT32 				TargetPower)
{
	UCHAR BBPReg;
	UCHAR PacketType;
	UCHAR BbpR47;
	UCHAR BBPR4, BBPR178;
	UCHAR TxRate;
	INT32 Power;
	MT7601_TX_ALC_DATA *pTxALCData = &pAd->chipCap.TxALCData;

	if (pTxALCData->TssiTriggered == 0) {
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD)) {
			MT7601_EnableTSSI(pAd);
			pTxALCData->TssiTriggered = 1;
		}

		return FALSE;
	}

	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R47, &BBPReg);
	if (BBPReg & 0x10)
		return FALSE;

	/* 4. Read TSSI */
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R47, &BbpR47);
	BbpR47 = (BbpR47 & ~0x07);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, BbpR47);
	RTMPusecDelay(500);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, TssiLinear0);


	/* 5. Read Temperature */
	BbpR47 = (BbpR47 & ~0x07) | 0x04;
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, BbpR47);
	RTMPusecDelay(500);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, &(pAd->chipCap.CurrentTemperBbpR49));

	BbpR47 = (BbpR47 & ~0x07) | 0x01;
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, BbpR47);
	RTMPusecDelay(500);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, &PacketType);

	DBGPRINT(RT_DEBUG_INFO, ("TSSI = 0x%X\n", *TssiLinear0));
	DBGPRINT(RT_DEBUG_INFO, ("temperature = 0x%X\n", pAd->chipCap.CurrentTemperBbpR49));
	DBGPRINT(RT_DEBUG_INFO, ("PacketType = 0x%X\n", PacketType));

	pTxALCData->TssiTriggered = 0;

#ifdef DOT11_N_SUPPORT
	Power = pAd->TxPower[pAd->CommonCfg.CentralChannel-1].Power;
#else
	Power = pAd->TxPower[pAd->CommonCfg.Channel-1].Power ;
#endif /* DOT11_N_SUPPORT */

	//DBGPRINT(RT_DEBUG_TRACE, ("Channel Desire Power = %d\n", Power));

	switch (PacketType & 0x03) {
		case 0:
			TxRate = (PacketType >> 4) & 0x03;
			DBGPRINT(RT_DEBUG_INFO, ("tx_11b_rate: %x\n", TxRate));
			switch (TxRate) {
				case 0:	// 1 Mbps
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_CCK_1M : BW20_MCS_POWER_CCK_1M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_CCK_1M;
					break;
				case 1:	// 2 Mbps
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_CCK_2M : BW20_MCS_POWER_CCK_2M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_CCK_2M;
					break;
				case 2:	// 5.5 Mbps
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_CCK_5M : BW20_MCS_POWER_CCK_5M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_CCK_5M;
					break;
				case 3:	// 11Mbps
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_CCK_11M : BW20_MCS_POWER_CCK_11M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_CCK_11M;
					break;
			}

			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &BBPR4);
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R178, &BBPR178);
			if (BBPR4 & 0x20) {
				if (BBPR178 == 0)
					Power += 9830;		// 8192 * 1.2
				else
					Power += 18022;		// 8192 * 2.2
			} else {
				if (BBPR178 == 0)
					Power += 24576;		// 8192 * 3
				else
					Power += 819;		/// 8192 * 0.1
			}
			break;
		case 1:
			TxRate = (PacketType >> 4) & 0x0F;
			DBGPRINT(RT_DEBUG_INFO, ("tx_11g_rate: %x\n", TxRate));
			switch (TxRate) {
				case 0xB:	// 6 Mbps
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_OFDM_6M : BW20_MCS_POWER_OFDM_6M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_6M;
					break;
				case 0xF:	// 9 Mbps
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_OFDM_9M : BW20_MCS_POWER_OFDM_9M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_9M;
					break;
				case 0xA:	// 12 Mbps
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_OFDM_12M : BW20_MCS_POWER_OFDM_12M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_12M;
					break;
				case 0xE:	// 18 Mbps
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_OFDM_18M : BW20_MCS_POWER_OFDM_18M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_18M;
					break;
				case 0x9:	// 24 Mbps
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_OFDM_24M : BW20_MCS_POWER_OFDM_24M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_24M;
					break;
				case 0xD:	// 36 Mbps
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_OFDM_36M : BW20_MCS_POWER_OFDM_36M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_36M;
					break;
				case 0x8:	// 48 Mbps
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_OFDM_48M : BW20_MCS_POWER_OFDM_48M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_48M;
					break;
				case 0xC:	// 54 Mbps
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_OFDM_54M : BW20_MCS_POWER_OFDM_54M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_54M;
					break;
			}
			break;
		default:
			BbpR47 = (BbpR47 & ~0x07) | 0x02;
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, BbpR47);
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, &TxRate);
			DBGPRINT(RT_DEBUG_INFO, ("tx_11n_rate: %x\n", TxRate));
			TxRate &= 0x7F;				// TxRate[7] is bandwidth
			switch (TxRate) {
				case 0x0:
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_HT_MCS0: BW20_MCS_POWER_HT_MCS0;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS0;
					break;
				case 0x1:
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_HT_MCS1: BW20_MCS_POWER_HT_MCS1;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS1;
					break;
				case 0x2:
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_HT_MCS2: BW20_MCS_POWER_HT_MCS2;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS2;
					break;
				case 0x3:
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_HT_MCS3: BW20_MCS_POWER_HT_MCS3;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS3;
					break;
				case 0x4:
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_HT_MCS4: BW20_MCS_POWER_HT_MCS4;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS4;
					break;
				case 0x5:
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_HT_MCS5: BW20_MCS_POWER_HT_MCS5;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS5;
					break;
				case 0x6:
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_HT_MCS6: BW20_MCS_POWER_HT_MCS6;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS6;
					break;
				case 0x7:
					Power += (pAd->CommonCfg.BBPCurrentBW == BW_40)? BW40_MCS_POWER_HT_MCS7: BW20_MCS_POWER_HT_MCS7;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_INFO, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS7;
					break;

			}
			break;
	}

	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BBPReg);
	switch (BBPReg & 0x3) {
		case 1:
			Power -= 49152;		// -6 dB*8192
			break;
		case 2:
			Power -= 98304;		//-12 dB*8192
			break;
		case 3:
			Power += 49152;		// 6 dB*8192
			break;
		case 0:
		default:
			break;
	}

	Power += pTxALCData->MT7601_TSSI_T0_Delta_Offset;

	*TargetPower = Power;
	DBGPRINT(RT_DEBUG_INFO, ("TargetPower: 0x%x(%d)\n", *TargetPower, *TargetPower));

	return TRUE;

}

VOID MT7601_AsicTxAlcGetAutoAgcOffset(
	IN PRTMP_ADAPTER 			pAd,
	IN PCHAR					pDeltaPwr,
	IN PCHAR					pTotalDeltaPwr,
	IN PCHAR					pAgcCompensate,
	IN PCHAR	 				pDeltaPowerByBbpR1)
{
	INT32 TargetPower, CurrentPower, PowerDiff;
	UCHAR TssiLinear0, TssiLinear1;
	CHAR tssi_offset;
	INT16 tssi_db, tssi_m_dc;
	UINT32 value;
	MT7601_TX_ALC_DATA *pTxALCData = &pAd->chipCap.TxALCData;

#ifdef MT7601FPGA
	return;
#endif /*MT7601FPGA */

	if (pAd->TxPowerCtrl.bInternalTxALC == FALSE)
		return;

	if (pAd->Mlme.OneSecPeriodicRound % 4 == 0) {
              // if base power is lower than 10 dBm use High VGA
#ifdef DOT11_N_SUPPORT
		if(pAd->TxPower[pAd->CommonCfg.CentralChannel-1].Power <= 20)
			pTxALCData->TSSI_USE_HVGA = 1;
		else
			pTxALCData->TSSI_USE_HVGA = 0;
#else
		if (pAd->TxPower[pAd->CommonCfg.Channel-1].Power <= 20)
			pTxALCData->TSSI_USE_HVGA = 1;
		else
			pTxALCData->TSSI_USE_HVGA = 0;
#endif /* DOT11_N_SUPPORT */

		if (MT7601_GetTssiCompensationParam(pAd, &TssiLinear0 , &TssiLinear1, &TargetPower) == FALSE)
			return;

		tssi_m_dc = TssiLinear0 - ((pTxALCData->TSSI_USE_HVGA == 1) ? pTxALCData->TssiDC0_HVGA : pTxALCData->TssiDC0);

		DBGPRINT(RT_DEBUG_INFO, ("tssi_m_dc: %d\n", tssi_m_dc));
		DBGPRINT(RT_DEBUG_INFO, ("TssiLinear0: %d\n", TssiLinear0));
		if (pTxALCData->TSSI_USE_HVGA == 1)
			DBGPRINT(RT_DEBUG_INFO, ("TssiDC0_HVGA: %d\n", pTxALCData->TssiDC0_HVGA));
		else
			DBGPRINT(RT_DEBUG_INFO, ("TssiDC0: %d\n", pTxALCData->TssiDC0));

		tssi_db = lin2dBd(tssi_m_dc);

#ifdef DOT11_N_SUPPORT
		if (pAd->CommonCfg.CentralChannel <= 4)
			tssi_offset = pTxALCData->MT7601_TSSI_OFFSET[0];
		else if (pAd->CommonCfg.CentralChannel >= 9)
			tssi_offset = pTxALCData->MT7601_TSSI_OFFSET[2];
		else
			tssi_offset = pTxALCData->MT7601_TSSI_OFFSET[1];
#else
		if (pAd->CommonCfg.Channel <= 4)
			tssi_offset = pTxALCData->MT7601_TSSI_OFFSET[0];
		else if (pAd->CommonCfg.Channel >= 9)
			tssi_offset = pTxALCData->MT7601_TSSI_OFFSET[2];
		else
			tssi_offset = pTxALCData->MT7601_TSSI_OFFSET[1];
#endif /* DOT11_N_SUPPORT */

		DBGPRINT(RT_DEBUG_INFO, ("tssi_offset: %d\n", tssi_offset));
		DBGPRINT(RT_DEBUG_INFO, ("tssi_offset<<9: %d\n", tssi_offset<<9));
		DBGPRINT(RT_DEBUG_INFO, ("TssiSlope: %d\n", pTxALCData->TssiSlope));
		DBGPRINT(RT_DEBUG_INFO, ("tssi_db: %d\n", tssi_db));
		if(pTxALCData->TSSI_USE_HVGA == 1)
			tssi_db -= pTxALCData->TSSI_DBOFFSET_HVGA;

		CurrentPower = (pTxALCData->TssiSlope*tssi_db) + (tssi_offset << 9);

		DBGPRINT(RT_DEBUG_INFO, ("CurrentPower: %d\n", CurrentPower));

		PowerDiff = TargetPower - CurrentPower;

		DBGPRINT(RT_DEBUG_INFO, ("PowerDiff: %d\n", PowerDiff));

		if((TssiLinear0 > 126) && ( PowerDiff > 0)) { // upper saturation
			DBGPRINT(RT_DEBUG_ERROR, ("%s :: upper saturation.\n", __FUNCTION__));
			PowerDiff = 0;
		} else {
			//if(((TssiLinear0 -TssiDC0) < 1) && (PowerDiff < 0)) 	// lower saturation
			if (((TssiLinear0 -((pTxALCData->TSSI_USE_HVGA == 1) ? pTxALCData->TssiDC0_HVGA : pTxALCData->TssiDC0)) < 1) && (PowerDiff < 0)) {// lower saturation
				DBGPRINT(RT_DEBUG_ERROR, ("%s :: lower saturation.\n", __FUNCTION__));
				PowerDiff = 0;
			}
		}

		if (((pTxALCData->PowerDiffPre ^ PowerDiff) < 0)
                  && ((PowerDiff  < 4096) && (PowerDiff  > -4096)) // +- 0.5
		   && ((pTxALCData->PowerDiffPre < 4096) && (pTxALCData->PowerDiffPre > -4096))) { // +- 0.5
			if ((PowerDiff > 0) && ((PowerDiff + pTxALCData->PowerDiffPre) >= 0))
				PowerDiff = 0;
			else if ((PowerDiff < 0) && ((PowerDiff + pTxALCData->PowerDiffPre) < 0))
				PowerDiff = 0;
			else
				pTxALCData->PowerDiffPre = PowerDiff;
		} else
			pTxALCData->PowerDiffPre =  PowerDiff;

		PowerDiff = PowerDiff + ((PowerDiff > 0) ? 2048 : -2048);
		PowerDiff = PowerDiff / 4096;

		DBGPRINT(RT_DEBUG_INFO, ("final PowerDiff: %d(0x%x)\n", PowerDiff, PowerDiff));

		RTMP_IO_READ32(pAd, TX_ALC_CFG_1, &value);
		CurrentPower = (value & 0x3F);
		CurrentPower = CurrentPower > 0x1F ? CurrentPower - 0x40 : CurrentPower;
		PowerDiff += CurrentPower;
		if (PowerDiff > 31)
			PowerDiff = 31;
		if (PowerDiff < -32)
			PowerDiff = -32;
		//PowerDiff = PowerDiff + (value & 0x3F);
		value = (value & ~0x3F) | (PowerDiff & 0x3F);
		RTMP_IO_WRITE32(pAd, TX_ALC_CFG_1, value);
		DBGPRINT(RT_DEBUG_INFO, ("MAC 13b4: 0x%x\n", value));

		//MT7601AsicTemperatureCompensation(pAd);

		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD)) {
			MT7601_EnableTSSI(pAd);
			pTxALCData->TssiTriggered = 1;
		}

	}

}
#endif /* RTMP_INTERNAL_TX_ALC */


INT MT7601_Bootup_Read_Temperature(
	IN	PRTMP_ADAPTER	pAd,
	OUT	CHAR*			Temperature)
{
	UINT32	MAC0504, MAC050C;
	UCHAR	BBPReg;
	int i;

	RTMP_IO_READ32(pAd, 0x50C, &MAC050C);
	RTMP_IO_READ32(pAd, 0x504, &MAC0504);

	RTMP_IO_WRITE32(pAd, 0x504, 0x0);
	RTMP_IO_WRITE32(pAd, 0x50C, 0x10);
	RTMP_IO_WRITE32(pAd, 0x504, 0x10);

	/* BBP R47[4] = 1 */
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R47, &BBPReg);
	BBPReg &= (~0x7f);
	BBPReg |= 0x10;
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, BBPReg);

	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, 0x40);

	i = 100;
	while (i > 0) {
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R47, &BBPReg);
		if((BBPReg & 0x10) == 0)
			break;
		i--;
	}

	BBPReg = (BBPReg & ~0x07) | 0x04;
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, BBPReg);
	RTMPusecDelay(500);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, Temperature);
	DBGPRINT(RT_DEBUG_TRACE, ("Boot up temperature = 0x%X\n", *Temperature));

	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, 0x0);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R21, &BBPReg);
	BBPReg |= 0x2;
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R21, BBPReg);
	BBPReg &= ~0x2;
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R21, BBPReg);

	RTMP_IO_WRITE32(pAd, 0x504, 0);
	RTMP_IO_WRITE32(pAd, 0x50C, MAC050C);
	RTMP_IO_WRITE32(pAd, 0x504, MAC0504);

	return TRUE;
}


INT MT7601_Read_Temperature(
	IN	PRTMP_ADAPTER	pAd,
	OUT	CHAR*			Temperature)
{
	UCHAR	BBPReg;
	int i;

#ifdef RTMP_INTERNAL_TX_ALC
	if ((pAd->chipCap.TxALCData.TssiTriggered == 1) && ( pAd->TxPowerCtrl.bInternalTxALC == TRUE))
		*Temperature = pAd->chipCap.CurrentTemperBbpR49;
	else
#endif /* RTMP_INTERNAL_TX_ALC */
	{
		/* BBP R47[4] = 1 */
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R47, &BBPReg);
		BBPReg &= (~0x7f);
		BBPReg |= 0x10;
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, BBPReg);

		i = 100;
		while (i > 0) {
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R47, &BBPReg);
			if((BBPReg & 0x10) == 0)
				break;
			i--;
		}

		BBPReg = (BBPReg & ~0x07) | 0x04;
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, BBPReg);
		RTMPusecDelay(500);
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, Temperature);
		//DBGPRINT(RT_DEBUG_TRACE, ("temperature = 0x%X\n", *Temperature));
	}

	return TRUE;
}


VOID MT7601SetRxAnt(
	IN PRTMP_ADAPTER    pAd,
	IN UCHAR            Ant)
{
	UCHAR Val;

	if ((RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS))  ||
	(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))   ||
	(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF)) ||
	(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)))
		return;

	/* For PPAD Debug, BBP R153[7] = 1 --> Main Ant, R153[7] = 0 --> Aux Ant */
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R150, 0x00); /* Disable ANTSW_OFDM */
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R151, 0x00); /* Disable ANTSW_CCK */
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R154, 0x00); /* Clear R154[4], Rx Ant is not bound to the previous rx packet selected Ant */

	if (Ant == 0) {
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R152, &Val);
		Val &= ~0x80;
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R152, Val); /* Main Ant */
		DBGPRINT(RT_DEBUG_OFF, ("\x1b[31m%s: MT7601 --> switch to main\x1b[m\n", __FUNCTION__));
	} else {
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R152, &Val);
		Val |= 0x80;
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R152, Val); /* Aux Ant */
		DBGPRINT(RT_DEBUG_OFF, ("\x1b[31m%s: MT7601 --> switch to aux\x1b[m\n", __FUNCTION__));
	}
}


/*
========================================================================
Routine Description:
	Initialize MT7601.

Arguments:
	pAd					- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
VOID MT7601_Init(RTMP_ADAPTER *pAd)
{
	RTMP_CHIP_OP *pChipOps = &pAd->chipOps;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	UINT32 Value;

	DBGPRINT(RT_DEBUG_TRACE, ("-->%s():\n", __FUNCTION__));

	pAd->RfIcType = RFIC_UNKNOWN;
	/*
		Init chip capabilities
	*/
	RTMP_IO_READ32(pAd, 0x00, &Value);
	pChipCap->ChipID = Value;

	pChipCap->MaxNss = 1;
	pChipCap->TXWISize = 20;
	pChipCap->RXWISize = 24;
	// TODO: check WPDMABurstSIZE???
	pChipCap->WPDMABurstSIZE = 3;

	pChipCap->SnrFormula = SNR_FORMULA2;
	pChipCap->FlgIsHwWapiSup = TRUE;
	pChipCap->VcoPeriod = 10;
	pChipCap->FlgIsVcoReCalMode = VCO_CAL_DISABLE;
	pChipCap->FlgIsHwAntennaDiversitySup = TRUE;
	pChipCap->FlgSwBasedPPAD = TRUE;
#ifdef STREAM_MODE_SUPPORT
	pChipCap->FlgHwStreamMode = FALSE;
#endif /* STREAM_MODE_SUPPORT */
#ifdef TXBF_SUPPORT
	pChipCap->FlgHwTxBfCap = FALSE;
#endif /* TXBF_SUPPORT */
#ifdef FIFO_EXT_SUPPORT
	pChipCap->FlgHwFifoExtCap = TRUE;
#endif /* FIFO_EXT_SUPPORT */

#ifdef CONFIG_RX_CSO_SUPPORT
	pChipCap->asic_caps |= fASIC_CAP_CSO;
#endif /* CONFIG_RX_CSO_SUPPORT */

	pChipCap->asic_caps |= (fASIC_CAP_PMF_ENC);

	pChipCap->phy_caps = fPHY_CAP_24G;
	pChipCap->phy_caps |= fPHY_CAP_HT;

	// TODO: check RfReg17WtMethod???
	//pChipCap->RfReg17WtMethod = RF_REG_WT_METHOD_STEP_ON;

	pChipCap->MaxNumOfRfId = 63;
	pChipCap->pRFRegTable = NULL;

	pChipCap->MaxNumOfBbpId = 255;
	pChipCap->pBBPRegTable = NULL;
	pChipCap->bbpRegTbSize = 0;

#ifdef NEW_MBSSID_MODE
	pChipCap->MBSSIDMode = MBSSID_MODE1;
#else
	pChipCap->MBSSIDMode = MBSSID_MODE0;
#endif /* NEW_MBSSID_MODE */



#ifdef RTMP_EFUSE_SUPPORT
	pChipCap->EFUSE_USAGE_MAP_START = 0x1E0;
	pChipCap->EFUSE_USAGE_MAP_END = 0x1FC;
	pChipCap->EFUSE_USAGE_MAP_SIZE = 29;
	pChipCap->EFUSE_DEFAULT_BIN = MT7601_EFUSE_DEFAULT_BIN;
	pChipCap->EFUSE_DEFAULT_BIN_SIZE = 0x100;
#endif /* RTMP_EFUSE_SUPPORT */

	pChipCap->WlanMemmapOffset = 0x410000;
	pChipCap->InbandPacketMaxLen = 192;

	RTMP_DRS_ALG_INIT(pAd, RATE_ALG_GRP);

	/*
		Following function configure beacon related parameters
		in pChipCap
			FlgIsSupSpecBcnBuf / BcnMaxHwNum /
			WcidHwRsvNum / BcnMaxHwSize / BcnBase[]
	*/
	rlt_bcn_buf_init(pAd);

	/*
		init operator
	*/

	/* BBP adjust */
	pChipOps->ChipBBPAdjust = MT7601_ChipBBPAdjust;

	/* Channel */
	pChipOps->ChipSwitchChannel = MT7601_ChipSwitchChannel;
	pChipOps->ChipAGCInit = MT7601_ChipAGCInit;

	pChipOps->AsicMacInit = NICInitMT7601MacRegisters;
	pChipOps->AsicBbpInit = NICInitMT7601BbpRegisters;
	pChipOps->AsicRfInit = NICInitMT7601RFRegisters;
	pChipOps->AsicRfTurnOn = NULL;

	pChipOps->AsicHaltAction = NULL;
	pChipOps->AsicRfTurnOff = NULL;
	pChipOps->AsicReverseRfFromSleepMode = NULL;
	pChipOps->AsicResetBbpAgent = NULL;

	/* MAC */

	/* EEPROM */
	pChipOps->NICInitAsicFromEEPROM = MT7601_NICInitAsicFromEEPROM;

	/* Temperature Compensation */
	pChipOps->InitTemperCompensation = MT7601_InitTemperatureCompensation;
	pChipOps->TemperCompensation = MT7601_TemperatureCompensation;

	/* Antenna */
	pChipOps->AsicAntennaDefaultReset = MT7601_AsicAntennaDefaultReset;

	/* TX ALC */
 	pChipOps->ATETssiCalibration = NULL;
	pChipOps->ATETssiCalibrationExtend = NULL;
	pChipOps->AsicTxAlcGetAutoAgcOffset = NULL;
	pChipOps->ATEReadExternalTSSI = NULL;
	pChipOps->TSSIRatio = NULL;

	/* Others */
#ifdef CARRIER_DETECTION_SUPPORT
	pAd->chipCap.carrier_func = TONE_RADAR_V2;
	pChipOps->ToneRadarProgram = ToneRadarProgram_v2;
#endif /* CARRIER_DETECTION_SUPPORT */

	/* Chip tuning */
	pChipOps->RxSensitivityTuning = NULL;
#ifdef RTMP_INTERNAL_TX_ALC
	pChipOps->InitDesiredTSSITable = MT7601_InitDesiredTSSITable;
	pChipOps->AsicTxAlcGetAutoAgcOffset = MT7601_AsicTxAlcGetAutoAgcOffset;
	pChipCap->TxALCData.TssiSlope = 0;
	pChipCap->TxALCData.TssiDC0 = 0;
	pChipCap->TxALCData.TssiDC0_HVGA = 0;
	pChipCap->TxALCData.TSSI_USE_HVGA = 0;
	pChipCap->TxALCData.PowerDiffPre = 100;
#endif /* RTMP_INTERNAL_TX_ALC */
	pChipOps->AsicGetTxPowerOffset = AsicGetTxPowerOffset;

	pChipOps->AsicExtraPowerOverMAC = MT7601_AsicExtraPowerOverMAC;

	pChipOps->SetRxAnt = MT7601SetRxAnt;


	pChipOps->DisableTxRx = MT7601DisableTxRx;

#ifdef RTMP_USB_SUPPORT
	pChipOps->AsicRadioOn = MT7601UsbAsicRadioOn;
	pChipOps->AsicRadioOff = MT7601UsbAsicRadioOff;
#endif


#ifdef RTMP_USB_SUPPORT
	pChipOps->loadFirmware = USBLoadFirmwareToAndes;
#endif

	//pChipOps->sendCommandToMcu = AsicSendCmdToAndes;
	//pChipOps->loadFirmware = NULL;
#ifdef CONFIG_ANDES_SUPPORT
	pChipCap->WlanMemmapOffset = 0x410000;
	pChipCap->InbandPacketMaxLen = 192;
	pChipCap->CmdRspRxRing = RX_RING1;
#endif
	pChipCap->MCUType = ANDES;
	pChipCap->FWImageName = MT7601_FirmwareImage;

/* Following callback functions already initiailized in RtmpChipOpsEepromHook( ) */
	/*  Calibration access related callback functions */
/*
	int (*eeinit)(struct _RTMP_ADAPTER *pAd);
	int (*eeread)(struct _RTMP_ADAPTER *pAd, USHORT offset, PUSHORT pValue);
	int (*eewrite)(struct _RTMP_ADAPTER *pAd, USHORT offset, USHORT value);
*/
	/* MCU related callback functions */
/*
	int (*loadFirmware)(struct _RTMP_ADAPTER *pAd);
	int (*eraseFirmware)(struct _RTMP_ADAPTER *pAd);
	int (*sendCommandToMcu)(struct _RTMP_ADAPTER *pAd, UCHAR cmd, UCHAR token, UCHAR arg0, UCHAR arg1, BOOLEAN FlgIsNeedLocked);
*/

/*
	Following callback functions already initiailized in RtmpChipOpsHook()
	1. Power save related
*/
#ifdef GREENAP_SUPPORT
	pChipOps->EnableAPMIMOPS = NULL;
	pChipOps->DisableAPMIMOPS = NULL;
#endif /* GREENAP_SUPPORT */
	// TODO: ------Upper parameters are not finialized yet!!

#ifdef MICROWAVE_OVEN_SUPPORT
	pChipOps->AsicMeasureFalseCCA = MT7601_AsicMeasureFalseCCA;
	pChipOps->AsicMitigateMicrowave = MT7601_AsicMitigateMicrowave;
#endif /* MICROWAVE_OVEN_SUPPORT */

#ifdef HDR_TRANS_SUPPORT
	if (1) {
		//UINT32 RegVal;

		/* enable TX/RX Header Translation */
		RTMP_IO_WRITE32(pAd, HT_RX_WCID_EN_BASE , 0xFF);	/* all RX WCID enable */

		/* black list - skip EAP-888e/DLS-890d */
		RTMP_IO_WRITE32(pAd, HT_RX_BL_BASE, 0x888e890d);
		//RTMP_IO_WRITE32(pAd, HT_RX_BL_BASE, 0x08000806);

		/* tsc conrotl */
/*
		RTMP_IO_READ32(pAd, 0x250, &RegVal);
		RegVal |= 0x6000;
		RTMP_IO_WRITE32(pAd, 0x250, RegVal);
*/
	}
#endif /* HDR_TRANS_SUPPORT */

#ifdef NEW_WOW_SUPPORT
	pChipOps->AsicWOWEnable = RT28xxAndesWOWEnable;
	pChipOps->AsicWOWDisable = RT28xxAndesWOWDisable;
#endif /* NEW_WOW_SUPPORT */

}


#ifdef MICROWAVE_OVEN_SUPPORT
VOID MT7601_AsicMeasureFalseCCA(
	IN PRTMP_ADAPTER pAd)
{
	UINT32 reg;
	/* restore LAN gain*/
	//printk("Stored_BBP_R65=%x @%s \n", pAd->CommonCfg.MO_Cfg.Stored_BBP_R65, __FUNCTION__);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R65, pAd->CommonCfg.MO_Cfg.Stored_BBP_R65);
	/* restore RF LPF threshold */
	rlt_rf_write(pAd, RF_BANK5, RF_R06, pAd->CommonCfg.MO_Cfg.Stored_RF_B5_R6);
	rlt_rf_write(pAd, RF_BANK5, RF_R07, pAd->CommonCfg.MO_Cfg.Stored_RF_B5_R7);

	/* clear false cca counter */
	RTMP_IO_READ32(pAd, RX_STA_CNT1, &reg);

	/* reset false CCA counter */
	pAd->CommonCfg.MO_Cfg.nFalseCCACnt = 0;
}


VOID MT7601_AsicMitigateMicrowave(
	IN PRTMP_ADAPTER pAd)
{
	UINT8	RegValue;
	//printk("Detect Microwave...\n");
	/* set middle gain */

	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R65, &RegValue);
	RegValue |= 0x08;
	RegValue &= 0xFB; /*BBP_R65[3:2] from 3 into 2 */
	/* narrow down RF LPF */
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R65, RegValue);
	rlt_rf_write(pAd, RF_BANK5, RF_R06, 0x3F);
	rlt_rf_write(pAd, RF_BANK5, RF_R07, 0x3F);

}
#endif /* MICROWAVE_OVEN_SUPPORT */

#endif /* MT7601 */

