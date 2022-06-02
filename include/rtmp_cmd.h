#ifndef __RTMP_CMD_H__
#define __RTMP_CMD_H__

#include "rtmp_type.h"

typedef struct _CmdQElmt {
	unsigned int command;
	void * buffer;
	unsigned long bufferlength;
	bool CmdFromNdis;
	bool SetOperation;
	struct _CmdQElmt *next;
} CmdQElmt, *PCmdQElmt;

typedef struct _CmdQ {
	unsigned int size;
	CmdQElmt *head;
	CmdQElmt *tail;
	unsigned int CmdQState;
} CmdQ, *PCmdQ;

#define EnqueueCmd(cmdq, cmdqelmt)		\
{										\
	if (cmdq->size == 0)				\
		cmdq->head = cmdqelmt;			\
	else								\
		cmdq->tail->next = cmdqelmt;	\
	cmdq->tail = cmdqelmt;				\
	cmdqelmt->next = NULL;				\
	cmdq->size++;						\
}

#define NDIS_OID	unsigned int

/* OS_RTCMDUp is only used in UTIL/NETIF module */
#define OS_RTCMDUp						RtmpOsCmdUp



/* RALINK command status code */
#define RTMP_IO_EINVAL							30000
#define RTMP_IO_EOPNOTSUPP						30001
#define RTMP_IO_EFAULT							30002
#define RTMP_IO_ENETDOWN						30003
#define RTMP_IO_E2BIG							30004
#define RTMP_IO_ENOMEM							30005
#define RTMP_IO_EAGAIN							30006
#define RTMP_IO_ENOTCONN						30007

enum {
#ifdef MAT_SUPPORT
	SHOW_IPV4_MAT_INFO = 1,
	SHOW_IPV6_MAT_INFO = 2,
	SHOW_ETH_CLONE_MAC = 3,
#endif /* MAT_SUPPORT */
	SHOW_CONN_STATUS = 4,
	SHOW_DRVIER_VERION = 5,
	SHOW_BA_INFO = 6,
	SHOW_DESC_INFO = 7,
#ifdef RTMP_MAC_USB
	SHOW_RXBULK_INFO = 8,
	SHOW_TXBULK_INFO = 9,
#endif /* RTMP_MAC_USB */
	RAIO_OFF = 10,
	RAIO_ON = 11,
#ifdef QOS_DLS_SUPPORT
	SHOW_DLS_ENTRY_INFO = 20,
#endif /* QOS_DLS_SUPPORT */
	SHOW_CFG_VALUE = 21,
	SHOW_ADHOC_ENTRY_INFO = 22,
	SHOW_DEV_INFO = 26,
	SHOW_STA_INFO = 27,
};


/* RALINK command handle ID */
/* ap commands */
typedef enum _CMD_RTPRIV_IOCTL_AP {

	/* general */
	CMD_RTPRIV_IOCTL_SET_WSCOOB = 0x0001,
	CMD_RTPRIV_IOCTL_GET_MAC_TABLE,
	CMD_RTPRIV_IOCTL_GSITESURVEY,
	CMD_RTPRIV_IOCTL_STATISTICS,
	CMD_RTPRIV_IOCTL_QUERY_BATABLE,
	CMD_RTPRIV_IOCTL_E2P,
	CMD_RTPRIV_IOCTL_BBP,
	CMD_RTPRIV_IOCTL_MAC,
	CMD_RTPRIV_IOCTL_RF,
	CMD_RT_PRIV_IOCTL,
	CMD_RTPRIV_IOCTL_SET,
	CMD_RTPRIV_IOCTL_SHOW,
	CMD_RTPRIV_IOCTL_GET_AR9_SHOW,
	CMD_RTPRIV_IOCTL_ATE,
	CMD_RTPRIV_IOCTL_CHID_2_FREQ,
	CMD_RTPRIV_IOCTL_FREQ_2_CHID,
	CMD_RTPRIV_IOCTL_GET_MAC_TABLE_STRUCT,

	/* mbss */
	CMD_RTPRIV_IOCTL_MBSS_BEACON_UPDATE,
	CMD_RTPRIV_IOCTL_MBSS_OPEN,
	CMD_RTPRIV_IOCTL_MBSS_CLOSE,
	CMD_RTPRIV_IOCTL_MBSS_INIT,
	CMD_RTPRIV_IOCTL_MBSS_REMOVE,

	/* wsc */
	CMD_RTPRIV_IOCTL_WSC_PROFILE,
	CMD_RTPRIV_IOCTL_WSC_INIT,

	/* apc */
	CMD_RTPRIV_IOCTL_APC_UP,
	CMD_RTPRIV_IOCTL_APC_DISCONNECT,
	CMD_RTPRIV_IOCTL_APC_INIT,
	CMD_RTPRIV_IOCTL_APC_OPEN,
	CMD_RTPRIV_IOCTL_APC_CLOSE,
	CMD_RTPRIV_IOCTL_APC_REMOVE,

	/* interface */
	CMD_RTPRIV_IOCTL_MAIN_OPEN,

	/* ioctl */
	CMD_RTPRIV_IOCTL_PREPARE,
	CMD_RTPRIV_IOCTL_AP_SIOCGIWAP,
	CMD_RTPRIV_IOCTL_AP_SIOCGIFHWADDR,
	CMD_RTPRIV_IOCTL_AP_SIOCGIWESSID,
	CMD_RTPRIV_IOCTL_AP_SIOCGIWRATEQ,
	CMD_RTPRIV_IOCTL_AP_SIOCSIWGENIE,

	/* can not exceed 0x5000 */
} CMD_RTPRIV_IOCTL_AP;

/* common commands */
typedef enum _CMD_RTPRIV_IOCTL_COMMON {

	/* general */
	CMD_RTPRIV_IOCTL_NETDEV_GET = 0x5000,
	CMD_RTPRIV_IOCTL_NETDEV_SET,
	CMD_RTPRIV_IOCTL_OPMODE_GET,
	CMD_RTPRIV_IOCTL_TASK_LIST_GET,
	CMD_RTPRIV_IOCTL_IRQ_INIT,
	CMD_RTPRIV_IOCTL_IRQ_RELEASE,
	CMD_RTPRIV_IOCTL_MSI_ENABLE,
	CMD_RTPRIV_IOCTL_NIC_NOT_EXIST,
	CMD_RTPRIV_IOCTL_MCU_SLEEP_CLEAR,
#ifdef CONFIG_APSTA_MIXED_SUPPORT
	CMD_RTPRIV_IOCTL_MAX_IN_BIT,
#endif /* CONFIG_APSTA_MIXED_SUPPORT */
	CMD_RTPRIV_IOCTL_ADAPTER_SUSPEND_SET,
	CMD_RTPRIV_IOCTL_ADAPTER_SUSPEND_CLEAR,
	CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_USB_ASICRADIO_OFF,
	CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_USB_ASICRADIO_ON,
	CMD_RTPRIV_IOCTL_SANITY_CHECK,
	CMD_RTPRIV_IOCTL_SANITY_CHECK_ON_SET_CMD,	
#ifdef EXT_BUILD_CHANNEL_LIST
	CMD_RTPRIV_SET_PRECONFIG_VALUE,
#endif /* EXT_BUILD_CHANNEL_LIST */


	/* mesh */
	CMD_RTPRIV_IOCTL_MESH_INIT,
	CMD_RTPRIV_IOCTL_MESH_REMOVE,
	CMD_RTPRIV_IOCTL_MESH_OPEN_PRE,
	CMD_RTPRIV_IOCTL_MESH_OPEN_POST,
	CMD_RTPRIV_IOCTL_MESH_IS_VALID,
	CMD_RTPRIV_IOCTL_MESH_CLOSE,

	/* p2p */
	CMD_RTPRIV_IOCTL_P2P_INIT,
	CMD_RTPRIV_IOCTL_P2P_REMOVE,
	CMD_RTPRIV_IOCTL_P2P_OPEN_PRE,
	CMD_RTPRIV_IOCTL_P2P_OPEN_POST,
	CMD_RTPRIV_IOCTL_P2P_IS_VALID,
	CMD_RTPRIV_IOCTL_P2P_CLOSE,

	/* usb */
	CMD_RTPRIV_IOCTL_USB_MORE_FLAG_SET,
	CMD_RTPRIV_IOCTL_USB_CONFIG_INIT,
	CMD_RTPRIV_IOCTL_USB_SUSPEND,
	CMD_RTPRIV_IOCTL_USB_RESUME,

	/* pci */
	CMD_RTPRIV_IOCTL_PCI_SUSPEND,
	CMD_RTPRIV_IOCTL_PCI_RESUME,
	CMD_RTPRIV_IOCTL_PCI_CSR_SET,
	CMD_RTPRIV_IOCTL_PCIE_INIT,

	/* cfg80211 */
	CMD_RTPRIV_IOCTL_CFG80211_CFG_START,

	/* inf ppa */
	CMD_RTPRIV_IOCTL_INF_PPA_INIT,
	CMD_RTPRIV_IOCTL_INF_PPA_EXIT,

	/* wireless */
	CMD_RTPRIV_IOCTL_BEACON_UPDATE,
	CMD_RTPRIV_IOCTL_RXPATH_GET,
	CMD_RTPRIV_IOCTL_CHAN_LIST_NUM_GET,
	CMD_RTPRIV_IOCTL_CHAN_LIST_GET,
	CMD_RTPRIV_IOCTL_FREQ_LIST_GET,

	/* interface */
	CMD_RTPRIV_IOCTL_VIRTUAL_INF_UP,
	CMD_RTPRIV_IOCTL_VIRTUAL_INF_DOWN,
	CMD_RTPRIV_IOCTL_VIRTUAL_INF_GET,
	CMD_RTPRIV_IOCTL_INF_TYPE_GET,
	CMD_RTPRIV_IOCTL_INF_STATS_GET,
	CMD_RTPRIV_IOCTL_INF_IW_STATUS_GET,
	CMD_RTPRIV_IOCTL_INF_MAIN_CREATE,
	CMD_RTPRIV_IOCTL_INF_MAIN_ID_GET,
	CMD_RTPRIV_IOCTL_INF_MAIN_CHECK,
	CMD_RTPRIV_IOCTL_INF_P2P_CHECK,

	/* ioctl */
	CMD_RTPRIV_IOCTL_SIOCGIWFREQ,
	CMD_RTPRIV_IOCTL_SIOCGIWNAME,

	/* wds */
	CMD_RTPRIV_IOCTL_WDS_INIT,
	CMD_RTPRIV_IOCTL_WDS_REMOVE,
	CMD_RTPRIV_IOCTL_WDS_STATS_GET,

	CMD_RTPRIV_IOCTL_MAC_ADDR_GET,

#ifdef RT_CFG80211_SUPPORT
	/* cfg802.11 */
	/* Note: All cfg commands must be continue. */
	CMD_RTPRIV_IOCTL_80211_START,
	CMD_RTPRIV_IOCTL_80211_CB_GET,
	CMD_RTPRIV_IOCTL_80211_CB_SET,
	CMD_RTPRIV_IOCTL_80211_CHAN_SET,
	CMD_RTPRIV_IOCTL_80211_VIF_CHG,
	CMD_RTPRIV_IOCTL_80211_SCAN,
	CMD_RTPRIV_IOCTL_80211_IBSS_JOIN,
	CMD_RTPRIV_IOCTL_80211_STA_LEAVE,
	CMD_RTPRIV_IOCTL_80211_STA_GET,
	CMD_RTPRIV_IOCTL_80211_KEY_ADD,
	CMD_RTPRIV_IOCTL_80211_KEY_DEFAULT_SET,
	CMD_RTPRIV_IOCTL_80211_CONNECT_TO,
	CMD_RTPRIV_IOCTL_80211_RFKILL,
	CMD_RTPRIV_IOCTL_80211_REG_NOTIFY_TO,
	CMD_RTPRIV_IOCTL_80211_UNREGISTER,
	CMD_RTPRIV_IOCTL_80211_BANDINFO_GET,
	CMD_RTPRIV_IOCTL_80211_SURVEY_GET,
	CMD_RTPRIV_IOCTL_80211_EXTRA_IES_SET,
	CMD_RTPRIV_IOCTL_80211_REMAIN_ON_CHAN_SET,
	CMD_RTPRIV_IOCTL_80211_MGMT_FRAME_REG,
	CMD_RTPRIV_IOCTL_80211_CHANNEL_LOCK,
	CMD_RTPRIV_IOCTL_80211_MGMT_FRAME_SEND,
	CMD_RTPRIV_IOCTL_80211_REMAIN_ON_CHAN_DUR_TIMER_INIT,
	CMD_RTPRIV_IOCTL_80211_CHANNEL_LIST_SET,
	CMD_RTPRIV_IOCTL_80211_ACTION_FRAME_REG,
	CMD_RTPRIV_IOCTL_80211_BEACON_ADD,
	CMD_RTPRIV_IOCTL_80211_BEACON_SET,
	CMD_RTPRIV_IOCTL_80211_BEACON_DEL,
	CMD_RTPRIV_IOCTL_80211_AP_KEY_ADD,
	CMD_RTPRIV_IOCTL_80211_CHANGE_BSS_PARM,
	CMD_RTPRIV_IOCTL_80211_AP_KEY_DEL,
	CMD_RTPRIV_IOCTL_80211_AP_PROBE_RSP,
	CMD_RTPRIV_IOCTL_80211_PORT_SECURED,
	CMD_RTPRIV_IOCTL_80211_AP_STA_DEL,
	CMD_RTPRIV_IOCTL_80211_CANCEL_REMAIN_ON_CHAN_SET,
	CMD_RTPRIV_IOCTL_80211_BITRATE_SET,
#ifdef RT_P2P_SPECIFIC_WIRELESS_EVENT
	CMD_RTPRIV_IOCTL_80211_SEND_WIRELESS_EVENT,
#endif /* RT_P2P_SPECIFIC_WIRELESS_EVENT */
	CMD_RTPRIV_IOCTL_80211_END,
#endif /* RT_CFG80211_SUPPORT */

	CMD_RTPRIV_IOCTL_ADAPTER_CSO_SUPPORT_TEST,
	CMD_RTPRIV_IOCTL_ADAPTER_TSO_SUPPORT_TEST,

	/* can not exceed 0xa000 */
	CMD_RTPRIV_IOCTL_80211_COM_LATEST_ONE,
} CMD_RTPRIV_IOCTL_COMMON;

#ifdef RT_CFG80211_SUPPORT
typedef struct __CMD_RTPRIV_IOCTL_80211_CHAN {

	unsigned char ChanId;

#define RT_CMD_80211_IFTYPE_STATION		0x00
#define RT_CMD_80211_IFTYPE_ADHOC		0x01
#define RT_CMD_80211_IFTYPE_MONITOR		0x02
	unsigned char IfType;

#define RT_CMD_80211_CHANTYPE_NOHT		0x00
#define RT_CMD_80211_CHANTYPE_HT20		0x01
#define RT_CMD_80211_CHANTYPE_HT40MINUS	0X02
#define RT_CMD_80211_CHANTYPE_HT40PLUS	0X03
	unsigned char ChanType;

	unsigned int MonFilterFlag;
} CMD_RTPRIV_IOCTL_80211_CHAN;

#define RT_CMD_80211_FILTER_FCSFAIL		0x01
#define RT_CMD_80211_FILTER_PLCPFAIL	0x02
#define RT_CMD_80211_FILTER_CONTROL		0x04
#define RT_CMD_80211_FILTER_OTHER_BSS	0x08

typedef struct __CMD_RTPRIV_IOCTL_80211_IBSS {

	unsigned int BeaconInterval;
	unsigned char *pSsid;
} CMD_RTPRIV_IOCTL_80211_IBSS;

typedef struct __CMD_RTPRIV_IOCTL_80211_STA {

	unsigned char MAC[6];
	unsigned long DataRate;

#define RT_CMD_80211_TXRATE_LEGACY		0x01
#define RT_CMD_80211_TXRATE_BW_40		0x02
#define RT_CMD_80211_TXRATE_SHORT_GI	0x04
	unsigned int TxRateFlags;
	
	unsigned int TxRateMCS;
	int Signal;
	unsigned int TxPacketCnt;
	unsigned int InactiveTime;
} CMD_RTPRIV_IOCTL_80211_STA;

typedef struct __CMD_RTPRIV_IOCTL_80211_KEY {

#define RT_CMD_80211_KEY_WEP			0x00
#define RT_CMD_80211_KEY_WPA			0x01
	unsigned char KeyType;
	unsigned char KeyBuf[50];
	unsigned char KeyId;
} CMD_RTPRIV_IOCTL_80211_KEY;

typedef struct __CMD_RTPRIV_IOCTL_80211_CONNECT {

	unsigned char WpaVer;
	bool FlgIs8021x;
	bool FlgIsAuthOpen;

#define RT_CMD_80211_CONN_ENCRYPT_NONE	0x01
#define RT_CMD_80211_CONN_ENCRYPT_WEP	0x02
#define RT_CMD_80211_CONN_ENCRYPT_TKIP	0x04
#define RT_CMD_80211_CONN_ENCRYPT_CCMP	0x08
	unsigned char PairwiseEncrypType;
	unsigned char GroupwiseEncrypType;

	unsigned char *pKey;
	unsigned int KeyLen;
	unsigned char KeyIdx;

	unsigned char *pSsid;
	unsigned int SsidLen;
} CMD_RTPRIV_IOCTL_80211_CONNECT;

typedef struct __CMD_RTPRIV_IOCTL_80211_REG_NOTIFY {

	unsigned char Alpha2[2];
	VOID *pWiphy;
} CMD_RTPRIV_IOCTL_80211_REG_NOTIFY;

typedef struct __CMD_RTPRIV_IOCTL_80211_SURVEY {

	VOID *pCfg80211;
/*	unsigned long long ChannelTime; */ /* idle + busy, not support */
	unsigned long long ChannelTimeBusy;
	unsigned long long ChannelTimeExtBusy;
} CMD_RTPRIV_IOCTL_80211_SURVEY;

#endif /* RT_CFG80211_SUPPORT */

/* station commands */

/* when adding any new type, please also add codes in LINUX_WEVENT_TRANSLATE */
#define RT_WLAN_EVENT_CUSTOM							0x01
#define RT_WLAN_EVENT_CGIWAP							0x02
#define RT_WLAN_EVENT_ASSOC_REQ_IE						0x03
#define RT_WLAN_EVENT_SCAN								0x04
#define RT_WLAN_EVENT_EXPIRED							0x05
#define RT_WLAN_EVENT_SHOWPIN							0x06
#define RT_WLAN_EVENT_PIN							0x07

typedef struct __RT_CMD_RATE_SET {
	IN unsigned int Rate;
	IN unsigned int Fixed;
} RT_CMD_RATE_SET;

typedef struct __RT_CMD_PARAM_SET {
	IN char * pThisChar;
	IN char * pValue;
} RT_CMD_PARAM_SET;

typedef struct __RT_CMD_SHARED_KEY_ADD {
	IN unsigned char KeyIdx;
	IN bool FlgHaveGTK;
} RT_CMD_SHARED_KEY_ADD;

typedef struct __RT_CMD_MBSS_KICKOUT {
	IN INT BssId;
	IN unsigned short Reason;
} RT_CMD_MBSS_KICKOUT;

typedef struct __RT_CMD_USB_MORE_FLAG_CONFIG {
	IN unsigned int VendorID;
	IN unsigned int ProductID;
} RT_CMD_USB_MORE_FLAG_CONFIG;

typedef struct __RT_CMD_USB_DEV_CONFIG {
	IN unsigned int NumberOfPipes;
	IN unsigned char BulkInEpAddr[2];
	IN unsigned short BulkInMaxPacketSize;
	IN unsigned char BulkOutEpAddr[6];
	IN unsigned short BulkOutMaxPacketSize;
	IN VOID *pConfig;
} RT_CMD_USB_DEV_CONFIG;

typedef struct __RT_CMD_CFG80211_CONFIG {
	IN VOID *pCfgDev;
	IN VOID(
	*CFG80211_Register) (
	IN VOID * pAd,
	IN VOID * pDev,
	IN VOID * pNetDev);
} RT_CMD_CFG80211_CONFIG;

typedef struct __RT_CMD_WAIT_QUEUE_LIST {
	OUT RTMP_OS_TASK *pMlmeTask;
	OUT RTMP_OS_TASK *pTimerTask;
	OUT RTMP_OS_TASK *pCmdQTask;
	OUT RTMP_OS_TASK *pWscTask;
} RT_CMD_WAIT_QUEUE_LIST;

typedef struct __RT_CMD_INF_UP_DOWN {

	IN	int (*rt28xx_open)(VOID *net_dev);
	IN	int (*rt28xx_close)(VOID *net_dev);
} RT_CMD_INF_UP_DOWN;

typedef struct __RT_CMD_STATS {
	IN VOID *pNetDev;
	OUT VOID *pStats;	/* point to pAd->stats */

	OUT unsigned long rx_packets;	/* total packets received       */
	OUT unsigned long tx_packets;	/* total packets transmitted */
	OUT unsigned long rx_bytes;	/* total bytes received         */
	OUT unsigned long tx_bytes;	/* total bytes transmitted      */
	OUT unsigned long rx_errors;	/* bad packets received         */
	OUT unsigned long tx_errors;	/* packet transmit problems     */
	OUT unsigned long multicast;	/* multicast packets received */
	OUT unsigned long collisions;

	OUT unsigned long rx_over_errors;	/* receiver ring buff overflow  */
	OUT unsigned long rx_crc_errors;	/* recved pkt with crc error    */
	OUT unsigned long rx_frame_errors;	/* recv'd frame alignment error */
	OUT unsigned long rx_fifo_errors;	/* recv'r fifo overrun                  */
} RT_CMD_STATS;

typedef struct __RT_CMD_IW_STATS {

	unsigned long priv_flags;
	unsigned char *dev_addr;

	VOID *pStats;		/* point to pAd->iw_stats */

	unsigned char qual;
	unsigned char level;
	unsigned char noise;
	unsigned char updated;
} RT_CMD_IW_STATS;

typedef struct __RT_CMD_PCIE_INIT {

	IN VOID *pPciDev;
	IN unsigned int ConfigDeviceID;
	IN unsigned int ConfigSubsystemVendorID;
	IN unsigned int ConfigSubsystemID;
} RT_CMD_PCIE_INIT;

typedef struct __RT_CMD_AP_IOCTL_CONFIG {
	IN VOID *net_dev;
	IN unsigned long priv_flags;
	IN char *pCmdData;
	IN int CmdId_RTPRIV_IOCTL_SET;
	IN char *name;
	IN INT apidx;

	OUT int Status;
} RT_CMD_AP_IOCTL_CONFIG;

typedef struct __RT_CMD_AP_IOCTL_SSID {
	IN unsigned long priv_flags;
	IN INT apidx;

	OUT char *pSsidStr;
	OUT int length;
} RT_CMD_AP_IOCTL_SSID;

typedef struct __RT_CMD_IOCTL_RATE {
	IN unsigned long priv_flags;
	OUT unsigned int BitRate;
} RT_CMD_IOCTL_RATE;

#define RTMP_CMD_STA_MODE_AUTO			0x00
#define RTMP_CMD_STA_MODE_ADHOC			0x01
#define RTMP_CMD_STA_MODE_INFRA			0x02
#define RTMP_CMD_STA_MODE_MONITOR		0x03

typedef struct __RT_CMD_STA_IOCTL_FREQ {
	IN int m;		/* Mantissa */
	IN short e;		/* Exponent */
} RT_CMD_STA_IOCTL_FREQ;

typedef struct __RT_CMD_STA_IOCTL_BSS {
	OUT unsigned char Bssid[6];
	OUT unsigned char ChannelQuality;
	OUT CHAR Rssi;
	OUT CHAR Noise;
} RT_CMD_STA_IOCTL_BSS;

typedef struct __RT_CMD_STA_IOCTL_BSS_LIST {
	IN unsigned int MaxNum;
	OUT unsigned int BssNum;
	OUT RT_CMD_STA_IOCTL_BSS *pList;
} RT_CMD_STA_IOCTL_BSS_LIST;

typedef struct __RT_CMD_STA_IOCTL_SCAN {
	IN unsigned char FlgScanThisSsid;
	IN unsigned int SsidLen;
	IN CHAR *pSsid;
	OUT int Status;
} RT_CMD_STA_IOCTL_SCAN;

typedef struct __RT_CMD_STA_IOCTL_BSS_TABLE {
	OUT unsigned char Bssid[6];
	OUT unsigned char Channel;
	OUT unsigned char BssType;
	OUT unsigned char HtCapabilityLen;

	OUT unsigned char SupRate[12];
	OUT unsigned char SupRateLen;
	OUT unsigned char ExtRate[12];
	OUT unsigned char ExtRateLen;

	OUT unsigned char SsidLen;
	OUT CHAR Ssid[32];

	OUT unsigned short CapabilityInfo;
	OUT unsigned char ChannelWidth, ShortGIfor40, ShortGIfor20, MCSSet;

	OUT unsigned short WpaIeLen;
	OUT unsigned char *pWpaIe;

	OUT unsigned short RsnIeLen;
	OUT unsigned char *pRsnIe;

	OUT unsigned short WpsIeLen;
	OUT unsigned char *pWpsIe;

	OUT unsigned char FlgIsPrivacyOn;

	OUT RT_CMD_STA_IOCTL_BSS Signal;
} RT_CMD_STA_IOCTL_BSS_TABLE;

typedef struct __RT_CMD_STA_IOCTL_SCAN_TABLE {
	IN unsigned long priv_flags;
	OUT unsigned int BssNr;
	OUT RT_CMD_STA_IOCTL_BSS_TABLE *pBssTable;	/* must be freed by caller */
	OUT unsigned char MainSharedKey[4][16];
} RT_CMD_STA_IOCTL_SCAN_TABLE;

typedef struct __RT_CMD_STA_IOCTL_SSID {
	IN unsigned char FlgAnySsid;
	INOUT unsigned int SsidLen;
	INOUT CHAR *pSsid;
	OUT int Status;
} RT_CMD_STA_IOCTL_SSID;

typedef struct __RT_CMD_STA_IOCTL_NICK_NAME {
	OUT unsigned int NameLen;
	OUT CHAR *pName;
} RT_CMD_STA_IOCTL_NICK_NAME;

typedef struct __RT_CMD_STA_IOCTL_SECURITY {
	INOUT CHAR *pData;
	INOUT unsigned short length;
	IN int KeyIdx;
	IN int MaxKeyLen;

#define RT_CMD_STA_IOCTL_SECURITY_ALG_NONE		0x01
#define RT_CMD_STA_IOCTL_SECURITY_ALG_WEP		0x02
#define RT_CMD_STA_IOCTL_SECURITY_ALG_TKIP		0x03
#define RT_CMD_STA_IOCTL_SECURITY_ALG_CCMP		0x04
	IN unsigned int Alg;

#define RT_CMD_STA_IOCTL_SECURTIY_EXT_SET_TX_KEY	0x01
#define RT_CMD_STA_IOCTL_SECURTIY_EXT_GROUP_KEY		0x02
	IN unsigned short ext_flags;

#define RT_CMD_STA_IOCTL_SECURITY_DISABLED		0x01
#define RT_CMD_STA_IOCTL_SECURITY_ENABLED		0x02
#define RT_CMD_STA_IOCTL_SECURITY_RESTRICTED	0x04
#define RT_CMD_STA_IOCTL_SECURITY_OPEN			0x08
#define RT_CMD_STA_IOCTL_SECURITY_NOKEY			0x10
#define RT_CMD_STA_IOCTL_SECURITY_MODE			0x20
	INOUT unsigned short flags;

	OUT int Status;
} RT_CMD_STA_IOCTL_SECURITY;

typedef struct __RT_CMD_STA_IOCTL_WSC_U32_ITEM {
	IN unsigned int *pUWrq;
	OUT int Status;
} RT_CMD_STA_IOCTL_WSC_U32_ITEM;

typedef struct __RT_CMD_STA_IOCTL_WSC_STR_ITEM {
	IN unsigned int Subcmd;
	IN CHAR *pData;
	IN unsigned int length;

	OUT int Status;
} RT_CMD_STA_IOCTL_WSC_STR_ITEM;

typedef struct __RT_CMD_STA_IOCTL_SHOW {
	IN CHAR *pData;
	IN unsigned int MaxSize;
	IN unsigned int InfType;
} RT_CMD_STA_IOCTL_SHOW;

#define RT_CMD_STA_IOCTL_IW_MLME_DEAUTH		0x01
#define RT_CMD_STA_IOCTL_IW_MLME_DISASSOC	0x02

typedef struct __RT_CMD_STA_IOCTL_SECURITY_ADV {

#define RT_CMD_STA_IOCTL_WPA_VERSION					0x10
#define RT_CMD_STA_IOCTL_WPA_VERSION1				0x11
#define RT_CMD_STA_IOCTL_WPA_VERSION2				0x12

#define RT_CMD_STA_IOCTL_WPA_PAIRWISE					0x20
#define RT_CMD_STA_IOCTL_WPA_PAIRWISE_NONE			0x21
#define RT_CMD_STA_IOCTL_WPA_PAIRWISE_WEP40			0x22
#define RT_CMD_STA_IOCTL_WPA_PAIRWISE_WEP104		0x23
#define RT_CMD_STA_IOCTL_WPA_PAIRWISE_TKIP			0x24
#define RT_CMD_STA_IOCTL_WPA_PAIRWISE_CCMP			0x25

#define RT_CMD_STA_IOCTL_WPA_GROUP						0x30
#define RT_CMD_STA_IOCTL_WPA_GROUP_NONE				0x31
#define RT_CMD_STA_IOCTL_WPA_GROUP_WEP40			0x32
#define RT_CMD_STA_IOCTL_WPA_GROUP_WEP104			0x33
#define RT_CMD_STA_IOCTL_WPA_GROUP_TKIP				0x34
#define RT_CMD_STA_IOCTL_WPA_GROUP_CCMP				0x35

#define RT_CMD_STA_IOCTL_WPA_KEY_MGMT					0x40
#define RT_CMD_STA_IOCTL_WPA_KEY_MGMT_1X			0x41
#define RT_CMD_STA_IOCTL_WPA_KEY_MGMT_WPS			0x42

#define RT_CMD_STA_IOCTL_WPA_AUTH_RX_UNENCRYPTED_EAPOL	0x50
#define RT_CMD_STA_IOCTL_WPA_AUTH_PRIVACY_INVOKED		0x60
#define RT_CMD_STA_IOCTL_WPA_AUTH_DROP_UNENCRYPTED		0x70

#define RT_CMD_STA_IOCTL_WPA_AUTH_80211_AUTH_ALG		0x80
#define RT_CMD_STA_IOCTL_WPA_AUTH_80211_AUTH_ALG_SHARED		0x81
#define RT_CMD_STA_IOCTL_WPA_AUTH_80211_AUTH_ALG_OPEN		0x82
#define RT_CMD_STA_IOCTL_WPA_AUTH_80211_AUTH_ALG_LEAP		0x83

#define RT_CMD_STA_IOCTL_WPA_AUTH_WPA_ENABLED			0x90

#define RT_CMD_STA_IOCTL_WPA_AUTH_COUNTERMEASURES			0xA0

	IN unsigned int flags;
	IN unsigned int value;
} RT_CMD_STA_IOCTL_SECURITY_ADV;

typedef struct __RT_CMD_STA_IOCTL_RSN_IE {
	INOUT unsigned int length;
	INOUT unsigned char *pRsnIe;
} RT_CMD_STA_IOCTL_RSN_IE;

typedef struct __RT_CMD_STA_IOCTL_PMA_SA {
#define RT_CMD_STA_IOCTL_PMA_SA_FLUSH					0x01
#define RT_CMD_STA_IOCTL_PMA_SA_REMOVE					0x02
#define RT_CMD_STA_IOCTL_PMA_SA_ADD						0x03
	IN unsigned int Cmd;
	IN unsigned char *pBssid;
	IN unsigned char *pPmkid;
} RT_CMD_STA_IOCTL_PMA_SA;

#endif /* __RTMP_CMD_H__ */
