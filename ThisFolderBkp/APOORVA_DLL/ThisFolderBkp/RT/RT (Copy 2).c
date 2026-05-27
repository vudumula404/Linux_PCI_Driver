#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"


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

int main() {
    int ret, modId, i, msgIdx,RtAdres=0,Sa=0;
    unsigned short wordCount;
    unsigned short txBuffer[BUFFER_SIZE];
    RT_MSG rtMsg;

    // Initialize DDC card
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

    SetRtAddrM(modId, RtAdres, 1);  // Set RT address = 2
    DefLookUp(modId, LOOKUP_1);   
    // Enable RT mode
    ret = DefMode(modId, RT);
     // Fill transmit buffer
    for (Sa = 1; Sa <31; Sa++)
    {
      for (i = 0; i < BUFFER_SIZE; ++i)
      {
          txBuffer[i] = Sa+i;      
      }
       printf("SA-%d %x\n",Sa,MemRead(modId,0x160+Sa,1));    
      WriteDataBlock(modId, Sa, txBuffer);
    }

    RunRt(modId);
    printf("RT is now running. Press Enter to continue...\n");    
    getchar();
    getchar();
    int msgNo = 1;
   for(msgNo=1;msgNo<=64;msgNo++)
   {
      ReadMsg1553(modId, msgNo, &rtMsg);
      printf("\n Message #%d\n", msgNo);
      printf("BSW      : 0x%X\n", rtMsg.bsw);
      printf("CMD      : 0x%X\n", rtMsg.cmd);
      printf("Word Cnt : 0x%X\n", rtMsg.wcnt);
      printf("SubAddr  : 0x%X\n", rtMsg.subaddr);
      printf("T/R      : 0x%X\n", rtMsg.t_r);

      wordCount = rtMsg.wcnt ? rtMsg.wcnt : BUFFER_SIZE;
      printf("Data     :\n\t");

      for (i = 0; i < wordCount; ++i) {
          printf("%04X ", rtMsg.data[i]);
          if ((i + 1) % 8 == 0) printf("\n\t");
      }
   }
      exit(1);
    //int msgNo = 1;
    while (!stop) {
        ret = ReadMsg1553(modId, msgNo, &rtMsg);
        if (ret == 0 && rtMsg.bsw != 0) {  // BSW != 0 indicates a valid message
            printf("\n Message #%d\n", msgNo);
            printf("BSW      : 0x%X\n", rtMsg.bsw);
            printf("CMD      : 0x%X\n", rtMsg.cmd);
            printf("Word Cnt : 0x%X\n", rtMsg.wcnt);
            printf("SubAddr  : 0x%X\n", rtMsg.subaddr);
            printf("T/R      : 0x%X\n", rtMsg.t_r);

            wordCount = rtMsg.wcnt ? rtMsg.wcnt : BUFFER_SIZE;
            printf("Data     :\n\t");

            for (i = 0; i < wordCount; ++i) {
                printf("%04X ", rtMsg.data[i]);
                if ((i + 1) % 8 == 0) printf("\n\t");
            }
            fflush(stdout);
        }

        msgNo++;
        if (msgNo > 1024) msgNo = 1;  // wrap around or limit

        //usleep(50000);  // poll every 50ms
        usleep(20000);  // poll every 1ms
    }
    printf("\n RT stopped.\n");
    return 0;
}

