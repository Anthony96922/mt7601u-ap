/*

*/


#include "rt_config.h"


/*
	========================================================================
	
	Routine Description:
		Calculates the duration which is required to transmit out frames
	with given size and specified rate.
					  
	Arguments:
		pTxWI		Pointer to head of each MPDU to HW.
		Ack 		Setting for Ack requirement bit
		Fragment	Setting for Fragment bit
		RetryMode	Setting for retry mode
		Ifs 		Setting for IFS gap
		Rate		Setting for transmit rate
		Service 	Setting for service
		Length		Frame length
		TxPreamble	Short or Long preamble when using CCK rates
		QueIdx - 0-3, according to 802.11e/d4.4 June/2003
		
	Return Value:
		None
	
	See also : BASmartHardTransmit()    !!!
	
	========================================================================
*/
VOID RTMPWriteTxWI(
	IN RTMP_ADAPTER *pAd,
	IN TXWI_STRUC *pOutTxWI,
	IN BOOLEAN FRAG,
	IN BOOLEAN CFACK,
	IN BOOLEAN InsTimestamp,
	IN BOOLEAN AMPDU,
	IN BOOLEAN Ack,
	IN BOOLEAN NSeq,		/* HW new a sequence.*/
	IN UCHAR BASize,
	IN UCHAR WCID,
	IN ULONG Length,
	IN UCHAR PID,
	IN UCHAR TID,
	IN UCHAR TxRate,
	IN UCHAR Txopmode,
	IN BOOLEAN CfAck,
	IN HTTRANSMIT_SETTING *pTransmit)
{
	PMAC_TABLE_ENTRY pMac = NULL;
	TXWI_STRUC TxWI, *pTxWI;
	UINT8 TXWISize = pAd->chipCap.TXWISize;

	if (WCID < MAX_LEN_OF_MAC_TABLE)
		pMac = &pAd->MacTab.Content[WCID];

	
	/* 
		Always use Long preamble before verifiation short preamble functionality works well.
		Todo: remove the following line if short preamble functionality works
	*/
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_SHORT_PREAMBLE_INUSED);
	NdisZeroMemory(&TxWI, TXWISize);
	pTxWI = &TxWI;
	pTxWI->TxWIFRAG= FRAG;
	pTxWI->TxWICFACK = CFACK;
	pTxWI->TxWITS= InsTimestamp;
	pTxWI->TxWIAMPDU = AMPDU;
	pTxWI->TxWIACK = Ack;
	pTxWI->TxWITXOP= Txopmode;
	
	pTxWI->TxWINSEQ = NSeq;
	/* John tune the performace with Intel Client in 20 MHz performance*/
#ifdef DOT11_N_SUPPORT
	BASize = pAd->CommonCfg.TxBASize;
#ifdef RT65xx
	if (IS_RT65XX(pAd))
	{
		if (BASize > 31)
			BASize = 31;
	}
	else
#endif /* RT65xx */
#ifdef MT7601
	if (IS_MT7601(pAd))
	{
		if (BASize > 15)
			BASize = 15;
	}
	else
#endif /* MT7601 */
	if (pAd->MACVersion == 0x28720200)
	{
		if (BASize > 13)
			BASize = 13;
	}
	else
	{
		if (BASize > 7)
			BASize = 7;
	}

	pTxWI->TxWIBAWinSize = BASize;
	pTxWI->TxWIShortGI = pTransmit->field.ShortGI;
	pTxWI->TxWISTBC = pTransmit->field.STBC;

#ifdef TXBF_SUPPORT
	if (pMac && pAd->chipCap.FlgHwTxBfCap)
	{
		if (pMac->TxSndgType == SNDG_TYPE_NDP  || pMac->TxSndgType == SNDG_TYPE_SOUNDING || pTxWI->eTxBF)
			pTxWI->TxWISTBC = 0;
	}
#endif /* TXBF_SUPPORT */
#endif /* DOT11_N_SUPPORT */
		
	pTxWI->TxWIWirelessCliID = WCID;
	pTxWI->TxWIMPDUByteCnt = Length;
#ifdef RLT_MAC
	pTxWI->TxWIPacketId = PID;
#endif /* RLT_MAC */
	
	/* If CCK or OFDM, BW must be 20*/
	pTxWI->TxWIBW = (pTransmit->field.MODE <= MODE_OFDM) ? (BW_20) : (pTransmit->field.BW);
#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
	if (pTxWI->TxWIBW)
		pTxWI->TxWIBW = (pAd->CommonCfg.AddHTInfo.AddHtInfo.RecomWidth == 0) ? (BW_20) : (pTransmit->field.BW);
#endif /* DOT11N_DRAFT3 */
#endif /* DOT11_N_SUPPORT */

	pTxWI->TxWIMCS = pTransmit->field.MCS;
	pTxWI->TxWIPHYMODE = pTransmit->field.MODE;
	pTxWI->TxWICFACK = CfAck;

#ifdef DOT11_N_SUPPORT
	if (pMac)
	{
		if (pAd->CommonCfg.bMIMOPSEnable)
		{
			if ((pMac->MmpsMode == MMPS_DYNAMIC) && (pTransmit->field.MCS > 7))
			{
				/* Dynamic MIMO Power Save Mode*/
				pTxWI->TxWIMIMOps = 1;
			}
			else if (pMac->MmpsMode == MMPS_STATIC)
			{
				/* Static MIMO Power Save Mode*/
				if (pTransmit->field.MODE >= MODE_HTMIX && pTransmit->field.MCS > 7)
				{
					pTxWI->TxWIMCS = 7;
					pTxWI->TxWIMIMOps = 0;
				}
			}
		}
		/*pTxWI->TxWIMIMOps = (pMac->PsMode == PWR_MMPS)? 1:0;*/
		pTxWI->TxWIMpduDensity = pMac->MpduDensity;
	}
#endif /* DOT11_N_SUPPORT */


#ifdef RLT_MAC
	pTxWI->TxWIPacketId = pTxWI->TxWIMCS;
#endif /* RLT_MAC */
	NdisMoveMemory(pOutTxWI, &TxWI, TXWISize);
//+++Add by shiang for debug
#ifdef DBG
	hex_dump("TxWI", (UCHAR *)pOutTxWI, TXWISize);
#endif /* DBG */
//---Add by shiang for debug
}


VOID RTMPWriteTxWI_Data(RTMP_ADAPTER *pAd, TXWI_STRUC *pTxWI, TX_BLK *pTxBlk)
{
	HTTRANSMIT_SETTING *pTransmit;
	MAC_TABLE_ENTRY *pMacEntry;
#ifdef DOT11_N_SUPPORT
	UCHAR BASize;
#endif /* DOT11_N_SUPPORT */
	UINT8 TXWISize = pAd->chipCap.TXWISize;
#ifdef WFA_VHT_PF
	BOOLEAN amsdu_in_ampdu = FALSE;
#endif /* WFA_VHT_PF */

	ASSERT(pTxWI);

	pTransmit = pTxBlk->pTransmit;
	pMacEntry = pTxBlk->pMacEntry;

	/*
		Always use Long preamble before verifiation short preamble functionality works well.
		Todo: remove the following line if short preamble functionality works
	*/
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_SHORT_PREAMBLE_INUSED);
	NdisZeroMemory(pTxWI, TXWISize);
	
	pTxWI->TxWIFRAG = TX_BLK_TEST_FLAG(pTxBlk, fTX_bAllowFrag);
	pTxWI->TxWIACK = TX_BLK_TEST_FLAG(pTxBlk, fTX_bAckRequired);
	pTxWI->TxWITXOP = pTxBlk->FrameGap;

	pTxWI->TxWIWirelessCliID = pTxBlk->Wcid;

#ifdef HDR_TRANS_SUPPORT
	if (pTxBlk->NeedTrans )
		pTxWI->TxWIMPDUByteCnt = pTxBlk->SrcBufLen;
	else
#endif /* HDR_TRANS_SUPPORT */
	pTxWI->TxWIMPDUByteCnt = pTxBlk->MpduHeaderLen + pTxBlk->SrcBufLen;
	pTxWI->TxWICFACK = TX_BLK_TEST_FLAG(pTxBlk, fTX_bPiggyBack);

	/* If CCK or OFDM, BW must be 20 */
	pTxWI->TxWIBW = (pTransmit->field.MODE <= MODE_OFDM) ? (BW_20) : (pTransmit->field.BW);
#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
	if (pTxWI->TxWIBW)
		pTxWI->TxWIBW = (pAd->CommonCfg.AddHTInfo.AddHtInfo.RecomWidth == 0) ? (BW_20) : (pTransmit->field.BW);
#endif /* DOT11N_DRAFT3 */

#ifdef WFA_VHT_PF
	if ((pAd->force_amsdu == TRUE) && (pTxBlk->TxFrameType == TX_AMSDU_FRAME) && 
		(pTxWI->TxWIWirelessCliID != MCAST_WCID) && pMacEntry)
	{
		DBGPRINT(RT_DEBUG_INFO, ("%s():f_amsdu=%d, pTxBlk->TxFrameType=%d, pMacEntry->TXBAbitmap=0x%x, pTxBlk->UserPriority=%d, cond_match=%d!\n", 
					__FUNCTION__, pAd->force_amsdu, pTxBlk->TxFrameType, pMacEntry->TXBAbitmap, 
					pTxBlk->UserPriority, ((pMacEntry->TXBAbitmap & (1<<pTxBlk->UserPriority)) != 0) ? TRUE : FALSE));


		if ((pAd->force_amsdu == TRUE) && (pTxBlk->TxFrameType == TX_AMSDU_FRAME)
			&& (pMacEntry && ((pMacEntry->TXBAbitmap & (1<<pTxBlk->UserPriority)) != 0)))
			amsdu_in_ampdu = TRUE;
	}
#endif /* WFA_VHT_PF */

#ifdef WFA_VHT_PF
	if (amsdu_in_ampdu)
		pTxWI->TxWIAMPDU = TRUE;
	else
#endif /* WFA_VHT_PF */
	pTxWI->TxWIAMPDU = ((pTxBlk->TxFrameType == TX_AMPDU_FRAME) ? TRUE : FALSE);

#ifdef TXBF_SUPPORT
	if(pTxBlk->TxSndgPkt > SNDG_TYPE_DISABLE)
		pTxWI->TxWIAMPDU = FALSE;
#endif /* TXBF_SUPPORT */

	BASize = pAd->CommonCfg.TxBASize;
	if((pTxBlk->TxFrameType == TX_AMPDU_FRAME
#ifdef WFA_VHT_PF
		|| amsdu_in_ampdu == TRUE
#endif /* WFA_VHT_PF */
		) && (pMacEntry))
	{
		UCHAR RABAOriIdx = pTxBlk->pMacEntry->BAOriWcidArray[pTxBlk->UserPriority];

		BASize = pAd->BATable.BAOriEntry[RABAOriIdx].BAWinSize;
	}

#ifdef TXBF_SUPPORT
	if (pTxBlk->TxSndgPkt == SNDG_TYPE_SOUNDING)
	{
		pTxWI->Sounding = 1;
		DBGPRINT(RT_DEBUG_TRACE, ("ETxBF in RTMPWriteTxWI_Data(): sending normal sounding, eTxBF=%d\n", pTxWI->eTxBF));
		pTxWI->iTxBF = 0;
	}
	else if (pTxBlk->TxSndgPkt == SNDG_TYPE_NDP)
	{
		if (pTxBlk->TxNDPSndgMcs >= 16)
			pTxWI->NDPSndRate = 2;
		else if (pTxBlk->TxNDPSndgMcs >= 8)
			pTxWI->NDPSndRate = 1;
		else
			pTxWI->NDPSndRate = 0;

		pTxWI->NDPSndBW = pTransmit->field.BW;
		pTxWI->iTxBF = 0;
	}
	else
	{
#ifdef MFB_SUPPORT
		if (pMacEntry && (pMacEntry->mrqCnt >0) && (pMacEntry->toTxMrq == TRUE))
			pTxWI->eTxBF = ~(pTransmit->field.eTxBF);
		else
#endif	/* MFB_SUPPORT */
			pTxWI->eTxBF = pTransmit->field.eTxBF;
		pTxWI->iTxBF = pTransmit->field.iTxBF;
	}
#endif /* TXBF_SUPPORT */

	pTxWI->TxWIBAWinSize = BASize;
	pTxWI->TxWIShortGI = pTransmit->field.ShortGI;
	pTxWI->TxWISTBC = pTransmit->field.STBC;
#ifdef TXBF_SUPPORT
	if (pTxBlk->TxSndgPkt == SNDG_TYPE_NDP  || pTxBlk->TxSndgPkt == SNDG_TYPE_SOUNDING || pTxWI->eTxBF)
		pTxWI->TxWISTBC = 0;
#endif /* TXBF_SUPPORT */

#endif /* DOT11_N_SUPPORT */
	
	pTxWI->TxWIMCS = pTransmit->field.MCS;
	pTxWI->TxWIPHYMODE = pTransmit->field.MODE;


#ifdef DOT11_N_SUPPORT
	if (pMacEntry)
	{
		if ((pMacEntry->MmpsMode == MMPS_DYNAMIC) && (pTransmit->field.MCS > 7))
		{
			/* Dynamic MIMO Power Save Mode*/
			pTxWI->TxWIMIMOps = 1;
		}
		else if (pMacEntry->MmpsMode == MMPS_STATIC)
		{
			/* Static MIMO Power Save Mode*/
			if (pTransmit->field.MODE >= MODE_HTMIX && pTransmit->field.MCS > 7)
			{
				pTxWI->TxWIMCS = 7;
				pTxWI->TxWIMIMOps = 0;
			}
		}

		pTxWI->TxWIMpduDensity = pMacEntry->MpduDensity;
	}
#endif /* DOT11_N_SUPPORT */
	
#ifdef TXBF_SUPPORT
	if (pTxBlk->TxSndgPkt > SNDG_TYPE_DISABLE)
	{
		pTxWI->TxWIMCS = 0;
		pTxWI->TxWIAMPDU = FALSE;
	}
#endif /* TXBF_SUPPORT */
	
#ifdef DBG_DIAGNOSE
	if (pTxBlk->QueIdx== 0)
	{
		pAd->DiagStruct.TxDataCnt[pAd->DiagStruct.ArrayCurIdx]++;
		pAd->DiagStruct.TxMcsCnt[pAd->DiagStruct.ArrayCurIdx][pTxWI->MCS]++;
	}
#endif /* DBG_DIAGNOSE */

#ifdef RLT_MAC
	/* for rate adapation*/
	pTxWI->TxWIPacketId = pTxWI->TxWIMCS;
#endif /* RLT_MAC */


#ifdef INF_AMAZON_SE
	/*Iverson patch for WMM A5-T07 ,WirelessStaToWirelessSta do not bulk out aggregate */
	if( RTMP_GET_PACKET_NOBULKOUT(pTxBlk->pPacket))
	{
		if(pTxWI->TxWIPHYMODE == MODE_CCK)
			pTxWI->TxWIPacketId = 6;
	}	
#endif /* INF_AMAZON_SE */	


#ifdef FPGA_MODE
	if (pAd->fpga_on & 0x2)
	{
		pTxWI->TxWIPHYMODE = pAd->data_phy;
		pTxWI->TxWIMCS = pAd->data_mcs;
		pTxWI->TxWIBW = pAd->data_bw;
		pTxWI->TxWIShortGI = pAd->data_gi;
		if (pAd->data_basize)
			pTxWI->TxWIBAWinSize = pAd->data_basize;
	}
#endif /* FPGA_MODE */


}


VOID RTMPWriteTxWI_Cache(
	IN RTMP_ADAPTER *pAd,
	INOUT TXWI_STRUC *pTxWI,
	IN TX_BLK *pTxBlk)
{
	HTTRANSMIT_SETTING *pTransmit;
	MAC_TABLE_ENTRY *pMacEntry;
#ifdef DOT11_N_SUPPORT
#endif /* DOT11_N_SUPPORT */
	
	
	/* update TXWI */
	pMacEntry = pTxBlk->pMacEntry;
	pTransmit = pTxBlk->pTransmit;
	
	if (pMacEntry->bAutoTxRateSwitch)
	{
		pTxWI->TxWITXOP = IFS_HTTXOP;

		/* If CCK or OFDM, BW must be 20*/
		pTxWI->TxWIBW = (pTransmit->field.MODE <= MODE_OFDM) ? (BW_20) : (pTransmit->field.BW);
		pTxWI->TxWIShortGI = pTransmit->field.ShortGI;
		pTxWI->TxWISTBC = pTransmit->field.STBC;

#ifdef TXBF_SUPPORT
		if (pTxBlk->TxSndgPkt == SNDG_TYPE_NDP  || pTxBlk->TxSndgPkt == SNDG_TYPE_SOUNDING || pTxWI->eTxBF)
			pTxWI->TxWISTBC = 0;
#endif /* TXBF_SUPPORT */

		pTxWI->TxWIMCS = pTransmit->field.MCS;
		pTxWI->TxWIPHYMODE = pTransmit->field.MODE;

#ifdef RLT_MAC
		/* set PID for TxRateSwitching*/
		pTxWI->TxWIPacketId = pTransmit->field.MCS;
#endif /* RLT_MAC */
		
	}

#ifdef DOT11_N_SUPPORT
	pTxWI->TxWIAMPDU = ((pMacEntry->NoBADataCountDown == 0) ? TRUE: FALSE);
#ifdef TXBF_SUPPORT
	if(pTxBlk->TxSndgPkt > SNDG_TYPE_DISABLE)
		pTxWI->TxWIAMPDU = FALSE;
#endif /* TXBF_SUPPORT */

	pTxWI->TxWIMIMOps = 0;

#ifdef DOT11N_DRAFT3
	if (pTxWI->TxWIBW)
		pTxWI->TxWIBW = (pAd->CommonCfg.AddHTInfo.AddHtInfo.RecomWidth == 0) ? (BW_20) : (pTransmit->field.BW);
#endif /* DOT11N_DRAFT3 */

    if (pAd->CommonCfg.bMIMOPSEnable)
    {
		/* MIMO Power Save Mode*/
		if ((pMacEntry->MmpsMode == MMPS_DYNAMIC) && (pTransmit->field.MCS > 7))
		{
			/* Dynamic MIMO Power Save Mode*/
			pTxWI->TxWIMIMOps = 1;
		}
		else if (pMacEntry->MmpsMode == MMPS_STATIC)
		{
			/* Static MIMO Power Save Mode*/
			if ((pTransmit->field.MODE >= MODE_HTMIX) && (pTransmit->field.MCS > 7))
			{
				pTxWI->TxWIMCS = 7;
				pTxWI->TxWIMIMOps = 0;
			}
		}
    }

#endif /* DOT11_N_SUPPORT */

#ifdef DBG_DIAGNOSE
	if (pTxBlk->QueIdx== 0)
	{
		pAd->DiagStruct.TxDataCnt[pAd->DiagStruct.ArrayCurIdx]++;
		pAd->DiagStruct.TxMcsCnt[pAd->DiagStruct.ArrayCurIdx][pTxWI->MCS]++;
	}
#endif /* DBG_DIAGNOSE */

#ifdef TXBF_SUPPORT
	if (pTxBlk->TxSndgPkt == SNDG_TYPE_SOUNDING)
	{
		pTxWI->Sounding = 1;
		pTxWI->eTxBF = 0;
		pTxWI->iTxBF = 0;
		DBGPRINT(RT_DEBUG_TRACE, ("ETxBF in RTMPWriteTxWI_Cache(): sending normal sounding, eTxBF=%d\n", pTxWI->eTxBF));
	}
	else if (pTxBlk->TxSndgPkt == SNDG_TYPE_NDP)
	{
		if (pTxBlk->TxNDPSndgMcs>=16)
			pTxWI->NDPSndRate = 2;
		else if (pTxBlk->TxNDPSndgMcs>=8)
			pTxWI->NDPSndRate = 1;
		else
			pTxWI->NDPSndRate = 0;
		pTxWI->Sounding = 0;
		pTxWI->eTxBF = 0;
		pTxWI->iTxBF = 0;

		pTxWI->NDPSndBW = pTransmit->field.BW;

/*
		DBGPRINT(RT_DEBUG_TRACE,
				("%s():ETxBF, sending ndp sounding(BW=%d, Rate=%d, eTxBF=%d)\n",
				__FUNCTION__, pTxWI->NDPSndBW, pTxWI->NDPSndRate, pTxWI->eTxBF));
*/
	}
	else
	{
		pTxWI->Sounding = 0;
#ifdef MFB_SUPPORT
		if (pMacEntry && pMacEntry->mrqCnt >0 && pMacEntry->toTxMrq == 1)
		{
			pTxWI->eTxBF = ~(pTransmit->field.eTxBF);
			DBGPRINT_RAW(RT_DEBUG_TRACE,("ETxBF in AP_AMPDU_Frame_Tx(): invert eTxBF\n"));
		}
		else
#endif	/* MFB_SUPPORT */
			pTxWI->eTxBF = pTransmit->field.eTxBF;

		pTxWI->iTxBF = pTransmit->field.iTxBF;

		if (pTxWI->eTxBF || pTxWI->iTxBF)
			pTxWI->TxWISTBC = 0;
	}

	if (pTxBlk->TxSndgPkt > SNDG_TYPE_DISABLE)
	{
		pTxWI->TxWIMCS = 0;
		pTxWI->TxWIAMPDU = FALSE;
	}
#endif /* TXBF_SUPPORT */

#ifdef HDR_TRANS_SUPPORT
	if (pTxBlk->NeedTrans )
		pTxWI->TxWIMPDUByteCnt = pTxBlk->SrcBufLen;
	else
#endif /* HDR_TRANS_SUPPORT */
	pTxWI->TxWIMPDUByteCnt = pTxBlk->MpduHeaderLen + pTxBlk->SrcBufLen;


#ifdef FPGA_MODE
	if (pAd->fpga_on & 0x2)
	{
		pTxWI->TxWIPHYMODE = pAd->data_phy;
		pTxWI->TxWIMCS = pAd->data_mcs;
		pTxWI->TxWIBW = pAd->data_bw;
		pTxWI->TxWIShortGI = pAd->data_gi;
		if (pAd->data_basize)
			pTxWI->TxWIBAWinSize = pAd->data_basize;
	}
#endif /* FPGA_MODE */


}


INT rtmp_mac_set_band(RTMP_ADAPTER *pAd, int  band)
{
	UINT32 val, band_cfg;


	RTMP_IO_READ32(pAd, TX_BAND_CFG, &band_cfg);
	val = band_cfg & (~0x6);
	switch (band)
	{
		case BAND_5G:
			val |= 0x02;
			break;
		case BAND_24G:
		default:
			val |= 0x4;
			break;
	}

	if (val != band_cfg)
		RTMP_IO_WRITE32(pAd, TX_BAND_CFG, val);

	return TRUE;
}


INT rtmp_mac_set_ctrlch(RTMP_ADAPTER *pAd, INT extch)
{
	UINT32 val, band_cfg;


	RTMP_IO_READ32(pAd, TX_BAND_CFG, &band_cfg);
	val = band_cfg & (~0x1);
	switch (extch)
	{
		case EXTCHA_ABOVE:
			val &= (~0x1);
			break;
		case EXTCHA_BELOW:
			val |= (0x1);
			break;
		case EXTCHA_NONE:
			val &= (~0x1);
			break;
	}

	if (val != band_cfg)
		RTMP_IO_WRITE32(pAd, TX_BAND_CFG, val);
	
	return TRUE;
}


INT rtmp_mac_set_mmps(RTMP_ADAPTER *pAd, INT ReduceCorePower)
{
	UINT32 mac_val, org_val;

	RTMP_IO_READ32(pAd, 0x1210, &org_val);
	mac_val = org_val;
	if (ReduceCorePower)
		mac_val |= 0x09;
	else
		mac_val &= ~0x09;

	if (mac_val != org_val)
		RTMP_IO_WRITE32(pAd, 0x1210, mac_val);

	return TRUE;
}

