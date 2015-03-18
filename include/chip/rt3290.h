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
	rt3290.h
 
    Abstract:
 
    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
 */

#ifndef __RT3290_H__
#define __RT3290_H__

#ifdef RT3290

#error "For RT3290, you should define the compile flag -DRTMP_PCI_SUPPORT"

#error "For RT3290, you should define the compile flag -DRTMP_MAC_PCI"

#error "For RT3290, you should define the compile flag -DRTMP_RF_RW_SUPPORT"

#error "For RT3290, you should define the compile flag -DRT30xx"

#include "chip/mac_pci.h"
#include "chip/rt30xx.h"

struct _RTMP_ADAPTER;


#define NIC3290_PCIe_DEVICE_ID 0x3290


#define RT3290_CHECK_SW_EEP_BUSY(pAd)	\
{	\
	UINT32 _val, _cnt = 0;	\
	do {	\
		RTMP_IO_FORCE_READ32(pAd, WLAN_FUN_INFO, &_val);	\
		if ((_val & 0x80000000) == 0 || (_val == 0xffffffff))	\
			break;	\
		_cnt++;	\
		DBGPRINT_ERR(("RT3290: EEP is busy!!!! BusyCnt%d : fail\n",  _cnt));	\
		RTMPusecDelay(500);	\
	} while (_cnt<300);	\
}	\

#define RT3290_CURRENT_LEAKAGE(_pAd,_A,_e)	\
{	\
	if (IS_RT3290(_pAd))	\
	{	\
		UINT32 btFunInfo, _val = 0;	\
		RTMP_IO_FORCE_READ32(_pAd, _A, &_val);	\
		if (_e)	\
			_val &= ~(EESK|EEDI);		\
		else	\
		{	\
			_val &= ~(EESK);		\
			_val |= EEDI;			\
		}	\
		RT3290_CHECK_SW_EEP_BUSY(_pAd);	\
		RTMP_IO_FORCE_READ32(_pAd, BT_FUN_INFO, &btFunInfo);	\
		btFunInfo |= 0x80000000;	\
		RTMP_IO_FORCE_WRITE32(_pAd, BT_FUN_INFO, btFunInfo);	\
		\
		RTMP_IO_FORCE_WRITE32(_pAd, _A, _V);	\
		\
		btFunInfo &= ~(0x80000000);	\
		RTMP_IO_FORCE_WRITE32(_pAd, BT_FUN_INFO, btFunInfo);	\
	}	\
}


VOID MlmeAntSelection(
	IN struct _RTMP_ADAPTER *pAd,
	IN ULONG	 AccuTxTotalCnt,
	IN ULONG	 TxErrorRatio,
	IN ULONG TxSuccess,
	IN CHAR Rssi);

INT RT3290_eeprom_access_grant(
	IN struct _RTMP_ADAPTER *pAd,
	IN BOOLEAN bGetCtrl);
	
VOID RTMP_MAC_PWRSV_EN(
	IN struct _RTMP_ADAPTER *pAd,
	IN BOOLEAN EnterIdle,
	IN BOOLEAN use40M);

VOID RTMPEnableWlan(
	IN struct _RTMP_ADAPTER *pAd,
	IN BOOLEAN bOn,
	IN BOOLEAN bResetWLAN);

VOID RT3290_Init(
	IN struct _RTMP_ADAPTER *pAd);

#endif /* RT3290 */

#endif /* __RT5390_H__ */

