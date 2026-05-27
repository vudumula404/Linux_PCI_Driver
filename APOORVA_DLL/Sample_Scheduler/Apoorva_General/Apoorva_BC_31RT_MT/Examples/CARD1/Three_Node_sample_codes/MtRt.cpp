
#include <windows.h>
#include <stdio.h>
#include <winioctl.h>
#include <conio.h>
#include "C:\Apoorva_General\Include\Libraries.h"

void main()
{
	int i,TempCnt,temp;
	unsigned short data_buffer[32],RtNo=0,msgno=1;
	RT_MSG RtBuff;
	MNTPACK buffer;
	
	printf("\n\t");
	InitCard(CARD1);

	printf("Enter the RT number  --->>  ");
	scanf("%d",&RtNo);
    
	//*************************** RT ****************************
 	ClearRam(2,MT,CARD1);
	SelMsgMt(2,MTSTACK_A,CARD1);
	RunMt(2,CARD1);


	ClearRam(1,RtNo,CARD1);
	ConfigureRT(1,RtNo,CARD1);
	for(i=0;i<=31;i++)
		data_buffer[i]=0xaeae; 		
	WriteDataBlock(1,RTSTACK_A,1,data_buffer,CARD1); 
	RunRt(1,RTSTACK_A,CARD1);
	
	printf("\n\nRT AND MT Waiting for BC Command.....\n\n");
	getchar();
	getchar();
	printf("Press any key to see the RT Results\n");
	ReadMsgRT(1,RTSTACK_A,1,&RtBuff,CARD1);     
	
	printf("\nbsw = %x",RtBuff.bsw);
	printf("\nTime Tag = %x",RtBuff.time_tag);
	printf("\ncmd = %x",RtBuff.cmd);
	printf("\nwcnt = %x",RtBuff.wcnt);
	printf("\nsubaddr = %x",RtBuff.subaddr);
	printf("\nt_r = %x\n",RtBuff.t_r);
	
	TempCnt = RtBuff.wcnt;
	if(TempCnt == 0)
		TempCnt = 32;
	for(i =0; i < TempCnt; i++)
		printf("%x ",RtBuff.data[i]);
	getchar();
		
	ReadMonPacket(2,MTSTACK_A,1,&buffer,CARD1);
	
	printf("\nCMD WORD CONTENTS ERR		=	%d",buffer.BlockSts.bitwise.CmdWrdErr);
	printf("\nRT-RT 2ND CMD ERR		=	%d",buffer.BlockSts.bitwise.RtRtSecCmd );
	printf("\nRT-RT Status Word RTAddr ERR	=	%d",buffer.BlockSts.bitwise.RtRtStatusAdd_err );
	printf("\nINVALID WORD			=	%d",buffer.BlockSts.bitwise.InVldWrd );
	printf("\nINCORRECT SYNC			=	%d",buffer.BlockSts.bitwise.InCrtSyc );
	printf("\nWORD COUNT ERROR		=	%d",buffer.BlockSts.bitwise.WrdCntErr );
	printf("\nRESERVED			=	%d",buffer.BlockSts.bitwise.reserved );
	printf("\nDATA STACK ROLLOVER		=	%d",buffer.BlockSts.bitwise.DataStkRoll );
	printf("\nGOOD DATA BLOCK XFER		=	%d",buffer.BlockSts.bitwise.DataBlkXfer );
	printf("\nNO RESPONSE TIMEOUT		=	%d",buffer.BlockSts.bitwise.RespTimOUt );
	printf("\nFORMAT ERROR             	=	%d",buffer.BlockSts.bitwise.FrmtErr );
	printf("\nRT-RT TRANSFER			=	%d",buffer.BlockSts.bitwise.RtRt );
	printf("\nERROR FLAG               	=	%d",buffer.BlockSts.bitwise.ErrFlg );
	printf("\nCHANNEL A/B              	=	%d",buffer.BlockSts.bitwise.ChAb );
	printf("\nSOM                		=	%d",buffer.BlockSts.bitwise.Som );
	printf("\nEOM                		=	%d\n",buffer.BlockSts.bitwise.Eom);
	getchar();
	temp=buffer.RecCmdWrd.bitwise.WrdCntOrMde;
	if (temp==0)
		temp=32;
    printf("\n%x \n",buffer.RecCmdWrd.value);
    for(i=0;i<temp;i++)
		printf("%x ",buffer.DataPack[i]); 
	printf("\n\t**************  OVER   *******************");
	ResetRt(1,CARD1);
	ResetMt(2,CARD1);
	CloseDeviceHandle(CARD1);
	getchar();	
		
 } //close for main


