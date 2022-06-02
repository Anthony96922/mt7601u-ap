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
	multi_channel.c
 
    Abstract:
 
    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
 */

 
#include "rt_config.h"

#ifdef CONFIG_MULTI_CHANNEL

unsigned int SwitchTime1, SwitchTime2, SwitchTime3, SwitchTime4, SwitchTime5, SwitchTime6, SwitchTime7, SwitchTime8;
unsigned int TempTimeLo, TempTimeHi;

extern INT Set_P2pCli_Enable_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

extern INT Set_P2p_OpMode_Proc(
	IN PRTMP_ADAPTER pAd,
	IN char * arg);

VOID RtmpPrepareHwNullFrame(
	IN PRTMP_ADAPTER pAd,
	IN PMAC_TABLE_ENTRY pEntry,
	IN bool bQosNull,
	IN bool bEOSP,
	IN unsigned char OldUP,
	IN unsigned char OpMode,
	IN unsigned char PwrMgmt,
	IN bool bWaitACK,
	IN CHAR Index)
{
	unsigned char TXWISize = pAd->chipCap.TXWISize;
	TXWI_STRUC *pTxWI;
	TXINFO_STRUC *pTxInfo;
	unsigned char * pNullFrame;
	PHEADER_802_11 pNullFr;
	unsigned int frameLen;
	unsigned int totalLen;
	unsigned char *ptr;
	unsigned int i;
	unsigned int longValue;
	unsigned char MlmeRate;

#ifdef RT_BIG_ENDIAN
	NDIS_STATUS    NState;
	unsigned char * pNullFrBuf;
#endif /* RT_BIG_ENDIAN */


	NdisZeroMemory(pAd->NullFrBuf, sizeof(pAd->NullFrBuf));
	pTxWI = (TXWI_STRUC *)&pAd->NullFrBuf[0];
	pNullFrame = &pAd->NullFrBuf[TXWISize];

	pNullFr = (PHEADER_802_11) pNullFrame;
	frameLen = sizeof(HEADER_802_11);

	pNullFr->FC.Type = BTYPE_DATA;
	pNullFr->FC.SubType = SUBTYPE_NULL_FUNC;
	pNullFr->FC.ToDs = 1;
	pNullFr->FC.FrDs = 0;

	COPY_MAC_ADDR(pNullFr->Addr1, pEntry->Addr);
	{
		COPY_MAC_ADDR(pNullFr->Addr2, pAd->CurrentAddress);
		COPY_MAC_ADDR(pNullFr->Addr3, pAd->CommonCfg.Bssid);
	}

	pNullFr->FC.PwrMgmt = PwrMgmt;

	pNullFr->Duration = pAd->CommonCfg.Dsifs + RTMPCalcDuration(pAd, pAd->CommonCfg.TxRate, 14);

	/* sequence is increased in MlmeHardTx */
	pNullFr->Sequence = pAd->Sequence;
	pAd->Sequence = (pAd->Sequence+1) & MAXSEQ; /* next sequence  */

	if (bQosNull)
	{
		unsigned char *qos_p = ((unsigned char *)pNullFr) + frameLen;

		pNullFr->FC.SubType = SUBTYPE_QOS_NULL;

		/* copy QOS control bytes */
		qos_p[0] = ((bEOSP) ? (1 << 4) : 0) | OldUP;
		qos_p[1] = 0;
		frameLen += 2;
	} /* End of if */

	RTMPWriteTxWI(pAd, pTxWI,  FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, 0, pEntry->Aid, frameLen,
		0, 0, (unsigned char)pAd->CommonCfg.MlmeTransmit.field.MCS, IFS_HTTXOP, FALSE, &pAd->CommonCfg.MlmeTransmit);

	dumpTxWI(pAd, pTxWI);

	if (bWaitACK)
		pTxWI->TxWITXRPT = 1;

	hex_dump("RtmpPrepareHwNullFrame", pAd->NullFrBuf, TXWISize + frameLen);

	totalLen = TXWISize + frameLen;
	pAd->NullFrLen = totalLen;
	ptr = pAd->NullFrBuf;

#ifdef RT_BIG_ENDIAN
	NState = os_alloc_mem(pAd, (unsigned char * *) &pNullFrBuf, 100);
	if ( NState == NDIS_STATUS_FAILURE )
		return;

	NdisZeroMemory(pNullFrame, 100);
	NdisMoveMemory(pNullFrBuf, pAd->NullFrBuf, totalLen);
	RTMPWIEndianChange(pAd, pNullFrBuf, TYPE_TXWI);
	RTMPFrameEndianChange(pAd, (unsigned char *)pNullFrBuf + TXWISize, DIR_WRITE, FALSE);

	ptr = pNullFrBuf;
#endif /* RT_BIG_ENDIAN */


	for (i = 0; i < totalLen; i += 4)
	{
		longValue =  *ptr + (*(ptr + 1) << 8) + (*(ptr + 2) << 16) + (*(ptr + 3) << 24);
		//hex_dump("null frame before",&longValue, 4);

		if (Index == 0)
			RTMP_IO_WRITE32(pAd, pAd->NullBufOffset[0] + i, longValue);
		else
			RTMP_IO_WRITE32(pAd, pAd->NullBufOffset[1] + i, longValue);

		//RTMP_IO_WRITE32(pAd, 0xB700 + i, longValue);
		//RTMP_IO_WRITE32(pAd, 0xB780 + i, longValue);

		ptr += 4;
	}
}


VOID RTMPHwSendNullFrame(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char TxRate,
	IN bool bQosNull,
	IN unsigned short PwrMgmt,
	IN CHAR Index)
{

	unsigned char TXWISize = pAd->chipCap.TXWISize;
	NDIS_STATUS    NState;
	PHEADER_802_11 pNullFr;
	unsigned char *ptr;
	unsigned int longValue;
#ifdef RT_BIG_ENDIAN
	unsigned char * pNullFrame;
#endif /* RT_BIG_ENDIAN */
	unsigned int Data = 0;

	DBGPRINT(RT_DEBUG_TRACE, ("%s - Send NULL Frame @%d Mbps...%d \n", __FUNCTION__, RateIdToMbps[pAd->CommonCfg.TxRate],PwrMgmt));

	pNullFr = (PHEADER_802_11)((&pAd->NullFrBuf[0]) +TXWISize);

	pNullFr->FC.PwrMgmt = PwrMgmt;

	pNullFr->Duration = pAd->CommonCfg.Dsifs + RTMPCalcDuration(pAd, TxRate, 14);

	/* sequence is increased in MlmeHardTx */
	pNullFr->Sequence = pAd->Sequence;
	pAd->Sequence = (pAd->Sequence+1) & MAXSEQ; /* next sequence  */

	//hex_dump("RtmpPrepareHwNullFrame", pAd->NullFrBuf,  pAd->NullFrLen);

	if (Index == 0)
	{
		ptr = pAd->NullFrBuf + TXWISize;

#ifdef RT_BIG_ENDIAN
		longValue =  (*ptr << 8) + *(ptr + 1) + (*(ptr + 2) << 16) + (*(ptr + 3) << 24);
#else
		longValue =  *ptr + (*(ptr + 1) << 8) + (*(ptr + 2) << 16) + (*(ptr + 3) << 24);
#endif /* RT_BIG_ENDIAN */
		RTMP_IO_WRITE32(pAd, pAd->NullBufOffset[0] + TXWISize, longValue);

		ptr = pAd->NullFrBuf + TXWISize + 20;	// update Sequence
		longValue =  *ptr + (*(ptr + 1) << 8) + (*(ptr + 2) << 16) + (*(ptr + 3) << 24);
		RTMP_IO_WRITE32(pAd, pAd->NullBufOffset[0] + TXWISize + 20, longValue);
		
		RTMP_IO_READ32(pAd, PBF_CFG, &Data);
		Data &= 0xffffff1f; /* Null 2 frame buffer select bit[5:7]=0 */
		RTMP_IO_WRITE32(pAd, PBF_CFG, Data);
	}
	else if (Index == 1)
	{
		ptr = pAd->NullFrBuf + TXWISize;
#ifdef RT_BIG_ENDIAN
		longValue =  (*ptr << 8) + *(ptr + 1) + (*(ptr + 2) << 16) + (*(ptr + 3) << 24);
#else
		longValue =  *ptr + (*(ptr + 1) << 8) + (*(ptr + 2) << 16) + (*(ptr + 3) << 24);
#endif /* RT_BIG_ENDIAN */
		RTMP_IO_WRITE32(pAd, pAd->NullBufOffset[1] + TXWISize, longValue);

		ptr = pAd->NullFrBuf + TXWISize + 20;	// update Sequence
		longValue =  *ptr + (*(ptr + 1) << 8) + (*(ptr + 2) << 16) + (*(ptr + 3) << 24);
		RTMP_IO_WRITE32(pAd, pAd->NullBufOffset[1] + TXWISize + 20, longValue);
		
		RTMP_IO_READ32(pAd, PBF_CFG, &Data);
		Data &= 0xffffff1f; /* Null 2 frame buffer select bit[5:7]=1 */
		Data |= 0x20; /* Null 2 frame buffer select bit[5:7]=1 */
		RTMP_IO_WRITE32(pAd, PBF_CFG, Data);
	}

	RTMP_IO_WRITE32(pAd, PBF_CTRL, 0x04);

}


/* 
	==========================================================================
	Description:
		Send out a NULL frame to a specified STA at a higher TX rate. The 
		purpose is to ensure the designated client is okay to received at this
		rate.
	==========================================================================
 */
VOID RtmpEnqueueLastNullFrame(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pAddr,
	IN unsigned char TxRate,
	IN unsigned char PID,
	IN unsigned char apidx,
    IN bool bQosNull,
    IN bool bEOSP,
    IN unsigned char OldUP,
    IN unsigned char PwrMgmt,
	IN unsigned char OpMode)
{
	unsigned char	NullFrame[48];
	unsigned long	Length;
	PHEADER_802_11	pHeader_802_11;
	MAC_TABLE_ENTRY *pEntry = NULL;
	PAPCLI_STRUCT pApCliEntry = NULL;

	pEntry = MacTableLookup(pAd, pAddr);

	if (pEntry == NULL)
		return;

	NdisZeroMemory(NullFrame, 48);
	Length = sizeof(HEADER_802_11);

	pHeader_802_11 = (PHEADER_802_11) NullFrame;
	
	pHeader_802_11->FC.Type = BTYPE_DATA;
	pHeader_802_11->FC.SubType = SUBTYPE_NULL_FUNC;
	pHeader_802_11->FC.ToDs = 1;

	COPY_MAC_ADDR(pHeader_802_11->Addr1, pEntry->Addr);
	{
		COPY_MAC_ADDR(pHeader_802_11->Addr2, pAd->CurrentAddress);
		COPY_MAC_ADDR(pHeader_802_11->Addr3, pAd->CommonCfg.Bssid);
	}

	pHeader_802_11->FC.PwrMgmt = PwrMgmt;
	
	pHeader_802_11->Duration = pAd->CommonCfg.Dsifs + RTMPCalcDuration(pAd, TxRate, 14);

	/* sequence is increased in MlmeHardTx */
	pHeader_802_11->Sequence = pAd->Sequence;
	pAd->Sequence = (pAd->Sequence+1) & MAXSEQ; /* next sequence  */

	/* Prepare QosNull function frame */
	if (bQosNull)
	{
		pHeader_802_11->FC.SubType = SUBTYPE_QOS_NULL;
		
		/* copy QOS control bytes */
		NullFrame[Length]	=  0;
		NullFrame[Length+1] =  0;
		Length += 2;/* if pad with 2 bytes for alignment, APSD will fail */
	}
}




VOID MCC_ChangeAction(
    IN void * SystemSpecific1, 
    IN void * FunctionContext, 
    IN void * SystemSpecific2, 
    IN void * SystemSpecific3) 
{
	RTMP_ADAPTER *pAd = (PRTMP_ADAPTER)FunctionContext;
	RtmpOsTaskWakeUp(&(pAd->MultiChannelTask));
}

VOID ConcurrentP2PConnectTimeout(
    IN void * SystemSpecific1, 
    IN void * FunctionContext, 
    IN void * SystemSpecific2, 
    IN void * SystemSpecific3) 
{
	int i;
	RTMP_ADAPTER *pAd = (PRTMP_ADAPTER)FunctionContext;
	PRT_P2P_CONFIG	pP2PCtrl = &pAd->P2pCfg;
	
	pAd->Mlme.ConcurrentP2PConnectTimerRunning = FALSE;
	pAd->P2pCfg.bStartP2pConnect = FALSE;

	if (pAd->P2pCfg.bStartP2pConnect)
	{
		P2pStopConnectThis(pAd);
		pAd->StaCfg.bAutoReconnect = TRUE;
		pP2PCtrl->bSentProbeRSP = FALSE;
		P2pStopScan(pAd);

		P2pGroupTabInit(pAd);
		pP2PCtrl->GoFormCurrentState = P2P_GO_FORM_IDLE;

		/* Restore P2P WSC Mode / Config Method */
		pP2PCtrl->WscMode = WSC_PIN_MODE; /* PIN */
		pP2PCtrl->ConfigMethod = 0x188;
		pP2PCtrl->Dpid = DEV_PASS_ID_NOSPEC;

		if (P2P_CLI_ON(pAd))
			Set_P2pCli_Enable_Proc(pAd, "0");

		Set_P2p_OpMode_Proc(pAd, "0");
		pAd->ApCfg.ApCliTab[0].WscControl.WscConfStatus = WSC_SCSTATE_UNCONFIGURED;
		pAd->ApCfg.MBSSID[0].WscControl.WscConfStatus = WSC_SCSTATE_UNCONFIGURED;
		OS_WAIT(200);

		if (INFRA_ON(pAd))
			MultiChannelSwitchToRa(pAd);
			
	}
}



static VOID MACBuffer_Change(
    RTMP_ADAPTER *pAd,
    bool	hcca_to_edca,
    bool	edca_to_hcca)
{
	MAC_TABLE_ENTRY *pEntry = NULL;
	PAPCLI_STRUCT pApCliEntry = NULL;
	unsigned int i = 0;
	unsigned int MacValue, Data, Data2;
	INT ret;
	unsigned int MTxCycle;
	unsigned int TimeStamp;
	bool bBlockIn2Out=FALSE;
	pApCliEntry = &pAd->ApCfg.ApCliTab[BSS0];
	INT ext_ch;

//	RTMP_SEM_EVENT_WAIT(&pAd->reg_atomic2, ret);
	if (edca_to_hcca)
		RTMP_OS_NETDEV_STOP_QUEUE(pAd->net_dev);
	if (hcca_to_edca)
		RTMP_OS_NETDEV_STOP_QUEUE(pApCliEntry->dev);
//	RTMP_SEM_EVENT_UP(&pAd->reg_atomic2);	


	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DEQUEUEPACKET);

	/* Disable EDCA or HCCA dequeue */
	if (edca_to_hcca)
			pAd->MultiChannelFlowCtl |= EDCA_AC0_DEQUEUE_DISABLE;// 1
	if (hcca_to_edca)
			pAd->MultiChannelFlowCtl |= HCCA_DEQUEUE_DISABLE;//16

	RTMP_IO_READ32(pAd, WMM_CTRL, &Data);

	if (edca_to_hcca)
		Data |= 0x80000000; /* bit 31 set to 1 */	/*  WMM Channel switch to EDCA2 */
	if (hcca_to_edca)
		Data &= 0x7fffffff; /* bit 31 set to 0 */	/*  WMM Channel switch to EDCA1 */

	RTMP_IO_WRITE32(pAd, WMM_CTRL, Data);

	/* Polling EDCA  or EDCA2 Out-Q until empty  */

	for (MTxCycle = 0;; MTxCycle++)
	{
		if (!bBlockIn2Out)
		{
			RTMP_IO_READ32(pAd, 0x438, &Data);
			if ((edca_to_hcca) && (((Data >> 16) & 0xff) == 0))	
			{	
				/* Disable EDCA1 In-Q to Out-Q */
				RTMP_IO_READ32(pAd, PBF_CFG, &Data);
				Data &= 0xffffEfff;/* bit 12 set to 0 */ //zero modify 20120807
				RTMP_IO_WRITE32(pAd, PBF_CFG, Data);
				bBlockIn2Out=TRUE;
			}	
			else if ((hcca_to_edca) && (((Data >> 24) & 0xff) == 0))	
			{
				/* Disable HCCA/EDCA2 In-Q to Out-Q */
				RTMP_IO_READ32(pAd, PBF_CFG, &Data);
				Data &= 0xffffDfff;/* set bit 13 set to 0 */ //zero modify 20120807
				//Data |= ((1 << 10) | (1 << 11));
				RTMP_IO_WRITE32(pAd, PBF_CFG, Data);
				bBlockIn2Out=TRUE;					
			}	
			else
			RTMPusecDelay(50);
		}
		else
		{
			RTMP_IO_READ32(pAd, TXQ_STA, &Data);
			if ((edca_to_hcca) && (((Data >> 19) & 0x1f) == 0))	
				break;
			else if ((hcca_to_edca) && (((Data >> 27) & 0x1f) == 0))
				break;
			else
				RTMPusecDelay(50);
		}
					
	}
	
	if (MTxCycle >= 2000)
	{
		if(edca_to_hcca)
			DBGPRINT(RT_DEBUG_ERROR, ("Polling EDCA Out-Q max(%x)\n", Data));
		if(hcca_to_edca)
			DBGPRINT(RT_DEBUG_ERROR, ("Polling HCCA Out-Q max\n"));

	}

	if (edca_to_hcca)
		RTMPHwSendNullFrame(pAd,
							pAd->CommonCfg.TxRate, 
							(pAd->CommonCfg.bWmmCapable & pAd->CommonCfg.APEdcaParm.bValid),
							 PWR_SAVE, 0);

	if (hcca_to_edca)
			RTMPHwSendNullFrame(pAd,
							pAd->CommonCfg.TxRate, 
							(pAd->CommonCfg.bWmmCapable & pAd->CommonCfg.APEdcaParm.bValid),
							PWR_SAVE, 1);


	RtmpOsMsDelay(20);
	/* Disable all Tx Out-Q */
	RTMP_IO_READ32(pAd, PBF_CFG, &Data);
	Data &= 0xfffffff3;/* bit 3 and bit 2 set to 0 */ //zero modify 20120807
	RTMP_IO_WRITE32(pAd, PBF_CFG, Data);



	if (hcca_to_edca)
	{
		if (pAd->StaCfg.BW == BW_40)
		{
			if (pAd->CommonCfg.CentralChannel > pAd->CommonCfg.Channel)
				ext_ch = EXTCHA_ABOVE;

			else
				ext_ch = EXTCHA_BELOW;	
		}
		else
			ext_ch = EXTCHA_NONE;

		AsicSetChannel(pAd, pAd->CommonCfg.CentralChannel, pAd->StaCfg.BW, ext_ch, FALSE);

	}

	if (edca_to_hcca)
	{
		if (pAd->P2pCfg.BW == BW_40)
		{
			if (pAd->ApCliMlmeAux.CentralChannel > pAd->ApCliMlmeAux.Channel)
				ext_ch = EXTCHA_ABOVE;
			else
				ext_ch = EXTCHA_BELOW;
	
		}
		else
			ext_ch = EXTCHA_NONE;

		AsicSetChannel(pAd, pAd->ApCliMlmeAux.CentralChannel, pAd->P2pCfg.BW, ext_ch, FALSE);

	}



	if (edca_to_hcca)
		RTMPHwSendNullFrame(pAd, 
					pAd->CommonCfg.TxRate, 
					(pAd->CommonCfg.bWmmCapable & pAd->CommonCfg.APEdcaParm.bValid),
					 PWR_ACTIVE, 1);

	if (hcca_to_edca)
		RTMPHwSendNullFrame(pAd,
					pAd->CommonCfg.TxRate, 
					(pAd->CommonCfg.bWmmCapable & pAd->CommonCfg.APEdcaParm.bValid),
					PWR_ACTIVE, 0);

	RTMP_IO_READ32(pAd, TSF_TIMER_DW0, &SwitchTime7);

	/* Enable EDCA or EDCA2 Tx In-Q and Out-Q */
	RTMP_IO_READ32(pAd, PBF_CFG, &Data);
	if (edca_to_hcca)
		Data |= ((1 << 3) | (1 << 13));/* bit 3 and bit 13 set to 1 */

	if (hcca_to_edca)
		Data |= ((1 << 2) | (1 << 12));/* bit 2  and bit 12 set to 1 */

	RTMP_IO_WRITE32(pAd, PBF_CFG, Data);
	RTMP_IO_READ32(pAd, TSF_TIMER_DW0, &SwitchTime8);

		if ((i == 10) || (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)))
		{
			DBGPRINT(RT_DEBUG_ERROR, ("Multi Channel Switch Retry count exhausted\n"));
		}

		/* Enable EDCA or EDCA2 dequeue */
	if (hcca_to_edca)
		pAd->MultiChannelFlowCtl &= ~EDCA_AC0_DEQUEUE_DISABLE; // 0

	if (edca_to_hcca)
		pAd->MultiChannelFlowCtl &= ~HCCA_DEQUEUE_DISABLE; // 0

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DEQUEUEPACKET);

//	RTMP_SEM_EVENT_WAIT(&pAd->reg_atomic2, ret);

	if (hcca_to_edca)
		RTMP_OS_NETDEV_WAKE_QUEUE(pAd->net_dev);

	if (edca_to_hcca)
		RTMP_OS_NETDEV_WAKE_QUEUE(pApCliEntry->dev);

//	RTMP_SEM_EVENT_UP(&pAd->reg_atomic2);	


}


static VOID ProcessEDCAToHCCA(
    RTMP_ADAPTER *pAd) 
{
	PAPCLI_STRUCT pApCliEntry = NULL;

	pApCliEntry = &pAd->ApCfg.ApCliTab[BSS0];

	unsigned int Data;

	if ((pApCliEntry->Valid) && INFRA_ON(pAd))
	{
		MACBuffer_Change(pAd,FALSE, TRUE);
		RTMPSetTimer(&pAd->Mlme.MCCTimer, pAd->Mlme.HCCAToEDCATimerValue);
	}
}


static VOID ProcessHCCAToEDCA(
    PRTMP_ADAPTER pAd)
{
	unsigned int MacValue;
	unsigned int i = 0;
	INT ret;
	bool bBlockIn2Out=FALSE;
	unsigned int Data;
	
	if (!P2P_INF_ON(pAd) && !P2P_GO_ON(pAd))
	{
		MAC_TABLE_ENTRY *pEntry = NULL;
		PAPCLI_STRUCT pApCliEntry = NULL;
		pApCliEntry = &pAd->ApCfg.ApCliTab[BSS0]; 

		if ((pApCliEntry->Valid) && INFRA_ON(pAd))
		{
			MACBuffer_Change(pAd,TRUE, FALSE);
			RTMPSetTimer(&pAd->Mlme.MCCTimer, pAd->Mlme.EDCAToHCCATimerValue);
		}
	}
}

static INT MultiChannelTaskThread(
    IN  unsigned long Context)
{
	RTMP_OS_TASK *pTask;
	RTMP_ADAPTER *pAd;
	INT	Status = 0;

	pTask = (RTMP_OS_TASK *)Context;
	pAd = (PRTMP_ADAPTER)RTMP_OS_TASK_DATA_GET(pTask);

	if (pAd == NULL)
		return 0;

	RtmpOSTaskCustomize(pTask);


	while (pTask && !RTMP_OS_TASK_IS_KILLED(pTask))
	{
		if (RtmpOSTaskWait(pAd, pTask, &Status) == FALSE)
		{
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);
			break;
		}

		if (Status != 0)
			break;


		if (INFRA_ON(pAd) && P2P_CLI_ON(pAd))
		{
			if (pAd->LatchRfRegs.Channel == pAd->ApCliMlmeAux.CentralChannel)
				pAd->MultiChannelAction = HCCA_TO_EDCA;
			else if (pAd->LatchRfRegs.Channel == pAd->CommonCfg.CentralChannel)
				pAd->MultiChannelAction = EDCA_TO_HCCA;

		} 
		/*else if (INFRA_ON(pAd))
		{
			// reset to default switct to ra0
			//MultiChannelSwitchToRa(pAd);
		} 
		else if (P2P_CLI_ON(pAd))
		{
			// reset to default switch to p2p0
			//MultiChannelSwitchToP2P(pAd);

		}*/

	
#ifdef RTMP_MAC_USB		
		/* device had been closed */
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))
			break;
#endif /* RTMP_MAC_USB */
		if (pAd->MultiChannelAction == HCCA_TO_EDCA)
			ProcessHCCAToEDCA(pAd);
		else if (pAd->MultiChannelAction == EDCA_TO_HCCA)
			ProcessEDCAToHCCA(pAd);
		else
			DBGPRINT(RT_DEBUG_TRACE, ("%s: Unkown Action(=%d)\n", __FUNCTION__, pAd->MultiChannelAction));



	}

	if (pTask)
		RtmpOSTaskNotifyToExit(pTask);
	
	return 0;
}

NDIS_STATUS MultiChannelThreadInit(
	IN  PRTMP_ADAPTER pAd)
{
	NDIS_STATUS status = NDIS_STATUS_FAILURE;
	RTMP_OS_TASK *pTask;
	
	DBGPRINT(RT_DEBUG_TRACE, ("--> %s\n", __FUNCTION__));

	pTask = &pAd->MultiChannelTask;
	pAd->MultiChannelAction = 0xFF;
	pAd->Mlme.HCCAToEDCATimerValue = HCCA_TIMEOUT;
	pAd->Mlme.EDCAToHCCATimerValue = EDCA_TIMEOUT;
	pAd->P2pCfg.bStartP2pConnect = FALSE;
	pAd->MultiChannelFlowCtl = 0;

	RTMPInitTimer(pAd, &pAd->Mlme.MCCTimer, GET_TIMER_FUNCTION(MCC_ChangeAction), pAd, FALSE);
	RTMPInitTimer(pAd, &pAd->Mlme.ConcurrentP2PConnectTimer, GET_TIMER_FUNCTION(ConcurrentP2PConnectTimeout), pAd, FALSE);

	RTMP_OS_TASK_INIT(pTask, "MultiChannelTask", pAd);
	status = RtmpOSTaskAttach(pTask, MultiChannelTaskThread, (unsigned long)&pAd->MultiChannelTask);
	DBGPRINT(RT_DEBUG_TRACE, ("<-- %s, status=%d!\n", __FUNCTION__, status));

	return status;
}

bool MultiChannelThreadExit(
	IN  PRTMP_ADAPTER pAd)
{	
	INT ret;

	MultiChannelTimerStop(pAd);
	
	ret = RtmpOSTaskKill(&pAd->MultiChannelTask);
	if (ret == NDIS_STATUS_FAILURE)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s: kill multi-channel task failed!\n", __FUNCTION__));
	}
	return TRUE;
}

VOID MultiChannelTimerStop(
	IN  PRTMP_ADAPTER pAd)
{

	bool bCancelled = FALSE;
	PAPCLI_STRUCT pApCliEntry = NULL;


	pApCliEntry = &pAd->ApCfg.ApCliTab[BSS0];
	pAd->MultiChannelAction = 0xFF;	
	RTMPCancelTimer(&pAd->Mlme.MCCTimer, &bCancelled);

	RTMP_OS_NETDEV_WAKE_QUEUE(pAd->net_dev);
	RTMP_OS_NETDEV_WAKE_QUEUE(pApCliEntry->dev);

	OS_WAIT(200);
}

VOID MultiChannelTimerStart(
	IN  PRTMP_ADAPTER pAd,
	IN MAC_TABLE_ENTRY  *pEntry)
{

		bool				bCancelled = FALSE;
		
		if (pAd->P2pCfg.bStartP2pConnect)
		{
			pAd->P2pCfg.bStartP2pConnect = FALSE;

			pAd->StaCfg.ReConnectCountDown = 5;

			if (pAd->Mlme.ConcurrentP2PConnectTimerRunning)
			{
				printk("iversondebug MultiChannelTimer start3 \n");

				RTMPCancelTimer(&pAd->Mlme.ConcurrentP2PConnectTimer, &bCancelled);
				pAd->Mlme.ConcurrentP2PConnectTimerRunning = FALSE;				
			}
		}

		RTMPSetTimer(&pAd->Mlme.MCCTimer, 200);
		
		pAd->Mlme.P2pStayTick = 0;
		pAd->Mlme.StaStayTick = 0;
}

VOID MultiChannelSwitchToRa(
	IN  PRTMP_ADAPTER pAd)
{
	PAPCLI_STRUCT pApCliEntry = NULL;
	pApCliEntry = &pAd->ApCfg.ApCliTab[BSS0];
	INT ext_ch;


//	MACBuffer_Change(pAd,TRUE, FALSE);
	MAC_TABLE_ENTRY *pEntry = NULL;
	unsigned int MacValue, Data, Data2;
	INT ret,i;
	unsigned int MTxCycle;
	bool bBlockIn2Out=FALSE;
	pApCliEntry = &pAd->ApCfg.ApCliTab[BSS0];

	RTMP_OS_NETDEV_STOP_QUEUE(pApCliEntry->dev);

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DEQUEUEPACKET);
	pAd->MultiChannelFlowCtl |= HCCA_DEQUEUE_DISABLE;//16
	RTMP_IO_READ32(pAd, TSF_TIMER_DW0, &SwitchTime1);
	RTMP_IO_READ32(pAd, WMM_CTRL, &Data);

	Data &= 0x7fffffff; /* bit 31 set to 0 */	/*  WMM Channel switch to EDCA1 */

	RTMP_IO_WRITE32(pAd, WMM_CTRL, Data);
	RTMP_IO_READ32(pAd, TSF_TIMER_DW0, &SwitchTime2);

	/* Polling EDCA  or EDCA2 Out-Q until empty  */

	for (MTxCycle = 0;; MTxCycle++)
	{
		if (!bBlockIn2Out)
		{
			RTMP_IO_READ32(pAd, 0x438, &Data);
			if ((((Data >> 24) & 0xff) == 0))	
			{
				/* Disable HCCA/EDCA2 In-Q to Out-Q */
				RTMP_IO_READ32(pAd, PBF_CFG, &Data);
				Data &= 0xffffDfff;/* set bit 13 set to 0 */ //zero modify 20120807
				//Data |= ((1 << 10) | (1 << 11));
				RTMP_IO_WRITE32(pAd, PBF_CFG, Data);
				bBlockIn2Out=TRUE;					
			}	
			else
			RTMPusecDelay(50);
		}
		else
		{
			RTMP_IO_READ32(pAd, TXQ_STA, &Data);
			if ((((Data >> 27) & 0x1f) == 0))
				break;
			else
				RTMPusecDelay(50);
		}
					
	}
	
	if (MTxCycle >= 2000)
	{
			DBGPRINT(RT_DEBUG_ERROR, ("Polling HCCA Out-Q max\n"));

	}

	RtmpOsMsDelay(20);
	/* Disable all Tx Out-Q */
	RTMP_IO_READ32(pAd, PBF_CFG, &Data);
	Data &= 0xfffffff3;/* bit 3 and bit 2 set to 0 */ //zero modify 20120807
	RTMP_IO_WRITE32(pAd, PBF_CFG, Data);


	if (pAd->StaCfg.BW == BW_40)
	{
		if (pAd->CommonCfg.CentralChannel > pAd->CommonCfg.Channel)
			ext_ch = EXTCHA_ABOVE;
		else
			ext_ch = EXTCHA_BELOW;	
	}
	else
		ext_ch = EXTCHA_NONE;

	AsicSetChannel(pAd, pAd->CommonCfg.CentralChannel, pAd->StaCfg.BW, ext_ch, FALSE);




	RTMPHwSendNullFrame(pAd, 
				pAd->CommonCfg.TxRate, 
				(pAd->CommonCfg.bWmmCapable & pAd->CommonCfg.APEdcaParm.bValid),
				PWR_ACTIVE, 0);

	RTMP_IO_READ32(pAd, TSF_TIMER_DW0, &SwitchTime7);

	/* Enable EDCA or EDCA2 Tx In-Q and Out-Q */
	RTMP_IO_READ32(pAd, PBF_CFG, &Data);
	Data |= ((1 << 2) | (1 << 12));/* bit 2  and bit 12 set to 1 */

	RTMP_IO_WRITE32(pAd, PBF_CFG, Data);

		if ((i == 10) || (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)))
		{
			DBGPRINT(RT_DEBUG_ERROR, ("Multi Channel Switch Retry count exhausted\n"));
		}

		/* Enable EDCA or EDCA2 dequeue */


	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DEQUEUEPACKET);

	pAd->MultiChannelFlowCtl = 0;

	RTMP_OS_NETDEV_WAKE_QUEUE(pAd->net_dev);

}

VOID MultiChannelSwitchToP2P(
	IN  PRTMP_ADAPTER pAd)
{
	PAPCLI_STRUCT pApCliEntry = NULL;
	pApCliEntry = &pAd->ApCfg.ApCliTab[BSS0];
	INT ext_ch;

	MAC_TABLE_ENTRY *pEntry = NULL;
	unsigned int i = 0;
	unsigned int MacValue, Data, Data2;
	INT ret;
	unsigned int MTxCycle;
	bool bBlockIn2Out=FALSE;


	RTMP_OS_NETDEV_STOP_QUEUE(pAd->net_dev);

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DEQUEUEPACKET);

	/* Disable EDCA or HCCA dequeue */
	pAd->MultiChannelFlowCtl |= EDCA_AC0_DEQUEUE_DISABLE;// 1

	RTMP_IO_READ32(pAd, TSF_TIMER_DW0, &SwitchTime1);

	RTMP_IO_READ32(pAd, WMM_CTRL, &Data);

	Data |= 0x80000000;/* bit 31 set to 1 */   /*  WMM Channel switch to EDCA2 */

	RTMP_IO_WRITE32(pAd, WMM_CTRL, Data);

	/* Polling EDCA  or EDCA2 Out-Q until empty  */

	for (MTxCycle = 0;; MTxCycle++)
	{
		if (!bBlockIn2Out)
		{
			RTMP_IO_READ32(pAd, 0x438, &Data);
			if ((((Data >> 16) & 0xff) == 0))	
			{	
				/* Disable EDCA1 In-Q to Out-Q */
				RTMP_IO_READ32(pAd, PBF_CFG, &Data);
				Data &= 0xffffEfff;/* bit 12 set to 0 */ //zero modify 20120807
				RTMP_IO_WRITE32(pAd, PBF_CFG, Data);
				bBlockIn2Out=TRUE;
			}	
			else
			RTMPusecDelay(50);
		}
		else
		{
			RTMP_IO_READ32(pAd, TXQ_STA, &Data);
			if ((((Data >> 19) & 0x1f) == 0))	
				break;
			else
				RTMPusecDelay(50);
		}
					
	}
	
	if (MTxCycle >= 2000)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Polling EDCA Out-Q max(%x)\n", Data));
	}


	RtmpOsMsDelay(20);
	/* Disable all Tx Out-Q */
	RTMP_IO_READ32(pAd, PBF_CFG, &Data);
	Data &= 0xfffffff3;/* bit 3 and bit 2 set to 0 */ //zero modify 20120807
	RTMP_IO_WRITE32(pAd, PBF_CFG, Data);


	if (pAd->P2pCfg.BW == BW_40)
	{
		if (pAd->ApCliMlmeAux.CentralChannel > pAd->ApCliMlmeAux.Channel)
			ext_ch = EXTCHA_ABOVE;
		else
			ext_ch = EXTCHA_BELOW;
	
	}
	else
		ext_ch = EXTCHA_NONE;

	AsicSetChannel(pAd, pAd->ApCliMlmeAux.CentralChannel, pAd->P2pCfg.BW, ext_ch, FALSE);




	RTMPHwSendNullFrame(pAd, 
				pAd->CommonCfg.TxRate, 
				(pAd->CommonCfg.bWmmCapable & pAd->CommonCfg.APEdcaParm.bValid),
				PWR_ACTIVE, 1);

	/* Enable EDCA or EDCA2 Tx In-Q and Out-Q */
	RTMP_IO_READ32(pAd, PBF_CFG, &Data);
	Data |= ((1 << 3) | (1 << 13));/* bit 3 and bit 13 set to 1 */


	RTMP_IO_WRITE32(pAd, PBF_CFG, Data);

	if ((i == 10) || (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)))
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Multi Channel Switch Retry count exhausted\n"));
	}

		/* Enable EDCA or EDCA2 dequeue */
	pAd->MultiChannelFlowCtl = 0; // 0


	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DEQUEUEPACKET);

	RTMP_OS_NETDEV_WAKE_QUEUE(pApCliEntry->dev);


}
#endif /* CONFIG_MULTI_CHANNEL */
