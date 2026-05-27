/*************************************************************************
 *  FILE: mt.h                                                           *
 *  DESCRIPTION: Include file for monitor mode routines.                 *
 *************************************************************************/
/***************************************************************/
typedef struct {
	unsigned mode_code    : 1;
	unsigned contiguous   : 1;
	unsigned channel_b_a  : 1;
	unsigned command_data : 1;
	unsigned error        : 1;
	unsigned broadcast    : 1;
	unsigned this_rt      : 1;
	unsigned word_flag    : 1;
	unsigned gap_time     : 8;
	} tag_word_def;

/***************************************************************/

typedef struct {
       unsigned int rx_word;
       tag_word_def tag_word;
       } MT_WORD_DEF;

/***************************************************************/
       typedef union {
		unsigned  value;
		struct {
			unsigned    CmdWrdErr   :1;
			unsigned    RtRtSecCmd  :1;
			unsigned    GapSycAdr   :1;
			unsigned    InVldWrd    :1;
			unsigned    InCrtSyc    :1;
			unsigned    WrdCntErr   :1;
			unsigned    reserved    :1;
			unsigned    DataStkRoll :1;
			unsigned    DataBlkXfer :1;
			unsigned    RespTimOUt  :1;
			unsigned    FrmtErr     :1;
			unsigned    RtRt        :1;
			unsigned    ErrFlg      :1;
			unsigned    ChAb        :1;
			unsigned    Som         :1;
			unsigned    Eom         :1;
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
		MNTBLKSTS  BlockSts;
		unsigned   TimeTag;
		unsigned   DataBlkPtr;
		MCMDWRD    RecCmdWrd;
		MCMDWRD    TxCmdWrd;
		unsigned   TxStatus;
		unsigned   RecStatus;
		unsigned   DataPack[40];
		}MNTPACK;
/***************************************************************/

typedef union {
			unsigned wrd;
			struct {
				unsigned subaddr :5;
				unsigned tr      :1;
				unsigned taddr   :5;
				unsigned wcnt    :5;
					 }cmd;
		   } CMD_WRD;


