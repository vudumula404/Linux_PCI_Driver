/* INCLUDE FILE: ACE.H

   THIS FILE DEFINES SERVERAL GLOBAL CONSTANTS WHICH ARE IMPLEMENTED
   THROUGHOUT THE BUS-61580  DRIVER. */

//#define MEMORY_SIZE			0xf000
//#define MEMORY_SIZE			0x1000
#define MEMORY_SIZE			0x10000
#define INTMASK_REG 		0x0 	/* read/write */
#define CONFIG_REG   		0x1 	/* read/write */
#define CONFIG_REG2  		0x2 	/* read/write */
#define CONFIG_REG3  		0x7 	/* read/write */
#define CONFIG_REG4  		0x8 	/* read/write */
#define CONFIG_REG5  		0x9 	/* read/write */

#define STACKPTR_REG 		0x3 	/* read only  */
#define START_REG    		0x3 	/* write only */
#define SUBADDRCNTRL_REG		0x4
#define TTAG_REG 			0x5
#define INTSTATUS_REG 		0x6
#define MESS_COUNTER 		0x0101
#define STACKA_PTR  		0x0100
#define MESS_COUNTER_B 		0x0105
#define STACKA_PTR_B  		0x0104
#define START 			0x0002
#define RESET 			0x0001
#define END_OF_FRAME 		0x00FF
#define STACK_1 			0x0000
#define STACK_2 			0x0800

#define BUS_A 			 1
#define BUS_B  			 0
#define TRANSMIT 		 1
#define RECEIVE   		 0
#define YES      		 1
#define NO       		 0
#define TMP_CNTR 		 0x0103 /* TEMP LOCATION TO STORE ORIGINAL VALUE OF MSG_CNTR */
#define TMP_PTR 		 0x0102 /* TEMP LOCATION TO STORE STACK POINTER VALUE  */
#define TMP_CNTR_B 		 0x0107 /* TEMP LOCATION TO STORE ORIGINAL VALUE OF MSG_CNTR */
#define TMP_PTR_B 		 0x0106 /* TEMP LOCATION TO STORE STACK POINTER VALUE  */
#define CNTR     		 0x0106 /* LOCATION TO STORE THE NUMBER OF TIMES BC HAS RUN */
#define ILLEGAL_TBL 		 0x0300
#define LOOKUP_1 		 0x0140
#define LOOKUP_2 		 0x01C0
#define DATA_BLK1		 0x400
#define LAST_STK_PTR 		 0x0101
#define LAST_MSG_READ 		 0x0102
#define LAST_MSG 		 0xffff
#define RUNNING			 1
#define HALTED  		 0
#define ALL   			 0xFFFF
#define BOTH   			 2
#define BCAST  			 0
#define NBCAST 			 1
#define BCST 			 2
#define TST_MODE 		 1
#define EXT_CLK  		 1
#define YES  		 1
#define NO  		 0


