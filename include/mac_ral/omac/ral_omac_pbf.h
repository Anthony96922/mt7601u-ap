/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering	the source code	is stricitly prohibited, unless	the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	ral_omac_pbf.h

	Abstract:
	Ralink Wireless Chip MAC related definition & structures

	Revision History:
	Who			When		  What
	--------	----------	  ----------------------------------------------
*/

#ifndef __RAL_OMAC_PBF_H__
#define __RAL_OMAC_PBF_H__


#include "rtmp_type.h"

/* ================================================================================= */
/* Register format  for PBF                                                                                                                                                     */
/* ================================================================================= */


/* Most are for debug. Driver doesn't touch PBF register. */
#define PBF_SYS_CTRL 	 0x0400

#ifdef RT_BIG_ENDIAN
typedef union _PBF_SYS_CTRL_STRUC {
	struct {
		unsigned int Reserved5:12; /* Reserved */
		unsigned int SHR_MSEL:1; /* Shared memory access selection */
		unsigned int PBF_MSEL:2; /* Packet buffer memory access selection */
		unsigned int HST_PM_SEL:1; /* The write selection of the host program RAM */
		unsigned int Reserved4:1; /* Reserved */
		unsigned int CAP_MODE:1; /* Packet buffer capture mode */
		unsigned int Reserved3:1; /* Reserved */
		unsigned int CLK_SEL:1; /* MAC/PBF clock source selection */
		unsigned int PBF_CLK_EN:1; /* PBF clock enable */
		unsigned int MAC_CLK_EN:1; /* MAC clock enable */
		unsigned int DMA_CLK_EN:1; /* DMA clock enable */
		unsigned int Reserved2:1; /* Reserved */
		unsigned int MCU_READY:1; /* MCU ready */
		unsigned int Reserved1:2; /* Reserved */
		unsigned int ASY_RESET:1; /* ASYNC interface reset */
		unsigned int PBF_RESET:1; /* PBF hardware reset */
		unsigned int MAC_RESET:1; /* MAC hardware reset */
		unsigned int DMA_RESET:1; /* DMA hardware reset */
		unsigned int MCU_RESET:1; /* MCU hardware reset */
	} field;
	unsigned int word;
} PBF_SYS_CTRL_STRUC;
#else
typedef union _PBF_SYS_CTRL_STRUC {
	struct {
		unsigned int MCU_RESET:1;
		unsigned int DMA_RESET:1;
		unsigned int MAC_RESET:1;
		unsigned int PBF_RESET:1;
		unsigned int ASY_RESET:1;
		unsigned int Reserved1:2;
		unsigned int MCU_READY:1;
		unsigned int Reserved2:1;
		unsigned int DMA_CLK_EN:1;
		unsigned int MAC_CLK_EN:1;
		unsigned int PBF_CLK_EN:1;
		unsigned int CLK_SEL:1;
		unsigned int Reserved3:1;
		unsigned int CAP_MODE:1;
		unsigned int Reserved4:1;
		unsigned int HST_PM_SEL:1;
		unsigned int PBF_MSEL:2;
		unsigned int SHR_MSEL:1;
		unsigned int Reserved5:12;
	}field;
	unsigned int word;
} PBF_SYS_CTRL_STRUC;
#endif


#define PBF_CFG			0x0408
#define PBF_MAX_PCNT	0x040C
#define PBF_CAP_CTRL	0x0440

#define BCN_OFFSET0		0x042C
#define BCN_OFFSET1		0x0430

#endif /* __RAL_OMAC_PBF_H__ */

