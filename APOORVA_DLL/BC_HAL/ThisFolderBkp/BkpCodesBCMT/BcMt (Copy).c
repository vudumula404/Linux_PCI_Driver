#include <stdio.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"


MSG_DEF_DDC	bc_page;
RT_MSG_DDC	RtBuff;
MSG_RESULT_DDC	msg_return;
unsigned short	frame_items[10],data_buffer[40],temp_data;
unsigned short	Temp_Buff[40];
MAJOR_FRAME Min_Cyc[128];

int DefMajorFrame1553(UCHAR Bus,USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],UCHAR CardId)
{
	

	
		unsigned int i,m,r=0,number_of_messages = 0;
		unsigned int msg_address,temp;
		unsigned int msg_offset,msg_offset1;
		USHORT Cnt_Time=0x0;
		
		if(Bus < 1 || Bus > 4)
			return (-2);
		
		if(no_of_minor_frames < 1 || no_of_minor_frames > 64)
			return (-24);
		
		for(m=0;m<no_of_minor_frames;m++)
		{
			/*if(frame_sequence[m].MinorFrame_Time < (frame_sequence[m].Mess_Count * 800)/1000)
				return 25;*/
			Cnt_Time = frame_sequence[m].Mess_Count << 9;//<<9 APOORVA PCI
			Cnt_Time = Cnt_Time | frame_sequence[m].MinorFrame_Time ;
			RegWrite(Bus,0x20 + m,Cnt_Time,1);//0x80 ApoorvaPCI
			//RegWrite(Bus,0x20 + m,0x1553,1);//0x80 ApoorvaPCI
			//RegWrite(Bus,m,0xa5a5,1);//0x80 ApoorvaPCI
			printf("RegAddress %x Read %x\n",RegRead(Bus,0x20+m,1),0x20+m);
		}

		for (m=0; m<no_of_minor_frames; m++)
			number_of_messages = number_of_messages + frame_sequence[m].Mess_Count;

		/*if(BCSTACK_A_B==BCSTACK_A)
		{
			if (number_of_messages <= StackA && number_of_messages >= 1)	
			{
				for (m=0; m<no_of_minor_frames; m++)
				{
					for (i=0; i<frame_sequence[m].Mess_Count; i++)
					{
						msg_offset  = 0x0000;
						temp = (frame_sequence[m].frame_items[i] -1);
						msg_address = ((temp) * 40) + 0x208;//0x0208 ApoorvaPCi4Node

						msg_offset  = ((r) * 4)  + 0x0002;
						MemWrite(Bus,msg_offset,Int_MsgA[frame_sequence[m].frame_items[i]],CardId);

						msg_offset  = ((r) * 4)  + 0x0003;
						MemWrite(Bus,msg_offset,msg_address,CardId);
						r++;
					}
				}
				MemWrite(Bus,msg_offset+4,END_OF_FRAME,CardId);
				
			}
			else
				return(-10);
		}
		else if(BCSTACK_A_B==BCSTACK_B)
		{
			if (number_of_messages <= StackB && number_of_messages >= 1)	
			{
				for (m=0; m<no_of_minor_frames; m++)
				{
					for (i=0; i<frame_sequence[m].Mess_Count; i++)
					{
						msg_offset  = 0x01c00;
						temp = (frame_sequence[m].frame_items[i]-1);
						msg_address = ((temp) * 39) + 0x0208 + (StackA * 39);

						msg_offset1  = msg_offset +  ((r) * 4)  + 0x0002;
						MemWrite(Bus,msg_offset1,Int_MsgB[frame_sequence[m].frame_items[i]],CardId);

						msg_offset  = msg_offset + ((r) * 4) + 0x0003;
						MemWrite(Bus,msg_offset,msg_address,CardId);
						r++;
					}
				}
				MemWrite(Bus,msg_offset+4,END_OF_FRAME,CardId);
			
			}
			else
				return(-10);
		}
		else
			return (-9);*/
	
		return 1;
	
}
void BCRetry1553_DDC(UCHAR ModId,USHORT sel)
{
	USHORT temp=0,val;
	printf("BCRetry_DDC %d \n",sel);
	if(sel)
	{
		RegWrite(ModId,CONFIG_REG3_DDC,0x8000,1);
		temp=RegRead(ModId,CONFIG_REG_DDC,1);
		RegWrite(ModId,CONFIG_REG_DDC,(temp&0xffe7)|(0x0013),1);
		printf("BCRetry_DDC %d temp %x\n",sel,temp);
		switch(sel)
		{

		case 1:
			val=RegRead(ModId,CONFIG_REG4_DDC,1);
			RegWrite(ModId,CONFIG_REG4_DDC,val | 0x0,1);
			break;

		case 2:
			val=RegRead(ModId,CONFIG_REG4_DDC,1);
			RegWrite(ModId,CONFIG_REG4_DDC,val | 0x1100,1);
			break;

		case 11:
			val=RegRead(ModId,CONFIG_REG_DDC,1);
			RegWrite(ModId,CONFIG_REG_DDC,val | 0x0008,1);
			val=RegRead(ModId,CONFIG_REG4_DDC,1);
			RegWrite(ModId,CONFIG_REG4_DDC,val | 0x0,1);
			break;

		case 12:
			val=RegRead(ModId,CONFIG_REG_DDC,1);
			RegWrite(ModId,CONFIG_REG_DDC,val | 0x0008,1);
			val=RegRead(ModId,CONFIG_REG4_DDC,1);
			RegWrite(ModId,CONFIG_REG4_DDC,val | 0x0080,1);
			break;

		case 21:
			val=RegRead(ModId,CONFIG_REG_DDC,1);
			RegWrite(ModId,CONFIG_REG_DDC,val | 0x0008,1);
			val=RegRead(ModId,CONFIG_REG4_DDC,1);
			RegWrite(ModId,CONFIG_REG4_DDC,val | 0x0100,1);
			break;

		case 22:
			val=RegRead(ModId,CONFIG_REG_DDC,1);
			RegWrite(ModId,CONFIG_REG_DDC,val | 0x0008,1);
			val=RegRead(ModId,CONFIG_REG4_DDC,1);
			RegWrite(ModId,CONFIG_REG4_DDC,val | 0x0180,1);
			break;
		}
	}
	else
	{
		temp=RegRead(ModId,CONFIG_REG_DDC,1);
		RegWrite(ModId,CONFIG_REG_DDC,(temp&0xffe7),1);
	}

}

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
	RegWrite(ModId,CONFIG_REG3_DDC,0x8700,1);
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
short DefMsg1553_DDC(UCHAR ModId,unsigned short msg_num,MSG_DEF_DDC def,unsigned short data_buffer[],unsigned short num_of_data_words)
{
unsigned int cmd=0;
unsigned int cntrl=0;
unsigned int cmd_offset=0;
unsigned int cntrl_offset=0;
unsigned int buff_offset=0;
int i,option=0,error=0;
unsigned int  CmdStack=0x1000;
unsigned int  DataStack=0x4000;
if (def.cmd_1.t_r == RECEIVE_DDC && def.rt_rt == NO && def.mode_code == NO)
	option = 0;
else if (def.cmd_1.t_r == TRANSMIT_DDC && def.rt_rt == NO && def.mode_code == NO)
	option = 1;
else if (def.mode_code == YES)
	option = 3;

	cmd =cmd | def.cmd_1.taddr<<11;
	cmd =cmd | def.cmd_1.t_r << 10;
	cmd =cmd | def.cmd_1.subaddr <<5;
	cmd =cmd | (def.cmd_1.wcnt % 32);
	cmd_offset = DataStack+((msg_num-1)*40)+1;
	MemWrite(ModId,cmd_offset,cmd,1);

	if(def.rt_rt == YES )
	{
		if (def.cmd_1.t_r != RECEIVE_DDC)
			error = -20;
		else if (def.cmd_1.wcnt != def.cmd_2.wcnt)
			error= -21;
		else  if (def.cmd_2.t_r != TRANSMIT_DDC)
			error = -22;

		else 
		{
			option = 2;
			cmd = 0;
			cmd =cmd | def.cmd_2.taddr<<11;
			cmd =cmd | def.cmd_2.t_r << 10;
			cmd =cmd | def.cmd_2.subaddr <<5;
			cmd =cmd | (def.cmd_2.wcnt % 32);
			cmd_offset= DataStack+((msg_num-1)*40)+2;
			MemWrite(ModId,cmd_offset,cmd,1);
		}
	 }
	 cntrl=cntrl | def.retry_enable<<8;
	 cntrl=cntrl | def.bus<<7;
	 cntrl=cntrl | def.self_test<<6;
	 cntrl=cntrl | def.mask_bcst_bit<<5;
	 cntrl=cntrl | def.mode_code<<2;
	 cntrl=cntrl | def.rt_rt;
	 cntrl=cntrl | def.broadcast<<1;
	 cntrl_offset = DataStack+((msg_num-1)*40);
	 MemWrite(ModId,cntrl_offset,cntrl,1);
       printf("msg_num %d cntrl_offset %x  Msg  %x\n",msg_num,cntrl_offset,MemRead(ModId,cntrl_offset,1));
//	if (def.cmd_1.taddr <=31 && (def.cmd_1.taddr+1) >= 0)
//	{
	switch (option)
	{
	case 0 :

		buff_offset = (DataStack + (msg_num -1) * 40) + 2 ;
		
		for (i=0; i < num_of_data_words; i++)
		MemWrite(ModId,buff_offset+i,data_buffer[i],1);
               
		buff_offset = DataStack + ((msg_num-1)*40)+2+num_of_data_words;
		MemWrite(ModId,buff_offset,def.loop,1);

		buff_offset = DataStack + (msg_num - 1)*40+2+num_of_data_words+1;
		MemWrite(ModId,buff_offset,def.cmd_1.status,1);
		
		break;

	case 1 :

		buff_offset = (CmdStack + (msg_num -1)*40) + 4;
		for (i=0; i<num_of_data_words;i++)
		MemWrite(ModId,buff_offset+i,data_buffer[i],1);

		buff_offset = CmdStack +((msg_num-1)*40)+2;
		MemWrite(ModId,buff_offset,def.loop,1);

		buff_offset = CmdStack + ((msg_num-1)*40)+3;
		MemWrite(ModId,buff_offset,def.cmd_1.status,1);
		break;

	case 2 :

		buff_offset = CmdStack + ((msg_num-1)*40)+3;
		MemWrite(ModId,buff_offset,def.loop,1);

		buff_offset = CmdStack + ((msg_num-1)*40)+4;
		MemWrite(ModId,buff_offset,def.cmd_2.status,1);

		buff_offset = (CmdStack + (msg_num -1)*40) + 5;
		for (i=0; i<num_of_data_words;i++)
			MemWrite(ModId,buff_offset+i,data_buffer[i],1);

		buff_offset = CmdStack + ((msg_num-1)*40)+5+num_of_data_words;
		MemWrite(ModId,buff_offset,def.cmd_1.status,1);
		break;

	case 3 :
	  
		if (def.cmd_1.subaddr == 31 || def.cmd_1.subaddr == 0)
		{
	if ((def.cmd_1.t_r == TRANSMIT_DDC) &&(( def.cmd_1.wcnt == 16) ||
		 ( def.cmd_1.wcnt == 18) || ( def.cmd_1.wcnt == 19)) )
	{
			buff_offset = CmdStack + ((msg_num-1)*40)+2;
			MemWrite(ModId,buff_offset,def.loop,1);
			buff_offset = CmdStack + ((msg_num-1)*40)+3;
			MemWrite(ModId,buff_offset,def.cmd_1.status,1);
		buff_offset = CmdStack + ((msg_num-1)*40)+4;
		MemWrite(ModId,buff_offset,data_buffer[0],1);
	}

else if (def.cmd_1.t_r == RECEIVE_DDC && (( def.cmd_1.wcnt == 20) ||
( def.cmd_1.wcnt == 17) || (def.cmd_1.wcnt == 21)) )
{
				buff_offset = CmdStack + ((msg_num-1)*40)+2;
				MemWrite(ModId,buff_offset,data_buffer[0],1);

				buff_offset = CmdStack + ((msg_num-1)*40)+3;
				MemWrite(ModId,buff_offset,def.loop,1);

				buff_offset = CmdStack + ((msg_num-1)*40)+4;
				MemWrite(ModId,buff_offset,def.cmd_1.status,1);

}

else
{
			buff_offset = CmdStack + ((msg_num-1)*40)+2;
			MemWrite(ModId,buff_offset,def.loop,1);

			buff_offset = CmdStack + ((msg_num-1)*40)+3;
			MemWrite(ModId,buff_offset,def.cmd_1.status,1);
} 
} 

		break;
}   
//		if (def.cmd_1.taddr >31 || (def.cmd_1.taddr+1) <0)
//			error = -23;
//	}
   return(error);
}
short DefFrame1553_DDC(UCHAR ModId,unsigned short number_of_messages,unsigned short mess_sequence[64])
{
 int i,error=0;
 unsigned int msg_address,temp;
 unsigned int msg_offset;
unsigned int CmdStack=0x1000;
	if (number_of_messages <= 1024 && number_of_messages >= 1)	
	{
		for (i=0; i<number_of_messages; i++)
		{
			msg_address = 0x0000;
			msg_offset  = 0x0000;
			temp = (mess_sequence[i]-1);
			msg_address = ((temp) * 40) + CmdStack;
			msg_offset  = ((i) * 4)  + 0x0003;
			MemWrite(ModId,msg_offset,msg_address,1);
		}

		MemWrite(ModId,msg_offset+4,END_OF_FRAME_DDC,1);
		if (number_of_messages >1024 || number_of_messages < 1)
			error = -24;
	}
	return(error);
}
short RunBc1553_DDC(UCHAR ModId,unsigned short frame,unsigned short position,unsigned short num_of_messages,unsigned short times)
{
	int				i,msg_addr,temp,init_pos,error=0;
	unsigned int	count;
	unsigned int CmdStack=0x1000;
	position	=	frame +(position-1)*4;
	init_pos	=	position+CmdStack;

    if(frame==0x0)
     {
      RegWrite(ModId,CONFIG_REG_DDC,RegRead(ModId,CONFIG_REG_DDC,1)&0xdfff,1);
	  for (i=position;i<num_of_messages;i++)
	
           {
		    msg_addr = position +3 ;
		   if (msg_addr == 0x00FF)
			error= -25;
	       }


	if (error == 0)
	 {
		for (i=0;i<times;i++)
		{
			MemWrite(ModId,CmdStack,init_pos,1);
			temp = 0xffff - num_of_messages;
			//MemWrite(ModId,MESS_COUNTER_DDC,temp,1);
			MemWrite(ModId,MESS_COUNTER_DDC,temp,1);
			MemWrite(ModId,TMP_CNTR_DDC,temp,1);     
			MemWrite(ModId,TMP_PTR_DDC,init_pos,1);   

			StartReset_DDC(ModId,START_DDC);

			count=MemRead(ModId,MESS_COUNTER_DDC,1);
			     
         	         while (count != 0xFFFF)
         	         {
		          count=MemRead(ModId,MESS_COUNTER_DDC,1);
           		  temp=0;
         		}
		}
           
	}
	return(error);
 }
else

  if(frame==0xf00)

         {
           RegWrite(ModId,CONFIG_REG_DDC,RegRead(ModId,CONFIG_REG_DDC,1)|CmdStack,1);

	       for (i=position;i<num_of_messages;i++)
	          {
		       msg_addr = position +3 ;
		      if (msg_addr == 0x00FF)
			  error= -25;
	          }


	if (error == 0)
        	{
		        for (i=0;i<times;i++)
		        {

                 		MemWrite(ModId,STACKA_PTR_B_DDC,init_pos,1);
			     temp = 0xffff - num_of_messages;
			     MemWrite(ModId,MESS_COUNTER_B_DDC,temp,1);
			     MemWrite(ModId,TMP_CNTR_B_DDC,temp,1);     
			     MemWrite(ModId,TMP_PTR_B_DDC,init_pos,1);   

			     StartReset_DDC(ModId,START_DDC);

			     count=MemRead(ModId,MESS_COUNTER_B_DDC,1);
          
        	     while (count != 0xFFFF)
	    	     count=MemRead(ModId,MESS_COUNTER_B_DDC,1);
     		     temp=0;
		         
                 }
           
	         }
       return(error);
           }
       return(error);
}


/*******************************************************************
*  This Program gives code for BC and MT 
********************************************************************/
int main()
{
        MNTPACK_DDC buffer;
	static		int 	CCSC_FD		=	0x0;
	int			i,Saddr=0,RtNo=0,MemAdr=0,Ch=0x0,TempCnt,RetVal=0x0,Times=0x1,POS1=0;
	unsigned short		MemBuff,MsgNo=0,Data=0;
	unsigned char		CardId,FailCount=0,ModId=0,MTModId=3,Com_Or_MemTest=0;
	char temp;
	unsigned short MinorCyle=0,MsgCnt=0;
       
	i = InitCard(1);
	if(i)
	  printf("Card Init Success %d \n",i);
	else
	  printf("Card Init Failed %d \n",i);      
        
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
	 
	  for(POS1=0;POS1<32;POS1++)
	    RegWrite(ModId,0x20 + POS1,0x0000,1);       
	   
	   /* RegWrite(ModId,0x1,0x0000,1);  
	        RegWrite(ModId,0x7,0x0000,1);   */ 
	  RetVal	=	ClearRam_DDC(ModId);
	  printf("BC Retval of ClearRam is :%d ModId %d\n",RetVal,ModId);
	  ResetAce_DDC(ModId); 
	  //printf("BC Retval of ResetAce is :%d\n",RetVal);
	  RetVal	=	DefMode_DDC(ModId,BC_DDC);
	  MemWrite(ModId,0X100,0X1000,1);
	  MemWrite(ModId,0X101,0X4000,1);
	    
	  printf("BCRetval of DefMode is :%x\n",RetVal);
       
	  printf("\n Enter anychar to see the Mt results after running the BC\n");
	  getchar(); 
	 //bc_page.retry_enable=YES;
	  /*for(Saddr=1;Saddr<=64;Saddr++)
	  {
	          if(Ch	== 1)
		   bc_page.bus       = BUS_A_DDC;		
	         else if(Ch == 0)
		    bc_page.bus       = BUS_B_DDC;
		  MsgNo++;		  
		 // bc_page.int_msg_gap   = 10;
		  bc_page.self_test     = NO;
		  bc_page.mask_bcst_bit = NO;			
		  bc_page.broadcast     = NO;
		  bc_page.mode_code     = NO;
		  bc_page.rt_rt         = NO;
		  bc_page.cmd_1.taddr   = 2;
		  bc_page.cmd_1.t_r     = RECEIVE_DDC;
		  bc_page.cmd_1.status  = 0xcccc;
		  bc_page.loop          = 0x0000;
		  bc_page.cmd_1.subaddr = 1;
		  bc_page.cmd_1.wcnt    = 32;  
		  for(i=0; i<32; i++)
			  data_buffer[i] = Data+i;
		  DefMsg1553_DDC(ModId,MsgNo,bc_page,data_buffer,32);
		  frame_items[MsgNo-1] = MsgNo;
	  }	
	   for(Saddr=65;Saddr<=255;Saddr++)
	  {
	        if(RtNo==31)
	         RtNo=0;
	          if(Ch	== 1)
		   bc_page.bus       = BUS_A_DDC;		
	         else if(Ch == 0)
		    bc_page.bus       = BUS_B_DDC;
		  MsgNo++;		  
		 // bc_page.int_msg_gap   = 10;
		  bc_page.self_test     = NO;
		  bc_page.mask_bcst_bit = NO;			
		  bc_page.broadcast     = NO;
		  bc_page.mode_code     = NO;
		  bc_page.rt_rt         = NO;
		  bc_page.cmd_1.taddr   = RtNo++;
		  bc_page.cmd_1.t_r     = RECEIVE_DDC;
		  bc_page.cmd_1.status  = 0xcccc;
		  bc_page.loop          = 0x0000;
		  bc_page.cmd_1.subaddr = 1;
		  bc_page.cmd_1.wcnt    = 32;  
		  for(i=0; i<32; i++)
			  data_buffer[i] = Data+i;
		  DefMsg1553_DDC(ModId,MsgNo,bc_page,data_buffer,32);
		  frame_items[MsgNo-1] = MsgNo;
	  }*/
	  #if 0
	  for(Saddr=1;Saddr<=256;Saddr++)
	  {
	        if(RtNo==8)
	          RtNo=0;
	          if(Ch	== 1)
		   bc_page.bus       = BUS_A_DDC;		
	         else if(Ch == 0)
		    bc_page.bus       = BUS_B_DDC;
		  MsgNo++;		  
		 // bc_page.int_msg_gap   = 10;
		  bc_page.self_test     = NO;
		  bc_page.mask_bcst_bit = NO;			
		  bc_page.broadcast     = NO;
		  bc_page.mode_code     = NO;
		  bc_page.rt_rt         = NO;
		  bc_page.cmd_1.taddr   = ++RtNo;;
		  bc_page.cmd_1.t_r     = RECEIVE_DDC;
		  bc_page.cmd_1.status  = 0xcccc;
		  bc_page.loop          = 0x0000;
		  bc_page.cmd_1.subaddr = 1;
		  bc_page.cmd_1.wcnt    = 32;  
		  for(i=0; i<32; i++)
			  data_buffer[i] = Data+i;
		  DefMsg1553_DDC(ModId,MsgNo,bc_page,data_buffer,32);
		  frame_items[MsgNo-1] = MsgNo;
	  }	
	  DefFrame1553_DDC(ModId,MsgNo,frame_items);
	   //ResetAce_DDC(ModId);	
	    
	    printf("BC Reg 0x10  is :%x\n",RegRead(ModId,0x10,1));
	  for(MinorCyle=0;MinorCyle<16;MinorCyle++)
	  {
	  //0x80 ApoorvaPCI*/
	  /*Cnt_Time = frame_sequence[m].Mess_Count << 8;//<<9 APOORVA PCI
	    Cnt_Time = Cnt_Time | frame_sequence[m].MinorFrame_Time ;
	    RegWrite(Bus,0x20 + m,Cnt_Time,CardId);//0x80 ApoorvaPCI*/
	     MsgCnt =(4<<9);
	     MsgCnt = (MsgCnt|19);
	     RegWrite(ModId,0x20+MinorCyle,MsgCnt,1);
	     printf("BC Reg 0x20  is :%x\n",RegRead(ModId,0x20+MinorCyle,1));     
	  }
	  RegWrite(ModId,0x10,16,1);
	 // MemWrite(ModId,MESS_COUNTER_DDC,18,1);
	    /*RegWrite(ModId,0x01,0x0f4f,1);  
	    RegWrite(ModId,0x07,0x8000,1); 
	    RegWrite(ModId,0x07,0x8000,1);*///Auto Repeat
	   RegWrite(ModId,0x01,0x0,1);  //Number Of Messgs
	   RegWrite(ModId,0x07,0x0,1); 
	    
          printf("BC Reg 0x03  is :%x\n",RegRead(ModId,0x7,1));
          printf("BC Reg 0x01  is :%x\n",RegRead(ModId,0x1,1));
         // StartReset_DDC(ModId,START_DDC);
	  RunBc1553_DDC(ModId,STACK_1_DDC,1,256,1);/*issue start Command*/
	  #endif
	  for(Saddr=1;Saddr<=198;Saddr++)
	  {
	        if(RtNo==16)
	          RtNo=0;
	          if(Ch	== 1)
		   bc_page.bus       = BUS_A_DDC;		
	         else if(Ch == 0)
		    bc_page.bus       = BUS_B_DDC;
		  MsgNo++;		
		  bc_page.self_test     = NO;
		  bc_page.mask_bcst_bit = NO;			
		  bc_page.broadcast     = NO;
		  bc_page.mode_code     = NO;
		  bc_page.rt_rt         = NO;
		  bc_page.cmd_1.taddr   = ++RtNo;
		  bc_page.cmd_1.t_r     = 0;//RECEIVE_DDC;
		  bc_page.cmd_1.status  = 0xcccc;
		  bc_page.loop          = 0x0000;
		  bc_page.cmd_1.subaddr = 1;
		  bc_page.cmd_1.wcnt    = 32;  
		  for(i=0; i<32; i++)
			  data_buffer[i] = Data+i;
		  DefMsg1553_DDC(ModId,MsgNo,bc_page,data_buffer,32);
		  frame_items[MsgNo-1] = MsgNo;
	  }	
	  DefFrame1553_DDC(ModId,MsgNo,frame_items);   
	    
	  for(MinorCyle=0;MinorCyle<13;MinorCyle++)
	  { 
	     MsgCnt =(16<<9);
	     MsgCnt = (MsgCnt|40);
	     RegWrite(ModId,0x20+MinorCyle,MsgCnt,1);
	     printf("BC Reg 0x20  is :%x\n",RegRead(ModId,0x20+MinorCyle,1));     
	  }
	  RegWrite(ModId,0x10,13,1);
	  printf("BC Reg 0x10  is :%x\n",RegRead(ModId,0x10,1));
	 // MemWrite(ModId,MESS_COUNTER_DDC,18,1);
	    /*RegWrite(ModId,0x01,0x0f4f,1);  
	    RegWrite(ModId,0x07,0x8000,1); 
	    RegWrite(ModId,0x07,0x8000,1);//Auto Repeat*/
	   RegWrite(ModId,0x01,0x0,1);  //Number Of Messgs
	   RegWrite(ModId,0x07,0x0,1); 
	    
          printf("BC Reg 0x03  is :%x\n",RegRead(ModId,0x7,1));
          printf("BC Reg 0x01  is :%x\n",RegRead(ModId,0x1,1));
          //StartReset_DDC(ModId,START_DDC);
	  RunBc1553_DDC(ModId,STACK_1_DDC,1,198,1);/*issue start Command*/
	/*   RegWrite(ModId,0x01,0x0f4f,1);  
	    RegWrite(ModId,0x07,0x8000,1); 
	    RegWrite(ModId,0x07,0x8000,1);  */
         /* printf("2BC Reg 0x03  is :%x\n",RegRead(ModId,0x7,1));
          printf("2BC Reg 0x01  is :%x\n",RegRead(ModId,0x1,1));*/
	   // StartReset_DDC(ModId,START_DDC);
	  /*RegWrite(ModId,0x20,0x214,1);
	    RegWrite(ModId,0x20,0x214,1);
	    RegWrite(ModId,0x21,0x214,1);*/
	    /*RegWrite(ModId,0x22,0x214,1);
	    RegWrite(ModId,0x23,0x214,1);
	    RegWrite(ModId,0x24,0x214,1);*/
	   // RegWrite(ModId,0x21,0x3f01,1);
	   // RegWrite(ModId,0x22,0x0201,1);
	   // RegWrite(ModId,0x23/* + POS1* /,0x0201,1);
	  
	  printf("Waiting..............!\n");
	  getchar();
	  ResetAce_DDC(ModId);	

	  /*for(i=1;i<=Times;i++)
	  {
		  RunBc1553_DDC(ModId,STACK_1_DDC,1,MsgNo,Times);/*issue start Command* /
		  //usleep(10);
	  }*/
	/*Min_Cyc[0].frame_items[0] = 1; 
	Min_Cyc[0].frame_items[1] = 2; 
	Min_Cyc[0].frame_items[2] = 3; 
	Min_Cyc[0].Mess_Count = 3;
	Min_Cyc[0].MinorFrame_Time = 5;

	Min_Cyc[1].frame_items[0] = 4; 
	Min_Cyc[1].frame_items[1] = 5; 
	Min_Cyc[1].frame_items[2] = 6; 
	Min_Cyc[1].Mess_Count = 3;
	Min_Cyc[1].MinorFrame_Time = 7;

	Min_Cyc[2].frame_items[0] = 7; 
	Min_Cyc[2].frame_items[1] = 8; 
	Min_Cyc[2].frame_items[2] = 9; 
	Min_Cyc[2].Mess_Count = 3;
	Min_Cyc[2].MinorFrame_Time = 9;
	
	Min_Cyc[3].frame_items[0] = 10; 
	Min_Cyc[3].frame_items[1] = 11; 
	Min_Cyc[3].frame_items[2] = 12; 
	Min_Cyc[3].Mess_Count = 3;
	Min_Cyc[3].MinorFrame_Time = 11;

	Min_Cyc[4].frame_items[0] = 13; 
	Min_Cyc[4].Mess_Count = 1;
	Min_Cyc[4].MinorFrame_Time = 13;
	
	DefMajorFrame1553(ModId,BCSTACK_A,5,Min_Cyc,CardId);*/
	//BCAutoRepeat(BcMod,YES,CARD1);
	//RunBcMajorFrame(ModId,BCSTACK_A,5,Min_Cyc,1,CardId);
	//RunBc_DDC(ModId,STACK_1_DDC,1,13,1);

/*
	  printf("Started RUNBC Major Frame\n");
	  getchar();
	  printf("\nFirst Message Result from BC Module");
	  getchar();
	  getchar();
	  for(POS1=1;POS1<=10;POS1++)
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
	  }*/
	  /*for(POS1=1;POS1<=4;POS1++)
	  {
	    ReadMonPacket_DDC(MTModId,POS1,&buffer);
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
          }*/
	return 0;
}  
