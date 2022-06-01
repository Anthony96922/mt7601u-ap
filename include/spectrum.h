
#ifndef __SPECTRUM_H__
#define __SPECTRUM_H__

#include "rtmp_type.h"
#include "spectrum_def.h"


unsigned char GetRegulatoryMaxTxPwr(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char channel);

CHAR RTMP_GetTxPwr(
	IN PRTMP_ADAPTER pAd,
	IN HTTRANSMIT_SETTING HTTxMode);

/*
	==========================================================================
	Description:
		Prepare Measurement request action frame and enqueue it into
		management queue waiting for transmition.
		
	Parametrs:
		1. the destination mac address of the frame.
	
	Return	: None.
	==========================================================================
 */
VOID MakeMeasurementReqFrame(
	IN PRTMP_ADAPTER pAd,
	OUT unsigned char * pOutBuffer,
	OUT unsigned long * pFrameLen,
	IN unsigned char TotalLen,
	IN unsigned char Category,
	IN unsigned char Action,
	IN unsigned char MeasureToken, 
	IN unsigned char MeasureReqMode,
	IN unsigned char MeasureReqType,
	IN unsigned short NumOfRepetitions);

/*
	==========================================================================
	Description:
		Prepare Measurement report action frame and enqueue it into
		management queue waiting for transmition.
		
	Parametrs:
		1. the destination mac address of the frame.
	
	Return	: None.
	==========================================================================
 */
VOID EnqueueMeasurementRep(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pDA,
	IN unsigned char DialogToken,
	IN unsigned char MeasureToken, 
	IN unsigned char MeasureReqMode,
	IN unsigned char MeasureReqType,
	IN unsigned char ReportInfoLen,
	IN unsigned char * pReportInfo);

/*
	==========================================================================
	Description:
		Prepare TPC Request action frame and enqueue it into
		management queue waiting for transmition.
		
	Parametrs:
		1. the destination mac address of the frame.
	
	Return	: None.
	==========================================================================
 */
VOID EnqueueTPCReq(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pDA,
	IN unsigned char DialogToken);

/*
	==========================================================================
	Description:
		Prepare TPC Report action frame and enqueue it into
		management queue waiting for transmition.
		
	Parametrs:
		1. the destination mac address of the frame.
	
	Return	: None.
	==========================================================================
 */
VOID EnqueueTPCRep(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pDA,
	IN unsigned char DialogToken,
	IN unsigned char TxPwr,
	IN unsigned char LinkMargin);

#ifdef WDS_SUPPORT
/*
	==========================================================================
	Description:
		Prepare Channel Switch Announcement action frame and enqueue it into
		management queue waiting for transmition.
		
	Parametrs:
		1. the destination mac address of the frame.
		2. Channel switch announcement mode.
		2. a New selected channel.
	
	Return	: None.
	==========================================================================
 */
VOID EnqueueChSwAnn(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pDA, 
	IN unsigned char ChSwMode,
	IN unsigned char NewCh);
#endif /* WDS_SUPPORT */

/*
	==========================================================================
	Description:
		Spectrun action frames Handler such as channel switch annoucement,
		measurement report, measurement request actions frames.
		
	Parametrs:
		Elme - MLME message containing the received frame
	
	Return	: None.
	==========================================================================
 */
VOID PeerSpectrumAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
INT Set_MeasureReq_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

INT Set_TpcReq_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);

INT Set_PwrConstraint(
	IN	PRTMP_ADAPTER	pAd, 
	IN	char *			arg);


NDIS_STATUS	MeasureReqTabInit(
	IN PRTMP_ADAPTER pAd);

VOID MeasureReqTabExit(
	IN PRTMP_ADAPTER pAd);

PMEASURE_REQ_ENTRY MeasureReqLookUp(
	IN PRTMP_ADAPTER	pAd,
	IN unsigned char			DialogToken);

PMEASURE_REQ_ENTRY MeasureReqInsert(
	IN PRTMP_ADAPTER	pAd,
	IN unsigned char			DialogToken);

VOID MeasureReqDelete(
	IN PRTMP_ADAPTER	pAd,
	IN unsigned char			DialogToken);

VOID InsertChannelRepIE(
	IN PRTMP_ADAPTER pAd,
	OUT unsigned char * pFrameBuf,
	OUT unsigned long * pFrameLen,
	IN char * pCountry,
	IN unsigned char RegulatoryClass);

VOID InsertTpcReportIE(
	IN PRTMP_ADAPTER pAd,
	OUT unsigned char * pFrameBuf,
	OUT unsigned long * pFrameLen,
	IN unsigned char TxPwr,
	IN unsigned char LinkMargin);

VOID InsertDialogToken(
	IN PRTMP_ADAPTER pAd,
	OUT unsigned char * pFrameBuf,
	OUT unsigned long * pFrameLen,
	IN unsigned char DialogToken);

NDIS_STATUS	TpcReqTabInit(
	IN PRTMP_ADAPTER pAd);

VOID TpcReqTabExit(
	IN PRTMP_ADAPTER pAd);

VOID NotifyChSwAnnToPeerAPs(
	IN PRTMP_ADAPTER pAd,
	IN unsigned char * pRA,
	IN unsigned char * pTA,
	IN unsigned char ChSwMode,
	IN unsigned char Channel);

VOID RguClass_BuildBcnChList(
	IN PRTMP_ADAPTER pAd,
	OUT unsigned char * pBuf,
	OUT	unsigned long * pBufLen);
#endif /* __SPECTRUM_H__ */

