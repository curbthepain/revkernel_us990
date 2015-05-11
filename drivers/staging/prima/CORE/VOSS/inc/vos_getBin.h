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

#if !defined( __VOS_GETBIN_H )
#define __VOS_GETBIN_H

/*                                                                          

                     

                                                             

                                           

                                                                      
                                                                   
                                                                          */

/*          */

/*                                                                          
               
                                                                          */
#include <vos_types.h>
#include <vos_status.h>

/*                                                                           
                                        
                                                                          */

/*                                                                           
                   
                                                                          */
//              
typedef enum
{
  //                        
  VOS_BINARY_ID_FIRMWARE,
  
  //                                  
  VOS_BINARY_ID_CONFIG,

  //                                                         
  VOS_BINARY_ID_COUNTRY_INFO,

  //                                          
  VOS_BINARY_ID_HO_CONFIG,

  //                                             
  VOS_BINARY_ID_DICT_CONFIG

  
} VOS_BINARY_ID;



/*                                                                          
                                        
                                                                          */
  

/*                                                                            
  
                                                              

                                                                             
                                            
  
      
                 
                           
        
  
                                                                       
         
                                                                         
                                                                       
                                                                   
                                                                   
                                                  
         
                                                                       
                                                     
         
                                                                          
                                                                          
                                                                       
                                                        
         
                                                                   
                                                     
         
                                                                     
                                              
          
                                                                        
                                         
          
                                                                       
                                             
          
                                                                     
                                                               

     
  
                                                                            */
VOS_STATUS vos_get_binary_blob( VOS_BINARY_ID binaryId, 
                                v_VOID_t *pBuffer, v_SIZE_t *pBufferSize );

/*                                                                             
                                                                    
                                                                             */
tVOS_CON_MODE vos_get_conparam( void );
tVOS_CONCURRENCY_MODE vos_get_concurrency_mode( void );
v_BOOL_t vos_concurrent_open_sessions_running(void);
v_BOOL_t vos_max_concurrent_connections_reached(void);

#endif //                        
