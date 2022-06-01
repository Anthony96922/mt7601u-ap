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

	All MAC80211/CFG80211 Function Prototype.

***************************************************************************/

#ifdef RT_CFG80211_SUPPORT

#define RT_CFG80211_REGISTER(__pDev, __pNetDev)								\
	CFG80211_Register(__pDev, __pNetDev);

#define RT_CFG80211_BEACON_CR_PARSE(__pAd, __pVIE, __LenVIE)				\
	CFG80211_BeaconCountryRegionParse((void *)__pAd, __pVIE, __LenVIE);

#define RT_CFG80211_CRDA_REG_HINT(__pAd, __pCountryIe, __CountryIeLen)		\
	CFG80211_RegHint((void *)__pAd, __pCountryIe, __CountryIeLen);

#define RT_CFG80211_CRDA_REG_HINT11D(__pAd, __pCountryIe, __CountryIeLen)	\
	CFG80211_RegHint11D((void *)__pAd, __pCountryIe, __CountryIeLen);

#define RT_CFG80211_CRDA_REG_RULE_APPLY(__pAd)								\
	CFG80211_RegRuleApply((void *)__pAd, NULL, __pAd->Cfg80211_Alpha2);

#define RT_CFG80211_SCANNING_INFORM(__pAd, __BssIdx, __ChanId, __pFrame,	\
			__FrameLen, __RSSI)									\
	CFG80211_Scaning((void *)__pAd, __BssIdx, __ChanId, __pFrame,			\
						__FrameLen, __RSSI);

#define RT_CFG80211_SCAN_END(__pAd, __FlgIsAborted)							\
	CFG80211_ScanEnd((void *)__pAd, __FlgIsAborted);

#define RT_CFG80211_REINIT(__pAd)											\
	CFG80211_SupBandReInit((void *)__pAd);									\

#define RT_CFG80211_CONN_RESULT_INFORM(__pAd, __pBSSID, __pReqIe, __ReqIeLen,\
			__pRspIe, __RspIeLen, __FlgIsSuccess)							\
	CFG80211_ConnectResultInform((void *)__pAd, __pBSSID,					\
			__pReqIe, __ReqIeLen, __pRspIe, __RspIeLen, __FlgIsSuccess);

#define RT_CFG80211_RFKILL_STATUS_UPDATE(_pAd, _active) \
	CFG80211_RFKillStatusUpdate(_pAd, _active);

#ifdef SINGLE_SKU
#define CFG80211_BANDINFO_FILL(__pAd, __pBandInfo)							\
{																			\
	(__pBandInfo)->RFICType = __pAd->RFICType;								\
	(__pBandInfo)->MpduDensity = __pAd->CommonCfg.BACapability.field.MpduDensity;\
	(__pBandInfo)->TxStream = __pAd->CommonCfg.TxStream;					\
	(__pBandInfo)->RxStream = __pAd->CommonCfg.RxStream;					\
	(__pBandInfo)->MaxTxPwr = __pAd->CommonCfg.DefineMaxTxPwr;				\
	if (WMODE_EQUAL(__pAd->CommonCfg.PhyMode, WMODE_B))				\
		(__pBandInfo)->FlgIsBMode = TRUE;									\
	else																	\
		(__pBandInfo)->FlgIsBMode = FALSE;									\
	(__pBandInfo)->MaxBssTable = MAX_LEN_OF_BSS_TABLE;						\
	(__pBandInfo)->RtsThreshold = pAd->CommonCfg.RtsThreshold;				\
	(__pBandInfo)->FragmentThreshold = pAd->CommonCfg.FragmentThreshold;	\
	(__pBandInfo)->RetryMaxCnt = 0;											\
	RTMP_IO_READ32(__pAd, TX_RTY_CFG, &((__pBandInfo)->RetryMaxCnt));		\
}
#else
#define CFG80211_BANDINFO_FILL(__pAd, __pBandInfo)							\
{																			\
	(__pBandInfo)->RFICType = __pAd->RFICType;								\
	(__pBandInfo)->MpduDensity = __pAd->CommonCfg.BACapability.field.MpduDensity;\
	(__pBandInfo)->TxStream = __pAd->CommonCfg.TxStream;					\
	(__pBandInfo)->RxStream = __pAd->CommonCfg.RxStream;					\
	(__pBandInfo)->MaxTxPwr = 0;											\
	if (WMODE_EQUAL(__pAd->CommonCfg.PhyMode, WMODE_B))				\
		(__pBandInfo)->FlgIsBMode = TRUE;									\
	else																	\
		(__pBandInfo)->FlgIsBMode = FALSE;									\
	(__pBandInfo)->MaxBssTable = MAX_LEN_OF_BSS_TABLE;						\
	(__pBandInfo)->RtsThreshold = pAd->CommonCfg.RtsThreshold;				\
	(__pBandInfo)->FragmentThreshold = pAd->CommonCfg.FragmentThreshold;	\
	(__pBandInfo)->RetryMaxCnt = 0;											\
	RTMP_IO_READ32(__pAd, TX_RTY_CFG, &((__pBandInfo)->RetryMaxCnt));		\
}
#endif /* SINGLE_SKU */


/* utilities used in DRV module */
int CFG80211DRV_IoctlHandle(
	IN	void					*pAdSrc,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq,
	IN	int						cmd,
	IN	unsigned short					subcmd,
	IN	void					*pData,
	IN	unsigned long					Data);

bool CFG80211DRV_OpsSetChannel(
	void						*pAdOrg,
	void						*pData);

bool CFG80211DRV_OpsChgVirtualInf(
	void						*pAdOrg,
	void						*pFlgFilter,
	unsigned char						IfType);

bool CFG80211DRV_OpsScan(
	void						*pAdOrg);

bool CFG80211DRV_OpsJoinIbss(
	void						*pAdOrg,
	void						*pData);

bool CFG80211DRV_OpsLeave(
	void						*pAdOrg);

bool CFG80211DRV_StaGet(
	void						*pAdOrg,
	void						*pData);

bool CFG80211DRV_Connect(
	void						*pAdOrg,
	void						*pData);

bool CFG80211DRV_KeyAdd(
	void						*pAdOrg,
	void						*pData);

void CFG80211DRV_RegNotify(
	void						*pAdOrg,
	void						*pData);

void CFG80211DRV_SurveyGet(
	void						*pAdOrg,
	void						*pData);

void CFG80211DRV_PmkidConfig(
	void						*pAdOrg,
	void						*pData);

void CFG80211_RegHint(
	IN void						*pAdCB,
	IN unsigned char					*pCountryIe,
	IN unsigned long					CountryIeLen);

void CFG80211_RegHint11D(
	IN void						*pAdCB,
	IN unsigned char					*pCountryIe,
	IN unsigned long					CountryIeLen);

void CFG80211_ScanEnd(
	IN void						*pAdCB,
	IN bool					FlgIsAborted);

void CFG80211_ConnectResultInform(
	IN void						*pAdCB,
	IN unsigned char					*pBSSID,
	IN unsigned char					*pReqIe,
	IN unsigned int					ReqIeLen,
	IN unsigned char					*pRspIe,
	IN unsigned int					RspIeLen,
	IN unsigned char					FlgIsSuccess);

bool CFG80211_SupBandReInit(
	IN void						*pAdCB);

void CFG80211_RegRuleApply(
	IN void						*pAdCB,
	IN void						*pWiphy,
	IN unsigned char					*pAlpha2);

void CFG80211_Scaning(
	IN void						*pAdCB,
	IN unsigned int					BssIdx,
	IN unsigned int					ChanId,
	IN unsigned char					*pFrame,
	IN unsigned int					FrameLen,
	IN int					RSSI);

#ifdef RFKILL_HW_SUPPORT
void CFG80211_RFKillStatusUpdate(
	IN void *					pAd,
	IN bool					active);
#endif /* RFKILL_HW_SUPPORT */

void CFG80211_UnRegister(
	IN void						*pAdOrg,
	IN void						*pNetDev);

#endif /* RT_CFG80211_SUPPORT */

/* End of cfg80211extr.h */
