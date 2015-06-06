/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2011, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	rt5592_ate.c

	Abstract:
	Specific ATE funcitons and variables for RT5572/RT5592

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/


#ifdef MT7601

#include "rt_config.h"

extern MT7601_FREQ_ITEM MT7601_Frequency_Plan[];
extern UINT32 NUM_OF_MT7601_CHNL;
extern RTMP_REG_PAIR MT7601_BBP_BW20RegTb[];
extern UCHAR MT7601_BBP_BW20RegTb_Size;
extern RTMP_REG_PAIR MT7601_BBP_BW40RegTb[];
extern UCHAR MT7601_BBP_BW40RegTb_Size;
extern RTMP_REG_PAIR MT7601_BBP_HighTempBW20RegTb[];
extern UCHAR MT7601_BBP_HighTempBW20RegTb_Size;
extern RTMP_REG_PAIR MT7601_BBP_HighTempBW40RegTb[];
extern UCHAR MT7601_BBP_HighTempBW40RegTb_Size;
extern RTMP_REG_PAIR MT7601_BBP_LowTempBW20RegTb[];
extern UCHAR MT7601_BBP_LowTempBW20RegTb_Size;
extern RTMP_REG_PAIR MT7601_BBP_LowTempBW40RegTb[];
extern UCHAR MT7601_BBP_LowTempBW40RegTb_Size;

VOID MT7601ATEAsicSwitchChannel(
    IN PRTMP_ADAPTER pAd)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	UINT32 Value = 0;
	CHAR TxPwer = 0;
	UCHAR index = 0, Channel = 0;
	/* added to prevent RF register reading error */
	UCHAR RFValue = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;

	SYNC_CHANNEL_WITH_QA(pATEInfo, &Channel);

	DBGPRINT(RT_DEBUG_TRACE, ("==> %s: SwitchChannel#%d BW = %x\n",
				__FUNCTION__, Channel, pAd->ate.TxWI.TxWIBW));

	/* fill Tx power value */
	TxPwer = pATEInfo->TxPower0;

	if (Channel > 14)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s: Can't find the Channel#%d \n", __FUNCTION__, Channel));
		return;
	}

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

	/* BBP setting */
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R62, (0x37 - GET_LNA_GAIN(pAd)));
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R63, (0x37 - GET_LNA_GAIN(pAd)));
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R64, (0x37 - GET_LNA_GAIN(pAd)));

		//RtmpUpdateFilterCoefficientControl(pAd, Channel);

	/* 
		vcocal_en (initiate VCO calibration (reset after completion)) - It should be at the end of RF configuration. 
	*/
	rlt_rf_write(pAd, RF_BANK0, RF_R04, 0x0A);
	rlt_rf_write(pAd, RF_BANK0, RF_R05, 0x20);
	rlt_rf_read(pAd, RF_BANK0, RF_R04, &RFValue);
	RFValue = RFValue | 0x80; 
	rlt_rf_write(pAd, RF_BANK0, RF_R04, RFValue);
	RTMPusecDelay(2000);

	rtmp_bbp_set_bw(pAd, pAd->ate.TxWI.TxWIBW);

	switch (pAd->ate.TxWI.TxWIBW)
	{
		case BW_20:
			if ( pChipCap->TemperatureMode == TEMPERATURE_MODE_HIGH )
			{
				AndesBBPRandomWritePair(pAd, MT7601_BBP_HighTempBW20RegTb, MT7601_BBP_HighTempBW20RegTb_Size);
			}
			else if ( pChipCap->TemperatureMode == TEMPERATURE_MODE_LOW )
			{
				AndesBBPRandomWritePair(pAd, MT7601_BBP_LowTempBW20RegTb, MT7601_BBP_LowTempBW20RegTb_Size);
			}
			else
			{
				AndesBBPRandomWritePair(pAd, MT7601_BBP_BW20RegTb, MT7601_BBP_BW20RegTb_Size);
			}


			/* Tx Filter BW */
			AndesCalibrationOP(pAd, ANDES_CALIBRATION_BW, 0x10001);
			/* Rx Filter BW */
			AndesCalibrationOP(pAd, ANDES_CALIBRATION_BW, 0x10000);
			break;
		case BW_40:
			if ( pChipCap->TemperatureMode == TEMPERATURE_MODE_HIGH )
			{
				AndesBBPRandomWritePair(pAd, MT7601_BBP_HighTempBW40RegTb, MT7601_BBP_HighTempBW40RegTb_Size);
			}
			else if ( pChipCap->TemperatureMode == TEMPERATURE_MODE_LOW )
			{
				AndesBBPRandomWritePair(pAd, MT7601_BBP_LowTempBW40RegTb, MT7601_BBP_LowTempBW40RegTb_Size);
			}
			else
			{
				AndesBBPRandomWritePair(pAd, MT7601_BBP_BW40RegTb, MT7601_BBP_BW40RegTb_Size);
			}

			/* Tx Filter BW */
			AndesCalibrationOP(pAd, ANDES_CALIBRATION_BW, 0x10101);
			/* Rx Filter BW */
			AndesCalibrationOP(pAd, ANDES_CALIBRATION_BW, 0x10100);

			break;
		default:			
			break;
	}

	ATEAsicSetTxRxPath(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("<== %s: SwitchChannel#%d\n",
				__FUNCTION__, Channel));

}


INT MT7601ATETxPwrHandler(
	IN PRTMP_ADAPTER pAd,
	IN char index)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	CHAR TxPower = 0;
	UCHAR MaxPower;
	UINT32 RegValue = 0;
	UCHAR Channel = pATEInfo->Channel;

#ifdef RALINK_QA
	if ((pATEInfo->bQATxStart == TRUE) || (pATEInfo->bQARxStart == TRUE))
	{
		return 0;
	}
	else
#endif /* RALINK_QA */
	if (index == 0)
	{
		TxPower = pATEInfo->TxPower0;
	}
	else
	{
		DBGPRINT_ERR(("%s : Only TxPower0 and TxPower1 are adjustable !\n", __FUNCTION__));
		DBGPRINT_ERR(("%s : TxPower%d is out of range !\n", __FUNCTION__, index));
		return -1;
	}

	if (Channel <= 14) /* G band */
	{

#ifdef RTMP_INTERNAL_TX_ALC
		if ( pATEInfo->bAutoTxAlc == FALSE )
		{
			RTMP_IO_READ32(pAd, TX_ALC_CFG_1, &RegValue);
			RegValue &= ~(0xFFFF);
			RTMP_IO_WRITE32(pAd, TX_ALC_CFG_1, RegValue);
		}
		else
		{
			RTMP_IO_WRITE32(pAd, TX_ALC_CFG_1, pAd->chipCap.TxALCData.InitTxAlcCfg1);
		}
#endif /* RTMP_INTERNAL_TX_ALC */

		RTMP_IO_READ32(pAd, TX_ALC_CFG_0, &RegValue);
		MaxPower = RegValue >> 24;
		RegValue = RegValue & (~0x3F3F);
		if ( TxPower > MaxPower )
			TxPower = MaxPower;
		RegValue |= (TxPower & 0x3F);
		RTMP_IO_WRITE32(pAd, TX_ALC_CFG_0, RegValue);

	}

	DBGPRINT(RT_DEBUG_TRACE, ("%s : (TxPower%d=%d)\n", __FUNCTION__, index, TxPower));
	
	return 0;
}	


/* 
==========================================================================
    Description:
        Set  RT5370 and RT5372 and RT5390 and RT5392  ATE RF BW
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	MT7601_Set_ATE_TX_BW_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	INT powerIndex;
	UCHAR value = 0;
	UCHAR BBPCurrentBW;
	INT IdReg;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;

	
	BBPCurrentBW = simple_strtol(arg, 0, 10);

	if ((BBPCurrentBW == 0))
	{
		pAd->ate.TxWI.TxWIBW = BW_20;
	}
	else
	{
		pAd->ate.TxWI.TxWIBW = BW_40;
 	}

	if ((pAd->ate.TxWI.TxWIPHYMODE == MODE_CCK) && (pAd->ate.TxWI.TxWIBW == BW_40))
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_BW_Proc!! Warning!! CCK only supports 20MHZ!!\nBandwidth switch to 20\n"));
		pAd->ate.TxWI.TxWIBW = BW_20;
	}

	if (pAd->ate.TxWI.TxWIBW == BW_20)
	{
		if (pAd->ate.Channel <= 14)
		{
			/* BW=20;G band */
 			for (powerIndex=0; powerIndex<MAX_TXPOWER_ARRAY_SIZE; powerIndex++)
 			{
				if (pAd->Tx20MPwrCfgGBand[powerIndex] == 0xffffffff)
					continue;
				RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + powerIndex*4, pAd->Tx20MPwrCfgGBand[powerIndex]);	
				RtmpOsMsDelay(5);				
			}
		}

		/* BW=20 */
		{
		/* Set BBP R4 bit[4:3]=0:0 */
 		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, &value);
 		value &= (~0x18);
 		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, value);
		}

		if ( pChipCap->TemperatureMode == TEMPERATURE_MODE_HIGH )
		{
			for(IdReg=0; IdReg < MT7601_BBP_HighTempBW20RegTb_Size; IdReg++)
			{
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, MT7601_BBP_HighTempBW20RegTb[IdReg].Register,
						MT7601_BBP_HighTempBW20RegTb[IdReg].Value);
			}
		}
		else if ( pChipCap->TemperatureMode == TEMPERATURE_MODE_LOW )
		{
			for(IdReg=0; IdReg < MT7601_BBP_LowTempBW20RegTb_Size; IdReg++)
			{
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, MT7601_BBP_LowTempBW20RegTb[IdReg].Register,
						MT7601_BBP_LowTempBW20RegTb[IdReg].Value);
			}
		}
		else
		{
			for(IdReg=0; IdReg < MT7601_BBP_BW20RegTb_Size; IdReg++)
			{
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, MT7601_BBP_BW20RegTb[IdReg].Register,
						MT7601_BBP_BW20RegTb[IdReg].Value);
			}
		}

		/* Please don't move this block backward. */
		/* BBP_R4 should be overwritten for every chip if the condition matched. */
		if (pAd->ate.Channel == 14)
		{
			INT TxMode = pAd->ate.TxWI.TxWIPHYMODE;

			if (TxMode == MODE_CCK)
			{
				/* when Channel==14 && Mode==CCK && BandWidth==20M, BBP R4 bit5=1 */
 				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, &value);
				value |= 0x20; /* set bit5=1 */
 				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, value);				
			}
		}
	}
	/* If bandwidth = 40M, set RF Reg4 bit 21 = 0. */
	else if (pAd->ate.TxWI.TxWIBW == BW_40)
	{
		if (pAd->ate.Channel <= 14)
		{
			/* BW=40;G band */
			for (powerIndex=0; powerIndex<MAX_TXPOWER_ARRAY_SIZE; powerIndex++)
			{
				if (pAd->Tx40MPwrCfgGBand[powerIndex] == 0xffffffff)
					continue;
				RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + powerIndex*4, pAd->Tx40MPwrCfgGBand[powerIndex]);	
				RtmpOsMsDelay(5);				
			}
		}		

		{
			/* Set BBP R4 bit[4:3]=1:0 */
			ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &value);
			value &= (~0x18);
			value |= 0x10;
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, value);
		}

			if ( pChipCap->TemperatureMode == TEMPERATURE_MODE_HIGH )
			{
				for(IdReg=0; IdReg < MT7601_BBP_HighTempBW40RegTb_Size; IdReg++)
				{
					RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, MT7601_BBP_HighTempBW40RegTb[IdReg].Register,
							MT7601_BBP_HighTempBW40RegTb[IdReg].Value);
				}
			}
			else if ( pChipCap->TemperatureMode == TEMPERATURE_MODE_LOW )
			{
				for(IdReg=0; IdReg < MT7601_BBP_LowTempBW40RegTb_Size; IdReg++)
				{
					RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, MT7601_BBP_LowTempBW40RegTb[IdReg].Register,
							MT7601_BBP_LowTempBW40RegTb[IdReg].Value);
				}
			}
			else
			{
				for(IdReg=0; IdReg < MT7601_BBP_BW40RegTb_Size; IdReg++)
				{
					RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, MT7601_BBP_BW40RegTb[IdReg].Register,
							MT7601_BBP_BW40RegTb[IdReg].Value);
				}
			}
		
	}

	DBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_BW_Proc (BBPCurrentBW = %d)\n", pAd->ate.TxWI.TxWIBW));
	DBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_BW_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif /* CONFIG_AP_SUPPORT */
	
	return TRUE;
}	






#ifdef RTMP_INTERNAL_TX_ALC


BOOLEAN MT7601ATEGetTssiCompensationParam(
	IN 		PRTMP_ADAPTER 		pAd,
	OUT 	PCHAR 				TssiLinear0,
	OUT 	PCHAR 				TssiLinear1,
	OUT 	PINT32 				TargetPower)
{
#define MAX_TSSI_WAITING_COUNT	40
	UCHAR BBPReg;
	UCHAR PacketType;
	UCHAR BbpR47;
	UCHAR TxRate;
	INT32 Power;
	UCHAR ch = 0;
	MT7601_TX_ALC_DATA *pTxALCData = &pAd->chipCap.TxALCData;

	if ((pAd->ate.Channel >= 1) && (pAd->ate.Channel <= 14))
	{
		ch = pAd->ate.Channel;
	}
	else
	{
		ch = 1;

		DBGPRINT(RT_DEBUG_ERROR, ("%s::Incorrect channel #%d\n", __FUNCTION__, pAd->ate.Channel));
	}

	if ( pTxALCData->TssiTriggered == 0 )
	{
		if ( RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD) )
		{
			MT7601_EnableTSSI(pAd);
			pTxALCData->TssiTriggered = 1;
		}

		return FALSE;
	}

	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R47, &BBPReg);
	if (BBPReg & 0x10)
	{
		return FALSE;
	}

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

	DBGPRINT(RT_DEBUG_TRACE, ("TSSI = 0x%X\n", *TssiLinear0));
	DBGPRINT(RT_DEBUG_TRACE, ("temperature = 0x%X\n", pAd->chipCap.CurrentTemperBbpR49));
	//DBGPRINT(RT_DEBUG_TRACE, ("PacketType = 0x%X\n", PacketType));

	Power = pAd->ate.TxPower0;

	//DBGPRINT(RT_DEBUG_TRACE, ("Channel Desire Power = %d\n", Power));

	switch ( PacketType & 0x03)
	{
		case 0:
			TxRate = (PacketType >> 2) & 0x03;
			DBGPRINT(RT_DEBUG_TRACE, ("tx_11b_rate: %x\n", TxRate));
			switch (TxRate)
			{
				case 0:	// 1 Mbps
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_CCK_1M : BW20_MCS_POWER_CCK_1M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_CCK_1M;
					break;
				case 1:	// 2 Mbps
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_CCK_2M : BW20_MCS_POWER_CCK_2M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_CCK_2M;
					break;
				case 2:	// 5.5 Mbps
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_CCK_5M : BW20_MCS_POWER_CCK_5M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_CCK_5M;
					break;
				case 3:	// 11Mbps
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_CCK_11M : BW20_MCS_POWER_CCK_11M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_CCK_11M;
					break;
			}

			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R178, &BBPReg);
			if ( BBPReg == 0 )
			{
				Power += 24576;			// 3 * 8192
			}
			else
			{
				Power += 819;			//0.1 x 8192;
			}
			break;
		case 1:
			TxRate = (PacketType >> 4) & 0x0F;
			DBGPRINT(RT_DEBUG_TRACE, ("tx_11g_rate: %x\n", TxRate));
			switch ( TxRate )
			{
				case 0xB:	// 6 Mbps
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_OFDM_6M : BW20_MCS_POWER_OFDM_6M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_6M;
					break;
				case 0xF:	// 9 Mbps
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_OFDM_9M : BW20_MCS_POWER_OFDM_9M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_9M;
					break;
				case 0xA:	// 12 Mbps
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_OFDM_12M : BW20_MCS_POWER_OFDM_12M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_12M;
					break;
				case 0xE:	// 18 Mbps
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_OFDM_18M : BW20_MCS_POWER_OFDM_18M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_18M;
					break;
				case 0x9:	// 24 Mbps
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_OFDM_24M : BW20_MCS_POWER_OFDM_24M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_24M;
					break;
				case 0xD:	// 36 Mbps
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_OFDM_36M : BW20_MCS_POWER_OFDM_36M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_36M;
					break;
				case 0x8:	// 48 Mbps
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_OFDM_48M : BW20_MCS_POWER_OFDM_48M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_48M;
					break;
				case 0xC:	// 54 Mbps
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_OFDM_54M : BW20_MCS_POWER_OFDM_54M;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_OFDM_54M;
					break;
			}
			break;
		default:
			BbpR47 = (BbpR47 & ~0x07) | 0x02;
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, BbpR47);
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, &TxRate);
			DBGPRINT(RT_DEBUG_TRACE, ("tx_11n_rate: %x\n", TxRate));
			TxRate &= 0x7F;				// TxRate[7] is bandwidth
			switch ( TxRate )
			{
				case 0x0:
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_HT_MCS0: BW20_MCS_POWER_HT_MCS0;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS0;
					break;
				case 0x1:
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_HT_MCS1: BW20_MCS_POWER_HT_MCS1;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS1;
					break;
				case 0x2:
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_HT_MCS2: BW20_MCS_POWER_HT_MCS2;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS2;
					break;
				case 0x3:
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_HT_MCS3: BW20_MCS_POWER_HT_MCS3;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS3;
					break;
				case 0x4:
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_HT_MCS4: BW20_MCS_POWER_HT_MCS4;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS4;
					break;
				case 0x5:
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_HT_MCS5: BW20_MCS_POWER_HT_MCS5;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS5;
					break;
				case 0x6:
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_HT_MCS6: BW20_MCS_POWER_HT_MCS6;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS6;
					break;
				case 0x7:
					Power += (pAd->ate.TxWI.TxWIBW == BW_40)? BW40_MCS_POWER_HT_MCS7: BW20_MCS_POWER_HT_MCS7;
					Power = Power << 12;
					DBGPRINT(RT_DEBUG_TRACE, ("Channel PWR + MCS PWR = %x\n", Power));
					Power += RF_PA_MODE_HT_MCS7;
					break;

			}
			break;
	}

	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BBPReg);
	switch ( BBPReg & 0x3 )
	{
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
	DBGPRINT(RT_DEBUG_TRACE, ("TargetPower: 0x%x(%d)\n", *TargetPower, *TargetPower));

	return TRUE;
	
}


VOID MT7601ATEAsicTxAlcGetAutoAgcOffset(
	IN PRTMP_ADAPTER 			pAd)
{
	INT32 TargetPower, CurrentPower, PowerDiff;
	UCHAR TssiLinear0, TssiLinear1;
	CHAR tssi_offset;
	INT16 tssi_db, tssi_m_dc;
	UINT32 value;
	UCHAR ch = 0;
	MT7601_TX_ALC_DATA *pTxALCData = &pAd->chipCap.TxALCData;
	//PATE_INFO pATEInfo = &(pAd->ate);

	//if (pATEInfo->OneSecPeriodicRound % 4 == 0)
	{

		if ((pAd->ate.Channel >= 1) && (pAd->ate.Channel <= 14))
		{
			ch = pAd->ate.Channel;
		}
		else
		{
			ch = 1;

			DBGPRINT(RT_DEBUG_ERROR, ("%s::Incorrect channel #%d\n", __FUNCTION__, pAd->ate.Channel));
		}

              // if base power is lower than 10 dBm use High VGA
		if(pAd->TxPower[ch - 1].Power <= 20)
			pTxALCData->TSSI_USE_HVGA = 1;
		else
			pTxALCData->TSSI_USE_HVGA = 0;

		if (MT7601ATEGetTssiCompensationParam(pAd, &TssiLinear0 , &TssiLinear1, &TargetPower) == FALSE )
			return;
		
		tssi_m_dc = TssiLinear0 - ((pTxALCData->TSSI_USE_HVGA == 1) ? pTxALCData->TssiDC0_HVGA : pTxALCData->TssiDC0);

		
		tssi_db = lin2dBd(tssi_m_dc);

		if ( ch <= 4 )
			tssi_offset = pTxALCData->MT7601_TSSI_OFFSET[0];
		else if ( ch >= 9 )
			tssi_offset = pTxALCData->MT7601_TSSI_OFFSET[2];
		else
			tssi_offset = pTxALCData->MT7601_TSSI_OFFSET[1];

              if(pTxALCData->TSSI_USE_HVGA == 1)
  			tssi_db -= pTxALCData->TSSI_DBOFFSET_HVGA;

		CurrentPower = (pTxALCData->TssiSlope*tssi_db) + (tssi_offset << 9);

		DBGPRINT(RT_DEBUG_TRACE, ("CurrentPower: %d\n", CurrentPower));
  			
		PowerDiff = TargetPower - CurrentPower;

		DBGPRINT(RT_DEBUG_TRACE, ("PowerDiff: %d\n", PowerDiff));

		if((TssiLinear0 > 126) && ( PowerDiff > 0)) 			// upper saturation
		{
			DBGPRINT(RT_DEBUG_ERROR, ("%s :: upper saturation.\n", __FUNCTION__));
			PowerDiff = 0;
		}
		else
		{
			//if(((TssiLinear0 -TssiDC0) < 1) && (PowerDiff < 0)) 	// lower saturation
			if(((TssiLinear0 -((pTxALCData->TSSI_USE_HVGA == 1) ? pTxALCData->TssiDC0_HVGA : pTxALCData->TssiDC0)) < 1) && (PowerDiff < 0)) 	// lower saturation
			{
				DBGPRINT(RT_DEBUG_ERROR, ("%s :: lower saturation.\n", __FUNCTION__));
				PowerDiff = 0;
			}
		}

		if( ((pTxALCData->PowerDiffPre ^ PowerDiff) < 0 )
                  && ( (PowerDiff  < 4096) && (PowerDiff  > -4096))			// +- 0.5
		   && ( (pTxALCData->PowerDiffPre < 4096) && (pTxALCData->PowerDiffPre > -4096)))		// +- 0.5
		{ 
			if((PowerDiff > 0) && ((PowerDiff + pTxALCData->PowerDiffPre) >= 0))
				PowerDiff = 0;
			else if((PowerDiff < 0) && ((PowerDiff + pTxALCData->PowerDiffPre) < 0))
				PowerDiff = 0;
			else
				pTxALCData->PowerDiffPre = PowerDiff;
		}
		else 
		{
			pTxALCData->PowerDiffPre =  PowerDiff;
		}
		
		PowerDiff = PowerDiff + ((PowerDiff>0)?2048:-2048);
		PowerDiff = PowerDiff / 4096;

		DBGPRINT(RT_DEBUG_TRACE, ("final PowerDiff: %d(0x%x)\n", PowerDiff, PowerDiff));

		RTMP_IO_READ32(pAd, TX_ALC_CFG_1, &value);
		CurrentPower = (value & 0x3F);
		CurrentPower = CurrentPower > 0x1F ? CurrentPower - 0x40 : CurrentPower;
		PowerDiff += CurrentPower;
		if ( PowerDiff > 31 )
			PowerDiff = 31;
		if ( PowerDiff < -32 )
			PowerDiff = -32;
		//PowerDiff = PowerDiff + (value & 0x3F);
		value = (value & ~0x3F) | (PowerDiff & 0x3F);
		RTMP_IO_WRITE32(pAd, TX_ALC_CFG_1, value);
		DBGPRINT(RT_DEBUG_TRACE, ("MAC 13b4: 0x%x\n", value));		

		//MT7601AsicTemperatureCompensation(pAd);

		if ( RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD) )
		{
			MT7601_EnableTSSI(pAd);
			pTxALCData->TssiTriggered = 1;
		}

	}


}
#endif /* RTMP_INTERNAL_TX_ALC */


VOID MT7601ATEAsicTemperatureCompensation(
	IN PRTMP_ADAPTER			pAd)
{
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	PATE_INFO pATEInfo = &(pAd->ate);

	if (pATEInfo->OneSecPeriodicRound % 4 == 0)
	{
#ifdef RTMP_INTERNAL_TX_ALC
		if ( pAd->TxPowerCtrl.bInternalTxALC == FALSE )
#endif /* RTMP_INTERNAL_TX_ALC */
			MT7601_Read_Temperature(pAd, &pChipCap->CurrentTemperBbpR49);

		MT7601AsicTemperatureCompensation(pAd, FALSE);
	}
}

/*
	==========================================================================
	Description:
		Gives CCK TX rate 2 more dB TX power.
		This routine works only in LINK UP in INFRASTRUCTURE mode.

		calculate desired Tx power in RF R3.Tx0~5,	should consider -
		0. if current radio is a noisy environment (pAd->DrsCounters.fNoisyEnvironment)
		1. TxPowerPercentage
		2. auto calibration based on TSSI feedback
		3. extra 2 db for CCK
		4. -10 db upon very-short distance (AvgRSSI >= -40db) to AP

	NOTE: Since this routine requires the value of (pAd->DrsCounters.fNoisyEnvironment),
		it should be called AFTER MlmeDynamicTxRatSwitching()
	==========================================================================
 */

VOID MT7601ATEAsicAdjustTxPower(
	IN PRTMP_ADAPTER pAd) 
{


#ifdef RTMP_INTERNAL_TX_ALC
	/* Get temperature compensation delta power value */
	MT7601ATEAsicTxAlcGetAutoAgcOffset(pAd);
#endif /* RTMP_INTERNAL_TX_ALC */

}


/* 
==========================================================================
    Description:
        Set MT7601 ATE RF central frequency offset
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	MT7601_Set_ATE_TX_FREQ_OFFSET_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UCHAR RFFreqOffset = 0;
	UCHAR RFValue = 0;
	RFFreqOffset = simple_strtol(arg, 0, 10);

	pAd->ate.RFFreqOffset = RFFreqOffset;

	if ( IS_MT7601(pAd))
	{
		rlt_rf_write(pAd, RF_BANK0, RF_R12, pAd->ate.RFFreqOffset);

		rlt_rf_write(pAd, RF_BANK0, RF_R04, 0x0A);
		rlt_rf_write(pAd, RF_BANK0, RF_R05, 0x20);
		rlt_rf_read(pAd, RF_BANK0, RF_R04, &RFValue);
		RFValue = RFValue | 0x80; 	/* vcocal_en (initiate VCO calibration (reset after completion)) - It should be at the end of RF configuration. */
		rlt_rf_write(pAd, RF_BANK0, RF_R04, RFValue);
		RTMPusecDelay(2000);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_FREQOFFSET_Proc (RFFreqOffset = %d)\n", pAd->ate.RFFreqOffset));
	DBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_FREQOFFSET_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif /* CONFIG_AP_SUPPORT */
	
	return TRUE;
}


VOID MT7601ATERxVGAInit(
	IN PRTMP_ADAPTER		pAd)
{
	UCHAR R66 = 0x14;
	
	//RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, 0x14);

	ATEBBPWriteWithRxChain(pAd, BBP_R66, R66, RX_CHAIN_ALL);


	return;
}


VOID MT7601ATEAsicSetTxRxPath(
    IN PRTMP_ADAPTER pAd)
{

	AsicSetRxAnt(pAd, pAd->ate.RxAntennaSel);
}


struct _ATE_CHIP_STRUCT MT7601ATE =
{
	/* functions */
	.ChannelSwitch = MT7601ATEAsicSwitchChannel,
	.TxPwrHandler = MT7601ATETxPwrHandler,
	.TssiCalibration = NULL,
	.ExtendedTssiCalibration = NULL /* RT5572_ATETssiCalibrationExtend */,
	.RxVGAInit = MT7601ATERxVGAInit,
	.AsicSetTxRxPath = MT7601ATEAsicSetTxRxPath,
	.AdjustTxPower = MT7601ATEAsicAdjustTxPower,
	//.AsicExtraPowerOverMAC = DefaultATEAsicExtraPowerOverMAC,
	.Set_BW_Proc = MT7601_Set_ATE_TX_BW_Proc,
	.Set_FREQ_OFFSET_Proc = MT7601_Set_ATE_TX_FREQ_OFFSET_Proc,
	.TemperCompensation = MT7601ATEAsicTemperatureCompensation,

	/* variables */
	.maxTxPwrCnt = 5,
	.bBBPStoreTXCARR = FALSE,
	.bBBPStoreTXCARRSUPP = FALSE,	
	.bBBPStoreTXCONT = FALSE,
	.bBBPLoadATESTOP = FALSE,/* ralink debug */
};


#endif /* MT7601 */

