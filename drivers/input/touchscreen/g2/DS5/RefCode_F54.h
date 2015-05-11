//                                                              
// Copyright ?2012 Synaptics Incorporated. All rights reserved.
//
//                                                             
//                                                             
//       
//
//                                                                 
//                                                             
//                                                             
// information in this file are not expressly or implicitly licensed
//                                                                
//                             
//                                                              

#include <linux/kernel.h>	//      
#include <linux/delay.h>	//      
#include <linux/time.h>		//                                            
#include <linux/string.h>	//      

#if 0
extern const int DefaultTimeout;

int F54Test(int input);
bool switchPage(int page);
void RunQueries(void);
void DeltaImageReport(void);
void RawImageReport(void);
void SensorSpeed(void);
void ADCRange(void);
void TxTxTest(void);
//                        
void HighResistanceTest(void);
void MaxMinTest(void);
int ImageTest(void);
void DeltaImageReport(void);

void SCAN_PDT(void);
#endif

extern void write_log(char *data);
extern void Read8BitRegisters(unsigned short regAddr, unsigned char *data, int length);
extern void Write8BitRegisters(unsigned short regAddr, unsigned char *data, int length);

