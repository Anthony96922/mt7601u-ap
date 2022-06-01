#ifdef VIDEO_TURBINE_SUPPORT
extern AP_VIDEO_STRUCT GLOBAL_AP_VIDEO_CONFIG;

void VideoModeUpdate(IN PRTMP_ADAPTER pAd);
void VideoModeDynamicTune(IN PRTMP_ADAPTER pAd);
unsigned int GetAsicDefaultRetry(IN PRTMP_ADAPTER pAd);
unsigned char GetAsicDefaultTxBA(IN PRTMP_ADAPTER pAd);
unsigned int GetAsicVideoRetry(IN PRTMP_ADAPTER pAd);
unsigned char GetAsicVideoTxBA(IN PRTMP_ADAPTER pAd);
void VideoConfigInit(IN PRTMP_ADAPTER pAd);
#endif /* VIDEO_TURBINE_SUPPORT */

