
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

    Libraries.h

Abstract:

    Defines All the Function Prototypes	

Author:

     Mahender Reddy G.   Jan 7 2004


Environment:

    Windows - 2000/XP


Revision History:

    
    Added Interrupt Functions Dec 23 2005

    Added DefFrameStackA Function Sep 5 2006

	Added Get_NoOfModulesForCard Function Sep 23 2006

    Added HW_LoopBack Function Oct 1 2006

    Added SetMT_CmdStack_Roll_OverCnt Function Oct 1 2006

    Added GetMT_CmdStack_Roll_OverCnt Function Oct 1 2006

    Added SetMT_DataStack_Roll_OverCnt Function Oct 1 2006

    Added GetMT_DataStack_Roll_OverCnt Function Oct 1 2006

	Added DefBuffSize Function for 31Rts Aug 30 2008

	Added DefDataBlockCB Function for 31Rts Aug 30 2008

	Added ReadDataBufferCB Function for 31Rts Aug 30 2008

	Added WriteDataBufferCB Function for 31Rts Aug 30 2008

	Added ClearCircularBufferRam() Function for 31Rts Aug 30 2008

	Changes in GetRTEOMCnt(), SetRTEOMCnt(), GetRTCIRCCnt(), SetRTCIRCCnt() Function Sep 2008

    
--*/


#include "include.h"

//#define EXP  _declspec (dllexport)

#define EXP  extern "C"__declspec (dllexport)
//#define EXP  __declspec (dllimport)

EXP UCHAR GetPLXport_A(ULONG offset,UCHAR CardId);
EXP BOOL SetPLXport_A(ULONG offset,UCHAR val ,UCHAR CardId);
///////////////////////////////// START of DDC Functions ////////////////////////////////////////////////////
EXP BOOL ResetCard();

EXP BOOL MemWrite(USHORT ,USHORT,UCHAR);
EXP USHORT MemRead(USHORT,UCHAR);
EXP PCHAR ClearRam(UCHAR);
EXP PCHAR ResetAce(UCHAR);
EXP BOOL StartReset(USHORT,UCHAR);
EXP BOOL SelfTest(UCHAR CardId);



EXP BOOL RegWrite(USHORT,USHORT,UCHAR);
EXP USHORT RegRead(USHORT,UCHAR);

EXP USHORT DefMode(USHORT,UCHAR);
EXP BOOL DefIntMask(INT_DEF,UCHAR);
EXP BOOL SelectModule(UCHAR,UCHAR);
EXP SHORT DefMsg(USHORT,MSG_DEF,USHORT data_buffer[],USHORT,UCHAR);
EXP SHORT DefMsg_StackB(USHORT msg_num,MSG_DEF def,USHORT data_buffer[]
			  ,USHORT num_of_data_words,UCHAR CardId);

EXP SHORT DefFrame_StackB(USHORT number_of_messages,USHORT mess_sequence[64],UCHAR CardId);

EXP SHORT RunBcInt(USHORT frame,USHORT position,USHORT num_of_messages,USHORT times,UCHAR CardId);

EXP SHORT DefFrame(USHORT,USHORT mess_sequence[64],UCHAR);
EXP SHORT RunBc(USHORT,USHORT,USHORT ,USHORT,UCHAR);
EXP SHORT ReadMsgResult(USHORT,USHORT,MSG_RESULT *result,UCHAR);

EXP PCHAR SetRtAddr(UCHAR Address[],UCHAR CardId);//UCHAR RtAddr,UCHAR CardId
EXP PCHAR SetRtAddrM(UCHAR Address[],UCHAR CardId);//UCHAR RtAddr,UCHAR ModuleId,UCHAR CardId
EXP SHORT DefLookUp(USHORT look_up_num,UCHAR CardId);
EXP BOOL DefFeatures(CFG2 options,UCHAR CardId);

EXP BOOL RunRt(UCHAR Address[],UCHAR CardId);
EXP SHORT ReadMsg(USHORT stack_entry,UCHAR RtNo,RT_MSG *message,UCHAR CardId);
EXP SHORT WriteDataBlock(UCHAR RtNo,USHORT ,USHORT data_buff[],UCHAR );

EXP BOOL SetSsflag(UCHAR RtNo,UCHAR CardId);
EXP BOOL SetSvcReq(UCHAR RtNo,UCHAR CardId);
EXP BOOL SetBusy(UCHAR RtNo,UCHAR CardId);
EXP BOOL SetDba(UCHAR RtNo,UCHAR CardId);

EXP BOOL ResetSsflag(UCHAR RtNo,UCHAR CardId);
EXP BOOL ResetSvcReq(UCHAR RtNo,UCHAR CardId);
EXP BOOL ResetBusy(UCHAR RtNo,UCHAR CardId);
EXP BOOL ResetDba(UCHAR RtNo,UCHAR CardId);

EXP BOOL SelMsgMt(UCHAR CardId);
EXP BOOL RunMt(UCHAR CardId);
EXP BOOL ReadMonPacket(USHORT DespNo,MNTPACK *MontDataBuff,UCHAR CardId);

EXP BOOL InterMsgGap(USHORT MsgNo,USHORT Gap,UCHAR CardId);

EXP BOOL SelWordMt(UCHAR CardId);


EXP BOOL SelAll(UCHAR);
EXP BOOL DeSelAll(UCHAR);



EXP BOOL EnableEnhancedRt(UCHAR CardId);
EXP SHORT DefMsgIlegal(USHORT subaddr,USHORT wcl,USHORT wcu,USHORT bcst,USHORT tx_recv,UCHAR CardId);
EXP SHORT DefModeCodeIllegal(int modecode,int bcst,UCHAR CardId);
EXP SHORT DefBuffSize(UCHAR RtNo,int subaddr,int mode,int size,UCHAR CardId);
EXP SHORT DefEomInt(int subaddr,int mode,UCHAR CardId);
EXP SHORT DefBuffInt(int subaddr,int mode,UCHAR CardId);
EXP SHORT ReadEnhancedMsg(USHORT stack_entry, RT_ENHCD_MSG *message,UCHAR CardId);
EXP SHORT WriteEnhancedDataBlock(UCHAR RtNo,USHORT subaddr,USHORT data_buff[],USHORT Buff_Size,UCHAR CardId);
EXP SHORT WriteEnhancedDataBlockTmp(USHORT Block_Num,USHORT data_buff[],UCHAR CardId);
//EXP SHORT ReadDataBlock(USHORT data_block,USHORT *data_buff,UCHAR CardId);
EXP SHORT ReadDataBlock(UCHAR RtNo,USHORT ,USHORT data_buff[],UCHAR );
EXP USHORT CheckMsgActivity(UCHAR CardId);

EXP SHORT DefDataBlk(USHORT t_r,USHORT subaddr,USHORT block_num,UCHAR CardId);
EXP BOOL DefBcEnhanceMode(UCHAR CardId);
EXP BOOL EnableInterMsgGap(UCHAR CardId);
EXP BOOL EnableFrameAutoRepeat(UCHAR IntTrigger,UCHAR CardId);
EXP BOOL BcFrameTime(USHORT FrameTime,UCHAR CardId);
//EXP BOOL AutoRetry(WORD MsgNo,UCHAR SinOrDub,UCHAR CardId);//mk;
EXP BOOL DisableIllegLogic(UCHAR CardId);
EXP int NumOfMsg(UCHAR CardId);
EXP BOOL SelCmd(CMD_WRD *temp,UCHAR CardId);

EXP BOOL DeSelCmd(CMD_WRD *temp,UCHAR CardId);


EXP BOOL MtCmdStkSize(USHORT ,UCHAR);
EXP BOOL MtDataStkSize(USHORT ,UCHAR);
EXP BOOL MtCmdStkRoll(UCHAR);
EXP BOOL MtDataStkRoll(UCHAR);
EXP BOOL MtEofMsg(UCHAR);
EXP BOOL DefDouBuff(UCHAR);
EXP SHORT DefDataBlkCB(UCHAR,UCHAR,UCHAR,UCHAR,UCHAR);
EXP BOOL EnableIMGap(UCHAR);
EXP SHORT DefDataBlkDB(USHORT ,USHORT ,USHORT ,UCHAR);
EXP BOOL BCRetry(USHORT,UCHAR);
EXP BOOL BCAutoRepeat(USHORT,UCHAR);
EXP BOOL BCTimeout(USHORT ,UCHAR );
EXP BOOL RTSetBusy(USHORT,UCHAR);
EXP BOOL RTMONOpen(UCHAR);
EXP BOOL RunRTMon(UCHAR);
EXP USHORT BuRTDefMsgLegal(USHORT,USHORT,USHORT,UCHAR );
EXP USHORT BuRTDefMsgIllegal(USHORT,USHORT,USHORT,UCHAR);


EXP UCHAR ReadCardId(UCHAR);
EXP BOOL SelectModuleE(UCHAR,UCHAR);
EXP BOOL SelectCard(UCHAR);
EXP BOOL SelectCardE(UCHAR);

EXP BOOL InitInterrupts(UCHAR,UCHAR);
EXP ULONG GetM1Cnt(UCHAR,UCHAR);
EXP ULONG GetM2Cnt(UCHAR,UCHAR);
EXP ULONG GetM3Cnt(UCHAR,UCHAR);
EXP ULONG GetM4Cnt(UCHAR,UCHAR);
EXP ULONG GetRtcCnt(UCHAR);
EXP BOOL SetM1Cnt(UCHAR,ULONG,UCHAR);
EXP BOOL SetM2Cnt(UCHAR,ULONG,UCHAR);
EXP BOOL SetM3Cnt(UCHAR,ULONG,UCHAR);
EXP BOOL SetM4Cnt(UCHAR,ULONG,UCHAR);
EXP BOOL SetRtcCnt(ULONG,UCHAR);
EXP BOOL GenerateRtc(UCHAR,USHORT,UCHAR);
EXP ULONG GetRtc1Cnt(UCHAR CardId);
EXP ULONG GetRtc2Cnt(UCHAR CardId);
EXP BOOL SetRtc1Cnt(ULONG,UCHAR CardId);
EXP BOOL SetRtc2Cnt(ULONG,UCHAR CardId);
EXP BOOL DisableInterrupts(UCHAR Val,UCHAR CardId);

EXP BOOL CloseDeviceHandle();

/////////////////////////////////////// END of DDC Functions ///////////////////////////////////

///////////////////   APOORVA PCI CARD Prototypes	////////////////
////////////////////////////////////////////////////////////////////

/////////////////////////////////////// START of APOORVA Functions ///////////////////////////////////


EXP BOOL InitDriver_A(UCHAR CardId);
EXP BOOL OnMemMap_A(UCHAR CardId);
EXP BOOL InitCard_A(UCHAR CardId);
EXP BOOL GetCardPresence_A(UCHAR);
EXP SHORT SelfTest_A(UCHAR CardId);
EXP BOOL SoftwareReset(UCHAR);
EXP PCHAR PrintErrorMsg(int);
EXP PCHAR GetMfgId_VerId(UCHAR);
EXP UCHAR	Get_NoOfModulesForCard(UCHAR);
EXP void DefErrors();


EXP USHORT MemRead_A(UCHAR Bus,ULONG len,UCHAR);
EXP USHORT MemWrite_A(UCHAR Bus,ULONG len, USHORT val,UCHAR);
EXP USHORT RegRead_A(UCHAR Bus,ULONG len,UCHAR);
EXP USHORT RegWrite_A(UCHAR Bus,ULONG len,USHORT val,UCHAR);
EXP CHAR	ClearRam_A(UCHAR Bus,USHORT Module,UCHAR);
EXP CHAR	ClearCircularBufferRam_A(UCHAR Bus,USHORT Module,UCHAR CardId);//------------------

EXP UCHAR WritePort_A(ULONG offset,UCHAR val,UCHAR);
EXP UCHAR ReadPort_A(UCHAR offset,UCHAR);
EXP CHAR CloseDeviceHandle_A(UCHAR CardId);

/////////////////////////////////////////////////////////////////////////////////////////////////////

EXP BSW_BITS ReadBswBits(USHORT blk_statwrd);
EXP STATUS_BITS ReadStatusBits(USHORT status);
EXP MSG_DEF ReturnCntrlFields(USHORT control);
EXP CMD ReturnCmdFields(USHORT command);

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////BUS CONTROLLER/////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

EXP CHAR ConfigureBC_A(UCHAR Bus,USHORT stack_size,UCHAR);
EXP CHAR IntMsgGapLeast_A(UCHAR Bus,UCHAR flag,UCHAR);
EXP SHORT DefMsg_StackA_A(UCHAR Bus,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words,UCHAR);
EXP SHORT DefMsg_StackB_A(UCHAR Bus,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words,UCHAR);
EXP SHORT DefMsg_A(UCHAR Bus,USHORT BCSTACK_A_B,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words,UCHAR);
EXP SHORT DefFrame_A(UCHAR Bus,USHORT BCSTACK_A_B,USHORT number_of_messages,USHORT mess_sequence[173],UCHAR);
EXP SHORT DefMajorFrame_A(UCHAR Bus,USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[128],UCHAR);
EXP SHORT DefMajorFrameStackA_A(UCHAR Bus,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[128],UCHAR);
EXP SHORT RunBc_A(UCHAR Bus,USHORT stackA_B,USHORT position,USHORT num_of_messages,ULONG times,UCHAR);
EXP SHORT RunBcInt_A(UCHAR Bus,USHORT stackA_B,USHORT position,USHORT num_of_messages,ULONG times,UCHAR);
EXP SHORT RunBcNowait_A(UCHAR Bus,USHORT stackA_B,USHORT position,USHORT num_of_messages,ULONG times,UCHAR CARD);
EXP SHORT RunBcMajorFrame_A(UCHAR Bus,USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[128],ULONG times,UCHAR);
EXP SHORT ReadMsgBC_A(UCHAR Bus,USHORT stackA_B,USHORT frame_item,MSG_RESULT *result,UCHAR);
EXP CHAR ResetBc_A(UCHAR Bus,UCHAR CardId);
EXP CHAR BCAutoRepeat_A(UCHAR Bus,UCHAR sel,UCHAR);
EXP CHAR BCRetry_A(UCHAR Bus,USHORT sel,UCHAR);
EXP CHAR ErrorInjection_A(UCHAR Bus,ERR_INJ Err,UCHAR CardId);
EXP CHAR ErrorInjectReset_A(UCHAR Bus,UCHAR CardId);
EXP CHAR InterMsgGap_A(UCHAR Bus,UCHAR MsgNo,USHORT Inter_Msg_Gap,UCHAR CardId);
EXP CHAR	BcTimeTagReset_A(UCHAR Bus,UCHAR);//----------------------
EXP CHAR	BcTTRResolution_A(UCHAR Bus,USHORT TimeRes,UCHAR);//-------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////Remote Terminal/////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
EXP CHAR ConfigureRT(UCHAR Bus,USHORT Module,UCHAR CardId);
EXP CHAR RunSingleRt(UCHAR Bus,UCHAR RTSTACK_A_B,UCHAR CardId);
EXP CHAR RtTimeTagReset_A(UCHAR Bus,UCHAR);
EXP CHAR RtTTRResolution_A(UCHAR Bus,USHORT TimeRes,UCHAR);
/////////////////////////////////////////////////////////////////////////////////////////////////////
EXP CHAR ReadMsgRT_A(UCHAR Bus,UCHAR MsgNo,UCHAR RTSTACK_A_B,RT_MSG *message,UCHAR);
EXP CHAR IllegalizeCmd_A(UCHAR,CMD_WRD *temp,UCHAR); 
EXP CHAR LegalizeCmd_A(UCHAR,CMD_WRD *temp,UCHAR);
EXP CHAR ResetRt(UCHAR Bus,UCHAR CardId);
EXP CHAR SetSingleSrvReq(UCHAR Bus,UCHAR req,UCHAR CardId);
EXP CHAR SetSingleBusyBit(UCHAR Bus,UCHAR flag,UCHAR CardId);
EXP CHAR SetSingleSubSys(UCHAR Bus,UCHAR flag,UCHAR CardId);
EXP CHAR SetSingleDBA(UCHAR Bus,UCHAR flag,UCHAR CardId);
EXP CHAR SetSingleTrFlag(UCHAR Bus,UCHAR Tr,UCHAR CardId);
////////////////////////////////  Enable/Disable Channels  ///////////////////////////////

EXP BOOL    EnableRtChannel(UCHAR Bus,UCHAR Channel,UCHAR CardId);
EXP BOOL    DisableRtChannel(UCHAR Bus,UCHAR Channel,UCHAR CardId);
///////////////////////////////   Enable/Disable Channels   for BCRTMT////////////////////////////////
//EXP BOOL    EnableRtChannel_A(UCHAR Bus,UCHAR Channel,UCHAR CardId);
//EXP BOOL    DisableRtChannel_A(UCHAR Bus,UCHAR Channel,UCHAR CardId);
//EXP	CHAR ConfigureRTCB_A(UCHAR Bus,USHORT Module,USHORT BlockSize,UCHAR);*/




//////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////  31 REMOTE TERMINALS	/////////////////

EXP CHAR	ConfigureRT_A(UCHAR Bus,UCHAR A_B,UCHAR Address[],UCHAR);
EXP CHAR	RunRt_A(UCHAR Bus,UCHAR RtStack_A_B,UCHAR Address[],UCHAR);
EXP	CHAR    StartRt_A(UCHAR Bus,UCHAR RtStack_A_B,UCHAR RtNo,UCHAR CardId);
EXP	CHAR	ResetRt_A(UCHAR Bus,USHORT Address,UCHAR CardId);
EXP	CHAR	ConfigureRTCB_A(UCHAR Bus,USHORT RtAddr,USHORT BlockSize,UCHAR);
EXP	CHAR	WriteDataBlockCB_A(UCHAR Bus,USHORT Module,USHORT datablock,USHORT data_buff[],USHORT BlockSize,UCHAR);
EXP CHAR    WriteDataBlock_A(UCHAR Bus,UCHAR RtNo,UCHAR SubAddr,USHORT data_buff[],UCHAR CardId);
EXP CHAR    ReadDataBlock_A(UCHAR Bus,UCHAR RtNo,UCHAR SubAddr,USHORT data_buff[],UCHAR CardId);  //For BcRtMt RtNo refers to RTSTACK_A_B 
//EXP CHAR	ReadDataBlockCB_A(UCHAR Bus,USHORT Module,USHORT data_block,USHORT *data_buff,USHORT BlockSize,UCHAR);

//EXP	CHAR	ReadDataBlockCB_A(UCHAR Bus,UCHAR subaddr,USHORT *data_buff,USHORT data_block,UCHAR CardId);
EXP	CHAR	ReadDataBlockCB_A(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,USHORT *data_buff,USHORT data_block,UCHAR CardId);



EXP CHAR	LegalizeAll_A(UCHAR Bus,UCHAR RtNo,UCHAR CardId);
EXP CHAR	IllegalizeAll_A(UCHAR Bus,UCHAR RtNo,UCHAR CardId);
EXP CHAR	SetSrvReq_A(UCHAR Bus,UCHAR RtNo,UCHAR req,UCHAR CardId);
EXP CHAR	SetTrFlag_A(UCHAR Bus,UCHAR RtNo,UCHAR Tr,UCHAR CardId);
EXP CHAR	SetSubSys_A(UCHAR Bus,UCHAR RtNo,UCHAR flag,UCHAR CardId);
EXP CHAR	SetBusy_A(UCHAR Bus,UCHAR RtNo,UCHAR flag,UCHAR CardId);
EXP CHAR	SetDBA_A(UCHAR Bus,UCHAR RtNo,UCHAR flag,UCHAR CardId);
EXP CHAR	ClrTTROnSync_A(UCHAR Bus,UCHAR RtNo,UCHAR TTRFlag,UCHAR CardId);
//EXP CHAR    ClrTTROnSync_A(UCHAR Bus,UCHAR flag,UCHAR CardId);//bcrtmt
EXP CHAR	LoadTTROnSync_A(UCHAR Bus,UCHAR RtNo,UCHAR flag,UCHAR CardId);
EXP SHORT	DefBuffSize_A(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,UCHAR t_r,USHORT size,UCHAR CardId);
EXP SHORT	DefDataBlkCB_A(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,UCHAR t_r,USHORT block_num,UCHAR CardId);
//EXP SHORT	DefDataBlkCB_A(UCHAR Bus,UCHAR subaddr,UCHAR t_r,USHORT block_num,UCHAR CardId);
EXP CHAR	WriteDataBufferCB_A(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,USHORT data_buff[],USHORT Buff_Size,UCHAR CardId);
EXP CHAR	ReadDataBufferCB_A(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,USHORT *data_buff,USHORT Buff_Size,UCHAR CardId);
//EXP CHAR	ReadDataBufferCB_A(UCHAR Bus,UCHAR subaddr,USHORT *data_buff,USHORT Buff_Size,UCHAR CardId);


////////////////////////////////  Enable/Disable Channels   for 31 rts///////////////////////////////

EXP BOOL    EnableRtChannel_A(UCHAR Bus,UCHAR RtNo,UCHAR Channel,UCHAR CardId);//-------------------
EXP BOOL    DisableRtChannel_A(UCHAR Bus,UCHAR RtNo,UCHAR Channel,UCHAR CardId);//------------------


//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
///////////////////////////  MONITOR TERMINAL	/////////////////////
EXP CHAR SelMsgMt_A(UCHAR Bus,UCHAR MTSTACK_A_B,UCHAR);
EXP CHAR RunMt_A(UCHAR Bus,UCHAR);
EXP CHAR ReadMonPacket_A(UCHAR Bus,UCHAR MTSTACK_A_B,USHORT DespNo,MNTPACK *MontDataBuff,UCHAR);
EXP CHAR SelWordMt_A(UCHAR Bus,UCHAR);
EXP CHAR DeSelAll_A(UCHAR Bus,UCHAR);
EXP CHAR SelAll_A(UCHAR Bus,UCHAR);
EXP CHAR SelCmd_A(UCHAR Bus,CMD_WRD *temp,UCHAR);
EXP CHAR DeSelCmd_A(UCHAR Bus,CMD_WRD *temp,UCHAR);
EXP CHAR ReadMtWord_A(UCHAR Bus,MT_WORD_DEF *result,UCHAR);
EXP CHAR ResetMt_A(UCHAR,UCHAR);
EXP CHAR MtTimeTagReset_A(UCHAR Bus,UCHAR);
EXP CHAR MtTTRResolution_A(UCHAR Bus,USHORT TimeRes,UCHAR);


////////////////////////////////////////////////////////////////////////////////////////////////
EXP CHAR ReadEnhancedMsgRT_A(UCHAR Bus,UCHAR RTSTACK_A_B,UCHAR MsgNo,RT_ENHCD_MSG *message,UCHAR CardId);
/////////////////////////////////INTERRUPT///////////////////////////////////

EXP CHAR  EventCreate(UCHAR);
EXP CHAR EventClose(UCHAR CardId);
EXP CHAR  WaitForInterrupt(UCHAR);
EXP LONG GetIntrStatus(UCHAR Bus,UCHAR);
EXP LONG GetTimerIntrStatus(UCHAR CardId);


EXP CHAR InitInterrupts_A(UCHAR,UCHAR val,UCHAR);
EXP CHAR InitTimerIntr_A(UCHAR,UCHAR);
EXP CHAR StopTimerIntr_A(UCHAR Value,UCHAR CardId);
EXP CHAR SelectTMR_A(UCHAR,UCHAR,UCHAR,UCHAR);
EXP CHAR DisableInterrupts_A(UCHAR,UCHAR);


EXP LONG GetBCEOMCnt_A(UCHAR,UCHAR);
EXP LONG GetRTEOMCnt_A(UCHAR,USHORT,UCHAR);
EXP LONG GetMTEOMCnt_A(UCHAR,UCHAR);
//EXP LONG GetRTCIRCCnt_A(UCHAR,UCHAR);
EXP LONG GetRTCIRCCnt_A(UCHAR Bus,USHORT RtNo,UCHAR CardId);	
EXP LONG GetRTDBCACnt_A(UCHAR,UCHAR);
EXP LONG GetTMR1Cnt_A(UCHAR);
EXP LONG GetTMR2Cnt_A(UCHAR);
EXP LONG GetTMR3Cnt_A(UCHAR);


EXP CHAR SetBCEOMCnt_A(UCHAR,ULONG value,UCHAR);
EXP CHAR SetRTEOMCnt_A(UCHAR,USHORT,ULONG value,UCHAR);
EXP CHAR SetMTEOMCnt_A(UCHAR,ULONG value,UCHAR);
//EXP CHAR SetRTCIRCCnt_A(UCHAR,ULONG value,UCHAR);
EXP CHAR SetRTCIRCCnt_A(UCHAR Bus,USHORT RtNo,ULONG value,UCHAR CardId)	;		//Set BC Interrupt Count to given Value;
EXP CHAR SetRTDBCACnt_A(UCHAR,ULONG value,UCHAR);
EXP CHAR SetTMR1Cnt_A(ULONG,UCHAR);
EXP CHAR SetTMR2Cnt_A(ULONG,UCHAR);
EXP CHAR SetTMR3Cnt_A(ULONG,UCHAR);

EXP SHORT GetMinorFrameCnt_A(UCHAR Bus,UCHAR CardId);
EXP CHAR InitEnhancedInterrupts_A(UCHAR Bus,UCHAR Value,UCHAR CardId);
EXP CHAR DisableEnhancedInterrupts_A(UCHAR Bus,UCHAR Value,UCHAR CardId);



EXP CHAR BcTimeTagReset_A(UCHAR Bus,UCHAR);
EXP CHAR BcTTRResolution_A(UCHAR Bus,USHORT TimeRes,UCHAR);
EXP LONG GetEndOfFrameCnt_A(UCHAR Bus,UCHAR CardId);
EXP CHAR SetEndOfFrameCnt_A(UCHAR Bus,ULONG value,UCHAR CardId);
EXP LONG GetMT_TTR_Roll_OverCnt_A(UCHAR Bus,UCHAR CardId);
EXP CHAR SetMT_TTR_Roll_OverCnt_A(UCHAR Bus,ULONG value,UCHAR CardId);
EXP BOOL ResetDevice_A(UCHAR CardId);

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////END/////////////////////////////////////////

