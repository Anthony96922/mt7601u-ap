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
	spectrum_def.h
 
    Abstract:
    Handle association related requests either from WSTA or from local MLME
 
    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
	Fonchi Wu    2008	  	   created for 802.11h
 */

#ifndef __SPECTRUM_DEF_H__
#define __SPECTRUM_DEF_H__


#define MAX_MEASURE_REQ_TAB_SIZE		32
/* Size of hash tab must be power of 2. */
#define MAX_HASH_MEASURE_REQ_TAB_SIZE	MAX_MEASURE_REQ_TAB_SIZE

#define MAX_TPC_REQ_TAB_SIZE			32
/* Size of hash tab must be power of 2. */
#define MAX_HASH_TPC_REQ_TAB_SIZE		MAX_TPC_REQ_TAB_SIZE

#define MIN_RCV_PWR				100		/* Negative value ((dBm) */

#define TPC_REQ_AGE_OUT			500		/* ms */
#define MQ_REQ_AGE_OUT			500		/* ms */

#define TPC_DIALOGTOKEN_HASH_INDEX(_DialogToken)	((_DialogToken) & (MAX_HASH_TPC_REQ_TAB_SIZE - 1))
#define MQ_DIALOGTOKEN_HASH_INDEX(_DialogToken)		((_DialogToken) & (MAX_MEASURE_REQ_TAB_SIZE - 1))

typedef struct _MEASURE_REQ_ENTRY
{
	struct _MEASURE_REQ_ENTRY *pNext;
	unsigned long lastTime;
	bool	Valid;
	unsigned char DialogToken;
	unsigned char MeasureDialogToken[3];	/* 0:basic measure, 1: CCA measure, 2: RPI_Histogram measure. */
} MEASURE_REQ_ENTRY, *PMEASURE_REQ_ENTRY;

typedef struct _MEASURE_REQ_TAB
{
	unsigned char Size;
	PMEASURE_REQ_ENTRY Hash[MAX_HASH_MEASURE_REQ_TAB_SIZE];
	MEASURE_REQ_ENTRY Content[MAX_MEASURE_REQ_TAB_SIZE];
} MEASURE_REQ_TAB, *PMEASURE_REQ_TAB;

typedef struct _TPC_REQ_ENTRY
{
	struct _TPC_REQ_ENTRY *pNext;
	unsigned long lastTime;
	bool Valid;
	unsigned char DialogToken;
} TPC_REQ_ENTRY, *PTPC_REQ_ENTRY;

typedef struct _TPC_REQ_TAB
{
	unsigned char Size;
	PTPC_REQ_ENTRY Hash[MAX_HASH_TPC_REQ_TAB_SIZE];
	TPC_REQ_ENTRY Content[MAX_TPC_REQ_TAB_SIZE];
} TPC_REQ_TAB, *PTPC_REQ_TAB;


/* The regulatory information */
typedef struct _DOT11_CHANNEL_SET
{
	unsigned char NumberOfChannels;
	unsigned char MaxTxPwr;
	unsigned char ChannelList[16];
} DOT11_CHANNEL_SET, *PDOT11_CHANNEL_SET;

typedef struct _DOT11_REGULATORY_INFORMATION
{
	unsigned char RegulatoryClass;
	DOT11_CHANNEL_SET ChannelSet;
} DOT11_REGULATORY_INFORMATION, *PDOT11_REGULATORY_INFORMATION;



#define RM_TPC_REQ				0
#define RM_MEASURE_REQ			1

#define RM_BASIC				0
#define RM_CCA					1
#define RM_RPI_HISTOGRAM		2
#define RM_CH_LOAD				3
#define RM_NOISE_HISTOGRAM		4


typedef struct GNU_PACKED _TPC_REPORT_INFO
{
	unsigned char TxPwr;
	unsigned char LinkMargin;
} TPC_REPORT_INFO, *PTPC_REPORT_INFO;

typedef struct GNU_PACKED _CH_SW_ANN_INFO
{
	unsigned char ChSwMode;
	unsigned char Channel;
	unsigned char ChSwCnt;
} CH_SW_ANN_INFO, *PCH_SW_ANN_INFO;

typedef union GNU_PACKED _MEASURE_REQ_MODE
{
#ifdef RT_BIG_ENDIAN
	struct GNU_PACKED
	{

		unsigned char :3;
		unsigned char DurationMandatory:1;
		unsigned char Report:1;
		unsigned char Request:1;
		unsigned char Enable:1;
		unsigned char Parallel:1;
	} field;
#else
	struct GNU_PACKED
	{
		unsigned char Parallel:1;
		unsigned char Enable:1;
		unsigned char Request:1;
		unsigned char Report:1;
		unsigned char DurationMandatory:1;
		unsigned char :3;
	} field;
#endif /* RT_BIG_ENDIAN */
	unsigned char word;
} MEASURE_REQ_MODE, *PMEASURE_REQ_MODE;

typedef struct GNU_PACKED _MEASURE_REQ
{
	unsigned char ChNum;
	unsigned long long MeasureStartTime;
	unsigned short MeasureDuration;
} MEASURE_REQ, *PMEASURE_REQ;

typedef struct GNU_PACKED _MEASURE_REQ_INFO
{
	unsigned char Token;
	MEASURE_REQ_MODE ReqMode;
	unsigned char ReqType;
	unsigned char Oct[0];
} MEASURE_REQ_INFO, *PMEASURE_REQ_INFO;

typedef union GNU_PACKED _MEASURE_BASIC_REPORT_MAP
{
#ifdef RT_BIG_ENDIAN
	struct GNU_PACKED
	{
		unsigned char Rev:3;

		unsigned char Unmeasure:1;
		unsigned char Radar:1;
		unsigned char UnidentifiedSignal:1;
		unsigned char OfdmPreamble:1;
		unsigned char BSS:1;
	} field;
#else
	struct GNU_PACKED
	{
		unsigned char BSS:1;

		unsigned char OfdmPreamble:1;
		unsigned char UnidentifiedSignal:1;
		unsigned char Radar:1;
		unsigned char Unmeasure:1;
		unsigned char Rev:3;
	} field;
#endif /* RT_BIG_ENDIAN */
	unsigned char word;
} MEASURE_BASIC_REPORT_MAP, *PMEASURE_BASIC_REPORT_MAP;

typedef struct GNU_PACKED _MEASURE_BASIC_REPORT
{
	unsigned char ChNum;
	unsigned long long MeasureStartTime;
	unsigned short MeasureDuration;
	MEASURE_BASIC_REPORT_MAP Map;
} MEASURE_BASIC_REPORT, *PMEASURE_BASIC_REPORT;

typedef struct GNU_PACKED _MEASURE_CCA_REPORT
{
	unsigned char ChNum;
	unsigned long long MeasureStartTime;
	unsigned short MeasureDuration;
	unsigned char CCA_Busy_Fraction;
} MEASURE_CCA_REPORT, *PMEASURE_CCA_REPORT;

typedef struct GNU_PACKED _MEASURE_RPI_REPORT
{
	unsigned char ChNum;
	unsigned long long MeasureStartTime;
	unsigned short MeasureDuration;
	unsigned char RPI_Density[8];
} MEASURE_RPI_REPORT, *PMEASURE_RPI_REPORT;

typedef union GNU_PACKED _MEASURE_REPORT_MODE
{
	struct GNU_PACKED
	{
#ifdef RT_BIG_ENDIAN
		unsigned char Rev:5;
		unsigned char Refused:1;
		unsigned char Incapable:1;
		unsigned char Late:1;
#else
		unsigned char Late:1;
		unsigned char Incapable:1;
		unsigned char Refused:1;
		unsigned char Rev:5;
#endif /* RT_BIG_ENDIAN */
	} field;
	unsigned char word;
} MEASURE_REPORT_MODE, *PMEASURE_REPORT_MODE;

typedef struct GNU_PACKED _MEASURE_REPORT_INFO
{
	unsigned char Token;
	unsigned char ReportMode;
	unsigned char ReportType;
	unsigned char Octect[0];
} MEASURE_REPORT_INFO, *PMEASURE_REPORT_INFO;

typedef struct GNU_PACKED _QUIET_INFO
{
	unsigned char QuietCnt;
	unsigned char QuietPeriod;
	unsigned short QuietDuration;
	unsigned short QuietOffset;
} QUIET_INFO, *PQUIET_INFO;

#endif /* __SPECTRUM_DEF_H__ */

