#include <windows.h>
#include <stdio.h>
#include <winioctl.h>
#include <conio.h>
#include <ctype.h>
#include "C:\Apoorva_General\Include\Libraries.h"

MSG_DEF bc_page;
MSG_RESULT msg_return;
MAJOR_FRAME Min_Cyc[128];
unsigned short frame_items[10],data_buffer[40],temp_data;
unsigned short rtmodid,Temp_Buff[40];
int ind;
ULONG int1=0,int2=0;


/*******************************************************************

	
********************************************************************/

void main()
{
	int i,TempCnt,temp,ch=1,k,j,Ch=CH_A;

		ULONG m,Counter=0;
		unsigned short data_buff[32];	
			
		UCHAR Address[31],CardId=1,RtMod=1,BcMod=1,MtMod=1;

		RT_MSG RtBuff;
		MNTPACK buffer;
		
		for(i=0;i<31;i++)
			Address[i]=0xff;
	
		InitCard(CardId);
		SoftwareReset(CardId);
		Sleep(500);
	
		RtMod=1;BcMod=1;MtMod=1;
		InitInterrupts(BcMod, INTR_BCEOM|INTR_TMR2,CardId);
		SetBCEOMCnt(BcMod,0,CARD1);

		RtMod=2;BcMod=2;MtMod=2;
		InitInterrupts(BcMod, INTR_BCEOM,CardId);
		SetBCEOMCnt(BcMod,0,CARD1);

		RtMod=4;BcMod=4;MtMod=4;
		InitInterrupts(BcMod, INTR_BCEOM,CardId);
		SetBCEOMCnt(BcMod,0,CARD1);

		RtMod=1;BcMod=1;MtMod=1;

//		SelectTMR (0, MS_4, 0, CARD1); // Selects Timer Interval
		SelectTMR (0, MS_1, 0, CARD1); // Selects Timer Interval
		SetTMR2Cnt(0,1);
//		InitEnhancedInterrupts(BcMod,INTR_BC_ENDOF_MINFRAME,CardId);
		InitTimerIntr(INTR_TMR2, CARD1); // Initialises Timer 2 Interrupts
	/*	ClearRam(1,MT,CARD1);
		SetMTEOMCnt(1,0,CARD1);
		printf("IntCnt---->>%d",GetMTEOMCnt(1,CARD1));
		SelMsgMt(1,MTSTACK_A,CARD1);
		RunMt(1,1);*/
		Counter=1;

		
	RtMod=1;BcMod=1;MtMod=1;
	ClearRam(BcMod,BC,CardId);
	//ConfigureBC(BcMod,128,CardId);
		RtMod=2;BcMod=2;MtMod=2;
	ClearRam(BcMod,BC,CardId);
//	ConfigureBC(BcMod,128,CardId);
		RtMod=3;BcMod=3;MtMod=3;
	ClearRam(BcMod,BC,CardId);
//	ConfigureBC(BcMod,128,CardId);
for(;;)
{
			

//rintf("press any char to stop %d  %d\n",Counter,GetBCEOMCnt(BcMod,CARD1));
	if(kbhit())
		{
			ch = getch();
			if(ch == 27)
			break;
		}
	
	SetTMR2Cnt(0,1);
//ClearRam(BcMod,BC,CardId);
//ConfigureBC(BcMod,128,CardId);
if(Counter==1)          /*Minor Cycle 0*/
{	
	RtMod=1;BcMod=1;MtMod=1;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<17; i++)
			data_buffer[i] = 0x0000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);
//RT_ADD 31 SUB_ADD 29 WORD_CT 2

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 29;
		bc_page.cmd_1.wcnt    = 2;

/*		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 29;
		bc_page.cmd_1.wcnt    = 2;*/


	
		for(i=0; i<2; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,2,CardId);

//RT_ADD 21 SUB_ADD 1 WORD_CT 14
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;

		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<14; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,14,CardId);
//RT_ADD 22 SUB_ADD 1 WORD_CT 14
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<14; i++)
		data_buffer[i] = i;
		
	
		DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,14,CardId);
//RT_ADD 12 SUB_ADD 1 WORD_CT 2
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.cmd_1.taddr   = 12;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x4000;
	
		DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,2,CardId);

///RT_ADD 11 SUB_ADD 1 WORD_CT 14
		bc_page.cmd_1.taddr   = 11;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x5000;
	
		DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,14,CardId);
//RT_ADD 13 SUB_ADD 1 WORD_CT 15
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 15;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x6000;

		DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,15,CardId);
	//RT_ADD 16 SUB_ADD 1 WORD_CT 15
		bc_page.cmd_1.taddr   = 16;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 15;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x7000;
	
		DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,15,CardId);
//RT_ADD 21 SUB_ADD 1 WORD_CT 13
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 13;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x8000;
	
		DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,13,CardId);
//RT_ADD 27 SUB_ADD 2 WORD_CT 32

		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
	data_buffer[i] = i;


	
		DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,32,CardId);
		//RT_ADD 22 SUB_ADD 1 WORD_CT 13
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 13;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x1200;
	
		DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,13,CardId);

		//RT_ADD 16 SUB_ADD 3 WORD_CT 1
		bc_page.cmd_1.taddr   = 16;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 1;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x1100;
	
		DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,1,CardId);
		//RT_ADD 14 SUB_ADD 3 WORD_CT 32
		bc_page.cmd_1.taddr   = 14;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;

		DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,32,CardId);
	
		//RT_ADD 15 SUB_ADD 3 WORD_CT 12
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 12;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x1300;
	
		DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,12,CardId);

		//RT_ADD 13 SUB_ADD 3 WORD_CT 32
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x1400;
	
		DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CardId);
		//RT_ADD 24 SUB_ADD 1 WORD_CT 32
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x1500;
	
		DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,32,CardId);


	//	BC-->RT
//RT_ADD 2 SUB_ADD 4 WORD_CT 32
		
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
			

	
		DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,32,CardId);
//RT-->BC
//RT_ADD 16 SUB_ADD 2 WORD_CT 6
		bc_page.cmd_1.taddr   = 16;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 6;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x1700;
	
		DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,6,CardId);
	//	RT-->BC
//RT_ADD 27 SUB_ADD 6 WORD_CT 6
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 6;
		bc_page.cmd_1.wcnt    = 6;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x1800;
	
		DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,6,CardId);

		//RT-->RT
//RT_ADD 2 SUB_ADD 8 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 8;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x1900;
	
		DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 9 SUB_ADD 20 WORD_CT 32
		bc_page.cmd_1.taddr   = 9;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 20;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 13 SUB_ADD 9 WORD_CT 4
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 9;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x2100;
	
		DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,4,CardId);

		//BC-->RT
//RT_ADD 21 SUB_ADD 2 WORD_CT 18
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 18;
		
		for(i=0; i<19; i++)
		data_buffer[i] = i;
		
			
	
		DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,18,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 2 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
	data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,32,CardId);


		

	
	//	BC-->RT
//RT_ADD 2 SUB_ADD 3 WORD_CT 32
		
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,32,CardId);
//RT-->BC
//RT_ADD 17 SUB_ADD 25 WORD_CT 5
		bc_page.cmd_1.taddr   = 17;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 25;
		bc_page.cmd_1.wcnt    = 5;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x2500;
	
		DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,32,CardId);
	//	RT-->BC
//RT_ADD 18 SUB_ADD 7 WORD_CT 6
		bc_page.cmd_1.taddr   = 18;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 7;
		bc_page.cmd_1.wcnt    = 6;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x2600;
	
		DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,32,CardId);

		//BC-->RT
//RT_ADD 2 SUB_ADD 20 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 20;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,28,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 24 SUB_ADD 4 WORD_CT 1
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 1;
		
		for(i=0; i<1; i++)
			data_buffer[0] = 0x0000;
	
		DefMsg(BcMod,BCSTACK_A,29,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 27 SUB_ADD 5 WORD_CT 2
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 5;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x2900;
	
		DefMsg(BcMod,BCSTACK_A,30,bc_page,data_buffer,32,CardId);///HERE ENDED FOR 29TH MSG WENT FOR LUNCH

		//RT-->BC
//RT_ADD 17 SUB_ADD 11 WORD_CT 4
		bc_page.cmd_1.taddr   = 17;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x3000;
	
		DefMsg(BcMod,BCSTACK_A,31,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 18 SUB_ADD 11 WORD_CT 4
		bc_page.cmd_1.taddr   = 18;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x3100;
	
		DefMsg(BcMod,BCSTACK_A,32,bc_page,data_buffer,32,CardId);
		//RT-->BC//poll msgs
//RT_ADD 7 SUB_ADD 0 WORD_CT 2
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 7;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x3200;
	
		DefMsg(BcMod,BCSTACK_A,33,bc_page,data_buffer,32,CardId);

	//	RT-->BC//poll
//RT_ADD 15 SUB_ADD 0 WORD_CT 2
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x3300;
	
		DefMsg(BcMod,BCSTACK_A,34,bc_page,data_buffer,32,CardId);
		///RT-->BC//poll
//RT_ADD 8 SUB_ADD 0 WORD_CT 2
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 8;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x3400;
	
		DefMsg(BcMod,BCSTACK_A,35,bc_page,data_buffer,32,CardId);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	frame_items[27]=28;
	frame_items[28]=29;
	frame_items[29]=30;
	frame_items[30]=31;
	frame_items[31]=32;
	frame_items[32]=33;
	frame_items[33]=34;
	frame_items[34]=35;


		DefFrame(BcMod,BCSTACK_A,35,frame_items,CardId);
		//SetBCEOMCnt(BcMod,0,CARD1);
		RunBc(BcMod,BCSTACK_A,1,35,1,CardId);
		
		/***********module 2*****/

			RtMod=2;BcMod=2;MtMod=2;
//	ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);
	
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);


	//	BC--> RT RT_ADD 31 SUB_ADD 29 WORD_CT 2
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = Ch;
	bc_page.self_test     = NO;
	bc_page.mask_bcst_bit = NO;			
	bc_page.broadcast     = YES;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 31;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 29;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0;i<3;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;


	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,2,CARD1);
		
	//BC--> RT RT_ADD 21 SUB_ADD 1 WORD_CT 14
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = Ch;
	bc_page.self_test     = NO;
	bc_page.mask_bcst_bit = NO;			
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;  
		for(i=0;i<15;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		 
		 
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,14,CARD1);

	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = Ch;
	bc_page.self_test     = NO;
	bc_page.mask_bcst_bit = NO;			
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0;i<15;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		 
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,14,CARD1);

	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 10;  
	
	for(i=0;i<11;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,10,CARD1);


	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 13;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0005;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,13,CARD1);

	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 13;    
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0006;
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,13,CARD1);


	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 32; 
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0007;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,32,CARD1);


	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 6;  
	for(i=0; i<7; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,6,CARD1);


	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;  
		for(i=0;i<15;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,14,CARD1);

	
	
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 5;  
	
	for(i=0; i<6; i++)
	data_buffer[0] = 0x0000;
	data_buffer[1] = 0x0001;
	data_buffer[2] = 0x0002;
	data_buffer[3] = 0x0003;
	data_buffer[4] = 0x0004;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,5,CARD1);


	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 12;     
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0011;
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,12,CARD1);

	bc_page.cmd_1.taddr   = 16;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 1;      
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0012;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,1,CARD1);


	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0013;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,4,CARD1);

	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 24;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0014;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,24,CARD1);

	bc_page.cmd_1.taddr   = 26;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0015;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,1,CARD1);


	//BC--> RT RT_ADD 13 SUB_ADD 7 WORD_CT 12
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 12;   

	for(i=0; i<13; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,12,CARD1);

	//BC--> RT RT_ADD 26 SUB_ADD 9 WORD_CT 5
	bc_page.cmd_1.taddr   = 26;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 9;
	bc_page.cmd_1.wcnt    = 5; 
	
	for(i=0; i<6; i++)
	data_buffer[0] = 0x0000;
	data_buffer[1] = 0x0001;
	data_buffer[2] = 0x0002;
	data_buffer[3] = 0x0003;
	data_buffer[4] = 0x0004;

	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,5,CARD1);

	//BC--> RT RT_ADD 15 SUB_ADD 6 WORD_CT 4
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 4;   

	for(i=0; i<5; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,4,CARD1);


	//BC--> RT RT_ADD 15 SUB_ADD 2 WORD_CT 24
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 24;   
	for(i=0; i<25; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,24,CARD1);

	//BC--> RT RT_ADD 5 SUB_ADD 1 WORD_CT 8
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 8;   
	for(i=0; i<9; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,8,CARD1);

	//BC--> RT RT_ADD 10 SUB_ADD 7 WORD_CT 1
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<2; i++)
		data_buffer[0] = 0x0000;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,1,CARD1);

	//BC--> RT RT_ADD 21 SUB_ADD 2 WORD_CT 18
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 18;   
	for(i=0; i<19; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,22,CARD1);

	//BC--> RT RT_ADD 10 SUB_ADD 1 WORD_CT 16
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 16;   
	for(i=0; i<17; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,16,CARD1);

	//RT--> BC RT_ADD 25 SUB_ADD 2 WORD_CT 11
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 11;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0024;
	DefMsg(2,BCSTACK_A,25,bc_page,data_buffer,11,CARD1);

	//RT--> BC RT_ADD 5 SUB_ADD 11 WORD_CT 4
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0025;
	DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,4,CARD1);

	//RT--> BC RT_ADD 16 SUB_ADD 11 WORD_CT 7
	bc_page.cmd_1.taddr   = 16;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 7;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0026;
	DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,7,CARD1);

    //RT--> BC RT_ADD 15 SUB_ADD 0 WORD_CT 2
	bc_page.mode_code     = YES;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0027;
	DefMsg(BcMod,BCSTACK_A,28,bc_page,data_buffer,2,CARD1);


	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	frame_items[27]=28;


	DefFrame(BcMod,BCSTACK_A,28,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,28,1,CardId);
		/**************module 3 ******************///Bus 3 M0
	RtMod=3;BcMod=3;MtMod=3;
	//	ClearRam(BcMod,BC,CardId);
		ConfigureBC(BcMod,128,CardId);

	
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);
	
//	BC--> RT RT_ADD 31 SUB_ADD 29 WORD_CT 2
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = Ch;
	bc_page.self_test     = NO;
	bc_page.mask_bcst_bit = NO;			
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 31;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 29;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0;i<3;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,2,CARD1);
	
	//BC--> RT RT_ADD 5 SUB_ADD 29 WORD_CT 2
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = Ch;
	bc_page.self_test     = NO;
	bc_page.mask_bcst_bit = NO;			
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 29;
	bc_page.cmd_1.wcnt    = 2;  

	for(i=0;i<3;i++)
	data_buffer[0] = 0x0000;
	data_buffer[1] = 0x0001;
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,2,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 1 WORD_CT 17
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0003;
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,17,CARD1);

	//BC--> RT RT_ADD 11 SUB_ADD 17 WORD_CT 15
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 17;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0;i<16;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,15,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 1 WORD_CT 12
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 12;  
	for(i=0;i<13;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,12,CARD1);

	//RT--> BC RT_ADD 3 SUB_ADD 15 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 15;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0006;
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,30,CARD1);

	//RT--> BC RT_ADD 3 SUB_ADD 9 WORD_CT 22
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 9;
	bc_page.cmd_1.wcnt    = 22;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0007;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,22,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 8 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0008;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,30,CARD1);

	//RT--> BC RT_ADD 5 SUB_ADD 19 WORD_CT 23
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 19;
	bc_page.cmd_1.wcnt    = 23;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0009;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,23,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 25 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 25;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0010;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 1 WORD_CT 16
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 16;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0011;
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,16,CARD1);
	
	//RT--> BC RT_ADD 11 SUB_ADD 8 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 14;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0012;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,14,CARD1);

	//RT--> RT RT_ADD 11 SUB_ADD 23 WORD_CT 32
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 23;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0013;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,13,CARD1);
	 
	//RT--> BC RT_ADD 3 SUB_ADD 7 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0014;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,30,CARD1);

	//RT--> BC RT_ADD 7 SUB_ADD 2 WORD_CT 4
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 4;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0015;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,4,CARD1);

	
	//RT--> BC RT_ADD 5 SUB_ADD 16 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0016;
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,32,CARD1);
	
	//RT--> RT RT_ADD 11 SUB_ADD 24 WORD_CT 17
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 24;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0017;
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,17,CARD1);
	
	
	//BC--> RT RT_ADD 3 SUB_ADD 3 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<18;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
	
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,17,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 16 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0019;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,11,CARD1);

	//RT--> RT RT_ADD 3 SUB_ADD 6 WORD_CT 32
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0020;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 5 WORD_CT 25
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 25;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0021;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,25,CARD1);
	
	
	//RT--> RT RT_ADD 3 SUB_ADD 22 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 22;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0022;
	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,17,CARD1);

	
	//BC--> RT RT_ADD 3 SUB_ADD 8 WORD_CT 9
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 9;  
	for(i=0;i<10;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,9,CARD1);
	
	//BC--> RT RT_ADD 7 SUB_ADD 1 WORD_CT 8
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 8;  
	for(i=0;i<9;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,8,CARD1);
	
	 
	//RT--> BC RT_ADD 3 SUB_ADD 6 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0025;
	DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,11,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;



	DefFrame(BcMod,BCSTACK_A,26,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,26,1,CardId);


		//Sleep(20):
// counter 0 zero completed//************1st  MinorCycle Started*********/////////////////////////////////////////
							/*bus-1 MINOR CYCLE 1*/

	}else if(Counter==2)
		{

		RtMod=1;BcMod=1;MtMod=1;
		//ClearRam(BcMod,BC,CardId);
		ConfigureBC(BcMod,128,CardId);

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x1000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);
	


		//BC-->RT
//RT_ADD 21 SUB_ADD 1 WORD_CT 14
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<15; i++)
		data_buffer[i] = i;
	
DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,32,CardId);
	//	BC-->RT
//RT_ADD 22 SUB_ADD 1 WORD_CT 14

		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<15; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,32,CardId);

		//RT-->BC
//RT_ADD 12 SUB_ADD 1 WORD_CT 2
		bc_page.cmd_1.taddr   = 12;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x3700;
	
		DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 11 SUB_ADD 1 WORD_CT 14
		bc_page.cmd_1.taddr   = 11;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x3800;
	
		DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 13 SUB_ADD 1 WORD_CT 15
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 15;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x3900;
	
		DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,32,CardId);///HERE ENDED FOR 29TH MSG WENT FOR LUNCH

		//BC-->RT
//RT_ADD 16 SUB_ADD 3 WORD_CT 2
		bc_page.cmd_1.taddr   = 16;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<3; i++)
			data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 15 SUB_ADD 1 WORD_CT 22
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 22;
		
			for(i=0; i<23; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,32,CardId);



//	BC-->RT
//RT_ADD 27 SUB_ADD 4 WORD_CT 4
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<5; i++)
			
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,32,CardId);
	//	RT-->BC
//RT_ADD 15 SUB_ADD 1 WORD_CT 17
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 17;
		
		for(i=0; i<17; i++)
			data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,32,CardId);

		//RT-->BC
//RT_ADD 21 SUB_ADD 6 WORD_CT 1
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 6;
		bc_page.cmd_1.wcnt    = 1;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x4400;
	
		DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 22 SUB_ADD 6 WORD_CT 1
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 6;
		bc_page.cmd_1.wcnt    = 1;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x4500;
	
		DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 13 SUB_ADD 4 WORD_CT 32
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x4600;
	
		DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 15 SUB_ADD 3 WORD_CT 24
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 24;
		
		for(i=0; i<25; i++)
		data_buffer[i] = i;
		
		DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 4 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
		DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CardId);


		//	BC-->RT
//RT_ADD 24 SUB_ADD 2 WORD_CT 32
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = i;	
			
		DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,32,CardId);
	//RT-->BC
//RT_ADD 21 SUB_ADD 3 WORD_CT 12
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 12;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x5000;
	
		DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,32,CardId);

		//RT-->BC
//RT_ADD 22 SUB_ADD 3 WORD_CT 12
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 12;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x5100;
	
		DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 12 SUB_ADD 30 WORD_CT 1
		bc_page.cmd_1.taddr   = 12;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 30;
		bc_page.cmd_1.wcnt    = 1;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x5200;
	
		DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 15 SUB_ADD 2 WORD_CT 24
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 24;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x5300;
	
		DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 2 SUB_ADD 11 WORD_CT 19
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 19;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x5400;
	
		DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 27 SUB_ADD 2 WORD_CT 4
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x5500;
	
		DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,32,CardId);

		//BC-->RT
//RT_ADD 22 SUB_ADD 2 WORD_CT 18
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 18;
		
		for(i=0; i<19; i++)
	data_buffer[i] = i;
DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,32,CardId);

		//RT-->BC
//RT_ADD 24 SUB_ADD 7 WORD_CT 6
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 7;
		bc_page.cmd_1.wcnt    = 6;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x5700;
	
		DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 2 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
		
		DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 3 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
	data_buffer[i] = i;
		
	DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,32,CardId);
			//BC-->RT
//RT_ADD 14 SUB_ADD 4 WORD_CT 27
		bc_page.cmd_1.taddr   = 14;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 27;

		for(i=0; i<27; i++)
	data_buffer[i] = i;
			
	
		DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,32,CardId);
					//BC-->RT
//RT_ADD 18 SUB_ADD 7 WORD_CT 3
		bc_page.cmd_1.taddr   = 18;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 7;
		bc_page.cmd_1.wcnt    = 3;
		
		for(i=0; i<3; i++)
		data_buffer[i] = i;
		DefMsg(BcMod,BCSTACK_A,28,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 24 SUB_ADD 10 WORD_CT 6
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 10;
		bc_page.cmd_1.wcnt    = 6;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x6200;
	
		DefMsg(BcMod,BCSTACK_A,29,bc_page,data_buffer,32,CardId);

		//RT-->BC
		//RT_ADD 16 SUB_ADD 11 WORD_CT 7 //msd
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 9;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x6300;
	
		DefMsg(BcMod,BCSTACK_A,30,bc_page,data_buffer,32,CardId);

				//RT-->BC
//RT_ADD 15 SUB_ADD 11 WORD_CT 9
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 9;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x6400;
	
		DefMsg(BcMod,BCSTACK_A,31,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 9 SUB_ADD 11 WORD_CT 4
		bc_page.cmd_1.taddr   = 9;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x6500;
	
		DefMsg(BcMod,BCSTACK_A,32,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 8 SUB_ADD 11 WORD_CT 4
		bc_page.cmd_1.taddr   = 8;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x6600;
	
		DefMsg(BcMod,BCSTACK_A,33,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 12 SUB_ADD 0 WORD_CT 2
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 12;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x6700;
	
		DefMsg(BcMod,BCSTACK_A,34,bc_page,data_buffer,32,CardId);

				//RT-->BC
//RT_ADD 13 SUB_ADD 0 WORD_CT 2
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x6800;
	
		DefMsg(BcMod,BCSTACK_A,35,bc_page,data_buffer,32,CardId);


	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	frame_items[27]=28;
	frame_items[28]=29;
	frame_items[29]=30;
	frame_items[30]=31;
	frame_items[31]=32;
	frame_items[32]=33;
	frame_items[33]=34;
	frame_items[34]=35;

		DefFrame(BcMod,BCSTACK_A,35,frame_items,CardId);
		//SetBCEOMCnt(BcMod,0,CARD1);
		RunBc(BcMod,BCSTACK_A,1,35,1,CardId);


		// counter 1 one completed//************2 nd  MinorCycle Started*********/////////////////////////////////////////
	/***********module 2*****/

	RtMod=2;BcMod=2;MtMod=2;
	//ClearRam(BcMod,BC,CardId);    //BUS 2 
	ConfigureBC(BcMod,128,CardId);


		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x1000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);


		//BC--> RT RT_ADD 21 SUB_ADD 1 WORD_CT 14
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;   
		for(i=0; i<14; i++)
			data_buffer[0] = 0x0000;
			data_buffer[1] = 0x0001;
			data_buffer[2] = 0x0002;
			data_buffer[3] = 0x0003;
			data_buffer[4] = 0x0004;
			data_buffer[5] = 0x0005;
			data_buffer[6] = 0x0006;
			data_buffer[7] = 0x0007;
			data_buffer[8] = 0x0008;
			data_buffer[9] = 0x0009;
			data_buffer[10] =0x000A;
			data_buffer[11] =0x000B;
			data_buffer[12] =0x000C;
			data_buffer[13] =0x000D;	
		DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 22 SUB_ADD 1 WORD_CT 14
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;   
		for(i=0; i<14; i++)
			data_buffer[0] = 0x0000;
			data_buffer[1] = 0x0001;
			data_buffer[2] = 0x0002;
			data_buffer[3] = 0x0003;
			data_buffer[4] = 0x0004;
			data_buffer[5] = 0x0005;
			data_buffer[6] = 0x0006;
			data_buffer[7] = 0x0007;
			data_buffer[8] = 0x0008;
			data_buffer[9] = 0x0009;
			data_buffer[10] =0x000A;
			data_buffer[11] =0x000B;
			data_buffer[12] =0x000C;
			data_buffer[13] =0x000D;	
		DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 25 SUB_ADD 1 WORD_CT 10
		bc_page.cmd_1.taddr   = 25;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 10;   
		for(i=0; i<10; i++)
			data_buffer[0] = 0x0000;
			data_buffer[1] = 0x0001;
			data_buffer[2] = 0x0002;
			data_buffer[3] = 0x0003;
			data_buffer[4] = 0x0004;
			data_buffer[5] = 0x0005;
			data_buffer[6] = 0x0006;
			data_buffer[7] = 0x0007;
			data_buffer[8] = 0x0008;
			data_buffer[9] = 0x0009;
		DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,10,CARD1);

//	BC--> RT RT_ADD 16 SUB_ADD 3 WORD_CT 2
		bc_page.cmd_1.taddr   = 16;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 2;   
		for(i=0; i<2; i++)
			data_buffer[0] = 0x0000;
			data_buffer[1] = 0x0001;

		DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,2,CARD1);

	//BC--> RT RT_ADD 6 SUB_ADD 5 WORD_CT 2
		bc_page.cmd_1.taddr   = 6;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 5;
		bc_page.cmd_1.wcnt    = 2;   
		for(i=0; i<2; i++)
			data_buffer[0] = 0x0000;
			data_buffer[1] = 0x0001;

		DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,2,CARD1);

		//BC--> RT RT_ADD 25 SUB_ADD 3 WORD_CT 6
		bc_page.cmd_1.taddr   = 25;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 6;   
		for(i=0; i<6; i++)
			data_buffer[0] = 0x0000;
			data_buffer[1] = 0x0001;
			data_buffer[2] = 0x0002;
			data_buffer[3] = 0x0003;
			data_buffer[4] = 0x0004;
			data_buffer[5] = 0x0005;
		DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,6,CARD1);
	
	//RT--> BC RT_ADD 11 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0034;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,14,CARD1);

	//RT--> BC RT_ADD 21 SUB_ADD 6 WORD_CT 1
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0035;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,1,CARD1);

	//RT--> BC RT_ADD 22 SUB_ADD 6 WORD_CT 1
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0036;
	DefMsg(2,BCSTACK_A,10,bc_page,data_buffer,1,CARD1);

	//RT--> BC RT_ADD 11 SUB_ADD 3 WORD_CT 2
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0037;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,2,CARD1);

 //BC--> RT RT_ADD 5 SUB_ADD 2 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
		data_buffer[24] = 0x0018;
		data_buffer[25] = 0x0019;
		data_buffer[26] = 0x001A;
		data_buffer[27] = 0x001B;
		data_buffer[28] = 0x001C;
		data_buffer[29] = 0x001D;
		data_buffer[30] = 0x001E;
		data_buffer[31] = 0x001F;
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,32,CARD1);

	//RT--> BC RT_ADD 16 SUB_ADD 2 WORD_CT 6
	bc_page.cmd_1.taddr   = 16;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 6;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0039;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,6,CARD1);

	//RT--> BC RT_ADD 15 SUB_ADD 1 WORD_CT 17
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 17;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0040;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,17,CARD1);

	//RT--> RT RT_ADD 2 SUB_ADD 15 WORD_CT 13
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 15;
	bc_page.cmd_1.wcnt    = 13;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0041;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,13,CARD1);


	//RT--> RT RT_ADD 2 SUB_ADD 16 WORD_CT 13
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 13;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0042;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,13,CARD1);

	//RT--> RT RT_ADD 13 SUB_ADD 9 WORD_CT 4
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 9;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0043;
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,4,CARD1);

	//BC--> RT RT_ADD 10 SUB_ADD 7 WORD_CT 1
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<1; i++)
		data_buffer[0] = 0x0000;
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,1,CARD1);

	//BC--> RT RT_ADD 22 SUB_ADD 2 WORD_CT 18
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 18;   
	for(i=0; i<18; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,18,CARD1);
 
//	RT--> RT RT_ADD 15 SUB_ADD 3 WORD_CT 24
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 24;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0046;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,24,CARD1);

//	BC--> RT RT_ADD 10 SUB_ADD 2 WORD_CT 27
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 27;   
	for(i=0; i<27; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
		data_buffer[24] = 0x0018;
		data_buffer[25] = 0x0019;
		data_buffer[26] = 0x001A;
	
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,27,CARD1);

	//RT--> BC RT_ADD 25 SUB_ADD 2 WORD_CT 11
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 11;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0048;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,11,CARD1);

	//RT--> BC RT_ADD 15 SUB_ADD 11 WORD_CT 9
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 9;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0049;
	DefMsg(2,BCSTACK_A,23,bc_page,data_buffer,9,CARD1);

	//RT--> BC RT_ADD 25 SUB_ADD 11 WORD_CT 5
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 5;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0050;
	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,5,CARD1);

	//RT--> BC RT_ADD 26 SUB_ADD 11 WORD_CT 5
	bc_page.cmd_1.taddr   = 26;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 5;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0051;
	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,5,CARD1);

//RT--> BC RT_ADD 6 SUB_ADD 11 WORD_CT 4
	bc_page.cmd_1.taddr   = 6;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0052;
	DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,4,CARD1);

	//RT--> BC RT_ADD 10 SUB_ADD 11 WORD_CT 4
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0053;
	DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,4,CARD1);

	//RT--> BC RT_ADD 6 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast     = NO;
	bc_page.mode_code     = YES;
	bc_page.cmd_1.taddr   = 6;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0054;
	DefMsg(BcMod,BCSTACK_A,28,bc_page,data_buffer,2,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	frame_items[27]=28;


	DefFrame(BcMod,BCSTACK_A,28,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,28,1,CardId);

						/*Minor Cycle 1 Completed*/
	/**********module 3 ///////*/
		RtMod=3;BcMod=3;MtMod=3;
		//ClearRam(BcMod,BC,CardId);
		ConfigureBC(BcMod,128,CardId);

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x1000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);
	//RT--> BC RT_ADD 5 SUB_ADD 1 WORD_CT 17
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0026;
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,17,CARD1);

	//BC--> RT RT_ADD 5 SUB_ADD 30 WORD_CT 10
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 30;
	bc_page.cmd_1.wcnt    = 10;  
	for(i=0;i<10;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,10,CARD1);

	//BC--> RT RT_ADD 11 SUB_ADD 17 WORD_CT 15
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 17;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0;i<15;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,15,CARD1);

	//BC--> RT RT_ADD 3 SUB_ADD 1 WORD_CT 12
    bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 12;  
	for(i=0;i<12;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;	
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,29,CARD1);
	
	//RT--> BCRT_ADD 3 SUB_ADD 15 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 15;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0030;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 8 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0031;
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,6,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 19 WORD_CT 23
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 19;
	bc_page.cmd_1.wcnt    = 23;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0032;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,7,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 27 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 27;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0033;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 11 SUB_ADD 8 WORD_CT 14
    bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 14;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0034;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,14,CARD1);

	//RT--> RT RT_ADD 11 SUB_ADD 23 WORD_CT 32
    bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 23;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0035;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,32,CARD1);
    
	//BC--> RT RT_ADD 3 SUB_ADD 5 WORD_CT 15
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0;i<15;i++)
		
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,11,CARD1);
    
	//RT--> BC RT_ADD 3 SUB_ADD 7 WORD_CT 30
    bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0037;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,12,CARD1);
	
	//BC--> RT RT_ADD 7 SUB_ADD 2 WORD_CT 32
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
		data_buffer[24] = 0x0018;
		data_buffer[25] = 0x0019;
		data_buffer[26] = 0x001A;
		data_buffer[27] = 0x001B;
		data_buffer[28] = 0x001C;
		data_buffer[29] = 0x001D;
		data_buffer[30] = 0x001E;
		data_buffer[31] = 0x001F;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,13,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 16 WORD_CT 32
    bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0039;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,17,CARD1);
		
	//RT--> RT RT_ADD 11 SUB_ADD 24 WORD_CT 17
    bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0040;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,15,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 3 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<17;i++)
	
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,16,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 16 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0042;
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,17,CARD1);
	
    //RT--> RT RT_ADD 3 SUB_ADD 9 WORD_CT 32
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 9;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0043;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,18,CARD1);
		
	//RT--> RT RT_ADD 3 SUB_ADD 6 WORD_CT 32
    bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0044;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,19,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 5 WORD_CT 25
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 25;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0045;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,20,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 22 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 22;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0046;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,21,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 8 WORD_CT 9
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 9;  
	for(i=0;i<9;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,22,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 30 WORD_CT 10
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 30;
	bc_page.cmd_1.wcnt    = 10;  
	for(i=0;i<10;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,23,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 6 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0049;
	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,24,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 11 WORD_CT 16
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 16;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0050;
	DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,16,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast     = NO;
	bc_page.mode_code     = YES;
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0051;
	DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,2,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;



	DefFrame(BcMod,BCSTACK_A,27,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,27,1,CardId);

	}else if(Counter==3)
		{
	

	RtMod=1;BcMod=1;MtMod=1;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);

///////////////////*********2nd MINOR CYCLE STARTD******//////////////
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x2000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);




		//BC-->RT
//RT_ADD 21 SUB_ADD 1 WORD_CT 14
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<14; i++)
	data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 22 SUB_ADD 1 WORD_CT 14
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
	for(i=0; i<14; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,32,CardId);
				//RT-->BC
//RT_ADD 12 SUB_ADD 1 WORD_CT 2
		bc_page.cmd_1.taddr   = 12;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x7000;
	
		DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,32,CardId);

		//RT-->BC
//RT_ADD 11 SUB_ADD 1 WORD_CT 14
		bc_page.cmd_1.taddr   = 11;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x7100;
	
		DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,32,CardId);
				//RT-->BC
//RT_ADD 13 SUB_ADD 1 WORD_CT 15
		
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 15;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x7200;
	
		DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 7 SUB_ADD 3 WORD_CT 8
		bc_page.cmd_1.taddr   = 7;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 8;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x7300;
	
		DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,32,CardId);
				//BC-->RT
//RT_ADD 16 SUB_ADD 1 WORD_CT 6
		bc_page.cmd_1.taddr   = 16;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 6;
		
		for(i=0; i<6; i++)
		data_buffer[0] = 0x1F40;
		data_buffer[1] = 0xFE1A;
		data_buffer[2] = 0x0053;
		data_buffer[3] = 0xFBE4;
		data_buffer[4] = 0x0000;
		data_buffer[5] = 0x00E5;
		DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,32,CardId);

		//BC-->RT
//RT_ADD 27 SUB_ADD 2 WORD_CT 32
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,32,CardId);
			//RT-->BC
//RT_ADD 12 SUB_ADD 4 WORD_CT 3
		bc_page.cmd_1.taddr   = 12;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 3;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x7600;
	
		DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,32,CardId);
			//BC-->RT
//RT_ADD 2 SUB_ADD 21 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 21;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
	data_buffer[i] = i;
		DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 24 SUB_ADD 9 WORD_CT 32
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 9;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x7800;
	
		DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 4 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)

		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 11 SUB_ADD 4 WORD_CT 7
		bc_page.cmd_1.taddr   = 11;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 7;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x8000;
	
		DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 11 SUB_ADD 1 WORD_CT 4
		bc_page.cmd_1.taddr   = 11;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<4; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 8 SUB_ADD 4 WORD_CT 3
		bc_page.cmd_1.taddr   = 8;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 3;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x8200;
	
		DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 27 SUB_ADD 6 WORD_CT 6
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 6;
		bc_page.cmd_1.wcnt    = 6;
		
		for(i=0; i<32; i++)
			data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 21 SUB_ADD 2 WORD_CT 18
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 18;
		
		for(i=0; i<18; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 24 SUB_ADD 6 WORD_CT 32
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 6;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x8500;
	
		DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 2 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
	
		DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 3 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 17 SUB_ADD 25 WORD_CT 5
		bc_page.cmd_1.taddr   = 17;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 25;
		bc_page.cmd_1.wcnt    = 5;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x8800;
	
		DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 17 SUB_ADD 9 WORD_CT 3
		bc_page.cmd_1.taddr   = 17;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 9;
		bc_page.cmd_1.wcnt    = 3;
		
		for(i=0; i<3; i++)
			data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 27 SUB_ADD 5 WORD_CT 2
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 5;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x9000;
	
		DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,32,CardId);

		//BC-->RT
//RT_ADD 2 SUB_ADD 17 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 17;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
		DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,32,CardId);

		//RT-->BC
//RT_ADD 24 SUB_ADD 11 WORD_CT 9
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 9;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x9200;
	
		DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,32,CardId);

		//RT-->BC
//RT_ADD 11 SUB_ADD 0 WORD_CT 2
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 11;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x9300;
	
		DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,32,CardId);

				//RT-->BC
//RT_ADD 16 SUB_ADD 0 WORD_CT 2
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 16;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x9400;
	
		DefMsg(BcMod,BCSTACK_A,28,bc_page,data_buffer,32,CardId);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	frame_items[27]=28;


		DefFrame(BcMod,BCSTACK_A,28,frame_items,CardId);
		//SetBCEOMCnt(BcMod,0,CARD1);
		RunBc(BcMod,BCSTACK_A,1,28,1,CardId);
	
/////2 comleted//////****Minor cycle 3 STARTED******////////////////////////////////////////////
		/***********module 2*****/

			RtMod=2;BcMod=2;MtMod=2;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);

	bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x2000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);

	//BC--> RT RT_ADD 21 SUB_ADD 1 WORD_CT 14
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<14; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
	
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 22 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<14; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 25 SUB_ADD 1 WORD_CT 10
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 10;   
	for(i=0; i<10; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,10,CARD1);

//RT--> BC RT_ADD 6 SUB_ADD 3 WORD_CT 7
	bc_page.cmd_1.taddr   = 6;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 7;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0058;
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,7,CARD1);

	//BC--> RT RT_ADD 25 SUB_ADD 3 WORD_CT 6
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 6;   
	for(i=0; i<6; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,6,CARD1);

	//RT--> BC RT_ADD 11 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0060;
	DefMsg(2,BCSTACK_A,7,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 11 SUB_ADD 1 WORD_CT 4
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<4; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
	
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,4,CARD1);

//	RT--> BC RT_ADD 16 SUB_ADD 25 WORD_CT 32
	bc_page.cmd_1.taddr   = 16;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 25;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0062;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,32,CARD1);

 //RT--> BC RT_ADD 5 SUB_ADD 2 WORD_CT 4
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0063;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,4,CARD1);

	//BC--> RT RT_ADD 16 SUB_ADD 1 WORD_CT 6
	bc_page.cmd_1.taddr   = 16;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 6;   
	for(i=0; i<6; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
	
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,6,CARD1);

	//RT--> BC RT_ADD 22 SUB_ADD 7 WORD_CT 21
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 21;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0065;
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,21,CARD1);

	//RT--> BC RT_ADD 21 SUB_ADD 7 WORD_CT 21
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 21;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0066;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,21,CARD1);

	//RT--> RT RT_ADD 2 SUB_ADD 11 WORD_CT 19
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 19;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0067;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,19,CARD1);

	//BC--> RT RT_ADD 15 SUB_ADD 1 WORD_CT 22
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 22;   
	for(i=0; i<22; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,22,CARD1);


	//BC--> RT RT_ADD 10 SUB_ADD 7 WORD_CT 1
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<1; i++)
		data_buffer[0] = 0x0000;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,1,CARD1);

	//BC--> RT RT_ADD 21 SUB_ADD 2 WORD_CT 18
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 18;   
	for(i=0; i<18; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
	
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,18,CARD1);

	//RT--> BC RT_ADD 13 SUB_ADD 1 WORD_CT 32
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0071;
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,32,CARD1);

	//BC--> RT RT_ADD 10 SUB_ADD 4 WORD_CT 5
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 5;   
	for(i=0; i<5; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,5,CARD1);

	//RT--> BC RT_ADD 25 SUB_ADD 2 WORD_CT 11
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 11;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0073;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,11,CARD1);


	//RT--> BC RT_ADD 5 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast = NO;
	bc_page.mode_code = YES;
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0074;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,2,CARD1);

	//RT--> BC RT_ADD 11 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast = NO;
	bc_page.mode_code = YES;
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0075;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,2,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	


	DefFrame(BcMod,BCSTACK_A,22,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,22,1,CardId);

	/**********module 3 ///////*/
		RtMod=3;BcMod=3;MtMod=3;
		//ClearRam(BcMod,BC,CardId);
		ConfigureBC(BcMod,128,CardId);

			bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x2000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);


	//RT--> BC RT_ADD 5 SUB_ADD 1 WORD_CT 17
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0052;
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT RT_ADD 5 SUB_ADD 26 WORD_CT 7
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 26;
	bc_page.cmd_1.wcnt    = 7;  
	for(i=0;i<7;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,7,CARD1);
	
	//BC--> RT RT_ADD 11 SUB_ADD 17 WORD_CT 15
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 17;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0;i<15;i++)
	
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,15,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 1 WORD_CT 12
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 12;  
	for(i=0;i<12;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,12,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 15 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 15;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0056;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 8 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0057;
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 19 WORD_CT 23
    bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0058;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC	RT_ADD 3 SUB_ADD 1 WORD_CT 16
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 16;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0059;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 24 WORD_CT 24
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 24;
	bc_page.cmd_1.wcnt    = 24;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0060;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,24,CARD1);
	
	//RT--> BC RT_ADD 11 SUB_ADD 8 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 14;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0061;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,14,CARD1);
	
	//RT--> RT RT_ADD 11 SUB_ADD 23 WORD_CT 32
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 23;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0062;
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 7 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0063;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 7 SUB_ADD 2 WORD_CT 4
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 4;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0064;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,4,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 16 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0065;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CARD1);
	
	//RT--> RT RT_ADD 11 SUB_ADD 24 WORD_CT 17
 	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 24;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0066;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 3 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<17;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,17,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 16 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0068;
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,11,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 6 WORD_CT 32
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0069;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 5 WORD_CT 25
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 25;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0070;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,25,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 22 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 22;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0071;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 8 WORD_CT 9
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 9;  
	for(i=0;i<9;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,9,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 6 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0073;
	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,11,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 18 WORD_CT 32
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 18;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0074;
	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,32,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 12 WORD_CT 3
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 12;
	bc_page.cmd_1.wcnt    = 3;  
	for(i=0;i<3;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,3,CARD1);
	
	//RT--> BC RT_ADD 11 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast     = NO;
	bc_page.mode_code     = YES;
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0076;
	DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,2,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;




	DefFrame(BcMod,BCSTACK_A,26,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,26,1,CardId);

	}else if(Counter==4)    //bUS-1 
		{

		RtMod=1;BcMod=1;MtMod=1;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x3000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);




		//BC-->RT
//RT_ADD 21 SUB_ADD 1 WORD_CT 14
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<14; i++)
			data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 22 SUB_ADD 1 WORD_CT 14

				bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<14; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 12 SUB_ADD 1 WORD_CT 2
		bc_page.cmd_1.taddr   = 12;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[0] = 0x9700;
			
		DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 11 SUB_ADD 1 WORD_CT 14
		bc_page.cmd_1.taddr   = 11;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x9800;
	
		DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 13 SUB_ADD 1 WORD_CT 15
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 15;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x9900;
	
		DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 15 SUB_ADD 6 WORD_CT 4
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 6;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<4; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 24 SUB_ADD 30 WORD_CT 1
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 30;
		bc_page.cmd_1.wcnt    = 1;
		
		for(i=0; i<1; i++)
			data_buffer[i] = 0x000F;
	
		DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 9 SUB_ADD 1 WORD_CT 1
		bc_page.cmd_1.taddr   = 9;
		bc_page.cmd_1.t_r     = TRANSMIT;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 1;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0102;
	
		DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 27 SUB_ADD 4 WORD_CT 4
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<4; i++)
			data_buffer[i] = i;
		DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 21 SUB_ADD 6 WORD_CT 2
		bc_page.cmd_1.taddr   = 11;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<2; i++)
		data_buffer[i] = i;
		DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 16 SUB_ADD 2 WORD_CT 7
		bc_page.cmd_1.taddr   = 16;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 7;
		
		for(i=0; i<7; i++)
			data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 24 SUB_ADD 3 WORD_CT 32
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0106;
	
		DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 4 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
		DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 24 SUB_ADD 2 WORD_CT 3
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 3;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0108;
	
		DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 22 SUB_ADD 6 WORD_CT 2
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 6;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<2; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 21 SUB_ADD 7 WORD_CT 21
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 7;
		bc_page.cmd_1.wcnt    = 21;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0110;
	
		DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 22 SUB_ADD 7 WORD_CT 21
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 7;
		bc_page.cmd_1.wcnt    = 21;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0111;
	
		DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 13 SUB_ADD 7 WORD_CT 12
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 7;
		bc_page.cmd_1.wcnt    = 12;
		
		for(i=0; i<12; i++)
			data_buffer[i] = i;
		
	
		DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 27 SUB_ADD 2 WORD_CT 4
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0113;
	
		DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 7 SUB_ADD 6 WORD_CT 3
		bc_page.cmd_1.taddr   = 7;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 6;
		bc_page.cmd_1.wcnt    = 3;
		
		for(i=0; i<32; i++)
	data_buffer[i] = i;
		
	
		DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 22 SUB_ADD 2 WORD_CT 18
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 18;
		
		for(i=0; i<18; i++)
		data_buffer[i] = i;
		
	
		DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 8 SUB_ADD 1 WORD_CT 1
		bc_page.cmd_1.taddr   = 8;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 1;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0116;
	
		DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 2 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 3 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 24 SUB_ADD 10 WORD_CT 6
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 10;
		bc_page.cmd_1.wcnt    = 6;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0119;
	
		DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 18 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 18;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
	data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 27 SUB_ADD 11 WORD_CT 4
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0121;
	
		DefMsg(BcMod,BCSTACK_A,28,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 17 SUB_ADD 0 WORD_CT 2
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 17;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0122;
	
		DefMsg(BcMod,BCSTACK_A,29,bc_page,data_buffer,32,CardId);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	frame_items[27]=28;
	frame_items[28]=29;



		DefFrame(BcMod,BCSTACK_A,29,frame_items,CardId);
		//SetBCEOMCnt(BcMod,0,CARD1);
		RunBc(BcMod,BCSTACK_A,1,29,1,CardId);
// counter 4 3rd completed//************4th th MinorCycle Started*********/////////////////////////////////////////
		/***********module 2*****/

			RtMod=2;BcMod=2;MtMod=2;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x3000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);
	//BC--> RT RT_ADD 21 SUB_ADD 1 WORD_CT 14
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0076;
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 22 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0077;
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 25 SUB_ADD 1 WORD_CT 10
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 10;   
	for(i=0; i<10; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,10,CARD1);

	//RT--> BC RT_ADD 13 SUB_ADD 4 WORD_CT 32
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0079;
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,32,CARD1);

	//BC--> RT RT_ADD 25 SUB_ADD 3 WORD_CT 6
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 6;   
	for(i=0; i<6; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,6,CARD1);

	//RT--> BC RT_ADD 11 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0081;
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,14,CARD1);

	//RT--> BC RT_ADD 16 SUB_ADD 1 WORD_CT 15
	bc_page.cmd_1.taddr   = 16;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 15;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0082;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,15,CARD1);

	//RT--> BC RT_ADD 26 SUB_ADD 4 WORD_CT 32

	bc_page.cmd_1.taddr   = 26;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0083;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,32,CARD1);

	//BC--> RT RT_ADD 5 SUB_ADD 2 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
		data_buffer[24] = 0x0018;
		data_buffer[25] = 0x0019;
		data_buffer[26] = 0x001A;
		data_buffer[27] = 0x001B;
		data_buffer[28] = 0x001C;
		data_buffer[29] = 0x001D;
		data_buffer[30] = 0x001E;
		data_buffer[31] = 0x001F;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,32,CARD1);

	//BC--> RT RT_ADD 26 SUB_ADD 5 WORD_CT 1
	bc_page.cmd_1.taddr   = 26;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<1; i++)
		data_buffer[0] = 0x0000;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,1,CARD1);

	//RT--> BC RT_ADD 25 SUB_ADD 4 WORD_CT 32

	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0086;
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,32,CARD1);


//	RT--> RT RT_ADD 2 SUB_ADD 10 WORD_CT 32
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 10;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0087;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,32,CARD1);

//	BC--> RT RT_ADD 6 SUB_ADD 1 WORD_CT 6
	bc_page.cmd_1.taddr   = 6;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 6;   
	for(i=0; i<6; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,6,CARD1);

	//BC--> RT RT_ADD 10 SUB_ADD 7 WORD_CT 1
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<1; i++)
		data_buffer[0] = 0x0000;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,1,CARD1);


	//BC--> RT RT_ADD 22 SUB_ADD 2 WORD_CT 18
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 18;   
	for(i=0; i<18; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,18,CARD1);

		//BC--> RT RT_ADD 10 SUB_ADD 5 WORD_CT 9
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 9;   
	for(i=0; i<9; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,9,CARD1);

	// RT--> BC RT_ADD 25 SUB_ADD 2 WORD_CT 11
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 11;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0092;
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,11,CARD1);

	//RT--> BC RT_ADD 25 SUB_ADD 3 WORD_CT 19
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 19;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0093;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,19,CARD1);

	//RT--> BC RT_ADD 11 SUB_ADD 11 WORD_CT 4
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0094;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,4,CARD1);

	//RT--> BC RT_ADD 21 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast = NO;
	bc_page.mode_code = YES;
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0095;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,2,CARD1);

	//RT--> BC RT_ADD 22 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast = NO;
	bc_page.mode_code = YES;
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0096;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,2,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	


	DefFrame(BcMod,BCSTACK_A,22,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,22,1,CardId);

	/**********module 3 ///////*/
		RtMod=3;BcMod=3;MtMod=3;
		//ClearRam(BcMod,BC,CardId);
		ConfigureBC(BcMod,128,CardId);

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x3000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);

	//RT--> BC RT_ADD 5 SUB_ADD 1 WORD_CT 17
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0077;
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT RT_ADD 5 SUB_ADD 30 WORD_CT 10
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 30;
	bc_page.cmd_1.wcnt    = 10;  
	for(i=0;i<10;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,10,CARD1);
	
	//BC--> RT RT_ADD 11 SUB_ADD 17 WORD_CT 15
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 17;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0;i<15;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,15,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 1 WORD_CT 12
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 12;  
	for(i=0;i<12;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,12,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 15 WORD_CT 30
  	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 15;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0081;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 8 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0082;
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 19 WORD_CT 23
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 19;
	bc_page.cmd_1.wcnt    = 23;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0083;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,23,CARD1);
	
	//RT--> BC RT_ADD 11 SUB_ADD 8 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 14;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0084;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,14,CARD1);
	
	//RT--> RT RT_ADD 11 SUB_ADD 23 WORD_CT 32
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 23;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0085;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,32,CARD1);
	
	//BC--> RT RT_ADD 11 SUB_ADD 18 WORD_CT 6
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 18;
	bc_page.cmd_1.wcnt    = 6;  
	for(i=0;i<6;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,10,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 5 WORD_CT 15
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0;i<15;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,15,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 7 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0088;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,30,CARD1);
	
	//BC--> RT RT_ADD 7 SUB_ADD 2 WORD_CT 32
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
		data_buffer[24] = 0x0018;
		data_buffer[25] = 0x0019;
		data_buffer[26] = 0x001A;
		data_buffer[27] = 0x001B;
		data_buffer[28] = 0x001C;
		data_buffer[29] = 0x001D;
		data_buffer[30] = 0x001E;
		data_buffer[31] = 0x001F;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 16 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0090;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CARD1);
	
	//RT--> RT RT_ADD 11 SUB_ADD 24 WORD_CT 17
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 24;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0091;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,15,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 3 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<17;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,16,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 16 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0093;
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,17,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 6 WORD_CT 32
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0094;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 5 WORD_CT 25
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 25;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0095;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,19,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 22 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 22;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0096;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,17,CARD1);
	
    //BC--> RT RT_ADD 3 SUB_ADD 8 WORD_CT 9
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 9;  
	for(i=0;i<9;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,9,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 30 WORD_CT 10
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 30;
	bc_page.cmd_1.wcnt    = 10;  
	for(i=0;i<10;i++)
	data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
	
	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,10,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 6 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0099;
	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,11,CARD1);
	
	//RT--> BC RT_ADD 7 SUB_ADD 11 WORD_CT 4
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 4;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0100;
	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,4,CARD1);
	
	//RT--> BC RT_ADD 11 SUB_ADD 11 WORD_CT 4
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 4;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0101;
	DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,4,CARD1);
	
	//RT--> BC RT_ADD 7 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast     = NO;
	bc_page.mode_code     = YES;
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0102;
	DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,2,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	


	DefFrame(BcMod,BCSTACK_A,27,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,27,1,CardId);

	}else if(Counter==5)
		{

		RtMod=1;BcMod=1;MtMod=1;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);


		//////********MINOR CYCLE 4 STARTED*********/////////
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x4000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);




		//BC-->RT
//RT_ADD 21 SUB_ADD 1 WORD_CT 14
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<14; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 22 SUB_ADD 1 WORD_CT 14

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
	
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<14; i++)
			data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 12 SUB_ADD 1 WORD_CT 2
		bc_page.cmd_1.taddr   = 12;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0125;
	
		DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 11 SUB_ADD 1 WORD_CT 14
		bc_page.cmd_1.taddr   = 11;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0126;
	
		DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 13 SUB_ADD 1 WORD_CT 15
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 15;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0127;
	
		DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 9 SUB_ADD 1 WORD_CT 16
		bc_page.cmd_1.taddr   = 9;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 16;
		
		for(i=0; i<16; i++)
	data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 21 SUB_ADD 1 WORD_CT 13
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 13;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0129;
	
		DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 27 SUB_ADD 2 WORD_CT 32
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
	data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 22 SUB_ADD 1 WORD_CT 13
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 13;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0131;
	
		DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 16 SUB_ADD 4 WORD_CT 2
		bc_page.cmd_1.taddr   = 16;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<2; i++)
			data_buffer[i] = i;
		DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 14 SUB_ADD 1 WORD_CT 1
		bc_page.cmd_1.taddr   = 14;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 1;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0133;
	
		DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 2 SUB_ADD 10 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 10;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0134;
	
		DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 13 SUB_ADD 3 WORD_CT 32
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0135;
	
		DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 24 SUB_ADD 1 WORD_CT 32
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0136;
	
		DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 4 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
	data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 16 SUB_ADD 25 WORD_CT 32
		bc_page.cmd_1.taddr   = 16;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 25;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0138;
	
		DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 21 SUB_ADD 5 WORD_CT 18
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 5;
		bc_page.cmd_1.wcnt    = 18;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0139;
	
		DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 27 SUB_ADD 5 WORD_CT 2
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 5;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0140;
	
		DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 22 SUB_ADD 5 WORD_CT 18
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 5;
		bc_page.cmd_1.wcnt    = 18;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0141;
	
		DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 13 SUB_ADD 9 WORD_CT 4
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 9;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0142;
	
		DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 24 SUB_ADD 4 WORD_CT 1
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 1;
		
		for(i=0; i<1; i++)
			data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 21 SUB_ADD 2 WORD_CT 18
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 18;
		
		for(i=0; i<18; i++)
	data_buffer[i] = i;
	

			
		DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 8 SUB_ADD 1 WORD_CT 16
		bc_page.cmd_1.taddr   = 8;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 16;
		
		for(i=0; i<16; i++)
	data_buffer[i] = i;
		
	
		DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 2 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 3 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 17 SUB_ADD 25 WORD_CT 5
		bc_page.cmd_1.taddr   = 17;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 25;
		bc_page.cmd_1.wcnt    = 5;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0148;
	
		DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 20 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 20;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
	data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,28,bc_page,data_buffer,32,CardId);
	
		
		
		
		//RT-->BC
//RT_ADD 27 SUB_ADD 6 WORD_CT 6
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 6;
		bc_page.cmd_1.wcnt    = 6;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0150;
	
		DefMsg(BcMod,BCSTACK_A,29,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 14 SUB_ADD 11 WORD_CT 5
		bc_page.cmd_1.taddr   = 14;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 5;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0151;
	
		DefMsg(BcMod,BCSTACK_A,30,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 7 SUB_ADD 11 WORD_CT 4
		bc_page.cmd_1.taddr   = 7;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0152;
	
		DefMsg(BcMod,BCSTACK_A,31,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 12 SUB_ADD 11 WORD_CT 4
		bc_page.cmd_1.taddr   = 12;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0153;
	
		DefMsg(BcMod,BCSTACK_A,32,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 11 SUB_ADD 11 WORD_CT 4
		bc_page.cmd_1.taddr   = 11;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0154;
	
		DefMsg(BcMod,BCSTACK_A,33,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 13 SUB_ADD 11 WORD_CT 4
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0155;
	
		DefMsg(BcMod,BCSTACK_A,34,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 24 SUB_ADD 0 WORD_CT 2
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0156;
	
		DefMsg(BcMod,BCSTACK_A,35,bc_page,data_buffer,32,CardId);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	frame_items[27]=28;
	frame_items[28]=29;
	frame_items[29]=30;
	frame_items[30]=31;
	frame_items[31]=32;
	frame_items[32]=33;
	frame_items[33]=34;
	frame_items[34]=35;

		DefFrame(BcMod,BCSTACK_A,35,frame_items,CardId);
		//SetBCEOMCnt(BcMod,0,CARD1);
		RunBc(BcMod,BCSTACK_A,1,35,1,CardId);
//  4th completed//************5th th MinorCycle Started*********/////////////////////////////////////////
		/***********module 2*****/

			RtMod=2;BcMod=2;MtMod=2;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x4000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);

	//BC--> RT RT_ADD 21 SUB_ADD 1 WORD_CT 14
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<14; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 22 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<14; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 25 SUB_ADD 1 WORD_CT 10
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 10;   
	for(i=0; i<10; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,10,CARD1);

	//RT--> BC RT_ADD 21 SUB_ADD 1 WORD_CT 13
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 13;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0100;
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,13,CARD1);


	//RT--> BC RT_ADD 22 SUB_ADD 1 WORD_CT 13
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 13;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0101;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,13,CARD1);


	//RT--> BC RT_ADD 13 SUB_ADD 3 WORD_CT 32
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0102;
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,32,CARD1);

	//BC--> RT RT_ADD 25 SUB_ADD 3 WORD_CT 6
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 6;   
	for(i=0; i<6; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
	
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,6,CARD1);

	//RT--> BC RT_ADD 11 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0104;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,14,CARD1);

	//RT--> BC RT_ADD 21 SUB_ADD 5 WORD_CT 18
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 18;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0105;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,18,CARD1);

	//RT--> BC RT_ADD 22 SUB_ADD 5 WORD_CT 18
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 18;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0106;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,18,CARD1);

	//BC-->RT RT_ADD 6 SUB_ADD 2 WORD_CT 7
	bc_page.cmd_1.taddr   = 6;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 7;   
	for(i=0; i<7; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,7,CARD1);

	//RT--> BC RT_ADD 5 SUB_ADD 2 WORD_CT 4
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0108;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,4,CARD1);

	//RT--> BC RT_ADD 15 SUB_ADD 20 WORD_CT 32
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 20;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0109;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,32,CARD1);

	//RT--> BC RT_ADD 15 SUB_ADD 21 WORD_CT 32
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 21;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0110;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CARD1);

	//BC--> RT RT_ADD 5 SUB_ADD 30 WORD_CT 1
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 30;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<1; i++)
		data_buffer[0] = 0x0000;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,1,CARD1);

	//BC--> RT RT_ADD 13 SUB_ADD 7 WORD_CT 12
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 12;   
	for(i=0; i<12; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,12,CARD1);

	//BC--> RT RT_ADD 15 SUB_ADD 2 WORD_CT 24
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 24;   
	for(i=0; i<24; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
	
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,24,CARD1);

	//BC--> RT RT_ADD 10 SUB_ADD 7 WORD_CT 1
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<1; i++)
		data_buffer[0] = 0x0000;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,1,CARD1);

	//BC--> RT RT_ADD 21 SUB_ADD 2 WORD_CT 18
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 18;   
	for(i=0; i<18; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
	
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,18,CARD1);

	//BC--> RT RT_ADD 10 SUB_ADD 6 WORD_CT 10
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 10;   
	for(i=0; i<10; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
	
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,10,CARD1);

	//RT--> BC RT_ADD 25 SUB_ADD 2 WORD_CT 11
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 11;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0117;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,11,CARD1);

	//RT--> BC RT_ADD 13 SUB_ADD 11 WORD_CT 4
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0118;
	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,4,CARD1);

	//RT--> BC RT_ADD 16 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast     = NO;
	bc_page.mode_code      = YES;
	bc_page.cmd_1.taddr   = 16;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0119;
	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,2,CARD1);

	//RT--> BC RT_ADD 14 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast     = NO;
	bc_page.mode_code      = YES;
	bc_page.cmd_1.taddr   = 14;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0120;
	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,2,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[23]=25;
	


	DefFrame(BcMod,BCSTACK_A,25,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,25,1,CardId);
	/**********module 3 ///////*/
		RtMod=3;BcMod=3;MtMod=3;
		//ClearRam(BcMod,BC,CardId);
		ConfigureBC(BcMod,128,CardId);

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x4000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);
	//BC--> RT RT_ADD 11 SUB_ADD 17 WORD_CT 15
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 17;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0;i<15;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
	
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,15,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 1 WORD_CT 12
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 12;  
	for(i=0;i<12;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,12,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 15 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 15;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0105;
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,30,CARD1);//
	
	//RT--> BC RT_ADD 3 SUB_ADD 9 WORD_CT 22
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 9;
	bc_page.cmd_1.wcnt    = 22;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0106;
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,22,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 8 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0107;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 19 WORD_CT 23
 	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 19;
	bc_page.cmd_1.wcnt    = 23;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0108;
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,23,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 1 WORD_CT 16
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 16;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0109;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,16,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 25 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 25;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0110;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 11 SUB_ADD 8 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 14;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0111;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,14,CARD1);
	
	//RT--> RT RT_ADD 11 SUB_ADD 23 WORD_CT 32
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 23;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0112;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 7 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0113;
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 7 SUB_ADD 2 WORD_CT 4
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 4;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0114;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,4,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 16 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0115;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,32,CARD1);
	
	//BC--> RT RT_ADD 7 SUB_ADD 30 WORD_CT 1
 	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 30;
	bc_page.cmd_1.wcnt    = 1;  
	for(i=0;i<32;i++)
		data_buffer[0] = 0x0000;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,1,CARD1);
	
	//RT--> RT RT_ADD 11 SUB_ADD 24 WORD_CT 17
 	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 24;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
	data_buffer[0] = 0x0117;
	
DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 3 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<17;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,17,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 16 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<11;i++)
		data_buffer[0] = 0x0119;
	
		
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,11,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 6 WORD_CT 32
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0120;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 5 WORD_CT 25
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 25;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0121;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,25,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 22 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 22;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0122;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 8 WORD_CT 9
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 9;  
	for(i=0;i<9;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
	
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,9,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 4 WORD_CT 4
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 4;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0124;
	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,4,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 6 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0125;
	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,11,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 0 WORD_CT 2
 	bc_page.broadcast     = NO;
	bc_page.mode_code     = YES;
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0126;
	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,2,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	


	DefFrame(BcMod,BCSTACK_A,25,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,25,1,CardId);
	}else if(Counter==6)// counter 5///////////////////
		{
		RtMod=1;BcMod=1;MtMod=1;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);


	////////*****Minor cycle 5******///////////////////////////////////

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x5000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);

		//BC-->RT
//RT_ADD 21 SUB_ADD 1 WORD_CT 14
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
			for(i=0; i<14; i++)
			data_buffer[i] = i;
		DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,32,CardId);

		//BC-->RT
//RT_ADD 22 SUB_ADD 1 WORD_CT 14
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
			for(i=0; i<14 ;i++)
		data_buffer[i] = i;
		DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 12 SUB_ADD 1 WORD_CT 2
		bc_page.cmd_1.taddr   = 12;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0159;
	
		DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 11 SUB_ADD 1 WORD_CT 14
		bc_page.cmd_1.taddr   = 11;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0160;
	
		DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 13 SUB_ADD 1 WORD_CT 15
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 15;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0161;
	
		DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 14 SUB_ADD 1 WORD_CT 32
		bc_page.cmd_1.taddr   = 14;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 7 SUB_ADD 4 WORD_CT 5
		bc_page.cmd_1.taddr   = 7;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 5;
		
		for(i=0; i<5; i++)
		data_buffer[i] = i;
		DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 27 SUB_ADD 4 WORD_CT 4
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<4; i++)
		data_buffer[i] = i;
		DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 14 SUB_ADD 15 WORD_CT 7
		bc_page.cmd_1.taddr   = 14;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 15;
		bc_page.cmd_1.wcnt    = 7;
		
		for(i=0; i<7; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 15 SUB_ADD 20 WORD_CT 32
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 20;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0166;
	
		DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 15 SUB_ADD 21 WORD_CT 32
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 21;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0167;
	
		DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 13 SUB_ADD 4 WORD_CT 32
		bc_page.cmd_1.taddr   = 13;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0168;
	
		DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 15 SUB_ADD 3 WORD_CT 24
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 24;
		
		for(i=0; i<24; i++)
	data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 4 WORD_CT 32
		
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 4;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 24 SUB_ADD 2 WORD_CT 32
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 16 SUB_ADD 26 WORD_CT 32
		bc_page.cmd_1.taddr   = 16;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 26;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 16;
	
		DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 15 SUB_ADD 2 WORD_CT 24
		bc_page.cmd_1.taddr   = 15;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 24;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0173;
	
		DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 2 SUB_ADD 11 WORD_CT 19
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 11;
		bc_page.cmd_1.wcnt    = 19;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0174;
	
		DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 24 SUB_ADD 3 WORD_CT 4
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<4; i++)
			data_buffer[i] = i;

	
		DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 27 SUB_ADD 2 WORD_CT 4
		bc_page.cmd_1.taddr   = 27;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 4;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0176;
	
		DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 22 SUB_ADD 2 WORD_CT 18
		bc_page.cmd_1.taddr   = 22;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 18;
		
		for(i=0; i<18; i++)
		data_buffer[i] = i;
		
	
		DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 24 SUB_ADD 8 WORD_CT 32
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 8;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0178;
	
		DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 2 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 2;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 2 SUB_ADD 3 WORD_CT 32
		bc_page.cmd_1.taddr   = 2;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 3;
		bc_page.cmd_1.wcnt    = 32;
		
		for(i=0; i<32; i++)
		data_buffer[i] = i;
	
		DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,32,CardId);
		//RT-->RT
//RT_ADD 24 SUB_ADD 10 WORD_CT 6
		bc_page.cmd_1.taddr   = 24;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 10;
		bc_page.cmd_1.wcnt    = 6;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0181;
	
		DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,32,CardId);
		//BC-->RT
//RT_ADD 7 SUB_ADD 28 WORD_CT 8
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;//
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 7;
		bc_page.cmd_1.t_r     = 0;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 28;
		bc_page.cmd_1.wcnt    = 8;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0182;
	
		DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,32,CardId);
		//RT-->BC
//RT_ADD 18 SUB_ADD 0 WORD_CT 2
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 18;
		bc_page.cmd_1.t_r     = 1;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 2;
		
		for(i=0; i<32; i++)
			data_buffer[i] = 0x0183;
	
		DefMsg(BcMod,BCSTACK_A,28,bc_page,data_buffer,32,CardId);
	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	frame_items[27]=28;



		DefFrame(BcMod,BCSTACK_A,28,frame_items,CardId);
		//SetBCEOMCnt(BcMod,0,CARD1);
		RunBc(BcMod,BCSTACK_A,1,28,1,CardId);
//  5th completed//************6th th MinorCycle Started*********/////////////////////////////////////////
		/***********module 2*****/

			RtMod=2;BcMod=2;MtMod=2;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);

bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x5000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);

	//BC--> RT RT_ADD 21 SUB_ADD 1 WORD_CT 14
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 31;
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<14; i++)
	data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
	
	DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 22 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<14; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 5 SUB_ADD 2 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
		data_buffer[24] = 0x0018;
		data_buffer[25] = 0x0019;
		data_buffer[26] = 0x001A;
		data_buffer[27] = 0x001B;
		data_buffer[28] = 0x001C;
		data_buffer[29] = 0x001D;
		data_buffer[30] = 0x001E;
		data_buffer[31] = 0x001F;
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,32,CARD1);

	//BC--> RT RT_ADD 25 SUB_ADD 1 WORD_CT 10
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 10;   
	for(i=0; i<10; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,10,CARD1);

	//BC--> RT RT_ADD 25 SUB_ADD 3 WORD_CT 6
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 6;   
	for(i=0; i<6; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
	
	
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,6,CARD1);

	//RT--> BC RT_ADD 11 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<14; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,14,CARD1);

	//RT--> BC RT_ADD 11 SUB_ADD 4 WORD_CT 7
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 7;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0127;
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,7,CARD1);

	//RT--> BC RT_ADD 21 SUB_ADD 4 WORD_CT 7
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 7;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0128;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,7,CARD1);

	//RT--> BC RT_ADD 22 SUB_ADD 4 WORD_CT 7
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 7;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0129;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,7,CARD1);

	//BC--> RT RT_ADD 16 SUB_ADD 4 WORD_CT 2
	bc_page.cmd_1.taddr   = 16;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<2; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
	
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,2,CARD1);

	//BC--> RT RT_ADD 16 SUB_ADD 2 WORD_CT 7
	bc_page.cmd_1.taddr   = 16;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 7;   
	for(i=0; i<7; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,7,CARD1);

	//RT--> RT RT_ADD 13 SUB_ADD 9 WORD_CT 4
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 9;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<4; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,4,CARD1);

	//RT--> RT RT_ADD 2 SUB_ADD 8 WORD_CT 32
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0133;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,32,CARD1);

	//BC--> RT RT_ADD 6 SUB_ADD 30 WORD_CT 1
	bc_page.cmd_1.taddr   = 6;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 30;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<32; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
		data_buffer[24] = 0x0018;
		data_buffer[25] = 0x0019;
		data_buffer[26] = 0x001A;
		data_buffer[27] = 0x001B;
		data_buffer[28] = 0x001C;
		data_buffer[29] = 0x001D;
		data_buffer[30] = 0x001E;
		data_buffer[31] = 0x001F;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,1,CARD1);

	//BC--> RT RT_ADD 10 SUB_ADD 7 WORD_CT 1
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<32; i++)
		data_buffer[0] = 0x0000;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,1,CARD1);

	//BC--> RT RT_ADD 22 SUB_ADD 2 WORD_CT 18
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 18;   
	for(i=0; i<18; i++)
	data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,18,CARD1);

	//RT--> RT RT_ADD 15 SUB_ADD 3 WORD_CT 24
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 24;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0137;
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,24,CARD1);

	//BC--> RT RT_ADD 10 SUB_ADD 3 WORD_CT 14
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<14; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
	
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,14,CARD1);

	//RT--> BC RT_ADD 25 SUB_ADD 2 WORD_CT 11
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 11;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0139;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,11,CARD1);

	//RT--> BC RT_ADD 25 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast	  =NO;
	bc_page.mode_code	  =YES;
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0140;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,2,CARD1);

	//RT--> BC RT_ADD 26 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast	  =NO;
	bc_page.mode_code	  =YES;
	bc_page.cmd_1.taddr   = 26;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0141;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,2,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;



	DefFrame(BcMod,BCSTACK_A,21,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,21,1,CardId);
	/**********module 3 ///////*/
		RtMod=3;BcMod=3;MtMod=3;
		//ClearRam(BcMod,BC,CardId);
		ConfigureBC(BcMod,128,CardId);


			bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x5000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);
	//RT--> BC RT_ADD 5 SUB_ADD 1 WORD_CT 17
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 31;
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<17;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT RT_ADD 5 SUB_ADD 30 WORD_CT 10
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 30;
	bc_page.cmd_1.wcnt    = 10;  
	for(i=0;i<10;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,10,CARD1);
	
	//BC--> RT RT_ADD 7 SUB_ADD 2 WORD_CT 32
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
		data_buffer[24] = 0x0018;
		data_buffer[25] = 0x0019;
		data_buffer[26] = 0x001A;
		data_buffer[27] = 0x001B;
		data_buffer[28] = 0x001C;
		data_buffer[29] = 0x001D;
		data_buffer[30] = 0x001E;
		data_buffer[31] = 0x001F;
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,32,CARD1);
	
	//BC--> RT RT_ADD 11 SUB_ADD 17 WORD_CT 15
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 17;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0;i<15;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
	
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,15,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 1 WORD_CT 12
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 12;  
	for(i=0;i<32;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
		data_buffer[24] = 0x0018;
		data_buffer[25] = 0x0019;
		data_buffer[26] = 0x001A;
		data_buffer[27] = 0x001B;
		data_buffer[28] = 0x001C;
		data_buffer[29] = 0x001D;
		data_buffer[30] = 0x001E;
		data_buffer[31] = 0x001F;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,12,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 15 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 15;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0131;
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 8 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 15;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0132;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 19 WORD_CT 23
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 19;
	bc_page.cmd_1.wcnt    = 23;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0133;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,23,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 27 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 27;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0134;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 11 SUB_ADD 8 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 14;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0135;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,14,CARD1);
	
	//RT--> RT RT_ADD 11 SUB_ADD 23 WORD_CT 32
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 23;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0136;
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,32,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 5 WORD_CT 15
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0;i<15;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,15,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 7 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0138;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 16 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0139;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CARD1);
	
	//RT--> RT RT_ADD 11 SUB_ADD 24 WORD_CT 17
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 24;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0140;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 3 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<17;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,17,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 16 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0142;
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,11,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 12 WORD_CT 31
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 12;
	bc_page.cmd_1.wcnt    = 31;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0143;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,31,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 9 WORD_CT 32
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 9;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0144;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,32,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 6 WORD_CT 32
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0145;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 5 WORD_CT 25
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 25;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0146;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,25,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 22 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 22;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0147;
	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 8 WORD_CT 9
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 9;  
	for(i=0;i<9;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		
	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,9,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 30 WORD_CT 10
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 30;
	bc_page.cmd_1.wcnt    = 10;  
	for(i=0;i<10;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		
	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,10,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 20 WORD_CT 4
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 20;
	bc_page.cmd_1.wcnt    = 4;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0150;
	DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,4,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 6 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0151;
	DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,11,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	


	DefFrame(BcMod,BCSTACK_A,27,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,27,1,CardId);

	}else if(Counter==7)// mc 6
		{	

	RtMod=1;BcMod=1;MtMod=1;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);
		///////*******Minor cycle 6 STARTED********////////
		//BC-->RT //RT_ADD 21 SUB_ADD 1 WORD_CT 14

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x6000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);


		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 21;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 1;
		bc_page.cmd_1.wcnt    = 14;  
		for(i=0; i<14; i++)
			data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,14,CARD1);
	//BC-->RT RT_ADD 22 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;  
	for(i=0; i<14; i++)
			data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 12 SUB_ADD 1 WORD_CT 2
	bc_page.cmd_1.taddr   = 12;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0186;

	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 11 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0187;

	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 13 SUB_ADD 1 WORD_CT 15
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 15;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0188;


	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,32,CARD1);
	//RT-->RT RT_ADD 2 SUB_ADD 14 WORD_CT 16
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 14;
	bc_page.cmd_1.wcnt    = 16; 
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0189;

	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 11 SUB_ADD 3 WORD_CT 2
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 2; 
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0190;

	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 27 SUB_ADD 2 WORD_CT 32
	bc_page.cmd_1.taddr   = 27;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 32; 
	for(i=0; i<32; i++)
		data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 15 SUB_ADD 4 WORD_CT 12
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 12;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0192;

	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 16 SUB_ADD 4 WORD_CT 12
	bc_page.cmd_1.taddr   = 16;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 12;
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0193;

	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 24 SUB_ADD 4 WORD_CT 7
	bc_page.cmd_1.taddr   = 24;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 7;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0194;

	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 24 SUB_ADD 9 WORD_CT 32
	bc_page.cmd_1.taddr   = 24;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 9;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0195;

	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 2 SUB_ADD 4 WORD_CT 32
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,32,CARD1);


	//RT-->BC RT_ADD 7 SUB_ADD 4 WORD_CT 9
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 9;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0197;

	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CARD1);


	//RT-->BC RT_ADD 21 SUB_ADD 4 WORD_CT 7
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 7;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0198;

	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,32,CARD1);

	
	//RT-->BC RT_ADD 22 SUB_ADD 4 WORD_CT 7
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 7;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0199;

	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,32,CARD1);


	//RT-->BC RT_ADD 27 SUB_ADD 6 WORD_CT 6
	bc_page.cmd_1.taddr   = 27;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 6;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0200;

	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,32,CARD1);


	//RT-->BC RT_ADD 9 SUB_ADD 4 WORD_CT 3
	bc_page.cmd_1.taddr   = 9;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 3;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0201;

	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,32,CARD1);


	//RT-->BC RT_ADD 14 SUB_ADD 4 WORD_CT 16
	bc_page.cmd_1.taddr   = 14;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 16;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0202;

	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,32,CARD1);


	//RT-->RT RT_ADD 15 SUB_ADD 4 WORD_CT 25
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 25;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0203;

	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,32,CARD1);


	//BC-->RT RT_ADD 15 SUB_ADD 5 WORD_CT 25
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 25;  
	for(i=0; i<25; i++)
		data_buffer[i] = i;
		


	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 21 SUB_ADD 2 WORD_CT 18
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 18;  
	for(i=0; i<18; i++)
		data_buffer[i] = i;
	

	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,32,CARD1);
	//RT--> BC RT_ADD 24 SUB_ADD 6 WORD_CT 32
	bc_page.cmd_1.taddr   = 24;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0206;


	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,32,CARD1);
	//BC-->RT  RT_ADD 2 SUB_ADD 2 WORD_CT 32
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,32,CARD1);
	


	//BC-->RT  RT_ADD 2 SUB_ADD 3 WORD_CT 32
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,32,CARD1);
	//RT-->BC  RT_ADD 17 SUB_ADD 25 WORD_CT 5
	bc_page.cmd_1.taddr   = 17;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 25;
	bc_page.cmd_1.wcnt    = 5;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0210;

		DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,32,CARD1);


	//RT-->RT 	RT_ADD 27 SUB_ADD 5 WORD_CT 2
	bc_page.cmd_1.taddr   = 27;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0211;

	DefMsg(BcMod,BCSTACK_A,28,bc_page,data_buffer,32,CARD1);
	//RT-->BC  RT_ADD 27 SUB_ADD 4 WORD_CT 4
	bc_page.cmd_1.taddr   = 27;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 4;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0212;


	DefMsg(BcMod,BCSTACK_A,29,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 9 SUB_ADD 26 WORD_CT 32
	bc_page.cmd_1.taddr   = 9;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 26;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,30,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 21 SUB_ADD 0 WORD_CT 2
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = YES;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0214;

	DefMsg(BcMod,BCSTACK_A,31,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 22 SUB_ADD 0 WORD_CT 2
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = YES;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0215;

	DefMsg(BcMod,BCSTACK_A,32,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 27 SUB_ADD 0 WORD_CT 2
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = YES;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 27;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0216;
	 
	DefMsg(BcMod,BCSTACK_A,33,bc_page,data_buffer,32,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	frame_items[27]=28;
	frame_items[28]=29;
	frame_items[29]=30;
	frame_items[30]=31;
	frame_items[31]=32;
	frame_items[32]=33;
	frame_items[33]=34;


		DefFrame(BcMod,BCSTACK_A,34,frame_items,CardId);
		//SetBCEOMCnt(BcMod,0,CARD1);
		RunBc(BcMod,BCSTACK_A,1,34,1,CardId);
//  6th completed//************7th th MinorCycle Started*********/////////////////////////////////////////
		/***********module 2*****/

			RtMod=2;BcMod=2;MtMod=2;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x6000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);
	//BC--> RT RT_ADD 21 SUB_ADD 1 WORD_CT 14
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<14; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,14,CARD1);


	//BC--> RT RT_ADD 22 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<14; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,14,CARD1);


	//RT--> BC RT_ADD 5 SUB_ADD 4 WORD_CT 6
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 6;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0144;
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,6,CARD1);


	//BC--> RT RT_ADD 25 SUB_ADD 1 WORD_CT 10
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 10;   
	for(i=0; i<10; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,10,CARD1);

//	RT--> BC RT_ADD 7 SUB_ADD 4 WORD_CT 7
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 7;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0146;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,7,CARD1);


//	BC--> RT RT_ADD 25 SUB_ADD 3 WORD_CT 6
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 6;   
	for(i=0; i<6; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,6,CARD1);

	//RT--> BC RT_ADD 11 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0148;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,14,CARD1);


	//RT--> BC RT_ADD 15 SUB_ADD 4 WORD_CT 12
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 12;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0149;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,12,CARD1);


	//RT--> RT RT_ADD 15 SUB_ADD 5 WORD_CT 25
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 25;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0150;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,25,CARD1);


	//RT--> BC RT_ADD 6 SUB_ADD 1 WORD_CT 6
	bc_page.cmd_1.taddr   = 6;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 6;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0151;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,6,CARD1);


	//RT--> BC RT_ADD 5 SUB_ADD 2 WORD_CT 4
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0152;
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,4,CARD1);


	//RT--> BC RT_ADD 16 SUB_ADD 4 WORD_CT 12
	bc_page.cmd_1.taddr   = 16;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 12;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0153;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,12,CARD1);


	//RT--> BC RT_ADD 16 SUB_ADD 26 WORD_CT 32
	bc_page.cmd_1.taddr   = 16;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 26;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0154;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,32,CARD1);


	//RT--> RT RT_ADD 2 SUB_ADD 11 WORD_CT 19
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 11;
	bc_page.cmd_1.wcnt    = 19;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0155;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,19,CARD1);



	//BC--> RT RT_ADD 15 SUB_ADD 4 WORD_CT 25
	bc_page.cmd_1.taddr   = 15;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 25;   
	for(i=0; i<25; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
		data_buffer[24] = 0x0018;
		
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,25,CARD1);


	//BC--> RT RT_ADD 25 SUB_ADD 9 WORD_CT 5
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 9;
	bc_page.cmd_1.wcnt    = 5;   
	for(i=0; i<5; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
	
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,5,CARD1);


	//BC--> RT RT_ADD 10 SUB_ADD 7 WORD_CT 1
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<1; i++)
		data_buffer[0] = 0x0000;
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,1,CARD1);

//	BC--> RT RT_ADD 21 SUB_ADD 2 WORD_CT 18
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 18;   
	for(i=0; i<18; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
	
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,18,CARD1);

	//RT--> BC RT_ADD 13 SUB_ADD 1 WORD_CT 32
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0160;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,32,CARD1);

	//BC--> RT RT_ADD 26 SUB_ADD 2 WORD_CT 5
	bc_page.cmd_1.taddr   = 26;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 5;   
	for(i=0; i<5; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
	
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,5,CARD1);

	//RT--> BC RT_ADD 10 SUB_ADD 4 WORD_CT 7
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 7;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0162;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,7,CARD1);

	//RT--> BC RT_ADD 25 SUB_ADD 2 WORD_CT 11
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 11;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0163;
	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,11,CARD1);

	//RT--> BC RT_ADD 13 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast	  =	NO;
	bc_page.mode_code	  =	YES;
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0164;
	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,2,CARD1);

	//RT--> BC RT_ADD 10 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast	  =	NO;
	bc_page.mode_code	  =	YES;
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0165;
	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,2,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;

	


	DefFrame(BcMod,BCSTACK_A,25,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,25,1,CardId);

	/**********module 3 ///////*/
		RtMod=3;BcMod=3;MtMod=3;
		//ClearRam(BcMod,BC,CardId);
		ConfigureBC(BcMod,128,CardId);

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x6000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);

	//RT--> BC RT_ADD 5 SUB_ADD 1 WORD_CT 17
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0152;
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT RT_ADD 5 SUB_ADD 26 WORD_CT 7
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 26;
	bc_page.cmd_1.wcnt    = 7;  
	for(i=0;i<7;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,7,CARD1);
	
	//RT--> BC RT_ADD 7 SUB_ADD 4 WORD_CT 6
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 6;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0154;
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,6,CARD1);
	
	//BC--> RT RT_ADD 11 SUB_ADD 17 WORD_CT 15
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 17;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0;i<15;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,15,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 1 WORD_CT 12
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 12;  
	for(i=0;i<12;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,12,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 15 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 15;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0157;
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,30,CARD1);
												
    //RT--> BC RT_ADD 3 SUB_ADD 8 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0158;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,30,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 19 WORD_CT 23
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 19;
	bc_page.cmd_1.wcnt    = 23;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0159;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,23,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 1 WORD_CT 16
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 16;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0160;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,16,CARD1);
	
	//RT--> BC RT_ADD 11 SUB_ADD 8 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 14;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0161;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,14,CARD1);
	
	//RT--> RT RT_ADD 11 SUB_ADD 23 WORD_CT 32
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 23;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0162;
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,32,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 7 WORD_CT 22
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 22;  
	for(i=0;i<22;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
	
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,22,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 7 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0164;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,30,CARD1);
		
	//RT--> BC RT_ADD 7 SUB_ADD 2 WORD_CT 4
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 4;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0165;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,4,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 16 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0166;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,32,CARD1);
	
	//RT--> RT RT_ADD 11 SUB_ADD 24 WORD_CT 17
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 24;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0167;
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 3 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<17;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,17,CARD1);
		
	//RT--> BC RT_ADD 3 SUB_ADD 16 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0168;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,11,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 6 WORD_CT 32
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0169;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 5 WORD_CT 25
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 25;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0170;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,25,CARD1);
	
	//RT--> RT RT_ADD 3 SUB_ADD 22 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 22;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0171;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT RT_ADD 3 SUB_ADD 8 WORD_CT 9
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 9;  
	for(i=0;i<9;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		
	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,9,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 6 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0173;
	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,11,CARD1);
	
	//RT--> BC RT_ADD 3 SUB_ADD 19 WORD_CT 32
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 19;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0174;
	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 2 WORD_CT 16
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 16;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0175;
	DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,16,CARD1);
	
	//RT--> BC RT_ADD 14 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast     = NO;
	bc_page.mode_code     = YES;
	bc_page.cmd_1.taddr   = 14;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0176;
	DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,2,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	


	DefFrame(BcMod,BCSTACK_A,27,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,27,1,CardId);

	}else if(Counter==8)// mc 7
		{

	

	RtMod=1;BcMod=1;MtMod=1;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);

	//////////******Minor Cycle 7 STARTED*******/

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x7000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);




	//BC-->RT RT_ADD 21 SUB_ADD 1 WORD_CT 14
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;  
	for(i=0; i<14; i++)
		data_buffer[i] = i;
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 22 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;  
	for(i=0; i<14; i++)
			data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 12 SUB_ADD 1 WORD_CT 2
	bc_page.cmd_1.taddr   = 12;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0219;

	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 11 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0220;

	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 13 SUB_ADD 1 WORD_CT 15
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x02221;

	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 2 SUB_ADD 1 WORD_CT 32
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = i;
		

	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,32,CARD1);
	//RT-->RT RT_ADD 2 SUB_ADD 12 WORD_CT 13
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 12;
	bc_page.cmd_1.wcnt    = 13;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0223;

	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 27 SUB_ADD 4 WORD_CT 4
	bc_page.cmd_1.taddr   = 27;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 4;  
	for(i=0; i<4; i++)
		data_buffer[i] = i;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,32,CARD1);
	//RT-->RT RT_ADD 2 SUB_ADD 13 WORD_CT 13
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 13;
	bc_page.cmd_1.wcnt    = 13;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0225;

	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 22 SUB_ADD 1 WORD_CT 1
	bc_page.cmd_1.taddr   = 12;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 1;  
	for(i=0; i<1; i++)
		data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 24 SUB_ADD 3 WORD_CT 32
	bc_page.cmd_1.taddr   = 24;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0227;

	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 2 SUB_ADD 4 WORD_CT 32
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 24 SUB_ADD 2 WORD_CT 3
	bc_page.cmd_1.taddr   = 24;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 3;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0229;

	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 2 SUB_ADD 7 WORD_CT 32
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 12 SUB_ADD 30 WORD_CT 1
	bc_page.cmd_1.taddr   = 12;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 30;
	bc_page.cmd_1.wcnt    = 1;  
	for(i=0; i<1; i++)
		data_buffer[i] = 0x000F;

	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,32,CARD1);
	//RT-->RT RT_ADD 2 SUB_ADD 14 WORD_CT 12
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 14;
	bc_page.cmd_1.wcnt    = 12;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0232;

	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 13 SUB_ADD 7 WORD_CT 12
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 12;  
	for(i=0; i<12; i++)
		data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 27 SUB_ADD 2 WORD_CT 4
	bc_page.cmd_1.taddr   = 27;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 3;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0234;

	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 22 SUB_ADD 2 WORD_CT 18
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 18;  
	for(i=0; i<18; i++)
	data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 24 SUB_ADD 1 WORD_CT 22
	bc_page.cmd_1.taddr   = 24;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 22;  
	for(i=0; i<22; i++)
		data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 2 SUB_ADD 2 WORD_CT 32
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = i;

	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 2 SUB_ADD 3 WORD_CT 32
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = i;
	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 17 SUB_ADD 4 WORD_CT 5
	bc_page.cmd_1.taddr   = 17;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 5;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0239;

	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 18 SUB_ADD 4 WORD_CT 5
	bc_page.cmd_1.taddr   = 18;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 5;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0240;

	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,32,CARD1);
	//RT-->RT RT_ADD 24 SUB_ADD 10 WORD_CT 6
	bc_page.cmd_1.taddr   = 24;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 10;
	bc_page.cmd_1.wcnt    = 6;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0241;


	DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,32,CARD1);
	//BC-->RT RT_ADD 2 SUB_ADD 19 WORD_CT 32
	bc_page.cmd_1.taddr   = 2;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 19;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0; i<32; i++)
		data_buffer[i] = i;
	DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 9 SUB_ADD 0 WORD_CT 2
	bc_page.cmd_1.taddr   = 9;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0243;

	DefMsg(BcMod,BCSTACK_A,28,bc_page,data_buffer,32,CARD1);
	//RT-->BC RT_ADD 14 SUB_ADD 0 WORD_CT 2
	bc_page.cmd_1.taddr   = 14;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;  
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0244;
	DefMsg(BcMod,BCSTACK_A,29,bc_page,data_buffer,32,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;
	frame_items[21]=22;
	frame_items[22]=23;
	frame_items[23]=24;
	frame_items[24]=25;
	frame_items[25]=26;
	frame_items[26]=27;
	frame_items[27]=28;
	frame_items[28]=29;


		DefFrame(BcMod,BCSTACK_A,29,frame_items,CardId);
		//SetBCEOMCnt(BcMod,0,CARD1);
		RunBc(BcMod,BCSTACK_A,1,29,1,CardId);

	/***********module 2*****/
		RtMod=2;BcMod=2;MtMod=2;
	//ClearRam(BcMod,BC,CardId);
	ConfigureBC(BcMod,128,CardId);


		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x7000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);

	//BC--> RT RT_ADD 21 SUB_ADD 1 WORD_CT 14
	bc_page.Int_Msg_Gap   = 10;
	bc_page.bus           = CH_A;
	bc_page.self_test	  = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = NO;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
    bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<14; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 22 SUB_ADD 1 WORD_CT 14
    bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<14; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,14,CARD1);

	//BC--> RT RT_ADD 25 SUB_ADD 1 WORD_CT 10
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 10;   
	for(i=0; i<10; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,10,CARD1);
	
	//RT--> BC RT_ADD 6 SUB_ADD 4 WORD_CT 5
	bc_page.cmd_1.taddr   = 6;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 5;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0169;
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,5,CARD1);

	//RT--> BC RT_ADD 13 SUB_ADD 4 WORD_CT 32
	bc_page.cmd_1.taddr   = 13;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0170;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,32,CARD1);

	//BC--> RT RT_ADD 25 SUB_ADD 3 WORD_CT 6
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 6;   
	for(i=0; i<6; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,6,CARD1);
	
	//RT--> BC RT_ADD 11 SUB_ADD 1 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 14;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0172;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,14,CARD1);
	
	//RT--> BC RT_ADD 5 SUB_ADD 1 WORD_CT 7
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 7;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0173;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,7,CARD1);
	
	//RT--> BC RT_ADD 6 SUB_ADD 2 WORD_CT 2
	bc_page.cmd_1.taddr   = 6;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0174;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,2,CARD1);
	
	//BC--> RT RT_ADD 5 SUB_ADD 2 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 32;   
	for(i=0; i<32; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
		data_buffer[24] = 0x0018;
		data_buffer[25] = 0x0019;
		data_buffer[26] = 0x001A;
		data_buffer[27] = 0x001B;
		data_buffer[28] = 0x001C;
		data_buffer[29] = 0x001D;
		data_buffer[30] = 0x001E;
		data_buffer[31] = 0x001F;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,32,CARD1);
	
	//RT--> BC RT_ADD 22 SUB_ADD 3 WORD_CT 12
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 12;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0176;
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,12,CARD1);
	
	//RT--> BC RT_ADD 21 SUB_ADD 3 WORD_CT 12
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 12;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0177;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,12,CARD1);
	//BC--> RT RT_ADD 21 SUB_ADD 6 WORD_CT 2
	bc_page.cmd_1.taddr   = 21;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<2; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,2,CARD1);
	
	//BC--> RT RT_ADD 22 SUB_ADD 6 WORD_CT 2
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<2; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;

	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,2,CARD1);

	//BC--> RT RT_ADD 10 SUB_ADD 7 WORD_CT 1
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 1;   
	for(i=0; i<1; i++)
		data_buffer[0] = 0x0000;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,1,CARD1);
	
	//BC--> RT RT_ADD 22 SUB_ADD 2 WORD_CT 18
	bc_page.cmd_1.taddr   = 22;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 18;   
	for(i=0; i<18; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
	
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,18,CARD1);
	 
	//BC--> RT RT_ADD 10 SUB_ADD 10 WORD_CT 10
	bc_page.cmd_1.taddr   = 10;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 10;
	bc_page.cmd_1.wcnt    = 10;   
	for(i=0; i<10; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
	
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,10,CARD1);
	
	//RT--> BC RT_ADD 25 SUB_ADD 2 WORD_CT 11
	bc_page.cmd_1.taddr   = 25;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 11;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0183;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,11,CARD1);

	//BC--> RT RT_ADD 5 SUB_ADD 23 WORD_CT 4
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 23;
	bc_page.cmd_1.wcnt    = 4;   
	for(i=0; i<4; i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
	
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,4,CARD1);

	//RT--> BC RT_ADD 7 SUB_ADD 0 WORD_CT 2
	bc_page.broadcast = NO;
	bc_page.mode_code = YES;
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 0;
	bc_page.cmd_1.wcnt    = 2;   
	for(i=0; i<32; i++)
		data_buffer[i] = 0x0185;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,2,CARD1);

	frame_items[0]=1;
	frame_items[1]=2;
	frame_items[2]=3;
	frame_items[3]=4;
	frame_items[4]=5;
	frame_items[5]=6;
	frame_items[6]=7;
	frame_items[7]=8;
	frame_items[8]=9;
	frame_items[9]=10;
	frame_items[10]=11;
	frame_items[11]=12;
	frame_items[12]=13;
	frame_items[13]=14;
	frame_items[14]=15;
	frame_items[15]=16;
	frame_items[16]=17;
	frame_items[17]=18;
	frame_items[18]=19;
	frame_items[19]=20;
	frame_items[20]=21;




	DefFrame(BcMod,BCSTACK_A,21,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,21,1,CardId);
		
	/**********module 3 ///////*/
		RtMod=3;BcMod=3;MtMod=3;
		//ClearRam(BcMod,BC,CardId);
		ConfigureBC(BcMod,128,CardId);

		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = YES;
		bc_page.mode_code     = YES;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
		bc_page.cmd_1.t_r     = RECEIVE;
		bc_page.cmd_1.status  = 0xcccc;
		bc_page.loop          = 0x0000;
		bc_page.cmd_1.subaddr = 0;
		bc_page.cmd_1.wcnt    = 17;
		for(i=0; i<32; i++)
			data_buffer[i] = 0x7000;
	
		DefMsg(BcMod,BCSTACK_A,1,bc_page,data_buffer,17,CardId);
	
										
//RT--> BC RT_ADD 5 SUB_ADD 1 WORD_CT 17
		bc_page.Int_Msg_Gap   = 10;
		bc_page.bus           = CH_A;
		bc_page.self_test	  = NO;
		bc_page.mask_bcst_bit = NO;
		bc_page.broadcast     = NO;
		bc_page.mode_code     = NO;
		bc_page.rt_rt         = NO;
		bc_page.cmd_1.taddr   = 31;
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0177;
	DefMsg(BcMod,BCSTACK_A,2,bc_page,data_buffer,17,CARD1);
	
	//BC--> RT 	RT_ADD 5 SUB_ADD 30 WORD_CT 10
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 30;
	bc_page.cmd_1.wcnt    = 10;  
	for(i=0;i<10;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
	
	DefMsg(BcMod,BCSTACK_A,3,bc_page,data_buffer,10,CARD1);

	//BC--> RT 	RT_ADD 11 SUB_ADD 17 WORD_CT 15
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 17;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0;i<15;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		
	DefMsg(BcMod,BCSTACK_A,4,bc_page,data_buffer,15,CARD1);

	//BC--> RT 	RT_ADD 3 SUB_ADD 1 WORD_CT 12
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 12;  
	for(i=0;i<12;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		
	DefMsg(BcMod,BCSTACK_A,5,bc_page,data_buffer,12,CARD1);

	//RT--> BC 	RT_ADD 3 SUB_ADD 15 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 15;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0181;
	DefMsg(BcMod,BCSTACK_A,6,bc_page,data_buffer,30,CARD1);

	//RT--> BC RT_ADD 3 SUB_ADD 8 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0182;
	DefMsg(BcMod,BCSTACK_A,7,bc_page,data_buffer,30,CARD1);

	//RT--> BC RT_ADD 5 SUB_ADD 19 WORD_CT 23
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 19;
	bc_page.cmd_1.wcnt    = 23;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0183;
	DefMsg(BcMod,BCSTACK_A,8,bc_page,data_buffer,23,CARD1);

	//RT--> BC RT_ADD 7 SUB_ADD 1 WORD_CT 7
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 7;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0184;
	DefMsg(BcMod,BCSTACK_A,9,bc_page,data_buffer,7,CARD1);

	//RT--> BC RT_ADD 11 SUB_ADD 8 WORD_CT 14
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 14;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0185;
	DefMsg(BcMod,BCSTACK_A,10,bc_page,data_buffer,14,CARD1);

	//RT--> RT RT_ADD 11 SUB_ADD 23 WORD_CT 32
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 23;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0186;
	DefMsg(BcMod,BCSTACK_A,11,bc_page,data_buffer,32,CARD1);

	//BC--> RT RT_ADD 3 SUB_ADD 5 WORD_CT 15
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 15;  
	for(i=0;i<15;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		
	DefMsg(BcMod,BCSTACK_A,12,bc_page,data_buffer,15,CARD1);

	//RT--> BC RT_ADD 3 SUB_ADD 4 WORD_CT 13
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 4;
	bc_page.cmd_1.wcnt    = 13;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0188;
	DefMsg(BcMod,BCSTACK_A,13,bc_page,data_buffer,13,CARD1);

	//RT--> BC RT_ADD 3 SUB_ADD 7 WORD_CT 30
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 7;
	bc_page.cmd_1.wcnt    = 30;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0189;
	DefMsg(BcMod,BCSTACK_A,14,bc_page,data_buffer,30,CARD1);

	//BC--> RT RT_ADD 7 SUB_ADD 2 WORD_CT 32
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 2;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		data_buffer[17] = 0x0011;
		data_buffer[18] = 0x0012;
		data_buffer[19] = 0x0013;
		data_buffer[20] = 0x0014;
		data_buffer[21] = 0x0015;
		data_buffer[22] = 0x0016;
		data_buffer[23] = 0x0017;
		data_buffer[24] = 0x0018;
		data_buffer[25] = 0x0019;
		data_buffer[26] = 0x001A;
		data_buffer[27] = 0x001B;
		data_buffer[28] = 0x001C;
		data_buffer[29] = 0x001D;
		data_buffer[30] = 0x001E;
		data_buffer[31] = 0x001F;
	DefMsg(BcMod,BCSTACK_A,15,bc_page,data_buffer,32,CARD1);

	//RT--> BC RT_ADD 5 SUB_ADD 16 WORD_CT 32
	bc_page.cmd_1.taddr   = 5;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0191;
	DefMsg(BcMod,BCSTACK_A,16,bc_page,data_buffer,32,CARD1);

	//RT--> RT RT_ADD 11 SUB_ADD 24 WORD_CT 17
	bc_page.cmd_1.taddr   = 11;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 24;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0192;
	DefMsg(BcMod,BCSTACK_A,17,bc_page,data_buffer,17,CARD1);

	//BC--> RT RT_ADD 3 SUB_ADD 3 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 3;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<17;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
		data_buffer[10] =0x000A;
		data_buffer[11] =0x000B;
		data_buffer[12] =0x000C;
		data_buffer[13] =0x000D;
		data_buffer[14] =0x000E; 
		data_buffer[15] =0x000F; 
		data_buffer[16] = 0x0010;
		
	DefMsg(BcMod,BCSTACK_A,18,bc_page,data_buffer,17,CARD1);

	//RT--> BC RT_ADD 3 SUB_ADD 16 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 16;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0194;
	DefMsg(BcMod,BCSTACK_A,19,bc_page,data_buffer,11,CARD1);

	//RT--> RT RT_ADD 3 SUB_ADD 6 WORD_CT 32
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 32;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0195;
	DefMsg(BcMod,BCSTACK_A,20,bc_page,data_buffer,32,CARD1);

	//RT--> BC RT_ADD 3 SUB_ADD 5 WORD_CT 25
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 5;
	bc_page.cmd_1.wcnt    = 25;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0196;
	DefMsg(BcMod,BCSTACK_A,21,bc_page,data_buffer,25,CARD1);


	//RT--> RT RT_ADD 3 SUB_ADD 22 WORD_CT 17
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 22;
	bc_page.cmd_1.wcnt    = 17;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0197;
	DefMsg(BcMod,BCSTACK_A,22,bc_page,data_buffer,17,CARD1);

	//BC--> RT RT_ADD 3 SUB_ADD 8 WORD_CT 9
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 8;
	bc_page.cmd_1.wcnt    = 9;  
	for(i=0;i<9;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		
	DefMsg(BcMod,BCSTACK_A,23,bc_page,data_buffer,9,CARD1);

	//RT--> RT RT_ADD 3 SUB_ADD 21 WORD_CT 16
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 21;
	bc_page.cmd_1.wcnt    = 16;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0199;
	DefMsg(BcMod,BCSTACK_A,24,bc_page,data_buffer,16,CARD1);

	//BC--> RT RT_ADD 3 SUB_ADD 30 WORD_CT 10
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 30;
	bc_page.cmd_1.wcnt    = 10;  
	for(i=0;i<10;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		data_buffer[4] = 0x0004;
		data_buffer[5] = 0x0005;
		data_buffer[6] = 0x0006;
		data_buffer[7] = 0x0007;
		data_buffer[8] = 0x0008;
		data_buffer[9] = 0x0009;
	
	DefMsg(BcMod,BCSTACK_A,25,bc_page,data_buffer,10,CARD1);

	//RT--> BC RT_ADD 3 SUB_ADD 6 WORD_CT 11
	bc_page.cmd_1.taddr   = 3;
	bc_page.cmd_1.t_r     = TRANSMIT;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 6;
	bc_page.cmd_1.wcnt    = 11;  
	for(i=0;i<32;i++)
		data_buffer[i] = 0x0201;
	DefMsg(BcMod,BCSTACK_A,26,bc_page,data_buffer,11,CARD1);

	//BC--> RT RT_ADD 7 SUB_ADD 23 WORD_CT 4
	bc_page.cmd_1.taddr   = 7;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 23;
	bc_page.cmd_1.wcnt    = 4;  
	for(i=0;i<4;i++)
		data_buffer[0] = 0x0000;
		data_buffer[1] = 0x0001;
		data_buffer[2] = 0x0002;
		data_buffer[3] = 0x0003;
		
	DefMsg(BcMod,BCSTACK_A,27,bc_page,data_buffer,4,CARD1);

frame_items[0] = 1; 
frame_items[1] = 2; 
frame_items[2] = 3; 
frame_items[3] = 4; 
frame_items[4] = 5; 
frame_items[5] = 6; 
frame_items[6] = 7; 
frame_items[7] = 8; 
frame_items[8] = 9; 
frame_items[9] = 10; 
frame_items[10] = 11; 
frame_items[11] = 12; 
frame_items[12] = 13; 
frame_items[13] = 14; 
frame_items[14] = 15; 
frame_items[15] = 16; 
frame_items[16] = 17; 
frame_items[17] = 18; 
frame_items[18] = 19; 
frame_items[19] = 20; 
frame_items[20] = 21; 
frame_items[21] = 22; 
frame_items[22] = 23; 
frame_items[23] = 24; 
frame_items[24] = 25; 
frame_items[25] = 26; 
frame_items[26] = 27; 



	DefFrame(BcMod,BCSTACK_A,27,frame_items,CardId);
	RunBc(BcMod,BCSTACK_A,1,27,1,CardId);

	}//////////if completed



while(GetTMR2Cnt (CARD1) < 20);
	printf("mod 1 Cyc num-> %d  Cnt->%d\t",Counter,GetBCEOMCnt(1,CARD1));
	printf("mod 2 Cyc num-> %d  Cnt->%d\t",Counter,GetBCEOMCnt(2,CARD1));
	printf("mod 3 Cyc num-> %d  Cnt->%d\n",Counter,GetBCEOMCnt(3,CARD1));
	Counter=(Counter%8)+1;

//printf ("\n\tTimer2 Counter -> %d\n", GetTMR2Cnt (CARD1));
//printf ("\n\tTimer Interrupt status -> %x\n", GetTimerIntrStatus (CARD1));
	}// for loop completed


//while(GetTMR2Cnt (CARD1) < 20);
//printf ("\n\tlast 2 Counter -> %d\n", GetTMR2Cnt (CARD1));
//printf ("\n\tTimer Interrupt status -> %x\n", GetTimerIntrStatus (CARD1));
		printf("\n\t\t\t\t\tBC run has been stopped \n");
		getchar();
		//getchar();
		//BCAutoRepeat(BcMod,NO,CardId);
		
		printf("\n \t1 Cnt-- %d\t\t\t",GetBCEOMCnt(1,CARD1));
		printf(" \t2 Cnt--%d\t\t\t",GetBCEOMCnt(2,CARD1));
		printf("\t3 Cnt--%d",GetBCEOMCnt(3,CARD1));
		getchar();
		getchar();


/*
    ReadMsgBC(BcMod,BCSTACK_A,1,&msg_return,CardId);	
	printf("***************	BC Results *************\n");
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
		printf("%x ",msg_return.data[i]);
	
	printf("\nLOOPBACK WORD      = %x \n",msg_return.loop_word);
	printf("STATUS             = %x \n",msg_return.status_1);
	getchar();
		
	ReadMsgRT(RtMod,1,1,&RtBuff,CardId);     
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
		printf("%x ",RtBuff.data[i]);
		getchar();		
		
		ReadMonPacket(MtMod,MTSTACK_A,1,&buffer,CardId);
		printf("***************  MT Results *************\n");
		printf("\nBlock Status Word -->> 	%x",buffer.BlockSts.value);
		printf("\nCMD WORD CONTENTS ERR		=	%d",buffer.BlockSts.bitwise.CmdWrdErr);
		printf("\nRT-RT 2ND CMD ERR		=	%d",buffer.BlockSts.bitwise.RtRtSecCmd );
		//printf("\nRT-RT GAP/SYNC/ADR ERR		=	%d",buffer.BlockSts.bitwise.GapSycAdr );
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
	if (temp==0) temp=32;
    
	printf("\nCmd Word  -->>  %x \n",buffer.RecCmdWrd.value);
	printf("\nRT Status Word  -->>  %x \n",buffer.RecStatus);	
	for(i=0;i<temp;i++)
		printf("%x ",buffer.DataPack[i]); */
		
	getchar();
	
	ResetBc(BcMod,CardId);
	ResetMt(MtMod,CardId);

	MemWrite(RtMod,0x200a,0x0,CardId);
	MemWrite(RtMod,0x200b,0x0,CardId);
	RegWrite(RtMod,0x2d,0x1,CardId);	

	//end of rtno
	
	DisableInterrupts(MtMod,CardId);		
	DisableInterrupts(BcMod,CardId);
	DisableInterrupts(RtMod,CardId);
	printf("\t**********************************************OVER*************************************************\n");
 	getchar();
/*	for(i=0x50;i<=0x7ff;i++)

		{Sleep(2);
		       	MemWrite(3,j,i,CardId);
				printf("memRead-->%x value of i-->%x\n",i,MemRead(3,j,CardId));
		}				
	*/
	CloseDeviceHandle(CardId);		
		
} //close for main


