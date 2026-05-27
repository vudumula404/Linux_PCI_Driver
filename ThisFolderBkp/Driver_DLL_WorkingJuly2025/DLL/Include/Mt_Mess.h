/*************************************************************************
 *  FILE: mt.h                                                           *
 *  DESCRIPTION: Include file for monitor mode routines.                 *
 *************************************************************************/
/***************************************************************/
typedef union {
		unsigned int value;
		struct {
			unsigned mode_code    : 1;
			unsigned contiguous   : 1;
			unsigned channel_b_a  : 1;
			unsigned command_data : 1;
			unsigned error        : 1;
			unsigned broadcast    : 1;
			unsigned this_rt      : 1;
			unsigned word_flag    : 1;
			unsigned gap_time     : 8;
			} def;
		} IDWORD;
/***************************************************************/
typedef struct {
	       	unsigned int rx_word;
       		IDWORD idword;
	       } MT_WORD_DEF;
/***************************************************************/
typedef union {
		unsigned  value;
		struct {
			unsigned CmdWrdErr		:1;// BIT --> 0
			unsigned RtRtSecCmd		:1;// BIT --> 1
			unsigned RtRtStatusAdd_err	:1;// BIT --> 2
			unsigned InVldWrd		:1;// BIT --> 3
			unsigned InCrtSyc		:1;// BIT --> 4
			unsigned WrdCntErr		:1;// BIT --> 5
			unsigned reserved		:1;// BIT --> 6
			unsigned DataStkRoll		:1;// BIT --> 7
			unsigned DataBlkXfer		:1;// BIT --> 8
			unsigned RespTimOUt		:1;// BIT --> 9
			unsigned FrmtErr		:1;// BIT --> 10
			unsigned RtRt			:1;// BIT --> 11
			unsigned ErrFlg			:1;// BIT --> 12
			unsigned ChAb			:1;// BIT --> 13
			unsigned Som			:1;// BIT --> 14
			unsigned Eom			:1;// BIT --> 15
			}bitwise;
		}MNTBLKSTS;
/***************************************************************/

typedef union  {
		unsigned int  value;
		struct  {
			unsigned int  WrdCntOrMde  :5;
			unsigned int  SubAddr      :5;
			unsigned int  TrnRecv      :1;
			unsigned int  RtAddr       :5;
			}bitwise;
		}MCMDWRD;

/***************************************************************/
typedef struct mntpack{
			MNTBLKSTS    	BlockSts;
			unsigned     	TimeTag;
			unsigned     	DataBlkPtr;
			MCMDWRD     	RecCmdWrd;
			MCMDWRD     	TxCmdWrd;
			unsigned     	TxStatus;
			unsigned     	RecStatus;
			unsigned	DataPack[40];
			}MNTPACK;
/***************************************************************/

typedef union {
		unsigned wrd;
		struct {
			unsigned wcnt    :5;
			unsigned subaddr :5;
			unsigned tr      :1;
			unsigned taddr   :5;
			 }cmd;
		} CMD_WRD;

