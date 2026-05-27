#include <stdio.h>
#include <stdint.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"

#define BUFFER_SIZE 32
#define HIGH  1
#define LOW  0
/*
 * Kept globals that are actively used by PCI API calls and message definition.
 * Removed unused temporaries and redundant buffers.
 */
MSG_DEF bc_page;
unsigned short data_buffer[40];
MAJOR_FRAME major_frames[128];
short DefMsg1553(UCHAR ModId,unsigned short msg_num,MSG_DEF def,unsigned short data_buffer[],unsigned short num_of_data_words,unsigned short IntMsgGap)
{
  unsigned int cmd=0;
  unsigned int cntrl=0;
  unsigned int cmd_offset=0;
  unsigned int cntrl_offset=0;
  unsigned int buff_offset=0; 
  int i,option=0,error=0;
  unsigned int  CmdStack=0x1000;
  unsigned int  DataStack=0x4000;

  if (def.cmd_1.t_r == RECEIVE && def.rt_rt == NO && def.mode_code == NO)
	  option = 0;
  else if (def.cmd_1.t_r == TRANSMIT && def.rt_rt == NO && def.mode_code == NO)
	  option = 1;
  else if (def.mode_code == YES)
	  option = 3;

	cmd =cmd | def.cmd_1.taddr<<11;
	cmd =cmd | def.cmd_1.t_r << 10;
	cmd =cmd | def.cmd_1.subaddr <<5;
	cmd =cmd | (def.cmd_1.wcnt % 32);
	cmd_offset = DataStack+((msg_num-1)*40)+1;
	MemWrite(ModId,cmd_offset,cmd,1);
       // printf("msg_num %d Address %x CommandWord %x\n",msg_num,cmd_offset,MemRead(ModId,cmd_offset,1));
	if(def.rt_rt == YES )
	{
		if (def.cmd_1.t_r != RECEIVE)
			error = -20;
		else if (def.cmd_1.wcnt != def.cmd_2.wcnt)
			error= -21;
		else  if (def.cmd_2.t_r != TRANSMIT)
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
	 //printf("msg_num %d Address %x ControlWord %x\n",msg_num,cntrl_offset,MemRead(ModId,cntrl_offset,1));
	 MemWrite(ModId, CmdStack + 2 + ((msg_num - 1) * 4), IntMsgGap, 1);//InterMesgGap	
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
	            if ((def.cmd_1.t_r == TRANSMIT) &&(( def.cmd_1.wcnt == 16) ||( def.cmd_1.wcnt == 18) || ( def.cmd_1.wcnt == 19)) )
	            {
	              buff_offset = DataStack + ((msg_num-1)*40)+2;
	              MemWrite(ModId,buff_offset,def.loop,1);
	              //printf("msg_num %d Address %x (TxCmd)def.cmd_1.wcnt %d def.loop %x\n",msg_num,buff_offset,def.cmd_1.wcnt,MemRead(ModId,buff_offset,1));
	              buff_offset = DataStack + ((msg_num-1)*40)+3;
	              MemWrite(ModId,buff_offset,def.cmd_1.status,1);
	              //printf("msg_num %d Address %x (TxCmd)def.cmd_1.wcnt %d def.status %x\n",msg_num,buff_offset,def.cmd_1.wcnt,MemRead(ModId,buff_offset,1));
	              buff_offset = DataStack + ((msg_num-1)*40)+4;
	              MemWrite(ModId,buff_offset,data_buffer[0],1);
                     // printf("msg_num %d Address %x (TxCmd)def.cmd_1.wcnt %d Data %x\n",msg_num,buff_offset,def.cmd_1.wcnt,MemRead(ModId,buff_offset,1));
	            }

                  else if (def.cmd_1.t_r == RECEIVE && (( def.cmd_1.wcnt == 20) || (def.cmd_1.wcnt == 17) || (def.cmd_1.wcnt == 21)) )
                  {
                    buff_offset = DataStack + ((msg_num-1)*40)+2;
                    MemWrite(ModId,buff_offset,data_buffer[0],1);
                    //printf("msg_num %d Address %x (RxCmd)def.cmd_1.wcnt %d Data %x\n",msg_num,buff_offset,def.cmd_1.wcnt,MemRead(ModId,buff_offset,1));
                    buff_offset = DataStack + ((msg_num-1)*40)+3;
                    MemWrite(ModId,buff_offset,def.loop,1);
                   // printf("msg_num %d Address %x (RxCmd)def.cmd_1.wcnt %d Loop %x\n",msg_num,buff_offset,def.cmd_1.wcnt,MemRead(ModId,buff_offset,1));
                    buff_offset = DataStack + ((msg_num-1)*40)+4;
                    MemWrite(ModId,buff_offset,def.cmd_1.status,1);
                   //  printf("msg_num %d Address %x (RxCmd)def.cmd_1.wcnt %d Status %x\n",msg_num,buff_offset,def.cmd_1.wcnt,MemRead(ModId,buff_offset,1));
                  }
                  else
                  {
                    buff_offset = DataStack + ((msg_num-1)*40)+2;
                    MemWrite(ModId,buff_offset,def.loop,1);
                    //printf("msg_num %d Address %x def.cmd_1.wcnt %d Loop %x\n",msg_num,buff_offset,def.cmd_1.wcnt,MemRead(ModId,buff_offset,1));
                    buff_offset = DataStack + ((msg_num-1)*40)+3;
                    MemWrite(ModId,buff_offset,def.cmd_1.status,1);
                    //printf("msg_num %d Address %x (RxCmd)def.cmd_1.wcnt %d Status %x\n",msg_num,buff_offset,def.cmd_1.wcnt,MemRead(ModId,buff_offset,1));
                  } 
                } 
	    break;
        }   

   return(error);
}
short RunBc1553(UCHAR ModId,unsigned short frame,unsigned short position,unsigned short num_of_messages,unsigned short times)
{
	int				i,msg_addr,temp,init_pos,error=0;
	unsigned int	count;
	unsigned int CmdStack=0x1000;	
	position	=	frame +(position-1)*4;
	init_pos	=	position+CmdStack;
  if (num_of_messages <= 1024 && num_of_messages >= 1)	
  {
    if(frame==0x0)
     {
        RegWrite(ModId,CONFIG_REG,RegRead(ModId,CONFIG_REG,1)&0xdfff,1);
	if(times==0)
        {
         RegWrite(ModId,0x7,0x8000,1);
         RegWrite(ModId,0x1,RegRead(ModId,0x1,1)|0x0100,1);   
         RegWrite(ModId,0x1E,num_of_messages,1);
         MemWrite(ModId,MESS_COUNTER,times,1);
         StartReset(ModId,START);
         printf("BC Message Based Continuous Scheduling\n");
        }
	else
        { 
          RegWrite(ModId,0x1E,num_of_messages,1);
          MemWrite(ModId,MESS_COUNTER,times,1);//MESS_COUNTER 0x101
          count=MemRead(ModId,MESS_COUNTER,1);
          printf("RegMsg_Cntr:(0x11) %x MemMsg_Cntr:(0x101) %x\n",RegRead(ModId,0x1E,1),count);			
          StartReset(ModId,START);
          count=RegRead(ModId,0x1,1);
           while (count != 0x0)//0x54c Completed
           {
              count=RegRead(ModId,0x1,1);//0x54d Incomplete
              count = count&1;
           }
           printf("Message based Scheduling Completed %x\n",RegRead(ModId,0x1,1));          
        }	
      }
        return(error);
    }
    else
      return(-23);  
}
void PriorityHighLow(UCHAR ModId, unsigned short msg_num, unsigned short HighLow)
{
    unsigned int data_stack_base = 0x4000;
    unsigned short PriorityRegAddress=0x1d;
    unsigned int msg_base = data_stack_base + (msg_num - 1) * 40; 
    RegWrite(ModId, PriorityRegAddress, HighLow, 1);
    if(HighLow==1)
      MemWrite(ModId, 0xff80, msg_base, 1);
    else
      MemWrite(ModId, 0xffc0, msg_base, 1);   
}
#if 0
void UpdateMsgData(UCHAR ModId, unsigned short msg_num, unsigned short *new_data, int num_words, int direction)
{
    unsigned int data_stack_base = 0x4000;
    unsigned int msg_base = data_stack_base + (msg_num - 1) * 40;
    unsigned int buff_offset;
    int idx;

    if (direction == RECEIVE)   // BC → RT
        buff_offset = msg_base + 2;
    else if (direction == TRANSMIT) // RT → BC
        buff_offset = msg_base + 4;
    else
        return;

    for (idx = 0; idx < num_words; idx++)
    {
        MemWrite(ModId, buff_offset + idx, new_data[idx], 1);
    }
}

int UpdateMsgCmdData(UCHAR ModId,
                     unsigned short msg_num,
                     unsigned short cmd1,
                     unsigned short cmd2,
                     unsigned short *payload_buffer,
                     unsigned short num_of_data_words,
                     int direction,       // RECEIVE / TRANSMIT / RT_RT / MODE_CODE
                     int bus)             // 0 = Bus A, 1 = Bus B
{
    unsigned int data_stack_base = 0x4000;
    unsigned int base = data_stack_base + (msg_num - 1) * 40;
    unsigned int offset, control_word;
    int idx;

    /* Update command words when provided */
    if (cmd1)
        MemWrite(ModId, base + 1, cmd1, 1);

    if (cmd2 && direction == 2)   /* RT–RT */
        MemWrite(ModId, base + 2, cmd2, 1);

    /* Build control word */
    control_word = 0;
    control_word |= (0 << 8);                 /* retry_enable placeholder */
    control_word |= (bus & 0x1) << 7;         /* bus: 0 = A, 1 = B */
    control_word |= (0 << 6);                 /* self_test */
    control_word |= (0 << 5);                 /* mask_bcst_bit */
    control_word |= ((direction == 3) ? 1 : 0) << 2; /* mode_code flag */
    control_word |= ((direction == 2) ? 1 : 0);      /* rt_rt flag */
    control_word |= (0 << 1);                 /* broadcast */

    MemWrite(ModId, base + 0, control_word, 1);

    /* Update payload area depending on message type */
    switch (direction)
    {
        case RECEIVE:   /* BC → RT */
            offset = base + 2;
            for (idx = 0; idx < num_of_data_words; idx++)
                MemWrite(ModId, offset + idx, payload_buffer[idx], 1);
            break;

        case TRANSMIT:  /* RT → BC */
            offset = base + 4;
            for (idx = 0; idx < num_of_data_words; idx++)
                MemWrite(ModId, offset + idx, payload_buffer[idx], 1);
            break;

        case 2:   /* RT–RT */
            offset = base + 5;
            for (idx = 0; idx < num_of_data_words; idx++)
                MemWrite(ModId, offset + idx, payload_buffer[idx], 1);
            break;

        case 3:   /* Mode Code */
        {
            if (cmd1) MemWrite(ModId, base + 1, cmd1, 1);

            unsigned short word_count = cmd1 & 0x1F;       /* 5 bits word count/mode code */
            unsigned short tr_flag    = (cmd1 >> 10) & 0x1;

            if (tr_flag == TRANSMIT && (word_count == 16 || word_count == 18 || word_count == 19)) {
                MemWrite(ModId, base + 2, 0x0001, 1);
                MemWrite(ModId, base + 3, 0x0000, 1);
                if (num_of_data_words > 0)
                    MemWrite(ModId, base + 4, payload_buffer[0], 1);
            }
            else if (tr_flag == RECEIVE && (word_count == 17 || word_count == 20 || word_count == 21)) {
                if (num_of_data_words > 0)
                    MemWrite(ModId, base + 2, payload_buffer[0], 1);
                MemWrite(ModId, base + 3, 0x0001, 1);
                MemWrite(ModId, base + 4, 0x0000, 1);
            }
            else {
                MemWrite(ModId, base + 2, 0x0001, 1);
                MemWrite(ModId, base + 3, 0x0000, 1);
            }
        }
        break;

        default:
            return -1; /* invalid */
    }

    return 0;
}
#endif
void BCRetry1553(UCHAR ModId,USHORT sel)
{
	USHORT temp=0,val;	
	if(sel)
	{
		RegWrite(ModId,CONFIG_REG3,0x8000,1);
		temp=RegRead(ModId,CONFIG_REG,1);
		RegWrite(ModId,CONFIG_REG,(temp&0xffe7)|(0x0013),1);		
		//RegWrite(ModId,CONFIG_REG,0x458,1);//(0x458 Double Retry Enale 0x450 for Single retry)
		printf("BCRetry sel %d \n",sel);
		switch(sel)
		{

		case 1://S		       
		        RegWrite(ModId,CONFIG_REG,0x450,1);
			RegWrite(ModId,CONFIG_REG3,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4,1);
			RegWrite(ModId,CONFIG_REG4,val | 0x1200 ,1);
			printf("SameBus(S) Reg(0x1) %x Reg(0x4) %x Reg(0x3) %x\n",RegRead(ModId,CONFIG_REG,1),RegRead(ModId,CONFIG_REG4,1),RegRead(ModId,CONFIG_REG3,1));
			break;

		case 2://A
		        RegWrite(ModId,CONFIG_REG,0x450/*RegRead(ModId,CONFIG_REG,1) | 0x0*/,1);		       
			RegWrite(ModId,CONFIG_REG3,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4,1);			
			RegWrite(ModId,CONFIG_REG4,val | 0x1300 ,1);
			printf("AlterBus(A)\n");
			break;
		case 3://SS
		        RegWrite(ModId,CONFIG_REG,RegRead(ModId,CONFIG_REG,1) | 0x8,1);
			RegWrite(ModId,CONFIG_REG3,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4,1);
			RegWrite(ModId,CONFIG_REG4,val | 0x1200 ,1);
			printf("DoubleRetry Same(SS)\n");
			break;
	      	case 4://AA
		        RegWrite(ModId,CONFIG_REG,RegRead(ModId,CONFIG_REG,1) | 0x8,1);
			RegWrite(ModId,CONFIG_REG3,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4,1);
			RegWrite(ModId,CONFIG_REG4,val | 0x1300 ,1);
			printf("DoubleRetry Alter(AA)\n");
			break;
		case 5://SA
		        RegWrite(ModId,CONFIG_REG,0x458,1);
		        RegWrite(ModId,CONFIG_REG,RegRead(ModId,CONFIG_REG,1) | 0x8,1);
			RegWrite(ModId,CONFIG_REG3,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4,1);			
			RegWrite(ModId,CONFIG_REG4,val | 0x1280 ,1);
			printf("Same Alter(SA)\n");
			break;
		case 6://AS
		        RegWrite(ModId,CONFIG_REG,RegRead(ModId,CONFIG_REG,1) | 0x8,1);
			RegWrite(ModId,CONFIG_REG3,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4,1);
			RegWrite(ModId,CONFIG_REG4,val | 0x1380 ,1);
			printf("Alter Same(AS)\n",RegRead(ModId,CONFIG_REG3,1),RegRead(ModId,CONFIG_REG4,1));
			break;
		}
	}
	else
	{
		temp=RegRead(ModId,CONFIG_REG,1);
		RegWrite(ModId,CONFIG_REG,(temp&0xffe7),1);
	}

}
unsigned long RunBcMajorFrame1553(UCHAR Bus,USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],unsigned long times)
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
         //printf("RegMsg_Cntr:(0x1) %x\n",RegRead(Bus,0x1,1));
       // RegWrite(Bus,0x07,0x3000,1);
        StartReset(Bus,START);		 
        printf("BC Frame Based Continuous Scheduling  %x\n",START);
      }
      else
      {     
          RegWrite(Bus,0x3,0x8000,1);
          MemWrite(Bus,CmdStack,init_pos,1);
          MemWrite(Bus,MESS_COUNTER,times,1);//MESS_COUNTER 0x101
         // MemWrite(Bus,TMP_CNTR,temp,1);     
         // MemWrite(Bus,TMP_PTR,init_pos,1);   
          RegWrite(Bus,0x3,0x8000,1);
            //printf("Frame Based Scheduling Completed %x\n",RegRead(Bus,0x1,1));  
          //RegWrite(Bus,0x01,0x044f,1);
          //RegWrite(Bus,0x7,0x0000,1);
          StartReset(Bus,START);
          //count=MemRead(Bus,MESS_COUNTER,1);
          //printf("RegRead 0x07 = %x\n",RegRead(Bus,0x07,1));
          count=RegRead(Bus,0x1,1);
          /*while (count != 0x44c)//0x44e Completed
          {
            count=RegRead(Bus,0x1,1);//0x44e Incomplete
          //  count = (0x44f|count);
             // printf("Frame Based Scheduling Completed %x\n",RegRead(Bus,0x1,1));   
          }*/
          while (count != 0)//0x44e Completed
          {
            count=RegRead(Bus,0x1,1);//0x44e Incomplete
            count = (count&2);//getchar();
             //printf("Frame Based Scheduling Completed %x %x\n",RegRead(Bus,0x1,1),count);   
          }
          printf("****Frame Based Scheduling Completed %x\n",RegRead(Bus,0x1,1));        			  
      }	
    }				
    return 1;
}
/*******************************************************************
*  This Program gives code for BC and MT
********************************************************************/
int main(void)
{    
    int idx = 0;
    unsigned short msg_sequence[1024];
    unsigned short minor_cycle_index = 0;
    unsigned short msg_count = 0;
    unsigned short minor_cycle_count = 0;
    unsigned short minor_cycle_time = 0;
    unsigned short inter_msg_gap = 0;
    unsigned short msg_no = 0;
    unsigned short data_value = 0,Retry=0,BCResponseTime=0;
    unsigned char card_id = 0;
    unsigned char fail_count = 0;
    unsigned char module_id = 0;
    unsigned char mt_module_id = 3;
    unsigned char mode_select = 0;
    int channel = 0; /* 0 = CH-B, 1 = CH-A */
    int run_times = 1;
    int use_frame_scheduler = 0;
    
    int init_status = InitCard(1);
    
    if (init_status)
        printf("Card Init Success %d\n", init_status);
    else
        printf("Card Init Failed %d\n", init_status);

    printf("\n\t\tSelect BC ModuleId:1/2/3\n");
    scanf("%hhu", &module_id);

    printf("\n\t\tEnter Channel (CH-A:1, CH-B:0) :");
    scanf("%d", &channel);
    printf("\n\t\tSingleRetry Option : Same:1/Alter:2");
    printf("\n\t\tDoubleRetry Option : SS[3]/AA[4]::SA[5]/AS[6]\t");
    scanf("%d",&Retry);
    printf("\n\t\tEnter BCResponseTime :1/2/3/4\t");
    scanf("%d", &BCResponseTime);
    printf("\n\t\tEnter Number Of Messages Count :");
    scanf("%hu", &msg_count);
    printf("\n\t\tEnter Messages/Frame Based Scheduler (0Message/1Frame):");
    scanf("%d", &use_frame_scheduler);

    if (use_frame_scheduler == 1)
    {
        printf("\n\t\tEnter Number Of MinorCycle Count :");
        scanf("%hu", &minor_cycle_count);
        printf("\n\t\tEnter Number Of MinorCycle Time :");
        scanf("%hu", &minor_cycle_time);
    }
    
    printf("\n\t\tEnter Number Of Messages To Run (Times*Messages):");
    scanf("%d", &run_times);
 
    (void)ClearRam(module_id);
    ResetAce(module_id);
     //RegWrite(module_id,0x7,0x8000,1);
    (void)DefMode(module_id, BC);
   switch (BCResponseTime)
    {
      case 1:
      RegWrite(module_id,0x7,0x8000,1);
      RegWrite(module_id,0x9,(RegRead(module_id,0x9,1))|0x0000,1);
      printf("(1BCWaits 24 Microseconds (If NoResponse) %x",(RegRead(module_id,0x9,1)));
      break;
      case 2:
      RegWrite(module_id,0x7,0x8000,1);
      RegWrite(module_id,0x9,(RegRead(module_id,0x9,1))|0x0200,1);
      printf("2BCWaits 28 Microseconds (If NoResponse) %x",(RegRead(module_id,0x9,1)));
      break;
      case 3:
      RegWrite(module_id,0x7,0x8000,1);
      RegWrite(module_id,0x9,(RegRead(module_id,0x9,1))|0x0400,1);
      printf("3BCWaits 56 Microseconds (If NoResponse) %x",(RegRead(module_id,0x9,1)));
      break;
      case 4:
      RegWrite(module_id,0x7,0x8000,1);
      RegWrite(module_id,0x9,(RegRead(module_id,0x9,1))|0x0600,1);
      printf("4BCWaits 136 Microseconds (If NoResponse) %x",(RegRead(module_id,0x9,1)));
      break;
    }
    if (channel == 1)
        bc_page.bus = BUS_A;
    else
        bc_page.bus = BUS_B;

    for (msg_no = 1; msg_no <= msg_count; msg_no++)
    {
        /* Reset defaults for every message */
        static int rt_address = 0;
        if (rt_address >= 30)
            rt_address = 0;

        bc_page.self_test = NO;
        bc_page.mask_bcst_bit = NO;
        bc_page.broadcast = NO;
        bc_page.mode_code = NO;
        bc_page.rt_rt = NO;
      //  bc_page.retry_enable=YES;
        bc_page.cmd_1.taddr = ++rt_address;
        bc_page.cmd_1.t_r = RECEIVE; /* default */

        if (msg_no <= 100)
        {
            bc_page.cmd_1.t_r = RECEIVE;//RECEIVE;        /* 0–100 → BC TO RT */
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            bc_page.loop = 0x0000;
            for (idx = 0; idx < 32; idx++)
                data_buffer[idx] = ++data_value;

            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no <= 200)
        {
            bc_page.cmd_1.t_r = TRANSMIT;        /* 100–200 → RT TO BC */
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            bc_page.loop = 0x0000;
            for (idx = 0; idx < 32; idx++)
                data_buffer[idx] = 0;

            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no <= 300)
        {
            /* Broadcast */
            bc_page.cmd_1.taddr = 31;
            bc_page.broadcast = YES;
            bc_page.cmd_1.t_r = RECEIVE;
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            bc_page.loop = 0x0000;
            for (idx = 0; idx < 32; idx++)
                data_buffer[idx] = ++data_value;

            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no <= 400)
        {
            /* RT-to-RT */
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            bc_page.loop = 0x0000;
            bc_page.cmd_1.t_r = RECEIVE;
            bc_page.cmd_2.t_r = TRANSMIT;
            bc_page.rt_rt = YES;
            bc_page.cmd_2.taddr = (rt_address == 30) ? 1 : 30;
            bc_page.cmd_2.status = 0xCCCC;
            bc_page.cmd_2.subaddr = 1;
            bc_page.cmd_2.wcnt = 32;
            //bc_page.cmd_2.loop = 0x0000;
            for (idx = 0; idx < 32; idx++)
                data_buffer[idx] = 0;

            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no <= 500)
        {
            bc_page.broadcast = YES;
            bc_page.cmd_1.taddr = 31;
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            bc_page.loop = 0x0000;
            bc_page.cmd_1.t_r = RECEIVE;
            bc_page.cmd_2.t_r = TRANSMIT;
            bc_page.rt_rt = YES;
            bc_page.cmd_2.taddr = (rt_address == 30) ? 1 : rt_address;
            bc_page.cmd_2.status = 0xCCCC;
            bc_page.cmd_2.subaddr = 1;
            bc_page.cmd_2.wcnt = 32;
            //bc_page.cmd_2.loop = 0x0000;
            for (idx = 0; idx < 32; idx++)
                data_buffer[idx] = 0;

            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no >= 601 && msg_no <= 632)
        {
            int datacount = 0;
            int mode_code = msg_no - 601; /* 0..31 */

            /* Initialize common mode code settings */
            bc_page.self_test = NO;
            bc_page.mask_bcst_bit = NO;
            bc_page.broadcast = NO;
            bc_page.rt_rt = NO;
            bc_page.mode_code = YES;
            bc_page.cmd_1.taddr = 1; /* Default to RT address 1 */
            bc_page.cmd_1.subaddr = 0x00; /* Mode codes always subaddr 0 */
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.loop = 0x0000;

            switch (mode_code)
            {
                case 0: /* Dynamic Bus Control Acceptance (DBCA) W/O Data */
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 0;
                    datacount = 0;
                    break;

                case 1: /* Sync W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 1;
                    datacount = 0;
                    break;

                case 2: /* Transmit Status W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 2;
                    datacount = 0;
                    break;

                case 3: /* Initiate Self Test W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 3;
                    datacount = 0;
                    break;

                case 4: /* Transmitter Shut Down W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 4;
                    datacount = 0;
                    break;

                case 5: /* Override Transmitter Shut Down W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 5;
                    datacount = 0;
                    break;

                case 6: /* Inhibit Terminal Flag W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 6;
                    datacount = 0;
                    break;

                case 7: /* Override Inhibit Terminal Flag W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 7;
                    datacount = 0;
                    break;

                case 8: /* Reset Remote Terminal W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 8;
                    datacount = 0;
                    break;

                case 16: /* Transmit Vector Word With Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 16;
                    datacount = 1;
                    data_buffer[0] = 0x0000;
                    break;

                case 17: /* Sync With Data (Broadcast Receive) */
                    bc_page.broadcast = YES;
                    bc_page.cmd_1.taddr = 31; /* Broadcast address */
                    bc_page.cmd_1.t_r = RECEIVE;
                    bc_page.cmd_1.wcnt = 17;
                    datacount = 1;
                    data_buffer[0] = ++data_value;
                    break;

                case 18: /* Transmit Last Command With Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.subaddr = 0;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 18;
                    datacount = 1;
                    data_buffer[0] = 0x0000;
                    break;

                case 19: /* Transmit Built-In-Test With Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.subaddr = 0;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 19;
                    datacount = 1;
                    data_buffer[0] = 0x0000;
                    break;

                case 20: /* Selected Transmitter Shutdown With Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = RECEIVE;
                    bc_page.cmd_1.wcnt = 20;
                    datacount = 1;
                    data_buffer[0] = ++data_value;
                    break;

                case 21: /* Override-Selected Tx Shutdown With Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = RECEIVE;
                    bc_page.cmd_1.wcnt = 21;
                    datacount = 1;
                    data_buffer[0] = ++data_value;
                    break;

                default: /* Undefined mode codes 9-15, 22-31 */
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 0;
                    datacount = 0;
                    data_buffer[0] = 0;
                    break;
            }

            /* Define the Mode Code message */
            DefMsg(module_id, msg_no, bc_page, data_buffer, datacount, inter_msg_gap);
        }
        else
        {
            bc_page.cmd_1.t_r = RECEIVE;        /* 500–1000 → BC TO RT */
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            bc_page.loop = 0x0000;
            for (idx = 0; idx < 32; idx++)
                data_buffer[idx] = ++data_value;

            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
    }
    
    if (use_frame_scheduler == 1)
    {
        for (minor_cycle_index = 0; minor_cycle_index < minor_cycle_count; minor_cycle_index++)
        {
            int start = minor_cycle_index * 100;
            int end = start + 100;

            for (idx = start; idx < end; idx++)
            {
                major_frames[minor_cycle_index].frame_items[idx - start] = idx + 1;
            }

            major_frames[minor_cycle_index].Mess_Count = 100;
            major_frames[minor_cycle_index].MinorFrame_Time = minor_cycle_time;
        }
        //PriorityHighLow(module_id, 10, 2);
        /*PriorityHighLow(module_id, 4, 1);
        PriorityHighLow(module_id, 3, 1);
        PriorityHighLow(module_id, 2, 1);*/
       //PriorityHighLow(module_id, 1, 1);
        BCRetry1553(module_id,Retry);
        DefMajorFrame(module_id, 0, minor_cycle_count, major_frames);
      //  printf("2SameBus(S) Reg(0x1) %x Reg(0x4) %x Reg(0x3) %x\n",RegRead(module_id,CONFIG_REG,1),RegRead(module_id,CONFIG_REG4,1),RegRead(module_id,CONFIG_REG3,1));
        //PriorityHighLow(module_id, 2, 1);
        printf("BC Major Frame Started Wait........! \n");
        RunBcMajorFrame1553(module_id, 0, minor_cycle_count, major_frames, run_times);
       // printf("3SameBus(S) Reg(0x1) %x Reg(0x4) %x Reg(0x3) %x\n",RegRead(module_id,CONFIG_REG,1),RegRead(module_id,CONFIG_REG4,1),RegRead(module_id,CONFIG_REG3,1));
        getchar();
        getchar();
        StartReset(module_id,RESET);
        printf("BC Stopped Enter To Restart \n");
         printf("ResetSameBus(S) Reg(0x1) %x Reg(0x4) %x Reg(0x3) %x\n",RegRead(module_id,CONFIG_REG,1),RegRead(module_id,CONFIG_REG4,1),RegRead(module_id,CONFIG_REG3,1));
        getchar();   
        printf("BC Started\n");
        StartReset(module_id,START);	
        printf("Wait.>>>>!\n");
        printf("StartSameBus(S) Reg(0x1) %x Reg(0x4) %x Reg(0x3) %x\n",RegRead(module_id,CONFIG_REG,1),RegRead(module_id,CONFIG_REG4,1),RegRead(module_id,CONFIG_REG3,1));
        getchar();   
        
         /*printf("Enter For High Priority Messages \n");
         getchar();
         getchar();
         printf("High Priority Mode Initiated\n");
         PriorityHighLow(module_id, 2, HIGH);
          unsigned short dynamic_data[32];
        for (idx = 0; idx < 32; idx++)
            dynamic_data[idx] = 0xaaaa + idx; 
        UpdateMsgData(module_id, 2, dynamic_data, 32, RECEIVE);    
        
        for (idx = 0; idx < 32; idx++)
            dynamic_data[idx] = 0xbbbb+ idx; 
        UpdateMsgData(module_id, 3, dynamic_data, 32, RECEIVE);       
         
        for (idx = 0; idx < 32; idx++)
            dynamic_data[idx] = 0xdddd+ idx; 
        UpdateMsgData(module_id, 4, dynamic_data, 32, RECEIVE);
        PriorityHighLow(module_id, 4, LOW);*/
        /*RunBcMajorFrame(module_id, 0, minor_cycle_count, major_frames, run_times);
        RunBcMajorFrame(module_id, 0, minor_cycle_count, major_frames, run_times);*/
        getchar();
       /*  printf("BC Prior %x \n",RegRead(module_id,0x1d,1));
        unsigned short dynamic_data[32];
        for (idx = 0; idx < 32; idx++)
            dynamic_data[idx] = 0x1553 + idx; 
        UpdateMsgData(module_id, 96, dynamic_data, 32, RECEIVE);

        for (idx = 0; idx < 32; idx++)
            dynamic_data[idx] = 0xABBA + idx;
        UpdateMsgData(module_id, 97, dynamic_data, 32, RECEIVE);

        for (idx = 0; idx < 32; idx++)
            dynamic_data[idx] = 0xBABA + idx;
        UpdateMsgData(module_id, 98, dynamic_data, 32, RECEIVE);

        for (idx = 0; idx < 32; idx++)
            dynamic_data[idx] = 0xCACA + idx;
        UpdateMsgData(module_id, 99, dynamic_data, 32, RECEIVE);*/
    }
    else
    {
        for (idx = 0; idx < (int)msg_count; idx++)
            msg_sequence[idx] = idx + 1;
        BCRetry1553(module_id,Retry);
        DefFrame(module_id, msg_count, msg_sequence);   
        //PriorityHighLow(module_id, 10, 2);
 printf("1SameBus(S) Reg(0x1) %x Reg(0x4) %x Reg(0x3) %x\n",RegRead(module_id,CONFIG_REG,1),RegRead(module_id,CONFIG_REG4,1),RegRead(module_id,CONFIG_REG3,1));
        RunBc1553(module_id, 0, 1, msg_count, run_times);
         printf("3SameBus(S) Reg(0x1) %x Reg(0x4) %x Reg(0x3) %x\n",RegRead(module_id,CONFIG_REG,1),RegRead(module_id,CONFIG_REG4,1),RegRead(module_id,CONFIG_REG3,1));
        getchar();
        getchar();
        StartReset(module_id,RESET);
        printf("BC Stopped Enter To Restart \n");
         printf("ResetSameBus(S) Reg(0x1) %x Reg(0x4) %x Reg(0x3) %x\n",RegRead(module_id,CONFIG_REG,1),RegRead(module_id,CONFIG_REG4,1),RegRead(module_id,CONFIG_REG3,1));
        getchar();   
        printf("BC Started\n");
        StartReset(module_id,START);	
        printf("Wait.>>>>!\n");
        printf("StartSameBus(S) Reg(0x1) %x Reg(0x4) %x Reg(0x3) %x\n",RegRead(module_id,CONFIG_REG,1),RegRead(module_id,CONFIG_REG4,1),RegRead(module_id,CONFIG_REG3,1));
        getchar();   
        
        unsigned short cmd_update_data[32];

        for (idx = 0; idx < 32; idx++)
            cmd_update_data[idx] = 0xa5a6 + idx;
        UpdateMsgCmdData(module_id,
                         95,
                         0x0820,
                         0x0000,
                         cmd_update_data,
                         32,
                         RECEIVE,
                         channel);

        for (idx = 0; idx < 32; idx++)
            cmd_update_data[idx] = 0xa5a7 + idx;
        UpdateMsgCmdData(module_id,
                         96,
                         0xf811,
                         0x0000,
                         cmd_update_data,
                         17,
                         3, /* ModeCode */
                         channel);

        for (idx = 0; idx < 32; idx++)
            cmd_update_data[idx] = 0xa5a8 + idx;
        UpdateMsgCmdData(module_id,
                         97,
                         0x1420,
                         0x0000,
                         cmd_update_data,
                         32,
                         TRANSMIT,
                         channel);

        for (idx = 0; idx < 32; idx++)
            cmd_update_data[idx] = 0xa5a9 + idx;
        UpdateMsgCmdData(module_id,
                         98,
                         0xfba2,
                         0x0000,
                         cmd_update_data,
                         32,
                         RECEIVE,
                         channel);

        for (idx = 0; idx < 32; idx++)
            cmd_update_data[idx] = 0xa5a9 + idx;
        UpdateMsgCmdData(module_id,
                         99,
                         0x0820,
                         0x1420,
                         cmd_update_data,
                         32,
                         2, /* RT-RT */
                         channel);
    }
        getchar();
    RegWrite(module_id, 0x07, 0x0000, 1);
    ResetAce(module_id);
    printf("Device Reset.............!\n");
    getchar();

    return 0;
}

