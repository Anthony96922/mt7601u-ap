/*

*/

#include "dot11ac_vht.h"


struct _RTMP_ADAPTER;
struct _RT_PHY_INFO;


void dump_vht_cap(struct _RTMP_ADAPTER *pAd, VHT_CAP_IE *vht_ie);
void dump_vht_op(struct _RTMP_ADAPTER *pAd, VHT_OP_IE *vht_ie);

int build_vht_ies(struct _RTMP_ADAPTER *pAd, unsigned char *buf, unsigned char frm);
int build_vht_cap_ie(RTMP_ADAPTER *pAd, unsigned char *buf);

unsigned char vht_cent_ch_freq(struct _RTMP_ADAPTER *pAd, unsigned char prim_ch);
int vht_mode_adjust(struct _RTMP_ADAPTER *pAd, MAC_TABLE_ENTRY *pEntry, VHT_CAP_IE *cap, VHT_OP_IE *op);
int SetCommonVHT(struct _RTMP_ADAPTER *pAd);
void rtmp_set_vht(struct _RTMP_ADAPTER *pAd, struct _RT_PHY_INFO *phy_info);

void assoc_vht_info_debugshow(
	IN RTMP_ADAPTER *pAd,
	IN MAC_TABLE_ENTRY *pEntry,
	IN VHT_CAP_IE *vht_cap,
	IN VHT_OP_IE *vht_op);

