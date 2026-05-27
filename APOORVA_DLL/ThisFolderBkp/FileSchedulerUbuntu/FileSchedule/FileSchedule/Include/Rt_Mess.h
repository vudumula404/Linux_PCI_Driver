
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

    Rt_Mess.h

Abstract:

    Defines Remote Terminal Structures.

Author:

     Mahender Reddy G.   Jan 7 2004

Environment:

    Windows - 2000/XP


Revision History:

    
    
--*/


typedef struct
             {
        unsigned int subsys_flag  : 1;
        unsigned int svc_request  : 1;
        unsigned int busy         : 1;
        unsigned int db_accept    : 1;
        unsigned int stop_on_err  : 1;
        unsigned int current_area : 1;
        unsigned int mode         : 2;
               } CONFIG_BITS;

typedef union {
					unsigned  value;
					struct {
							unsigned int Cmd_Word_err			: 1;			// BIT --> 0
							unsigned int RttoRt_SecCmd_err		: 1;			// BIT --> 1
							unsigned int RttoRt_StatusAddr_err	: 1;			// BIT --> 2
							unsigned int Invalid_Word			: 1;			// BIT --> 3
							unsigned int Sync_err				: 1;			// BIT --> 4
							unsigned int WordCnt_err			: 1;			// BIT --> 5
							unsigned int IllegCmd_err			: 1;			// BIT --> 6
							unsigned int CircBuff_RollOver		: 1;			// BIT --> 7
							unsigned int loop_tstfail			: 1;			// BIT --> 8
							unsigned int resp_timeout			: 1;			// BIT --> 9
							unsigned int msg_fmt_err			: 1;			// BIT --> 10
							unsigned int RttoRt_fmt_err			: 1;			// BIT --> 11
							unsigned int err_flag				: 1;			// BIT --> 12
							unsigned int bus					: 1;			// BIT --> 13
							unsigned int som					: 1;			// BIT --> 14
							unsigned int eom					: 1;			// BIT --> 15
					}bitwise;
               } RT_BSW_BITS;

typedef struct
              {
				RT_BSW_BITS  bsw_bits;               /*   block status  word bits       */
                unsigned int bsw;
                unsigned int cmd;
				unsigned int time_tag;
				unsigned int wcnt    : 5;
                unsigned int subaddr : 5;
                unsigned int t_r     : 1;
				unsigned int data[32];
              } RT_MSG;


typedef struct
             {
				RT_BSW_BITS  bsw_bits; 
                unsigned int bsw;
                unsigned int cmd;
                unsigned int time_tag;
                unsigned int address;
                unsigned int data[32];
                unsigned int subaddr ;
                unsigned int t_r ;
                unsigned int rt;
                unsigned int wcnt ;
            }RT_ENHCD_MSG;

/* THIS FILE CONTAINS STRUCTURES FOR THE SECOND CONFIGURATION REGISTER
   ON THE BUS-61580 DEMO CARD */

typedef struct
         {
             int b_disable;
             int ttag_res;
             int clr_ttag_on_sync;
             int load_ttag_on_sync;
             int intstat_autoclear;
             int lev_pulse_select;
             int clr_servreq;
             int enhanced_rtmm;
             int sep_bcst_data;
            } CFG2;

