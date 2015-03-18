/****************************************************************************

    Module Name:
	rt_usb_util.c
 
    Abstract:
	Any utility is used in UTIL module for USB function.
 
    Revision History:
    Who        When          What
    ---------  ----------    ----------------------------------------------

***************************************************************************/

#define RTMP_MODULE_OS
#define RTMP_MODULE_OS_UTIL

#include "rtmp_comm.h"
#include "rtmp_osabl.h"
#include "rt_os_util.h"

#ifdef RTMP_MAC_USB
#ifdef OS_ABL_SUPPORT
MODULE_LICENSE("GPL");
#endif /* OS_ABL_SUPPORT */

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
/*
========================================================================
Routine Description:
	Dump URB information.

Arguments:
	purb_org		- the URB

Return Value:
	None

Note:
========================================================================
*/
void dump_urb(VOID *purb_org)
{
	struct urb *purb = (struct urb *)purb_org;

	printk("urb                  :0x%08lx\n", (unsigned long)purb);
	printk("\tdev                   :0x%08lx\n", (unsigned long)purb->dev);
	printk("\t\tdev->state          :0x%d\n", purb->dev->state);
	printk("\tpipe                  :0x%08x\n", purb->pipe);
	printk("\tstatus                :%d\n", purb->status);
	printk("\ttransfer_flags        :0x%08x\n", purb->transfer_flags);
	printk("\ttransfer_buffer       :0x%08lx\n", (unsigned long)purb->transfer_buffer);
	printk("\ttransfer_buffer_length:%d\n", purb->transfer_buffer_length);
	printk("\tactual_length         :%d\n", purb->actual_length);
	printk("\tsetup_packet          :0x%08lx\n", (unsigned long)purb->setup_packet);
	printk("\tstart_frame           :%d\n", purb->start_frame);
	printk("\tnumber_of_packets     :%d\n", purb->number_of_packets);
	printk("\tinterval              :%d\n", purb->interval);
	printk("\terror_count           :%d\n", purb->error_count);
	printk("\tcontext               :0x%08lx\n", (unsigned long)purb->context);
	printk("\tcomplete              :0x%08lx\n\n", (unsigned long)purb->complete);
}
#else
void dump_urb(VOID *purb_org)
{
	return;
}
#endif /* LINUX_VERSION_CODE */






#ifdef OS_ABL_SUPPORT
/*
========================================================================
Routine Description:
	Register a USB driver.

Arguments:
	new_driver		- the driver

Return Value:
	0				- successfully
	Otherwise		- fail

Note:
========================================================================
*/
int rausb_register(VOID * new_driver)
{
	return usb_register((struct usb_driver *)new_driver);
}
EXPORT_SYMBOL(rausb_register);


/*
========================================================================
Routine Description:
	De-Register a USB driver.

Arguments:
	new_driver		- the driver

Return Value:
	None

Note:
========================================================================
*/
void rausb_deregister(VOID * driver)
{
	usb_deregister((struct usb_driver *)driver);
}
EXPORT_SYMBOL(rausb_deregister);


/*
========================================================================
Routine Description:
	Create a new urb for a USB driver to use.

Arguments:
	iso_packets		- number of iso packets for this urb

Return Value:
	the URB

Note:
========================================================================
*/
struct urb *rausb_alloc_urb(int iso_packets)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	return usb_alloc_urb(iso_packets, GFP_ATOMIC);
#else
	return usb_alloc_urb(iso_packets);
#endif /* LINUX_VERSION_CODE */
}
EXPORT_SYMBOL(rausb_alloc_urb);


/*
========================================================================
Routine Description:
	Free the memory used by a urb.

Arguments:
	urb				- the URB

Return Value:
	None

Note:
========================================================================
*/
void rausb_free_urb(VOID *urb)
{
	usb_free_urb((struct urb *)urb);
}
EXPORT_SYMBOL(rausb_free_urb);


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
/*
========================================================================
Routine Description:
	Release a use of the usb device structure.

Arguments:
	dev				- the USB device

Return Value:
	None

Note:
========================================================================
*/
void rausb_put_dev(VOID *dev)
{
	usb_put_dev((struct usb_device *)dev);
}
EXPORT_SYMBOL(rausb_put_dev);


/*
========================================================================
Routine Description:
	Increments the reference count of the usb device structure.

Arguments:
	dev				- the USB device

Return Value:
	the device with the incremented reference counter

Note:
========================================================================
*/
struct usb_device *rausb_get_dev(VOID *dev)
{
	return usb_get_dev((struct usb_device *)dev);
}
EXPORT_SYMBOL(rausb_get_dev);
#endif /* LINUX_VERSION_CODE */


/*
========================================================================
Routine Description:
	Issue an asynchronous transfer request for an endpoint.

Arguments:
	urb				- the URB

Return Value:
	0				- successfully
	Otherwise		- fail

Note:
========================================================================
*/
int rausb_submit_urb(VOID *urb)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	return usb_submit_urb((struct urb *)urb, GFP_ATOMIC);
#else
	return usb_submit_urb((struct urb *)urb);
#endif /* LINUX_VERSION_CODE */
}
EXPORT_SYMBOL(rausb_submit_urb);

/*
========================================================================
Routine Description:
	Allocate dma-consistent buffer.

Arguments:
	dev				- the USB device
	size			- buffer size
	dma				- used to return DMA address of buffer

Return Value:
	a buffer that may be used to perform DMA to the specified device

Note:
========================================================================
*/
void *rausb_buffer_alloc(VOID *dev,
							size_t size,
							ra_dma_addr_t *dma)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	dma_addr_t DmaAddr = (dma_addr_t)(*dma);
	void *buf;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
	buf = usb_alloc_coherent(dev, size, GFP_ATOMIC, &DmaAddr);
#else
	buf = usb_buffer_alloc(dev, size, GFP_ATOMIC, &DmaAddr);
#endif
	*dma = (ra_dma_addr_t)DmaAddr;
	return buf;

#else
	return kmalloc(size, GFP_ATOMIC);
#endif
}
EXPORT_SYMBOL(rausb_buffer_alloc);


/*
========================================================================
Routine Description:
	Free memory allocated with usb_buffer_alloc.

Arguments:
	dev				- the USB device
	size			- buffer size
	addr			- CPU address of buffer
	dma				- used to return DMA address of buffer

Return Value:
	None

Note:
========================================================================
*/
void rausb_buffer_free(VOID *dev,
							size_t size,
							void *addr,
							ra_dma_addr_t dma)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	dma_addr_t DmaAddr = (dma_addr_t)(dma);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
	usb_free_coherent(dev, size, addr, DmaAddr);
#else
	usb_buffer_free(dev, size, addr, DmaAddr);
#endif
#else
	kfree(addr);
#endif
}
EXPORT_SYMBOL(rausb_buffer_free);

/*
========================================================================
Routine Description:
	Send a control message to a device.

Arguments:
	dev				- the USB device

Return Value:
	0				- successfully
	Otherwise		- fail

Note:
========================================================================
*/
int rausb_control_msg(VOID *dev,
						unsigned int pipe,
						__u8 request,
						__u8 requesttype,
						__u16 value,
						__u16 index,
						void *data,
						__u16 size,
						int timeout)
{
	int ret;

	ret = usb_control_msg((struct usb_device *)dev, pipe, request, requesttype, value, index,
							data, size, timeout);
	if (ret == -ENODEV)
		return RTMP_USB_CONTROL_MSG_ENODEV;
	if (ret < 0)
		return RTMP_USB_CONTROL_MSG_FAIL;
	return ret;
}
EXPORT_SYMBOL(rausb_control_msg);

unsigned int rausb_sndctrlpipe(VOID *dev, ULONG address)
{
	return usb_sndctrlpipe(dev, address);
}
EXPORT_SYMBOL(rausb_sndctrlpipe);

unsigned int rausb_rcvctrlpipe(VOID *dev, ULONG address)
{
	return usb_rcvctrlpipe(dev, address);
}
EXPORT_SYMBOL(rausb_rcvctrlpipe);


/*
========================================================================
Routine Description:
	Cancel a transfer request and wait for it to finish.

Arguments:
	urb				- the URB

Return Value:
	None

Note:
========================================================================
*/
void rausb_kill_urb(VOID *urb)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,7)
	usb_kill_urb((struct urb *)urb);
#else
	usb_unlink_urb((struct urb *)urb);
#endif /* LINUX_VERSION_CODE */
}
EXPORT_SYMBOL(rausb_kill_urb);

#endif /* OS_ABL_SUPPORT */


VOID RtmpOsUsbEmptyUrbCheck(
	IN	VOID				**ppWait,
	IN	NDIS_SPIN_LOCK		*pBulkInLock,
	IN	UCHAR				*pPendingRx)
{
	UINT32 i = 0;
	DECLARE_WAIT_QUEUE_HEAD(unlink_wakeup); 
	DECLARE_WAITQUEUE(wait, current);


	/* ensure there are no more active urbs. */
	add_wait_queue (&unlink_wakeup, &wait);
	*ppWait = &unlink_wakeup;

	/* maybe wait for deletions to finish. */
	i = 0;
	/*while((i < 25) && atomic_read(&pAd->PendingRx) > 0) */
	while(i < 25)
	{
/*		unsigned long IrqFlags; */

		RTMP_SEM_LOCK(pBulkInLock);
		if (*pPendingRx == 0)
		{
			RTMP_SEM_UNLOCK(pBulkInLock);
			break;
		}
		RTMP_SEM_UNLOCK(pBulkInLock);
		
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,9)
		msleep(UNLINK_TIMEOUT_MS);	/*Time in millisecond */
#else
		RTMPusecDelay(UNLINK_TIMEOUT_MS*1000);	/*Time in microsecond */
#endif
		i++;
	}
	*ppWait = NULL;
	remove_wait_queue (&unlink_wakeup, &wait); 
}


VOID	RtmpOsUsbInitHTTxDesc(
	IN	VOID			*pUrbSrc,
	IN	VOID			*pUsb_Dev,
	IN	UINT			BulkOutEpAddr,
	IN	PUCHAR			pSrc,
	IN	ULONG			BulkOutSize,
	IN	USB_COMPLETE_HANDLER	Func,
	IN	VOID			*pTxContext,
	IN	ra_dma_addr_t		TransferDma)
{
	PURB pUrb = (PURB)pUrbSrc;
	dma_addr_t DmaAddr = (dma_addr_t)(TransferDma);


	ASSERT(pUrb);

	/*Initialize a tx bulk urb */
	RTUSB_FILL_HTTX_BULK_URB(pUrb,
						pUsb_Dev,
						BulkOutEpAddr,
						pSrc,
						BulkOutSize,
						(usb_complete_t)Func,
						pTxContext,
						DmaAddr);
}


VOID	RtmpOsUsbInitRxDesc(
	IN	VOID			*pUrbSrc,
	IN	VOID			*pUsb_Dev,
	IN	UINT			BulkInEpAddr,
	IN	UCHAR			*pTransferBuffer,
	IN	UINT32			BufSize,
	IN	USB_COMPLETE_HANDLER	Func,
	IN	VOID			*pRxContext,
	IN	ra_dma_addr_t		TransferDma)
{
	PURB pUrb = (PURB)pUrbSrc;
	dma_addr_t DmaAddr = (dma_addr_t)(TransferDma);


	ASSERT(pUrb);

	/*Initialize a rx bulk urb */
	RTUSB_FILL_RX_BULK_URB(pUrb,
						pUsb_Dev,
						BulkInEpAddr,
						pTransferBuffer,
						BufSize,
						(usb_complete_t)Func,
						pRxContext,
						DmaAddr);
}


VOID *RtmpOsUsbContextGet(
	IN	VOID			*pUrb)
{
	return ((purbb_t)pUrb)->rtusb_urb_context;
}


NTSTATUS RtmpOsUsbStatusGet(
	IN	VOID			*pUrb)
{
	return ((purbb_t)pUrb)->rtusb_urb_status;
}


VOID RtmpOsUsbDmaMapping(
	IN	VOID			*pUrb)
{
	RTUSB_URB_DMA_MAPPING(((purbb_t)pUrb));
}


/*
========================================================================
Routine Description:
	Get the data pointer from the URB.

Arguments:
	pUrb			- USB URB

Return Value:
	the data pointer

Note:
========================================================================
*/
VOID *RtmpOsUsbUrbDataGet(
	IN	VOID					*pUrb)
{
	return RTMP_USB_URB_DATA_GET(pUrb);
}


/*
========================================================================
Routine Description:
	Get the status from the URB.

Arguments:
	pUrb			- USB URB

Return Value:
	the status

Note:
========================================================================
*/
NTSTATUS RtmpOsUsbUrbStatusGet(
	IN	VOID					*pUrb)
{
	return RTMP_USB_URB_STATUS_GET(pUrb);
}


/*
========================================================================
Routine Description:
	Get the data length from the URB.

Arguments:
	pUrb			- USB URB

Return Value:
	the data length

Note:
========================================================================
*/
ULONG RtmpOsUsbUrbLenGet(
	IN	VOID					*pUrb)
{
	return RTMP_USB_URB_LEN_GET(pUrb);
}

/*
========================================================================
Routine Description:
	Get USB Vendor ID.

Arguments:
	pUsbDev			- the usb device

Return Value:
	the name

Note:
========================================================================
*/
UINT32 RtmpOsGetUsbDevVendorID(IN VOID *pUsbDev) {
	return ((struct usb_device *) pUsbDev)->descriptor.idVendor;
}

/*
========================================================================
Routine Description:
	Get USB Product ID.

Arguments:
	pUsbDev			- the usb device

Return Value:
	the name

Note:
========================================================================
*/
UINT32 RtmpOsGetUsbDevProductID(IN VOID *pUsbDev) {
	return ((struct usb_device *) pUsbDev)->descriptor.idProduct;
}

#endif /* RTMP_MAC_USB */

