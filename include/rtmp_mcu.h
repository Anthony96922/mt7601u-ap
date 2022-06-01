/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	rtmp_mcu.h

	Abstract:
	Miniport header file for mcu related information

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/

#ifndef __RTMP_MCU_H__
#define __RTMP_MCU_H__

enum MCU_TYPE {
	SWMCU,
	M8051,
	ANDES,
};


struct _RTMP_ADAPTER;

typedef void (*CMD_RSP_HANDLER)(struct _RTMP_ADAPTER *pAd, unsigned char *Data);

/*
 * CMD Unit (8051, Andes, ...,and etc)
 */
struct CMD_UNIT {
	union {
		struct {
			unsigned char Command;
			unsigned char Token;
			unsigned char Arg0;
			unsigned char Arg1;
		} MCU51;
		struct {
			unsigned char Type;
			unsigned short CmdPayloadLen;
			unsigned char * CmdPayload;
			unsigned short RspPayloadLen;
			unsigned char * RspPayload;
			unsigned long Timeout;
			bool NeedRsp;
			bool NeedWait;
			CMD_RSP_HANDLER CmdRspHdler;
		} ANDES;
	} u;
};


struct MCU_CTRL {
	unsigned char CmdSeq;
	NDIS_SPIN_LOCK CmdRspEventListLock;
	DL_LIST CmdRspEventList;
};


struct CMD_RSP_EVENT {
	DL_LIST List;
	unsigned char CmdSeq;	
	unsigned int Timeout;
	bool NeedWait;
	void *	AckDone;
	unsigned char **RspPayload;
	unsigned short *RspPayloadLen;
};

VOID ChipOpsMCUHook(struct _RTMP_ADAPTER *pAd, enum MCU_TYPE MCUType);
VOID MCUCtrlInit(struct _RTMP_ADAPTER *pAd);
VOID MCUCtrlExit(struct _RTMP_ADAPTER *pAd);

#endif 
