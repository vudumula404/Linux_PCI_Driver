
/*************************************************************

This header file contains base addresses of the pc_add_on_1553

and all addresses of the resources etc

*************************************************************/

/************************************************************

The following section contains base addresses defintions

*************************************************************/
//#define               GNRL                                 IoGnrlM
//#define               GNRL                                 IoGnrlS
//#define               IOGNRLBASEADDRESSM                    0x0
//#define               IOGNRLBASEADDRESSS                    0x0


/***************************************************************

The following section contaions addresses of the resources

with respect base addresses.Also it contains the BIT-WISE

definitions of READ & WRITE REGISTERS.

****************************************************************/

#define               WR_CARD_ID                           GNRL
#define               WR_CARD_ID1                          GNRL + 0x20
#define               RD_CARD_ID                           GNRL + 0x08
//#define               CARDSELECT                           0x01

/*#define               CARD1                                0x01
#define               CARD2                                0x02
#define               CARD3                                0x03*/

//#define               MOD0                                 0x00
//#define               MOD1                                 0x01
//#define               MOD2                                 0x02
//#define               EXT                                0x40

/****************************************************************

                    REGISTER DEFINTION OF RD_CARD_ID

*****************************************************************
  D7       D6       D5     D4      D3      D2      D1      D0
******************************************************************
*       *       *       *       *       *       *       *        *
* BRD   * BRD   * BRD   * BRD   * CLR#  * MCODE * MCODE * CARD   *
* CODE3 * CODE2 * CODE1 * CODE0 * ( 1 ) *   1   *   0   * STATUS *
*       *       *       *       *       *       *       *        *
******************************************************************/

#define               CARDSTATUS                           0x01
#define               NOMODULE                             0x06
#define               MASK_MODULE                          0x06
#define               MASK_BRD_CODE                        0xf1
#define               MASK_ALL_CODE                        0xf7

/****************************************************************

        CHIP SELECT FOR 8254 & REGISTER VALUES

****************************************************************/

#define               CS_TIMER                             GNRL + 0x04
#define               CS_TIMER_COUNTER_0                   CS_TIMER + 0x00
#define               CS_TIMER_COUNTER_1                   CS_TIMER + 0x01
#define               CS_TIMER_COUNTER_2                   CS_TIMER + 0x02
#define               CS_TIMER_CNTRL_WORD                  CS_TIMER + 0x03

#define               COUNTER0_CNTRL_WORD                  0x34
#define               COUNTER1_CNTRL_WORD                  0x74
#define               COUNTER2_CNTRL_WORD                  0xB4
#define               COUNTER0                             0
#define               COUNTER1                             1
#define               COUNTER2                             2

#define               COUNTER0_LATCH_CMD                   0x0
#define               COUNTER1_LATCH_CMD                   0x40
#define               COUNTER2_LATCH_CMD                   0x80


/****************************************************************

                    REGISTER DEFINTION OF RT_ADDR

*****************************************************************
  D7       D6       D5     D4      D3      D2      D1      D0
******************************************************************
*       *       *       *       *       *       *       *        *
*       *       *       *       *       *       *       *  RT    *
*  XX   *SSFLAG * RTA4  * RTA3  * RTA2  * RTA1  * RTA0  * PARITY *
*       *       *       *       *       *       *       *        *
******************************************************************/

#define               WR_RT_ADDR                           GNRL + 0x09

#define               MASK_RT_ADDR                         0x3e
#define               PARITY_RT_ADDR                       0x01

/****************************************************************

                    REGISTER DEFINTION FOR
 INTERRUPT ENABLE,TEST INTERRUPT GENERATION,READING INTERRUPT VECTOR
& CLEARING INTERRUPTS.

*****************************************************************
  D7       D6       D5     D4      D3      D2      D1      D0
******************************************************************
*       *       *       *       *       *       *       *        *
*       *       *       *       *       *       *       *        *
*  XX   *  EXT  * TIMR2 * TIMR1 * TIMR0 * BUS2  * BUS1  * BUS0   *
*       *  INT  *       *       *       *       *       *        *
******************************************************************/

#define               WR_INTR_ENBL                         GNRL + 0x0B
#define               RD_INTR_ENBL                         GNRL + 0x0B
#define               RD_INTR_VECTOR                       GNRL + 0x0C

#define               INTR_BUS0                            0x01
#define               INTR_BUS1                            0x02
#define               INTR_BUS2                            0x04
#define               INTR_TIMER0                          0x08
#define               INTR_TIMER1                          0x10
#define               INTR_TIMER2                          0x20
#define               INTR_EXT                             0x40
#define               INTR_ALL                             0x7F
#define               INTR_MASK                            0x7F

/****************************************************************

               REGISTER DEFINTION FOR CLEARING INTERRUPTS.

*****************************************************************
  D7       D6       D5     D4      D3      D2      D1      D0
******************************************************************
*       *       *       *       *       *       *       *        *
*       *       *       *       *       *       *       *        *
*  XX   *  EXT  * TIMR2 * TIMR1 * TIMR0 * BUS2  * BUS1  * BUS0   *
*       *  INT  *       *       *       *       *       *        *
******************************************************************/

#define               WR_INTR_CLR                          GNRL + 0x0C

#define               CLR_INTR_BUS0                        ~(0x01)
#define               CLR_INTR_BUS1                        ~(0x02)
#define               CLR_INTR_BUS2                        ~(0x04)
#define               CLR_INTR_TIMER0                      ~(0x08)
#define               CLR_INTR_TIMER1                      ~(0x10)
#define               CLR_INTR_TIMER2                      ~(0x20)
#define               CLR_INTR_EXT                         ~(0x40)
#define               CLR_INTR_ALL                         ~(0x7F)


/****************************************************************

     REGISTER DEFINTION FOR READING EXTERNAL INTERRUPT SOURCES

*****************************************************************
  D7       D6       D5     D4      D3      D2      D1      D0
******************************************************************
*       *       *       *       *       *       *       *        *
*  EXT  * EXT   * EXT   * EXT   * EXT   * EXT   * EXT   *  EXT   *
*  INT7 * INT6  * INT5  * INT4  * INT3  * INT2  * INT1  * INT0   *
*       *       *       *       *       *       *       *        *
******************************************************************/

#define               RD_INTR_EXT0                         GNRL + 0x0d

#define               INTR_EXT0                            0x01
#define               INTR_EXT1                            0x02
#define               INTR_EXT2                            0x04
#define               INTR_EXT3                            0x08
#define               INTR_EXT4                            0x10
#define               INTR_EXT5                            0x20
#define               INTR_EXT6                            0x40
#define               INTR_EXT7                            0x80

