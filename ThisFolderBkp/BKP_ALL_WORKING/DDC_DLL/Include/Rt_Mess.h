/* INCLUDE FILE: RT_MESS.H

  INCLUDE FILE FOR THE DEFINITION OF RT MESSAGE STRUCTURES */

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
		unsigned int Cmd_Word_err		: 1;// BIT --> 0
		unsigned int RttoRt_SecCmd_err		: 1;// BIT --> 1
		unsigned int RttoRt_StatusAddr_err	: 1;// BIT --> 2
		unsigned int Invalid_Word		: 1;// BIT --> 3
		unsigned int Sync_err			: 1;// BIT --> 4
		unsigned int WordCnt_err		: 1;// BIT --> 5
		unsigned int IllegCmd_err		: 1;// BIT --> 6
		unsigned int CircBuff_RollOver		: 1;// BIT --> 7
		unsigned int loop_tstfail		: 1;// BIT --> 8
		unsigned int resp_timeout		: 1;// BIT --> 9
		unsigned int msg_fmt_err		: 1;// BIT --> 10
		unsigned int RttoRt_fmt_err		: 1;// BIT --> 11
		unsigned int err_flag			: 1;// BIT --> 12
		unsigned int bus			: 1;// BIT --> 13
		unsigned int som			: 1;// BIT --> 14
		unsigned int eom			: 1;// BIT --> 15
		}bitwise;
	} RT_BSW_BITS;

typedef struct
              {
		RT_BSW_BITS  bsw_bits;	/*block status  word bits*/
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
