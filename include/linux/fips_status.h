#ifndef _UAPI_FIPS_STATUS__H
#define _UAPI_FIPS_STATUS__H

#include <linux/types.h>
#include <linux/ioctl.h>

/* 
                                      
                     
                                      
                           
                       
                  
                              
                                
                                  
                               
                       
                                    
                          
                       
                                
                                  
                       
*/
enum fips_status {
		FIPS140_STATUS_NA				= 0,
		FIPS140_STATUS_PASS_CRYPTO		= 1,
		FIPS140_STATUS_QCRYPTO_ALLOWED	= 2,
		FIPS140_STATUS_PASS				= 3,
		FIPS140_STATUS_FAIL				= 0xFF
};
#endif /*                      */
