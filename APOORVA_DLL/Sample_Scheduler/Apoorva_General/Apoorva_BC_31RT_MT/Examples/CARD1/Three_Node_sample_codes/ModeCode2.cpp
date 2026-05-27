#include <windows.h>
#include <stdio.h>
#include <winioctl.h>
#include <conio.h>
#include "C:\Apoorva_General\Include\Libraries.h"

MSG_DEF bc_page;
MSG_RESULT msg_return;
unsigned short frame_items[10];
/*******************************************************************
  Transmit Last Status Word Mode Code 
	
********************************************************************/
void main()
{
	int i,TempCnt,Ret=1;
	USHORT data_buffer[32]; 
	RT_MSG RtBuff;
	UCHAR Address[32];
	
	InitCard(CARD1);
	SoftwareReset();
	//***************************		RT		****************************

	Address[RT1] = RT1;
	ClearRam(BUS_3,RT1,CARD1);
	ConfigureRT(BUS_3,RT1,Address,CARD1);
	RunRt(BUS_3,RT1,Address,CARD1);


	//***************************		BC		****************************
	
	ClearRam(BUS_3,BC,CARD1);
	ConfigureBC(BUS_3,128,CARD1);
	
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
		data_buffer[i] = 0x1000;
	DefMsg(BUS_3,BCSTACK_A,1,bc_page,data_buffer,32,CARD1);

	bc_page.bus           = CH_A; 
	bc_page.self_test     = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = YES;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 1;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 32;
	bc_page.cmd_1.wcnt    = 2;  

	for(i=0; i<32; i++)
		data_buffer[i] = 0x0;
	DefMsg(BUS_3,BCSTACK_A,2,bc_page,data_buffer,2,CARD1);

	
	frame_items[0] = 1;
	frame_items[1] = 2;
	DefFrame(BUS_3,BCSTACK_A,2,frame_items,CARD1);
	
	RunBc(BUS_3,BCSTACK_A,1,2,1,CARD1);			/*issue start Command*/

	printf("\nPress any key to see BC Results");
	getchar();

	ReadMsgBC(BUS_3,BCSTACK_A,1,&msg_return,CARD1);
	
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
	
	ReadMsgBC(BUS_3,BCSTACK_A,2,&msg_return,CARD1);
	
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
	
	TempCnt= msg_return.cmd1_bits.wcnt;
		
	if(TempCnt == 0)
		TempCnt = 32;

	for(i=0;i<1;i++)
		printf("%x ",msg_return.data[i]);

	printf("\nLOOPBACK WORD			-->>	%x \n",msg_return.loop_word);
	printf("\nLast STATUS Word		-->>	%x \n",msg_return.status_1);
	getchar();
	/*
	printf("\n\n Message Result from RT  Module");
	
	ReadMsgRT(BUS_3,RTSTACK_A,1,&RtBuff,CARD1);   
	printf("\n******\tTime Tag = %x\t******\n",RtBuff.time_tag);
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
	*/
	getchar();
	printf("\n\t**************  OVER   *******************\n");
	ResetBc(BUS_3,CARD1);
	ResetRt(BUS_3,CARD1);
	CloseDeviceHandle(CARD1);

 }  
