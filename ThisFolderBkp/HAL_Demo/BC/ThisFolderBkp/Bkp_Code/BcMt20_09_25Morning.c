#include <stdio.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"
#define BUFFER_SIZE 32
MSG_DEF	bc_page;
RT_MSG	RtBuff;
MSG_RESULT	msg_return;
unsigned short	frame_items[4096],data_buffer[40],temp_data;
unsigned short	Temp_Buff[40];
MAJOR_FRAME Min_Cyc[128];
void UpdateMsgData(UCHAR ModId, unsigned short msg_num, unsigned short *new_data, int num_words, int direction)
{
    unsigned int DataStack = 0x4000;
    unsigned int msg_base  = DataStack + (msg_num - 1) * 40;
    unsigned int buff_offset;
    int i;

    if (direction == RECEIVE)   // BC → RT
        buff_offset = msg_base + 2;
    else if (direction == TRANSMIT) // RT → BC
        buff_offset = msg_base + 4;
    else
        return;

    for (i = 0; i < num_words; i++)
    {
        MemWrite(ModId, buff_offset + i, new_data[i], 1);
    }
}
int UpdateMsgCmdData(UCHAR ModId,
                     unsigned short msg_num,
                     unsigned short cmd1,
                     unsigned short cmd2,
                     unsigned short *data_buffer,
                     unsigned short num_of_data_words,
                     int direction,       // RECEIVE / TRANSMIT / RT_RT / MODE_CODE
                     int bus)             // 0 = Bus A, 1 = Bus B
{
    unsigned int DataStack = 0x4000;
    unsigned int base      = DataStack + (msg_num - 1) * 40;
    unsigned int offset, cntrl;
    int i;

    // --- update cmd1 ---
    if (cmd1)
        MemWrite(ModId, base + 1, cmd1, 1);

    // --- update cmd2 (RT–RT only) ---
    if (cmd2 && direction == 2)   // assume 2 = RT_RT
        MemWrite(ModId, base + 2, cmd2, 1);

    // --- build control word ---
    cntrl = 0;
    cntrl |= (0 << 8);                 // retry_enable placeholder
    cntrl |= (bus & 0x1) << 7;         // bus: 0 = A, 1 = B
    cntrl |= (0 << 6);                 // self_test
    cntrl |= (0 << 5);                 // mask_bcst_bit
    cntrl |= ((direction == 3) ? 1 : 0) << 2; // mode_code flag
    cntrl |= ((direction == 2) ? 1 : 0);      // rt_rt flag
    cntrl |= (0 << 1);                 // broadcast

    MemWrite(ModId, base + 0, cntrl, 1);

    // --- data update ---
    switch (direction)
    {
        case RECEIVE:   // BC → RT
            offset = base + 2;
            for (i = 0; i < num_of_data_words; i++)
                MemWrite(ModId, offset + i, data_buffer[i], 1);
            break;

        case TRANSMIT:  // RT → BC
            offset = base + 4;
            for (i = 0; i < num_of_data_words; i++)
                MemWrite(ModId, offset + i, data_buffer[i], 1);
            break;

        case 2:   // RT–RT
            offset = base + 5;
            for (i = 0; i < num_of_data_words; i++)
                MemWrite(ModId, offset + i, data_buffer[i], 1);
            break;

        case 3:   // Mode Code
        {
            if (cmd1) MemWrite(ModId, base + 1, cmd1, 1);

            unsigned short wcnt = cmd1 & 0x1F;       // 5 bits word count/mode code
            unsigned short tr   = (cmd1 >> 10) & 0x1;

            if (tr == TRANSMIT && (wcnt == 16 || wcnt == 18 || wcnt == 19)) {
                MemWrite(ModId, base + 2, 0x0001, 1);
                MemWrite(ModId, base + 3, 0x0000, 1);
                if (num_of_data_words > 0)
                    MemWrite(ModId, base + 4, data_buffer[0], 1);
            }
            else if (tr == RECEIVE && (wcnt == 17 || wcnt == 20 || wcnt == 21)) {
                if (num_of_data_words > 0)
                    MemWrite(ModId, base + 2, data_buffer[0], 1);
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
            return -1; // invalid
    }

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
        unsigned short MinorCyle=0,MsgCnt=0,MinorCyleCnt=0,MinorCyleTime=0,InterMsgGap=10;
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
	 	  
	  RetVal	=	ClearRam(ModId);
	  ResetAce(ModId); 
	  RetVal	=	DefMode(ModId,BC);     
		  
	  if(Ch	==	1)
		bc_page.bus       = BUS_A;		
	  else if(Ch	==	0)
		bc_page.bus       = BUS_B;
	    for (MsgNo = 1; MsgNo <= MsgCnt; MsgNo++)
            {
                // Reset defaults for every message
                if (RtAddre >= 30)
                    RtAddre = 0;

                bc_page.self_test     = NO;
                bc_page.mask_bcst_bit = NO;
                bc_page.broadcast     = NO;
                bc_page.mode_code     = NO;
                bc_page.rt_rt         = NO;
                //bc_page.retry_enable=YES;

                bc_page.cmd_1.taddr   = ++RtAddre;    
                bc_page.cmd_1.t_r     = RECEIVE;   // default

                // Message group selection
                if (MsgNo <= 100)                           
                {
                    bc_page.cmd_1.t_r = RECEIVE;        // 0–100 → BC TO RT                   
                    bc_page.cmd_1.status  = 0xCCCC;
                    bc_page.cmd_1.subaddr = 1;
                    bc_page.cmd_1.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                    for (i = 0; i < 32; i++)
                      data_buffer[i] = ++Data;
                   // Define message
                    DefMsg(ModId, MsgNo, bc_page, data_buffer, 32,InterMsgGap);
                }
                else if (MsgNo <= 200)                      // 100–200 → RT TO BC
                {
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.status  = 0xCCCC;
                    bc_page.cmd_1.subaddr = 1;
                    bc_page.cmd_1.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                     for (i = 0; i < 32; i++)
                      data_buffer[i] = 0;
                   // Define message
                    DefMsg(ModId, MsgNo, bc_page, data_buffer, 32,InterMsgGap);
                }
                else if (MsgNo <= 300)                      // 200–300 → Broadcast
                {
                    bc_page.cmd_1.taddr = 31;
                    bc_page.broadcast   = YES;
                    bc_page.cmd_1.t_r   = RECEIVE;
                    bc_page.cmd_1.status  = 0xCCCC;
                    bc_page.cmd_1.subaddr = 1;
                    bc_page.cmd_1.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                    //printf("BROADCAST %d\n", bc_page.broadcast);
                     for (i = 0; i < 32; i++)
                      data_buffer[i] = ++Data;
                   // Define message
                    DefMsg(ModId, MsgNo, bc_page, data_buffer, 32,InterMsgGap);
                }
                else if (MsgNo <= 400)                      // 300–400 → RT-to-RT
                {
                    bc_page.cmd_1.status  = 0xCCCC;
                    bc_page.cmd_1.subaddr = 1;
                    bc_page.cmd_1.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                    bc_page.cmd_1.t_r   = RECEIVE;
                    bc_page.cmd_2.t_r   = TRANSMIT;
                    bc_page.rt_rt       = YES;
                    bc_page.cmd_2.taddr = (RtAddre == 30) ? 1 : 30;
                    bc_page.cmd_2.status  = 0xCCCC;
                    bc_page.cmd_2.subaddr = 1;
                    bc_page.cmd_2.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                     for (i = 0; i < 32; i++)
                      data_buffer[i] = 0;
                   // Define message
                    DefMsg(ModId, MsgNo, bc_page, data_buffer, 32,InterMsgGap);
                }
                else if (MsgNo <= 500)                      // 300–400 → RT-to-RT
                {
                    bc_page.broadcast     = YES;
                    bc_page.cmd_1.taddr   = 31;    
                    bc_page.cmd_1.status  = 0xCCCC;
                    bc_page.cmd_1.subaddr = 1;
                    bc_page.cmd_1.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                    bc_page.cmd_1.t_r   = RECEIVE;
                    bc_page.cmd_2.t_r   = TRANSMIT;
                    bc_page.rt_rt       = YES;
                    bc_page.cmd_2.taddr = (RtAddre == 30) ? 1 : RtAddre;
                    bc_page.cmd_2.status  = 0xCCCC;
                    bc_page.cmd_2.subaddr = 1;
                    bc_page.cmd_2.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                     for (i = 0; i < 32; i++)
                      data_buffer[i] = 0;
                   // Define message
                    DefMsg(ModId, MsgNo, bc_page, data_buffer, 32,InterMsgGap);
                }
                else if (MsgNo >= 601 && MsgNo <= 632) // Reserve MsgNo range for Mode Codes
                {
                    int datacount = 0;
                    int mode_code = MsgNo - 601; // 0..31
                                       
                    // Initialize common mode code settings
                    bc_page.self_test = NO;
                    bc_page.mask_bcst_bit = NO;
                    bc_page.broadcast = NO;
                    bc_page.rt_rt = NO;
                    bc_page.mode_code = YES;
                    bc_page.cmd_1.taddr = 1; // Default to RT address 1
                    bc_page.cmd_1.subaddr = 0x00; // Mode codes always subaddr 0
                    bc_page.cmd_1.status = 0xCCCC;
                    bc_page.loop = 0x0000;
                    
                    switch (mode_code)
                    {
                        case 0: // Dynamic Bus Control Acceptance (DBCA) W/O Data BroadCast No {Tx}
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 0;
                            datacount = 0;
                            break;
                            
                        case 1: // Sync W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 1;
                            datacount = 0;
                            break;                            
                        case 2: // Transmit Status W/O Data BroadCast No {Tx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 2;
                            datacount = 0;
                            break;                            
                        case 3: // Initiate Self Test W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 3;
                            datacount = 0;
                            break;
                            
                        case 4: // Transmitter Shut Down W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 4;
                            datacount = 0;
                            break;
                            
                        case 5: // Override Transmitter Shut Down W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 5;
                            datacount = 0;
                            break;
                            
                        case 6: // Inhibit Terminal Flag W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 6;
                            datacount = 0;
                            break;
                            
                        case 7: // Override Inhibit Terminal Flag W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 7;
                            datacount = 0;
                            break;
                            
                        case 8: // Reset Remote Terminal W/O Data BroadCast YES {Tx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 8;
                            datacount = 0;
                            break;
                            
                        case 16: // Transmit Vector Word With Data BroadCast YES {Tx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 16;
                            datacount = 1;
                            data_buffer[0] = 0x0000;
                            break;
                            
                        case 17: // Sync With Data With Data BroadCast YES {Rx}
                            bc_page.broadcast = YES;
                            bc_page.cmd_1.taddr = 31; // Broadcast address
                            bc_page.cmd_1.t_r = RECEIVE;
                            bc_page.cmd_1.wcnt = 17;
                            datacount = 1;
                            data_buffer[0] = ++Data;
                            break;
                            
                        case 18: // Transmit Last Command With Data BroadCast YES {Tx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.subaddr = 0; // Mode codes always subaddr 0
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 18;
                            datacount = 1;
                            data_buffer[0] = 0x0000;
                            break;
                            
                        case 19: // Transmit Built-In-Test With Data BroadCast YES {Tx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.subaddr = 0; // Mode codes always subaddr 0
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 19;
                            datacount = 1;
                            data_buffer[0] = 0x0000;
                            break;
                            
                        case 20: // Selected Transmitter S/D With Data BroadCast YES {Rx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = RECEIVE;
                            bc_page.cmd_1.wcnt = 20;
                            datacount = 1;
                            data_buffer[0] = ++Data;
                            break;
                            
                        case 21: // Override-Selected Tx S/D With Data BroadCast YES {Rx}
                            bc_page.broadcast = NO;
                            bc_page.cmd_1.taddr = 1; // Broadcast address
                            bc_page.cmd_1.t_r = RECEIVE;
                            bc_page.cmd_1.wcnt = 21;
                            datacount = 1;
                            data_buffer[0] = ++Data;
                            break;
                            
                        default: // Handle undefined mode codes 9-15, 22-31
                            bc_page.cmd_1.t_r = TRANSMIT;
                            bc_page.cmd_1.wcnt = 0;
                            datacount = 0;
                            data_buffer[0] = 0;
                            break;
                    }
                    
                    // Define the Mode Code message
                    DefMsg(ModId, MsgNo, bc_page, data_buffer, datacount,InterMsgGap);
                }
                else
                {
                    bc_page.cmd_1.t_r = RECEIVE;        // 500–1000 → BC TO RT                   
                    bc_page.cmd_1.status  = 0xCCCC;
                    bc_page.cmd_1.subaddr = 1;
                    bc_page.cmd_1.wcnt    = 32;
                    bc_page.loop          = 0x0000;
                    for (i = 0; i < 32; i++)
                      data_buffer[i] = ++Data;
                   // Define message
                    DefMsg(ModId, MsgNo, bc_page, data_buffer, 32,InterMsgGap);
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
	    DefMajorFrame(ModId,0,MinorCyleCnt,Min_Cyc);
	    printf("BC Major Frame Started Wait........! \n");	
	    RunBcMajorFrame(ModId,0,MinorCyleCnt,Min_Cyc,Times);
	     unsigned short new_data[32];
            for (i = 0; i < 32; i++)
                new_data[i] = 0x1553 + i;   // some dynamic pattern
            UpdateMsgData(ModId, 96, new_data, 32, RECEIVE);
            
            for (i = 0; i < 32; i++)
                new_data[i] = 0xABBA+ i;   // some dynamic pattern
            UpdateMsgData(ModId, 97, new_data, 32, RECEIVE);
            for (i = 0; i < 32; i++)
                new_data[i] = 0xBABA + i;   // some dynamic pattern
            UpdateMsgData(ModId, 98, new_data, 32, RECEIVE);
            
            for (i = 0; i < 32; i++)
                new_data[i] = 0xCACA+ i;   // some dynamic pattern
            UpdateMsgData(ModId, 99, new_data, 32, RECEIVE);

	    //printf("BC Major Frame Completed........! \n");	    
	}
	else
	{
	  for(POS1=0;POS1<(MsgCnt);POS1++)
	      mess_sequence[POS1]= POS1+1;
	  DefFrame(ModId,MsgCnt,mess_sequence);
	  RunBc(ModId,0,1,MsgCnt,Times);
	  // getchar();
	  unsigned short new_data[32];	  
        // Msg #95
        for (int i = 0; i < 32; i++)
            new_data[i] = 0xa5a6 + i;
        UpdateMsgCmdData(ModId,
                         95,
                         0x0820,
                         0x0000,
                         new_data,
                         32,
                         RECEIVE,
                         Ch);

        // Msg #96
        for (int i = 0; i < 32; i++)
            new_data[i] = 0xa5a7 + i;
        UpdateMsgCmdData(ModId,
                         96,
                         0xf811,
                         0x0000,
                         new_data,
                         17,
                         3,//ModeCode
                         Ch);

        // Msg #97
        for (int i = 0; i < 32; i++)
            new_data[i] = 0xa5a8 + i;
        UpdateMsgCmdData(ModId,
                         97,
                         0x1420,
                         0x0000,
                         new_data,
                         32,
                         TRANSMIT,
                         Ch);

        // Msg #98
        for (int i = 0; i < 32; i++)
            new_data[i] = 0xa5a9 + i;
        UpdateMsgCmdData(ModId,
                         98,
                         0xfba2,
                         0x0000,
                         new_data,
                         32,
                         RECEIVE,
                         Ch);
        for (int i = 0; i < 32; i++)
            new_data[i] = 0xa5a9 + i;
        UpdateMsgCmdData(ModId,
                         99,
                         0x820,
                         0x1420,
                         new_data,
                         32,
                         2,//RT-RT
                         Ch);

       // getchar();
        }
        
        //printf("Address(0x1) %x\n",RegRead(ModId,0x1,1));
       // getchar();
	/*for(i=0;i<Times;i++)
	{
	  for(POS1=1;POS1<(MsgCnt+1);POS1++)
	  {	   
                ReadMsgResult(ModId, STACK_1, POS1, &msg_return);
                printf("\nBC Msg #%d\nBSW: 0x%X CMD: 0x%X SubAddr: %d WCnt: %d T/R: %d\nData:\n", POS1, msg_return.bsw,msg_return.cmd_1,         msg_return.cmd1_bits.subaddr, msg_return.cmd1_bits.wcnt, msg_return.cmd1_bits.t_r);
                for (int j = 0; j < (msg_return.cmd1_bits.wcnt ? msg_return.cmd1_bits.wcnt : BUFFER_SIZE); ++j)
                {
                    printf("%04X ", msg_return.data[j]);
                    if ((j + 1) % 8 == 0) printf("\n");
                }
                printf("\nLoopWord: %04X Status: %04X\n", msg_return.loop_word, msg_return.status_1);
      	  }
    	}*/
	 // StartReset(ModId,START);
	 
        //getchar();        
        RegWrite(ModId,0x07,0x0000,1); 
        ResetAce(ModId);	
        printf("Device Reset.............!\n");
        getchar();        	  
	return 0;
}  


