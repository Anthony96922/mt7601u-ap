/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2010, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************/

/****************************************************************************

    Abstract:

	All related POWER SAVE function body.

***************************************************************************/

#include "rt_config.h"


/*
	========================================================================
	Routine Description:
		This routine is used to do insert packet into power-saveing queue.
	
	Arguments:
		pAd: Pointer to our adapter
		pPacket: Pointer to send packet
		pMacEntry: portint to entry of MacTab. the pMacEntry store attribute of client (STA).
		QueIdx: Priority queue idex.

	Return Value:
		NDIS_STATUS_SUCCESS:If succes to queue the packet into TxSwQueue.
		NDIS_STATUS_FAILURE: If failed to do en-queue.
========================================================================
*/
NDIS_STATUS RtmpInsertPsQueue(
	IN PRTMP_ADAPTER pAd,
	IN PNDIS_PACKET pPacket,
	IN MAC_TABLE_ENTRY *pMacEntry,
	IN UCHAR QueIdx)
{
	ULONG IrqFlags;
#ifdef UAPSD_SUPPORT
	/* put the U-APSD packet to its U-APSD queue by AC ID */
	UINT32 ac_id = QueIdx - QID_AC_BE; /* should be >= 0 */


	if (UAPSD_MR_IS_UAPSD_AC(pMacEntry, ac_id))
	{
		UAPSD_PacketEnqueue(pAd, pMacEntry, pPacket, ac_id);

	}
	else
#endif /* UAPSD_SUPPORT */
	{
		if (pMacEntry->PsQueue.Number >= MAX_PACKETS_IN_PS_QUEUE)
		{
			RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_FAILURE);			
			return NDIS_STATUS_FAILURE;			
		}
		else
		{
			DBGPRINT(RT_DEBUG_TRACE, ("legacy ps> queue a packet!\n"));
			RTMP_IRQ_LOCK(&pAd->irq_lock, IrqFlags);
			InsertTailQueue(&pMacEntry->PsQueue, PACKET_TO_QUEUE_ENTRY(pPacket));
			RTMP_IRQ_UNLOCK(&pAd->irq_lock, IrqFlags);
		}
	}

#ifdef CONFIG_AP_SUPPORT
	/* mark corresponding TIM bit in outgoing BEACON frame */
#ifdef UAPSD_SUPPORT
	if (UAPSD_MR_IS_NOT_TIM_BIT_NEEDED_HANDLED(pMacEntry, QueIdx))
	{
		/* 1. the station is UAPSD station;
		2. one of AC is non-UAPSD (legacy) AC;
		3. the destinated AC of the packet is UAPSD AC. */
		/* So we can not set TIM bit due to one of AC is legacy AC */
	}
	else
#endif /* UAPSD_SUPPORT */
	{
		WLAN_MR_TIM_BIT_SET(pAd, pMacEntry->apidx, pMacEntry->Aid);

	}
#endif /* CONFIG_AP_SUPPORT */

	return NDIS_STATUS_SUCCESS;
}


/*
	==========================================================================
	Description:
		This routine is used to clean up a specified power-saving queue. It's
		used whenever a wireless client is deleted.
	==========================================================================
 */
VOID RtmpCleanupPsQueue(
	IN  PRTMP_ADAPTER   pAd,
	IN  PQUEUE_HEADER   pQueue)
{
	PQUEUE_ENTRY pEntry;
	PNDIS_PACKET pPacket;

	DBGPRINT(RT_DEBUG_TRACE, ("RtmpCleanupPsQueue (0x%08lx)...\n", (ULONG)pQueue));

	while (pQueue->Head)
	{
		DBGPRINT(RT_DEBUG_TRACE,
					("RtmpCleanupPsQueue %ld...\n",pQueue->Number));

		pEntry = RemoveHeadQueue(pQueue);
		/*pPacket = CONTAINING_RECORD(pEntry, NDIS_PACKET, MiniportReservedEx); */
		pPacket = QUEUE_ENTRY_TO_PACKET(pEntry);
		RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_FAILURE);

		DBGPRINT(RT_DEBUG_TRACE, ("RtmpCleanupPsQueue pkt = %lx...\n", (ULONG)pPacket));
	}
}


/*
  ========================================================================
  Description:
	This routine frees all packets in PSQ that's destined to a specific DA.
	BCAST/MCAST in DTIMCount=0 case is also handled here, just like a PS-POLL 
	is received from a WSTA which has MAC address FF:FF:FF:FF:FF:FF
  ========================================================================
*/
VOID RtmpHandleRxPsPoll(
	IN	PRTMP_ADAPTER	pAd,
	IN	PUCHAR			pAddr,
	IN	USHORT			Aid,
    IN	BOOLEAN			isActive)
{ 
	PQUEUE_ENTRY	  pEntry;
	PMAC_TABLE_ENTRY  pMacEntry;
	unsigned long		IrqFlags;

	/*DBGPRINT(RT_DEBUG_TRACE,("rcv PS-POLL (AID=%d) from %02x:%02x:%02x:%02x:%02x:%02x\n", */
	/*	  Aid, pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5])); */

	pMacEntry = &pAd->MacTab.Content[Aid];
	if (RTMPEqualMemory(pMacEntry->Addr, pAddr, MAC_ADDR_LEN))
	{
		/*
			Sta is change to Power Active stat.
			Reset ContinueTxFailCnt
		*/
		pMacEntry->ContinueTxFailCnt = 0;

#ifdef UAPSD_SUPPORT
		if (UAPSD_MR_IS_ALL_AC_UAPSD(isActive, pMacEntry))
		{
			/*
				IEEE802.11e spec.
				11.2.1.7 Receive operation for STAs in PS mode during the CP
				When a non-AP QSTA that is using U-APSD and has all ACs
				delivery-enabled detects that the bit corresponding to its AID
				is set in the TIM, the non-AP QSTA shall issue a trigger frame
				or a PS-Poll frame to retrieve the buffered MSDU or management
				frames.

				WMM Spec. v1.1a 070601
				3.6.2	U-APSD STA Operation
				3.6.2.3	In case one or more ACs are not
				delivery-enabled ACs, the WMM STA may retrieve MSDUs and
				MMPDUs belonging to those ACs by sending PS-Polls to the WMM AP.
				In case all ACs are delivery enabled ACs, WMM STA should only
				use trigger frames to retrieve MSDUs and MMPDUs belonging to
				those ACs, and it should not send PS-Poll frames.

				Different definitions in IEEE802.11e and WMM spec.
				But we follow the WiFi WMM Spec.
			*/

			DBGPRINT(RT_DEBUG_TRACE, ("All AC are UAPSD, can not use PS-Poll\n"));
			return; /* all AC are U-APSD, can not use PS-Poll */
		} /* End of if */
#endif /* UAPSD_SUPPORT */

		/*NdisAcquireSpinLock(&pAd->MacTabLock); */
		/*NdisAcquireSpinLock(&pAd->TxSwQueueLock); */
		RTMP_IRQ_LOCK(&pAd->irq_lock, IrqFlags);
		if (isActive == FALSE)
		{
			if (pMacEntry->PsQueue.Head)
			{
#ifdef UAPSD_SUPPORT
				UINT32 NumOfOldPsPkt;
				NumOfOldPsPkt = pAd->TxSwQueue[QID_AC_BE].Number;
#endif /* UAPSD_SUPPORT */

				pEntry = RemoveHeadQueue(&pMacEntry->PsQueue);
				if ( pMacEntry->PsQueue.Number >=1 )
					RTMP_SET_PACKET_MOREDATA(RTPKT_TO_OSPKT(pEntry), TRUE);
				InsertTailQueueAc(pAd, pMacEntry, &pAd->TxSwQueue[QID_AC_BE], pEntry);

#ifdef UAPSD_SUPPORT
				/* we need to call RTMPDeQueuePacket() immediately as below */
				if (NumOfOldPsPkt != pAd->TxSwQueue[QID_AC_BE].Number)
				{
					if (RTMP_GET_PACKET_DHCP(RTPKT_TO_OSPKT(pEntry)) ||
						RTMP_GET_PACKET_EAPOL(RTPKT_TO_OSPKT(pEntry)) ||
						RTMP_GET_PACKET_WAI(RTPKT_TO_OSPKT(pEntry)))
					{
						/*
							These packets will use 1M/6M rate to send.
							If you use 1M(2.4G)/6M(5G) to send, no statistics
							count in NICUpdateFifoStaCounters().

							So we can not count it for UAPSD; Or the SP will
							not closed until timeout.
						*/
						;
					}
					else
						UAPSD_MR_MIX_PS_POLL_RCV(pAd, pMacEntry);
				}
#endif /* UAPSD_SUPPORT */
			}
			else
			{
				/*
					or transmit a (QoS) Null Frame;

					In addtion, in Station Keep Alive mechanism, we need to
					send a QoS Null frame to detect the station live status.
				*/
				BOOLEAN bQosNull = FALSE;

				if (CLIENT_STATUS_TEST_FLAG(pMacEntry, fCLIENT_STATUS_WMM_CAPABLE))
					bQosNull = TRUE;

				RtmpEnqueueNullFrame(pAd, pMacEntry->Addr, pMacEntry->CurrTxRate,
										Aid, pMacEntry->apidx, bQosNull, TRUE, 0);
			}
		}
		else
		{
#ifdef UAPSD_SUPPORT
			/* deliver all queued UAPSD packets */
			UAPSD_AllPacketDeliver(pAd, pMacEntry);

			/* end the SP if exists */
			UAPSD_MR_ENTRY_RESET(pAd, pMacEntry);
#endif /* UAPSD_SUPPORT */

			while(pMacEntry->PsQueue.Head)
			{
/*				if (pAd->TxSwQueue[QID_AC_BE].Number <= */
/*                    (pAd->PortCfg.TxQueueSize + (MAX_PACKETS_IN_PS_QUEUE>>1))) */
				{
					pEntry = RemoveHeadQueue(&pMacEntry->PsQueue);
					InsertTailQueueAc(pAd, pMacEntry, &pAd->TxSwQueue[QID_AC_BE], pEntry);
				}
/*                else */
/*					break; */
				/* End of if */
			} /* End of while */
		} /* End of if */

		/*NdisReleaseSpinLock(&pAd->TxSwQueueLock); */
		/*NdisReleaseSpinLock(&pAd->MacTabLock); */

		if ((Aid > 0) && (Aid < MAX_LEN_OF_MAC_TABLE) &&
			(pMacEntry->PsQueue.Number == 0))
		{
			/* clear corresponding TIM bit because no any PS packet */
#ifdef CONFIG_AP_SUPPORT
			WLAN_MR_TIM_BIT_CLEAR(pAd, pMacEntry->apidx, Aid);
#endif /* CONFIG_AP_SUPPORT */
			pMacEntry->PsQIdleCount = 0;
		}

		RTMP_IRQ_UNLOCK(&pAd->irq_lock, IrqFlags);

		/* Dequeue outgoing frames from TxSwQueue0..3 queue and process it */
		/* TODO: 2004-12-27 it's not a good idea to handle "More Data" bit here. because the */
		/* RTMPDeQueue process doesn't guarantee to de-queue the desired MSDU from the corresponding */
		/* TxSwQueue/PsQueue when QOS in-used. We should consider "HardTransmt" this MPDU */
		/* using MGMT queue or things like that. */
		RTMPDeQueuePacket(pAd, FALSE, NUM_OF_TX_RING, MAX_TX_PROCESS);

	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR,("rcv PS-POLL (AID=%d not match) from %02x:%02x:%02x:%02x:%02x:%02x\n", 
			  Aid, pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5]));

	}
}


/*
	==========================================================================
	Description:
		Update the station current power save mode. Calling this routine also
		prove the specified client is still alive. Otherwise AP will age-out
		this client once IdleCount exceeds a threshold.
	==========================================================================
 */
BOOLEAN RtmpPsIndicate(
	IN PRTMP_ADAPTER pAd, 
	IN PUCHAR pAddr, 
	IN ULONG Wcid, 
	IN UCHAR Psm) 
{
	MAC_TABLE_ENTRY *pEntry;
    UCHAR old_psmode;

	if (Wcid >= MAX_LEN_OF_MAC_TABLE)
	{
		return PWR_ACTIVE;	
	}

	pEntry = &pAd->MacTab.Content[Wcid];
	old_psmode = pEntry->PsMode;
/*	if (pEntry) */
	{
		/*
			Change power save mode first because we will call
			RTMPDeQueuePacket() in RtmpHandleRxPsPoll().

			Or when Psm = PWR_ACTIVE, we will not do Aggregation in
			RTMPDeQueuePacket().
		*/
		pEntry->NoDataIdleCount = 0;
		pEntry->PsMode = Psm;

		if ((old_psmode == PWR_ACTIVE) && (Psm == PWR_SAVE))
		{
			DBGPRINT(RT_DEBUG_TRACE,
					("RtmpPsIndicate - %02x:%02x:%02x:%02x:%02x:%02x sleep!\n",
					pAddr[0],pAddr[1],pAddr[2],pAddr[3],pAddr[4],pAddr[5]));
		}

		if ((old_psmode == PWR_SAVE) && (Psm == PWR_ACTIVE))
		{
			/* TODO: For RT2870, how to handle about the BA when STA in PS mode???? */

			DBGPRINT(RT_DEBUG_TRACE,
					("RtmpPsIndicate - %02x:%02x:%02x:%02x:%02x:%02x wakes up, "
					"act like rx PS-POLL\n",
					pAddr[0],pAddr[1],pAddr[2],pAddr[3],pAddr[4],pAddr[5]));

			/* sleep station awakes, move all pending frames from PSQ to TXQ if any */
			RtmpHandleRxPsPoll(pAd, pAddr, pEntry->Aid, TRUE);
		}

		/* move to above section */
/*		pEntry->NoDataIdleCount = 0; */
/*		pEntry->PsMode = Psm; */
	} 
/*	else */
/*	{ */
		/* not in table, try to learn it ???? why bother? */
/*	} */
	return old_psmode;
}



