

#include "ace.h"
#include "addr.h"
#include "mode.h"
#include "int.h"
#include "reg2.h"
#include "modecode.h"
#include "gnrl.h"
#include "mess.h"
#include "mt.h"
#include "rt_mess.h"

#define BOOL bool
#define LONG long
#define CHAR	char
  
//void	WritePort(ULONG,UCHAR);
//UCHAR	ReadPort(ULONG);
    
unsigned int	ClearRam(UCHAR ModId);
void	ResetAce(UCHAR ModId);  //BOOL
void	StartReset(UCHAR ModId,USHORT); //BOOL

int	InitCard(UCHAR CardId);
CHAR	GetCardPresence(UCHAR);
SHORT	SelfTest(UCHAR CardId);
CHAR	SoftwareReset(UCHAR);
//PCHAR	PrintErrorMsg(int);
//PCHAR	GetMfgId_VerId(UCHAR);
//void	DefErrors();
CHAR	ConfigICSR(UCHAR CardId);

/*USHORT	MemRead(UCHAR Bus,ULONG len,UCHAR);
USHORT	MemWrite(UCHAR Bus,ULONG len, USHORT val,UCHAR);
USHORT	RegRead(UCHAR Bus,ULONG len,UCHAR);
USHORT	RegWrite(UCHAR Bus,ULONG len,USHORT val,UCHAR);*/
/*USHORT	MemRead(ULONG len);
USHORT	MemWrite(ULONG len, USHORT val);
USHORT	RegRead(UCHAR Bus,ULONG len,UCHAR);
USHORT	RegWrite(UCHAR Bus,ULONG len,USHORT val,UCHAR);*/

//void ConfigICSR();

  USHORT DefMode(UCHAR ModId,USHORT);
  void DefIntMask(UCHAR ModId,INT_DEF);
  short DefMsg(UCHAR ModId,unsigned short msg_num,MSG_DEF def,unsigned short data_buffer[],unsigned short num_of_data_words,unsigned short IntMsgGap);
  SHORT DefMsg_StackB(UCHAR ModId,USHORT,MSG_DEF,USHORT data_buffer[],USHORT);
  short DefFrame(UCHAR ModId,unsigned short number_of_messages,unsigned short mess_sequence[1024]);
  SHORT DefFrame_StackB(UCHAR ModId,USHORT,USHORT mess_sequence[64]);
  short RunBc(UCHAR ModId,unsigned short frame,unsigned short position,unsigned short num_of_messages,unsigned short times);
  SHORT RunBcInt(UCHAR ModId,USHORT,USHORT,USHORT ,USHORT);
  CMD		ReturnCmdFields(UCHAR ModId,USHORT);
  MSG_DEF		ReturnCntrlFields(UCHAR ModId,USHORT);
  BSW_BITS		ReadBswBits(UCHAR ModId,USHORT);
  STATUS_BITS	ReadStatusBits(UCHAR ModId,USHORT);
  SHORT ReadMsgResult(UCHAR ModId,USHORT,USHORT,MSG_RESULT *result);
  SHORT DefLookUp(UCHAR ModId,USHORT );
  void DefFeatures(UCHAR ModId,CFG2);
  void EnableEnhancedRt(UCHAR ModId);
  SHORT DefMsgIlegal(UCHAR ModId,USHORT,USHORT,USHORT,USHORT,USHORT);
  SHORT DefModeCodeIllegal(UCHAR ModId,int,int);
  SHORT DefBuffSize(UCHAR ModId,int,int,int);

  SHORT DefEomInt(UCHAR ModId,int,int);
  SHORT DefBuffInt(UCHAR ModId,int,int);
  SHORT ReadEnhancedMsg(UCHAR ModId,USHORT, RT_ENHCD_MSG *message);
  INTSTAT_REG ReturnIntStatus(UCHAR ModId,USHORT);
  SHORT WriteDataBlock(UCHAR ModId,USHORT,USHORT data_buff[]);
  SHORT ReadDataBlock(UCHAR ModId,USHORT,USHORT *data_buff);
  void SetSsflag(UCHAR ModId);
  void SetSvcReq(UCHAR ModId);
  void SetBusy(UCHAR ModId);
  void SetDba(UCHAR ModId);
  void ResetSsflag(UCHAR ModId);
  void ResetSvcReq(UCHAR ModId);
  void ResetBusy(UCHAR ModId);
  void ResetDba(UCHAR ModId);
  CONFIG_BITS ReadConfigBits(UCHAR ModId);
  void RunRt(UCHAR ModId);
  USHORT CheckMsgActivity(UCHAR ModId);
  SHORT ReadMsg(UCHAR ModId,USHORT, RT_MSG *message);
  SHORT DefDataBlk(UCHAR ModId,USHORT,USHORT,USHORT);
  void DefErrors(UCHAR ModId);

  void Version(UCHAR ModId);
  void DefBcEnhanceMode(UCHAR ModId);
  void EnableInterMsgGap(UCHAR ModId);
  void InterMsgGap(UCHAR ModId,USHORT,USHORT);
  void EnableFrameAutoRepeat(UCHAR ModId,UCHAR);
  void BcFrameTime(UCHAR ModId,USHORT);
  void AutoRetry(UCHAR ModId,USHORT,UCHAR);
  void DisableIllegLogic(UCHAR ModId);
  void SelectModule(UCHAR ModId,UCHAR,UCHAR);  //BOOL
  UCHAR CalculateParity(UCHAR ModId,UCHAR);
//CHAR  SetRtAddrM(UCHAR,UCHAR,UCHAR );
char SetRtAddrM(UCHAR ModId,unsigned short RtAddr,unsigned char CardId);
 // void InitInterrupts(unsigned char Intr,unsigned char ModId);
  //void  InitInterrupts(UCHAR ModId,UCHAR,UCHAR);
  void DisableInterrupts(UCHAR ModId,UCHAR,UCHAR);
  int GetM1Cnt();
  int GetM2Cnt();
  int GetM3Cnt();
  int GetRtcCnt();
  int GetRtc1Cnt();
  int GetRtc2Cnt();
  void SetM1Cnt();
  void SetM2Cnt();
  void SetM3Cnt();
  void SetRtcCnt();
  void SetRtc1Cnt();
  void SetRtc2Cnt();
  void GenerateRtc(UCHAR ModId,UCHAR,UCHAR,UCHAR);
  void SelWordMt(UCHAR ModId);
  SHORT ReadMtWord(UCHAR ModId,MT_WORD_DEF *result);
  void SelMsgMt(UCHAR ModId);
  void RunMt(UCHAR ModId);
  void ReadMonPacket(UCHAR ModId,USHORT ,MNTPACK *MontDataBuff);
  int NumOfMsg(UCHAR ModId);
  void SelCmd(UCHAR ModId,CMD_WRD *temp);
  void DeSelCmd(UCHAR ModId,CMD_WRD *temp);
  void SelAll(UCHAR ModId);
  void DeSelAll(UCHAR ModId);
  void MtCmdStkSize(UCHAR ModId,USHORT stksiz);
  void MtDataStkSize(UCHAR ModId,USHORT stksiz);
  void MtCmdStkRoll(UCHAR ModId);
  void MtDataStkRoll(UCHAR ModId);
  void MtEofMsg(UCHAR ModId);
  void DefDouBuff(UCHAR ModId);
  SHORT DefDataBlkCB(UCHAR ModId,USHORT,USHORT,USHORT);
  void EnableIMGap(UCHAR ModId);
  SHORT DefDataBlkDB(UCHAR ModId,USHORT t_r,USHORT subaddr,USHORT block_num);
  void BCRetry(UCHAR ModId,USHORT);
  void BCAutoRepeat(UCHAR ModId,USHORT);
  void BCAutoRepeat(UCHAR ModId,USHORT);
  void RTSetBusy(UCHAR ModId,USHORT);
  void RTMONOpen(UCHAR ModId);
  void RunRTMon(UCHAR ModId);
  USHORT BuRTDefMsgLegal(USHORT,USHORT,USHORT );
  USHORT BuRTDefMsgIllegal(USHORT,USHORT,USHORT);
  void CloseDeviceHandle();
  int WriteMemW(UCHAR ModId,unsigned short Data,unsigned int Offset);
  int WriteMemA(UCHAR ModId,unsigned short Data,unsigned int Offset);
  int DefMajorFrame(UCHAR Bus,USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[]);
  unsigned long RunBcMajorFrame(UCHAR Bus,USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],unsigned long times);
  void UpdateMsgData(UCHAR ModId, unsigned short msg_num, unsigned short *new_data, int num_words, int direction);
  int UpdateMsgCmdData(UCHAR ModId,unsigned short msg_num,unsigned short cmd1,unsigned short cmd2,unsigned short *payload_buffer,unsigned short num_of_data_words,int direction,       // RECEIVE / TRANSMIT / RT_RT / MODE_CODE
                     int bus);
  void SetTerFlag(UCHAR ModId);
  void ResetTerFlag(UCHAR ModId);
  void PriorityHighLow(UCHAR ModId, unsigned short msg_num, unsigned short HighLow);
  void BCResponseTimeE(UCHAR ModId,UCHAR Enable, unsigned short BCResponseTime);
  void BCStartStop(UCHAR ModId,UCHAR Stop);
  int ReadMsgRtCounter(UCHAR ModId);
  int ReadMsgMtCounter(UCHAR ModId);
  
  void BC_Mode(int BcMod, int channel, int NoOfTimes);

void MT_Mode(int mtModId);

void RT_Mode(int BcMod, int rtModId, int rtAddr);

void BC_MT_Mode(int BcMod, int mtModId, int channel, int NoOfTimes);

void BC_RT_Mode(int BcMod, int rtModId, int rtAddr, int channel, int NoOfTimes);

void RT_MT_Mode(int rtModId, int mtModId, int rtAddr);

void RT_Status_Mode(int rtModId, int rtAddr, int RtStatusBits);

void BC_MT_Mode_snode(int BcMod, int mtModId, int channel, int NoOfTimes);

void RT_MT_Mode_snode(int rtModId, int rtAddr);

 CHAR DisableInterruptsRT(UCHAR RtModId, USHORT DISABLE, UCHAR CARDID);
 
CHAR DisableInterruptsMT(UCHAR RtModId, USHORT DISABLE, UCHAR CARDID);
  
CHAR DisableInterruptsBC(UCHAR RtModId, USHORT DISABLE,UCHAR CARDID);

CHAR EnableInterruptsRT(UCHAR RtModId,USHORT ENABLE_RT_INT,UCHAR CARDID);
CHAR EnableInterruptsMT(UCHAR RtModId,USHORT ENABLE_RT_INT,UCHAR CARDID);
CHAR EnableInterruptsBC(UCHAR RtModId,USHORT ENABLE_RT_INT,UCHAR CARDID);


LONG GetBCSOMCnt(UCHAR Bus, UCHAR CardId);
LONG GetBCEOMCnt(UCHAR Bus, UCHAR CardId);
LONG GetBCEOFCnt(UCHAR Bus, UCHAR CardId);
LONG GetBCEOMINFRAMECnt(UCHAR Bus, UCHAR CardId);
LONG GetMTSOMCnt(UCHAR Bus, UCHAR CardId);
LONG GetMTEOMCnt(UCHAR Bus, UCHAR CardId);
LONG GetMTEOMCnt_dummy(UCHAR Bus, UCHAR CardId);
LONG GetRTEOMCnt(UCHAR Bus, UCHAR CardId);
LONG GetBCEOFCnt_D(UCHAR Bus, UCHAR CardId);


CHAR SetBCSOMCnt(UCHAR Bus, LONG value, UCHAR CardId);
CHAR SetBCEOMCnt(UCHAR Bus, LONG value, UCHAR CardId);
CHAR SetBCEOFCnt(UCHAR Bus, LONG value, UCHAR CardId);
CHAR SetBCEOMMINORFRAMECnt(UCHAR Bus, LONG value, UCHAR CardId);
CHAR SetMTSOMCnt(UCHAR Bus, LONG value, UCHAR CardId);
CHAR SetMTEOMCnt(UCHAR Bus, LONG value, UCHAR CardId);
CHAR SetRTSOMCnt(UCHAR Bus, LONG value, UCHAR CardId);
CHAR SetRTEOMCnt(UCHAR Bus, LONG value, UCHAR CardId);



//USHORT MemRead(UCHAR Bus, ULONG len, UCHAR CardId);
//USHORT MemWrite(UCHAR Bus,ULONG len, USHORT val,UCHAR CardId);

//USHORT RegRead(UCHAR Bus, ULONG len, UCHAR CardId);
//USHORT RegWrite(UCHAR Bus,ULONG len,USHORT val,UCHAR CardId);
void disableNonCanonicalMode(void);
void ConfigureRT(UCHAR rtModId, unsigned char rtAddr);
void Async(int BcMod, int mtModId, int channel,int NoOfTimes, int msg_no, int prior);
void Async_Broadcast(int BcMod, int msg_no, int prior);
CHAR InitInterrupts(UCHAR Bus,USHORT Value,UCHAR CardId);

void Configure_DMA_READ(UCHAR Bus, UCHAR CardId);
int Trigger_DMA(UCHAR Bus, UCHAR CardId,uint32_t value);
void read_dma_block(void);
void Configure_DMA_Write(UCHAR Bus, UCHAR CardId);
int DMA_Source_Destination(uint32_t source,uint32_t destination);
void write_via_dma(UCHAR Bus,int total_bytes);
void DEFMSG_DMA(UCHAR ModId,unsigned short msg_num,MSG_DEF def,unsigned short data_buffer[], unsigned short num_of_data_words,unsigned short IntMsgGap);





