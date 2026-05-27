				    /*DOUBLE NODE PROGRAM*/

#include <stdio.h>

//#include "../Include/Include.h"
//#include "/home/brahmos/Apoorva31Rts-4NodeDll/Include/Libraries.h"
#include "/home/acl/APOORVA_CCPMC/Apoorva31Rts-4NodeDll/Include/Libraries.h"
MSG_DEF bc_page;
RT_MSG RtBuff;
MSG_RESULT msg_return;
unsigned short frame_items[10],data_buffer[40],temp_data;
int ind;

/*******************************************************************
*  This Program gives code for Bc to Rt Receive On Bus A
********************************************************************/
int main()
{
	int i,TempCnt,BcMod=1,MtMod=1,RtMod=1,MemAdr=0;
	char temp;
	char Address[32];

	unsigned short data_buffer[40],data_buff[40];

	MNTPACK buffer;
	i = InitCard(1);
	if(i)
	  printf("Card Init Success %d \n",i);
	else
	  printf("Card Init Failed %d \n",i);
/*	getchar();
	for(MemAdr=0x0;MemAdr<=0x1fff;MemAdr++)
	  MemWrite(1,MemAdr,MemAdr,1);	
	for(MemAdr=0x0;MemAdr<=0x1fff;MemAdr++)
	  printf("MemRead (0x1)%x \n",MemRead(1,MemAdr,1));
	//printf("\nMemRead (0x1)%x \n\n\n",MemRead(1,0x2,1));
	//MemWrite(1,2,0x1553,1);
getchar();
exit(1);*/
getchar();
	printf("\n\n\nMemWrite completed \n\n\n");
		
		//printf("\n\n\nMemRead (0x2)%x \n\n\n",MemRead(1,0x2,1));
		
	//InitCard(1);
	//SoftwareReset();
	printf("Card Initialized..\n");
	
	InitInterrupts(BcMod,INTR_BCEOM,1);
	InitInterrupts(MtMod,INTR_MTEOM,1);
	InitInterrupts(RtMod,INTR_RTEOM,1);

	SetBCEOMCnt(BcMod,0,1);
	SetMTEOMCnt(MtMod,0,1);
	SetRTEOMCnt(1,1,0,1);

	for(i=0;i<31;i++)
	{
		Address[i]=0xff;
		ResetRt(RtMod,i,1);
	}

	ClearRam(1,RT1,1);
	Address[RT1]=RT1;
	ConfigureRT(1,0,Address,1);
	
	for(i=0;i<=31;i++)
		data_buff[i]=0x5555; 		
	WriteDataBlock(1,RT1,1,data_buff,1); 
	SetRTEOMCnt(1,1,0,1);
	RunRt(1,0,Address,1);


	ResetMt(MtMod,1);
	ClearRam(MtMod,MT,1);
	SelMsgMt(MtMod,MTSTACK_A,1);
	SetMTEOMCnt(1,0,1);
	RunMt(MtMod,1);

	ClearRam(BcMod,BC,1);
	ConfigureBC(BcMod,128,1);
	

	bc_page.bus           = CH_A;
	bc_page.self_test     = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 1;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 32;
	
	for(i=0; i<32; i++)
		data_buffer[i] = 0x1234;

	DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,32,1);

	bc_page.int_msg_gap   = 10;
	bc_page.cmd_1.taddr   = 1;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 32;
	
	for(i=0; i<32; i++)
		data_buffer[i] = 0x1234;

	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,32,1);

	frame_items[0] = 1;
	frame_items[1] = 2;
	
	DefFrame(BcMod,BCSTACK_A,2,frame_items,1);
	SetBCEOMCnt(1,0,1);
	
	RunBc(BcMod,BCSTACK_A,1,2,1,1);
	printf("Press any key to see results \n");
	getchar();
	printf("BC EOM Interr %d\n",GetBCEOMCnt(1,1));
	printf("MT EOM Interr %d\n",GetMTEOMCnt(1,1));

int k=0;
	
for(k=1;k<3;k++)
{
    	ReadMsgBC(BcMod,BCSTACK_A,k,&msg_return,1);	
	printf("\n***************BC Results *************\n");
	printf("\n TIME TAG REGISTER:%x \n",msg_return.time_tag);
	printf("\n COMMAND WORD IS %x \n",msg_return.cmd_1);
	printf("THE  MESSAGE WORD COUNT = %x \n",msg_return.cmd1_bits.wcnt);
	printf("THE  MESSAGE SUBADDRESS = %x \n",msg_return.cmd1_bits.subaddr);
	printf("THE T/R BIT OF MSG#1 IS SET TO %x \n",msg_return.cmd1_bits.t_r);
	printf("THE  MESSAGE TADDRESS IS= %x \n",msg_return.cmd1_bits.taddr);
	printf("\n THE STATUS BITS OF THE MESSAGE ARE AS FOLLLOWS \n");
	printf("THE BLOCK STATUS WORD IS %x \n",msg_return.bsw);
	printf("\nLOOP TEST FAIL     = %x\n",msg_return.bsw_bits.loop_tstfail);
	printf("RESP TIME OUT      = %x \n",msg_return.bsw_bits.resp_timeout);
	printf("MESSAGE FORMAT     = %x \n",msg_return.bsw_bits.msg_fmt_err);
	printf("STATUS WORD ERROR  = %x \n",msg_return.bsw_bits.stat_wrd_err);
	printf("ERROR FLAG         = %x \n",msg_return.bsw_bits.err_flag);
	printf("BUS                = %x \n",msg_return.bsw_bits.bus);
	printf("START OF MESSAGE   = %x \n",msg_return.bsw_bits.som);
	printf("END OF MESSAGE     = %x \n",msg_return.bsw_bits.eom);
	printf("\nDATA RECEIVED \n");

	TempCnt= msg_return.cmd1_bits.wcnt;

	if(TempCnt == 0)
		TempCnt = 32;

	for(i=0;i<TempCnt;i++)
	{
		if((i==8)||(i==16)||(i==24))
		printf("\n");
		printf("%x ",msg_return.data[i]);
	}
	
	printf("\nLOOPBACK WORD      = %x \n",msg_return.loop_word);
	printf("STATUS             = %x \n",msg_return.status_1);
	getchar();
//int k=0;
	
//for(k=1;k<3;k++)
//{	
	ReadMsgRT(RtMod,k,1,&RtBuff,1);
	printf("\n***************RT RESULTS************\n");
	printf("\nbsw = %x",RtBuff.bsw);
	printf("\nTime Tag = %x",RtBuff.time_tag);
	printf("\ncmd = %x",RtBuff.cmd);
	printf("\nwcnt = %x",RtBuff.wcnt);
	printf("\nsubaddr = %x",RtBuff.subaddr);
	printf("\nt_r = %x",RtBuff.t_r);
	printf("\nt_r = %x\n\n",RtBuff.t_r);
	TempCnt = RtBuff.wcnt;

	if(TempCnt == 0)
		TempCnt = 32;

	for(i =0; i < TempCnt; i++)
	{
		if((i==8)||(i==16)||(i==24))
			printf("\n");

		printf("%x ",RtBuff.data[i]);
	}
	
	getchar();		
		
	ReadMonPacket(MtMod,MTSTACK_A,k,&buffer,1);
	printf("\n***************  MT Results *************\n");
	printf("\nBlock Status Word -->> 	%x",buffer.BlockSts.value);
	printf("\nCMD WORD CONTENTS ERR		=	%d",buffer.BlockSts.bitwise.CmdWrdErr);
	printf("\nRT-RT 2ND CMD ERR		=	%d",buffer.BlockSts.bitwise.RtRtSecCmd );
	printf("\nINVALID WORD			=	%d",buffer.BlockSts.bitwise.InVldWrd );
	printf("\nINCORRECT SYNC			=%d",buffer.BlockSts.bitwise.InCrtSyc );
	printf("\nWORD COUNT ERROR		=	%d",buffer.BlockSts.bitwise.WrdCntErr );
	printf("\nRESERVED			=	%d",buffer.BlockSts.bitwise.reserved );
	printf("\nDATA STACK ROLLOVER		=	%d",buffer.BlockSts.bitwise.DataStkRoll );
	printf("\nGOOD DATA BLOCK XFER		=	%d",buffer.BlockSts.bitwise.DataBlkXfer );
	printf("\nNO RESPONSE TIMEOUT		=	%d",buffer.BlockSts.bitwise.RespTimOUt );
	printf("\nFORMAT ERROR             	=	%d",buffer.BlockSts.bitwise.FrmtErr );
	printf("\nRT-RT TRANSFER			=	%d",buffer.BlockSts.bitwise.RtRt );
	printf("\nERROR FLAG               	=	%d",buffer.BlockSts.bitwise.ErrFlg );
	printf("\nCHANNEL A/B              	=	%d",buffer.BlockSts.bitwise.ChAb );
	printf("\nSOM   data             		=	%d",buffer.BlockSts.bitwise.Som );
	printf("\nEOM                		=	%d\n",buffer.BlockSts.bitwise.Eom);
		getchar();
	temp=buffer.RecCmdWrd.bitwise.WrdCntOrMde;
	if (temp==0) temp=32;
    
	printf("\nCmd Word  -->>  %x \n",buffer.RecCmdWrd.value);
	printf("\nRT Status Word  -->>  %x \n",buffer.RecStatus);	

	for(i=0;i<temp;i++)
		data_buffer[i]=buffer.DataPack[i];
	
	for(i=0;i<32;i++)
	{
		
		if((i==8)||(i==16)||(i==24))
		printf("\n");
		printf("%x ",data_buffer[i]); 
	}

}
		
	getchar();
	
	DisableInterrupts(BcMod,1);		
	DisableInterrupts(RtMod,1);	
	DisableInterrupts(MtMod,1);

	ResetMt(MtMod,1);
	
	for(i=0;i<31;i++)
		ResetRt(RtMod,i,1);
	
	ResetBc(BcMod,1);		


	printf("\n***OVER*******");
 	getchar();				
	
	CloseDeviceHandle(1);	getchar();
	return 0;
}  
