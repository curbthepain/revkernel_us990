/*
 * Copyright 2005-2010 Analog Devices Inc.
 *
 * Licensed under the ADI BSD license or the GPL-2 (or later)
 */

#ifndef _DEF_BF561_H
#define _DEF_BF561_H

/*                                                                                   */
/*                         */
/*                                                                                   */

/*                                                    */

#define PLL_CTL                0xFFC00000	/*                               */
#define PLL_DIV			        0xFFC00004	/*                              */
#define VR_CTL			        0xFFC00008	/*                                             */
#define PLL_STAT               0xFFC0000C	/*                              */
#define PLL_LOCKCNT            0xFFC00010	/*                                  */
#define CHIPID                 0xFFC00014       /*                  */

/*                                                                                            */
#define DOUBLE_FAULT            (DOUBLE_FAULT_B|DOUBLE_FAULT_A)
#define RESET_DOUBLE            (SWRST_DBL_FAULT_B|SWRST_DBL_FAULT_A)
#define RESET_WDOG              (SWRST_WDT_B|SWRST_WDT_A)
#define RESET_SOFTWARE          (SWRST_OCCURRED)

/*                                                                                      */
#define SWRST                   0xFFC00100	/*                         */
#define SYSCR                   0xFFC00104	/*                                     */
#define SIC_RVECT               0xFFC00108	/*                                   */
#define SIC_IMASK0              0xFFC0010C	/*                               */
#define SIC_IMASK1              0xFFC00110	/*                               */
#define SIC_IAR0                0xFFC00124	/*                                     */
#define SIC_IAR1                0xFFC00128	/*                                     */
#define SIC_IAR2                0xFFC0012C	/*                                     */
#define SIC_IAR3                0xFFC00130	/*                                     */
#define SIC_IAR4                0xFFC00134	/*                                     */
#define SIC_IAR5                0xFFC00138	/*                                     */
#define SIC_IAR6                0xFFC0013C	/*                                     */
#define SIC_IAR7                0xFFC00140	/*                                     */
#define SIC_ISR0                0xFFC00114	/*                                 */
#define SIC_ISR1                0xFFC00118	/*                                 */
#define SIC_IWR0                0xFFC0011C	/*                                        */
#define SIC_IWR1                0xFFC00120	/*                                        */

/*                                                                                      */
#define SICB_SWRST              0xFFC01100	/*          */
#define SICB_SYSCR              0xFFC01104	/*          */
#define SICB_RVECT              0xFFC01108	/*                                   */
#define SICB_IMASK0             0xFFC0110C	/*                               */
#define SICB_IMASK1             0xFFC01110	/*                               */
#define SICB_IAR0               0xFFC01124	/*                                     */
#define SICB_IAR1               0xFFC01128	/*                                     */
#define SICB_IAR2               0xFFC0112C	/*                                     */
#define SICB_IAR3               0xFFC01130	/*                                     */
#define SICB_IAR4               0xFFC01134	/*                                     */
#define SICB_IAR5               0xFFC01138	/*                                     */
#define SICB_IAR6               0xFFC0113C	/*                                     */
#define SICB_IAR7               0xFFC01140	/*                                     */
#define SICB_ISR0               0xFFC01114	/*                                 */
#define SICB_ISR1               0xFFC01118	/*                                 */
#define SICB_IWR0               0xFFC0111C	/*                                        */
#define SICB_IWR1               0xFFC01120	/*                                        */

/*                                                               */
#define WDOGA_CTL 				0xFFC00200	/*                           */
#define WDOGA_CNT 				0xFFC00204	/*                         */
#define WDOGA_STAT 				0xFFC00208	/*                          */

/*                                                               */
#define WDOGB_CTL 				0xFFC01200	/*                           */
#define WDOGB_CNT 				0xFFC01204	/*                         */
#define WDOGB_STAT 				0xFFC01208	/*                          */

/*                                           */

/*
                                                          
                                                   
 */
#define BFIN_UART_THR			0xFFC00400  /*                           */
#define BFIN_UART_RBR			0xFFC00400  /*                         */
#define BFIN_UART_DLL			0xFFC00400  /*                          */
#define BFIN_UART_IER			0xFFC00404  /*                           */
#define BFIN_UART_DLH			0xFFC00404  /*                           */
#define BFIN_UART_IIR			0xFFC00408  /*                                   */
#define BFIN_UART_LCR			0xFFC0040C  /*                       */
#define BFIN_UART_MCR			0xFFC00410  /*                        */
#define BFIN_UART_LSR			0xFFC00414  /*                      */
#define BFIN_UART_MSR			0xFFC00418  /*                       */
#define BFIN_UART_SCR			0xFFC0041C  /*                      */
#define BFIN_UART_GCTL			0xFFC00424  /*                         */

/*                                          */
#define SPI0_REGBASE          		0xFFC00500
#define SPI_CTL               		0xFFC00500	/*                      */
#define SPI_FLG               		0xFFC00504	/*                   */
#define SPI_STAT              		0xFFC00508	/*                     */
#define SPI_TDBR              		0xFFC0050C	/*                                   */
#define SPI_RDBR              		0xFFC00510	/*                                  */
#define SPI_BAUD              		0xFFC00514	/*                        */
#define SPI_SHADOW            		0xFFC00518	/*                          */

/*                                               */
#define TIMER0_CONFIG 				0xFFC00600	/*                               */
#define TIMER0_COUNTER 				0xFFC00604	/*                         */
#define TIMER0_PERIOD 				0xFFC00608	/*                        */
#define TIMER0_WIDTH 				0xFFC0060C	/*                       */

#define TIMER1_CONFIG 				0xFFC00610	/*                               */
#define TIMER1_COUNTER 				0xFFC00614	/*                         */
#define TIMER1_PERIOD 				0xFFC00618	/*                        */
#define TIMER1_WIDTH 				0xFFC0061C	/*                       */

#define TIMER2_CONFIG 				0xFFC00620	/*                               */
#define TIMER2_COUNTER 				0xFFC00624	/*                         */
#define TIMER2_PERIOD 				0xFFC00628	/*                        */
#define TIMER2_WIDTH 				0xFFC0062C	/*                       */

#define TIMER3_CONFIG 				0xFFC00630	/*                               */
#define TIMER3_COUNTER 				0xFFC00634	/*                         */
#define TIMER3_PERIOD 				0xFFC00638	/*                        */
#define TIMER3_WIDTH 				0xFFC0063C	/*                       */

#define TIMER4_CONFIG 				0xFFC00640	/*                               */
#define TIMER4_COUNTER 				0xFFC00644	/*                         */
#define TIMER4_PERIOD 				0xFFC00648	/*                        */
#define TIMER4_WIDTH 				0xFFC0064C	/*                       */

#define TIMER5_CONFIG 				0xFFC00650	/*                               */
#define TIMER5_COUNTER 				0xFFC00654	/*                         */
#define TIMER5_PERIOD 				0xFFC00658	/*                        */
#define TIMER5_WIDTH 				0xFFC0065C	/*                       */

#define TIMER6_CONFIG 				0xFFC00660	/*                               */
#define TIMER6_COUNTER 				0xFFC00664	/*                         */
#define TIMER6_PERIOD 				0xFFC00668	/*                        */
#define TIMER6_WIDTH 				0xFFC0066C	/*                       */

#define TIMER7_CONFIG 				0xFFC00670	/*                               */
#define TIMER7_COUNTER 				0xFFC00674	/*                         */
#define TIMER7_PERIOD 				0xFFC00678	/*                        */
#define TIMER7_WIDTH 				0xFFC0067C	/*                       */

#define TMRS8_ENABLE 				0xFFC00680	/*                       */
#define TMRS8_DISABLE 				0xFFC00684	/*                        */
#define TMRS8_STATUS 				0xFFC00688	/*                       */

/*                                                */
#define TIMER8_CONFIG 				0xFFC01600	/*                               */
#define TIMER8_COUNTER 				0xFFC01604	/*                         */
#define TIMER8_PERIOD 				0xFFC01608	/*                        */
#define TIMER8_WIDTH 				0xFFC0160C	/*                       */

#define TIMER9_CONFIG 				0xFFC01610	/*                               */
#define TIMER9_COUNTER 				0xFFC01614	/*                         */
#define TIMER9_PERIOD 				0xFFC01618	/*                        */
#define TIMER9_WIDTH 				0xFFC0161C	/*                       */

#define TIMER10_CONFIG 				0xFFC01620	/*                                */
#define TIMER10_COUNTER 			0xFFC01624	/*                          */
#define TIMER10_PERIOD 				0xFFC01628	/*                         */
#define TIMER10_WIDTH 				0xFFC0162C	/*                        */

#define TIMER11_CONFIG 				0xFFC01630	/*                                */
#define TIMER11_COUNTER 			0xFFC01634	/*                          */
#define TIMER11_PERIOD 				0xFFC01638	/*                         */
#define TIMER11_WIDTH 				0xFFC0163C	/*                        */

#define TMRS4_ENABLE 				0xFFC01640	/*                       */
#define TMRS4_DISABLE 				0xFFC01644	/*                        */
#define TMRS4_STATUS 				0xFFC01648	/*                       */

/*                                                         */
#define FIO0_FLAG_D 				0xFFC00700	/*                    */
#define FIO0_FLAG_C 				0xFFC00704	/*                     */
#define FIO0_FLAG_S 				0xFFC00708	/*                   */
#define FIO0_FLAG_T 				0xFFC0070C	/*                      */
#define FIO0_MASKA_D 				0xFFC00710	/*                                     */
#define FIO0_MASKA_C 				0xFFC00714	/*                                      */
#define FIO0_MASKA_S 				0xFFC00718	/*                                    */
#define FIO0_MASKA_T 				0xFFC0071C	/*                                       */
#define FIO0_MASKB_D 				0xFFC00720	/*                                     */
#define FIO0_MASKB_C 				0xFFC00724	/*                                      */
#define FIO0_MASKB_S 				0xFFC00728	/*                                    */
#define FIO0_MASKB_T 				0xFFC0072C	/*                                       */
#define FIO0_DIR 					0xFFC00730	/*                         */
#define FIO0_POLAR 					0xFFC00734	/*                        */
#define FIO0_EDGE 					0xFFC00738	/*                                     */
#define FIO0_BOTH 					0xFFC0073C	/*                                 */
#define FIO0_INEN 					0xFFC00740	/*                            */

/*                                                         */
#define FIO1_FLAG_D 				0xFFC01500	/*                                           */
#define FIO1_FLAG_C 				0xFFC01504	/*                     */
#define FIO1_FLAG_S 				0xFFC01508	/*                   */
#define FIO1_FLAG_T 				0xFFC0150C	/*                                    */
#define FIO1_MASKA_D 				0xFFC01510	/*                                     */
#define FIO1_MASKA_C 				0xFFC01514	/*                                      */
#define FIO1_MASKA_S 				0xFFC01518	/*                                    */
#define FIO1_MASKA_T 				0xFFC0151C	/*                                       */
#define FIO1_MASKB_D 				0xFFC01520	/*                                     */
#define FIO1_MASKB_C 				0xFFC01524	/*                                      */
#define FIO1_MASKB_S 				0xFFC01528	/*                                    */
#define FIO1_MASKB_T 				0xFFC0152C	/*                                       */
#define FIO1_DIR 					0xFFC01530	/*                         */
#define FIO1_POLAR 					0xFFC01534	/*                        */
#define FIO1_EDGE 					0xFFC01538	/*                                     */
#define FIO1_BOTH 					0xFFC0153C	/*                                 */
#define FIO1_INEN 					0xFFC01540	/*                            */

/*                                                       */
#define FIO2_FLAG_D 				0xFFC01700	/*                                           */
#define FIO2_FLAG_C 				0xFFC01704	/*                     */
#define FIO2_FLAG_S 				0xFFC01708	/*                   */
#define FIO2_FLAG_T 				0xFFC0170C	/*                                    */
#define FIO2_MASKA_D 				0xFFC01710	/*                                     */
#define FIO2_MASKA_C 				0xFFC01714	/*                                      */
#define FIO2_MASKA_S 				0xFFC01718	/*                                    */
#define FIO2_MASKA_T 				0xFFC0171C	/*                                       */
#define FIO2_MASKB_D 				0xFFC01720	/*                                     */
#define FIO2_MASKB_C 				0xFFC01724	/*                                      */
#define FIO2_MASKB_S 				0xFFC01728	/*                                    */
#define FIO2_MASKB_T 				0xFFC0172C	/*                                       */
#define FIO2_DIR 					0xFFC01730	/*                         */
#define FIO2_POLAR 					0xFFC01734	/*                        */
#define FIO2_EDGE 					0xFFC01738	/*                                     */
#define FIO2_BOTH 					0xFFC0173C	/*                                 */
#define FIO2_INEN 					0xFFC01740	/*                            */

/*                                             */
#define SPORT0_TCR1     	 	0xFFC00800	/*                                          */
#define SPORT0_TCR2      	 	0xFFC00804	/*                                          */
#define SPORT0_TCLKDIV        		0xFFC00808	/*                               */
#define SPORT0_TFSDIV          		0xFFC0080C	/*                                    */
#define SPORT0_TX	             	0xFFC00810	/*                         */
#define SPORT0_RX	            	0xFFC00818	/*                         */
#define SPORT0_RCR1      	 		0xFFC00820	/*                                          */
#define SPORT0_RCR2      	 		0xFFC00824	/*                                          */
#define SPORT0_RCLKDIV        		0xFFC00828	/*                              */
#define SPORT0_RFSDIV          		0xFFC0082C	/*                                   */
#define SPORT0_STAT            		0xFFC00830	/*                        */
#define SPORT0_CHNL            		0xFFC00834	/*                                 */
#define SPORT0_MCMC1           		0xFFC00838	/*                                               */
#define SPORT0_MCMC2           		0xFFC0083C	/*                                               */
#define SPORT0_MTCS0           		0xFFC00840	/*                                                 */
#define SPORT0_MTCS1           		0xFFC00844	/*                                                 */
#define SPORT0_MTCS2           		0xFFC00848	/*                                                 */
#define SPORT0_MTCS3           		0xFFC0084C	/*                                                 */
#define SPORT0_MRCS0           		0xFFC00850	/*                                                */
#define SPORT0_MRCS1           		0xFFC00854	/*                                                */
#define SPORT0_MRCS2           		0xFFC00858	/*                                                */
#define SPORT0_MRCS3           		0xFFC0085C	/*                                                */

/*                                             */
#define SPORT1_TCR1     	 		0xFFC00900	/*                                          */
#define SPORT1_TCR2      	 		0xFFC00904	/*                                          */
#define SPORT1_TCLKDIV        		0xFFC00908	/*                               */
#define SPORT1_TFSDIV          		0xFFC0090C	/*                                    */
#define SPORT1_TX	             	0xFFC00910	/*                         */
#define SPORT1_RX	            	0xFFC00918	/*                         */
#define SPORT1_RCR1      	 		0xFFC00920	/*                                          */
#define SPORT1_RCR2      	 		0xFFC00924	/*                                          */
#define SPORT1_RCLKDIV        		0xFFC00928	/*                              */
#define SPORT1_RFSDIV          		0xFFC0092C	/*                                   */
#define SPORT1_STAT            		0xFFC00930	/*                        */
#define SPORT1_CHNL            		0xFFC00934	/*                                 */
#define SPORT1_MCMC1           		0xFFC00938	/*                                               */
#define SPORT1_MCMC2           		0xFFC0093C	/*                                               */
#define SPORT1_MTCS0           		0xFFC00940	/*                                                 */
#define SPORT1_MTCS1           		0xFFC00944	/*                                                 */
#define SPORT1_MTCS2           		0xFFC00948	/*                                                 */
#define SPORT1_MTCS3           		0xFFC0094C	/*                                                 */
#define SPORT1_MRCS0           		0xFFC00950	/*                                                */
#define SPORT1_MRCS1           		0xFFC00954	/*                                                */
#define SPORT1_MRCS2           		0xFFC00958	/*                                                */
#define SPORT1_MRCS3           		0xFFC0095C	/*                                                */

/*                                                               */
#define EBIU_AMGCTL					0xFFC00A00	/*                                             */
#define EBIU_AMBCTL0				0xFFC00A04	/*                                             */
#define EBIU_AMBCTL1				0xFFC00A08	/*                                             */

/*                                                                        */
#define EBIU_SDGCTL					0xFFC00A10	/*                               */
#define EBIU_SDBCTL					0xFFC00A14	/*                             */
#define EBIU_SDRRC 					0xFFC00A18	/*                                     */
#define EBIU_SDSTAT					0xFFC00A1C	/*                       */

/*                                                                           */
#define PPI0_CONTROL 				0xFFC01000	/*                       */
#define PPI0_STATUS 				0xFFC01004	/*                      */
#define PPI0_COUNT 					0xFFC01008	/*                              */
#define PPI0_DELAY 					0xFFC0100C	/*                           */
#define PPI0_FRAME 					0xFFC01010	/*                            */

/*                                                                          */
#define PPI1_CONTROL 				0xFFC01300	/*                       */
#define PPI1_STATUS 				0xFFC01304	/*                      */
#define PPI1_COUNT 					0xFFC01308	/*                              */
#define PPI1_DELAY 					0xFFC0130C	/*                           */
#define PPI1_FRAME 					0xFFC01310	/*                            */

/*                              */
#define	DMAC0_TC_PER  0xFFC00B0C	/*                         */
#define	DMAC0_TC_CNT  0xFFC00B10	/*                                       */
#define	DMAC1_TC_PER  0xFFC01B0C	/*                         */
#define	DMAC1_TC_CNT  0xFFC01B10	/*                                */

/*                                                     */
#define DMA1_0_CONFIG 0xFFC01C08	/*                                       */
#define DMA1_0_NEXT_DESC_PTR 0xFFC01C00	/*                                        */
#define DMA1_0_START_ADDR 0xFFC01C04	/*                              */
#define DMA1_0_X_COUNT 0xFFC01C10	/*                                 */
#define DMA1_0_Y_COUNT 0xFFC01C18	/*                                 */
#define DMA1_0_X_MODIFY 0xFFC01C14	/*                                          */
#define DMA1_0_Y_MODIFY 0xFFC01C1C	/*                                          */
#define DMA1_0_CURR_DESC_PTR 0xFFC01C20	/*                                           */
#define DMA1_0_CURR_ADDR 0xFFC01C24	/*                                        */
#define DMA1_0_CURR_X_COUNT 0xFFC01C30	/*                                         */
#define DMA1_0_CURR_Y_COUNT 0xFFC01C38	/*                                         */
#define DMA1_0_IRQ_STATUS 0xFFC01C28	/*                                          */
#define DMA1_0_PERIPHERAL_MAP 0xFFC01C2C	/*                                        */

#define DMA1_1_CONFIG 0xFFC01C48	/*                                       */
#define DMA1_1_NEXT_DESC_PTR 0xFFC01C40	/*                                        */
#define DMA1_1_START_ADDR 0xFFC01C44	/*                              */
#define DMA1_1_X_COUNT 0xFFC01C50	/*                                 */
#define DMA1_1_Y_COUNT 0xFFC01C58	/*                                 */
#define DMA1_1_X_MODIFY 0xFFC01C54	/*                                          */
#define DMA1_1_Y_MODIFY 0xFFC01C5C	/*                                          */
#define DMA1_1_CURR_DESC_PTR 0xFFC01C60	/*                                           */
#define DMA1_1_CURR_ADDR 0xFFC01C64	/*                                        */
#define DMA1_1_CURR_X_COUNT 0xFFC01C70	/*                                         */
#define DMA1_1_CURR_Y_COUNT 0xFFC01C78	/*                                         */
#define DMA1_1_IRQ_STATUS 0xFFC01C68	/*                                          */
#define DMA1_1_PERIPHERAL_MAP 0xFFC01C6C	/*                                        */

#define DMA1_2_CONFIG 0xFFC01C88	/*                                       */
#define DMA1_2_NEXT_DESC_PTR 0xFFC01C80	/*                                        */
#define DMA1_2_START_ADDR 0xFFC01C84	/*                              */
#define DMA1_2_X_COUNT 0xFFC01C90	/*                                 */
#define DMA1_2_Y_COUNT 0xFFC01C98	/*                                 */
#define DMA1_2_X_MODIFY 0xFFC01C94	/*                                          */
#define DMA1_2_Y_MODIFY 0xFFC01C9C	/*                                          */
#define DMA1_2_CURR_DESC_PTR 0xFFC01CA0	/*                                           */
#define DMA1_2_CURR_ADDR 0xFFC01CA4	/*                                        */
#define DMA1_2_CURR_X_COUNT 0xFFC01CB0	/*                                         */
#define DMA1_2_CURR_Y_COUNT 0xFFC01CB8	/*                                         */
#define DMA1_2_IRQ_STATUS 0xFFC01CA8	/*                                          */
#define DMA1_2_PERIPHERAL_MAP 0xFFC01CAC	/*                                        */

#define DMA1_3_CONFIG 0xFFC01CC8	/*                                       */
#define DMA1_3_NEXT_DESC_PTR 0xFFC01CC0	/*                                        */
#define DMA1_3_START_ADDR 0xFFC01CC4	/*                              */
#define DMA1_3_X_COUNT 0xFFC01CD0	/*                                 */
#define DMA1_3_Y_COUNT 0xFFC01CD8	/*                                 */
#define DMA1_3_X_MODIFY 0xFFC01CD4	/*                                          */
#define DMA1_3_Y_MODIFY 0xFFC01CDC	/*                                          */
#define DMA1_3_CURR_DESC_PTR 0xFFC01CE0	/*                                           */
#define DMA1_3_CURR_ADDR 0xFFC01CE4	/*                                        */
#define DMA1_3_CURR_X_COUNT 0xFFC01CF0	/*                                         */
#define DMA1_3_CURR_Y_COUNT 0xFFC01CF8	/*                                         */
#define DMA1_3_IRQ_STATUS 0xFFC01CE8	/*                                          */
#define DMA1_3_PERIPHERAL_MAP 0xFFC01CEC	/*                                        */

#define DMA1_4_CONFIG 0xFFC01D08	/*                                       */
#define DMA1_4_NEXT_DESC_PTR 0xFFC01D00	/*                                        */
#define DMA1_4_START_ADDR 0xFFC01D04	/*                              */
#define DMA1_4_X_COUNT 0xFFC01D10	/*                                 */
#define DMA1_4_Y_COUNT 0xFFC01D18	/*                                 */
#define DMA1_4_X_MODIFY 0xFFC01D14	/*                                          */
#define DMA1_4_Y_MODIFY 0xFFC01D1C	/*                                          */
#define DMA1_4_CURR_DESC_PTR 0xFFC01D20	/*                                           */
#define DMA1_4_CURR_ADDR 0xFFC01D24	/*                                        */
#define DMA1_4_CURR_X_COUNT 0xFFC01D30	/*                                         */
#define DMA1_4_CURR_Y_COUNT 0xFFC01D38	/*                                         */
#define DMA1_4_IRQ_STATUS 0xFFC01D28	/*                                          */
#define DMA1_4_PERIPHERAL_MAP 0xFFC01D2C	/*                                        */

#define DMA1_5_CONFIG 0xFFC01D48	/*                                       */
#define DMA1_5_NEXT_DESC_PTR 0xFFC01D40	/*                                        */
#define DMA1_5_START_ADDR 0xFFC01D44	/*                              */
#define DMA1_5_X_COUNT 0xFFC01D50	/*                                 */
#define DMA1_5_Y_COUNT 0xFFC01D58	/*                                 */
#define DMA1_5_X_MODIFY 0xFFC01D54	/*                                          */
#define DMA1_5_Y_MODIFY 0xFFC01D5C	/*                                          */
#define DMA1_5_CURR_DESC_PTR 0xFFC01D60	/*                                           */
#define DMA1_5_CURR_ADDR 0xFFC01D64	/*                                        */
#define DMA1_5_CURR_X_COUNT 0xFFC01D70	/*                                         */
#define DMA1_5_CURR_Y_COUNT 0xFFC01D78	/*                                         */
#define DMA1_5_IRQ_STATUS 0xFFC01D68	/*                                          */
#define DMA1_5_PERIPHERAL_MAP 0xFFC01D6C	/*                                        */

#define DMA1_6_CONFIG 0xFFC01D88	/*                                       */
#define DMA1_6_NEXT_DESC_PTR 0xFFC01D80	/*                                        */
#define DMA1_6_START_ADDR 0xFFC01D84	/*                              */
#define DMA1_6_X_COUNT 0xFFC01D90	/*                                 */
#define DMA1_6_Y_COUNT 0xFFC01D98	/*                                 */
#define DMA1_6_X_MODIFY 0xFFC01D94	/*                                          */
#define DMA1_6_Y_MODIFY 0xFFC01D9C	/*                                          */
#define DMA1_6_CURR_DESC_PTR 0xFFC01DA0	/*                                           */
#define DMA1_6_CURR_ADDR 0xFFC01DA4	/*                                        */
#define DMA1_6_CURR_X_COUNT 0xFFC01DB0	/*                                         */
#define DMA1_6_CURR_Y_COUNT 0xFFC01DB8	/*                                         */
#define DMA1_6_IRQ_STATUS 0xFFC01DA8	/*                                          */
#define DMA1_6_PERIPHERAL_MAP 0xFFC01DAC	/*                                        */

#define DMA1_7_CONFIG 0xFFC01DC8	/*                                       */
#define DMA1_7_NEXT_DESC_PTR 0xFFC01DC0	/*                                        */
#define DMA1_7_START_ADDR 0xFFC01DC4	/*                              */
#define DMA1_7_X_COUNT 0xFFC01DD0	/*                                 */
#define DMA1_7_Y_COUNT 0xFFC01DD8	/*                                 */
#define DMA1_7_X_MODIFY 0xFFC01DD4	/*                                          */
#define DMA1_7_Y_MODIFY 0xFFC01DDC	/*                                          */
#define DMA1_7_CURR_DESC_PTR 0xFFC01DE0	/*                                           */
#define DMA1_7_CURR_ADDR 0xFFC01DE4	/*                                        */
#define DMA1_7_CURR_X_COUNT 0xFFC01DF0	/*                                         */
#define DMA1_7_CURR_Y_COUNT 0xFFC01DF8	/*                                         */
#define DMA1_7_IRQ_STATUS 0xFFC01DE8	/*                                          */
#define DMA1_7_PERIPHERAL_MAP 0xFFC01DEC	/*                                        */

#define DMA1_8_CONFIG 0xFFC01E08	/*                                       */
#define DMA1_8_NEXT_DESC_PTR 0xFFC01E00	/*                                        */
#define DMA1_8_START_ADDR 0xFFC01E04	/*                              */
#define DMA1_8_X_COUNT 0xFFC01E10	/*                                 */
#define DMA1_8_Y_COUNT 0xFFC01E18	/*                                 */
#define DMA1_8_X_MODIFY 0xFFC01E14	/*                                          */
#define DMA1_8_Y_MODIFY 0xFFC01E1C	/*                                          */
#define DMA1_8_CURR_DESC_PTR 0xFFC01E20	/*                                           */
#define DMA1_8_CURR_ADDR 0xFFC01E24	/*                                        */
#define DMA1_8_CURR_X_COUNT 0xFFC01E30	/*                                         */
#define DMA1_8_CURR_Y_COUNT 0xFFC01E38	/*                                         */
#define DMA1_8_IRQ_STATUS 0xFFC01E28	/*                                          */
#define DMA1_8_PERIPHERAL_MAP 0xFFC01E2C	/*                                        */

#define DMA1_9_CONFIG 0xFFC01E48	/*                                       */
#define DMA1_9_NEXT_DESC_PTR 0xFFC01E40	/*                                        */
#define DMA1_9_START_ADDR 0xFFC01E44	/*                              */
#define DMA1_9_X_COUNT 0xFFC01E50	/*                                 */
#define DMA1_9_Y_COUNT 0xFFC01E58	/*                                 */
#define DMA1_9_X_MODIFY 0xFFC01E54	/*                                          */
#define DMA1_9_Y_MODIFY 0xFFC01E5C	/*                                          */
#define DMA1_9_CURR_DESC_PTR 0xFFC01E60	/*                                           */
#define DMA1_9_CURR_ADDR 0xFFC01E64	/*                                        */
#define DMA1_9_CURR_X_COUNT 0xFFC01E70	/*                                         */
#define DMA1_9_CURR_Y_COUNT 0xFFC01E78	/*                                         */
#define DMA1_9_IRQ_STATUS 0xFFC01E68	/*                                          */
#define DMA1_9_PERIPHERAL_MAP 0xFFC01E6C	/*                                        */

#define DMA1_10_CONFIG 0xFFC01E88	/*                                        */
#define DMA1_10_NEXT_DESC_PTR 0xFFC01E80	/*                                         */
#define DMA1_10_START_ADDR 0xFFC01E84	/*                               */
#define DMA1_10_X_COUNT 0xFFC01E90	/*                                  */
#define DMA1_10_Y_COUNT 0xFFC01E98	/*                                  */
#define DMA1_10_X_MODIFY 0xFFC01E94	/*                                           */
#define DMA1_10_Y_MODIFY 0xFFC01E9C	/*                                           */
#define DMA1_10_CURR_DESC_PTR 0xFFC01EA0	/*                                            */
#define DMA1_10_CURR_ADDR 0xFFC01EA4	/*                                         */
#define DMA1_10_CURR_X_COUNT 0xFFC01EB0	/*                                          */
#define DMA1_10_CURR_Y_COUNT 0xFFC01EB8	/*                                          */
#define DMA1_10_IRQ_STATUS 0xFFC01EA8	/*                                           */
#define DMA1_10_PERIPHERAL_MAP 0xFFC01EAC	/*                                         */

#define DMA1_11_CONFIG 0xFFC01EC8	/*                                        */
#define DMA1_11_NEXT_DESC_PTR 0xFFC01EC0	/*                                         */
#define DMA1_11_START_ADDR 0xFFC01EC4	/*                               */
#define DMA1_11_X_COUNT 0xFFC01ED0	/*                                  */
#define DMA1_11_Y_COUNT 0xFFC01ED8	/*                                  */
#define DMA1_11_X_MODIFY 0xFFC01ED4	/*                                           */
#define DMA1_11_Y_MODIFY 0xFFC01EDC	/*                                           */
#define DMA1_11_CURR_DESC_PTR 0xFFC01EE0	/*                                            */
#define DMA1_11_CURR_ADDR 0xFFC01EE4	/*                                         */
#define DMA1_11_CURR_X_COUNT 0xFFC01EF0	/*                                          */
#define DMA1_11_CURR_Y_COUNT 0xFFC01EF8	/*                                          */
#define DMA1_11_IRQ_STATUS 0xFFC01EE8	/*                                           */
#define DMA1_11_PERIPHERAL_MAP 0xFFC01EEC	/*                                         */

/*                                                            */
#define MDMA_D0_CONFIG 0xFFC01F08	/*                                           */
#define MDMA_D0_NEXT_DESC_PTR 0xFFC01F00	/*                                                     */
#define MDMA_D0_START_ADDR 0xFFC01F04	/*                                           */
#define MDMA_D0_X_COUNT 0xFFC01F10	/*                                              */
#define MDMA_D0_Y_COUNT 0xFFC01F18	/*                                              */
#define MDMA_D0_X_MODIFY 0xFFC01F14	/*                                                   */
#define MDMA_D0_Y_MODIFY 0xFFC01F1C	/*                                                   */
#define MDMA_D0_CURR_DESC_PTR 0xFFC01F20	/*                                                 */
#define MDMA_D0_CURR_ADDR 0xFFC01F24	/*                                             */
#define MDMA_D0_CURR_X_COUNT 0xFFC01F30	/*                                               */
#define MDMA_D0_CURR_Y_COUNT 0xFFC01F38	/*                                               */
#define MDMA_D0_IRQ_STATUS 0xFFC01F28	/*                                              */
#define MDMA_D0_PERIPHERAL_MAP 0xFFC01F2C	/*                                            */

#define MDMA_S0_CONFIG 0xFFC01F48	/*                                      */
#define MDMA_S0_NEXT_DESC_PTR 0xFFC01F40	/*                                                */
#define MDMA_S0_START_ADDR 0xFFC01F44	/*                                      */
#define MDMA_S0_X_COUNT 0xFFC01F50	/*                                         */
#define MDMA_S0_Y_COUNT 0xFFC01F58	/*                                         */
#define MDMA_S0_X_MODIFY 0xFFC01F54	/*                                                     */
#define MDMA_S0_Y_MODIFY 0xFFC01F5C	/*                                                     */
#define MDMA_S0_CURR_DESC_PTR 0xFFC01F60	/*                                                   */
#define MDMA_S0_CURR_ADDR 0xFFC01F64	/*                                        */
#define MDMA_S0_CURR_X_COUNT 0xFFC01F70	/*                                                 */
#define MDMA_S0_CURR_Y_COUNT 0xFFC01F78	/*                                                 */
#define MDMA_S0_IRQ_STATUS 0xFFC01F68	/*                                         */
#define MDMA_S0_PERIPHERAL_MAP 0xFFC01F6C	/*                                       */

#define MDMA_D1_CONFIG 0xFFC01F88	/*                                           */
#define MDMA_D1_NEXT_DESC_PTR 0xFFC01F80	/*                                                     */
#define MDMA_D1_START_ADDR 0xFFC01F84	/*                                           */
#define MDMA_D1_X_COUNT 0xFFC01F90	/*                                              */
#define MDMA_D1_Y_COUNT 0xFFC01F98	/*                                              */
#define MDMA_D1_X_MODIFY 0xFFC01F94	/*                                                   */
#define MDMA_D1_Y_MODIFY 0xFFC01F9C	/*                                                   */
#define MDMA_D1_CURR_DESC_PTR 0xFFC01FA0	/*                                                 */
#define MDMA_D1_CURR_ADDR 0xFFC01FA4	/*                                      */
#define MDMA_D1_CURR_X_COUNT 0xFFC01FB0	/*                                               */
#define MDMA_D1_CURR_Y_COUNT 0xFFC01FB8	/*                                               */
#define MDMA_D1_IRQ_STATUS 0xFFC01FA8	/*                                       */
#define MDMA_D1_PERIPHERAL_MAP 0xFFC01FAC	/*                                     */

#define MDMA_S1_CONFIG 0xFFC01FC8	/*                                      */
#define MDMA_S1_NEXT_DESC_PTR 0xFFC01FC0	/*                                                */
#define MDMA_S1_START_ADDR 0xFFC01FC4	/*                                      */
#define MDMA_S1_X_COUNT 0xFFC01FD0	/*                                         */
#define MDMA_S1_Y_COUNT 0xFFC01FD8	/*                                         */
#define MDMA_S1_X_MODIFY 0xFFC01FD4	/*                                                     */
#define MDMA_S1_Y_MODIFY 0xFFC01FDC	/*                                                     */
#define MDMA_S1_CURR_DESC_PTR 0xFFC01FE0	/*                                                   */
#define MDMA_S1_CURR_ADDR 0xFFC01FE4	/*                                        */
#define MDMA_S1_CURR_X_COUNT 0xFFC01FF0	/*                                                 */
#define MDMA_S1_CURR_Y_COUNT 0xFFC01FF8	/*                                                 */
#define MDMA_S1_IRQ_STATUS 0xFFC01FE8	/*                                         */
#define MDMA_S1_PERIPHERAL_MAP 0xFFC01FEC	/*                                       */

/*                                                     */
#define DMA2_0_CONFIG 0xFFC00C08	/*                                       */
#define DMA2_0_NEXT_DESC_PTR 0xFFC00C00	/*                                        */
#define DMA2_0_START_ADDR 0xFFC00C04	/*                              */
#define DMA2_0_X_COUNT 0xFFC00C10	/*                                 */
#define DMA2_0_Y_COUNT 0xFFC00C18	/*                                 */
#define DMA2_0_X_MODIFY 0xFFC00C14	/*                                          */
#define DMA2_0_Y_MODIFY 0xFFC00C1C	/*                                          */
#define DMA2_0_CURR_DESC_PTR 0xFFC00C20	/*                                           */
#define DMA2_0_CURR_ADDR 0xFFC00C24	/*                                        */
#define DMA2_0_CURR_X_COUNT 0xFFC00C30	/*                                         */
#define DMA2_0_CURR_Y_COUNT 0xFFC00C38	/*                                         */
#define DMA2_0_IRQ_STATUS 0xFFC00C28	/*                                          */
#define DMA2_0_PERIPHERAL_MAP 0xFFC00C2C	/*                                        */

#define DMA2_1_CONFIG 0xFFC00C48	/*                                       */
#define DMA2_1_NEXT_DESC_PTR 0xFFC00C40	/*                                        */
#define DMA2_1_START_ADDR 0xFFC00C44	/*                              */
#define DMA2_1_X_COUNT 0xFFC00C50	/*                                 */
#define DMA2_1_Y_COUNT 0xFFC00C58	/*                                 */
#define DMA2_1_X_MODIFY 0xFFC00C54	/*                                          */
#define DMA2_1_Y_MODIFY 0xFFC00C5C	/*                                          */
#define DMA2_1_CURR_DESC_PTR 0xFFC00C60	/*                                           */
#define DMA2_1_CURR_ADDR 0xFFC00C64	/*                                        */
#define DMA2_1_CURR_X_COUNT 0xFFC00C70	/*                                         */
#define DMA2_1_CURR_Y_COUNT 0xFFC00C78	/*                                         */
#define DMA2_1_IRQ_STATUS 0xFFC00C68	/*                                          */
#define DMA2_1_PERIPHERAL_MAP 0xFFC00C6C	/*                                        */

#define DMA2_2_CONFIG 0xFFC00C88	/*                                       */
#define DMA2_2_NEXT_DESC_PTR 0xFFC00C80	/*                                        */
#define DMA2_2_START_ADDR 0xFFC00C84	/*                              */
#define DMA2_2_X_COUNT 0xFFC00C90	/*                                 */
#define DMA2_2_Y_COUNT 0xFFC00C98	/*                                 */
#define DMA2_2_X_MODIFY 0xFFC00C94	/*                                          */
#define DMA2_2_Y_MODIFY 0xFFC00C9C	/*                                          */
#define DMA2_2_CURR_DESC_PTR 0xFFC00CA0	/*                                           */
#define DMA2_2_CURR_ADDR 0xFFC00CA4	/*                                        */
#define DMA2_2_CURR_X_COUNT 0xFFC00CB0	/*                                         */
#define DMA2_2_CURR_Y_COUNT 0xFFC00CB8	/*                                         */
#define DMA2_2_IRQ_STATUS 0xFFC00CA8	/*                                          */
#define DMA2_2_PERIPHERAL_MAP 0xFFC00CAC	/*                                        */

#define DMA2_3_CONFIG 0xFFC00CC8	/*                                       */
#define DMA2_3_NEXT_DESC_PTR 0xFFC00CC0	/*                                        */
#define DMA2_3_START_ADDR 0xFFC00CC4	/*                              */
#define DMA2_3_X_COUNT 0xFFC00CD0	/*                                 */
#define DMA2_3_Y_COUNT 0xFFC00CD8	/*                                 */
#define DMA2_3_X_MODIFY 0xFFC00CD4	/*                                          */
#define DMA2_3_Y_MODIFY 0xFFC00CDC	/*                                          */
#define DMA2_3_CURR_DESC_PTR 0xFFC00CE0	/*                                           */
#define DMA2_3_CURR_ADDR 0xFFC00CE4	/*                                        */
#define DMA2_3_CURR_X_COUNT 0xFFC00CF0	/*                                         */
#define DMA2_3_CURR_Y_COUNT 0xFFC00CF8	/*                                         */
#define DMA2_3_IRQ_STATUS 0xFFC00CE8	/*                                          */
#define DMA2_3_PERIPHERAL_MAP 0xFFC00CEC	/*                                        */

#define DMA2_4_CONFIG 0xFFC00D08	/*                                       */
#define DMA2_4_NEXT_DESC_PTR 0xFFC00D00	/*                                        */
#define DMA2_4_START_ADDR 0xFFC00D04	/*                              */
#define DMA2_4_X_COUNT 0xFFC00D10	/*                                 */
#define DMA2_4_Y_COUNT 0xFFC00D18	/*                                 */
#define DMA2_4_X_MODIFY 0xFFC00D14	/*                                          */
#define DMA2_4_Y_MODIFY 0xFFC00D1C	/*                                          */
#define DMA2_4_CURR_DESC_PTR 0xFFC00D20	/*                                           */
#define DMA2_4_CURR_ADDR 0xFFC00D24	/*                                        */
#define DMA2_4_CURR_X_COUNT 0xFFC00D30	/*                                         */
#define DMA2_4_CURR_Y_COUNT 0xFFC00D38	/*                                         */
#define DMA2_4_IRQ_STATUS 0xFFC00D28	/*                                          */
#define DMA2_4_PERIPHERAL_MAP 0xFFC00D2C	/*                                        */

#define DMA2_5_CONFIG 0xFFC00D48	/*                                       */
#define DMA2_5_NEXT_DESC_PTR 0xFFC00D40	/*                                        */
#define DMA2_5_START_ADDR 0xFFC00D44	/*                              */
#define DMA2_5_X_COUNT 0xFFC00D50	/*                                 */
#define DMA2_5_Y_COUNT 0xFFC00D58	/*                                 */
#define DMA2_5_X_MODIFY 0xFFC00D54	/*                                          */
#define DMA2_5_Y_MODIFY 0xFFC00D5C	/*                                          */
#define DMA2_5_CURR_DESC_PTR 0xFFC00D60	/*                                           */
#define DMA2_5_CURR_ADDR 0xFFC00D64	/*                                        */
#define DMA2_5_CURR_X_COUNT 0xFFC00D70	/*                                         */
#define DMA2_5_CURR_Y_COUNT 0xFFC00D78	/*                                         */
#define DMA2_5_IRQ_STATUS 0xFFC00D68	/*                                          */
#define DMA2_5_PERIPHERAL_MAP 0xFFC00D6C	/*                                        */

#define DMA2_6_CONFIG 0xFFC00D88	/*                                       */
#define DMA2_6_NEXT_DESC_PTR 0xFFC00D80	/*                                        */
#define DMA2_6_START_ADDR 0xFFC00D84	/*                              */
#define DMA2_6_X_COUNT 0xFFC00D90	/*                                 */
#define DMA2_6_Y_COUNT 0xFFC00D98	/*                                 */
#define DMA2_6_X_MODIFY 0xFFC00D94	/*                                          */
#define DMA2_6_Y_MODIFY 0xFFC00D9C	/*                                          */
#define DMA2_6_CURR_DESC_PTR 0xFFC00DA0	/*                                           */
#define DMA2_6_CURR_ADDR 0xFFC00DA4	/*                                        */
#define DMA2_6_CURR_X_COUNT 0xFFC00DB0	/*                                         */
#define DMA2_6_CURR_Y_COUNT 0xFFC00DB8	/*                                         */
#define DMA2_6_IRQ_STATUS 0xFFC00DA8	/*                                          */
#define DMA2_6_PERIPHERAL_MAP 0xFFC00DAC	/*                                        */

#define DMA2_7_CONFIG 0xFFC00DC8	/*                                       */
#define DMA2_7_NEXT_DESC_PTR 0xFFC00DC0	/*                                        */
#define DMA2_7_START_ADDR 0xFFC00DC4	/*                              */
#define DMA2_7_X_COUNT 0xFFC00DD0	/*                                 */
#define DMA2_7_Y_COUNT 0xFFC00DD8	/*                                 */
#define DMA2_7_X_MODIFY 0xFFC00DD4	/*                                          */
#define DMA2_7_Y_MODIFY 0xFFC00DDC	/*                                          */
#define DMA2_7_CURR_DESC_PTR 0xFFC00DE0	/*                                           */
#define DMA2_7_CURR_ADDR 0xFFC00DE4	/*                                        */
#define DMA2_7_CURR_X_COUNT 0xFFC00DF0	/*                                         */
#define DMA2_7_CURR_Y_COUNT 0xFFC00DF8	/*                                         */
#define DMA2_7_IRQ_STATUS 0xFFC00DE8	/*                                          */
#define DMA2_7_PERIPHERAL_MAP 0xFFC00DEC	/*                                        */

#define DMA2_8_CONFIG 0xFFC00E08	/*                                       */
#define DMA2_8_NEXT_DESC_PTR 0xFFC00E00	/*                                        */
#define DMA2_8_START_ADDR 0xFFC00E04	/*                              */
#define DMA2_8_X_COUNT 0xFFC00E10	/*                                 */
#define DMA2_8_Y_COUNT 0xFFC00E18	/*                                 */
#define DMA2_8_X_MODIFY 0xFFC00E14	/*                                          */
#define DMA2_8_Y_MODIFY 0xFFC00E1C	/*                                          */
#define DMA2_8_CURR_DESC_PTR 0xFFC00E20	/*                                           */
#define DMA2_8_CURR_ADDR 0xFFC00E24	/*                                        */
#define DMA2_8_CURR_X_COUNT 0xFFC00E30	/*                                         */
#define DMA2_8_CURR_Y_COUNT 0xFFC00E38	/*                                         */
#define DMA2_8_IRQ_STATUS 0xFFC00E28	/*                                          */
#define DMA2_8_PERIPHERAL_MAP 0xFFC00E2C	/*                                        */

#define DMA2_9_CONFIG 0xFFC00E48	/*                                       */
#define DMA2_9_NEXT_DESC_PTR 0xFFC00E40	/*                                        */
#define DMA2_9_START_ADDR 0xFFC00E44	/*                              */
#define DMA2_9_X_COUNT 0xFFC00E50	/*                                 */
#define DMA2_9_Y_COUNT 0xFFC00E58	/*                                 */
#define DMA2_9_X_MODIFY 0xFFC00E54	/*                                          */
#define DMA2_9_Y_MODIFY 0xFFC00E5C	/*                                          */
#define DMA2_9_CURR_DESC_PTR 0xFFC00E60	/*                                           */
#define DMA2_9_CURR_ADDR 0xFFC00E64	/*                                        */
#define DMA2_9_CURR_X_COUNT 0xFFC00E70	/*                                         */
#define DMA2_9_CURR_Y_COUNT 0xFFC00E78	/*                                         */
#define DMA2_9_IRQ_STATUS 0xFFC00E68	/*                                          */
#define DMA2_9_PERIPHERAL_MAP 0xFFC00E6C	/*                                        */

#define DMA2_10_CONFIG 0xFFC00E88	/*                                        */
#define DMA2_10_NEXT_DESC_PTR 0xFFC00E80	/*                                         */
#define DMA2_10_START_ADDR 0xFFC00E84	/*                               */
#define DMA2_10_X_COUNT 0xFFC00E90	/*                                  */
#define DMA2_10_Y_COUNT 0xFFC00E98	/*                                  */
#define DMA2_10_X_MODIFY 0xFFC00E94	/*                                           */
#define DMA2_10_Y_MODIFY 0xFFC00E9C	/*                                           */
#define DMA2_10_CURR_DESC_PTR 0xFFC00EA0	/*                                            */
#define DMA2_10_CURR_ADDR 0xFFC00EA4	/*                                         */
#define DMA2_10_CURR_X_COUNT 0xFFC00EB0	/*                                          */
#define DMA2_10_CURR_Y_COUNT 0xFFC00EB8	/*                                          */
#define DMA2_10_IRQ_STATUS 0xFFC00EA8	/*                                           */
#define DMA2_10_PERIPHERAL_MAP 0xFFC00EAC	/*                                         */

#define DMA2_11_CONFIG 0xFFC00EC8	/*                                        */
#define DMA2_11_NEXT_DESC_PTR 0xFFC00EC0	/*                                         */
#define DMA2_11_START_ADDR 0xFFC00EC4	/*                               */
#define DMA2_11_X_COUNT 0xFFC00ED0	/*                                  */
#define DMA2_11_Y_COUNT 0xFFC00ED8	/*                                  */
#define DMA2_11_X_MODIFY 0xFFC00ED4	/*                                           */
#define DMA2_11_Y_MODIFY 0xFFC00EDC	/*                                           */
#define DMA2_11_CURR_DESC_PTR 0xFFC00EE0	/*                                            */
#define DMA2_11_CURR_ADDR 0xFFC00EE4	/*                                         */
#define DMA2_11_CURR_X_COUNT 0xFFC00EF0	/*                                          */
#define DMA2_11_CURR_Y_COUNT 0xFFC00EF8	/*                                          */
#define DMA2_11_IRQ_STATUS 0xFFC00EE8	/*                                           */
#define DMA2_11_PERIPHERAL_MAP 0xFFC00EEC	/*                                         */

/*                                                            */
#define MDMA_D2_CONFIG 0xFFC00F08	/*                                                    */
#define MDMA_D2_NEXT_DESC_PTR 0xFFC00F00	/*                                                     */
#define MDMA_D2_START_ADDR 0xFFC00F04	/*                                           */
#define MDMA_D2_X_COUNT 0xFFC00F10	/*                                                */
#define MDMA_D2_Y_COUNT 0xFFC00F18	/*                                                */
#define MDMA_D2_X_MODIFY 0xFFC00F14	/*                                                   */
#define MDMA_D2_Y_MODIFY 0xFFC00F1C	/*                                                   */
#define MDMA_D2_CURR_DESC_PTR 0xFFC00F20	/*                                                 */
#define MDMA_D2_CURR_ADDR 0xFFC00F24	/*                                             */
#define MDMA_D2_CURR_X_COUNT 0xFFC00F30	/*                                                   */
#define MDMA_D2_CURR_Y_COUNT 0xFFC00F38	/*                                                   */
#define MDMA_D2_IRQ_STATUS 0xFFC00F28	/*                                                */
#define MDMA_D2_PERIPHERAL_MAP 0xFFC00F2C	/*                                                     */

#define MDMA_S2_CONFIG 0xFFC00F48	/*                                               */
#define MDMA_S2_NEXT_DESC_PTR 0xFFC00F40	/*                                                */
#define MDMA_S2_START_ADDR 0xFFC00F44	/*                                      */
#define MDMA_S2_X_COUNT 0xFFC00F50	/*                                                  */
#define MDMA_S2_Y_COUNT 0xFFC00F58	/*                                                  */
#define MDMA_S2_X_MODIFY 0xFFC00F54	/*                                                   */
#define MDMA_S2_Y_MODIFY 0xFFC00F5C	/*                                                   */
#define MDMA_S2_CURR_DESC_PTR 0xFFC00F60	/*                                                   */
#define MDMA_S2_CURR_ADDR 0xFFC00F64	/*                                        */
#define MDMA_S2_CURR_X_COUNT 0xFFC00F70	/*                                                  */
#define MDMA_S2_CURR_Y_COUNT 0xFFC00F78	/*                                                  */
#define MDMA_S2_IRQ_STATUS 0xFFC00F68	/*                                                  */
#define MDMA_S2_PERIPHERAL_MAP 0xFFC00F6C	/*                                                */

#define MDMA_D3_CONFIG 0xFFC00F88	/*                                                    */
#define MDMA_D3_NEXT_DESC_PTR 0xFFC00F80	/*                                                     */
#define MDMA_D3_START_ADDR 0xFFC00F84	/*                                           */
#define MDMA_D3_X_COUNT 0xFFC00F90	/*                                                */
#define MDMA_D3_Y_COUNT 0xFFC00F98	/*                                                */
#define MDMA_D3_X_MODIFY 0xFFC00F94	/*                                                   */
#define MDMA_D3_Y_MODIFY 0xFFC00F9C	/*                                                   */
#define MDMA_D3_CURR_DESC_PTR 0xFFC00FA0	/*                                                    */
#define MDMA_D3_CURR_ADDR 0xFFC00FA4	/*                                                 */
#define MDMA_D3_CURR_X_COUNT 0xFFC00FB0	/*                                                   */
#define MDMA_D3_CURR_Y_COUNT 0xFFC00FB8	/*                                                   */
#define MDMA_D3_IRQ_STATUS 0xFFC00FA8	/*                                                  */
#define MDMA_D3_PERIPHERAL_MAP 0xFFC00FAC	/*                                                     */

#define MDMA_S3_CONFIG 0xFFC00FC8	/*                                               */
#define MDMA_S3_NEXT_DESC_PTR 0xFFC00FC0	/*                                                */
#define MDMA_S3_START_ADDR 0xFFC00FC4	/*                                      */
#define MDMA_S3_X_COUNT 0xFFC00FD0	/*                                                  */
#define MDMA_S3_Y_COUNT 0xFFC00FD8	/*                                                  */
#define MDMA_S3_X_MODIFY 0xFFC00FD4	/*                                                  */
#define MDMA_S3_Y_MODIFY 0xFFC00FDC	/*                                                     */
#define MDMA_S3_CURR_DESC_PTR 0xFFC00FE0	/*                                                   */
#define MDMA_S3_CURR_ADDR 0xFFC00FE4	/*                                        */
#define MDMA_S3_CURR_X_COUNT 0xFFC00FF0	/*                                                 */
#define MDMA_S3_CURR_Y_COUNT 0xFFC00FF8	/*                                                 */
#define MDMA_S3_IRQ_STATUS 0xFFC00FE8	/*                                                  */
#define MDMA_S3_PERIPHERAL_MAP 0xFFC00FEC	/*                                                */

/*                                                           */
#define IMDMA_D0_CONFIG 0xFFC01808	/*                                         */
#define IMDMA_D0_NEXT_DESC_PTR 0xFFC01800	/*                                                   */
#define IMDMA_D0_START_ADDR 0xFFC01804	/*                                         */
#define IMDMA_D0_X_COUNT 0xFFC01810	/*                                            */
#define IMDMA_D0_Y_COUNT 0xFFC01818	/*                                            */
#define IMDMA_D0_X_MODIFY 0xFFC01814	/*                                                 */
#define IMDMA_D0_Y_MODIFY 0xFFC0181C	/*                                                 */
#define IMDMA_D0_CURR_DESC_PTR 0xFFC01820	/*                                                  */
#define IMDMA_D0_CURR_ADDR 0xFFC01824	/*                                           */
#define IMDMA_D0_CURR_X_COUNT 0xFFC01830	/*                                                    */
#define IMDMA_D0_CURR_Y_COUNT 0xFFC01838	/*                                                    */
#define IMDMA_D0_IRQ_STATUS 0xFFC01828	/*                                            */

#define IMDMA_S0_CONFIG 0xFFC01848	/*                                    */
#define IMDMA_S0_NEXT_DESC_PTR 0xFFC01840	/*                                              */
#define IMDMA_S0_START_ADDR 0xFFC01844	/*                                    */
#define IMDMA_S0_X_COUNT 0xFFC01850	/*                                       */
#define IMDMA_S0_Y_COUNT 0xFFC01858	/*                                       */
#define IMDMA_S0_X_MODIFY 0xFFC01854	/*                                                   */
#define IMDMA_S0_Y_MODIFY 0xFFC0185C	/*                                                   */
#define IMDMA_S0_CURR_DESC_PTR 0xFFC01860	/*                                                 */
#define IMDMA_S0_CURR_ADDR 0xFFC01864	/*                                      */
#define IMDMA_S0_CURR_X_COUNT 0xFFC01870	/*                                               */
#define IMDMA_S0_CURR_Y_COUNT 0xFFC01878	/*                                               */
#define IMDMA_S0_IRQ_STATUS 0xFFC01868	/*                                       */

#define IMDMA_D1_CONFIG 0xFFC01888	/*                                         */
#define IMDMA_D1_NEXT_DESC_PTR 0xFFC01880	/*                                                   */
#define IMDMA_D1_START_ADDR 0xFFC01884	/*                                         */
#define IMDMA_D1_X_COUNT 0xFFC01890	/*                                            */
#define IMDMA_D1_Y_COUNT 0xFFC01898	/*                                            */
#define IMDMA_D1_X_MODIFY 0xFFC01894	/*                                                 */
#define IMDMA_D1_Y_MODIFY 0xFFC0189C	/*                                                 */
#define IMDMA_D1_CURR_DESC_PTR 0xFFC018A0	/*                                                  */
#define IMDMA_D1_CURR_ADDR 0xFFC018A4	/*                                           */
#define IMDMA_D1_CURR_X_COUNT 0xFFC018B0	/*                                                    */
#define IMDMA_D1_CURR_Y_COUNT 0xFFC018B8	/*                                                    */
#define IMDMA_D1_IRQ_STATUS 0xFFC018A8	/*                                            */

#define IMDMA_S1_CONFIG 0xFFC018C8	/*                                    */
#define IMDMA_S1_NEXT_DESC_PTR 0xFFC018C0	/*                                              */
#define IMDMA_S1_START_ADDR 0xFFC018C4	/*                                    */
#define IMDMA_S1_X_COUNT 0xFFC018D0	/*                                       */
#define IMDMA_S1_Y_COUNT 0xFFC018D8	/*                                       */
#define IMDMA_S1_X_MODIFY 0xFFC018D4	/*                                                   */
#define IMDMA_S1_Y_MODIFY 0xFFC018DC	/*                                                   */
#define IMDMA_S1_CURR_DESC_PTR 0xFFC018E0	/*                                                 */
#define IMDMA_S1_CURR_ADDR 0xFFC018E4	/*                                      */
#define IMDMA_S1_CURR_X_COUNT 0xFFC018F0	/*                                               */
#define IMDMA_S1_CURR_Y_COUNT 0xFFC018F8	/*                                               */
#define IMDMA_S1_IRQ_STATUS 0xFFC018E8	/*                                       */

/*                                                                                   */
/*                          */
/*                                                                               */

/*              */
#define CHIPID_VERSION         0xF0000000
#define CHIPID_FAMILY          0x0FFFF000
#define CHIPID_MANUFACTURE     0x00000FFE

/*                  */
#define COREB_SRAM_INIT		0x0020

/*            */
#define SYSTEM_RESET           0x0007	/*                                   */
#define DOUBLE_FAULT_A         0x0008	/*                                  */
#define DOUBLE_FAULT_B         0x0010	/*                                  */
#define SWRST_DBL_FAULT_A      0x0800	/*                           */
#define SWRST_DBL_FAULT_B      0x1000	/*                           */
#define SWRST_WDT_B		       0x2000	/*                  */
#define SWRST_WDT_A		       0x4000	/*                  */
#define SWRST_OCCURRED         0x8000	/*              */

/*                                                                    */

/*                  */
/*            */
/*            */
/*            */

/*                      */
/*                        */
#define Peripheral_IVG(Per_number, IVG_number)    \
    ((IVG_number) - 7) << (((Per_number) % 8) * 4)	/*                                                  */
    /*                                           */
    /*                                           */

/*                   */
/*                                                                            */
#define SIC_UNMASK_ALL         0x00000000	/*                                  */
#define SIC_MASK_ALL           0xFFFFFFFF	/*                                */
#define SIC_MASK(x)	       (1 << (x))	/*                              */
#define SIC_UNMASK(x) (0xFFFFFFFF ^ (1 << (x)))	/*                                */

/*               */
#define IWR_DISABLE_ALL        0x00000000	/*                                */
#define IWR_ENABLE_ALL         0xFFFFFFFF	/*                               */
/*                                         */
#define IWR_ENABLE(x)	       (1 << (x))	/*                             */
#define IWR_DISABLE(x) (0xFFFFFFFF ^ (1 << (x)))	/*                              */

/*                                                                          */

/*                            */
#define PORT_EN              0x00000001	/*                  */
#define PORT_DIR             0x00000002	/*                          */
#define XFR_TYPE             0x0000000C	/*                    */
#define PORT_CFG             0x00000030	/*                        */
#define FLD_SEL              0x00000040	/*                         */
#define PACK_EN              0x00000080	/*                  */
#define DMA32                0x00000100	/*                       */
#define SKIP_EN              0x00000200	/*                         */
#define SKIP_EO              0x00000400	/*                            */
#define DLENGTH              0x00003800	/*                  */
#define DLEN_8		     0x0	/*                                 */
#define DLEN(x)	(((x-9) & 0x07) << 11)	/*                                              */
#define DLEN_10              0x00000800 /*                       */
#define DLEN_11              0x00001000 /*                       */
#define DLEN_12              0x00001800 /*                       */
#define DLEN_13              0x00002000 /*                       */
#define DLEN_14              0x00002800 /*                       */
#define DLEN_15              0x00003000 /*                       */
#define DLEN_16              0x00003800 /*                       */
#define POL                  0x0000C000	/*                             */
#define	POLC		0x4000		/*                    */
#define	POLS		0x8000		/*                         */

/*                  */
#define FLD	             0x00000400	/*                   */
#define FT_ERR	             0x00000800	/*                   */
#define OVR	             0x00001000	/*                     */
#define UNDR	             0x00002000	/*                     */
#define ERR_DET	      	     0x00004000	/*                          */
#define ERR_NCOR	     0x00008000	/*                               */

/*                                                          */

/*                                                                            */

#define CTYPE	            0x00000040	/*                            */
#define CTYPE_P             6	/*                                         */
#define PCAP8	            0x00000080	/*                                 */
#define PCAP16	            0x00000100	/*                                */
#define PCAP32	            0x00000200	/*                                */
#define PCAPWR	            0x00000400	/*                               */
#define PCAPRD	            0x00000800	/*                              */
#define PMAP	            0x00007000	/*                          */

/*                                                                   */

/*                           */

/*                       */
#define TIMEN0	0x0001
#define TIMEN1	0x0002
#define TIMEN2	0x0004
#define TIMEN3	0x0008
#define TIMEN4	0x0010
#define TIMEN5	0x0020
#define TIMEN6	0x0040
#define TIMEN7	0x0080
#define TIMEN8	0x0001
#define TIMEN9	0x0002
#define TIMEN10	0x0004
#define TIMEN11	0x0008

#define TIMEN0_P	0x00
#define TIMEN1_P	0x01
#define TIMEN2_P	0x02
#define TIMEN3_P	0x03
#define TIMEN4_P	0x04
#define TIMEN5_P	0x05
#define TIMEN6_P	0x06
#define TIMEN7_P	0x07
#define TIMEN8_P	0x00
#define TIMEN9_P	0x01
#define TIMEN10_P	0x02
#define TIMEN11_P	0x03

/*                        */
#define TIMDIS0		0x0001
#define TIMDIS1		0x0002
#define TIMDIS2		0x0004
#define TIMDIS3		0x0008
#define TIMDIS4		0x0010
#define TIMDIS5		0x0020
#define TIMDIS6		0x0040
#define TIMDIS7		0x0080
#define TIMDIS8		0x0001
#define TIMDIS9		0x0002
#define TIMDIS10	0x0004
#define TIMDIS11	0x0008

#define TIMDIS0_P	0x00
#define TIMDIS1_P	0x01
#define TIMDIS2_P	0x02
#define TIMDIS3_P	0x03
#define TIMDIS4_P	0x04
#define TIMDIS5_P	0x05
#define TIMDIS6_P	0x06
#define TIMDIS7_P	0x07
#define TIMDIS8_P	0x00
#define TIMDIS9_P	0x01
#define TIMDIS10_P	0x02
#define TIMDIS11_P	0x03

/*                       */
#define TIMIL0		0x00000001
#define TIMIL1		0x00000002
#define TIMIL2		0x00000004
#define TIMIL3		0x00000008
#define TIMIL4		0x00010000
#define TIMIL5		0x00020000
#define TIMIL6		0x00040000
#define TIMIL7		0x00080000
#define TIMIL8		0x0001
#define TIMIL9		0x0002
#define TIMIL10		0x0004
#define TIMIL11		0x0008
#define TOVF_ERR0	0x00000010
#define TOVF_ERR1	0x00000020
#define TOVF_ERR2	0x00000040
#define TOVF_ERR3	0x00000080
#define TOVF_ERR4	0x00100000
#define TOVF_ERR5	0x00200000
#define TOVF_ERR6	0x00400000
#define TOVF_ERR7	0x00800000
#define TOVF_ERR8	0x0010
#define TOVF_ERR9	0x0020
#define TOVF_ERR10	0x0040
#define TOVF_ERR11	0x0080
#define TRUN0		0x00001000
#define TRUN1		0x00002000
#define TRUN2		0x00004000
#define TRUN3		0x00008000
#define TRUN4		0x10000000
#define TRUN5		0x20000000
#define TRUN6		0x40000000
#define TRUN7		0x80000000
#define TRUN8		0x1000
#define TRUN9		0x2000
#define TRUN10		0x4000
#define TRUN11		0x8000

#define TIMIL0_P	0x00
#define TIMIL1_P	0x01
#define TIMIL2_P	0x02
#define TIMIL3_P	0x03
#define TIMIL4_P	0x10
#define TIMIL5_P	0x11
#define TIMIL6_P	0x12
#define TIMIL7_P	0x13
#define TIMIL8_P	0x00
#define TIMIL9_P	0x01
#define TIMIL10_P	0x02
#define TIMIL11_P	0x03
#define TOVF_ERR0_P	0x04
#define TOVF_ERR1_P	0x05
#define TOVF_ERR2_P	0x06
#define TOVF_ERR3_P	0x07
#define TOVF_ERR4_P	0x14
#define TOVF_ERR5_P	0x15
#define TOVF_ERR6_P	0x16
#define TOVF_ERR7_P	0x17
#define TOVF_ERR8_P	0x04
#define TOVF_ERR9_P	0x05
#define TOVF_ERR10_P	0x06
#define TOVF_ERR11_P	0x07
#define TRUN0_P		0x0C
#define TRUN1_P		0x0D
#define TRUN2_P		0x0E
#define TRUN3_P		0x0F
#define TRUN4_P		0x1C
#define TRUN5_P		0x1D
#define TRUN6_P		0x1E
#define TRUN7_P		0x1F
#define TRUN8_P		0x0C
#define TRUN9_P		0x0D
#define TRUN10_P	0x0E
#define TRUN11_P	0x0F

/*                                                                       */
#define TOVL_ERR0 TOVF_ERR0
#define TOVL_ERR1 TOVF_ERR1
#define TOVL_ERR2 TOVF_ERR2
#define TOVL_ERR3 TOVF_ERR3
#define TOVL_ERR4 TOVF_ERR4
#define TOVL_ERR5 TOVF_ERR5
#define TOVL_ERR6 TOVF_ERR6
#define TOVL_ERR7 TOVF_ERR7
#define TOVL_ERR8 TOVF_ERR8
#define TOVL_ERR9 TOVF_ERR9
#define TOVL_ERR10 TOVF_ERR10
#define TOVL_ERR11 TOVF_ERR11
#define TOVL_ERR0_P TOVF_ERR0_P
#define TOVL_ERR1_P TOVF_ERR1_P
#define TOVL_ERR2_P TOVF_ERR2_P
#define TOVL_ERR3_P TOVF_ERR3_P
#define TOVL_ERR4_P TOVF_ERR4_P
#define TOVL_ERR5_P TOVF_ERR5_P
#define TOVL_ERR6_P TOVF_ERR6_P
#define TOVL_ERR7_P TOVF_ERR7_P
#define TOVL_ERR8_P TOVF_ERR8_P
#define TOVL_ERR9_P TOVF_ERR9_P
#define TOVL_ERR10_P TOVF_ERR10_P
#define TOVL_ERR11_P TOVF_ERR11_P

/*                         */
#define PWM_OUT		0x0001
#define WDTH_CAP	0x0002
#define EXT_CLK		0x0003
#define PULSE_HI	0x0004
#define PERIOD_CNT	0x0008
#define IRQ_ENA		0x0010
#define TIN_SEL		0x0020
#define OUT_DIS		0x0040
#define CLK_SEL		0x0080
#define TOGGLE_HI	0x0100
#define EMU_RUN		0x0200
#define ERR_TYP(x)	((x & 0x03) << 14)

#define TMODE_P0		0x00
#define TMODE_P1		0x01
#define PULSE_HI_P		0x02
#define PERIOD_CNT_P		0x03
#define IRQ_ENA_P		0x04
#define TIN_SEL_P		0x05
#define OUT_DIS_P		0x06
#define CLK_SEL_P		0x07
#define TOGGLE_HI_P		0x08
#define EMU_RUN_P		0x09
#define ERR_TYP_P0		0x0E
#define ERR_TYP_P1		0x0F

/*                                                                            */

/*              */
#define AMCKEN			0x0001	/*               */
#define AMBEN_B0		0x0002	/*                                        */
#define AMBEN_B0_B1		0x0004	/*                                             */
#define AMBEN_B0_B1_B2	0x0006	/*                                              */
#define AMBEN_ALL		0x0008	/*                                                       */
#define B0_PEN			0x0010	/*                               */
#define B1_PEN			0x0020	/*                               */
#define B2_PEN			0x0040	/*                               */
#define B3_PEN			0x0080	/*                               */

/*                      */
#define AMCKEN_P		0x00000000	/*               */
#define AMBEN_P0		0x00000001	/*                                                                            */
#define AMBEN_P1		0x00000002	/*                                                                               */
#define AMBEN_P2		0x00000003	/*                                                                           */
#define B0_PEN_P			0x004	/*                               */
#define B1_PEN_P			0x005	/*                               */
#define B2_PEN_P			0x006	/*                               */
#define B3_PEN_P			0x007	/*                               */

/*               */
#define B0RDYEN	0x00000001	/*                                        */
#define B0RDYPOL 0x00000002	/*                                                     */
#define B0TT_1	0x00000004	/*                                                     */
#define B0TT_2	0x00000008	/*                                                      */
#define B0TT_3	0x0000000C	/*                                                      */
#define B0TT_4	0x00000000	/*                                                      */
#define B0ST_1	0x00000010	/*                                                                    */
#define B0ST_2	0x00000020	/*                                                                     */
#define B0ST_3	0x00000030	/*                                                                     */
#define B0ST_4	0x00000000	/*                                                                     */
#define B0HT_1	0x00000040	/*                                                                         */
#define B0HT_2	0x00000080	/*                                                                          */
#define B0HT_3	0x000000C0	/*                                                                          */
#define B0HT_0	0x00000000	/*                                                                          */
#define B0RAT_1			0x00000100	/*                                   */
#define B0RAT_2			0x00000200	/*                                    */
#define B0RAT_3			0x00000300	/*                                    */
#define B0RAT_4			0x00000400	/*                                    */
#define B0RAT_5			0x00000500	/*                                    */
#define B0RAT_6			0x00000600	/*                                    */
#define B0RAT_7			0x00000700	/*                                    */
#define B0RAT_8			0x00000800	/*                                    */
#define B0RAT_9			0x00000900	/*                                    */
#define B0RAT_10		0x00000A00	/*                                     */
#define B0RAT_11		0x00000B00	/*                                     */
#define B0RAT_12		0x00000C00	/*                                     */
#define B0RAT_13		0x00000D00	/*                                     */
#define B0RAT_14		0x00000E00	/*                                     */
#define B0RAT_15		0x00000F00	/*                                     */
#define B0WAT_1			0x00001000	/*                                    */
#define B0WAT_2			0x00002000	/*                                     */
#define B0WAT_3			0x00003000	/*                                     */
#define B0WAT_4			0x00004000	/*                                     */
#define B0WAT_5			0x00005000	/*                                     */
#define B0WAT_6			0x00006000	/*                                     */
#define B0WAT_7			0x00007000	/*                                     */
#define B0WAT_8			0x00008000	/*                                     */
#define B0WAT_9			0x00009000	/*                                     */
#define B0WAT_10		0x0000A000	/*                                      */
#define B0WAT_11		0x0000B000	/*                                      */
#define B0WAT_12		0x0000C000	/*                                      */
#define B0WAT_13		0x0000D000	/*                                      */
#define B0WAT_14		0x0000E000	/*                                      */
#define B0WAT_15		0x0000F000	/*                                      */
#define B1RDYEN			0x00010000	/*                                        */
#define B1RDYPOL		0x00020000	/*                                                     */
#define B1TT_1			0x00040000	/*                                                     */
#define B1TT_2			0x00080000	/*                                                      */
#define B1TT_3			0x000C0000	/*                                                      */
#define B1TT_4			0x00000000	/*                                                      */
#define B1ST_1			0x00100000	/*                                                                         */
#define B1ST_2			0x00200000	/*                                                                          */
#define B1ST_3			0x00300000	/*                                                                          */
#define B1ST_4			0x00000000	/*                                                                          */
#define B1HT_1			0x00400000	/*                                                                            */
#define B1HT_2			0x00800000	/*                                                                             */
#define B1HT_3			0x00C00000	/*                                                                             */
#define B1HT_0			0x00000000	/*                                                                             */
#define B1RAT_1			0x01000000	/*                                   */
#define B1RAT_2			0x02000000	/*                                    */
#define B1RAT_3			0x03000000	/*                                    */
#define B1RAT_4			0x04000000	/*                                    */
#define B1RAT_5			0x05000000	/*                                    */
#define B1RAT_6			0x06000000	/*                                    */
#define B1RAT_7			0x07000000	/*                                    */
#define B1RAT_8			0x08000000	/*                                    */
#define B1RAT_9			0x09000000	/*                                    */
#define B1RAT_10		0x0A000000	/*                                     */
#define B1RAT_11		0x0B000000	/*                                     */
#define B1RAT_12		0x0C000000	/*                                     */
#define B1RAT_13		0x0D000000	/*                                     */
#define B1RAT_14		0x0E000000	/*                                     */
#define B1RAT_15		0x0F000000	/*                                     */
#define B1WAT_1			0x10000000	/*                                    */
#define B1WAT_2			0x20000000	/*                                     */
#define B1WAT_3			0x30000000	/*                                     */
#define B1WAT_4			0x40000000	/*                                     */
#define B1WAT_5			0x50000000	/*                                     */
#define B1WAT_6			0x60000000	/*                                     */
#define B1WAT_7			0x70000000	/*                                     */
#define B1WAT_8			0x80000000	/*                                     */
#define B1WAT_9			0x90000000	/*                                     */
#define B1WAT_10		0xA0000000	/*                                      */
#define B1WAT_11		0xB0000000	/*                                      */
#define B1WAT_12		0xC0000000	/*                                      */
#define B1WAT_13		0xD0000000	/*                                      */
#define B1WAT_14		0xE0000000	/*                                      */
#define B1WAT_15		0xF0000000	/*                                      */

/*               */
#define B2RDYEN			0x00000001	/*                                        */
#define B2RDYPOL		0x00000002	/*                                                     */
#define B2TT_1			0x00000004	/*                                                     */
#define B2TT_2			0x00000008	/*                                                      */
#define B2TT_3			0x0000000C	/*                                                      */
#define B2TT_4			0x00000000	/*                                                      */
#define B2ST_1			0x00000010	/*                                                                         */
#define B2ST_2			0x00000020	/*                                                                          */
#define B2ST_3			0x00000030	/*                                                                          */
#define B2ST_4			0x00000000	/*                                                                          */
#define B2HT_1			0x00000040	/*                                                                            */
#define B2HT_2			0x00000080	/*                                                                             */
#define B2HT_3			0x000000C0	/*                                                                             */
#define B2HT_0			0x00000000	/*                                                                             */
#define B2RAT_1			0x00000100	/*                                   */
#define B2RAT_2			0x00000200	/*                                    */
#define B2RAT_3			0x00000300	/*                                    */
#define B2RAT_4			0x00000400	/*                                    */
#define B2RAT_5			0x00000500	/*                                    */
#define B2RAT_6			0x00000600	/*                                    */
#define B2RAT_7			0x00000700	/*                                    */
#define B2RAT_8			0x00000800	/*                                    */
#define B2RAT_9			0x00000900	/*                                    */
#define B2RAT_10		0x00000A00	/*                                     */
#define B2RAT_11		0x00000B00	/*                                     */
#define B2RAT_12		0x00000C00	/*                                     */
#define B2RAT_13		0x00000D00	/*                                     */
#define B2RAT_14		0x00000E00	/*                                     */
#define B2RAT_15		0x00000F00	/*                                     */
#define B2WAT_1			0x00001000	/*                                    */
#define B2WAT_2			0x00002000	/*                                     */
#define B2WAT_3			0x00003000	/*                                     */
#define B2WAT_4			0x00004000	/*                                     */
#define B2WAT_5			0x00005000	/*                                     */
#define B2WAT_6			0x00006000	/*                                     */
#define B2WAT_7			0x00007000	/*                                     */
#define B2WAT_8			0x00008000	/*                                     */
#define B2WAT_9			0x00009000	/*                                     */
#define B2WAT_10		0x0000A000	/*                                      */
#define B2WAT_11		0x0000B000	/*                                      */
#define B2WAT_12		0x0000C000	/*                                      */
#define B2WAT_13		0x0000D000	/*                                      */
#define B2WAT_14		0x0000E000	/*                                      */
#define B2WAT_15		0x0000F000	/*                                      */
#define B3RDYEN			0x00010000	/*                                        */
#define B3RDYPOL		0x00020000	/*                                                     */
#define B3TT_1			0x00040000	/*                                                     */
#define B3TT_2			0x00080000	/*                                                      */
#define B3TT_3			0x000C0000	/*                                                      */
#define B3TT_4			0x00000000	/*                                                      */
#define B3ST_1			0x00100000	/*                                                                         */
#define B3ST_2			0x00200000	/*                                                                          */
#define B3ST_3			0x00300000	/*                                                                          */
#define B3ST_4			0x00000000	/*                                                                          */
#define B3HT_1			0x00400000	/*                                                                            */
#define B3HT_2			0x00800000	/*                                                                             */
#define B3HT_3			0x00C00000	/*                                                                             */
#define B3HT_0			0x00000000	/*                                                                             */
#define B3RAT_1			0x01000000	/*                                   */
#define B3RAT_2			0x02000000	/*                                    */
#define B3RAT_3			0x03000000	/*                                    */
#define B3RAT_4			0x04000000	/*                                    */
#define B3RAT_5			0x05000000	/*                                    */
#define B3RAT_6			0x06000000	/*                                    */
#define B3RAT_7			0x07000000	/*                                    */
#define B3RAT_8			0x08000000	/*                                    */
#define B3RAT_9			0x09000000	/*                                    */
#define B3RAT_10		0x0A000000	/*                                     */
#define B3RAT_11		0x0B000000	/*                                     */
#define B3RAT_12		0x0C000000	/*                                     */
#define B3RAT_13		0x0D000000	/*                                     */
#define B3RAT_14		0x0E000000	/*                                     */
#define B3RAT_15		0x0F000000	/*                                     */
#define B3WAT_1			0x10000000	/*                                    */
#define B3WAT_2			0x20000000	/*                                     */
#define B3WAT_3			0x30000000	/*                                     */
#define B3WAT_4			0x40000000	/*                                     */
#define B3WAT_5			0x50000000	/*                                     */
#define B3WAT_6			0x60000000	/*                                     */
#define B3WAT_7			0x70000000	/*                                     */
#define B3WAT_8			0x80000000	/*                                     */
#define B3WAT_9			0x90000000	/*                                     */
#define B3WAT_10		0xA0000000	/*                                      */
#define B3WAT_11		0xB0000000	/*                                      */
#define B3WAT_12		0xC0000000	/*                                      */
#define B3WAT_13		0xD0000000	/*                                      */
#define B3WAT_14		0xE0000000	/*                                      */
#define B3WAT_15		0xF0000000	/*                                      */

/*                                                                             */

/*                   */
#define SCTLE			0x00000001	/*                                               */
#define CL_2			0x00000008	/*                              */
#define CL_3			0x0000000C	/*                              */
#define PFE			0x00000010	/*                       */
#define PFP			0x00000020	/*                                         */
#define TRAS_1			0x00000040	/*                      */
#define TRAS_2			0x00000080	/*                       */
#define TRAS_3			0x000000C0	/*                       */
#define TRAS_4			0x00000100	/*                       */
#define TRAS_5			0x00000140	/*                       */
#define TRAS_6			0x00000180	/*                       */
#define TRAS_7			0x000001C0	/*                       */
#define TRAS_8			0x00000200	/*                       */
#define TRAS_9			0x00000240	/*                       */
#define TRAS_10			0x00000280	/*                        */
#define TRAS_11			0x000002C0	/*                        */
#define TRAS_12			0x00000300	/*                        */
#define TRAS_13			0x00000340	/*                        */
#define TRAS_14			0x00000380	/*                        */
#define TRAS_15			0x000003C0	/*                        */
#define TRP_1			0x00000800	/*                     */
#define TRP_2			0x00001000	/*                      */
#define TRP_3			0x00001800	/*                      */
#define TRP_4			0x00002000	/*                      */
#define TRP_5			0x00002800	/*                      */
#define TRP_6			0x00003000	/*                      */
#define TRP_7			0x00003800	/*                      */
#define TRCD_1			0x00008000	/*                      */
#define TRCD_2			0x00010000	/*                       */
#define TRCD_3			0x00018000	/*                       */
#define TRCD_4			0x00020000	/*                       */
#define TRCD_5			0x00028000	/*                       */
#define TRCD_6			0x00030000	/*                       */
#define TRCD_7			0x00038000	/*                       */
#define TWR_1			0x00080000	/*                     */
#define TWR_2			0x00100000	/*                      */
#define TWR_3			0x00180000	/*                      */
#define PUPSD			0x00200000	/*                     */
#define PSM			0x00400000	/*                                                                              */
#define PSS				0x00800000	/*                                                     */
#define SRFS			0x01000000	/*                               */
#define EBUFE			0x02000000	/*                                  */
#define FBBRW			0x04000000	/*                                     */
#define EMREN			0x10000000	/*                               */
#define TCSR			0x20000000	/*                                              */
#define CDDBG			0x40000000	/*                                          */

/*                   */
#define EB0_E				0x00000001	/*                              */
#define EB0_SZ_16			0x00000000	/*                                 */
#define EB0_SZ_32			0x00000002	/*                                 */
#define EB0_SZ_64			0x00000004	/*                                 */
#define EB0_SZ_128			0x00000006	/*                                  */
#define EB0_CAW_8			0x00000000	/*                                                   */
#define EB0_CAW_9			0x00000010	/*                                                   */
#define EB0_CAW_10			0x00000020	/*                                                   */
#define EB0_CAW_11			0x00000030	/*                                                   */

#define EB1_E				0x00000100	/*                              */
#define EB1__SZ_16			0x00000000	/*                                 */
#define EB1__SZ_32			0x00000200	/*                                 */
#define EB1__SZ_64			0x00000400	/*                                 */
#define EB1__SZ_128			0x00000600	/*                                  */
#define EB1__CAW_8			0x00000000	/*                                                   */
#define EB1__CAW_9			0x00001000	/*                                                   */
#define EB1__CAW_10			0x00002000	/*                                                   */
#define EB1__CAW_11			0x00003000	/*                                                   */

#define EB2__E				0x00010000	/*                              */
#define EB2__SZ_16			0x00000000	/*                                 */
#define EB2__SZ_32			0x00020000	/*                                 */
#define EB2__SZ_64			0x00040000	/*                                 */
#define EB2__SZ_128			0x00060000	/*                                  */
#define EB2__CAW_8			0x00000000	/*                                                   */
#define EB2__CAW_9			0x00100000	/*                                                   */
#define EB2__CAW_10			0x00200000	/*                                                   */
#define EB2__CAW_11			0x00300000	/*                                                   */

#define EB3__E				0x01000000	/*                              */
#define EB3__SZ_16			0x00000000	/*                                 */
#define EB3__SZ_32			0x02000000	/*                                 */
#define EB3__SZ_64			0x04000000	/*                                 */
#define EB3__SZ_128			0x06000000	/*                                  */
#define EB3__CAW_8			0x00000000	/*                                                   */
#define EB3__CAW_9			0x10000000	/*                                                   */
#define EB3__CAW_10			0x20000000	/*                                                   */
#define EB3__CAW_11			0x30000000	/*                                                   */

/*                   */
#define SDCI			0x00000001	/*                           */
#define SDSRA			0x00000002	/*                                    */
#define SDPUA			0x00000004	/*                        */
#define SDRS			0x00000008	/*                         */
#define SDEASE		    0x00000010	/*                                     */
#define BGSTAT			0x00000020	/*             */

#endif				/*              */