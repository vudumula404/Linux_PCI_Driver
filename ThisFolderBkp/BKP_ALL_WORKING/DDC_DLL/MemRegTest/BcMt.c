#include <stdio.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"


MSG_DEF_DDC	bc_page;
RT_MSG_DDC	RtBuff;
MSG_RESULT_DDC	msg_return;
unsigned short	frame_items[10],data_buffer[40],temp_data;
unsigned short	Temp_Buff[40];
void ReadMonPacket(UCHAR ModId,unsigned short DespNo,MNTPACK_DDC *MontDataBuff)
{

	unsigned short Offset,i,ptr,readp=0;
	unsigned short temp;
	
	Offset 				= 	( 0x400 + ((DespNo * 4) - 4));
	/*for(readp = 0;readp<0xfff;readp++)
	  printf("data %x address %d\n",MemRead(ModId,readp,1),readp);*/
	MontDataBuff->BlockSts.value	=	MemRead(ModId,Offset,1);
	/*printf("Bsw %x Mem%x ModId: %d DespNo %d Offset %d\n",MontDataBuff->BlockSts.value,MemRead(ModId,Offset,1),ModId,DespNo,Offset);*/
	MontDataBuff->TimeTag 		=	MemRead(ModId,Offset+1,1);
	//printf("TimeTag %x\n",MontDataBuff->TimeTag);
	MontDataBuff->DataBlkPtr 	=	MemRead(ModId,Offset+2,1);
	//printf("DBLK %x\n",MontDataBuff->DataBlkPtr );
	ptr 				= 	MontDataBuff->DataBlkPtr;
	MontDataBuff->RecCmdWrd.value	=	MemRead(ModId,Offset+3,1);
	temp				=	MontDataBuff->RecCmdWrd.value;
	temp				=	MontDataBuff->RecCmdWrd.bitwise.WrdCntOrMde;
	if(temp==0) temp=32;

	if( MontDataBuff->RecCmdWrd.bitwise.TrnRecv == 1 )
	{
		ptr++;
		if(ptr==0x1000)
		{
			ptr=0x800;
		}
		for(i=0; i < temp;i++)
		{
			MontDataBuff->DataPack[i] =   MemRead(ModId,ptr,1);
			ptr=ptr+1;
			if(ptr==0x1000)
			{
				ptr=0x800;
			}
		}
	}

	else if( MontDataBuff->RecCmdWrd.bitwise.TrnRecv == 0 )
	{
		for(i=0; i < temp;i++)
		{
			MontDataBuff->DataPack[i] =   MemRead(ModId,ptr,1);
			if(ptr==0x1000)
			{
				ptr=0x800;
			}
			else
				ptr= ptr+1;
		}

	}
}
void SelMsgMt(UCHAR ModId)
{
	//extern unsigned short MsgMonStkPtr;
	unsigned short	i;
	
	RegWrite(ModId,CONFIG_REG3_DDC,0x8000,1);
	RegWrite(ModId,CONFIG_REG3_DDC,0x8500,1);
	RegWrite(ModId,CONFIG_REG_DDC,0x5000,1);
	MemWrite(ModId,0x102,0x400,1);				//CMD word Stk Ptr//0x400
	MemWrite(ModId,0x103,0x800,1);				//Data word Stk Ptr//0x800
	for(i=0; i < 128; i++)
	{
		MemWrite(ModId,0x280+i,0xffff,1);
	}
	//MsgMonStkPtr = 0x400;
	//return;
}

/*******************************************************************
*  This Program gives code for BC and MT 
********************************************************************/
int main()
{
        MNTPACK_DDC buffer;
	static		int 	CCSC_FD		=	0x0;
	int			i,MemAdr=0,Ch=0x0,TempCnt,RetVal=0x0,Times=0x1,POS1=0;
	unsigned short		MemBuff,MsgNo=0,Data=0;
	unsigned char		CardId,FailCount=0,ModId=0,MTModId=3,Com_Or_MemTest=0;
	char temp;
       
	i = InitCard(1);
	if(i)
	  printf("Card Init Success %d \n",i);
	else
	  printf("Card Init Failed %d \n",i);
	   /*******************REGISTER TEST********************/
	/*for(MemAdr=0x0;MemAdr<=0x1f;MemAdr++)
	  RegWrite(1,MemAdr,MemAdr,1);	
	for(MemAdr=0x0;MemAdr<=0x1f;MemAdr++)
	{
	    MemBuff = RegRead(1,MemAdr,1);
	    if(MemBuff!=MemAdr)
	    {
	      printf("MemAdr %x MemBuff %x \n",MemAdr,MemBuff);
	      FAIL=0;
	    }
	}
	if(FAIL)
	{
	  printf("PASS : Last Address %x MemBuff %x \n",MemAdr,MemBuff);
	}
	else
	  printf("FAIL : LAST Address %x MemBuff %x \n",MemAdr,MemBuff);
	  exit(1);*/
	  /*******************MEMORY TEST STARTS********************/
	  printf("1553B MemTest Enter 1 / CommTest Enter 0\n");
	  scanf("%d",&Com_Or_MemTest);
        if(Com_Or_MemTest==1)
        {
          printf("\n\t\tSelect ModuleId:1/2/3\n");
	  scanf("%d",&ModId);	
           // for(MemAdr=0x0;MemAdr<0x10fff;MemAdr++)
            for(MemAdr=0x0;MemAdr<MEMORY_SIZE_DDC;MemAdr++)
              MemWrite(ModId,MemAdr,MemAdr,1);	
            //for(MemAdr=0x0;MemAdr<0x10fff;MemAdr++)
            for(MemAdr=0x0;MemAdr<MEMORY_SIZE_DDC;MemAdr++)
            {
                MemBuff = MemRead(ModId,MemAdr,1);
                if(MemBuff!=(MemAdr & 0xffff))
                {
                  printf("MemAdr %x MemBuff %x \n",MemAdr,MemBuff);
                  FailCount=1;
                }
            }
            if(FailCount)
            {
              printf("MEMORY TEST :FAIL %d\n",FailCount);
            }
            else
              printf("MEMORY TEST :PASS %d\n",FailCount);              
        }
        else
        {
	  printf("\n\t\tSelect BC ModuleId:1/2/3\n");
	  scanf("%d",&ModId);	
	  if(ModId==1)	  
	    MTModId=2;	  
	  if (ModId==2)
	   MTModId=1;
	  if (ModId==3)
	   MTModId=2;
	  printf("\n\t\tEnter 16-Bit Hexadecimal Data (Ex :a5a5) :");
	  scanf("%x",&Data);

	  printf("\n\t\tEnter Channel (CH-A:1, CH-B:0) :");
	  scanf("%x",&Ch);
	  
	  printf("\n\t\tEnter Number Of Messages To Transmit to RT :");
	  scanf("%d",&Times);
         
	 
          RetVal = ClearRam_DDC(MTModId);
           printf("MT Retval of ClearRam is :%d ModId %d\n",RetVal,MTModId);
	  ResetAce_DDC(MTModId);	  
	  RetVal = DefMode_DDC(MTModId,MT_DDC);
	  //RegWrite(MTModId,CONFIG_REG_DDC,0x4f4f,1);  
	  printf("MT Retval of DefMode is :%x\n",RegRead(MTModId,CONFIG_REG_DDC,1));
	  //SelMsgMt_DDC(MTModId);
	  SelMsgMt(MTModId);
	  RunMt_DDC(MTModId);
	  
	   RetVal	=	ClearRam_DDC(ModId);
	  printf("BC Retval of ClearRam is :%d ModId %d\n",RetVal,ModId);
	  ResetAce_DDC(ModId); 
	  //printf("BC Retval of ResetAce is :%d\n",RetVal);
	  RetVal	=	DefMode_DDC(ModId,BC_DDC);
	  printf("BCRetval of DefMode is :%x\n",RetVal);
       
	  printf("\n Enter anychar to see the Mt results after running the BC\n");
	  getchar();
	  /* Set up the 1553 messages and descriptor stack */
	  /* SET UP MESSAGE BLOCKS */
	  /* Broadcast Messege */	
	  //getchar();
	  if(Ch	==	1)
		  bc_page.bus       = BUS_A_DDC;		
	  else if(Ch	==	0)
		  bc_page.bus       = BUS_B_DDC;		
	  bc_page.self_test     = NO_DDC;
	  bc_page.mask_bcst_bit = NO_DDC;
	  bc_page.broadcast     = NO_DDC;
	  bc_page.mode_code     = NO_DDC;
	  bc_page.rt_rt         = NO_DDC;
	  bc_page.cmd_1.taddr   = 1;
	  bc_page.cmd_1.t_r     = RECEIVE_DDC;
	  bc_page.cmd_1.status  = 0xcccc;
	  bc_page.loop          = 0x0000;
	  bc_page.cmd_1.subaddr = 1;
	  bc_page.cmd_1.wcnt    = 32;

	  for(i=0; i< 32; i++)
		  data_buffer[i] = Data;
		  
	  DefMsg_DDC(ModId,1,bc_page,data_buffer,32);
	  
	  bc_page.cmd_1.taddr   = 2;
	  bc_page.cmd_1.t_r     = RECEIVE_DDC;
	  bc_page.cmd_1.status  = 0xcccc;
	  bc_page.loop          = 0x0000;
	  bc_page.cmd_1.subaddr = 2;
	  bc_page.cmd_1.wcnt    = 32;

	  for(i=0; i< 32; i++)
		  data_buffer[i] = Data;
		  
	  DefMsg_DDC(ModId,2,bc_page,data_buffer,32);
	  
	  bc_page.cmd_1.taddr   = 3;
	  bc_page.cmd_1.t_r     = TRANSMIT_DDC;
	  bc_page.cmd_1.status  = 0xcccc;
	  bc_page.loop          = 0x0000;
	  bc_page.cmd_1.subaddr = 3;
	  bc_page.cmd_1.wcnt    = 32;

	  for(i=0; i< 32; i++)
		  data_buffer[i] = Data;
		  
	  DefMsg_DDC(ModId,3,bc_page,data_buffer,32);
	  
	  bc_page.cmd_1.taddr   = 4;
	  bc_page.cmd_1.t_r     = TRANSMIT_DDC;
	  bc_page.cmd_1.status  = 0xcccc;
	  bc_page.loop          = 0x0000;
	  bc_page.cmd_1.subaddr = 4;
	  bc_page.cmd_1.wcnt    = 32;

	  for(i=0; i< 32; i++)
		  data_buffer[i] = Data;
		  
	  DefMsg_DDC(ModId,4,bc_page,data_buffer,32);
	  
	 /* bc_page.self_test     = NO_DDC;
	  bc_page.mask_bcst_bit = NO_DDC;
	  bc_page.broadcast     = YES_DDC;
	  bc_page.mode_code     = NO_DDC;
	  bc_page.rt_rt         = NO_DDC;
	  bc_page.cmd_1.taddr   = 31;
	  bc_page.cmd_1.t_r     = RECEIVE_DDC;
	  bc_page.cmd_1.status  = 0xcccc;
	  bc_page.loop          = 0x0000;
	  bc_page.cmd_1.subaddr = 1;
	  bc_page.cmd_1.wcnt    = 32;

	  for(i=0; i< 32; i++)
		  data_buffer[i] = Data;
		  
	  DefMsg_DDC(ModId,5,bc_page,data_buffer,32);
	  
	  bc_page.self_test     = NO_DDC;
	  bc_page.mask_bcst_bit = NO_DDC;
	  bc_page.broadcast     = YES_DDC;
	  bc_page.mode_code     = YES_DDC;
	  bc_page.rt_rt         = NO_DDC;
	  bc_page.cmd_1.taddr   = 31;
	  bc_page.cmd_1.t_r     = RECEIVE_DDC;
	  bc_page.cmd_1.status  = 0xcccc;
	  bc_page.loop          = 0x0000;
	  bc_page.cmd_1.subaddr = 0;
	  bc_page.cmd_1.wcnt    = 17;

	  for(i=0; i< 17; i++)
		  data_buffer[i] = 0XA5A5;
		  
	  DefMsg_DDC(ModId,6,bc_page,data_buffer,32);
	  
	  bc_page.self_test     = NO_DDC;
	  bc_page.mask_bcst_bit = NO_DDC;
	  bc_page.broadcast     = NO_DDC;
	  bc_page.mode_code     = NO_DDC;
	  bc_page.rt_rt         = YES_DDC;	  
	  bc_page.cmd_1.status  = 0xcccc;
	  bc_page.cmd_2.status  = 0xcccc;
	  bc_page.loop          = 0x0000;	  
          bc_page.cmd_1.t_r= RECEIVE_DDC;
          bc_page.cmd_2.t_r= TRANSMIT_DDC;
          bc_page.cmd_1.taddr = 1;
          bc_page.cmd_1.subaddr = 1;
          bc_page.cmd_2.taddr = 2;
          bc_page.cmd_2.subaddr = 2;    
          bc_page.cmd_1.wcnt    = 32;
          bc_page.cmd_2.wcnt    = 32;
	  for(i=0; i< 32; i++)
		  data_buffer[i] = Data;
		  
	  DefMsg_DDC(ModId,7,bc_page,data_buffer,32);*/
	  /* Modecode Message (Transmit StatusWord) */
	  for(POS1=0;POS1<4;POS1++)
	    frame_items[POS1] = POS1+1;
	  DefFrame_DDC(ModId,4,frame_items);
	  getchar();
	  printf("Press Any Key To Run BC\n");
	  getchar();

	  for(i=1;i<=Times;i++)
	  {
		  RunBc_DDC(ModId,STACK_1_DDC,1,4,1);/*issue start Command*/
		  //usleep(10);
	  }
	  printf("After RUNBC\n");
	  getchar();
	  /*print Bc results */

	  printf("\nFirst Message Result from BC Module");
	  getchar();
	  getchar();
	  for(POS1=1;POS1<=4;POS1++)
	  {
	    ReadMsgResult_DDC(ModId,STACK_1_DDC,POS1,&msg_return);

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
	    getchar();
	    
	    printf("\nLOOPBACK WORD      = %x \n",msg_return.loop_word);
	    printf("STATUS             = %x \n",msg_return.status_1);
        	  getchar();	
	  }
	  for(POS1=1;POS1<=4;POS1++)
	  {
	    ReadMonPacket(MTModId,POS1,&buffer);
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
	   // printf("\nRT Status Word  -->>  %x \n",buffer.RecStatus);	

	    for(i=0;i<temp;i++)
		    data_buffer[i]=buffer.DataPack[i];
          }
	}
	
	return 0;
}  
