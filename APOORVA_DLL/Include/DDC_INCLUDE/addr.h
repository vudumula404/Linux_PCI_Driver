#define END_OF_FRAME		0x00FF
#define BCSTACK_A		0x0000
#define BCSTACK_B		0x01C00
#define MESS_COUNTER_A		0x0201
#define STACK_PTR_A		0x0200
//#define MESS_COUNTER_B		0x0205
#define STACK_PTR_B		0x0204
#define INIT_MSG_CNT_A		0x0203 /* TEMP LOCATION TO STORE ORIGINAL VALUE OF MSG_CNTR */
#define INIT_STK_PTR_A		0x0202 /* TEMP LOCATION TO STORE STACK POINTER VALUE  */
#define INIT_MSG_CNT_B		0x0207 /* TEMP LOCATION TO STORE ORIGINAL VALUE OF MSG_CNTR */
#define INIT_STK_PTR_B		0x0206 /* TEMP LOCATION TO STORE STACK POINTER VALUE  */
#define               GNRL                                 IoGnrl
#define               IOGNRLBASEADDRESS                    0x0


#define               CARD1                                0x00
#define               CARD2                                0x00

#define               MOD0                                 0x00
#define               MOD1                                 0x01
#define               MOD2                                 0x02
#define               NOMODULE                             0x06
#define               MASK_MODULE                          0x06
#define               MASK_BRD_CODE                        0xf1
#define               MASK_ALL_CODE                        0xf7

#define               COUNTER0_CNTRL_WORD                  0x34
#define               COUNTER1_CNTRL_WORD                  0x74
#define               COUNTER2_CNTRL_WORD                  0xB4
#define               COUNTER0                             0
#define               COUNTER1                             1
#define               COUNTER2                             2


#define               INTR_BUS0                            0x08
#define               INTR_BUS1                            0x10
#define               INTR_BUS2                            0x20
#define               INTR_TIMER0                          0x01
#define               INTR_TIMER1                          0x02
#define               INTR_TIMER2                          0x04

#define               INTR_ALL                             0x3F
#define               INTR_MASK                            0x3F

#define               MASK_RT_ADDR                         0x3e
#define               PARITY_RT_ADDR                       0x01

