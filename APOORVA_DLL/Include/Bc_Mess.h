/* INCLUDE FILE FOR THE DEFINITION OF MESSAGES, DEFINES CONSTANTS */

//
typedef struct {
                 unsigned  wcnt:5;
                 unsigned  subaddr:5;
                 unsigned  t_r:1;
                 unsigned  taddr:5;
                 unsigned int status;
               } CMD;

typedef struct {

                CMD  cmd_1,cmd_2;
       		unsigned int  comm_type : 1;
	   	unsigned int  int_msg_gap : 2;
		unsigned int  retry_enable : 1;
       		unsigned int  bus : 1;
	        unsigned int  loop;
	        unsigned int  self_test : 1;
	        unsigned int  mask_bcst_bit : 1;
	        unsigned int  broadcast : 1;
	        unsigned int  mode_code : 1;
	        unsigned int  rt_rt :  1;
		unsigned int  Int_Msg_Gap;
                } MSG_DEF;

typedef struct {
		unsigned int Invalid_Word		: 1;// BIT --> 0
		unsigned int Sync_err			: 1;// BIT --> 1
		unsigned int WordCnt_err		: 1;// BIT --> 2
		unsigned int Incorrect_Status_Add	: 1;// BIT --> 3
		unsigned int loop_tstfail		: 1;// BIT --> 8
        	unsigned int resp_timeout		: 1;// BIT --> 9
        	unsigned int msg_fmt_err		: 1;// BIT --> 10
        	unsigned int stat_wrd_err		: 1;// BIT --> 11
        	unsigned int err_flag			: 1;// BIT --> 12
	        unsigned int bus			: 1;// BIT --> 13
        	unsigned int som			: 1;// BIT --> 14
        	unsigned int eom			: 1;// BIT --> 15
               } BSW_BITS;




typedef struct
            {
                unsigned int term_flag    : 1;
                unsigned int db_accept    : 1;
                unsigned int subsys_flag  : 1;
                unsigned int busy         : 1;
                unsigned int bcst_recv    : 1;
                unsigned int svc_request  : 1;
                unsigned int instrument   : 1;
                unsigned int msg_error    : 1;
                unsigned int rt_addr      : 1;
            } STATUS_BITS;

typedef struct {
        BSW_BITS  bsw_bits;               /*   block status bits       */
        STATUS_BITS  stat1_bits,stat2_bits ; /* status bits word 1 & 2   */
        CMD  cmd1_bits,cmd2_bits;   /* bits in the command word */

       unsigned int  bsw;       /* block status word   */
       unsigned int  time_tag;  /* time tag            */
       unsigned int  cntrl_word;/* control word        */
       unsigned int  cmd_1;     /* command word 1      */
       unsigned int  cmd_2;     /* command word 2      */
       unsigned int  status_1 ; /* status word 1       */
       unsigned int  status_2 ; /* status word 2       */
       unsigned int  loop_word;/* loop back word       */
       unsigned int  data[32]; /* 32 data words max i.e. 0-31 */
       } MSG_RESULT;

typedef struct
            {
                unsigned int Mess_Count;
                unsigned int MinorFrame_Time;
                unsigned short frame_items[128];
            } MAJOR_FRAME;

typedef struct {

                
	unsigned int  Error_Inject 	: 1;
       	unsigned int  Bi_Phase_EncErr 	: 1;
	unsigned int  Word_Length 	: 1;
       	unsigned int  Sync_Err 		: 1;
       	unsigned int  Parity_Err 	: 1;
       	unsigned int  Cmd_Data 		: 1;
       	unsigned int  DataCnt_Err 	: 1;
       	unsigned int  Err_Info 		: 5;
       	unsigned int  WordLen_Size 	: 2;
       	unsigned int  Sync_Info 	: 2;
                } ERR_INJ;
