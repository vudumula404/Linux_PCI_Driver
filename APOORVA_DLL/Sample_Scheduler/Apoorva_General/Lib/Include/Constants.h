
/*++

    (C) 2004, Apollo Computing Laboratories (P) Ltd.  All rights reserved.

	NO PART  OF  THIS  PROGRAM OR PUBLICATION MAY BE REPRODUCED, TRANSMITTED,
	TRANSCRIBED, STORED IN A RETRIEVAL  SYSTEM,  OR  TRANSLATED	INTO ANY
	LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
	MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE,
	WITHOUT THE PRIOR WRITTEN PERMISSION OF :
                                                  
			APOLLO COMPUTING LABORATORIES (P) LTD.
                                                  
            12-13-197/403-405,Pavani Anasuya Tower,
                                                   
            Tarnaka, Hyderabad - 500 017. INDIA.   
                                                   
            E-mail : mktg@apollocomputing.com  



Module Name:

    Constants.h

Abstract:

    Defines MACRO Deffinitions	

Author:

     Mahender Reddy G.   Jan 7 2004


Environment:

    Windows - 2000/XP


Revision History:

    

    
--*/



#ifndef __CONSTANTS_H_
#define __CONSTANTS_H_

#define RT0		0
#define RT1		1
#define RT2		2
#define RT3		3
#define RT4		4
#define RT5		5
#define RT6		6
#define RT7		7
#define RT8		8
#define RT9		9
#define RT10	10
#define RT11	11
#define RT12	12
#define RT13	13
#define RT14	14
#define RT15	15
#define RT16	16
#define RT17	17
#define RT18	18
#define RT19	19
#define RT20	20
#define RT21	21
#define RT22	22
#define RT23	23
#define RT24	24
#define RT25	25
#define RT26	26
#define RT27	27
#define RT28	28
#define RT29	29
#define RT30	30

#define BC	31
#define RT	0
#define MT	32

#define CH_A  1		
#define CH_B  0
#define BUS_1  1
#define BUS_2  2
#define CARD1  1
#define CARD2  2
#define CARD3  3
#define CARD4  4
#define TRANSMIT 1
#define RECEIVE  0
#define YES      1
#define NO       0
#define PRESENT      1

#define MESS_COUNTER_A 0x0201
#define STACK_PTR_A  0x0200
#define MESS_COUNTER_B 0x0205
#define STACK_PTR_B  0x0204

#define BCSTACK_A 0x0000
#define BCSTACK_B 0x01C00
#define START 0x0002
#define RESET 0x0001
#define END_OF_FRAME 0x00FF
#define RT_CHANNEL_ENA_DIS 0x2140

#define FOREVER -1
#define INIT_MSG_CNT_A 0x0203 /* TEMP LOCATION TO STORE ORIGINAL VALUE OF MSG_CNTR */
#define INIT_STK_PTR_A  0x0202 /* TEMP LOCATION TO STORE STACK POINTER VALUE  */
#define INIT_MSG_CNT_B 0x0207 /* TEMP LOCATION TO STORE ORIGINAL VALUE OF MSG_CNTR */
#define INIT_STK_PTR_B  0x0206 /* TEMP LOCATION TO STORE STACK POINTER VALUE  */


#define RTSTACK_A	0
#define RTSTACK_B	1
#define MTSTACK_A	0
#define MTSTACK_B	1

/* BC Auto Retry Related constants */
#define S		0	//	Same Bus
#define A		2	//	Alternate Bus
#define NORET	4	//	NO RETRY
#define SS		1	//	Double Retry Same Bus
#define SA		3	//	Double Retry Same and Alternate Bus	
#define AS		7	//	Double Retry Alternate and Same Bus
#define AA		5	//	Double Retry Alternate and Alternate Bus
/*********************************END****************************/

/* INCLUDE FILE: ACE.H

   THIS FILE DEFINES SERVERAL GLOBAL CONSTANTS WHICH ARE IMPLEMENTED
   THROUGHOUT THE BUS-61580  DRIVER. */

#define MEMORY_SIZE 0xf000
#define INTMASK_REG  0x0 /* read/write */
#define CONFIG_REG   0x1 /*read/write */
#define CONFIG_REG2  0x2 /* read/write */
#define CONFIG_REG3  0x7 /* read/write */
#define CONFIG_REG4  0x8 /* read/write */
#define CONFIG_REG5  0x9 /* read/write */

#define STACKPTR_REG 0x3 /* read only */
#define START_REG    0x3 /*write only */
#define SUBADDRCNTRL_REG 0x4
#define TTAG_REG 0x5
#define INTSTATUS_REG 0x6





#define CNTR     0x0106 /* LOCATION TO STORE THE NUMBER OF TIMES BC HAS RUN */

#define ILLEGAL_TBL 0x0300
#define LOOKUP_1 0x0140
#define LOOKUP_2 0x01C0
#define DATA_BLK1 0x400
#define LAST_STK_PTR 0x0101
#define LAST_MSG_READ 0x0102
#define LAST_MSG 0xffff
#define RUNNING 1
#define HALTED  0
#define ALL    0xFFFF
#define BOTH   2
#define BCAST  0
#define NBCAST 1
#define BCST 2
#define TST_MODE 1
#define EXT_CLK  1


#define INTR_BCEOM					1  //use for enable/disable BC EOM interrupt
#define INTR_MTEOM					2  //use for enable/disable MT EOM interrupt
#define INTR_RTEOM					4  //use for enable/disable RT EOM interrupt
#define INTR_RTCIRC					8  //use for enable/disable RT Circular Buffer rollover interrupt
#define INTR_RTDBCA					0x10  //use for enable/disable RT DBCA interrupt
#define INTR_TMR1					0x20  //use for enable/disable RTC1 interrupt
#define INTR_TMR2					0x40  //use for enable/disable RTC2 interrupt
#define INTR_TMR3					0x80  //use for enable/disable RTC3 interrupt
#define INTR_BC_ENDOF_MINFRAME		1  //use for enable/disable BC_ENDOF_MINFRAME interrupt
#define INTR_MT_TTR_ROLL_OVER		2  //use for enable/disable MT_TTR_ROLL_OVER interrupt

#define INTR_MT_DATASTACK_ROLL_OVER		4  //use for enable/disable MT_DATASTACK_ROLL_OVER interrupt
#define INTR_MT_CMDSTACK_ROLL_OVER		8  //use for enable/disable MT_CMDSTACK_ROLL_OVER interrupt

#define INTR_EXTERNAL1					1  //use for enable/disable External interrupt
#define INTR_EXTERNAL2					2  //use for enable/disable External interrupt



//*********************   TIMER	1	***************************
#define	US_1		0
#define US_2		1
#define US_4		2
#define US_8		3
#define US_16		4
#define US_32		5
#define US_64		6
#define US_128		7

//*********************   TIMER	2	***************************
#define	US_250		0			//250		Micro Seconds
#define US_500		1			//500		Micro Seconds
#define MS_1		2			//1			Milli Seconds
#define MS_2		3			//2			Milli Seconds
#define MS_4		4			//4			Milli Seconds
#define MS_8		5			//8			Milli Seconds
#define MS_16		6			//16		Milli Seconds
#define MS_32		7			//32		Milli Seconds

//*********************   TIMER	3	***************************
#define	MS_65		0			//65		Milli Seconds
#define MS_125		1			//125		Milli Seconds
#define MS_250		2			//250		Milli Seconds
#define MS_500		3			//500		Milli Seconds
#define SEC_1		4			//1			Seconds
#define SEC_2		5			//2			Seconds
#define SEC_4		6			//4			Seconds
#define SEC_8		7			//8			Seconds

#endif
