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
	rtmp_mcu.c

	Abstract:

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/


#include	"rt_config.h"

INT MCUBurstWrite(PRTMP_ADAPTER pAd, unsigned int Offset, unsigned int *Data, unsigned int Cnt)
{
	RTUSBMultiWrite_nBytes(pAd, Offset, Data, Cnt * 4, 64); 
	return NDIS_STATUS_SUCCESS;
}

INT MCURandomWrite(PRTMP_ADAPTER pAd, RTMP_REG_PAIR *RegPair, unsigned int Num)
{
	unsigned int Index;
	
	for (Index = 0; Index < Num; Index++)
		RTMP_IO_WRITE32(pAd, RegPair->Register, RegPair->Value);
	return NDIS_STATUS_SUCCESS;
}

VOID ChipOpsMCUHook(PRTMP_ADAPTER pAd, enum MCU_TYPE MCUType)
{

	RTMP_CHIP_OP *pChipOps = &pAd->chipOps;


	if (MCUType == M8051) 
	{
		pChipOps->sendCommandToMcu = RtmpAsicSendCommandToMcu;
		pChipOps->BurstWrite = MCUBurstWrite;
		pChipOps->RandomWrite = MCURandomWrite;
	}

#ifdef CONFIG_ANDES_SUPPORT
	if (MCUType == ANDES) 
	{

#ifdef RTMP_USB_SUPPORT
		pChipOps->loadFirmware = USBLoadFirmwareToAndes;
#endif
		//pChipOps->sendCommandToMcu = AsicSendCmdToAndes;
		pChipOps->Calibration = AndesCalibrationOP;
		pChipOps->BurstWrite =  AndesBurstWrite;
		pChipOps->BurstRead = AndesBurstRead;
		pChipOps->RandomRead = AndesRandomRead;
		pChipOps->RFRandomRead = AndesRFRandomRead;
		pChipOps->ReadModifyWrite = AndesReadModifyWrite;
		pChipOps->RFReadModifyWrite = AndesRFReadModifyWrite;
		pChipOps->RandomWrite = AndesRandomWrite;
		pChipOps->RFRandomWrite = AndesRFRandomWrite;
		pChipOps->PwrSavingOP = AndesPwrSavingOP;
	}
#endif
}
