

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
    
int	ClearRam_DDC();
void	ResetAce_DDC();  //BOOL
void	StartReset_DDC(USHORT); //BOOL

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
  void EnableFrameAutoRepeat_DDC(UCHAR);
  void BcFrameTime_DDC(USHORT);
  void AutoRetry_DDC(USHORT,UCHAR);
  void DisableIllegLogic_DDC();
  void SelectModule_DDC(UCHAR,UCHAR);  //BOOL
  UCHAR CalculateParity_DDC(UCHAR);
//CHAR  SetRtAddrM_DDC(UCHAR,UCHAR,UCHAR );
char SetRtAddrM_DDC(unsigned short RtAddr,unsigned int CCSC_FD,unsigned char CardId);

  void  InitInterrupts_DDC(UCHAR,UCHAR);
  void DisableInterrupts_DDC(UCHAR,UCHAR);
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
  void GenerateRtc_DDC(UCHAR,UCHAR,UCHAR);
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
  
