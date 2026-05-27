#define END_OF_FRAME		0x00FF
#define BCSTACK_A		0x0000
#define BCSTACK_B		0x01C00
#define MESS_COUNTER_A		0x0201
#define STACK_PTR_A		0x0200
#define MESS_COUNTER_B		0x0205
#define STACK_PTR_B		0x0204
#define INIT_MSG_CNT_A		0x0203 /* TEMP LOCATION TO STORE ORIGINAL VALUE OF MSG_CNTR */
#define INIT_STK_PTR_A		0x0202 /* TEMP LOCATION TO STORE STACK POINTER VALUE  */
#define INIT_MSG_CNT_B		0x0207 /* TEMP LOCATION TO STORE ORIGINAL VALUE OF MSG_CNTR */
#define INIT_STK_PTR_B		0x0206 /* TEMP LOCATION TO STORE STACK POINTER VALUE  */
#define               GNRL_DDC                                 IoGnrl
#define               IOGNRLBASEADDRESS_DDC                    0x0


#define               CARD1_DDC                                0x00
#define               CARD2_DDC                                0x00

#define               MOD0_DDC                                 0x00
#define               MOD1_DDC                                 0x01
#define               MOD2_DDC                                 0x02
#define               NOMODULE_DDC                             0x06
#define               MASK_MODULE_DDC                          0x06
#define               MASK_BRD_CODE_DDC                        0xf1
#define               MASK_ALL_CODE_DDC                        0xf7

#define               COUNTER0_CNTRL_WORD_DDC                  0x34
#define               COUNTER1_CNTRL_WORD_DDC                  0x74
#define               COUNTER2_CNTRL_WORD_DDC                  0xB4
#define               COUNTER0_DDC                             0
#define               COUNTER1_DDC                             1
#define               COUNTER2_DDC                             2


#define               INTR_BUS0_DDC                            0x08
#define               INTR_BUS1_DDC                            0x10
#define               INTR_BUS2_DDC                            0x20
#define               INTR_TIMER0_DDC                          0x01
#define               INTR_TIMER1_DDC                          0x02
#define               INTR_TIMER2_DDC                          0x04

#define               INTR_ALL_DDC                             0x3F
#define               INTR_MASK_DDC                            0x3F

#define               MASK_RT_ADDR_DDC                         0x3e
#define               PARITY_RT_ADDR_DDC                       0x01

