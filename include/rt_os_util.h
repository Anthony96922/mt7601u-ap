/****************************************************************************

    Module Name:
	rt_os_util.h

	Abstract:
	All function prototypes are provided from UTIL modules.

	Note:
	But can not use any OS key word and compile option here.
	All functions are provided from UTIL modules.

    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------

***************************************************************************/

#ifndef __RT_OS_UTIL_H__
#define __RT_OS_UTIL_H__

/* ============================ rt_linux.c ================================== */
/* General */
VOID RtmpUtilInit(VOID);

/* OS Time */
VOID RTMPusecDelay(
	IN	unsigned long					usec);

VOID RtmpOsUsDelay(unsigned long value);

VOID RtmpOsMsDelay(
	IN	unsigned long					msec);

void RTMP_GetCurrentSystemTime(
	IN	LARGE_INTEGER			*time);

void RTMP_GetCurrentSystemTick(
	IN	unsigned long					*pNow);

VOID RtmpOsWait(
	IN	unsigned int					Time);

unsigned int RtmpOsTimerAfter(
	IN	unsigned long					a,
	IN	unsigned long					b);

unsigned int RtmpOsTimerBefore(
	IN	unsigned long					a,
	IN	unsigned long					b);

VOID RtmpOsGetSystemUpTime(
	IN	unsigned long					*pTime);

unsigned int RtmpOsTickUnitGet(VOID);

/* OS Memory */
NDIS_STATUS os_alloc_mem(
	IN	VOID					*pReserved,
	OUT	unsigned char					**mem,
	IN	unsigned long					size);

NDIS_STATUS os_alloc_mem_suspend(
	IN	VOID					*pReserved,
	OUT	unsigned char					**mem,
	IN	unsigned long					size);

NDIS_STATUS os_free_mem(
	IN	VOID					*pReserved,
	IN	void *					mem);

NDIS_STATUS AdapterBlockAllocateMemory(
	IN	void *					handle,
	OUT	void *					*ppAd,
	IN	unsigned int					SizeOfpAd);

VOID *RtmpOsVmalloc(
	IN	unsigned long					Size);

VOID RtmpOsVfree(
	IN	VOID					*pMem);

unsigned long RtmpOsCopyFromUser(
	OUT	VOID					*to,
	IN	const void				*from,
	IN	unsigned long					n);

unsigned long RtmpOsCopyToUser(
	OUT VOID					*to,
	IN	const void				*from,
	IN	unsigned long					n);

bool RtmpOsStatsAlloc(
	IN	VOID					**ppStats,
	IN	VOID					**ppIwStats);

/* OS Packet */
PNDIS_PACKET RtmpOSNetPktAlloc(
	IN	VOID					*pReserved,
	IN	int						size);

PNDIS_PACKET RTMP_AllocateFragPacketBuffer(
	IN	VOID					*pReserved,
	IN	unsigned long					Length);

NDIS_STATUS RTMPAllocateNdisPacket(
	IN	VOID					*pReserved,
	OUT PNDIS_PACKET			*ppPacket,
	IN	unsigned char *					pHeader,
	IN	unsigned int					HeaderLen,
	IN	unsigned char *					pData,
	IN	unsigned int					DataLen);

VOID RTMPFreeNdisPacket(
	IN	VOID					*pReserved,
	IN	PNDIS_PACKET			pPacket);

NDIS_STATUS Sniff2BytesFromNdisBuffer(
	IN  PNDIS_BUFFER			pFirstBuffer,
	IN  unsigned char           		DesiredOffset,
	OUT unsigned char *          		pByte0,
	OUT unsigned char *          		pByte1);

void RTMP_QueryPacketInfo(
	IN  PNDIS_PACKET			pPacket,
	OUT PACKET_INFO  			*pPacketInfo,
	OUT unsigned char *		 			*pSrcBufVA,
	OUT	unsigned int		 			*pSrcBufLen);

PNDIS_PACKET DuplicatePacket(
	IN	PNET_DEV				pNetDev,
	IN	PNDIS_PACKET			pPacket,
	IN	unsigned char					FromWhichBSSID);

PNDIS_PACKET duplicate_pkt(
	IN	PNET_DEV				pNetDev,
	IN	unsigned char *					pHeader802_3,
    IN  unsigned int            		HdrLen,
	IN	unsigned char *					pData,
	IN	unsigned long					DataSize,
	IN	unsigned char					FromWhichBSSID);

PNDIS_PACKET duplicate_pkt_with_TKIP_MIC(
	IN	VOID					*pReserved,
	IN	PNDIS_PACKET			pOldPkt);

PNDIS_PACKET duplicate_pkt_with_VLAN(
	IN	PNET_DEV				pNetDev,
	IN	unsigned short					VLAN_VID,
	IN	unsigned short					VLAN_Priority,
	IN	unsigned char *					pHeader802_3,
    IN  unsigned int            		HdrLen,
	IN	unsigned char *					pData,
	IN	unsigned long					DataSize,
	IN	unsigned char					FromWhichBSSID,
	IN	unsigned char					*TPID);

typedef void (*RTMP_CB_8023_PACKET_ANNOUNCE)(
			IN	VOID			*pCtrlBkPtr, 
			IN	PNDIS_PACKET	pPacket,
			IN	unsigned char			OpMode);

bool RTMPL2FrameTxAction(
	IN  VOID					*pCtrlBkPtr,
	IN	PNET_DEV				pNetDev,
	IN	RTMP_CB_8023_PACKET_ANNOUNCE _announce_802_3_packet,
	IN	unsigned char					apidx,
	IN	unsigned char *					pData,
	IN	unsigned int					data_len,
	IN	unsigned char			OpMode);

PNDIS_PACKET ExpandPacket(
	IN	VOID					*pReserved,
	IN	PNDIS_PACKET			pPacket,
	IN	unsigned int					ext_head_len,
	IN	unsigned int					ext_tail_len);

PNDIS_PACKET ClonePacket(
	IN	VOID					*pReserved,
	IN	PNDIS_PACKET			pPacket,
	IN	unsigned char *					pData,
	IN	unsigned long					DataSize);

void wlan_802_11_to_802_3_packet(
	IN	PNET_DEV				pNetDev,
	IN	unsigned char					OpMode,
	IN	unsigned short					VLAN_VID,
	IN	unsigned short					VLAN_Priority,
	IN	PNDIS_PACKET			pRxPacket,
	IN	unsigned char					*pData,
	IN	unsigned long					DataSize,
	IN	unsigned char *					pHeader802_3,
	IN  unsigned char					FromWhichBSSID,
	IN	unsigned char					*TPID);

#ifdef HDR_TRANS_SUPPORT
VOID RtmpOsSetPacket(
	IN PNET_DEV pNetDev,
	IN PNDIS_PACKET pRxPacket,
	IN unsigned char *pData,
	IN unsigned long DataSize);
#endif /* HDR_TRANS_SUPPORT */

void send_monitor_packets(
	IN	PNET_DEV				pNetDev,
	IN	PNDIS_PACKET			pRxPacket,
	IN	PHEADER_802_11			pHeader,
	IN	unsigned char					*pData,
	IN	unsigned short					DataSize,
	IN	unsigned char					L2PAD,
	IN	unsigned char					PHYMODE,
	IN	unsigned char					BW,
	IN	unsigned char					ShortGI,
	IN	unsigned char					MCS,
	IN	unsigned char					AMPDU,
	IN	unsigned char					STBC,
	IN	unsigned char					RSSI1,
	IN	unsigned char					BssMonitorFlag11n,
	IN	unsigned char					*pDevName,
	IN	unsigned char					Channel,
	IN	unsigned char					CentralChannel,
	IN	unsigned int					MaxRssi);

unsigned char VLAN_8023_Header_Copy(
	IN	unsigned short					VLAN_VID,
	IN	unsigned short					VLAN_Priority,
	IN	unsigned char *					pHeader802_3,
	IN	unsigned int            		HdrLen,
	OUT unsigned char *					pData,
	IN	unsigned char					FromWhichBSSID,
	IN	unsigned char					*TPID);

VOID RtmpOsPktBodyCopy(
	IN	PNET_DEV				pNetDev,
	IN	PNDIS_PACKET			pNetPkt,
	IN	unsigned long					ThisFrameLen,
	IN	unsigned char *					pData);

INT RtmpOsIsPktCloned(
	IN	PNDIS_PACKET			pNetPkt);

PNDIS_PACKET RtmpOsPktCopy(
	IN	PNDIS_PACKET			pNetPkt);

PNDIS_PACKET RtmpOsPktClone(
	IN	PNDIS_PACKET			pNetPkt);

VOID RtmpOsPktDataPtrAssign(
	IN	PNDIS_PACKET			pNetPkt,
	IN	unsigned char					*pData);

VOID RtmpOsPktLenAssign(
	IN	PNDIS_PACKET			pNetPkt,
	IN	LONG					Len);

VOID RtmpOsPktTailAdjust(
	IN	PNDIS_PACKET			pNetPkt,
	IN	unsigned int					removedTagLen);

unsigned char * RtmpOsPktTailBufExtend(
	IN	PNDIS_PACKET			pNetPkt,
	IN	unsigned int					Len);

unsigned char * RtmpOsPktHeadBufExtend(
	IN	PNDIS_PACKET			pNetPkt,
	IN	unsigned int					Len);

VOID RtmpOsPktReserve(
	IN	PNDIS_PACKET			pNetPkt,
	IN	unsigned int					Len);

VOID RtmpOsPktProtocolAssign(
	IN	PNDIS_PACKET			pNetPkt);

VOID RtmpOsPktInfPpaSend(
	IN	PNDIS_PACKET			pNetPkt);

VOID RtmpOsPktRcvHandle(
	IN	PNDIS_PACKET			pNetPkt);

VOID RtmpOsPktNatMagicTag(
	IN	PNDIS_PACKET			pNetPkt);

VOID RtmpOsPktNatNone(
	IN	PNDIS_PACKET			pNetPkt);

VOID RtmpOsPktInit(
	IN	PNDIS_PACKET			pNetPkt,
	IN	PNET_DEV				pNetDev,
	IN	unsigned char					*pData,
	IN	unsigned short					DataSize);

PNDIS_PACKET RtmpOsPktIappMakeUp(
	IN	PNET_DEV				pNetDev,
	IN	unsigned char					*pMac);

bool RtmpOsPktOffsetInit(VOID);

unsigned short RtmpOsNtohs(
	IN	unsigned short					Value);

unsigned short RtmpOsHtons(
	IN	unsigned short					Value);

unsigned int RtmpOsNtohl(
	IN	unsigned int					Value);

unsigned int RtmpOsHtonl(
	IN	unsigned int					Value);

/* OS File */
struct file *RtmpOSFileOpen(char *pPath,  int flag, int mode);
int RtmpOSFileClose(struct file *osfd);
void RtmpOSFileSeek(struct file *osfd, int offset);
int RtmpOSFileRead(struct file *osfd, char *pDataPtr, size_t readLen);
int RtmpOSFileWrite(struct file *osfd, char *pDataPtr, size_t writeLen);

int RtmpOsFileIsErr(
	IN	VOID					*pFile);

/* OS Network Interface */
int RtmpOSNetDevAddrSet(
	IN unsigned char					OpMode,
	IN PNET_DEV 				pNetDev,
	IN unsigned char *					pMacAddr,
	IN unsigned char *					dev_name);

void RtmpOSNetDevClose(
	IN PNET_DEV					pNetDev);

void RtmpOSNetDevFree(
	IN	PNET_DEV				pNetDev);

INT RtmpOSNetDevAlloc(
	IN	PNET_DEV				*new_dev_p,
	IN	unsigned int					privDataSize);

INT RtmpOSNetDevOpsAlloc(
	IN	void *					*pNetDevOps);


PNET_DEV RtmpOSNetDevGetByName(
	IN	PNET_DEV				pNetDev,
	IN	char *					pDevName);

void RtmpOSNetDeviceRefPut(
	IN	PNET_DEV				pNetDev);

INT RtmpOSNetDevDestory(
	IN	VOID					*pReserved,
	IN	PNET_DEV				pNetDev);

void RtmpOSNetDevDetach(
	IN	PNET_DEV				pNetDev);

int RtmpOSNetDevAttach(
	IN	unsigned char					OpMode,
	IN	PNET_DEV				pNetDev, 
	IN	RTMP_OS_NETDEV_OP_HOOK	*pDevOpHook);

void RtmpOSNetDevProtect(
	IN bool lock_it);

PNET_DEV RtmpOSNetDevCreate(
	IN	int					MC_RowID,
	IN	unsigned int					*pIoctlIF,
	IN	INT 					devType,
	IN	INT						devNum,
	IN	INT						privMemSize,
	IN	char *					pNamePrefix);

bool RtmpOSNetDevIsUp(
	IN	VOID					*pDev);

unsigned char *RtmpOsNetDevGetPhyAddr(
	IN	VOID					*pDev);

VOID RtmpOsNetQueueStart(
	IN	PNET_DEV				pDev);

VOID RtmpOsNetQueueStop(
	IN	PNET_DEV				pDev);

VOID RtmpOsNetQueueWake(
	IN	PNET_DEV				pDev);

VOID RtmpOsSetPktNetDev(
	IN	VOID					*pPkt,
	IN	VOID					*pDev);

PNET_DEV RtmpOsPktNetDevGet(
	IN	VOID					*pPkt);

char *RtmpOsGetNetDevName(
	IN	VOID					*pDev);

VOID RtmpOsSetNetDevPriv(
	IN	VOID					*pDev,
	IN	VOID					*pPriv);

VOID *RtmpOsGetNetDevPriv(
	IN	VOID					*pDev);

unsigned short RtmpDevPrivFlagsGet(
	IN	VOID					*pDev);

VOID RtmpDevPrivFlagsSet(
	IN	VOID					*pDev,
	IN	unsigned short					PrivFlags);

VOID RtmpOsSetNetDevType(VOID *pDev, unsigned short Type);

VOID RtmpOsSetNetDevTypeMonitor(VOID *pDev);


/* OS Semaphore */
VOID RtmpOsCmdUp(RTMP_OS_TASK *pCmdQTask);
bool RtmpOsSemaInitLocked(RTMP_OS_SEM *pSemOrg, LIST_HEADER *pSemList);
bool RtmpOsSemaInit(RTMP_OS_SEM *pSemOrg, LIST_HEADER *pSemList);
bool RtmpOsSemaDestory(RTMP_OS_SEM *pSemOrg);
INT RtmpOsSemaWaitInterruptible(RTMP_OS_SEM *pSemOrg);
VOID RtmpOsSemaWakeUp(RTMP_OS_SEM *pSemOrg);
VOID RtmpOsMlmeUp(RTMP_OS_TASK *pMlmeQTask);

/* OS Task */
bool RtmpOsTaskletSche(RTMP_NET_TASK_STRUCT *pTasklet);

bool RtmpOsTaskletInit(
	RTMP_NET_TASK_STRUCT *pTasklet,
	VOID (*pFunc)(unsigned long data),
	unsigned long Data,
	LIST_HEADER *pTaskletList);

bool RtmpOsTaskletKill(RTMP_NET_TASK_STRUCT *pTasklet);

VOID RtmpOsTaskletDataAssign(
	RTMP_NET_TASK_STRUCT *pTasklet,
	unsigned long Data);

VOID RtmpOsTaskWakeUp(RTMP_OS_TASK *pTaskOrg);

int RtmpOsTaskIsKilled(RTMP_OS_TASK *pTaskOrg);

bool RtmpOsCheckTaskLegality(RTMP_OS_TASK *pTaskOrg);

bool RtmpOSTaskAlloc(
	IN	RTMP_OS_TASK			*pTask,
	IN	LIST_HEADER				*pTaskList);

VOID RtmpOSTaskFree(
	IN	RTMP_OS_TASK			*pTask);

NDIS_STATUS RtmpOSTaskKill(
	IN	RTMP_OS_TASK			*pTaskOrg);

INT RtmpOSTaskNotifyToExit(
	IN	RTMP_OS_TASK			*pTaskOrg);

VOID RtmpOSTaskCustomize(
	IN	RTMP_OS_TASK			*pTaskOrg);

NDIS_STATUS RtmpOSTaskAttach(
	IN	RTMP_OS_TASK			*pTaskOrg,
	IN	RTMP_OS_TASK_CALLBACK	fn,
	IN	unsigned long					arg);

NDIS_STATUS RtmpOSTaskInit(
	IN	RTMP_OS_TASK			*pTaskOrg,
	IN	char *					pTaskName,
	IN	VOID					*pPriv,
	IN	LIST_HEADER				*pTaskList,
	IN	LIST_HEADER				*pSemList);

bool RtmpOSTaskWait(
	IN	VOID					*pReserved,
	IN	RTMP_OS_TASK			*pTaskOrg,
	IN	int					*pStatus);

VOID *RtmpOsTaskDataGet(RTMP_OS_TASK *pTaskOrg);

int RtmpThreadPidKill(RTMP_OS_PID	 PID);

/* OS Cache */
VOID RtmpOsDCacheFlush(unsigned long AddrStart, unsigned long Size);

/* OS Timer */
VOID RTMP_SetPeriodicTimer(
	IN	NDIS_MINIPORT_TIMER *pTimerOrg, 
	IN	unsigned long timeout);

VOID RTMP_OS_Init_Timer(
	IN	VOID 					*pReserved,
	IN	NDIS_MINIPORT_TIMER		*pTimerOrg, 
	IN	TIMER_FUNCTION			function,
	IN	void *					data,
	IN	LIST_HEADER				*pTimerList);

VOID RTMP_OS_Add_Timer(
	IN	NDIS_MINIPORT_TIMER *pTimerOrg,
	IN	unsigned long timeout);

VOID RTMP_OS_Mod_Timer(
	IN	NDIS_MINIPORT_TIMER *pTimerOrg,
	IN	unsigned long timeout);

VOID RTMP_OS_Del_Timer(
	IN	NDIS_MINIPORT_TIMER		*pTimerOrg,
	OUT	bool					*pCancelled);

VOID RTMP_OS_Release_Timer(
	IN	NDIS_MINIPORT_TIMER		*pTimerOrg);

bool RTMP_OS_Alloc_Rsc(
	IN	LIST_HEADER				*pRscList,
	IN	VOID 					*pRsc,
	IN	unsigned int					RscLen);

VOID RTMP_OS_Free_Rscs(
	IN	LIST_HEADER				*pRscList);

/* OS Lock */
bool RtmpOsAllocateLock(
	IN	NDIS_SPIN_LOCK			*pLock,
	IN	LIST_HEADER				*pLockList);

VOID RtmpOsFreeSpinLock(
	IN	NDIS_SPIN_LOCK			*pLockOrg);

VOID RtmpOsSpinLockBh(
	IN	NDIS_SPIN_LOCK			*pLockOrg);

VOID RtmpOsSpinUnLockBh(NDIS_SPIN_LOCK *pLockOrg);
VOID RtmpOsIntLock(NDIS_SPIN_LOCK *pLockOrg, unsigned long *pIrqFlags);
VOID RtmpOsIntUnLock(NDIS_SPIN_LOCK *pLockOrg, unsigned long IrqFlags);

/* OS PID */
VOID RtmpOsGetPid(unsigned long *pDst, unsigned long PID);
VOID RtmpOsTaskPidInit(RTMP_OS_PID *pPid);

/* OS I/O */
VOID RTMP_PCI_Writel(unsigned long Value, VOID *pAddr);
VOID RTMP_PCI_Writew(unsigned long Value, VOID *pAddr);
VOID RTMP_PCI_Writeb(unsigned long Value, VOID *pAddr);
unsigned long RTMP_PCI_Readl(VOID *pAddr);
unsigned long RTMP_PCI_Readw(VOID *pAddr);
unsigned long RTMP_PCI_Readb(VOID *pAddr);

int RtmpOsPciConfigReadWord(
	IN	VOID					*pDev,
	IN	unsigned int					Offset,
	OUT unsigned short					*pValue);

int RtmpOsPciConfigWriteWord(
	IN	VOID					*pDev,
	IN	unsigned int					Offset,
	IN	unsigned short					Value);

int RtmpOsPciConfigReadDWord(
	IN	VOID					*pDev,
	IN	unsigned int					Offset,
	OUT unsigned int					*pValue);

int RtmpOsPciConfigWriteDWord(
	IN	VOID					*pDev,
	IN	unsigned int					Offset,
	IN	unsigned int					Value);

int RtmpOsPciFindCapability(
	IN	VOID					*pDev,
	IN	int						Cap);

VOID *RTMPFindHostPCIDev(VOID *pPciDevSrc);

int RtmpOsPciMsiEnable(VOID *pDev);
VOID RtmpOsPciMsiDisable(VOID *pDev);

/* OS Wireless */
unsigned long RtmpOsMaxScanDataGet(VOID);

/* OS Interrutp */
int RtmpOsIsInInterrupt(VOID);

/* OS USB */
VOID *RtmpOsUsbUrbDataGet(VOID *pUrb);
NTSTATUS RtmpOsUsbUrbStatusGet(VOID *pUrb);
unsigned long RtmpOsUsbUrbLenGet(VOID *pUrb);

/* OS Atomic */
bool RtmpOsAtomicInit(RTMP_OS_ATOMIC *pAtomic, LIST_HEADER *pAtomicList);
VOID RtmpOsAtomicDestroy(RTMP_OS_ATOMIC *pAtomic);
LONG RtmpOsAtomicRead(RTMP_OS_ATOMIC *pAtomic);
VOID RtmpOsAtomicDec(RTMP_OS_ATOMIC *pAtomic);
VOID RtmpOsAtomicInterlockedExchange(RTMP_OS_ATOMIC *pAtomicSrc, LONG Value);

/* OS Utility */
void hex_dump(char *str, unsigned char * pSrcBufVA, unsigned int SrcBufLen);

typedef VOID (*RTMP_OS_SEND_WLAN_EVENT)(
	IN	VOID					*pAdSrc,
	IN	unsigned short					Event_flag,
	IN	unsigned char * 					pAddr,
	IN  unsigned char					BssIdx,
	IN	CHAR					Rssi);

VOID RtmpOsSendWirelessEvent(
	IN	VOID			*pAd,
	IN	unsigned short			Event_flag,
	IN	unsigned char * 			pAddr,
	IN	unsigned char			BssIdx,
	IN	CHAR			Rssi,
	IN	RTMP_OS_SEND_WLAN_EVENT pFunc);

#ifdef CONFIG_AP_SUPPORT
void SendSignalToDaemon(
	IN	INT sig,
	IN	RTMP_OS_PID	 pid,
	IN	unsigned long pid_no);
#endif /* CONFIG_AP_SUPPORT */

int RtmpOSWrielessEventSend(
	IN	PNET_DEV				pNetDev,
	IN	unsigned int					eventType,
	IN	INT						flags,
	IN	unsigned char *					pSrcMac,
	IN	unsigned char *					pData,
	IN	unsigned int					dataLen);

int RtmpOSWrielessEventSendExt(
	IN	PNET_DEV				pNetDev,
	IN	unsigned int					eventType,
	IN	INT						flags,
	IN	unsigned char *					pSrcMac,
	IN	unsigned char *					pData,
	IN	unsigned int					dataLen,
	IN	unsigned int					family);

unsigned int RtmpOsWirelessExtVerGet(VOID);

#ifdef DBG
VOID RtmpDrvAllMacPrint(
	IN VOID						*pReserved,
	IN unsigned int					*pBufMac,
	IN unsigned int					AddrStart,
	IN unsigned int					AddrEnd,
	IN unsigned int					AddrStep);
#endif /* DBG */

VOID RtmpDrvAllE2PPrint(
	IN	VOID					*pReserved,
	IN	unsigned short					*pMacContent,
	IN	unsigned int					AddrEnd,
	IN	unsigned int					AddrStep);

#ifdef DBG
VOID RtmpDrvAllRFPrint(
	IN VOID *pReserved,
	IN unsigned char *pBuf,
	IN unsigned int BufLen);
#endif /* DBG */

int RtmpOSIRQRelease(
	IN	PNET_DEV				pNetDev,
	IN	unsigned int					infType,
	IN	PPCI_DEV				pci_dev,
	IN	bool					*pHaveMsi);

VOID RtmpOsWlanEventSet(
	IN	VOID					*pReserved,
	IN	bool					*pCfgWEnt,
	IN	bool					FlgIsWEntSup);

unsigned short RtmpOsGetUnaligned(unsigned short *pWord);

unsigned int RtmpOsGetUnaligned32(unsigned int *pWord);

unsigned long RtmpOsGetUnalignedlong(unsigned long *pWord);

long RtmpOsSimpleStrtol(
	IN	const char				*cp,
	IN	char 					**endp,
	IN	unsigned int			base);

VOID RtmpOsOpsInit(RTMP_OS_ABL_OPS *pOps);

/* ============================ rt_os_util.c ================================ */
VOID RtmpDrvMaxRateGet(
	IN VOID *pReserved,
	IN unsigned char MODE,
	IN unsigned char ShortGI,
	IN unsigned char BW,
	IN unsigned char MCS,
	OUT unsigned int *pRate);

char * rtstrchr(const char * s, int c);

char *   WscGetAuthTypeStr(unsigned short authFlag);

char *   WscGetEncryTypeStr(unsigned short encryFlag);

unsigned short WscGetAuthTypeFromStr(char * arg);

unsigned short WscGetEncrypTypeFromStr(char * arg);

VOID RtmpMeshDown(
	IN VOID *pDrvCtrlBK,
	IN bool WaitFlag,
	IN bool (*RtmpMeshLinkCheck)(IN VOID *pAd));

unsigned short RtmpOsNetPrivGet(PNET_DEV pDev);

bool RtmpOsCmdDisplayLenCheck(
	IN	unsigned int					LenSrc,
	IN	unsigned int					Offset);

VOID    WpaSendMicFailureToWpaSupplicant(
	IN	PNET_DEV				pNetDev,
    IN  bool					bUnicast);

int wext_notify_event_assoc(
	IN	PNET_DEV				pNetDev,
	IN	unsigned char					*ReqVarIEs,
	IN	unsigned int					ReqVarIELen);

VOID    SendAssocIEsToWpaSupplicant( 
	IN	PNET_DEV				pNetDev,
	IN	unsigned char					*ReqVarIEs,
	IN	unsigned int					ReqVarIELen);

void * RtmpInitCompletion(VOID);

unsigned long RtmpWaitForCompletionTimeout(VOID *Completion, unsigned long Expire);

VOID RtmpComplete(VOID *Completion);

unsigned long RtmpMsecsToJiffies(unsigned int msecs);
/* ============================ rt_rbus_pci_util.c ========================== */
void RtmpAllocDescBuf(
	IN PPCI_DEV pPciDev,
	IN unsigned int Index,
	IN unsigned long Length,
	IN bool Cached,
	OUT VOID **VirtualAddress,
	OUT PNDIS_PHYSICAL_ADDRESS	PhysicalAddress);

void RtmpFreeDescBuf(
	IN PPCI_DEV pPciDev,
	IN unsigned long Length,
	IN VOID *VirtualAddress,
	IN NDIS_PHYSICAL_ADDRESS	PhysicalAddress);

void RTMP_AllocateFirstTxBuffer(
	IN PPCI_DEV pPciDev,
	IN unsigned int Index,
	IN unsigned long Length,
	IN bool Cached,
	OUT VOID **VirtualAddress,
	OUT PNDIS_PHYSICAL_ADDRESS	PhysicalAddress);

void RTMP_FreeFirstTxBuffer(
	IN	PPCI_DEV				pPciDev,
	IN	unsigned long					Length,
	IN	bool					Cached,
	IN	void *					VirtualAddress,
	IN	NDIS_PHYSICAL_ADDRESS	PhysicalAddress);

PNDIS_PACKET RTMP_AllocateRxPacketBuffer(
	IN	VOID					*pReserved,
	IN	VOID					*pPciDev,
	IN	unsigned long					Length,
	IN	bool					Cached,
	OUT	void *					*VirtualAddress,
	OUT	PNDIS_PHYSICAL_ADDRESS	PhysicalAddress);


int rausb_autopm_put_interface (
	IN	VOID			*intfsrc);

int rausb_autopm_get_interface (
	IN	VOID			*intfsrc);

ra_dma_addr_t linux_pci_map_single(void *pPciDev, void *ptr, size_t size, int sd_idx, int direction);

void linux_pci_unmap_single(void *pPciDev, ra_dma_addr_t dma_addr, size_t size, int direction);

/* ============================ rt_usb_util.c =============================== */
#ifdef RTMP_MAC_USB
void dump_urb(VOID *purb);

int rausb_register(VOID * new_driver);

void rausb_deregister(VOID * driver);

/*struct urb *rausb_alloc_urb(int iso_packets); */

void rausb_free_urb(VOID *urb);

void rausb_put_dev(VOID *dev);

struct usb_device *rausb_get_dev(VOID *dev);

int rausb_submit_urb(VOID *urb);

void *rausb_buffer_alloc(VOID *dev,
							size_t size,
							ra_dma_addr_t *dma);

void rausb_buffer_free(VOID *dev,
							size_t size,
							void *addr,
							ra_dma_addr_t dma);

int rausb_control_msg(VOID *dev,
						unsigned int pipe,
						__u8 request,
						__u8 requesttype,
						__u16 value,
						__u16 index,
						void *data,
						__u16 size,
						int timeout);

unsigned int rausb_sndctrlpipe(VOID *dev, unsigned long address);

unsigned int rausb_rcvctrlpipe(VOID *dev, unsigned long address);

void rausb_kill_urb(VOID *urb);

VOID RtmpOsUsbEmptyUrbCheck(
	IN	VOID				**ppWait,
	IN	NDIS_SPIN_LOCK		*pBulkInLock,
	IN	unsigned char				*pPendingRx);

typedef VOID (*USB_COMPLETE_HANDLER)(VOID *);

VOID	RtmpOsUsbInitHTTxDesc(
	IN	VOID			*pUrbSrc,
	IN	VOID			*pUsb_Dev,
	IN	unsigned int			BulkOutEpAddr,
	IN	unsigned char *			pSrc,
	IN	unsigned long			BulkOutSize,
	IN	USB_COMPLETE_HANDLER	Func,
	IN	VOID			*pTxContext,
	IN	ra_dma_addr_t		TransferDma);

VOID	RtmpOsUsbInitRxDesc(
	IN	VOID			*pUrbSrc,
	IN	VOID			*pUsb_Dev,
	IN	unsigned int			BulkInEpAddr,
	IN	unsigned char			*pTransferBuffer,
	IN	unsigned int			BufSize,
	IN	USB_COMPLETE_HANDLER	Func,
	IN	VOID			*pRxContext,
	IN	ra_dma_addr_t		TransferDma);

VOID *RtmpOsUsbContextGet(
	IN	VOID			*pUrb);

NTSTATUS RtmpOsUsbStatusGet(
	IN	VOID			*pUrb);

VOID RtmpOsUsbDmaMapping(
	IN	VOID			*pUrb);
#endif /* RTMP_MAC_USB */

#if defined(RTMP_RBUS_SUPPORT) || defined(RTMP_FLASH_SUPPORT)
void RtmpFlashRead(
	unsigned char * p,
	unsigned long a,
	unsigned long b);

void RtmpFlashWrite(
	unsigned char * p,
	unsigned long a,
	unsigned long b);
#endif /* defined(RTMP_RBUS_SUPPORT) || defined(RTMP_FLASH_SUPPORT) */

unsigned int RtmpOsGetUsbDevVendorID(
	IN VOID *pUsbDev);

unsigned int RtmpOsGetUsbDevProductID(
	IN VOID *pUsbDev);

/* CFG80211 */
#ifdef RT_CFG80211_SUPPORT
typedef struct __CFG80211_BAND {

	unsigned char RFICType;
	unsigned char MpduDensity;
	unsigned char TxStream;
	unsigned char RxStream;
	unsigned int MaxTxPwr;
	unsigned int MaxBssTable;

	unsigned short RtsThreshold;
	unsigned short FragmentThreshold;
	unsigned int RetryMaxCnt; /* bit0~7: short; bit8 ~ 15: long */
	bool FlgIsBMode;
} CFG80211_BAND;

VOID CFG80211OS_UnRegister(
	IN VOID						*pCB,
	IN VOID						*pNetDev);

bool CFG80211_SupBandInit(
	IN VOID						*pCB,
	IN CFG80211_BAND 			*pBandInfo,
	IN VOID						*pWiphyOrg,
	IN VOID						*pChannelsOrg,
	IN VOID						*pRatesOrg);

bool CFG80211OS_SupBandReInit(
	IN VOID						*pCB,
	IN CFG80211_BAND 			*pBandInfo);

VOID CFG80211OS_RegHint(
	IN VOID						*pCB,
	IN unsigned char					*pCountryIe,
	IN unsigned long					CountryIeLen);

VOID CFG80211OS_RegHint11D(
	IN VOID						*pCB,
	IN unsigned char					*pCountryIe,
	IN unsigned long					CountryIeLen);

bool CFG80211OS_BandInfoGet(
	IN VOID						*pCB,
	IN VOID						*pWiphyOrg,
	OUT VOID					**ppBand24,
	OUT VOID					**ppBand5);

unsigned int CFG80211OS_ChanNumGet(
	IN VOID						*pCB,
	IN VOID						*pWiphyOrg,
	IN unsigned int					IdBand);

bool CFG80211OS_ChanInfoGet(
	IN VOID						*pCB,
	IN VOID						*pWiphyOrg,
	IN unsigned int					IdBand,
	IN unsigned int					IdChan,
	OUT unsigned int					*pChanId,
	OUT unsigned int					*pPower,
	OUT bool					*pFlgIsRadar);

bool CFG80211OS_ChanInfoInit(
	IN VOID						*pCB,
	IN unsigned int					InfoIndex,
	IN unsigned char					ChanId,
	IN unsigned char					MaxTxPwr,
	IN bool					FlgIsNMode,
	IN bool					FlgIsBW20M);

VOID CFG80211OS_Scaning(
	IN VOID						*pCB,
	IN unsigned int					ChanId,
	IN unsigned char					*pFrame,
	IN unsigned int					FrameLen,
	IN int					RSSI,
	IN bool					FlgIsNMode,
	IN unsigned char					BW);

VOID CFG80211OS_ScanEnd(
	IN VOID						*pCB,
	IN bool					FlgIsAborted);

void CFG80211OS_ConnectResultInform(
	IN VOID						*pCB,
	IN unsigned char					*pBSSID,
	IN unsigned char					*pReqIe,
	IN unsigned int					ReqIeLen,
	IN unsigned char					*pRspIe,
	IN unsigned int					RspIeLen,
	IN unsigned char					FlgIsSuccess);
#endif /* RT_CFG80211_SUPPORT */




/* ================================ MACRO =================================== */
#define RTMP_UTIL_DCACHE_FLUSH(__AddrStart, __Size)

/* ================================ EXTERN ================================== */
extern unsigned char SNAP_802_1H[6];
extern unsigned char SNAP_BRIDGE_TUNNEL[6];
extern unsigned char EAPOL[2];
extern unsigned char TPID[];
extern unsigned char IPX[2];
extern unsigned char APPLE_TALK[2];
extern unsigned char NUM_BIT8[8];
extern unsigned long RTPktOffsetData, RTPktOffsetLen, RTPktOffsetCB;

extern unsigned long OS_NumOfMemAlloc, OS_NumOfMemFree;

extern int ralinkrate[];
extern unsigned int RT_RateSize;

#ifdef PLATFORM_UBM_IPX8
#include "vrut_ubm.h"
#endif /* PLATFORM_UBM_IPX8 */

int  RtPrivIoctlSetVal(VOID);

#endif /* __RT_OS_UTIL_H__ */
