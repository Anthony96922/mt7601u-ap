/*

*/


#include "rt_config.h"


#ifdef RLT_MAC

#ifndef MT7601

NDIS_STATUS NICInitBBP(RTMP_ADAPTER *pAd)
{
	INT idx;
	
	/* Read BBP register, make sure BBP is up and running before write new data*/
	if (rlt_bbp_is_ready(pAd) == FALSE)
		return NDIS_STATUS_FAILURE;

	/* re-config specific BBP registers for individual chip */
	if (pAd->chipCap.pBBPRegTable)
	{
		RTMP_REG_PAIR *reg = (RTMP_REG_PAIR *)pAd->chipCap.pBBPRegTable;
		
		for (idx = 0; idx < pAd->chipCap.bbpRegTbSize; idx++)
		{
			RTMP_BBP_IO_WRITE32(pAd, reg[idx].Register, reg[idx].Value);
			DBGPRINT(RT_DEBUG_TRACE, ("BBP[%x]=0x%x\n", 
					reg[idx].Register, reg[idx].Value));
		}
	}

	if (pAd->chipOps.AsicBbpInit != NULL)
		pAd->chipOps.AsicBbpInit(pAd);

	// TODO: shiang-6590, check these bbp registers if need to remap to new BBP_Registers

	return NDIS_STATUS_SUCCESS;
	
}


INT rtmp_bbp_set_txdac(struct _RTMP_ADAPTER *pAd, INT tx_dac)
{
	UINT32 txbe, txbe_r5 = 0;
	
	RTMP_BBP_IO_READ32(pAd, TXBE_R5, &txbe_r5);
	txbe = txbe_r5 & (~0x3);
	switch (tx_dac)
	{
		case 2:
			txbe |= 0x3;
			break;
		case 1:
		case 0:
		default:		
			txbe &= (~0x3);
			break;
	}

	if (txbe != txbe_r5)
		RTMP_BBP_IO_WRITE32(pAd, TXBE_R5, txbe);

	return TRUE;
}


INT rtmp_bbp_set_rxpath(struct _RTMP_ADAPTER *pAd, INT rxpath)
{
	UINT32 agc, agc_r0 = 0;

	RTMP_BBP_IO_READ32(pAd, AGC1_R0, &agc_r0);
	agc = agc_r0 & (~0x18);
	if(rxpath == 2)
		agc |= (0x8);
	else if(rxpath == 1)
		agc |= (0x0);

	if (agc != agc_r0)
		RTMP_BBP_IO_WRITE32(pAd, AGC1_R0, agc);

//DBGPRINT(RT_DEBUG_OFF, ("%s(): rxpath=%d, Set AGC1_R0=0x%x, agc_r0=0x%x\n", __FUNCTION__, rxpath, agc, agc_r0));
//		RTMP_BBP_IO_READ32(pAd, AGC1_R0, &agc);
//DBGPRINT(RT_DEBUG_OFF, ("%s(): rxpath=%d, After write, Get AGC1_R0=0x%x,\n", __FUNCTION__, rxpath, agc));

	return TRUE;
}


static UCHAR vht_prim_ch_val[] = {
	42, 36, 0,
	42, 40, 1,
	42, 44, 2,
	42, 48, 3,
	58, 52, 0,
	58, 56, 1,
	58, 60, 2,
	58, 64, 3,
	106, 100, 0,
	106, 104, 1,
	106, 108, 2,
	106, 112, 3,
	122, 116, 0,
	122, 120, 1,
	122, 124, 2,
	122, 128, 3,
	138, 132, 0,
	138, 136, 1,
	138, 140, 2,
	138, 144, 3,
	155, 149, 0,
	155, 153, 1,
	155, 157, 2,
	155, 161, 3
};

	
INT rtmp_bbp_set_ctrlch(struct _RTMP_ADAPTER *pAd, INT ext_ch)
{
	UINT32 agc, agc_r0 = 0;
	UINT32 be, be_r0 = 0;


	RTMP_BBP_IO_READ32(pAd, AGC1_R0, &agc_r0);
	agc = agc_r0 & (~0x300);
	RTMP_BBP_IO_READ32(pAd, TXBE_R0, &be_r0);
	be = (be_r0 & (~0x03));
#ifdef DOT11_VHT_AC
	if (pAd->CommonCfg.BBPCurrentBW == BW_80 && 
		pAd->CommonCfg.Channel >= 36 &&
		pAd->CommonCfg.vht_cent_ch)
	{
		if (pAd->CommonCfg.Channel < pAd->CommonCfg.vht_cent_ch)
		{
			switch (pAd->CommonCfg.vht_cent_ch - pAd->CommonCfg.Channel)
			{
				case 6:
					be |= 0;
					agc |=0x000;
					break;
				case 2:
					be |= 1;
					agc |=0x100;
					break;

			}
		}
		else if (pAd->CommonCfg.Channel > pAd->CommonCfg.vht_cent_ch)
		{
			switch (pAd->CommonCfg.Channel - pAd->CommonCfg.vht_cent_ch)
			{
				case 6:
					be |= 0x3;
					agc |=0x300;
					break;
				case 2:
					be |= 0x2;
					agc |=0x200;
					break;
			}
		}
	}
	else
#endif /* DOT11_VHT_AC */
	{
		switch (ext_ch)
		{
			case EXTCHA_BELOW:
				agc |= 0x100;
				be |= 0x01;
				break;
			case EXTCHA_ABOVE:
				agc &= (~0x300);
				be &= (~0x03);
				break;
			case EXTCHA_NONE:
			default:
				agc &= (~0x300);
				be &= (~0x03);
				break;
		}
	}
	if (agc != agc_r0)
		RTMP_BBP_IO_WRITE32(pAd, AGC1_R0, agc);

	if (be != be_r0)
		RTMP_BBP_IO_WRITE32(pAd, TXBE_R0, be);

//DBGPRINT(RT_DEBUG_OFF, ("%s(): ext_ch=%d, Set AGC1_R0=0x%x, agc_r0=0x%x\n", __FUNCTION__, ext_ch, agc, agc_r0));
//		RTMP_BBP_IO_READ32(pAd, AGC1_R0, &agc);
//DBGPRINT(RT_DEBUG_OFF, ("%s(): ext_ch=%d, After write, Get AGC1_R0=0x%x,\n", __FUNCTION__, ext_ch, agc));

	return TRUE;
}

/*
	<<Gamma2.1 Control Registers Rev1.3.pdf>>
	BBP bandwidth (CORE_R1[4:3]) change procedure:
	1. Hold BBP in reset by setting CORE_R4[0] to '1'
	2. Wait 0.5 us to ensure BBP is in the idle State
	3. Change BBP bandwidth with CORE_R1[4:3]
		CORE_R1 (Bit4:3)
		0: 20MHz
		1: 10MHz (11J)
		2: 40MHz
		3: 80MHz
	4. Wait 0.5 us for BBP clocks to settle
	5. Release BBP from reset by clearing CORE_R4[0]
*/
INT rtmp_bbp_set_bw(struct _RTMP_ADAPTER *pAd, INT bw)
{
	UINT32 core, core_r1 = 0, core_r4 = 0;
	UINT32 agc, agc_r0 = 0;

	RTMP_BBP_IO_READ32(pAd, CORE_R1, &core_r1);
	core = (core_r1 & (~0x18));
	RTMP_BBP_IO_READ32(pAd, AGC1_R0, &agc_r0);
	agc = agc_r0 & (~0x7000);
	switch (bw)
	{
		case BW_80:
			core |= 0x18;
			agc |= 0x7000;
			break;
		case BW_40:
			core |= 0x10;
			agc |= 0x3000;
			break;
		case BW_20:
			core &= (~0x18);
			agc |= 0x1000;
			break;
		case BW_10:
			core |= 0x08;
			agc |= 0x1000;
			break;
	}

	if (core != core_r1) 
	{
#ifdef RT65xx
		if (IS_RT6590(pAd))
		{
			/*
				Hold BBP in reset by setting CORE_R4[0]=1
			*/
			RTMP_BBP_IO_READ32(pAd, CORE_R4, &core_r4);
			core_r4 |= 0x00000001;
			RTMP_BBP_IO_WRITE32(pAd, CORE_R4, core_r4);

			/*
				Wait 0.5 us to ensure BBP is in the idle state.
			*/
			RtmpOsUsDelay(1);
		}
#endif /* RT65xx */
	

		RTMP_BBP_IO_WRITE32(pAd, CORE_R1, core);

#ifdef RT65xx
		if (IS_RT6590(pAd))
		{
			/*
				Wait 0.5 us for BBP clocks to settle.
			*/
			RtmpOsUsDelay(1);

			/*
				Release BBP from reset by clearing CORE_R4[0].
			*/
			RTMP_BBP_IO_READ32(pAd, CORE_R4, &core_r4);
			core_r4 &= ~(0x00000001);
			RTMP_BBP_IO_WRITE32(pAd, CORE_R4, core_r4);
		}
#endif /* RT65xx */	
	}

	if (agc != agc_r0) {
		RTMP_BBP_IO_WRITE32(pAd, AGC1_R0, agc);
//DBGPRINT(RT_DEBUG_OFF, ("%s(): bw=%d, Set AGC1_R0=0x%x, agc_r0=0x%x\n", __FUNCTION__, bw, agc, agc_r0));
//		RTMP_BBP_IO_READ32(pAd, AGC1_R0, &agc);
//DBGPRINT(RT_DEBUG_OFF, ("%s(): bw=%d, After write, Get AGC1_R0=0x%x,\n", __FUNCTION__, bw, agc));
	}

	pAd->CommonCfg.BBPCurrentBW = bw;

	return TRUE;
}


INT rtmp_bbp_set_mmps(struct _RTMP_ADAPTER *pAd, BOOLEAN ReduceCorePower)
{
	UINT32 bbp_val, org_val;

	RTMP_BBP_IO_READ32(pAd, AGC1_R0, &org_val);
	bbp_val = org_val;
	if (ReduceCorePower)
		bbp_val |= 0x04;
	else
		bbp_val &= ~0x04;

	if (bbp_val != org_val)
		RTMP_BBP_IO_WRITE32(pAd, AGC1_R0, bbp_val);

	return TRUE;
}


INT rtmp_bbp_get_agc(struct _RTMP_ADAPTER *pAd, CHAR *agc, RX_CHAIN_IDX chain)
{
	UCHAR idx, val;
	UINT32 bbp_val, bbp_reg = AGC1_R8;


	if (((pAd->MACVersion & 0xffff0000) < 0x28830000) || 
		(pAd->Antenna.field.RxPath == 1))
	{
		chain = RX_CHAIN_0;
	}

	idx = val = 0;
	while(chain != 0)
	{
		if (idx >= pAd->Antenna.field.RxPath)
			break;

		if (chain & 0x01)
		{
			RTMP_BBP_IO_READ32(pAd, bbp_reg, &bbp_val);
			val = ((bbp_val & (0x0000ff00)) >> 8) & 0xff;
			break;
		}
		chain >>= 1;
		bbp_reg += 4;
		idx++;
	}

	*agc = val;

	return NDIS_STATUS_SUCCESS;
}


INT rtmp_bbp_set_agc(struct _RTMP_ADAPTER *pAd, UCHAR agc, RX_CHAIN_IDX chain)
{
	UCHAR idx = 0;
	UINT32 bbp_val, bbp_reg = AGC1_R8;
	
	if (((pAd->MACVersion & 0xf0000000) < 0x28830000) || 
		(pAd->Antenna.field.RxPath == 1))
	{
		chain = RX_CHAIN_0;
	}

	while (chain != 0)
	{
		if (idx >= pAd->Antenna.field.RxPath)
			break;
		
		if (idx & 0x01)
		{
			RTMP_BBP_IO_READ32(pAd, bbp_reg, &bbp_val);
			bbp_val = (bbp_val & 0xffff00ff) | (agc << 8);
			RTMP_BBP_IO_WRITE32(pAd, bbp_reg, bbp_val);

			DBGPRINT(RT_DEBUG_INFO, 
					("%s(Idx):Write(R%d,val:0x%x) to Chain(0x%x, idx:%d)\n",
						__FUNCTION__, bbp_reg, bbp_val, chain, idx));
		}
		chain >>= 1;
		bbp_reg += 4;
		idx++;
	}

	return TRUE;
}


INT rtmp_bbp_set_filter_coefficient_ctrl(RTMP_ADAPTER *pAd, UCHAR Channel)
{
	UINT32 bbp_val = 0, org_val = 0;

	if (Channel == 14)
	{
		/* when Channel==14 && Mode==CCK && BandWidth==20M, BBP R4 bit5=1 */
		RTMP_BBP_IO_READ32(pAd, CORE_R1, &org_val);
		bbp_val = org_val;
		if (WMODE_EQUAL(pAd->CommonCfg.PhyMode, WMODE_B))
			bbp_val |= 0x20;
		else
			bbp_val &= (~0x20);

		if (bbp_val != org_val)
			RTMP_BBP_IO_WRITE32(pAd, CORE_R1, bbp_val);
	}

	return TRUE;
}


UCHAR rtmp_bbp_get_random_seed(RTMP_ADAPTER *pAd)
{
	UINT32 value, value2;
	UCHAR seed;

	RTMP_BBP_IO_READ32(pAd, AGC1_R16, &value);
	seed = (UCHAR)((value & 0xff) ^ ((value & 0xff00) >> 8)^ 
					((value & 0xff0000) >> 16));
	RTMP_BBP_IO_READ32(pAd, RXO_R9, &value2);

	return (UCHAR)(seed ^ (value2 & 0xff)^ ((value2 & 0xff00) >> 8));
}


INT rlt_bbp_is_ready(struct _RTMP_ADAPTER *pAd)
{
	INT idx = 0;
	UINT32 val;
	
	do 
	{
		RTMP_BBP_IO_READ32(pAd, CORE_R0, &val);
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))			
			return FALSE;
		DBGPRINT(RT_DEBUG_TRACE, ("BBP version = %x\n", val));
	} while ((++idx < 20) && ((val == 0xffffffff) || (val == 0x0)));

	return (((val == 0xffffffff) || (val == 0x0)) ? FALSE : TRUE);
}

#endif /* MT7601 */

#endif /* RLT_MAC */

