#ifndef __AP_CFG_H__
#define __AP_CFG_H__


#include "rt_config.h"

int RTMPAPPrivIoctlSet(
	IN RTMP_ADAPTER *pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT *pIoctlCmdStr);

int RTMPAPPrivIoctlShow(
	IN RTMP_ADAPTER *pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT *pIoctlCmdStr);

#ifdef INF_AR9
#ifdef AR9_MAPI_SUPPORT
int RTMPAPPrivIoctlAR9Show(
	IN RTMP_ADAPTER *pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT *pIoctlCmdStr);

void RTMPAR9IoctlGetMacTable(
	IN PRTMP_ADAPTER pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);

void RTMPIoctlGetSTAT2(
	IN PRTMP_ADAPTER pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);

void RTMPIoctlGetRadioDynInfo(
	IN PRTMP_ADAPTER pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);
#endif /*AR9_MAPI_SUPPORT*/
#endif/* INF_AR9 */

int RTMPAPSetInformation(
	IN	PRTMP_ADAPTER	pAd,
	IN	OUT	RTMP_IOCTL_INPUT_STRUCT	*rq,
	IN	int				cmd);

int RTMPAPQueryInformation(
	IN	PRTMP_ADAPTER       pAd,
	IN	OUT	RTMP_IOCTL_INPUT_STRUCT    *rq,
	IN	int                 cmd);
	
void RTMPIoctlStatistics(
	IN PRTMP_ADAPTER pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);


void RTMPIoctlGetMacTable(
	IN PRTMP_ADAPTER pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);


void RTMPAPIoctlE2PROM(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  RTMP_IOCTL_INPUT_STRUCT    *wrq);

#ifdef DBG
void RTMPAPIoctlBBP(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  RTMP_IOCTL_INPUT_STRUCT    *wrq);

void RTMPAPIoctlMAC(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  RTMP_IOCTL_INPUT_STRUCT    *wrq);


#endif /* DBG */

void RtmpDrvMaxRateGet(
	IN	void					*pReserved,
/*	IN	PHTTRANSMIT_SETTING		pHtPhyMode, */
	IN	unsigned char					MODE,
	IN	unsigned char					ShortGI,
	IN	unsigned char					BW,
	IN	unsigned char					MCS,
	OUT	unsigned int					*pRate);

#ifdef WSC_AP_SUPPORT
void RTMPIoctlWscProfile(
	IN PRTMP_ADAPTER pAdapter, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);

void RTMPIoctlWscProfile(
	IN PRTMP_ADAPTER pAdapter, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);
/*add by woody */
#ifdef INF_AR9
#ifdef AR9_MAPI_SUPPORT
void RTMPAR9IoctlWscProfile(
	IN PRTMP_ADAPTER pAdapter, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);

void RTMPIoctlWscPINCode(
	IN PRTMP_ADAPTER pAdapter, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);


void RTMPIoctlWscStatus(
	IN PRTMP_ADAPTER pAdapter, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);

void RTMPIoctlGetWscDynInfo(
	IN PRTMP_ADAPTER pAdapter, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);

void RTMPIoctlGetWscRegsDynInfo(
	IN PRTMP_ADAPTER pAdapter, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);
#endif/*AR9_MAPI_SUPPORT*/
#endif/* INF_AR9 */
#endif /* WSC_AP_SUPPORT */

#ifdef DOT11_N_SUPPORT
void RTMPIoctlQueryBaTable(
	IN	PRTMP_ADAPTER	pAd, 
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq);
#endif /* DOT11_N_SUPPORT */

#ifdef DOT1X_SUPPORT
void RTMPIoctlStaticWepCopy(
	IN	PRTMP_ADAPTER	pAd, 
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq);

void RTMPIoctlRadiusData(
	IN PRTMP_ADAPTER	pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT		*wrq);

void RTMPIoctlAddWPAKey(
	IN	PRTMP_ADAPTER	pAd, 
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq);

void RTMPIoctlAddPMKIDCache(
	IN	PRTMP_ADAPTER	pAd, 
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq);

void RTMPIoctlSetIdleTimeout(
	IN	PRTMP_ADAPTER	pAd, 
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq);
#endif /* DOT1X_SUPPORT */

int	ApCfg_Set_AuthMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	int				apidx,
	IN	char *			arg);

int	ApCfg_Set_MaxStaNum_Proc(
	IN PRTMP_ADAPTER 	pAd,
	IN int				apidx,
	IN char * 			arg);

int	ApCfg_Set_IdleTimeout_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);


#ifdef APCLI_SUPPORT
#ifdef APCLI_WPA_SUPPLICANT_SUPPORT
void RTMPApCliAddKey(
	IN	PRTMP_ADAPTER	    pAd, 
	IN 	int				apidx,
	IN	PNDIS_APCLI_802_11_KEY    pApcliKey);
#endif /* APCLI_WPA_SUPPLICANT_SUPPORT */
#endif /* APCLI_SUPPORT */
#endif /* __AP_CFG_H__ */

