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
	rtmp_and.h

	Abstract:

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/

#ifndef __RTMP_AND_H__
#define __RTMP_AND_H__

/*
 * Power opration
 */
enum PWR_OP {
	RADIO_OFF = 0x30,
	RADIO_ON,
	RADIO_OFF_AUTO_WAKEUP,
	RADIO_OFF_ADVANCE,
	RADIO_ON_ADVANCE,
};


/*
 * Calibration ID
 */
enum CALIBRATION_ID {
	R_CALIBRATION = 1,
	DCOC_CALIBRATION,
};

/*
 * Function set ID
 */
enum FUN_ID {
	Q_SELECT = 1,
	ATOMIC_TSSI_SETTING  = 5,
};

/*
 * Command response RX Ring selection
 */
enum RX_RING_ID {
	RX_RING0,
	RX_RING1,
};

/*
 * Command type table 
 */
enum CMD_TYPE {
	CMD_FUN_SET_OP = 1,
	CMD_BURST_WRITE = 8,
	CMD_READ_MODIFY_WRITE,
	CMD_RANDOM_READ,
	CMD_BURST_READ,
	CMD_RANDOM_WRITE = 12,
	CMD_LED_MODE_OP = 16,
	CMD_POWER_SAVING_OP = 20,
	CMD_WOW_CONFIG,
	CMD_WOW_QUERY,
	CMD_WOW_FEATURE = 24,
	CMD_CARRIER_DETECT_OP = 28,
	CMD_RADOR_DETECT_OP,
	CMD_SWITCH_CHANNEL_OP,
	CMD_CALIBRATION_OP,
	CMD_BEACON_OP,
	CMD_ANTENNA_OP
};

/*
 * Event type table
 */
enum EVENT_TYPE {
	CMD_DONE,
	CMD_ERROR,
	CMD_RETRY,
	EVENT_PWR_RSP,
	EVENT_WOW_RSP,
	EVENT_CARRIER_DETECT_RSP,
	EVENT_DFS_DETECT_RSP,
};

#define ANDES_CALIBRATION_R			1
#define ANDES_CALIBRATION_LC			3
#define ANDES_CALIBRATION_LOFT		4
#define ANDES_CALIBRATION_TXIQ		5
#define ANDES_CALIBRATION_BW			6
#define ANDES_CALIBRATION_DPD			7
#define ANDES_CALIBRATION_RXIQ		8
#define ANDES_CALIBRATION_TXDCOC		9

INT AsicSendCommandToAndes(PRTMP_ADAPTER pAd, struct CMD_UNIT CmdUnit);
NDIS_STATUS USBLoadFirmwareToAndes(RTMP_ADAPTER *pAd);
NDIS_STATUS PCILoadFirmwareToAndes(RTMP_ADAPTER *pAd);
INT AsicSendCmdToAndes(PRTMP_ADAPTER pAd, struct CMD_UNIT *CmdUnit);
INT AndesBurstWrite(PRTMP_ADAPTER pAd, UINT32 Offset, UINT32 *Data, UINT32 Count);
INT AndesBurstRead(PRTMP_ADAPTER pAd, UINT32 Offset, UINT32 Cnt, UINT32 *Data);
INT AndesRandomRead(PRTMP_ADAPTER pAd, RTMP_REG_PAIR *RegPair, UINT32 Num);
INT AndesRFRandomRead(PRTMP_ADAPTER pAd, BANK_RF_REG_PAIR *RegPair, UINT32 Num);
INT AndesReadModifyWrite(PRTMP_ADAPTER pAd, R_M_W_REG *RegPair, UINT32 Num);
INT AndesRFReadModifyWrite(PRTMP_ADAPTER pAd, RF_R_M_W_REG *RegPair, UINT32 Num);
INT AndesRandomWritePair(PRTMP_ADAPTER pAd, RTMP_REG_PAIR *RegPair, UINT32 Num);
INT AndesRFRandomWritePair(PRTMP_ADAPTER pAd, BANK_RF_REG_PAIR *RegPair, UINT32 Num);
INT AndesRandomWrite(PRTMP_ADAPTER pAd, UINT32 Num, ...);
INT AndesRFRandomWrite(PRTMP_ADAPTER pAd, UINT32 Num, ...);
#ifdef MT7601
INT AndesBBPRandomWritePair(PRTMP_ADAPTER pAd, RTMP_REG_PAIR *RegPair, UINT32 Num);
INT AndesBBPRandomWrite(PRTMP_ADAPTER pAd, UINT32 Num, ...);
#endif /* MT7601 */
INT AndesFunSetOP(PRTMP_ADAPTER pAd, UINT32 FunID, UINT32 Param);
INT AndesPwrSavingOP(PRTMP_ADAPTER pAd, UINT32 PwrOP, UINT32 PwrLevel, 
					UINT32 ListenInterval, UINT32 PreTBTTLeadTime,
					UINT8 TIMByteOffset, UINT8 TIMBytePattern);
INT AndesCalibrationOP(PRTMP_ADAPTER, UINT32 CalibrationID, UINT32 Param);
BOOLEAN IsInBandCmdProcessing(PRTMP_ADAPTER pAd);
UCHAR GetCmdRspNum(PRTMP_ADAPTER pAd);
#endif
