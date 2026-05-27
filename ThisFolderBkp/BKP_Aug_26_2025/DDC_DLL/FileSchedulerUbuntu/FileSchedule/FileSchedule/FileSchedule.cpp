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
#include "FileHeader.h"
#include "Struct.h"
//#include "C:\FileSchedulerProject\FileSchedule\FileSchedule\Include\Libraries.h"
unsigned short int * m_rgpwMessagePointer[100];
#define MAX_SOURCE_ADDRESS              31
#define MAX_DEST_ADDRESS                31
#define MAX_SOURCE_SUBADDRESS           31
#define MAX_DEST_SUBADDRESS             31
#define MAX_WORD_COUNT                  32
#define MAX_MINOR_FRAME_NUMBER          7
#define TIMEBROADCASTCHNNLBMSGID 17
#define MINORFRAMETIME			20	//ms
#define TIMEBROADCASTCHNNLAMSGID 16
MAJOR_FRAME m_MinorFrame[10];
unsigned short int m_urgswTimeBroadcastData[3];
char m_rgszMessagesDatabase[200][100];
unsigned char m_ubyMessageNumber;
 MSG_DEF bc_page;
 unsigned short frame_items[128];
 USHORT data_buffer[40]; 
 MNTPACK MontDataBuff;
 MSG_RESULT msg_return;
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
void BcScheduleFile();
void BcFileLoad();

void main()
{
	char szFilenameB1[15]; 
	 int ret = InitCard_A(CARD1);
	SoftwareReset(CARD1);
	Sleep(500);
	InitInterrupts_A(1,INTR_BCEOM|INTR_TMR2,CARD1);
	InitTimerIntr_A(INTR_TMR2,CARD1);
	SelectTMR_A(0,MS_1,0,CARD1);
	SetTMR2Cnt_A(0,CARD1);
	SelectModule(0,CARD1);
	//InitInterrupts_A(BcMod, INTR_BCEOM|INTR_TMR2,CARD1);
	SetBCEOMCnt_A(1,0,CARD1);
	strcpy(szFilenameB1,"pmc_b1.txt");
	fnExtractMessages(szFilenameB1);
	fnHandle1553BCommunication();
}



void fnExtractMessages(char * szFilename)
{
	//MSG_DEF	MsgDef;
	char			szFileString[100]; 
	unsigned char	ubyLoopCount,ubyMinorFrameNumber;
	unsigned char	ubyTimeFrequency,ubyReceiveMessageCount,ubyTransmitMessageCount;
	unsigned char	ubySourceAddress, ubyDestAddress, ubySourceSubAddress, ubyDestSubAddress, ubyChannelSel;
	unsigned char	ubyWordCount;
	FILE			*fpstConfigFile;
	USHORT data_buffer[25];
	UCHAR RtMod = 1,BcMod = 1,MTMOD=1;
	int k,l,m,n,TempCnt;

	ClearRam_A(BcMod,BC,CARD1);
	ConfigureBC_A(BcMod,128,CARD1);

	ubyTransmitMessageCount=0;
	ubyReceiveMessageCount=0;	
	
	

	fpstConfigFile = fopen(szFilename,"rt");
	if(fpstConfigFile == NULL)
	{
		printf("Unable to open Configuration File\n");
		exit(1);
	}

	//Read the selected ConfigFile 
	for(ubyLoopCount=0;ubyLoopCount<3;ubyLoopCount++)
	{
		fgets(szFileString,256,fpstConfigFile);
	}
	/* First three lines contain header information that need not be processed */

	//Define Sync Msgs for all Minorframes
	//fnDefineSyncMsg();

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
	
	//***************************		BC		****************************
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
		//CHANGES DONE ON SEP-23-2013 V2.6 R0.0
	}
	fclose(fpstConfigFile);
}

void fnGetMessageName(unsigned char ubyMessageNumber, char * szMessageName)
{
	sscanf(m_rgszMessagesDatabase[ubyMessageNumber],"%s%s",szMessageName,szMessageName);
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
unsigned char fnGetFrequency(unsigned char ubyMessageNumber)
{
//	unsigned char m_ubyMessageNumber;
	char szFreq[25];
	//strcpy(m_rgszMessagesDatabase[m_ubyMessageNumber],szFileString);
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
unsigned int fnHandle1553BCommunication()
{
	//static C1553BSimAndMon *pcl1553BSimAndMon = (C1553BSimAndMon *)lpVoid;
	unsigned short uhwFrameItems[20],BCMODL1=1;
	unsigned short uhwMsgIndex1=0,wMsgIndex2=0,uhwMsgId=0;
	unsigned int	uwMessCount1=0,uwMessCount2=0,uwTxMsgCount=0;
	USHORT data_buffer[32];
	unsigned char ubyWordCount=0;
	static unsigned int uhwBusSilenceCount=0;
	long lEOF =0;
	
	

	//CHANGES DONE ON SEP-23-2013 V2.6 R0.0
	//while(pcl1553BSimAndMon->vstFlags.bSchedulerRunning == TRUE)
	{
		//pcl1553BSimAndMon->vstFlags.bThreadStatus = TRUE;
		
		for (int wMinorFrameIndex = MINOR_CYCLE_ZERO; wMinorFrameIndex <=MINOR_CYCLE_SEVEN; wMinorFrameIndex++)
		{
			SetTMR2Cnt_A(0,CARD1);
			//SetTMR2Cnt_A(BCMODL2,CARD1);
			//SetEndOfFrameCnt_A(BCMODL1,0,CARD1);

			if (wMinorFrameIndex == MINOR_CYCLE_ZERO)
			{
				fnUpdateTimeBroadcastInfo();
			}
#if 0
			//Schedule on Lane1
			if (pcl1553BSimAndMon->m_fBusSilence[BUS1] == TRUE)
			{
				if (uhwBusSilenceCount > pcl1553BSimAndMon->m_wNoofIteration)
				{
					uhwBusSilenceCount =0;
					pcl1553BSimAndMon->m_fBusSilence[BUS1] = FALSE;
					pcl1553BSimAndMon->m_clButtonBusSilence.EnableWindow(TRUE);
				}
				else
				{
					uhwBusSilenceCount++;	
				}
			}

			if (theApp.iLane1 == LANE1 && pcl1553BSimAndMon->m_fBusSilence[BUS1] == FALSE)
#endif
			{
				uhwMsgIndex1=0;
				//uwMessCount1 = pcl1553BSimAndMon->m_MinorFrame[wMinorFrameIndex].Mess_Count;
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
						//for(int i=0; i< ubyWordCount; i++)
						m_stMsg[uhwMsgId].databuffer[i] = 
							[m_stMsg[uhwMsgId].ubyRxMsgCount][i];
					}
					BCRetry_A(BCMODL1,AA,CARD1);
					DefMsg_A(BCMODL1,BCSTACK_A,uhwMsgIndex1,bc_page,m_stMsg[uhwMsgId].databuffer,ubyWordCount ,CARD1);

					uhwFrameItems[wMsgIndex] = uhwMsgIndex1;
					
				}
			
				DefFrame_A(BCMODL1,BCSTACK_A,uwMessCount1,uhwFrameItems,CARD1);
				RunBc_A(BCMODL1,BCSTACK_A,1,uwMessCount1,1,CARD1);

				//Simulate DI, if 'Automated Test' is clicked and DI test is completed (previously if any)
				/*if(vfAutoDITest == TRUE)
				{
					pclDisInpDlg->fnSimulateDI();
				}*/
				
			}
			
#if 0
			//Schedule on Lane2
			if (m_fBusSilence[BUS2] == TRUE)
			{
				if (uhwBusSilenceCount > pcl1553BSimAndMon->m_wNoofIteration)
				{
					uhwBusSilenceCount =0;
					pcl1553BSimAndMon->m_fBusSilence[BUS2] = FALSE;
					pcl1553BSimAndMon->m_clButtonBusSilence.EnableWindow(TRUE);
				}
				else
				{
					uhwBusSilenceCount++;
				}
			}
			if (theApp.iLane2 == LANE2 && pcl1553BSimAndMon->m_fBusSilence[BUS2] == FALSE)
			{
				uhwMsgIndex1=0;
				uwMessCount2 = pcl1553BSimAndMon->m_MinorFrameB2[wMinorFrameIndex].Mess_Count;
 				memset(uhwFrameItems,0,sizeof(uhwFrameItems));
				
				for(int wMsgIndex = 0; wMsgIndex < uwMessCount2; wMsgIndex++)
				{
					uhwMsgId = pcl1553BSimAndMon->m_MinorFrameB2[wMinorFrameIndex].frame_items[wMsgIndex];
					uhwMsgIndex1++;
					bc_page.Int_Msg_Gap   = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.Int_Msg_Gap;
					bc_page.broadcast     = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.broadcast;
					bc_page.bus           = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.bus;	
					bc_page.retry_enable  = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.retry_enable;
					bc_page.cmd_1.status  = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.cmd_1.status;
					bc_page.cmd_1.subaddr = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.cmd_1.subaddr;
					bc_page.cmd_1.t_r     = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.cmd_1.t_r;
					bc_page.cmd_1.taddr   = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.cmd_1.taddr;
					bc_page.cmd_1.wcnt    = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.cmd_1.wcnt;  
					bc_page.loop          = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.loop;
					bc_page.mask_bcst_bit = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.mask_bcst_bit;
					bc_page.mode_code     = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.mode_code;
					bc_page.rt_rt         = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.rt_rt;
					bc_page.self_test     = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.self_test;

					if (wMinorFrameIndex == MINOR_CYCLE_ZERO)
					{
						if((uhwMsgId == TIMEBROADCASTCHNNLBMSGID) || (uhwMsgId == TIMEBROADCASTCHNNLAMSGID))//Update Timebroadcast data for MinorFrame 0
						{
							for(int i=0;i<bc_page.cmd_1.wcnt;i++)
							pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].databuffer[i] = pcl1553BSimAndMon->m_urgswTimeBroadcastData[i];
						}
					}
					if((bc_page.cmd_1.t_r == RECEIVE) && (bc_page.mode_code == NO) && (bc_page.broadcast  == NO))//Update from Receive Msgs
					{
						if(bc_page.cmd_1.wcnt == 0) ubyWordCount = 32;
						else ubyWordCount = bc_page.cmd_1.wcnt;
						for(int i=0; i< ubyWordCount; i++)
						pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].databuffer[i] = pcl1553BSimAndMon->m_pReceiveMessages[BUS2]->m_rgpwMessagePointerB2[pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].ubyRxMsgCount][i];
					}
					BCRetry_A(BCMODL2,AA,CARD1);
					DefMsg_A(BCMODL2,BCSTACK_B,uhwMsgIndex1,pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef,pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].databuffer,ubyWordCount,CARD1);

					uhwFrameItems[wMsgIndex] = uhwMsgIndex1;
				}

				DefFrame_A(BCMODL2,BCSTACK_B,uwMessCount2,uhwFrameItems,CARD1);
				RunBc_A(BCMODL2,BCSTACK_B,1,uwMessCount2,1,CARD1);

			}
#endif
			//Wait for 20ms MinorFrame Timer
			while(GetTMR2Cnt_A(CARD1) < 20);
		//	lEOF = GetEndOfFrameCnt_A(BCMODL1,CARD1);			
#if 0
			//Read from Lane1
			if (theApp.iLane1 == LANE1 && pcl1553BSimAndMon->m_fBusSilence[BUS1] == FALSE /*&& lEOF == 1*/)
			{
				wMsgIndex2 = 1;
				for(int wMsgIndex = 0; wMsgIndex < uwMessCount1;wMsgIndex++)
				{
					uhwMsgId = pcl1553BSimAndMon->m_MinorFrame[wMinorFrameIndex].frame_items[wMsgIndex];
					uwTxMsgCount =pcl1553BSimAndMon->m_stMsg[uhwMsgId].ubyTxMsgCount;
					ReadMsgBC_A(BCMODL1,BCSTACK_B,wMsgIndex2,&pcl1553BSimAndMon->m_stMsg[uhwMsgId].MsgReturn,CARD1);
					wMsgIndex2++;
				
					if((pcl1553BSimAndMon->m_stMsg[uhwMsgId].MsgDef.cmd_1.t_r == TRANSMIT) && (pcl1553BSimAndMon->m_stMsg[uhwMsgId].MsgDef.mode_code == NO) )
					{
						if (pcl1553BSimAndMon->m_stMsg[uhwMsgId].MsgDef.cmd_1.wcnt == 0)
							ubyWordCount = 32;
						else ubyWordCount = pcl1553BSimAndMon->m_stMsg[uhwMsgId].MsgDef.cmd_1.wcnt;
						for(int j=0; j< ubyWordCount; j++)
						{
							pcl1553BSimAndMon->m_pTransmitMessages[BUS1]->m_rgpwMessagePointer[uwTxMsgCount][j]= pcl1553BSimAndMon->m_stMsg[uhwMsgId].MsgReturn.data[j];
						}
					}

					//Automate DITest if 'Automated DI Test' is selected
					if(vfAutoDITest == TRUE)
					{
						memcpy(vstMsg[uhwMsgId].MsgReturn.data,pcl1553BSimAndMon->m_stMsg[uhwMsgId].MsgReturn.data,sizeof(Msg));
						pclDisInpDlg->fnAutomatedDITest(uhwMsgId);
					}
				}
				/*Log the data in ATP, Normal mode till Msg Log is ON*/
				if (theApp.iModeOfOperation != ESS_MODE && pcl1553BSimAndMon->m_wMsgLogOnLogOffRadio != 1)
				{
					pcl1553BSimAndMon->fnGetLogData(wMinorFrameIndex);
				}
			}

			//Read from Lane2
			if (theApp.iLane2 == LANE2 && pcl1553BSimAndMon->m_fBusSilence[BUS2] == FALSE)
			{
				wMsgIndex2 = 1;
				for(int wMsgIndex = 0; wMsgIndex < uwMessCount2;wMsgIndex++)
				{
					uhwMsgId = pcl1553BSimAndMon->m_MinorFrameB2[wMinorFrameIndex].frame_items[wMsgIndex];
					uwTxMsgCount =pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].ubyTxMsgCount;
					ReadMsgBC_A(BCMODL2,BCSTACK_B,wMsgIndex2,&pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgReturn,CARD1);
					wMsgIndex2++;
						
					if((pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.cmd_1.t_r == TRANSMIT) && (pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.mode_code == NO) )
					{
						if (pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.cmd_1.wcnt == 0)
							ubyWordCount = 32;
						else ubyWordCount = pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgDef.cmd_1.wcnt;
						for(int j=0; j< ubyWordCount; j++)
						{
							pcl1553BSimAndMon->m_pTransmitMessages[BUS2]->m_rgpwMessagePointerB2[uwTxMsgCount][j]= pcl1553BSimAndMon->m_stMsgB2[uhwMsgId].MsgReturn.data[j];
						}
					}
				}
				/*Log the data in ATP, Normal mode till Msg Log is ON*/
				if (theApp.iModeOfOperation != ESS_MODE && pcl1553BSimAndMon->m_wMsgLogOnLogOffRadio != 1)
				{
					pcl1553BSimAndMon->fnGetLogDataB2(wMinorFrameIndex);
				}
			}
#endif
		}
		/*if (theApp.iModeOfOperation == ESS_MODE)
		{
			if(vubyInitFrame<10)
			{
				vubyInitFrame++;
			}
			else if(vubyInitFrame==10)
			{
					pcl1553BSimAndMon->OnTimer(CHANGE_FRAME);
			}
		}
	
		pcl1553BSimAndMon->OnTimer(LOG_1553_BUS_DATA);
		pcl1553BSimAndMon->vstFlags.bThreadStatus = FALSE;*/
		
	}
	printf("Interrupts:%d\n",GetBCEOMCnt_A(1,CARD1));
	ResetBc_A(1,CARD1);	
	ResetMt_A(1,CARD1);
	printf("\n\t**************  OVER   *******************");
	DisableEnhancedInterrupts_A(1,INTR_BC_ENDOF_MINFRAME,CARD1);	
	CloseDeviceHandle_A(CARD1);

//	ubyInitFrame = 0;
//
//#ifdef CARD_PRESENT
//		if(theApp.iLane1 == LANE1)
//		{
//			configure(CHIP_ONE,BASE_ADDRESS,IRQLINE);
//			COMMAND_BLOCK CmdBlock_Image;
//
//
//			readmessage(CHIP_ONE,pcl1553BSimAndMon->m_uswCommandArea-8,1,&CmdBlock_Image);
//			
//			Execute_CmdBlk_And_Branch(CHIP_ONE,pcl1553BSimAndMon->m_uswCommandArea,0x100);
//		}
//		if(theApp.iLane2 == LANE2)
//		{
//			configure(CHIP_TWO,BASE_ADDRESS2,IRQLINE);
//
//			Execute_CmdBlk_And_Branch(CHIP_TWO,pcl1553BSimAndMon->m_uswCommandAreaB2,0x100);
//		}
//#endif
//	while(pcl1553BSimAndMon->vstFlags.bSchedulerRunning == TRUE)
//	{
//		if( (theApp.iLane1==LANE1) && (theApp.iLane2==LANE2) )
//		{
//			configure(CHIP_ONE,BASE_ADDRESS,IRQLINE);
//			uhwAddress = getregister(CHIP_ONE,CMDBLKPTREG);
//			uwLane_2FrameAdrs = uwLane1_2FrameAdrs;
//			uwLane_LastFrameAdrs = uwLane1_LastFrameAdrs;
//		}
//		else if( (theApp.iLane1==LANE1) && (theApp.iLane2!=LANE2) )
//		{
//			configure(CHIP_ONE,BASE_ADDRESS,IRQLINE);
//			uhwAddress = getregister(CHIP_ONE,CMDBLKPTREG);
//			uwLane_2FrameAdrs = uwLane1_2FrameAdrs;
//			uwLane_LastFrameAdrs = uwLane1_LastFrameAdrs;
//		}
//		else if( (theApp.iLane1!=LANE1) && (theApp.iLane2==LANE2) )
//		{
//			configure(CHIP_TWO,BASE_ADDRESS2,IRQLINE);
//			uhwAddress = getregister(CHIP_TWO,CMDBLKPTREG);
//			uwLane_2FrameAdrs = uwLane2_2FrameAdrs;
//			uwLane_LastFrameAdrs = uwLane2_LastFrameAdrs;
//		}
//		else
//		{
//			AfxMessageBox(" Invalid LANE mode ");
//			pcl1553BSimAndMon->OnButtonStopSched();
//			exit(0);
//
//		}
//
//		switch(en1553BState)
//		{
//			case WAIT_FOR_SECOND_MINOR_FRAME_SCHEDULED:
//				{
//					if(uhwAddress >= uwLane_2FrameAdrs)
//					{
//						en1553BState = WAIT_FOR_LAST_MINOR_FRAME_SCHEDULED;
//
//					}
//				}
//				break;
//			case WAIT_FOR_LAST_MINOR_FRAME_SCHEDULED:
//				{
//					if(uhwAddress >= uwLane_LastFrameAdrs)
//					{
//
//						/*if(pcl1553BSimAndMon->m_uswMinuteCounter == 1)
//						{
//							if(theApp.iLane1 == LANE1)
//							{
//								pcl1553BSimAndMon->m_pReceiveMessages->m_rgpwMessagePointer[0][0] &= 0xFE3F;
//								configure(CHIP_ONE,BASE_ADDRESS,IRQLINE);
//#ifdef CARD_PRESENT
//								putdatainmessage(CHIP_ONE,0x100,0x1C,pcl1553BSimAndMon->m_pReceiveMessages->m_rgpwMessagePointer[0]);
//#endif
//							}
//
//							if(theApp.iLane2 == LANE2)
//							{
//
//								pcl1553BSimAndMon->m_pReceiveMessages->m_rgpwMessagePointerB2[0][0] &= 0xFE3F;
//								configure(CHIP_ONE,BASE_ADDRESS,IRQLINE);
//#ifdef CARD_PRESENT
//								putdatainmessage(CHIP_TWO,0x100,0x1C,pcl1553BSimAndMon->m_pReceiveMessages->m_rgpwMessagePointerB2[0]);
//#endif
//							}
//
//						}*/
//						if(pcl1553BSimAndMon->m_ubyfLogInitData==true)
//						{
//							ubyInitFrame++;
//							if(ubyInitFrame==10)
//							{
//								pcl1553BSimAndMon->OnTimer(CHANGE_FRAME);
//							}
//							else
//							{
//								pcl1553BSimAndMon->OnTimer(LOG_1553_BUS_DATA);
//							}
//						}
//						else
//						{
//							pcl1553BSimAndMon->OnTimer(LOG_1553_BUS_DATA);
//						}
//						en1553BState = WAIT_FOR_FIRST_MINOR_FRAME_SCHEDULED;
//
//					}
//				}
//				break;
//			case WAIT_FOR_FIRST_MINOR_FRAME_SCHEDULED:
//				{
//					if(uhwAddress <= uwLane_2FrameAdrs)
//					{
//						en1553BState = WAIT_FOR_SECOND_MINOR_FRAME_SCHEDULED;
//					}
//				}
//				break;
//			default:
//				{
//					AfxMessageBox("Invalid State");
//					pcl1553BSimAndMon->OnButtonStopSched();
//					exit(0);
//				}
//				break;
//		}
//	}
//CHANGES DONE ON SEP-23-2013 V2.6 R0.0
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

	/*theApp.urgswTimeBroadcastData[0] = m_urgswTimeBroadcastData[0];
	theApp.urgswTimeBroadcastData[1] = m_urgswTimeBroadcastData[1];*/
}