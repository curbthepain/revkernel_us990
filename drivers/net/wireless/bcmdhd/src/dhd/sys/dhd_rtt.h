/*
 * Header file of Broadcom Dongle Host Driver (DHD)
 * Copyright (C) 1999-2014, Broadcom Corporation
 *
 *      Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 *
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 *
 *      Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
 *
 * $Id: dhd_rtt.h 423669 2014-07-01 13:01:56Z $
 */
#ifndef __DHD_RTT_H__
#define __DHD_RTT_H__

#include "dngl_stats.h"

#define RTT_MAX_TARGET_CNT 10
#define RTT_MAX_FRAME_CNT 25
#define RTT_MAX_RETRY_CNT 10
#define DEFAULT_FTM_CNT 6
#define DEFAULT_RETRY_CNT 6


/*                                                */
#define WL_MAXRATE	108	/*                  */
#define WL_RATE_1M	2	/*                  */
#define WL_RATE_2M	4	/*                  */
#define WL_RATE_5M5	11	/*                  */
#define WL_RATE_11M	22	/*                  */
#define WL_RATE_6M	12	/*                  */
#define WL_RATE_9M	18	/*                  */
#define WL_RATE_12M	24	/*                  */
#define WL_RATE_18M	36	/*                  */
#define WL_RATE_24M	48	/*                  */
#define WL_RATE_36M	72	/*                  */
#define WL_RATE_48M	96	/*                  */
#define WL_RATE_54M	108	/*                  */


enum rtt_role {
	RTT_INITIATOR = 0,
	RTT_TARGET = 1
};
enum rtt_status {
	RTT_STOPPED = 0,
	RTT_STARTED = 1
};
typedef int64_t wifi_timestamp; /*                      */
typedef int64_t wifi_timespan;

typedef enum {
	RTT_INVALID,
	RTT_ONE_WAY,
	RTT_TWO_WAY,
	RTT_AUTO
} rtt_type_t;

typedef enum {
	RTT_PEER_STA,
	RTT_PEER_AP,
	RTT_PEER_P2P,
	RTT_PEER_NAN,
	RTT_PEER_INVALID
} rtt_peer_type_t;

typedef enum rtt_reason {
	RTT_REASON_SUCCESS,
	RTT_REASON_FAILURE,
	RTT_REASON_NO_RSP,
	RTT_REASON_REJECTED,
	RTT_REASON_NOT_SCHEDULED_YET,
	RTT_REASON_TIMEOUT,
	RTT_REASON_AP_ON_DIFF_CH,
	RTT_REASON_AP_NO_CAP,
	RTT_REASON_ABORT
} rtt_reason_t;

typedef enum rtt_capability {
	RTT_CAP_NONE = 0,
	RTT_CAP_ONE_WAY	 = (1 << (0)),
	RTT_CAP_11V_WAY  = (1 << (1)),  /*             */
	RTT_CAP_11MC_WAY  = (1 << (2)), /*              */
	RTT_CAP_VS_WAY = (1 << (3)) /*                      */
} rtt_capability_t;

typedef struct wifi_channel_info {
	wifi_channel_width_t width;
	wifi_channel center_freq; /*                        */
	wifi_channel center_freq0; /*                                 */
	wifi_channel center_freq1; /*                                                    */
} wifi_channel_info_t;

typedef struct wifi_rate {
	uint32 preamble :3; /*                                                */
	uint32 nss		:2; /*                                 */
	uint32 bw		:3; /*                                         */
	/*                                                                  
                               
 */
	uint32 rateMcsIdx :8;
	uint32 reserved :16; /*          */
	uint32 bitrate; /*                  */
} wifi_rate_t;

typedef struct rtt_target_info {
	struct ether_addr addr;
	rtt_type_t type; /*          */
	rtt_peer_type_t peer; /*           */
	wifi_channel_info_t channel; /*                     */
	chanspec_t chanspec; /*                      */
	int8	continuous; /*                                         */
	bool	disable; /*                             */
	uint32	interval; /*                                                              */
	uint32	measure_cnt; /*                                                 */
	uint32	ftm_cnt; /*                                        */
	uint32	retry_cnt; /*                                  */
} rtt_target_info_t;

typedef struct rtt_result {
	struct list_head list;
	uint16 ver;			/*         */
	rtt_target_info_t *target_info; /*             */
	uint16 mode;			/*                        */
	uint16 method;			/*                      */
	uint8  err_code;		/*                      */
	uint8  TOF_type;		/*                        */
	wifi_rate_t tx_rate;           /*         */
	struct ether_addr peer_mac;	/*                          */
	int32 distance;		/*                                */
	uint32 meanrtt;			/*            */
	uint32 modertt;			/*            */
	uint32 medianrtt;		/*            */
	uint32 sdrtt;			/*                           */
	int16  avg_rssi;		/*                                */
	int16  validfrmcnt;		/*                               */
	wifi_timestamp ts; /*                                                             */
	uint16 ftm_cnt;			/*                                                   */
	ftm_sample_t ftm_buff[1];	/*                */
} rtt_result_t;

typedef struct rtt_report {
	struct ether_addr addr;
	uint num_measurement; /*                                                */
	rtt_reason_t status; /*               */
	rtt_type_t type; /*          */
	rtt_peer_type_t peer; /*           */
	wifi_channel_info_t channel; /*                     */
	wifi_rssi  rssi; /*                                 */
	wifi_rssi  rssi_spread; /*                                                       */
	wifi_rate_t tx_rate;           /*         */
	wifi_timespan rtt;	/*                                 */
	wifi_timespan rtt_sd;	/*                                       */
	wifi_timespan rtt_spread; /*                                                   */
	int32 distance; /*                           */
	int32 distance_sd; /*                                     */
	int32 distance_spread;	/*                                                             */
	wifi_timestamp ts; /*                                                      */
} rtt_report_t;

/*                  */
typedef struct rtt_capabilities {
	uint8 rtt_one_sided_supported;	/*                                             */
	uint8 rtt_11v_supported;		/*                                         */
	uint8 rtt_ftm_supported;	/*                                         */
	uint8 rtt_vs_supported;		/*                                                 */
} rtt_capabilities_t;

typedef struct rtt_config_params {
	int8 rtt_target_cnt;
	rtt_target_info_t target_info[RTT_MAX_TARGET_CNT];
} rtt_config_params_t;

typedef void (*dhd_rtt_compl_noti_fn)(void *ctx, void *rtt_data);
/*                                              */
int dhd_dev_rtt_set_cfg(struct net_device *dev, void *buf);

int dhd_dev_rtt_cancel_cfg(struct net_device *dev, struct ether_addr *mac_list, int mac_cnt);

int dhd_dev_rtt_register_noti_callback(struct net_device *dev,
	void *ctx, dhd_rtt_compl_noti_fn noti_fn);

int dhd_dev_rtt_unregister_noti_callback(struct net_device *dev, dhd_rtt_compl_noti_fn noti_fn);

int dhd_dev_rtt_capability(struct net_device *dev, rtt_capabilities_t *capa);

/*                       */
chanspec_t dhd_rtt_convert_to_chspec(wifi_channel_info_t channel);

int dhd_rtt_set_cfg(dhd_pub_t *dhd, rtt_config_params_t *params);

int dhd_rtt_stop(dhd_pub_t *dhd, struct ether_addr *mac_list, int mac_cnt);

int dhd_rtt_register_noti_callback(dhd_pub_t *dhd, void *ctx, dhd_rtt_compl_noti_fn noti_fn);

int dhd_rtt_unregister_noti_callback(dhd_pub_t *dhd, dhd_rtt_compl_noti_fn noti_fn);

int dhd_rtt_event_handler(dhd_pub_t *dhd, wl_event_msg_t *event, void *event_data);

int dhd_rtt_capability(dhd_pub_t *dhd, rtt_capabilities_t *capa);

int dhd_rtt_init(dhd_pub_t *dhd);

int dhd_rtt_deinit(dhd_pub_t *dhd);
#endif /*               */
