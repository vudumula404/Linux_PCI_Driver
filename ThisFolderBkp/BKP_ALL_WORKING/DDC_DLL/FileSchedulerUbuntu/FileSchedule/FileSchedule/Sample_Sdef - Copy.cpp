// Sample.cpp : Defines the entry point for the console application.
//

// FileLoadRt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <winioctl.h>
#include <conio.h>
#include <time.h>
#include "C:\FileSchedulerProject\FileSchedule\FileSchedule\Include\Libraries.h"
//#include "C:\FileLoadRt\FileLoadRt\Include\Libraries.h"
//#include "C:\Users\mrajesh\Desktop\Apoorva_ATP\Include\Libraries.h"

MSG_DEF bc_page;
typedef struct _MSGPREP
{
	unsigned short MessageNumber[255];
	unsigned short RxRtAddres[32];
	unsigned short RxRtSaddr[32];
	unsigned short TxRtAddres[32];
	unsigned short TxRtSaddr[32];
	unsigned short Wcnt[32];
	char MsgTyp[32];
	char Channel[32];
	unsigned short TrBit[32];
	unsigned short Data[255][32];
}MSGPREP;

FILE *fp,*fpxl;
unsigned long 	TPtr,FilePtr,Fseek,m_ulTimeTag;
int i,j,loop=2;
unsigned short DataPack[32],tmpWRD[32],Tr,Bsw,RecCmdWrd,RecStatus,RecRtAddr,RecSubAddr,TxCmdWrd,TxStatus,TxRtAddr,TxSubAddr,Wcnt;
unsigned char m_WordMonit[4][32][32][32];
unsigned short m_ulmsgno,Fmsgno;
char Bus,MsgType; 
MSGPREP msgprep;

void fnExtractMessages(char *);
unsigned char fnGetMinorFrame(unsigned char ubyMessageNumber);
void fnDefineSyncMsg();
unsigned char fnGetFrequency(unsigned char ubyMessageNumber);
unsigned char fnGetMinorFrame(unsigned char ubyMessageNumber);
void fnDefineSyncMsg();
unsigned char fnGetFrequency(unsigned char ubyMessageNumber);
unsigned char fnGetSourceAddress(unsigned char ubyMessageNumber);
unsigned char fnGetDestAddress(unsigned char ubyMessageNumber);
unsigned char fnGetSourceSubAddress(unsigned char ubyMessageNumber);
unsigned char fnGetDestSubAddress(unsigned char ubyMessageNumber);
unsigned char fnGetSourceSubAddress(unsigned char ubyMessageNumber);
unsigned char fnGetDestSubAddress(unsigned char ubyMessageNumber);
unsigned char fnGetChannelSel(unsigned char ubyMessageNumber);
unsigned char fnGetWordCount(unsigned char ubyMessageNumber);
void fnGetMessageName(unsigned char ubyMessageNumber, char * szMessageName);
void fnUpdateTimeBroadcastInfo();
unsigned int fnHandle1553BCommunication();
void OnButtonLoadframe(); 
void fnAllocateMemoryForReceiveDataWords(char *);
void fnExtractReceiveMessageInfo(char *);
void fnInitialiseMessageWords();
void fnAllocateMemoryForTransmitDataWords(char *);
void fnExtractTransmitMessageInfo(char * );
void ActivateMc(void);
void fnCheckFor24IterationsInLane1(bool fRTResponseAvailable);
unsigned short IntOpenBC1(USHORT ModuleId ,USHORT CardId);
unsigned short IntOpenBC2(USHORT ModuleId ,USHORT CardId);
unsigned short IntOpenBC3(USHORT ModuleId ,USHORT CardId);
void fnGetLogData(unsigned char ubyMinorFrameNumber);
void fnCheckFor24IterationsInLane1(bool fRTResponseAvailable);

HANDLE hthread1,hthread3,hthread2,hthread4;
char m_rgszTransmitMessagesDatabase[100][100];
char m_rgszReceiveMessagesDatabase [100][100];
char m_rgszMessagesDatabase[200][100];
unsigned char m_ubyMessageNumber;
unsigned short int m_urgswTimeBroadcastData[3];
MAJOR_FRAME m_MinorFrame[10];
unsigned short int urgswTimeBroadcastData[2];
int m_wBus1Bus2;
unsigned char m_ubyfInitialiseMessages;
unsigned short int * m_rgpwMessagePointer[100];
bool	m_fRTRespondingToLane1;
unsigned char m_ubyB1RTResponseRetryCounter;
bool m_fB1RTResponseAvailable;

#define MAX_MINOR_FRAME_NUMBER          7
#define CIU_BROADCAST_ADDRESS 31
#define MINOR_CYCLE_ZERO		0
#define MINOR_CYCLE_ONE			1
#define MINOR_CYCLE_TWO			2
#define MINOR_CYCLE_THREE		3
#define MINOR_CYCLE_FOUR		4
#define MINOR_CYCLE_FIVE		5 
#define MINOR_CYCLE_SIX			6
#define MINOR_CYCLE_SEVEN		7
#define MAX_DEST_ADDRESS                31
#define MAX_DEST_SUBADDRESS             31
#define MAX_SOURCE_ADDRESS              31
#define MAX_SOURCE_SUBADDRESS           31
#define MAX_WORD_COUNT                  32
#define MINORFRAMETIME			20	//ms
#define TIMEBROADCASTCHNNLAMSGID 16
#define TIMEBROADCASTCHNNLBMSGID 17
#define ALLOCATE                 10
#define NOALLOCATE               30
#define REALLOCATE               110
#define BUS1                     0
#define MAX_LOG_FILE_SIZE		5000000
#define CIU_RT_ADDRESS	11
#define CIU_BROADCAST_ADDRESS 31


struct Msg
	{
		MSG_DEF	MsgDef;
		MSG_RESULT MsgReturn;
		USHORT databuffer[32];
		char szMessageName[20];
		unsigned char ubyRxMsgCount;
		unsigned char ubyTxMsgCount;
	}m_stMsg[50], m_stMsgB2[50];

void ActivateRT();
void InitCard_init(void);
void Init_MT();
int Schedule();

#define MODULE_ID_0		0
#define MODULE_ID_1		1
#define MODULE_ID_2		2

errno_t err1,err2,err3,Mterr1,Mterr2,Mterr3;
FILE *Fp1_1=NULL,*Fp2_1=NULL,*Fp3_1=NULL;
FILE *Fp1_B=NULL,*Fp2_B=NULL,*Fp3_B=NULL;
int Mc1Cardno,Mc2Cardno,Mc3Cardno;
unsigned char Mc1Modno, Mc2Modno,Mc3Modno;
bool Mc1LruInitialised = FALSE, Mc2LruInitialised = FALSE, Mc3LruInitialised = FALSE;
HANDLE hMutexObj;
void DeActivateMT(void);
void CloseMT1Module();
int	  MT1Initialised,MT2Initialised ,MT3Initialised;


int main()
{
	char szFilenameB1[15];
	m_urgswTimeBroadcastData[0] = 0;
	m_urgswTimeBroadcastData[1] = 0;

	InitCard_A(CARD1);
	/*SoftwareReset(CARD1);
	Sleep(500);*/
	//InitInterrupts_A(1, INTR_BCEOM|INTR_RTEOM|INTR_MTEOM,CARD1);
	InitInterrupts_A(1,INTR_BCEOM|INTR_TMR2,CARD1);
	InitInterrupts_A(1,INTR_TMR2,1);
	SelectTMR_A(0,MS_1,0,CARD1);
	SetTMR2Cnt_A(0,CARD1);
	SelectModule(0,CARD1);
	SetBCEOMCnt_A(1,0,CARD1);
	SetMTEOMCnt_A(1,0,CARD1);
	ActivateRT();
	Init_MT();
	ClearRam_A(1,BC,CARD1);
	ConfigureBC_A(1,128,CARD1);
	getchar();
	getchar();
	OnButtonLoadframe();
	//Schedule();
	fnHandle1553BCommunication();

	
	/*int ret = InitCard_A(CARD1);
	SoftwareReset(CARD1);
	Sleep(500);
	InitInterrupts_A(1,INTR_BCEOM|INTR_TMR2,CARD1);
	InitTimerIntr_A(INTR_TMR2,CARD1);
	SelectTMR_A(0,MS_1,0,CARD1);
	SetTMR2Cnt_A(0,CARD1);
	SelectModule(0,CARD1);
	//InitInterrupts_A(BcMod, INTR_BCEOM|INTR_TMR2,CARD1);
	SetBCEOMCnt_A(1,0,CARD1);
	strcpy(szFilenameB1,"INIT_B1.txt");
	fnExtractMessages(szFilenameB1);
	fnHandle1553BCommunication();
	return TRUE;*/
}
void fnExtractMessages(char * szFilename)
{
	char			szFileString[100]; 
	unsigned char	ubyLoopCount,ubyMinorFrameNumber;
	unsigned char	ubyTimeFrequency,ubyReceiveMessageCount,ubyTransmitMessageCount;
	unsigned char	ubySourceAddress, ubyDestAddress, ubySourceSubAddress, ubyDestSubAddress, ubyChannelSel;
	unsigned char	ubyWordCount;
	USHORT data_buffer[25];
	FILE			*fpstConfigFile;
	int k=0;
	
	ubyTransmitMessageCount=0;
	ubyReceiveMessageCount=0;	

	//fp = fopen("INIT_B1.txt","r");	
	fpstConfigFile = fopen(szFilename,"rt");
	if(fpstConfigFile == NULL)
	{
		printf("Unable to open Configuration File");
		exit(1);
	}

	for(ubyLoopCount=0;ubyLoopCount<3;ubyLoopCount++)
	{
		fgets(szFileString,256,fpstConfigFile);
	}

		//Define Sync Msgs for all Minorframes
	fnDefineSyncMsg();

	while (!feof(fpstConfigFile))
	{
		fgets(szFileString,256,fpstConfigFile);

		char szTempBuf[10];
		int iRet = sscanf(szFileString,"%s%s%s",szTempBuf,szTempBuf,szTempBuf);

		if(iRet == 3)
		{
			strcpy(m_rgszMessagesDatabase[m_ubyMessageNumber],szFileString);

			ubyMinorFrameNumber	= fnGetMinorFrame(m_ubyMessageNumber);
			ubyTimeFrequency	= fnGetFrequency(m_ubyMessageNumber);
			ubySourceAddress	= fnGetSourceAddress(m_ubyMessageNumber);
			ubyDestAddress		= fnGetDestAddress(m_ubyMessageNumber);
			ubySourceSubAddress = fnGetSourceSubAddress(m_ubyMessageNumber);
			ubyDestSubAddress	= fnGetDestSubAddress(m_ubyMessageNumber);
			ubyChannelSel		= fnGetChannelSel(m_ubyMessageNumber);
			ubyWordCount		= fnGetWordCount(m_ubyMessageNumber);
			fnGetMessageName(m_ubyMessageNumber,m_stMsg[m_ubyMessageNumber].szMessageName);

			m_stMsg[m_ubyMessageNumber].MsgDef.Int_Msg_Gap	 = 10;
			m_stMsg[m_ubyMessageNumber].MsgDef.bus           = ubyChannelSel; 
			m_stMsg[m_ubyMessageNumber].MsgDef.self_test     = NO;
			m_stMsg[m_ubyMessageNumber].MsgDef.retry_enable = YES;
			m_stMsg[m_ubyMessageNumber].MsgDef.mask_bcst_bit = NO;
			m_stMsg[m_ubyMessageNumber].MsgDef.broadcast     = NO;
			m_stMsg[m_ubyMessageNumber].MsgDef.mode_code     = NO;
			m_stMsg[m_ubyMessageNumber].MsgDef.rt_rt         = NO;
			m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.taddr   = ubyDestAddress;
			m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.status  = 0xcccc;
			m_stMsg[m_ubyMessageNumber].MsgDef.loop          = 0x0000;
			m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.subaddr = ubyDestSubAddress;
			m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.wcnt    = ubyWordCount; 

			if(ubySourceAddress == 32)/* BC->RT transfer */
			{
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.t_r			= RECEIVE;
				m_stMsg[m_ubyMessageNumber].ubyRxMsgCount = ubyReceiveMessageCount;
				ubyReceiveMessageCount++;
			}
			if(ubyDestAddress == 32)/* RT->BC transfer */
			{
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.t_r	 = TRANSMIT;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.taddr   = ubySourceAddress;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.subaddr = ubySourceSubAddress;
				m_stMsg[m_ubyMessageNumber].ubyTxMsgCount = ubyTransmitMessageCount;
				ubyTransmitMessageCount++;
			}
			if(( (ubyDestSubAddress == 0) || (ubyDestSubAddress == 31)) && ubySourceAddress == 32)/* Mode code *//* Source is BC.*/
			{
				m_stMsg[m_ubyMessageNumber].MsgDef.mode_code     = YES;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.t_r	 = TRANSMIT;
			}
			if(( (ubySourceSubAddress == 0) || (ubySourceSubAddress == 31)) && (ubyDestAddress == 32))/* Mode code *//* Source is RT.*/
			{
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.taddr   = ubySourceAddress;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.subaddr = ubySourceSubAddress;
				m_stMsg[m_ubyMessageNumber].MsgDef.mode_code     = YES;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.t_r	 = TRANSMIT;
			}	
			if((ubySourceAddress != 32) && (ubyDestAddress != 32))/* RT to RT transfer */
			{
				m_stMsg[m_ubyMessageNumber].MsgDef.rt_rt         = YES;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.taddr   = ubySourceAddress;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.status  = 0xcccc;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.subaddr = ubySourceSubAddress;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.wcnt    = ubyWordCount;  
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.t_r	 = TRANSMIT;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_2.taddr   = ubyDestAddress;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_2.status  = 0xcccc;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_2.subaddr = ubyDestSubAddress;
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_2.wcnt    = ubyWordCount; 
				m_stMsg[m_ubyMessageNumber].MsgDef.cmd_2.t_r	 = RECEIVE;
			}

			switch(ubyTimeFrequency)
			{
				case 160: 
					m_MinorFrame[ubyMinorFrameNumber].frame_items[m_MinorFrame[ubyMinorFrameNumber].Mess_Count] = m_ubyMessageNumber;
					m_MinorFrame[ubyMinorFrameNumber].Mess_Count++;
				break;
				case 80:
					m_MinorFrame[ubyMinorFrameNumber].frame_items[m_MinorFrame[ubyMinorFrameNumber].Mess_Count] = m_ubyMessageNumber;
					m_MinorFrame[ubyMinorFrameNumber].Mess_Count++;
					ubyMinorFrameNumber = ubyMinorFrameNumber + 4;
					ubyMinorFrameNumber = (ubyMinorFrameNumber == 8) ? 8 : (ubyMinorFrameNumber % 8);
					m_MinorFrame[ubyMinorFrameNumber].frame_items[m_MinorFrame[ubyMinorFrameNumber].Mess_Count] = m_ubyMessageNumber;
					m_MinorFrame[ubyMinorFrameNumber].Mess_Count++;
				break;
				case 40:
					m_MinorFrame[ubyMinorFrameNumber].frame_items[m_MinorFrame[ubyMinorFrameNumber].Mess_Count] = m_ubyMessageNumber;
					m_MinorFrame[ubyMinorFrameNumber].Mess_Count++;
					ubyMinorFrameNumber = ubyMinorFrameNumber + 2;
					ubyMinorFrameNumber = (ubyMinorFrameNumber == 8) ? 8 : (ubyMinorFrameNumber % 8);
					m_MinorFrame[ubyMinorFrameNumber].frame_items[m_MinorFrame[ubyMinorFrameNumber].Mess_Count] = m_ubyMessageNumber;
					m_MinorFrame[ubyMinorFrameNumber].Mess_Count++;
					ubyMinorFrameNumber = ubyMinorFrameNumber + 2;
					ubyMinorFrameNumber = (ubyMinorFrameNumber == 8) ? 8 : (ubyMinorFrameNumber % 8);
					m_MinorFrame[ubyMinorFrameNumber].frame_items[m_MinorFrame[ubyMinorFrameNumber].Mess_Count] = m_ubyMessageNumber;
					m_MinorFrame[ubyMinorFrameNumber].Mess_Count++;
					ubyMinorFrameNumber = ubyMinorFrameNumber + 2;
					ubyMinorFrameNumber = (ubyMinorFrameNumber == 8) ? 8 : (ubyMinorFrameNumber % 8);
					m_MinorFrame[ubyMinorFrameNumber].frame_items[m_MinorFrame[ubyMinorFrameNumber].Mess_Count] = m_ubyMessageNumber;
					m_MinorFrame[ubyMinorFrameNumber].Mess_Count++;
				break;
				case 20:
					 for(ubyLoopCount = MINOR_CYCLE_ZERO;ubyLoopCount<= MINOR_CYCLE_SEVEN;ubyLoopCount++)
					 {					 
						m_MinorFrame[ubyLoopCount].frame_items[m_MinorFrame[ubyLoopCount].Mess_Count] = m_ubyMessageNumber;
						m_MinorFrame[ubyLoopCount].Mess_Count++;
					 }
				break;
			}
			m_ubyMessageNumber++;	

		}
	}
	fclose(fpstConfigFile);

}

unsigned char fnGetMinorFrame(unsigned char ubyMessageNumber)
{
	char szMinorFrameNumber[4];
	
	sscanf(m_rgszMessagesDatabase[ubyMessageNumber],"%s",szMinorFrameNumber);
	if(atoi(szMinorFrameNumber)>MAX_MINOR_FRAME_NUMBER)
	{
		return MAX_MINOR_FRAME_NUMBER + 1;
	}
	
	if(atoi(szMinorFrameNumber)<1)
	{
		return 1;
	}
	
	return (atoi(szMinorFrameNumber));
}

void fnDefineSyncMsg()
{
	int ubyData = 0x0000;
	m_ubyMessageNumber=0;
	m_urgswTimeBroadcastData[0]=0;
	m_urgswTimeBroadcastData[1]=0;
	memset(m_stMsg,0,sizeof(m_stMsg));

	//Define Sync Message for each minorframe on Channel A and Channel B
	 for( unsigned char ubyMinorFrameIndex = 0; ubyMinorFrameIndex < 8; ubyMinorFrameIndex++)
	 {
		 m_MinorFrame[ubyMinorFrameIndex].Mess_Count = 0;

		//sync msg1
		m_stMsg[m_ubyMessageNumber].MsgDef.Int_Msg_Gap	 = 10;
		m_stMsg[m_ubyMessageNumber].MsgDef.bus			 = CH_A;
		m_stMsg[m_ubyMessageNumber].MsgDef.self_test     = NO;
		m_stMsg[m_ubyMessageNumber].MsgDef.retry_enable =  YES;
		m_stMsg[m_ubyMessageNumber].MsgDef.mask_bcst_bit = NO;			
		m_stMsg[m_ubyMessageNumber].MsgDef.broadcast     = YES;
		m_stMsg[m_ubyMessageNumber].MsgDef.mode_code     = YES;
		m_stMsg[m_ubyMessageNumber].MsgDef.rt_rt         = NO;
		m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.taddr   = CIU_BROADCAST_ADDRESS;
		m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.t_r     = RECEIVE;
		m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.status  = 0xcccc;
		m_stMsg[m_ubyMessageNumber].MsgDef.loop          = 0x0000;
		m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.subaddr = 0;
		m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.wcnt    = 17;  
		strcpy(m_stMsg[m_ubyMessageNumber].szMessageName, "Sync Data");
		for(int i=0; i<m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.wcnt; i++)
			m_stMsg[m_ubyMessageNumber].databuffer[i] = ubyData;
		
		m_MinorFrame[ubyMinorFrameIndex].frame_items[m_MinorFrame[ubyMinorFrameIndex].Mess_Count] = m_ubyMessageNumber;
		m_MinorFrame[ubyMinorFrameIndex].Mess_Count++;

		m_ubyMessageNumber++;
		
		//sync msg2
		m_stMsg[m_ubyMessageNumber].MsgDef.Int_Msg_Gap	 = 10;
		m_stMsg[m_ubyMessageNumber].MsgDef.bus           = CH_B;
		m_stMsg[m_ubyMessageNumber].MsgDef.self_test     = NO;
		m_stMsg[m_ubyMessageNumber].MsgDef.retry_enable = YES;
		m_stMsg[m_ubyMessageNumber].MsgDef.mask_bcst_bit = NO;			
		m_stMsg[m_ubyMessageNumber].MsgDef.broadcast     = YES;
		m_stMsg[m_ubyMessageNumber].MsgDef.mode_code     = YES;
		m_stMsg[m_ubyMessageNumber].MsgDef.rt_rt         = NO;
		m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.taddr   = CIU_BROADCAST_ADDRESS;
		m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.t_r     = RECEIVE;
		m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.status  = 0xcccc;
		m_stMsg[m_ubyMessageNumber].MsgDef.loop          = 0x0000;
		m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.subaddr = 0;
		m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.wcnt    = 17;  
		strcpy(m_stMsg[m_ubyMessageNumber].szMessageName, "Sync Data");
		for(int i=0; i<m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.wcnt; i++)
				m_stMsg[m_ubyMessageNumber].databuffer[i] = ubyData;
		
		m_MinorFrame[ubyMinorFrameIndex].frame_items[m_MinorFrame[ubyMinorFrameIndex].Mess_Count] = m_ubyMessageNumber;
		m_MinorFrame[ubyMinorFrameIndex].Mess_Count++;

		ubyData+=0x1000;
		m_ubyMessageNumber++;
	 }

	 //Define TimeBroadCast Message
	m_stMsg[m_ubyMessageNumber].MsgDef.Int_Msg_Gap	 = 10;
	m_stMsg[m_ubyMessageNumber].MsgDef.bus           = CH_A;
	m_stMsg[m_ubyMessageNumber].MsgDef.self_test     = NO;
	m_stMsg[m_ubyMessageNumber].MsgDef.mask_bcst_bit = NO;	
	m_stMsg[m_ubyMessageNumber].MsgDef.retry_enable = YES;
	m_stMsg[m_ubyMessageNumber].MsgDef.broadcast     = YES;
	m_stMsg[m_ubyMessageNumber].MsgDef.mode_code     = NO;
	m_stMsg[m_ubyMessageNumber].MsgDef.rt_rt         = NO;
	m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.taddr   = CIU_BROADCAST_ADDRESS;
	m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.t_r     = RECEIVE;
	m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.status  = 0xcccc;
	m_stMsg[m_ubyMessageNumber].MsgDef.loop          = 0x0000;
	m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.subaddr = 0x1D;
	m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.wcnt    = 2;  
	strcpy(m_stMsg[m_ubyMessageNumber].szMessageName, "OAC1_12");
	for(int i=0; i<m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.wcnt; i++)
				m_stMsg[m_ubyMessageNumber].databuffer[i] = m_urgswTimeBroadcastData[i];
	
	m_MinorFrame[MINOR_CYCLE_ZERO].frame_items[m_MinorFrame[MINOR_CYCLE_ZERO].Mess_Count] = m_ubyMessageNumber;
	m_MinorFrame[MINOR_CYCLE_ZERO].Mess_Count++;
	m_ubyMessageNumber++;

	 //Define TimeBroadCast Message
	m_stMsg[m_ubyMessageNumber].MsgDef.Int_Msg_Gap	 = 10;
	m_stMsg[m_ubyMessageNumber].MsgDef.bus           = CH_B;
	m_stMsg[m_ubyMessageNumber].MsgDef.self_test     = NO;
	m_stMsg[m_ubyMessageNumber].MsgDef.retry_enable = YES;
	m_stMsg[m_ubyMessageNumber].MsgDef.mask_bcst_bit = NO;			
	m_stMsg[m_ubyMessageNumber].MsgDef.broadcast     = YES;
	m_stMsg[m_ubyMessageNumber].MsgDef.mode_code     = NO;
	m_stMsg[m_ubyMessageNumber].MsgDef.rt_rt         = NO;
	m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.taddr   = CIU_BROADCAST_ADDRESS;
	m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.t_r     = RECEIVE;
	m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.status  = 0xcccc;
	m_stMsg[m_ubyMessageNumber].MsgDef.loop          = 0x0000;
	m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.subaddr = 0x1D;
	m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.wcnt    = 2;  
	strcpy(m_stMsg[m_ubyMessageNumber].szMessageName, "OAC1_12");
	for(int i=0; i<m_stMsg[m_ubyMessageNumber].MsgDef.cmd_1.wcnt; i++)
				m_stMsg[m_ubyMessageNumber].databuffer[i] = m_urgswTimeBroadcastData[i];
	
	m_MinorFrame[MINOR_CYCLE_ZERO].frame_items[m_MinorFrame[MINOR_CYCLE_ZERO].Mess_Count] = m_ubyMessageNumber;
	m_MinorFrame[MINOR_CYCLE_ZERO].Mess_Count++;
	m_ubyMessageNumber++;

}
unsigned char fnGetFrequency(unsigned char ubyMessageNumber)
{
	char szFreq[25];
	
	sscanf(m_rgszMessagesDatabase[ubyMessageNumber],"%s%s%s%s%s%s%s%s%s",
				szFreq,szFreq,szFreq,szFreq,szFreq,szFreq,szFreq,szFreq,szFreq );

	if(strcmp(szFreq,"6.25") == 0)
	{
		return 160;
	}

	if(strcmp(szFreq,"12.5") == 0)
	{
		return 80;
	}

	if(strcmp(szFreq,"25") == 0)
	{
		return 40;
	}

	if(strcmp(szFreq,"50") == 0)
	{
		return 20;
	}

	return 160;
}




unsigned char fnGetSourceAddress(unsigned char ubyMessageNumber)
{
	char szSourceAddress[25];
	
	sscanf(m_rgszMessagesDatabase[ubyMessageNumber],"%s%s%s",szSourceAddress,szSourceAddress,szSourceAddress);

	if(strcmp(szSourceAddress,"--") == 0)
	{
		return 32;		/* if source address is BC, then return 32 */
	}

	if(atoi(szSourceAddress)>MAX_SOURCE_ADDRESS)
	{
		return MAX_SOURCE_ADDRESS;
	}
	
	if(atoi(szSourceAddress)<0)
	{
		return 0;
	}

	return (atoi(szSourceAddress));
	
}

unsigned char fnGetDestAddress(unsigned char ubyMessageNumber)
{
	char szDestAddress[25];
	
	sscanf(m_rgszMessagesDatabase[ubyMessageNumber],"%s%s%s%s",szDestAddress,szDestAddress,szDestAddress,szDestAddress,szDestAddress);

	if(strcmp(szDestAddress,"--") == 0)
	{
		return 32;		/* if dest address is BC, then return 32 */
	}

	if(atoi(szDestAddress)>MAX_DEST_ADDRESS)
	{
		return MAX_DEST_ADDRESS;
	}

	if(atoi(szDestAddress)<0)
	{
		return 0;
	}

	return (atoi(szDestAddress));
}



unsigned char fnGetSourceSubAddress(unsigned char ubyMessageNumber)
{
	char szSourceSubAddress[25];
	
	sscanf(m_rgszMessagesDatabase[ubyMessageNumber],"%s%s%s%s%s",szSourceSubAddress,szSourceSubAddress,
				szSourceSubAddress,szSourceSubAddress,szSourceSubAddress,szSourceSubAddress);

	if(strcmp(szSourceSubAddress,"--") == 0)
	{
		return 32;	
	}

	if(atoi(szSourceSubAddress)>MAX_SOURCE_SUBADDRESS)
	{
		return MAX_SOURCE_SUBADDRESS;
	}

	if(atoi(szSourceSubAddress)<0)
	{
		return 0;
	}

	return (atoi(szSourceSubAddress));	
}


unsigned char fnGetDestSubAddress(unsigned char ubyMessageNumber)
{
	char szDestSubAddress[25];
	
	sscanf(m_rgszMessagesDatabase[ubyMessageNumber],"%s%s%s%s%s%s",szDestSubAddress,szDestSubAddress,
				szDestSubAddress,szDestSubAddress,szDestSubAddress,szDestSubAddress,szDestSubAddress);

	if(strcmp(szDestSubAddress,"--") == 0)
	{
		return 32;		
	}
	
	if(atoi(szDestSubAddress)>MAX_DEST_SUBADDRESS)
	{
		return MAX_DEST_SUBADDRESS;
	}

	if(atoi(szDestSubAddress)<0)
	{
		return 0;
	}

	return (atoi(szDestSubAddress));	


}


unsigned char fnGetChannelSel(unsigned char ubyMessageNumber)
{

	char szChannelSel[25];
	
	sscanf(m_rgszMessagesDatabase[ubyMessageNumber],"%s%s%s%s%s%s%s",szChannelSel,szChannelSel,
				szChannelSel,szChannelSel,szChannelSel,szChannelSel,szChannelSel,szChannelSel );

	if(strcmp(szChannelSel,"A") == 0)
	{
		return CH_A;
	}
	else
	{
		return CH_B;
	}

}



unsigned char fnGetWordCount(unsigned char ubyMessageNumber)
{
	char szWordCount[25];
	
	sscanf(m_rgszMessagesDatabase[ubyMessageNumber],"%s%s%s%s%s%s%s%s",szWordCount,szWordCount,szWordCount,
				szWordCount,szWordCount,szWordCount,szWordCount,szWordCount);

	if(atoi(szWordCount)>MAX_WORD_COUNT)
	{
		return MAX_WORD_COUNT;
	}

	if(atoi(szWordCount)<1)
	{
		return 1;
	}

	return (atoi(szWordCount));	
}

void fnGetMessageName(unsigned char ubyMessageNumber, char * szMessageName)
{
	sscanf(m_rgszMessagesDatabase[ubyMessageNumber],"%s%s",szMessageName,szMessageName);
}
unsigned int fnHandle1553BCommunication()
{   
	char chr;
	unsigned short uhwFrameItems[20];
	unsigned short uhwMsgIndex1=0,wMsgIndex2=0,uhwMsgId=0;
	unsigned int	uwMessCount1=0,uwMessCount2=0,uwTxMsgCount=0;
	USHORT data_buffer[32];
	unsigned char ubyWordCount=0;

	while(1)
	{
		
	for (int wMinorFrameIndex = MINOR_CYCLE_ZERO; wMinorFrameIndex <=MINOR_CYCLE_SEVEN; wMinorFrameIndex++)
	{
			SetTMR2Cnt_A(0,CARD1);
			//SetTMR2Cnt_A(BCMODL2,CARD1);
			//SetEndOfFrameCnt_A(BCMODL1,0,CARD1);
			
			if (wMinorFrameIndex == MINOR_CYCLE_ZERO)
			{
				fnUpdateTimeBroadcastInfo();
			}
				uhwMsgIndex1=0;
				uwMessCount1 = m_MinorFrame[wMinorFrameIndex].Mess_Count;
				memset(uhwFrameItems,0,sizeof(uhwFrameItems));
			
				for(int wMsgIndex = 0; wMsgIndex < uwMessCount1; wMsgIndex++)
				{
					uhwMsgId = m_MinorFrame[wMinorFrameIndex].frame_items[wMsgIndex];
					uhwMsgIndex1++;
					
					bc_page.Int_Msg_Gap   = m_stMsg[uhwMsgId].MsgDef.Int_Msg_Gap;
					bc_page.broadcast     = m_stMsg[uhwMsgId].MsgDef.broadcast;
					bc_page.bus           = m_stMsg[uhwMsgId].MsgDef.bus;	
					bc_page.retry_enable =  m_stMsg[uhwMsgId].MsgDef.retry_enable;
					bc_page.cmd_1.status  = m_stMsg[uhwMsgId].MsgDef.cmd_1.status;
					bc_page.cmd_1.subaddr = m_stMsg[uhwMsgId].MsgDef.cmd_1.subaddr;
					bc_page.cmd_1.t_r     = m_stMsg[uhwMsgId].MsgDef.cmd_1.t_r;

					bc_page.cmd_1.taddr   = m_stMsg[uhwMsgId].MsgDef.cmd_1.taddr;
					bc_page.cmd_1.wcnt    = m_stMsg[uhwMsgId].MsgDef.cmd_1.wcnt;  
					bc_page.loop          = m_stMsg[uhwMsgId].MsgDef.loop;
					bc_page.mask_bcst_bit = m_stMsg[uhwMsgId].MsgDef.mask_bcst_bit;
					bc_page.mode_code     = m_stMsg[uhwMsgId].MsgDef.mode_code;
					bc_page.rt_rt         = m_stMsg[uhwMsgId].MsgDef.rt_rt;
					bc_page.self_test     = m_stMsg[uhwMsgId].MsgDef.self_test;
					if (wMinorFrameIndex == MINOR_CYCLE_ZERO)
					{
						if((uhwMsgId == TIMEBROADCASTCHNNLBMSGID) || (uhwMsgId == TIMEBROADCASTCHNNLAMSGID))//Update Timebroadcast data for MinorFrame 0
						{
							for(int i=0;i<bc_page.cmd_1.wcnt;i++)
							m_stMsg[uhwMsgId].databuffer[i] = m_urgswTimeBroadcastData[i];
						}
					}
					if((bc_page.cmd_1.t_r == RECEIVE) && (bc_page.mode_code == NO) && (bc_page.broadcast  == NO))//Update from Receive Msgs
					{
						if (bc_page.cmd_1.wcnt == 0)
							ubyWordCount = 32;
						else ubyWordCount = bc_page.cmd_1.wcnt;
						/*for(int i=0; i< ubyWordCount; i++)
						m_stMsg[uhwMsgId].databuffer[i] = m_pReceiveMessages[BUS1]->m_rgpwMessagePointer[m_stMsg[uhwMsgId].ubyRxMsgCount][i];*/
					}
					
					//BCRetry_A(1,AA,CARD1);
					DefMsg_A(1,BCSTACK_A,uhwMsgIndex1,bc_page,m_stMsg[uhwMsgId].databuffer,ubyWordCount ,CARD1);
					uhwFrameItems[wMsgIndex] = uhwMsgIndex1;
					
				}
			
				DefFrame_A(1,BCSTACK_A,uwMessCount1,uhwFrameItems,CARD1);
				//RunBc_A(1,BCSTACK_A,1,uwMessCount1,1,CARD1);
				RunBcNowait_A(1,BCSTACK_A,1,uwMessCount1,1,CARD1);
			LARGE_INTEGER Frequency;		
			LARGE_INTEGER liStart;
			LARGE_INTEGER liStop;
			double dftDuration;
			
			if(!QueryPerformanceFrequency(&Frequency))
			{}
		  	else
			{
				 QueryPerformanceCounter(&liStart);			
				 do
				 {
					   QueryPerformanceCounter(&liStop);

					  LONGLONG llTimeDiff = liStop.QuadPart - liStart.QuadPart;
				  	 
					  dftDuration = /*(double)*/ llTimeDiff * 1000.0 / /*(double)*/ Frequency.QuadPart;
					  //printf("%lf\n",dftDuration);
				  }
				  while(dftDuration<20.0000);
				 // printf("%lf\n",dftDuration);
			/*	 if(dftDuration>20.0000)
				   fprintf(Tm,"%lf\n",dftDuration);*/
			}
				//printf("Interrupts:%d\n",GetBCEOMCnt_A(1,CARD1));
				
		}
		if(_kbhit())
		{
			chr = _getch();
			if(chr == 27)
			{
				
				ResetBc_A(1,CARD1);	
				ResetMt_A(1,CARD1);
				printf("\n\t**************  OVER   *******************");
				DisableEnhancedInterrupts_A(1,INTR_BC_ENDOF_MINFRAME,CARD1);	
				CloseDeviceHandle_A(CARD1);
				exit(0);
			}
		}
	}
			
 return 0;
}
void fnUpdateTimeBroadcastInfo()
{

	unsigned char ubyMilliSec,ubySec,ubyMin,ubyHr;

	ubyMilliSec = m_urgswTimeBroadcastData[1] & 0x00FF;
	ubySec = m_urgswTimeBroadcastData[1] >> 8;
	ubyMin = m_urgswTimeBroadcastData[0] & 0x00FF;
	ubyHr = m_urgswTimeBroadcastData[0] >> 8;

	ubyMilliSec += 8;
	
	if(ubyMilliSec > 49)
	{
		ubyMilliSec %= 50;
		ubySec += 1;

		if(ubySec == 60)
		{
			ubySec = 0;
			ubyMin += 1;
		}

		if(ubyMin == 60)
		{
			ubyMin = 0;
			ubyHr += 1;
		}

		if(ubyHr == 255)
		{
			ubyHr = 0;
		}
	}

	m_urgswTimeBroadcastData[1] = ubySec;
	m_urgswTimeBroadcastData[1] = (m_urgswTimeBroadcastData[1] << 8)|ubyMilliSec;
	m_urgswTimeBroadcastData[0] = ubyHr;
	m_urgswTimeBroadcastData[0] = (m_urgswTimeBroadcastData[0] << 8)|ubyMin ;

	urgswTimeBroadcastData[0] = m_urgswTimeBroadcastData[0];
	urgswTimeBroadcastData[1] = m_urgswTimeBroadcastData[1];
}


void ActivateRT()
{
	UCHAR Address[31],Saddres=1,RtMod=1;
	USHORT data_buff[40],Rtno=0,RtNo=0; 
	UCHAR RT_Address;
	/*bc_int.value = 0;
	bc_int.bifields.eom = 1;*/
	/*bc_int.bc_eom = 0 ;		
	bc_int.fmterr_statset = 0;  
	bc_int.eom = 1;	*/

	RT_Address=0xff;
	for(Rtno=0;Rtno<=30;Rtno++)
	{
		ClearRam_A(RtMod,Rtno,CARD1);
		Address[Rtno]=Rtno;
		ConfigureRT_A(RtMod,0,Address,CARD1);
		for(i=0;i<=31;i++)
			data_buff[i]=0x1553+i;  	
			
		for(Saddres=0;Saddres<=30;Saddres++)
		{
		WriteDataBlock_A(RtMod,Rtno,Saddres,data_buff,CARD1); 
		}
			
	}
	RunRt_A(RtMod,0,Address,CARD1);	
	RtMod =2;
	for(Rtno=0;Rtno<=30;Rtno++)
	{
		ClearRam_A(RtMod,Rtno,CARD1);
		Address[Rtno]=Rtno;
		ConfigureRT_A(RtMod,0,Address,CARD1);
		for(i=0;i<=31;i++)
			data_buff[i]=0x1553+i;  	
			
		for(Saddres=0;Saddres<=30;Saddres++)
		{
		WriteDataBlock_A(RtMod,Rtno,Saddres,data_buff,CARD1); 
		}
			
	}
	RunRt_A(RtMod,0,Address,CARD1);	

	RtMod =3;
	for(Rtno=0;Rtno<=30;Rtno++)
	{
		ClearRam_A(RtMod,Rtno,CARD1);
		Address[Rtno]=Rtno;
		ConfigureRT_A(RtMod,0,Address,CARD1);
		for(i=0;i<=31;i++)
			data_buff[i]=0x1553+i;  	
			
		for(Saddres=0;Saddres<=30;Saddres++)
		{
		WriteDataBlock_A(RtMod,Rtno,Saddres,data_buff,CARD1); 
		}
			
	}
	RunRt_A(RtMod,0,Address,CARD1);	
}

void Init_MT()
{
	Mterr1  = fopen_s(&Fp1_1,"Bus1Log.dat", "w");
	Mterr2 = fopen_s(&Fp2_1,"Bus2Log.dat", "w");	
	Mterr3 = fopen_s(&Fp3_1,"Bus3Log.dat", "w");	
	if((Mterr1==0)||(Mterr3==0)||(Mterr2==0))
	{}
	else 
	{
		perror( "perror says open failed" );
		exit(0);		 
	}
	

	hMutexObj=CreateEvent(NULL,FALSE,TRUE,(LPCWSTR)"MyMutexObj");
	atexit(DeActivateMT);
	printf("Modules Initialized.......\n");
		ResetMt_A(1,CARD1);		
		ClearRam_A(1,MT,CARD1);
		SelMsgMt_A(1,0,CARD1);
		SetMTEOMCnt_A(1,0,CARD1);	
		RunMt_A(1,CARD1);

		ResetMt_A(2,CARD1);		
		ClearRam_A(2,MT,CARD1);
		SelMsgMt_A(2,0,CARD1);
		SetMTEOMCnt_A(2,0,CARD1);	
		RunMt_A(2,CARD1);

		ResetMt_A(3,CARD1);		
		ClearRam_A(3,MT,CARD1);
		SelMsgMt_A(3,0,CARD1);
		SetMTEOMCnt_A(3,0,CARD1);	
		RunMt_A(3,CARD1);
}
void InitCard_init(void) /*PC Based OAC*/
{
   Mc1Cardno = 1;
   Mc1Modno  = MODULE_ID_0;

   Mc2Cardno = 1;
   Mc2Modno  = MODULE_ID_1;

   Mc3Cardno = 1; 
   Mc3Modno  = MODULE_ID_2;
}

void DeActivateMT(void)
{
	printf("\n MT Deactivated \n");
	CloseMT1Module();
	/*CloseMT2Module();
	CloseMT3Module();*/
}

void CloseMT1Module()
{	
	MT1Initialised = FALSE;
	MT2Initialised = FALSE;
	MT3Initialised = FALSE;
	
	SelectModule(1,CARD1);
	DisableInterrupts(INTR_BUS0,CARD1);
	SelectModule(2,CARD1);
	DisableInterrupts(INTR_BUS1,CARD1);
	SelectModule(3,CARD1);
	DisableInterrupts(INTR_BUS2,CARD1);		

	SelectModule(1,CARD1);
	ResetAce(CARD1);
	SelectModule(2,CARD1);
	ResetAce(CARD1);
	SelectModule(3,CARD1);
	ResetAce(CARD1);
	printf("\t\tWait.!\n");
	Sleep(500);
	//CloseHandle(hthread1);//CloseDeviceHandle(CARD1);
}

void ActivateMc(void)
{
	if(Mc1LruInitialised == FALSE) 
	{
		Mc1LruInitialised = IntOpenBC1(Mc1Modno,Mc1Cardno);	
		if(Mc1LruInitialised == TRUE)	printf(" BC1 INITIALISED ");   
	}/**/

	//uncomment
	if(Mc2LruInitialised == FALSE)
	{
		Mc2LruInitialised = IntOpenBC2(Mc2Modno,Mc2Cardno);
		if(Mc2LruInitialised == TRUE)	printf(" BC2 INTIALISED ");
	}/**/
	
	if(Mc3LruInitialised == FALSE)
	{
		Mc3LruInitialised = IntOpenBC3(Mc3Modno,Mc3Cardno);
		if(Mc3LruInitialised == TRUE)	printf(" BC3 INTIALISED");
	}/**/
}
void OnButtonLoadframe() 
{
	char			szFilenameB1[15], szFilenameB2[15];
	unsigned char	ubyLoopCount;
	
		strcpy(szFilenameB1,"INIT_B1.txt");


	fnExtractMessages(szFilenameB1);
	
//	fnAllocateMemoryForReceiveDataWords(szFilenameB1);

//	fnAllocateMemoryForTransmitDataWords(szFilenameB1);

}

void fnAllocateMemoryForReceiveDataWords(char * szFilenameB1)
{
	m_ubyMessageNumber = 0;	
	m_wBus1Bus2 = BUS1;
	fnExtractReceiveMessageInfo(szFilenameB1);
	fnInitialiseMessageWords();	

}

void fnExtractReceiveMessageInfo(char * szFilename)
{

	char szFileString[100], szSourceName[20]; 
	
	unsigned char ubyLoopCount;

	
	FILE *fpstConfigFile;
//	fpstConfigFile = fopen("INIT_B1.txt","r");	
	fpstConfigFile = fopen(szFilename,"rt");
	if(fpstConfigFile == NULL)
	{
		printf("Unable to open Configuration File");

		exit(1);
	}


	for(ubyLoopCount=0;ubyLoopCount<3;ubyLoopCount++)
	{
		fgets(szFileString,256,fpstConfigFile);
	}
	/* First three lines contain header information that need
	not be processed */
	
	while (!feof(fpstConfigFile))
	{

		fgets(szFileString,256,fpstConfigFile);

		int iRet = sscanf(szFileString,"%s%s%s",szSourceName,szSourceName,szSourceName);

		if(iRet== 3)
		{
			if(strcmp(szSourceName,"--") == 0)
			{
				/* Receive Message Found. Store the file string in Receive Messages Database for future use*/
				if(m_wBus1Bus2 == BUS1)
				{
					strcpy(m_rgszReceiveMessagesDatabase[m_ubyMessageNumber],szFileString);
					m_ubyMessageNumber++;			
				}
			}
		}
				
	}

	fclose(fpstConfigFile);

}

void fnInitialiseMessageWords()
{

	unsigned char ubyLoopCount, ubyLoopCount1;

	
	if(m_wBus1Bus2 == BUS1)
	{
		if(m_ubyfInitialiseMessages != NOALLOCATE)
		{
			if(m_ubyfInitialiseMessages == ALLOCATE)
			{
				for(ubyLoopCount=0;ubyLoopCount<100;ubyLoopCount++)
				{
					m_rgpwMessagePointer[ubyLoopCount] = NULL;
					m_rgpwMessagePointer[ubyLoopCount] = (unsigned short int *)malloc(32 * sizeof(unsigned short int));

					for(ubyLoopCount1=0;ubyLoopCount1<32;ubyLoopCount1++)
					{
						m_rgpwMessagePointer[ubyLoopCount][ubyLoopCount1] = 0;
					}
				}
			}
						
			if(m_ubyfInitialiseMessages == REALLOCATE)
			{
				//fnReInitialiseDisplay();	
				
				for(ubyLoopCount=0;ubyLoopCount<100;ubyLoopCount++)
				{				
					for(ubyLoopCount1=0;ubyLoopCount1<32;ubyLoopCount1++)
					{
						m_rgpwMessagePointer[ubyLoopCount][ubyLoopCount1] = 0;
					}
				}
			}			

			m_ubyfInitialiseMessages = NOALLOCATE;
		}
	}
	
}




void fnAllocateMemoryForTransmitDataWords(char * szFilenameB1)
{
	//CHANGES DONE ON SEP-23-2013 V2.6 R0.0
	m_ubyMessageNumber = 0;	
	m_wBus1Bus2 = BUS1;
	fnExtractTransmitMessageInfo(szFilenameB1);
	fnInitialiseMessageWords();
}
void fnExtractTransmitMessageInfo(char * szFilename)
{
	char szFileString[100], szSourceName[25]; 
	
	unsigned char ubyLoopCount;

	
	FILE *fpstConfigFile;

	fpstConfigFile = fopen(szFilename,"rt");
	if(fpstConfigFile == NULL)
	{
		printf("Unable to open Configuration File");

		exit(1);
	}


	for(ubyLoopCount=0;ubyLoopCount<3;ubyLoopCount++)
	{
		fgets(szFileString,256,fpstConfigFile);
	}
	/* First three lines contain header information that need
	not be processed */
	
	while (!feof(fpstConfigFile))
	{

		fgets(szFileString,256,fpstConfigFile);

		int iRet = sscanf(szFileString,"%s%s%s%s",szSourceName,szSourceName,szSourceName,szSourceName);

		if(iRet == 4)
		{
			if(strcmp(szSourceName,"--") == 0)
			{
				/* Transmit Message Found. Store the file string in Transmit Messages Database for future use*/
				if(m_wBus1Bus2 == BUS1)
				{
					strcpy(m_rgszTransmitMessagesDatabase[m_ubyMessageNumber],szFileString);
					m_ubyMessageNumber++;			
				}
			}	
		}		
	}

	fclose(fpstConfigFile);
}


unsigned short IntOpenBC1(USHORT ModuleId ,USHORT CardId)
{
	ClearRam_A(1,BC,CARD1);
	ConfigureBC_A(1,128,CARD1);
#if 0
	//InitInterrupts(INTR_BUS0,(UCHAR)CardId);
	//InitInterrupts_A(1,INTR_BCEOM|INTR_MTEOM,CARD1);
	//InitInterrupts_A(1,INTR_MTEOM,CARD1);
	
	/*bc_int.value = 0;
	bc_int.bifields.eom = 1;*/
	bc_int.bc_eom = 0 ;		
	bc_int.fmterr_statset = 0;  
	bc_int.eom = 1;		

	SetM1Cnt(0,0,CARD1);
	//SetM1Cnt(0);
	SelectModule(1,CARD1);
	ClearRam(CARD1);
	ResetAce(CARD1);	
	DefMode(BC,CARD1);

	/*BCAutoRepeat(YES);
	BcFrameTime(0xff);*/
	DefIntMask(bc_int,CARD1);//For EOM Int.
#endif
	return TRUE;
}
 
unsigned short IntOpenBC2(USHORT ModuleId ,USHORT CardId)
{
	ClearRam_A(2,BC,CARD1);
	ConfigureBC_A(2,128,CARD1);
#if 0
	//InitInterrupts(INTR_BUS1,(UCHAR)CardId);
	
	/*bc_int.value = 0;
	bc_int.bifields.eom = 1;*/
	//InitInterrupts_A(2,INTR_BCEOM|INTR_MTEOM,CARD1);
	//InitInterrupts_A(2,INTR_MTEOM,CARD1);
	
	bc_int.bc_eom = 0 ;		
	bc_int.fmterr_statset = 0;  
	bc_int.eom = 1;	

	SetM2Cnt(0,0,CARD1);
	SelectModule(2,CARD1);
	ClearRam(CARD1);
	ResetAce(CARD1);	
	DefMode(BC,CARD1);

	/*BCAutoRepeat(YES);
	BcFrameTime(0xff);*/
	DefIntMask(bc_int,CARD1);//For EOM Int.
#endif
	return TRUE;
}

unsigned short IntOpenBC3(USHORT ModuleId ,USHORT CardId)
{
	ClearRam_A(3,BC,CARD1);
	ConfigureBC_A(3,128,CARD1);
#if 0
	//InitInterrupts(INTR_BUS2,(UCHAR)CardId);
		
	//InitInterrupts_A(3,INTR_BCEOM|INTR_MTEOM,CARD1);
	//InitInterrupts_A(3,INTR_MTEOM,CARD1);
	/*bc_int.value = 0;
	bc_int.bifields.eom = 1;*/
	bc_int.bc_eom = 0 ;		
	bc_int.fmterr_statset = 0;  
	bc_int.eom = 1;	

	SetM3Cnt(0,0,CARD1);
	SelectModule(3,CARD1);
	ClearRam(CARD1);
	ResetAce(CARD1);	
	DefMode(BC,CARD1);

	/*BCAutoRepeat(YES);
	BcFrameTime(0xff);*/
	DefIntMask(bc_int,CARD1);//For EOM Int.
#endif
	return TRUE;
}

int Schedule()
{
	FILE *Tm = NULL;
	ULONG last_call_time;
	ULONG now_time;
	int ThreadRunFlag=0;
	int freqR1,freqR3,freqR2,freq4; 
	char chr;
	int freqR4;
	LARGE_INTEGER Freq4;
/*	Tmlog  = fopen_s(&Tm,"TimeLog.dat", "w");
		
	if((Tmlog==0))
	{}
	else 
	{
		perror( "perror says open failed" );
		exit(0);		 
	}
*/
	freqR4= QueryPerformanceFrequency(&Freq4);		
		//get_time_hud();
	while(ThreadRunFlag==0)
	{
		SetEvent(hMutexObj);
		{

		//	CountResponse = QueryPerformanceCounter(&StartCounter);
			LARGE_INTEGER Frequency;		
			LARGE_INTEGER liStart;
			LARGE_INTEGER liStop;
			double dftDuration;
			
			if(!QueryPerformanceFrequency(&Frequency))
			{}
		  // Start time measurement
			else
			{
				  QueryPerformanceCounter(&liStart);
			 
				/*if(Mc1LruInitialised == TRUE || Mc2LruInitialised == TRUE || Mc3LruInitialised == TRUE)
					Find_minor_cycleno_and_mccount();
						
				if(Mc1LruInitialised == TRUE)
				{
					Send20msSynch((UCHAR)Mc1Modno,CARD1,MC_minor_cycle_number, 1);
					msg_count[0][MC_minor_cycle_number] = Pos1;
				}

				if(Mc2LruInitialised == TRUE)
				{
					Send20msSynch((UCHAR)Mc2Modno,CARD1,MC_minor_cycle_number, 1);
					msg_count[1][MC_minor_cycle_number] = Pos2;
				}

				if(Mc3LruInitialised == TRUE)
				{
					Send20msSynch((UCHAR)Mc3Modno,CARD1,MC_minor_cycle_number, 1);
					msg_count[2][MC_minor_cycle_number] = Pos3;
				}
	
				Bus_Scheduel();//scheduel/**/
						
				//B1553_Bus_Scheduling();//framing	

				//Msg_counterRead();
				//Bus_Data_Recv();//reading results	
				  //OnButtonLoadframe();
				  //fnHandle1553BCommunication();
				 do
				 {
					   QueryPerformanceCounter(&liStop);

					  LONGLONG llTimeDiff = liStop.QuadPart - liStart.QuadPart;
				  	 
					  dftDuration = /*(double)*/ llTimeDiff * 1000.0 / /*(double)*/ Frequency.QuadPart;
					  printf("%lf\n",dftDuration);
				  }
				  while(dftDuration<20.0000);
				  printf("%lf\n",dftDuration);
			/*	 if(dftDuration>20.0000)
				   fprintf(Tm,"%lf\n",dftDuration);*/
			}
			

		//	Pos1=0;Pos2=0;Pos3=0;
		
			/*if(MC_minor_cycle_number == 0)//if(major_cycle == 3)//remove
			break;/**/
		}
		ResetEvent(hMutexObj);
		if(_kbhit())
		{
			chr = _getch();
		
			if(chr == 27)
			{		
				ThreadRunFlag=1;
				CloseHandle(hthread1);
				CloseHandle(hthread2);
				CloseHandle(hthread3);
				//CloseHandle(hthread4);
				break;
				//exit(0);
			}
		}
		//	_fcloseall();

	}
		
	return 0;	
}