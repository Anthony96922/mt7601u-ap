/*

*/

#ifdef RLT_RF

#include "rt_config.h"


NDIS_STATUS rlt_rf_write(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR bank,
	IN UCHAR regID,
	IN UCHAR value)
{
	RLT_RF_CSR_CFG rfcsr = { { 0 } };
	UINT i = 0;
	NDIS_STATUS	 ret;

#ifdef MT7601FPGA
	return;
#endif /*MT7601FPGA */


	if (pAd->WlanFunCtrl.field.WLAN_EN == 0)
	{
		DBGPRINT_ERR(("rlt_rf_write. Not allow to write RF 0x%x : fail\n",  regID));	
		return STATUS_UNSUCCESSFUL;
	}

#ifdef RTMP_MAC_USB
	if (IS_USB_INF(pAd)) {
		RTMP_SEM_EVENT_WAIT(&pAd->reg_atomic, ret);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("reg_atomic get failed(ret=%d)\n", ret));
			return STATUS_UNSUCCESSFUL;
		}
	}
#endif /* RTMP_MAC_USB */

	ASSERT((regID <= pAd->chipCap.MaxNumOfRfId));

	ret = STATUS_UNSUCCESSFUL;
	do
	{
		RTMP_IO_READ32(pAd, RF_CSR_CFG, &rfcsr.word);

		if (!rfcsr.field.RF_CSR_KICK)
			break;
		i++;
	}
	while ((i < MAX_BUSY_COUNT) && (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)));

	if ((i == MAX_BUSY_COUNT) || (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)))
	{
		DBGPRINT_RAW(RT_DEBUG_ERROR, ("Retry count exhausted or device removed!!!\n"));
		goto done;
	}

	rfcsr.field.RF_CSR_WR = 1;
	rfcsr.field.RF_CSR_KICK = 1;
	rfcsr.field.RF_CSR_REG_BANK = bank;
	rfcsr.field.RF_CSR_REG_ID = regID;


	rfcsr.field.RF_CSR_DATA = value;
	RTMP_IO_WRITE32(pAd, RF_CSR_CFG, rfcsr.word);

	ret = NDIS_STATUS_SUCCESS;
done:


#ifdef RTMP_MAC_USB
	if (IS_USB_INF(pAd)) {
		RTMP_SEM_EVENT_UP(&pAd->reg_atomic);
	}
#endif /* RTMP_MAC_USB */

	return ret;
}


/*
	========================================================================
	
	Routine Description: Read RT30xx RF register through MAC

	Arguments:

	Return Value:

	IRQL = 
	
	Note:
	
	========================================================================
*/
NDIS_STATUS rlt_rf_read(
	IN RTMP_ADAPTER *pAd,
	IN UCHAR bank,
	IN UCHAR regID,
	IN UCHAR *pValue)
{
	RLT_RF_CSR_CFG rfcsr = { { 0 } };
	UINT i=0, k=0;
	NDIS_STATUS	 ret = STATUS_UNSUCCESSFUL;


	if (pAd->WlanFunCtrl.field.WLAN_EN == 0)
	{
		DBGPRINT_ERR(("rlt_rf_read. Not allow to read RF 0x%x : fail\n",  regID));	
		return STATUS_UNSUCCESSFUL;
	}

#ifdef RTMP_MAC_USB
	if (IS_USB_INF(pAd)) {
		RTMP_SEM_EVENT_WAIT(&pAd->reg_atomic, i);
		if (i != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("reg_atomic get failed(ret=%d)\n", i));
			return STATUS_UNSUCCESSFUL;
		}
	}
#endif /* RTMP_MAC_USB */

	ASSERT((regID <= pAd->chipCap.MaxNumOfRfId));

	for (i=0; i<MAX_BUSY_COUNT; i++)
	{
		if(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))
			goto done;
			
		RTMP_IO_READ32(pAd, RF_CSR_CFG, &rfcsr.word);

		if (rfcsr.field.RF_CSR_KICK == BUSY)
				continue;
		
		rfcsr.word = 0;
		rfcsr.field.RF_CSR_WR = 0;
		rfcsr.field.RF_CSR_KICK = 1;
		rfcsr.field.RF_CSR_REG_ID = regID;
		rfcsr.field.RF_CSR_REG_BANK = bank;
		RTMP_IO_WRITE32(pAd, RF_CSR_CFG, rfcsr.word);
		
		for (k=0; k<MAX_BUSY_COUNT; k++)
		{
			if(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))
				goto done;
				
			RTMP_IO_READ32(pAd, RF_CSR_CFG, &rfcsr.word);

			if (rfcsr.field.RF_CSR_KICK == IDLE)
				break;
		}
		
		if ((rfcsr.field.RF_CSR_KICK == IDLE) &&
			(rfcsr.field.RF_CSR_REG_ID == regID) &&
			(rfcsr.field.RF_CSR_REG_BANK == bank))
		{
			*pValue = (UCHAR)(rfcsr.field.RF_CSR_DATA);
			break;
		}
	}

	if (rfcsr.field.RF_CSR_KICK == BUSY)
	{																	
		DBGPRINT_ERR(("RF read R%d=0x%X fail, i[%d], k[%d]\n", regID, rfcsr.word,i,k));
		goto done;
	}
	ret = STATUS_SUCCESS;

done:
#ifdef RTMP_MAC_USB
	if (IS_USB_INF(pAd)) {
		RTMP_SEM_EVENT_UP(&pAd->reg_atomic);
	}
#endif /* RTMP_MAC_USB */

	return ret;
}

#endif /* RLT_RF */

