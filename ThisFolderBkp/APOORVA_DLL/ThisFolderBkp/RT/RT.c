#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"
/**********************************************************************/
    /*Error Bits Test:
    SetTerFlag(modId);    ResetTerFlag(modId);    SetDba(modId);
    SetSvcReq(modId);     SetBusy(modId);         SetSsflag(modId);*/
    /*Mode Code Test: 
    Enable SetDba(); DBCA(0th Mode) => 
    Enable SetTerFlag();(6th Mode)Inhibit Terminal Flag (Which suppress the TermFlag) 
    (7th Mode)Override Inhibit Terminal Flag/(8th Mode)Reset Rt (to original position)
    Enable SetSvcReq(modId);  Send Transmit Vector Word(16th mode)*/
    /*BC Prepare: 
    Initiate SelfTest(3rd Mode), Wait for 100 MilliSec then send Tx BIT Word (19th Mode) 
    Sync Without Data(1st Mode) (Rt Should reset the Internal Timer)
    Transmit Status(2nd Mode) (Rt Should transmit last command status)
    Transmitter S/D (4th mode) (Rt Should stop sending Status/data on current Channel)
    Over ride Transmitter S/D (5th mode) (Rt Should Enable sending Status/data on current Channel)
    Sync With Data(17th Mode)(Rt Should receive the mode code data)
    Transmit Last Command(Rt Should Transmit Last received Command)
/*********************************************************************/

#define BUFFER_SIZE 32

volatile sig_atomic_t stop = 0;

void handle_sigint(int sig) {
    stop = 1;
    printf("\n Stopping RT polling loop...\n");
}
short ReadMsg1553(UCHAR ModId,unsigned short stack_entry,RT_MSG *message)
{
	CONFIG_BITS config;
	unsigned short msg,look_up,i,offset,temp;
	unsigned short error=0;
	if ((stack_entry <=1024 || stack_entry == LAST_MSG) && stack_entry >=1)
	{
		if(stack_entry == LAST_MSG)
		{
			msg = MemRead(ModId,STACKA_PTR,1);
			message->bsw = MemRead(ModId,STACK_1 + (msg - 4),1 );
			message->cmd = MemRead(ModId,STACK_1 + (msg-1),1);
		}
		else if(stack_entry != LAST_MSG)
		{
			temp = MemRead(ModId,STACK_1 + ((stack_entry * 4) - 4),1);
			message->bsw = MemRead(ModId,STACK_1 + ((stack_entry * 4)-4),1);
			message->cmd = MemRead(ModId,STACK_1 + ((stack_entry * 4)-1),1);
		}
		message->wcnt    = (message->cmd) & 0x1f;
		message->subaddr = (message->cmd>>5) & 0x1f;
		message->t_r     =  (message->cmd>>10) & 0x01;
		config = ReadConfigBits(ModId);
		if (config.current_area == 0)
			look_up= LOOKUP_1;
		else
			look_up = LOOKUP_2;
		if (message->t_r == TRANSMIT)
			look_up = look_up+32;
		offset = MemRead(ModId,look_up + message->subaddr,1);
		temp = message->wcnt;
		if(temp == 0)
			temp = 32;
		for (i=0;i<temp;i++)
			message->data[i] = MemRead(ModId,offset+i,1);
	}
	else
		error = 36;
	return(error);
}
unsigned int ClearRam1553(UCHAR ModId)
{
    unsigned int i;
    unsigned short val;
    int errorCount = 0;

    // Clear the RAM
    for (i = 0; i < MEMORY_SIZE; i++)
    {
        MemWrite(ModId, i, 0x0, 1);
    }

    // Verify RAM contents
    for (i = 0; i < MEMORY_SIZE; i++)
    {
        val = MemRead(ModId, i, 1);
        if (val != 0)
        {
            printf("RAM Clear Failed: Addr=0x%04X Value=0x%04X\n", i, val);
            errorCount++;
        }
    }

    if (errorCount == 0)
    {
        printf("RAM cleared successfully.\n");
        return 1;
    }
    else
    {
        printf("RAM clear completed with %d errors.\n", errorCount);
        return 0;
    }
}

void SetTerFlag(UCHAR ModId)
{
    unsigned int config;
    config = RegRead(ModId,CONFIG_REG,1);
    config = config & 0xFF2F;
    RegWrite(ModId,CONFIG_REG,config,1);
}
void ResetTerFlag(UCHAR ModId)
  {
	unsigned int config;
	config = RegRead(ModId,CONFIG_REG,1);
	config = config | 0xFFCF;
	RegWrite(ModId,CONFIG_REG,config,1);
  }
int main() {
    int ret, modId, i, msgIdx,RtAdres=0,Sa=0;
    unsigned short wordCount;
    unsigned short txBuffer[BUFFER_SIZE];
    RT_MSG rtMsg;
    FILE* logFile = NULL;
    unsigned long Lcount = 1;
    unsigned short currentDesc = 0, lastDesc = 0, pos = 1;
    unsigned int config;
    // Initialize DDC card
    logFile = fopen("Rt_log.txt", "w");	
        //setvbuf(logFile, NULL, _IOFBF, 65536); 
        if (!logFile) {
            perror("Failed to open log file");
            return -1;
        }
    ret = InitCard(1);
    if (ret < 0) {
        printf("\nERROR: Failed to acquire device handle. Exiting.\n");
        return 1;
    }
    printf("CCPMC device opened successfully (Handle: %d)\n", ret);

    // Select RT module
    printf("Enter RT Module ID (1/2/3): ");
    scanf("%d", &modId);
    printf("Enter Any RT Address (between 1 to 30): ");
    scanf("%d", &RtAdres);

    // Basic RT setup
    ClearRam1553(modId);
    ResetAce(modId);
    ResetDba(modId);
    ResetSvcReq(modId);
    ResetBusy(modId);
    ResetSsflag(modId);
    SetRtAddrM(modId, RtAdres, 1);  // Set RT address = 2
    DefLookUp(modId, LOOKUP_1);   
    // Enable RT mode
    ret = DefMode(modId, RT);

   //SetTerFlag(modId);
   //ResetTerFlag(modId);
    //SetDba(modId);
    //SetSvcReq(modId);
    RegWrite(modId,CONFIG_REG3,0x8000,1);
    RegWrite(modId,CONFIG_REG3,0x8001,1);
   //SetBusy(modId);
   // SetSsflag(modId);
    // Fill transmit buffer
    for (Sa = 1; Sa <31; Sa++)
    {
      for (i = 0; i < BUFFER_SIZE; ++i)
      {
          txBuffer[i] = Sa+i;      
      }
      // printf("SA-%d %x\n",Sa,MemRead(modId,0x160+Sa,1));    
      WriteDataBlock(modId, Sa, txBuffer);
    }
    MemWrite(modId,0x120,0xa5a5,1);
    RunRt(modId);
    printf("RT is now running. Press Cntr+c to skip...\n");    
    while (!stop)
    {
          currentDesc = MemRead(modId, 0x100,1);//(((MemRead(modId, 0x0,1))-0x1000)/4);//MtMsgCnt(MTModId);
         //usleep(1000);
          //if (lastDesc != currentDesc) 
          {
            while (lastDesc != currentDesc ) 
            {
                ReadMsg(modId, pos, &rtMsg);
                /*fprintf(logFile,"\n Message #%d\n", Lcount);
                fprintf(logFile,"BSW      : 0x%X\n", rtMsg.bsw);
                fprintf(logFile,"CMD      : 0x%X\n", rtMsg.cmd);
                fprintf(logFile,"Word Cnt : 0x%X\n", rtMsg.wcnt);
                fprintf(logFile,"SubAddr  : 0x%d\n", rtMsg.subaddr);
                fprintf(logFile,"T/R      : 0x%X\n", rtMsg.t_r);*/
                printf("\n Message #%d\n", Lcount);
                printf("BSW      : 0x%X\n", rtMsg.bsw);
                printf("CMD      : 0x%X\n", rtMsg.cmd);
                printf("Word Cnt : 0x%X\n", rtMsg.wcnt);
                printf("SubAddr  : 0x%d\n", rtMsg.subaddr);
                printf("T/R      : 0x%X\n", rtMsg.t_r);
                printf("BIT_STAT : 0x%X\n", RegRead(modId,0x0F,1));
                printf("RT_SWR   : 0x%X\n", RegRead(modId,0x0E,1));
                
                wordCount = rtMsg.wcnt ? rtMsg.wcnt : BUFFER_SIZE;
                 if(rtMsg.subaddr==0)
                  wordCount=1;
                //fprintf(logFile,"Data     :\n\t");
                 printf("Data     :\n\t");

                for (i = 0; i < wordCount; ++i) {
                    //fprintf(logFile,"%04X ", rtMsg.data[i]);
                    printf("%04X ", rtMsg.data[i]);
                    if ((i + 1) % 8 == 0) //fprintf(logFile,"\n\t");
                    printf("\n\t");
                }
               
                 //fprintf(logFile,"MsgNo %d lastDesc %x currentDesc %x %x\n", Lcount,lastDesc,currentDesc,/*((*/MemRead(modId, 0x100,1)/*-0x1000)/4)*/);
                 //printf("MsgNo %d lastDesc %x currentDesc %x %x\n", Lcount,lastDesc,currentDesc,/*((*/MemRead(modId, 0x100,1)/*-0x1000)/4)*/);
                // printf("MsgNo %d\n", Lcount);
                fflush(stdout);      
                pos = (pos >= 64) ? 1 : pos + 1;   
                 Lcount++;      
                //lastDesc++;
                lastDesc = currentDesc;//((MemRead(MTModId, 0x102, 1)-0x1000)/4);// currentDesc;               
            }
          }          
      }   
      fclose(logFile);
    printf("\n RT stopped.\n");
    return 0;
}

