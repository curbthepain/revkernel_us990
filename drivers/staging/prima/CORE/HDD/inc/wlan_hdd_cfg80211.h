/*
 * Copyright (c) 2012-2014 The Linux Foundation. All rights reserved.
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * This file was originally distributed by Qualcomm Atheros, Inc.
 * under proprietary terms before Copyright ownership was assigned
 * to the Linux Foundation.
 */

#if !defined( HDD_CFG80211_H__ )
#define HDD_CFG80211_H__


/*                                                                            

                            

                                        

                                                                            */

/*          */


//                                                                   
#define GAS_INITIAL_REQ "\x04\x0a"
#define GAS_INITIAL_REQ_SIZE 2

#define GAS_INITIAL_RSP "\x04\x0b"
#define GAS_INITIAL_RSP_SIZE 2

#define GAS_COMEBACK_REQ "\x04\x0c"
#define GAS_COMEBACK_REQ_SIZE 2

#define GAS_COMEBACK_RSP "\x04\x0d"
#define GAS_COMEBACK_RSP_SIZE 2

#define P2P_PUBLIC_ACTION_FRAME "\x04\x09\x50\x6f\x9a\x09"
#define P2P_PUBLIC_ACTION_FRAME_SIZE 6

#define P2P_ACTION_FRAME "\x7f\x50\x6f\x9a\x09"
#define P2P_ACTION_FRAME_SIZE 5

#define SA_QUERY_FRAME_REQ "\x08\x00"
#define SA_QUERY_FRAME_REQ_SIZE 2

#define SA_QUERY_FRAME_RSP "\x08\x01"
#define SA_QUERY_FRAME_RSP_SIZE 2

#define HDD_P2P_WILDCARD_SSID "DIRECT-" //                            
#define HDD_P2P_WILDCARD_SSID_LEN 7

#define WNM_BSS_ACTION_FRAME "\x0a\x07"
#define WNM_BSS_ACTION_FRAME_SIZE 2

#define WNM_NOTIFICATION_FRAME "\x0a\x1a"
#define WNM_NOTIFICATION_FRAME_SIZE 2

#define WPA_OUI_TYPE   "\x00\x50\xf2\x01"
#define BLACKLIST_OUI_TYPE   "\x00\x50\x00\x00"
#define WHITELIST_OUI_TYPE   "\x00\x50\x00\x01"
#define WPA_OUI_TYPE_SIZE  4

#define WLAN_BSS_MEMBERSHIP_SELECTOR_HT_PHY 127
#define BASIC_RATE_MASK   0x80
#define RATE_MASK         0x7f

#ifdef WLAN_ENABLE_AGEIE_ON_SCAN_RESULTS
/*                             */
#define QCOM_VENDOR_IE_ID 221
#define QCOM_OUI1         0x00
#define QCOM_OUI2         0xA0
#define QCOM_OUI3         0xC6
#define QCOM_VENDOR_IE_AGE_TYPE  0x100
#define QCOM_VENDOR_IE_AGE_LEN   11

#ifdef FEATURE_WLAN_TDLS
#define WLAN_IS_TDLS_SETUP_ACTION(action) \
         ((SIR_MAC_TDLS_SETUP_REQ <= action) && (SIR_MAC_TDLS_SETUP_CNF >= action))
#if !defined (TDLS_MGMT_VERSION2)
#define TDLS_MGMT_VERSION2 0
#endif
#endif

#define MAX_CHANNEL MAX_2_4GHZ_CHANNEL + NUM_5GHZ_CHANNELS

typedef struct {
   u8 element_id;
   u8 len;
   u8 oui_1;
   u8 oui_2;
   u8 oui_3;
   u32 type;
   u32 age;
}__attribute__((packed)) qcom_ie_age ;
#endif

enum qca_nl80211_vendor_subcmds {
    QCA_NL80211_VENDOR_SUBCMD_UNSPEC = 0,
    QCA_NL80211_VENDOR_SUBCMD_TEST = 1,

    /*                                */
    QCA_NL80211_VENDOR_SUBCMD_ROAMING = 9,

    QCA_NL80211_VENDOR_SUBCMD_AVOID_FREQUENCY = 10,
    QCA_NL80211_VENDOR_SUBCMD_DFS_CAPABILITY =  11,
    QCA_NL80211_VENDOR_SUBCMD_NAN =  12,
    QCA_NL80211_VENDOR_SUBCMD_STATS_EXT = 13,

    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_SET = 14,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_GET = 15,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_CLR = 16,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_RADIO_RESULTS = 17,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_IFACE_RESULTS = 18,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_PEERS_RESULTS = 19,
    /*                                    */
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_START = 20,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_STOP = 21,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_GET_VALID_CHANNELS = 22,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_GET_CAPABILITIES = 23,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_GET_CACHED_RESULTS = 24,
    /*                                                      */
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SCAN_RESULTS_AVAILABLE = 25,
    /*                                                              
                                                         
      */
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_FULL_SCAN_RESULT = 26,
    /*                                               */
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SCAN_EVENT = 27,
    /*                          */
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_HOTLIST_AP_FOUND = 28,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SET_BSSID_HOTLIST = 29,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_RESET_BSSID_HOTLIST = 30,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SIGNIFICANT_CHANGE = 31,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SET_SIGNIFICANT_CHANGE = 32,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_RESET_SIGNIFICANT_CHANGE = 33,
    /*          */
    QCA_NL80211_VENDOR_SUBCMD_TDLS_ENABLE = 34,
    QCA_NL80211_VENDOR_SUBCMD_TDLS_DISABLE = 35,
    QCA_NL80211_VENDOR_SUBCMD_TDLS_GET_STATUS = 36,
    QCA_NL80211_VENDOR_SUBCMD_TDLS_STATE = 37,
    /*                        */
    QCA_NL80211_VENDOR_SUBCMD_GET_SUPPORTED_FEATURES = 38,
    QCA_NL80211_VENDOR_SUBCMD_MAC_OUI = 39,
    /*                */
    QCA_NL80211_VENDOR_SUBCMD_NO_DFS_FLAG = 40,
    /*                        */
    QCA_NL80211_VENDOR_SUBCMD_GET_CONCURRENCY_MATRIX = 42
};

enum qca_nl80211_vendor_subcmds_index {
#ifdef FEATURE_WLAN_CH_AVOID
    QCA_NL80211_VENDOR_SUBCMD_AVOID_FREQUENCY_INDEX,
#endif /*                       */
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_SET_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_GET_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_LL_STATS_CLR_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_LL_RADIO_STATS_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_LL_IFACE_STATS_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_LL_PEER_INFO_STATS_INDEX,
    /*                */
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_START_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_STOP_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_GET_CAPABILITIES_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_GET_CACHED_RESULTS_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SCAN_RESULTS_AVAILABLE_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_FULL_SCAN_RESULT_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SCAN_EVENT_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_HOTLIST_AP_FOUND_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SET_BSSID_HOTLIST_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_RESET_BSSID_HOTLIST_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SIGNIFICANT_CHANGE_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_SET_SIGNIFICANT_CHANGE_INDEX,
    QCA_NL80211_VENDOR_SUBCMD_EXTSCAN_RESET_SIGNIFICANT_CHANGE_INDEX,
    /*        */
    QCA_NL80211_VENDOR_SUBCMD_TDLS_STATE_CHANGE_INDEX,
};

enum qca_wlan_vendor_attr
{
    QCA_WLAN_VENDOR_ATTR_INVALID = 0,
    /*                                                  */
    QCA_WLAN_VENDOR_ATTR_DFS     = 1,
    /*                                       */
    QCA_WLAN_VENDOR_ATTR_NAN     = 2,
    /*                                             */
    QCA_WLAN_VENDOR_ATTR_STATS_EXT     = 3,
    /*                                             */
    QCA_WLAN_VENDOR_ATTR_IFINDEX     = 4,
    /*                                                                   
                                   */
    QCA_WLAN_VENDOR_ATTR_ROAMING_POLICY = 5,
    QCA_WLAN_VENDOR_ATTR_MAC_ADDR = 6,
    /*           */
    QCA_WLAN_VENDOR_ATTR_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_MAX = QCA_WLAN_VENDOR_ATTR_AFTER_LAST - 1,
};

/*        */
enum qca_wlan_vendor_attr_tdls_enable
{
    QCA_WLAN_VENDOR_ATTR_TDLS_ENABLE_INVALID = 0,
    /*                                      */
    QCA_WLAN_VENDOR_ATTR_TDLS_ENABLE_MAC_ADDR,
    /*                                                        */
    QCA_WLAN_VENDOR_ATTR_TDLS_ENABLE_CHANNEL,
    QCA_WLAN_VENDOR_ATTR_TDLS_ENABLE_GLOBAL_OPERATING_CLASS,
    QCA_WLAN_VENDOR_ATTR_TDLS_ENABLE_MAX_LATENCY_MS,
    QCA_WLAN_VENDOR_ATTR_TDLS_ENABLE_MIN_BANDWIDTH_KBPS,
    /*           */
    QCA_WLAN_VENDOR_ATTR_TDLS_ENABLE_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_TDLS_ENABLE_MAX =
         QCA_WLAN_VENDOR_ATTR_TDLS_ENABLE_AFTER_LAST - 1,
};

enum qca_wlan_vendor_attr_tdls_disable
{
    QCA_WLAN_VENDOR_ATTR_TDLS_DISABLE_INVALID = 0,
    /*                                      */
    QCA_WLAN_VENDOR_ATTR_TDLS_DISABLE_MAC_ADDR,
    /*           */
    QCA_WLAN_VENDOR_ATTR_TDLS_DISABLE_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_TDLS_DISABLE_MAX =
       QCA_WLAN_VENDOR_ATTR_TDLS_DISABLE_AFTER_LAST - 1,
};

enum qca_wlan_vendor_attr_tdls_get_status
{
    QCA_WLAN_VENDOR_ATTR_TDLS_GET_STATUS_INVALID = 0,
    /*                                      */
    QCA_WLAN_VENDOR_ATTR_TDLS_GET_STATUS_MAC_ADDR,
    /*                                                        */
    QCA_WLAN_VENDOR_ATTR_TDLS_GET_STATUS_STATE,
    QCA_WLAN_VENDOR_ATTR_TDLS_GET_STATUS_REASON,
    QCA_WLAN_VENDOR_ATTR_TDLS_GET_STATUS_CHANNEL,
    QCA_WLAN_VENDOR_ATTR_TDLS_GET_STATUS_GLOBAL_OPERATING_CLASS,
    /*           */
    QCA_WLAN_VENDOR_ATTR_TDLS_GET_STATUS_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_TDLS_GET_STATUS_MAX =
      QCA_WLAN_VENDOR_ATTR_TDLS_GET_STATUS_AFTER_LAST - 1,
};

enum qca_wlan_vendor_attr_tdls_state
{
    QCA_WLAN_VENDOR_ATTR_TDLS_STATE_INVALID = 0,
    /*                                      */
    QCA_WLAN_VENDOR_ATTR_TDLS_STATE_MAC_ADDR,
    /*                                                        */
    QCA_WLAN_VENDOR_ATTR_TDLS_NEW_STATE,
    QCA_WLAN_VENDOR_ATTR_TDLS_STATE_REASON,
    QCA_WLAN_VENDOR_ATTR_TDLS_STATE_CHANNEL,
    QCA_WLAN_VENDOR_ATTR_TDLS_STATE_GLOBAL_OPERATING_CLASS,
    /*           */
    QCA_WLAN_VENDOR_ATTR_TDLS_STATE_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_TDLS_STATE_MAX =
        QCA_WLAN_VENDOR_ATTR_TDLS_STATE_AFTER_LAST - 1,
};


#ifdef WLAN_FEATURE_LINK_LAYER_STATS

enum qca_wlan_vendor_attr_ll_stats_set
{
    QCA_WLAN_VENDOR_ATTR_LL_STATS_SET_INVALID = 0,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_SET_CONFIG_MPDU_SIZE_THRESHOLD = 1,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_SET_CONFIG_AGGRESSIVE_STATS_GATHERING,
    /*           */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_SET_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_SET_MAX =
                    QCA_WLAN_VENDOR_ATTR_LL_STATS_SET_AFTER_LAST - 1,
};

enum qca_wlan_vendor_attr_ll_stats_get
{
    QCA_WLAN_VENDOR_ATTR_LL_STATS_GET_INVALID = 0,
    /*                                                                  
                                                                          
                                                                     
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_GET_CONFIG_REQ_ID,
    /*                                                 
                                              
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_GET_CONFIG_REQ_MASK,
    /*           */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_GET_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_GET_MAX =
                  QCA_WLAN_VENDOR_ATTR_LL_STATS_GET_AFTER_LAST - 1,
};

enum qca_wlan_vendor_attr_ll_stats_clr
{
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CLR_INVALID = 0,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CLR_CONFIG_REQ_MASK,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CLR_CONFIG_STOP_REQ,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CLR_CONFIG_RSP_MASK,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CLR_CONFIG_STOP_RSP,

    QCA_WLAN_VENDOR_ATTR_LL_STATS_CLR_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CLR_MAX       =
                        QCA_WLAN_VENDOR_ATTR_LL_STATS_CLR_AFTER_LAST - 1,
};

enum qca_wlan_vendor_attr_ll_stats_results
{
    QCA_WLAN_VENDOR_ATTR_LL_STATS_INVALID = 0,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RESULTS_REQ_ID = 1,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_BEACON_RX,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_MGMT_RX,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_MGMT_ACTION_RX,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_MGMT_ACTION_TX,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_RSSI_MGMT,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_RSSI_DATA,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_RSSI_ACK,
    /*                                                                  
                                         
     */

    /*                                              
                                       */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_INFO_MODE,
    /*                                                    */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_INFO_MAC_ADDR,
    /*                                   
                                               
                               
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_INFO_STATE,
    /*                                            
                                                         
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_INFO_ROAMING,
    /*                                           */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_INFO_CAPABILITIES,
    /*                                                           */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_INFO_SSID,
    /*                                           */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_INFO_BSSID,
    /*                                                                     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_INFO_AP_COUNTRY_STR,
    /*                                                                        */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_INFO_COUNTRY_STR,

    /*                                                                
                                            
     */

    /*                                                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_AC,
    /*                                                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_TX_MPDU,
    /*                                                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_RX_MPDU,
    /*                                                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_TX_MCAST,
    /*                                                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_RX_MCAST,
    /*                                                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_RX_AMPDU,
    /*                                                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_TX_AMPDU,
    /*                                                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_MPDU_LOST,
    /*                                                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_RETRIES,
    /*                                                       */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_RETRIES_SHORT,
    /*                                                       */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_RETRIES_LONG,
    /*                                                       */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_CONTENTION_TIME_MIN,
    /*                                                       */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_CONTENTION_TIME_MAX,
    /*                                                       */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_CONTENTION_TIME_AVG,
    /*                                                       */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_AC_CONTENTION_NUM_SAMPLES,
    /*                                       */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_IFACE_NUM_PEERS,

    /*                                                                 
                                         
     */

    /*                                                                   */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_PEER_INFO_TYPE,
    /*                                           
                                           
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_PEER_INFO_MAC_ADDRESS,
    /*                                                   
                                        
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_PEER_INFO_CAPABILITIES,
    /*                                       */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_PEER_INFO_NUM_RATES,

    /*                                         */
    /*                     */
    /*                                                                   */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RATE_PREAMBLE,
    /*                                                     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RATE_NSS,
    /*                                                              */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RATE_BW,
    /*                                                                     
                                                            */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RATE_MCS_INDEX,

    /*                                                    */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RATE_BIT_RATE,

    /*                                                                 
                                         
     */

    /*                                                                        
                                                                      
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RATE_TX_MPDU,
    /*                                                       
                                             */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RATE_RX_MPDU,
    /*                                                            
                                                             
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RATE_MPDU_LOST,
    /*                                                               
                             
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RATE_RETRIES,
    /*                                                                     
                           */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RATE_RETRIES_SHORT,
    /*                                                                    
                           
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RATE_RETRIES_LONG,

    QCA_WLAN_VENDOR_ATTR_LL_STATS_RADIO_ID,
    /*                                                               
                           
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RADIO_ON_TIME,
    /*                                                         
                                       
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RADIO_TX_TIME,
    /*                                                         
                                            
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RADIO_RX_TIME,
    /*                                                         
                                                
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RADIO_ON_TIME_SCAN,
    /*                                                         
                                             
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RADIO_ON_TIME_NBD,
    /*                                                         
                                               
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RADIO_ON_TIME_EXTSCAN,
    /*                                                         
                                                 
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RADIO_ON_TIME_ROAM_SCAN,
    /*                                                         
                                                
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RADIO_ON_TIME_PNO_SCAN,
    /*                                                         
                                                                    
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RADIO_ON_TIME_HS20,
    /*                                           */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RADIO_NUM_CHANNELS,

    /*                                                               
                                                
     */

    /*                                                                      */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CHANNEL_INFO_WIDTH,
    /*                                              */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CHANNEL_INFO_CENTER_FREQ,
    /*                                                             */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CHANNEL_INFO_CENTER_FREQ0,
    /*                                                              */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CHANNEL_INFO_CENTER_FREQ1,

    /*                                                                   
                                     
     */

    /*                                                                      
                                                                    
                              
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CHANNEL_ON_TIME,
    /*                                                                
                                                                     
                          
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CHANNEL_CCA_BUSY_TIME,


    QCA_WLAN_VENDOR_ATTR_LL_STATS_NUM_RADIOS,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_CH_INFO,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_PEER_INFO,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_PEER_INFO_RATE_INFO,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_WMM_INFO,

    /*                                                                        
                                                                  
                                                          
                                 
     */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_RESULTS_MORE_DATA,

    /*           */
    QCA_WLAN_VENDOR_ATTR_LL_STATS_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_LL_STATS_MAX   =
                        QCA_WLAN_VENDOR_ATTR_LL_STATS_AFTER_LAST -1,
};


#endif /*                               */

#ifdef WLAN_FEATURE_EXTSCAN

enum qca_wlan_vendor_attr_extscan_config_params
{
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_SUBCMD_CONFIG_PARAM_INVALID = 0,

    /*                                                                         
                                                                        
                                     
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_SUBCMD_CONFIG_PARAM_REQUEST_ID,

    /*                               
                                                                         
      */
    /*                       */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_GET_VALID_CHANNELS_CONFIG_PARAM_WIFI_BAND,
    /*                       */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_GET_VALID_CHANNELS_CONFIG_PARAM_MAX_CHANNELS,

    /*                                       
                                                            
      */

    /*                                          */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_CHANNEL_SPEC_CHANNEL,
    /*                                          */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_CHANNEL_SPEC_DWELL_TIME,
    /*                                                          */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_CHANNEL_SPEC_PASSIVE,
    /*                                     */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_CHANNEL_SPEC_CLASS,

    /*                                             */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_BUCKET_SPEC_INDEX,
    /*                             */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_BUCKET_SPEC_BAND,
    /*                                               */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_BUCKET_SPEC_PERIOD,
    /*                                                */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_BUCKET_SPEC_REPORT_EVENTS,
    /*                                                     
                                        
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_BUCKET_SPEC_NUM_CHANNEL_SPECS,

    /*                                                                 
                                                                              
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_CHANNEL_SPEC,

    /*                                                 */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_SCAN_CMD_PARAMS_BASE_PERIOD,
    /*                                                                  
                                                              
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_SCAN_CMD_PARAMS_MAX_AP_PER_SCAN,
    /*                                                                        
             
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_SCAN_CMD_PARAMS_REPORT_THRESHOLD,
    /*                                                                        
                                                                             
                                           
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_SCAN_CMD_PARAMS_NUM_BUCKETS,

    /*                                                                
                                                                            
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_BUCKET_SPEC,

    /*                      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_GET_CACHED_SCAN_RESULTS_CONFIG_PARAM_FLUSH,
    /*                                                                  */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_GET_CACHED_SCAN_RESULTS_CONFIG_PARAM_MAX,

    /*                                      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_AP_THRESHOLD_PARAM_BSSID,
    /*                     */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_AP_THRESHOLD_PARAM_RSSI_LOW,
    /*                     */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_AP_THRESHOLD_PARAM_RSSI_HIGH,
    /*                       */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_AP_THRESHOLD_PARAM_CHANNEL,


    /*                                                                     
                                                                         
                                     
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_BSSID_HOTLIST_PARAMS_NUM_AP,

    /*                                                                       
                                                                              
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_AP_THRESHOLD_PARAM,

    /*                                                             */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_SIGNIFICANT_CHANGE_PARAMS_RSSI_SAMPLE_SIZE,
    /*                                                             */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_SIGNIFICANT_CHANGE_PARAMS_LOST_AP_SAMPLE_SIZE,
    /*                                                          */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_SIGNIFICANT_CHANGE_PARAMS_MIN_BREACHING,
    /*                                                             
                                                                   
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_SIGNIFICANT_CHANGE_PARAMS_NUM_AP,

    /*           */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_SUBCMD_CONFIG_PARAM_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_SUBCMD_CONFIG_PARAM_MAX =
        QCA_WLAN_VENDOR_ATTR_EXTSCAN_SUBCMD_CONFIG_PARAM_AFTER_LAST - 1,

};

enum qca_wlan_vendor_attr_extscan_results
{
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_INVALID = 0,

    /*                                                                       
                                          
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_REQUEST_ID,

    /*                                                                 
                                                   
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_STATUS,

    /*                                   */
    /*                                                              
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_NUM_CHANNELS,
    /*                                                                      
                       
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_CHANNELS,

    /*                                 */
    /*                      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_CAPABILITIES_MAX_SCAN_CACHE_SIZE,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_CAPABILITIES_MAX_SCAN_BUCKETS,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_CAPABILITIES_MAX_AP_CACHE_PER_SCAN,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_CAPABILITIES_MAX_RSSI_SAMPLE_SIZE,
    /*                    */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_CAPABILITIES_MAX_SCAN_REPORTING_THRESHOLD,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_CAPABILITIES_MAX_HOTLIST_APS,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_CAPABILITIES_MAX_SIGNIFICANT_WIFI_CHANGE_APS,
    /*                      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_CAPABILITIES_MAX_BSSID_HISTORY_ENTRIES,

    /*                             
                                                                            
     */

    /*                       */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_NUM_RESULTS_AVAILABLE,


    /*                             
                                                                       
      */

    /*                                    
                                                         
     */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_LIST,

    /*                                                               */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_RESULT_TIME_STAMP,
    /*                                                */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_RESULT_SSID,
    /*                                      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_RESULT_BSSID,
    /*                                                 */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_RESULT_CHANNEL,
    /*                     */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_RESULT_RSSI,
    /*                       */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_RESULT_RTT,
    /*                       */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_RESULT_RTT_SD,
    /*                       */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_RESULT_BEACON_PERIOD,
    /*                       */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_RESULT_CAPABILITY,
    /*                                                 */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_RESULT_IE_LENGTH,
    /*                                                              
                                                                      
                                                                            
     */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_RESULT_IE_DATA,
    /*                                                                      
                 
     */
    /*                                                                      
                  
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_RESULT_MORE_DATA,

    /*                       
                                                                 
      */
    /*                      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_EVENT_TYPE,
    /*                       */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SCAN_EVENT_STATUS,

    /*                       
                                                                       
      */
    /*                                                                    
                                      
      */

    /*                       
                                                                         
      */
    /*                                      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SIGNIFICANT_CHANGE_RESULT_BSSID,
    /*                       */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SIGNIFICANT_CHANGE_RESULT_CHANNEL,
    /*                                                                     
                                                                 
      */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SIGNIFICANT_CHANGE_RESULT_NUM_RSSI,
    /*                                                                  
                                                                     
     */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_SIGNIFICANT_CHANGE_RESULT_RSSI_LIST,

    /*           */
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_MAX =
        QCA_WLAN_VENDOR_ATTR_EXTSCAN_RESULTS_AFTER_LAST - 1,
};

#endif /*                      */
enum qca_wlan_vendor_attr_set_scanning_mac_oui{
    QCA_WLAN_VENDOR_ATTR_SET_SCANNING_MAC_OUI_INVALID = 0,
    QCA_WLAN_VENDOR_ATTR_SET_SCANNING_MAC_OUI = 1,
    /*           */
    QCA_WLAN_VENDOR_ATTR_SET_SCANNING_MAC_OUI_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_SET_SCANNING_MAC_OUI_MAX =
        QCA_WLAN_VENDOR_ATTR_SET_SCANNING_MAC_OUI_AFTER_LAST - 1,
};

enum qca_wlan_vendor_attr_get_supported_features {
    QCA_WLAN_VENDOR_ATTR_FEATURE_SET_INVALID = 0,
    /*                       */
    QCA_WLAN_VENDOR_ATTR_FEATURE_SET = 1,
    /*           */
    QCA_WLAN_VENDOR_ATTR_FEATURE_SET_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_FEATURE_SET_MAX =
        QCA_WLAN_VENDOR_ATTR_FEATURE_SET_AFTER_LAST - 1,
};

/*                               
                                                                
 */
enum qca_wlan_vendor_attr_get_concurrency_matrix {
    QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_INVALID = 0,
    /*                       */
    QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_CONFIG_PARAM_SET_SIZE_MAX = 1,
    /*                       */
    QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_RESULTS_SET_SIZE = 2,
    /*                                                          
                                
     */
    QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_RESULTS_SET = 3,
    /*           */
    QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_MAX =
        QCA_WLAN_VENDOR_ATTR_GET_CONCURRENCY_MATRIX_AFTER_LAST - 1,
};

/*                 */
#define WIFI_FEATURE_INFRA              0x0001   /*                           */
#define WIFI_FEATURE_INFRA_5G           0x0002   /*                        */
#define WIFI_FEATURE_HOTSPOT            0x0004   /*                      */
#define WIFI_FEATURE_P2P                0x0008   /*             */
#define WIFI_FEATURE_SOFT_AP            0x0010   /*         */
#define WIFI_FEATURE_EXTSCAN            0x0020   /*                    */
#define WIFI_FEATURE_NAN                0x0040   /*                   
                                                               */
#define WIFI_FEATURE_D2D_RTT            0x0080   /*                      */
#define WIFI_FEATURE_D2AP_RTT           0x0100   /*                  */
#define WIFI_FEATURE_BATCH_SCAN         0x0200   /*                       */
#define WIFI_FEATURE_PNO                0x0400   /*                           */
#define WIFI_FEATURE_ADDITIONAL_STA     0x0800   /*                      */
#define WIFI_FEATURE_TDLS               0x1000   /*                     
                                                          */
#define WIFI_FEATURE_TDLS_OFFCHANNEL    0x2000   /*                     
                                                            */
#define WIFI_FEATURE_EPR                0x4000   /*                          */
#define WIFI_FEATURE_AP_STA             0x8000   /*                   
                                                                */
/*                        */
enum qca_wlan_vendor_attr_set_no_dfs_flag
{
    QCA_WLAN_VENDOR_ATTR_SET_NO_DFS_FLAG_INVALID = 0,
    /*                       */
    QCA_WLAN_VENDOR_ATTR_SET_NO_DFS_FLAG = 1,
    /*           */
    QCA_WLAN_VENDOR_ATTR_SET_NO_DFS_FLAG_AFTER_LAST,
    QCA_WLAN_VENDOR_ATTR_SET_NO_DFS_FLAG_MAX =
        QCA_WLAN_VENDOR_ATTR_SET_NO_DFS_FLAG_AFTER_LAST - 1,
};


/*                                                           
                                                              
                   
 */
#define QCA_NL80211_VENDOR_ID                0x001374


#ifdef FEATURE_WLAN_CH_AVOID
#define HDD_MAX_AVOID_FREQ_RANGES   4
typedef struct sHddAvoidFreqRange
{
   u32 startFreq;
   u32 endFreq;
} tHddAvoidFreqRange;

typedef struct sHddAvoidFreqList
{
   u32 avoidFreqRangeCount;
   tHddAvoidFreqRange avoidFreqRange[HDD_MAX_AVOID_FREQ_RANGES];
} tHddAvoidFreqList;
#endif /*                       */

struct cfg80211_bss* wlan_hdd_cfg80211_update_bss_db( hdd_adapter_t *pAdapter,
                                      tCsrRoamInfo *pRoamInfo
                                      );

#ifdef FEATURE_WLAN_LFR
int wlan_hdd_cfg80211_pmksa_candidate_notify(
                    hdd_adapter_t *pAdapter, tCsrRoamInfo *pRoamInfo,
                    int index, bool preauth );
#endif

#ifdef FEATURE_WLAN_LFR_METRICS
VOS_STATUS wlan_hdd_cfg80211_roam_metrics_preauth(hdd_adapter_t *pAdapter,
                                                  tCsrRoamInfo *pRoamInfo);

VOS_STATUS wlan_hdd_cfg80211_roam_metrics_preauth_status(
    hdd_adapter_t *pAdapter, tCsrRoamInfo *pRoamInfo, bool preauth_status);

VOS_STATUS wlan_hdd_cfg80211_roam_metrics_handover(hdd_adapter_t *pAdapter,
                                                   tCsrRoamInfo *pRoamInfo);
#endif

#ifdef FEATURE_WLAN_WAPI
void wlan_hdd_cfg80211_set_key_wapi(hdd_adapter_t* pAdapter,
              u8 key_index, const u8 *mac_addr, u8 *key , int key_Len);
#endif
struct wiphy *wlan_hdd_cfg80211_wiphy_alloc(int priv_size);

int wlan_hdd_cfg80211_scan( struct wiphy *wiphy,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,6,0))
                            struct net_device *dev,
#endif
                            struct cfg80211_scan_request *request);

int wlan_hdd_cfg80211_update_band(struct wiphy *wiphy, eCsrBand eBand);

int wlan_hdd_cfg80211_init(struct device *dev,
                               struct wiphy *wiphy,
                               hdd_config_t *pCfg
                                         );

int wlan_hdd_cfg80211_register( struct wiphy *wiphy);
void wlan_hdd_cfg80211_register_frames(hdd_adapter_t* pAdapter);

void wlan_hdd_cfg80211_deregister_frames(hdd_adapter_t* pAdapter);
#ifdef CONFIG_ENABLE_LINUX_REG
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0))
void wlan_hdd_linux_reg_notifier(struct wiphy *wiphy, struct regulatory_request *request);
#else
int wlan_hdd_linux_reg_notifier(struct wiphy *wiphy, struct regulatory_request *request);
#endif
#else
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0))
void wlan_hdd_crda_reg_notifier(struct wiphy *wiphy, struct regulatory_request *request);
#else
int wlan_hdd_crda_reg_notifier(struct wiphy *wiphy, struct regulatory_request *request);
#endif
#endif

extern v_VOID_t hdd_connSetConnectionState( hdd_station_ctx_t *pHddStaCtx,
                                        eConnectionState connState );
VOS_STATUS wlan_hdd_validate_operation_channel(hdd_adapter_t *pAdapter,int channel);
#ifdef FEATURE_WLAN_TDLS
int wlan_hdd_cfg80211_send_tdls_discover_req(struct wiphy *wiphy,
                            struct net_device *dev, u8 *peer);
#endif
#ifdef WLAN_FEATURE_GTK_OFFLOAD
extern void wlan_hdd_cfg80211_update_replayCounterCallback(void *callbackContext,
                            tpSirGtkOffloadGetInfoRspParams pGtkOffloadGetInfoRsp);
#endif
void* wlan_hdd_change_country_code_cb(void *pAdapter);
void hdd_select_cbmode( hdd_adapter_t *pAdapter,v_U8_t operationChannel);


#ifdef FEATURE_WLAN_CH_AVOID
int wlan_hdd_send_avoid_freq_event(hdd_context_t *pHddCtx,
                                   tHddAvoidFreqList *pAvoidFreqList);
#endif /*                       */

#ifdef WLAN_FEATURE_EXTSCAN
void wlan_hdd_cfg80211_extscan_callback(void *ctx, const tANI_U16 evType,
                                      void *pMsg);
#endif /*                      */

#endif
