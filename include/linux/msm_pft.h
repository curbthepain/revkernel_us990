/* Copyright (c) 2014, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef MSM_PFT_H_
#define MSM_PFT_H_

#include <linux/types.h>

/* 
                                                   
  
                               
                                          
                                        
                                                               
           
                                               
                                                      
 */
enum pft_command_opcode {
	PFT_CMD_OPCODE_SET_STATE,
	PFT_CMD_OPCODE_UPDATE_REG_APP_UID,
	PFT_CMD_OPCODE_PERFORM_IN_PLACE_FILE_ENC,
	/* */
	PFT_CMD_OPCODE_MAX_COMMAND_INDEX
};

/* 
                                                 
  
                                                 
                                                                          
                                                                             
                                                                              
                
                                                                             
                                                                              
                           
 */
enum pft_state {
	PFT_STATE_DEACTIVATED,
	PFT_STATE_DEACTIVATING,
	PFT_STATE_KEY_REMOVED,
	PFT_STATE_REMOVING_KEY,
	PFT_STATE_KEY_LOADED,
	/*          */
	PFT_STATE_MAX_INDEX
};

/* 
                                                                
          
  
                                                             
                         
                                
                                                               
                                   
                                                         
                   
                                                     
              
                                              
                                                          
                                                   
                                                         
                                                              
                             
                                          
                                                               
                          
 */
enum pft_command_response_code {
	PFT_CMD_RESP_SUCCESS,
	PFT_CMD_RESP_GENERAL_ERROR,
	PFT_CMD_RESP_INVALID_COMMAND,
	PFT_CMD_RESP_INVALID_CMD_PARAMS,
	PFT_CMD_RESP_INVALID_STATE,
	PFT_CMD_RESP_ALREADY_IN_STATE,
	PFT_CMD_RESP_INPLACE_FILE_IS_OPEN,
	PFT_CMD_RESP_ENT_FILES_CLOSING_FAILURE,
	/*          */
	PFT_CMD_RESP_MAX_INDEX
};

/* 
                                                   
  
                                                   
                                                    
 */
struct pft_command_response {
	__u32 command_id;
	__u32 error_code;
};

/* 
                                   
  
                                         
                                         
                                                  
                                     
                                                         
                                                               
                                                     
 */
struct pft_command {
	__u32 opcode;
	union {
		struct {
			/*                */
			__u32 state;
		} set_state;
		struct {
			__u32 items_count; /*                 */
			__u32 table[0]; /*               */
		} update_app_list;
		struct {
			__u32 file_descriptor;
		} preform_in_place_file_enc;
	};
};

#endif /*            */
