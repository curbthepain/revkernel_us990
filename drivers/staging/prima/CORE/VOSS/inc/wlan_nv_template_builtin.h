/*
 * Copyright (c) 2012-2013 The Linux Foundation. All rights reserved.
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

#if !defined _WLAN_NV_TEMPLATE_BUILTIN_H
#define _WLAN_NV_TEMPLATE_BUILTIN_H


/*                                                                           
                                            

                                                                       
                                                                

                                   

                                         
                                                                        
                                              
                                                                           */


/*
                                           
                                            
                                           
                          
                                                                        
                

                     
                      
                                                            
                                                 
                                                              
                      
                                                    
                                                  
                                                  
                                                       
                                                         

                      
                                                                             
                                                                         
                     
                                                                               
                                                                          
                                          
                                                                             
                                         
                                                                                
                                                                  
                                                        
                                                         
                                                                               
                                        
                                                                               
                         
                                                                         
                                
                                                                           
                                                                          
                                                                               
                                                                                
                                                     
                                                                           
                   
                                                                      
                                                                     
                                  
                          
                                                                              
                                                          
                                                                              
                                                      
                                                  
                                                                        
                                              
                                                                            
                                                                       
                      
                                                                          
                                                        
                                                                             
                                             

                                                                              
                                                                               
                                           
                                                                       
                                                                              
                                                                                
                                                                             
                                                
                                                                            
                                                                          
                                                                           
                    
             
                                                                
                                                               
                                                                           
*/


#include "wlan_nv_types.h"

int allocNvBinTable(int tblIdx, int numEntries);
int allocNvBinEnum(int enumIdx, int numEntries);

typedef enum _nvFieldSizeEnums {
   INDEX_ENUM_ALL = 0,
   INDEX_ENUM_NUM_PHY_MAX_TX_CHAINS,
   INDEX_ENUM_NUM_REG_DOMAINS,
   INDEX_ENUM_NUM_RF_SUBBANDS,
   INDEX_ENUM_NUM_RF_CHANNELS,
   INDEX_ENUM_NUM_2_4GHZ_CHANNELS,
   INDEX_ENUM_NUM_802_11_MODES,
   INDEX_ENUM_NUM_HAL_PHY_RATES,
   INDEX_ENUM_BUILTIN_LAST,
   INDEX_ENUM_BUILTIN_MAX,
} _NV_FIELD_SIZE_ENUMS;

#define INDEX_ENUM_MAX  MAX(INDEX_ENUM_PREDEFINED_MAX, INDEX_ENUM_BUILTIN_MAX)

int getEnumNoOfFields(int enumIdx);
extern _NV_TEMPLATE_ENUM NvEnumsBuiltIn[/*              */][ENUM_ENTRIES_MAX];

typedef enum _nvTableIDs {
   TABLE_sHalNv,
   TABLE_sNvFields,
   TABLE_sRegulatoryChannel,
   TABLE_sRssiChannelOffsets,
   TABLE_sCalData,
   TABLE_sTxBbFilterMode,
   TABLE_sOfdmCmdPwrOffset,
   TABLE_sDefaultCountry,
   TABLE_sFwConfig,
   TABLE_tTpcPowerTable,
   TABLE_tRateGroupPwr,
   TABLE_tRateGroupPwrVR,
   TABLE_sRegulatoryDomains,
   TABLE_sHwCalValues,
   TABLE_sNvTables,
   TABLE_BUILTIN_LAST,
   TABLE_BUILTIN_MAX,
} _NV_TABLE_ID;

#define TABLES_MAX    MAX(TABLE_PREDEFINED_MAX, TABLE_BUILTIN_MAX)

int getTableNoOfFields(int tblIdx);
extern _NV_TEMPLATE_TABLE NvTablesBuiltIn[/*          */][TABLE_ENTRIES_MAX];

#endif //                                        
