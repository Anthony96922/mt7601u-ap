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
	rtmp_init.c

	Abstract:
	Miniport generic portion header file

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/
#include	"rt_config.h"

#ifdef OS_ABL_FUNC_SUPPORT
/* Os utility link: printk, scanf */
RTMP_OS_ABL_OPS RaOsOps, *pRaOsOps = &RaOsOps;
#endif /* OS_ABL_FUNC_SUPPORT */

#define RT3090A_DEFAULT_INTERNAL_LNA_GAIN	0x0A
UCHAR NUM_BIT8[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
#ifdef DBG
char*   CipherName[] = {"none","wep64","wep128","TKIP","AES","CKIP64","CKIP128","CKIP152","SMS4"};
#endif


/* 
	ASIC register initialization sets
*/
RTMP_REG_PAIR MACRegTable[] = {
#ifndef MT7601
#if defined(HW_BEACON_OFFSET) && (HW_BEACON_OFFSET == 0x200)
	{BCN_OFFSET0,			0xf8f0e8e0}, /* 0x3800(e0), 0x3A00(e8), 0x3C00(f0), 0x3E00(f8), 512B for each beacon */
	{BCN_OFFSET1,			0x6f77d0c8}, /* 0x3200(c8), 0x3400(d0), 0x1DC0(77), 0x1BC0(6f), 512B for each beacon */
#elif defined(HW_BEACON_OFFSET) && (HW_BEACON_OFFSET == 0x100)
	{BCN_OFFSET0,			0xece8e4e0}, /* 0x3800, 0x3A00, 0x3C00, 0x3E00, 512B for each beacon */
	{BCN_OFFSET1,			0xfcf8f4f0}, /* 0x3800, 0x3A00, 0x3C00, 0x3E00, 512B for each beacon */
#endif /* HW_BEACON_OFFSET */
#endif /* MT7601 */

	{LEGACY_BASIC_RATE,		0x0000013f}, /*  Basic rate set bitmap*/
	{HT_BASIC_RATE,		0x00008003}, /* Basic HT rate set , 20M, MCS=3, MM. Format is the same as in TXWI.*/
	{MAC_SYS_CTRL,		0x00}, /* 0x1004, , default Disable RX*/
	{RX_FILTR_CFG,		0x17f97}, /*0x1400  , RX filter control,  */
	{BKOFF_SLOT_CFG,	0x209}, /* default set short slot time, CC_DELAY_TIME should be 2	 */
	/*{TX_SW_CFG0,		0x40a06},  Gary,2006-08-23 */
	{TX_SW_CFG0,		0x0}, 		/* Gary,2008-05-21 for CWC test */
	{TX_SW_CFG1,		0x80606}, /* Gary,2006-08-23 */
	{TX_LINK_CFG,		0x1020},		/* Gary,2006-08-23 */
	/*{TX_TIMEOUT_CFG,	0x00182090},	 CCK has some problem. So increase timieout value. 2006-10-09 MArvek RT*/
	{TX_TIMEOUT_CFG,	0x000a2090},	/* CCK has some problem. So increase timieout value. 2006-10-09 MArvek RT , Modify for 2860E ,2007-08-01*/
	{MAX_LEN_CFG,		MAX_AGGREGATION_SIZE | 0x00001000},	/* 0x3018, MAX frame length. Max PSDU = 16kbytes.*/
#ifndef MT7601
	{LED_CFG,		0x7f031e46}, /* Gary, 2006-08-23*/
#endif /* MT7601 */

#ifdef RLT_MAC

#ifdef MT7601
#ifdef CONFIG_MULTI_CHANNEL
	{TX_MAX_PCNT,		0x1f1f1f1f/*0x1fbf1f1f */},
#else
	{TX_MAX_PCNT,		0x1fbf1f1f},
#endif /* CONFIG_MULTI_CHANNEL */
	{RX_MAX_PCNT,		0x9f},
#else
	{TX_MAX_PCNT,		0xbfbf3f1f},
	{RX_MAX_PCNT,		0x9f},
#endif

// TODO: shiang-6590, need set this in FPGA mode
#ifdef RTMP_MAC_USB
#endif /* RTMP_MAC_USB */
#else
#ifdef INF_AMAZON_SE
	{PBF_MAX_PCNT,			0x1F3F6F6F}, 	/*iverson modify for usb issue, 2008/09/19*/
											/* 6F + 6F < total page count FE*/
											/* so that RX doesn't occupy TX's buffer space when WMM congestion.*/
#else
	{PBF_MAX_PCNT,			0x1F3FBF9F}, 	/*0x1F3f7f9f},		Jan, 2006/04/20*/
#endif /* INF_AMAZON_SE */
#endif /* RLT_MAC */

	/*{TX_RTY_CFG,			0x6bb80408},	 Jan, 2006/11/16*/
/* WMM_ACM_SUPPORT*/
/*	{TX_RTY_CFG,			0x6bb80101},	 sample*/
	{TX_RTY_CFG,			0x47d01f0f},	/* Jan, 2006/11/16, Set TxWI->ACK =0 in Probe Rsp Modify for 2860E ,2007-08-03*/
	
	{AUTO_RSP_CFG,			0x00000013},	/* Initial Auto_Responder, because QA will turn off Auto-Responder*/
	{CCK_PROT_CFG,			0x05740003 /*0x01740003*/},	/* Initial Auto_Responder, because QA will turn off Auto-Responder. And RTS threshold is enabled. */
	{OFDM_PROT_CFG,			0x05740003 /*0x01740003*/},	/* Initial Auto_Responder, because QA will turn off Auto-Responder. And RTS threshold is enabled. */
#ifdef RTMP_MAC_USB
#ifndef MT7601
	{PBF_CFG, 				0xf40006}, 		/* Only enable Queue 2*/
	{WPDMA_GLO_CFG,			0x00000030},		// MT7601U not support WPDMA
#endif /* MT7601 */
	{MM40_PROT_CFG,			0x3F44084},		/* Initial Auto_Responder, because QA will turn off Auto-Responder*/
#endif /* RTMP_MAC_USB */
	{GF20_PROT_CFG,			0x01744004},    /* set 19:18 --> Short NAV for MIMO PS*/
	{GF40_PROT_CFG,			0x03F44084},    
	{MM20_PROT_CFG,			0x01744004},    
	{TXOP_CTRL_CFG,			0x0000583f, /*0x0000243f*/ /*0x000024bf*/},	/*Extension channel backoff.*/
	{TX_RTS_CFG,			0x01092b20},		// enable RTS fall back

	{EXP_ACK_TIME,			0x002400ca},	/* default value */
	{TXOP_HLDR_ET, 			0x00000002},

	/* Jerry comments 2008/01/16: we use SIFS = 10us in CCK defaultly, but it seems that 10us
		is too small for INTEL 2200bg card, so in MBSS mode, the delta time between beacon0
		and beacon1 is SIFS (10us), so if INTEL 2200bg card connects to BSS0, the ping
		will always lost. So we change the SIFS of CCK from 10us to 16us. */
	{XIFS_TIME_CFG,			0x33a41010},
#if defined(RT65xx) || defined(MT7601)
	{PWR_PIN_CFG,			0x00000000},
#else
	{PWR_PIN_CFG,			0x00000003},	/* patch for 2880-E*/
#endif /* defined(RT65xx) || defined(MT7601) */
};

#ifdef CONFIG_AP_SUPPORT
RTMP_REG_PAIR	APMACRegTable[] =	{
	{WMM_AIFSN_CFG,		0x00001173},
	{WMM_CWMIN_CFG,	0x00002344},
	{WMM_CWMAX_CFG,	0x000034a6},
	{WMM_TXOP0_CFG,		0x00100020},
	{WMM_TXOP1_CFG,		0x002F0038},
	{TBTT_SYNC_CFG,		0x00012000},
#ifdef STREAM_MODE_SUPPORT
	{TX_CHAIN_ADDR0_L,	0xFFFFFFFF},	/* Broadcast frames are in stream mode*/
	{TX_CHAIN_ADDR0_H,	0x3FFFF},
#endif /* STREAM_MODE_SUPPORT */
};
#endif /* CONFIG_AP_SUPPORT */



#define	NUM_MAC_REG_PARMS		(sizeof(MACRegTable) / sizeof(RTMP_REG_PAIR))
#ifdef CONFIG_AP_SUPPORT
#define	NUM_AP_MAC_REG_PARMS	(sizeof(APMACRegTable) / sizeof(RTMP_REG_PAIR))
#endif /* CONFIG_AP_SUPPORT */


/*
	Use the global variable is not a good solution.
	But we can not put it to pAd and use the lock in pAd of RALINK_TIMER_STRUCT;
	Or when the structure is cleared, we maybe get NULL for pAd and can not lock.
	Maybe we can put pAd in RTMPSetTimer/ RTMPModTimer/ RTMPCancelTimer.
*/
NDIS_SPIN_LOCK TimerSemLock;


/*
	========================================================================
	
	Routine Description:
		Allocate RTMP_ADAPTER data block and do some initialization

	Arguments:
		Adapter		Pointer to our adapter

	Return Value:
		NDIS_STATUS_SUCCESS
		NDIS_STATUS_FAILURE

	IRQL = PASSIVE_LEVEL

	Note:
	
	========================================================================
*/
NDIS_STATUS	RTMPAllocAdapterBlock(
	IN  PVOID	handle,
	OUT	VOID	**ppAdapter)
{
	PRTMP_ADAPTER	pAd = NULL;
	NDIS_STATUS		Status;
	INT 			index;
	UCHAR			*pBeaconBuf = NULL;


#ifdef OS_ABL_FUNC_SUPPORT
	/* must put the function before any print message */
	/* init OS utilities provided from UTIL module */
	RtmpOsOpsInit(&RaOsOps);
#endif /* OS_ABL_FUNC_SUPPORT */

	DBGPRINT(RT_DEBUG_TRACE, ("--> RTMPAllocAdapterBlock\n"));

	/* init UTIL module */
	RtmpUtilInit();

	*ppAdapter = NULL;

	do
	{
		/* Allocate RTMP_ADAPTER memory block*/
/*		pBeaconBuf = kmalloc(MAX_BEACON_SIZE, MEM_ALLOC_FLAG);*/
		os_alloc_mem(NULL, (UCHAR **)&pBeaconBuf, MAX_BEACON_SIZE);
		if (pBeaconBuf == NULL)
		{
			Status = NDIS_STATUS_FAILURE;
			DBGPRINT_ERR(("Failed to allocate memory - BeaconBuf!\n"));
			break;
		}
		NdisZeroMemory(pBeaconBuf, MAX_BEACON_SIZE);

		Status = AdapterBlockAllocateMemory(handle, (PVOID *)&pAd, sizeof(RTMP_ADAPTER));
		if (Status != NDIS_STATUS_SUCCESS)
		{
			DBGPRINT_ERR(("Failed to allocate memory - ADAPTER\n"));
			break;
		}
		else
		{
			/* init resource list (must be after pAd allocation) */
			initList(&pAd->RscTimerMemList);
			initList(&pAd->RscTaskMemList);
			initList(&pAd->RscLockMemList);
			initList(&pAd->RscTaskletMemList);
			initList(&pAd->RscSemMemList);
			initList(&pAd->RscAtomicMemList);

			initList(&pAd->RscTimerCreateList);

			pAd->OS_Cookie = handle;
#ifdef WORKQUEUE_BH
			((POS_COOKIE)(handle))->pAd_va = pAd;
#endif /* WORKQUEUE_BH */
		}
		pAd->BeaconBuf = pBeaconBuf;
		DBGPRINT(RT_DEBUG_OFF, ("=== pAd = %p, size = %d ===\n", pAd, (UINT32)sizeof(RTMP_ADAPTER)));

		if (RtmpOsStatsAlloc(&pAd->stats, &pAd->iw_stats) == FALSE)
		{
			Status = NDIS_STATUS_FAILURE;
			break;
		}

		/* Init spin locks*/
		NdisAllocateSpinLock(pAd, &pAd->MgmtRingLock);

#if defined(RT3290) || defined(RT65xx) || defined(MT7601)
#endif /* defined(RT3290) || defined(RT65xx) || defined(MT7601) */

		for (index = 0; index < NUM_OF_TX_RING; index++)
		{
			NdisAllocateSpinLock(pAd, &pAd->TxSwQueueLock[index]);
			NdisAllocateSpinLock(pAd, &pAd->DeQueueLock[index]);
			pAd->DeQueueRunning[index] = FALSE;
		}

#ifdef RESOURCE_PRE_ALLOC
		/* 
			move this function from rt28xx_init() to here. now this function only allocate memory and
			leave the initialization job to RTMPInitTxRxRingMemory() which called in rt28xx_init().
		*/
		Status = RTMPAllocTxRxRingMemory(pAd);
		if (Status != NDIS_STATUS_SUCCESS)
		{
			DBGPRINT_ERR(("Failed to allocate memory - TxRxRing\n"));
			break;
		}
#endif /* RESOURCE_PRE_ALLOC */

		NdisAllocateSpinLock(pAd, &pAd->irq_lock);

		NdisAllocateSpinLock(pAd, &TimerSemLock);

#ifdef RALINK_ATE
#ifdef RTMP_MAC_USB
		RTMP_OS_ATMOIC_INIT(&pAd->BulkOutRemained, &pAd->RscAtomicMemList);
		RTMP_OS_ATMOIC_INIT(&pAd->BulkInRemained, &pAd->RscAtomicMemList);
#endif /* RTMP_MAC_USB */
#endif /* RALINK_ATE */

#ifdef CONFIG_AP_SUPPORT
#ifdef UAPSD_SUPPORT
/*        UAPSD_Init(pAd);  move to rt28xx_init*/
#endif /* UAPSD_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

		/* assign function pointers*/
#ifdef MAT_SUPPORT
		/* init function pointers, used in OS_ABL */
/*		RTMP_MATOpsInit(pAd);  move to rt28xx_init*/
#endif /* MAT_SUPPORT */
	} while (FALSE);

	if ((Status != NDIS_STATUS_SUCCESS) && (pBeaconBuf))
/*		kfree(pBeaconBuf);*/
		os_free_mem(NULL, pBeaconBuf);

	if ((Status != NDIS_STATUS_SUCCESS) && (pAd != NULL))
	{
		if (pAd->stats != NULL)
			os_free_mem(NULL, pAd->stats);

		if (pAd->iw_stats != NULL)
			os_free_mem(NULL, pAd->iw_stats);
	}

	if (pAd != NULL) /* compile warning: avoid use NULL pointer when pAd == NULL */
		*ppAdapter = (VOID *)pAd;


	/*
		Init ProbeRespIE Table
	*/
	for (index = 0; index < MAX_LEN_OF_BSS_TABLE; index++) 
	{
		if (os_alloc_mem(pAd,&pAd->ProbeRespIE[index].pIe, MAX_VIE_LEN) == NDIS_STATUS_SUCCESS)
			RTMPZeroMemory(pAd->ProbeRespIE[index].pIe, MAX_VIE_LEN);
		else
			pAd->ProbeRespIE[index].pIe = NULL;
	}

	DBGPRINT_S(Status, ("<-- RTMPAllocAdapterBlock, Status=%x\n", Status));
	return Status;
}


/*
	========================================================================
	
	Routine Description:
		Read initial parameters from EEPROM
		
	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
VOID NICReadEEPROMParameters(RTMP_ADAPTER *pAd, PSTRING mac_addr)
{
	USHORT i, value, value2;
	EEPROM_TX_PWR_STRUC Power;
	EEPROM_VERSION_STRUC Version;
	EEPROM_ANTENNA_STRUC Antenna;
	EEPROM_NIC_CONFIG2_STRUC NicConfig2;
	USHORT  Addr01,Addr23,Addr45 ;
	MAC_DW0_STRUC csr2;
	MAC_DW1_STRUC csr3;


	DBGPRINT(RT_DEBUG_TRACE, ("--> NICReadEEPROMParameters\n"));	

#ifdef RT3290
	if (IS_RT3290(pAd))
		RT3290_eeprom_access_grant(pAd, TRUE);
#endif /* RT3290 */

	if (pAd->chipOps.eeinit)
	{
		pAd->chipOps.eeinit(pAd);
#ifdef RTMP_EFUSE_SUPPORT
#ifdef RALINK_ATE
		if(!pAd->bFroceEEPROMBuffer && pAd->bEEPROMFile)
		{
			DBGPRINT(RT_DEBUG_OFF, ("--> NICReadEEPROMParameters::(Efuse)Load to EEPROM Buffer Mode\n"));
			eFuseLoadEEPROM(pAd);
		}
#endif /* RALINK_ATE */
#endif /* RTMP_EFUSE_SUPPORT */
	}

	/* Read MAC setting from EEPROM and record as permanent MAC address */
	DBGPRINT(RT_DEBUG_TRACE, ("Initialize MAC Address from E2PROM \n"));

	RT28xx_EEPROM_READ16(pAd, 0x04, Addr01);
	RT28xx_EEPROM_READ16(pAd, 0x06, Addr23);
	RT28xx_EEPROM_READ16(pAd, 0x08, Addr45);

	pAd->PermanentAddress[0] = (UCHAR)(Addr01 & 0xff);
	pAd->PermanentAddress[1] = (UCHAR)(Addr01 >> 8);
	pAd->PermanentAddress[2] = (UCHAR)(Addr23 & 0xff);
	pAd->PermanentAddress[3] = (UCHAR)(Addr23 >> 8);
	pAd->PermanentAddress[4] = (UCHAR)(Addr45 & 0xff);
	pAd->PermanentAddress[5] = (UCHAR)(Addr45 >> 8);

	/*more conveninet to test mbssid, so ap's bssid &0xf1*/
	if (pAd->PermanentAddress[0] == 0xff)
		pAd->PermanentAddress[0] = RandomByte(pAd)&0xf8;
			
	DBGPRINT(RT_DEBUG_TRACE, ("E2PROM MAC: =%02x:%02x:%02x:%02x:%02x:%02x\n",
								PRINT_MAC(pAd->PermanentAddress)));

	/* Assign the actually working MAC Address */
	if (pAd->bLocalAdminMAC)
	{		
		DBGPRINT(RT_DEBUG_TRACE, ("Use the MAC address what is assigned from Configuration file(.dat). \n"));
	}
	else if (mac_addr && 
			 strlen((PSTRING)mac_addr) == 17 &&
			 (strcmp(mac_addr, "00:00:00:00:00:00") != 0))
	{
		INT j;
		PSTRING	macptr;

		macptr = (PSTRING) mac_addr;
		for (j=0; j<MAC_ADDR_LEN; j++)
		{
			AtoH(macptr, &pAd->CurrentAddress[j], 1);
			macptr=macptr+3;
		}	
		
		DBGPRINT(RT_DEBUG_TRACE, ("Use the MAC address what is assigned from Moudle Parameter. \n"));
	}
	else
	{
		COPY_MAC_ADDR(pAd->CurrentAddress, pAd->PermanentAddress);
		DBGPRINT(RT_DEBUG_TRACE, ("Use the MAC address what is assigned from EEPROM. \n"));
	}

	/* Set the current MAC to ASIC */	
	csr2.field.Byte0 = pAd->CurrentAddress[0];
	csr2.field.Byte1 = pAd->CurrentAddress[1];
	csr2.field.Byte2 = pAd->CurrentAddress[2];
	csr2.field.Byte3 = pAd->CurrentAddress[3];
	RTMP_IO_WRITE32(pAd, MAC_ADDR_DW0, csr2.word);
	csr3.word = 0;
	csr3.field.Byte4 = pAd->CurrentAddress[4];
	{
		csr3.field.Byte5 = pAd->CurrentAddress[5];
		csr3.field.U2MeMask = 0xff;
	}
	RTMP_IO_WRITE32(pAd, MAC_ADDR_DW1, csr3.word);

#ifdef HDR_TRANS_SUPPORT
	RTMP_IO_WRITE32(pAd, HT_MAC_ADDR_DW0, csr2.word);
	csr3.word &= 0xff00ffff;
	csr3.word |= 0x00410000;				// HW test code
	RTMP_IO_WRITE32(pAd, HT_MAC_ADDR_DW1, csr3.word);
#endif /* HDR_TRANS_SUPPORT */

	DBGPRINT_RAW(RT_DEBUG_TRACE,("Current MAC: =%02x:%02x:%02x:%02x:%02x:%02x\n",
					PRINT_MAC(pAd->CurrentAddress)));

	/* if E2PROM version mismatch with driver's expectation, then skip*/
	/* all subsequent E2RPOM retieval and set a system error bit to notify GUI*/
	RT28xx_EEPROM_READ16(pAd, EEPROM_VERSION_OFFSET, Version.word);
	pAd->EepromVersion = Version.field.Version + Version.field.FaeReleaseNumber * 256;
	DBGPRINT(RT_DEBUG_TRACE, ("E2PROM: Version = %d, FAE release #%d\n", Version.field.Version, Version.field.FaeReleaseNumber));

#ifdef MT7601
	if ( IS_MT7601(pAd) )
	{
		if (Version.field.Version > MT7601_VALID_EEPROM_VERSION)
		{
			DBGPRINT_ERR(("MT7601 E2PROM: WRONG VERSION 0x%x, should be %d\n",Version.field.Version, MT7601_VALID_EEPROM_VERSION));
		}

	}
	else
#endif /* MT7601 */
	if (Version.field.Version > VALID_EEPROM_VERSION)
	{
		DBGPRINT_ERR(("E2PROM: WRONG VERSION 0x%x, should be %d\n",Version.field.Version, VALID_EEPROM_VERSION));
	}

	/* Read BBP default value from EEPROM and store to array(EEPROMDefaultValue) in pAd*/
	RT28xx_EEPROM_READ16(pAd, EEPROM_NIC1_OFFSET, value);
	pAd->EEPROMDefaultValue[EEPROM_NIC_CFG1_OFFSET] = value;

	RT28xx_EEPROM_READ16(pAd, EEPROM_NIC2_OFFSET, value);
	pAd->EEPROMDefaultValue[EEPROM_NIC_CFG2_OFFSET] = value;

	/* if not return early. cause fail at emulation.*/
	/* Init the channel number for TX channel power*/
#ifdef RT8592
	if (IS_RT8592(pAd))
		RT85592_ReadChannelPwr(pAd);
	else
#endif /* RT8592 */
#ifdef RT65xx
	if (IS_RT6590(pAd))
		RT6590_ReadChannelPwr(pAd);
	else
#endif /* RT65xx */
#ifdef MT7601
	if (IS_MT7601(pAd))
		MT7601_ReadChannelPwr(pAd);
	else
#endif /* MT7601 */
		RTMPReadChannelPwr(pAd);

	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_COUNTRY_REGION, value);	/* Country Region*/
		pAd->EEPROMDefaultValue[EEPROM_COUNTRY_REG_OFFSET] = value;
	}

#if defined(BT_COEXISTENCE_SUPPORT) || defined(RT3290)
	RT28xx_EEPROM_READ16(pAd, EEPROM_NIC3_OFFSET, value);
	pAd->EEPROMDefaultValue[EEPROM_NIC_CFG3_OFFSET] = value;
	pAd->NicConfig3.word = pAd->EEPROMDefaultValue[EEPROM_NIC_CFG3_OFFSET];
#endif /* defined(BT_COEXISTENCE_SUPPORT) || defined(RT3290) */

// TODO: check this after receive TC6008 EEPROM format
#if !defined(RT65xx) && !defined(MT7601) 
	for(i = 0; i < 8; i++)
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_BBP_BASE_OFFSET + i*2, value);
		pAd->EEPROMDefaultValue[i+EEPROM_BBP_ARRAY_OFFSET] = value;
	}
#endif /* !defined(RT65xx) && !defined(MT7601) */

	/* We have to parse NIC configuration 0 at here.*/
	/* If TSSI did not have preloaded value, it should reset the TxAutoAgc to false*/
	/* Therefore, we have to read TxAutoAgc control beforehand.*/
	/* Read Tx AGC control bit*/
	Antenna.word = pAd->EEPROMDefaultValue[EEPROM_NIC_CFG1_OFFSET];

#ifdef RT65xx
	if (IS_RT8592(pAd)) {
		DBGPRINT(RT_DEBUG_OFF, ("RT85592: Antenna.RfIcType=%d, TxPath=%d, RxPath=%d\n",
					Antenna.field.RfIcType, Antenna.field.TxPath, Antenna.field.RxPath));
		// TODO: fix me!!
		Antenna.word = 0;
		Antenna.field.BoardType = 0;
		Antenna.field.RfIcType = 0xf;
		Antenna.field.TxPath = 2;
		Antenna.field.RxPath = 2;
	}
	else if (IS_RT65XX(pAd)) {
		// TODO: shiang-6590, now we force the ant as 1*1, remove it after EEPROM is ready!
		Antenna.word = 0;
		Antenna.field.BoardType = 0;
		Antenna.field.RfIcType = 0xf;
		Antenna.field.TxPath = 1;
		Antenna.field.RxPath = 1;
	}
#endif /* RT65xx */

	/* must be put here, because RTMP_CHIP_ANTENNA_INFO_DEFAULT_RESET() will clear *
	 * EPROM 0x34~3 */
#ifdef TXRX_SW_ANTDIV_SUPPORT
	/* EEPROM 0x34[15:12] = 0xF is invalid, 0x2~0x3 is TX/RX SW AntDiv */
	if (((Antenna.word & 0xFF00) != 0xFF00) && (Antenna.word & 0x2000))  
	{																	  
		pAd->chipCap.bTxRxSwAntDiv = TRUE;		/* for GPIO switch */
		DBGPRINT(RT_DEBUG_OFF, ("\x1b[mAntenna word %X/%d, AntDiv %d\x1b[m\n",
					Antenna.word, Antenna.field.BoardType, pAd->NicConfig2.field.AntDiversity));
	}
#endif /* TXRX_SW_ANTDIV_SUPPORT */


	// TODO: shiang, why we only check oxff00??
	if ((Antenna.word & 0xFF00) == 0xFF00)
/*	if (Antenna.word == 0xFFFF)*/
		RTMP_CHIP_ANTENNA_INFO_DEFAULT_RESET(pAd, &Antenna);

	/* Choose the desired Tx&Rx stream.*/
	if ((pAd->CommonCfg.TxStream == 0) || (pAd->CommonCfg.TxStream > Antenna.field.TxPath))
		pAd->CommonCfg.TxStream = Antenna.field.TxPath;

	if ((pAd->CommonCfg.RxStream == 0) || (pAd->CommonCfg.RxStream > Antenna.field.RxPath))
	{
		pAd->CommonCfg.RxStream = Antenna.field.RxPath;

		if ((pAd->MACVersion != RALINK_3883_VERSION) &&
			(pAd->MACVersion != RALINK_2883_VERSION) &&
			(pAd->CommonCfg.RxStream > 2))
		{
			/* only 2 Rx streams for RT2860 series*/
			pAd->CommonCfg.RxStream = 2;
		}
	}

	/* EEPROM offset 0x36 - NIC Configuration 1 */
	NicConfig2.word = pAd->EEPROMDefaultValue[EEPROM_NIC_CFG2_OFFSET];

#ifdef WSC_INCLUDED
	/* WSC hardware push button function 0811 */
	//if ((pAd->MACVersion == 0x28600100) || (pAd->MACVersion == 0x28700100))
		WSC_HDR_BTN_MR_HDR_SUPPORT_SET(pAd, NicConfig2.field.EnableWPSPBC);
#endif /* WSC_INCLUDED */

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		if (NicConfig2.word == 0xffff)
			NicConfig2.word = 0;
	}
#endif /* CONFIG_AP_SUPPORT */	


	if (NicConfig2.field.DynamicTxAgcControl == 1)
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = TRUE;
	else
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = FALSE;
	
	
	/* Save value for future using */
	pAd->NicConfig2.word = NicConfig2.word;
	
	DBGPRINT_RAW(RT_DEBUG_TRACE, ("NICReadEEPROMParameters: RxPath = %d, TxPath = %d\n", Antenna.field.RxPath, Antenna.field.TxPath));

	/* Save the antenna for future use*/
	pAd->Antenna.word = Antenna.word;

	/* Set the RfICType here, then we can initialize RFIC related operation callbacks*/
	pAd->Mlme.RealRxPath = (UCHAR) Antenna.field.RxPath;

	pAd->RfIcType = (UCHAR) Antenna.field.RfIcType;

#ifdef RT65xx
	// TODO: shiang-6590, currently we don't have eeprom value, so directly force to set it as 0xff
	if (IS_RT8592(pAd)) {
		pAd->Mlme.RealRxPath = 2;
		pAd->RfIcType = RFIC_UNKNOWN;
	}
	else	 if (IS_RT65XX(pAd)) {
		pAd->Mlme.RealRxPath = 1;
		pAd->RfIcType = RFIC_UNKNOWN;
	}
#endif /* RT65xx */

	/* check if the chip supports 5G band */
	if (WMODE_CAP_5G(pAd->CommonCfg.PhyMode))
	{
		if (!RFIC_IS_5G_BAND(pAd))
		{
			DBGPRINT_RAW(RT_DEBUG_ERROR,
						("%s():Err! chip not support 5G band %d!\n",
						__FUNCTION__, pAd->RfIcType));
#ifdef DOT11_N_SUPPORT
			/* change to bgn mode */
			Set_WirelessMode_Proc(pAd, "9");
#else
			/* change to bg mode */
			Set_WirelessMode_Proc(pAd, "0");
#endif /* DOT11_N_SUPPORT */
			pAd->RFICType = RFIC_24GHZ;
		}
		pAd->RFICType = RFIC_24GHZ | RFIC_5GHZ;
	}
	else
		pAd->RFICType = RFIC_24GHZ;

	RTMP_NET_DEV_NICKNAME_INIT(pAd);

#ifdef RT65xx
	if (IS_RT6590(pAd))
	{
		; // TODO: wait TC6008 EEPROM format
	}
	else
#endif /* RT65xx */
#ifdef MT7601
	if (IS_MT7601(pAd))
	{
		; // move to MT7601_InitDesiredTSSITable
	}
	else
#endif /* MT7601 */
	/* Read TSSI reference and TSSI boundary for temperature compensation. This is ugly*/
	/* 0. 11b/g*/
	{
		/* these are tempature reference value (0x00 ~ 0xFE)
		   ex: 0x00 0x15 0x25 0x45 0x88 0xA0 0xB5 0xD0 0xF0
		   TssiPlusBoundaryG [4] [3] [2] [1] [0] (smaller) +
		   TssiMinusBoundaryG[0] [1] [2] [3] [4] (larger) */
		{
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND1, Power.word);
			pAd->TssiMinusBoundaryG[4] = Power.field.Byte0;
			pAd->TssiMinusBoundaryG[3] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND2, Power.word);
			pAd->TssiMinusBoundaryG[2] = Power.field.Byte0;
			pAd->TssiMinusBoundaryG[1] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND3, Power.word);
			pAd->TssiRefG   = Power.field.Byte0; /* reference value [0] */
			pAd->TssiPlusBoundaryG[1] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND4, Power.word);
			pAd->TssiPlusBoundaryG[2] = Power.field.Byte0;
			pAd->TssiPlusBoundaryG[3] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND5, Power.word);
			pAd->TssiPlusBoundaryG[4] = Power.field.Byte0;
			pAd->TxAgcStepG = Power.field.Byte1;    
			pAd->TxAgcCompensateG = 0;
			pAd->TssiMinusBoundaryG[0] = pAd->TssiRefG;
			pAd->TssiPlusBoundaryG[0]  = pAd->TssiRefG;

			/* Disable TxAgc if the based value is not right*/
			if (pAd->TssiRefG == 0xff)
				pAd->bAutoTxAgcG = FALSE;
		}

		DBGPRINT(RT_DEBUG_TRACE,("E2PROM: G Tssi[-4 .. +4] = %d %d %d %d - %d -%d %d %d %d, step=%d, tuning=%d\n",
			pAd->TssiMinusBoundaryG[4], pAd->TssiMinusBoundaryG[3], pAd->TssiMinusBoundaryG[2], pAd->TssiMinusBoundaryG[1],
			pAd->TssiRefG,
			pAd->TssiPlusBoundaryG[1], pAd->TssiPlusBoundaryG[2], pAd->TssiPlusBoundaryG[3], pAd->TssiPlusBoundaryG[4],
			pAd->TxAgcStepG, pAd->bAutoTxAgcG));
	}	

#ifdef RT65xx
	if (IS_RT6590(pAd))
	{
		; // TODO: wait TC6008 EEPROM format
	}
	else
#endif /* RT65xx */
#ifdef MT7601
	if (IS_MT7601(pAd))
	{
		; // MT7601 not support A Band
	}
	else
#endif /* MT7601 */
	/* 1. 11a*/
	{
		{
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND1, Power.word);
			pAd->TssiMinusBoundaryA[4] = Power.field.Byte0;
			pAd->TssiMinusBoundaryA[3] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND2, Power.word);
			pAd->TssiMinusBoundaryA[2] = Power.field.Byte0;
			pAd->TssiMinusBoundaryA[1] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND3, Power.word);
			pAd->TssiRefA = Power.field.Byte0;
			pAd->TssiPlusBoundaryA[1] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND4, Power.word);
			pAd->TssiPlusBoundaryA[2] = Power.field.Byte0;
			pAd->TssiPlusBoundaryA[3] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND5, Power.word);
			pAd->TssiPlusBoundaryA[4] = Power.field.Byte0;
			pAd->TxAgcStepA = Power.field.Byte1;    
			pAd->TxAgcCompensateA = 0;
			pAd->TssiMinusBoundaryA[0] = pAd->TssiRefA;
			pAd->TssiPlusBoundaryA[0]  = pAd->TssiRefA;

			/* Disable TxAgc if the based value is not right*/
			if (pAd->TssiRefA == 0xff)
				pAd->bAutoTxAgcA = FALSE;
		}

		DBGPRINT(RT_DEBUG_TRACE,("E2PROM: A Tssi[-4 .. +4] = %d %d %d %d - %d -%d %d %d %d, step=%d, tuning=%d\n",
			pAd->TssiMinusBoundaryA[4], pAd->TssiMinusBoundaryA[3], pAd->TssiMinusBoundaryA[2], pAd->TssiMinusBoundaryA[1],
			pAd->TssiRefA,
			pAd->TssiPlusBoundaryA[1], pAd->TssiPlusBoundaryA[2], pAd->TssiPlusBoundaryA[3], pAd->TssiPlusBoundaryA[4],
			pAd->TxAgcStepA, pAd->bAutoTxAgcA));
	}	
	pAd->BbpRssiToDbmDelta = 0x0;
	
	/* Read frequency offset setting for RF*/
		RT28xx_EEPROM_READ16(pAd, EEPROM_FREQ_OFFSET, value);

	if ((value & 0x00FF) != 0x00FF)
		pAd->RfFreqOffset = (ULONG) (value & 0x00FF);
	else
		pAd->RfFreqOffset = 0;

#ifdef MT7601
	if ( IS_MT7601(pAd) )
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_FREQ_OFFSET_COMPERSATION, value);
		value = (value >> 8) & 0xFF;
		if ( value != 0xFF )
		{
			if ( value & 0x80 )
				pAd->RfFreqOffset -= (value & 0x7F);
			else
				pAd->RfFreqOffset += value;
		}
		
	}
#endif /* MT7601 */
	

	DBGPRINT(RT_DEBUG_TRACE, ("E2PROM: RF FreqOffset=0x%lx \n", pAd->RfFreqOffset));

	// 40M BW in 2.4GHz band (37h)
	if ( pAd->NicConfig2.field.BW40MAvailForG )
	{
		pAd->CommonCfg.RegTransmitSetting.field.BW  = BW_20;
		DBGPRINT(RT_DEBUG_TRACE, ("E2PROM: BW_20\n"));
	}

	/*CountryRegion byte offset (38h)*/
	{
		value = pAd->EEPROMDefaultValue[EEPROM_COUNTRY_REG_OFFSET] >> 8;		/* 2.4G band*/
		value2 = pAd->EEPROMDefaultValue[EEPROM_COUNTRY_REG_OFFSET] & 0x00FF;	/* 5G band*/
	}
	
	if ((value <= REGION_MAXIMUM_BG_BAND) || (value == REGION_32_BG_BAND) || (value == REGION_33_BG_BAND))
	{
		pAd->CommonCfg.CountryRegion = ((UCHAR) value) | 0x80;
	}

	if (value2 <= REGION_MAXIMUM_A_BAND)
	{
		pAd->CommonCfg.CountryRegionForABand = ((UCHAR) value2) | 0x80;
	}
	
	/* Get RSSI Offset on EEPROM 0x9Ah & 0x9Ch.*/
	/* The valid value are (-10 ~ 10) */
	/* */
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_BG_OFFSET, value);
		pAd->BGRssiOffset[0] = value & 0x00ff;
		pAd->BGRssiOffset[1] = (value >> 8);
	}

#ifdef MT7601
	if (IS_MT7601(pAd))
	{
		; // MT7601 not support BGRssiOffset[2]
	}
	else
#endif /* MT7601 */
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_BG_OFFSET+2, value);
/*		if (IS_RT2860(pAd))  RT2860 supports 3 Rx and the 2.4 GHz RSSI #2 offset is in the EEPROM 0x48*/
			pAd->BGRssiOffset[2] = value & 0x00ff;
		pAd->ALNAGain1 = (value >> 8);
	}

#ifdef RT65xx
	if (IS_RT6590(pAd))
	{
		/*
			MT7650 E1 doesn't support external LNA.
			2012/04/12
		*/
		; // TODO: wait TC6008 EEPROM format
	}
	else
#endif /* RT65xx */
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_LNA_OFFSET, value);
		pAd->BLNAGain = value & 0x00ff;
		pAd->ALNAGain0 = (value >> 8);
	}
	

#ifdef MT7601
	if (IS_MT7601(pAd))
	{
		;	// MT7601 not support A Band
	}
	else
#endif /* MT7601 */
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_A_OFFSET, value);
		pAd->ARssiOffset[0] = value & 0x00ff;
		pAd->ARssiOffset[1] = (value >> 8);
	}

#ifdef MT7601
	if (IS_MT7601(pAd))
	{
		;	// MT7601 not support A Band
	}
	else
#endif /* MT7601 */
	{
		RT28xx_EEPROM_READ16(pAd, (EEPROM_RSSI_A_OFFSET+2), value);
		pAd->ARssiOffset[2] = value & 0x00ff;
		pAd->ALNAGain2 = (value >> 8);
	}


	if (((UCHAR)pAd->ALNAGain1 == 0xFF) || (pAd->ALNAGain1 == 0x00))
		pAd->ALNAGain1 = pAd->ALNAGain0;
	if (((UCHAR)pAd->ALNAGain2 == 0xFF) || (pAd->ALNAGain2 == 0x00))
		pAd->ALNAGain2 = pAd->ALNAGain0;

	/* Validate 11a/b/g RSSI 0/1/2 offset.*/
	for (i =0 ; i < 3; i++)
	{
		if ((pAd->BGRssiOffset[i] < -10) || (pAd->BGRssiOffset[i] > 10))
			pAd->BGRssiOffset[i] = 0;

		if ((pAd->ARssiOffset[i] < -10) || (pAd->ARssiOffset[i] > 10))
			pAd->ARssiOffset[i] = 0;
	}

	
#ifdef LED_CONTROL_SUPPORT
	/* LED Setting */
	RTMPGetLEDSetting(pAd);
#endif /* LED_CONTROL_SUPPORT */
		
		RTMPReadTxPwrPerRate(pAd);

#ifdef SINGLE_SKU
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_DEFINE_MAX_TXPWR, pAd->CommonCfg.DefineMaxTxPwr);
	}

	/*
		Some dongle has old EEPROM value, use ModuleTxpower for saving correct value fo DefineMaxTxPwr.
		ModuleTxpower will override DefineMaxTxPwr (value from EEPROM) if ModuleTxpower is not zero.
	*/
	if (pAd->CommonCfg.ModuleTxpower > 0) 
		pAd->CommonCfg.DefineMaxTxPwr = pAd->CommonCfg.ModuleTxpower;

	DBGPRINT(RT_DEBUG_TRACE, ("TX Power set for SINGLE SKU MODE is : 0x%04x \n", pAd->CommonCfg.DefineMaxTxPwr));
	
	pAd->CommonCfg.bSKUMode = FALSE;
	if ((pAd->CommonCfg.DefineMaxTxPwr & 0xFF) <= 0x50)
	{
		if (IS_RT3883(pAd))
			pAd->CommonCfg.bSKUMode = TRUE;
		else if ((pAd->CommonCfg.AntGain > 0) && (pAd->CommonCfg.BandedgeDelta >= 0))
			pAd->CommonCfg.bSKUMode = TRUE;
	}
	DBGPRINT(RT_DEBUG_TRACE, ("Single SKU Mode is %s\n", 
				pAd->CommonCfg.bSKUMode ? "Enable" : "Disable"));
#endif /* SINGLE_SKU */

#ifdef SINGLE_SKU_V2
	InitSkuRateDiffTable(pAd);
#endif /* SINGLE_SKU_V2 */


#ifdef RTMP_INTERNAL_TX_ALC
#ifdef RT65xx
	if (IS_RT6590(pAd))
	{
		; // TODO: wait TC6008 EEPROM format
	}
	else
#endif /* RT65xx */
	{
		/*
			Internal Tx ALC support is starting from RT3370 / RT3390, which combine PA / LNA in single chip.
			The old chipset don't have this, add new feature flag RTMP_INTERNAL_TX_ALC.
		*/
		RT28xx_EEPROM_READ16(pAd, EEPROM_NIC2_OFFSET, value);
		if (value == 0xFFFF) /*EEPROM is empty*/
		   	pAd->TxPowerCtrl.bInternalTxALC = FALSE;
		else if (value & 1<<13) 
		   	pAd->TxPowerCtrl.bInternalTxALC = TRUE;
		else 
		   	pAd->TxPowerCtrl.bInternalTxALC = FALSE;

	}
	DBGPRINT(RT_DEBUG_TRACE, ("TXALC> bInternalTxALC = %d\n", pAd->TxPowerCtrl.bInternalTxALC));
#endif /* RTMP_INTERNAL_TX_ALC */



#ifdef RT3290
	if (IS_RT3290(pAd))
		RT3290_eeprom_access_grant(pAd, FALSE);
#endif /* RT3290 */

	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->Antenna.field.BoardType = %d, IS_MINI_CARD(pAd) = %d, IS_RT5390U(pAd) = %d\n", 
		__FUNCTION__,
		pAd->Antenna.field.BoardType,
		IS_MINI_CARD(pAd),
		IS_RT5390U(pAd)));

	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICReadEEPROMParameters\n"));
}


/*
	========================================================================
	
	Routine Description:
		Set default value from EEPROM
		
	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL
	
	Note:
		
	========================================================================
*/
VOID	NICInitAsicFromEEPROM(
	IN	PRTMP_ADAPTER	pAd)
{
#ifdef RALINK_ATE
	USHORT value;
#endif /* RALINK_ATE */
	EEPROM_NIC_CONFIG2_STRUC NicConfig2;
	
	DBGPRINT(RT_DEBUG_TRACE, ("--> NICInitAsicFromEEPROM\n"));

#if !defined(RT65xx) && !defined(MT7601)
	for(USHORT i = EEPROM_BBP_ARRAY_OFFSET; i < NUM_EEPROM_BBP_PARMS; i++)
	{
		UCHAR BbpRegIdx, BbpValue;
	
		if ((pAd->EEPROMDefaultValue[i] != 0xFFFF) && (pAd->EEPROMDefaultValue[i] != 0))
		{
			BbpRegIdx = (UCHAR)(pAd->EEPROMDefaultValue[i] >> 8);
			BbpValue  = (UCHAR)(pAd->EEPROMDefaultValue[i] & 0xff);
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BbpRegIdx, BbpValue);
		}
	}
#endif /* #if !defined(RT65xx) && !defined(MT7601) */

	NicConfig2.word = pAd->NicConfig2.word;

	/* finally set primary ant */
	AntCfgInit(pAd);

	RTMP_CHIP_ASIC_INIT_TEMPERATURE_COMPENSATION(pAd);

	if (pAd->chipOps.AsicRfInit)
		pAd->chipOps.AsicRfInit(pAd);


#ifdef RTMP_MAC_USB
		if (IS_RT30xx(pAd)|| IS_RT3572(pAd))
		{
			RTMP_CHIP_OP *pChipOps = &pAd->chipOps;
			if (pChipOps->AsicReverseRfFromSleepMode)
				pChipOps->AsicReverseRfFromSleepMode(pAd, TRUE);
		}
#endif /* RTMP_MAC_USB */
	/* Turn off patching for cardbus controller*/
	if (NicConfig2.field.CardbusAcceleration == 1)
	{
/*		pAd->bTest1 = TRUE;*/
	}

	if (NicConfig2.field.DynamicTxAgcControl == 1)
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = TRUE;
	else
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = FALSE;

#ifdef RTMP_INTERNAL_TX_ALC
	/*
	    Internal Tx ALC support is starting from RT3370 / RT3390, which combine PA / LNA in single chip.
	    The old chipset don't have this, add new feature flag RTMP_INTERNAL_TX_ALC.
	 */

	/* Internal Tx ALC */
	if (((NicConfig2.field.DynamicTxAgcControl == 1) && 
            (NicConfig2.field.bInternalTxALC == 1)) ||
            ((!IS_RT3390(pAd)) && (!IS_RT3350(pAd)) &&
            (!IS_RT3352(pAd)) && (!IS_RT5350(pAd)) && (!IS_RT5390(pAd)) && (!IS_RT3290(pAd)) && (!IS_MT7601(pAd))))
	{
		/*
			If both DynamicTxAgcControl and bInternalTxALC are enabled,
			it is a wrong configuration.
			If the chipset does not support Internal TX ALC, we shall disable it.
		*/
			pAd->TxPowerCtrl.bInternalTxALC = FALSE;
	}
	else
	{
		if (NicConfig2.field.bInternalTxALC == 1)
		{
			pAd->TxPowerCtrl.bInternalTxALC = TRUE;
		}
		else
		{
			pAd->TxPowerCtrl.bInternalTxALC = FALSE;
		}
	}

	
	/* Old 5390 NIC always disables the internal ALC */
	
	if (pAd->MACVersion == 0x53900501)
	{
		pAd->TxPowerCtrl.bInternalTxALC = FALSE;
	}

	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPowerCtrl.bInternalTxALC = %d\n", 
		__FUNCTION__, 
		pAd->TxPowerCtrl.bInternalTxALC));
#endif /* RTMP_INTERNAL_TX_ALC */

#ifdef RALINK_ATE
	RT28xx_EEPROM_READ16(pAd, EEPROM_TSSI_GAIN_AND_ATTENUATION, value);
	value = (value & 0x00FF);	
	
	if (IS_RT5390(pAd))
	{
		pAd->TssiGain = 0x02;	 /* RT5390 uses 2 as TSSI gain/attenuation default value */
	}
	else
	{
		pAd->TssiGain = 0x03; /* RT5392 uses 3 as TSSI gain/attenuation default value */
	}	
	
	if ((value != 0x00) && (value != 0xFF))
	{
		pAd->TssiGain =  (UCHAR) (value & 0x000F);
	}
	
	DBGPRINT(RT_DEBUG_TRACE, ("%s: EEPROM_TSSI_GAIN_AND_ATTENUATION = 0x%X, pAd->TssiGain=0x%x\n", 
				__FUNCTION__, 
				value, 
				pAd->TssiGain));
#endif // RALINK_ATE //

	rtmp_bbp_set_rxpath(pAd, pAd->Antenna.field.RxPath);
	

	RTMP_EEPROM_ASIC_INIT(pAd);


	DBGPRINT(RT_DEBUG_TRACE, ("TxPath = %d, RxPath = %d, RFIC=%d\n", 
				pAd->Antenna.field.TxPath, pAd->Antenna.field.RxPath, pAd->RfIcType));
	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICInitAsicFromEEPROM\n"));
}


#ifdef DBG
VOID dump_pdma_reg(RTMP_ADAPTER *pAd)
{
	

}
#endif /* DBG */




VOID AsicInitBcnBuf(IN RTMP_ADAPTER *pAd)
{
	int idx;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	
	
	for (idx = 0; idx < pChipCap->BcnMaxHwNum; idx++)
		pAd->BeaconOffset[idx] = pChipCap->BcnBase[idx];

	DBGPRINT(RT_DEBUG_TRACE, ("< Beacon Information: >\n"));
	DBGPRINT(RT_DEBUG_TRACE, ("\tFlgIsSupSpecBcnBuf = %s\n", pChipCap->FlgIsSupSpecBcnBuf ? "TRUE" : "FALSE"));
	DBGPRINT(RT_DEBUG_TRACE, ("\tBcnMaxHwNum = %d\n", pChipCap->BcnMaxHwNum));
	DBGPRINT(RT_DEBUG_TRACE, ("\tBcnMaxNum = %d\n", pChipCap->BcnMaxNum));
	DBGPRINT(RT_DEBUG_TRACE, ("\tBcnMaxHwSize = 0x%x\n", pChipCap->BcnMaxHwSize));
	DBGPRINT(RT_DEBUG_TRACE, ("\tWcidHwRsvNum = %d\n", pChipCap->WcidHwRsvNum));
	for (idx = 0; idx < pChipCap->BcnMaxHwNum; idx++) {
		DBGPRINT(RT_DEBUG_TRACE, ("\t\tBcnBase[%d] = 0x%x, pAd->BeaconOffset[%d]=0x%x\n",
					idx, pChipCap->BcnBase[idx], idx, pAd->BeaconOffset[idx]));
	}


#ifdef RLT_MAC
	{
		RTMP_REG_PAIR bcn_mac_reg_tb[] = {
			{BCN_OFFSET0, 0x18100800},
			{BCN_OFFSET1, 0x38302820},
			{BCN_OFFSET2, 0x58504840},
			{BCN_OFFSET3, 0x78706860},
		};
		for (idx = 0; idx < 4; idx ++)
		{
			RTMP_IO_WRITE32(pAd, (USHORT)bcn_mac_reg_tb[idx].Register, 
									bcn_mac_reg_tb[idx].Value);
		}
	}
#endif /* RLT_MAC */

}


/*
	========================================================================
	
	Routine Description:
		Initialize NIC hardware

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
NDIS_STATUS	NICInitializeAdapter(
	IN	PRTMP_ADAPTER	pAd,
	IN   BOOLEAN    bHardReset)
{
	NDIS_STATUS     Status = NDIS_STATUS_SUCCESS;
	ULONG j=0;


	DBGPRINT(RT_DEBUG_TRACE, ("--> NICInitializeAdapter\n"));
	
	/* Set DMA global configuration except TX_DMA_EN and RX_DMA_EN bits */
retry:

	if (AsicWaitPDMAIdle(pAd, 100, 1000) != TRUE) {
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))
			return NDIS_STATUS_FAILURE;
	}

	

	/* Initialze ASIC for TX & Rx operation*/
	if (NICInitializeAsic(pAd , bHardReset) != NDIS_STATUS_SUCCESS)
	{
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))
			return NDIS_STATUS_FAILURE;

		if (pAd->chipOps.loadFirmware)
		{
			if (j++ == 0)
			{
				NICLoadFirmware(pAd);
				goto retry;
			}
		}
		return NDIS_STATUS_FAILURE;
	}



	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICInitializeAdapter\n"));
	return Status;
}

/*
	========================================================================
	
	Routine Description:
		Initialize ASIC

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
NDIS_STATUS	NICInitializeAsic(
	IN	PRTMP_ADAPTER	pAd,
	IN  BOOLEAN		bHardReset)
{
	ULONG			Index = 0;
	UINT32			MACValue = 0;
#ifdef RTMP_MAC_USB
	UINT32			Counter = 0;
	USB_DMA_CFG_STRUC UsbCfg;
#endif /* RTMP_MAC_USB */
#if !(defined(RTMP_MAC_USB) && defined(RLT_MAC))
	USHORT			KeyIdx;
#endif

#ifdef RLT_MAC
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
#endif

	DBGPRINT(RT_DEBUG_TRACE, ("--> NICInitializeAsic\n"));



#ifdef RTMP_MAC_USB
	/* Make sure MAC gets ready after NICLoadFirmware().*/
	
	Index = 0;
	
	/*To avoid hang-on issue when interface up in kernel 2.4, */
	/*we use a local variable "MacCsr0" instead of using "pAd->MACVersion" directly.*/
	if (WaitForAsicReady(pAd) != TRUE)
			return NDIS_STATUS_FAILURE;

	// TODO: shiang, how about the value setting of pAd->MACVersion?? Original it assigned here

	DBGPRINT(RT_DEBUG_TRACE, ("%s():MACVersion[Ver:Rev=0x%08x]\n",
			__FUNCTION__, pAd->MACVersion));


	if ( !IS_MT7601(pAd) )
	{
	/* turn on bit13 (set to zero) after rt2860D. This is to solve high-current issue.*/
	RTMP_IO_READ32(pAd, PBF_SYS_CTRL, &MACValue);
	MACValue &= (~0x2000);
	RTMP_IO_WRITE32(pAd, PBF_SYS_CTRL, MACValue);
	}
	
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x3);
	RTMP_IO_WRITE32(pAd, USB_DMA_CFG, 0x0);

#ifdef RTMP_MAC
	RTUSBVenderReset(pAd);
#endif
	RTMPusecDelay(1000); 
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x0);
#endif /* RTMP_MAC_USB */

#ifdef RTMP_MAC_USB
	UsbCfg.word = 0;
	
	/* USB1.1 do not use bulk in aggregation */
	if (pAd->BulkInMaxPacketSize == 512)
		UsbCfg.field.RxBulkAggEn = 1;

	/* for last packet, PBF might use more than limited, so minus 2 to prevent from error */
		UsbCfg.field.RxBulkAggLmt = (MAX_RXBULK_SIZE /1024)-3;
	UsbCfg.field.RxBulkAggTOut = 0x80; 

	UsbCfg.field.RxBulkEn = 1;
	UsbCfg.field.TxBulkEn = 1;
	
	RTMP_IO_WRITE32(pAd, USB_DMA_CFG, UsbCfg.word);

#ifdef MT7601
	if ( IS_MT7601(pAd) )
	{
		UsbCfg.field.UDMA_RX_WL_DROP = 1;
		RTMP_IO_WRITE32(pAd, USB_DMA_CFG, UsbCfg.word);
	
		UsbCfg.field.UDMA_RX_WL_DROP = 0;
		RTMP_IO_WRITE32(pAd, USB_DMA_CFG, UsbCfg.word);

	}
	
#endif /* MT7601 */

#ifdef RLT_MAC
	/* Select Q2 to receive command response */
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
	AndesFunSetOP(pAd, Q_SELECT, pChipCap->CmdRspRxRing);
	RTUSBBulkCmdRspEventReceive(pAd);

	RTUSBBulkReceive(pAd);
#endif /* RLT_MAC */
#endif /* RTMP_MAC_USB */

#if defined(RTMP_MAC_USB) && defined(RLT_MAC)

	AndesRandomWritePair(pAd, MACRegTable, NUM_MAC_REG_PARMS);
#else

	/* Initialize MAC register to default value*/
	for(Index=0; Index<NUM_MAC_REG_PARMS; Index++)
	{
		RTMP_IO_WRITE32(pAd, (USHORT)MACRegTable[Index].Register, MACRegTable[Index].Value);
	}
#endif /* defined(RTMP_MAC_USB) && defined(RLT_MAC) */

	AsicInitBcnBuf(pAd);

	/* re-set specific MAC registers for individual chip */
	if (pAd->chipOps.AsicMacInit != NULL)
		pAd->chipOps.AsicMacInit(pAd);

	
	/* Before program BBP, we need to wait BBP/RF get wake up.*/
	Index = 0;
	do
	{
		RTMP_IO_READ32(pAd, MAC_STATUS_CFG, &MACValue);

		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))			
			return NDIS_STATUS_FAILURE;

		if ((MACValue & 0x03) == 0)	/* if BB.RF is stable*/
			break;
		
		DBGPRINT(RT_DEBUG_TRACE, ("Check MAC_STATUS_CFG  = Busy = %x\n", MACValue));
		RTMPusecDelay(1000);
	} while (Index++ < 100);


#ifdef RTMP_MAC_USB
	if ( !IS_MT7601(pAd) )
	{
	/* The commands to firmware should be after these commands, these commands will init firmware*/
	/* PCI and USB are not the same because PCI driver needs to wait for PCI bus ready*/
		RTUSBWriteMACRegister(pAd, H2M_BBP_AGENT, 0, FALSE); /* initialize BBP R/W access agent. */
		RTUSBWriteMACRegister(pAd,H2M_MAILBOX_CSR,0, FALSE);
		RTUSBWriteMACRegister(pAd, H2M_INT_SRC, 0, FALSE);
		AsicSendCommandToMcu(pAd, 0x72, 0x00, 0x00, 0x00, FALSE); /* reset rf by MCU supported by new firmware */
	}
#endif /* RTMP_MAC_USB */

	/* Wait to be stable.*/
	RTMPusecDelay(1000);
	pAd->LastMCUCmd = 0x72;

	NICInitBBP(pAd);
	

	if ((IS_RT3883(pAd)) || IS_RT65XX(pAd) || IS_MT7601(pAd) ||
		((pAd->MACVersion >= RALINK_2880E_VERSION) &&
		(pAd->MACVersion < RALINK_3070_VERSION))) /* 3*3*/
	{
		/* enlarge MAX_LEN_CFG*/
		UINT32 csr;
		RTMP_IO_READ32(pAd, MAX_LEN_CFG, &csr);
#if defined(RT2883) || defined(RT3883) || defined(RT3593) || defined(RT65xx) || defined(MT7601)
		if (IS_RT2883(pAd) || IS_RT3883(pAd) || IS_RT3593(pAd) || IS_RT65XX(pAd) || IS_MT7601(pAd)) 
		{
			csr |= 0x3fff;
		}
		else
#endif /* defined(RT2883) || defined(RT3883) || defined(RT3593) */
		{
			csr &= 0xFFF;
			csr |= 0x2000;
		}
		RTMP_IO_WRITE32(pAd, MAX_LEN_CFG, csr);
	}

#ifdef RTMP_MAC_USB
#ifdef RLT_MAC
	{
	UINT32 MACValue[128 * 2];

	for (Index = 0; Index < 128 * 2; Index+=2)
	{
		MACValue[Index] = 0xffffffff;
		MACValue[Index + 1] = 0x00ffffff;
	}

	AndesBurstWrite(pAd, MAC_WCID_BASE, MACValue, 128 * 2);

	}
#else
	{
	UCHAR	MAC_Value[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0,0};

	/*Initialize WCID table*/
	for(Index =0 ;Index < 254;Index++)
	{
		RTUSBMultiWrite(pAd, (USHORT)(MAC_WCID_BASE + Index * 8), MAC_Value, 8, FALSE);
	}
	}

#endif /* RLT_MAC */
#endif /* RTMP_MAC_USB */


	/* Clear raw counters*/
	NicResetRawCounters(pAd);
	
	/* ASIC will keep garbage value after boot*/
	/* Clear all shared key table when initial*/
	/* This routine can be ignored in radio-ON/OFF operation. */
	if (bHardReset)
	{
#if defined(RTMP_MAC_USB) && defined(RLT_MAC)
		{
			UINT32 MACValue[4];

			for (Index = 0; Index < 4; Index++)
				MACValue[Index] = 0;

			AndesBurstWrite(pAd, SHARED_KEY_MODE_BASE, MACValue, 4);
		}

		/* Clear all pairwise key table when initial*/
		{
			UINT32 MACValue[256];

			for (Index = 0; Index < 256; Index++)
				MACValue[Index] = 1;

			AndesBurstWrite(pAd, MAC_WCID_ATTRIBUTE_BASE, MACValue, 256);
		}
#else

		printk("SHARED_KEY_MODE_BASE = %x\n", SHARED_KEY_MODE_BASE);
		printk("MAC_WCID_ATTRIBUTE_BASE = %x\n", MAC_WCID_ATTRIBUTE_BASE);
		for (KeyIdx = 0; KeyIdx < 4; KeyIdx++)
		{
			RTMP_IO_WRITE32(pAd, SHARED_KEY_MODE_BASE + 4*KeyIdx, 0);
		}

		/* Clear all pairwise key table when initial*/
		for (KeyIdx = 0; KeyIdx < 256; KeyIdx++)
		{
			RTMP_IO_WRITE32(pAd, MAC_WCID_ATTRIBUTE_BASE + (KeyIdx * HW_WCID_ATTRI_SIZE), 1);
		}
#endif /* defined(RTMP_MAC_USB) && defined(RLT_MAC) */
	}

	/* assert HOST ready bit*/
/*  RTMP_IO_WRITE32(pAd, MAC_CSR1, 0x0);  2004-09-14 asked by Mark*/
/*  RTMP_IO_WRITE32(pAd, MAC_CSR1, 0x4);*/

	/* It isn't necessary to clear this space when not hard reset. */
	if (bHardReset == TRUE)
	{
		/* clear all on-chip BEACON frame space */
#ifdef CONFIG_AP_SUPPORT
		INT	i, apidx;
		for (apidx = 0; apidx < HW_BEACON_MAX_COUNT(pAd); apidx++)
		{
			if (pAd->BeaconOffset[apidx] > 0) {
				// TODO: shiang-6590, if we didn't define MBSS_SUPPORT, the pAd->BeaconOffset[x] may set as 0 when chipCap.BcnMaxHwNum != HW_BEACON_MAX_COUNT
				for (i = 0; i < HW_BEACON_OFFSET; i+=4)
					RTMP_CHIP_UPDATE_BEACON(pAd, pAd->BeaconOffset[apidx] + i, 0x00, 4); 
			}
#ifdef RTMP_MAC_USB
			IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
			{
				if (pAd->CommonCfg.pBeaconSync)
					pAd->CommonCfg.pBeaconSync->BeaconBitMap &= (~(BEACON_BITMAP_MASK & (1 << apidx)));
			}
#endif /* RTMP_MAC_USB */
		}
#endif /* CONFIG_AP_SUPPORT */
	}
	
#ifdef RTMP_MAC_USB
	AsicDisableSync(pAd);

	/* Clear raw counters*/
	NicResetRawCounters(pAd);

	/* Default PCI clock cycle per ms is different as default setting, which is based on PCI.*/
	RTMP_IO_READ32(pAd, USB_CYC_CFG, &Counter);
	Counter&=0xffffff00;
	Counter|=0x000001e;
	RTMP_IO_WRITE32(pAd, USB_CYC_CFG, Counter);
#endif /* RTMP_MAC_USB */


#ifdef RT3290
	if (IS_RT3290(pAd))
	{
		UINT32 coex_val;
		//halt wlan tx when bt_rx_busy asserted
		RTMP_IO_READ32(pAd, COEXCFG2, &coex_val);
		coex_val |= 0x100;
		RTMP_IO_WRITE32(pAd, COEXCFG2, coex_val);
	}
#endif /* RT3290 */

	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICInitializeAsic\n"));
	return NDIS_STATUS_SUCCESS;
}




#ifdef CONFIG_AP_SUPPORT
#endif /* CONFIG_AP_SUPPORT */

VOID NICUpdateFifoStaCounters(
	IN PRTMP_ADAPTER pAd)
{
	TX_STA_FIFO_STRUC	StaFifo;
	MAC_TABLE_ENTRY		*pEntry = NULL;
	UINT32				i = 0;
	UCHAR				pid = 0, wcid = 0;
	INT32				reTry;
	UCHAR				succMCS;

#ifdef RALINK_ATE		
	/* Nothing to do in ATE mode */
	if (ATE_ON(pAd))
		return;
#endif /* RALINK_ATE */

#ifdef RTMP_MAC_USB
#endif /* RTMP_MAC_USB */


#ifdef RT65xx
	// TODO: shiang-6590, for 8592 now we have tx-status report packet from hardware!!
	if (IS_RT65XX(pAd))
		return;
#endif /* RT65xx */


		do
		{
			RTMP_IO_READ32(pAd, TX_STA_FIFO, &StaFifo.word);

			if (StaFifo.field.bValid == 0)
				break;
		
			wcid = (UCHAR)StaFifo.field.wcid;

#ifdef DBG_CTRL_SUPPORT
#ifdef INCLUDE_DEBUG_QUEUE
		if (pAd->CommonCfg.DebugFlags & DBF_DBQ_TXFIFO) {
			dbQueueEnqueue(0x73, (UCHAR *)(&StaFifo.word));
		}
#endif /* INCLUDE_DEBUG_QUEUE */
#endif /* DBG_CTRL_SUPPORT */

		/* ignore NoACK and MGMT frame use 0xFF as WCID */
			if ((StaFifo.field.TxAckRequired == 0) || (wcid >= MAX_LEN_OF_MAC_TABLE))
			{
				i++;
				continue;
			}

			/* PID store Tx MCS Rate */
			pid = (UCHAR)StaFifo.field.PidType;

			pEntry = &pAd->MacTab.Content[wcid];

			pEntry->DebugFIFOCount++;


#ifdef DOT11_N_SUPPORT
#ifdef TXBF_SUPPORT
			/* Update BF statistics*/
			if (pAd->chipCap.FlgHwTxBfCap)
			{
				int succMCS = (StaFifo.field.SuccessRate & 0x7F);
				int origMCS = pid;

				if (succMCS==32)
					origMCS = 32;
#ifdef DOT11N_SS3_SUPPORT
				if (succMCS>origMCS && pEntry->HTCapability.MCSSet[2]==0xff)
					origMCS += 16;
#endif /* DOT11N_SS3_SUPPORT */

				if (succMCS>origMCS)
					origMCS = succMCS+1;

				/* MCS16 falls back to MCS8*/
				if (origMCS>=16 && succMCS<=8)
					succMCS += 8;

				/* MCS8 falls back to 0 */
				if (origMCS >= 8 && succMCS == 0)
					succMCS += 7;

				reTry = origMCS-succMCS;

				if (StaFifo.field.eTxBF) {
					if (StaFifo.field.TxSuccess)
						pEntry->TxBFCounters.ETxSuccessCount++;
					else
						pEntry->TxBFCounters.ETxFailCount++;
					pEntry->TxBFCounters.ETxRetryCount += reTry;
				}
				else if (StaFifo.field.iTxBF) {
					if (StaFifo.field.TxSuccess)
						pEntry->TxBFCounters.ITxSuccessCount++;
					else
						pEntry->TxBFCounters.ITxFailCount++;
					pEntry->TxBFCounters.ITxRetryCount += reTry;
				}
				else {
					if (StaFifo.field.TxSuccess)
						pEntry->TxBFCounters.TxSuccessCount++;
					else
						pEntry->TxBFCounters.TxFailCount++;
					pEntry->TxBFCounters.TxRetryCount += reTry;
				}
			}
#endif /* TXBF_SUPPORT */
#endif /* DOT11_N_SUPPORT */

#ifdef UAPSD_SUPPORT
			UAPSD_SP_AUE_Handle(pAd, pEntry, StaFifo.field.TxSuccess);
#endif /* UAPSD_SUPPORT */


			if (!StaFifo.field.TxSuccess)
			{
				pEntry->FIFOCount++;
				pEntry->OneSecTxFailCount++;

				if (pEntry->FIFOCount >= 1)
				{
#ifdef DOT11_N_SUPPORT
					pEntry->NoBADataCountDown = 64;
#endif /* DOT11_N_SUPPORT */


					/* Update the continuous transmission counter.*/
					pEntry->ContinueTxFailCnt++;

					if(pEntry->PsMode == PWR_ACTIVE)
					{
#ifdef DOT11_N_SUPPORT
						int tid;
						for (tid=0; tid<NUM_OF_TID; tid++)
							BAOriSessionTearDown(pAd, pEntry->Aid,  tid, FALSE, FALSE);
#endif /* DOT11_N_SUPPORT */

#ifdef WDS_SUPPORT
						/* fix WDS Jam issue*/
						if(IS_ENTRY_WDS(pEntry)
							&& (pEntry->LockEntryTx == FALSE)
							&& (pEntry->ContinueTxFailCnt >= pAd->ApCfg.EntryLifeCheck))
						{ 
							DBGPRINT(RT_DEBUG_TRACE, ("Entry %02x:%02x:%02x:%02x:%02x:%02x Blocked!! (Fail Cnt = %d)\n",
								PRINT_MAC(pEntry->Addr), pEntry->ContinueTxFailCnt ));

							pEntry->LockEntryTx = TRUE;
						}
#endif /* WDS_SUPPORT */
					}
				}
#ifdef CONFIG_AP_SUPPORT
#endif /* CONFIG_AP_SUPPORT */
			}
			else
			{
#ifdef DOT11_N_SUPPORT
				if ((pEntry->PsMode != PWR_SAVE) && (pEntry->NoBADataCountDown > 0))
				{
					pEntry->NoBADataCountDown--;
					if (pEntry->NoBADataCountDown==0)
					{
						DBGPRINT(RT_DEBUG_TRACE, ("@\n"));
					}
				}
#endif /* DOT11_N_SUPPORT */
				pEntry->FIFOCount = 0;
				pEntry->OneSecTxNoRetryOkCount++;


				/* update NoDataIdleCount when sucessful send packet to STA.*/
				pEntry->NoDataIdleCount = 0;
				pEntry->ContinueTxFailCnt = 0;
#ifdef WDS_SUPPORT
				pEntry->LockEntryTx = FALSE;
#endif /* WDS_SUPPORT */

			}

			succMCS = StaFifo.field.SuccessRate & 0x7F;
#ifdef DOT11N_SS3_SUPPORT
			if (pEntry->HTCapability.MCSSet[2] == 0xff)
			{
				if (succMCS > pid)
					pid = pid + 16;
			}
#endif /* DOT11N_SS3_SUPPORT */

			if (StaFifo.field.TxSuccess)
			{
				pEntry->TXMCSExpected[pid]++;
				if (pid == succMCS)
					pEntry->TXMCSSuccessful[pid]++;
				else 
					pEntry->TXMCSAutoFallBack[pid][succMCS]++;
			}
			else
			{
				pEntry->TXMCSFailed[pid]++;
			}

#ifdef DOT11N_SS3_SUPPORT
			if (pid >= 16 && succMCS <= 8)
				succMCS += (2 - (succMCS >> 3)) * 7;
#endif /* DOT11N_SS3_SUPPORT */

			reTry = pid - succMCS;

			if (reTry > 0)
			{
				/* MCS8 falls back to 0 */
				if (pid>=8 && succMCS==0)
					reTry -= 7;
				else if ((pid >= 12) && succMCS <=7)
					reTry -= 4;

				pEntry->OneSecTxRetryOkCount += reTry;
			}

			i++;	/* ASIC store 16 stack*/
		} while ( i < (TX_RING_SIZE<<1) );

}


#ifdef FIFO_EXT_SUPPORT
BOOLEAN NicGetMacFifoTxCnt(
	IN RTMP_ADAPTER *pAd,
	IN MAC_TABLE_ENTRY *pEntry)
{
	if (pEntry->Aid >= 1 && pEntry->Aid <= 8)
	{
		WCID_TX_CNT_STRUC wcidTxCnt;
		UINT32 regAddr;
		
		regAddr = WCID_TX_CNT_0 + (pEntry->Aid - 1) * 4;
		RTMP_IO_READ32(pAd, regAddr, &wcidTxCnt.word);

		pEntry->fifoTxSucCnt += wcidTxCnt.field.succCnt;
		pEntry->fifoTxRtyCnt += wcidTxCnt.field.reTryCnt;
	}

	return TRUE;
}


VOID AsicFifoExtSet(IN RTMP_ADAPTER *pAd)
{
	if (pAd->chipCap.FlgHwFifoExtCap)
	{
		RTMP_IO_WRITE32(pAd, WCID_MAPPING_0, 0x04030201);
		RTMP_IO_WRITE32(pAd, WCID_MAPPING_1, 0x08070605);
	}
}


VOID AsicFifoExtEntryClean(
	IN RTMP_ADAPTER * pAd, 
	IN MAC_TABLE_ENTRY *pEntry)
{
	WCID_TX_CNT_STRUC wcidTxCnt;
	UINT32 regAddr;
			
	if (pAd->chipCap.FlgHwFifoExtCap)
	{
		/* We clean the fifo info when MCS is 0 and Aid is from 1~8 */
		if (pEntry->Aid >=1  && pEntry->Aid <= 8)
		{		
			regAddr = WCID_TX_CNT_0 + (pEntry->Aid - 1) * 4;
			RTMP_IO_READ32(pAd, regAddr, &wcidTxCnt.word);
		}
	}
}
#endif /* FIFO_EXT_SUPPORT */


/*
	========================================================================
	
	Routine Description:
		Read Tx statistic raw counters from hardware registers and record to
		related software variables for later on query

	Arguments:
		pAd					Pointer to our adapter
		pStaTxCnt0			Pointer to record "TX_STA_CNT0" (0x170c)
		pStaTxCnt1			Pointer to record "TX_STA_CNT1" (0x1710)

	Return Value:
		None

	========================================================================
*/
VOID NicGetTxRawCounters(
	IN RTMP_ADAPTER *pAd,
	IN TX_STA_CNT0_STRUC *pStaTxCnt0,
	IN TX_STA_CNT1_STRUC *pStaTxCnt1)
{

	RTMP_IO_READ32(pAd, TX_STA_CNT0, &pStaTxCnt0->word);
	RTMP_IO_READ32(pAd, TX_STA_CNT1, &pStaTxCnt1->word);

	pAd->bUpdateBcnCntDone = TRUE;	/* not appear in Rory's code */
	pAd->RalinkCounters.OneSecBeaconSentCnt += pStaTxCnt0->field.TxBeaconCount;
	pAd->RalinkCounters.OneSecTxRetryOkCount += pStaTxCnt1->field.TxRetransmit;
	pAd->RalinkCounters.OneSecTxNoRetryOkCount += pStaTxCnt1->field.TxSuccess;
	pAd->RalinkCounters.OneSecTxFailCount += pStaTxCnt0->field.TxFailCount;

#ifdef STATS_COUNT_SUPPORT
	pAd->WlanCounters.TransmittedFragmentCount.u.LowPart += pStaTxCnt1->field.TxSuccess;
	pAd->WlanCounters.RetryCount.u.LowPart += pStaTxCnt1->field.TxRetransmit;
	pAd->WlanCounters.FailedCount.u.LowPart += pStaTxCnt0->field.TxFailCount;
#endif /* STATS_COUNT_SUPPORT */


}


/*
	========================================================================
	
	Routine Description:
		Clean all Tx/Rx statistic raw counters from hardware registers

	Arguments:
		pAd					Pointer to our adapter

	Return Value:
		None

	========================================================================
*/
VOID NicResetRawCounters(RTMP_ADAPTER *pAd)
{
	UINT32 Counter;
	
	RTMP_IO_READ32(pAd, RX_STA_CNT0, &Counter);
	RTMP_IO_READ32(pAd, RX_STA_CNT1, &Counter);
	RTMP_IO_READ32(pAd, RX_STA_CNT2, &Counter);
	RTMP_IO_READ32(pAd, TX_STA_CNT0, &Counter);
	RTMP_IO_READ32(pAd, TX_STA_CNT1, &Counter);
	RTMP_IO_READ32(pAd, TX_STA_CNT2, &Counter);
}


/*
	========================================================================
	
	Routine Description:
		Read statistical counters from hardware registers and record them
		in software variables for later on query

	Arguments:
		pAd					Pointer to our adapter

	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	========================================================================
*/
VOID NICUpdateRawCounters(
	IN PRTMP_ADAPTER pAd)
{
	UINT32	OldValue;/*, Value2;*/
	/*ULONG	PageSum, OneSecTransmitCount;*/
	/*ULONG	TxErrorRatio, Retry, Fail;*/
	RX_STA_CNT0_STRUC	RxStaCnt0;
#ifndef MICROWAVE_OVEN_SUPPORT
	RX_STA_CNT1_STRUC	RxStaCnt1;
#endif /* MICROWAVE_OVEN_SUPPORT */
	RX_STA_CNT2_STRUC	RxStaCnt2;
	TX_STA_CNT0_STRUC 	TxStaCnt0;
	TX_STA_CNT1_STRUC	StaTx1;
	TX_STA_CNT2_STRUC	StaTx2;
#ifdef STATS_COUNT_SUPPORT
	TX_NAG_AGG_CNT_STRUC	TxAggCnt;
	TX_AGG_CNT0_STRUC	TxAggCnt0;
	TX_AGG_CNT1_STRUC	TxAggCnt1;
	TX_AGG_CNT2_STRUC	TxAggCnt2;
	TX_AGG_CNT3_STRUC	TxAggCnt3;
	TX_AGG_CNT4_STRUC	TxAggCnt4;
	TX_AGG_CNT5_STRUC	TxAggCnt5;
	TX_AGG_CNT6_STRUC	TxAggCnt6;
	TX_AGG_CNT7_STRUC	TxAggCnt7;
#endif /* STATS_COUNT_SUPPORT */
	COUNTER_RALINK		*pRalinkCounters;


	pRalinkCounters = &pAd->RalinkCounters;
#ifdef RTMP_MAC_USB
#ifdef STATS_COUNT_SUPPORT
	if(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF))
		return;

	if(RTMP_TEST_FLAG(pAd, fRTMP_PS_MCU_SLEEP))
		return;
#endif /* STATS_COUNT_SUPPORT */
#endif /* RTMP_MAC_USB */


	RTMP_IO_READ32(pAd, RX_STA_CNT0, &RxStaCnt0.word);
	RTMP_IO_READ32(pAd, RX_STA_CNT2, &RxStaCnt2.word);

#ifdef CONFIG_AP_SUPPORT
#ifdef CARRIER_DETECTION_SUPPORT
	if ((pAd->CommonCfg.CarrierDetect.Enable == FALSE) || (pAd->OpMode == OPMODE_STA))
#endif /* CARRIER_DETECTION_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */
	{
#ifdef MICROWAVE_OVEN_SUPPORT
		/* Update RX PLCP error counter*/
		pAd->PrivateInfo.PhyRxErrCnt += NICSumPLCPErrCnt(pAd); /* get PLCP error count per sec */
		/* Update False CCA counter*/
		pAd->RalinkCounters.OneSecFalseCCACnt += NICSumFalseCCACnt(pAd); /* get FalseCCA count per sec */
		pAd->RalinkCounters.FalseCCACnt += pAd->RalinkCounters.OneSecFalseCCACnt;
#else
		RTMP_IO_READ32(pAd, RX_STA_CNT1, &RxStaCnt1.word);
		/* Update RX PLCP error counter*/
		pAd->PrivateInfo.PhyRxErrCnt += RxStaCnt1.field.PlcpErr;
		/* Update False CCA counter*/
		pAd->RalinkCounters.OneSecFalseCCACnt += RxStaCnt1.field.FalseCca;
		pAd->RalinkCounters.FalseCCACnt += RxStaCnt1.field.FalseCca;
		
#endif /* MICROWAVE_OVEN_SUPPORT */
	}

#ifdef STATS_COUNT_SUPPORT
	/* Update FCS counters*/
	OldValue= pAd->WlanCounters.FCSErrorCount.u.LowPart;
	pAd->WlanCounters.FCSErrorCount.u.LowPart += (RxStaCnt0.field.CrcErr); /* >> 7);*/
	if (pAd->WlanCounters.FCSErrorCount.u.LowPart < OldValue)
		pAd->WlanCounters.FCSErrorCount.u.HighPart++;
#endif /* STATS_COUNT_SUPPORT */

	/* Add FCS error count to private counters*/
	pRalinkCounters->OneSecRxFcsErrCnt += RxStaCnt0.field.CrcErr;
	OldValue = pRalinkCounters->RealFcsErrCount.u.LowPart;
	pRalinkCounters->RealFcsErrCount.u.LowPart += RxStaCnt0.field.CrcErr;
	if (pRalinkCounters->RealFcsErrCount.u.LowPart < OldValue)
		pRalinkCounters->RealFcsErrCount.u.HighPart++;

	/* Update Duplicate Rcv check*/
	pRalinkCounters->DuplicateRcv += RxStaCnt2.field.RxDupliCount;
#ifdef STATS_COUNT_SUPPORT
	pAd->WlanCounters.FrameDuplicateCount.u.LowPart += RxStaCnt2.field.RxDupliCount;
#endif /* STATS_COUNT_SUPPORT */
	/* Update RX Overflow counter*/
	pAd->Counters8023.RxNoBuffer += (RxStaCnt2.field.RxFifoOverflowCount);
	
	/*pAd->RalinkCounters.RxCount = 0;*/
#ifdef RTMP_MAC_USB
	if (pRalinkCounters->RxCount != pAd->watchDogRxCnt)
	{
		pAd->watchDogRxCnt = pRalinkCounters->RxCount;
		pAd->watchDogRxOverFlowCnt = 0;
	}
	else
	{
		if (RxStaCnt2.field.RxFifoOverflowCount)
			pAd->watchDogRxOverFlowCnt++;
		else
			pAd->watchDogRxOverFlowCnt = 0;
	}
#endif /* RTMP_MAC_USB */

	
	/*if (!OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_TX_RATE_SWITCH_ENABLED) || */
	/*	(OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_TX_RATE_SWITCH_ENABLED) && (pAd->MacTab.Size != 1)))*/
	if (!pAd->bUpdateBcnCntDone)
	{
		/* Update BEACON sent count*/
		NicGetTxRawCounters(pAd, &TxStaCnt0, &StaTx1);
		RTMP_IO_READ32(pAd, TX_STA_CNT2, &StaTx2.word);
	}


	/*if (pAd->bStaFifoTest == TRUE)*/
#ifdef STATS_COUNT_SUPPORT
	{
		RTMP_IO_READ32(pAd, TX_AGG_CNT, &TxAggCnt.word);
	RTMP_IO_READ32(pAd, TX_AGG_CNT0, &TxAggCnt0.word);
	RTMP_IO_READ32(pAd, TX_AGG_CNT1, &TxAggCnt1.word);
	RTMP_IO_READ32(pAd, TX_AGG_CNT2, &TxAggCnt2.word);
	RTMP_IO_READ32(pAd, TX_AGG_CNT3, &TxAggCnt3.word);
		RTMP_IO_READ32(pAd, TX_AGG_CNT4, &TxAggCnt4.word);
		RTMP_IO_READ32(pAd, TX_AGG_CNT5, &TxAggCnt5.word);
		RTMP_IO_READ32(pAd, TX_AGG_CNT6, &TxAggCnt6.word);
		RTMP_IO_READ32(pAd, TX_AGG_CNT7, &TxAggCnt7.word);
		pRalinkCounters->TxAggCount += TxAggCnt.field.AggTxCount;
		pRalinkCounters->TxNonAggCount += TxAggCnt.field.NonAggTxCount;
		pRalinkCounters->TxAgg1MPDUCount += TxAggCnt0.field.AggSize1Count;
		pRalinkCounters->TxAgg2MPDUCount += TxAggCnt0.field.AggSize2Count;
		
		pRalinkCounters->TxAgg3MPDUCount += TxAggCnt1.field.AggSize3Count;
		pRalinkCounters->TxAgg4MPDUCount += TxAggCnt1.field.AggSize4Count;
		pRalinkCounters->TxAgg5MPDUCount += TxAggCnt2.field.AggSize5Count;
		pRalinkCounters->TxAgg6MPDUCount += TxAggCnt2.field.AggSize6Count;
	
		pRalinkCounters->TxAgg7MPDUCount += TxAggCnt3.field.AggSize7Count;
		pRalinkCounters->TxAgg8MPDUCount += TxAggCnt3.field.AggSize8Count;
		pRalinkCounters->TxAgg9MPDUCount += TxAggCnt4.field.AggSize9Count;
		pRalinkCounters->TxAgg10MPDUCount += TxAggCnt4.field.AggSize10Count;

		pRalinkCounters->TxAgg11MPDUCount += TxAggCnt5.field.AggSize11Count;
		pRalinkCounters->TxAgg12MPDUCount += TxAggCnt5.field.AggSize12Count;
		pRalinkCounters->TxAgg13MPDUCount += TxAggCnt6.field.AggSize13Count;
		pRalinkCounters->TxAgg14MPDUCount += TxAggCnt6.field.AggSize14Count;

		pRalinkCounters->TxAgg15MPDUCount += TxAggCnt7.field.AggSize15Count;
		pRalinkCounters->TxAgg16MPDUCount += TxAggCnt7.field.AggSize16Count;

		/* Calculate the transmitted A-MPDU count*/
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += TxAggCnt0.field.AggSize1Count;
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt0.field.AggSize2Count >> 1);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt1.field.AggSize3Count / 3);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt1.field.AggSize4Count >> 2);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt2.field.AggSize5Count / 5);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt2.field.AggSize6Count / 6);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt3.field.AggSize7Count / 7);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt3.field.AggSize8Count >> 3);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt4.field.AggSize9Count / 9);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt4.field.AggSize10Count / 10);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt5.field.AggSize11Count / 11);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt5.field.AggSize12Count / 12);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt6.field.AggSize13Count / 13);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt6.field.AggSize14Count / 14);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt7.field.AggSize15Count / 15);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt7.field.AggSize16Count >> 4);	
	}
#endif /* STATS_COUNT_SUPPORT */			

#ifdef DBG_DIAGNOSE
	{
		RtmpDiagStruct	*pDiag;
		UCHAR			ArrayCurIdx, i;
		
		pDiag = &pAd->DiagStruct;
		ArrayCurIdx = pDiag->ArrayCurIdx;
		
		if (pDiag->inited == 0)
		{
			NdisZeroMemory(pDiag, sizeof(struct _RtmpDiagStrcut_));
			pDiag->ArrayStartIdx = pDiag->ArrayCurIdx = 0;
			pDiag->inited = 1;
		}
		else
		{
			/* Tx*/
			pDiag->TxFailCnt[ArrayCurIdx] = TxStaCnt0.field.TxFailCount;
			pDiag->TxAggCnt[ArrayCurIdx] = TxAggCnt.field.AggTxCount;
			pDiag->TxNonAggCnt[ArrayCurIdx] = TxAggCnt.field.NonAggTxCount;

			pDiag->TxAMPDUCnt[ArrayCurIdx][0] = TxAggCnt0.field.AggSize1Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][1] = TxAggCnt0.field.AggSize2Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][2] = TxAggCnt1.field.AggSize3Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][3] = TxAggCnt1.field.AggSize4Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][4] = TxAggCnt2.field.AggSize5Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][5] = TxAggCnt2.field.AggSize6Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][6] = TxAggCnt3.field.AggSize7Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][7] = TxAggCnt3.field.AggSize8Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][8] = TxAggCnt4.field.AggSize9Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][9] = TxAggCnt4.field.AggSize10Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][10] = TxAggCnt5.field.AggSize11Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][11] = TxAggCnt5.field.AggSize12Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][12] = TxAggCnt6.field.AggSize13Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][13] = TxAggCnt6.field.AggSize14Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][14] = TxAggCnt7.field.AggSize15Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][15] = TxAggCnt7.field.AggSize16Count;

			pDiag->RxCrcErrCnt[ArrayCurIdx] = RxStaCnt0.field.CrcErr;
			
			INC_RING_INDEX(pDiag->ArrayCurIdx,  DIAGNOSE_TIME);
			ArrayCurIdx = pDiag->ArrayCurIdx;
			for (i =0; i < 9; i++)
			{
				pDiag->TxDescCnt[ArrayCurIdx][i]= 0;
				pDiag->TxSWQueCnt[ArrayCurIdx][i] =0;
				pDiag->TxMcsCnt[ArrayCurIdx][i] = 0;
				pDiag->RxMcsCnt[ArrayCurIdx][i] = 0;
			}
			pDiag->TxDataCnt[ArrayCurIdx] = 0;
			pDiag->TxFailCnt[ArrayCurIdx] = 0;
			pDiag->RxDataCnt[ArrayCurIdx] = 0;
			pDiag->RxCrcErrCnt[ArrayCurIdx]  = 0;
/*			for (i = 9; i < 16; i++)*/
			for (i = 9; i < 24; i++) /* 3*3*/
			{
				pDiag->TxDescCnt[ArrayCurIdx][i] = 0;
				pDiag->TxMcsCnt[ArrayCurIdx][i] = 0;
				pDiag->RxMcsCnt[ArrayCurIdx][i] = 0;
}

			if (pDiag->ArrayCurIdx == pDiag->ArrayStartIdx)
				INC_RING_INDEX(pDiag->ArrayStartIdx,  DIAGNOSE_TIME);
		}
		
	}
#endif /* DBG_DIAGNOSE */


}

NDIS_STATUS NICLoadFirmware(
	IN PRTMP_ADAPTER pAd)
{
	NDIS_STATUS	 status = NDIS_STATUS_SUCCESS;

	if (pAd->chipOps.loadFirmware)
		status = pAd->chipOps.loadFirmware(pAd);
	return status;
}


/*
	========================================================================
	
	Routine Description:
		erase 8051 firmware image in MAC ASIC

	Arguments:
		Adapter						Pointer to our adapter

	IRQL = PASSIVE_LEVEL
		
	========================================================================
*/
VOID NICEraseFirmware(
	IN PRTMP_ADAPTER pAd)
{
	if (pAd->chipOps.eraseFirmware)
		pAd->chipOps.eraseFirmware(pAd);
	
}


/*
	========================================================================
	
	Routine Description:
		Compare two memory block

	Arguments:
		pSrc1		Pointer to first memory address
		pSrc2		Pointer to second memory address
		
	Return Value:
		0:			memory is equal
		1:			pSrc1 memory is larger
		2:			pSrc2 memory is larger

	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/
ULONG	RTMPCompareMemory(
	IN	PVOID	pSrc1,
	IN	PVOID	pSrc2,
	IN	ULONG	Length)
{
	PUCHAR	pMem1;
	PUCHAR	pMem2;
	ULONG	Index = 0;

	pMem1 = (PUCHAR) pSrc1;
	pMem2 = (PUCHAR) pSrc2;

	for (Index = 0; Index < Length; Index++)
	{
		if (pMem1[Index] > pMem2[Index])
			return (1);
		else if (pMem1[Index] < pMem2[Index])
			return (2);
	}

	/* Equal*/
	return (0);
}


/*
	========================================================================
	
	Routine Description:
		Zero out memory block

	Arguments:
		pSrc1		Pointer to memory address
		Length		Size

	Return Value:
		None
		
	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/
VOID	RTMPZeroMemory(
	IN	PVOID	pSrc,
	IN	ULONG	Length)
{
	PUCHAR	pMem;
	ULONG	Index = 0;

	pMem = (PUCHAR) pSrc;

	for (Index = 0; Index < Length; Index++)
	{
		pMem[Index] = 0x00;
	}
}


/*
	========================================================================
	
	Routine Description:
		Copy data from memory block 1 to memory block 2

	Arguments:
		pDest		Pointer to destination memory address
		pSrc		Pointer to source memory address
		Length		Copy size
		
	Return Value:
		None

	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/
VOID RTMPMoveMemory(
	OUT	PVOID	pDest,
	IN	PVOID	pSrc,
	IN	ULONG	Length)
{
	PUCHAR	pMem1;
	PUCHAR	pMem2;
	UINT	Index;

	ASSERT((Length==0) || (pDest && pSrc));

	pMem1 = (PUCHAR) pDest;
	pMem2 = (PUCHAR) pSrc;

	for (Index = 0; Index < Length; Index++)
	{
		pMem1[Index] = pMem2[Index];
	}
}

VOID UserCfgExit(
	IN RTMP_ADAPTER *pAd)
{
#ifdef DOT11_N_SUPPORT
	BATableExit(pAd);
#endif /* DOT11_N_SUPPORT */


	NdisFreeSpinLock(&pAd->MacTabLock);
}

/*
	========================================================================
	
	Routine Description:
		Initialize port configuration structure

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
VOID UserCfgInit(RTMP_ADAPTER *pAd)
{
	UINT i;
#ifdef CONFIG_AP_SUPPORT
	UINT j;
#endif /* CONFIG_AP_SUPPORT */
/*	EDCA_PARM DefaultEdcaParm;*/
    UINT key_index, bss_index;
	UINT8 TXWISize = pAd->chipCap.TXWISize;

	DBGPRINT(RT_DEBUG_TRACE, ("--> UserCfgInit\n"));    
	 
	pAd->IndicateMediaState = NdisMediaStateDisconnected; 
	
	/*  part I. intialize common configuration*/
	pAd->CommonCfg.BasicRateBitmap = 0xF;
	pAd->CommonCfg.BasicRateBitmapOld = 0xF;
#ifdef CONFIG_MULTI_CHANNEL
	pAd->Multi_Channel_Enable = FALSE;
#endif /* CONFIG_MULTI_CHANNEL */

#ifdef RTMP_MAC_USB
	pAd->BulkOutReq = 0;
	
	pAd->BulkOutComplete = 0;
	pAd->BulkOutCompleteOther = 0;
	pAd->BulkOutCompleteCancel = 0;
	pAd->BulkInReq = 0;
	pAd->BulkInComplete = 0;
	pAd->BulkInCompleteFail = 0;

	/*pAd->QuickTimerP = 100;*/
	/*pAd->TurnAggrBulkInCount = 0;*/
	pAd->bUsbTxBulkAggre = 0;

#ifdef LED_CONTROL_SUPPORT
	/* init as unsed value to ensure driver will set to MCU once.*/
	pAd->LedCntl.LedIndicatorStrength = 0xFF;
#endif /* LED_CONTROL_SUPPORT */

	pAd->CommonCfg.MaxPktOneTxBulk = 2;
	pAd->CommonCfg.TxBulkFactor = 1;
	pAd->CommonCfg.RxBulkFactor =1;

	pAd->CommonCfg.TxPower = 100; /*mW*/

	NdisZeroMemory(&pAd->CommonCfg.IOTestParm, sizeof(pAd->CommonCfg.IOTestParm));

#endif /* RTMP_MAC_USB */

	for(key_index=0; key_index<SHARE_KEY_NUM; key_index++)
	{
		for(bss_index = 0; bss_index < MAX_MBSSID_NUM(pAd) + MAX_P2P_NUM; bss_index++)
		{
			pAd->SharedKey[bss_index][key_index].KeyLen = 0;
			pAd->SharedKey[bss_index][key_index].CipherAlg = CIPHER_NONE;
		}
	}

	pAd->bLocalAdminMAC = FALSE;
	pAd->EepromAccess = FALSE;
	
	pAd->Antenna.word = 0; 
	pAd->CommonCfg.BBPCurrentBW = BW_20;


	pAd->bAutoTxAgcA = FALSE;			/* Default is OFF*/
	pAd->bAutoTxAgcG = FALSE;			/* Default is OFF*/
	
#if defined(RTMP_INTERNAL_TX_ALC) || defined(RTMP_TEMPERATURE_COMPENSATION)
	pAd->TxPowerCtrl.bInternalTxALC = FALSE; /* Off by default */
	pAd->TxPowerCtrl.idxTxPowerTable = 0;
	pAd->TxPowerCtrl.idxTxPowerTable2 = 0;
#ifdef RTMP_TEMPERATURE_COMPENSATION
	pAd->TxPowerCtrl.LookupTableIndex = 0;
#endif /* RTMP_TEMPERATURE_COMPENSATION */
#endif /* RTMP_INTERNAL_TX_ALC || RTMP_TEMPERATURE_COMPENSATION */

	pAd->RfIcType = RFIC_2820;

	/* Init timer for reset complete event*/
	pAd->CommonCfg.CentralChannel = 1;
	pAd->bForcePrintTX = FALSE;
	pAd->bForcePrintRX = FALSE;
	pAd->bStaFifoTest = FALSE;
	pAd->bProtectionTest = FALSE;
	pAd->bHCCATest = FALSE;
	pAd->bGenOneHCCA = FALSE;
	pAd->CommonCfg.Dsifs = 10;      /* in units of usec */
	pAd->CommonCfg.TxPower = 100; /* mW*/
	pAd->CommonCfg.TxPowerPercentage = 0xffffffff; /* AUTO*/
	pAd->CommonCfg.TxPowerDefault = 0xffffffff; /* AUTO*/
	pAd->CommonCfg.TxPreamble = Rt802_11PreambleAuto; /* use Long preamble on TX by defaut*/
	pAd->CommonCfg.bUseZeroToDisableFragment = FALSE;
	pAd->CommonCfg.RtsThreshold = 2347;
	pAd->CommonCfg.FragmentThreshold = 2346;
	pAd->CommonCfg.UseBGProtection = 0;    /* 0: AUTO*/
	pAd->CommonCfg.bEnableTxBurst = TRUE; /* 0;    	*/
	pAd->CommonCfg.PhyMode = 0xff;     /* unknown*/
	pAd->CommonCfg.SavedPhyMode = pAd->CommonCfg.PhyMode;
	

#ifdef CONFIG_AP_SUPPORT
#endif /* CONFIG_AP_SUPPORT */

#ifdef CARRIER_DETECTION_SUPPORT
	pAd->CommonCfg.CarrierDetect.delta = CARRIER_DETECT_DELTA;
	pAd->CommonCfg.CarrierDetect.div_flag = CARRIER_DETECT_DIV_FLAG;
	pAd->CommonCfg.CarrierDetect.criteria = CARRIER_DETECT_CRITIRIA;
	pAd->CommonCfg.CarrierDetect.threshold = CARRIER_DETECT_THRESHOLD;
	pAd->CommonCfg.CarrierDetect.recheck1 = CARRIER_DETECT_RECHECK_TIME;
	pAd->CommonCfg.CarrierDetect.CarrierGoneThreshold = CARRIER_GONE_TRESHOLD;
	pAd->CommonCfg.CarrierDetect.VGA_Mask = CARRIER_DETECT_DEFAULT_MASK;
	pAd->CommonCfg.CarrierDetect.Packet_End_Mask = CARRIER_DETECT_DEFAULT_MASK;
	pAd->CommonCfg.CarrierDetect.Rx_PE_Mask = CARRIER_DETECT_DEFAULT_MASK;
#endif /* CARRIER_DETECTION_SUPPORT */

#ifdef DFS_SUPPORT
	pAd->CommonCfg.RadarDetect.bDfsInit = FALSE;
#endif /* DFS_SUPPORT */

	pAd->Dot11_H.ChMovingTime = 65;

#ifdef UAPSD_SUPPORT
#ifdef CONFIG_AP_SUPPORT
{
	UINT32 IdMbss;

	for(IdMbss=0; IdMbss<HW_BEACON_MAX_NUM; IdMbss++)
		UAPSD_INFO_INIT(&pAd->ApCfg.MBSSID[IdMbss].UapsdInfo);
}
#endif /* CONFIG_AP_SUPPORT */
#endif /* UAPSD_SUPPORT */
	pAd->CommonCfg.bNeedSendTriggerFrame = FALSE;
	pAd->CommonCfg.TriggerTimerCount = 0;
	pAd->CommonCfg.bAPSDForcePowerSave = FALSE;
	/*pAd->CommonCfg.bCountryFlag = FALSE;*/
	pAd->CommonCfg.TxStream = 0;
	pAd->CommonCfg.RxStream = 0;

	NdisZeroMemory(&pAd->BeaconTxWI, TXWISize);

#ifdef DOT11_N_SUPPORT
	NdisZeroMemory(&pAd->CommonCfg.HtCapability, sizeof(pAd->CommonCfg.HtCapability));
	pAd->HTCEnable = FALSE;
	pAd->bBroadComHT = FALSE;
	pAd->CommonCfg.bRdg = FALSE;

#ifdef DOT11N_DRAFT3
	pAd->CommonCfg.Dot11OBssScanPassiveDwell = dot11OBSSScanPassiveDwell;	/* Unit : TU. 5~1000*/
	pAd->CommonCfg.Dot11OBssScanActiveDwell = dot11OBSSScanActiveDwell;	/* Unit : TU. 10~1000*/
	pAd->CommonCfg.Dot11BssWidthTriggerScanInt = dot11BSSWidthTriggerScanInterval;	/* Unit : Second	*/
	pAd->CommonCfg.Dot11OBssScanPassiveTotalPerChannel = dot11OBSSScanPassiveTotalPerChannel;	/* Unit : TU. 200~10000*/
	pAd->CommonCfg.Dot11OBssScanActiveTotalPerChannel = dot11OBSSScanActiveTotalPerChannel;	/* Unit : TU. 20~10000*/
	pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor = dot11BSSWidthChannelTransactionDelayFactor;
	pAd->CommonCfg.Dot11OBssScanActivityThre = dot11BSSScanActivityThreshold;	/* Unit : percentage*/
	pAd->CommonCfg.Dot11BssWidthChanTranDelay = (pAd->CommonCfg.Dot11BssWidthTriggerScanInt * pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor);

	pAd->CommonCfg.bBssCoexEnable = TRUE; /* by default, we enable this feature, you can disable it via the profile or ioctl command*/
	pAd->CommonCfg.BssCoexApCntThr = 0;
	pAd->CommonCfg.Bss2040NeedFallBack = 0;
#endif  /* DOT11N_DRAFT3 */

	NdisZeroMemory(&pAd->CommonCfg.AddHTInfo, sizeof(pAd->CommonCfg.AddHTInfo));
	pAd->CommonCfg.BACapability.field.MMPSmode = MMPS_ENABLE;
	pAd->CommonCfg.BACapability.field.MpduDensity = 0;
	pAd->CommonCfg.BACapability.field.Policy = IMMED_BA;
	pAd->CommonCfg.BACapability.field.RxBAWinLimit = 64; /*32;*/
	pAd->CommonCfg.BACapability.field.TxBAWinLimit = 64; /*32;*/
	DBGPRINT(RT_DEBUG_TRACE, ("--> UserCfgInit. BACapability = 0x%x\n", pAd->CommonCfg.BACapability.word));    

	pAd->CommonCfg.BACapability.field.AutoBA = FALSE;	
	BATableInit(pAd, &pAd->BATable);

	pAd->CommonCfg.bExtChannelSwitchAnnouncement = 1;
	pAd->CommonCfg.bHTProtect = 1;
	pAd->CommonCfg.bMIMOPSEnable = TRUE;
#ifdef GREENAP_SUPPORT
	pAd->ApCfg.bGreenAPEnable=FALSE;
	pAd->ApCfg.bGreenAPActive = FALSE;
	pAd->ApCfg.GreenAPLevel= GREENAP_WITHOUT_ANY_STAS_CONNECT;
#endif /* GREENAP_SUPPORT */
	pAd->CommonCfg.bBADecline = FALSE;
	pAd->CommonCfg.bDisableReordering = FALSE;

	if (pAd->MACVersion == 0x28720200)
		pAd->CommonCfg.TxBASize = 13; /*by Jerry recommend*/
	else
		pAd->CommonCfg.TxBASize = 7;

	pAd->CommonCfg.REGBACapability.word = pAd->CommonCfg.BACapability.word;
#endif /* DOT11_N_SUPPORT */

	/*pAd->CommonCfg.HTPhyMode.field.BW = BW_20;*/
	/*pAd->CommonCfg.HTPhyMode.field.MCS = MCS_AUTO;*/
	/*pAd->CommonCfg.HTPhyMode.field.ShortGI = GI_800;*/
	/*pAd->CommonCfg.HTPhyMode.field.STBC = STBC_NONE;*/
	pAd->CommonCfg.TxRate = RATE_6;
	
	pAd->CommonCfg.MlmeTransmit.field.MCS = MCS_RATE_6;
	pAd->CommonCfg.MlmeTransmit.field.BW = BW_20;
	pAd->CommonCfg.MlmeTransmit.field.MODE = MODE_OFDM;

	pAd->CommonCfg.BeaconPeriod = 100;     /* in mSec*/

#ifdef STREAM_MODE_SUPPORT
	if (pAd->chipCap.FlgHwStreamMode)
	{
		pAd->CommonCfg.StreamMode = 3;
		pAd->CommonCfg.StreamModeMCS = 0x0B0B;
	}
#endif /* STREAM_MODE_SUPPORT */

#ifdef TXBF_SUPPORT
	pAd->CommonCfg.ETxBfNoncompress = 0;
	pAd->CommonCfg.ETxBfIncapable = 0;
#endif /* TXBF_SUPPORT */

#ifdef NEW_RATE_ADAPT_SUPPORT
	pAd->CommonCfg.lowTrafficThrd = 2;
	pAd->CommonCfg.TrainUpRule = 2; // 1;
	pAd->CommonCfg.TrainUpRuleRSSI = -70; // 0;
	pAd->CommonCfg.TrainUpLowThrd = 90;
	pAd->CommonCfg.TrainUpHighThrd = 110;
#endif /* NEW_RATE_ADAPT_SUPPORT */



#ifdef CFO_TRACK
#endif /* CFO_TRACK */

#ifdef DBG_CTRL_SUPPORT
	pAd->CommonCfg.DebugFlags = 0;
#endif /* DBG_CTRL_SUPPORT */

#ifdef WAPI_SUPPORT
	pAd->CommonCfg.wapi_usk_rekey_method = REKEY_METHOD_DISABLE;
	pAd->CommonCfg.wapi_msk_rekey_method = REKEY_METHOD_DISABLE;
	pAd->CommonCfg.wapi_msk_rekey_cnt = 0;
#endif /* WAPI_SUPPORT */

#ifdef MCAST_RATE_SPECIFIC
	pAd->CommonCfg.MCastPhyMode.word
	= pAd->MacTab.Content[MCAST_WCID].HTPhyMode.word;
#endif /* MCAST_RATE_SPECIFIC */

	/* WFA policy - disallow TH rate in WEP or TKIP cipher */
	pAd->CommonCfg.HT_DisallowTKIP = TRUE;

	/* Frequency for rate adaptation */
	pAd->ra_interval = DEF_RA_TIME_INTRVAL;
	pAd->ra_fast_interval = DEF_QUICK_RA_TIME_INTERVAL;

#ifdef AGS_SUPPORT
	if (pAd->rateAlg == RATE_ALG_AGS)
		pAd->ra_fast_interval = AGS_QUICK_RA_TIME_INTERVAL;
#endif /* AGS_SUPPORT */

	/* Tx Sw queue length setting */
	pAd->TxSwQMaxLen = MAX_PACKETS_IN_QUEUE;
	
	pAd->CommonCfg.bRalinkBurstMode = FALSE;


	/* global variables mXXXX used in MAC protocol state machines*/
	OPSTATUS_SET_FLAG(pAd, fOP_STATUS_RECEIVE_DTIM);
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_ADHOC_ON);
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_INFRA_ON);

	/* PHY specification*/
	pAd->CommonCfg.PhyMode = (WMODE_B | WMODE_G);		/* default PHY mode*/
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_SHORT_PREAMBLE_INUSED);  /* CCK use LONG preamble*/


	/* Default for extra information is not valid*/
	pAd->ExtraInfo = EXTRA_INFO_CLEAR;
	
	/* Default Config change flag*/
	pAd->bConfigChanged = FALSE;

	/* */
	/* part III. AP configurations*/
	
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		/* Set MBSS Default Configurations*/
		pAd->ApCfg.BssidNum = MAX_MBSSID_NUM(pAd);
		for(j = BSS0; j < pAd->ApCfg.BssidNum; j++)
		{
			pAd->ApCfg.MBSSID[j].AuthMode = Ndis802_11AuthModeOpen;
			pAd->ApCfg.MBSSID[j].WepStatus = Ndis802_11EncryptionDisabled;
			pAd->ApCfg.MBSSID[j].GroupKeyWepStatus = Ndis802_11EncryptionDisabled;
			pAd->ApCfg.MBSSID[j].DefaultKeyId = 0;
			pAd->ApCfg.MBSSID[j].WpaMixPairCipher = MIX_CIPHER_NOTUSE;
			pAd->ApCfg.MBSSID[j].RekeyCountDown = 0;	/* it's used for WPA rekey */

#ifdef DOT1X_SUPPORT
			pAd->ApCfg.MBSSID[j].IEEE8021X = FALSE;
			pAd->ApCfg.MBSSID[j].PreAuth = FALSE;

			/* PMK cache setting*/
			pAd->ApCfg.MBSSID[j].PMKCachePeriod = (10 * 60 * OS_HZ); /* unit : tick(default: 10 minute)*/
			NdisZeroMemory(&pAd->ApCfg.MBSSID[j].PMKIDCache, sizeof(NDIS_AP_802_11_PMKID));

			/* dot1x related per BSS */
			pAd->ApCfg.MBSSID[j].radius_srv_num = 0;
			pAd->ApCfg.MBSSID[j].NasIdLen = 0;
#endif /* DOT1X_SUPPORT */

			/* VLAN related */
        		pAd->ApCfg.MBSSID[j].VLAN_VID = 0;

			/* Default MCS as AUTO*/
			pAd->ApCfg.MBSSID[j].bAutoTxRateSwitch = TRUE;
			pAd->ApCfg.MBSSID[j].DesiredTransmitSetting.field.MCS = MCS_AUTO;

			/* Default is zero. It means no limit.*/
			pAd->ApCfg.MBSSID[j].MaxStaNum = 0;
			pAd->ApCfg.MBSSID[j].StaCount = 0;
			
#ifdef WSC_AP_SUPPORT
			pAd->ApCfg.MBSSID[j].WscSecurityMode = 0xff;
			{
				PWSC_CTRL pWscControl;
				INT idx;
#ifdef WSC_V2_SUPPORT
				PWSC_V2_INFO	pWscV2Info;
#endif /* WSC_V2_SUPPORT */
				/*
					WscControl cannot be zero here, because WscControl timers are initial in MLME Initialize 
					and MLME Initialize is called before UserCfgInit.

				*/
				pWscControl = &pAd->ApCfg.MBSSID[j].WscControl;
				NdisZeroMemory(&pWscControl->RegData, sizeof(WSC_REG_DATA));
				NdisZeroMemory(&pAd->CommonCfg.WscStaPbcProbeInfo, sizeof(WSC_STA_PBC_PROBE_INFO));
				pWscControl->WscMode = 1;
				pWscControl->WscConfStatus = 1;
#ifdef WSC_V2_SUPPORT
				pWscControl->WscConfigMethods= 0x238C;
#else
				pWscControl->WscConfigMethods= 0x0084;
#endif /* WSC_V2_SUPPORT */
				pWscControl->RegData.ReComputePke = 1;
				pWscControl->lastId = 1;
				/* pWscControl->EntryIfIdx = (MIN_NET_DEVICE_FOR_MBSSID | j); */
				pWscControl->pAd = pAd;
				pWscControl->WscRejectSamePinFromEnrollee = FALSE;
				pAd->CommonCfg.WscPBCOverlap = FALSE;
				pWscControl->WscConfMode = 0;
				pWscControl->WscStatus = 0;
				pWscControl->WscState = 0;
				pWscControl->WscPinCode = 0;
				pWscControl->WscLastPinFromEnrollee = 0;
				pWscControl->WscEnrollee4digitPinCode = FALSE;
				pWscControl->WscEnrolleePinCode = 0;
				pWscControl->WscSelReg = 0;
				pWscControl->WscUseUPnP = 0;
				pWscControl->bWCNTest = FALSE;
				pWscControl->WscKeyASCII = 0; /* default, 0 (64 Hex) */
				
				/*
					Enrollee 192 random bytes for DH key generation
				*/
				for (idx = 0; idx < 192; idx++)
					pWscControl->RegData.EnrolleeRandom[idx] = RandomByte(pAd);

				/* Enrollee Nonce, first generate and save to Wsc Control Block*/
				for (idx = 0; idx < 16; idx++)
				{
					pWscControl->RegData.SelfNonce[idx] = RandomByte(pAd);
				}
				NdisZeroMemory(&pWscControl->WscDefaultSsid, sizeof(NDIS_802_11_SSID));
				NdisZeroMemory(&pWscControl->Wsc_Uuid_Str[0], UUID_LEN_STR);
				NdisZeroMemory(&pWscControl->Wsc_Uuid_E[0], UUID_LEN_HEX);
				pWscControl->bCheckMultiByte = FALSE;
				pWscControl->bWscAutoTigeer = FALSE;
				pWscControl->bWscFragment = FALSE;
				pWscControl->WscFragSize = 128;
				pWscControl->WscRxBufLen = 0;
				pWscControl->pWscRxBuf = NULL;
				os_alloc_mem(pAd, &pWscControl->pWscRxBuf, MGMT_DMA_BUFFER_SIZE);
				if (pWscControl->pWscRxBuf)
					NdisZeroMemory(pWscControl->pWscRxBuf, MGMT_DMA_BUFFER_SIZE);
				pWscControl->WscTxBufLen = 0;
				pWscControl->pWscTxBuf = NULL;
				os_alloc_mem(pAd, &pWscControl->pWscTxBuf, MGMT_DMA_BUFFER_SIZE);
				if (pWscControl->pWscTxBuf)
					NdisZeroMemory(pWscControl->pWscTxBuf, MGMT_DMA_BUFFER_SIZE);
				initList(&pWscControl->WscPeerList);
				NdisAllocateSpinLock(pAd, &pWscControl->WscPeerListSemLock);
				pWscControl->PinAttackCount = 0;
				pWscControl->bSetupLock = FALSE;
#ifdef WSC_V2_SUPPORT
				pWscV2Info = &pWscControl->WscV2Info;
				pWscV2Info->bWpsEnable = TRUE;
				pWscV2Info->ExtraTlv.TlvLen = 0;
				pWscV2Info->ExtraTlv.TlvTag = 0;
				pWscV2Info->ExtraTlv.pTlvData = NULL;
				pWscV2Info->ExtraTlv.TlvType = TLV_ASCII;
				pWscV2Info->bEnableWpsV2 = TRUE;
				pWscControl->SetupLockTime = WSC_WPS_AP_SETUP_LOCK_TIME;
				pWscControl->MaxPinAttack = WSC_WPS_AP_MAX_PIN_ATTACK;
#endif /* WSC_V2_SUPPORT */

			}
#endif /* WSC_AP_SUPPORT */


			for(i = 0; i < WLAN_MAX_NUM_OF_TIM; i++)
	        		pAd->ApCfg.MBSSID[j].TimBitmaps[i] = 0;
		}
		pAd->ApCfg.DtimCount  = 0;
		pAd->ApCfg.DtimPeriod = DEFAULT_DTIM_PERIOD;

		pAd->ApCfg.ErpIeContent = 0;

		pAd->ApCfg.StaIdleTimeout = MAC_TABLE_AGEOUT_TIME;

#ifdef IDS_SUPPORT
		/* Default disable IDS threshold and reset all IDS counters*/
		pAd->ApCfg.IdsEnable = FALSE;
		pAd->ApCfg.AuthFloodThreshold = 0;
		pAd->ApCfg.AssocReqFloodThreshold = 0;
		pAd->ApCfg.ReassocReqFloodThreshold = 0;
		pAd->ApCfg.ProbeReqFloodThreshold = 0;
		pAd->ApCfg.DisassocFloodThreshold = 0;
		pAd->ApCfg.DeauthFloodThreshold = 0;
		pAd->ApCfg.EapReqFloodThreshold = 0;
		RTMPClearAllIdsCounter(pAd);
#endif /* IDS_SUPPORT */

#ifdef WDS_SUPPORT
		APWdsInitialize(pAd);
#endif /* WDS_SUPPORT*/

#ifdef WSC_INCLUDED
		pAd->WriteWscCfgToDatFile = 0xFF;
		pAd->WriteWscCfgToAr9DatFile = FALSE;
#ifdef CONFIG_AP_SUPPORT
		pAd->bWscDriverAutoUpdateCfg = TRUE;
#endif /* CONFIG_AP_SUPPORT */
#endif /* WSC_INCLUDED */

#ifdef APCLI_SUPPORT
		pAd->ApCfg.FlgApCliIsUapsdInfoUpdated = FALSE;

		for(j = 0; j < MAX_APCLI_NUM; j++) 
		{
			pAd->ApCfg.ApCliTab[j].AuthMode = Ndis802_11AuthModeOpen;
			pAd->ApCfg.ApCliTab[j].WepStatus = Ndis802_11WEPDisabled;
			pAd->ApCfg.ApCliTab[j].bAutoTxRateSwitch = TRUE;
			pAd->ApCfg.ApCliTab[j].DesiredTransmitSetting.field.MCS = MCS_AUTO;
			pAd->ApCfg.ApCliTab[j].UapsdInfo.bAPSDCapable = FALSE;

#ifdef APCLI_WPA_SUPPLICANT_SUPPORT
			pAd->ApCfg.ApCliTab[j].IEEE8021X=FALSE;
			pAd->ApCfg.ApCliTab[j].IEEE8021x_required_keys=FALSE;
			pAd->ApCfg.ApCliTab[j].bRSN_IE_FromWpaSupplicant=FALSE;
			pAd->ApCfg.ApCliTab[j].bLostAp=FALSE;
			pAd->ApCfg.ApCliTab[j].bScanReqIsFromWebUI=FALSE;
			pAd->ApCfg.ApCliTab[j].bConfigChanged=FALSE;
			pAd->ApCfg.ApCliTab[j].DesireSharedKeyId=0;
			pAd->ApCfg.ApCliTab[j].WpaSupplicantUP=WPA_SUPPLICANT_DISABLE;
			pAd->ApCfg.ApCliTab[j].WpaSupplicantScanCount=0;
			pAd->ApCfg.ApCliTab[j].pWpsProbeReqIe=NULL;
			pAd->ApCfg.ApCliTab[j].WpsProbeReqIeLen=0;
			pAd->ApCfg.ApCliTab[j].pWpaAssocIe=NULL;
			pAd->ApCfg.ApCliTab[j].WpaAssocIeLen=0;
			pAd->ApCfg.ApCliTab[j].SavedPMKNum=0;
			RTMPZeroMemory(pAd->ApCfg.ApCliTab[j].SavedPMK, (PMKID_NO * sizeof(BSSID_INFO)));
#endif/*APCLI_WPA_SUPPLICANT_SUPPORT*/

		}
#endif /* APCLI_SUPPORT */
		pAd->ApCfg.EntryClientCount = 0;
	}
#endif /* CONFIG_AP_SUPPORT */


#ifdef IP_ASSEMBLY
	if (pAd->OpMode == OPMODE_STA)
	{
		pAd->StaCfg.bFragFlag = TRUE;
	}
#endif /* IP_ASSEMBLY */

	/* part IV. others*/
	
	/* dynamic BBP R66:sensibity tuning to overcome background noise*/
	pAd->BbpTuning.bEnable                = TRUE;  
	pAd->BbpTuning.FalseCcaLowerThreshold = 100;
	pAd->BbpTuning.FalseCcaUpperThreshold = 512;
	pAd->BbpTuning.R66Delta               = 4;
	pAd->Mlme.bEnableAutoAntennaCheck = TRUE;
	
	
	/* Also initial R66CurrentValue, RTUSBResumeMsduTransmission might use this value.*/
	/* if not initial this value, the default value will be 0.*/
	
	pAd->BbpTuning.R66CurrentValue = 0x38;

	pAd->Bbp94 = BBPR94_DEFAULT;
	pAd->BbpForCCK = FALSE;
	
	/* Default is FALSE for test bit 1*/
	/*pAd->bTest1 = FALSE;*/
	
	/* initialize MAC table and allocate spin lock*/
	NdisZeroMemory(&pAd->MacTab, sizeof(MAC_TABLE));
	InitializeQueueHeader(&pAd->MacTab.McastPsQueue);
	NdisAllocateSpinLock(pAd, &pAd->MacTabLock);

	/*RTMPInitTimer(pAd, &pAd->RECBATimer, RECBATimerTimeout, pAd, TRUE);*/
	/*RTMPSetTimer(&pAd->RECBATimer, REORDER_EXEC_INTV);*/


	pAd->CommonCfg.bWiFiTest = FALSE;

#ifdef CONFIG_AP_SUPPORT
	pAd->ApCfg.EntryLifeCheck = MAC_ENTRY_LIFE_CHECK_CNT;

#endif /* CONFIG_AP_SUPPORT */


	pAd->RxAnt.Pair1PrimaryRxAnt = 0;
	pAd->RxAnt.Pair1SecondaryRxAnt = 1;

		pAd->RxAnt.EvaluatePeriod = 0;
		pAd->RxAnt.RcvPktNumWhenEvaluate = 0;
#ifdef CONFIG_AP_SUPPORT
		pAd->RxAnt.Pair1AvgRssiGroup1[0] = pAd->RxAnt.Pair1AvgRssiGroup1[1] = 0;
		pAd->RxAnt.Pair1AvgRssiGroup2[0] = pAd->RxAnt.Pair1AvgRssiGroup2[1] = 0;
#endif /* CONFIG_AP_SUPPORT */


#ifdef TXRX_SW_ANTDIV_SUPPORT
		pAd->chipCap.bTxRxSwAntDiv = FALSE;
#endif /* TXRX_SW_ANTDIV_SUPPORT */


#if defined(AP_SCAN_SUPPORT) || defined(CONFIG_STA_SUPPORT)
	for (i = 0; i < MAX_LEN_OF_BSS_TABLE; i++) 
	{
		PBSS_ENTRY	pBssEntry = &pAd->ScanTab.BssEntry[i];
		
		if (pAd->ProbeRespIE[i].pIe)
			pBssEntry->pVarIeFromProbRsp = pAd->ProbeRespIE[i].pIe;
		else
			pBssEntry->pVarIeFromProbRsp = NULL;
	}
#endif /* defined(AP_SCAN_SUPPORT) || defined(CONFIG_STA_SUPPORT) */


#ifdef WSC_INCLUDED
	NdisZeroMemory(&pAd->CommonCfg.WscStaPbcProbeInfo, sizeof(WSC_STA_PBC_PROBE_INFO));
	pAd->CommonCfg.WscPBCOverlap = FALSE;
#endif /* WSC_INCLUDED */



#ifdef WFD_SUPPORT
	WfdCfgInit(pAd);
#endif /* WFD_SUPPORT */


#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	pAd->WOW_Cfg.bEnable = FALSE;	
	pAd->WOW_Cfg.bWOWFirmware = FALSE;	/* load normal firmware */
	pAd->WOW_Cfg.bInBand = TRUE;		/* use in-band signal */
	pAd->WOW_Cfg.nSelectedGPIO = 1;
	pAd->WOW_Cfg.nDelay = 3; /* (3+1)*3 = 12 sec */
	pAd->WOW_Cfg.nHoldTime = 1; /* 1*10 = 10 ms */
	DBGPRINT(RT_DEBUG_OFF, ("WOW Enable %d, WOWFirmware %d\n", pAd->WOW_Cfg.bEnable, pAd->WOW_Cfg.bWOWFirmware));
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */

	/* 802.11H and DFS related params*/
	pAd->Dot11_H.CSCount = 0;
	pAd->Dot11_H.CSPeriod = 10;
	pAd->Dot11_H.RDMode = RD_NORMAL_MODE;
	pAd->Dot11_H.bDFSIndoor = 1;


#ifdef FPGA_MODE
	pAd->fpga_on = 0x0;
	pAd->tx_kick_cnt = 0;
	pAd->data_phy = 0;
	pAd->data_mcs = 0;
	pAd->data_bw = 0;
	pAd->data_gi = 0;
#endif /* FPGA_MODE */

#ifdef MICROWAVE_OVEN_SUPPORT
	if (pAd->OpMode == OPMODE_AP)
		pAd->CommonCfg.MO_Cfg.bEnable = TRUE;
	else
		pAd->CommonCfg.MO_Cfg.bEnable = FALSE;
	pAd->CommonCfg.MO_Cfg.nFalseCCATh = MO_FALSE_CCA_TH;
#endif /* MICROWAVE_OVEN_SUPPORT */

	DBGPRINT(RT_DEBUG_TRACE, ("<-- UserCfgInit\n"));
}

/* IRQL = PASSIVE_LEVEL*/
UCHAR BtoH(STRING ch)
{
	if (ch >= '0' && ch <= '9') return (ch - '0');        /* Handle numerals*/
	if (ch >= 'A' && ch <= 'F') return (ch - 'A' + 0xA);  /* Handle capitol hex digits*/
	if (ch >= 'a' && ch <= 'f') return (ch - 'a' + 0xA);  /* Handle small hex digits*/
	return(255);
}


/*  FUNCTION: AtoH(char *, UCHAR *, int)*/

/*  PURPOSE:  Converts ascii string to network order hex*/

/*  PARAMETERS:*/
/*    src    - pointer to input ascii string*/
/*    dest   - pointer to output hex*/
/*    destlen - size of dest*/

/*  COMMENTS:*/

/*    2 ascii bytes make a hex byte so must put 1st ascii byte of pair*/
/*    into upper nibble and 2nd ascii byte of pair into lower nibble.*/

/* IRQL = PASSIVE_LEVEL*/

void AtoH(PSTRING src, PUCHAR dest, int destlen)
{
	PSTRING srcptr;
	PUCHAR destTemp;

	srcptr = src;	
	destTemp = (PUCHAR) dest; 

	while(destlen--)
	{
		*destTemp = BtoH(*srcptr++) << 4;    /* Put 1st ascii byte in upper nibble.*/
		*destTemp += BtoH(*srcptr++);      /* Add 2nd ascii byte to above.*/
		destTemp++;
	}
}


/*
========================================================================
Routine Description:
	Add a timer to the timer list.

Arguments:
	pAd				- WLAN control block pointer
	pRsc			- the OS resource

Return Value:
	None

Note:
========================================================================
*/
VOID	RTMP_TimerListAdd(
	IN	PRTMP_ADAPTER			pAd,
	IN	VOID					*pRsc)
{
	LIST_HEADER *pRscList = &pAd->RscTimerCreateList;
	LIST_RESOURCE_OBJ_ENTRY *pObj;


	/* try to find old entry */
	pObj = (LIST_RESOURCE_OBJ_ENTRY *)(pRscList->pHead);
	while(1)
	{
		if (pObj == NULL)
			break;
		if ((ULONG)(pObj->pRscObj) == (ULONG)pRsc)
			return; /* exists */
		pObj = pObj->pNext;
	}

	/* allocate a timer record entry */
	os_alloc_mem(NULL, (UCHAR **)&(pObj), sizeof(LIST_RESOURCE_OBJ_ENTRY));
	if (pObj == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s: alloc timer obj fail!\n", __FUNCTION__));
		return;
	}
	else
	{
		pObj->pRscObj = pRsc;
		insertTailList(pRscList, (LIST_ENTRY *)pObj);
		DBGPRINT(RT_DEBUG_ERROR, ("%s: add timer obj %lx!\n", __FUNCTION__, (ULONG)pRsc));
	}
}


/*
========================================================================
Routine Description:
	Cancel all timers in the timer list.

Arguments:
	pAd				- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
VOID	RTMP_TimerListRelease(
	IN	PRTMP_ADAPTER			pAd)
{
	LIST_HEADER *pRscList = &pAd->RscTimerCreateList;
	LIST_RESOURCE_OBJ_ENTRY *pObj, *pObjOld;
	BOOLEAN Cancel;


	/* try to find old entry */
	pObj = (LIST_RESOURCE_OBJ_ENTRY *)(pRscList->pHead);
	while(1)
	{
		if (pObj == NULL)
			break;
		DBGPRINT(RT_DEBUG_TRACE, ("%s: Cancel timer obj %lx!\n", __FUNCTION__, (ULONG)(pObj->pRscObj)));
		RTMPReleaseTimer(pObj->pRscObj, &Cancel);
		pObjOld = pObj;
		pObj = pObj->pNext;
		os_free_mem(NULL, pObjOld);
	}

	/* reset TimerList */
	initList(&pAd->RscTimerCreateList);
}


VOID RTMP_AllTimerListRelease(RTMP_ADAPTER *pAd)
{
   LIST_HEADER *pRscList = &pAd->RscTimerCreateList;
   LIST_RESOURCE_OBJ_ENTRY *pObj, *pObjOld;
   BOOLEAN Cancel;

   /* try to find old entry */
   pObj = (LIST_RESOURCE_OBJ_ENTRY *)(pRscList->pHead);
   while(1)
   {
           if (pObj == NULL)
                   break;
           DBGPRINT(RT_DEBUG_TRACE, ("%s: Cancel timer obj %lx!\n", __FUNCTION__, (ULONG)(pObj->pRscObj)));
           pObjOld = pObj;
           pObj = pObj->pNext;
           RTMPReleaseTimer(pObjOld->pRscObj, &Cancel);
           os_free_mem(NULL, pObjOld);
   }

   /* reset TimerList */
   initList(&pAd->RscTimerCreateList);
}


/*
	========================================================================
	
	Routine Description:
		Init timer objects

	Arguments:
		pAd			Pointer to our adapter
		pTimer				Timer structure
		pTimerFunc			Function to execute when timer expired
		Repeat				Ture for period timer

	Return Value:
		None

	Note:
		
	========================================================================
*/
VOID	RTMPInitTimer(
	IN	PRTMP_ADAPTER			pAd,
	IN	PRALINK_TIMER_STRUCT	pTimer,
	IN	PVOID					pTimerFunc,
	IN	PVOID					pData,
	IN	BOOLEAN					Repeat)
{
	RTMP_SEM_LOCK(&TimerSemLock);

	RTMP_TimerListAdd(pAd, pTimer);

	
	/* Set Valid to TRUE for later used.*/
	/* It will crash if we cancel a timer or set a timer */
	/* that we haven't initialize before.*/
	/* */
	pTimer->Valid      = TRUE;
	
	pTimer->PeriodicType = Repeat;
	pTimer->State      = FALSE;
	pTimer->cookie = (ULONG) pData;
	pTimer->pAd = pAd;

	RTMP_OS_Init_Timer(pAd, &pTimer->TimerObj,	pTimerFunc, (PVOID) pTimer, &pAd->RscTimerMemList);	
	DBGPRINT(RT_DEBUG_TRACE,("%s: %lx\n",__FUNCTION__, (ULONG)pTimer));

	RTMP_SEM_UNLOCK(&TimerSemLock);
}


/*
	========================================================================
	
	Routine Description:
		Init timer objects

	Arguments:
		pTimer				Timer structure
		Value				Timer value in milliseconds

	Return Value:
		None

	Note:
		To use this routine, must call RTMPInitTimer before.
		
	========================================================================
*/
VOID	RTMPSetTimer(
	IN	PRALINK_TIMER_STRUCT	pTimer,
	IN	ULONG					Value)
{
	RTMP_SEM_LOCK(&TimerSemLock);

	if (pTimer->Valid)
	{
		RTMP_ADAPTER *pAd;
		
		pAd = (RTMP_ADAPTER *)pTimer->pAd;
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS | fRTMP_ADAPTER_NIC_NOT_EXIST))
		{
			DBGPRINT_ERR(("RTMPSetTimer failed, Halt in Progress!\n"));
			RTMP_SEM_UNLOCK(&TimerSemLock);
			return;
		}
		
		pTimer->TimerValue = Value;
		pTimer->State      = FALSE;
		if (pTimer->PeriodicType == TRUE)
		{
			pTimer->Repeat = TRUE;
			RTMP_SetPeriodicTimer(&pTimer->TimerObj, Value);
		}
		else
		{
			pTimer->Repeat = FALSE;
			RTMP_OS_Add_Timer(&pTimer->TimerObj, Value);
		}

		DBGPRINT(RT_DEBUG_INFO,("%s: %lx\n",__FUNCTION__, (ULONG)pTimer));
	}
	else
	{
		DBGPRINT_ERR(("RTMPSetTimer failed, Timer hasn't been initialize!\n"));
	}
	RTMP_SEM_UNLOCK(&TimerSemLock);
}


/*
	========================================================================
	
	Routine Description:
		Init timer objects

	Arguments:
		pTimer				Timer structure
		Value				Timer value in milliseconds

	Return Value:
		None

	Note:
		To use this routine, must call RTMPInitTimer before.
		
	========================================================================
*/
VOID	RTMPModTimer(
	IN	PRALINK_TIMER_STRUCT	pTimer,
	IN	ULONG					Value)
{
	BOOLEAN	Cancel;


	RTMP_SEM_LOCK(&TimerSemLock);

	if (pTimer->Valid)
	{
		pTimer->TimerValue = Value;
		pTimer->State      = FALSE;
		if (pTimer->PeriodicType == TRUE)
		{
			RTMP_SEM_UNLOCK(&TimerSemLock);
			RTMPCancelTimer(pTimer, &Cancel);
			RTMPSetTimer(pTimer, Value);
		}
		else
		{
			RTMP_OS_Mod_Timer(&pTimer->TimerObj, Value);
			RTMP_SEM_UNLOCK(&TimerSemLock);
		}
		DBGPRINT(RT_DEBUG_TRACE,("%s: %lx\n",__FUNCTION__, (ULONG)pTimer));
	}
	else
	{
		DBGPRINT_ERR(("RTMPModTimer failed, Timer hasn't been initialize!\n"));
		RTMP_SEM_UNLOCK(&TimerSemLock);
	}
}


/*
	========================================================================
	
	Routine Description:
		Cancel timer objects

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	Note:
		1.) To use this routine, must call RTMPInitTimer before.
		2.) Reset NIC to initial state AS IS system boot up time.
		
	========================================================================
*/
VOID RTMPCancelTimer(
	IN PRALINK_TIMER_STRUCT	pTimer,
	OUT BOOLEAN		*pCancelled)
{
	RTMP_SEM_LOCK(&TimerSemLock);

	if (pTimer->Valid)
	{
		if (pTimer->State == FALSE)
			pTimer->Repeat = FALSE;
		
		RTMP_OS_Del_Timer(&pTimer->TimerObj, pCancelled);
		
		if (*pCancelled == TRUE)
			pTimer->State = TRUE;

#ifdef RTMP_TIMER_TASK_SUPPORT
		/* We need to go-through the TimerQ to findout this timer handler and remove it if */
		/*		it's still waiting for execution.*/
		RtmpTimerQRemove(pTimer->pAd, pTimer);
#endif /* RTMP_TIMER_TASK_SUPPORT */

		DBGPRINT(RT_DEBUG_INFO,("%s: %lx\n",__FUNCTION__, (ULONG)pTimer));
	}
	else
	{
		DBGPRINT(RT_DEBUG_INFO,("RTMPCancelTimer failed, Timer hasn't been initialize!\n"));
	}

	RTMP_SEM_UNLOCK(&TimerSemLock);
}


VOID	RTMPReleaseTimer(
	IN	PRALINK_TIMER_STRUCT	pTimer,
	OUT	BOOLEAN					*pCancelled)
{
	RTMP_SEM_LOCK(&TimerSemLock);

	if (pTimer->Valid)
	{
		if (pTimer->State == FALSE)
			pTimer->Repeat = FALSE;
		
		RTMP_OS_Del_Timer(&pTimer->TimerObj, pCancelled);
		
		if (*pCancelled == TRUE)
			pTimer->State = TRUE;

#ifdef RTMP_TIMER_TASK_SUPPORT
		/* We need to go-through the TimerQ to findout this timer handler and remove it if */
		/*		it's still waiting for execution.*/
		RtmpTimerQRemove(pTimer->pAd, pTimer);
#endif /* RTMP_TIMER_TASK_SUPPORT */

		/* release timer */
		RTMP_OS_Release_Timer(&pTimer->TimerObj);

		pTimer->Valid = FALSE;

		DBGPRINT(RT_DEBUG_INFO,("%s: %lx\n",__FUNCTION__, (ULONG)pTimer));
	}
	else
	{
		DBGPRINT(RT_DEBUG_INFO,("RTMPReleasefailed, Timer hasn't been initialize!\n"));
	}

	RTMP_SEM_UNLOCK(&TimerSemLock);
}


/*
	========================================================================
	
	Routine Description:
		Enable RX 

	Arguments:
		pAd						Pointer to our adapter

	Return Value:
		None

	IRQL <= DISPATCH_LEVEL
	
	Note:
		Before Enable RX, make sure you have enabled Interrupt.
	========================================================================
*/
VOID RTMPEnableRxTx(
	IN PRTMP_ADAPTER	pAd)
{
	UINT32 rx_filter_flag;

	DBGPRINT(RT_DEBUG_TRACE, ("==> RTMPEnableRxTx\n"));

	RT28XXDMAEnable(pAd);

	/* enable RX of MAC block*/
	if (pAd->OpMode == OPMODE_AP)
	{
		rx_filter_flag = APNORMAL;

#ifdef CONFIG_AP_SUPPORT
#ifdef IDS_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			if (pAd->ApCfg.IdsEnable)
				rx_filter_flag &= (~0x4);	/* Don't drop those not-U2M frames*/
		}
#endif /* IDS_SUPPORT */			
#endif /* CONFIG_AP_SUPPORT */

		RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, rx_filter_flag);     /* enable RX of DMA block*/
	}
	
	{
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0xc);
//+++Add by shiang for debug for pbf_loopback
			//RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x2c);
//---Add by shiang for debug
//+++Add by shiang for debug invalid RxWI->WCID 
#ifdef RT8592
#endif /* RT8592 */
//---Add by shiang for  debug invalid RxWI->WCID 
	}

	DBGPRINT(RT_DEBUG_TRACE, ("<== RTMPEnableRxTx\n"));	
}


void CfgInitHook(PRTMP_ADAPTER pAd)
{
	pAd->bBroadComHT = TRUE;
}


static INT RtmpChipOpsRegister(
	IN RTMP_ADAPTER *pAd,
	IN INT			infType)
{
	RTMP_CHIP_OP	*pChipOps = &pAd->chipOps;
	int status;

	memset(pChipOps, 0, sizeof(RTMP_CHIP_OP));

	RtmpChipOpsHook(pAd);

	/* MCU related */
	ChipOpsMCUHook(pAd, pAd->chipCap.MCUType);

	/* set eeprom related hook functions */
	status = RtmpChipOpsEepromHook(pAd, infType);

	return status;
}


#ifdef RTMP_MAC_USB
BOOLEAN PairEP(RTMP_ADAPTER *pAd, UINT8 EP, UINT8 Index, UINT8 InOut)
{
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;

	if (Index == 0 && InOut == 0)
	{
		pChipCap->CommandBulkOutAddr = EP;
		DBGPRINT(RT_DEBUG_OFF, ("Endpoint(%x) is for In-band Command\n", EP));
		return TRUE;
	}
	
	if (Index > 0 && Index < 5 && InOut == 0)
	{
		pChipCap->WMM0ACBulkOutAddr[Index - 1] = EP; 
		DBGPRINT(RT_DEBUG_OFF, ("Endpoint(%x) is for WMM0 AC%d\n", EP, Index - 1));
		return TRUE;
	}
	
	if (Index == 5 && InOut == 0)
	{
		pChipCap->WMM1ACBulkOutAddr = EP;
		DBGPRINT(RT_DEBUG_OFF, ("Endpoint(%x) is for WMM1 AC0\n", EP));
		return TRUE;
	}

	if (Index == 0 && InOut == 1)
	{
		pChipCap->DataBulkInAddr = EP;
		DBGPRINT(RT_DEBUG_OFF, ("Endpoint(%x) is for Data-In\n", EP));
		return TRUE;
	}


	if (Index == 1 && InOut == 1)
	{
		pChipCap->CommandRspBulkInAddr = EP;
		DBGPRINT(RT_DEBUG_OFF, ("Endpoint(%x) is for Command Rsp\n", EP));
		return TRUE;
	}
				
	return FALSE;
}
#endif /* RTMP_MAC_USB */


INT RtmpRaDevCtrlInit(VOID *pAdSrc, RTMP_INF_TYPE infType)
{
	RTMP_ADAPTER *pAd = (PRTMP_ADAPTER)pAdSrc;
	UINT8 i;

	/* Assign the interface type. We need use it when do register/EEPROM access.*/
	pAd->infType = infType;


#ifdef CONFIG_AP_SUPPORT
	pAd->OpMode = OPMODE_AP;
	DBGPRINT(RT_DEBUG_TRACE, ("AP Driver version-%s\n", AP_DRIVER_VERSION));
#endif /* CONFIG_AP_SUPPORT */


#ifdef RTMP_MAC_USB
	RTMP_SEM_EVENT_INIT(&(pAd->UsbVendorReq_semaphore), &pAd->RscSemMemList);
	RTMP_SEM_EVENT_INIT(&(pAd->reg_atomic), &pAd->RscSemMemList);
	RTMP_SEM_EVENT_INIT(&(pAd->hw_atomic), &pAd->RscSemMemList);
	os_alloc_mem(pAd, (PUCHAR *)&pAd->UsbVendorReqBuf, MAX_PARAM_BUFFER_SIZE - 1);
	if (pAd->UsbVendorReqBuf == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Allocate vendor request temp buffer failed!\n"));
		return FALSE;
	}

	
#endif /* RTMP_MAC_USB */

	RtmpChipOpsRegister(pAd, infType);

#ifdef RTMP_MAC_USB
	for (i = 0; i < 6; i++)
	{
		if (!PairEP(pAd, pAd->BulkOutEpAddr[i], i, 0))
			DBGPRINT(RT_DEBUG_ERROR, ("Invalid bulk out ep(%x)\n", pAd->BulkOutEpAddr[i]));
	}

	for (i = 0; i < 2; i++)
	{
		if (!PairEP(pAd, pAd->BulkInEpAddr[i], i, 1))
			DBGPRINT(RT_DEBUG_ERROR, ("Invalid bulk in ep(%x)\n", pAd->BulkInEpAddr[i]));
	}

#endif

#ifdef MULTIPLE_CARD_SUPPORT
{
	extern BOOLEAN RTMP_CardInfoRead(PRTMP_ADAPTER pAd);

	/* find its profile path*/
	pAd->MC_RowID = -1; /* use default profile path*/
	RTMP_CardInfoRead(pAd);

	if (pAd->MC_RowID == -1)
#ifdef CONFIG_AP_SUPPORT
		strcpy(pAd->MC_FileName, AP_PROFILE_PATH);
#endif /* CONFIG_AP_SUPPORT */

	DBGPRINT(RT_DEBUG_TRACE, ("MC> ROW = %d, PATH = %s\n", pAd->MC_RowID, pAd->MC_FileName));
}
#endif /* MULTIPLE_CARD_SUPPORT */

#if defined(CONFIG_CSO_SUPPORT) || defined(CONFIG_RX_CSO_SUPPORT)
	if (pAd->chipCap.asic_caps | fASIC_CAP_CSO)
		RTMP_SET_MORE_FLAG(pAd, fASIC_CAP_CSO);
#endif /* defined(CONFIG_CSO_SUPPORT) || defined(CONFIG_RX_CSO_SUPPORT) */
	return 0;
}


BOOLEAN RtmpRaDevCtrlExit(IN VOID *pAdSrc)
{
	PRTMP_ADAPTER	pAd = (PRTMP_ADAPTER)pAdSrc;
	INT index;
	
#ifdef MULTIPLE_CARD_SUPPORT
extern UINT8  MC_CardUsed[MAX_NUM_OF_MULTIPLE_CARD];

	if ((pAd->MC_RowID >= 0) && (pAd->MC_RowID <= MAX_NUM_OF_MULTIPLE_CARD))
		MC_CardUsed[pAd->MC_RowID] = 0; /* not clear MAC address*/
#endif /* MULTIPLE_CARD_SUPPORT */

#ifdef RTMP_MAC_USB
	RTMP_SEM_EVENT_DESTORY(&(pAd->UsbVendorReq_semaphore));
	RTMP_SEM_EVENT_DESTORY(&(pAd->reg_atomic));
	RTMP_SEM_EVENT_DESTORY(&(pAd->hw_atomic));


	if (pAd->UsbVendorReqBuf)
		os_free_mem(pAd, pAd->UsbVendorReqBuf);
#endif /* RTMP_MAC_USB */

	/*
		Free ProbeRespIE Table
	*/
	for (index = 0; index < MAX_LEN_OF_BSS_TABLE; index++) 
	{
		if (pAd->ProbeRespIE[index].pIe)
			os_free_mem(pAd, pAd->ProbeRespIE[index].pIe);
	}

#ifdef RESOURCE_PRE_ALLOC
	RTMPFreeTxRxRingMemory(pAd);
#endif /* RESOURCE_PRE_ALLOC */

#ifdef RT65xx
	if (IS_RT6590(pAd) && (pAd->WlanFunCtrl.field.WLAN_EN == 1))
		ral_wlan_chip_onoff(pAd, FALSE, FALSE);
#endif /* RT65xx */

#ifdef MT7601
	if (IS_MT7601(pAd) && (pAd->WlanFunCtrl.field.WLAN_EN == 1) )
		MT7601_WLAN_ChipOnOff(pAd, FALSE, FALSE);
#endif /* MT7601 */

	RTMPFreeAdapter(pAd);

	return TRUE;
}


#ifdef CONFIG_AP_SUPPORT
#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
VOID RTMP_11N_D3_TimerInit(
	IN PRTMP_ADAPTER pAd)
{
	RTMPInitTimer(pAd, &pAd->CommonCfg.Bss2040CoexistTimer, GET_TIMER_FUNCTION(Bss2040CoexistTimeOut), pAd, FALSE);
}
#endif /* DOT11N_DRAFT3 */
#endif /* DOT11_N_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */


#ifdef VENDOR_FEATURE3_SUPPORT
VOID RTMP_IO_WRITE32(
	PRTMP_ADAPTER pAd,
	UINT32 Offset,
	UINT32 Value)
{
	_RTMP_IO_WRITE32(pAd, Offset, Value);
}

VOID RTMP_BBP_IO_READ8_BY_REG_ID(
	PRTMP_ADAPTER pAd,
	UINT32 Offset,
	UINT8 *pValue)
{
#ifdef RT8592
	if (IS_RT8592(pAd))
		return;
#endif /* RT8592 */

#ifdef RT65xx
	if (IS_RT65XX(pAd))
		return;
#endif /* RT65xx */

#ifdef MT7601
	if (IS_MT7601(pAd))
		return;
#endif /* MT7601 */
	_RTMP_BBP_IO_READ8_BY_REG_ID(pAd, Offset, pValue);
}

VOID RTMP_BBP_IO_READ8(
	PRTMP_ADAPTER pAd,
	UCHAR Offset,
	UINT8 *pValue,
	BOOLEAN FlgValidMCR)
{
#ifdef RT8592
	if (IS_RT8592(pAd))
		return;
#endif /* RT8592 */

#ifdef RT65xx
	if (IS_RT65XX(pAd))
		return;
#endif /* RT65xx */

#ifdef MT7601
	if (IS_MT7601(pAd))
		return;
#endif /* MT7601 */

	_RTMP_BBP_IO_READ8(pAd, Offset, pValue, FlgValidMCR);
}

VOID RTMP_BBP_IO_WRITE8_BY_REG_ID(
	PRTMP_ADAPTER pAd,
	UINT32 Offset,
	UINT8 Value)
{
#ifdef RT8592
	if (IS_RT8592(pAd))
		return;
#endif /* RT8592 */

#ifdef RT65xx
	if (IS_RT65XX(pAd))
		return;
#endif /* RT65xx */

#ifdef MT7601
	if (IS_MT7601(pAd))
		return;
#endif /* MT7601 */

	_RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, Offset, Value);
}

VOID RTMP_BBP_IO_WRITE8(
	PRTMP_ADAPTER pAd,
	UCHAR Offset,
	UINT8 Value,
	BOOLEAN FlgValidMCR)
{
#ifdef RT8592
	if (IS_RT8592(pAd))
		return;
#endif /* RT8592 */

#ifdef RT65xx
	if (IS_RT65XX(pAd))
		return;
#endif /* RT65xx */

#ifdef MT7601
	if (IS_MT7601(pAd))
		return;
#endif /* MT7601 */

	_RTMP_BBP_IO_WRITE8(pAd, Offset, Value, FlgValidMCR);
}
#endif /* VENDOR_FEATURE3_SUPPORT */



VOID AntCfgInit(
IN  PRTMP_ADAPTER   pAd)
{

	
#ifdef TXRX_SW_ANTDIV_SUPPORT
	/* EEPROM 0x34[15:12] = 0xF is invalid, 0x2~0x3 is TX/RX SW AntDiv */
	DBGPRINT(RT_DEBUG_OFF, ("%s: bTxRxSwAntDiv %d\n", __FUNCTION__, pAd->chipCap.bTxRxSwAntDiv));
	if (pAd->chipCap.bTxRxSwAntDiv)
	{
		DBGPRINT(RT_DEBUG_OFF, ("Antenna word %X/%d, AntDiv %d\n",
					pAd->Antenna.word, pAd->Antenna.field.BoardType, pAd->NicConfig2.field.AntDiversity));
	}
#endif /* TXRX_SW_ANTDIV_SUPPORT */

	{
		if (pAd->NicConfig2.field.AntOpt == 1) /* ant selected by efuse */
		{	
			if (pAd->NicConfig2.field.AntDiversity == 0) /* main */
			{
				pAd->RxAnt.Pair1PrimaryRxAnt = 0;
				pAd->RxAnt.Pair1SecondaryRxAnt = 1;
			}
			else /* aux */
			{
				pAd->RxAnt.Pair1PrimaryRxAnt = 1;
				pAd->RxAnt.Pair1SecondaryRxAnt = 0;
			}
		}
		else if (pAd->NicConfig2.field.AntDiversity == 0) /* Ant div off: default ant is main */
		{
			pAd->RxAnt.Pair1PrimaryRxAnt = 0;
			pAd->RxAnt.Pair1SecondaryRxAnt = 1;
		}
		else if (pAd->NicConfig2.field.AntDiversity == 1)/* Ant div on */
		{ /* eeprom on, but sw ant div support is not enabled: default ant is main */
			pAd->RxAnt.Pair1PrimaryRxAnt = 0;
			pAd->RxAnt.Pair1SecondaryRxAnt = 1;
		}
	}

	DBGPRINT(RT_DEBUG_OFF, ("%s: primary/secondary ant %d/%d\n",
					__FUNCTION__,
					pAd->RxAnt.Pair1PrimaryRxAnt,
					pAd->RxAnt.Pair1SecondaryRxAnt));
}

#ifdef MICROWAVE_OVEN_SUPPORT
VOID NICUpdateRxStatusCnt1(
IN  PRTMP_ADAPTER   pAd,
IN  UCHAR	Idx)
{
	RX_STA_CNT1_STRUC   RxStaCnt1;

	RTMP_IO_READ32(pAd, RX_STA_CNT1, &RxStaCnt1.word);

	if (Idx < MLME_TASK_EXEC_MULTIPLE)
	{
		pAd->RalinkCounters.FalseCCACnt_100MS[Idx] = RxStaCnt1.field.FalseCca;
		pAd->RalinkCounters.PLCPErrCnt_100MS[Idx] = RxStaCnt1.field.PlcpErr;
	}
}

UINT32 NICSumFalseCCACnt(
IN  PRTMP_ADAPTER   pAd)
{
	UCHAR Idx;
	UINT32 FalseCCACnt = 0;

	for (Idx = 0; Idx < MLME_TASK_EXEC_MULTIPLE; Idx++)
		FalseCCACnt += pAd->RalinkCounters.FalseCCACnt_100MS[Idx];

	return FalseCCACnt;
}

UINT32 NICSumPLCPErrCnt(
IN  PRTMP_ADAPTER   pAd)
{
	UCHAR Idx;
	UINT32 PLCPErrCnt = 0;

	for (Idx = 0; Idx < MLME_TASK_EXEC_MULTIPLE; Idx++)
		PLCPErrCnt += pAd->RalinkCounters.PLCPErrCnt_100MS[Idx];

	return PLCPErrCnt;
}
#endif /* MICROWAVE_OVEN_SUPPORT */
