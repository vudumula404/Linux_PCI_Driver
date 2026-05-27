#include <stdio.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"

MSG_DEF_DDC	bc_page;
RT_MSG_DDC	RtBuff;
MSG_RESULT_DDC	msg_return;
unsigned short	frame_items[4096],data_buffer[40],temp_data;
unsigned short	Temp_Buff[40];
MAJOR_FRAME Min_Cyc[128];
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
      // printf("Defmsg_num %d cntrl_offset %x  Msg  %x\n",msg_num,cntrl_offset,MemRead(ModId,cntrl_offset,1));
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
	            if ((def.cmd_1.t_r == TRANSMIT_DDC) &&(( def.cmd_1.wcnt == 16) ||( def.cmd_1.wcnt == 18) || ( def.cmd_1.wcnt == 19)) )
	            {
	              buff_offset = CmdStack + ((msg_num-1)*40)+2;
	              MemWrite(ModId,buff_offset,def.loop,1);
	              buff_offset = CmdStack + ((msg_num-1)*40)+3;
	              MemWrite(ModId,buff_offset,def.cmd_1.status,1);
	              buff_offset = CmdStack + ((msg_num-1)*40)+4;
	              MemWrite(ModId,buff_offset,data_buffer[0],1);
	            }

                  else if (def.cmd_1.t_r == RECEIVE_DDC && (( def.cmd_1.wcnt == 20) || (def.cmd_1.wcnt == 17) || (def.cmd_1.wcnt == 21)) )
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

   return(error);
}
int DefMajorFrame1553(UCHAR Bus,USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],UCHAR CardId)
{	
  unsigned int i,m,r=0,number_of_messages = 0,framecounter=0;
  unsigned int msg_address,temp;
  unsigned int msg_offset,msg_offset1;
  USHORT Cnt_Time=0x0;
  
  if(Bus < 1 || Bus > 3)
      return (-2);

  if(no_of_minor_frames < 1 || no_of_minor_frames > 32)
	  return (-24);  
  for(m=0;m<no_of_minor_frames;m++)
  {	  
    Cnt_Time = frame_sequence[m].Mess_Count << 9;//<<9 APOORVA PCI
    Cnt_Time = Cnt_Time | (frame_sequence[m].MinorFrame_Time) ;
    RegWrite(Bus,0x3,0x8000,1);
    RegWrite(Bus,0x24 + m,Cnt_Time,1);
   // printf("Data %x Addres %x\n",RegRead(Bus,0x24+m,1),0x24+m);
  } 
  RegWrite(Bus,0x7,0x8000,1);
  RegWrite(Bus,0x3,0x8000,1);
  RegWrite(Bus,0x10,(0x400|no_of_minor_frames),1);
  printf("Address(0x10) %x no_of_minor_frames %d\n",RegRead(Bus,0x10,1),no_of_minor_frames);
//RegWrite(Bus,0x2E,0xf064,1);
//printf("Addre(0x2E) Data %x\n",RegRead(Bus,0x2E,1));
   
  for (m=0; m<no_of_minor_frames; m++)
    number_of_messages = number_of_messages + frame_sequence[m].Mess_Count;
  if (number_of_messages <= 1024 && number_of_messages >= 1)	
  {      
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
           //printf("DefFrame_num %d msg_offset %x  Msg  %x\n",i,msg_offset,MemRead(Bus,msg_offset,1));
        }
      } 
    MemWrite(Bus,(msg_offset+0x1000)+4,END_OF_FRAME_DDC,1);
    if (number_of_messages >1024 || number_of_messages < 1)
     return(-24);
  }
  else
    return(-10);
  return 1;
	
}
unsigned long RunBcMajorFrame1553(UCHAR Bus,USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],unsigned long times,UCHAR CardId)
{
  if(CardId == 1)
  {
    if(Bus < 1 || Bus > 3)
	  return (-2);
    if(no_of_minor_frames < 1 || no_of_minor_frames > 32)
	    return (-24);
          int i,m,init_pos,position,temp;
    position = stackA_B;
    USHORT count,num_of_messages=0,Mcnt=0,TimesValue;
    //position = position-1;				//////////////////  14-2-2k6
    unsigned int CmdStack=0x1000;
    position	=	stackA_B +(position-1)*4;
    init_pos	=	position+CmdStack;
    for (m=0; m<no_of_minor_frames; m++)
      num_of_messages = num_of_messages + frame_sequence[m].Mess_Count;

    if(stackA_B==0x0)
    {		
      if(times == 0)
      {
        RegWrite(Bus,0x01,0x054f,1);  
        RegWrite(Bus,0x07,0x8000,1); 
       // RegWrite(Bus,0x07,0x3000,1);
        StartReset_DDC(Bus,START_DDC);		 
        printf("Continuous Scheduling  %x\n",START_DDC);
      }
      else
      {
        //for (i=0;i<times;i++)
        {
          RegWrite(Bus,0x3,0x8000,1);
          MemWrite(Bus,CmdStack,init_pos,1);
          temp = 0xffff - (num_of_messages*times);
          MemWrite(Bus,MESS_COUNTER_DDC,temp,1);//MESS_COUNTER_DDC 0x101
          printf("MESS_COUNTER_DDC %d num_of_messages*times %d\n",MemRead(Bus,MESS_COUNTER_DDC,1),num_of_messages*times);
          MemWrite(Bus,TMP_CNTR_DDC,temp,1);     
          MemWrite(Bus,TMP_PTR_DDC,init_pos,1);   
          RegWrite(Bus,0x3,0x8000,1);
          StartReset_DDC(Bus,START_DDC);
          count=MemRead(Bus,MESS_COUNTER_DDC,1);
          while (count != 0xFFFF)
          {
          count=MemRead(Bus,MESS_COUNTER_DDC,1);
          //printf("MESS_COUNTER_DDC %d Readcount %x  temp %x num_of_messages  %d\n",MESS_COUNTER_DDC,count,temp,num_of_messages);
          temp=0;
          }
        }			  
      }	
    }				
    return 1;	
  }
  else
  return 0;
}
/*******************************************************************
*  This Program gives code for BC and MT 
********************************************************************/
int main()
{        
	int			RtAddre=0,i,MemAdr=0,Ch=0x0,TempCnt,RetVal=0x0,Times=0x1,POS1=0;
	unsigned short		MemBuff,MsgNo=0,Data=0;
	unsigned char		CardId,FailCount=0,ModId=0,MTModId=3,Com_Or_MemTest=0;
	char temp;
        unsigned short MinorCyle=0,MsgCnt=0,MinorCyleCnt=0,MinorCyleTime=0;
	i = InitCard(1);
	if(i)
	  printf("Card Init Success %d \n",i);
	else
	  printf("Card Init Failed %d \n",i);	         
       
	  printf("\n\t\tSelect BC ModuleId:1/2/3\n");
	  scanf("%d",&ModId);	
	
	  /*printf("\n\t\tEnter 16-Bit Hexadecimal Data (Ex :a5a5) :");
	  scanf("%x",&Data);*/

	  printf("\n\t\tEnter Channel (CH-A:1, CH-B:0) :");
	  scanf("%x",&Ch);	  
	  printf("\n\t\tEnter Number Of Messages Count :");
	  scanf("%d",&MsgCnt);
	  printf("\n\t\tEnter Number Of MinorCycle Count :");
	  scanf("%d",&MinorCyleCnt);
	   printf("\n\t\tEnter Number Of MinorCycle Time :");
	  scanf("%d",&MinorCyleTime);
	  
	  printf("\n\t\tEnter Number Of Messages To Run (Times*Messages):");
	  scanf("%d",&Times);
	 
	  //printf("(0x9)Data %x\n",RegRead(ModId,0x9,1));
	   RetVal	=	ClearRam_DDC(ModId);
	  printf("BC Retval of ClearRam is :%d ModId %d\n",RetVal,ModId);
	  ResetAce_DDC(ModId); 
	  //printf("BC Retval of ResetAce is :%d\n",RetVal);
	  //RetVal	=	DefMode_DDC(ModId,BC_DDC);
	  RegWrite(ModId,0x1,0x054f,1);
	  MemWrite(ModId,0X100,0X1000,1);
	  MemWrite(ModId,0X1003,0X4000,1);
	  RegWrite(ModId,0x15,MsgCnt*Times,1);
	  
	  printf("BCRetval of DefMode is :%x\n",RetVal);
          printf("RegRead 0x15 = %x",RegRead(ModId,0x15,1));
          
	  printf("\n Enter anychar to see the Mt results after running the BC\n");
	  getchar();	  
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
	      //bc_page.retry_enable=YES;
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
	    /*for(POS1=0;POS1<20;POS1++)
             {
                Min_Cyc[0].frame_items[POS1] = POS1+1; 
                Min_Cyc[0].Mess_Count = 20;
                Min_Cyc[0].MinorFrame_Time = MinorCyleTime;
             }
             for(POS1=20;POS1<40;POS1++)
             {
                Min_Cyc[1].frame_items[POS1-20] = POS1+1; 
                Min_Cyc[1].Mess_Count = 20;
                Min_Cyc[1].MinorFrame_Time = MinorCyleTime;
             }*/
	  // BCRetry1553_DDC(ModId,11);	
	  //64*16= BCRetry1553_DDC(ModId,2);	1024
	  //MsgCnt>31 not Working
	   /*for(MinorCyle=0;MinorCyle<MinorCyleCnt;MinorCyle++)
	   {
	     for(POS1=0;POS1<MsgCnt;POS1++)
	     {
	        Min_Cyc[MinorCyle].frame_items[POS1] = POS1+1; 
	     }
	     Min_Cyc[MinorCyle].Mess_Count = MsgCnt;
	     Min_Cyc[MinorCyle].MinorFrame_Time = MinorCyleTime;
	   }*/
          // for(MinorCyle=0;MinorCyle<MinorCyleCnt;MinorCyle++)
           {
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
            /* for(POS1=400;POS1<500;POS1++)
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
             } */             
           }               	
	DefMajorFrame1553(ModId,0,MinorCyleCnt,Min_Cyc,1);
	printf("RegRead 0x01 = %x\n",RegRead(ModId,0x01,1));
	printf("RegRead 0x07 = %x\n",RegRead(ModId,0x07,1));
	printf("BC Major Frame Started Wait........! \n");	
	RunBcMajorFrame1553(ModId,0,MinorCyleCnt,Min_Cyc,Times,1);
	printf("BC Major Frame Completed........! \n");
	printf("RegRead 0x01 = %x\n",RegRead(ModId,0x01,1));
	printf("RegRead 0x07 = %x\n",RegRead(ModId,0x07,1));
        getchar();
        RegWrite(ModId,0x07,0x0000,1); 
        ResetAce_DDC(ModId);	
        printf("Device Reset.............!\n");
        getchar();
        getchar();	  
	return 0;
}  
