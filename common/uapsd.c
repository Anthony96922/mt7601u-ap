/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
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

	All related WMM UAPSD function body.

	Two EOSP sent mechanism:
	1. Use DMA Done to do the check.
		a. safe but not accuracy, because maybe all packets are still in hardware
			DMA, not yet be sent to air, but DMA Done are all 1
		b. in USB chip, the data completion does not mean a packet is sent, it
			means a block of data is sent, so we need to use another method to
			know how many packets are sent in the data completion function

	2. Use TX Statistics to do the check.
		a. unsafe but accuracy, because too many traffic are sent before TX Done
			Interrupt occurs and the statistics count will be overwrited.
		b. management frames will not be counted
		c. frames using 1Mbps (CCK) will no be counted
		d. legacy ps frame sent after a PS-Poll frame is also be counted, we
			need to handle the mix mode case: some AC are legacy PS and some AC
			are UAPSD
		e. in USB chip, only one statistics counter for all station entries;
			so only use DMA Done mechanism in USB device; but in PCI chip,
			one statistics counter for each station entries.

	All ACs have two UAPSD modes:
	1. Delivery-enabled
		meaningful mode for AC in AP

	2. Trigger-enabled
		meaningful mode for AC in STA

	Only WMM ACM is used, we need to discriminate between Delivery-enabled and
	Trigger-enabled AC, or all AC are Delivery/Trigger-enabled or not
	Delivery/Trigger-enabled.

***************************************************************************/

#define MODULE_WMM_UAPSD
#include "rt_config.h"

#ifdef UAPSD_SUPPORT
#include "uapsd.h"

/*#define UAPSD_DEBUG */

/* used to enable or disable UAPSD power save queue maintain mechanism */
UCHAR gUAPSD_FlgNotQueueMaintain;

#ifdef UAPSD_DEBUG
UINT32 gUAPSD_SP_CloseAbnormalNum;
#endif /* UAPSD_DEBUG */

#ifdef UAPSD_TIMING_RECORD_FUNC
/* all unit: us */

UCHAR  gUAPSD_TimingFlag;
UINT32 gUAPSD_TimingIndexUapsd;
UINT32 gUAPSD_TimingLoopIndex;

/* ISR start timestamp */
UINT64 gUAPSD_TimingIsr[UAPSD_TIMING_RECORD_MAX];

/* Tasklet start timestamp */
UINT64 gUAPSD_TimingTasklet[UAPSD_TIMING_RECORD_MAX];

UINT64 gUAPSD_TimingTrgRcv[UAPSD_TIMING_RECORD_MAX];
UINT64 gUAPSD_TimingMov2Tx[UAPSD_TIMING_RECORD_MAX];
UINT64 gUAPSD_TimingTx2Air[UAPSD_TIMING_RECORD_MAX];

UINT32 gUAPSD_TimingSumIsr2Tasklet;
UINT32 gUAPSD_TimingSumTrig2Txqueue;
UINT32 gUAPSD_TimingSumTxqueue2Air;
#endif /* UAPSD_TIMING_RECORD_FUNC */


#ifdef VENDOR_FEATURE3_SUPPORT
static VOID UAPSD_InsertTailQueueAc(
	IN	RTMP_ADAPTER	*pAd,
	IN	MAC_TABLE_ENTRY	*pEntry,
	IN	QUEUE_HEADER	*pQueueHeader,
	IN	QUEUE_ENTRY		*pQueueEntry);
#endif /* VENDOR_FEATURE3_SUPPORT */




/*
========================================================================
Routine Description:
    UAPSD Module Init.

Arguments:
	pAd		Pointer to our adapter

Return Value:
    None

Note:
========================================================================
*/
VOID UAPSD_Init(
	IN	PRTMP_ADAPTER		pAd)
{
    /* allocate a lock resource for SMP environment */
	NdisAllocateSpinLock(pAd, &pAd->UAPSDEOSPLock);

#ifdef UAPSD_DEBUG
	DBGPRINT(RT_DEBUG_TRACE, ("uapsd> allocate a spinlock!\n"));
#endif /* UAPSD_DEBUG */

#ifdef UAPSD_DEBUG
	gUAPSD_SP_CloseAbnormalNum = 0;
#endif /* UAPSD_DEBUG */

#ifdef UAPSD_TIMING_RECORD_FUNC
	gUAPSD_TimingFlag = 0; /* default: DISABLE */
	gUAPSD_TimingIndexUapsd = 0;
	gUAPSD_TimingLoopIndex = 0;


	gUAPSD_TimingSumIsr2Tasklet = 0;
	gUAPSD_TimingSumTrig2Txqueue = 0;
	gUAPSD_TimingSumTxqueue2Air = 0;
#endif /* UAPSD_TIMING_RECORD_FUNC */
}


/*
========================================================================
Routine Description:
    UAPSD Module Release.

Arguments:
	pAd		Pointer to our adapter

Return Value:
    None

Note:
========================================================================
*/
VOID UAPSD_Release(
    IN  PRTMP_ADAPTER       pAd)
{
    /* free the lock resource for SMP environment */
    NdisFreeSpinLock(&pAd->UAPSDEOSPLock);

#ifdef UAPSD_DEBUG
	DBGPRINT(RT_DEBUG_TRACE, ("uapsd> release a spinlock!\n"));
#endif /* UAPSD_DEBUG */
} /* End of UAPSD_Release */

/*
========================================================================
Routine Description:
    Check if ASIC can enter sleep mode. Not software sleep.

Arguments:
	pAd		Pointer to our adapter

Return Value:
    None

Note:
========================================================================
*/
VOID RtmpAsicSleepHandle(
    IN  PRTMP_ADAPTER       pAd)
{
	BOOLEAN FlgCanAsicSleep = TRUE;



} /* End of RtmpAsicSleepHandle */



/*
========================================================================
Routine Description:
    Close current Service Period.

Arguments:
	pAd				Pointer to our adapter
	pEntry			Close the SP of the entry

Return Value:
    None

Note:
========================================================================
*/
VOID UAPSD_SP_Close(
    IN  PRTMP_ADAPTER       pAd,
	IN	MAC_TABLE_ENTRY		*pEntry)
{
	if ((pEntry != NULL) && (pEntry->PsMode == PWR_SAVE))
	{
		RTMP_SEM_LOCK(&pAd->UAPSDEOSPLock);

		if (pEntry->bAPSDFlagSPStart != 0)
		{
			/* SP is started for the station */
#ifdef UAPSD_DEBUG
			DBGPRINT(RT_DEBUG_TRACE, ("uapsd> [3] close SP!\n"));
#endif /* UAPSD_DEBUG */

			if (pEntry->pUAPSDEOSPFrame != NULL)
	        {
				/*
					SP will be closed, should not have EOSP frame
					if exists, release it
				*/
#ifdef UAPSD_DEBUG
				DBGPRINT(RT_DEBUG_TRACE, ("uapsd> [3] Free EOSP (UP = %d)!\n",
						RTMP_GET_PACKET_UP(
							QUEUE_ENTRY_TO_PACKET(pEntry->pUAPSDEOSPFrame))));
#endif /* UAPSD_DEBUG */

				RELEASE_NDIS_PACKET(pAd,
									QUEUE_ENTRY_TO_PACKET(pEntry->pUAPSDEOSPFrame),
	                                NDIS_STATUS_FAILURE);
				pEntry->pUAPSDEOSPFrame = NULL;
	        } /* End of if */

			/* re-init SP related parameters */
			pEntry->UAPSDTxNum = 0;
			//pEntry->bAPSDFlagSPStart = 0;
			pEntry->bAPSDFlagEOSPOK = 0;
			pEntry->bAPSDFlagLegacySent = 0;
			UAPSD_SP_END(pAd, pEntry);

#ifdef RTMP_MAC_USB
			pEntry->UAPSDTagOffset[QID_AC_BE] = 0;
			pEntry->UAPSDTagOffset[QID_AC_BK] = 0;
			pEntry->UAPSDTagOffset[QID_AC_VI] = 0;
			pEntry->UAPSDTagOffset[QID_AC_VO] = 0;
#endif /* RTMP_MAC_USB */
	    } /* End of if */

		RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
	} /* End of if */
} /* End of UAPSD_SP_Close */

/*
========================================================================
Routine Description:
	Check if the SP for entry is closed.

Arguments:
	pAd				Pointer to our adapter
	pEntry			the peer entry

Return Value:
	None

Note:
========================================================================
*/
BOOLEAN UAPSD_SP_IsClosed(
    IN  PRTMP_ADAPTER       pAd,
	IN	MAC_TABLE_ENTRY		*pEntry)
{
	BOOLEAN FlgIsSpClosed = TRUE;


	RTMP_SEM_LOCK(&pAd->UAPSDEOSPLock);
	if ((pEntry != NULL) && (pEntry->PsMode == PWR_SAVE))
	{
		if (pEntry->bAPSDFlagSPStart != 0)
			FlgIsSpClosed = FALSE;
	}
	RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);

	return FlgIsSpClosed;
}

/*
========================================================================
Routine Description:
    Deliver all queued packets.

Arguments:
	pAd            Pointer to our adapter
	*pEntry        STATION

Return Value:
    None

Note:
	SMP protection by caller for packet enqueue.
========================================================================
*/
VOID UAPSD_AllPacketDeliver(
	IN	PRTMP_ADAPTER		pAd,
	IN	MAC_TABLE_ENTRY		*pEntry)
{
	QUEUE_HEADER *pQueApsd;
	PQUEUE_ENTRY pQueEntry;
	UCHAR QueIdList[WMM_NUM_OF_AC] = { QID_AC_BE, QID_AC_BK,
                                         QID_AC_VI, QID_AC_VO };
	INT32 IdAc, QueId; /* must be signed, can not be unsigned */


	RTMP_SEM_LOCK(&pAd->UAPSDEOSPLock);

    /* check if the EOSP frame is yet transmitted out */
	if (pEntry->pUAPSDEOSPFrame != NULL)
    {
        /* queue the EOSP frame to SW queue to be transmitted */
		QueId = RTMP_GET_PACKET_UAPSD_QUE_ID(
							QUEUE_ENTRY_TO_PACKET(pEntry->pUAPSDEOSPFrame));

		if (QueId > QID_AC_VO)
        {
            /* should not be here, only for sanity */
			QueId = QID_AC_BE;
        } /* End of if */

		UAPSD_INSERT_QUEUE_AC(pAd, pEntry, &pAd->TxSwQueue[QueId],
						pEntry->pUAPSDEOSPFrame);

		pEntry->pUAPSDEOSPFrame = NULL;
		pEntry->UAPSDTxNum = 0;
    } /* End of if */

    /* deliver ALL U-APSD packets from AC3 to AC0 (AC0 to AC3 is also ok) */
	for(IdAc=(WMM_NUM_OF_AC-1); IdAc>=0; IdAc--)
    {
		pQueApsd = &(pEntry->UAPSDQueue[IdAc]);
		QueId = QueIdList[IdAc];

		while(pQueApsd->Head)
        {
			pQueEntry = RemoveHeadQueue(pQueApsd);
			UAPSD_INSERT_QUEUE_AC(pAd, pEntry, &pAd->TxSwQueue[QueId], pQueEntry);
        } /* End of while */
    } /* End of for */

	RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
} /* End of UAPSD_AllPacketDeliver */


/*
========================================================================
Routine Description:
    Parse the UAPSD field in WMM element in (re)association request frame.

Arguments:
	pAd				Pointer to our adapter
	*pEntry			STATION
	*pElm			QoS information field
	FlgApsdCapable	TRUE: Support UAPSD

Return Value:
    None

Note:
	No protection is needed.

	1. Association -> TSPEC:
		use static UAPSD settings in Association
		update UAPSD settings in TSPEC

	2. Association -> TSPEC(11r) -> Reassociation:
		update UAPSD settings in TSPEC
		backup static UAPSD settings in Reassociation

	3. Association -> Reassociation:
		update UAPSD settings in TSPEC
		backup static UAPSD settings in Reassociation
========================================================================
*/
VOID UAPSD_AssocParse(
	IN	PRTMP_ADAPTER		pAd,
	IN	MAC_TABLE_ENTRY		*pEntry,
	IN	UCHAR				*pElm,
	IN	BOOLEAN				FlgApsdCapable)
{
	PQBSS_STA_INFO_PARM  pQosInfo;
	UCHAR UAPSD[4];
	UINT32 IdApsd;


    /* check if the station enables UAPSD function */
	if ((pEntry) && (FlgApsdCapable == TRUE))
    {
        /* backup its UAPSD parameters */
		pQosInfo = (PQBSS_STA_INFO_PARM) pElm;


		UAPSD[QID_AC_BE] = pQosInfo->UAPSD_AC_BE;
		UAPSD[QID_AC_BK] = pQosInfo->UAPSD_AC_BK;
		UAPSD[QID_AC_VI] = pQosInfo->UAPSD_AC_VI;
		UAPSD[QID_AC_VO] = pQosInfo->UAPSD_AC_VO;

		pEntry->MaxSPLength = pQosInfo->MaxSPLength;

		DBGPRINT(RT_DEBUG_TRACE, ("apsd> UAPSD %d %d %d %d!\n",
					pQosInfo->UAPSD_AC_BE, pQosInfo->UAPSD_AC_BK,
					pQosInfo->UAPSD_AC_VI, pQosInfo->UAPSD_AC_VO));
		DBGPRINT(RT_DEBUG_TRACE,
					("apsd> MaxSPLength = %d\n", pEntry->MaxSPLength));

		/* use static UAPSD setting of association request frame */
		for(IdApsd=0; IdApsd<4; IdApsd++)
		{
			pEntry->bAPSDCapablePerAC[IdApsd] = UAPSD[IdApsd];
			pEntry->bAPSDDeliverEnabledPerAC[IdApsd] = UAPSD[IdApsd];

		} /* End of for */

		if ((pEntry->bAPSDCapablePerAC[QID_AC_BE] == 0) &&
			(pEntry->bAPSDCapablePerAC[QID_AC_BK] == 0) &&
			(pEntry->bAPSDCapablePerAC[QID_AC_VI] == 0) &&
			(pEntry->bAPSDCapablePerAC[QID_AC_VO] == 0))
        {
			CLIENT_STATUS_CLEAR_FLAG(pEntry, fCLIENT_STATUS_APSD_CAPABLE);
        }
        else
        {
			CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_APSD_CAPABLE);
        } /* End of if */

		if ((pEntry->bAPSDCapablePerAC[QID_AC_BE] == 1) &&
			(pEntry->bAPSDCapablePerAC[QID_AC_BK] == 1) &&
			(pEntry->bAPSDCapablePerAC[QID_AC_VI] == 1) &&
			(pEntry->bAPSDCapablePerAC[QID_AC_VO] == 1))
        {
            /* all AC are U-APSD */
			DBGPRINT(RT_DEBUG_TRACE, ("apsd> all AC are UAPSD\n"));
			pEntry->bAPSDAllAC = 1;
        }
        else
        {
            /* at least one AC is not U-APSD */
			DBGPRINT(RT_DEBUG_TRACE, ("apsd> at least one AC is not UAPSD %d %d %d %d\n",
					pEntry->bAPSDCapablePerAC[QID_AC_BE],
					pEntry->bAPSDCapablePerAC[QID_AC_BK],
					pEntry->bAPSDCapablePerAC[QID_AC_VI],
					pEntry->bAPSDCapablePerAC[QID_AC_VO]));
			pEntry->bAPSDAllAC = 0;
        } /* End of if */

		pEntry->VirtualTimeout = 0;

		DBGPRINT(RT_DEBUG_TRACE, ("apsd> MaxSPLength = %d\n", pEntry->MaxSPLength));
    } /* End of if */
} /* End of UAPSD_AssocParse */


/*
========================================================================
Routine Description:
    Enqueue a UAPSD packet.

Arguments:
	pAd				Pointer to our adapter
	*pEntry			STATION
	pPacket			UAPSD dnlink packet
	IdAc			UAPSD AC ID (0 ~ 3)

Return Value:
    None

Note:
========================================================================
*/
VOID UAPSD_PacketEnqueue(
	IN	PRTMP_ADAPTER		pAd,
	IN	MAC_TABLE_ENTRY		*pEntry,
	IN	PNDIS_PACKET		pPacket,
	IN	UINT32				IdAc)
{
	/*
		1. the STATION is UAPSD STATION;
		2. AC ID is legal;
		3. the AC is UAPSD AC.
		so we queue the packet to its UAPSD queue
	*/

	/* [0] ~ [3], QueIdx base is QID_AC_BE */
	QUEUE_HEADER *pQueUapsd;


	/* check if current queued UAPSD packet number is too much */
	if (pEntry == NULL)
	{
		RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_FAILURE);
		DBGPRINT(RT_DEBUG_TRACE, ("uapsd> pEntry == NULL!\n"));
		return;
	} /* End of if */

	pQueUapsd = &(pEntry->UAPSDQueue[IdAc]);

	if (pQueUapsd->Number >= MAX_PACKETS_IN_UAPSD_QUEUE)
    	{
	        /* too much queued pkts, free (discard) the tx packet */
	        DBGPRINT(RT_DEBUG_TRACE,
                 ("uapsd> many(%ld) WCID(%d) AC(%d)\n",
				pQueUapsd->Number,
				RTMP_GET_PACKET_WCID(pPacket),
				IdAc));

		RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_FAILURE);
	}
	else
	{
        	/* queue the tx packet to the U-APSD queue of the AC */
		RTMP_SEM_LOCK(&pAd->UAPSDEOSPLock);
		InsertTailQueue(pQueUapsd, PACKET_TO_QUEUE_ENTRY(pPacket));
		RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);

#ifdef UAPSD_DEBUG
		if (RTMP_GET_PACKET_MGMT_PKT(pPacket) == 1)
		{
			DBGPRINT(RT_DEBUG_TRACE, ("ps> mgmt to uapsd queue...\n"));
		}
		else
		{
			DBGPRINT(RT_DEBUG_TRACE,
					("ps> data (0x%08lx) (AC%d) to uapsd queue (num of pkt = %ld)...\n",
					(ULONG)pPacket, IdAc,
					pQueUapsd->Number));
		} /* End of if */
#endif /* UAPSD_DEBUG */
	} /* End of if */
} /* End of UAPSD_PacketEnqueue */



/*
========================================================================
Routine Description:
    Maintenance our UAPSD PS queue.  Release all queued packet if timeout.

Arguments:
	pAd				Pointer to our adapter
	*pEntry			STATION

Return Value:
    None

Note:
	If in RT2870, pEntry can not be removed during UAPSD_QueueMaintenance()
========================================================================
*/
VOID UAPSD_QueueMaintenance(
	IN	PRTMP_ADAPTER		pAd,
	IN	MAC_TABLE_ENTRY		*pEntry)
{
	QUEUE_HEADER *pQue;
	UINT32 IdAc;
	BOOLEAN FlgUapsdPkt, FlgEospPkt;
#ifdef RTMP_MAC_USB
	ULONG IrqFlags;
#endif /* RTMP_MAC_USB */


	/* sanity check */
	if (gUAPSD_FlgNotQueueMaintain)
		return;
	/* End of if */

	if (pEntry->PsMode != PWR_SAVE)
		return; /* UAPSD packet only for power-save STA, not active STA */
	/* End of if */

    /* init */
	RTMP_SEM_LOCK(&pAd->UAPSDEOSPLock);

	pQue = pEntry->UAPSDQueue;
	FlgUapsdPkt = 0;
	FlgEospPkt = 0;

    /* check if more than one U-APSD packets exists */
	for(IdAc=0; IdAc<WMM_NUM_OF_AC; IdAc++)
    {
		if (pQue[IdAc].Head != NULL)
        {
			/*
				At least one U-APSD packets exists so we need to check if
				queued U-APSD packets are timeout.
			*/
			FlgUapsdPkt = 1;
            break;
        } /* End of if */
    } /* End of for */

	if (pEntry->pUAPSDEOSPFrame != NULL)
		FlgEospPkt = 1;
	/* End of if */

    /* check if any queued UAPSD packet exists */
	if (FlgUapsdPkt || FlgEospPkt)
    {
#ifdef RTMP_MAC_USB
		RTMP_IRQ_LOCK(&pAd->irq_lock, IrqFlags);
#endif /* RTMP_MAC_USB */

		pEntry->UAPSDQIdleCount ++;

		if (pEntry->UAPSDQIdleCount > pAd->MacTab.MsduLifeTime)
        {
			if (FlgUapsdPkt)
			{
				DBGPRINT(RT_DEBUG_TRACE,
						("uapsd> UAPSD queue timeout! clean all queued frames...\n"));
			} /* End of if */

			if (FlgEospPkt)
			{
				DBGPRINT(RT_DEBUG_TRACE,
						("uapsd> UAPSD EOSP timeout! clean the EOSP frame!\n"));
			} /* End of if */

            /* UAPSDQIdleCount will be 0 after trigger frame is received */

            /* clear all U-APSD packets */
			if (FlgUapsdPkt)
			{
				for(IdAc=0; IdAc<WMM_NUM_OF_AC; IdAc++)
					RtmpCleanupPsQueue(pAd, &pQue[IdAc]);
	            /* End of for */
			} /* End of if */

            /* free the EOSP frame */
			pEntry->UAPSDTxNum = 0;

			if (pEntry->pUAPSDEOSPFrame != NULL)
            {
				RELEASE_NDIS_PACKET(pAd,
								QUEUE_ENTRY_TO_PACKET(pEntry->pUAPSDEOSPFrame),
                                    NDIS_STATUS_FAILURE);
				pEntry->pUAPSDEOSPFrame = NULL;
            } /* End of if */

			pEntry->bAPSDFlagEOSPOK = 0;
			//pEntry->bAPSDFlagSPStart = 0;
			pEntry->bAPSDFlagLegacySent = 0;
			UAPSD_SP_END(pAd, pEntry);

            /* clear idle counter */
			pEntry->UAPSDQIdleCount = 0;

#ifdef CONFIG_AP_SUPPORT
			/* check TIM bit */
			if (pEntry->PsQueue.Number == 0)
			{
				WLAN_MR_TIM_BIT_CLEAR(pAd, pEntry->apidx, pEntry->Aid);
			} /* End of if */
#endif /* CONFIG_AP_SUPPORT */
        } /* End of if */

#ifdef RTMP_MAC_USB
		RTMP_IRQ_UNLOCK(&pAd->irq_lock, IrqFlags);
#endif /* RTMP_MAC_USB */
    }
    else
	{
		/* clear idle counter */
		pEntry->UAPSDQIdleCount = 0;
	} /* End of if (FlgUapsdPkt) */

	RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);

	/* virtual timeout handle */
	RTMP_PS_VIRTUAL_TIMEOUT_HANDLE(pEntry);
} /* End of UAPSD_QueueMaintenance */


/*
========================================================================
Routine Description:
    Close SP in Tx Done, not Tx DMA Done.

Arguments:
	pAd            Pointer to our adapter
	pEntry			destination entry
	FlgSuccess		0:tx success, 1:tx fail

Return Value:
    None

Note:
	For RT28xx series, for packetID=0 or multicast frame, no statistics
	count can be got, ex: ARP response or DHCP packets, we will use
	low rate to set (CCK, MCS=0=packetID).
	So SP will not be close until UAPSD_EPT_SP_INT timeout.

	So if the tx rate is 1Mbps for a entry, we will use DMA done, not
	use UAPSD_SP_AUE_Handle().
========================================================================
*/
VOID UAPSD_SP_AUE_Handle(
	IN RTMP_ADAPTER		*pAd,
    IN MAC_TABLE_ENTRY	*pEntry,
	IN UCHAR			FlgSuccess)
{
#ifdef UAPSD_SP_ACCURATE
	USHORT QueId;


	if (pEntry == NULL)
		return;
	/* End of if */

	if (pEntry->PsMode == PWR_ACTIVE)
	{
#ifdef UAPSD_DEBUG
		DBGPRINT(RT_DEBUG_TRACE, ("uapsd> aux: Station actives! Close SP!\n"));
#endif /* UAPSD_DEBUG */
		//pEntry->bAPSDFlagSPStart = 0;
		pEntry->bAPSDFlagEOSPOK = 0;
		UAPSD_SP_END(pAd, pEntry);
		return;
	} /* End of if */

	if (pEntry->PsMode == PWR_SAVE)
	{
		BOOLEAN FlgEosp;


		RTMP_SEM_LOCK(&pAd->UAPSDEOSPLock);

		if (pEntry->bAPSDFlagSpRoughUse != 0)
		{
			RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
			return; /* use DMA mechanism, not statistics count mechanism */
		} /* End of if */

#ifdef UAPSD_DEBUG
		DBGPRINT(RT_DEBUG_TRACE, ("uapsd> aux: Tx Num = %d\n", pEntry->UAPSDTxNum));
#endif /* UAPSD_DEBUG */

		FlgEosp = FALSE;

		if (pEntry->bAPSDFlagSPStart == 0)
		{
			/*
				When SP is not started, all packets are from legacy PS queue.
				One downlink packet for one PS-Poll packet.
			*/
			pEntry->bAPSDFlagLegacySent = 0;

#ifdef UAPSD_DEBUG
			DBGPRINT(RT_DEBUG_TRACE, ("uapsd> legacy PS packet is sent!\n"));
#endif /* UAPSD_DEBUG */
		}
		else
		{
#ifdef UAPSD_TIMING_RECORD_FUNC
			UAPSD_TIMING_RECORD(pAd, UAPSD_TIMING_RECORD_TX2AIR);
#endif /* UAPSD_TIMING_RECORD_FUNC */
		} /* End of if */

		/* record current time */
		UAPSD_TIME_GET(pAd, pEntry->UAPSDTimeStampLast);

        /* Note: UAPSDTxNum does NOT include the EOSP packet */
		if (pEntry->UAPSDTxNum > 0)
        {
            /* some UAPSD packets are not yet transmitted */

			if (pEntry->UAPSDTxNum == 1)
            {
                /* this is the last UAPSD packet */
				if (pEntry->pUAPSDEOSPFrame != NULL)
                {
                    /* transmit the EOSP frame */
					PNDIS_PACKET pPkt;


#ifdef UAPSD_DEBUG
					DBGPRINT(RT_DEBUG_TRACE, ("uapsd> aux: send EOSP frame...\n"));
#endif /* UAPSD_DEBUG */

					pPkt = QUEUE_ENTRY_TO_PACKET(pEntry->pUAPSDEOSPFrame);
					QueId = RTMP_GET_PACKET_UAPSD_QUE_ID(pPkt);

					if (QueId > QID_AC_VO)
                    {
                        /* should not be here, only for sanity */
						QueId = QID_AC_BE;
                    } /* End of if */

					UAPSD_INSERT_QUEUE_AC(pAd, pEntry, &pAd->TxSwQueue[QueId],
									pEntry->pUAPSDEOSPFrame);

					pEntry->pUAPSDEOSPFrame = NULL;
					FlgEosp = TRUE;
                } /* End of if */
            } /* End of if */

            /* a UAPSD frame is transmitted so decrease the counter */
			pEntry->UAPSDTxNum --;

			RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);

			/* maybe transmit the EOSP frame */
			if (FlgEosp == TRUE)
			{
				POS_COOKIE pCookie;

				pCookie = (POS_COOKIE) pAd->OS_Cookie;

				/*
					Too many functions call NICUpdateFifoStaCounters() and
					NICUpdateFifoStaCounters() will call UAPSD_SP_AUE_Handle(),
					if we call RTMPDeQueuePacket() here, double-IRQ LOCK will
					occur. so we need to activate a tasklet to send EOSP frame.

					ex: RTMPDeQueuePacket() --> RTMPFreeTXDUponTxDmaDone() -->
						NICUpdateFifoStaCounters() --> UAPSD_SP_AUE_Handle() -->
						RTMPDeQueuePacket() ERROR! or

						RTMPHandleTxRingDmaDoneInterrupt() -->
						RTMP_IRQ_LOCK() -->
						RTMPFreeTXDUponTxDmaDone() -->
						NICUpdateFifoStaCounters() -->
						UAPSD_SP_AUE_Handle() -->
						RTMPDeQueuePacket() -->
						DEQUEUE_LOCK() -->
						RTMP_IRQ_LOCK() ERROR!
						*/
				RTMP_OS_TASKLET_SCHE(&pCookie->uapsd_eosp_sent_task);
			}
			/* must return here; Or double unlock UAPSDEOSPLock */
			return;
        }
        else
        {
            /* UAPSDTxNum == 0 so the packet is the EOSP packet */

			if (pAd->bAPSDFlagSPSuspend == 1)
			{
#ifdef UAPSD_DEBUG
				DBGPRINT(RT_DEBUG_TRACE, ("uapsd> aux: SP is suspend, keep SP if exists!\n"));
#endif /* UAPSD_DEBUG */

				/* keep SP, not to close SP */
				pEntry->bAPSDFlagEOSPOK = 1;
			}

			if ((pEntry->bAPSDFlagSPStart != 0) &&
				(pAd->bAPSDFlagSPSuspend == 0))
            {
				//pEntry->bAPSDFlagSPStart = 0;
				pEntry->bAPSDFlagEOSPOK = 0;
				UAPSD_SP_END(pAd, pEntry);

#ifdef UAPSD_DEBUG
				DBGPRINT(RT_DEBUG_TRACE, ("uapsd> aux: close a SP.\n\n\n"));
#endif /* UAPSD_DEBUG */
            }
        } 

		RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
	}
#endif /* UAPSD_SP_ACCURATE */
}



/*
========================================================================
Routine Description:
    Close current Service Period.

Arguments:
	pAd				Pointer to our adapter

Return Value:
    None

Note:
    When we receive EOSP frame tx done interrupt and a uplink packet
    from the station simultaneously, we will regard it as a new trigger
    frame because the packet is received when EOSP frame tx done interrupt.

    We can not sure the uplink packet is sent after old SP or in the old SP.
    So we must close the old SP in receive done ISR to avoid the problem.
========================================================================
*/
VOID UAPSD_SP_CloseInRVDone(
	IN	PRTMP_ADAPTER		pAd)
{
	UINT32 IdEntry;
	int		FirstWcid = 0;


	if (pAd->MacTab.fAnyStationInPsm == FALSE)
		return; /* no any station is in power save mode */
	/* End of if */


    /* check for all CLIENT's UAPSD Service Period */
	for(IdEntry = FirstWcid; IdEntry < MAX_LEN_OF_MAC_TABLE; IdEntry++)
    {
		MAC_TABLE_ENTRY *pEntry = &pAd->MacTab.Content[IdEntry];


		RTMP_SEM_LOCK(&pAd->UAPSDEOSPLock);

        /* check if SP is started and EOSP is transmitted ok */
		if ((pEntry->bAPSDFlagSPStart != 0) &&
			(pEntry->bAPSDFlagEOSPOK != 0))
        {
			/*
				1. SP is started;
				2. EOSP frame is sent ok.
			*/

			/*
				We close current SP for the STATION so we can receive new
				trigger frame from the STATION again.
			*/

#ifdef UAPSD_DEBUG
			DBGPRINT(RT_DEBUG_TRACE,("uapsd> close SP in UAPSD_SP_CloseInRVDone()!\n\n\n"));
#endif /* UAPSD_DEBUG */

			//pEntry->bAPSDFlagSPStart = 0;
			pEntry->bAPSDFlagEOSPOK = 0;
			pEntry->bAPSDFlagLegacySent = 0;
			UAPSD_SP_END(pAd, pEntry);
        } /* End of if */

		RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
    } /* End of for */
} /* End of UAPSD_SP_CloseInRVDone */




#ifdef UAPSD_TIMING_RECORD_FUNC
/*
========================================================================
Routine Description:
	Enable/Disable Timing Record Function.

Arguments:
	pAd				Pointer to our adapter
	Flag			1 (Enable) or 0 (Disable)

Return Value:
	None

Note:
========================================================================
*/
VOID UAPSD_TimingRecordCtrl(
	IN	UINT32				Flag)
{
	if (gUAPSD_TimingFlag == UAPSD_TIMING_CTRL_SUSPEND)
		return;
	/* End of if */

	gUAPSD_TimingFlag = Flag;
} /* End of UAPSD_TimingRecordCtrl */


/*
========================================================================
Routine Description:
	Record some timings.

Arguments:
	pAd				Pointer to our adapter
	Type			The timing is for what type

Return Value:
	None

Note:
	UAPSD_TIMING_RECORD_ISR
	UAPSD_TIMING_RECORD_TASKLET
	UAPSD_TIMING_RECORD_TRG_RCV
	UAPSD_TIMING_RECORD_MOVE2TX
	UAPSD_TIMING_RECORD_TX2AIR
========================================================================
*/
VOID UAPSD_TimingRecord(
	IN	PRTMP_ADAPTER		pAd,
	IN	UINT32				Type)
{
	UINT32 Index;

	if (gUAPSD_TimingFlag == UAPSD_TIMING_CTRL_STOP)
		return;
	/* End of if */

	if ((gUAPSD_TimingFlag == UAPSD_TIMING_CTRL_SUSPEND) &&
		(Type != UAPSD_TIMING_RECORD_TX2AIR))
	{
		return;
	} /* End of if */

	Index = gUAPSD_TimingIndexUapsd;

	switch(Type)
	{
		case UAPSD_TIMING_RECORD_ISR:
			/* start to record the timing */
			UAPSD_TIMESTAMP_GET(pAd, gUAPSD_TimingIsr[Index]);
			break;

		case UAPSD_TIMING_RECORD_TASKLET:
			UAPSD_TIMESTAMP_GET(pAd, gUAPSD_TimingTasklet[Index]);
			break;

		case UAPSD_TIMING_RECORD_TRG_RCV:
			if (gUAPSD_TimingLoopIndex == 0)
			{
				/*
					The trigger frame is the first received frame.
					The received time will be the time recorded in ISR.
				*/
				gUAPSD_TimingTrgRcv[Index] = gUAPSD_TimingIsr[Index];
			}
			else
			{
				/*
					Some packets are handled before the trigger frame so
					we record next one.
				*/
				UAPSD_TIMING_RECORD_STOP();
			} /* End of if */
			break;

		case UAPSD_TIMING_RECORD_MOVE2TX:
			UAPSD_TIMESTAMP_GET(pAd, gUAPSD_TimingMov2Tx[Index]);

			/* prepare to wait for tx done */
			UAPSD_TimingRecordCtrl(UAPSD_TIMING_CTRL_SUSPEND);
			break;

		case UAPSD_TIMING_RECORD_TX2AIR:
			UAPSD_TIMESTAMP_GET(pAd, gUAPSD_TimingTx2Air[Index]);

			/* sum the delay */
			gUAPSD_TimingSumIsr2Tasklet += \
				(UINT32)(gUAPSD_TimingTasklet[Index] - gUAPSD_TimingIsr[Index]);
			gUAPSD_TimingSumTrig2Txqueue += \
				(UINT32)(gUAPSD_TimingMov2Tx[Index] - gUAPSD_TimingTrgRcv[Index]);
			gUAPSD_TimingSumTxqueue2Air += \
				(UINT32)(gUAPSD_TimingTx2Air[Index] - gUAPSD_TimingMov2Tx[Index]);

			/* display average delay */
			if ((Index % UAPSD_TIMING_RECORD_DISPLAY_TIMES) == 0)
			{
				DBGPRINT(RT_DEBUG_TRACE, ("uapsd> Isr2Tasklet=%d, Trig2Queue=%d, Queue2Air=%d micro seconds\n",
						gUAPSD_TimingSumIsr2Tasklet/
											UAPSD_TIMING_RECORD_DISPLAY_TIMES,
						gUAPSD_TimingSumTrig2Txqueue/
											UAPSD_TIMING_RECORD_DISPLAY_TIMES,
						gUAPSD_TimingSumTxqueue2Air/
											UAPSD_TIMING_RECORD_DISPLAY_TIMES));
				gUAPSD_TimingSumIsr2Tasklet = 0;
				gUAPSD_TimingSumTrig2Txqueue = 0;
				gUAPSD_TimingSumTxqueue2Air = 0;
			} /* End of if */

			/* ok, a record is finished; prepare to record the next one */
			gUAPSD_TimingIndexUapsd ++;

			if (gUAPSD_TimingIndexUapsd >= UAPSD_TIMING_RECORD_MAX)
				gUAPSD_TimingIndexUapsd = 0;
			/* End of if */

			/* stop the record */
			gUAPSD_TimingFlag = UAPSD_TIMING_CTRL_STOP;

			DBGPRINT(RT_DEBUG_TRACE, ("sam> Isr->Tasklet:%d, Trig->TxQueue:%d, TxQueue->TxDone:%d\n",
				(UINT32)(gUAPSD_TimingTasklet[Index] - gUAPSD_TimingIsr[Index]),
				(UINT32)(gUAPSD_TimingMov2Tx[Index] - gUAPSD_TimingTrgRcv[Index]),
				(UINT32)(gUAPSD_TimingTx2Air[Index] - gUAPSD_TimingMov2Tx[Index])));
			break;
	} /* End of switch */
} /* End of UAPSD_TimingRecord */


/*
========================================================================
Routine Description:
	Record the loop index for received packet handle.

Arguments:
	pAd				Pointer to our adapter
	LoopIndex		The RxProcessed in APRxDoneInterruptHandle()

Return Value:
	None

Note:
========================================================================
*/
VOID UAPSD_TimeingRecordLoopIndex(
	IN	UINT32				LoopIndex)
{
	gUAPSD_TimingLoopIndex = LoopIndex;
} /* End of UAPSD_TimeingRecordLoopIndex */

#endif /* UAPSD_TIMING_RECORD_FUNC */


/*
========================================================================
Routine Description:
    Handle PS-Poll Frame.

Arguments:
	pAd				Pointer to our adapter
	*pEntry			the source STATION

Return Value:
    TRUE			Handle OK
	FALSE			Handle FAIL

Note:
========================================================================
*/
BOOLEAN UAPSD_PsPollHandle(
	IN	PRTMP_ADAPTER		pAd,
	IN	MAC_TABLE_ENTRY		*pEntry)
{
	QUEUE_HEADER	*pAcPsQue;
	QUEUE_HEADER	*pAcSwQue;
	PQUEUE_ENTRY	pQuedEntry;
	PNDIS_PACKET	pQuedPkt;
	UINT32	AcQueId;
    /*
		AC ID          = VO > VI > BK > BE
		so we need to change BE & BK
		=> AC priority = VO > VI > BE > BK
	*/
	UINT32	AcPriority[WMM_NUM_OF_AC] = { 1, 0, 2, 3 };
	UCHAR	QueIdList[WMM_NUM_OF_AC] = { QID_AC_BE, QID_AC_BK,
                                            QID_AC_VI, QID_AC_VO };
	BOOLEAN	FlgQueEmpty;
	INT32	IdAc; /* must be signed, can not use unsigned */
	UINT32	Aid, QueId;


	/* sanity check */
	if (pEntry == NULL)
		return FALSE; /* fatal error */
	/* End of if */

	/* init */
	FlgQueEmpty = TRUE;
	pAcSwQue = NULL;
	pQuedPkt = NULL;

	/* sanity check */
	RTMP_SEM_LOCK(&pAd->UAPSDEOSPLock);

	if (pEntry->bAPSDAllAC == 0)
	{
		RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
		return FALSE; /* not all AC are delivery-enabled */
	} /* End of if */

	if (pEntry->bAPSDFlagSPStart != 0)
	{
		RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
		return FALSE; /* its service period is not yet ended */
	} /* End of if */

	/* from highest priority AC3 --> AC2 --> AC0 --> lowest priority AC1 */
	for (IdAc=(WMM_NUM_OF_AC-1); IdAc>=0; IdAc--)
	{
		AcQueId = AcPriority[IdAc];

		/*
			NOTE: get U-APSD queue pointer here to speed up, do NOT use
			pEntry->UAPSDQueue[AcQueId] throughout codes because
			compiler will compile it to many assembly codes.
		*/
		pAcPsQue = &pEntry->UAPSDQueue[AcQueId];

		/* check if any U-APSD packet is queued for the AC */
		if (pAcPsQue->Head == NULL)
			continue;
		/* End of if */

		/* at least one U-APSD packet exists here */

		/* put U-APSD packets to the AC software queue */
		if ((pAcPsQue->Head != NULL) && (pQuedPkt == NULL))
		{
			/* get AC software queue */
			QueId = QueIdList[AcQueId];
			pAcSwQue = &pAd->TxSwQueue[QueId];

			/* get the U-APSD packet */
			pQuedEntry = RemoveHeadQueue(pAcPsQue);
			pQuedPkt = QUEUE_ENTRY_TO_PACKET(pQuedEntry);

			if (pQuedPkt != NULL)
			{
				/*
					WMM Specification V1.1 3.6.1.7
                       The More Data bit (b13) of the directed MSDU or MMPDU
                       associated with delivery-enabled ACs and destined for
                       that WMM STA indicates that more frames are buffered for
					the delivery-enabled ACs.
				*/
				RTMP_SET_PACKET_MOREDATA(pQuedPkt, TRUE);

				/* set U-APSD flag & its software queue ID */
				RTMP_SET_PACKET_UAPSD(pQuedPkt, TRUE, QueId);
			} /* End of if */
		} /* End of while */

		if (pAcPsQue->Head != NULL)
		{
			/* still have packets in queue */
			FlgQueEmpty = FALSE;
			break;
		} /* End of if */
	} /* End of for */

	if (pQuedPkt != NULL)
	{
		if (FlgQueEmpty == TRUE)
		{
			/*
				No any more queued U-APSD packet so clear More Data bit of
				the last frame.
			*/
			RTMP_SET_PACKET_MOREDATA(pQuedPkt, FALSE);
		} /* End of if */

		UAPSD_INSERT_QUEUE_AC(pAd, pEntry, pAcSwQue, pQuedPkt);
	} /* End of if */

	/* clear corresponding TIM bit */

	/* get its AID for the station */
	Aid = pEntry->Aid;

	if ((pEntry->bAPSDAllAC == 1) && (FlgQueEmpty == TRUE))
	{
		/* all AC are U-APSD and no any U-APSD packet is queued, set TIM */

#ifdef CONFIG_AP_SUPPORT
		/* clear TIM bit */
		if ((Aid > 0) && (Aid < MAX_LEN_OF_MAC_TABLE))
		{
			WLAN_MR_TIM_BIT_CLEAR(pAd, pEntry->apidx, Aid);
		} /* End of if */
#endif /* CONFIG_AP_SUPPORT */
	} /* End of if */

	/* reset idle timeout here whenever a trigger frame is received */
	pEntry->UAPSDQIdleCount = 0;

	RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);

	/* Dequeue outgoing frames from TxSwQueue0..3 queue and process it */
	RTMPDeQueuePacket(pAd, FALSE, NUM_OF_TX_RING, MAX_TX_PROCESS);
	return TRUE;
}

/*
========================================================================
Routine Description:
    Get the queue status for delivery-enabled AC.

Arguments:
	pAd					Pointer to our adapter
	pEntry				the peer entry
	pFlgIsAnyPktForBK	TRUE: At lease a BK packet is queued
	pFlgIsAnyPktForBE	TRUE: At lease a BE packet is queued
	pFlgIsAnyPktForVI	TRUE: At lease a VI packet is queued
	pFlgIsAnyPktForVO	TRUE: At lease a VO packet is queued

Return Value:
    None

Note:
========================================================================
*/
VOID UAPSD_QueueStatusGet(
	IN	PRTMP_ADAPTER		pAd,
	IN	MAC_TABLE_ENTRY		*pEntry,
	OUT	BOOLEAN				*pFlgIsAnyPktForBK,
	OUT BOOLEAN				*pFlgIsAnyPktForBE,
	OUT BOOLEAN				*pFlgIsAnyPktForVI,
	OUT BOOLEAN				*pFlgIsAnyPktForVO)
{
	/* init */
	*pFlgIsAnyPktForBK = FALSE;
	*pFlgIsAnyPktForBE = FALSE;
	*pFlgIsAnyPktForVI = FALSE;
	*pFlgIsAnyPktForVO = FALSE;

	/* sanity check */
	if (pEntry == NULL)
		return;

	/* get queue status */
	RTMP_SEM_LOCK(&pAd->UAPSDEOSPLock);
	if (pEntry->UAPSDQueue[QID_AC_BK].Head != NULL)
		*pFlgIsAnyPktForBK = TRUE;
	if (pEntry->UAPSDQueue[QID_AC_BE].Head != NULL)
		*pFlgIsAnyPktForBE = TRUE;
	if (pEntry->UAPSDQueue[QID_AC_VI].Head != NULL)
		*pFlgIsAnyPktForVI = TRUE;
	if (pEntry->UAPSDQueue[QID_AC_VO].Head != NULL)
		*pFlgIsAnyPktForVO = TRUE;
	RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
}

/*
========================================================================
Routine Description:
    Handle UAPSD Trigger Frame.

Arguments:
	pAd				Pointer to our adapter
	*pEntry			the source STATION
	UpOfFrame		the UP of the trigger frame

Return Value:
    None

Note:
========================================================================
*/
VOID UAPSD_TriggerFrameHandle(
	IN	PRTMP_ADAPTER		pAd,
	IN	MAC_TABLE_ENTRY		*pEntry,
	IN	UCHAR				UpOfFrame)
{
	QUEUE_HEADER	*pAcPsQue;
	QUEUE_HEADER	*pAcSwQue, *pLastAcSwQue;
	PQUEUE_ENTRY	pQuedEntry;
	PNDIS_PACKET	pQuedPkt;

	UINT32	AcQueId;
	UINT32	TxPktNum, SpMaxLen;
    /*
		AC ID          = VO > VI > BK > BE
		so we need to change BE & BK
		=> AC priority = VO > VI > BE > BK
	*/
	UINT32	AcPriority[WMM_NUM_OF_AC] = { 1, 0, 2, 3 };
	/* 0: deliver all U-APSD packets */
	UINT32	SpLenMap[WMM_NUM_OF_AC] = { 0, 2, 4, 6 };
	UCHAR	QueIdList[WMM_NUM_OF_AC] = { QID_AC_BE, QID_AC_BK,
                                            QID_AC_VI, QID_AC_VO };
	BOOLEAN	FlgQueEmpty;
	BOOLEAN	FlgNullSnd;
	BOOLEAN	FlgMgmtFrame;
	UINT32	Aid, QueId;
	INT32	IdAc; /* must be signed, can not use unsigned */
/*	ULONG    FlgIrq; */

#ifdef UAPSD_SP_ACCURATE
	ULONG	TimeNow;
#endif /* UAPSD_SP_ACCURATE */


	/* sanity check for Service Period of the STATION */
	RTMP_SEM_LOCK(&pAd->UAPSDEOSPLock);

#ifdef UAPSD_DEBUG
	DBGPRINT(RT_DEBUG_TRACE, ("\nuapsd> bAPSDFlagLegacySent = %d!\n",
			pEntry->bAPSDFlagLegacySent));
#endif /* UAPSD_DEBUG */

	if (pEntry->bAPSDFlagSPStart != 0)
	{
		/*
			reset ContinueTxFailCnt
		*/
		pEntry->ContinueTxFailCnt = 0;

		/*
			WMM Specification V1.1 3.6.1.5
               A Trigger Frame received by the WMM AP from a WMM STA that
               already has an USP underway shall not trigger the start of a new
			USP.
		*/

		/*
			Current SP for the STATION is not yet ended so the packet is
			normal DATA packet.
		*/

#ifdef UAPSD_DEBUG
		DBGPRINT(RT_DEBUG_TRACE, ("uapsd> sorry! SP is not yet closed!\n"));
#endif /* UAPSD_DEBUG */

#ifdef UAPSD_SP_ACCURATE
		/*
			The interval between the data frame from QSTA and last confirmed
			packet from QAP in UAPSD_SP_AUE_Handle() is too large so maybe
			we suffer the worse case.

			Currently, if we send any packet with 1Mbps in 2.4GHz and 6Mbps
			in 5GHz, no any statistics count for the packet so the SP can
			not be closed.
		*/
		UAPSD_TIME_GET(pAd, TimeNow);

		if ((TimeNow - pEntry->UAPSDTimeStampLast) >= UAPSD_EPT_SP_INT)
		{
#ifdef UAPSD_DEBUG
			DBGPRINT(RT_DEBUG_TRACE, ("uapsd> SP period is too large so SP is closed first!"
						" (%lu %lu %lu)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n",
						TimeNow, pEntry->UAPSDTimeStampLast,
						(TimeNow - pEntry->UAPSDTimeStampLast)));

			gUAPSD_SP_CloseAbnormalNum ++;
#endif /* UAPSD_DEBUG */

			RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
			UAPSD_SP_Close(pAd, pEntry);
			RTMP_SEM_LOCK(&pAd->UAPSDEOSPLock);
		}
		else
		{
#endif /* UAPSD_SP_ACCURATE */

			RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
            return;

#ifdef UAPSD_SP_ACCURATE
		}
#endif /* UAPSD_SP_ACCURATE */
	}

#ifdef UAPSD_TIMING_RECORD_FUNC
	UAPSD_TIMING_RECORD(pAd, UAPSD_TIMING_RECORD_TRG_RCV);
#endif /* UAPSD_TIMING_RECORD_FUNC */

#ifdef UAPSD_DEBUG
	if (pEntry->pUAPSDEOSPFrame != NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("uapsd> EOSP is not NULL!\n"));
		RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
		return;
	}
#endif /* UAPSD_DEBUG */

	if (pEntry->MaxSPLength >= 4)
	{
		/* fatal error, should be 0 ~ 3 so reset it to 0 */
		DBGPRINT(RT_DEBUG_TRACE,
					("uapsd> MaxSPLength >= 4 (%d)!\n", pEntry->MaxSPLength));
		pEntry->MaxSPLength = 0;
	}


#ifdef UAPSD_SP_ACCURATE
	/* mark the start time for the SP */
	UAPSD_TIME_GET(pAd, pEntry->UAPSDTimeStampLast);


	/* check if current rate of the entry is 1Mbps (2.4GHz) or 6Mbps (5GHz) */

#ifdef RTMP_MAC_USB
	/* always use rough mechanism */
	pEntry->bAPSDFlagSpRoughUse = 1;
#endif /* RTMP_MAC_USB */
#else

	pEntry->bAPSDFlagSpRoughUse = 1;
#endif /* UAPSD_SP_ACCURATE */


	/* sanity Check for UAPSD condition */
	if (UpOfFrame >= 8)
		UpOfFrame = 1; /* shout not be here */

	/* get the AC ID of incoming packet */
	AcQueId = MapUserPriorityToAccessCategory[UpOfFrame];

	/* check whether the AC is trigger-enabled AC */
	if (pEntry->bAPSDCapablePerAC[AcQueId] == 0)
	{
		/*
			WMM Specification V1.1 Page 4
               Trigger Frame: A QoS Data or QoS Null frame from a WMM STA in
               Power Save Mode associated with an AC the WMM STA has configured
               to be a trigger-enabled AC.

               A QoS Data or QoS Null frame that indicates transition to/from
               Power Save Mode is not considered to be a Trigger Frame and the
			AP shall not respond with a QoS Null frame.
		*/

		/*
			ERROR! the AC does not belong to a trigger-enabled AC or
			the ACM of the AC is set.
		*/
		RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
		return;
	}


	/* enqueue U-APSD packets to AC software queues */

	/*
		Protect TxSwQueue0 & McastPsQueue because use them in
		interrupt context.
	*/
/*	RTMP_IRQ_LOCK(FlgIrq); */

	/* init */
	FlgQueEmpty = TRUE;
	TxPktNum = 0;
	SpMaxLen = SpLenMap[pEntry->MaxSPLength];
	pAcSwQue = NULL;
	pLastAcSwQue = NULL;
	pQuedPkt = NULL;
	FlgMgmtFrame = 0;

	/* from highest priority AC3 --> AC2 --> AC0 --> lowest priority AC1 */
	for (IdAc=(WMM_NUM_OF_AC-1); IdAc>=0; IdAc--)
	{
		AcQueId = AcPriority[IdAc];

		/* check if the AC is delivery-enable AC */
		if (pEntry->bAPSDDeliverEnabledPerAC[AcQueId] == 0)
			continue;

		/*
			NOTE: get U-APSD queue pointer here to speed up, do NOT use
			pEntry->UAPSDQueue[AcQueId] throughout codes because
			compiler will compile it to many assembly codes.
		*/
		pAcPsQue = &pEntry->UAPSDQueue[AcQueId];

		/* check if any U-APSD packet is queued for the AC */
		if (pAcPsQue->Head == NULL)
			continue;

		/* at least one U-APSD packet exists here */

		/* get AC software queue */
		QueId = QueIdList[AcQueId];
		pAcSwQue = &pAd->TxSwQueue[QueId];

		/* put U-APSD packets to the AC software queue */
		while(pAcPsQue->Head)
		{
			/* check if Max SP Length != 0 */
			if (SpMaxLen != 0)
			{
				/*
					WMM Specification V1.1 3.6.1.7
                       At each USP for a WMM STA, the WMM AP shall attempt to
                       transmit at least one MSDU or MMPDU, but no more than the
                       value encoded in the Max SP Length field in the QoS Info
                       Field of a WMM Information Element from delivery-enabled
					ACs, that are destined for the WMM STA.
				*/
				if (TxPktNum >= SpMaxLen)
				{
					/*
						Some queued U-APSD packets still exists so we will
						not clear MoreData bit of the packet.
					*/
					FlgQueEmpty = FALSE;
					break;
				}
			}

			/* count U-APSD packet number */
			TxPktNum ++;

			/* queue last U-APSD packet */
			if (pQuedPkt != NULL)
			{
				/* enqueue U-APSD packet to transmission software queue */

				/*
					WMM Specification V1.1 3.6.1.7
                       Each buffered frame shall be delivered using the access
					parameters of its AC.
				*/
				UAPSD_INSERT_QUEUE_AC(pAd, pEntry, pLastAcSwQue, pQuedPkt);
			}

			/* get the U-APSD packet */
			pQuedEntry = RemoveHeadQueue(pAcPsQue);
			pQuedPkt = QUEUE_ENTRY_TO_PACKET(pQuedEntry);

			if (pQuedPkt != NULL)
			{
				if (RTMP_GET_PACKET_MGMT_PKT(pQuedPkt) == 1)
					FlgMgmtFrame = 1;

				/*
					WMM Specification V1.1 3.6.1.7
                       The More Data bit (b13) of the directed MSDU or MMPDU
                       associated with delivery-enabled ACs and destined for
                       that WMM STA indicates that more frames are buffered for
					the delivery-enabled ACs.
				*/
				RTMP_SET_PACKET_MOREDATA(pQuedPkt, TRUE);

				/* set U-APSD flag & its software queue ID */
				RTMP_SET_PACKET_UAPSD(pQuedPkt, TRUE, QueId);
			}

			/* backup its software queue pointer */
			pLastAcSwQue = pAcSwQue;
		}

		if (FlgQueEmpty == FALSE)
		{
			/* FlgQueEmpty will be FALSE only when TxPktNum >= SpMaxLen */
			break;
		}
	}

	/*
		For any mamagement UAPSD frame, we use DMA to do SP check
		because no any FIFO statistics for management frame.
	*/
	if (FlgMgmtFrame)
		pEntry->bAPSDFlagSpRoughUse = 1;

	/*
		No need to protect EOSP handle code because we will be here
		only when last SP is ended.
	*/
	FlgNullSnd = FALSE;

	if (TxPktNum >= 1)
	{
		if (FlgQueEmpty == TRUE)
		{
			/*
				No any more queued U-APSD packet so clear More Data bit of
				the last frame.
			*/
			RTMP_SET_PACKET_MOREDATA(pQuedPkt, FALSE);
		}
	}

	//pEntry->bAPSDFlagSPStart = 1; /* set the SP start flag */
	UAPSD_SP_START(pAd, pEntry); /* set the SP start flag */
	pEntry->bAPSDFlagEOSPOK = 0;

#ifdef UAPSD_DEBUG
{
	ULONG DebugTimeNow;

	UAPSD_TIME_GET(pAd, DebugTimeNow);

	DBGPRINT(RT_DEBUG_TRACE, ("uapsd> start a SP (Tx Num = %d) (Rough SP = %d) "
			"(Has Any Mgmt = %d) (Abnormal = %d) (Time = %lu)\n",
			TxPktNum, pEntry->bAPSDFlagSpRoughUse, FlgMgmtFrame,
			gUAPSD_SP_CloseAbnormalNum, DebugTimeNow));
}
#endif /* UAPSD_DEBUG */

	if (TxPktNum <= 1)
	{
		/* if no data needs to tx, respond with QosNull for the trigger frame */
		pEntry->pUAPSDEOSPFrame = NULL;
		pEntry->UAPSDTxNum = 0;

		if (TxPktNum <= 0)
		{
			FlgNullSnd = TRUE;

#ifdef UAPSD_DEBUG
			DBGPRINT(RT_DEBUG_TRACE,
					 ("uapsd> No data, send a Qos-Null frame with ESOP bit on and "
					  "UP=%d to end USP\n", UpOfFrame));
#endif /* RELEASE_EXCLUDE */
		}
		else
        {
			/* only one packet so send it directly */
			RTMP_SET_PACKET_EOSP(pQuedPkt, TRUE);
			UAPSD_INSERT_QUEUE_AC(pAd, pEntry, pLastAcSwQue, pQuedPkt);

#ifdef UAPSD_DEBUG
			DBGPRINT(RT_DEBUG_TRACE,
					("uapsd> Only one packet with UP = %d\n",
					RTMP_GET_PACKET_UP(pQuedPkt)));
#endif /* RELEASE_EXCLUDE */
		} /* End of if */

		/*
			We will send the QoS Null frame below and we will hande the
			QoS Null tx done in RTMPFreeTXDUponTxDmaDone().
		*/
	}
	else
	{
		/* more than two U-APSD packets */

		/*
			NOTE: EOSP bit != !MoreData bit because Max SP Length,
			we can not use MoreData bit to decide EOSP bit.
		*/

		/*
			Backup the EOSP frame and
			we will transmit the EOSP frame in RTMPFreeTXDUponTxDmaDone().
		*/
		RTMP_SET_PACKET_EOSP(pQuedPkt, TRUE);

		pEntry->pUAPSDEOSPFrame = (PQUEUE_ENTRY)pQuedPkt;
		pEntry->UAPSDTxNum = TxPktNum-1; /* skip the EOSP frame */
	}

#ifdef UAPSD_DEBUG
	if ((pEntry->pUAPSDEOSPFrame != NULL) &&
		(RTMP_GET_PACKET_MGMT_PKT(pEntry->pUAPSDEOSPFrame) == 1))
	{
		DBGPRINT(RT_DEBUG_TRACE, ("uapsd> The EOSP frame is a management frame.\n"));
	}
#endif /* UAPSD_DEBUG */


#ifdef UAPSD_SP_ACCURATE
	/* count for legacy PS packet */

	/*
		Note: A worse case for mix mode (UAPSD + legacy PS):
			PS-Poll --> legacy ps packet --> trigger frame --> QoS Null frame
			(QSTA)		(QAP)				 (QSTA)			   (QAP)

		where statistics handler is NICUpdateFifoStaCounters().

		If we receive the trigger frame before the legacy ps packet is sent to
		the air, when we call statistics handler in tx done, it maybe the
		legacy ps statistics, not the QoS Null frame statistics, so we will
		do UAPSD counting fail.

		We need to count the legacy PS here if it is not yet sent to the air.
	*/

	/*
		Note: in addition, only one legacy PS need to count because one legacy
		packet for one PS-Poll packet; if we receive a trigger frame from a
		station, it means that only one legacy ps packet is possible not sent
		to the air, it is impossible more than 2 legacy packets are not yet
		sent to the air.
	*/

	if ((pEntry->bAPSDFlagSpRoughUse == 0) &&
		(pEntry->bAPSDFlagLegacySent != 0))
	{
		pEntry->UAPSDTxNum ++;

#ifdef UAPSD_DEBUG
		DBGPRINT(RT_DEBUG_TRACE, ("uapsd> A legacy PS is sent! UAPSDTxNum = %d\n",
				pEntry->UAPSDTxNum));
#endif /* UAPSD_DEBUG */
	}
#endif /* UAPSD_SP_ACCURATE */


	/* clear corresponding TIM bit */

	/* get its AID for the station */
	Aid = pEntry->Aid;

	if ((pEntry->bAPSDAllAC == 1) && (FlgQueEmpty == 1))
	{
		/* all AC are U-APSD and no any U-APSD packet is queued, set TIM */

#ifdef CONFIG_AP_SUPPORT
		/* clear TIM bit */
		if ((Aid > 0) && (Aid < MAX_LEN_OF_MAC_TABLE))
		{
			WLAN_MR_TIM_BIT_CLEAR(pAd, pEntry->apidx, Aid);
		}
#endif /* CONFIG_AP_SUPPORT */
	}

	/* reset idle timeout here whenever a trigger frame is received */
	pEntry->UAPSDQIdleCount = 0;

	RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);


	/* check if NULL Frame is needed to be transmitted */

	/* it will be crashed, when spin locked in kernel 2.6 */
	if (FlgNullSnd)
	{
		/* bQosNull = bEOSP = TRUE = 1 */

		/*
			Use management queue to tx QoS Null frame to avoid delay so
			us_of_frame is not used.
		*/
		RtmpEnqueueNullFrame(pAd, pEntry->Addr, pEntry->CurrTxRate,
							Aid, pEntry->apidx, TRUE, TRUE, UpOfFrame);

#ifdef UAPSD_DEBUG
		DBGPRINT(RT_DEBUG_TRACE, ("uapsd> end a SP by a QoS Null frame!\n"));
#endif /* UAPSD_DEBUG */
	}

#ifdef UAPSD_TIMING_RECORD_FUNC
	UAPSD_TIMING_RECORD(pAd, UAPSD_TIMING_RECORD_MOVE2TX);
#endif /* UAPSD_TIMING_RECORD_FUNC */

	/* Dequeue outgoing frames from TxSwQueue0..3 queue and process it */
	RTMPDeQueuePacket(pAd, FALSE, NUM_OF_TX_RING, MAX_TX_PROCESS);
}


#ifdef RTMP_MAC_USB
/*
========================================================================
Routine Description:
    Tag current offset of the AC in USB URB tx buffer.

Arguments:
	pAd				Pointer to our adapter
	*pPkt			the tx packet
    Wcid			destination entry id
	PktOffset		USB tx buffer offset

Return Value:
    None

Note:
	Only for RT2870.
========================================================================
*/
VOID UAPSD_TagFrame(
	IN	RTMP_ADAPTER		*pAd,
	IN	NDIS_PACKET			*pPkt,
	IN	UCHAR				Wcid,
	IN	UINT32				PktOffset)
{
	MAC_TABLE_ENTRY *pEntry;
	UCHAR AcQueId;


	if ((Wcid == MCAST_WCID) || (Wcid >= MAX_LEN_OF_MAC_TABLE))
		return; /* the frame is broadcast/multicast frame */
	/* End of if */

	pEntry = &pAd->MacTab.Content[Wcid];

	if (pEntry->bAPSDFlagSPStart == 0)
		return; /* SP is not started */
	/* End of if */

#ifdef UAPSD_DEBUG
	DBGPRINT(RT_DEBUG_TRACE, ("uapsd> prepare to tag the frame...\n"));
#endif /* UAPSD_DEBUG */

	if (RTMP_GET_PACKET_UAPSD_Flag(pPkt) == TRUE)
	{
		/* mark the latest USB tx buffer offset for the priority */
		AcQueId = RTMP_GET_PACKET_UAPSD_QUE_ID(pPkt);
		pEntry->UAPSDTagOffset[AcQueId] = PktOffset;

#ifdef UAPSD_DEBUG
		DBGPRINT(RT_DEBUG_TRACE, ("uapsd> tag offset = %d\n", PktOffset));
#endif /* UAPSD_DEBUG */
	} /* End of if */
} /* End of UAPSD_TagFrame */


/*
========================================================================
Routine Description:
    Check if UAPSD packets are tx ok.

Arguments:
	pAd				Pointer to our adapter
	AcQueId			TX completion for the AC (0 ~ 3)
	bulkStartPos
	bulkEnPos

Return Value:
    None

Note:
	Only for RT2870.
========================================================================
*/
VOID UAPSD_UnTagFrame(
	IN	RTMP_ADAPTER	*pAd,
	IN	UCHAR			AcQueId,
	IN	UINT32			bulkStartPos,
	IN	UINT32			bulkEnPos)
{
	MAC_TABLE_ENTRY *pEntry;
	UINT32 IdEntry;
	UINT32 TxPktTagOffset;
	UINT16 QueId;
	int		FirstWcid = 1;


	RTMP_SEM_LOCK(&pAd->UAPSDEOSPLock);

	/* loop for all entries to check whether we need to close their SP */
	for(IdEntry = FirstWcid; IdEntry < MAX_LEN_OF_MAC_TABLE; IdEntry++)
	{
		pEntry = &pAd->MacTab.Content[IdEntry];

		if ((IS_ENTRY_CLIENT(pEntry)
			)
			 && (pEntry->bAPSDFlagSPStart == 1) &&
			(pEntry->UAPSDTagOffset[AcQueId] != 0))
		{
#ifdef UAPSD_DEBUG
			DBGPRINT(RT_DEBUG_ERROR, ("uapsd> bulkStartPos = %d\n", bulkStartPos));
			DBGPRINT(RT_DEBUG_ERROR, ("uapsd> bulkEnPos = %d\n", bulkEnPos));
			DBGPRINT(RT_DEBUG_ERROR, ("uapsd> record offset = %d\n", pEntry->UAPSDTagOffset[AcQueId]));
#endif /* UAPSD_DEBUG */

			/*
				1. tx tag is in [bulkStartPos, bulkEnPos];
				2. when bulkEnPos < bulkStartPos
			*/
			TxPktTagOffset = pEntry->UAPSDTagOffset[AcQueId];

			if (((TxPktTagOffset >= bulkStartPos) &&
					(TxPktTagOffset <= bulkEnPos)) ||
				((bulkEnPos < bulkStartPos) &&
					(TxPktTagOffset >= bulkStartPos)) ||
				((bulkEnPos < bulkStartPos) &&
					(TxPktTagOffset <= bulkEnPos)))
			{
				/* ok, some UAPSD frames of the AC are transmitted */
				pEntry->UAPSDTagOffset[AcQueId] = 0;

				if (pEntry->UAPSDTxNum == 0)
				{
					/* ok, all UAPSD frames are transmitted */
					//pEntry->bAPSDFlagSPStart = 0;
					pEntry->bAPSDFlagEOSPOK = 0;
					UAPSD_SP_END(pAd, pEntry);

					if (pEntry->pUAPSDEOSPFrame != NULL)
		            {
						/* should not be here */
						RELEASE_NDIS_PACKET(pAd,
								QUEUE_ENTRY_TO_PACKET(pEntry->pUAPSDEOSPFrame),
		                                    NDIS_STATUS_FAILURE);
						pEntry->pUAPSDEOSPFrame = NULL;
		            } /* End of if */

#ifdef UAPSD_DEBUG
					DBGPRINT(RT_DEBUG_ERROR, ("uapsd> [1] close SP (%d)!\n", AcQueId));
#endif /* UAPSD_DEBUG */
					continue; /* check next station */
				} /* End of if */

				if ((pEntry->UAPSDTagOffset[QID_AC_BE] == 0) &&
					(pEntry->UAPSDTagOffset[QID_AC_BK] == 0) &&
					(pEntry->UAPSDTagOffset[QID_AC_VI] == 0) &&
					(pEntry->UAPSDTagOffset[QID_AC_VO] == 0))
				{
					/*
						OK, UAPSD frames of all AC for the entry are transmitted
						except the EOSP frame.
					*/

					if (pEntry->pUAPSDEOSPFrame != NULL)
	                {
	                    /* transmit the EOSP frame */
						PNDIS_PACKET pPkt;

						pPkt = QUEUE_ENTRY_TO_PACKET(pEntry->pUAPSDEOSPFrame);
						QueId = RTMP_GET_PACKET_UAPSD_QUE_ID(pPkt);

						if (QueId > QID_AC_VO)
	                    {
	                        /* should not be here, only for sanity */
							QueId = QID_AC_BE;
	                    } /* End of if */

#ifdef UAPSD_DEBUG
				DBGPRINT(RT_DEBUG_ERROR, ("uapsd> enqueue the EOSP frame...\n"));
#endif /* UAPSD_DEBUG */

						UAPSD_INSERT_QUEUE_AC(pAd, pEntry,
										&pAd->TxSwQueue[QueId],
										pEntry->pUAPSDEOSPFrame);

						pEntry->pUAPSDEOSPFrame = NULL;

						/*
							The EOSP frame will be put into ASIC to tx
							in RTMPHandleTxRingDmaDoneInterrupt(),
							not the function.
						*/

						/* de-queue packet here to speed up EOSP frame response */
						printk("%s:: ----> RTMPDeQueuePacket\n", __FUNCTION__);
						RTMPDeQueuePacket(pAd, FALSE, NUM_OF_TX_RING, MAX_TX_PROCESS);
					}
					else
					{
						/* only when 1 data frame with EOSP = 1 is transmitted */
						//pEntry->bAPSDFlagSPStart = 0;
						pEntry->bAPSDFlagEOSPOK = 0;
						UAPSD_SP_END(pAd, pEntry);

#ifdef UAPSD_DEBUG
						DBGPRINT(RT_DEBUG_ERROR, ("uapsd> [2] close SP (%d)!\n", AcQueId));
#endif /* UAPSD_DEBUG */
					} /* End of if */

					/* no any EOSP frames are queued and prepare to close the SP */
					pEntry->UAPSDTxNum = 0;
				} /* End of if */
			} /* End of if */
		} /* End of if */
	} /* End of for */

	RTMP_SEM_UNLOCK(&pAd->UAPSDEOSPLock);
} /* End of UAPSD_UnTagFrame */
#endif /* RTMP_MAC_USB */


#ifdef VENDOR_FEATURE3_SUPPORT
/*
========================================================================
Routine Description:
    Queue packet to a AC software queue.

Arguments:
	pAd				Pointer to our adapter
	pEntry			The station
	pQueueHeader	The software queue header of the AC
	bulkEnPos		The packet entry

Return Value:
    None

Note:
	Only for code size reduce purpose.
========================================================================
*/
static VOID UAPSD_InsertTailQueueAc(
	IN	RTMP_ADAPTER	*pAd,
	IN	MAC_TABLE_ENTRY	*pEntry,
	IN	QUEUE_HEADER	*pQueueHeader,
	IN	QUEUE_ENTRY		*pQueueEntry)
{
	InsertTailQueueAc(pAd, pEntry, pQueueHeader, pQueueEntry);
} /* End of InsertTailQueueAc */
#endif /* VENDOR_FEATURE3_SUPPORT */

#endif /* UAPSD_SUPPORT */

