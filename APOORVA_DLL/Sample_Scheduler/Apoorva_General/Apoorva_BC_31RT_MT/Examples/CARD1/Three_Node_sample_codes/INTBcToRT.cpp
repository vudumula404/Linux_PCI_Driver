#include <windows.h>
#include <stdio.h>
#include <winioctl.h>
#include <conio.h>
#include "C:\Apoorva_General\Include\Libraries.h"

MSG_DEF bc_page;

MSG_RESULT msg_return;
unsigned short frame_items[10];

/*******************************************************************
  This Program gives code for BC and RT 
	
********************************************************************/
void main()
{
	int i,TempCnt;
	USHORT data_buffer[32],RtNo=0,Ch=0; 
	RT_MSG RtBuff;
	UCHAR Address[32];

	printf("\n\t");
	InitCard(CARD1);

	InitInterrupts(1,INTR_RTEOM,CARD1);
	InitInterrupts(1,INTR_BCEOM,CARD1);
	

	printf("\n\nEnter the RT Number   ( 0 - 30)-->>  ");
	scanf("%d",&RtNo);
	printf("\n\nEnter the CHANNEL   (  Ch-A -> 1 / Ch-B -> 0)-->>  ");
	scanf("%d",&Ch);
	//***************************		BC		****************************
	Address[RtNo] = RtNo;

	ClearRam(1,RtNo,CARD1);
	ConfigureRT(1,RtNo,Address,CARD1);
	for(i=0;i<=31;i++)
		data_buffer[i]=0xBABA; 		
	WriteDataBlock(1,RtNo,1,data_buffer,CARD1); 
	RunRt(1,RtNo,Address,CARD1);
	printf("\nMODULE 1 is Configured as RT");
	

	
	ClearRam(1,BC,CARD1);
	ConfigureBC(1,128,CARD1);
	printf("\nMODULE 2 is Configured as BC");
	
	bc_page.bus           = 1;
	bc_page.self_test     = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 1;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x1001;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 32;  

	for(i=0; i<32; i++)
		data_buffer[i] = 0x1234;
	DefMsg(1,BCSTACK_A,1,bc_page,data_buffer,32,CARD1);


	frame_items[0] = 1;
	DefFrame(1,BCSTACK_A,1,frame_items,CARD1);
	
	
	RunBc(1,BCSTACK_A,1,1,1,CARD1);	
	getchar();

	//while(GetBCEOMCnt(2,CARD1)!=1);
		
	ReadMsgBC(1,BCSTACK_A,1,&msg_return,CARD1);
	
	printf("\nRTEOMCount=%d\n",GetRTEOMCnt(BUS_1,RtNo,CARD1));	
	printf("\nBCEOMCount=%d\n",GetBCEOMCnt(BUS_2,CARD1));	
	getchar();
	printf("\nPress any key to see BC Results");
	getchar();
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
		printf("%x ",msg_return.data[i]);
	
	printf("\nLOOPBACK WORD      = %x \n",msg_return.loop_word);
	printf("\nSTATUS             = %x \n",msg_return.status_1);
	getchar();
	getchar();
	
	printf("\n\n Message Result from RT  Module");
	ReadMsgRT(1,RTSTACK_A,1,&RtBuff,CARD1);        
	printf("\nbsw = %x",RtBuff.bsw);
	printf("\ncmd = %x",RtBuff.cmd);
	printf("\nwcnt = %x",RtBuff.wcnt);
	printf("\nsubaddr = %x",RtBuff.subaddr);
	printf("\nt_r = %x",RtBuff.t_r);
	printf("\nRt data\n");
	TempCnt = RtBuff.wcnt;
	if(TempCnt == 0)
		TempCnt = 32;
	for(i =0; i < TempCnt; i++)
		printf("%x ",RtBuff.data[i]);
	printf("\n\t**************  OVER   *******************");
	ResetRt(1,CARD1);
	DisableInterrupts(1,CARD1);
	DisableInterrupts(1,CARD1);	    
	CloseDeviceHandle(CARD1);

 }  
