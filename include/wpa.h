/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	wpa.h

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Name		Date			Modification logs
*/

#ifndef	__WPA_H__
#define	__WPA_H__

#ifndef ROUND_UP
#define ROUND_UP(__x, __y) \
	(((unsigned long)((__x)+((__y)-1))) & ((unsigned long)~((__y)-1)))
#endif

#define	SET_UINT16_TO_ARRARY(_V, _LEN)		\
{											\
	_V[0] = ((unsigned short)_LEN) >> 8;			\
	_V[1] = ((unsigned short)_LEN & 0xFF);					\
}

#define	INC_UINT16_TO_ARRARY(_V, _LEN)			\
{												\
	unsigned short	var_len;							\
												\
	var_len = (_V[0]<<8) | (_V[1]);				\
	var_len += _LEN;							\
												\
	_V[0] = (var_len & 0xFF00) >> 8;			\
	_V[1] = (var_len & 0xFF);					\
}

#define	CONV_ARRARY_TO_UINT16(_V)	((_V[0]<<8) | (_V[1]))

#define	ADD_ONE_To_64BIT_VAR(_V)		\
{										\
	UCHAR	cnt = LEN_KEY_DESC_REPLAY;	\
	do									\
	{									\
		cnt--;							\
		_V[cnt]++;						\
		if (cnt == 0)					\
			break;						\
	}while (_V[cnt] == 0);				\
}

#define INC_TX_TSC(_tsc, _cnt)                          \
{                                                       \
    INT i=0;                                            \
	while (++_tsc[i] == 0x0)                            \
    {                                                   \
        i++;                                            \
		if (i == (_cnt))                                \
			break;                                      \
	}                                                   \
}

#define IS_WPA_CAPABILITY(a)       (((a) >= Ndis802_11AuthModeWPA) && ((a) <= Ndis802_11AuthModeWPA1PSKWPA2PSK))

/* 	
	WFA recommend to restrict the encryption type in 11n-HT mode.
 	So, the WEP and TKIP shall not be allowed to use HT rate. 
 */
#define IS_INVALID_HT_SECURITY(_mode)		\
	(((_mode) == Ndis802_11Encryption1Enabled) || \
	 ((_mode) == Ndis802_11Encryption2Enabled))

#define MIX_CIPHER_WPA_TKIP_ON(x)       (((x) & 0x08) != 0)
#define MIX_CIPHER_WPA_AES_ON(x)        (((x) & 0x04) != 0)
#define MIX_CIPHER_WPA2_TKIP_ON(x)      (((x) & 0x02) != 0)
#define MIX_CIPHER_WPA2_AES_ON(x)       (((x) & 0x01) != 0)

/* Some definition are different between Keneral mode and Daemon mode */
#ifdef WPA_DAEMON_MODE
/* The definition for Daemon mode */
#define WPA_GET_BSS_NUM(_pAd)		(_pAd)->mbss_num

#define WPA_GET_PMK(_pAd, _pEntry, _pmk)					\
{															\
	_pmk = _pAd->MBSS[_pEntry->apidx].PMK;					\
}

#define WPA_GET_GTK(_pAd, _pEntry, _gtk)					\
{															\
	_gtk = _pAd->MBSS[_pEntry->apidx].GTK;					\
}

#define WPA_GET_GROUP_CIPHER(_pAd, _pEntry, _cipher)		\
{															\
	_cipher = (_pAd)->MBSS[_pEntry->apidx].GroupEncrypType;	\
}

#define WPA_GET_DEFAULT_KEY_ID(_pAd, _pEntry, _idx)			\
{															\
	_idx = (_pAd)->MBSS[_pEntry->apidx].DefaultKeyId;		\
}

#define WPA_GET_BMCST_TSC(_pAd, _pEntry, _tsc)				\
{															\
	_tsc = 1;												\
}

#define WPA_BSSID(_pAd, _apidx)		(_pAd)->MBSS[_apidx].wlan_addr

#define WPA_OS_MALLOC(_p, _s)		\
{									\
	_p = os_malloc(_s);			\
}

#define WPA_OS_FREE(_p)		\
{								\
	os_free(_p);				\
}

#define WPA_GET_CURRENT_TIME(_time)		\
{										\
	struct timeval tv;					\
	gettimeofday(&tv, NULL);			\
	*(_time) = tv.tv_sec;					\
}

#else
/* The definition for Driver mode */

#if defined(CONFIG_AP_SUPPORT) && defined(CONFIG_STA_SUPPORT)
#define WPA_GET_BSS_NUM(_pAd)		(((_pAd)->OpMode == OPMODE_AP) ? (_pAd)->ApCfg.BssidNum : 1)
#define WPA_GET_GROUP_CIPHER(_pAd, _pEntry, _cipher)					\
	{																	\
	_cipher = Ndis802_11WEPDisabled;								\
		if ((_pAd)->OpMode == OPMODE_AP)								\
		{																\
		if (IS_ENTRY_APCLI(_pEntry) && 								\
			((_pEntry)->MatchAPCLITabIdx < MAX_APCLI_NUM))			\
			_cipher = (_pAd)->ApCfg.ApCliTab[(_pEntry)->MatchAPCLITabIdx].GroupCipher;	\
			else if ((_pEntry)->apidx < (_pAd)->ApCfg.BssidNum)			\
				_cipher = (_pAd)->ApCfg.MBSSID[_pEntry->apidx].GroupKeyWepStatus;\
		}																\
		else															\
			_cipher = (_pAd)->StaCfg.GroupCipher;						\
	}

#define WPA_BSSID(_pAd, _apidx) 	(((_pAd)->OpMode == OPMODE_AP) ?\
									(_pAd)->ApCfg.MBSSID[_apidx].Bssid :\
									(_pAd)->CommonCfg.Bssid)
#elif defined(CONFIG_AP_SUPPORT)
#define WPA_GET_BSS_NUM(_pAd)		(_pAd)->ApCfg.BssidNum
#define WPA_GET_GROUP_CIPHER(_pAd, _pEntry, _cipher)				\
	{																\
	_cipher = Ndis802_11WEPDisabled;							\
	if (IS_ENTRY_APCLI(_pEntry) && 								\
		((_pEntry)->MatchAPCLITabIdx < MAX_APCLI_NUM))			\
		_cipher = (_pAd)->ApCfg.ApCliTab[(_pEntry)->MatchAPCLITabIdx].GroupCipher;	\
		else if ((_pEntry)->apidx < (_pAd)->ApCfg.BssidNum)			\
			_cipher = (_pAd)->ApCfg.MBSSID[_pEntry->apidx].GroupKeyWepStatus;\
	}

#define WPA_BSSID(_pAd, _apidx) 	(_pAd)->ApCfg.MBSSID[_apidx].Bssid

#elif defined(CONFIG_STA_SUPPORT)
#define WPA_GET_BSS_NUM(_pAd)		1
#define WPA_GET_GROUP_CIPHER(_pAd, _pEntry, _cipher)				\
	{																\
		_cipher = (_pAd)->StaCfg.GroupCipher;						\
	}
#define WPA_BSSID(_pAd, _apidx) 	(_pAd)->CommonCfg.Bssid
#endif /* defined(CONFIG_STA_SUPPORT) */

#define WPA_OS_MALLOC(_p, _s)		\
{									\
	os_alloc_mem(NULL, (unsigned char * *)&_p, _s);		\
}

#define WPA_OS_FREE(_p)		\
{							\
	os_free_mem(NULL, _p);	\
}

#define WPA_GET_CURRENT_TIME(_time)		NdisGetSystemUpTime(_time);

#endif /* End of Driver Mode */

#ifdef CONFIG_AP_SUPPORT
/*========================================
	The prototype is defined in ap_wpa.c
  ========================================*/
VOID WPA_APSetGroupRekeyAction(
	IN PRTMP_ADAPTER pAd);

#endif /* CONFIG_AP_SUPPORT */

/*========================================
	The prototype is defined in cmm_wpa.c
  ========================================*/
void inc_iv_byte(
	UCHAR *iv,
	UINT len,
	UINT cnt);

BOOLEAN WpaMsgTypeSubst(
	IN UCHAR EAPType,
	OUT INT *MsgType);

VOID PRF(
	IN UCHAR *key,
	IN INT key_len,
	IN UCHAR *prefix,
	IN INT prefix_len,
	IN UCHAR *data,
	IN INT data_len,
	OUT UCHAR *output,
	IN INT len);

int RtmpPasswordHash(
	char *password,
	unsigned char *ssid,
	int ssidlength,
	unsigned char *output);

	VOID KDF(
	IN unsigned char * key,
	IN INT key_len,
	IN unsigned char * label,
	IN INT label_len,
	IN unsigned char * data,
	IN INT data_len,
	OUT unsigned char * output,
	IN USHORT len);

unsigned char * WPA_ExtractSuiteFromRSNIE(
	IN unsigned char * rsnie,
	IN UINT rsnie_len,
	IN unsigned char type,
	OUT unsigned char *count);

VOID WpaShowAllsuite(
	IN unsigned char * rsnie,
	IN UINT rsnie_len);

VOID RTMPInsertRSNIE(
	IN unsigned char * pFrameBuf,
	OUT unsigned long * pFrameLen,
	IN unsigned char * rsnie_ptr,
	IN unsigned char rsnie_len,
	IN unsigned char * pmkid_ptr,
	IN unsigned char pmkid_len);

/* 
 =====================================	
 	function prototype in cmm_wpa.c
 =====================================	
*/
VOID RTMPToWirelessSta(
	IN PRTMP_ADAPTER pAd,
	IN PMAC_TABLE_ENTRY pEntry,
	IN unsigned char * pHeader802_3,
	IN UINT HdrLen,
	IN unsigned char * pData,
	IN UINT DataLen,
	IN BOOLEAN bClearFrame);

VOID WpaDerivePTK(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR *PMK,
	IN UCHAR *ANonce,
	IN UCHAR *AA,
	IN UCHAR *SNonce,
	IN UCHAR *SA,
	OUT UCHAR *output,
	IN UINT len);

VOID WpaDeriveGTK(
	IN UCHAR *PMK,
	IN UCHAR *GNonce,
	IN UCHAR *AA,
	OUT UCHAR *output,
	IN UINT len);

VOID GenRandom(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR *macAddr,
	OUT UCHAR *random);

BOOLEAN RTMPCheckWPAframe(
	IN PRTMP_ADAPTER pAd,
	IN PMAC_TABLE_ENTRY pEntry,
	IN unsigned char * pData,
	IN unsigned long DataByteCount,
	IN UCHAR FromWhichBSSID);

#ifdef HDR_TRANS_SUPPORT
BOOLEAN RTMPCheckWPAframe_Hdr_Trns(
	IN PRTMP_ADAPTER pAd,
	IN PMAC_TABLE_ENTRY pEntry,
	IN unsigned char * pData,
	IN unsigned long DataByteCount,
	IN UCHAR FromWhichBSSID);
#endif /* HDR_TRANS_SUPPORT */

BOOLEAN RTMPParseEapolKeyData(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pKeyData,
	IN UCHAR KeyDataLen,
	IN UCHAR GroupKeyIndex,
	IN UCHAR MsgType,
	IN BOOLEAN bWPA2,
	IN MAC_TABLE_ENTRY *pEntry);

VOID WPA_ConstructKdeHdr(
	IN unsigned char data_type,
	IN unsigned char data_len,
	OUT unsigned char * pBuf);

VOID ConstructEapolMsg(
	IN PMAC_TABLE_ENTRY pEntry,
	IN UCHAR GroupKeyWepStatus,
	IN UCHAR MsgType,
	IN UCHAR DefaultKeyIdx,
	IN UCHAR *KeyNonce,
	IN UCHAR *TxRSC,
	IN UCHAR *GTK,
	IN UCHAR *RSNIE,
	IN UCHAR RSNIE_Len,
	OUT PEAPOL_PACKET pMsg);

PCIPHER_KEY RTMPSwCipherKeySelection(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pIV,
	IN RX_BLK *pRxBlk,
	IN PMAC_TABLE_ENTRY pEntry);

NDIS_STATUS RTMPSoftDecryptionAction(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pHdr,
	IN UCHAR UserPriority,
	IN PCIPHER_KEY pKey,
	INOUT unsigned char * pData,
	INOUT unsigned short *DataByteCnt);

VOID RTMPSoftConstructIVHdr(
	IN UCHAR CipherAlg,
	IN UCHAR key_id,
	IN unsigned char * pTxIv,
	OUT unsigned char * pHdrIv,
	OUT unsigned char *hdr_iv_len);

VOID RTMPSoftEncryptionAction(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR CipherAlg,
	IN unsigned char * pHdr,
	IN unsigned char * pSrcBufData,
	IN unsigned int SrcBufLen,
	IN UCHAR KeyIdx,
	IN PCIPHER_KEY pKey,
	OUT unsigned char *ext_len);

VOID RTMPMakeRSNIE(
	IN PRTMP_ADAPTER pAd,
	IN UINT AuthMode,
	IN UINT WepStatus,
	IN UCHAR apidx);

VOID WPAInstallPairwiseKey(
	PRTMP_ADAPTER pAd,
	unsigned char BssIdx,
	PMAC_TABLE_ENTRY pEntry,
	BOOLEAN bAE);

VOID WPAInstallSharedKey(
	PRTMP_ADAPTER pAd,
	unsigned char GroupCipher,
	unsigned char BssIdx,
	unsigned char KeyIdx,
	unsigned char Wcid,
	BOOLEAN bAE,
	unsigned char * pGtk,
	unsigned char GtkLen);

VOID RTMPSetWcidSecurityInfo(
	PRTMP_ADAPTER pAd,
	unsigned char BssIdx,
	unsigned char KeyIdx,
	unsigned char CipherAlg,
	unsigned char Wcid,
	unsigned char KeyTabFlag);

VOID CalculateMIC(
	IN UCHAR KeyDescVer,
	IN UCHAR *PTK,
	OUT PEAPOL_PACKET pMsg);

char * GetEapolMsgType(
	CHAR msg);


/* 
 =====================================	
 	function prototype in cmm_wep.c
 =====================================	
*/
UINT RTMP_CALC_FCS32(
	IN UINT Fcs,
	IN unsigned char * Cp,
	IN INT Len);

VOID RTMPConstructWEPIVHdr(
	IN unsigned char key_idx,
	IN UCHAR *pn,
	OUT UCHAR *iv_hdr);

BOOLEAN RTMPSoftEncryptWEP(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pIvHdr,
	IN PCIPHER_KEY pKey,
	INOUT unsigned char * pData,
	IN unsigned long DataByteCnt);

BOOLEAN RTMPSoftDecryptWEP(
	IN PRTMP_ADAPTER pAd,
	IN PCIPHER_KEY pKey,
	INOUT unsigned char * pData,
	INOUT unsigned short *DataByteCnt);

/* 
 =====================================	
 	function prototype in cmm_tkip.c
 =====================================	
*/
BOOLEAN RTMPSoftDecryptTKIP(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pHdr,
	IN UCHAR UserPriority,
	IN PCIPHER_KEY pKey,
	INOUT unsigned char * pData,
	IN unsigned short *DataByteCnt);

VOID TKIP_GTK_KEY_WRAP(
	IN UCHAR *key,
	IN UCHAR *iv,
	IN UCHAR *input_text,
	IN unsigned int input_len,
	OUT UCHAR *output_text);

VOID TKIP_GTK_KEY_UNWRAP(
	IN UCHAR *key,
	IN UCHAR *iv,
	IN UCHAR *input_text,
	IN unsigned int input_len,
	OUT UCHAR *output_text);

/* 
 =====================================	
 	function prototype in cmm_aes.c
 =====================================	
*/
BOOLEAN RTMPSoftDecryptAES(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pData,
	IN unsigned long DataByteCnt,
	IN PCIPHER_KEY pWpaKey);

VOID RTMPConstructCCMPHdr(
	IN unsigned char key_idx,
	IN UCHAR *pn,
	OUT UCHAR *ccmp_hdr);

BOOLEAN RTMPSoftEncryptCCMP(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pHdr,
	IN unsigned char * pIV,
	IN unsigned char * pKey,
	INOUT unsigned char * pData,
	IN unsigned int DataLen);

BOOLEAN RTMPSoftDecryptCCMP(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pHdr,
	IN PCIPHER_KEY pKey,
	INOUT unsigned char * pData,
	INOUT unsigned short *DataLen);

VOID CCMP_test_vector(
	IN PRTMP_ADAPTER pAd,
	IN INT input);

#endif
