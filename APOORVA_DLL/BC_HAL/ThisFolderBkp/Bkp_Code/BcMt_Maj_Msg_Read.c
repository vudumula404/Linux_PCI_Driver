#include <stdio.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"
#define BUFFER_SIZE 32
MSG_DEF_DDC	bc_page;
RT_MSG_DDC	RtBuff;
MSG_RESULT_DDC	msg_return;
unsigned short	frame_items[4096],data_buffer[40],temp_data;
unsigned short	Temp_Buff[40];
MAJOR_FRAME Min_Cyc[128];
short ReadMsgResult1553_DDC(UCHAR ModId,unsigned short frame_num,unsigned short frame_item,MSG_RESULT_DDC *result)
{
         MSG_DEF_DDC	data;
         unsigned int position,cntrl_wrd,msg_addr;
         unsigned int  CmdStack=0x1000;
        unsigned int  DataStack=0x4000;
         unsigned int i,temp;
         int		option,error=0;

        if (frame_num != STACK_1_DDC )
	  error = -26;
       
        if (frame_item >1024 || frame_item <1 )
	  error = -27;

        position = CmdStack+frame_num +((frame_item-1)*4);
        result->bsw  = MemRead(ModId,position,1);
        result->bsw_bits = ReadBswBits_DDC(ModId,result->bsw);
        result->time_tag = MemRead(ModId,position+1,1);
        msg_addr = MemRead(ModId,position+3,1);  

        cntrl_wrd = MemRead(ModId,msg_addr,1);
        result->cntrl_word = cntrl_wrd;
        data = ReturnCntrlFields_DDC(ModId,cntrl_wrd);
        result->cmd_1 = MemRead(ModId,msg_addr+1,1);
	
        if (data.rt_rt == 0 && data.broadcast == 0 && data.mode_code == 0)
        {
          result->cmd1_bits = ReturnCmdFields_DDC(ModId,result->cmd_1);
          if (result->cmd1_bits.t_r == RECEIVE_DDC )
             option = 0;
          else if (result->cmd1_bits.t_r == TRANSMIT_DDC)
          option = 1;
	          
         }
         else if (data.rt_rt == 1 )
	  option = 2;  
         else if ( data.mode_code == 1 && data.broadcast == 0 )
         {
            result->cmd1_bits = ReturnCmdFields_DDC(ModId,result->cmd_1);
            if ( (result->cmd1_bits.wcnt & 0x0010) != 0 ) 
            {
               if (result->cmd1_bits.t_r == RECEIVE_DDC)
                option = 3;    
               else if (result->cmd1_bits.t_r == TRANSMIT_DDC )
                option = 4;
            }
            else if ((result->cmd1_bits.wcnt & 0x0010) == 0)
            option = 5;      
          }
          if(data.broadcast == 1)
          {
            result->cmd1_bits = ReturnCmdFields_DDC(ModId,result->cmd_1);
            option = 6;
            if(data.mode_code == 1)
            {
            if ((result->cmd1_bits.wcnt & 0x0010) == 0)
            option = 7;
            else
            option = 8;
            }
          }
	switch (option)
	{
		case 0 :   
			temp = result->cmd1_bits.wcnt;
			if(temp == 0)
				temp = 32;
			for (i=0;i<temp;i++)
			result->data[i] = MemRead(ModId,msg_addr + 2 +i,1);
			result->loop_word = MemRead(ModId,msg_addr + 1 + temp + 1,1);
			result->status_1 = MemRead(ModId,msg_addr + 1 + temp + 2,1);
			result->stat1_bits = ReadStatusBits_DDC(ModId,result->status_1);
			break;
		case 1 :   
			result->loop_word = MemRead(ModId,msg_addr + 2,1);
			result->status_1 = MemRead(ModId,msg_addr + 3,1);
			result->stat1_bits = ReadStatusBits_DDC(ModId,result->status_1);
			temp = result->cmd1_bits.wcnt;
			if(temp == 0)
				temp = 32;
			for (i=0;i< temp;i++)
				result->data[i] = MemRead(ModId,msg_addr + 4 + i,1);
			break;
		case 2 :
			result->cmd_1 = MemRead(ModId,msg_addr+1,1);
			result->cmd1_bits = ReturnCmdFields_DDC(ModId,result->cmd_1);
			result->cmd_2 = MemRead(ModId,msg_addr + 2,1);
			result->cmd2_bits = ReturnCmdFields_DDC(ModId,result->cmd_2);
			result->loop_word = MemRead(ModId,msg_addr + 3,1);
			result->status_1 = MemRead(ModId,msg_addr +  4,1);
			result->stat1_bits = ReadStatusBits_DDC(ModId,result->status_1);
			temp = result->cmd1_bits.wcnt;
			if(temp == 0)
				temp = 32;
			for(i=0;i<temp;i++)
				result->data[i] = MemRead(ModId,msg_addr + 5 + i,1);
			result->status_2 = MemRead(ModId,msg_addr + 5 + i,1);
			result->stat2_bits = ReadStatusBits_DDC(ModId,result->status_2);
			break;
		case 3 :   
			result->data[0] = MemRead(ModId,msg_addr + 2 ,1);
			result->loop_word = MemRead(ModId,msg_addr + 3,1);
			result->status_1 = MemRead(ModId,msg_addr + 4,1);
			break;
		case 4 :
			result->loop_word = MemRead(ModId,msg_addr + 2,1);
			result->status_1 = MemRead(ModId,msg_addr + 3,1);
			result->stat1_bits = ReadStatusBits_DDC(ModId,result->status_1);
			result->data[0] = MemRead(ModId,msg_addr + 4,1);
			break;
		case 5 :

			result->loop_word = MemRead(ModId,msg_addr + 2,1);
			result->status_1 = MemRead(ModId,msg_addr + 3,1);
			result->stat1_bits = ReadStatusBits_DDC(ModId,result->status_1);
			break;
		case 6 :

			temp = result->cmd1_bits.wcnt;
			if(temp == 0)
			temp = 32;
			for (i=0;i<temp;i++)
			{
				result->data[i] = MemRead(ModId,msg_addr + 2 +i,1);
			}
			result->loop_word = MemRead(ModId,msg_addr + 1 + temp + 1,1);
			break;
		case 7 :
			result->loop_word = MemRead(ModId,msg_addr + 2,1);
			break;
		case 8 :
			result->data[0] = MemRead(ModId,msg_addr + 2,1);
			result->loop_word = MemRead(ModId,msg_addr + 3,1);
			break;

	}  
	return (error);
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
	 /*if(msg_num<100)
	  MemWrite(ModId, CmdStack + 2 + ((msg_num - 1) * 4), 1000, 1);
	else if((msg_num>100)&&(msg_num<200))
	  MemWrite(ModId, CmdStack + 2 + ((msg_num - 1) * 4), 2000, 1);
	else*/
	 // MemWrite(ModId, CmdStack + 2 + ((msg_num - 1) * 4), 1000, 1);
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
		buff_offset = (DataStack + (msg_num -1)*40) + 4;
		for (i=0; i<num_of_data_words;i++)
		MemWrite(ModId,buff_offset+i,data_buffer[i],1);
		buff_offset = DataStack +((msg_num-1)*40)+2;
		MemWrite(ModId,buff_offset,def.loop,1);
		buff_offset = DataStack + ((msg_num-1)*40)+3;
		MemWrite(ModId,buff_offset,def.cmd_1.status,1);
		break;
	case 2 :
		buff_offset = DataStack + ((msg_num-1)*40)+3;
		MemWrite(ModId,buff_offset,def.loop,1);
		buff_offset = DataStack + ((msg_num-1)*40)+4;
		MemWrite(ModId,buff_offset,def.cmd_2.status,1);
		buff_offset = (DataStack + (msg_num -1)*40) + 5;
		for (i=0; i<num_of_data_words;i++)
			MemWrite(ModId,buff_offset+i,data_buffer[i],1);
		buff_offset = DataStack + ((msg_num-1)*40)+5+num_of_data_words;
		MemWrite(ModId,buff_offset,def.cmd_1.status,1);
		break;

	case 3 :	  
		if (def.cmd_1.subaddr == 31 || def.cmd_1.subaddr == 0)
		{
	            if ((def.cmd_1.t_r == TRANSMIT_DDC) &&(( def.cmd_1.wcnt == 16) ||( def.cmd_1.wcnt == 18) || ( def.cmd_1.wcnt == 19)) )
	            {
	              buff_offset = DataStack + ((msg_num-1)*40)+2;
	              MemWrite(ModId,buff_offset,def.loop,1);
	              buff_offset = DataStack + ((msg_num-1)*40)+3;
	              MemWrite(ModId,buff_offset,def.cmd_1.status,1);
	              buff_offset = DataStack + ((msg_num-1)*40)+4;
	              MemWrite(ModId,buff_offset,data_buffer[0],1);
	            }

                  else if (def.cmd_1.t_r == RECEIVE_DDC && (( def.cmd_1.wcnt == 20) || (def.cmd_1.wcnt == 17) || (def.cmd_1.wcnt == 21)) )
                  {
                    buff_offset = DataStack + ((msg_num-1)*40)+2;
                    MemWrite(ModId,buff_offset,data_buffer[0],1);
                    buff_offset = DataStack + ((msg_num-1)*40)+3;
                    MemWrite(ModId,buff_offset,def.loop,1);
                    buff_offset = DataStack + ((msg_num-1)*40)+4;
                    MemWrite(ModId,buff_offset,def.cmd_1.status,1);
                  }
                  else
                  {
                    buff_offset = DataStack + ((msg_num-1)*40)+2;
                    MemWrite(ModId,buff_offset,def.loop,1);
                    buff_offset = DataStack + ((msg_num-1)*40)+3;
                    MemWrite(ModId,buff_offset,def.cmd_1.status,1);
                  } 
                } 
	    break;
        }   

   return(error);
}
short DefFrame1553_DDC(UCHAR ModId,unsigned short number_of_messages,unsigned short mess_sequence[1024])
{
 int i,error=0;
 unsigned int msg_address,temp;
 unsigned int msg_offset;
//unsigned int CmdStack=0x1000;
//unsigned int CmdStack=0x140;
	if (number_of_messages <= 1024 && number_of_messages >= 1)	
	{
		for (i=0; i<number_of_messages; i++)
		{
			msg_address = 0x0000;
			msg_offset  = 0x0000;
			temp = (mess_sequence[i]-1);
			msg_address = ((temp) * 40) + 0x4000;
			msg_offset  = 0x1000+((i) * 4)  + 0x0003;
			MemWrite(ModId,msg_offset,msg_address,1);
			// printf("DefFrame_num %d msg_offset %x  Msg  %x\n",i,msg_offset,MemRead(ModId,msg_offset,1));
		}

		MemWrite(ModId,(msg_offset+0x1000)+4,END_OF_FRAME_DDC,1);
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
	//unsigned int CmdStack=0x140;
	position	=	frame +(position-1)*4;
	init_pos	=	position+CmdStack;

    if(frame==0x0)
     {
          RegWrite(ModId,CONFIG_REG_DDC,RegRead(ModId,CONFIG_REG_DDC,1)&0xdfff,1);
	  for (i=position;i<num_of_messages;i++)
	
           {
		    msg_addr = position +3 ;
		   /*if (msg_addr == 0x00FF)
			error= -25;*/
	       }

        if(times==0)
        {
         RegWrite(ModId,0x7,0x8000,1);
         RegWrite(ModId,0x1,RegRead(ModId,0x1,1)|0x0100,1);
         printf("RegMsg_Cntr:(0x1) %x\n",RegRead(ModId,0x1,1));
         MemWrite(ModId,MESS_COUNTER_DDC,num_of_messages,1);
         StartReset_DDC(ModId,START_DDC);
         printf("BC Message Based Continuous Scheduling\n");
        }
	else
	 {
		//for (i=0;i<times;i++)
		{
			//MemWrite(ModId,CmdStack,init_pos,1);
			times=times<<7;
	                RegWrite(ModId,0x10,times,1);
	               
			temp = 0xffff - (num_of_messages/**times*/);
			MemWrite(ModId,MESS_COUNTER_DDC,num_of_messages,1);//MESS_COUNTER_DDC 0x101
			count=MemRead(ModId,MESS_COUNTER_DDC,1);
		        printf("RegMsg_Cntr:(0x11) %x MemMsg_Cntr:(0x101) %x\n",RegRead(ModId,0x11,1),count);
			/*printf("MESS_COUNTER_DDC %x temp %x num_of_messages  %d\n",MESS_COUNTER_DDC,temp,num_of_messages);
			getchar();
			getchar();*/
			/*MemWrite(ModId,TMP_CNTR_DDC,temp,1);     
			MemWrite(ModId,TMP_PTR_DDC,init_pos,1);  */ 

			StartReset_DDC(ModId,START_DDC);

			/*count=MemRead(ModId,MESS_COUNTER_DDC,1);
			     
         	         while (count != 0xFFFF)
         	         {
		          count=MemRead(ModId,MESS_COUNTER_DDC,1);
		         // printf("MESS_COUNTER_DDC %d Readcount %x  temp %x num_of_messages  %d\n",MESS_COUNTER_DDC,count,temp,num_of_messages);
           		  temp=0;
         		}*/
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
 // RegWrite(Bus,0x7,0x8000,1);
  RegWrite(Bus,0x3,0x8000,1);
  RegWrite(Bus,0x10,no_of_minor_frames,1);
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
        
         RegWrite(Bus,0x07,0x8000,1); 
          RegWrite(Bus,0x01,0x054f,1);            
        // RegWrite(Bus,0x1,RegRead(Bus,0x1,1)|0x0100,1);
         printf("RegMsg_Cntr:(0x1) %x\n",RegRead(Bus,0x1,1));
       // RegWrite(Bus,0x07,0x3000,1);
        StartReset_DDC(Bus,START_DDC);		 
        printf("BC Frame Based Continuous Scheduling  %x\n",START_DDC);
      }
      else
      {
        //for (i=0;i<times;i++)
        {
          times=times<<7;
	  RegWrite(Bus,0x10,(RegRead(Bus,0x10,1)|times),1);
	  printf("RegRead 0x10 = %x\n",RegRead(Bus,0x10,1));
          RegWrite(Bus,0x3,0x8000,1);
          MemWrite(Bus,CmdStack,init_pos,1);
          //temp = 0xffff - (num_of_messages*times);
          //MemWrite(Bus,MESS_COUNTER_DDC,temp,1);//MESS_COUNTER_DDC 0x101
         // printf("MESS_COUNTER_DDC %d num_of_messages*times %d\n",MemRead(Bus,MESS_COUNTER_DDC,1),num_of_messages*times);
         // MemWrite(Bus,TMP_CNTR_DDC,temp,1);     
         // MemWrite(Bus,TMP_PTR_DDC,init_pos,1);   
          RegWrite(Bus,0x3,0x8000,1);
          RegWrite(Bus,0x01,0x044f,1);
          RegWrite(Bus,0x7,0x0000,1);
          StartReset_DDC(Bus,START_DDC);
          //count=MemRead(Bus,MESS_COUNTER_DDC,1);
          printf("RegRead 0x07 = %x\n",RegRead(Bus,0x07,1));
          /*while (count != 0xFFFF)
          {
          count=MemRead(Bus,MESS_COUNTER_DDC,1);
          //printf("MESS_COUNTER_DDC %d Readcount %x  temp %x num_of_messages  %d\n",MESS_COUNTER_DDC,count,temp,num_of_messages);
          temp=0;
          }*/
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
	int			RtAddre=0,i,MemAdr=0,Ch=0x0,TempCnt,RetVal=0x0,Times=0x1,POS1=0,MsgFrame=0;
	unsigned short		MemBuff,MsgNo=0,Data=0;
	unsigned char		CardId,FailCount=0,ModId=0,MTModId=3,Com_Or_MemTest=0;
	char temp;
	unsigned short mess_sequence[1024];
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
	  printf("\n\t\tEnter Messages/Frame Based Scheduler (0Message/1Frame):");
	  scanf("%d",&MsgFrame);
	  if(MsgFrame==1)
	  {
	    printf("\n\t\tEnter Number Of MinorCycle Count :");
	    scanf("%d",&MinorCyleCnt);
	    printf("\n\t\tEnter Number Of MinorCycle Time :");
	    scanf("%d",&MinorCyleTime);
	  }
	  
	  printf("\n\t\tEnter Number Of Messages To Run (Times*Messages):");
	  scanf("%d",&Times);
	 	  
	  RetVal	=	ClearRam_DDC(ModId);
	  printf("BC Retval of ClearRam is :%d ModId %d\n",RetVal,ModId);
	  ResetAce_DDC(ModId); 
	  //printf("BC Retval of ResetAce is :%d\n",RetVal);
	  //RetVal	=	DefMode_DDC(ModId,BC_DDC);
	  RegWrite(ModId,0x1,0x054f,1);
	  MemWrite(ModId,0X100,0X1000,1);
	  MemWrite(ModId,0X1003,0X4000,1);
	 // RegWrite(ModId,0x15,MsgCnt*Times,1);
	  
	  printf("BCRetval of DefMode is :%x\n",RetVal);
          //printf("RegRead 0x15 = %x",RegRead(ModId,0x15,1));
          
	  printf("\n Enter anychar to see the Mt results after running the BC\n");
	  getchar();	  
	  if(Ch	==	1)
		bc_page.bus       = BUS_A_DDC;		
	  else if(Ch	==	0)
		bc_page.bus       = BUS_B_DDC;
	    for (MsgNo = 1; MsgNo <= MsgCnt; MsgNo++)
            {
                // Reset defaults for every message
                if (RtAddre >= 30)
                    RtAddre = 0;

                bc_page.self_test     = NO_DDC;
                bc_page.mask_bcst_bit = NO_DDC;
                bc_page.broadcast     = NO_DDC;
                bc_page.mode_code     = NO_DDC;
                bc_page.rt_rt         = NO_DDC;
                //bc_page.retry_enable=YES;

                bc_page.cmd_1.taddr   = ++RtAddre;    
                bc_page.cmd_1.t_r     = RECEIVE_DDC;   // default

                // Message group selection
                if (MsgNo <= 100)                           
                {
                    bc_page.cmd_1.t_r = RECEIVE_DDC;        // 0–100 → BC TO RT                   
                    bc_page.cmd_1.status  = 0xCCCC;
                    bc_page.cmd_1.subaddr = 1;
                    bc_page.cmd_1.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                    for (i = 0; i < 32; i++)
                      data_buffer[i] = ++Data;
                   // Define message
                    DefMsg1553_DDC(ModId, MsgNo, bc_page, data_buffer, 32);
                }
                else if (MsgNo <= 200)                      // 100–200 → RT TO BC
                {
                    bc_page.cmd_1.t_r = TRANSMIT_DDC;
                    bc_page.cmd_1.status  = 0xCCCC;
                    bc_page.cmd_1.subaddr = 1;
                    bc_page.cmd_1.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                     for (i = 0; i < 32; i++)
                      data_buffer[i] = 0;
                   // Define message
                    DefMsg1553_DDC(ModId, MsgNo, bc_page, data_buffer, 32);
                }
                else if (MsgNo <= 300)                      // 200–300 → Broadcast
                {
                    bc_page.cmd_1.taddr = 31;
                    bc_page.broadcast   = YES_DDC;
                    bc_page.cmd_1.t_r   = RECEIVE_DDC;
                    bc_page.cmd_1.status  = 0xCCCC;
                    bc_page.cmd_1.subaddr = 1;
                    bc_page.cmd_1.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                    //printf("BROADCAST %d\n", bc_page.broadcast);
                     for (i = 0; i < 32; i++)
                      data_buffer[i] = ++Data;
                   // Define message
                    DefMsg1553_DDC(ModId, MsgNo, bc_page, data_buffer, 32);
                }
                else if (MsgNo <= 400)                      // 300–400 → RT-to-RT
                {
                    bc_page.cmd_1.status  = 0xCCCC;
                    bc_page.cmd_1.subaddr = 1;
                    bc_page.cmd_1.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                    bc_page.cmd_1.t_r   = RECEIVE_DDC;
                    bc_page.cmd_2.t_r   = TRANSMIT_DDC;
                    bc_page.rt_rt       = YES_DDC;
                    bc_page.cmd_2.taddr = (RtAddre == 30) ? 1 : 30;
                    bc_page.cmd_2.status  = 0xCCCC;
                    bc_page.cmd_2.subaddr = 1;
                    bc_page.cmd_2.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                     for (i = 0; i < 32; i++)
                      data_buffer[i] = 0;
                   // Define message
                    DefMsg1553_DDC(ModId, MsgNo, bc_page, data_buffer, 32);
                }
                else if (MsgNo <= 500)                      // 300–400 → RT-to-RT
                {
                    bc_page.broadcast     = YES_DDC;
                    bc_page.cmd_1.taddr   = 31;    
                    bc_page.cmd_1.status  = 0xCCCC;
                    bc_page.cmd_1.subaddr = 1;
                    bc_page.cmd_1.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                    bc_page.cmd_1.t_r   = RECEIVE_DDC;
                    bc_page.cmd_2.t_r   = TRANSMIT_DDC;
                    bc_page.rt_rt       = YES_DDC;
                    bc_page.cmd_2.taddr = (RtAddre == 30) ? 1 : RtAddre;
                    bc_page.cmd_2.status  = 0xCCCC;
                    bc_page.cmd_2.subaddr = 1;
                    bc_page.cmd_2.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                     for (i = 0; i < 32; i++)
                      data_buffer[i] = 0;
                   // Define message
                    DefMsg1553_DDC(ModId, MsgNo, bc_page, data_buffer, 32);
                }
                else if (MsgNo >= 601 && MsgNo <= 632) // Reserve MsgNo range for Mode Codes
                {
                    int datacount = 0;
                    int mode_code = MsgNo - 601; // 0..31
                                       
                    // Initialize common mode code settings
                    bc_page.self_test = NO_DDC;
                    bc_page.mask_bcst_bit = NO_DDC;
                    bc_page.broadcast = NO_DDC;
                    bc_page.rt_rt = NO_DDC;
                    bc_page.mode_code = YES_DDC;
                    bc_page.cmd_1.taddr = 1; // Default to RT address 1
                    bc_page.cmd_1.subaddr = 0x00; // Mode codes always subaddr 0
                    bc_page.cmd_1.status = 0xCCCC;
                    bc_page.loop = 0x0000;
                    
                    switch (mode_code)
                    {
                        case 0: // Dynamic Bus Control Acceptance (DBCA) W/O Data BroadCast No {Tx}
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 0;
                            datacount = 0;
                            break;
                            
                        case 1: // Sync W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 1;
                            datacount = 0;
                            break;                            
                        case 2: // Transmit Status W/O Data BroadCast No {Tx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 2;
                            datacount = 0;
                            break;                            
                        case 3: // Initiate Self Test W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 3;
                            datacount = 0;
                            break;
                            
                        case 4: // Transmitter Shut Down W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 4;
                            datacount = 0;
                            break;
                            
                        case 5: // Override Transmitter Shut Down W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 5;
                            datacount = 0;
                            break;
                            
                        case 6: // Inhibit Terminal Flag W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 6;
                            datacount = 0;
                            break;
                            
                        case 7: // Override Inhibit Terminal Flag W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 7;
                            datacount = 0;
                            break;
                            
                        case 8: // Reset Remote Terminal W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 8;
                            datacount = 0;
                            break;
                            
                        case 16: // Transmit Vector Word With Data BroadCast YES {Tx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 16;
                            datacount = 1;
                            data_buffer[0] = 0x0000;
                            break;
                            
                        case 17: // Sync With Data With Data BroadCast YES {Rx}
                            bc_page.broadcast = YES_DDC;
                            bc_page.cmd_1.taddr = 31; // Broadcast address
                            bc_page.cmd_1.t_r = RECEIVE_DDC;
                            bc_page.cmd_1.wcnt = 17;
                            datacount = 1;
                            data_buffer[0] = ++Data;
                            break;
                            
                        case 18: // Transmit Last Command With Data BroadCast YES {Tx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.subaddr = 0; // Mode codes always subaddr 0
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 18;
                            datacount = 1;
                            data_buffer[0] = 0x0000;
                            break;
                            
                        case 19: // Transmit Built-In-Test With Data BroadCast YES {Tx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.subaddr = 0; // Mode codes always subaddr 0
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 19;
                            datacount = 1;
                            data_buffer[0] = 0x0000;
                            break;
                            
                        case 20: // Selected Transmitter S/D With Data BroadCast YES {Rx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = RECEIVE_DDC;
                            bc_page.cmd_1.wcnt = 20;
                            datacount = 1;
                            data_buffer[0] = ++Data;
                            break;
                            
                        case 21: // Override-Selected Tx S/D With Data BroadCast YES {Rx}
                            bc_page.broadcast = NO_DDC;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = RECEIVE_DDC;
                            bc_page.cmd_1.wcnt = 21;
                            datacount = 1;
                            data_buffer[0] = ++Data;
                            break;
                            
                        default: // Handle undefined mode codes 9-15, 22-31
                            bc_page.cmd_1.t_r = TRANSMIT_DDC;
                            bc_page.cmd_1.wcnt = 0;
                            datacount = 0;
                            data_buffer[0] = 0;
                            break;
                    }
                    
                    // Define the Mode Code message
                    DefMsg1553_DDC(ModId, MsgNo, bc_page, data_buffer, datacount);
                }
                else
                {
                    bc_page.cmd_1.t_r = RECEIVE_DDC;        // 500–1000 → BC TO RT                   
                    bc_page.cmd_1.status  = 0xCCCC;
                    bc_page.cmd_1.subaddr = 1;
                    bc_page.cmd_1.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                    for (i = 0; i < 32; i++)
                      data_buffer[i] = ++Data;
                   // Define message
                    DefMsg1553_DDC(ModId, MsgNo, bc_page, data_buffer, 32);
                }            
                
            }
           
	  if(MsgFrame==1)
	  {
	    for (MinorCyle = 0; MinorCyle < MinorCyleCnt; MinorCyle++)
            {
                int start = MinorCyle * 100;
                int end   = start + 100;

                for (POS1 = start; POS1 < end; POS1++)
                {
                    Min_Cyc[MinorCyle].frame_items[POS1 - start] = POS1 + 1;
                }

                Min_Cyc[MinorCyle].Mess_Count      = 100;        
                Min_Cyc[MinorCyle].MinorFrame_Time = MinorCyleTime;
            }	         	
	    DefMajorFrame1553(ModId,0,MinorCyleCnt,Min_Cyc,1);
	    printf("BC Major Frame Started Wait........! \n");	
	    RunBcMajorFrame1553(ModId,0,MinorCyleCnt,Min_Cyc,Times,1);
	    printf("BC Major Frame Completed........! \n");
	    getchar();
	}
	else
	{
	  for(POS1=0;POS1<(MsgCnt);POS1++)
	      mess_sequence[POS1]= POS1+1;
	  DefFrame1553_DDC(ModId,MsgCnt,mess_sequence);
	  RunBc1553_DDC(ModId,0,1,MsgCnt,Times);
	  getchar();
	}
	for(i=0;i<Times;i++)
	{
	  for(POS1=1;POS1<(MsgCnt+1);POS1++)
	  {	   
                ReadMsgResult1553_DDC(ModId, STACK_1_DDC, POS1, &msg_return);
                printf("\nBC Msg #%d\nBSW: 0x%X CMD: 0x%X SubAddr: %d WCnt: %d T/R: %d\nData:\n", POS1, msg_return.bsw,msg_return.cmd_1,         msg_return.cmd1_bits.subaddr, msg_return.cmd1_bits.wcnt, msg_return.cmd1_bits.t_r);
                for (int j = 0; j < (msg_return.cmd1_bits.wcnt ? msg_return.cmd1_bits.wcnt : BUFFER_SIZE); ++j)
                {
                    printf("%04X ", msg_return.data[j]);
                    if ((j + 1) % 8 == 0) printf("\n");
                }
                printf("\nLoopWord: %04X Status: %04X\n", msg_return.loop_word, msg_return.status_1);
      	  }
    	}
	 // StartReset_DDC(ModId,START_DDC);
	 
        //getchar();
        RegWrite(ModId,0x07,0x0000,1); 
        ResetAce_DDC(ModId);	
        printf("Device Reset.............!\n");
        getchar();
        getchar();	  
	return 0;
}  
