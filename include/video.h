#ifdef VIDEO_TURBINE_SUPPORT
extern AP_VIDEO_STRUCT GLOBAL_AP_VIDEO_CONFIG;

VOID VideoModeUpdate(IN PRTMP_ADAPTER pAd);
VOID VideoModeDynamicTune(IN PRTMP_ADAPTER pAd);
unsigned int GetAsicDefaultRetry(IN PRTMP_ADAPTER pAd);
unsigned char GetAsicDefaultTxBA(IN PRTMP_ADAPTER pAd);
unsigned int GetAsicVideoRetry(IN PRTMP_ADAPTER pAd);
unsigned char GetAsicVideoTxBA(IN PRTMP_ADAPTER pAd);
VOID VideoConfigInit(IN PRTMP_ADAPTER pAd);
#endif /* VIDEO_TURBINE_SUPPORT */

