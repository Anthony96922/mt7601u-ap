#ifndef __CS_H__
#define __CS_H__

#ifdef CARRIER_DETECTION_SUPPORT
#define CARRIER_DETECT_RECHECK_TIME			3
#define CARRIER_GONE_TRESHOLD				10 /* Radar tone count in 1 sec */
#define CARRIER_DETECT_THRESHOLD			0x0fffffff
#define CARRIER_DETECT_STOP_RATIO			2
#define CARRIER_DETECT_STOP_RECHECK_TIME		4
#define CARRIER_DETECT_DEFAULT_MASK			20
#define CARRIER_DETECT_DELTA				7
#define CARRIER_DETECT_DIV_FLAG				0
#define CD_INT_POLLING_CMD			0x67
#define CARRIER_DETECT_CRITIRIA				7000

typedef enum CD_STATE_n {
	CD_NORMAL,
	CD_SILENCE,
	CD_MAX_STATE
} CD_STATE;

typedef enum _TONE_RADAR_VERSION {
	DISABLE_TONE_RADAR = 0,
	TONE_RADAR_V1,
	TONE_RADAR_V2
} TONE_RADAR_VERSION;

typedef struct CARRIER_DETECTION_s {
	bool Enable;
	unsigned char CDSessionTime;
	unsigned char CDPeriod;
	CD_STATE CD_State;
	unsigned char delta;
	unsigned char SymRund;
	unsigned char div_flag;
	unsigned int threshold;
	unsigned char recheck;
	unsigned char recheck1;
	unsigned int TimeStamp; /*unit:16us*/
	unsigned int criteria;
	unsigned long idle_time;
	unsigned long busy_time;
	unsigned long Debug;
	unsigned long OneSecIntCount;
	unsigned char CarrierGoneThreshold;
	unsigned char VGA_Mask;
	unsigned char Packet_End_Mask;
	unsigned char Rx_PE_Mask;
} CARRIER_DETECTION_STRUCT, *PCARRIER_DETECTION_STRUCT;

#ifdef CARRIER_DETECTION_FIRMWARE_SUPPORT
/* Mcu command */
#define CD_ONOFF_MCU_CMD			0x65
#define CD_CHECKOUT_MCU_CMD			0x66
/* share memory offsets */
#define CD_CRITERIA     			0x4CB2
#define CD_CHECK_COUNT  			0x4CB9
#define RADAR_TONE_COUNT			0x4CBE
#define CD_RECHECK      			0x4CBF
#undef CARRIER_DETECT_RECHECK_TIME
#undef CARRIER_GONE_TRESHOLD
#undef CARRIER_DETECT_THRESHOLD
#define CARRIER_DETECT_RECHECK_TIME			5
#define CARRIER_GONE_TRESHOLD				35
#define CARRIER_DETECT_THRESHOLD			0x4fffffff

/* Parameters needed to decide the Carrier Detect State */
typedef struct _CARRIER_DETECT_PARAM {
		unsigned char	RadarToneCount; /* Number of radar tones in 100 msec*/
		unsigned char	ReCheck;
} CARRIER_DETECT_PARAM, *PCARRIER_DETECT_PARAM;

/* For debug print */
typedef struct _CARRIER_DETECT_DEBUG {
	 	unsigned char	delta_div;
		unsigned char	internalRadarToneCount;
		unsigned short	Criteria;
		unsigned int	Threshold;
		unsigned char	Count;
		unsigned char	CheckCount;
		unsigned char 	Reserved;
		unsigned char	VGA_Mask;
		unsigned char	PckMask;
		unsigned char	RxPeMask;
		unsigned char	RadarToneCount;
		unsigned char	ReCheck;
} CARRIER_DETECT_DEBUG, *PCARRIER_DETECT_DEBUG;
#endif /*CARRIER_DETECTION_FIRMWARE_SUPPORT*/

INT isCarrierDetectExist(
	IN PRTMP_ADAPTER pAd);

INT CarrierDetectReset(
	IN PRTMP_ADAPTER pAd);

extern VOID RtmpOsMsDelay(
	IN	unsigned long	msec);

INT Set_CarrierCriteria_Proc(
	IN PRTMP_ADAPTER 	pAd,
	IN char * 			arg);

INT Set_CarrierReCheck_Proc(
	IN PRTMP_ADAPTER	pAd,
	IN char * 			arg);

INT Set_CarrierGoneThreshold_Proc(
	IN PRTMP_ADAPTER	pAd,
	IN char *			arg);

INT Set_CarrierStopCheck_Proc(
	IN PRTMP_ADAPTER 	pAd,
	IN char * 			arg);

INT	Set_CarrierDebug_Proc(
	IN PRTMP_ADAPTER	pAd,
	IN	char * 		arg);

INT	Set_CarrierDelta_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

INT	Set_CarrierDivFlag_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

INT	Set_CarrierThrd_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

/* v2 functions */
INT	Set_CarrierSymRund_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

INT Set_CarrierMask_Proc(
    IN PRTMP_ADAPTER   pAd,
    IN char *         arg);

VOID CSInit(
	IN PRTMP_ADAPTER pAd);

VOID CarrierDetectionStart(
	IN PRTMP_ADAPTER pAd);

VOID RTMPHandleRadarInterrupt(
	IN PRTMP_ADAPTER  pAd);

VOID CarrierDetectionStop(
	IN PRTMP_ADAPTER	pAd);

VOID ToneRadarProgram_v1(
	IN PRTMP_ADAPTER pAd,
	IN unsigned long threshold);

VOID ToneRadarProgram_v2(
	IN PRTMP_ADAPTER pAd,
	IN unsigned long threshold);

#ifdef CARRIER_DETECTION_FIRMWARE_SUPPORT
VOID CarrierDetectionPeriodicStateCtrl(
	IN PRTMP_ADAPTER pAd);
#endif /* CARRIER_DETECTION_FIRMWARE_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
INT Set_CarrierDetect_Proc(
	IN PRTMP_ADAPTER pAd,
	IN char * arg);
#endif /*CONFIG_AP_SUPPORT*/
#endif /* CARRIER_DETECTION_SUPPORT */

#endif /*__CS_H__*/
