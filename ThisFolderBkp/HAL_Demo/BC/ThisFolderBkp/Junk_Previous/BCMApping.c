short DefMsg1553_DDC(UCHAR ModId,unsigned short msg_num,MSG_DEF_DDC def,unsigned short data_buffer[],unsigned short num_of_data_words)
{
        unsigned int  CmdStack=0x1000;
        unsigned int  DataStack=0x4000;

	cmd =cmd | def.cmd_1.taddr<<11;
	cmd =cmd | def.cmd_1.t_r << 10;
	cmd =cmd | def.cmd_1.subaddr <<5;
	cmd =cmd | (def.cmd_1.wcnt % 32);
	cmd_offset = DataStack+((msg_num-1)*40)+1;
	MemWrite(ModId,cmd_offset,cmd,1);
	
       cntrl=cntrl | def.retry_enable<<8;
       cntrl=cntrl | def.bus<<7;
       cntrl=cntrl | def.self_test<<6;
       cntrl=cntrl | def.mask_bcst_bit<<5;
       cntrl=cntrl | def.mode_code<<2;
       cntrl=cntrl | def.rt_rt;
       cntrl=cntrl | def.broadcast<<1;
       cntrl_offset = DataStack+((msg_num-1)*40);
       MemWrite(ModId,cntrl_offset,cntrl,1);
       
       buff_offset = (DataStack + (msg_num -1) * 40) + 2 ;
		
      for (i=0; i < num_of_data_words; i++)
        MemWrite(ModId,buff_offset+i,data_buffer[i],1);
                 
      buff_offset = DataStack + ((msg_num-1)*40)+2+num_of_data_words;
      MemWrite(ModId,buff_offset,def.loop,1);

      buff_offset = DataStack + (msg_num - 1)*40+2+num_of_data_words+1;
      MemWrite(ModId,buff_offset,def.cmd_1.status,1);
}
short DefFrame1553_DDC(UCHAR ModId,unsigned short number_of_messages,unsigned short mess_sequence[64])//Message Based
{
 int i,error=0;
 unsigned int msg_address,temp;
 unsigned int msg_offset;
   
    for (i=0; i<number_of_messages; i++)
    {
	    msg_address = 0x0000;
	    msg_offset  = 0x0000;
	    temp = (mess_sequence[i]-1);
	    msg_address = ((temp) * 40) + 0x4000;
	    msg_offset  = 0x1000+((i) * 4)  + 0x0003;
	    MemWrite(ModId,msg_offset,msg_address,1);
    }
    MemWrite(ModId,(msg_offset+0x1000)+4,1024,1);
    if (number_of_messages >1024 || number_of_messages < 1)
	    error = -24;
   
}
short RunBc1553_DDC(UCHAR ModId,unsigned short frame,unsigned short position,unsigned short num_of_messages,unsigned short times)//Message Based
{
	int				i,msg_addr,temp,init_pos,error=0;
	unsigned int	count;
	unsigned int CmdStack=0x1000;
	position	=	frame +(position-1)*4;
	init_pos	=	position+CmdStack;

    if(frame==0x0)
     {
          RegWrite(ModId,CONFIG_REG_DDC,RegRead(ModId,CONFIG_REG_DDC,1)&0xdfff,1);	
	  for (i=0;i<times;i++)
	  {
	      MemWrite(ModId,CmdStack,init_pos,1);
	      temp = 0xffff - num_of_messages;
	      MemWrite(ModId,MESS_COUNTER_DDC,temp,1);//MESS_COUNTER_DDC 0x101
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
	
	return(error);
      } 
}
int DefMajorFrame1553(UCHAR Bus,USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],UCHAR CardId)
{	
  unsigned int i,m,r=0,number_of_messages = 0,framecounter=0;
  unsigned int msg_address,temp;
  unsigned int msg_offset,msg_offset1;
  USHORT Cnt_Time=0x0;

  if(no_of_minor_frames < 1 || no_of_minor_frames > 32)
	  return (-24);  
  for(m=0;m<no_of_minor_frames;m++)
  {	  
    Cnt_Time = frame_sequence[m].Mess_Count << 9;//<<9 APOORVA PCI
    Cnt_Time = Cnt_Time | (frame_sequence[m].MinorFrame_Time) ;
    RegWrite(Bus,0x3,0x8000,1);
    RegWrite(Bus,0x24 + m,Cnt_Time,1);	 
  }
      
  RegWrite(Bus,0x3,0x8000,1);

  RegWrite(Bus,0x10,(0x400|no_of_minor_frames),1);
    
  for (m=0; m<no_of_minor_frames; m++)
  number_of_messages = number_of_messages + frame_sequence[m].Mess_Count;

   for (m=0; m<no_of_minor_frames; m++)
   {
     for (i=0; i<frame_sequence[m].Mess_Count; i++)
     {
      msg_address = 0x0000;
      msg_offset  = 0x0000;
      temp = (frame_sequence[m].frame_items[i]-1);
      msg_address = ((temp) * 40) + 0x4000;
      msg_offset  = 0x1000+((framecounter) * 4)  + 0x0003;
      MemWrite(Bus,msg_offset,msg_address,1);
      framecounter++;           
      }
    }
    MemWrite(Bus,(msg_offset+0x1000)+4,END_OF_FRAME_DDC,1);//END_OF_FRAME_DDC =1024
}
unsigned long RunBcMajorFrame1553(UCHAR Bus,USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],unsigned long times,UCHAR CardId)
{
      position = stackA_B;		
      unsigned int CmdStack=0x1000;
      position	=	stackA_B;
      init_pos	=	position+CmdStack;
      
      for (m=0; m<no_of_minor_frames; m++)
       num_of_messages = num_of_messages + frame_sequence[m].Mess_Count;
			  
      RegWrite(Bus,0x3,0x8000,1);
      MemWrite(Bus,CmdStack,init_pos,1);
      temp = 0xffff - (num_of_messages*times);

      MemWrite(Bus,MESS_COUNTER_DDC,temp,1);//MESS_COUNTER_DDC 0x101

      MemWrite(Bus,TMP_CNTR_DDC,temp,1);     
      MemWrite(Bus,TMP_PTR_DDC,init_pos,1);                          

      StartReset_DDC(Bus,START_DDC);
      count=MemRead(Bus,MESS_COUNTER_DDC,1);

      while (count != 0xFFFF)
      {
        count=MemRead(Bus,MESS_COUNTER_DDC,1);
        temp=0;
      }   
}
int main()
{ 	
       unsigned int CmdStack=0x1000;      
	i = InitCard(1);
	if(i)
	  printf("Card Init Success %d \n",i);
	else
	  printf("Card Init Failed %d \n",i);	       
       
	  RetVal	=	ClearRam_DDC(ModId);
	  RegWrite(ModId,0x3,0x1,1);//Reset Register
	  RegWrite(ModId,CONFIG_REG_DDC,0x0f0f,1);  //Def Mode as BC
	  
	  MemWrite(ModId,0X100,0X1000,1);//Command Satck	  
	  MemWrite(ModId,0X1003,0X4000,1); Data Stack
	  
	 RegWrite(ModId,0x15,MsgCnt*Times,1);	  

	  if(Ch	==	1)
		bc_page.bus       = BUS_A_DDC;		
	  else if(Ch	==	0)
		bc_page.bus       = BUS_B_DDC;
	    for(MsgNo=1;MsgNo<(MsgCnt+1);MsgNo++)
	    {
	      if(RtAddre>=30)
	       RtAddre=0;
	        
	      bc_page.self_test     = NO_DDC;
	      bc_page.mask_bcst_bit = NO_DDC;
	      bc_page.broadcast     = NO_DDC;
	      bc_page.mode_code     = NO_DDC;
	      bc_page.rt_rt         = NO_DDC;	     
	      bc_page.cmd_1.t_r     = RECEIVE_DDC;//TRANSMIT_DDC;//RECEIVE_DDC;	
	      bc_page.cmd_1.taddr   = ++RtAddre;    
	      bc_page.cmd_1.status  = 0xcccc;
	      bc_page.loop          = 0x0000;
	      bc_page.cmd_1.subaddr = 1;
	      bc_page.cmd_1.wcnt    = 32;

	      for(i=0; i< 32; i++)
		      data_buffer[i] = /*Data*/MsgNo;	          
	      DefMsg1553_DDC(ModId,MsgNo,bc_page,data_buffer,32);
	  } 
           
         for(POS1=0;POS1<100;POS1++)
         {
            Min_Cyc[0].frame_items[POS1] = POS1+1; 
            Min_Cyc[0].Mess_Count = 100;
            Min_Cyc[0].MinorFrame_Time = MinorCyleTime;
         }
         for(POS1=100;POS1<200;POS1++)
         {
            Min_Cyc[1].frame_items[POS1-100] = POS1+1; 
            Min_Cyc[1].Mess_Count = 100;
            Min_Cyc[1].MinorFrame_Time = MinorCyleTime;
         }
         for(POS1=200;POS1<300;POS1++)
         {
            Min_Cyc[2].frame_items[POS1-200] = POS1+1; 
            Min_Cyc[2].Mess_Count = 100;
            Min_Cyc[2].MinorFrame_Time = MinorCyleTime;
         }
         for(POS1=300;POS1<400;POS1++)
         {
            Min_Cyc[3].frame_items[POS1-300] = POS1+1;
            Min_Cyc[3].Mess_Count = 100;
            Min_Cyc[3].MinorFrame_Time = MinorCyleTime;
         }
         for(POS1=400;POS1<500;POS1++)
         {
            Min_Cyc[4].frame_items[POS1-400] = POS1+1; 
            Min_Cyc[4].Mess_Count = 100;
            Min_Cyc[4].MinorFrame_Time = MinorCyleTime;
         }
         for(POS1=500;POS1<600;POS1++)
         {
            Min_Cyc[5].frame_items[POS1-500] = POS1+1; 
            Min_Cyc[5].Mess_Count = 100;
            Min_Cyc[5].MinorFrame_Time = MinorCyleTime;
         }
         for(POS1=600;POS1<700;POS1++)
         {
            Min_Cyc[6].frame_items[POS1-600] = POS1+1;
            Min_Cyc[6].Mess_Count = 100;
            Min_Cyc[6].MinorFrame_Time = MinorCyleTime;
         }
         for(POS1=700;POS1<800;POS1++)
         {
            Min_Cyc[7].frame_items[POS1-700] = POS1+1; 
            Min_Cyc[7].Mess_Count = 100;
            Min_Cyc[7].MinorFrame_Time = MinorCyleTime;
         }
         for(POS1=800;POS1<900;POS1++)
         {
            Min_Cyc[8].frame_items[POS1-800] = POS1+1; 
            Min_Cyc[8].Mess_Count = 100;
            Min_Cyc[8].MinorFrame_Time = MinorCyleTime;
         }
         for(POS1=900;POS1<1024;POS1++)
         {
            Min_Cyc[9].frame_items[POS1-900] = POS1+1; 
            Min_Cyc[9].Mess_Count = 124;
            Min_Cyc[9].MinorFrame_Time = MinorCyleTime;
         }          
           	
	DefMajorFrame1553(ModId,0,MinorCyleCnt,Min_Cyc,1);       
	
	printf("BC Major Frame Started Wait........! \n");
	
	RunBcMajorFrame1553(ModId,0,MinorCyleCnt,Min_Cyc,Times,1);

	getchar();
        printf("BC Major Frame Completed........! \n");
     
        ResetAce_DDC(ModId);	
        printf("Device Reset.............!\n");
        getchar();
        getchar();
	  
	return 0;
}  
