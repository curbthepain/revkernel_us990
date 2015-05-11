#ifndef _ASM_SCORE_SCOREREGS_H
#define _ASM_SCORE_SCOREREGS_H

#include <linux/linkage.h>

/*                */
#define TIME0BASE		0x96080000
#define P_TIMER0_CTRL		(TIME0BASE + 0x00)
#define P_TIMER0_CPP_CTRL	(TIME0BASE + 0x04)
#define P_TIMER0_PRELOAD	(TIME0BASE + 0x08)
#define P_TIMER0_CPP_REG	(TIME0BASE + 0x0C)
#define P_TIMER0_UPCNT		(TIME0BASE + 0x10)

/*                           */
/*                    */
#define TMR_DISABLE	0x0000
#define TMR_ENABLE	0x0001

/*                        */
#define TMR_IE_DISABLE	0x0000
#define TMR_IE_ENABLE	0x0002

/*                     */
#define TMR_OE_DISABLE	0x0004
#define TMR_OE_ENABLE	0x0000

/*                                 */
#define TMR_UD_DOWN	0x0000
#define TMR_UD_UP	0x0010

/*                                         */
#define TMR_UDS_UD	0x0000
#define TMR_UDS_EXTUD	0x0020

/*                       */
#define TMR_OM_TOGGLE	0x0000
#define TMR_OM_PILSE	0x0040

/*                                               */
#define TMR_ES_PE	0x0000
#define TMR_ES_NE	0x0100
#define TMR_ES_BOTH	0x0200

/*                           */
#define TMR_M_FREE	0x0000 /*                         */
#define TMR_M_PERIODIC	0x0400 /*                     */
#define TMR_M_FC	0x0800 /*                           */
#define TMR_M_PC	0x0c00 /*                       */

#define SYSTEM_CLOCK		(27*1000000/4)		/*        */
#endif /*                        */