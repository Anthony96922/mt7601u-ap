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
void RtmpUtilInit(void);

/* OS Time */
void RTMPusecDelay(
	IN	unsigned long					usec);

void RtmpOsUsDelay(unsigned long value);

void RtmpOsMsDelay(
	IN	unsigned long					msec);

void RTMP_GetCurrentSystemTime(
	IN	LARGE_INTEGER			*time);

void RTMP_GetCurrentSystemTick(
	IN	unsigned long					*pNow);

void RtmpOsWait(
	IN	unsigned int					Time);

unsigned int RtmpOsTimerAfter(
	IN	unsigned long					a,
	IN	unsigned long					b);

unsigned int RtmpOsTimerBefore(
	IN	unsigned long					a,
	IN	unsigned long					b);

void RtmpOsGetSystemUpTime(
	IN	unsigned long					*pTime);

unsigned int RtmpOsTickUnitGet(void);

/* OS Memory */
NDIS_STATUS os_alloc_mem(
	IN	void					*pReserved,
	OUT	unsigned char					**mem,
	IN	unsigned long					size);

NDIS_STATUS os_alloc_mem_suspend(
	IN	void					*pReserved,
	OUT	unsigned char					**mem,
	IN	unsigned long					size);

NDIS_STATUS os_free_mem(
	IN	void					*pReserved,
	IN	void *					mem);

NDIS_STATUS AdapterBlockAllocateMemory(
	IN	void *					handle,
	OUT	void *					*ppAd,
	IN	unsigned int					SizeOfpAd);

void *RtmpOsVmalloc(
	IN	unsigned long					Size);

void RtmpOsVfree(
	IN	void					*pMem);

unsigned long RtmpOsCopyFromUser(
	OUT	void					*to,
	IN	const void				*from,
	IN	unsigned long					n);

unsigned long RtmpOsCopyToUser(
	OUT void					*to,
	IN	const void				*from,
	IN	unsigned long					n);

bool RtmpOsStatsAlloc(
	IN	void					**ppStats,
	IN	void					**ppIwStats);

/* OS Packet */
PNDIS_PACKET RtmpOSNetPktAlloc(
	IN	void					*pReserved,
	IN	int						size);

PNDIS_PACKET RTMP_AllocateFragPacketBuffer(
	IN	void					*pReserved,
	IN	unsigned long					Length);

NDIS_STATUS RTMPAllocateNdisPacket(
	IN	void					*pReserved,
	OUT PNDIS_PACKET			*ppPacket,
	IN	unsigned char *					pHeader,
	IN	unsigned int					HeaderLen,
	IN	unsigned char *					pData,
	IN	unsigned int					DataLen);

void RTMPFreeNdisPacket(
	IN	void					*pReserved,
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
	IN	void					*pReserved,
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
			IN	void			*pCtrlBkPtr, 
			IN	PNDIS_PACKET	pPacket,
			IN	unsigned char			OpMode);

bool RTMPL2FrameTxAction(
	IN  void					*pCtrlBkPtr,
	IN	PNET_DEV				pNetDev,
	IN	RTMP_CB_8023_PACKET_ANNOUNCE _announce_802_3_packet,
	IN	unsigned char					apidx,
	IN	unsigned char *					pData,
	IN	unsigned int					data_len,
	IN	unsigned char			OpMode);

PNDIS_PACKET ExpandPacket(
	IN	void					*pReserved,
	IN	PNDIS_PACKET			pPacket,
	IN	unsigned int					ext_head_len,
	IN	unsigned int					ext_tail_len);

PNDIS_PACKET ClonePacket(
	IN	void					*pReserved,
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
void RtmpOsSetPacket(
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

void RtmpOsPktBodyCopy(
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

void RtmpOsPktDataPtrAssign(
	IN	PNDIS_PACKET			pNetPkt,
	IN	unsigned char					*pData);

void RtmpOsPktLenAssign(
	IN	PNDIS_PACKET			pNetPkt,
	IN	long					Len);

void RtmpOsPktTailAdjust(
	IN	PNDIS_PACKET			pNetPkt,
	IN	unsigned int					removedTagLen);

unsigned char * RtmpOsPktTailBufExtend(
	IN	PNDIS_PACKET			pNetPkt,
	IN	unsigned int					Len);

unsigned char * RtmpOsPktHeadBufExtend(
	IN	PNDIS_PACKET			pNetPkt,
	IN	unsigned int					Len);

void RtmpOsPktReserve(
	IN	PNDIS_PACKET			pNetPkt,
	IN	unsigned int					Len);

void RtmpOsPktProtocolAssign(
	IN	PNDIS_PACKET			pNetPkt);

void RtmpOsPktInfPpaSend(
	IN	PNDIS_PACKET			pNetPkt);

void RtmpOsPktRcvHandle(
	IN	PNDIS_PACKET			pNetPkt);

void RtmpOsPktNatMagicTag(
	IN	PNDIS_PACKET			pNetPkt);

void RtmpOsPktNatNone(
	IN	PNDIS_PACKET			pNetPkt);

void RtmpOsPktInit(
	IN	PNDIS_PACKET			pNetPkt,
	IN	PNET_DEV				pNetDev,
	IN	unsigned char					*pData,
	IN	unsigned short					DataSize);

PNDIS_PACKET RtmpOsPktIappMakeUp(
	IN	PNET_DEV				pNetDev,
	IN	unsigned char					*pMac);

bool RtmpOsPktOffsetInit(void);

unsigned short RtmpOsNtohs(
	IN	unsigned short					Value);

unsigned short RtmpOsHtons(
	IN	unsigned short					Value);

unsigned int RtmpOsNtohl(
	IN	unsigned int					Value);

unsigned int RtmpOsHtonl(
	IN	unsigned int					Value);

/* OS File */
RTMP_OS_FD RtmpOSFileOpen(char *pPath,  int flag, int mode);
int RtmpOSFileClose(RTMP_OS_FD osfd);
void RtmpOSFileSeek(RTMP_OS_FD osfd, int offset);
int RtmpOSFileRead(RTMP_OS_FD osfd, char *pDataPtr, int readLen);
int RtmpOSFileWrite(RTMP_OS_FD osfd, char *pDataPtr, int writeLen);

int RtmpOsFileIsErr(
	IN	void					*pFile);

void RtmpOSFSInfoChange(
	IN	RTMP_OS_FS_INFO			*pOSFSInfoOrg,
	IN	bool					bSet);

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
	IN	void					*pReserved,
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
	IN	void					*pDev);

unsigned char *RtmpOsNetDevGetPhyAddr(
	IN	void					*pDev);

void RtmpOsNetQueueStart(
	IN	PNET_DEV				pDev);

void RtmpOsNetQueueStop(
	IN	PNET_DEV				pDev);

void RtmpOsNetQueueWake(
	IN	PNET_DEV				pDev);

void RtmpOsSetPktNetDev(
	IN	void					*pPkt,
	IN	void					*pDev);

PNET_DEV RtmpOsPktNetDevGet(
	IN	void					*pPkt);

char *RtmpOsGetNetDevName(
	IN	void					*pDev);

void RtmpOsSetNetDevPriv(
	IN	void					*pDev,
	IN	void					*pPriv);

void *RtmpOsGetNetDevPriv(
	IN	void					*pDev);

unsigned short RtmpDevPrivFlagsGet(
	IN	void					*pDev);

void RtmpDevPrivFlagsSet(
	IN	void					*pDev,
	IN	unsigned short					PrivFlags);

void RtmpOsSetNetDevType(void *pDev, unsigned short Type);

void RtmpOsSetNetDevTypeMonitor(void *pDev);


/* OS Semaphore */
void RtmpOsCmdUp(RTMP_OS_TASK *pCmdQTask);
bool RtmpOsSemaInitLocked(RTMP_OS_SEM *pSemOrg, LIST_HEADER *pSemList);
bool RtmpOsSemaInit(RTMP_OS_SEM *pSemOrg, LIST_HEADER *pSemList);
bool RtmpOsSemaDestory(RTMP_OS_SEM *pSemOrg);
INT RtmpOsSemaWaitInterruptible(RTMP_OS_SEM *pSemOrg);
void RtmpOsSemaWakeUp(RTMP_OS_SEM *pSemOrg);
void RtmpOsMlmeUp(RTMP_OS_TASK *pMlmeQTask);

/* OS Task */
bool RtmpOsTaskletSche(RTMP_NET_TASK_STRUCT *pTasklet);

bool RtmpOsTaskletInit(
	RTMP_NET_TASK_STRUCT *pTasklet,
	void (*pFunc)(unsigned long data),
	unsigned long Data,
	LIST_HEADER *pTaskletList);

bool RtmpOsTaskletKill(RTMP_NET_TASK_STRUCT *pTasklet);

void RtmpOsTaskletDataAssign(
	RTMP_NET_TASK_STRUCT *pTasklet,
	unsigned long Data);

void RtmpOsTaskWakeUp(RTMP_OS_TASK *pTaskOrg);

int RtmpOsTaskIsKilled(RTMP_OS_TASK *pTaskOrg);

bool RtmpOsCheckTaskLegality(RTMP_OS_TASK *pTaskOrg);

bool RtmpOSTaskAlloc(
	IN	RTMP_OS_TASK			*pTask,
	IN	LIST_HEADER				*pTaskList);

void RtmpOSTaskFree(
	IN	RTMP_OS_TASK			*pTask);

NDIS_STATUS RtmpOSTaskKill(
	IN	RTMP_OS_TASK			*pTaskOrg);

INT RtmpOSTaskNotifyToExit(
	IN	RTMP_OS_TASK			*pTaskOrg);

void RtmpOSTaskCustomize(
	IN	RTMP_OS_TASK			*pTaskOrg);

NDIS_STATUS RtmpOSTaskAttach(
	IN	RTMP_OS_TASK			*pTaskOrg,
	IN	RTMP_OS_TASK_CALLBACK	fn,
	IN	unsigned long					arg);

NDIS_STATUS RtmpOSTaskInit(
	IN	RTMP_OS_TASK			*pTaskOrg,
	IN	char *					pTaskName,
	IN	void					*pPriv,
	IN	LIST_HEADER				*pTaskList,
	IN	LIST_HEADER				*pSemList);

bool RtmpOSTaskWait(
	IN	void					*pReserved,
	IN	RTMP_OS_TASK			*pTaskOrg,
	IN	int					*pStatus);

void *RtmpOsTaskDataGet(RTMP_OS_TASK *pTaskOrg);

int RtmpThreadPidKill(RTMP_OS_PID	 PID);

/* OS Cache */
void RtmpOsDCacheFlush(unsigned long AddrStart, unsigned long Size);

/* OS Timer */
void RTMP_SetPeriodicTimer(
	IN	NDIS_MINIPORT_TIMER *pTimerOrg, 
	IN	unsigned long timeout);

void RTMP_OS_Init_Timer(
	IN	void 					*pReserved,
	IN	NDIS_MINIPORT_TIMER		*pTimerOrg, 
	IN	TIMER_FUNCTION			function,
	IN	void *					data,
	IN	LIST_HEADER				*pTimerList);

void RTMP_OS_Add_Timer(
	IN	NDIS_MINIPORT_TIMER *pTimerOrg,
	IN	unsigned long timeout);

void RTMP_OS_Mod_Timer(
	IN	NDIS_MINIPORT_TIMER *pTimerOrg,
	IN	unsigned long timeout);

void RTMP_OS_Del_Timer(
	IN	NDIS_MINIPORT_TIMER		*pTimerOrg,
	OUT	bool					*pCancelled);

void RTMP_OS_Release_Timer(
	IN	NDIS_MINIPORT_TIMER		*pTimerOrg);

bool RTMP_OS_Alloc_Rsc(
	IN	LIST_HEADER				*pRscList,
	IN	void 					*pRsc,
	IN	unsigned int					RscLen);

void RTMP_OS_Free_Rscs(
	IN	LIST_HEADER				*pRscList);

/* OS Lock */
bool RtmpOsAllocateLock(
	IN	NDIS_SPIN_LOCK			*pLock,
	IN	LIST_HEADER				*pLockList);

void RtmpOsFreeSpinLock(
	IN	NDIS_SPIN_LOCK			*pLockOrg);

void RtmpOsSpinLockBh(
	IN	NDIS_SPIN_LOCK			*pLockOrg);

void RtmpOsSpinUnLockBh(NDIS_SPIN_LOCK *pLockOrg);
void RtmpOsIntLock(NDIS_SPIN_LOCK *pLockOrg, unsigned long *pIrqFlags);
void RtmpOsIntUnLock(NDIS_SPIN_LOCK *pLockOrg, unsigned long IrqFlags);

/* OS PID */
void RtmpOsGetPid(unsigned long *pDst, unsigned long PID);
void RtmpOsTaskPidInit(RTMP_OS_PID *pPid);

/* OS I/O */
void RTMP_PCI_Writel(unsigned long Value, void *pAddr);
void RTMP_PCI_Writew(unsigned long Value, void *pAddr);
void RTMP_PCI_Writeb(unsigned long Value, void *pAddr);
unsigned long RTMP_PCI_Readl(void *pAddr);
unsigned long RTMP_PCI_Readw(void *pAddr);
unsigned long RTMP_PCI_Readb(void *pAddr);

int RtmpOsPciConfigReadWord(
	IN	void					*pDev,
	IN	unsigned int					Offset,
	OUT unsigned short					*pValue);

int RtmpOsPciConfigWriteWord(
	IN	void					*pDev,
	IN	unsigned int					Offset,
	IN	unsigned short					Value);

int RtmpOsPciConfigReadDWord(
	IN	void					*pDev,
	IN	unsigned int					Offset,
	OUT unsigned int					*pValue);

int RtmpOsPciConfigWriteDWord(
	IN	void					*pDev,
	IN	unsigned int					Offset,
	IN	unsigned int					Value);

int RtmpOsPciFindCapability(
	IN	void					*pDev,
	IN	int						Cap);

void *RTMPFindHostPCIDev(void *pPciDevSrc);

int RtmpOsPciMsiEnable(void *pDev);
void RtmpOsPciMsiDisable(void *pDev);

/* OS Wireless */
unsigned long RtmpOsMaxScanDataGet(void);

/* OS Interrutp */
int RtmpOsIsInInterrupt(void);

/* OS USB */
void *RtmpOsUsbUrbDataGet(void *pUrb);
NTSTATUS RtmpOsUsbUrbStatusGet(void *pUrb);
unsigned long RtmpOsUsbUrbLenGet(void *pUrb);

/* OS Atomic */
bool RtmpOsAtomicInit(RTMP_OS_ATOMIC *pAtomic, LIST_HEADER *pAtomicList);
void RtmpOsAtomicDestroy(RTMP_OS_ATOMIC *pAtomic);
long RtmpOsAtomicRead(RTMP_OS_ATOMIC *pAtomic);
void RtmpOsAtomicDec(RTMP_OS_ATOMIC *pAtomic);
void RtmpOsAtomicInterlockedExchange(RTMP_OS_ATOMIC *pAtomicSrc, long Value);

/* OS Utility */
void hex_dump(char *str, unsigned char * pSrcBufVA, unsigned int SrcBufLen);

typedef void (*RTMP_OS_SEND_WLAN_EVENT)(
	IN	void					*pAdSrc,
	IN	unsigned short					Event_flag,
	IN	unsigned char * 					pAddr,
	IN  unsigned char					BssIdx,
	IN	char					Rssi);

void RtmpOsSendWirelessEvent(
	IN	void			*pAd,
	IN	unsigned short			Event_flag,
	IN	unsigned char * 			pAddr,
	IN	unsigned char			BssIdx,
	IN	char			Rssi,
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

unsigned int RtmpOsWirelessExtVerGet(void);

#ifdef DBG
void RtmpDrvAllMacPrint(
	IN void						*pReserved,
	IN unsigned int					*pBufMac,
	IN unsigned int					AddrStart,
	IN unsigned int					AddrEnd,
	IN unsigned int					AddrStep);
#endif /* DBG */

void RtmpDrvAllE2PPrint(
	IN	void					*pReserved,
	IN	unsigned short					*pMacContent,
	IN	unsigned int					AddrEnd,
	IN	unsigned int					AddrStep);

#ifdef DBG
void RtmpDrvAllRFPrint(
	IN void *pReserved,
	IN unsigned int *pBuf,
	IN unsigned int BufLen);
#endif /* DBG */

int RtmpOSIRQRelease(
	IN	PNET_DEV				pNetDev,
	IN	unsigned int					infType,
	IN	PPCI_DEV				pci_dev,
	IN	bool					*pHaveMsi);

void RtmpOsWlanEventSet(
	IN	void					*pReserved,
	IN	bool					*pCfgWEnt,
	IN	bool					FlgIsWEntSup);

unsigned short RtmpOsGetUnaligned(unsigned short *pWord);

unsigned int RtmpOsGetUnaligned32(unsigned int *pWord);

unsigned long RtmpOsGetUnalignedlong(unsigned long *pWord);

long RtmpOsSimpleStrtol(
	IN	const char				*cp,
	IN	char 					**endp,
	IN	unsigned int			base);

void RtmpOsOpsInit(RTMP_OS_ABL_OPS *pOps);

/* ============================ rt_os_util.c ================================ */
void RtmpDrvMaxRateGet(
	IN void *pReserved,
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

void RtmpMeshDown(
	IN void *pDrvCtrlBK,
	IN bool WaitFlag,
	IN bool (*RtmpMeshLinkCheck)(IN void *pAd));

unsigned short RtmpOsNetPrivGet(PNET_DEV pDev);

bool RtmpOsCmdDisplayLenCheck(
	IN	unsigned int					LenSrc,
	IN	unsigned int					Offset);

void    WpaSendMicFailureToWpaSupplicant(
	IN	PNET_DEV				pNetDev,
    IN  bool					bUnicast);

int wext_notify_event_assoc(
	IN	PNET_DEV				pNetDev,
	IN	unsigned char					*ReqVarIEs,
	IN	unsigned int					ReqVarIELen);

void    SendAssocIEsToWpaSupplicant( 
	IN	PNET_DEV				pNetDev,
	IN	unsigned char					*ReqVarIEs,
	IN	unsigned int					ReqVarIELen);

void * RtmpInitCompletion(void);

unsigned long RtmpWaitForCompletionTimeout(void *Completion, unsigned long Expire);

void RtmpComplete(void *Completion);

unsigned long RtmpMsecsToJiffies(unsigned int msecs);
/* ============================ rt_rbus_pci_util.c ========================== */
void RtmpAllocDescBuf(
	IN PPCI_DEV pPciDev,
	IN unsigned int Index,
	IN unsigned long Length,
	IN bool Cached,
	OUT void **VirtualAddress,
	OUT PNDIS_PHYSICAL_ADDRESS	PhysicalAddress);

void RtmpFreeDescBuf(
	IN PPCI_DEV pPciDev,
	IN unsigned long Length,
	IN void *VirtualAddress,
	IN NDIS_PHYSICAL_ADDRESS	PhysicalAddress);

void RTMP_AllocateFirstTxBuffer(
	IN PPCI_DEV pPciDev,
	IN unsigned int Index,
	IN unsigned long Length,
	IN bool Cached,
	OUT void **VirtualAddress,
	OUT PNDIS_PHYSICAL_ADDRESS	PhysicalAddress);

void RTMP_FreeFirstTxBuffer(
	IN	PPCI_DEV				pPciDev,
	IN	unsigned long					Length,
	IN	bool					Cached,
	IN	void *					VirtualAddress,
	IN	NDIS_PHYSICAL_ADDRESS	PhysicalAddress);

PNDIS_PACKET RTMP_AllocateRxPacketBuffer(
	IN	void					*pReserved,
	IN	void					*pPciDev,
	IN	unsigned long					Length,
	IN	bool					Cached,
	OUT	void *					*VirtualAddress,
	OUT	PNDIS_PHYSICAL_ADDRESS	PhysicalAddress);


int rausb_autopm_put_interface (
	IN	void			*intfsrc);

int rausb_autopm_get_interface (
	IN	void			*intfsrc);

ra_dma_addr_t linux_pci_map_single(void *pPciDev, void *ptr, size_t size, int sd_idx, int direction);

void linux_pci_unmap_single(void *pPciDev, ra_dma_addr_t dma_addr, size_t size, int direction);

/* ============================ rt_usb_util.c =============================== */
#ifdef RTMP_MAC_USB
void dump_urb(void *purb);

int rausb_register(void * new_driver);

void rausb_deregister(void * driver);

/*struct urb *rausb_alloc_urb(int iso_packets); */

void rausb_free_urb(void *urb);

void rausb_put_dev(void *dev);

struct usb_device *rausb_get_dev(void *dev);

int rausb_submit_urb(void *urb);

void *rausb_buffer_alloc(void *dev,
							size_t size,
							ra_dma_addr_t *dma);

void rausb_buffer_free(void *dev,
							size_t size,
							void *addr,
							ra_dma_addr_t dma);

int rausb_control_msg(void *dev,
						unsigned int pipe,
						__u8 request,
						__u8 requesttype,
						__u16 value,
						__u16 index,
						void *data,
						__u16 size,
						int timeout);

unsigned int rausb_sndctrlpipe(void *dev, unsigned long address);

unsigned int rausb_rcvctrlpipe(void *dev, unsigned long address);

void rausb_kill_urb(void *urb);

void RtmpOsUsbEmptyUrbCheck(
	IN	void				**ppWait,
	IN	NDIS_SPIN_LOCK		*pBulkInLock,
	IN	unsigned char				*pPendingRx);

typedef void (*USB_COMPLETE_HANDLER)(void *);

void	RtmpOsUsbInitHTTxDesc(
	IN	void			*pUrbSrc,
	IN	void			*pUsb_Dev,
	IN	unsigned int			BulkOutEpAddr,
	IN	unsigned char *			pSrc,
	IN	unsigned long			BulkOutSize,
	IN	USB_COMPLETE_HANDLER	Func,
	IN	void			*pTxContext,
	IN	ra_dma_addr_t		TransferDma);

void	RtmpOsUsbInitRxDesc(
	IN	void			*pUrbSrc,
	IN	void			*pUsb_Dev,
	IN	unsigned int			BulkInEpAddr,
	IN	unsigned char			*pTransferBuffer,
	IN	unsigned int			BufSize,
	IN	USB_COMPLETE_HANDLER	Func,
	IN	void			*pRxContext,
	IN	ra_dma_addr_t		TransferDma);

void *RtmpOsUsbContextGet(
	IN	void			*pUrb);

NTSTATUS RtmpOsUsbStatusGet(
	IN	void			*pUrb);

void RtmpOsUsbDmaMapping(
	IN	void			*pUrb);
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
	IN void *pUsbDev);

unsigned int RtmpOsGetUsbDevProductID(
	IN void *pUsbDev);

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

void CFG80211OS_UnRegister(
	IN void						*pCB,
	IN void						*pNetDev);

bool CFG80211_SupBandInit(
	IN void						*pCB,
	IN CFG80211_BAND 			*pBandInfo,
	IN void						*pWiphyOrg,
	IN void						*pChannelsOrg,
	IN void						*pRatesOrg);

bool CFG80211OS_SupBandReInit(
	IN void						*pCB,
	IN CFG80211_BAND 			*pBandInfo);

void CFG80211OS_RegHint(
	IN void						*pCB,
	IN unsigned char					*pCountryIe,
	IN unsigned long					CountryIeLen);

void CFG80211OS_RegHint11D(
	IN void						*pCB,
	IN unsigned char					*pCountryIe,
	IN unsigned long					CountryIeLen);

bool CFG80211OS_BandInfoGet(
	IN void						*pCB,
	IN void						*pWiphyOrg,
	OUT void					**ppBand24,
	OUT void					**ppBand5);

unsigned int CFG80211OS_ChanNumGet(
	IN void						*pCB,
	IN void						*pWiphyOrg,
	IN unsigned int					IdBand);

bool CFG80211OS_ChanInfoGet(
	IN void						*pCB,
	IN void						*pWiphyOrg,
	IN unsigned int					IdBand,
	IN unsigned int					IdChan,
	OUT unsigned int					*pChanId,
	OUT unsigned int					*pPower,
	OUT bool					*pFlgIsRadar);

bool CFG80211OS_ChanInfoInit(
	IN void						*pCB,
	IN unsigned int					InfoIndex,
	IN unsigned char					ChanId,
	IN unsigned char					MaxTxPwr,
	IN bool					FlgIsNMode,
	IN bool					FlgIsBW20M);

void CFG80211OS_Scaning(
	IN void						*pCB,
	IN unsigned int					ChanId,
	IN unsigned char					*pFrame,
	IN unsigned int					FrameLen,
	IN int					RSSI,
	IN bool					FlgIsNMode,
	IN unsigned char					BW);

void CFG80211OS_ScanEnd(
	IN void						*pCB,
	IN bool					FlgIsAborted);

void CFG80211OS_ConnectResultInform(
	IN void						*pCB,
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

int  RtPrivIoctlSetVal(void);

#endif /* __RT_OS_UTIL_H__ */
