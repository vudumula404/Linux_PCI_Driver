//#include "stdafx.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <immintrin.h> 
#include "/home/acl/APOORVA_CCPMC//APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
/**********************************************************************************************/
    //This Program Contains BC scheduler Rt and MT Thread
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
/************************************************************************************************/
#ifndef TRUE
#define TRUE 1
#endif
#define BUS_A 1
#define BUS_B 0
#ifndef FALSE
#define FALSE 0
#endif
#define RECEIVE 0
#define TRANSMIT 1
#define YES 1
#define NO 0
bool Card_Type = true;
#define BUFFER_SIZE 32
#define CARD1 1
unsigned char channel = 0; /* 0 = CH-B, 1 = CH-A */
typedef unsigned char  BYTE;
typedef unsigned long  DWORD;
pthread_t thread;
pthread_t mtThread;
typedef struct {
    int hour;
    int min;
    int sec;
} TIME;
typedef  unsigned short   UBIT16;
typedef  short            SBIT16;

typedef void* HANDLE;

typedef long long LARGE_INTEGER;

typedef int errno_t;
MSG_DEF bc_page,bc_sync,bc_page_msg[32];
MSG_RESULT	msg_return; 
RT_MSG RtBuff;
MNTPACK buffer;
MAJOR_FRAME major_frames[128];
unsigned short frame_items[128],data_buffer[40],temp_data,BCDataCounter=0;
unsigned short i;
ULONG m,temp,testNum=0;
void ConfigureRT(UCHAR rtModId, unsigned char rtAddr);
void PrepareBCmessage(int BUS,int channel,int interGap,int Taddrs ,int SubAddr,int t_r,int Wcnt,USHORT data ,int MessageNum,int CARD);
int ThreadRunFlag=0;
void *BCThread_Bus(void *lpParameter);
void BCSchedule_Bus(int Times, unsigned char module_id,unsigned char channel);
void* StartMTThread(void* arg);
void* RTThread_Bus(void* arg);
typedef long long LONGLONG;
pthread_mutex_t mutexObj = PTHREAD_MUTEX_INITIALIZER;
unsigned short txBuffer[BUFFER_SIZE];

typedef struct
{
	 UBIT16      minutes     : 8 ;
	 UBIT16      hour        : 8 ;
}HOUR_MIN;

typedef struct
{
	 UBIT16      count_ms20	: 8 ;
	 UBIT16      seconds	: 8 ;
}SEC_20MS;
typedef struct
{
	 HOUR_MIN    hour_min ;
	 SEC_20MS    sec_20ms ;
}TIME_BROADCAST ;

TIME_BROADCAST MC1_12_broadcast_mesg;
TIME_BROADCAST b1_broadcast_test,b2_broadcast_test,b3_broadcast_test;


short MC_minor_cycle_number=-1;
HANDLE hMutexObj;

struct timeval t_struct;

void ActivateMc(unsigned char ModId);

#define MSG_COUNT 1024
FILE* logFile = NULL;
FILE* RtlogFile=NULL;
int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    int ret;

    tcgetattr(STDIN_FILENO, &oldt);             // Save old terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);           // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);    // Apply new terminal settings
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // Non-blocking read

    ch = getchar();

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        ret = 1;
    }
    else
    {
        ret = 0;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);    // Restore old terminal settings
    fcntl(STDIN_FILENO, F_SETFL, oldf);         // Restore file status
    return ret;
}
static struct termios orig_termios;  // To save original terminal settings

void enableNonCanonicalMode(void)
{
    struct termios new_termios;

    // Get current terminal attributes
    tcgetattr(STDIN_FILENO, &orig_termios);

    // Copy original and modify for noncanonical mode
    new_termios = orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);  // disable canonical mode and echo
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;

    // Apply new settings immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void disableNonCanonicalMode(void)
{
    // Restore the original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void handle_sigint(int sig) {
    printf("\nExiting...\n");
    if (logFile) fclose(logFile);
     if (RtlogFile) fclose(RtlogFile);
    exit(0);
}
void ConfigureMt(unsigned char MTModId)
{
  ClearRam(MTModId);
  ResetAce(MTModId);  
  // Initialize descriptor pointer from hardware (0xB register)      
  RegWrite(MTModId, 0x3, 0x1, 1);                               
  RegWrite(MTModId, 0x3, 0x2, 1);
  RegWrite(MTModId, CONFIG_REG, 0x5000, 1);
  SelMsgMt(MTModId);
  RunMt(MTModId);
}

int main(void)
{
    int NoOfTimes = 0, rtAddr = 0, RtStatusBits = 0;
    int BcMod = 1, rtModId = 2, mtModId = 3;
    char input[32];
    fd_set set;
    struct timeval timeout;

    logFile = fopen("mt_log.txt", "w");
    if (!logFile)
    {
        perror("Failed to open log file");
        return -1;
    }
    RtlogFile = fopen("Rt_log.txt", "w");
    if (!RtlogFile)
    {
        perror("Failed to open log file");
        return NULL;
    }

    if (!InitCard(CARD1))
    {
        printf("\n\tINVALID CARD ID OR CARD MAY NOT BE PRESENT\n\n");
        fclose(logFile);
        fclose(RtlogFile);
        return -1;
    }
     ResetAce(1);
     ResetAce(2);
     ResetAce(3);
    while (1)
    {
        printf("\n\n*********** ACL-APOORVA(CCPMC-1553B) BC(As per HAL Scheduler)/RT/MT CARD ATP ***********\n");
        printf("\n\tSelect Mode:\n");
        
        printf("\t 1. BC MODE ONLY\n");
        printf("\t 2. MT MODE ONLY\n");
        printf("\t 3. RT MODE ONLY\n");
        printf("\t 4. BC & MT Mode\n");
        printf("\t 5. BC & RT Mode\n");
        printf("\t 6. RT & MT Mode\n");
        printf("\t 7. RT Status Bits\n");
        printf("\t 0. Exit\n\n");

        printf("\tEnter your choice: ");
        scanf("%d", &testNum);      
       
        if (testNum == 0)
        {
            printf("\nExiting...\n");
            break;
        }

        switch (testNum)
        {
            case 1: // BC Mode
                printf("\n\tEnter Channel (CH-A:1, CH-B:0) :");
                scanf("%d", &channel);               
                printf("\n\tEnter No. of Times to Run BC Scheduler (0 = Continuous): ");
                scanf("%d", &NoOfTimes);
                printf("\n\tSelected: BC Mode\n");
                enableNonCanonicalMode();               
                ActivateMc(BcMod);
                BCSchedule_Bus(NoOfTimes, BcMod,channel);
                break;
            case 2: // MT Mode
                printf("\n\tSelected: MT Mode\n");
                enableNonCanonicalMode();                
                ConfigureMt(mtModId);
                pthread_create(&mtThread, NULL, StartMTThread, &mtModId);                
                break;
           case 3: // RT Mode
                printf("\n\tSelect RT Address (1–30): ");
                scanf("%d", &rtAddr);
                printf("\n\tSelected: RT Mode\n");
                enableNonCanonicalMode();
                ConfigureRT(rtModId, rtAddr);                                
                pthread_create(&thread, NULL, RTThread_Bus, &rtModId);
                break;
            case 4: // BC + MT Mode
                printf("\n\tEnter Channel (CH-A:1, CH-B:0) :");
                scanf("%d", &channel);
                printf("\n\tEnter No. of Times to Run BC Scheduler (0 = Continuous): ");
                scanf("%d", &NoOfTimes);
                printf("\n\tSelected: BC & MT Mode\n");
                enableNonCanonicalMode();
                ConfigureMt(mtModId);
                ActivateMc(BcMod);           
                pthread_create(&mtThread, NULL, StartMTThread, &mtModId);
                BCSchedule_Bus(NoOfTimes, BcMod,channel);
                break;
            case 5: // BC + RT Mode
                printf("\n\tSelect RT Address (1–30): ");
                scanf("%d", &rtAddr);
                printf("\n\tEnter Channel (CH-A:1, CH-B:0) :");
                scanf("%d", &channel);
                printf("\n\tEnter No. of Times to Run BC Scheduler (0 = Continuous): ");
                scanf("%d", &NoOfTimes);
                printf("\n\tSelected: BC & RT Mode\n");
                enableNonCanonicalMode();                
                ActivateMc(BcMod);
                ConfigureRT(rtModId, rtAddr);               
                pthread_create(&thread, NULL, RTThread_Bus, &rtModId);
                BCSchedule_Bus(NoOfTimes, BcMod,channel);
                break;          
            case 6: // RT + MT Mode
                printf("\n\tSelect RT Address (1–30): ");
                scanf("%d", &rtAddr);
                printf("\n\tSelected: RT & MT Mode\n");
                enableNonCanonicalMode();
                ConfigureRT(rtModId, rtAddr);
                ConfigureMt(mtModId);
                pthread_create(&mtThread, NULL, StartMTThread, &mtModId);
                pthread_create(&thread, NULL, RTThread_Bus, &rtModId);
                break;            
            case 7: // RT Status Bits Control
                printf("\n\tSelect RT Address (1–30): ");
                scanf("%d", &rtAddr);

                printf("\n\tSelected: RT-Address %d\n", rtAddr);
                printf("\n\tRT Status Bit Options:\n");
                printf("\t 1. Service Request\n");
                printf("\t 2. Busy\n");
                printf("\t 3. Subsystem Flag\n");
                printf("\t 4. DBA\n");
                printf("\t 5. Terminal Flag\n");
                printf("\tEnter Option (1–5): ");
                scanf("%d", &RtStatusBits);

                enableNonCanonicalMode();
                ClearRam(rtModId);
                SetRtAddrM(rtModId, rtAddr, 1);
                DefLookUp(rtModId, LOOKUP_1);
                DefMode(rtModId, RT);
                RegWrite(rtModId, CONFIG_REG3, 0x8000, 1);
                RunRt(rtModId);

                switch (RtStatusBits)
                {
                    case 1:
                       /* RegWrite(modId,CONFIG_REG3,0x8000,1);
                        RegWrite(modId,CONFIG_REG3,0x8001,1);*/
                        SetSvcReq(rtModId);
                       // MemWrite(rtModId,0x120,0xa5a5,1);
                        printf("\n\tService Request set. Press ENTER to reset.\n");
                        getchar(); getchar();
                        ResetSvcReq(rtModId);
                        printf("\tService Request Flag Reset.\n");
                        break;

                    case 2:
                        SetBusy(rtModId);
                        printf("\n\tBusy flag set. Press ENTER to reset.\n");
                        getchar(); getchar();
                        ResetBusy(rtModId);
                        printf("\tBusy Flag Reset.\n");
                        break;

                    case 3:
                        SetSsflag(rtModId);
                        printf("\n\tSubsystem Flag set. Press ENTER to reset.\n");
                        getchar(); getchar();
                        ResetSsflag(rtModId);
                        printf("\tSubsystem Flag Reset.\n");
                        break;

                    case 4:
                        SetDba(rtModId);
                        printf("\n\tDBA Flag set. Press ENTER to reset.\n");
                        getchar(); getchar();
                        ResetDba(rtModId);
                        printf("\tDBA Flag Reset.\n");
                        break;

                    case 5:
                        SetTerFlag(rtModId);
                        printf("\n\tTerminal Flag set. Press ENTER to reset.\n");
                        getchar(); getchar();
                        ResetTerFlag(rtModId);
                        SetRtAddrM(rtModId, rtAddr, 1);
                        DefLookUp(rtModId, LOOKUP_1);
                        DefMode(rtModId, RT);
                        RegWrite(rtModId, CONFIG_REG3, 0x8000, 1);
                        RunRt(rtModId);
                        printf("\tTerminal Flag Reset.\n");
                        break;

                    default:
                        printf("\n\tInvalid option! Please enter 1–5.\n");
                        break;
                }
                break;  

            default:
                printf("\n\tInvalid test number! Please enter 1–4.\n");
                continue;
        }

        // Wait for keypress to exit mode
        ThreadRunFlag = 0;
        while (ThreadRunFlag == 0)
        {
            FD_ZERO(&set);
            FD_SET(STDIN_FILENO, &set);

            timeout.tv_sec = 0;
            timeout.tv_usec = 100000; // 100ms

            if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0)
            {
                char ch;
                read(STDIN_FILENO, &ch, 1);                
                printf("\nKey pressed: Exiting mode...\n");
                ResetAce(BcMod);
                ResetAce(rtModId);
                ResetAce(mtModId);
                ThreadRunFlag = 1;
            }
        }

        // Clean up threads
        pthread_join(thread, NULL);
        if (testNum != 1) pthread_join(mtThread, NULL);
        disableNonCanonicalMode();
    }    
    printf("\n***************************************** OVER ****************************************\n");
    pthread_mutex_destroy(&mutexObj);
    fclose(logFile);
    fclose(RtlogFile);
    printf("\nProgram terminated normally.\n");

    return 0;
}


void ConfigureRT(UCHAR rtModId, unsigned char rtAddr)
{
  int Sa=0;
 // Basic RT setup
    ClearRam(rtModId);
    ResetAce(rtModId);    
    SetRtAddrM(rtModId, rtAddr, 1);
    DefLookUp(rtModId, LOOKUP_1);   
    // Enable RT mode
    DefMode(rtModId, RT);   
    RegWrite(rtModId,CONFIG_REG3,0x8000,1);
    
    for (Sa = 1; Sa <31; Sa++)
    {
      for (i = 0; i < BUFFER_SIZE; ++i)
      {
          txBuffer[i] = Sa+i;      
      }
      // printf("SA-%d %x\n",Sa,MemRead(modId,0x160+Sa,1));    
      WriteDataBlock(rtModId, Sa, txBuffer);
    }
    //MemWrite(rtModId,0x120,0xa5a5,1);//Enable ServiceReq(TxVectorWord)
    RunRt(rtModId); 
}
void ActivateMc(unsigned char ModId)
{ 
    int RetVal=0;
    RetVal = ClearRam(ModId);	 
    ResetAce(ModId); 
    RetVal = DefMode(ModId,BC);
    //printf("ModuleId of DefMode is :%d\n",ModId);	 
}
void BCSchedule_Bus(int Times, unsigned char module_id, unsigned char channel)
{
    struct timeval start, stop;
    long long llTimeDiff = 0,counter=0;
    int globalIncre=0,PreC=0;
    unsigned short MsCoun=0;
    unsigned short inter_msg_gap=20,msg_no=0,idx=0,data_value=0,Wcount=0;
    double dftDuration = 0.0;
    int prevPos = 0;  
    UBIT16 data[20][32], sub_addr[10], wordcnt[20];
    printf("\tBus Controller Scheduling Preparation Starts...!\n");
 
    /*************Total 350 Messages*********************/
	
  for(MC_minor_cycle_number=0;MC_minor_cycle_number<8;MC_minor_cycle_number++)
  {    
    switch(MC_minor_cycle_number)
          {
            case 0 :	       
        /* ================= FRAME 0 ================= */
              for (msg_no = 1; msg_no <= 41; msg_no++)
              {
                  ++MsCoun;         
                  memset(&bc_page, 0, sizeof(bc_page));
                  if (channel == 1)
                    bc_page.bus = BUS_A;
                  else
                     bc_page.bus = BUS_B;
                  bc_page.bus = channel;
                  bc_page.self_test = NO;
                  bc_page.mask_bcst_bit = NO;
                  bc_page.broadcast = NO;
                  bc_page.mode_code = NO;
                  bc_page.rt_rt = NO;
                  bc_page.loop = 0x0000;
                  bc_page.cmd_1.status = 0xCCCC;
                  bc_page.cmd_1.subaddr = 1;
                  bc_page.cmd_1.wcnt = 1;
                  bc_page.cmd_1.t_r = RECEIVE;

                  switch (msg_no)
                  {
                      /* ---- Mode Codes ---- */
                      case 1:  // 31 (A)    
                          bc_page.broadcast = YES;
                          bc_page.mode_code = YES;
                          bc_page.cmd_1.taddr = 31;
                          bc_page.cmd_1.subaddr = 0;
                          bc_page.cmd_1.t_r = RECEIVE;
                          bc_page.cmd_1.wcnt = 17;
                          //bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 16; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE;
                          break;
                      /* ---- Broadcast Messages ---- */
                      case 2:  // 31 (A), Subaddr 29
                          bc_page.broadcast = YES;
                          bc_page.cmd_1.taddr = 31;
                          bc_page.cmd_1.subaddr = 29;
                          bc_page.cmd_1.t_r = RECEIVE;
                          bc_page.cmd_1.wcnt = 2;
                          
                          time_t now = time(NULL);
                          struct tm t = *localtime(&now);

                          struct timeval tv;
                          gettimeofday(&tv, NULL);
                          unsigned long ms = (tv.tv_usec / 1000) % 1000;

                          MC1_12_broadcast_mesg.hour_min.hour     = (UBIT16)t.tm_hour;
                          MC1_12_broadcast_mesg.hour_min.minutes  = (UBIT16)t.tm_min;
                          MC1_12_broadcast_mesg.sec_20ms.seconds  = (UBIT16)t.tm_sec;
                          MC1_12_broadcast_mesg.sec_20ms.count_ms20 = (UBIT16)(ms / 20); // 0 to 49

                          memcpy(data[0], &MC1_12_broadcast_mesg, sizeof(TIME_BROADCAST));                           
                          break;   
                      /* ---- Regular RX/TX ---- */                        
                      case 3:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 16; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 4:  bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 5:  bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 6:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 24; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 7:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 8: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 26; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 9: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 26; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 10: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 11: bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 12: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 5;  bc_page.cmd_1.t_r = RECEIVE; break;
                      case 13: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 12; bc_page.cmd_1.wcnt = 25; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 14: bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 4;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 15: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 16: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 24; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 17: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 18: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 12; bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 19: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 20: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 9;  bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 21: bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 4;  bc_page.cmd_1.t_r = RECEIVE; break;
                      case 22: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 14; bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 23: bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 24; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 24: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 29; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 25: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 10; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 26: bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 8;  bc_page.cmd_1.t_r = RECEIVE; break;
                      case 27: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                      case 28: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 21; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 29: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 15; bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 30: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 17; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 31: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 32: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 33: bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 11; bc_page.cmd_1.wcnt = 4;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 34: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 11; bc_page.cmd_1.wcnt = 4;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 35: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 36: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 26; bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 37: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 18; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 38: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 19; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 39: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                      /* ---- Mode Codes ---- */
                      case 40: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 0;bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; break;
                      case 41: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 0;bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; break;

                      default:
                          continue;
                  }
                  int Wcount = bc_page.cmd_1.wcnt ? bc_page.cmd_1.wcnt : 32;
              
                  if (bc_page.cmd_1.t_r != TRANSMIT)
                  {
                      if (bc_page.cmd_1.subaddr == 0)
                      {
                          data_buffer[0] = MC_minor_cycle_number;//MC_minor_cycle_number;
                          //printf("MC %d\n",data_buffer[0]);
                      }
                      else if((bc_page.cmd_1.taddr==31)&&(bc_page.cmd_1.subaddr==29))
                      {
                         data_buffer[0] = data[0][0];
                         data_buffer[1] = data[0][1];
                        // printf("MC0 %d\n",data_buffer[0]);
                      }
                      else
                      {
                          for (idx = 0; idx < Wcount; idx++)
                              data_buffer[idx] = ++data_value;
                      }
                  }
                   //printf("MC %x\n",data_buffer[0]);
                  DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);                                   
              }
              break;
            case 1 :
             /* ==================== FRAME 1 MESSAGE DEFINITION ==================== */
              for (msg_no = 1; msg_no <= 47; msg_no++)
              {
                  ++MsCoun;
                  memset(&bc_page, 0, sizeof(bc_page));
                  if (channel == 1)
                    bc_page.bus = BUS_A;
                  else
                     bc_page.bus = BUS_B;
                  bc_page.self_test = NO;
                  bc_page.mask_bcst_bit = NO;
                  bc_page.broadcast = NO;
                  bc_page.mode_code = NO;

                  bc_page.rt_rt = NO;
                  bc_page.loop = 0x0000;
                  bc_page.cmd_1.status = 0xCCCC;
                  bc_page.cmd_1.subaddr = 1;
                  bc_page.cmd_1.wcnt = 1;
                  bc_page.cmd_1.t_r = RECEIVE;

                  int datacount = 0;

                  switch (msg_no)
                  {
                      /* ========== MODE CODES ========== */
                      case 1:  // 31 (A) ModeCode TX   
                        bc_page.broadcast = YES;
                        bc_page.mode_code = YES;
                        bc_page.cmd_1.taddr = 31;
                        bc_page.cmd_1.t_r = RECEIVE;
                        bc_page.cmd_1.subaddr = 0;
                        bc_page.cmd_1.wcnt = 17;
                        break;
                      /* ========== RX MESSAGES ========== */
                      case 2:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 16; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 3:  bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 4:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 24; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 5:  bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 6:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 7: bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 5;  bc_page.cmd_1.t_r = RECEIVE; break;
                      case 8: bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 24; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 9: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 8;  bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 10: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 11: bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 12: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;

                      /* ========== TX MESSAGES ========== */
                      case 13: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 14: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = TRANSMIT; break;

                      /* Continue RX sequence */
                      case 15: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 11; bc_page.cmd_1.wcnt = 6;  bc_page.cmd_1.t_r = RECEIVE; break;
                      case 16: bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 2;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 17: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 18: bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 19: bc_page.cmd_1.taddr = 13; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 12; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 20: bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 21: bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 19; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 22: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 15; bc_page.cmd_1.wcnt = 26; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 23: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 8;  bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = RECEIVE; break;

                      /* ========== RT-RT ========== */
                      case 24:
                          bc_page.rt_rt = YES;
                          bc_page.cmd_1.taddr = 15; bc_page.cmd_1.t_r = RECEIVE; bc_page.cmd_1.subaddr = 9;  bc_page.cmd_1.wcnt = 4;
                          bc_page.cmd_2.taddr = 13; bc_page.cmd_2.t_r = TRANSMIT; bc_page.cmd_2.subaddr = 5; bc_page.cmd_2.wcnt = 4;
                          break;

                      /* ========== More RX/TX ========== */
                      case 25: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 26: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 29; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 27: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 10; bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 28: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                      case 29: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 21; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 30: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 17; bc_page.cmd_1.t_r = RECEIVE; break;

                      /* ========== RT-RT #2 ========== */
                      case 31:
                          bc_page.rt_rt = YES;
                          bc_page.cmd_1.taddr = 13; bc_page.cmd_1.t_r = RECEIVE; bc_page.cmd_1.subaddr = 13; bc_page.cmd_1.wcnt = 24;
                          bc_page.cmd_2.taddr = 15; bc_page.cmd_2.t_r = TRANSMIT; bc_page.cmd_2.subaddr = 3;  bc_page.cmd_2.wcnt = 24;
                          break;

                      /* ========== TXs ========== */
                      case 32: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 33: bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 11; bc_page.cmd_1.wcnt = 9;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 34: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 11; bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 35: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 11; bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 36: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 11; bc_page.cmd_1.wcnt = 4;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 37: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 11; bc_page.cmd_1.wcnt = 4;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 38: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 8;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 39: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 8;  bc_page.cmd_1.wcnt = 2;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 40: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 0;  bc_page.cmd_1.wcnt = 2;  bc_page.cmd_1.t_r = TRANSMIT; break;

                      /* ========== RX tail ========== */
                      case 41: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 42: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 26; bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;

                      /* ========== Final TX ========== */
                      case 43: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 18; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 44: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 19; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 45: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                      case 46: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5; bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = TRANSMIT; break;

                      /* ========== MODE CODES END ========== */
                      case 47: // RT6 (A)                        
                          bc_page.mode_code = YES;
                          bc_page.cmd_1.taddr = 6;
                          bc_page.cmd_1.t_r = TRANSMIT;
                          bc_page.cmd_1.subaddr = 0;
                          bc_page.cmd_1.wcnt = 0;
                          datacount = 0;
                          break;

                      default:
                          continue;
                  }

                  int Wcount = bc_page.cmd_1.wcnt ? bc_page.cmd_1.wcnt : 32;
                  if(bc_page.rt_rt!=YES)
                  {
                    if (bc_page.cmd_1.t_r != TRANSMIT)
                    {
                        if (bc_page.cmd_1.subaddr == 0)
                        {
                            data_buffer[0] =  MC_minor_cycle_number;
                            //printf("MC1 %d\n",data_buffer[0]);
                        }                       
                        else
                        {
                            for (idx = 0; idx < Wcount; idx++)
                                data_buffer[idx] = ++data_value;
                        }
                    }
                  }
                  DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);      
                  
              }
       break;
            case 2 :
             /* ================= FRAME 2 ================= */
            for (msg_no = 1; msg_no <= 41; msg_no++)
            {
                ++MsCoun;
               memset(&bc_page, 0, sizeof(bc_page));
               if (channel == 1)
                  bc_page.bus = BUS_A;
                else
                   bc_page.bus = BUS_B;
                bc_page.self_test = NO;
                bc_page.mask_bcst_bit = NO;
                bc_page.broadcast = NO;
                bc_page.mode_code = NO;
                bc_page.rt_rt = NO;
                bc_page.loop = 0x0000;
                bc_page.cmd_1.status = 0xCCCC;
                bc_page.cmd_1.subaddr = 1;
                bc_page.cmd_1.wcnt = 1;
                bc_page.cmd_1.t_r = RECEIVE;

                int datacount = 0;

                switch (msg_no)
                {
                    /* ---- Mode Codes ---- */
                    case 1:  // 31(A)
                        bc_page.broadcast = YES;
                        bc_page.mode_code = YES;
                        bc_page.cmd_1.taddr = 31;
                        bc_page.cmd_1.t_r = RECEIVE;
                        bc_page.cmd_1.subaddr = 0;
                        bc_page.cmd_1.wcnt = 17;
                        break;
                    /* ---- Regular RX ---- */
                    case 2:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 16; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 3:  bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 4:  bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 5:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 24; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 6:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 7: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 8: bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 9: bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 5;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 10: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 11: bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 25; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 12: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 3;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 13: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 14: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 12; bc_page.cmd_1.wcnt = 25; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 15: bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 4;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 16: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 17: bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 9;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 18: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 21; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 19: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 21; bc_page.cmd_1.t_r = TRANSMIT; break;

                    /* ---- RT-to-RT ---- */
                    case 20:
                        bc_page.rt_rt = YES;
                        bc_page.cmd_1.taddr = 15;  // TX RT
                        bc_page.cmd_1.subaddr = 6; // TXSA
                        bc_page.cmd_2.taddr = 2;   // RX RT
                        bc_page.cmd_2.subaddr = 4; // RXSA
                        bc_page.cmd_1.wcnt = 19;
                        bc_page.cmd_2.wcnt = 19;
                        bc_page.cmd_1.t_r = RECEIVE;
                        bc_page.cmd_2.t_r = TRANSMIT;
                        break;

                    /* ---- More RX/TX ---- */
                    case 21: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 12; bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 22: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 23: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 29; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 24: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 25: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 21; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 26: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 14; bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 27: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 17; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 28: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 10; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 29: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 5;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 30: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 31: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 15; bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 32: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 17; bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 33: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 18; bc_page.cmd_1.wcnt = 25; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 34: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 35: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 26; bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 36: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 18; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 37: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 19; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 38: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 39: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = TRANSMIT; break;

                    /* ---- Mode Codes ---- */
                    case 40: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 0;bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; break;
                    case 41: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 0;bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; break;        

                    default:
                        continue;
                }
                int Wcount = bc_page.cmd_1.wcnt ? bc_page.cmd_1.wcnt : 32;
                 if(bc_page.rt_rt!=YES)
                 {
                    if (bc_page.cmd_1.t_r != TRANSMIT)
                    {
                        if (bc_page.cmd_1.subaddr == 0)
                        {
                            data_buffer[0] =  MC_minor_cycle_number;
                            //printf("MC2 %d\n",data_buffer[0]);
                        }
                        else
                        {
                            for (idx = 0; idx < Wcount; idx++)
                                data_buffer[idx] = ++data_value;
                        }
                    }
                  }
                  DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);       
            }
	  
              break;
            case 3 :
             /* ================= FRAME 3 ================= */
              for (msg_no = 1; msg_no <= 42; msg_no++)
              {
                  ++MsCoun;
                  memset(&bc_page, 0, sizeof(bc_page));
                   if (channel == 1)
                    bc_page.bus = BUS_A;
                  else
                     bc_page.bus = BUS_B;
                  bc_page.self_test = NO;
                  bc_page.mask_bcst_bit = NO;
                  bc_page.broadcast = NO;
                  bc_page.mode_code = NO;
                  bc_page.rt_rt = NO;
                  bc_page.loop = 0x0000;
                  bc_page.cmd_1.status = 0xCCCC;
                  bc_page.cmd_1.subaddr = 1;
                  bc_page.cmd_1.wcnt = 1;
                  bc_page.cmd_1.t_r = RECEIVE;

                  int datacount = 0;

                  switch (msg_no)
                  {
                      /* ---- Mode Codes ---- */
                      case 1:  // 31(A) 
                        bc_page.broadcast = YES;
                        bc_page.mode_code = YES;
                        bc_page.cmd_1.taddr = 31;
                        bc_page.cmd_1.t_r = RECEIVE;
                        bc_page.cmd_1.subaddr = 0;
                        bc_page.cmd_1.wcnt = 17;
                       break;
                      /* ---- Regular RX/TX ---- */
                      case 2:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 16; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 3:  bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 4:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 24; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 5:  bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 6:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 7: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 8: bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 9: bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 10: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 11: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 12: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 13: bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 14: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                      case 15: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;

                      /* ---- RT-RT ---- */
                      case 16:
                          bc_page.rt_rt = YES;
                          bc_page.cmd_1.taddr = 15;  // TX RT
                          bc_page.cmd_1.subaddr = 10; // TX SA
                          bc_page.cmd_2.taddr = 2;   // RX RT
                          bc_page.cmd_2.subaddr = 10; // RX SA
                          bc_page.cmd_1.wcnt = 32;
                          bc_page.cmd_2.wcnt = 32;
                          bc_page.cmd_1.t_r = RECEIVE;
                          bc_page.cmd_2.t_r = TRANSMIT;
                          break;

                      /* ---- Continue RX/TX ---- */
                      case 17: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 17; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 18: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 8;  bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 19: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 20: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = RECEIVE; break;
                      case 21: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                      case 22: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 21; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 23: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 10; bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 24: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 17; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 25: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 9;  bc_page.cmd_1.t_r = RECEIVE; break;
                      case 26: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 27: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 28: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 8;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 29: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 19; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 30: bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 11; bc_page.cmd_1.wcnt = 4;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 31: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 9;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 32: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 8;  bc_page.cmd_1.wcnt = 2;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 33: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 0;  bc_page.cmd_1.wcnt = 0;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 34: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 35: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 26; bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 36: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 18; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 37: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 19; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 38: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 10; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                      case 39: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = TRANSMIT; break;
                      case 40: bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 11; bc_page.cmd_1.wcnt = 17; bc_page.cmd_1.t_r = TRANSMIT; break;

                      /* ---- Mode Codes ---- */
                      case 41: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 0;bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; break;
                      case 42: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 0;bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; break;
                      default:
                          continue;
                  }
                  int Wcount = bc_page.cmd_1.wcnt ? bc_page.cmd_1.wcnt : 32;

                   if(bc_page.rt_rt!=YES)
                   {
                      if (bc_page.cmd_1.t_r != TRANSMIT)
                      {
                          if (bc_page.cmd_1.subaddr == 0)
                          {
                              data_buffer[0] =  MC_minor_cycle_number;                               
                          }
                          else
                          {
                              for (idx = 0; idx < Wcount; idx++)
                                  data_buffer[idx] = ++data_value;
                          }
                      }
                  }
                  DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);    
              }
              break;  
            case 4 :
             /* ================= FRAME 4 ================= */
            for (msg_no = 1; msg_no <= 44; msg_no++)
            {
                ++MsCoun;
                memset(&bc_page, 0, sizeof(bc_page));
                 if (channel == 1)
                    bc_page.bus = BUS_A;
                  else
                     bc_page.bus = BUS_B;
                bc_page.self_test = NO;
                bc_page.mask_bcst_bit = NO;
                bc_page.broadcast = NO;
                bc_page.mode_code = NO;
                bc_page.rt_rt = NO;
                bc_page.loop = 0x0000;
                bc_page.cmd_1.status = 0xCCCC;
                bc_page.cmd_1.subaddr = 1;
                bc_page.cmd_1.wcnt = 1;
                bc_page.cmd_1.t_r = RECEIVE;

                switch (msg_no)
                {
                    /* ---- Mode Codes ---- */
                    case 1:  // 31 (A)   
                        bc_page.broadcast = YES;
                        bc_page.mode_code = YES;
                        bc_page.cmd_1.taddr = 31;
                        bc_page.cmd_1.t_r = RECEIVE;
                        bc_page.cmd_1.subaddr = 0;
                        bc_page.cmd_1.wcnt = 17;
                        break;

                    /* ---- Regular RX/TX ---- */
                    case 2:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 16; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 3:  bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 4:  bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 5:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 24; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 6:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 7: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 26; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 8: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 26; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 9: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 10: bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 11: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 18; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 12: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 18; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 13: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 12; bc_page.cmd_1.wcnt = 25; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 14: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 15: bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 4;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 16: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 17: bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 20; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 18: bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 21; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 19: bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 30; bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 20: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 12; bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 21: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 22: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 9;  bc_page.cmd_1.wcnt = 17; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 23: bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 24; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 24: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 14; bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 25: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 29; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 26: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 27: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 10; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 28: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 21; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 29: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 30: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 17; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 31: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 15; bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 32: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 33: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 34: bc_page.cmd_1.taddr = 13; bc_page.cmd_1.subaddr = 11; bc_page.cmd_1.wcnt = 4;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 35: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 36: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 26; bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 37: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 18; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 38: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 19; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 39: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 40: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 9;  bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 41: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 42: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = TRANSMIT; break;

                    /* ---- Mode Codes ---- */
                    case 43: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 0;bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; break;
                    case 44: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 14; bc_page.cmd_1.subaddr = 0;bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; break;
                     default:
                        continue;
                }

                int Wcount = bc_page.cmd_1.wcnt ? bc_page.cmd_1.wcnt : 32;

                 if(bc_page.rt_rt!=YES)
                 {
                    if (bc_page.cmd_1.t_r != TRANSMIT)
                    {
                        if (bc_page.cmd_1.subaddr == 0)
                        {
                            data_buffer[0] =  MC_minor_cycle_number;                             
                        }
                        else
                        {
                            for (idx = 0; idx < Wcount; idx++)
                                data_buffer[idx] = ++data_value;
                        }
                    }
                  }
                  DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);      
              }
              break;				
            case 5 :
             /* ================= FRAME 5 ================= */
            for (msg_no = 1; msg_no <= 43; msg_no++)
            {
               ++MsCoun;
                memset(&bc_page, 0, sizeof(bc_page));
                if (channel == 1)
                    bc_page.bus = BUS_A;
                  else
                     bc_page.bus = BUS_B;
                bc_page.self_test = NO;
                bc_page.mask_bcst_bit = NO;
                bc_page.broadcast = NO;
                bc_page.mode_code = NO;
                bc_page.rt_rt = NO;
                bc_page.loop = 0x0000;
                bc_page.cmd_1.status = 0xCCCC;
                bc_page.cmd_1.subaddr = 1;
                bc_page.cmd_1.wcnt = 1;
                bc_page.cmd_1.t_r = RECEIVE;

                switch (msg_no)
                {
                    /* ---- Mode Codes ---- */
                    case 1:  // 31 (A) 
                        bc_page.broadcast = YES;
                        bc_page.mode_code = YES;
                        bc_page.cmd_1.taddr = 31;
                        bc_page.cmd_1.t_r = RECEIVE;
                        bc_page.cmd_1.subaddr = 0;
                        bc_page.cmd_1.wcnt = 17;
                        break;

                    /* ---- Regular RX/TX/RT-RT ---- */
                    case 2:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 16; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 3:  bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 4:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 24; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 5:  bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 6:  bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 7: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 8: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 9: bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 10: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 11: bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 12: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 13: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 14: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 11; bc_page.cmd_1.wcnt = 6;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 15: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 16: bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 2;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 17: bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 18: bc_page.cmd_1.taddr = 13; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 12; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 19:  
                          bc_page.rt_rt = YES;
                          bc_page.cmd_1.taddr = 13;  // RX RT
                          bc_page.cmd_1.subaddr = 9; // RX SA
                          bc_page.cmd_2.taddr = 15;   // TX RT
                          bc_page.cmd_2.subaddr = 5; // TX SA
                          bc_page.cmd_1.wcnt = 4;
                          bc_page.cmd_2.wcnt = 4;
                          bc_page.cmd_1.t_r = RECEIVE;
                          bc_page.cmd_2.t_r = TRANSMIT;
                          break;
                    case 20: 
                          bc_page.rt_rt = YES;
                          bc_page.cmd_1.taddr = 13;  // RX RT
                          bc_page.cmd_1.subaddr = 8; // RX SA
                          bc_page.cmd_2.taddr = 15;   // TX RT
                          bc_page.cmd_2.subaddr = 8; // TX SA
                          bc_page.cmd_1.wcnt = 32;
                          bc_page.cmd_2.wcnt = 32;
                          bc_page.cmd_1.t_r = RECEIVE;
                          bc_page.cmd_2.t_r = TRANSMIT;
                          break;
                    case 21: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 22: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 8;  bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 23: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 29; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 24: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 25: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 26: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 10; bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 27: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 17; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 28:  
                          bc_page.rt_rt = YES;
                          bc_page.cmd_1.taddr = 15;  // RX RT
                          bc_page.cmd_1.subaddr = 3; // RX SA
                          bc_page.cmd_2.taddr = 13;   // TX RT
                          bc_page.cmd_2.subaddr = 13; // TX SA
                          bc_page.cmd_1.wcnt = 24;
                          bc_page.cmd_2.wcnt = 24;
                          bc_page.cmd_1.t_r = RECEIVE;
                          bc_page.cmd_2.t_r = TRANSMIT;break;
                    case 29: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 8;  bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 30: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 15; bc_page.cmd_1.wcnt = 12; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 31: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 32: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 33: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 8;  bc_page.cmd_1.wcnt = 2;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 34: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 0;  bc_page.cmd_1.wcnt = 2;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 35: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 16; bc_page.cmd_1.wcnt = 22; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 36: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 37: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 26; bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 38: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 18; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 39: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 19; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 40: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 41: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = TRANSMIT; break;

                    /* Modecodes at the end */
                    case 42: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 0;bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; break;
                    case 43: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 0;bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; break;                    
                    default:
                        continue;
                }
                int Wcount = bc_page.cmd_1.wcnt ? bc_page.cmd_1.wcnt : 32;

                 if(bc_page.rt_rt!=YES)
                 {
                    if (bc_page.cmd_1.t_r != TRANSMIT)
                    {
                        if (bc_page.cmd_1.subaddr == 0)
                        {
                            data_buffer[0] =  MC_minor_cycle_number;                             
                        }
                        else
                        {
                            for (idx = 0; idx < Wcount; idx++)
                                data_buffer[idx] = ++data_value;
                        }
                    }
                  }
                  DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);    
              }
       break;				
            case 6 :
              /* ================= FRAME 6 ================= */
            for (msg_no = 1; msg_no <= 45; msg_no++)
            {
               ++MsCoun;
                memset(&bc_page, 0, sizeof(bc_page));
                if (channel == 1)
                  bc_page.bus = BUS_A;
                else
                   bc_page.bus = BUS_B;
                bc_page.self_test = NO;
                bc_page.mask_bcst_bit = NO;
                bc_page.broadcast = NO;
                bc_page.mode_code = NO;
                bc_page.rt_rt = NO;
                bc_page.loop = 0x0000;
                bc_page.cmd_1.status = 0xCCCC;
                bc_page.cmd_1.subaddr = 1;
                bc_page.cmd_1.wcnt = 1;
                bc_page.cmd_1.t_r = RECEIVE;

                switch(msg_no)
                {
                    /* Mode Codes */
                    case 1:
                        bc_page.broadcast = YES;
                        bc_page.mode_code = YES;
                        bc_page.cmd_1.taddr = 31;
                        bc_page.cmd_1.t_r = RECEIVE;
                        bc_page.cmd_1.subaddr = 0;
                        bc_page.cmd_1.wcnt = 17;
                        break;

                    /* Regular RX/TX/RT-RT messages */
                    case 2:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 16; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 3:  bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 4:  bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 5:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 24; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 6:  bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 7: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 8: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 28; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 9: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 10: bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 11: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 12: bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 20; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 13:  
                          bc_page.rt_rt = YES;
                          bc_page.cmd_1.taddr = 15;  // RX RT
                          bc_page.cmd_1.subaddr = 5; // RX SA
                          bc_page.cmd_2.taddr = 13;   // TX RT
                          bc_page.cmd_2.subaddr = 15; // TX SA
                          bc_page.cmd_1.wcnt = 25;
                          bc_page.cmd_2.wcnt = 25;
                          bc_page.cmd_1.t_r = RECEIVE;
                          bc_page.cmd_2.t_r = TRANSMIT;break;
                    case 14: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 15: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 12; bc_page.cmd_1.wcnt = 25; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 16: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 17: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 18: bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 4;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 19: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 20: bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 20; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 21: bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 26; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 22: 
                          bc_page.rt_rt = YES;
                          bc_page.cmd_1.taddr = 13;  // RX RT
                          bc_page.cmd_1.subaddr = 2; // RX SA
                          bc_page.cmd_2.taddr = 15;   // TX RT
                          bc_page.cmd_2.subaddr = 6; // TX SA
                          bc_page.cmd_1.wcnt = 19;
                          bc_page.cmd_2.wcnt = 19;
                          bc_page.cmd_1.t_r = RECEIVE;
                          bc_page.cmd_2.t_r = TRANSMIT;break;
                    case 23: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 12; bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 24: bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 25; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 25: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 9;  bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 26: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 27: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 29; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 28: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 29: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 21; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 30: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 10; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 31: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 17; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 32: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 5;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 33: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 15; bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 34: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 35: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 36: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 17; bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 37: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 18; bc_page.cmd_1.wcnt = 25; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 38: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 39: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 26; bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 40: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 18; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 41: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 19; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 42: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 43: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = TRANSMIT; break;

                    /* Mode Codes at the end */
                    case 44: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 13;bc_page.cmd_1.subaddr = 0; bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; break;
                    case 45: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 0;bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; break;
                     default:
                        continue;
                }
               int Wcount = bc_page.cmd_1.wcnt ? bc_page.cmd_1.wcnt : 32;

                 if(bc_page.rt_rt!=YES)
                 {
                    if (bc_page.cmd_1.t_r != TRANSMIT)
                    {
                        if (bc_page.cmd_1.subaddr == 0)
                        {
                            data_buffer[0] =  MC_minor_cycle_number;                              
                        }
                        else
                        {
                            for (idx = 0; idx < Wcount; idx++)
                                data_buffer[idx] = ++data_value;
                        }
                    }
                  }
                DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);    
            }
      break;	
            case 7 :
              /* ================= FRAME 7 ================= */
            for (msg_no = 1; msg_no <= 47; msg_no++)
            {
                ++MsCoun;
                memset(&bc_page, 0, sizeof(bc_page));
                 if (channel == 1)
                    bc_page.bus = BUS_A;
                  else
                     bc_page.bus = BUS_B;
                bc_page.self_test = NO;
                bc_page.mask_bcst_bit = NO;
                bc_page.broadcast = NO;
                bc_page.mode_code = NO;
                bc_page.rt_rt = NO;
                bc_page.loop = 0x0000;
                bc_page.cmd_1.status = 0xCCCC;
                bc_page.cmd_1.subaddr = 1;
                bc_page.cmd_1.wcnt = 1;
                bc_page.cmd_1.t_r = RECEIVE;

                switch(msg_no)
                {
                    /* Mode Codes */
                    case 1: 
                        bc_page.broadcast = YES; 
                        bc_page.mode_code = YES;
                        bc_page.cmd_1.taddr = 31;
                        bc_page.cmd_1.t_r = RECEIVE;
                        bc_page.cmd_1.subaddr = 0;
                        bc_page.cmd_1.wcnt = 17;
                        break;

                    /* Regular RX/TX/RT-RT messages */
                    case 2:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 16; bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 3:  bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 4:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 5:  bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 6:  bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 7: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 9;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 8: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 9: bc_page.cmd_1.taddr = 11; bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 10: bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 11: bc_page.cmd_1.taddr = 6;  bc_page.cmd_1.subaddr = 17; bc_page.cmd_1.wcnt = 6;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 12: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 28; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 13: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 14: bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 32; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 15: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 12; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 16: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 4;  bc_page.cmd_1.wcnt = 12; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 17: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 2;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 18: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 8;  bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 19: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 2;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 20: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 7;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 21: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 29; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 22: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 23: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 10; bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 24: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 21; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 25: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 1;  bc_page.cmd_1.wcnt = 17; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 26: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 10; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 27: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 6;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 28: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 11; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 29: bc_page.cmd_1.taddr = 5;  bc_page.cmd_1.subaddr = 23; bc_page.cmd_1.wcnt = 4;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 30: bc_page.cmd_1.taddr = 21; bc_page.cmd_1.subaddr = 8;  bc_page.cmd_1.wcnt = 2;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 31: bc_page.cmd_1.taddr = 22; bc_page.cmd_1.subaddr = 0;  bc_page.cmd_1.wcnt = 2;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 32: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 14; bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 33: bc_page.cmd_1.taddr = 10; bc_page.cmd_1.subaddr = 2;  bc_page.cmd_1.wcnt = 27; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 34: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 10; bc_page.cmd_1.wcnt = 14; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 35: bc_page.cmd_1.taddr = 7;  bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 30; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 36:
                          bc_page.rt_rt = YES;
                          bc_page.cmd_1.taddr = 2;  // RX RT
                          bc_page.cmd_1.subaddr = 16; // RX SA
                          bc_page.cmd_2.taddr = 21;   // TX RT
                          bc_page.cmd_2.subaddr = 2; // TX SA
                          bc_page.cmd_1.wcnt = 13;
                          bc_page.cmd_2.wcnt = 13;
                          bc_page.cmd_1.t_r = RECEIVE;
                          bc_page.cmd_2.t_r = TRANSMIT;break;
                    case 37:  
                          bc_page.rt_rt = YES;
                          bc_page.cmd_1.taddr = 2;  // RX RT
                          bc_page.cmd_1.subaddr = 16; // RX SA
                          bc_page.cmd_2.taddr = 22;   // TX RT
                          bc_page.cmd_2.subaddr = 2; // TX SA
                          bc_page.cmd_1.wcnt = 16;
                          bc_page.cmd_2.wcnt = 16;
                          bc_page.cmd_1.t_r = RECEIVE;
                          bc_page.cmd_2.t_r = TRANSMIT;break;
                    case 38: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 7;  bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 39: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 26; bc_page.cmd_1.wcnt = 16; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 40: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 18; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 41: bc_page.cmd_1.taddr = 25; bc_page.cmd_1.subaddr = 19; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 42: bc_page.cmd_1.taddr = 15; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 20; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 43: bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 3;  bc_page.cmd_1.wcnt = 21; bc_page.cmd_1.t_r = TRANSMIT; break;
                    case 44: bc_page.cmd_1.taddr = 16; bc_page.cmd_1.subaddr = 10; bc_page.cmd_1.wcnt = 10; bc_page.cmd_1.t_r = RECEIVE; break;
                    case 45: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = RECEIVE; break;
                    case 46: bc_page.cmd_1.taddr = 26; bc_page.cmd_1.subaddr = 5;  bc_page.cmd_1.wcnt = 1;  bc_page.cmd_1.t_r = TRANSMIT; break;

                    /* Mode Codes */
                    case 47: bc_page.mode_code = YES; bc_page.cmd_1.taddr = 7; bc_page.cmd_1.subaddr = 0; bc_page.cmd_1.t_r = TRANSMIT;      bc_page.cmd_1.wcnt = 0; break;
                    default:
                        continue;
                }

               int Wcount = bc_page.cmd_1.wcnt ? bc_page.cmd_1.wcnt : 32;

                 if(bc_page.rt_rt!=YES)
                 {
                    if (bc_page.cmd_1.t_r != TRANSMIT)
                    {
                        if (bc_page.cmd_1.subaddr == 0)
                        {
                            data_buffer[0] =  MC_minor_cycle_number;                              
                        }
                        else
                        {
                            for (idx = 0; idx < Wcount; idx++)
                                data_buffer[idx] = ++data_value;
                        }
                    }
                  }
                  DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);      
            }

              break;
            default :break;
            }             
             int msg_count = MsCoun - prevPos;  // number of messages for this minor cycle
             int idx;         
             
              for (idx = 0; idx < msg_count; idx++)
              {
                  major_frames[MC_minor_cycle_number].frame_items[idx] = prevPos+idx+1;
                 /*printf("MC[%d]idx[%d]: MsgCount=%d start[%d] end[%d] idx-start[%d]\n",MC_minor_cycle_number,
                   idx+1,
                   msg_count,

                   start,end,idx-start);*/
              }
              major_frames[MC_minor_cycle_number].Mess_Count      = msg_count;
              major_frames[MC_minor_cycle_number].MinorFrame_Time = 20;
              prevPos = MsCoun;              
         }	
	DefMajorFrame(module_id, 0, 8, major_frames);
        //PriorityHighLow(module_id, 2, 1);
        printf("\tBC Scheduling Started Wait........! \n");
        RunBcMajorFrame(module_id, 0,8, major_frames, Times);
        getchar();
        printf("\tEnter to Reset BC........! \n");
        getchar();  
        ResetAce(module_id);
        printf("\tBC Reset ........Success! \n");
}
void ProcessMTDescriptor(
    unsigned char MTModId,
    unsigned short desc,
    unsigned short pos,
    unsigned long *msgCount,
    struct timeval *prev_tv)
{
    MNTPACK buffer;
    char dir = 'R';
    unsigned short ptr, temp, cmd, rtAddr, subAddr, wc_or_mode, trBit;
    struct timeval tv;
    struct tm *t;

    /* Read descriptor header from descriptor slot 'pos' */
    unsigned short Offset = 0x1000 + ((pos * 4) - 4);
    buffer.BlockSts.value  = MemRead(MTModId, Offset, 1);
    buffer.TimeTag         = MemRead(MTModId, Offset + 1, 1);
    buffer.DataBlkPtr      = MemRead(MTModId, Offset + 2, 1);
    ptr                    = buffer.DataBlkPtr;
    buffer.RecCmdWrd.value = MemRead(MTModId, Offset + 3, 1);

    /* Decode command basic fields */
    cmd        = buffer.RecCmdWrd.value;
    rtAddr     = (cmd >> 11) & 0x1F;
    trBit      = (cmd >> 10) & 0x01;
    subAddr    = (cmd >> 5)  & 0x1F;
    wc_or_mode = cmd & 0x1F;
    if (wc_or_mode == 0) wc_or_mode = 32;
    dir = (trBit == 1) ? 'T' : 'R';

    /* Timestamp and header logging */
    gettimeofday(&tv, NULL);
    t = localtime(&tv.tv_sec);
    fprintf(logFile, "\nTime [%02d:%02d:%02d.%06ld] Msg#%lu \n",
            t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec, *msgCount);
    fprintf(logFile, "BlkSts=0x%04X  Cmd=0x%04X\n", buffer.BlockSts.value, buffer.RecCmdWrd.value);
    *prev_tv = tv;

    /* Flags for clarity */
    int is_rt_to_rt = (buffer.BlockSts.value & 0x0800) ? 1 : 0;
    int no_resp    = (buffer.BlockSts.value & 0x0200) ? 1 : 0;

    /* RT->RT special handling */
    if (is_rt_to_rt)
    {
        /* Read second command and status */
        unsigned short secondCmd = MemRead(MTModId, ptr, 1);
        unsigned short txStatus  = MemRead(MTModId, ptr + 1, 1);

        unsigned short rt_tx  = (secondCmd >> 11) & 0x1F;
        unsigned short sa_tx  = (secondCmd >> 5)  & 0x1F;
        unsigned short wc_tx  = secondCmd & 0x1F;
        if (no_resp)
        {
            /* No response for RT->RT */
            fprintf(logFile, "Decoded RT->RT: %u-%u-X (No Response!)  2ndCmd=0x%04X TxRT=%u SA=%u TxStatus=0x%04X\n",
                    rtAddr, subAddr, secondCmd, rt_tx, sa_tx, txStatus);
        }
        else
        {
            if (wc_tx == 0) wc_tx = 32;
            fprintf(logFile, "Decoded: %u-%u-X-%u  2ndCmd=0x%04X TxRT=%u SA=%u TxStatus=0x%04X\n",
                    rtAddr, subAddr, wc_tx, secondCmd, rt_tx, sa_tx, txStatus);

            /* Move pointer to first data word for the second command */
            ptr += 2;
            /* Read data words for the second command */
            fprintf(logFile, "Data (RT->RT second cmd):\n");
            for (int i = 0; i < (int)wc_tx; i++)
            {
                buffer.DataPack[i] = MemRead(MTModId, ptr++, 1);
                fprintf(logFile, "%04X ", buffer.DataPack[i]);
                if ((i + 1) % 8 == 0) fprintf(logFile, "\n");
            }
            fprintf(logFile, "\n");
        }

        /* Done with RT->RT: do NOT fall through to generic data-read below */
    }
    else
    {
        /* Normal RT/BC case */
        if (no_resp)
        {
            fprintf(logFile, "Decoded: %u-%u-%c-%u (No Response!)\n", rtAddr, subAddr, dir, wc_or_mode);
        }
        else
        {
            fprintf(logFile, "Decoded: %u-%u-%c-%u\n", rtAddr, subAddr, dir, wc_or_mode);
        }

        temp = wc_or_mode;
        if (subAddr == 0) 
        {
          if((temp==16)||(temp==17)||(temp==18))
            temp = 1; /* Mode command fix */
          else
            temp = 0;
        }

        /* If TrnRecv==1, hardware places an extra word (pointer needs advance) */
        if (buffer.RecCmdWrd.bitwise.TrnRecv == 1)
        {
            /* advance ptr to data words (matches original behavior) */
            ptr++;

            if (!no_resp)
            {
                fprintf(logFile, "Data:\n");
                for (int i = 0; i < (int)temp; i++)
                {
                    buffer.DataPack[i] = MemRead(MTModId, ptr++, 1);
                    fprintf(logFile, "%04X ", buffer.DataPack[i]);
                    if ((i + 1) % 8 == 0) fprintf(logFile, "\n");
                }
                fprintf(logFile, "\n");
            }
        }
        else
        {          
            fprintf(logFile, "Data:\n");
            if (buffer.RecCmdWrd.value == 0xF811) temp = 1; /* special-case */
            for (int i = 0; i < (int)temp; i++)
            {
                buffer.DataPack[i] = MemRead(MTModId, ptr++, 1);
                fprintf(logFile, "%04X ", buffer.DataPack[i]);
                if ((i + 1) % 8 == 0) fprintf(logFile, "\n");
            }
            fprintf(logFile, "\n");            
        }
    }    
    if ((*msgCount % 1000) == 0)
        fflush(logFile);
}

void* StartMTThread(void* arg)
{
    unsigned char MTModId = *((unsigned char*)arg);
    unsigned short current = 0, last = 0, prev = 0;
    unsigned short pos = 0;
    unsigned long msgCount = 0;
    struct timeval prev_tv;

    signal(SIGINT, handle_sigint); 
    last = ReadMsgMtCounter(MTModId);//MemRead(MTModId, 0x102, 1);
    //RegRead(MTModId, 0xB, 1); 
    gettimeofday(&prev_tv, NULL);

    while (ThreadRunFlag == 0)
    {
        current =ReadMsgMtCounter(MTModId);// MemRead(MTModId, 0x102, 1);// 
        //RegRead(MTModId, 0xB, 1);
     
        if (last != current)
        {
           ++last;          
            if (pos != 0)
            {            
                ProcessMTDescriptor(MTModId, last, pos, &msgCount, &prev_tv);
                 ++msgCount;    
            }
            prev = last;
                 
            pos = (pos >= MSG_COUNT) ? 1 : pos + 1;
        }       
    }    
     //ProcessMTDescriptor(MTModId, ++last, ++pos, &msgCount, &prev_tv);
     printf("MT Reading Over. Total messages: %lu \n", (msgCount));   
    return 0;
}
short ReadMsg1553(UCHAR ModId,unsigned short stack_entry,RT_MSG *message,unsigned long MsgCnt)//Use for TxVectorWord
{
	CONFIG_BITS config;
	unsigned short msg,look_up,i,offset,temp;
	unsigned short error=0;
	if ((stack_entry <=64 || stack_entry == LAST_MSG) && stack_entry >=1)
	{
	    if(stack_entry == LAST_MSG)
	    {
	      msg = MemRead(ModId,STACKA_PTR,1);
	      message->bsw = MemRead(ModId,STACK_1 + (msg - 4),1 );
	      message->cmd = MemRead(ModId,STACK_1 + (msg-1),1);
	    }
	   else if(stack_entry != LAST_MSG)
           {
	     // temp = MemRead(ModId,STACK_1 + ((stack_entry * 4) - 4),1);
	     // message->bsw = MemRead(ModId,STACK_1 + ((stack_entry * 4)-4),1);
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
	    //printf("Message #%lu\n", MsgCnt);
            fprintf(RtlogFile, "\nMessage #%lu\n", MsgCnt);

            //printf("BSW      : 0x%X\n", rtMsg.bsw);
            //fprintf(RtlogFile, "BSW      : 0x%X\n", message->bsw );

            //printf("CMD      : 0x%X\n", rtMsg.cmd);
            fprintf(RtlogFile, "CMD      : 0x%X\n", message->cmd);

            //printf("Word Cnt : 0x%X\n", rtMsg.wcnt);
            fprintf(RtlogFile, "Word Cnt : 0x%X\n", message->wcnt) ;

            //printf("SubAddr  : %d\n", rtMsg.subaddr);
            fprintf(RtlogFile, "SubAddr  : %d\n", message->subaddr);

           // printf("T/R      : 0x%X\n", rtMsg.t_r);
            fprintf(RtlogFile, "T/R      : 0x%X\n", message->t_r);            

            //printf("Data     :\n\t");

            fprintf(RtlogFile, "Data     :\n\t");      
	   
            temp = message->wcnt;
            if(temp == 0)
	            temp = 32;
             if(message->subaddr==0)
             {
	       message->data[0] = MemRead(ModId,offset+0,1);
	       fprintf(RtlogFile, "%04X ",message->data[0]);
             }
             else
             {
                for (i=0;i<temp;i++)
                {
                    message->data[i] = MemRead(ModId,offset+i,1);
                    fprintf(RtlogFile, "%04X ",message->data[i]);
                    if ((i + 1) % 8 == 0)
                    {  // printf("\n\t");
                        fprintf(RtlogFile, "\n\t");
                    }
                }
            }
            fflush(RtlogFile);
	}
	else
		error = 36;
	return(error);
}
void* RTThread_Bus(void* arg)
{
    unsigned char modId = *((unsigned char*)arg);
    unsigned short channel=1;
    unsigned short currentDesc = 0, lastDesc = 0, pos = 1;
    unsigned long msgCount = 1;
    unsigned short wordCount;
    int i;
  
    // lastDesc = RegRead(modId, 0x10, 1);
    lastDesc = ReadMsgRtCounter(modId);
    //MemRead(modId, 0x100, 1);

    while (ThreadRunFlag == 0)
    {
        currentDesc = ReadMsgRtCounter(modId);//RegRead(modId, 0x10, 1);
        //MemRead(modId, 0x100, 1);
       
        if (lastDesc != currentDesc)
        {
             RT_MSG rtMsg;
             ReadMsg1553(modId, pos, &rtMsg,msgCount);
             /*fprintf(RtlogFile, "\nMessage #%lu\n", msgCount);
             printf("\nMessage #%lu\n", msgCount);*/
            #if 0
            if(rtMsg.cmd==0xfba2)
            {       
              UBIT16 data[20][32], sub_addr[10], wordcnt[20];
             
              time_t now = time(NULL);
              struct tm t = *localtime(&now);


              struct timeval tv;
              gettimeofday(&tv, NULL);
              unsigned long ms = (tv.tv_usec / 1000) % 1000;

              MC1_12_broadcast_mesg.hour_min.hour     = (UBIT16)t.tm_hour;
              MC1_12_broadcast_mesg.hour_min.minutes  = (UBIT16)t.tm_min;
              MC1_12_broadcast_mesg.sec_20ms.seconds  = (UBIT16)t.tm_sec;

              MC1_12_broadcast_mesg.sec_20ms.count_ms20 = (UBIT16)(ms / 20);
              
              memcpy(data[0], &MC1_12_broadcast_mesg, sizeof(TIME_BROADCAST));              
              unsigned short dynamic_data[32],idx=0;

              for (idx = 0; idx < 2; idx++)
                dynamic_data[idx] = data[0][idx];
              UpdateMsgData(1, 2, dynamic_data, 2, RECEIVE);

              unsigned short cmd_update_data[32];
              for (idx = 0; idx < 32; idx++)
                  cmd_update_data[idx] = 0x1553 + idx;
              UpdateMsgCmdData(1,

                               95,
                               0x0820,
                               0x0000,
                               cmd_update_data,

                               32,
                               RECEIVE,
                               channel);

              
            }
            #endif
            pos = (pos >= 64) ? 1 : pos + 1;
            ++msgCount;
            lastDesc = currentDesc;
        }
    }
    printf("RT Reading Over. Total messages: %lu\n", (msgCount-1));
    fclose(logFile);
    return NULL;
}

#if 0
void* RTThread_Bus(void* arg)
{
    unsigned char modId = *((unsigned char*)arg);
   int ret, i, msgIdx,RtAdres=0,Sa=0,TimeB=0;
    unsigned short wordCount,channel=1;
    unsigned short txBuffer[BUFFER_SIZE];
    RT_MSG rtMsg;
    FILE* logFile = NULL;
    unsigned long Lcount = 1;
    unsigned short currentDesc = 0, lastDesc = 0, pos = 1;
    unsigned int config;   
    logFile = fopen("Rt_log.txt", "w");	
      
    if (!logFile) 
    {
        perror("Failed to open log file");
        return -1;
    }
  while(ThreadRunFlag==0)
  {
   currentDesc = MemRead(modId, 0x100,1);//(((MemRead(modId, 0x0,1))-0x1000)/4);//MtMsgCnt(MTModId);
   //if (lastDesc != currentDesc) 
    {
      while (lastDesc != currentDesc ) 
      {
          ReadMsg(modId, pos, &rtMsg);
          #if 0
          if(rtMsg.cmd==0xfba2)
          {
            TimeB++;
            UBIT16 data[20][32], sub_addr[10], wordcnt[20];
             
            time_t now = time(NULL);
            struct tm t = *localtime(&now);

            struct timeval tv;
            gettimeofday(&tv, NULL);
            unsigned long ms = (tv.tv_usec / 1000) % 1000;

            MC1_12_broadcast_mesg.hour_min.hour     = (UBIT16)t.tm_hour;
            MC1_12_broadcast_mesg.hour_min.minutes  = (UBIT16)t.tm_min;
            MC1_12_broadcast_mesg.sec_20ms.seconds  = (UBIT16)t.tm_sec;
            MC1_12_broadcast_mesg.sec_20ms.count_ms20 = (UBIT16)(ms / 20);
            
            memcpy(data[0], &MC1_12_broadcast_mesg, sizeof(TIME_BROADCAST));              
            unsigned short dynamic_data[32],idx=0;
            for (idx = 0; idx < 2; idx++)
              dynamic_data[idx] = data[0][idx];
            UpdateMsgData(1, 2, dynamic_data, 2, RECEIVE);
            unsigned short cmd_update_data[32];
            for (idx = 0; idx < 32; idx++)
                cmd_update_data[idx] = 0x1553 + idx;
            UpdateMsgCmdData(1,
                             95,
                             0x0820,
                             0x0000,
                             cmd_update_data,
                             32,
                             RECEIVE,
                             channel);
            
          }
          #endif
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
         /* printf("BIT_STAT : 0x%X\n", RegRead(modId,0x0F,1));
          printf("RT_SWR   : 0x%X\n", RegRead(modId,0x0E,1));*/       
          
          wordCount = rtMsg.wcnt ? rtMsg.wcnt : BUFFER_SIZE;
           if(rtMsg.subaddr==0)
            wordCount=1;
          //fprintf(logFile,"Data     :\n\t");
           printf("Data     :\n\t");
          for (i = 0; i < wordCount; ++i)
          {
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
}
#endif

