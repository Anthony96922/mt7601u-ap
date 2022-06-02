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
	eeprom.h

	Abstract:
	Miniport header file for eeprom related information

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/
#ifndef __EEPROM_H__
#define __EEPROM_H__

/* For ioctl check usage */
#define EEPROM_IS_PROGRAMMED		0x80


#ifdef RTMP_MAC_USB
#define EEPROM_SIZE					0x400
#endif /* RTMP_MAC_USB */


#ifdef RT_BIG_ENDIAN
typedef	union _EEPROM_WORD_STRUC {
	struct {
		unsigned char	Byte1;				// High Byte
		unsigned char	Byte0;				// Low Byte
	} field;
	unsigned short	word;
} EEPROM_WORD_STRUC;
#else
typedef	union _EEPROM_WORD_STRUC {
	struct {
		unsigned char	Byte0;
		unsigned char	Byte1;
	} field;
	unsigned short	word;
} EEPROM_WORD_STRUC;
#endif


/* ------------------------------------------------------------------- */
/*  E2PROM data layout */
/* ------------------------------------------------------------------- */

/* Board type */

#define BOARD_TYPE_MINI_CARD		0	/* Mini card */
#define BOARD_TYPE_USB_PEN		1	/* USB pen */

/*
	EEPROM antenna select format
*/

#ifdef RT_BIG_ENDIAN
typedef union _EEPROM_NIC_CINFIG2_STRUC {
	struct {
		unsigned short DACTestBit:1;	/* control if driver should patch the DAC issue */
		unsigned short CoexBit:1;
		unsigned short bInternalTxALC:1;	/* Internal Tx ALC */
		unsigned short AntOpt:1;	/* Fix Antenna Option: 0:Main; 1: Aux */
		unsigned short AntDiversity:1;	/* Antenna diversity */
		unsigned short Rsv1:1;	/* must be 0 */
		unsigned short BW40MAvailForA:1;	/* 0:enable, 1:disable */
		unsigned short BW40MAvailForG:1;	/* 0:enable, 1:disable */
		unsigned short EnableWPSPBC:1;	/* WPS PBC Control bit */
		unsigned short BW40MSidebandForA:1;
		unsigned short BW40MSidebandForG:1;
		unsigned short CardbusAcceleration:1;	/* !!! NOTE: 0 - enable, 1 - disable */
		unsigned short ExternalLNAForA:1;	/* external LNA enable for 5G */
		unsigned short ExternalLNAForG:1;	/* external LNA enable for 2.4G */
		unsigned short DynamicTxAgcControl:1;	/* */
		unsigned short HardwareRadioControl:1;	/* Whether RF is controlled by driver or HW. 1:enable hw control, 0:disable */
	} field;
	unsigned short word;
} EEPROM_NIC_CONFIG2_STRUC, *PEEPROM_NIC_CONFIG2_STRUC;
#else
typedef union _EEPROM_NIC_CINFIG2_STRUC {
	struct {
		unsigned short HardwareRadioControl:1;	/* 1:enable, 0:disable */
		unsigned short DynamicTxAgcControl:1;	/* */
		unsigned short ExternalLNAForG:1;	/* */
		unsigned short ExternalLNAForA:1;	/* external LNA enable for 2.4G */
		unsigned short CardbusAcceleration:1;	/* !!! NOTE: 0 - enable, 1 - disable */
		unsigned short BW40MSidebandForG:1;
		unsigned short BW40MSidebandForA:1;
		unsigned short EnableWPSPBC:1;	/* WPS PBC Control bit */
		unsigned short BW40MAvailForG:1;	/* 0:enable, 1:disable */
		unsigned short BW40MAvailForA:1;	/* 0:enable, 1:disable */
		unsigned short Rsv1:1;	/* must be 0 */
		unsigned short AntDiversity:1;	/* Antenna diversity */
		unsigned short AntOpt:1;	/* Fix Antenna Option: 0:Main; 1: Aux */
		unsigned short bInternalTxALC:1;	/* Internal Tx ALC */
		unsigned short CoexBit:1;
		unsigned short DACTestBit:1;	/* control if driver should patch the DAC issue */
	} field;
	unsigned short word;
} EEPROM_NIC_CONFIG2_STRUC, *PEEPROM_NIC_CONFIG2_STRUC;
#endif


#if defined(BT_COEXISTENCE_SUPPORT) || defined(RT3290)
#ifdef RTMP_USB_SUPPORT
#ifdef RT_BIG_ENDIAN
typedef union _EEPROM_NIC_CINFIG3_STRUC {
	struct {
		unsigned short Rsv1:7;	/* must be 0 */
		unsigned short CoexMethod:1;
		unsigned short TxStream:4;	/* Number of Tx stream */
		unsigned short RxStream:4;	/* Number of rx stream */
	} field;
	unsigned short word;
} EEPROM_NIC_CONFIG3_STRUC, *PEEPROM_NIC_CONFIG3_STRUC;
#else
typedef union _EEPROM_NIC_CINFIG3_STRUC {
	struct {
		unsigned short RxStream:4;	/* Number of rx stream */
		unsigned short TxStream:4;	/* Number of Tx stream */
		unsigned short CoexMethod:1;
		unsigned short Rsv1:7;	/* must be 0 */
	} field;
	unsigned short word;
} EEPROM_NIC_CONFIG3_STRUC, *PEEPROM_NIC_CONFIG3_STRUC;
#endif
#endif /* RTMP_USB_SUPPORT */

#endif /* defined(BT_COEXISTENCE_SUPPORT) || defined(RT3290) */



/*
	TX_PWR Value valid range 0xFA(-6) ~ 0x24(36)
*/
#ifdef RT_BIG_ENDIAN
typedef union _EEPROM_TX_PWR_STRUC {
	struct {
		signed char Byte1;	/* High Byte */
		signed char Byte0;	/* Low Byte */
	} field;
	unsigned short word;
} EEPROM_TX_PWR_STRUC, *PEEPROM_TX_PWR_STRUC;
#else
typedef union _EEPROM_TX_PWR_STRUC {
	struct {
		signed char Byte0;	/* Low Byte */
		signed char Byte1;	/* High Byte */
	} field;
	unsigned short word;
} EEPROM_TX_PWR_STRUC, *PEEPROM_TX_PWR_STRUC;
#endif

#ifdef RT_BIG_ENDIAN
typedef union _EEPROM_VERSION_STRUC {
	struct {
		unsigned char Version;	/* High Byte */
		unsigned char FaeReleaseNumber;	/* Low Byte */
	} field;
	unsigned short word;
} EEPROM_VERSION_STRUC, *PEEPROM_VERSION_STRUC;
#else
typedef union _EEPROM_VERSION_STRUC {
	struct {
		unsigned char FaeReleaseNumber;	/* Low Byte */
		unsigned char Version;	/* High Byte */
	} field;
	unsigned short word;
} EEPROM_VERSION_STRUC, *PEEPROM_VERSION_STRUC;
#endif

#ifdef RT_BIG_ENDIAN
typedef union _EEPROM_LED_STRUC {
	struct {
		unsigned short Rsvd:3;	/* Reserved */
		unsigned short LedMode:5;	/* Led mode. */
		unsigned short PolarityGPIO_4:1;	/* Polarity GPIO#4 setting. */
		unsigned short PolarityGPIO_3:1;	/* Polarity GPIO#3 setting. */
		unsigned short PolarityGPIO_2:1;	/* Polarity GPIO#2 setting. */
		unsigned short PolarityGPIO_1:1;	/* Polarity GPIO#1 setting. */
		unsigned short PolarityGPIO_0:1;	/* Polarity GPIO#0 setting. */
		unsigned short PolarityACT:1;	/* Polarity ACT setting. */
		unsigned short PolarityRDY_A:1;	/* Polarity RDY_A setting. */
		unsigned short PolarityRDY_G:1;	/* Polarity RDY_G setting. */
	} field;
	unsigned short word;
} EEPROM_LED_STRUC, *PEEPROM_LED_STRUC;
#else
typedef union _EEPROM_LED_STRUC {
	struct {
		unsigned short PolarityRDY_G:1;	/* Polarity RDY_G setting. */
		unsigned short PolarityRDY_A:1;	/* Polarity RDY_A setting. */
		unsigned short PolarityACT:1;	/* Polarity ACT setting. */
		unsigned short PolarityGPIO_0:1;	/* Polarity GPIO#0 setting. */
		unsigned short PolarityGPIO_1:1;	/* Polarity GPIO#1 setting. */
		unsigned short PolarityGPIO_2:1;	/* Polarity GPIO#2 setting. */
		unsigned short PolarityGPIO_3:1;	/* Polarity GPIO#3 setting. */
		unsigned short PolarityGPIO_4:1;	/* Polarity GPIO#4 setting. */
		unsigned short LedMode:5;	/* Led mode. */
		unsigned short Rsvd:3;	/* Reserved */
	} field;
	unsigned short word;
} EEPROM_LED_STRUC, *PEEPROM_LED_STRUC;
#endif

#ifdef RT_BIG_ENDIAN
typedef union _EEPROM_TXPOWER_DELTA_STRUC {
	struct {
		unsigned char TxPowerEnable:1;	/* Enable */
		unsigned char Type:1;	/* 1: plus the delta value, 0: minus the delta value */
		unsigned char DeltaValue:6;	/* Tx Power dalta value (MAX=4) */
	} field;
	unsigned char value;
} EEPROM_TXPOWER_DELTA_STRUC, *PEEPROM_TXPOWER_DELTA_STRUC;
#else
typedef union _EEPROM_TXPOWER_DELTA_STRUC {
	struct {
		unsigned char DeltaValue:6;	/* Tx Power dalta value (MAX=4) */
		unsigned char Type:1;	/* 1: plus the delta value, 0: minus the delta value */
		unsigned char TxPowerEnable:1;	/* Enable */
	} field;
	unsigned char value;
} EEPROM_TXPOWER_DELTA_STRUC, *PEEPROM_TXPOWER_DELTA_STRUC;
#endif


#ifdef RT_BIG_ENDIAN
typedef union _EEPROM_TX_PWR_OFFSET_STRUC
{
	struct
	{
		unsigned char	Byte1;	/* High Byte */
		unsigned char	Byte0;	/* Low Byte */
	} field;
	
	unsigned short		word;
} EEPROM_TX_PWR_OFFSET_STRUC, *PEEPROM_TX_PWR_OFFSET_STRUC;
#else
typedef union _EEPROM_TX_PWR_OFFSET_STRUC
{
	struct
	{
		unsigned char	Byte0;	/* Low Byte */
		unsigned char	Byte1;	/* High Byte */
	} field;

	unsigned short		word;
} EEPROM_TX_PWR_OFFSET_STRUC, *PEEPROM_TX_PWR_OFFSET_STRUC;
#endif /* RT_BIG_ENDIAN */


struct _RTMP_ADAPTER;



#ifdef RTMP_USB_SUPPORT
NTSTATUS RTUSBReadEEPROM16(
	IN struct _RTMP_ADAPTER *pAd,
	IN unsigned short offset,
	OUT	unsigned short *pData);

NTSTATUS RTUSBWriteEEPROM16(
	IN struct _RTMP_ADAPTER *pAd, 
	IN unsigned short offset, 
	IN unsigned short value);
#endif /* RTMP_USB_SUPPORT */


#if defined(RTMP_RBUS_SUPPORT) || defined(RTMP_FLASH_SUPPORT)
NDIS_STATUS rtmp_nv_init(struct _RTMP_ADAPTER *pAd);
int rtmp_ee_flash_read(struct _RTMP_ADAPTER *pAd, unsigned short Offset, unsigned short *pVal);
int rtmp_ee_flash_write(struct _RTMP_ADAPTER *pAd, unsigned short Offset, unsigned short data);
VOID rtmp_ee_flash_read_all(struct _RTMP_ADAPTER *pAd, unsigned short *Data);
VOID rtmp_ee_flash_write_all(struct _RTMP_ADAPTER *pAd, unsigned short *Data);
#endif /* defined(RTMP_RBUS_SUPPORT) || defined(RTMP_FLASH_SUPPORT) */


#ifdef RTMP_EFUSE_SUPPORT
INT eFuseLoadEEPROM(struct _RTMP_ADAPTER *pAd);
INT eFuseWriteEeeppromBuf(struct _RTMP_ADAPTER *pAd);
VOID eFuseGetFreeBlockCount(struct _RTMP_ADAPTER *pAd, unsigned int *EfuseFreeBlock);

int rtmp_ee_efuse_read16(struct _RTMP_ADAPTER *pAd, unsigned short Offset, unsigned short *pVal);
int rtmp_ee_efuse_write16(struct _RTMP_ADAPTER *pAd, unsigned short Offset, unsigned short data);


NTSTATUS eFuseRead(struct _RTMP_ADAPTER *pAd, unsigned short Offset, unsigned short *pData, unsigned short len);
NTSTATUS eFuseWrite(struct _RTMP_ADAPTER *pAd, unsigned short Offset, unsigned short *pData, unsigned short len);

INT eFuse_init(struct _RTMP_ADAPTER *pAd);
INT efuse_probe(struct _RTMP_ADAPTER *pAd);

#ifdef RALINK_ATE
INT Set_LoadEepromBufferFromEfuse_Proc(
	IN struct _RTMP_ADAPTER *pAd,
	IN char *			arg);
#endif /* RALINK_ATE */
#endif /* RTMP_EFUSE_SUPPORT */


/*************************************************************************
  *	Public function declarations for prom operation callback functions setting
  ************************************************************************/
INT RtmpChipOpsEepromHook(struct _RTMP_ADAPTER *pAd, INT infType);

#endif /* __EEPROM_H__ */
