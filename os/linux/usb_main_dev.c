/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************/

#define RTMP_MODULE_OS

/*#include "rt_config.h" */
#include "rtmp_comm.h"
#include "rt_os_util.h"
#include "rt_os_net.h"


/* Following information will be show when you run 'modinfo' */
/* *** If you have a solution for the bug in current version of driver, please mail to me. */
/* Otherwise post to forum in ralinktech's web site(www.ralinktech.com) and let all users help you. *** */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Paul Lin <paul_lin@ralinktech.com>");
MODULE_DESCRIPTION("RT2870 Wireless Lan Linux Driver");

#ifdef MODULE_VERSION
MODULE_VERSION(AP_DRIVER_VERSION);
#endif

extern USB_DEVICE_ID rtusb_dev_id[];
extern INT const rtusb_usb_id_len;

static void rt2870_disconnect(
	IN struct usb_device *dev,
	IN VOID *pAd);

static int rt2870_probe(
	IN struct usb_interface *intf,
	IN struct usb_device *usb_dev,
	IN const USB_DEVICE_ID *dev_id,
	IN VOID **ppAd);

#ifndef PF_NOFREEZE
#define PF_NOFREEZE  0
#endif


/*extern int rt28xx_close(IN struct net_device *net_dev); */
/*extern int rt28xx_open(struct net_device *net_dev); */

static BOOLEAN USBDevConfigInit(
	IN struct usb_device 	*dev,
	IN struct usb_interface *intf, 
	IN VOID					*pAd);

VOID RT28XXVendorSpecificCheck(
	IN struct usb_device 	*dev,
	IN VOID 				*pAd)
{

	RT_CMD_USB_MORE_FLAG_CONFIG Config = { dev->descriptor.idVendor,
						dev->descriptor.idProduct };
	RTMP_DRIVER_USB_MORE_FLAG_SET(pAd, &Config);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)

/**************************************************************************/
/**************************************************************************/
/*tested for kernel 2.4 series */
/**************************************************************************/
/**************************************************************************/
static void *rtusb_probe(struct usb_device *dev, UINT interface,
						const USB_DEVICE_ID *id_table);
static void rtusb_disconnect(struct usb_device *dev, void *ptr);

struct usb_driver rtusb_driver = {
		name:RTMP_DRV_NAME,
		probe:rtusb_probe,
		disconnect:rtusb_disconnect,
		id_table:rtusb_dev_id,
	};


static BOOLEAN USBDevConfigInit(
	IN struct usb_device *dev,
	IN struct usb_interface *intf,
	IN VOID *pAd)
{
	struct usb_interface_descriptor *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	ULONG BulkOutIdx;
	ULONG BulkInIdx;
	UINT32 i;
	RT_CMD_USB_DEV_CONFIG Config, *pConfig = &Config;

	iface_desc = &intf->altsetting[0];

	/* get # of enpoints */
	pConfig->NumberOfPipes = iface_desc->bNumEndpoints;
	DBGPRINT(RT_DEBUG_TRACE, ("NumEndpoints = %d\n", iface_desc->bNumEndpoints));		 

	/* Configure Pipes */
	endpoint = &iface_desc->endpoint[0];
	BulkOutIdx = 0;
	BulkInIdx = 0;

	for(i = 0; i < pConfig->NumberOfPipes; i++)
	{
		if ((endpoint[i].bmAttributes == USB_ENDPOINT_XFER_BULK) && 
			((endpoint[i].bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN))
		{
			pConfig->BulkInEpAddr[BulkInIdx++] = endpoint[i].bEndpointAddress;
			pConfig->BulkInMaxPacketSize = endpoint[i].wMaxPacketSize;

			DBGPRINT_RAW(RT_DEBUG_TRACE, ("BULK IN MaximumPacketSize = %d\n", pConfig->BulkInMaxPacketSize));
			DBGPRINT_RAW(RT_DEBUG_TRACE, ("EP address = 0x%2x\n", endpoint[i].bEndpointAddress));
		}
		else if ((endpoint[i].bmAttributes == USB_ENDPOINT_XFER_BULK) && 
				((endpoint[i].bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT))
		{
			/* There are 6 bulk out EP. EP6 highest priority. */
			/* EP1-4 is EDCA.  EP5 is HCCA. */
			pConfig->BulkOutEpAddr[BulkOutIdx++] = endpoint[i].bEndpointAddress;
			pConfig->BulkOutMaxPacketSize = endpoint[i].wMaxPacketSize;

			DBGPRINT_RAW(RT_DEBUG_TRACE, ("BULK OUT MaximumPacketSize = %d\n", pConfig->BulkOutMaxPacketSize));
			DBGPRINT_RAW(RT_DEBUG_TRACE, ("EP address = 0x%2x\n", endpoint[i].bEndpointAddress));
		}
	}

	if (!(pConfig->BulkInEpAddr && pConfig->BulkOutEpAddr[0]))
	{
#ifdef DBG
		printk("Could not find both bulk-in and bulk-out endpoints\n");
#endif /* DBG */
		return FALSE;
	}

	pConfig->pConfig = dev->config;
	RTMP_DRIVER_USB_CONFIG_INIT(pAd, pConfig);
	RT28XXVendorSpecificCheck(dev, pAd);

	return TRUE;
}

static void *rtusb_probe(struct usb_device *dev, UINT interface,
						const USB_DEVICE_ID *id)
{
	struct usb_interface *intf;
	VOID *pAd;
	int rv;


	/* get the active interface descriptor */
	intf = &dev->actconfig->interface[interface];

	/* call generic probe procedure. */
	rv = rt2870_probe(intf, dev, id, &pAd);
	if (rv != 0)
		pAd = NULL;
	
	return (void *)pAd;
}

/*Disconnect function is called within exit routine */
static void rtusb_disconnect(struct usb_device *dev, void *ptr)
{
	rt2870_disconnect(dev, ptr);
}


#else	/* else if we are kernel 2.6 series */


/**************************************************************************/
/**************************************************************************/
/*tested for kernel 2.6series */
/**************************************************************************/
/**************************************************************************/

#ifdef CONFIG_PM

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,10)
#define pm_message_t u32
#endif

static int rt2870_suspend(struct usb_interface *intf, pm_message_t state);
static int rt2870_resume(struct usb_interface *intf);
#endif /* CONFIG_PM */

static int rtusb_probe (struct usb_interface *intf,
						const USB_DEVICE_ID *id);
static void rtusb_disconnect(struct usb_interface *intf);

static BOOLEAN USBDevConfigInit(
	IN struct usb_device 	*dev,
	IN struct usb_interface *intf, 
	IN VOID 				*pAd)
{
	struct usb_host_interface *iface_desc;
	ULONG BulkOutIdx;
	ULONG BulkInIdx;
	UINT32 i;
	RT_CMD_USB_DEV_CONFIG Config, *pConfig = &Config;
	
	/* get the active interface descriptor */
	iface_desc = intf->cur_altsetting;

	/* get # of enpoints  */
	pConfig->NumberOfPipes = iface_desc->desc.bNumEndpoints;
	DBGPRINT(RT_DEBUG_TRACE, ("NumEndpoints = %d\n", iface_desc->desc.bNumEndpoints));

	/* Configure Pipes */
	BulkOutIdx = 0;
	BulkInIdx = 0;

	for (i = 0; i < pConfig->NumberOfPipes; i++)
	{ 
		if ((iface_desc->endpoint[i].desc.bmAttributes == USB_ENDPOINT_XFER_BULK) && 
			((iface_desc->endpoint[i].desc.bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN))
		{
			if (BulkInIdx < 2)
			{
				pConfig->BulkInEpAddr[BulkInIdx++] = iface_desc->endpoint[i].desc.bEndpointAddress;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
				pConfig->BulkInMaxPacketSize = le2cpu16(iface_desc->endpoint[i].desc.wMaxPacketSize);
#else
				pConfig->BulkInMaxPacketSize = iface_desc->endpoint[i].desc.wMaxPacketSize;
#endif /* LINUX_VERSION_CODE */

				DBGPRINT_RAW(RT_DEBUG_TRACE, ("BULK IN MaxPacketSize = %d\n", pConfig->BulkInMaxPacketSize));
				DBGPRINT_RAW(RT_DEBUG_TRACE, ("EP address = 0x%2x\n", iface_desc->endpoint[i].desc.bEndpointAddress));
			} 
			else
			{
				DBGPRINT(RT_DEBUG_ERROR, ("Bulk IN endpoint nums large than 2\n"));
			}
		}
		else if ((iface_desc->endpoint[i].desc.bmAttributes == USB_ENDPOINT_XFER_BULK) && 
				((iface_desc->endpoint[i].desc.bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT))
		{
			if (BulkOutIdx < 6)
			{
				/* there are 6 bulk out EP. EP6 highest priority. */
				/* EP1-4 is EDCA.  EP5 is HCCA. */
				pConfig->BulkOutEpAddr[BulkOutIdx++] = iface_desc->endpoint[i].desc.bEndpointAddress;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
				pConfig->BulkOutMaxPacketSize = le2cpu16(iface_desc->endpoint[i].desc.wMaxPacketSize);
#else
				pConfig->BulkOutMaxPacketSize = iface_desc->endpoint[i].desc.wMaxPacketSize;
#endif

				DBGPRINT_RAW(RT_DEBUG_TRACE, ("BULK OUT MaxPacketSize = %d\n", pConfig->BulkOutMaxPacketSize));
				DBGPRINT_RAW(RT_DEBUG_TRACE, ("EP address = 0x%2x\n", iface_desc->endpoint[i].desc.bEndpointAddress));
			}
			else
			{
				DBGPRINT(RT_DEBUG_ERROR, ("Bulk Out endpoint nums large than 6\n"));
			}
		}
	}

	if (!(pConfig->BulkInEpAddr && pConfig->BulkOutEpAddr[0]))
	{
#ifdef DBG
		printk("%s: Could not find both bulk-in and bulk-out endpoints\n", __FUNCTION__);
#endif /* DBG */
		return FALSE;
	}

	pConfig->pConfig = &dev->config->desc;
	usb_set_intfdata(intf, pAd);
	RTMP_DRIVER_USB_CONFIG_INIT(pAd, pConfig);
	RT28XXVendorSpecificCheck(dev, pAd);

	return TRUE;
	
}



static int rtusb_probe (struct usb_interface *intf,
						const USB_DEVICE_ID *id)
{	
	VOID *pAd;
	struct usb_device *dev;
	int rv;

	dev = interface_to_usbdev(intf);
	dev = usb_get_dev(dev);
	
	rv = rt2870_probe(intf, dev, id, &pAd);
	if (rv != 0)
	{
		usb_put_dev(dev);
	}
#ifdef IFUP_IN_PROBE
	else
	{
		if (VIRTUAL_IF_UP(pAd) != 0)
		{
			pAd = usb_get_intfdata(intf);
			usb_set_intfdata(intf, NULL);
			rt2870_disconnect(dev, pAd);
			rv = -ENOMEM;
		}
	}
#endif /* IFUP_IN_PROBE */	
	return rv;
}


static void rtusb_disconnect(struct usb_interface *intf)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	VOID *pAd;

	pAd = usb_get_intfdata(intf);
#ifdef IFUP_IN_PROBE	
	VIRTUAL_IF_DOWN(pAd);
#endif /* IFUP_IN_PROBE */	
	usb_set_intfdata(intf, NULL);	

	rt2870_disconnect(dev, pAd);

#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
#ifdef DBG
	printk("rtusb_disconnect usb_autopm_put_interface\n");
#endif /* DBG */
	usb_autopm_put_interface(intf);
#ifdef DBG
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
	printk("rt2870_disconnect pm_usage_cnt %d\n", atomic_read(&intf->pm_usage_cnt));
#else
	printk("rt2870_disconnect pm_usage_cnt %d\n", intf->pm_usage_cnt);
#endif
#endif /* DBG */
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */

}


struct usb_driver rtusb_driver = {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15)
	.owner = THIS_MODULE,
#endif	
	.name = RTMP_DRV_NAME,
	.probe = rtusb_probe,
	.disconnect = rtusb_disconnect,
	.id_table = rtusb_dev_id,

#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	.supports_autosuspend = 1,
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
	suspend: rt2870_suspend,
	resume:	 rt2870_resume,
#endif /* CONFIG_PM */
	.supports_autosuspend = 1,
};

#ifdef CONFIG_PM

VOID RT2870RejectPendingPackets(
	IN	VOID	*pAd)
{
	/* clear PS packets */
	/* clear TxSw packets */
}

static int rt2870_suspend(
	struct usb_interface *intf,
	pm_message_t state)
{
	//struct net_device *net_dev;
	VOID *pAd = usb_get_intfdata(intf);
#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	UCHAR Flag;

	RTMP_DRIVER_ADAPTER_RT28XX_WOW_STATUS(pAd, &Flag);
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */

	DBGPRINT(RT_DEBUG_TRACE, ("rt2870_suspend()\n"));

#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	UCHAR Flag;
	DBGPRINT(RT_DEBUG_ERROR, ("rt2870_suspend()\n"));

#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	if (Flag == FALSE)
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */
	{
/*	if(!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF)) */
		RTMP_DRIVER_ADAPTER_END_DISSASSOCIATE(pAd);
		RTMP_DRIVER_ADAPTER_IDLE_RADIO_OFF_TEST(pAd, &Flag);
		if(!Flag)
		{
			/*RT28xxUsbAsicRadioOff(pAd); */
			RTMP_DRIVER_ADAPTER_RT28XX_USB_ASICRADIO_OFF(pAd);
		}
	}
	/*RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_SUSPEND); */
	RTMP_DRIVER_ADAPTER_SUSPEND_SET(pAd);
	return 0;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */



#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	if (Flag == TRUE)
		RTMP_DRIVER_ADAPTER_RT28XX_WOW_ENABLE(pAd);
	else
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */
	{
		RTMP_DRIVER_ADAPTER_RT28XX_USB_ASICRADIO_OFF(pAd);
		RTMP_DRIVER_ADAPTER_SUSPEND_SET(pAd);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("rt2870_suspend()\n"));
	return 0;
}

static int rt2870_resume(
	struct usb_interface *intf)
{
	//struct net_device *net_dev;
	VOID *pAd = usb_get_intfdata(intf);
#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	UCHAR Flag;
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */

#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	int pm_usage_cnt;
	UCHAR Flag;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */

#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	RTMP_DRIVER_ADAPTER_RT28XX_WOW_STATUS(pAd, &Flag);
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */

#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
	pm_usage_cnt = atomic_read(&intf->pm_usage_cnt);	
#else
	pm_usage_cnt = intf->pm_usage_cnt;
#endif

	if(pm_usage_cnt <= 0)
		usb_autopm_get_interface(intf);

	DBGPRINT(RT_DEBUG_ERROR, ("rt2870_resume()\n"));

	/*RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_SUSPEND); */
	RTMP_DRIVER_ADAPTER_SUSPEND_CLEAR(pAd);

#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	if (Flag == FALSE)
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */
		/*RT28xxUsbAsicRadioOn(pAd); */
		RTMP_DRIVER_ADAPTER_RT28XX_USB_ASICRADIO_ON(pAd);

	DBGPRINT(RT_DEBUG_ERROR, ("rt2870_resume()\n"));

	return 0;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */


	DBGPRINT(RT_DEBUG_TRACE, ("rt2870_resume()\n"));


#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	if (Flag == TRUE)
		RTMP_DRIVER_ADAPTER_RT28XX_WOW_DISABLE(pAd);
	else
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */
	{
		RTMP_DRIVER_ADAPTER_SUSPEND_CLEAR(pAd);
		RTMP_DRIVER_ADAPTER_RT28XX_USB_ASICRADIO_ON(pAd);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("rt2870_resume()\n"));
	return 0;
}
#endif /* CONFIG_PM */
#endif /* LINUX_VERSION_CODE */


/* Init driver module */
INT __init rtusb_init(void)
{
	printk(RTMP_DRV_NAME ": init\n");
	return usb_register(&rtusb_driver);
}

/* Deinit driver module */
VOID __exit rtusb_exit(void)
{
	usb_deregister(&rtusb_driver);
	printk(RTMP_DRV_NAME ": exit\n");
}

module_init(rtusb_init);
module_exit(rtusb_exit);

/*---------------------------------------------------------------------	*/
/* function declarations						*/
/*---------------------------------------------------------------------	*/



/*
========================================================================
Routine Description:
    Release allocated resources.

Arguments:
    *dev				Point to the PCI or USB device
	pAd					driver control block pointer

Return Value:
    None

Note:
========================================================================
*/
static void rt2870_disconnect(struct usb_device *dev, VOID *pAd)
{
	struct net_device *net_dev;


	DBGPRINT(RT_DEBUG_ERROR, ("rtusb_disconnect: unregister usbnet usb-%s-%s\n",
				dev->bus->bus_name, dev->devpath));
	if (!pAd)
	{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)	/* kernel 2.4 series */
		while(MOD_IN_USE > 0)
			MOD_DEC_USE_COUNT;
#else
		usb_put_dev(dev);
#endif /* LINUX_VERSION_CODE */

#ifdef DBG
		printk("rtusb_disconnect: pAd == NULL!\n");
#endif /* DBG */
		return;
	}
/*	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST); */
	RTMP_DRIVER_NIC_NOT_EXIST_SET(pAd);

	/* for debug, wait to show some messages to /proc system */
	udelay(1);

	RTMP_DRIVER_NET_DEV_GET(pAd, &net_dev);

	RtmpPhyNetDevExit(pAd, net_dev);

	/* FIXME: Shall we need following delay and flush the schedule?? */
	udelay(1);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)	/* kernel 2.4 series */
#else
	flush_scheduled_work();
#endif /* LINUX_VERSION_CODE */
	udelay(1);

#ifdef RT_CFG80211_SUPPORT
	RTMP_DRIVER_80211_UNREGISTER(pAd, net_dev);
#endif /* RT_CFG80211_SUPPORT */

	/* free the root net_device */
//	RtmpOSNetDevFree(net_dev);

	RtmpRaDevCtrlExit(pAd);

	/* free the root net_device */
	RtmpOSNetDevFree(net_dev);

	/* release a use of the usb device structure */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)	/* kernel 2.4 series */
	while(MOD_IN_USE > 0)
		MOD_DEC_USE_COUNT;
#else
	usb_put_dev(dev);
#endif /* LINUX_VERSION_CODE */
	udelay(1);

	DBGPRINT(RT_DEBUG_ERROR, ("RTUSB disconnected successfully\n"));
}


static int rt2870_probe(
	IN struct usb_interface *intf,
	IN struct usb_device *usb_dev,
	IN const USB_DEVICE_ID *dev_id,
	IN VOID **ppAd)
{
	struct  net_device		*net_dev = NULL;
	VOID       				*pAd = (VOID *) NULL;
	INT                 	status, rv;
	PVOID					handle;
	RTMP_OS_NETDEV_OP_HOOK	netDevHook;
	ULONG					OpMode;
#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
/*	INT 		pm_usage_cnt; */
	INT		 res =1 ; 
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */	

	DBGPRINT(RT_DEBUG_TRACE, ("rt2870_probe()\n"));
	
#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND

        res = usb_autopm_get_interface(intf);
	if (res)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("rt2870_probe autopm_resume fail\n"));
		return -EIO;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
	atomic_set(&intf->pm_usage_cnt, 1);
#ifdef DBG
	printk("rt2870_probe ====> pm_usage_cnt %d\n", atomic_read(&intf->pm_usage_cnt));
#endif /* DBG */
#else
        intf->pm_usage_cnt = 1;
#ifdef DBG
	printk("rt2870_probe ====> pm_usage_cnt %d\n", intf->pm_usage_cnt);
#endif /* DBG */
#endif
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */

/*RtmpDevInit============================================= */
	/* Allocate RTMP_ADAPTER adapter structure */
/*	handle = kmalloc(sizeof(struct os_cookie), GFP_KERNEL); */
	os_alloc_mem(NULL, (UCHAR **)&handle, sizeof(struct os_cookie));
	if (handle == NULL)
	{
#ifdef DBG
		printk("rt2870_probe(): Allocate memory for os handle failed!\n");
#endif /* DBG */
		return -ENOMEM;
	}
	memset(handle, 0, sizeof(struct os_cookie));

	((POS_COOKIE)handle)->pUsb_Dev = usb_dev;


	/* set/get operators to/from DRIVER module */
#ifdef OS_ABL_FUNC_SUPPORT
	/* get DRIVER operations */
	RtmpNetOpsInit(pRtmpDrvNetOps);
	RTMP_DRV_OPS_FUNCTION(pRtmpDrvOps, pRtmpDrvNetOps, NULL, NULL);
	RtmpNetOpsSet(pRtmpDrvNetOps);
#endif /* OS_ABL_FUNC_SUPPORT */

	rv = RTMPAllocAdapterBlock(handle, &pAd);
	if (rv != NDIS_STATUS_SUCCESS) 
	{
/*		kfree(handle); */
		os_free_mem(NULL, handle);
		goto err_out;
	}

/*USBDevInit============================================== */
	if (USBDevConfigInit(usb_dev, intf, pAd) == FALSE)
		goto err_out_free_radev;

	RtmpRaDevCtrlInit(pAd, RTMP_DEV_INF_USB);
	
/*NetDevInit============================================== */
	net_dev = RtmpPhyNetDevInit(pAd, &netDevHook);
	if (net_dev == NULL)
		goto err_out_free_radev;
	
	/* Here are the net_device structure with usb specific parameters. */


/*All done, it's time to register the net device to linux kernel. */
	/* Register this device */
#ifdef RT_CFG80211_SUPPORT
{
/*	pAd->pCfgDev = &(usb_dev->dev); */
/*	pAd->CFG80211_Register = CFG80211_Register; */
/*	RTMP_DRIVER_CFG80211_INIT(pAd, usb_dev); */

	/*
		In 2.6.32, cfg80211 register must be before register_netdevice();
		We can not put the register in rt28xx_open();
		Or you will suffer NULL pointer in list_add of
		cfg80211_netdev_notifier_call().
	*/
	CFG80211_Register(pAd, &(usb_dev->dev), net_dev);
}
#endif /* RT_CFG80211_SUPPORT */

	RTMP_DRIVER_OP_MODE_GET(pAd, &OpMode);
	status = RtmpOSNetDevAttach(OpMode, net_dev, &netDevHook);
	if (status != 0)
		goto err_out_free_netdev;

/*#ifdef KTHREAD_SUPPORT */

	*ppAd = pAd;

#ifdef INF_PPA_SUPPORT
/*	pAd->pDirectpathCb = (PPA_DIRECTPATH_CB *) kmalloc (sizeof(PPA_DIRECTPATH_CB), GFP_ATOMIC); */
/*	os_alloc_mem(NULL, (UCHAR **)&(pAd->pDirectpathCb), sizeof(PPA_DIRECTPATH_CB)); */
	RTMP_DRIVER_INF_PPA_INIT(pAd);
#endif /* INF_PPA_SUPPORT */

#ifdef PRE_ASSIGN_MAC_ADDR
	UCHAR PermanentAddress[MAC_ADDR_LEN];
	RTMP_DRIVER_MAC_ADDR_GET(pAd, &PermanentAddress[0]);
	DBGPRINT(RT_DEBUG_TRACE, ("%s MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n", __FUNCTION__, PermanentAddress[0], PermanentAddress[1], PermanentAddress[2], PermanentAddress[3], PermanentAddress[4], PermanentAddress[5]));
	/* Set up the Mac address */
	RtmpOSNetDevAddrSet(OpMode, net_dev, &PermanentAddress[0], NULL);
#endif /* PRE_ASSIGN_MAC_ADDR */

#ifdef EXT_BUILD_CHANNEL_LIST
	RTMP_DRIVER_SET_PRECONFIG_VALUE(pAd);
#endif /* EXT_BUILD_CHANNEL_LIST */

	DBGPRINT(RT_DEBUG_TRACE, ("rt2870_probe()\n"));

	return 0;

	/* --------------------------- ERROR HANDLE --------------------------- */	
err_out_free_netdev:
	RtmpOSNetDevFree(net_dev);
	
err_out_free_radev:
	RTMPFreeAdapter(pAd);
	
err_out:
	*ppAd = NULL;

	return -1;
}


#ifdef OS_ABL_SUPPORT
/* USB complete handlers in LINUX */
RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkOutDataPacketComplete = NULL;
RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkOutMLMEPacketComplete = NULL;
RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkOutNullFrameComplete = NULL;
#ifdef CONFIG_MULTI_CHANNEL
RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkOutHCCANullFrameComplete = NULL;
#endif /* CONFIG_MULTI_CHANNEL */
RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkOutRTSFrameComplete = NULL;
RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkOutPsPollComplete = NULL;
RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkRxComplete = NULL;
RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkCmdRspEventComplete = NULL;

USBHST_STATUS RTUSBBulkOutDataPacketComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs)
{
	RtmpDrvUsbBulkOutDataPacketComplete((VOID *)pURB);
}

USBHST_STATUS RTUSBBulkOutMLMEPacketComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs)
{
	RtmpDrvUsbBulkOutMLMEPacketComplete((VOID *)pURB);
}

USBHST_STATUS RTUSBBulkOutNullFrameComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs)
{
	RtmpDrvUsbBulkOutNullFrameComplete((VOID *)pURB);
}

#ifdef CONFIG_MULTI_CHANNEL
USBHST_STATUS RTUSBBulkOutHCCANullFrameComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs)
{
	RtmpDrvUsbBulkOutHCCANullFrameComplete((VOID *)pURB);
}
#endif /* CONFIG_MULTI_CHANNEL */


USBHST_STATUS RTUSBBulkOutRTSFrameComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs)
{
	RtmpDrvUsbBulkOutRTSFrameComplete((VOID *)pURB);
}

USBHST_STATUS RTUSBBulkOutPsPollComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs)
{
	RtmpDrvUsbBulkOutPsPollComplete((VOID *)pURB);
}

USBHST_STATUS RTUSBBulkRxComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs)
{
	RtmpDrvUsbBulkRxComplete((VOID *)pURB);
}

USBHST_STATUS RTUSBBulkCmdRspEventComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs)
{
	RtmpDrvUsbBulkCmdRspEventComplete((VOID *)pURB);
}

VOID RtmpNetOpsInit(
	IN VOID			*pDrvNetOpsSrc)
{
	RTMP_NET_ABL_OPS *pDrvNetOps = (RTMP_NET_ABL_OPS *)pDrvNetOpsSrc;


	pDrvNetOps->RtmpNetUsbBulkOutDataPacketComplete = (RTMP_DRV_USB_COMPLETE_HANDLER)RTUSBBulkOutDataPacketComplete;
	pDrvNetOps->RtmpNetUsbBulkOutMLMEPacketComplete = (RTMP_DRV_USB_COMPLETE_HANDLER)RTUSBBulkOutMLMEPacketComplete;
	pDrvNetOps->RtmpNetUsbBulkOutNullFrameComplete = (RTMP_DRV_USB_COMPLETE_HANDLER)RTUSBBulkOutNullFrameComplete;
#ifdef CONFIG_MULTI_CHANNEL
	pDrvNetOps->RtmpNetUsbBulkOutHCCANullFrameComplete = (RTMP_DRV_USB_COMPLETE_HANDLER)RTUSBBulkOutHCCANullFrameComplete;
#endif /* CONFIG_MULTI_CHANNEL */
	pDrvNetOps->RtmpNetUsbBulkOutRTSFrameComplete = (RTMP_DRV_USB_COMPLETE_HANDLER)RTUSBBulkOutRTSFrameComplete;
	pDrvNetOps->RtmpNetUsbBulkOutPsPollComplete = (RTMP_DRV_USB_COMPLETE_HANDLER)RTUSBBulkOutPsPollComplete;
	pDrvNetOps->RtmpNetUsbBulkRxComplete = (RTMP_DRV_USB_COMPLETE_HANDLER)RTUSBBulkRxComplete;
	pDrvNetOps->RtmpNetUsbBulkCmdRspEventComplete = (RTMP_DRV_USB_COMPLETE_HANDLER)RTUSBBulkCmdRspEventComplete;
}


VOID RtmpNetOpsSet(
	IN VOID *pDrvNetOpsSrc)
{
	RTMP_NET_ABL_OPS *pDrvNetOps = (RTMP_NET_ABL_OPS *)pDrvNetOpsSrc;


	RtmpDrvUsbBulkOutDataPacketComplete = pDrvNetOps->RtmpDrvUsbBulkOutDataPacketComplete;
	RtmpDrvUsbBulkOutMLMEPacketComplete = pDrvNetOps->RtmpDrvUsbBulkOutMLMEPacketComplete;
	RtmpDrvUsbBulkOutNullFrameComplete = pDrvNetOps->RtmpDrvUsbBulkOutNullFrameComplete;
#ifdef CONFIG_MULTI_CHANNEL
	RtmpDrvUsbBulkOutHCCANullFrameComplete = pDrvNetOps->RtmpDrvUsbBulkOutHCCANullFrameComplete;
#endif /* CONFIG_MULTI_CHANNEL */

	RtmpDrvUsbBulkOutRTSFrameComplete = pDrvNetOps->RtmpDrvUsbBulkOutRTSFrameComplete;
	RtmpDrvUsbBulkOutPsPollComplete = pDrvNetOps->RtmpDrvUsbBulkOutPsPollComplete;
	RtmpDrvUsbBulkRxComplete = pDrvNetOps->RtmpDrvUsbBulkRxComplete;
	RtmpDrvUsbBulkCmdRspEventComplete = pDrvNetOps->RtmpDrvUsbBulkCmdRspEventComplete;
}
#endif /* OS_ABL_SUPPORT */
