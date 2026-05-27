#include "Constants.h"
//#include "Bc_Mess.h"
#include "Rt_Mess.h"
#include "Mt_Mess.h"

#define BOOL	bool
#define USHORT	unsigned short
#define CHAR	char
#define UCHAR	unsigned char
#define ULONG	unsigned long
#define LONG	long
#define PCHAR	char*
#define SHORT	unsigned short


//Initialize Card by paassing card ID
int	InitCard(UCHAR CardId);
//Get CArd presense for particular card 
CHAR	GetCardPresence(UCHAR CardId);
SHORT	SelfTest(UCHAR CardId);
CHAR	SoftwareReset(UCHAR CardId);
PCHAR	PrintErrorMsg(int);//Yet To Add with comments
PCHAR	GetMfgId_VerId(UCHAR CardId);//Yet To Add with comments
void	DefErrors();//Yet To Add with comments
CHAR	ConfigICSR(UCHAR CardId);//Yet To Add with comments

//Read Memory Locations 
USHORT	MemRead(UCHAR Bus_Id,ULONG len,UCHAR CardId);
USHORT	MemWrite(UCHAR Bus_Id,ULONG len, USHORT val,UCHAR CardId);
USHORT	RegRead(UCHAR Bus_Id,ULONG len,UCHAR CardId);
USHORT	RegWrite(UCHAR Bus_Id,ULONG len,USHORT val,UCHAR CardId);
CHAR	ClearRam(UCHAR Bus_Id,USHORT Module,UCHAR CardId);
CHAR	ConfigureBC(UCHAR Bus_Id,USHORT stack_size,UCHAR CardId);
CHAR	IntMsgGapLeast(UCHAR Bus_Id,UCHAR flag,UCHAR CardId);
SHORT	DefMsg_StackA(UCHAR Bus_Id,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words,UCHAR CardId);
SHORT	DefMsg_StackB(UCHAR Bus_Id,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words,UCHAR CardId);
SHORT	DefMsg(UCHAR Bus_Id,USHORT BCSTACK_A_B,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words,UCHAR CardId);
SHORT	DefFrame(UCHAR Bus_Id,USHORT BCSTACK_A_B,USHORT number_of_messages,USHORT mess_sequence[173],UCHAR CardId);
SHORT	DefMajorFrame(UCHAR Bus_Id,USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[128],UCHAR CardId);
SHORT	RunBc(UCHAR Bus_Id,USHORT stackA_B,USHORT Position,USHORT num_of_messages,ULONG times,UCHAR CardId);
SHORT	RunBcMajorFrame(UCHAR Bus_Id,USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[128],ULONG times,UCHAR CardId);
SHORT	ReadMsgBC(UCHAR Bus_Id,USHORT stackA_B,USHORT frame_item,MSG_RESULT *result,UCHAR CardId);
CHAR	ResetBc(UCHAR Bus_Id,UCHAR CardId);

BSW_BITS	ReadBswBits(USHORT blk_statwrd);
STATUS_BITS	ReadStatusBits(USHORT status);
MSG_DEF		ReturnCntrlFields(USHORT control);
CMD		ReturnCmdFields(USHORT command);
CHAR		BCAutoRepeat(UCHAR Bus_Id,UCHAR sel,UCHAR CardId);
CHAR		BCRetry(UCHAR Bus_Id,USHORT sel,UCHAR CardId);

CHAR	ConfigureRT(UCHAR Bus_Id,UCHAR A_B,UCHAR Address[],UCHAR CardId);
CHAR	RunRt(UCHAR Bus_Id,UCHAR RtStack_A_B,UCHAR Address[],UCHAR CardId);
CHAR	WriteDataBlock(UCHAR Bus_Id,UCHAR RtNo,UCHAR subaddr,USHORT data_buff[],UCHAR CardId);
CHAR	ReadMsgRT(UCHAR Bus_Id,UCHAR MsgNo,UCHAR RtNo,RT_MSG *message,UCHAR CardId);
CHAR	ConfigureRTCB(UCHAR Bus_Id,USHORT Module,USHORT BlockSize,UCHAR CardId);
CHAR	WriteDataBlockCB(UCHAR Bus_Id,USHORT Module,USHORT datablock,USHORT data_buff[],USHORT BlockSize,UCHAR CardId);
CHAR	ReadDataBlockCB(UCHAR Bus_Id,USHORT Module,USHORT data_block,USHORT *data_buff,USHORT BlockSize,UCHAR CardId);
CHAR	LegalizeAll(UCHAR,UCHAR,UCHAR CardId);
CHAR	IllegalizeAll(UCHAR,UCHAR,UCHAR CardId);
CHAR	IllegalizeCmd(UCHAR,CMD_WRD *temp,UCHAR CardId); 
CHAR	LegalizeCmd(UCHAR,CMD_WRD *temp,UCHAR CardId);

CHAR	SetSrvReq(UCHAR,UCHAR RtNo,UCHAR req,UCHAR CardId);
CHAR	SetTrFlag(UCHAR,UCHAR RtNo,UCHAR Tr,UCHAR CardId);
CHAR	SetSubSys(UCHAR,UCHAR RtNo,UCHAR flag,UCHAR CardId);
CHAR	SetBusy(UCHAR,UCHAR RtNo,UCHAR flag,UCHAR CardId);
CHAR	SetDBA(UCHAR,UCHAR RtNo,UCHAR flag,UCHAR CardId);

CHAR	ClrTTROnSync(UCHAR Bus_Id,UCHAR RtNo,UCHAR TTRFlag,UCHAR CardId);
CHAR	LoadTTROnSync(UCHAR,UCHAR RtNo,UCHAR flag,UCHAR CardId);
CHAR	SelMsgMt(UCHAR Bus_Id,UCHAR MTSTACK_A_B,UCHAR CardId);
CHAR	RunMt(UCHAR Bus_Id,UCHAR CardId);
CHAR	ReadMonPacket(UCHAR Bus_Id,UCHAR MTSTACK_A_B,USHORT DespNo,MNTPACK *MontDataBuff,UCHAR CardId);

CHAR	SelWordMt(UCHAR Bus_Id,UCHAR CardId);
CHAR	DeSelAll(UCHAR Bus_Id,UCHAR CardId);
CHAR	SelAll(UCHAR Bus_Id,UCHAR CardId);
CHAR	SelCmd(UCHAR Bus_Id,CMD_WRD *temp,UCHAR CardId);
CHAR	DeSelCmd(UCHAR Bus_Id,CMD_WRD *temp,UCHAR CardId);
CHAR	ReadMtWord(UCHAR Bus_Id,MT_WORD_DEF *result,UCHAR CardId);
 
CHAR	InitInterrupts(UCHAR,USHORT val,UCHAR CardId);
CHAR	InitTimerIntr(USHORT,UCHAR CardId);
CHAR	StopTimerIntr(USHORT Value,UCHAR CardId);
CHAR	SelectTMR(UCHAR,UCHAR,UCHAR,UCHAR CardId);
CHAR	DisableInterrupts(UCHAR,UCHAR CardId);
LONG	GetBCEOMCnt(UCHAR,UCHAR CardId);
LONG	GetRTEOMCnt(UCHAR,USHORT RtNo,UCHAR CardId);
LONG	GetMTEOMCnt(UCHAR,UCHAR CardId);
LONG	GetMTEOMCnt_dummy(UCHAR,UCHAR CardId);
LONG	GetRTCIRCCnt(UCHAR,USHORT RtNo,UCHAR CardId);
LONG	GetRTDBCACnt(UCHAR,UCHAR CardId);
LONG	GetTMR1Cnt(UCHAR CardId);
LONG	GetTMR2Cnt(UCHAR CardId);
LONG	GetTMR3Cnt(UCHAR CardId);

CHAR	SetBCEOMCnt(UCHAR,ULONG value,UCHAR CardId);
CHAR	SetRTEOMCnt(UCHAR,USHORT RtNo,ULONG value,UCHAR CardId);
CHAR	SetMTEOMCnt(UCHAR,ULONG value,UCHAR CardId);
CHAR	SetRTCIRCCnt(UCHAR,USHORT RtNo,ULONG value,UCHAR CardId);
CHAR	SetRTDBCACnt(UCHAR,ULONG value,UCHAR CardId);
CHAR	SetTMR1Cnt(ULONG,UCHAR CardId);
CHAR	SetTMR2Cnt(ULONG,UCHAR CardId);
CHAR	SetTMR3Cnt(ULONG,UCHAR CardId);

CHAR	ResetMt(UCHAR,UCHAR CardId);
CHAR	WritePort(ULONG offset,UCHAR val,UCHAR CardId);
UCHAR	ReadPort(ULONG offset,UCHAR CardId);
CHAR	BcTimeTagReset(UCHAR Bus_Id,UCHAR CardId);
CHAR	RtTimeTagReset(UCHAR Bus_Id,UCHAR CardId);
CHAR	MtTimeTagReset(UCHAR Bus_Id,UCHAR CardId);
CHAR	BcTTRResolution(UCHAR Bus_Id,USHORT TimeRes,UCHAR CardId);
CHAR	RtTTRResolution(UCHAR Bus_Id,USHORT TimeRes,UCHAR CardId);
CHAR	MtTTRResolution(UCHAR Bus_Id,USHORT TimeRes,UCHAR CardId);

CHAR	ErrorInjection(UCHAR Bus_Id,ERR_INJ Err,UCHAR CardId);
CHAR	ErrorInjectReset(UCHAR Bus_Id,UCHAR CardId);

SHORT	GetMinorFrameCnt(UCHAR Bus_Id,UCHAR CardId);
CHAR	MsgEnableDisable(UCHAR Bus_Id,UCHAR MsgNos[],UCHAR CardId);
CHAR	InterMsgGap(UCHAR Bus_Id,UCHAR MsgNo,USHORT Inter_Msg_Gap,UCHAR CardId);
CHAR	InitEnhancedInterrupts(UCHAR Bus_Id,UCHAR Value,UCHAR CardId);
CHAR	DisableEnhancedInterrupts(UCHAR Bus_Id,UCHAR Value,UCHAR CardId);
LONG	GetEndOfFrameCnt(UCHAR Bus_Id,UCHAR CardId);
LONG	GetMT_TTR_Roll_OverCnt(UCHAR Bus_Id,UCHAR CardId);
CHAR	SetEndOfFrameCnt(UCHAR Bus_Id,ULONG value,UCHAR CardId);
CHAR	SetMT_TTR_Roll_OverCnt(UCHAR Bus_Id,ULONG value,UCHAR CardId);

CHAR	ResetRt(UCHAR Bus_Id,UCHAR Address,UCHAR CardId);

LONG	GetMT_DataStack_Roll_OverCnt(UCHAR Bus_Id,UCHAR CardId);
LONG	GetMT_CmdStack_Roll_OverCnt(UCHAR Bus_Id,UCHAR CardId);
CHAR	SetMT_DataStack_Roll_OverCnt(UCHAR Bus_Id,ULONG value,UCHAR CardId);
CHAR	SetMT_CmdStack_Roll_OverCnt(UCHAR Bus_Id,ULONG value,UCHAR CardId);

SHORT	DefBuffSize(UCHAR Bus_Id,UCHAR RtNo,UCHAR subaddr,UCHAR t_r,USHORT size,UCHAR CardId);
SHORT	DefDataBlkCB(UCHAR Bus_Id,UCHAR RtNo,UCHAR subaddr,UCHAR t_r,USHORT block_num,UCHAR CardId);
CHAR	WriteDataBlockCB(UCHAR Bus_Id,USHORT RtAddr,USHORT datablock,USHORT data_buff[],USHORT BlockSize,UCHAR CardId);
CHAR	WriteDataBufferCB(UCHAR Bus_Id,UCHAR RtNo,UCHAR subaddr,USHORT data_buff[],USHORT Buff_Size,UCHAR CardId);
CHAR	ReadDataBlockCB(UCHAR Bus_Id,USHORT RtAddr,USHORT datablock,USHORT *data_buff,USHORT BlockSize,UCHAR CardId);
CHAR	ReadDataBufferCB(UCHAR Bus_Id,UCHAR RtNo,UCHAR subaddr,USHORT *data_buff,USHORT Buff_Size,UCHAR CardId);

CHAR EnableRtChannel(UCHAR Bus_Id,UCHAR RtNo,UCHAR Channel,UCHAR CardId);
CHAR DisableRtChannel(UCHAR Bus_Id,UCHAR RtNo,UCHAR Channel,UCHAR CardId);

void BC_Mode(int BcMod, int channel, int NoOfTimes);

void MT_Mode(int mtModId);

void RT_Mode(int BcMod, int rtModId, int rtAddr);

void BC_MT_Mode(int BcMod, int mtModId, int channel, int NoOfTimes);

void BC_RT_Mode(int BcMod, int rtModId, int rtAddr, int channel, int NoOfTimes);

void RT_MT_Mode(int rtModId, int mtModId, int rtAddr);

void RT_Status_Mode(int rtModId, int rtAddr, int RtStatusBits);

void BC_MT_Mode_snode(int BcMod, int mtModId, int channel, int NoOfTimes);

void RT_MT_Mode_snode(int rtModId, int rtAddr);

#endif

CHAR	CloseDeviceHandle(UCHAR CardId);
void	EnableDriverDebug(UCHAR CardId);
void	DisableDriverDebug(UCHAR CardId);  
void ConfigureRT(UCHAR rtModId, unsigned char rtAddr);
#if 0
//void	WritePort(ULONG,UCHAR CardId);
//UCHAR	ReadPort(ULONG);
    
int	ClearRam_DDC();
void	ResetAce_DDC();  //BOOL
void	StartReset_DDC(USHORT); //BOOL

/*USHORT	MemRead_DDC(ULONG len);
USHORT	MemWrite_DDC(ULONG len, USHORT val);
USHORT	RegRead_DDC(UCHAR Bus_Id,ULONG len,UCHAR CardId);
USHORT	RegWrite_DDC(UCHAR Bus_Id,ULONG len,USHORT val,UCHAR CardId);*/

void ConfigICSR_DDC();

  USHORT DefMode_DDC(USHORT);
  void DefIntMask_DDC(INT_DEF_DDC);
  SHORT DefMsg_DDC(USHORT,MSG_DEF_DDC,USHORT data_buffer[],USHORT);
  SHORT DefMsg_StackB_DDC(USHORT,MSG_DEF_DDC,USHORT data_buffer[],USHORT);
  SHORT DefFrame_DDC(USHORT,USHORT mess_sequence[64]);
  SHORT DefFrame_StackB_DDC(USHORT,USHORT mess_sequence[64]);
  SHORT RunBc_DDC(USHORT,USHORT,USHORT ,USHORT);
  SHORT RunBcInt_DDC(USHORT,USHORT,USHORT ,USHORT);
  CMD_DDC		ReturnCmdFields_DDC(USHORT);
  MSG_DEF_DDC		ReturnCntrlFields_DDC(USHORT);
  BSW_BITS_DDC		ReadBswBits_DDC(USHORT);
  STATUS_BITS_DDC	ReadStatusBits_DDC(USHORT);
  SHORT ReadMsgResult_DDC(USHORT,USHORT,MSG_RESULT_DDC *result);
  SHORT DefLookUp_DDC(USHORT );
  void DefFeatures_DDC(CFG2_DDC);
  void EnableEnhancedRt_DDC();
  SHORT DefMsgIlegal_DDC(USHORT,USHORT,USHORT,USHORT,USHORT);
  SHORT DefModeCodeIllegal_DDC(int,int);
  SHORT DefBuffSize_DDC(int,int,int);

  SHORT DefEomInt_DDC(int,int);
  SHORT DefBuffInt_DDC(int,int);
  SHORT ReadEnhancedMsg_DDC(USHORT, RT_ENHCD_MSG_DDC *message);
  INTSTAT_REG_DDC ReturnIntStatus_DDC(USHORT);
  SHORT WriteDataBlock_DDC(USHORT,USHORT data_buff[]);
  SHORT ReadDataBlock_DDC(USHORT,USHORT *data_buff);
  void SetSsflag_DDC();
  void SetSvcReq_DDC();
  void SetBusy_DDC();
  void SetDba_DDC();
  void ResetSsflag_DDC();
  void ResetSvcReq_DDC();
  void ResetBusy_DDC();
  void ResetDba_DDC();
  CONFIG_BITS_DDC ReadConfigBits_DDC();
  void RunRt_DDC();
  USHORT CheckMsgActivity_DDC();
  SHORT ReadMsg_DDC(USHORT, RT_MSG_DDC *message);
  SHORT DefDataBlk_DDC(USHORT,USHORT,USHORT);
  void DefErrors_DDC();

  void Version_DDC();
  void DefBcEnhanceMode_DDC();
  void EnableInterMsgGap_DDC();
  void InterMsgGap_DDC(USHORT,USHORT);
  void EnableFrameAutoRepeat_DDC(UCHAR CardId);
  void BcFrameTime_DDC(USHORT);
  void AutoRetry_DDC(USHORT,UCHAR CardId);
  void DisableIllegLogic_DDC();
  void SelectModule_DDC(UCHAR,UCHAR CardId);  //BOOL
  UCHAR CalculateParity_DDC(UCHAR CardId);
//CHAR  SetRtAddrM_DDC(UCHAR,UCHAR,UCHAR );
char SetRtAddrM_DDC(unsigned short RtAddr,unsigned int CCSC_FD,unsigned char CardId);

  void  InitInterrupts_DDC(UCHAR,UCHAR CardId);
  void DisableInterrupts_DDC(UCHAR,UCHAR CardId);
  int GetM1Cnt_DDC();
  int GetM2Cnt_DDC();
  int GetM3Cnt_DDC();
  int GetRtcCnt_DDC();
  int GetRtc1Cnt_DDC();
  int GetRtc2Cnt_DDC();
  void SetM1Cnt_DDC();
  void SetM2Cnt_DDC();
  void SetM3Cnt_DDC();
  void SetRtcCnt_DDC();
  void SetRtc1Cnt_DDC();
  void SetRtc2Cnt_DDC();
  void GenerateRtc_DDC(UCHAR,UCHAR,UCHAR CardId);
  void SelWordMt_DDC();
  SHORT ReadMtWord_DDC(MT_WORD_DEF_DDC *result);
  void SelMsgMt_DDC();
  void RunMt_DDC();
  void ReadMonPacket_DDC(USHORT ,MNTPACK_DDC *MontDataBuff);
  int NumOfMsg_DDC();
  void SelCmd_DDC(CMD_WRD_DDC *temp);
  void DeSelCmd_DDC(CMD_WRD_DDC *temp);
  void SelAll_DDC();
  void DeSelAll_DDC();
  void MtCmdStkSize_DDC(USHORT stksiz);
  void MtDataStkSize_DDC(USHORT stksiz);
  void MtCmdStkRoll_DDC();
  void MtDataStkRoll_DDC();
  void MtEofMsg_DDC();
  void DefDouBuff_DDC();
  SHORT DefDataBlkCB_DDC(USHORT,USHORT,USHORT);
  void EnableIMGap_DDC();
  SHORT DefDataBlkDB_DDC(USHORT t_r,USHORT subaddr,USHORT block_num);
  void BCRetry_DDC(USHORT);
  void BCAutoRepeat_DDC(USHORT);
  void BCAutoRepeat_DDC(USHORT);
  void RTSetBusy_DDC(USHORT);
  void RTMONOpen_DDC();
  void RunRTMon_DDC();
  USHORT BuRTDefMsgLegal_DDC(USHORT,USHORT,USHORT );
  USHORT BuRTDefMsgIllegal_DDC(USHORT,USHORT,USHORT);
  void CloseDeviceHandle_DDC();
  #endif
