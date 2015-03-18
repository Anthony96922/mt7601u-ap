/*

*/


#include "rt_config.h"


INT get_pkt_phymode_by_rxwi(RXWI_STRUC *rxwi)
{
	return rxwi->RXWI_O.phy_mode;
}

INT get_pkt_rssi_by_rxwi(struct _RTMP_ADAPTER *pAd, RXWI_STRUC *rxwi, INT size, CHAR *rssi)
{
	switch (size) {
		case 3:
			rssi[2] = rxwi->RxWIRSSI2;
		case 2:
			rssi[1] = rxwi->RxWIRSSI1;
		case 1:
		default:
#ifdef MT7601
			if ( IS_MT7601(pAd) )
				rssi[0] = rxwi->RxWISNR2;
			else
#endif /* MT7601 */
				rssi[0] = rxwi->RxWIRSSI0;
			break;
	}

	return 0;
}


INT get_pkt_snr_by_rxwi(struct _RTMP_ADAPTER *pAd, RXWI_STRUC *rxwi, INT size, UCHAR *snr)
{

#ifdef MT7601
	/*
		snr[2] = Gain Report 
		snr[1] = Antenna report
	*/
		
	if ( IS_MT7601(pAd) )
	{
		snr[0] = rxwi->RxWISNR0;
		snr[1] = 0;
		snr[2] = 0;
		return 0;
	}
#endif /* MT7601 */

	switch (size) {
		case 3:
			snr[2] = rxwi->RxWISNR2;
		case 2:
			snr[1] = rxwi->RxWISNR1;
		case 1:
		default:
			snr[0] = rxwi->RxWISNR0;
			break;
	}
	
	return 0;
}

