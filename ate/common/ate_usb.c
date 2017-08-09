/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2006, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

 	Module Name:
	ate_usb.c

	Abstract:

	Revision History:
	Who			When	    What
	--------	----------  ----------------------------------------------
	Name		Date	    Modification logs
*/

#ifdef RTMP_MAC_USB

#include "rt_config.h"

extern UCHAR EpToQueue[];
/* 802.11 MAC Header, Type:Data, Length:24bytes + 6 bytes QOS/HTC + 2 bytes padding */
extern UCHAR TemplateFrame[32];

INT TxDmaBusy(
	IN PRTMP_ADAPTER pAd)
{
	INT result;
	USB_DMA_CFG_STRUC UsbCfg;

	RTMP_IO_READ32(pAd, USB_DMA_CFG, &UsbCfg.word);	/* disable DMA */
	result = (UsbCfg.field.TxBusy) ? TRUE : FALSE;

	return result;
}


INT RxDmaBusy(
	IN PRTMP_ADAPTER pAd)
{
	INT result;
	USB_DMA_CFG_STRUC UsbCfg;

	RTMP_IO_READ32(pAd, USB_DMA_CFG, &UsbCfg.word);	/* disable DMA */
	result = (UsbCfg.field.RxBusy) ? TRUE : FALSE;

	return result;
}


VOID RtmpDmaEnable(
	IN PRTMP_ADAPTER pAd,
	IN INT Enable)
{
	BOOLEAN value;
	ULONG WaitCnt;
	USB_DMA_CFG_STRUC UsbCfg;
	
	value = Enable > 0 ? 1 : 0;

	/* check DMA is in busy mode. */
	WaitCnt = 0;

	while (TxDmaBusy(pAd) || RxDmaBusy(pAd))
	{
		RTMPusecDelay(10);
		if (WaitCnt++ > 100)
			break;
	}
	RTMP_IO_READ32(pAd, USB_DMA_CFG, &UsbCfg.word);	/* disable DMA */
	UsbCfg.field.TxBulkEn = value;
	UsbCfg.field.RxBulkEn = value;
	RTMP_IO_WRITE32(pAd, USB_DMA_CFG, UsbCfg.word);	/* abort all TX rings */
	RtmpOsMsDelay(5);

	return;
}


static VOID ATEWriteTxWI(
	IN	PRTMP_ADAPTER	pAd,
	IN	TXWI_STRUC *pTxWI,
	IN	BOOLEAN			FRAG,	
	IN	BOOLEAN			InsTimestamp,
	IN	BOOLEAN 		AMPDU,
	IN	BOOLEAN 		Ack,
	IN	BOOLEAN 		NSeq,		/* HW new a sequence. */
	IN	UCHAR			BASize,
	IN	UCHAR			WCID,
	IN	ULONG			Length,
	IN	UCHAR 			PID,
	IN	UCHAR			MIMOps,
	IN	UCHAR			Txopmode,	
	IN	BOOLEAN			CfAck,	
	IN	HTTRANSMIT_SETTING	Transmit)
{
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_SHORT_PREAMBLE_INUSED);
	pTxWI->TxWIFRAG= FRAG;
	pTxWI->TxWITS= InsTimestamp;
	pTxWI->TxWIAMPDU = AMPDU;

	pTxWI->TxWIMIMOps = PWR_ACTIVE;
	pTxWI->TxWIMpduDensity = 4;
	pTxWI->TxWIACK = Ack;
	pTxWI->TxWITXOP = Txopmode;
	pTxWI->TxWINSEQ = NSeq;
	//pTxWI->TxWIBAWinSize = BASize;	
	pTxWI->TxWIBAWinSize = 21;	

	pTxWI->TxWIWirelessCliID = WCID;
	pTxWI->TxWIMPDUByteCnt = Length; 
	//pTxWI->TxWIPacketId = PID; 
	pTxWI->TxWIPacketId = 7; 
	
	pTxWI->TxWIBW = Transmit.field.BW;
	pTxWI->TxWIShortGI = Transmit.field.ShortGI;
	pTxWI->TxWISTBC= Transmit.field.STBC;
	
	pTxWI->TxWIMCS = Transmit.field.MCS;
	pTxWI->TxWIPHYMODE= Transmit.field.MODE;
	pTxWI->TxWICFACK = CfAck;

	return;
}


/*
========================================================================
	Routine	Description:
		Write TxInfo for ATE mode.
		
	Return Value:
		None
========================================================================
*/
static VOID ATEWriteTxInfo(
	IN	PRTMP_ADAPTER	pAd,
	IN	TXINFO_STRUC *pTxInfo,
	IN	USHORT		USBDMApktLen,
	IN	BOOLEAN		bWiv,
	IN	UCHAR			QueueSel,
	IN	UCHAR			NextValid,
	IN	UCHAR			TxBurst)
{
#ifdef RLT_MAC
	struct _TXINFO_NMAC_PKT *nmac_info;

	nmac_info = (struct _TXINFO_NMAC_PKT *)pTxInfo;
	nmac_info->pkt_80211 = 1;
	nmac_info->info_type = 0;
	nmac_info->d_port = 0;
	nmac_info->cso = 0;
	nmac_info->tso = 0;
#endif /* RLT_MAC */

#ifdef RTMP_MAC
	/* ATE doesn't support checksum offload. */
	pTxInfo->TxInfoCSO = 1;
	pTxInfo->TxInfoUSO = 0;
	pTxInfo->TxInfoTCPOffset = 0;
	pTxInfo->TxInfoIPOffset = 0;
#endif /* RTMP_MAC */

	pTxInfo->TxInfoPktLen = USBDMApktLen;
	pTxInfo->TxInfoQSEL = QueueSel;

	if (QueueSel != FIFO_EDCA)
		DBGPRINT(RT_DEBUG_TRACE, ("======= QueueSel != FIFO_EDCA =======\n"));

	pTxInfo->TxInfoUDMANextVld = NextValid;
	pTxInfo->TxInfoUDMATxburst = TxBurst;
	pTxInfo->TxInfoWIV = bWiv;
#ifndef USB_BULK_BUF_ALIGMENT
	pTxInfo->TxInfoSwLstRnd = 0;
#else
	pTxInfo->bFragLasAlignmentsectiontRound = 0;
#endif /* USB_BULK_BUF_ALIGMENT */

	return;
}


INT ATESetUpFrame(
	IN PRTMP_ADAPTER pAd,
	IN UINT32 TxIdx)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	UINT pos = 0;
	PTX_CONTEXT	pNullContext;
	PUCHAR			pDest;
	HTTRANSMIT_SETTING	TxHTPhyMode;
	TXWI_STRUC *pTxWI;
	TXINFO_STRUC *pTxInfo;	
	UINT32			TransferBufferLength, OrgBufferLength = 0;
	UCHAR			padLen = 0;
	UINT8 TXWISize = pAd->chipCap.TXWISize;
#ifdef RALINK_QA
	PHEADER_802_11	pHeader80211 = NULL;
#endif /* RALINK_QA */
	
	if ((RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS)) ||
		(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET)) ||
		(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS)) ||
		(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)))
	{
		return -1;
	}

	/* We always use QID_AC_BE and FIFO_EDCA in ATE mode. */

	pNullContext = &(pAd->NullContext[0]);
	ASSERT(pNullContext != NULL);
	
	if (pNullContext->InUse == FALSE)
	{
		/* set the in use bit */
		pNullContext->InUse = TRUE;
		NdisZeroMemory(&(pAd->NullFrame), sizeof(HEADER_802_11));
		
		/* fill 802.11 header */
#ifdef RALINK_QA
		if (pATEInfo->bQATxStart == TRUE) 
		{
			pHeader80211 = NdisMoveMemory(&(pAd->NullFrame),
				pATEInfo->Header, pATEInfo->HLen);
		}
		else
#endif /* RALINK_QA */
		{
			NdisMoveMemory(&(pAd->NullFrame), TemplateFrame,
			sizeof(HEADER_802_11));
		}

#ifdef RT_BIG_ENDIAN
		RTMPFrameEndianChange(pAd, (PUCHAR)&(pAd->NullFrame), DIR_READ, FALSE);
#endif /* RT_BIG_ENDIAN */

#ifdef RALINK_QA
		if (pATEInfo->bQATxStart == TRUE) 
		{
			/* modify sequence number... */
			if (pATEInfo->TxDoneCount == 0)
			{
				pATEInfo->seq = pHeader80211->Sequence;
			}
			else
			{
				pHeader80211->Sequence = ++pATEInfo->seq;
			}
			/* We already got all the address fields from QA GUI. */
		}
		else
#endif /* RALINK_QA */
		{
			COPY_MAC_ADDR(pAd->NullFrame.Addr1, pATEInfo->Addr1);
			COPY_MAC_ADDR(pAd->NullFrame.Addr2, pATEInfo->Addr2);
			COPY_MAC_ADDR(pAd->NullFrame.Addr3, pATEInfo->Addr3);
		}

		RTMPZeroMemory(&pAd->NullContext[0].TransferBuffer->field.WirelessPacket[0], TX_BUFFER_NORMSIZE);
		pTxInfo = (TXINFO_STRUC *)&pAd->NullContext[0].TransferBuffer->field.WirelessPacket[0];

#ifdef RALINK_QA
		if (pATEInfo->bQATxStart == TRUE) 
		{
			/* Avoid to exceed the range of WirelessPacket[]. */
			ASSERT(pATEInfo->TxInfo.TxInfoPktLen <= (MAX_FRAME_SIZE - 34/* == 2312 */));
			NdisMoveMemory(pTxInfo, &(pATEInfo->TxInfo), sizeof(pATEInfo->TxInfo));
		}
		else
#endif /* RALINK_QA */
		{
			/* Avoid to exceed the range of WirelessPacket[]. */
			ASSERT(pATEInfo->TxLength <= (MAX_FRAME_SIZE - 34/* == 2312 */));

			/* pTxInfo->TxInfoPktLen will be updated to include padding later */
			ATEWriteTxInfo(pAd, pTxInfo, (USHORT)(TXWISize + pATEInfo->TxLength)
			, TRUE, EpToQueue[MGMTPIPEIDX], FALSE,  FALSE);
			pTxInfo->TxInfoQSEL = FIFO_EDCA;
		}

		pTxWI = (TXWI_STRUC *)&pAd->NullContext[0].TransferBuffer->field.WirelessPacket[TXINFO_SIZE];

		/* fill TxWI */
		if (pATEInfo->bQATxStart == TRUE) 
		{
			TxHTPhyMode.field.BW = pATEInfo->TxWI.TxWIBW;
			TxHTPhyMode.field.ShortGI = pATEInfo->TxWI.TxWIShortGI;
			TxHTPhyMode.field.STBC = pATEInfo->TxWI.TxWISTBC;
			TxHTPhyMode.field.MCS = pATEInfo->TxWI.TxWIMCS;
			TxHTPhyMode.field.MODE = pATEInfo->TxWI.TxWIPHYMODE;
			ATEWriteTxWI(pAd, pTxWI, pATEInfo->TxWI.TxWIFRAG, pATEInfo->TxWI.TxWITS,
				pATEInfo->TxWI.TxWIAMPDU, pATEInfo->TxWI.TxWIACK, pATEInfo->TxWI.TxWINSEQ, 
				pATEInfo->TxWI.TxWIBAWinSize, BSSID_WCID,
				pATEInfo->TxWI.TxWIMPDUByteCnt/* include 802.11 header */,
				pATEInfo->TxWI.TxWIPacketId,
				0, pATEInfo->TxWI.TxWITXOP/*IFS_HTTXOP*/, pATEInfo->TxWI.TxWICFACK
				/*FALSE*/, TxHTPhyMode);
		}
		else
		{
			TxHTPhyMode.field.BW = pATEInfo->TxWI.TxWIBW;
			TxHTPhyMode.field.ShortGI = pATEInfo->TxWI.TxWIShortGI;
			TxHTPhyMode.field.STBC = 0;
			TxHTPhyMode.field.MCS = pATEInfo->TxWI.TxWIMCS;
			TxHTPhyMode.field.MODE = pATEInfo->TxWI.TxWIPHYMODE;

			ATEWriteTxWI(pAd, pTxWI,  FALSE, FALSE, FALSE, FALSE
				/* No ack required. */, FALSE, 0, BSSID_WCID, pATEInfo->TxLength,
				0, 0, IFS_HTTXOP, FALSE, TxHTPhyMode);
		}

		hex_dump("ATE", pAd->NullContext[0].TransferBuffer->field.WirelessPacket, 24);
		dump_txinfo(pAd, pTxInfo);
		dumpTxWI(pAd, pTxWI);
		
		
		RTMPMoveMemory(&pAd->NullContext[0].TransferBuffer->field.WirelessPacket[TXINFO_SIZE + TXWISize],
			&pAd->NullFrame, sizeof(HEADER_802_11));

		pDest = &(pAd->NullContext[0].TransferBuffer->field.WirelessPacket[TXINFO_SIZE + TXWISize + sizeof(HEADER_802_11)]);

		/* prepare frame payload */
#ifdef RALINK_QA
		if (pATEInfo->bQATxStart == TRUE) 
		{
			/* copy the pattern one by one to the frame payload */
			if ((pATEInfo->PLen != 0) && (pATEInfo->DLen != 0))
			{
				for (pos = 0; pos < pATEInfo->DLen; pos += pATEInfo->PLen)
				{
					RTMPMoveMemory(pDest, pATEInfo->Pattern, pATEInfo->PLen);
					pDest += pATEInfo->PLen;
				}
			}
			TransferBufferLength = TXINFO_SIZE + TXWISize + pATEInfo->TxWI.TxWIMPDUByteCnt;
		}
		else
#endif /* RALINK_QA */
		{
		    for (pos = 0; pos < (pATEInfo->TxLength - sizeof(HEADER_802_11)); pos++)
		    {
				/* default payload is 0xA5 */
				*pDest = pATEInfo->Payload;
				pDest += 1;
		    }
			TransferBufferLength = TXINFO_SIZE + TXWISize + pATEInfo->TxLength;
		}

		OrgBufferLength = TransferBufferLength;
		TransferBufferLength = (TransferBufferLength + 3) & (~3);

		/* Always add 4 extra bytes at every packet. */
		padLen = TransferBufferLength - OrgBufferLength + 4;/* 4 == last packet padding */

		/* 
			RTMP_PKT_TAIL_PADDING == 11.
			[11 == 3(max 4 byte padding) + 4(last packet padding) + 4(MaxBulkOutsize align padding)]		
		*/
		ASSERT((padLen <= (RTMP_PKT_TAIL_PADDING - 4/* 4 == MaxBulkOutsize alignment padding */)));

		/* Now memzero all extra padding bytes. */
		NdisZeroMemory(pDest, padLen);
		pDest += padLen;

		/* Update pTxInfo->TxInfoPktLen to include padding. */
		pTxInfo->TxInfoPktLen = TransferBufferLength - TXINFO_SIZE;

		TransferBufferLength += 4;

		/* If TransferBufferLength is multiple of 64, add extra 4 bytes again. */
		if ((TransferBufferLength % pAd->BulkOutMaxPacketSize) == 0)
		{
			NdisZeroMemory(pDest, 4);
			TransferBufferLength += 4;
		}

		/* Fill out frame length information for global Bulk out arbitor. */
		pAd->NullContext[0].BulkOutSize = TransferBufferLength;
	}

#ifdef RT_BIG_ENDIAN
	RTMPWIEndianChange(pAd, (PUCHAR)pTxWI, TYPE_TXWI);
	RTMPFrameEndianChange(pAd, (((PUCHAR)pTxInfo) + TXWISize + TXINFO_SIZE), DIR_WRITE, FALSE);
	RTMPDescriptorEndianChange((PUCHAR)pTxInfo, TYPE_TXINFO);
#endif /* RT_BIG_ENDIAN */

	hex_dump("ATE TX", &pAd->NullContext[0].TransferBuffer->field.WirelessPacket[0], TXWISize + TXINFO_SIZE);

	return 0;
}


/*
========================================================================
	
	Routine Description:

	Arguments:

	Return Value:
		None

	Note:
	
========================================================================
*/
VOID ATE_RTUSBBulkOutDataPacket(
	IN	PRTMP_ADAPTER	pAd,
	IN	UCHAR			BulkOutPipeId)
{
	PTX_CONTEXT		pNullContext = &(pAd->NullContext[0]);
	PURB			pUrb;
	INT			ret = 0;
	ULONG			IrqFlags;


	ASSERT(BulkOutPipeId == 0);

	/* Build up the frame first. */
	BULK_OUT_LOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);

	if (pAd->BulkOutPending[BulkOutPipeId] == TRUE)
	{
		BULK_OUT_UNLOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);
		return;
	}

	pAd->BulkOutPending[BulkOutPipeId] = TRUE;
	BULK_OUT_UNLOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);

	/* Increase total transmit byte counter. */
	pAd->RalinkCounters.OneSecTransmittedByteCount +=  pNullContext->BulkOutSize; 
	pAd->RalinkCounters.TransmittedByteCount +=  pNullContext->BulkOutSize;

	/* Clear ATE frame bulk out flag. */
	RTUSB_CLEAR_BULK_FLAG(pAd, fRTUSB_BULK_OUT_DATA_ATE);

	/* Init Tx context descriptor. */
	pNullContext->IRPPending = TRUE;
	RTUSBInitTxDesc(pAd, pNullContext, BulkOutPipeId,
		(usb_complete_t)RTUSBBulkOutDataPacketComplete);
	pUrb = pNullContext->pUrb;
	
	if ((ret = RTUSB_SUBMIT_URB(pUrb))!=0)
	{
		DBGPRINT_ERR(("ATE_RTUSBBulkOutDataPacket: Submit Tx URB failed %d\n", ret));
		return;
	}

	pAd->BulkOutReq++;

	return;
}


/*
========================================================================
	
	Routine Description:

	Arguments:

	Return Value:
		None

	Note:
	
========================================================================
*/
VOID ATE_RTUSBCancelPendingBulkInIRP(
	IN	PRTMP_ADAPTER	pAd)
{
	PRX_CONTEXT		pRxContext = NULL;
	UINT			rx_ring_index;

	DBGPRINT(RT_DEBUG_TRACE, ("--->ATE_RTUSBCancelPendingBulkInIRP\n"));

	for (rx_ring_index = 0; rx_ring_index < (RX_RING_SIZE); rx_ring_index++)
	{
		pRxContext = &(pAd->RxContext[rx_ring_index]);

		if (pRxContext->IRPPending == TRUE)
		{
			RTUSB_UNLINK_URB(pRxContext->pUrb);
			pRxContext->IRPPending = FALSE;
			pRxContext->InUse = FALSE;
		}
	}

	DBGPRINT(RT_DEBUG_TRACE, ("<---ATE_RTUSBCancelPendingBulkInIRP\n"));

	return;
}


/*
========================================================================
	
	Routine Description:

	Arguments:

	Return Value:
		None

	Note:
	
========================================================================
*/
VOID ATEResetBulkIn(
	IN PRTMP_ADAPTER	pAd)
{
	if ((pAd->PendingRx > 0) && (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)))
	{
		DBGPRINT_ERR(("ATE : BulkIn IRP Pending!!!\n"));
		ATE_RTUSBCancelPendingBulkInIRP(pAd);
		RtmpOsMsDelay(100);
		pAd->PendingRx = 0;
	}

	return;
}


/*
========================================================================
	
	Routine Description:

	Arguments:

	Return Value:

	Note:
	
========================================================================
*/
INT ATEResetBulkOut(
	IN PRTMP_ADAPTER	pAd)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	PTX_CONTEXT	pNullContext = &(pAd->NullContext);
	INT ret=0;

	pNullContext->IRPPending = TRUE;

	/*
		If driver is still in ATE TXFRAME mode, 
		keep on transmitting ATE frames.
	*/
	DBGPRINT(RT_DEBUG_TRACE, ("pATEInfo->Mode == %d\npAd->ContinBulkOut == %d\npAd->BulkOutRemained == %d\n",
		pATEInfo->Mode, pAd->ContinBulkOut, atomic_read(&pAd->BulkOutRemained)));

	if ((pATEInfo->Mode == ATE_TXFRAME) && ((pAd->ContinBulkOut == TRUE) || (atomic_read(&pAd->BulkOutRemained) > 0)))
    {
		DBGPRINT(RT_DEBUG_TRACE, ("After CMDTHREAD_RESET_BULK_OUT, continue to bulk out frames !\n"));

		/* Init Tx context descriptor. */
		RTUSBInitTxDesc(pAd, pNullContext, 0/* pAd->bulkResetPipeid */, (usb_complete_t)RTUSBBulkOutDataPacketComplete);
		
		if ((ret = RTUSB_SUBMIT_URB(pNullContext->pUrb))!=0)
		{
			DBGPRINT_ERR(("ATE_RTUSBBulkOutDataPacket: Submit Tx URB failed %d\n", ret));
		}

		pAd->BulkOutReq++;
	}

	return ret;
}


/*
========================================================================
	
	Routine Description:

	Arguments:

	Return Value:

	IRQL = 
	
	Note:
	
========================================================================
*/
VOID RTUSBRejectPendingPackets(
	IN	PRTMP_ADAPTER	pAd)
{
	UCHAR			Index;
	PQUEUE_ENTRY	pEntry;
	PNDIS_PACKET	pPacket;
	PQUEUE_HEADER	pQueue;
	

	for (Index = 0; Index < 4; Index++)
	{
		NdisAcquireSpinLock(&pAd->TxSwQueueLock[Index]);
		while (pAd->TxSwQueue[Index].Head != NULL)
		{
			pQueue = (PQUEUE_HEADER) &(pAd->TxSwQueue[Index]);
			pEntry = RemoveHeadQueue(pQueue);
			pPacket = QUEUE_ENTRY_TO_PACKET(pEntry);
			RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_FAILURE);
		}
		NdisReleaseSpinLock(&pAd->TxSwQueueLock[Index]);

	}

}

#endif /* RTMP_MAC_USB */

