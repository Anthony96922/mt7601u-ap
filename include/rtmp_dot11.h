/*

*/

#ifndef __DOT11_BASE_H__
#define __DOT11_BASE_H__

#include "rtmp_type.h"

#ifdef DOT11_VHT_AC
#include "dot11ac_vht.h"
#endif /* DOT11_VHT_AC */

#ifdef TXBF_SUPPORT
/* CSI/Steering values */
#define DOT11N_BF_FB_NONE		0
#define DOT11N_BF_FB_CSI		1
#define DOT11N_BF_FB_NOCOMP	2
#define DOT11N_BF_FB_COMP		3
#endif /* TXBF_SUPPORT */

/* 4-byte HTC field.  maybe included in any frame except non-QOS data frame.  The Order bit must set 1. */
typedef struct GNU_PACKED _HT_CONTROL{
#ifdef RT_BIG_ENDIAN
	unsigned int RDG:1;
	unsigned int ACConstraint:1;
	unsigned int rsv2:5;
	unsigned int NDPAnnounce:1;
	unsigned int CSISTEERING:2;
	unsigned int rsv1:2;
	unsigned int CalSeq:2;
	unsigned int CalPos:2;
	unsigned int MFBorASC:7;
	unsigned int MFSI:3;
	unsigned int MSI:3;
	unsigned int MRQ:1;
	unsigned int TRQ:1;
	unsigned int vht:1;
#else
	unsigned int vht:1;		/* indicate for VHT variant or HT variant */
	unsigned int TRQ:1;		/*sounding request */
	unsigned int MRQ:1;		/*MCS feedback. Request for a MCS feedback */
	unsigned int MSI:3;		/*MCS Request, MRQ Sequence identifier */
	unsigned int MFSI:3;		/*SET to the received value of MRS. 0x111 for unsolicited MFB. */
	unsigned int MFBorASC:7;	/*Link adaptation feedback containing recommended MCS. 0x7f for no feedback or not available */
	unsigned int CalPos:2;	/* calibration position */
	unsigned int CalSeq:2;	/*calibration sequence */
	unsigned int rsv1:2;		/* Reserved */
	unsigned int CSISTEERING:2;	/*CSI/ STEERING */
	unsigned int NDPAnnounce:1;	/* ZLF announcement */
	unsigned int rsv2:5;		/*calibration sequence */
	unsigned int ACConstraint:1;	/*feedback request */
	unsigned int RDG:1;		/*RDG / More PPDU */
#endif				/* !RT_BIG_ENDIAN */
} HT_CONTROL, *PHT_CONTROL;

/* 2-byte QOS CONTROL field */
typedef struct GNU_PACKED _QOS_CONTROL{
#ifdef RT_BIG_ENDIAN
	unsigned short Txop_QueueSize:8;
	unsigned short AMsduPresent:1;
	unsigned short AckPolicy:2;	/*0: normal ACK 1:No ACK 2:scheduled under MTBA/PSMP  3: BA */
	unsigned short EOSP:1;
	unsigned short TID:4;
#else
	unsigned short TID:4;
	unsigned short EOSP:1;
	unsigned short AckPolicy:2;	/*0: normal ACK 1:No ACK 2:scheduled under MTBA/PSMP  3: BA */
	unsigned short AMsduPresent:1;
	unsigned short Txop_QueueSize:8;
#endif				/* !RT_BIG_ENDIAN */
} QOS_CONTROL, *PQOS_CONTROL;


typedef struct GNU_PACKED _AC_PARAM_RECORD{
	unsigned char aci_aifsn;
	unsigned char ecw_max:4;
	unsigned char ecw_min: 4;
	unsigned short txop_limit;
}AC_PARAM_RECORD;


typedef struct GNU_PACKED _PSPOLL_FRAME {
	FRAME_CONTROL FC;
	unsigned short Aid;
	unsigned char Bssid[MAC_ADDR_LEN];
	unsigned char Ta[MAC_ADDR_LEN];
} PSPOLL_FRAME, *PPSPOLL_FRAME;


typedef struct GNU_PACKED _RTS_FRAME {
	FRAME_CONTROL FC;
	unsigned short Duration;
	unsigned char Addr1[MAC_ADDR_LEN];
	unsigned char Addr2[MAC_ADDR_LEN];
} RTS_FRAME, *PRTS_FRAME;

#endif /* __DOT11_BASE_H__ */
