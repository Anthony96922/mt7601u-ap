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
    dot11ac_vht.h
 
    Abstract:
	Defined IE/frame structures of 802.11ac (D1.2).
 
    Revision History:
    Who        When          What
    ---------  ----------    ----------------------------------------------
    Shiang Tu  01-11-2012    created for 11ac
 */

#ifdef DOT11_VHT_AC


#ifndef __DOT11AC_VHT_H
#define __DOT11AC_VHT_H

#include "rtmp_type.h"


#define IE_VHT_CAP		191
#define IE_VHT_OP		192


/*
	IEEE 802.11AC D2.0, sec 8.4.2.160.2
	VHT Capabilities Info field

	max_mpdu_len: MAximun MPDU Length
		->Indicate the max MPDU length.
				0: 3895 octets(Max A-MSDU length in HT Cap set to 3839)
				1: 7991 octets(Max A-MSDU length in HT Cap set to 7935)
				2: 11454 octets(Max A-MSDU length in HT Cap set to 7935)
				3: reserved
	ch_width: Supported Channel Width Set 
		->Indicates the channel widths supported by the STA.
				0: the STA does not support either 160 or 80+80 MHz
				1: the STA support 160 MHz
				2: the STA support 160 MHz and 80 + 80 MHz
				3: reserved
	rx_ldpc: Rx LDPC
		-> Indicates support of receiving LDPC coded packets
				0: not support
				1: support
	sgi_80M: Short GI for 80 MHz
		-> Indicates short GI support for the reception of VHT+CBW80 packet
				0: not support
				1: support
	sgi_160M: Short GI for 160 and 80 + 80 MHz
		->Indicates rx short GI for VHT+(CBW160 and CBW80+80) packet
				0: not support
				1: support
	tx_stbc: Tx STBC
		-> Indicates support for tx of at least 2x1 STBC
				0: not support
				1: support
	rx_stbc: Rx STBC
		-> Indicates support for rx of PPDUs using STBC
				0: not support
				1: support 1SS
				2: support 1SS and 2SS
				3: support 1SS, 2SS and 3SS
				4: support 1SS, 2SS, 3SS and 4SS
				5,6,7: reserved
	bfer_cap_su: SU Beamformer Capable
		->Indicates support for operation as a single user beamformer
				0: not support
				1: support
	bfee_cap_su: SU Beamformee Capable
		-> Indicates support for operation as a single user beamformee
				0: not support
				1: support				
	cmp_st_num_bfer: Compressed Steering Number of Beamformer Antenna Supported
		-> Beamformee's capability indicateing the max number of beamformer 
			antennas the beamformee can support when sending compressed
			beamforming feedback
				If SU beamformee capable, set to the max value minus 1.
				otehrwise, reserved.
	num_snd_dimension: Number of Sounding Dimensions
		-> Beamformer's capability indicating the number of antennas used for
			beamformed transmissions.
				If SU beamformer capable, set to value minus 1.
				otehrwise, reserved.
	bfer_cap_mu: MU Beamformer Capable
		-> Indicates support for operation as an MU beamformer
				0: if not supported or if sent by a non-AP STA
				1: supported
	bfee_cap_mu: MU Beamformee Capable
		-> Indicates support for operation as an MU beamformer
				0: if not supported or if snet by an AP
				1: supported
	vht_txop_ps: VHT TXOP PS
		-> Indicates whether or not the AP supports VHT TXOP Power Save Mode or
			whether or not the STA is in VHT TXOP Power Save Mode
		->When tx by a VHT AP in the VHT Capabilities element included in Beacon, 
			ProbeResp, AssocResp, and ReassocResp, frames:
				0: if the VHT AP does not support VHT TXOP PS in the BSS.
				1: if the VHT AP support TXOP PS in the BSS.
		->When tx by a VHT non-AP STA in the VHT Capabilities element included
			in AssocReq, ReassocReq and ProbReq frames:
				0: if the VHT STA is not in TXOP Power Save Mode.
				1: if the VHT STA is in TXOP Power Save Mode.
	htc_vht_cap: +HTC-VHT Capable
		-> Indicates whether or not the STA supports receiving an HT Control 
			field in the VHT format
				0: if not support
				1: if support
	max_ampdu_exp: Maximum A-MPDU Length Exponent
		-> Indicates the maximum length of A-MPDU pre-EOF padding that the STA
			can receive.
		->The length defined by this field is equal to 2^(13 + max_ampdu_exp) -1
				0~7 : integer in the range of 0 to 7.
	vht_link_adapt: VHT Link Adaptation Capable
		-> Indicates whether or not the STA support link adaptation using VHT
			variant HT Control field.
		-> This field is ignored if the _HTC-VHT Capble field is set to 0.
				0: (No Feedback), if the STA does not provide VHT MFB
				2: (Unsolicited), if the STA provides only unsolicited VHT MFB
				3: (Both), if the STA can provide VHT MFB in response to VHT MRQ
					and if the STA provides unsolicited VHT MFB.
				1: reserved
	rx_ant_consistency: Rx Antenna Pattern Consistency
		->Indicates the possibility of Rx antenna pattern change
				0: if Rx antenna pattern might change during association
				1: if Rx antenna pattern does not change during association
	tx_ant_consistency: Tx Antenna Pattern Consistency
		->Indicates the possibility of Tx antenna pattern change
				0: if Tx antenna pattern might change during association
				1: if Tx antenna pattern does not change during association
*/
typedef struct GNU_PACKED _VHT_CAP_INFO{
#ifdef RT_BIG_ENDIAN
	unsigned int rsv:2;
	unsigned int tx_ant_consistency:1;
	unsigned int rx_ant_consistency:1;
	unsigned int vht_link_adapt:2;
	unsigned int max_ampdu_exp:3;
	unsigned int htc_vht_cap:1;
	unsigned int vht_txop_ps:1;
	unsigned int bfee_cap_mu:1;
	unsigned int bfer_cap_mu:1;
	unsigned int num_snd_dimension:3;

	unsigned int cmp_st_num_bfer:3;
	unsigned int bfee_cap_su:1;
	unsigned int bfer_cap_su:1;
	unsigned int rx_stbc:3;

	unsigned int tx_stbc:1;
	unsigned int sgi_160M:1;
	unsigned int sgi_80M:1;
	unsigned int rx_ldpc:1;
	unsigned int ch_width:2;
	unsigned int max_mpdu_len:2;
#else
	unsigned int max_mpdu_len:2;	/* 0: 3895, 1: 7991, 2: 11454, 3: rsv */
	unsigned int ch_width:2;	/* */
	unsigned int rx_ldpc:1;
	unsigned int sgi_80M:1;
	unsigned int sgi_160M:1;
	unsigned int tx_stbc:1;

	unsigned int rx_stbc:3;
	unsigned int bfer_cap_su:1;
	unsigned int bfee_cap_su:1;
	unsigned int cmp_st_num_bfer:3;

	unsigned int num_snd_dimension:3;
	unsigned int bfer_cap_mu:1;
	unsigned int bfee_cap_mu:1;
	unsigned int vht_txop_ps:1;
	unsigned int htc_vht_cap:1;
	unsigned int max_ampdu_exp:3;
	unsigned int vht_link_adapt:2;
	unsigned int rx_ant_consistency:1;
	unsigned int tx_ant_consistency:1;	
	unsigned int rsv:2;
#endif /* RT_BIG_ENDIAN */
}VHT_CAP_INFO;


/*
	IEEE 802.11AC D2.0, sec 8.4.2.160.3
	Rx MCS Map and Tx MCS map, figure 8-401bt

	mcs_ss1: Max MCS for 1SS
	mcs_ss2: Max MCS for 2SS
	mcs_ss3: Max MCS for 3SS
	mcs_ss4: Max MCS for 4SS
	mcs_ss5: Max MCS for 5SS
	mcs_ss6: Max MCS for 6SS
	mcs_ss7: Max MCS for 7SS
	mcs_ss8: Max MCS for 8SS

	The 2-bit MAx MCS for n SS field for each number of spatial streams n = 1~8
	is encoded as following:
		0: indicates support for MCS 0~7
		1: indicates support for MCS 0~8
		2: indicates support for MCS 0~9
		3: indicates that n spatial streams is not supported.
	Note: some MCSs are not be valid for particular bandwidth and number of
		spatial stream combinations.
*/
#define VHT_MCS_CAP_7	0
#define VHT_MCS_CAP_8	1
#define VHT_MCS_CAP_9	2
#define VHT_MCS_CAP_NA	3

typedef struct GNU_PACKED _VHT_MCS_MAP{
#ifdef RT_BIG_ENDIAN
	unsigned short mcs_ss8:2;
	unsigned short mcs_ss7:2;
	unsigned short mcs_ss6:2;
	unsigned short mcs_ss5:2;
	unsigned short mcs_ss4:2;
	unsigned short mcs_ss3:2;
	unsigned short mcs_ss2:2;
	unsigned short mcs_ss1:2;
#else
	unsigned short mcs_ss1:2;
	unsigned short mcs_ss2:2;
	unsigned short mcs_ss3:2;
	unsigned short mcs_ss4:2;
	unsigned short mcs_ss5:2;
	unsigned short mcs_ss6:2;
	unsigned short mcs_ss7:2;
	unsigned short mcs_ss8:2;
#endif /* RT_BIG_ENDIAN */
}VHT_MCS_MAP;


/*
	IEEE 802.11AC D2.0, sec 8.4.2.160.3
	VHT Supported MCS Set field, figure 8-401bs

	rx_mcs_map: Rx MCS Map
		-> Indicates the maximum MCS that can be received for each number of
			spatial streams
				See "VHT_MCS_MAP"
	rx_high_rate: Rx Highest Supported Data Rate
		-> Indicates the maximum data rate that the STA can receive
		-> In unit of 1Mb/s where 1 represents 1Mb/s, and incrementing in steps
			of 1 Mb/s.
		-> If the maximum data rate expressed in Mb/s is not an integer, then
			the value is rounded up to the next integer.
	tx_mcs_map: Tx MCS Map
	tx_high_rate: Tx Highest Supported Data Rate
		-> Indicates the maximum data rate that the STA will transmit
		-> In unit of 1Mb/s where 1 represents 1Mb/s, and incrementing in steps
			of 1 Mb/s.
		-> If the maximum data rate expressed in Mb/s is not an integer, then
			the value is rounded up to the next integer.
*/

// TODO: shiang-6590, check the layout of this data structure!!!!
typedef struct GNU_PACKED _VHT_MCS_SET{
#ifdef RT_BIG_ENDIAN
	unsigned short rsv2:3;
	unsigned short tx_high_rate:13;
	struct _VHT_MCS_MAP tx_mcs_map;
	unsigned short rsv:3;
	unsigned short rx_high_rate:13;
	struct _VHT_MCS_MAP rx_mcs_map;	
#else
	struct _VHT_MCS_MAP rx_mcs_map;	
	unsigned short rx_high_rate:13;
	unsigned short rsv:3;
	struct _VHT_MCS_MAP tx_mcs_map;
	unsigned short tx_high_rate:13;
	unsigned short rsv2:3;
#endif /* RT_BIG_ENDIAN */
}VHT_MCS_SET;


/*
	IEEE 802.11AC D2.0, sec 8.4.2.160.1
	VHT Capabilities Element structure

	eid: Element ID
			191   (IE_VHT_CAP)
	len: Length
			12
	vht_cap: VHT Capabilities Info
		->contains a numner of fields that are used to advertise VHT capabilities
			of a VHT STA
	mcs_set: VHT supported MCS Set
		->Used to convey the combinations of MCSs and spatial streams a STA
			supports for both reception and transmission.
*/
typedef struct GNU_PACKED _VHT_CAP_IE{
	VHT_CAP_INFO vht_cap;
	VHT_MCS_SET mcs_set;
}VHT_CAP_IE;


/*
	IEEE 802.11AC D2.0, sec 8.4.2.161
	VHT Operation Information field, figure 8-401bv

	The operation of VHT STAs in the BSS is controlled by the HT Operation
	element and the VHT Operation element.
	
	ch_width: Channel Width
		-> This field, together with the HT Operation element STA Channel Width
			field, defines the BSS operating channel width.
				0: for 20MHz or 40MHz operating channel width
				1: for 80MHz operating channel width
				2: for 160MHz operating channel width
				3: for 80+80MHz operating channel width
				4~255: reserved
	center_freq_1: Channel Center Frequency Segment 1
		-> Defines the channel center frequency for an 80 and 160MHz VHT BSS
			and the segment 1 channel center frequency for an 80+90MHz VHT BSS.
		-> For 80MHZ or 160MHz operating channel width, indicates the channel
			center frequency index for the 80MHz or 160MHz channel on which the
			VHT BSS operates.
		->For 80+80MHz operating channel width, indicates the channel center
			frequency index for the 80MHz channel of frequency segment 1 on
			which the VHT BSS operates.
		->Set 0 for 20MHz or 40MHz operating channel width.
			
	center_freq_2: Channel Center Frequency Segment 2
		-> Defines the seg 2 channel center frequency for an 80+80MHz VHT BSS
		->For a 80+80MHz operating channel width, indicates the channel center
			frequency index of the 80MHz channel of frequency segment 2 on
			which the VHT BSS operates. Reserved otherwise.
*/
typedef struct GNU_PACKED _VHT_OP_INFO{
	unsigned char ch_width;
	unsigned char center_freq_1;
	unsigned char center_freq_2;
}VHT_OP_INFO;


/*
	IEEE 802.11AC D2.0, sec 8.4.2.161
	VHT Operation element, figure 8-401bu

	The operation of VHT STAs in the BSS is controlled by the HT Operation
	element and the VHT Operation element.
	
	eid: Element ID
			192 (IE_VHT_OP)
	len: Length
			5
	vht_op_info: VHT Operation Information
	basic_mcs_set: VHT Basic MCS Set
*/
typedef struct GNU_PACKED _VHT_OP_IE{
	VHT_OP_INFO vht_op_info;
	VHT_MCS_MAP basic_mcs_set;
}VHT_OP_IE;


/*
	IEEE 802.11AC D2.0, sec 8.4.2.163
	Wide Bandwidth Channel Switch element, figure 8-401bx

	included in the Channel Switch Announcement frames.
	
	new_ch_width: New STA Channel Width
	center_freq_1: New Channel Center Frequency Segment 1
	center_freq_2: New Channel Center Frequency Segment 2

	The definition of upper subfields is the same as "VHT_OP_INFO"
*/
typedef struct GNU_PACKED _WIDE_BW_CH_SWITCH_IE{
	unsigned char e_id;
	unsigned int len;
	unsigned char new_ch_width;
	unsigned char center_freq_1;
	unsigned char center_freq_2;
}WIDE_BW_CH_SWITCH_IE;


/*
	IEEE 802.11AC D2.0, sec 8.4.2.164
	VHT Transmit Power Envelope element

	
*/
typedef struct GNU_PACKED _CH_SEG_PAIR{
	unsigned char ch_center_freq;
	unsigned char seg_ch_width;
}CH_SEG_PAIR;


/*
	IEEE 802.11AC D2.0, sec 8.4.2.164
	VHT Transmit Power Envelope element

	max_txpwr: Maximum Transmit Power
		-> Define the maximum transmit power limit of the tx bandwidth defined
			by the VHT Transmit Power Envelop element. The Maximum Transmit
			Power field is a 8 bit 2's complement signed integer in the range of 
			-64 dBm to 63.5 dBm with a 0.5 dB step.

	NOTE: The following two subfields may repeated as needed.
		center_freq_1: Channel Center Frequency Segment
		ch_seg_width: Segment Channel Width
*/
typedef struct GNU_PACKED _VHT_TXPWR_ENV_IE{
	unsigned char e_id;
	unsigned char len;
	unsigned char max_txpwr;
	CH_SEG_PAIR ch_seg_pair[0];
}VHT_TXPWR_ENV_IE;



typedef struct  GNU_PACKED _VHT_CONTROL{
#ifdef RT_BIG_ENDIAN
	unsigned int RDG:1;
	unsigned int ACConstraint:1;
	unsigned int unso_mfb:1;
	unsigned int fb_tx_type:1;
	unsigned int coding:1;
	unsigned int gid_h:3;
	unsigned int mfb_snr:6;
	unsigned int mfb_bw:2;
	unsigned int mfb_mcs:4;
	unsigned int mfb_n_sts:3;
	unsigned int mfsi_gidl:3;
	unsigned int stbc_ind:1;
	unsigned int comp_msi:2;
	unsigned int mrq:1;
	unsigned int rsv:1;
	unsigned int vht:1;
#else
	unsigned int vht:1;
	unsigned int rsv:1;
	unsigned int mrq:1;
	unsigned int comp_msi:2;
	unsigned int stbc_ind:1;
	unsigned int mfsi_gidl:3;
	unsigned int mfb_n_sts:3;
	unsigned int mfb_mcs:4;
	unsigned int mfb_bw:2;
	unsigned int mfb_snr:6;
	unsigned int gid_h:3;
	unsigned int coding:1;
	unsigned int fb_tx_type:1;
	unsigned int unso_mfb:1;
	unsigned int ACConstraint:1;
	unsigned int RDG:1;
#endif
}VHT_CONTROL;


typedef struct GNU_PACKED _NDPA_PKT{
	unsigned short frm_ctrl;
	unsigned short duration;
	unsigned char ra[MAC_ADDR_LEN];
	unsigned char ta[MAC_ADDR_LEN];
	unsigned char snd_seq;
}DNPA_PKT;


	
#endif /* __DOT11AC_VHT_H */

#endif /* DOT11_VHT_AC */

