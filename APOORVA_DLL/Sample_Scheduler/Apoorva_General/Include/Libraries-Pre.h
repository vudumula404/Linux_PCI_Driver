
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

#define EXP  _declspec (dllexport)

//#define EXP  extern "C"__declspec (dllexport)
//#define EXP  __declspec (dllimport)

///////////////////   APOORVA PCI CARD Prototypes	////////////////
////////////////////////////////////////////////////////////////////

//////////////////		General		///////////////////////////////

EXP BOOL	InitDriver(UCHAR CardId);
EXP BOOL	OnMemMap(UCHAR CardId); 
EXP BOOL	InitCard(UCHAR CardId);
EXP BOOL	GetCardPresence(UCHAR);
EXP SHORT	SelfTest(UCHAR CardId);
EXP BOOL	SoftwareReset(UCHAR);
EXP PCHAR	PrintErrorMsg(int);
EXP PCHAR	GetMfgId_VerId(UCHAR);
EXP void	DefErrors();
EXP UCHAR	Get_NoOfModulesForCard(UCHAR);

EXP USHORT	MemRead(UCHAR Bus,ULONG len,UCHAR);
EXP USHORT	MemWrite(UCHAR Bus,ULONG len, USHORT val,UCHAR);
EXP USHORT	RegRead(UCHAR Bus,ULONG len,UCHAR);
EXP USHORT	RegWrite(UCHAR Bus,ULONG len,USHORT val,UCHAR);
EXP CHAR	ClearRam(UCHAR Bus,USHORT Module,UCHAR);
EXP CHAR    ClearCircularBufferRam(UCHAR Bus,USHORT Module,UCHAR CardId);

EXP UCHAR	WritePort(ULONG offset,UCHAR val,UCHAR);
EXP UCHAR	ReadPort(UCHAR offset,UCHAR);
EXP CHAR	CloseDeviceHandle(UCHAR CardId);

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

EXP BSW_BITS	ReadBswBits(USHORT blk_statwrd);
EXP STATUS_BITS ReadStatusBits(USHORT status);
EXP MSG_DEF		ReturnCntrlFields(USHORT control);
EXP CMD			ReturnCmdFields(USHORT command);

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
/////////////////////		BUS CONTROLLER		////////////////////



EXP CHAR	ConfigureBC(UCHAR Bus,USHORT stack_size,UCHAR);
EXP CHAR	IntMsgGapLeast(UCHAR Bus,UCHAR flag,UCHAR);
EXP SHORT	DefMsg_StackA(UCHAR Bus,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words,UCHAR);
EXP SHORT	DefMsg_StackB(UCHAR Bus,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words,UCHAR);
EXP SHORT	DefMsg(UCHAR Bus,USHORT BCSTACK_A_B,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words,UCHAR);
EXP SHORT	DefFrame(UCHAR Bus,USHORT BCSTACK_A_B,USHORT number_of_messages,USHORT mess_sequence[173],UCHAR);
EXP SHORT	DefFrameStackA(UCHAR Bus,USHORT number_of_messages,USHORT mess_sequence[],UCHAR CardId);
EXP SHORT	RunBc(UCHAR Bus,USHORT stackA_B,USHORT position,USHORT num_of_messages,ULONG times,UCHAR);
EXP SHORT	DefMajorFrame(UCHAR Bus,USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],UCHAR);
EXP SHORT	DefMajorFrameStackA(UCHAR Bus,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],UCHAR);
EXP SHORT	RunBcMajorFrame(UCHAR Bus,USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],ULONG times,UCHAR);
EXP SHORT	ReadMsgBC(UCHAR Bus,USHORT stackA_B,USHORT frame_item,MSG_RESULT *result,UCHAR);
EXP CHAR	ResetBc(UCHAR Bus,UCHAR CardId);
EXP CHAR	BCAutoRepeat(UCHAR Bus,UCHAR sel,UCHAR);
EXP CHAR	BCRetry(UCHAR Bus,USHORT sel,UCHAR);
EXP CHAR	BcTimeTagReset(UCHAR Bus,UCHAR);
EXP CHAR	BcTTRResolution(UCHAR Bus,USHORT TimeRes,UCHAR);
EXP CHAR	InterMsgGap(UCHAR Bus,UCHAR MsgNo,USHORT Inter_Msg_Gap,UCHAR CardId);
EXP CHAR	MsgEnableDisable(UCHAR Bus,BOOL MsgNos[],UCHAR);
EXP CHAR	ErrorInjection(UCHAR Bus,ERR_INJ Err,UCHAR CardId);
EXP CHAR	ErrorInjectReset(UCHAR Bus,UCHAR CardId);

/////////////////////////////////////////////////////////////////////
///////////////////////////  REMOTE TERMINAL	/////////////////////

EXP CHAR	ConfigureRT(UCHAR Bus,USHORT Module,UCHAR CardId);
EXP CHAR	RunRt(UCHAR Bus,UCHAR RTSTACK_A_B,UCHAR CardId);
EXP CHAR	ResetRt(UCHAR Bus,UCHAR CardId);
EXP CHAR	WriteDataBlockCB(UCHAR Bus,UCHAR subaddr,USHORT data_buff[],USHORT datablock,UCHAR CardId);
EXP CHAR	ReadDataBlockCB(UCHAR Bus,UCHAR subaddr,USHORT *data_buff,USHORT data_block,UCHAR CardId);
EXP CHAR	WriteDataBufferCB(UCHAR Bus,UCHAR subaddr,USHORT data_buff[],USHORT Buff_Size,UCHAR CardId);
EXP CHAR	ReadDataBufferCB(UCHAR Bus,UCHAR subaddr,USHORT *data_buff,USHORT Buff_Size,UCHAR CardId);
EXP CHAR	LegalizeAll(UCHAR Bus,UCHAR CardId);
EXP CHAR	IllegalizeAll(UCHAR Bus,UCHAR CardId);
EXP CHAR	SetSrvReq(UCHAR Bus,UCHAR req,UCHAR CardId);
EXP CHAR	SetTrFlag(UCHAR Bus,UCHAR Tr,UCHAR CardId);
EXP CHAR	SetSubSys(UCHAR Bus,UCHAR flag,UCHAR CardId);
EXP CHAR	SetBusy(UCHAR Bus,UCHAR flag,UCHAR CardId);
EXP CHAR	SetDBA(UCHAR Bus,UCHAR flag,UCHAR CardId);
EXP CHAR	ClrTTROnSync(UCHAR Bus,UCHAR flag,UCHAR CardId);
EXP CHAR	LoadTTROnSync(UCHAR Bus,UCHAR flag,UCHAR CardId);
EXP CHAR	RtTimeTagReset(UCHAR Bus,UCHAR CardId);
EXP CHAR	RtTTRResolution(UCHAR Bus,USHORT TimeRes,UCHAR);
EXP SHORT	DefBuffSize(UCHAR Bus,UCHAR subaddr,UCHAR t_r,USHORT size,UCHAR CardId);
EXP SHORT	DefDataBlkCB(UCHAR Bus,UCHAR subaddr,UCHAR t_r,USHORT block_num,UCHAR CardId);
EXP CHAR	HW_LoopBack(UCHAR Bus,UCHAR Saddr[],UCHAR CardId);

////////////////////////////////////////////////////////////////////

EXP CHAR	WriteDataBlock(UCHAR Bus,UCHAR RTSTACK_A_B,UCHAR subaddr,USHORT data_buff[],UCHAR);
//EXP CHAR	WriteDataBlock(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,USHORT data_buff[],UCHAR);
//EXP CHAR	ReadMsgRT(UCHAR Bus,UCHAR MsgNo,UCHAR RTSTACK_A_B,RT_MSG *message,UCHAR);
EXP CHAR	ReadMsgRT(UCHAR Bus,UCHAR MsgNo,UCHAR RtNo,RT_MSG *message,UCHAR);//For BcRtMt RtNo refers to RTSTACK_A_B 
EXP CHAR	IllegalizeCmd(UCHAR Bus,CMD_WRD *temp,UCHAR CardId); 
EXP CHAR	LegalizeCmd(UCHAR Bus,CMD_WRD *temp,UCHAR CardId);

////////////////////////////////  Enable/Disable Channels  ///////////////////////////////

EXP BOOL    EnableRtChannel(UCHAR Bus,UCHAR Channel,UCHAR CardId);
EXP BOOL    DisableRtChannel(UCHAR Bus,UCHAR Channel,UCHAR CardId);

/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
///////////////////////////  31 REMOTE TERMINALS	/////////////////

EXP CHAR	ConfigureRT(UCHAR Bus,UCHAR A_B,UCHAR Address[],UCHAR);
EXP CHAR	RunRt(UCHAR Bus,UCHAR RtStack_A_B,UCHAR Address[],UCHAR);
EXP	CHAR    StartRt(UCHAR Bus,UCHAR RtStack_A_B,UCHAR RtNo,UCHAR CardId);
EXP	CHAR	ResetRt(UCHAR Bus,UCHAR RtNo,UCHAR CardId);
EXP	CHAR	ConfigureRTCB(UCHAR Bus,USHORT RtAddr,USHORT BlockSize,UCHAR);
EXP	CHAR	WriteDataBlockCB(UCHAR Bus,USHORT Module,USHORT datablock,USHORT data_buff[],USHORT BlockSize,UCHAR);
EXP	CHAR	ReadDataBlockCB(UCHAR Bus,USHORT Module,USHORT data_block,USHORT *data_buff,USHORT BlockSize,UCHAR);
EXP CHAR	LegalizeAll(UCHAR Bus,UCHAR RtNo,UCHAR CardId);
EXP CHAR	IllegalizeAll(UCHAR Bus,UCHAR RtNo,UCHAR CardId);
EXP CHAR	SetSrvReq(UCHAR Bus,UCHAR RtNo,UCHAR req,UCHAR CardId);
EXP CHAR	SetTrFlag(UCHAR Bus,UCHAR RtNo,UCHAR Tr,UCHAR CardId);
EXP CHAR	SetSubSys(UCHAR Bus,UCHAR RtNo,UCHAR flag,UCHAR CardId);
EXP CHAR	SetBusy(UCHAR Bus,UCHAR RtNo,UCHAR flag,UCHAR CardId);
EXP CHAR	SetDBA(UCHAR Bus,UCHAR RtNo,UCHAR flag,UCHAR CardId);
EXP CHAR	ClrTTROnSync(UCHAR Bus,UCHAR RtNo,UCHAR TTRFlag,UCHAR CardId);
EXP CHAR	LoadTTROnSync(UCHAR Bus,UCHAR RtNo,UCHAR flag,UCHAR CardId);
EXP SHORT	DefBuffSize(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,UCHAR t_r,USHORT size,UCHAR CardId);
EXP SHORT	DefDataBlkCB(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,UCHAR t_r,USHORT block_num,UCHAR CardId);
EXP CHAR	WriteDataBufferCB(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,USHORT data_buff[],USHORT Buff_Size,UCHAR CardId);
EXP CHAR	ReadDataBufferCB(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,USHORT *data_buff,USHORT Buff_Size,UCHAR CardId);

////////////////////////////////  Enable/Disable Channels  ///////////////////////////////

EXP BOOL    EnableRtChannel(UCHAR Bus,UCHAR RtNo,UCHAR Channel,UCHAR CardId);
EXP BOOL    DisableRtChannel(UCHAR Bus,UCHAR RtNo,UCHAR Channel,UCHAR CardId);

/////////////////////////////////////////////////////////////////////
///////////////////////////  MONITOR TERMINAL	/////////////////////

EXP CHAR	SelMsgMt(UCHAR Bus,UCHAR MTSTACK_A_B,UCHAR);
EXP CHAR	RunMt(UCHAR Bus,UCHAR);
EXP CHAR	ReadMonPacket(UCHAR Bus,UCHAR MTSTACK_A_B,USHORT DespNo,MNTPACK *MontDataBuff,UCHAR);
EXP CHAR	SelWordMt(UCHAR Bus,UCHAR);
EXP CHAR	DeSelAll(UCHAR Bus,UCHAR);
EXP CHAR	SelAll(UCHAR Bus,UCHAR);
EXP CHAR	SelCmd(UCHAR Bus,CMD_WRD *temp,UCHAR);
EXP CHAR	DeSelCmd(UCHAR Bus,CMD_WRD *temp,UCHAR);
EXP CHAR	ReadMtWord(UCHAR Bus,MT_WORD_DEF *result,UCHAR);
EXP CHAR	ResetMt(UCHAR,UCHAR);
EXP CHAR	MtTimeTagReset(UCHAR Bus,UCHAR);
EXP CHAR	MtTTRResolution(UCHAR Bus,USHORT TimeRes,UCHAR);

/////////////////////////////////////////////////////////////////////
///////////////////////////  INTERRUPT	/////////////////////////////

EXP CHAR	EventCreate(UCHAR);
EXP CHAR	EventClose(UCHAR CardId);
EXP CHAR	WaitForInterrupt(UCHAR);
EXP LONG	GetIntrStatus(UCHAR Bus,UCHAR);
EXP LONG	GetTimerIntrStatus(UCHAR CardId);

EXP CHAR	InitInterrupts(UCHAR,UCHAR val,UCHAR);
EXP CHAR	InitTimerIntr(UCHAR,UCHAR);
EXP CHAR	InitExternalInterrupts(UCHAR,UCHAR val,UCHAR);
EXP CHAR	StopTimerIntr(UCHAR Value,UCHAR CardId);
EXP CHAR	SelectTMR(UCHAR,UCHAR,UCHAR,UCHAR);
EXP CHAR	DisableInterrupts(UCHAR,UCHAR);
EXP CHAR	DisableExternalInterrupts(UCHAR,UCHAR);

EXP LONG	GetBCEOMCnt(UCHAR,UCHAR);
EXP LONG	GetRTEOMCnt(UCHAR,USHORT RtNo,UCHAR);
EXP LONG	GetMTEOMCnt(UCHAR,UCHAR);
EXP LONG	GetRTCIRCCnt(UCHAR,USHORT RtNo,UCHAR);
EXP LONG	GetRTDBCACnt(UCHAR,UCHAR);
EXP LONG	GetTMR1Cnt(UCHAR);
EXP LONG	GetTMR2Cnt(UCHAR);
EXP LONG	GetTMR3Cnt(UCHAR);
EXP LONG	GetExternalCnt(UCHAR,UCHAR,volatile ULONG *DataBuffer);
EXP LONG	GetSecondExternalCnt(UCHAR,UCHAR);


EXP CHAR	SetBCEOMCnt(UCHAR,ULONG value,UCHAR);
EXP CHAR	SetRTEOMCnt(UCHAR,USHORT RtNo,ULONG value,UCHAR);
EXP CHAR	SetMTEOMCnt(UCHAR,ULONG value,UCHAR);
EXP CHAR	SetRTDBCACnt(UCHAR,ULONG value,UCHAR);
EXP CHAR	SetRTCIRCCnt(UCHAR,USHORT RtNo,ULONG value,UCHAR);
EXP CHAR	SetTMR1Cnt(ULONG,UCHAR);
EXP CHAR	SetTMR2Cnt(ULONG,UCHAR);
EXP CHAR	SetTMR3Cnt(ULONG,UCHAR);
EXP CHAR	SetExternalCnt(UCHAR,ULONG value,UCHAR);
EXP CHAR	SetSecondExternalCnt(UCHAR,ULONG value,UCHAR);


EXP SHORT	GetMinorFrameCnt(UCHAR Bus,UCHAR CardId);

EXP CHAR	InitEnhancedInterrupts(UCHAR Bus,UCHAR Value,UCHAR CardId);
EXP CHAR	DisableEnhancedInterrupts(UCHAR Bus,UCHAR Value,UCHAR CardId);
EXP LONG	GetEndOfFrameCnt(UCHAR Bus,UCHAR CardId);
EXP CHAR	SetEndOfFrameCnt(UCHAR Bus,ULONG value,UCHAR CardId);
EXP LONG	GetMT_TTR_Roll_OverCnt(UCHAR Bus,UCHAR CardId);
EXP CHAR	SetMT_TTR_Roll_OverCnt(UCHAR Bus,ULONG value,UCHAR CardId);
EXP LONG	GetMT_CmdStack_Roll_OverCnt(UCHAR Bus,UCHAR CardId);
EXP CHAR	SetMT_CmdStack_Roll_OverCnt(UCHAR Bus,ULONG value,UCHAR CardId);
EXP LONG	GetMT_DataStack_Roll_OverCnt(UCHAR Bus,UCHAR CardId);
EXP CHAR	SetMT_DataStack_Roll_OverCnt(UCHAR Bus,ULONG value,UCHAR CardId);


////////////////////////////////////////////////////////////////////
////////////////		APOORVA PCMCIA		////////////////////////

//////////////////		General		///////////////////////////////

EXP BOOL	InitDriver();
EXP BOOL	OnMemMap(); 
EXP BOOL	InitCard();
EXP BOOL	GetCardPresence();
EXP SHORT	SelfTest();
EXP PCHAR	GetMfgId_VerId();
EXP BOOL	SoftwareReset();
EXP BOOL	SetIndex(ULONG val);

EXP USHORT	MemRead(ULONG len);
EXP USHORT	MemWrite(ULONG len, USHORT val);
EXP UCHAR	AtrMemRead(ULONG len);
EXP UCHAR	AtrMemWrite(ULONG len, UCHAR val);
EXP USHORT	RegRead(ULONG len);
EXP USHORT	RegWrite(ULONG len,USHORT val);
EXP CHAR	ClearRam(USHORT Module);

EXP CHAR	WritePort(ULONG offset,UCHAR val); 
EXP UCHAR	ReadPort(ULONG offset);
EXP BOOL	CloseDeviceHandle();

////////////////////////////////////////////////////////////////////
/////////////////////		BUS CONTROLLER		////////////////////

EXP CHAR	ConfigureBC(USHORT stack_size);
EXP CHAR	IntMsgGapLeast(UCHAR flag);
EXP SHORT	DefMsg_StackA(USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words);
EXP SHORT	DefMsg_StackB(USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words);
EXP SHORT	DefMsg(USHORT BCSTACK_A_B,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words);
EXP SHORT	DefFrame(USHORT BCSTACK_A_B,USHORT number_of_messages,USHORT mess_sequence[173]);
EXP SHORT	DefFrameStackA(USHORT number_of_messages,USHORT mess_sequence[]);
EXP SHORT	DefMajorFrame(USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[128]);
EXP SHORT	DefMajorFrameStackA(USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[128]);
EXP SHORT	RunBc(USHORT stackA_B,USHORT position,USHORT num_of_messages,ULONG times);
EXP SHORT	RunBcMajorFrame(USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[128],ULONG times);
EXP SHORT	ReadMsgBC(USHORT stackA_B,USHORT frame_item,MSG_RESULT *result);
EXP CHAR	ResetBc();
EXP CHAR	BCAutoRepeat(UCHAR sel);
EXP CHAR	BCRetry(USHORT sel);
EXP CHAR	InterMsgGap(UCHAR MsgNo,USHORT Inter_Msg_Gap);
EXP CHAR	ErrorInjection(ERR_INJ Err);
EXP CHAR	ErrorInjectReset();
EXP CHAR	MsgEnableDisable(BOOL MsgNos[]);
EXP CHAR	BcTimeTagReset();
EXP CHAR	BcTTRResolution(USHORT TimeRes);

/////////////////////////////////////////////////////////////////////
///////////////////////////  REMOTE TERMINAL	/////////////////////

EXP CHAR    ClearCircularBufferRam(USHORT Module);
EXP CHAR	ConfigureRT(USHORT Module);
EXP CHAR	RunRt(UCHAR RTSTACK_A_B);
EXP CHAR	ResetRt();
EXP CHAR	WriteDataBlockCB(UCHAR subaddr,USHORT data_buff[],USHORT datablock);
EXP CHAR	ReadDataBlockCB(UCHAR subaddr,USHORT *data_buff,USHORT data_block);
EXP CHAR	WriteDataBufferCB(UCHAR subaddr,USHORT data_buff[],USHORT Buff_Size);
EXP CHAR	ReadDataBufferCB(UCHAR subaddr,USHORT *data_buff,USHORT Buff_Size);
EXP CHAR	LegalizeAll();
EXP CHAR	IllegalizeAll();
EXP CHAR	SetSrvReq(UCHAR req);
EXP CHAR	SetTrFlag(UCHAR Tr);
EXP CHAR	SetSubSys(UCHAR flag);
EXP CHAR	SetBusy(UCHAR flag);
EXP CHAR	SetDBA(UCHAR flag);
EXP CHAR	ClrTTROnSync(UCHAR TTRFlag);
EXP CHAR	LoadTTROnSync(UCHAR flag);
EXP CHAR	RtTimeTagReset();
EXP CHAR	RtTTRResolution(USHORT TimeRes);
EXP SHORT	DefBuffSize(UCHAR subaddr,UCHAR t_r,USHORT size);
EXP SHORT	DefDataBlkCB(UCHAR subaddr,UCHAR t_r,USHORT block_num);
EXP CHAR	HW_LoopBack(UCHAR Saddr[]);

////////////////////////////////  Enable/Disable Channels  ///////////////////////////////

EXP BOOL    EnableRtChannel(UCHAR Channel);
EXP BOOL    DisableRtChannel(UCHAR Channel);

/////////////////////////////////////////////////////////////////////

EXP CHAR	WriteDataBlock(UCHAR RTSTACK_A_B,UCHAR subaddr,USHORT data_buff[]);// BCRTMT
//EXP CHAR	WriteDataBlock(UCHAR RtNo,UCHAR SubAddr,USHORT data_buff[]);	// 31 RTs
//EXP CHAR	ReadMsgRT(UCHAR MsgNo,UCHAR RTSTACK_A_B,RT_MSG *message); // BCRTMT
EXP CHAR	ReadMsgRT(UCHAR MsgNo,UCHAR RtNo,RT_MSG *message);  // 31 RTs
EXP CHAR	IllegalizeCmd(CMD_WRD *temp); 
EXP CHAR	LegalizeCmd(CMD_WRD *temp);

/////////////////////////////////////////////////////////////////////
///////////////////////////  31 REMOTE TERMINALS	/////////////////

EXP CHAR	ConfigureRT(UCHAR A_B,UCHAR Address[]);
EXP CHAR	RunRt(UCHAR RtStack_A_B,UCHAR Address[]);
EXP CHAR	ResetRt(UCHAR Address);
EXP SHORT	DefBuffSize(UCHAR RtNo,UCHAR subaddr,UCHAR t_r,USHORT size);
EXP SHORT	DefDataBlkCB(UCHAR RtNo,UCHAR subaddr,UCHAR t_r,USHORT block_num);
EXP CHAR	ReadDataBufferCB(UCHAR RtNo,UCHAR subaddr,USHORT *data_buff,USHORT Buff_Size);
EXP CHAR	WriteDataBufferCB(UCHAR RtNo,UCHAR subaddr,USHORT data_buff[],USHORT Buff_Size);
EXP	CHAR	ConfigureRTCB(USHORT Module,USHORT BlockSize);
EXP CHAR	WriteDataBlockCB(USHORT RtAddr,USHORT datablock,USHORT data_buff[],USHORT BlockSize);
EXP CHAR	ReadDataBlockCB(USHORT RtAddr,USHORT datablock,USHORT *data_buff,USHORT BlockSize);
EXP CHAR	LegalizeAll(UCHAR RtNo);
EXP CHAR	IllegalizeAll(UCHAR RtNo);
EXP CHAR	SetSrvReq(UCHAR RtNo,UCHAR req);
EXP CHAR	SetTrFlag(UCHAR RtNo,UCHAR Tr);
EXP CHAR	SetSubSys(UCHAR RtNo,UCHAR flag);
EXP CHAR	SetBusy(UCHAR RtNo,UCHAR flag);
EXP CHAR	SetDBA(UCHAR RtNo,UCHAR flag);
EXP CHAR	ClrTTROnSync(UCHAR RtNo,UCHAR TTRFlag);
EXP CHAR	LoadTTROnSync(UCHAR RtNo,UCHAR flag);

////////////////////////////////  Enable/Disable Channels  ///////////////////////////////

EXP BOOL    EnableRtChannel(UCHAR RtNo,UCHAR Channel);
EXP BOOL    DisableRtChannel(UCHAR RtNo,UCHAR Channel);

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
///////////////////////////  MONITOR TERMINAL	/////////////////////

EXP CHAR	SelMsgMt(UCHAR MTSTACK_A_B);
EXP CHAR	RunMt();
EXP CHAR	ReadMonPacket(UCHAR MTSTACK_A_B,USHORT DespNo,MNTPACK *MontDataBuff);
EXP CHAR	SelWordMt();
EXP CHAR	DeSelAll();
EXP CHAR	SelAll();
EXP CHAR	SelCmd(CMD_WRD *temp);
EXP CHAR	DeSelCmd(CMD_WRD *temp);
EXP CHAR	ReadMtWord(MT_WORD_DEF *result);
EXP CHAR	ResetMt();
EXP CHAR	MtTimeTagReset();
EXP CHAR	MtTTRResolution(USHORT TimeRes);

/////////////////////////////////////////////////////////////////////
///////////////////////////  INTERRUPT	/////////////////////////////

EXP CHAR	EventCreate();
EXP CHAR	EventClose();
EXP CHAR	WaitForInterrupt();
EXP LONG	GetIntrStatus();
EXP LONG	GetTimerIntrStatus();


EXP CHAR	InitInterrupts(UCHAR val);
EXP CHAR	InitTimerIntr(USHORT);
EXP CHAR	StopTimerIntr(USHORT Value);
EXP CHAR	SelectTMR(UCHAR,UCHAR,UCHAR);
EXP CHAR	DisableInterrupts();

EXP SHORT	GetMinorFrameCnt();
EXP LONG	GetMTEOMCnt();
EXP CHAR	SetMTEOMCnt(ULONG value);
EXP LONG	GetBCEOMCnt();
EXP CHAR	SetBCEOMCnt(ULONG value);
EXP LONG	GetRTEOMCnt(USHORT RtNo);
EXP CHAR	SetRTEOMCnt(USHORT RtNo,ULONG value);
EXP LONG	GetRTCIRCCnt(USHORT RtNo);
EXP CHAR	SetRTCIRCCnt(USHORT RtNo,ULONG value);
EXP LONG	GetRTDBCACnt();
EXP CHAR	SetRTDBCACnt(ULONG value);

EXP LONG	GetTMR1Cnt();
EXP LONG	GetTMR2Cnt();
EXP LONG	GetTMR3Cnt();
EXP CHAR	SetTMR1Cnt(ULONG value);
EXP CHAR	SetTMR2Cnt(ULONG value);
EXP CHAR	SetTMR3Cnt(ULONG value);



EXP CHAR	InitEnhancedInterrupts(UCHAR Value);
EXP CHAR	DisableEnhancedInterrupts(UCHAR Value);
EXP LONG	GetEndOfFrameCnt();
EXP CHAR	SetEndOfFrameCnt(ULONG value);	
EXP CHAR	SetMT_TTR_Roll_OverCnt(ULONG value);			
EXP LONG	GetMT_TTR_Roll_OverCnt();
EXP LONG	GetMT_CmdStack_Roll_OverCnt();
EXP CHAR	SetMT_CmdStack_Roll_OverCnt(ULONG value);
EXP LONG	GetMT_DataStack_Roll_OverCnt();
EXP CHAR	SetMT_DataStack_Roll_OverCnt(ULONG value);

/*
/////////////////////////////Serial Controller/////////////////////////////////////////

EXP UCHAR	ReadSerial(UCHAR,UCHAR len);
EXP BOOL	WriteSerial(UCHAR,UCHAR len,UCHAR val);

EXP UCHAR	ReadChar(UCHAR,UCHAR len);
EXP BOOL	WriteChar(UCHAR,UCHAR len,UCHAR val);

EXP BOOL	InitRS422(UCHAR,UINT);
EXP UCHAR	CheckLSR_Rx(UCHAR);
EXP UCHAR	CheckLSR_Tx(UCHAR);
EXP UCHAR	Write_Fifo(UCHAR,UCHAR*,UCHAR);
EXP UCHAR	Read_Fifo(UCHAR,UCHAR*,UCHAR);


EXP LONG	GetUART_CHANNEL_INTRCnt(UCHAR Channel);
EXP CHAR	SetUART_CHANNEL_INTRCnt(UCHAR Channel,ULONG value);
EXP CHAR	InitUARTInterrupts(UCHAR Channel);
EXP CHAR	DisableUARTInterrupts(UCHAR Channel);
*/


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


///////////////////////////////END/////////////////////////////////////////

