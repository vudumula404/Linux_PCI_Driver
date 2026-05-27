#include "Constants.h"
#include "Bc_Mess.h"
#include "Rt_Mess.h"
#include "Mt_Mess.h"


#define BOOL bool
#define USHORT unsigned short
#define CHAR  char
#define UCHAR  unsigned char
#define ULONG  unsigned long
#define LONG  long
#define PCHAR char*
#define SHORT  unsigned short


int InitCard(UCHAR CardId);
CHAR GetCardPresence(UCHAR);
SHORT SelfTest(UCHAR CardId);
CHAR SoftwareReset(UCHAR);
PCHAR PrintErrorMsg(int);
PCHAR GetMfgId_VerId(UCHAR);
void DefErrors();
CHAR ConfigICSR(UCHAR CardId);
int sel_subaddr(int node,int wrd);
int desel_subaddr(int node,int wrd);


 USHORT MemRead(UCHAR Bus,ULONG len,UCHAR);
 USHORT MemWrite(UCHAR Bus,ULONG len, USHORT val,UCHAR);
 USHORT RegRead(UCHAR Bus,ULONG len,UCHAR);
 USHORT RegWrite(UCHAR Bus,ULONG len,USHORT val,UCHAR);
 CHAR ClearRam(UCHAR Bus,USHORT Module,UCHAR);
 CHAR ConfigureBC(UCHAR Bus,USHORT stack_size,UCHAR);
 CHAR IntMsgGapLeast(UCHAR Bus,UCHAR flag,UCHAR);
SHORT DefMsg_StackA(UCHAR Bus,USHORT msg_num,MSG_DEF def,USHORT data_buffer[]
			  ,USHORT num_of_data_words,UCHAR CardId);
SHORT DefMsg_StackB(UCHAR Bus,USHORT msg_num,MSG_DEF def,USHORT data_buffer[]
			  ,USHORT num_of_data_words,UCHAR CardId);

SHORT DefMsg(UCHAR Bus,USHORT BCSTACK_A_B,USHORT msg_num,MSG_DEF def,
		USHORT data_buffer[],USHORT num_of_data_words,UCHAR CardId);
 
SHORT DefFrame(UCHAR Bus,USHORT BCSTACK_A_B,USHORT number_of_messages,
 			 USHORT mess_sequence[173],UCHAR CardId);

SHORT DefMajorFrame(UCHAR Bus,USHORT BCSTACK_A_B,USHORT no_of_minor_frames
			,MAJOR_FRAME frame_sequence[128],UCHAR CardId);

SHORT RunBc(UCHAR Bus,USHORT stackA_B,USHORT position,USHORT num_of_messages
						,ULONG times,UCHAR);

SHORT RunBcMajorFrame(UCHAR Bus,USHORT stackA_B,USHORT no_of_minor_frames
		,MAJOR_FRAME frame_sequence[128],ULONG times,UCHAR CardId);

SHORT ReadMsgBC(UCHAR Bus,USHORT stackA_B,USHORT frame_item,
				MSG_RESULT *result,UCHAR);
CHAR ResetBc(UCHAR Bus,UCHAR CardId);
 BSW_BITS ReadBswBits(USHORT blk_statwrd);
 STATUS_BITS ReadStatusBits(USHORT status);
 MSG_DEF ReturnCntrlFields(USHORT control);
 CMD ReturnCmdFields(USHORT command);
 CHAR BCAutoRepeat(UCHAR Bus,UCHAR sel,UCHAR);
 CHAR BCRetry(UCHAR Bus,USHORT sel,UCHAR);

CHAR ConfigureRT(UCHAR Bus,UCHAR A_B,UCHAR Address[],UCHAR);
CHAR RunRt(UCHAR Bus,UCHAR RtStack_A_B,UCHAR Address[],UCHAR);
CHAR WriteDataBlock(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,USHORT data_buff[]
							,UCHAR);
CHAR ReadMsgRT(UCHAR Bus,UCHAR MsgNo,UCHAR RtNo,RT_MSG *message,UCHAR);
CHAR ConfigureRTCB(UCHAR Bus,USHORT Module,USHORT BlockSize,UCHAR);
CHAR WriteDataBlockCB(UCHAR Bus,USHORT Module,USHORT datablock,
				USHORT data_buff[],USHORT BlockSize,UCHAR);
CHAR ReadDataBlockCB(UCHAR Bus,USHORT Module,USHORT data_block,
				USHORT *data_buff,USHORT BlockSize,UCHAR);
CHAR LegalizeAll(UCHAR,UCHAR,UCHAR);
CHAR IllegalizeAll(UCHAR,UCHAR,UCHAR);
CHAR IllegalizeCmd(UCHAR,CMD_WRD *temp,UCHAR); 
CHAR LegalizeCmd(UCHAR,CMD_WRD *temp,UCHAR);

CHAR SetSrvReq(UCHAR,UCHAR RtNo,UCHAR req,UCHAR CardId);
CHAR SetTrFlag(UCHAR,UCHAR RtNo,UCHAR Tr,UCHAR CardId);
CHAR SetSubSys(UCHAR,UCHAR RtNo,UCHAR flag,UCHAR CardId);
CHAR SetBusy(UCHAR,UCHAR RtNo,UCHAR flag,UCHAR CardId);
CHAR SetDBA(UCHAR,UCHAR RtNo,UCHAR flag,UCHAR CardId);

CHAR ClrTTROnSync(UCHAR Bus,UCHAR RtNo,UCHAR TTRFlag,UCHAR CardId);
CHAR LoadTTROnSync(UCHAR,UCHAR RtNo,UCHAR flag,UCHAR CardId);
CHAR SelMsgMt(UCHAR Bus,UCHAR MTSTACK_A_B,UCHAR);
CHAR RunMt(UCHAR Bus,UCHAR);
CHAR ReadMonPacket(UCHAR Bus,UCHAR MTSTACK_A_B,USHORT DespNo,
					MNTPACK *MontDataBuff,UCHAR);
CHAR SelWordMt(UCHAR Bus,UCHAR);
CHAR DeSelAll(UCHAR Bus,UCHAR);
CHAR SelAll(UCHAR Bus,UCHAR);
CHAR SelCmd(UCHAR Bus,CMD_WRD *temp,UCHAR);
CHAR DeSelCmd(UCHAR Bus,CMD_WRD *temp,UCHAR);
CHAR ReadMtWord(UCHAR Bus,MT_WORD_DEF *result,UCHAR);
 
CHAR InitInterrupts(UCHAR,USHORT val,UCHAR);
CHAR InitTimerIntr(USHORT,UCHAR);
CHAR StopTimerIntr(USHORT Value,UCHAR CardId);
CHAR SelectTMR(UCHAR,UCHAR,UCHAR,UCHAR);
CHAR DisableInterrupts(UCHAR,UCHAR);
LONG GetBCEOMCnt(UCHAR,UCHAR);
LONG GetRTEOMCnt(UCHAR,UCHAR);
LONG GetMTEOMCnt(UCHAR,UCHAR);
LONG GetRTCIRCCnt(UCHAR,UCHAR);
LONG GetRTDBCACnt(UCHAR,UCHAR);
LONG GetTMR1Cnt(UCHAR);
LONG GetTMR2Cnt(UCHAR);
LONG GetTMR3Cnt(UCHAR);

CHAR SetBCEOMCnt(UCHAR,ULONG value,UCHAR);
CHAR SetRTEOMCnt(UCHAR,ULONG value,UCHAR);
CHAR SetMTEOMCnt(UCHAR,ULONG value,UCHAR);
CHAR SetRTCIRCCnt(UCHAR,ULONG value,UCHAR);
CHAR SetRTDBCACnt(UCHAR,ULONG value,UCHAR);
CHAR SetTMR1Cnt(ULONG,UCHAR);
CHAR SetTMR2Cnt(ULONG,UCHAR);
CHAR SetTMR3Cnt(ULONG,UCHAR);

CHAR ResetMt(UCHAR,UCHAR);
CHAR WritePort(ULONG offset,USHORT val,UCHAR CardId);
UCHAR ReadPort(UCHAR offset,UCHAR CardId);
CHAR BcTimeTagReset(UCHAR Bus,UCHAR);
CHAR RtTimeTagReset(UCHAR Bus,UCHAR);
CHAR MtTimeTagReset(UCHAR Bus,UCHAR);
CHAR BcTTRResolution(UCHAR Bus,USHORT TimeRes,UCHAR);
CHAR RtTTRResolution(UCHAR Bus,USHORT TimeRes,UCHAR);
CHAR MtTTRResolution(UCHAR Bus,USHORT TimeRes,UCHAR);

CHAR ErrorInjection(UCHAR Bus,ERR_INJ Err,UCHAR CardId);
CHAR ErrorInjectReset(UCHAR Bus,UCHAR CardId);

SHORT GetMinorFrameCnt(UCHAR Bus,UCHAR CardId);
CHAR MsgEnableDisable(UCHAR Bus,UCHAR MsgNos[],UCHAR CardId);
CHAR InterMsgGap(UCHAR Bus,UCHAR MsgNo,USHORT Inter_Msg_Gap,UCHAR CardId);
CHAR InitEnhancedInterrupts(UCHAR Bus,UCHAR Value,UCHAR CardId);
CHAR DisableEnhancedInterrupts(UCHAR Bus,UCHAR Value,UCHAR CardId);
LONG GetEndOfFrameCnt(UCHAR Bus,UCHAR CardId);
LONG GetMT_TTR_Roll_OverCnt(UCHAR Bus,UCHAR CardId);
CHAR SetEndOfFrameCnt(UCHAR Bus,ULONG value,UCHAR CardId);
CHAR SetMT_TTR_Roll_OverCnt(UCHAR Bus,ULONG value,UCHAR CardId);
CHAR CloseDeviceHandle(UCHAR CardId);

/*CHAR SetSOMCnt(UCHAR Bus, ULONG value, UCHAR CardId);
LONG GetSOMCnt(UCHAR Bus, UCHAR CardId);

CHAR EnableInterruptsBC(UCHAR Bus,USHORT Value,UCHAR CardId);
CHAR EnableInterruptsRT(UCHAR Bus,USHORT Value,UCHAR CardId);
CHAR EnableInterruptsMT(UCHAR Bus,USHORT Value,UCHAR CardId);

CHAR SetBCSOMCnt(UCHAR Bus, ULONG value, UCHAR CardId);
CHAR SetMTSOMCnt(UCHAR Bus, ULONG value, UCHAR CardId);
CHAR SetRTSOMCnt(UCHAR Bus, ULONG value, UCHAR CardId);

LONG GetBCSOMCnt(UCHAR Bus,  UCHAR CardId);
LONG GetMTSOMCnt(UCHAR Bus,  UCHAR CardId);
LONG GetRTSOMCnt(UCHAR Bus,  UCHAR CardId);*/


LONG GetBCSOMCnt(UCHAR Bus, UCHAR CardId);
LONG GetBCEOMCnt(UCHAR Bus, UCHAR CardId);
LONG GetBCEOFCnt(UCHAR Bus, UCHAR CardId);
LONG GetBCEOMINFRAMECnt(UCHAR Bus, UCHAR CardId);
LONG GetMTSOMCnt(UCHAR Bus, UCHAR CardId);
LONG GetMTEOMCnt(UCHAR Bus, UCHAR CardId);
LONG GetMTEOMCnt_dummy(UCHAR Bus, UCHAR CardId);
LONG GetRTEOMCnt(UCHAR Bus, UCHAR CardId);
LONG GetBCEOFCnt_D(UCHAR Bus, UCHAR CardId);

CHAR SetBCSOMCnt(UCHAR Bus, ULONG value, UCHAR CardId);
CHAR SetBCEOMCnt(UCHAR Bus, ULONG value, UCHAR CardId);
CHAR SetBCEOFCnt(UCHAR Bus, ULONG value, UCHAR CardId);
CHAR SetBCEOMMINORFRAMECnt(UCHAR Bus, ULONG value, UCHAR CardId);
CHAR SetMTSOMCnt(UCHAR Bus, ULONG value, UCHAR CardId);
CHAR SetMTEOMCnt(UCHAR Bus, ULONG value, UCHAR CardId);
CHAR SetRTSOMCnt(UCHAR Bus, ULONG value, UCHAR CardId);
CHAR SetRTEOMCnt(UCHAR Bus, ULONG value, UCHAR CardId);

void disableNonCanonicalMode(void);
USHORT RegRead(UCHAR Bus, ULONG len, UCHAR CardId);
USHORT RegWrite(UCHAR Bus,ULONG len,USHORT val,UCHAR CardId);
USHORT MemRead(UCHAR Bus, ULONG len, UCHAR CardId);
USHORT MemWrite(UCHAR Bus,ULONG len, USHORT val,UCHAR CardId);
CHAR InitInterrupts(UCHAR Bus,USHORT Value,UCHAR CardId);
void ConfigureRT(UCHAR rtModId, unsigned char rtAddr);
void Async(int BcMod, int mtModId, int channel,int NoOfTimes, int msg_no, int prior);
