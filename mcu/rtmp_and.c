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
	rtmp_and.c

	Abstract:
	on-chip CPU related codes

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/

#include	"rt_config.h"


#ifdef RT_BIG_ENDIAN
typedef struct GNU_PACKED _TXINFO_NMAC_CMD_PKT{
	UINT32 info_type:2;
	UINT32 d_port:3;
	UINT32 cmd_type:7;
	UINT32 cmd_seq:4;
	UINT32 pkt_len:16;
}TXINFO_NMAC_CMD_PKT;
#else
typedef struct GNU_PACKED _TXINFO_NMAC_CMD_PKT {
	UINT32 pkt_len:16;
	UINT32 cmd_seq:4;
	UINT32 cmd_type:7;
	UINT32 d_port:3;
	UINT32 info_type:2;
}TXINFO_NMAC_CMD_PKT;
#endif /* RT_BIG_ENDIAN */




#ifdef RTMP_USB_SUPPORT

USBHST_STATUS USBUploadFWComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs)
{
	VOID	*SentToMCUDone = RTMP_OS_USB_CONTEXT_GET(pURB);

	RtmpComplete(SentToMCUDone);
}


static NDIS_STATUS USBLoadIVB(RTMP_ADAPTER *pAd)
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	UINT32 i;
	USHORT Value;
	USHORT Index;
	USHORT Temp;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;

	Status = RTUSB_VendorRequest(pAd,
								 USBD_TRANSFER_DIRECTION_OUT,
								 DEVICE_VENDOR_REQUEST_OUT,
								 0x01,
								 0x12,
								 0x00,
								 pChipCap->FWImageName + 32,
								 64);

	if (Status)
	{
			DBGPRINT(RT_DEBUG_ERROR, ("Upload IVB Fail\n"));
			return Status;
	}

	return Status;
}


NDIS_STATUS USBLoadFirmwareToAndes(RTMP_ADAPTER *pAd)
{
	PURB pURB;
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	ra_dma_addr_t DataDMA;
	PUCHAR DataBuffer;
	TXINFO_NMAC_CMD *TxInfoCmd;	
	INT32 SentLen;
	UINT32 CurLen;
	UINT32 MACValue, Loop = 0;
	USHORT Value;
	INT Ret;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	USB_DMA_CFG_STRUC UsbCfg;
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;
	//struct completion SentToMCUDone;
	VOID *SentToMCUDone;
	UINT32 ILMLen, DLMLen;
	USHORT FWVersion, BuildVersion;

	DBGPRINT(RT_DEBUG_TRACE, ("%s\n", __FUNCTION__));

	if (pChipCap->IsComboChip)
	{
loadfw_protect:
		RTUSBReadMACRegister(pAd, SEMAPHORE_00, &MACValue);
		Loop++;

		if (((MACValue & 0x01) == 0) && (Loop < 10000))
			goto loadfw_protect;
	}

	/* Enable USB_DMA_CFG */
	RTUSBWriteMACRegister(pAd, USB_DMA_CFG, 0xC00000, FALSE);

	/* Check MCU if ready */
	RTUSBReadMACRegister(pAd, COM_REG0, &MACValue);

	if (MACValue == 0x01)
		goto error0;

	RTUSBVenderReset(pAd);
	//mdelay(5);
	RtmpOsMsDelay(5);
	
	ILMLen = (*(pChipCap->FWImageName + 3) << 24) | (*(pChipCap->FWImageName + 2) << 16) |
			 (*(pChipCap->FWImageName + 1) << 8) | (*pChipCap->FWImageName);

	DLMLen = (*(pChipCap->FWImageName + 7) << 24) | (*(pChipCap->FWImageName + 6) << 16) |
			 (*(pChipCap->FWImageName + 5) << 8) | (*(pChipCap->FWImageName + 4));

	FWVersion = (*(pChipCap->FWImageName + 11) << 8) | (*(pChipCap->FWImageName + 10));

	BuildVersion = (*(pChipCap->FWImageName + 9) << 8) | (*(pChipCap->FWImageName + 8));
	
	DBGPRINT(RT_DEBUG_OFF, ("FW Version:%d.%d.%02d ", (FWVersion & 0xf000) >> 8,
						(FWVersion & 0x0f00) >> 8, FWVersion & 0x00ff));
	DBGPRINT(RT_DEBUG_OFF, ("Build:%x\n", BuildVersion));
	DBGPRINT(RT_DEBUG_OFF, ("Build Time:"));

	for (Loop = 0; Loop < 16; Loop++)
		DBGPRINT(RT_DEBUG_OFF, ("%c", *(pChipCap->FWImageName + 16 + Loop)));

	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("ILM Length = %d(bytes)\n", ILMLen));
	DBGPRINT(RT_DEBUG_OFF, ("DLM Length = %d(bytes)\n", DLMLen));

	RTMP_IO_WRITE32(pAd, 0xa44, 0x0);

	RTMP_IO_WRITE32(pAd, 0x230, 0x84210);
	//RTMP_IO_WRITE32(pAd, 0x230, 0x41210);

	RTMP_IO_WRITE32(pAd, 0x400, 0x80c00);

	RTMP_IO_WRITE32(pAd, 0x800, 0x01);

	RTMP_IO_READ32(pAd, 0x0404, &MACValue);
	MACValue |= 0xF;
	RTMP_IO_WRITE32(pAd, 0x0404, MACValue);

	/* Enable FCE */
	RTMP_IO_WRITE32(pAd, FCE_PSE_CTRL, 0x01);

	/* Enable USB_DMA_CFG */
	RTMP_IO_WRITE32(pAd, USB_DMA_CFG, 0xC00000);

#ifdef MT7601
	if ( IS_MT7601(pAd) )
	{
		USB_DMA_CFG_STRUC UsbCfg;

		RTMP_IO_READ32(pAd, USB_DMA_CFG, &UsbCfg.word);
		UsbCfg.field.TxClear = 1;
		RTMP_IO_WRITE32(pAd, USB_DMA_CFG, UsbCfg.word);
		UsbCfg.field.TxClear = 0;
		RTMP_IO_WRITE32(pAd, USB_DMA_CFG, UsbCfg.word);
	}
#endif /* MT7601 */

	/* FCE tx_fs_base_ptr */
	RTMP_IO_WRITE32(pAd, TX_CPU_PORT_FROM_FCE_BASE_PTR, 0x400230);

	/* FCE tx_fs_max_cnt */
	RTMP_IO_WRITE32(pAd, TX_CPU_PORT_FROM_FCE_MAX_COUNT, 0x01); 

	/* FCE pdma enable */
	RTMP_IO_WRITE32(pAd, FCE_PDMA_GLOBAL_CONF, 0x44);  

	/* FCE skip_fs_en */
	RTMP_IO_WRITE32(pAd, FCE_SKIP_FS, 0x03);

	/* Allocate URB */
	pURB = RTUSB_ALLOC_URB(0);

	if (!pURB)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Can not allocate URB\n"));
		Status = NDIS_STATUS_RESOURCES; 
		goto error0;
	}

	/* Allocate TransferBuffer */
	DataBuffer = RTUSB_URB_ALLOC_BUFFER(pObj->pUsb_Dev, 14592, &DataDMA);
		
	if (!DataBuffer)
	{
		Status = NDIS_STATUS_RESOURCES;
		goto error1;
	}


	DBGPRINT(RT_DEBUG_OFF, ("Loading FW"));
	
	//init_completion(&SentToMCUDone);
	SentToMCUDone = RtmpInitCompletion();

	CurLen = 0x40; 

	/* Loading ILM */
	while (1)
	{
/* ++ dump firmware ++ */
/* ++ dump firmware ++ */

		SentLen = (ILMLen - CurLen) >= 14336 ? 14336 : (ILMLen - CurLen);

		if (SentLen > 0)
		{
			TxInfoCmd = (TXINFO_NMAC_CMD *)DataBuffer;
			TxInfoCmd->info_type = CMD_PACKET;
			TxInfoCmd->pkt_len = SentLen;
			TxInfoCmd->d_port = CPU_TX_PORT;

/* ++ dump firmware ++ */
/* ++ dump firmware ++ */


#ifdef RT_BIG_ENDIAN
			RTMPDescriptorEndianChange((PUCHAR)TxInfoCmd, TYPE_TXINFO);
#endif
			NdisMoveMemory(DataBuffer + sizeof(*TxInfoCmd), pChipCap->FWImageName + 32 + CurLen, SentLen);

			NdisZeroMemory(DataBuffer + sizeof(*TxInfoCmd) + SentLen, 4);

			Value = CurLen & 0xFFFF;

			/* Set FCE DMA descriptor */
			Status = RTUSB_VendorRequest(pAd,
										 USBD_TRANSFER_DIRECTION_OUT,
										 DEVICE_VENDOR_REQUEST_OUT,
										 0x42,
										 Value,
										 0x230,
										 NULL,
										 0);


			if (Status)
			{
				DBGPRINT(RT_DEBUG_ERROR, ("Set FCE DMA descriptor fail\n"));
				goto error2;
			}
			
			Value = ((CurLen & 0xFFFF0000) >> 16);

			/* Set FCE DMA descriptor */
			Status = RTUSB_VendorRequest(pAd,
										 USBD_TRANSFER_DIRECTION_OUT,
										 DEVICE_VENDOR_REQUEST_OUT,
										 0x42,
										 Value,
										 0x232,
										 NULL,
										 0);

			if (Status)
			{
				DBGPRINT(RT_DEBUG_ERROR, ("Set FCE DMA descriptor fail\n"));
				goto error2;
			}

			

			CurLen += SentLen;

			while ((SentLen % 4) != 0)
				SentLen++;

			Value = ((SentLen << 16) & 0xFFFF);

			/* Set FCE DMA length */
			Status = RTUSB_VendorRequest(pAd,
										 USBD_TRANSFER_DIRECTION_OUT,
										 DEVICE_VENDOR_REQUEST_OUT,
										 0x42,
										 Value,
										 0x234,
										 NULL,
										 0);

			if (Status)
			{
				DBGPRINT(RT_DEBUG_ERROR, ("Set FCE DMA length fail\n"));
				goto error2;
			}
			
			Value = (((SentLen << 16) & 0xFFFF0000) >> 16);

			/* Set FCE DMA length */
			Status = RTUSB_VendorRequest(pAd,
										 USBD_TRANSFER_DIRECTION_OUT,
										 DEVICE_VENDOR_REQUEST_OUT,
										 0x42,
										 Value,
										 0x236,
										 NULL,
										 0);

			if (Status)
			{
				DBGPRINT(RT_DEBUG_ERROR, ("Set FCE DMA length fail\n"));
				goto error2;
			}
		
			/* Initialize URB descriptor */
			RTUSB_FILL_HTTX_BULK_URB(pURB,
									 pObj->pUsb_Dev,
									 pChipCap->CommandBulkOutAddr,
									 DataBuffer,
									 SentLen + sizeof(*TxInfoCmd) + 4,
									 USBUploadFWComplete,
									 //&SentToMCUDone,
									 SentToMCUDone,
									 DataDMA);

			Status = RTUSB_SUBMIT_URB(pURB);

			if (Status)
			{
				DBGPRINT(RT_DEBUG_ERROR, ("SUBMIT URB fail\n"));
				goto error2;
			}

			DBGPRINT(RT_DEBUG_INFO, ("%s: submit URB, SentLen = %d, ILMLen = %d, CurLen = %d\n", __FUNCTION__, SentLen, ILMLen, CurLen));
	
			//if (!wait_for_completion_timeout(&SentToMCUDone, msecs_to_jiffies(1000)))
			if (!RtmpWaitForCompletionTimeout(SentToMCUDone, RtmpMsecsToJiffies(1000)))
			{
				RTUSB_UNLINK_URB(pURB);
				Ret = NDIS_STATUS_FAILURE;
				DBGPRINT(RT_DEBUG_ERROR, ("Upload FW timeout\n"));
				goto error2;
			}
			DBGPRINT(RT_DEBUG_OFF, ("."));

			RTMP_IO_READ32(pAd, TX_CPU_PORT_FROM_FCE_CPU_DESC_INDEX, &MACValue);
			MACValue++;
			RTMP_IO_WRITE32(pAd, TX_CPU_PORT_FROM_FCE_CPU_DESC_INDEX, MACValue);

/* ++ dump firmware ++ */
/* ++ dump firmware ++ */

		}
		else
		{
			break;
		}		

		/* Check if DMA done */
		Loop = 0;
		do
		{
			RTMP_IO_READ32(pAd, COM_REG1, &MACValue);
			if (MACValue & 0x80000000)			// DDONE 0x400234, bit[31]
				break;
			Loop++;
			RtmpOsMsDelay(5);
		} while (Loop <= 100);
		
	}

	os_free_mem(NULL, SentToMCUDone);

	//init_completion(&SentToMCUDone);
	SentToMCUDone = RtmpInitCompletion();
	CurLen = 0x00;

	/* Loading DLM */
	while (1)
	{
		SentLen = (DLMLen - CurLen) >= 14336 ? 14336 : (DLMLen - CurLen);

		if (SentLen > 0)
		{
			TxInfoCmd = (TXINFO_NMAC_CMD *)DataBuffer;
			TxInfoCmd->info_type = CMD_PACKET;
			TxInfoCmd->pkt_len = SentLen;
			TxInfoCmd->d_port = CPU_TX_PORT;

#ifdef RT_BIG_ENDIAN
			RTMPDescriptorEndianChange((PUCHAR)TxInfoCmd, TYPE_TXINFO);
#endif
			NdisMoveMemory(DataBuffer + sizeof(*TxInfoCmd), pChipCap->FWImageName + 32 + ILMLen + CurLen, SentLen);
	
			NdisZeroMemory(DataBuffer + sizeof(*TxInfoCmd) + SentLen + 4, 4);

			Value = ((CurLen + 0x80000) & 0xFFFF);

			/* Set FCE DMA descriptor */
			Status = RTUSB_VendorRequest(pAd,
										 USBD_TRANSFER_DIRECTION_OUT,
										 DEVICE_VENDOR_REQUEST_OUT,
										 0x42,
										 Value,
										 0x230,
										 NULL,
										 0);


			if (Status)
			{
				DBGPRINT(RT_DEBUG_ERROR, ("Set FCE DMA descriptor fail\n"));
				goto error2;
			}
			
			Value = (((CurLen + 0x80000) & 0xFFFF0000) >> 16);

			/* Set FCE DMA descriptor */
			Status = RTUSB_VendorRequest(pAd,
										 USBD_TRANSFER_DIRECTION_OUT,
										 DEVICE_VENDOR_REQUEST_OUT,
										 0x42,
										 Value,
										 0x232,
										 NULL,
										 0);

			if (Status)
			{
				DBGPRINT(RT_DEBUG_ERROR, ("Set FCE DMA descriptor fail\n"));
				goto error2;
			}

			

			CurLen += SentLen;

			while ((SentLen % 4) != 0)
				SentLen++;

			Value = ((SentLen << 16) & 0xFFFF);

			/* Set FCE DMA length */
			Status = RTUSB_VendorRequest(pAd,
										 USBD_TRANSFER_DIRECTION_OUT,
										 DEVICE_VENDOR_REQUEST_OUT,
										 0x42,
										 Value,
										 0x234,
										 NULL,
										 0);

			if (Status)
			{
				DBGPRINT(RT_DEBUG_ERROR, ("Set FCE DMA length fail\n"));
				goto error2;
			}
			
			Value = (((SentLen << 16) & 0xFFFF0000) >> 16);

			/* Set FCE DMA length */
			Status = RTUSB_VendorRequest(pAd,
										 USBD_TRANSFER_DIRECTION_OUT,
										 DEVICE_VENDOR_REQUEST_OUT,
										 0x42,
										 Value,
										 0x236,
										 NULL,
										 0);

			if (Status)
			{
				DBGPRINT(RT_DEBUG_ERROR, ("Set FCE DMA length fail\n"));
				goto error2;
			}
		
			/* Initialize URB descriptor */
			RTUSB_FILL_HTTX_BULK_URB(pURB,
									 pObj->pUsb_Dev,
									 pChipCap->CommandBulkOutAddr,
									 DataBuffer,
									 SentLen + sizeof(*TxInfoCmd) + 4,
									 USBUploadFWComplete,
									 //&SentToMCUDone,
									 SentToMCUDone,
									 DataDMA);

			Status = RTUSB_SUBMIT_URB(pURB);

			if (Status)
			{
				DBGPRINT(RT_DEBUG_ERROR, ("SUBMIT URB fail\n"));
				goto error2;
			}

			DBGPRINT(RT_DEBUG_INFO, ("%s: submit URB, SentLen = %d, DLMLen = %d, CurLen = %d\n", __FUNCTION__, SentLen, DLMLen, CurLen));
	
			//if (!wait_for_completion_timeout(&SentToMCUDone, msecs_to_jiffies(1000)))
			if (!RtmpWaitForCompletionTimeout(SentToMCUDone, RtmpMsecsToJiffies(1000)))
			{
				RTUSB_UNLINK_URB(pURB);
				Ret = NDIS_STATUS_FAILURE;
				DBGPRINT(RT_DEBUG_ERROR, ("Upload FW timeout\n"));
				goto error2;
			}
			DBGPRINT(RT_DEBUG_OFF, ("."));

			RTUSBReadMACRegister(pAd, TX_CPU_PORT_FROM_FCE_CPU_DESC_INDEX, &MACValue);
			MACValue++;
			RTUSBWriteMACRegister(pAd, TX_CPU_PORT_FROM_FCE_CPU_DESC_INDEX, MACValue, FALSE);
		}
		else
		{
			break;
		}		

		//mdelay(5);
		RtmpOsMsDelay(5);
	}

	os_free_mem(NULL, SentToMCUDone);

	/* Upload new 64 bytes interrupt vector */
	DBGPRINT(RT_DEBUG_OFF, ("\n"));
	Status = USBLoadIVB(pAd);
	
	/* Check MCU if ready */
	Loop = 0;
	do
	{
		RTMP_IO_READ32(pAd, COM_REG0, &MACValue);
		if (MACValue == 0x1)
			break;
		RtmpOsMsDelay(10);
		Loop++;
	} while (Loop <= 100);


	DBGPRINT(RT_DEBUG_TRACE, ("%s: COM_REG0(0x%x) = 0x%x\n", __FUNCTION__, COM_REG0, MACValue));

	if (MACValue != 0x1)
		Status = NDIS_STATUS_FAILURE;
	
error2:
	/* Free TransferBuffer */
	RTUSB_URB_FREE_BUFFER(pObj->pUsb_Dev, 14592, 
								DataBuffer, DataDMA);

error1:
	/* Free URB */
	RTUSB_FREE_URB(pURB);

error0: 	
	if (pChipCap->IsComboChip)
		RTUSBWriteMACRegister(pAd, SEMAPHORE_00, 0x1, FALSE);
	return Status;
}
#endif /* RTMP_USB_SUPPORT */


VOID MCUCtrlInit(PRTMP_ADAPTER pAd)
{
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_POLL_IDLE);
	NdisZeroMemory(MCtrl, sizeof(*MCtrl));
	MCtrl->CmdSeq = 0;
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
	NdisAllocateSpinLock(pAd, &MCtrl->CmdRspEventListLock);
	DlListInit(&MCtrl->CmdRspEventList);
}


VOID MCUCtrlExit(PRTMP_ADAPTER pAd)
{
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;
	struct CMD_RSP_EVENT *CmdRspEvent, *CmdRspEventTmp;
	INT32 Ret;
	unsigned long IrqFlags;

	RtmpOsMsDelay(30);
	
	RTMP_IRQ_LOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
	DlListForEachSafe(CmdRspEvent, CmdRspEventTmp, &MCtrl->CmdRspEventList, struct CMD_RSP_EVENT, List)
	{
		if (!CmdRspEvent->NeedWait)
		{
			DlListDel(&CmdRspEvent->List);
			os_free_mem(NULL, CmdRspEvent);
		}
	}
	RTMP_IRQ_UNLOCK(&MCtrl->CmdRspEventListLock, IrqFlags);

	NdisFreeSpinLock(&MCtrl->CmdRspEventListLock);
	NdisZeroMemory(MCtrl, sizeof(*MCtrl));
}


BOOLEAN IsInBandCmdProcessing(PRTMP_ADAPTER pAd)
{
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;
	unsigned long IrqFlags;
	BOOLEAN Ret;

	RTMP_IRQ_LOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
	
	if (DlListEmpty(&MCtrl->CmdRspEventList))
		Ret =  FALSE;
	else
		Ret = TRUE;

	RTMP_IRQ_UNLOCK(&MCtrl->CmdRspEventListLock, IrqFlags);

	return Ret;
}


UCHAR GetCmdRspNum(PRTMP_ADAPTER pAd)
{
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;
	unsigned long IrqFlags;
	UCHAR Num = 0;
	Num = DlListLen(&MCtrl->CmdRspEventList);

	return Num;
}


static inline UCHAR GetCmdSeq(PRTMP_ADAPTER pAd)
{
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;
	struct CMD_RSP_EVENT *CmdRspEvent, *CmdRspEventTmp;
	unsigned long IrqFlags;
	UCHAR TryCount = 0;

	RTMP_IRQ_LOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
get_seq:
	MCtrl->CmdSeq >= 0xf ? MCtrl->CmdSeq = 1 : MCtrl->CmdSeq++;
	TryCount++;
	DlListForEachSafe(CmdRspEvent, CmdRspEventTmp, &MCtrl->CmdRspEventList, struct CMD_RSP_EVENT, List)
	{
		if (CmdRspEvent->CmdSeq == MCtrl->CmdSeq)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("Command(seq: %d) is still running\n", MCtrl->CmdSeq));
			
			if (TryCount > 128)
			{
				break;
			}
			else
			{
				printk("CmdRspNum = %d\n", GetCmdRspNum(pAd));
				goto get_seq;
			}
		}
	}
	RTMP_IRQ_UNLOCK(&MCtrl->CmdRspEventListLock, IrqFlags);

	return MCtrl->CmdSeq;
}


#ifdef RTMP_MAC_USB


USBHST_STATUS USBKickOutCmdComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs)
{
	//struct completion *SentToMCUDone = (struct completion *)RTMP_OS_USB_CONTEXT_GET(pURB);

	//complete(SentToMCUDone);

	VOID	*SentToMCUDone = RTMP_OS_USB_CONTEXT_GET(pURB);

	RtmpComplete(SentToMCUDone);
}


INT USBKickOutCmd(PRTMP_ADAPTER pAd, UCHAR *Buf, UINT32 Len)
{
	PURB pURB;
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	PCHAR DataBuffer;
	ra_dma_addr_t DataDMA;
	INT Ret;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	//struct completion SentToMCUDone;
	VOID	*SentToMCUDone;

	//if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST | fRTMP_ADAPTER_RADIO_OFF | fRTMP_ADAPTER_HALT_IN_PROGRESS))
		//return NDIS_STATUS_FAILURE;

	/* Allocate URB */
	pURB = RTUSB_ALLOC_URB(0);

	if (!pURB)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Can not allocate URB\n"));
		Status = NDIS_STATUS_RESOURCES; 
		goto error0;
	}

	/* Allocate TransferBuffer */
	DataBuffer = RTUSB_URB_ALLOC_BUFFER(pObj->pUsb_Dev, 512, &DataDMA);
		
	if (!DataBuffer)
	{
		Status = NDIS_STATUS_RESOURCES;
		goto error1;
	}
	
	NdisMoveMemory(DataBuffer, Buf, Len);

	NdisZeroMemory(DataBuffer + Len, 4);


	//init_completion(&SentToMCUDone);
	SentToMCUDone = RtmpInitCompletion();

	/* Initialize URB descriptor */
	RTUSB_FILL_HTTX_BULK_URB(pURB,
							 pObj->pUsb_Dev,
							 pChipCap->CommandBulkOutAddr,
							 DataBuffer,
							 Len + 4,
							 USBKickOutCmdComplete,
							 //&SentToMCUDone,
							 SentToMCUDone,
							 DataDMA);

	Status = RTUSB_SUBMIT_URB(pURB);

	if (Status)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("SUBMIT URB fail\n"));
		goto error2;
	}
			
	//if (!wait_for_completion_timeout(&SentToMCUDone, msecs_to_jiffies(500)))
	if (!RtmpWaitForCompletionTimeout(SentToMCUDone, RtmpMsecsToJiffies(500)))
	{
		RTUSB_UNLINK_URB(pURB);
		Ret = NDIS_STATUS_FAILURE;
		DBGPRINT(RT_DEBUG_ERROR, ("%s Timeout\n", __FUNCTION__));
		hex_dump("CmdBuffer", (char *)DataBuffer, Len + 4);
	}

error2:
	/* Free TransferBuffer */
	RTUSB_URB_FREE_BUFFER(pObj->pUsb_Dev, 512, 
								DataBuffer, DataDMA);

	os_free_mem(NULL, SentToMCUDone);


error1:
	/* Free URB */
	RTUSB_FREE_URB(pURB);

error0: 
	return Status;
}
#endif /* RTMP_MAC_USB */


INT AsicSendCmdToAndes(PRTMP_ADAPTER pAd, struct CMD_UNIT *CmdUnit)
{
	UINT32 VarLen;
	UCHAR *Pos, *Buf;
	TXINFO_NMAC_CMD *TxInfoCmd;
	INT32 Ret = NDIS_STATUS_SUCCESS;
	struct MCU_CTRL *MCtrl = &pAd->MCUCtrl;
	struct CMD_RSP_EVENT *CmdRspEvent;
	ULONG Expire;
	unsigned long IrqFlags;

	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD))
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s: !fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD && fRTMP_ADAPTER_IDLE_RADIO_OFF\n", __FUNCTION__));
		return NDIS_STATUS_FAILURE;
	}

	if (pAd->PM_FlgSuspend)
		return NDIS_STATUS_FAILURE;

	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST | fRTMP_ADAPTER_IDLE_RADIO_OFF | fRTMP_ADAPTER_HALT_IN_PROGRESS))
		return NDIS_STATUS_FAILURE;

	VarLen = sizeof(*TxInfoCmd) + CmdUnit->u.ANDES.CmdPayloadLen;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	NdisZeroMemory(Buf, VarLen);

	Pos = Buf;
	TxInfoCmd = (TXINFO_NMAC_CMD *)Pos;
	TxInfoCmd->info_type = CMD_PACKET;
	TxInfoCmd->d_port = CPU_TX_PORT;
	TxInfoCmd->cmd_type = CmdUnit->u.ANDES.Type;

	if (CmdUnit->u.ANDES.NeedRsp)
	{
		TxInfoCmd->cmd_seq = GetCmdSeq(pAd);

		//printk("cmd seq = %d\n", TxInfoCmd->cmd_seq);

		os_alloc_mem(NULL, (UCHAR **)&CmdRspEvent, sizeof(*CmdRspEvent));

		if (!CmdRspEvent)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("%s Not available memory\n", __FUNCTION__));
			Ret = NDIS_STATUS_RESOURCES;
			goto error;
		}

		NdisZeroMemory(CmdRspEvent, sizeof(*CmdRspEvent));

		CmdRspEvent->CmdSeq = TxInfoCmd->cmd_seq;
		CmdRspEvent->Timeout = CmdUnit->u.ANDES.Timeout;
		CmdRspEvent->RspPayload = &CmdUnit->u.ANDES.RspPayload;
		CmdRspEvent->RspPayloadLen = &CmdUnit->u.ANDES.RspPayloadLen;

		if (CmdUnit->u.ANDES.NeedWait)
		{
			CmdRspEvent->NeedWait = TRUE;
			CmdRspEvent->AckDone = RtmpInitCompletion();
		}

		RTMP_IRQ_LOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
		DlListAddTail(&MCtrl->CmdRspEventList, &CmdRspEvent->List);
		RTMP_IRQ_UNLOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
	}
	else
	{	
		TxInfoCmd->cmd_seq = 0;
	}

	TxInfoCmd->pkt_len = CmdUnit->u.ANDES.CmdPayloadLen;

#ifdef RT_BIG_ENDIAN
	RTMPDescriptorEndianChange((PUCHAR)TxInfoCmd, TYPE_TXINFO);
#endif

	Pos += sizeof(*TxInfoCmd);
	
	NdisMoveMemory(Pos, CmdUnit->u.ANDES.CmdPayload, CmdUnit->u.ANDES.CmdPayloadLen);
	
#ifdef RTMP_USB_SUPPORT
	USBKickOutCmd(pAd, Buf, VarLen);
#endif


	/* Wait for Command Rsp */
	if (CmdUnit->u.ANDES.NeedWait) {
		ULONG Timeout = CmdUnit->u.ANDES.Timeout;
		Expire = Timeout ? RtmpMsecsToJiffies(Timeout) : RtmpMsecsToJiffies(300);
		if (!RtmpWaitForCompletionTimeout(CmdRspEvent->AckDone, Expire))
		{
			Ret = NDIS_STATUS_FAILURE;
			DBGPRINT(RT_DEBUG_ERROR, ("Wait for command response timeout(300ms)\n"));
		}
	
		RTMP_IRQ_LOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
		DlListDel(&CmdRspEvent->List);
		os_free_mem(NULL, CmdRspEvent->AckDone);
		os_free_mem(NULL, CmdRspEvent);
		RTMP_IRQ_UNLOCK(&MCtrl->CmdRspEventListLock, IrqFlags);
	}

error:
	os_free_mem(NULL, Buf);

	return Ret;
}

static VOID CmdDoneHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{


}


static VOID CmdErrorHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{


}


static VOID CmdRetryHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{


}


static VOID PwrRspEventHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{


}


static VOID WowRspEventHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{


}


static VOID CarrierDetectRspEventHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{



}


static VOID DFSDetectRspEventHandler(PRTMP_ADAPTER pAd, UCHAR *Data)
{



}


CMD_RSP_HANDLER CmdRspHandlerTable[] =
{
	CmdDoneHandler,
	CmdErrorHandler,
	CmdRetryHandler,
	PwrRspEventHandler,
	WowRspEventHandler,
	CarrierDetectRspEventHandler,
	DFSDetectRspEventHandler,
};


INT AndesBurstWrite(PRTMP_ADAPTER pAd, UINT32 Offset, UINT32 *Data, UINT32 Cnt)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen, OffsetNum, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	BOOLEAN LastPacket = FALSE;

	OffsetNum = Cnt / ((pChipCap->InbandPacketMaxLen - sizeof(Offset)) / 4);

	if (Cnt % ((pChipCap->InbandPacketMaxLen - sizeof(Offset)) / 4))
		VarLen = sizeof(Offset) * (OffsetNum + 1) + 4 * Cnt;
	else
		VarLen = sizeof(Offset) * OffsetNum + 4 * Cnt;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);

	Pos = Buf;
	
	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);
		
		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;
	
		CurHeader = Pos;

		Value = cpu2le32(Offset + pChipCap->WlanMemmapOffset + CurIndex * 4);
		NdisMoveMemory(Pos, &Value, 4);
		Pos += 4;

		for (i = 0; i < ((SentLen - 4) / 4); i++)
		{
			Value = cpu2le32(Data[i + CurIndex]);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		};

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_BURST_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket && (Cnt > 1))
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
		
		CurIndex += ((SentLen - 4) / 4);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesBurstRead(PRTMP_ADAPTER pAd, UINT32 Offset, UINT32 Cnt, UINT32 *Data)
{
	struct CMD_UNIT CmdUnit;
	UINT32 CurLen = 0, CmdLen, RspLen, OffsetNum, ReceiveLen;
	CHAR *Pos, *Pos1, *CmdBuf, *RspBuf, *CurCmdHeader, *CurRspHeader;
	UINT32 i, Value, Status = NDIS_STATUS_SUCCESS, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;

	OffsetNum = Cnt / ((pChipCap->InbandPacketMaxLen - sizeof(Offset)) / 4);

	if (Cnt % ((pChipCap->InbandPacketMaxLen - sizeof(Offset)) / 4))
	{
		CmdLen = 8 * (OffsetNum + 1); 
		RspLen = sizeof(Offset) * (OffsetNum + 1) + 4 * Cnt;
	}
	else
	{
		CmdLen = 8 * OffsetNum;
		RspLen = sizeof(Offset) * OffsetNum + 4 * Cnt;
	}

	os_alloc_mem(pAd, (UCHAR **)&CmdBuf, CmdLen);
	os_alloc_mem(pAd, (UCHAR **)&RspBuf, RspLen);
	
	Pos = CmdBuf;
	Pos1 = RspBuf;

	while (CurLen < RspLen)
	{
		ReceiveLen = (RspLen - CurLen) > pChipCap->InbandPacketMaxLen 
									   ? pChipCap->InbandPacketMaxLen 
									   : (RspLen - CurLen);

		CurCmdHeader = Pos;
		CurRspHeader = Pos1;
		
		Value = cpu2le32(Offset + pChipCap->WlanMemmapOffset + CurIndex * 4);
		NdisMoveMemory(Pos, &Value, 4);
		Pos += 4;

		Value = cpu2le32((ReceiveLen - 4) / 4);
		NdisMoveMemory(Pos, &Value, 4);
		Pos += 4;

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
		CmdUnit.u.ANDES.Type = CMD_BURST_READ;
		CmdUnit.u.ANDES.CmdPayloadLen = 8;
		CmdUnit.u.ANDES.CmdPayload = CurCmdHeader;
		CmdUnit.u.ANDES.RspPayload = CurRspHeader;
		CmdUnit.u.ANDES.RspPayloadLen = ReceiveLen;
		CmdUnit.u.ANDES.NeedRsp = TRUE;
		CmdUnit.u.ANDES.NeedWait = TRUE;
		CmdUnit.u.ANDES.Timeout = 0;

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;

		if (CmdUnit.u.ANDES.RspPayloadLen == ReceiveLen)
		{
			NdisMoveMemory(&Data[CurIndex], CmdUnit.u.ANDES.RspPayload + 4, CmdUnit.u.ANDES.RspPayloadLen - 4);
			Pos1 += ReceiveLen;

			for (i = 0; i < (ReceiveLen - 4) / 4; i++)
			{
				Data[i + CurIndex] = le2cpu32(Data[i + CurIndex]);
			}
		}
		else
		{
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Rsp len(%d) != Expect len (%d)\n", 
				__FUNCTION__, CmdUnit.u.ANDES.RspPayloadLen, ReceiveLen));

			Status = NDIS_STATUS_FAILURE;

			goto error;
		}
		
		CurIndex += ((ReceiveLen - 4) / 4);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:

	os_free_mem(NULL, CmdBuf);
	os_free_mem(NULL, RspBuf);

	return Status;
}


INT AndesRandomRead(PRTMP_ADAPTER pAd, RTMP_REG_PAIR *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	UINT32 VarLen = Num * 8, CurLen = 0, ReceiveLen;
	CHAR *Pos, *Pos1, *CmdBuf, *RspBuf, *CurCmdHeader, *CurRspHeader;
	UINT32 i, Value, Status = NDIS_STATUS_SUCCESS, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;

	os_alloc_mem(pAd, (UCHAR **)&CmdBuf, VarLen);
	os_alloc_mem(pAd, (UCHAR **)&RspBuf, VarLen);

	NdisZeroMemory(CmdBuf, VarLen);

	Pos = CmdBuf;
	Pos1 = RspBuf;

	while (CurLen < VarLen)
	{
		ReceiveLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									   ? pChipCap->InbandPacketMaxLen 
									   : (VarLen - CurLen);

		CurCmdHeader = Pos;
		CurRspHeader = Pos1;

		for (i = 0; i < ReceiveLen / 8; i++)
		{
			Value = cpu2le32(RegPair[i + CurIndex].Register + pChipCap->WlanMemmapOffset);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 8;
		}
	
		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
		CmdUnit.u.ANDES.Type = CMD_RANDOM_READ;
		CmdUnit.u.ANDES.CmdPayloadLen = ReceiveLen;
		CmdUnit.u.ANDES.CmdPayload = CurCmdHeader;
		CmdUnit.u.ANDES.RspPayload = CurRspHeader;
		CmdUnit.u.ANDES.RspPayloadLen = ReceiveLen;
		CmdUnit.u.ANDES.NeedRsp = TRUE;
		CmdUnit.u.ANDES.NeedWait = TRUE;
		CmdUnit.u.ANDES.Timeout = 0;

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
	
		if (CmdUnit.u.ANDES.RspPayloadLen == ReceiveLen)
		{
			for (i = 0; i < ReceiveLen / 8; i++)
			{
				NdisMoveMemory(&RegPair[i + CurIndex].Value, CmdUnit.u.ANDES.RspPayload + 8 * i + 4, 4);
				RegPair[i + CurIndex].Value = le2cpu32(RegPair[i + CurIndex].Value);
			}
		}
		else
		{
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Rsp len(%d) != Expect len (%d)\n", 
				__FUNCTION__, CmdUnit.u.ANDES.RspPayloadLen, ReceiveLen));

			Status = NDIS_STATUS_FAILURE;

			goto error;
		}
		
		CurIndex += ReceiveLen / 8;
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:	
	os_free_mem(NULL, CmdBuf);
	os_free_mem(NULL, RspBuf);

	return Status;
}


INT AndesRFRandomRead(PRTMP_ADAPTER pAd, BANK_RF_REG_PAIR *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	UINT32 VarLen = Num * 8, CurLen = 0, ReceiveLen;
	CHAR *Pos, *Pos1, *CmdBuf, *RspBuf, *CurCmdHeader, *CurRspHeader;
	UINT32 i, Value, Status = NDIS_STATUS_SUCCESS, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;

	os_alloc_mem(pAd, (UCHAR **)&CmdBuf, VarLen);
	os_alloc_mem(pAd, (UCHAR **)&RspBuf, VarLen);

	NdisZeroMemory(CmdBuf, VarLen);
	Pos = CmdBuf;
	Pos1 = RspBuf;

	while (CurLen < VarLen)
	{
		ReceiveLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									   ? pChipCap->InbandPacketMaxLen 
									   : (VarLen - CurLen);

		CurCmdHeader = Pos;
		CurRspHeader = Pos1;

		for (i = 0; i < ReceiveLen / 8; i++)
		{
			Value = 0;
	
			/* RF selection */
			Value = (Value & ~0x80000000) | 0x80000000;

			/* RF bank */
			Value = (Value & ~0x00ff0000) | (RegPair[i + CurIndex].Bank << 16);

			/* RF Index */
			Value = (Value & ~0x0000ffff) | RegPair[i + CurIndex].Register;

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 8;
		}
	
		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
		CmdUnit.u.ANDES.Type = CMD_RANDOM_READ;
		CmdUnit.u.ANDES.CmdPayloadLen = ReceiveLen;
		CmdUnit.u.ANDES.CmdPayload = CurCmdHeader;
		CmdUnit.u.ANDES.RspPayload = CurRspHeader;
		CmdUnit.u.ANDES.RspPayloadLen = ReceiveLen;
		CmdUnit.u.ANDES.NeedRsp = TRUE;
		CmdUnit.u.ANDES.NeedWait = TRUE;
		CmdUnit.u.ANDES.Timeout = 0;

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
	
	
		if (CmdUnit.u.ANDES.RspPayloadLen == ReceiveLen)
		{
			for (i = 0; i < ReceiveLen / 8; i++)
			{
				NdisMoveMemory(&RegPair[i + CurIndex].Value, CmdUnit.u.ANDES.RspPayload + 8 * i + 4, 1);
			}
		}
		else
		{
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Rsp len(%d) != Expect len (%d)\n", 
				__FUNCTION__, CmdUnit.u.ANDES.RspPayloadLen, ReceiveLen));

			Status = NDIS_STATUS_FAILURE;

			goto error;
		}
		
		CurIndex += ReceiveLen / 8;
		CurLen += pChipCap->InbandPacketMaxLen;
	}
	
error:
	os_free_mem(NULL, CmdBuf);
	os_free_mem(NULL, RspBuf);

	return Status;
}


INT AndesReadModifyWrite(PRTMP_ADAPTER pAd, R_M_W_REG *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 12, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	BOOLEAN LastPacket = FALSE;
	
	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);

	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);
		
		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;

		CurHeader = Pos;

		for (i = 0; i < (SentLen / 12); i++)
		{
			/* Address */
			Value = cpu2le32(RegPair[i + CurIndex].Register + pChipCap->WlanMemmapOffset);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			/* ClearBitMask */
			Value = cpu2le32(RegPair[i + CurIndex].ClearBitMask);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			/* UpdateData */
			Value = cpu2le32(RegPair[i + CurIndex].Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		}

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_READ_MODIFY_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
	
		CurIndex += (SentLen / 12);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesRFReadModifyWrite(PRTMP_ADAPTER pAd, RF_R_M_W_REG *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 12, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	BOOLEAN LastPacket = FALSE;
	
	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);

	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);
		
		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;
		
		CurHeader = Pos;
		
		for (i = 0; i < SentLen / 12; i++)
		{
			Value = 0;
			/* RF selection */
			Value = (Value & ~0x80000000) | 0x80000000;

			/* RF bank */
			Value = (Value & ~0x00ff0000) | (RegPair[i + CurIndex].Bank << 16);

			/* RF Index */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].Register;

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			Value = 0;
			/* ClearBitMask */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].ClearBitMask;
		
			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			Value = 0;
			/* UpdateData */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].Value;

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		}
	
		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_READ_MODIFY_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
		
		CurIndex += (SentLen / 12);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesRandomWritePair(PRTMP_ADAPTER pAd, RTMP_REG_PAIR *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 8, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	BOOLEAN LastPacket = FALSE;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);

		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;

		CurHeader = Pos;
		
		for (i = 0; i < (SentLen / 8); i++)
		{
			/* Address */
			Value = cpu2le32(RegPair[i + CurIndex].Register + pChipCap->WlanMemmapOffset);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			/* UpdateData */
			Value = cpu2le32(RegPair[i + CurIndex].Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		};

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_RANDOM_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
	
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
	
		CurIndex += (SentLen / 8);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesRandomWrite(PRTMP_ADAPTER pAd, UINT32 Num, ...)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 8, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	va_list argptr;
	BOOLEAN LastPacket = FALSE;

	va_start(argptr, Num);

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);

		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;

		CurHeader = Pos;
		
		for (i = 0; i < (SentLen / 8); i++)
		{
			/* Address */
			Value = cpu2le32( va_arg(argptr, UINT32) +pChipCap->WlanMemmapOffset);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			/* UpdateData */
			Value = cpu2le32(va_arg(argptr, UINT32));
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		};

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_RANDOM_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
	
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
	
		CurIndex += (SentLen / 8);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);
	va_end(argptr);

	return NDIS_STATUS_SUCCESS;
}


INT AndesRFRandomWritePair(PRTMP_ADAPTER pAd, BANK_RF_REG_PAIR *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 8, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	BOOLEAN LastPacket = FALSE;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);
		
		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;

		CurHeader = Pos;
		
		for (i = 0; i < (SentLen / 8); i++)
		{
			Value = 0;
			/* RF selection */
			Value = (Value & ~0x80000000) | 0x80000000;

			/* RF bank */
			Value = (Value & ~0x00ff0000) | (RegPair[i + CurIndex].Bank << 16);

			/* RF Index */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].Register;
			
			//printk("Value = %x RF Bank = %d and Index = %d\n", Value, RegPair[i + CurIndex].Bank, RegPair[i + CurIndex].Register);

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			Value = 0;
			/* UpdateData */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].Value;

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		}

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_RANDOM_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
		
		CurIndex += (SentLen / 8);
		CurLen += pChipCap->InbandPacketMaxLen;
	}


error:
	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesRFRandomWrite(PRTMP_ADAPTER pAd, UINT32 Num, ...)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 8, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	va_list argptr;
	BOOLEAN LastPacket = FALSE;

	va_start(argptr, Num);

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);

		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;

		CurHeader = Pos;
		
		for (i = 0; i < (SentLen / 8); i++)
		{
			Value = 0;
			/* RF selection */
			Value = (Value & ~0x80000000) | 0x80000000;

			/* RF bank */
			Value = (Value & ~0x00ff0000) | (va_arg(argptr, UINT) << 16);

			/* RF Index */
			Value = (Value & ~0x000000ff) | va_arg(argptr, UINT);
			
			//printk("Value = %x RF Bank = %d and Index = %d\n", Value, RegPair[i + CurIndex].Bank, RegPair[i + CurIndex].Register);

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			Value = 0;
			/* UpdateData */
			Value = (Value & ~0x000000ff) | va_arg(argptr, UINT);

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		}

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_RANDOM_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;
		
		CurIndex += (SentLen / 8);
		CurLen += pChipCap->InbandPacketMaxLen;
	}


error:
	os_free_mem(NULL, Buf);
	va_end(argptr);

	return NDIS_STATUS_SUCCESS;
}


#ifdef MT7601
INT AndesBBPRandomWritePair(PRTMP_ADAPTER pAd, RTMP_REG_PAIR *RegPair, UINT32 Num)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 8, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	BOOLEAN LastPacket = FALSE;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);

		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;
		
		CurHeader = Pos;
		
		for (i = 0; i < (SentLen / 8); i++)
		{
			Value = 0;
			/* BBP selection */
			Value = (Value & ~0x40000000) | 0x40000000;

			/* BBP Index */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].Register;
			
			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			Value = 0;
			/* UpdateData */
			Value = (Value & ~0x000000ff) | RegPair[i + CurIndex].Value;

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		}

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_RANDOM_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;

		CurIndex += (SentLen / 8);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesBBPRandomWrite(PRTMP_ADAPTER pAd, UINT32 Num, ...)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf, *CurHeader;
	UINT32 VarLen = Num * 8, CurLen = 0, SentLen;
	UINT32 Value, i, CurIndex = 0;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	INT32 Ret;
	va_list argptr;
	BOOLEAN LastPacket = FALSE;

	va_start(argptr, Num);

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);
	
	Pos = Buf;

	while (CurLen < VarLen)
	{
		SentLen = (VarLen - CurLen) > pChipCap->InbandPacketMaxLen 
									? pChipCap->InbandPacketMaxLen : (VarLen - CurLen);

		if ((SentLen < pChipCap->InbandPacketMaxLen) || (CurLen + pChipCap->InbandPacketMaxLen) == VarLen)
			LastPacket = TRUE;
		
		CurHeader = Pos;
		
		for (i = 0; i < (SentLen / 8); i++)
		{
			Value = 0;
			/* BBP selection */
			Value = (Value & ~0x40000000) | 0x40000000;

			/* BBP Index */
			Value = (Value & ~0x000000ff) | va_arg(argptr, UINT);
			
			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;

			Value = 0;
			/* UpdateData */
			Value = (Value & ~0x000000ff) | va_arg(argptr, UINT);

			Value = cpu2le32(Value);
			NdisMoveMemory(Pos, &Value, 4);
			Pos += 4;
		}

		NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
		CmdUnit.u.ANDES.Type = CMD_RANDOM_WRITE;
		CmdUnit.u.ANDES.CmdPayloadLen = SentLen;
		CmdUnit.u.ANDES.CmdPayload = CurHeader;

		if (LastPacket)
		{
			CmdUnit.u.ANDES.NeedRsp = TRUE;
			CmdUnit.u.ANDES.NeedWait = TRUE;
			CmdUnit.u.ANDES.Timeout = 0;
		}

		Ret = AsicSendCmdToAndes(pAd, &CmdUnit);
		
		if (Ret != NDIS_STATUS_SUCCESS)
			goto error;

		CurIndex += (SentLen / 8);
		CurLen += pChipCap->InbandPacketMaxLen;
	}

error:
	os_free_mem(NULL, Buf);
	va_end(argptr);

	return NDIS_STATUS_SUCCESS;
}

#endif


INT AndesPwrSavingOP(PRTMP_ADAPTER pAd, UINT32 PwrOP, UINT32 PwrLevel, 
					UINT32 ListenInterval, UINT32 PreTBTTLeadTime,
					UINT8 TIMByteOffset, UINT8 TIMBytePattern)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf;
	UINT32 VarLen;
	UINT32 Value;
	INT32 Ret;

	/* Power operation and Power Level */
	VarLen = 8;

	if (PwrOP == RADIO_OFF_ADVANCE)
	{
		/* Listen interval, Pre-TBTT, TIM info */
		VarLen += 12;
	}

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);

	Pos = Buf;

	/* Power operation */
	Value = cpu2le32(PwrOP);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;

	/* Power Level */
	Value = cpu2le32(PwrLevel);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;

	if ( (PwrOP == RADIO_OFF_ADVANCE) || (PwrOP == RADIO_OFF_AUTO_WAKEUP))
	{
		/* Listen interval */
		Value = cpu2le32(ListenInterval);
		NdisMoveMemory(Pos, &Value, 4);
		Pos += 4;


		/* Pre TBTT lead time */
		Value = cpu2le32(PreTBTTLeadTime);
		NdisMoveMemory(Pos, &Value, 4);
		Pos += 4;
	}

	if (PwrOP == RADIO_OFF_ADVANCE)
	{
		/* TIM Info */
		Value = (Value & ~0x000000ff) | TIMBytePattern;
		Value = (Value & ~0x0000ff00) | (TIMByteOffset << 8);
		Value = cpu2le32(Value);
		NdisMoveMemory(Pos, &Value, 4);
		Pos += 4;
	}

	NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
	CmdUnit.u.ANDES.Type = CMD_POWER_SAVING_OP;
	CmdUnit.u.ANDES.CmdPayloadLen = VarLen;
	CmdUnit.u.ANDES.CmdPayload = Buf;

	CmdUnit.u.ANDES.NeedRsp = FALSE;
	CmdUnit.u.ANDES.NeedWait = FALSE;
		
	CmdUnit.u.ANDES.Timeout = 0;

	Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesFunSetOP(PRTMP_ADAPTER pAd, UINT32 FunID, UINT32 Param)
{
	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf;
	UINT32 VarLen;
	UINT32 Value;
	INT32 Ret;

	/* Function ID and Parameter */
	VarLen = 8;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);

	Pos = Buf;
	
	/* Function ID */
	Value = cpu2le32(FunID);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;

	/* Parameter */
	Value = cpu2le32(Param);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;
	
	NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
	CmdUnit.u.ANDES.Type = CMD_FUN_SET_OP;
	CmdUnit.u.ANDES.CmdPayloadLen = VarLen;
	CmdUnit.u.ANDES.CmdPayload = Buf;

	if ( FunID == 5  )
	{
		CmdUnit.u.ANDES.NeedRsp = TRUE;
		CmdUnit.u.ANDES.NeedWait = TRUE;
		CmdUnit.u.ANDES.Timeout = 0;
	}

	Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}


INT AndesCalibrationOP(PRTMP_ADAPTER pAd, UINT32 CalibrationID, UINT32 Param)
{

	struct CMD_UNIT CmdUnit;
	CHAR *Pos, *Buf;
	UINT32 VarLen;
	UINT32 Value;
	INT32 Ret;

	/* Calibration ID and Parameter */
	VarLen = 8;

	os_alloc_mem(pAd, (UCHAR **)&Buf, VarLen);

	Pos = Buf;
	
	/* Calibration ID */
	Value = cpu2le32(CalibrationID);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;

	/* Parameter */
	Value = cpu2le32(Param);
	NdisMoveMemory(Pos, &Value, 4);
	Pos += 4;
	
	NdisZeroMemory(&CmdUnit, sizeof(CmdUnit));
	
	CmdUnit.u.ANDES.Type = CMD_CALIBRATION_OP;
	CmdUnit.u.ANDES.CmdPayloadLen = VarLen;
	CmdUnit.u.ANDES.CmdPayload = Buf;
	
	CmdUnit.u.ANDES.NeedRsp = TRUE;
	CmdUnit.u.ANDES.NeedWait = TRUE;
	CmdUnit.u.ANDES.Timeout = 0;

	Ret = AsicSendCmdToAndes(pAd, &CmdUnit);

	os_free_mem(NULL, Buf);

	return NDIS_STATUS_SUCCESS;
}

