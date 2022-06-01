/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2005, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attempt
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	wapi_def.h

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------	
*/

#ifndef __WAPI_DEF_H__
#define __WAPI_DEF_H__

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#ifndef INOUT
#define INOUT
#endif
#ifndef MAC_ADDR_LEN
#define MAC_ADDR_LEN   6
#endif 
#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif

#define WAPI_IFNAMSIZ		16
#define MAX_WAPI_MBSSID_NUM 8
#define MAX_ID_NO 10

#define LENGTH_WAI_H	12
#define LEN_WAPI_TSC	16
#define LEN_WPI_MIC		16
#define LEN_WPI_IV_HDR	18
#define LEN_WAPI_NMK	16
#define LEN_WAPI_GTK	32

/* trigger message from driver */
#define WAI_MLME_CERT_AUTH_START	1
#define WAI_MLME_KEY_HS_START		2
#define WAI_MLME_UPDATE_BK			3
#define WAI_MLME_UPDATE_USK			4
#define WAI_MLME_UPDATE_MSK			5
#define WAI_MLME_DISCONNECT			0xff

#define WAPI_KEY_UPDATE_EXEC_INTV   1000				/* 1 sec */

/* WAPI rekey method */
#define REKEY_METHOD_DISABLE        0
#define REKEY_METHOD_TIME           1
#define REKEY_METHOD_PKT            2
/*#define REKEY_METHOD_TIME_PKT     3 */

#define STATUS_WAPI_KEY_INVALID		1
#define STATUS_WAPI_IV_MISMATCH		2
#define STATUS_WAPI_MIC_DIFF		3

extern unsigned char AE_BCAST_PN[LEN_WAPI_TSC];
extern unsigned char ASUE_UCAST_PN[LEN_WAPI_TSC];
extern unsigned char AE_UCAST_PN[LEN_WAPI_TSC];

/* WAPI authentication mode */
typedef enum _WAPI_AUTH_MODE
{
   WAPI_AUTH_DISABLE,
   WAPI_AUTH_PSK,
   WAPI_AUTH_CERT,
} WAPI_AUTH_MODE, *PWAPI_AUTH_MODE;

/* WAPI authentication mode */
typedef enum _KEY_TYPE_MODE
{
   HEX_MODE,
   ASCII_MODE
} KEY_TYPE_MODE, *PKEY_TYPE_MODE;

/* the defintion of WAI header */
typedef	struct GNU_PACKED _HEADER_WAI	{    
    unsigned short          version;
	unsigned char			type;
	unsigned char			sub_type;
	unsigned short			reserved;
	unsigned short			length;
	unsigned short			pkt_seq;
	unsigned char			frag_seq;
	unsigned char			flag;
}	HEADER_WAI, *PHEADER_WAI;

/* For WAPI */
typedef struct GNU_PACKED _WAPIIE {
    unsigned short  version;    
    unsigned short  acount;
    struct GNU_PACKED {
        unsigned char oui[4];
    }auth[1];
} WAPIIE, *PWAPIIE;

/* unicast key suite */
typedef struct GNU_PACKED _WAPIIE_UCAST {
    unsigned short ucount;
    struct GNU_PACKED {
        unsigned char oui[4];
    }ucast[1];
} WAPIIE_UCAST,*PWAPIIE_UCAST;

/* multi-cast key suite and capability */
typedef struct GNU_PACKED _WAPIIE_MCAST {
    unsigned char   mcast[4];
    unsigned short  capability;
} WAPIIE_MCAST,*PWAPIIE_MCAST;

/* the relative to wapi daemon */
typedef struct GNU_PACKED _COMMON_WAPI_INFO
{	
	unsigned char			wapi_ifname[WAPI_IFNAMSIZ];		/* wai negotiation */
	unsigned char			wapi_ifname_len;			
	unsigned char 			preauth_ifname[WAPI_IFNAMSIZ];	/* pre-authentication */
	unsigned char			preauth_ifname_len;
	unsigned char			as_cert_no;
	unsigned char			as_cert_path[MAX_ID_NO][128];			/* the path of as certification */
	unsigned char			as_cert_path_len[MAX_ID_NO];
	unsigned char			ca_cert_path[128];			/* the path of ca certification */
	unsigned char			ca_cert_path_len;
	unsigned char			user_cert_path[128];		/* the path of local user certification */
	unsigned char			user_cert_path_len;		
	unsigned int			wapi_as_ip;					/* the ip address of authentication server */
	unsigned int			wapi_as_port;				/* the port of authentication server */
} COMMON_WAPI_INFO, *PCOMMON_WAPI_INFO;

typedef struct GNU_PACKED _MBSS_WAPI_INFO
{	
	unsigned char			ifname[WAPI_IFNAMSIZ];
	unsigned char			ifname_len;
	unsigned char			auth_mode;	
    unsigned char       	psk[64];
	unsigned char			psk_len;	
	unsigned char			wie[128];
	unsigned char			wie_len;
} MBSS_WAPI_INFO, *PMBSS_WAPI_INFO;

/* It's used by wapi daemon to require relative configuration */
typedef struct GNU_PACKED _WAPI_CONF
{
    unsigned char				mbss_num;					/* indicate multiple BSS number */
	COMMON_WAPI_INFO	comm_wapi_info;		
	MBSS_WAPI_INFO		mbss_wapi_info[MAX_WAPI_MBSSID_NUM];
} WAPI_CONF, *PWAPI_CONF;

#ifdef LINUX
#define WapiMoveMemory(Destination, Source, Length) memmove(Destination, Source, Length)
#define WapiZeroMemory(Destination, Length)         memset(Destination, 0, Length)
#define WapiEqualMemory(Source1, Source2, Length)   (!memcmp(Source1, Source2, Length))
#endif /* LINUX */

#endif /* __WAPI_DEF_H__ */

