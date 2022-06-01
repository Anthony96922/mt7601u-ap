/****************************************************************************

    Module Name:
    rt_os_util.c
 
    Abstract:
	All functions provided from UTIL module are put here (OS independent).
 
    Revision History:
    Who        When          What
    ---------  ----------    ----------------------------------------------

***************************************************************************/

#define RTMP_MODULE_OS
#define RTMP_MODULE_OS_UTIL

/*#include "rt_config.h"
*/
#include "rtmp_comm.h"
#include "rt_os_util.h"
#include "rtmp_osabl.h"


unsigned int RalinkRate[256] =
	{2,  4, 11, 22, 12, 18,   24,  36, 48, 72, 96, 108, 109, 110, 111, 112,
	13, 26,   39,  52,  78, 104, 117, 130, 26,  52,  78, 104, 156, 208, 234, 260,
	39, 78,  117, 156, 234, 312, 351, 390,
	27, 54,   81, 108, 162, 216, 243, 270, 54, 108, 162, 216, 324, 432, 486, 540,
	81, 162, 243, 324, 486, 648, 729, 810,
	14, 29,   43,  57,  87, 115, 130, 144, 29, 59,   87, 115, 173, 230, 260, 288,
	43, 87,  130, 173, 260, 317, 390, 433,
	30, 60,   90, 120, 180, 240, 270, 300, 60, 120, 180, 240, 360, 480, 540, 600,
	90, 180, 270, 360, 540, 720, 810, 900,
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,
	20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,
	40,41,42,43,44,45,46,47}; /* 3*3
*/

VOID RtmpDrvMaxRateGet(
	IN	VOID					*pReserved,
/*	IN	PHTTRANSMIT_SETTING		pHtPhyMode,
*/
	IN	unsigned char					MODE,
	IN	unsigned char					ShortGI,
	IN	unsigned char					BW,
	IN	unsigned char					MCS,
	OUT	unsigned int					*pRate)
{
	int rate_index = 0;

#ifdef DOT11_N_SUPPORT
	if (MODE >= MODE_HTMIX)
	{
	/*                	rate_index = 16 + ((unsigned char)pHtPhyMode->field.BW *16) + ((unsigned char)pHtPhyMode->field.ShortGI *32) + ((unsigned char)pHtPhyMode->field.MCS);
*/
		rate_index = 16 + ((unsigned char)BW *24) + ((unsigned char)ShortGI *48) + ((unsigned char)MCS);
	}
	else 
#endif /* DOT11_N_SUPPORT */
		if (MODE == MODE_OFDM)
			rate_index = (unsigned char)(MCS) + 4;
		else 
			rate_index = (unsigned char)(MCS);

	if (rate_index < 0)
		rate_index = 0;

	if (rate_index > 255)
		rate_index = 255;
    
	*pRate = RalinkRate[rate_index] * 500000;
}


char *rtstrchr(const char * s, int c)
{
    for(; *s != (char) c; ++s)
        if (*s == '\0')
            return NULL;
    return (char *) s;
}


VOID RtmpMeshDown(
	IN VOID *pDrvCtrlBK,
	IN bool WaitFlag,
	IN bool	 (*RtmpMeshLinkCheck)(IN VOID *pAd))
{
}



	
bool RtmpOsCmdDisplayLenCheck(
	IN unsigned int LenSrc,
	IN unsigned int Offset)
{
	if (LenSrc > (IW_PRIV_SIZE_MASK - Offset))
		return FALSE;

	return TRUE;
}


#if defined(WPA_SUPPLICANT_SUPPORT) || defined(APCLI_WPA_SUPPLICANT_SUPPORT)
VOID WpaSendMicFailureToWpaSupplicant(
	IN PNET_DEV pNetDev,
	IN bool bUnicast)
{    
	char custom[IW_CUSTOM_MAX] = {0};
    
	snprintf(custom, sizeof(custom), "MLME-MICHAELMICFAILURE.indication");
	if(bUnicast)
		sprintf(custom, "%s unicast", custom);

	RtmpOSWrielessEventSend(pNetDev, RT_WLAN_EVENT_CUSTOM, -1, NULL, (unsigned char *)custom, strlen(custom));
	
	return;
}
#endif /* defined(WPA_SUPPLICANT_SUPPORT) || defined(APCLI_WPA_SUPPLICANT_SUPPORT) */






int  RtPrivIoctlSetVal(VOID)
{
    return (int)RTPRIV_IOCTL_SET;
}


#ifdef RTMP_USB_SUPPORT
void * RtmpInitCompletion(VOID)
{
	struct completion *comp = NULL;
	os_alloc_mem(NULL, (unsigned char **)&comp, sizeof(struct completion));
	init_completion(comp);
	return (void *)comp;
}


unsigned long RtmpWaitForCompletionTimeout(VOID *Completion, unsigned long Expire)
{
	return wait_for_completion_timeout((struct completion *)Completion, Expire);
}


VOID RtmpComplete(VOID *Completion)
{
	complete((struct completion *)Completion);
}
#endif /* RTMP_USB_SUPPORT */

unsigned long RtmpMsecsToJiffies(unsigned int msecs)
{
	return msecs_to_jiffies(msecs);
}


