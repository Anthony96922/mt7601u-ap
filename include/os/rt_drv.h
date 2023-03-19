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
	rt_drv.h

    Abstract:
	Put all virtual OS related definition/structure/MACRO here except
	standard ANSI C function.

	Note:
	No any OS related definition/MACRO is defined here.

    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
*/

/* same as rt_linux.h to warn users the two files can not be used simultaneously */
#ifndef __RT_LINUX_H__
#define __RT_LINUX_H__

#include "os/rt_linux_cmm.h"
#include <linux/string.h>
#include <linux/ctype.h>

#undef AP_WSC_INCLUDED
#undef STA_WSC_INCLUDED
#undef WSC_INCLUDED

#ifdef CONFIG_AP_SUPPORT
#ifdef WSC_AP_SUPPORT
#define AP_WSC_INCLUDED
#endif /* WSC_AP_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */


#if defined(WSC_AP_SUPPORT) || defined(WSC_STA_SUPPORT)
#define WSC_INCLUDED
#endif

/*#ifdef RTMP_USB_SUPPORT */
typedef VOID	*PUSB_DEV;
typedef VOID	*purbb_t;
typedef VOID	pregs;
/*typedef struct usb_ctrlrequest devctrlrequest; */
/*#endif */

/***********************************************************************************
 *	Profile related sections
 ***********************************************************************************/
#ifdef CONFIG_AP_SUPPORT
#ifdef RTMP_MAC_USB
#ifdef INF_AMAZON_SE
#define AP_PROFILE_PATH			"/ramdisk/etc/wifi/RT2870AP/RT2870AP.txt"
#define AP_RTMP_FIRMWARE_FILE_NAME	"/ramdisk/etc/wifi/RT2870AP/RT2870AP.bin"
#else
#define AP_PROFILE_PATH			"/etc/wifi/RT2870AP/RT2870AP.txt"
#define AP_RTMP_FIRMWARE_FILE_NAME	"/etc/wifi/RT2870AP/RT2870AP.bin"
#endif
#define AP_NIC_DEVICE_NAME		"RT2870AP"
#define AP_DRIVER_VERSION		"3.0.0.1"
#ifdef MULTIPLE_CARD_SUPPORT
#define CARD_INFO_PATH			"/etc/wifi/RT2870AP/RT2870APCard.txt"
#endif /* MULTIPLE_CARD_SUPPORT */
#endif /* RTMP_MAC_USB */
#endif /* CONFIG_AP_SUPPORT */

#ifdef SINGLE_SKU_V2
#define SINGLE_SKU_TABLE_FILE_NAME	"/etc/wifi/RT2870STA/SingleSKU.txt"
#endif /* SINGLE_SKU_V2 */

/***********************************************************************************
 *	Compiler related definitions
 ***********************************************************************************/
#undef __inline
#define __inline		static inline
#define IN
#define OUT
#define INOUT
#define NDIS_STATUS		INT


/***********************************************************************************
 *	OS Specific definitions and data structures
 ***********************************************************************************/
typedef void			* PPCI_DEV;
typedef void			* PNET_DEV;
typedef void			* PNDIS_PACKET;
typedef char			NDIS_PACKET;
typedef PNDIS_PACKET		* PPNDIS_PACKET;
typedef ra_dma_addr_t		NDIS_PHYSICAL_ADDRESS;
typedef ra_dma_addr_t		* PNDIS_PHYSICAL_ADDRESS;
typedef void			* NDIS_HANDLE;
typedef char			* PNDIS_BUFFER;

#undef KERN_ERR
#define KERN_ERR


/***********************************************************************************
 *	Network related constant definitions
 ***********************************************************************************/
#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

#define ETH_LENGTH_OF_ADDRESS	6

#define NDIS_STATUS_SUCCESS                     0x00
#define NDIS_STATUS_FAILURE                     0x01
#define NDIS_STATUS_INVALID_DATA		0x02
#define NDIS_STATUS_RESOURCES                   0x03

#define NDIS_SET_PACKET_STATUS(_p, _status)		do {} while(0)
#define NdisWriteErrorLogEntry(_a, _b, _c, _d)		do {} while(0)

/* statistics counter */
#define STATS_INC_RX_PACKETS(_pAd, _dev)
#define STATS_INC_TX_PACKETS(_pAd, _dev)

#define STATS_INC_RX_BYTESS(_pAd, _dev, len)
#define STATS_INC_TX_BYTESS(_pAd, _dev, len)

#define STATS_INC_RX_ERRORS(_pAd, _dev)
#define STATS_INC_TX_ERRORS(_pAd, _dev)

#define STATS_INC_RX_DROPPED(_pAd, _dev)
#define STATS_INC_TX_DROPPED(_pAd, _dev)


/***********************************************************************************
 *	Ralink Specific network related constant definitions
 ***********************************************************************************/
#define MIN_NET_DEVICE_FOR_MBSSID		0x00		/*0x00,0x10,0x20,0x30 */
#define MIN_NET_DEVICE_FOR_WDS			0x10		/*0x40,0x50,0x60,0x70 */
#define MIN_NET_DEVICE_FOR_APCLI		0x20
#define MIN_NET_DEVICE_FOR_MESH			0x30

#define NET_DEVICE_REAL_IDX_MASK		0x0f		/* for each operation mode, we maximum support 15 entities. */


/***********************************************************************************
 *	OS signaling related constant definitions
 ***********************************************************************************/

/***********************************************************************************
 *	OS file operation related data structure definitions
 ***********************************************************************************/
#define IS_FILE_OPEN_ERR(_fd)	RtmpOsFileIsErr((_fd))

#ifndef O_RDONLY
#define O_RDONLY			RTMP_FILE_RDONLY
#endif /* O_RDONLY */

#ifndef O_WRONLY
#define O_WRONLY			RTMP_FILE_WRONLY
#endif /* O_WRONLY */

#ifndef O_CREAT
#define O_CREAT				RTMP_FILE_CREAT
#endif /* O_CREAT */

#ifndef O_TRUNC
#define O_TRUNC				RTMP_FILE_TRUNC
#endif /* O_TRUNC */


/***********************************************************************************
 *	OS semaphore related data structure and definitions
 ***********************************************************************************/
#define RTCMDUp				RtmpOsCmdUp


/***********************************************************************************
 *	OS Memory Access related data structure and definitions
 ***********************************************************************************/
#define NdisMoveMemory(Destination, Source, Length) memmove(Destination, Source, Length)
#define NdisCopyMemory(Destination, Source, Length) memcpy(Destination, Source, Length)
#define NdisZeroMemory(Destination, Length)         memset(Destination, 0, Length)
#define NdisFillMemory(Destination, Length, Fill)   memset(Destination, Fill, Length)
#define NdisCmpMemory(Destination, Source, Length)  memcmp(Destination, Source, Length)
#define NdisEqualMemory(Source1, Source2, Length)   (!memcmp(Source1, Source2, Length))
#define RTMPEqualMemory(Source1, Source2, Length)	(!memcmp(Source1, Source2, Length))

#define MlmeAllocateMemory(_pAd, _ppVA)			os_alloc_mem(_pAd, _ppVA, MGMT_DMA_BUFFER_SIZE)
#define MlmeFreeMemory(_pAd, _pVA)			os_free_mem(_pAd, _pVA)

#define COPY_MAC_ADDR(Addr1, Addr2)             	memcpy((Addr1), (Addr2), MAC_ADDR_LEN)


/***********************************************************************************
 *	OS task related data structure and definitions
 ***********************************************************************************/
#define RTMP_OS_PID				unsigned long /* value or pointer */

#define RTMP_GET_OS_PID(_a, _b)			RtmpOsGetPid(&_a, _b);
#define CHECK_TASK_LEGALITY(_task)		RtmpOsCheckTaskLegality(_task)
#define ATE_KILL_THREAD_PID			RtmpThreadPidKill

typedef INT (*RTMP_OS_TASK_CALLBACK)(unsigned long);


/***********************************************************************************
 * IOCTL related definitions and data structures.
 **********************************************************************************/
#define NET_IOCTL				VOID
#define PNET_IOCTL				VOID *

/* undef them to avoid compile errors in rt_symb.c */
#undef EINVAL
#undef EOPNOTSUPP
#undef EFAULT
#undef ENETDOWN
#undef E2BIG
#undef ENOMEM
#undef EAGAIN
#undef ENOTCONN

#define EINVAL					(-RTMP_IO_EINVAL)
#define EOPNOTSUPP      			(-RTMP_IO_EOPNOTSUPP)
#define EFAULT					(-RTMP_IO_EFAULT)
#define ENETDOWN				(-RTMP_IO_ENETDOWN)
#define E2BIG					(-RTMP_IO_E2BIG)
#define ENOMEM					(-RTMP_IO_ENOMEM)
#define EAGAIN					(-RTMP_IO_EAGAIN)
#define ENOTCONN				(-RTMP_IO_ENOTCONN)


/***********************************************************************************
 * Timer related definitions and data structures.
 **********************************************************************************/
#define OS_HZ					RtmpOsTickUnitGet()

typedef void (*TIMER_FUNCTION)(unsigned long);

#define OS_WAIT					RtmpOsWait

#define RTMP_TIME_AFTER			RtmpOsTimerAfter
#define RTMP_TIME_BEFORE		RtmpOsTimerBefore

#define ONE_TICK				1

#define NdisGetSystemUpTime		RtmpOsGetSystemUpTime


/***********************************************************************************
 *	OS specific cookie data structure binding to RTMP_ADAPTER
 ***********************************************************************************/

/* do not have compile option in the structure for UTIL module */
struct os_cookie {

#ifdef RTMP_MAC_USB
	VOID					*pUsb_Dev;

#endif /* RTMP_MAC_USB */

#ifdef WORKQUEUE_BH
	unsigned int		     		pAd_va;
#endif /* WORKQUEUE_BH */

	RTMP_NET_TASK_STRUCT	rx_done_task;
	RTMP_NET_TASK_STRUCT	cmd_rsp_event_task;
	RTMP_NET_TASK_STRUCT	mgmt_dma_done_task;
	RTMP_NET_TASK_STRUCT	ac0_dma_done_task;
#ifdef RALINK_ATE
	RTMP_NET_TASK_STRUCT	ate_ac0_dma_done_task;
#endif /* RALINK_ATE */
	RTMP_NET_TASK_STRUCT	ac1_dma_done_task;
	RTMP_NET_TASK_STRUCT	ac2_dma_done_task;
	RTMP_NET_TASK_STRUCT	ac3_dma_done_task;
	RTMP_NET_TASK_STRUCT	hcca_dma_done_task;
	RTMP_NET_TASK_STRUCT	tbtt_task;


#ifdef UAPSD_SUPPORT
	RTMP_NET_TASK_STRUCT	uapsd_eosp_sent_task;
#endif /* UAPSD_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
#ifdef DFS_SUPPORT
#ifdef DFS_SOFTWARE_SUPPORT
	RTMP_NET_TASK_STRUCT	pulse_radar_detect_task;
	RTMP_NET_TASK_STRUCT	width_radar_detect_task;
#endif /* DFS_SOFTWARE_SUPPORT */
#endif /* DFS_SUPPORT */

#ifdef CARRIER_DETECTION_SUPPORT
	RTMP_NET_TASK_STRUCT	carrier_sense_task;
#endif /* CARRIER_DETECTION_SUPPORT */

#ifdef DFS_SUPPORT
	RTMP_NET_TASK_STRUCT	dfs_task;
#endif /* DFS_SUPPORT */

#endif /* CONFIG_AP_SUPPORT */

#ifdef RTMP_MAC_USB
	RTMP_NET_TASK_STRUCT	null_frame_complete_task;
	RTMP_NET_TASK_STRUCT	pspoll_frame_complete_task;
#endif /* RTMP_MAC_USB */

#ifdef CONFIG_MULTI_CHANNEL
	RTMP_NET_TASK_STRUCT	hcca_null_frame_complete_task;
#endif /* CONFIG_MULTI_CHANNEL */

	RTMP_OS_PID		apd_pid; /*802.1x daemon pid */
	unsigned long		apd_pid_nr;
#ifdef CONFIG_AP_SUPPORT
#ifdef IAPP_SUPPORT
/*	RT_SIGNAL_STRUC		RTSignal; */
	RTMP_OS_PID		IappPid; /*IAPP daemon pid */
	unsigned long		IappPid_nr;
#endif /* IAPP_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */
#ifdef WAPI_SUPPORT
	RTMP_OS_PID		wapi_pid; /*wapi daemon pid */
	unsigned long		wapi_pid_nr;
#endif /* WAPI_SUPPORT */
	INT			ioctl_if_type;
	INT 			ioctl_if;
};

typedef struct os_cookie	* POS_COOKIE;


/***********************************************************************************
 *	OS debugging and printing related definitions and data structure
 ***********************************************************************************/
#define PRINT_MAC(addr)	\
	addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]

#ifdef DBG
extern unsigned long		RTDebugLevel;
extern unsigned long		RTDebugFunc;

#define DBGPRINT_RAW(Level, Fmt)	\
do {					\
	unsigned long __gLevel = (Level) & 0xff;\
	unsigned long __fLevel = ((Level)>>8) & 0xffffff;\
	if (__gLevel <= RTDebugLevel)	\
	{                               \
		if ((RTDebugFunc == 0) || \
			((RTDebugFunc != 0) && (((__fLevel & RTDebugFunc)!= 0) || (__gLevel <= RT_DEBUG_ERROR)))) { \
			printk(RTMP_DRV_NAME ": "); \
			printk Fmt;	\
		}			\
	}				\
} while(0)

#define DBGPRINT(Level, Fmt)    DBGPRINT_RAW(Level, Fmt)


#define DBGPRINT_ERR(Fmt)		\
{					\
	printk(RTMP_DRV_NAME ": error: ");		\
	printk Fmt;			\
}

#define DBGPRINT_S(Status, Fmt)		\
{					\
	printk(RTMP_DRV_NAME ": ");	\
	printk Fmt;			\
}
#else
#define DBGPRINT(Level, Fmt)
#define DBGPRINT_RAW(Level, Fmt)
#define DBGPRINT_S(Status, Fmt)
#define DBGPRINT_ERR(Fmt)
#endif

#undef  ASSERT
#ifdef DBG
#define ASSERT(x)							\
{									\
	if (!(x))							\
		printk(__FILE__ ":%d assert " #x "failed\n", __LINE__);	\
}
#else
#define ASSERT(x)
#endif /* DBG */

void hex_dump(char *str, unsigned char *pSrcBufVA, unsigned int SrcBufLen);


/*********************************************************************************************************
	The following code are not revised, temporary put it here.
  *********************************************************************************************************/


/***********************************************************************************
 * Device DMA Access related definitions and data structures.
 **********************************************************************************/
/*#ifdef RTMP_MAC_PCI*/
#define size_t						unsigned long

ra_dma_addr_t linux_pci_map_single(void *handle, void *ptr, size_t size, int sd_idx, int direction);
void linux_pci_unmap_single(void *handle, ra_dma_addr_t dma_addr, size_t size, int direction);

#define pci_enable_msi		RtmpOsPciMsiEnable
#define pci_disable_msi		RtmpOsPciMsiDisable

#define PCI_MAP_SINGLE_DEV(_handle, _ptr, _size, _sd_idx, _dir)				\
	linux_pci_map_single(_handle, _ptr, _size, _sd_idx, _dir)
	
#define PCI_UNMAP_SINGLE(_pAd, _ptr, _size, _dir)						\
	linux_pci_unmap_single(((POS_COOKIE)(_pAd->OS_Cookie))->pci_dev, _ptr, _size, _dir)

#define PCI_ALLOC_CONSISTENT(_pci_dev, _size, _ptr)							\
	pci_alloc_consistent(_pci_dev, _size, _ptr)

#define PCI_FREE_CONSISTENT(_pci_dev, _size, _virtual_addr, _physical_addr)	\
	pci_free_consistent(_pci_dev, _size, _virtual_addr, _physical_addr)
/*#endif RTMP_MAC_PCI*/

#define DEV_ALLOC_SKB(_pAd, _Pkt, _length)									\
	_Pkt = RtmpOSNetPktAlloc(_pAd, _length);

/*#ifdef RTMP_MAC_USB */
/*#define PCI_MAP_SINGLE(_handle, _ptr, _size, _dir)				(unsigned long)0 */
/*#define PCI_UNMAP_SINGLE(_handle, _ptr, _size, _dir) */
/*#endif RTMP_MAC_USB */


/*
 * unsigned long
 * RTMP_GetPhysicalAddressLow(
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress);
 */
#define RTMP_GetPhysicalAddressLow(phy_addr)		(phy_addr)

/*
 * unsigned long
 * RTMP_GetPhysicalAddressHigh(
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress);
 */
#define RTMP_GetPhysicalAddressHigh(phy_addr)		(0)

/*
 * VOID
 * RTMP_SetPhysicalAddressLow(
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress,
 *   IN unsigned long  Value);
 */
#define RTMP_SetPhysicalAddressLow(phy_addr, Value)	\
			phy_addr = Value;

/*
 * VOID
 * RTMP_SetPhysicalAddressHigh(
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress,
 *   IN unsigned long  Value);
 */
#define RTMP_SetPhysicalAddressHigh(phy_addr, Value)

#define NdisMIndicateStatus(_w, _x, _y, _z)



/***********************************************************************************
 * Device Register I/O Access related definitions and data structures.
 **********************************************************************************/
#define readl		RTMP_PCI_Readl
#define readw		RTMP_PCI_Readw
#define readb		RTMP_PCI_Readb
#define writel		RTMP_PCI_Writel
#define writew		RTMP_PCI_Writew
#define writeb		RTMP_PCI_Writeb

/* TODO: We can merge two readl to a function to speed up or one real/writel */


#ifdef RTMP_MAC_USB
#define RTMP_IO_FORCE_READ32(_A, _R, _pV)								\
	RTUSBReadMACRegister((_A), (_R), (unsigned int *) (_pV))

#define RTMP_IO_FORCE_WRITE32(_A, _R, _V)	\
	do{\
		/* if ((_R) != 0x404)*/ /* TODO:shiang-6590, depends on sw porting guide, don't acccess it now */\
			RTUSBWriteMACRegister((_A), (_R), (unsigned int) (_V), FALSE);		\
	}while(0)

#define RTMP_IO_READ32(_A, _R, _pV)								\
	RTUSBReadMACRegister((_A), (_R), (unsigned int *) (_pV))

#define RTMP_IO_READ8(_A, _R, _pV)								\
{																\
}

#define RTMP_IO_WRITE32(_A, _R, _V)								\
	RTUSBWriteMACRegister((_A), (_R), (unsigned int) (_V), FALSE)

#define RTMP_IO_WRITE8(_A, _R, _V)								\
{																\
	unsigned short	_Val = _V;											\
	RTUSBSingleWrite((_A), (_R), (unsigned short) (_Val), FALSE);		\
}


#define RTMP_IO_WRITE16(_A, _R, _V)								\
{																\
	RTUSBSingleWrite((_A), (_R), (unsigned short) (_V), FALSE);			\
}

#define RTMP_SYS_IO_READ32
#define RTMP_SYS_IO_WRITE32
#endif /* RTMP_MAC_USB */

#define pci_read_config_word	RtmpOsPciConfigReadWord
#define pci_write_config_word	RtmpOsPciConfigWriteWord
#define pci_read_config_dword	RtmpOsPciConfigReadDWord
#define pci_write_config_dword	RtmpOsPciConfigWriteDWord
#define pci_find_capability		RtmpOsPciFindCapability

#define RTMP_USB_URB_DATA_GET	RtmpOsUsbUrbDataGet
#define RTMP_USB_URB_STATUS_GET	RtmpOsUsbUrbStatusGet
#define RTMP_USB_URB_LEN_GET	RtmpOsUsbUrbLenGet

#define IW_SCAN_MAX_DATA		RTMP_OS_MAX_SCAN_DATA_GET()

/***********************************************************************************
 *	Network Related data structure and marco definitions
 ***********************************************************************************/
#define PKTSRC_NDIS             0x7f
#define PKTSRC_DRIVER           0x0f

#define RTMP_OS_NETDEV_GET_PHYADDR	RtmpOsNetDevGetPhyAddr
#define SET_OS_PKT_NETDEV			RtmpOsSetPktNetDev
#define RTMP_OS_NETDEV_GET_DEVNAME	RtmpOsGetNetDevName
#define RTMP_OS_NETDEV_SET_TYPE		RtmpOsSetNetDevType
#define RTMP_OS_NETDEV_SET_TYPE_MONITOR		RtmpOsSetNetDevTypeMonitor

#define QUEUE_ENTRY_TO_PACKET(pEntry) \
	(PNDIS_PACKET)(pEntry)

#define PACKET_TO_QUEUE_ENTRY(pPacket) \
	(PQUEUE_ENTRY)(pPacket)

#define RTMP_OS_NETDEV_STATE_RUNNING(_pNetDev)	(RtmpOSNetDevIsUp(_pNetDev) == TRUE)

#define RELEASE_NDIS_PACKET(_pReserved, _pPacket, _Status)				\
{																		\
        RTMPFreeNdisPacket(_pReserved, _pPacket);						\
}

/*
 * packet helper
 * 	- convert internal rt packet to os packet or
 *             os packet to rt packet
 */
extern unsigned long RTPktOffsetData, RTPktOffsetLen, RTPktOffsetCB;

#define RTPKT_TO_OSPKT(_p)		(_p)
#define OSPKT_TO_RTPKT(_p)		(_p)

#define GET_OS_PKT_DATAPTR(_pkt) \
		((unsigned char *)(*(unsigned long *)((unsigned char *)_pkt + RTPktOffsetData)))

#define SET_OS_PKT_DATAPTR		\
		RtmpOsPktDataPtrAssign

#define GET_OS_PKT_LEN(_pkt) 	\
		(*(unsigned int *)((unsigned char *)_pkt + RTPktOffsetLen))

#define SET_OS_PKT_LEN			\
		RtmpOsPktLenAssign

#define GET_OS_PKT_CB(_pkt) 	\
		((unsigned char *)((unsigned char *)_pkt + RTPktOffsetCB))

#define GET_OS_PKT_NETDEV(_pkt) RtmpOsPktNetDevGet

#define OS_PKT_CLONED			\
		RtmpOsIsPktCloned

#define OS_PKT_COPY				\
		RtmpOsPktCopy

#define OS_PKT_TAIL_ADJUST		\
		RtmpOsPktTailAdjust

#define OS_PKT_HEAD_BUF_EXTEND	\
		RtmpOsPktHeadBufExtend

#define OS_PKT_TAIL_BUF_EXTEND	\
		RtmpOsPktTailBufExtend

#define OS_PKT_RESERVE			\
		RtmpOsPktReserve

#define OS_PKT_CLONE(_pAd, _pkt, _src, _flag)		\
		_src = RtmpOsPktClone((_pkt));

#define RTMP_OS_PKT_INIT			RtmpOsPktInit

extern unsigned int RtmpOsGetUnaligned32(
	IN unsigned int				*pWord);

extern unsigned long RtmpOsGetUnalignedlong(
	IN unsigned long				*pWord);

#define get_unaligned		RtmpOsGetUnaligned
#define get_unaligned32		RtmpOsGetUnaligned32
#define get_unalignedlong	RtmpOsGetUnalignedlong

#define OS_NTOHS		RtmpOsNtohs
#define OS_HTONS		RtmpOsHtons
#define OS_NTOHL		RtmpOsNtohl
#define OS_HTONL		RtmpOsHtonl

#ifndef ntohs
#define ntohs			OS_NTOHS
#endif /* ntohs */
#ifndef htons
#define htons			OS_HTONS
#endif /* htons */
#ifndef ntohl
#define ntohl			OS_NTOHL
#endif /* ntohl */
#ifndef htonl
#define htonl			OS_HTONL
#endif /* htonl */

#define RTMP_OS_NETDEV_START_QUEUE				RtmpOsNetQueueStart
#define RTMP_OS_NETDEV_STOP_QUEUE				RtmpOsNetQueueStop
#define RTMP_OS_NETDEV_WAKE_QUEUE				RtmpOsNetQueueWake


#define CB_OFF  10

#define PACKET_CB_ASSIGN(_p, _offset)	\
	(*((unsigned char *)_p + RTPktOffsetCB + _offset))

#define PACKET_CB(_p, _offset)	\
	(*((unsigned char *)_p + RTPktOffsetCB + CB_OFF + _offset))

/* User Priority */
#define RTMP_SET_PACKET_UP(_p, _prio)			(PACKET_CB(_p, 0) = _prio)
#define RTMP_GET_PACKET_UP(_p)					(PACKET_CB(_p, 0))

/* Fragment # */
#define RTMP_SET_PACKET_FRAGMENTS(_p, _num)		(PACKET_CB(_p, 1) = _num)
#define RTMP_GET_PACKET_FRAGMENTS(_p)			(PACKET_CB(_p, 1))

/* 0x0 ~0x7f: TX to AP's own BSS which has the specified AID. if AID>127, set bit 7 in RTMP_SET_PACKET_EMACTAB too. */
/*(this value also as MAC(on-chip WCID) table index) */
/* 0x80~0xff: TX to a WDS link. b0~6: WDS index */
#define RTMP_SET_PACKET_WCID(_p, _wdsidx)		(PACKET_CB(_p, 2) = _wdsidx)
#define RTMP_GET_PACKET_WCID(_p)          		(PACKET_CB(_p, 2))

/* 0xff: PKTSRC_NDIS, others: local TX buffer index. This value affects how to a packet */
#define RTMP_SET_PACKET_SOURCE(_p, _pktsrc)		(PACKET_CB(_p, 3) = _pktsrc)
#define RTMP_GET_PACKET_SOURCE(_p)       		(PACKET_CB(_p, 3))

/* RTS/CTS-to-self protection method */
#define RTMP_SET_PACKET_RTS(_p, _num)      		(PACKET_CB(_p, 4) = _num)
#define RTMP_GET_PACKET_RTS(_p)          		(PACKET_CB(_p, 4))
/* see RTMP_S(G)ET_PACKET_EMACTAB */

/* TX rate index */
#define RTMP_SET_PACKET_TXRATE(_p, _rate)		(PACKET_CB(_p, 5) = _rate)
#define RTMP_GET_PACKET_TXRATE(_p)		  		(PACKET_CB(_p, 5))

/* From which Interface */
#define RTMP_SET_PACKET_IF(_p, _ifdx)			(PACKET_CB(_p, 6) = _ifdx)
#define RTMP_GET_PACKET_IF(_p)		  			(PACKET_CB(_p, 6))
#define RTMP_SET_PACKET_NET_DEVICE_MBSSID(_p, _bss)		RTMP_SET_PACKET_IF((_p), (_bss))
#define RTMP_SET_PACKET_NET_DEVICE_WDS(_p, _bss)		RTMP_SET_PACKET_IF((_p), ((_bss) + MIN_NET_DEVICE_FOR_WDS))
#define RTMP_SET_PACKET_NET_DEVICE_APCLI(_p, _idx)   	RTMP_SET_PACKET_IF((_p), ((_idx) + MIN_NET_DEVICE_FOR_APCLI))
#define RTMP_SET_PACKET_NET_DEVICE_MESH(_p, _idx)   	RTMP_SET_PACKET_IF((_p), ((_idx) + MIN_NET_DEVICE_FOR_MESH))
#define RTMP_SET_PACKET_NET_DEVICE_P2P(_p, _idx)   		RTMP_SET_PACKET_IF((_p), ((_idx) + MIN_NET_DEVICE_FOR_P2P_GO))
#define RTMP_GET_PACKET_NET_DEVICE_MBSSID(_p)			RTMP_GET_PACKET_IF((_p))
#define RTMP_GET_PACKET_NET_DEVICE(_p)					RTMP_GET_PACKET_IF((_p))

#define RTMP_SET_PACKET_MOREDATA(_p, _morebit)	(PACKET_CB(_p, 7) = _morebit)
#define RTMP_GET_PACKET_MOREDATA(_p)			(PACKET_CB(_p, 7))



#ifdef UAPSD_SUPPORT
/* if we queue a U-APSD packet to any software queue, we will set the U-APSD
   flag and its physical queue ID for it */
#define RTMP_SET_PACKET_UAPSD(_p, _flg_uapsd, _que_id) \
                    (PACKET_CB(_p, 9) = ((_flg_uapsd<<7) | _que_id))

#define RTMP_SET_PACKET_QOS_NULL(_p)     (PACKET_CB(_p, 9) = 0xff)
#define RTMP_GET_PACKET_QOS_NULL(_p)	 (PACKET_CB(_p, 9))
#define RTMP_SET_PACKET_NON_QOS_NULL(_p) (PACKET_CB(_p, 9) = 0x00)
#define RTMP_GET_PACKET_UAPSD_Flag(_p)   ((PACKET_CB(_p, 9) & 0x80) >> 7)
#define RTMP_GET_PACKET_UAPSD_QUE_ID(_p) (PACKET_CB(_p, 9) & 0x7f)

#define RTMP_SET_PACKET_EOSP(_p, _flg)   (PACKET_CB(_p, 10) = _flg)
#define RTMP_GET_PACKET_EOSP(_p)         (PACKET_CB(_p, 10))
#endif /* UAPSD_SUPPORT */


/* */
/*	Sepcific Pakcet Type definition */
/* */
#define RTMP_PACKET_SPECIFIC_CB_OFFSET	11

#define RTMP_PACKET_SPECIFIC_DHCP		0x01
#define RTMP_PACKET_SPECIFIC_EAPOL		0x02
#define RTMP_PACKET_SPECIFIC_IPV4		0x04
#define RTMP_PACKET_SPECIFIC_WAI		0x08
#define RTMP_PACKET_SPECIFIC_VLAN		0x10
#define RTMP_PACKET_SPECIFIC_LLCSNAP	0x20
#define RTMP_PACKET_SPECIFIC_TDLS		0x40

/*Specific */
#define RTMP_SET_PACKET_SPECIFIC(_p, _flg)	   	(PACKET_CB(_p, 11) = _flg)

/*DHCP */
#define RTMP_SET_PACKET_DHCP(_p, _flg)   								\
			do{															\
				if (_flg)												\
					PACKET_CB(_p, 11) |= (RTMP_PACKET_SPECIFIC_DHCP);	\
				else													\
					PACKET_CB(_p, 11) &= (~RTMP_PACKET_SPECIFIC_DHCP);	\
			}while(0)
#define RTMP_GET_PACKET_DHCP(_p)		(PACKET_CB(_p, 11) & RTMP_PACKET_SPECIFIC_DHCP)

/*EAPOL */
#define RTMP_SET_PACKET_EAPOL(_p, _flg)   								\
			do{															\
				if (_flg)												\
					PACKET_CB(_p, 11) |= (RTMP_PACKET_SPECIFIC_EAPOL);	\
				else													\
					PACKET_CB(_p, 11) &= (~RTMP_PACKET_SPECIFIC_EAPOL);	\
			}while(0)
#define RTMP_GET_PACKET_EAPOL(_p)		(PACKET_CB(_p, 11) & RTMP_PACKET_SPECIFIC_EAPOL)

/*WAI */
#define RTMP_SET_PACKET_WAI(_p, _flg)   								\
			do{															\
				if (_flg)												\
					PACKET_CB(_p, 11) |= (RTMP_PACKET_SPECIFIC_WAI);	\
				else													\
					PACKET_CB(_p, 11) &= (~RTMP_PACKET_SPECIFIC_WAI);	\
			}while(0)
#define RTMP_GET_PACKET_WAI(_p)			(PACKET_CB(_p, 11) & RTMP_PACKET_SPECIFIC_WAI)

#define RTMP_GET_PACKET_LOWRATE(_p)		(PACKET_CB(_p, 11) & (RTMP_PACKET_SPECIFIC_EAPOL | RTMP_PACKET_SPECIFIC_DHCP | RTMP_PACKET_SPECIFIC_WAI))

/*VLAN */
#define RTMP_SET_PACKET_VLAN(_p, _flg)   								\
			do{															\
				if (_flg)												\
					PACKET_CB(_p, 11) |= (RTMP_PACKET_SPECIFIC_VLAN);	\
				else													\
					PACKET_CB(_p, 11) &= (~RTMP_PACKET_SPECIFIC_VLAN);	\
			}while(0)
#define RTMP_GET_PACKET_VLAN(_p)		(PACKET_CB(_p, 11) & RTMP_PACKET_SPECIFIC_VLAN)

/*LLC/SNAP */
#define RTMP_SET_PACKET_LLCSNAP(_p, _flg)   								\
			do{																\
				if (_flg)													\
					PACKET_CB(_p, 11) |= (RTMP_PACKET_SPECIFIC_LLCSNAP);	\
				else														\
					PACKET_CB(_p, 11) &= (~RTMP_PACKET_SPECIFIC_LLCSNAP);	\
			}while(0)
			
#define RTMP_GET_PACKET_LLCSNAP(_p)		(PACKET_CB(_p, 11) & RTMP_PACKET_SPECIFIC_LLCSNAP)

/* IP */
#define RTMP_SET_PACKET_IPV4(_p, _flg)									\
			do{															\
				if (_flg)												\
					PACKET_CB(_p, 11) |= (RTMP_PACKET_SPECIFIC_IPV4);	\
				else													\
					PACKET_CB(_p, 11) &= (~RTMP_PACKET_SPECIFIC_IPV4);	\
			}while(0)
			
#define RTMP_GET_PACKET_IPV4(_p)		(PACKET_CB(_p, 11) & RTMP_PACKET_SPECIFIC_IPV4)

/* TDLS */
#define RTMP_SET_PACKET_TDLS(_p, _flg)									\
			do{															\
				if (_flg)												\
					(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) |= (RTMP_PACKET_SPECIFIC_TDLS);	\
				else																		\
					(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) &= (~RTMP_PACKET_SPECIFIC_TDLS);	\
			}while(0)
			
#define RTMP_GET_PACKET_TDLS(_p)		(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11] & RTMP_PACKET_SPECIFIC_TDLS)

/* If this flag is set, it indicates that this EAPoL frame MUST be clear. */
#define RTMP_SET_PACKET_CLEAR_EAP_FRAME(_p, _flg)   (PACKET_CB(_p, 12) = _flg)
#define RTMP_GET_PACKET_CLEAR_EAP_FRAME(_p)         (PACKET_CB(_p, 12))


#ifdef DOT11_VHT_AC
#define MAX_PACKETS_IN_QUEUE				1024 /*(512)*/
#else
#define MAX_PACKETS_IN_QUEUE				(512)
#endif /* DOT11_VHT_AC */


/* use bit3 of cb[CB_OFF+16] */
#define RTMP_SET_PACKET_MGMT_PKT(_p, _flg)	        \
        PACKET_CB(_p, 16) = (PACKET_CB(_p, 16) & 0xF7) | ((_flg & 0x01) << 3);
#define RTMP_GET_PACKET_MGMT_PKT(_p)				\
		((PACKET_CB(_p, 16) & 0x08) >> 3)

/* use bit0 of cb[CB_OFF+20] */
#define RTMP_SET_PACKET_MGMT_PKT_DATA_QUE(_p, _flg)	\
        PACKET_CB(_p, 20) = (PACKET_CB(_p, 20) & 0xFE) | (_flg & 0x01);
#define RTMP_GET_PACKET_MGMT_PKT_DATA_QUE(_p)		\
		(PACKET_CB(_p, 20) & 0x01)

#define RTMP_SET_PACKET_5VT(_p, _flg)   (PACKET_CB(_p, 22) = _flg)
#define RTMP_GET_PACKET_5VT(_p)         (PACKET_CB(_p, 22))

#define RTMP_SET_PACKET_PROTOCOL(_p, _protocol) {\
	(PACKET_CB(_p, 23) = (unsigned char)((_protocol) & 0x00ff)); \
	(PACKET_CB(_p, 24) = (unsigned char)(((_protocol) & 0xff00) >> 8)); \
}

#define RTMP_GET_PACKET_PROTOCOL(_p) \
	((((unsigned short)PACKET_CB(_p, 23)) << 8) \
	| ((unsigned short)PACKET_CB(_p, 24)))

#ifdef INF_AMAZON_SE
/* [CB_OFF+28], 1B, Iverson patch for WMM A5-T07 ,WirelessStaToWirelessSta do not bulk out aggregate */
#define RTMP_SET_PACKET_NOBULKOUT(_p, _morebit)			(PACKET_CB(_p, 28) = _morebit)
#define RTMP_GET_PACKET_NOBULKOUT(_p)					(PACKET_CB(_p, 28))
#endif /* INF_AMAZON_SE */
/* Max skb->cb = 48B = [CB_OFF+38] */




#if defined(CONFIG_CSO_SUPPORT) || defined(CONFIG_RX_CSO_SUPPORT)
#define RTMP_SET_TCP_CHKSUM_FAIL(_p, _flg) 	(PACKET_CB(_p, 30) = _flg);
#define RTMP_GET_TCP_CHKSUM_FAIL(_p)		(PACKET_CB(_p, 30))
#endif /* defined(CONFIG_CSO_SUPPORT) || defined(CONFIG_RX_CSO_SUPPORT) */



/***********************************************************************************
 *	Other function prototypes definitions
 ***********************************************************************************/
void RTMP_GetCurrentSystemTime(LARGE_INTEGER *time);




#ifdef RTMP_USB_SUPPORT
/******************************************************************************

  	USB related definitions

******************************************************************************/

#define RTMP_USB_PKT_COPY			RtmpOsPktBodyCopy

/*typedef struct usb_device_id USB_DEVICE_ID; */

#ifdef INF_AMAZON_SE
#define BULKAGGRE_SIZE          30
#else
#define BULKAGGRE_SIZE          60
#endif /* INF_AMAZON_SE */

/*#define RT28XX_PUT_DEVICE			rausb_put_dev */
#define RTUSB_ALLOC_URB				rausb_alloc_urb
#define RTUSB_SUBMIT_URB			rausb_submit_urb
#define RTUSB_URB_ALLOC_BUFFER		rausb_buffer_alloc
#define RTUSB_URB_FREE_BUFFER		rausb_buffer_free
#define RTUSB_FREE_URB				rausb_free_urb
#define RTUSB_UNLINK_URB			rausb_kill_urb
#define USB_CONTROL_MSG				rausb_control_msg
#define usb_sndctrlpipe				rausb_sndctrlpipe
#define usb_rcvctrlpipe				rausb_rcvctrlpipe
#define RTUSB_AUTOPM_PUT_INTERFACE	rausb_autopm_put_interface
#define RTUSB_AUTOPM_GET_INTERFACE	rausb_autopm_get_interface



#define RTUSB_CONTROL_MSG(pUsb_Dev, uEndpointAddress, Request, RequestType, Value,Index, tmpBuf, TransferBufferLength, timeout, ret)	\
  		do{	\
			if (RequestType == DEVICE_VENDOR_REQUEST_OUT)	\
				ret = USB_CONTROL_MSG(pUsb_Dev, usb_sndctrlpipe(pUsb_Dev, uEndpointAddress), Request, RequestType, Value, Index, tmpBuf, TransferBufferLength, timeout);	\
			else if (RequestType == DEVICE_VENDOR_REQUEST_IN)	\
				ret = USB_CONTROL_MSG(pUsb_Dev, usb_rcvctrlpipe(pUsb_Dev, uEndpointAddress), Request, RequestType, Value, Index, tmpBuf, TransferBufferLength, timeout);	\
			else	\
			{	\
				DBGPRINT(RT_DEBUG_ERROR, ("vendor request direction is failed\n"));	\
				ret = -1;	\
			}	\
		}while(0)


extern VOID dump_urb(VOID *purb);

typedef VOID		USBHST_STATUS;
typedef int		URBCompleteStatus;
#define RTMP_OS_WAIT_QUEUE_HEAD		VOID
typedef VOID (*usb_complete_t)(VOID *);

#define RtmpUsbBulkOutDataPacketComplete		pRtmpDrvNetOps->RtmpNetUsbBulkOutDataPacketComplete
#define RtmpUsbBulkOutMLMEPacketComplete		pRtmpDrvNetOps->RtmpNetUsbBulkOutMLMEPacketComplete
#define RtmpUsbBulkOutNullFrameComplete			pRtmpDrvNetOps->RtmpNetUsbBulkOutNullFrameComplete
#ifdef CONFIG_MULTI_CHANNEL
#define RtmpUsbBulkOutHCCANullFrameComplete			pRtmpDrvNetOps->RtmpNetUsbBulkOutHCCANullFrameComplete
#endif /* CONFIG_MULTI_CHANNEL */

#define RtmpUsbBulkOutRTSFrameComplete			pRtmpDrvNetOps->RtmpNetUsbBulkOutRTSFrameComplete
#define RtmpUsbBulkOutPsPollComplete			pRtmpDrvNetOps->RtmpNetUsbBulkOutPsPollComplete
#define RtmpUsbBulkRxComplete					pRtmpDrvNetOps->RtmpNetUsbBulkRxComplete
#define RtmpUsbBulkCmdRspEventComplete			pRtmpDrvNetOps->RtmpNetUsbBulkCmdRspEventComplete

#define RTUSBBulkOutDataPacketComplete(Status, pURB, pt_regs)    RTUSBBulkOutDataPacketComplete(pURB)
#define RTUSBBulkOutMLMEPacketComplete(Status, pURB, pt_regs)    RTUSBBulkOutMLMEPacketComplete(pURB)
#define RTUSBBulkOutNullFrameComplete(Status, pURB, pt_regs)     RTUSBBulkOutNullFrameComplete(pURB)
#define RTUSBBulkOutRTSFrameComplete(Status, pURB, pt_regs)      RTUSBBulkOutRTSFrameComplete(pURB)
#define RTUSBBulkOutPsPollComplete(Status, pURB, pt_regs)        RTUSBBulkOutPsPollComplete(pURB)
#define RTUSBBulkRxComplete(Status, pURB, pt_regs)               RTUSBBulkRxComplete(pURB)
#define RTUSBBulkCmdRspEventComplete(Status, pURB, pt_regs)	 	 RTUSBBulkCmdRspEventComplete(pURB)
#define USBUploadFWComplete(Status, pURB, pt_regs)               USBUploadFWComplete(pURB)
#define USBKickOutCmdComplete(Status, pURB, pt_regs)               USBKickOutCmdComplete(pURB)
#ifdef CONFIG_MULTI_CHANNEL
#define RTUSBBulkOutHCCANullFrameComplete(Status, pURB, pt_regs)	RTUSBBulkOutHCCANullFrameComplete(pURB)
#endif /* CONFIG_MULTI_CHANNEL */


USBHST_STATUS RTUSBBulkOutDataPacketComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkOutMLMEPacketComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkOutNullFrameComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkOutRTSFrameComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkOutPsPollComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkRxComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkCmdRspEventComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
#ifdef CONFIG_MULTI_CHANNEL
USBHST_STATUS RTUSBBulkOutHCCANullFrameComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
#endif /* CONFIG_MULTI_CHANNEL */

#define rtusb_urb_context  context
#define rtusb_urb_status   status

#define RTMP_OS_USB_CONTEXT_GET		RtmpOsUsbContextGet
#define RTMP_OS_USB_STATUS_GET		RtmpOsUsbStatusGet

#define RTUSB_URB_DMA_MAPPING		RtmpOsUsbDmaMapping

#define RTUSB_FILL_TX_BULK_URB		RtmpOsUsbInitHTTxDesc
#define RTUSB_FILL_HTTX_BULK_URB	RtmpOsUsbInitHTTxDesc
#define RTUSB_FILL_RX_BULK_URB		RtmpOsUsbInitRxDesc

#undef in_interrupt
#define in_interrupt		RtmpOsIsInInterrupt

extern VOID *rausb_alloc_urb(int iso_packets);
extern VOID rausb_free_urb(VOID *urb);
extern int rausb_submit_urb(VOID *urb);
extern VOID *rausb_buffer_alloc(VOID *dev,
								size_t size,
								ra_dma_addr_t *dma);
extern VOID rausb_buffer_free(VOID *dev,
								size_t size,
								VOID *addr,
								ra_dma_addr_t dma);
extern VOID rausb_kill_urb(VOID *urb);

extern int rausb_control_msg(VOID *dev,
						unsigned int pipe,
						__u8 request,
						__u8 requesttype,
						__u16 value,
						__u16 index,
						void *data,
						__u16 size,
						int timeout);

#endif /* RTMP_USB_SUPPORT */


#ifdef RALINK_ATE
/******************************************************************************

  	ATE related definitions

******************************************************************************/
#define ate_print printk
#define ATEDBGPRINT DBGPRINT

#ifdef RTMP_MAC_USB
#ifdef CONFIG_AP_SUPPORT
#define EEPROM_BIN_FILE_NAME  "/etc/wifi/RT2870AP/e2p.bin"
#endif /* CONFIG_AP_SUPPORT */
#endif /* RTMP_MAC_USB */

#endif /* RALINK_ATE */


/* OS definition re-declaration */
#ifndef NULL
#define NULL			0
#endif

#ifndef ETH_P_IPV6
#define ETH_P_IPV6		0x86DD
#endif

#ifndef ETH_P_IP
#define ETH_P_IP        0x0800          /* Internet Protocol packet     */
#endif

#ifndef ETH_ALEN
#define ETH_ALEN		6
#endif

#undef KERN_EMERG
#define KERN_EMERG
#undef KERN_WARNING
#define KERN_WARNING

#undef copy_from_user
#undef copy_to_user

#define RTMP_OS_MAX_SCAN_DATA_GET		RtmpOsMaxScanDataGet
#define vmalloc							RtmpOsVmalloc
#define vfree							RtmpOsVfree
#define copy_from_user					RtmpOsCopyFromUser
#define copy_to_user					RtmpOsCopyToUser
#define simple_strtol					RtmpOsSimpleStrtol

#undef atomic_read
#undef atomic_dec
#undef InterlockedExchange
#define atomic_read						RtmpOsAtomicRead
#define atomic_dec						RtmpOsAtomicDec
#define InterlockedExchange				RtmpOsAtomicInterlockedExchange

extern int sprintf(char * buf, const char * fmt, ...);
extern int sscanf(const char *, const char *, ...);

#define printk							pRaOsOps->ra_printk
#define snprintf						pRaOsOps->ra_snprintf

#endif /* __RT_LINUX_H__ */
