// Simplified and cleaned up version of the original 1553B BC/RT/MT communication test application

#include <stdio.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"

#define BUFFER_SIZE 32
#define MSG_COUNT 1023

MSG_DEF bc_page;
RT_MSG rtMsg;
MSG_RESULT msg_return;
MNTPACK mtBuffer;
unsigned short txBuffer[BUFFER_SIZE], data_buffer[BUFFER_SIZE], frame_items[MSG_COUNT];

void ConfigureRT(UCHAR rtModId, unsigned char rtAddr)
{
    ClearRam(rtModId);
    ResetAce(rtModId);
    SetRtAddrM(rtModId, rtAddr, 1);
    DefLookUp(rtModId, LOOKUP_1);
    for (int i = 0; i < BUFFER_SIZE; ++i) txBuffer[i] = 0x1553 + i;
    WriteDataBlock(rtModId, 1, txBuffer);
    DefMode(rtModId, RT);
    RunRt(rtModId);
}

void ConfigureMT(UCHAR mtModId)
{
    ClearRam(mtModId);
    ResetAce(mtModId);
    DefMode(mtModId, MT);
    SelMsgMt(mtModId);
    RunMt(mtModId);
}
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
void ConfigureBC(UCHAR bcModId,UCHAR rtAddr, unsigned short data)
{
    ClearRam(bcModId);
    ResetAce(bcModId);
    DefMode(bcModId, BC);   

    for (int i = 0; i < MSG_COUNT; ++i) {
        bc_page.bus       = BUS_A;
        bc_page.self_test = NO;
        bc_page.broadcast = NO;
        bc_page.mode_code = NO;
        bc_page.rt_rt     = NO;
        //bc_page.retry_enable=YES;
        bc_page.cmd_1.taddr = /*i +*/ rtAddr;
        bc_page.cmd_1.t_r = RECEIVE;///*(i < 2) ? */RECEIVE /*: TRANSMIT*/;
        bc_page.cmd_1.status = 0xcccc;
        bc_page.loop = 0x0000;
        bc_page.cmd_1.subaddr = rtAddr;
        bc_page.cmd_1.wcnt = BUFFER_SIZE;
         for (int i = 0; i < BUFFER_SIZE; ++i) data_buffer[i] = data++;
        DefMsg(bcModId, i + 1, bc_page, data_buffer, BUFFER_SIZE,10);
        frame_items[i] = i + 1;
    }
    //BCRetry1553(bcModId,1);
    DefFrame(bcModId, MSG_COUNT, frame_items);
}

void PrintRTMessages(UCHAR rtModId)
{
    for (int i = 1; i <= MSG_COUNT; ++i) {
        ReadMsg(rtModId, i, &rtMsg);
        printf("\nRT Msg #%d\nCMD: 0x%X WCnt: %d SubAddr: %d TR: %d\nData:\n", i, rtMsg.cmd, rtMsg.wcnt, rtMsg.subaddr, rtMsg.t_r);
        for (int j = 0; j < (rtMsg.wcnt ? rtMsg.wcnt : BUFFER_SIZE); ++j) {
            printf("%04X ", rtMsg.data[j]);
            if ((j + 1) % 8 == 0) printf("\n");
        }
        printf("\n");
    }
}

void PrintBCResults(UCHAR bcModId)
{
    for (int i = 1; i <= MSG_COUNT; ++i) {
        ReadMsgResult(bcModId, STACK_1, i, &msg_return);
        printf("\nBC Msg #%d\nBSW: 0x%X CMD: 0x%X SubAddr: %d WCnt: %d T/R: %d\nData:\n", i, msg_return.bsw,msg_return.cmd_1, msg_return.cmd1_bits.subaddr, msg_return.cmd1_bits.wcnt, msg_return.cmd1_bits.t_r);
        for (int j = 0; j < (msg_return.cmd1_bits.wcnt ? msg_return.cmd1_bits.wcnt : BUFFER_SIZE); ++j) {
            printf("%04X ", msg_return.data[j]);
            if ((j + 1) % 8 == 0) printf("\n");
        }
        printf("\nLoopWord: %04X Status: %04X\n", msg_return.loop_word, msg_return.status_1);
    }
}
void PollMtMessages(UCHAR MTModId)
{
    MNTPACK mtPacket;
    unsigned short descriptor;
    char temp;

    //printf("\nPolling MT messages (Press Ctrl+C to exit):\n");
    //while (1)
    {
        for (descriptor = 1; descriptor <= MSG_COUNT; descriptor++)
        {
            ReadMonPacket(MTModId, descriptor, &mtPacket);
            //if (mtPacket.BlockSts.bitwise.Som && mtPacket.BlockSts.bitwise.Eom)
            {
                printf("\n[MT] Packet at descriptor %d\n", descriptor);
                printf("Cmd Word    : 0x%04X\n", mtPacket.RecCmdWrd.value);
                printf("Time Tag    : 0x%04X\n", mtPacket.TimeTag);
                int words = mtPacket.RecCmdWrd.bitwise.WrdCntOrMde;
                if (words == 0) words = 32;
                printf("Data:\n\t");
                for (int i = 0; i < words; i++)
                {
                    printf("%04X ", mtPacket.DataPack[i]);
                    if ((i + 1) % 8 == 0) printf("\n\t");
                }
                printf("\n");
            }
        }
       // usleep(10000); // 10 ms delay
    }
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
int main()
{
    UCHAR bcModId, rtModId, mtModId, rtAddr;
    unsigned short data;

    if (!InitCard(1)) {
        printf("Device initialization failed.\n");
        return -1;
    }
    /*InitInterrupts( 0,1);
    RegWrite(1,INTMASK_REG,0x1,1);*/
    printf("Enter BC ModuleId (1-3): "); scanf("%hhu", &bcModId);
    /*printf("Enter RT ModuleId (1-3): "); scanf("%hhu", &rtModId);
    printf("Enter MT ModuleId (1-3): "); scanf("%hhu", &mtModId);*/
    printf("Enter RT Address (1-30): "); scanf("%hhu", &rtAddr);
    printf("Enter 16-bit data to send (hex): "); scanf("%hx", &data);

   /* ConfigureRT(rtModId, rtAddr);
    ConfigureMT(mtModId);*/
    ConfigureBC(bcModId,rtAddr, data);

    printf("Press Enter to Run BC Frame..."); getchar(); getchar();
    RunBc1553(bcModId, STACK_1, 1, MSG_COUNT, 1);
    getchar();
   // printf("\nBC Results: %d",GetM1Cnt());
    
    PrintBCResults(bcModId);
   /* printf("\nMT Results:");
    PollMtMessages(mtModId);
    printf("\nRT Messages:");
    PrintRTMessages(rtModId);*/

    return 0;
}


