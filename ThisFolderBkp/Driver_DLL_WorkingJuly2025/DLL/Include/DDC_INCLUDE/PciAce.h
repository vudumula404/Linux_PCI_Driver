

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
  
//void	WritePort(ULONG,UCHAR);
//UCHAR	ReadPort(ULONG);
    
unsigned int	ClearRam_DDC(UCHAR ModId);
void	ResetAce_DDC(UCHAR ModId);  //BOOL
void	StartReset_DDC(UCHAR ModId,USHORT); //BOOL

int	InitCard(UCHAR CardId);
CHAR	GetCardPresence(UCHAR);
SHORT	SelfTest(UCHAR CardId);
CHAR	SoftwareReset(UCHAR);
//PCHAR	PrintErrorMsg(int);
//PCHAR	GetMfgId_VerId(UCHAR);
void	DefErrors();
CHAR	ConfigICSR(UCHAR CardId);

/*USHORT	MemRead(UCHAR Bus,ULONG len,UCHAR);
USHORT	MemWrite(UCHAR Bus,ULONG len, USHORT val,UCHAR);
USHORT	RegRead(UCHAR Bus,ULONG len,UCHAR);
USHORT	RegWrite(UCHAR Bus,ULONG len,USHORT val,UCHAR);*/
/*USHORT	MemRead_DDC(ULONG len);
USHORT	MemWrite_DDC(ULONG len, USHORT val);
USHORT	RegRead_DDC(UCHAR Bus,ULONG len,UCHAR);
USHORT	RegWrite_DDC(UCHAR Bus,ULONG len,USHORT val,UCHAR);*/

void ConfigICSR_DDC();

  USHORT DefMode_DDC(UCHAR ModId,USHORT);
  void DefIntMask_DDC(UCHAR ModId,INT_DEF_DDC);
  SHORT DefMsg_DDC(UCHAR ModId,USHORT,MSG_DEF_DDC,USHORT data_buffer[],USHORT);
  SHORT DefMsg_StackB_DDC(UCHAR ModId,USHORT,MSG_DEF_DDC,USHORT data_buffer[],USHORT);
  SHORT DefFrame_DDC(UCHAR ModId,USHORT,USHORT mess_sequence[64]);
  SHORT DefFrame_StackB_DDC(UCHAR ModId,USHORT,USHORT mess_sequence[64]);
  SHORT RunBc_DDC(UCHAR ModId,USHORT,USHORT,USHORT ,USHORT);
  SHORT RunBcInt_DDC(UCHAR ModId,USHORT,USHORT,USHORT ,USHORT);
  CMD_DDC		ReturnCmdFields_DDC(UCHAR ModId,USHORT);
  MSG_DEF_DDC		ReturnCntrlFields_DDC(UCHAR ModId,USHORT);
  BSW_BITS_DDC		ReadBswBits_DDC(UCHAR ModId,USHORT);
  STATUS_BITS_DDC	ReadStatusBits_DDC(UCHAR ModId,USHORT);
  SHORT ReadMsgResult_DDC(UCHAR ModId,USHORT,USHORT,MSG_RESULT_DDC *result);
  SHORT DefLookUp_DDC(UCHAR ModId,USHORT );
  void DefFeatures_DDC(UCHAR ModId,CFG2_DDC);
  void EnableEnhancedRt_DDC(UCHAR ModId);
  SHORT DefMsgIlegal_DDC(UCHAR ModId,USHORT,USHORT,USHORT,USHORT,USHORT);
  SHORT DefModeCodeIllegal_DDC(UCHAR ModId,int,int);
  SHORT DefBuffSize_DDC(UCHAR ModId,int,int,int);

  SHORT DefEomInt_DDC(UCHAR ModId,int,int);
  SHORT DefBuffInt_DDC(UCHAR ModId,int,int);
  SHORT ReadEnhancedMsg_DDC(UCHAR ModId,USHORT, RT_ENHCD_MSG_DDC *message);
  INTSTAT_REG_DDC ReturnIntStatus_DDC(UCHAR ModId,USHORT);
  SHORT WriteDataBlock_DDC(UCHAR ModId,USHORT,USHORT data_buff[]);
  SHORT ReadDataBlock_DDC(UCHAR ModId,USHORT,USHORT *data_buff);
  void SetSsflag_DDC(UCHAR ModId);
  void SetSvcReq_DDC(UCHAR ModId);
  void SetBusy_DDC(UCHAR ModId);
  void SetDba_DDC(UCHAR ModId);
  void ResetSsflag_DDC(UCHAR ModId);
  void ResetSvcReq_DDC(UCHAR ModId);
  void ResetBusy_DDC(UCHAR ModId);
  void ResetDba_DDC(UCHAR ModId);
  CONFIG_BITS_DDC ReadConfigBits_DDC(UCHAR ModId);
  void RunRt_DDC(UCHAR ModId);
  USHORT CheckMsgActivity_DDC(UCHAR ModId);
  SHORT ReadMsg_DDC(UCHAR ModId,USHORT, RT_MSG_DDC *message);
  SHORT DefDataBlk_DDC(UCHAR ModId,USHORT,USHORT,USHORT);
  void DefErrors_DDC(UCHAR ModId);

  void Version_DDC(UCHAR ModId);
  void DefBcEnhanceMode_DDC(UCHAR ModId);
  void EnableInterMsgGap_DDC(UCHAR ModId);
  void InterMsgGap_DDC(UCHAR ModId,USHORT,USHORT);
  void EnableFrameAutoRepeat_DDC(UCHAR ModId,UCHAR);
  void BcFrameTime_DDC(UCHAR ModId,USHORT);
  void AutoRetry_DDC(UCHAR ModId,USHORT,UCHAR);
  void DisableIllegLogic_DDC(UCHAR ModId);
  void SelectModule_DDC(UCHAR ModId,UCHAR,UCHAR);  //BOOL
  UCHAR CalculateParity_DDC(UCHAR ModId,UCHAR);
//CHAR  SetRtAddrM_DDC(UCHAR,UCHAR,UCHAR );
char SetRtAddrM_DDC(UCHAR ModId,unsigned short RtAddr,unsigned int CCSC_FD,unsigned char CardId);

  void  InitInterrupts_DDC(UCHAR ModId,UCHAR,UCHAR);
  void DisableInterrupts_DDC(UCHAR ModId,UCHAR,UCHAR);
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
  void GenerateRtc_DDC(UCHAR ModId,UCHAR,UCHAR,UCHAR);
  void SelWordMt_DDC(UCHAR ModId);
  SHORT ReadMtWord_DDC(UCHAR ModId,MT_WORD_DEF_DDC *result);
  void SelMsgMt_DDC(UCHAR ModId);
  void RunMt_DDC(UCHAR ModId);
  void ReadMonPacket_DDC(UCHAR ModId,USHORT ,MNTPACK_DDC *MontDataBuff);
  int NumOfMsg_DDC(UCHAR ModId);
  void SelCmd_DDC(UCHAR ModId,CMD_WRD_DDC *temp);
  void DeSelCmd_DDC(UCHAR ModId,CMD_WRD_DDC *temp);
  void SelAll_DDC(UCHAR ModId);
  void DeSelAll_DDC(UCHAR ModId);
  void MtCmdStkSize_DDC(UCHAR ModId,USHORT stksiz);
  void MtDataStkSize_DDC(UCHAR ModId,USHORT stksiz);
  void MtCmdStkRoll_DDC(UCHAR ModId);
  void MtDataStkRoll_DDC(UCHAR ModId);
  void MtEofMsg_DDC(UCHAR ModId);
  void DefDouBuff_DDC(UCHAR ModId);
  SHORT DefDataBlkCB_DDC(UCHAR ModId,USHORT,USHORT,USHORT);
  void EnableIMGap_DDC(UCHAR ModId);
  SHORT DefDataBlkDB_DDC(UCHAR ModId,USHORT t_r,USHORT subaddr,USHORT block_num);
  void BCRetry_DDC(UCHAR ModId,USHORT);
  void BCAutoRepeat_DDC(UCHAR ModId,USHORT);
  void BCAutoRepeat_DDC(UCHAR ModId,USHORT);
  void RTSetBusy_DDC(UCHAR ModId,USHORT);
  void RTMONOpen_DDC(UCHAR ModId);
  void RunRTMon_DDC(UCHAR ModId);
  USHORT BuRTDefMsgLegal_DDC(USHORT,USHORT,USHORT );
  USHORT BuRTDefMsgIllegal_DDC(USHORT,USHORT,USHORT);
  void CloseDeviceHandle_DDC();
  
