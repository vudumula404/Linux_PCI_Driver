#include <stdio.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"
//#include "/home/acl/APOORVA_CCPMC/BKP_Aug_26_2025/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"

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
//unsigned int  CmdStack=0x1000;
//unsigned int  DataStack=0x140;
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

		buff_offset = (DataStack + (msg_num -1)*40) + 4;
		for (i=0; i<num_of_data_words;i++)
		MemWrite(ModId,buff_offset+i,data_buffer[i],1);

		buff_offset = DataStack +((msg_num-1)*40)+2;
		MemWrite(ModId,buff_offset,def.loop,1);

		buff_offset = DataStack + ((msg_num-1)*40)+3;
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


	if (error == 0)
	 {
	               if(times == 0)
			{
			  /*RegWrite(Bus,0x58,0x0,CardId);
			  RegWrite(Bus,0x59,0x0,CardId);
			  RegWrite(Bus,CR1, ((RegRead(Bus,CR1,CardId)&0xDFFF) | 0x0100),CardId);//13th bit -->0 for STACKA with Auto Repeat ON*/
			  RegWrite(ModId,0x01,0x054f,1);  
			  RegWrite(ModId,0x07,0x8000,1);
	                  StartReset_DDC(ModId,START_DDC);	
	                  //StartReset_DDC(ModId,START_DDC);	
	                  printf("Continuous Scheduling  %x MsgCount %x\n",START_DDC,MemWrite(ModId,MESS_COUNTER_DDC,1));
			}
			else
		//for (i=0;i<times;i++)
		{
			MemWrite(ModId,CmdStack,init_pos,1);
			temp = 0xffff - (num_of_messages*times);
			MemWrite(ModId,MESS_COUNTER_DDC,temp,1);//MESS_COUNTER_DDC 0x101
			/*printf("MESS_COUNTER_DDC %x temp %x num_of_messages  %d\n",MESS_COUNTER_DDC,temp,num_of_messages);
			getchar();
			getchar();*/
			MemWrite(ModId,TMP_CNTR_DDC,temp,1);     
			MemWrite(ModId,TMP_PTR_DDC,init_pos,1);   

			StartReset_DDC(ModId,START_DDC);

			count=MemRead(ModId,MESS_COUNTER_DDC,1);
			     
         	         while (count != 0xFFFF)
         	         {
		          count=MemRead(ModId,MESS_COUNTER_DDC,1);
		         // printf("MESS_COUNTER_DDC %d Readcount %x  temp %x num_of_messages  %d\n",MESS_COUNTER_DDC,count,temp,num_of_messages);
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
void BCRetry1553_DDC(UCHAR ModId,USHORT sel)
{
	USHORT temp=0,val;
	printf("BCRetry_DDC %d \n",sel);
	if(sel)
	{
		RegWrite(ModId,CONFIG_REG3_DDC,0x8000,1);
		temp=RegRead(ModId,CONFIG_REG_DDC,1);
		RegWrite(ModId,CONFIG_REG_DDC,(temp&0xffe7)|(0x0013),1);
		//RegWrite(ModId,CONFIG_REG_DDC,(temp&0xffe7)|(0x0010),1);
		printf("BCRetry_DDC sel %d CONFIG_REG1_DDC %x\n",sel,RegRead(ModId,CONFIG_REG_DDC,1));
		switch(sel)
		{

		case 1:
			val=RegRead(ModId,CONFIG_REG4_DDC,1);
			RegWrite(ModId,CONFIG_REG4_DDC,val | 0x0,1);
			break;

		case 2:
		        RegWrite(ModId,CONFIG_REG3_DDC,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4_DDC,1);
			//RegWrite(ModId,CONFIG_REG4_DDC,val | 0x1180 ,1);
			RegWrite(ModId,CONFIG_REG4_DDC,val | 0x1180 ,1);
			printf("CONFIG_REG3_DDC %x CONFIG_REG4_DDC %x\n",RegRead(ModId,CONFIG_REG3_DDC,1),RegRead(ModId,CONFIG_REG4_DDC,1));
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
/*******************************************************************
*  This Program gives code for BC and MT 
********************************************************************/
int main()
{
        MNTPACK_DDC buffer;
	static		int 	CCSC_FD		=	0x0;
	int			RtAddre=0,i,MemAdr=0,Ch=0x0,TempCnt,RetVal=0x0,Times=0x1,POS1=0;
	unsigned short		MemBuff,MsgNo=0,Data=0;
	unsigned char		CardId,FailCount=0,ModId=0,MTModId=3,Com_Or_MemTest=0;
	char temp;
       unsigned short MinorCyle=0,MsgCnt=0,MinorCyleCnt=0,MinorCyleTime=0;
       unsigned short mess_sequence[1024];
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
	  
	  printf("\n\t\tEnter Number Of Messages To Run (Times*Messages):");
	  scanf("%d",&Times);
	 
	  //printf("(0x9)Data %x\n",RegRead(ModId,0x9,1));
	   RetVal	=	ClearRam_DDC(ModId);
	  printf("BC Retval of ClearRam is :%d ModId %d\n",RetVal,ModId);
	  ResetAce_DDC(ModId); 
	  //printf("BC Retval of ResetAce is :%d\n",RetVal);
	   RegWrite(ModId,0x7,0x8000,1);
	 // RegWrite(ModId,0x9,0x0000,1);//BC Response Time 0x600=>130 us//0x400=>50 us,//0x200=>22.5 us
	  RetVal	=	DefMode_DDC(ModId,BC_DDC);
	  MemWrite(ModId,0X100,0X1000,1);
	  MemWrite(ModId,0X1003,0X4000,1);	
	
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
	      bc_page.cmd_1.t_r     = 1;//RECEIVE_DDC;//RECEIVE_DDC;	
	      bc_page.cmd_1.taddr   = ++RtAddre;
	     
	     /* if((MsgNo>0)&&(MsgNo<16))
	        bc_page.cmd_1.t_r     = RECEIVE_DDC;	  
	      else if((MsgNo>15)&&(MsgNo<31))
	        bc_page.cmd_1.t_r     = TRANSMIT_DDC;
	      else if((MsgNo>30)&&(MsgNo<46))
	      {
	        bc_page.cmd_1.taddr    = 31;
	        bc_page.broadcast     = YES_DDC;
	        bc_page.cmd_1.t_r     = RECEIVE_DDC;
	        printf("BROADCAST %d\n",bc_page.broadcast);
	      }	      
	      else if((MsgNo>45)&&(MsgNo<61))
	      {
	         
	        bc_page.self_test     = NO_DDC;
	        bc_page.mask_bcst_bit = NO_DDC;
	        bc_page.broadcast     = NO_DDC;
	        bc_page.mode_code     = NO_DDC;	       
	        //bc_page.retry_enable=YES;
	        bc_page.cmd_1.t_r     = RECEIVE_DDC;	
	        bc_page.cmd_2.t_r     = TRANSMIT_DDC;
	        bc_page.rt_rt         = YES_DDC;
	        if(RtAddre==30)
	          bc_page.cmd_2.taddr =   1;
	        else
	          bc_page.cmd_2.taddr =   30;
	        bc_page.cmd_2.status  = 0xcccc;
	        bc_page.loop          = 0x0000;
	        bc_page.cmd_2.subaddr = 1;
	        bc_page.cmd_2.wcnt    = 32;
	      }
	      else if((MsgNo>60)&&(MsgNo<101))
	      {
	        bc_page.cmd_1.t_r     = RECEIVE_DDC;	         
	        bc_page.self_test     = NO_DDC;
	        bc_page.mask_bcst_bit = NO_DDC;
	        bc_page.broadcast     = NO_DDC;
	        bc_page.mode_code     = NO_DDC;
	        bc_page.rt_rt         = NO_DDC;
	      }*/
	      bc_page.cmd_1.status  = 0xcccc;
	      bc_page.loop          = 0x0000;
	      bc_page.cmd_1.subaddr = 1;
	      bc_page.cmd_1.wcnt    = 32;

	      for(i=0; i< 32; i++)
		      data_buffer[i] = /*Data*/MsgNo;
	          
	      DefMsg1553_DDC(ModId,MsgNo,bc_page,data_buffer,32);
	  } 
	  
	  for(POS1=0;POS1<1024;POS1++)
	   mess_sequence[POS1]= POS1+1;
	  DefFrame1553_DDC(ModId,1024,mess_sequence);
	  RunBc1553_DDC(ModId,0,1,1024,Times);
	  
	  // BCRetry1553_DDC(ModId,11);	
	  /*for (POS1 = 0; POS1 < 0xa000; POS1++) 
	  {        
            printf("%04X %04X %04d ", 0x4000 + POS1, MemRead(ModId, 0x4000 + POS1, 1),POS1);
            if ((POS1 + 1) % 6 == 0)  // print 8 pairs per line
                printf("\n");
         }*/
	/*StartReset_DDC(ModId,START_DDC);
	 for(POS1=0;POS1<MinorCyleCnt;POS1++)
        {        
	     printf("Data %x Addres %x\n",RegRead(ModId,0x24+POS1,1),0x24+POS1);
        }*/
	//BCAutoRepeat(BcMod,YES,CARD1);
	printf("BC Major Frame Started Completed Enter........! \n");
	getchar();
	//RunBcMajorFrame1553(ModId,0,MinorCyleCnt,Min_Cyc,Times,1);
	getchar();
        printf("BC Frame Completed........! \n");
     
        ResetAce_DDC(ModId);	
        printf("Device Reset.............!\n");
        getchar();
        getchar();
	  
	return 0;
}  
