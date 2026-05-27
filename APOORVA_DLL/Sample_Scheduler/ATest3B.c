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
//
//#include "/home/acl/APOORVA_CCPMC/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
//#include "/home/desktop/Documents/APOORVA_CCPMC/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
//#include "/home/ccpmc/Documents/CCPMC_ANIL/Anil/APOORVA_CCPMC/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
//#include "/home/ccpmc/Documents/CCPMC_ANIL/Anil/APOORVA_CCPMC/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
//#include "/home/ccpmc/Documents/CCPMC_ANIL/NEW/Working_DMA_1553/17-01-2026/APOORVA_CCPMC_DMA/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"


//#include "/home/ccpmc/Downloads/ccpmc_test/ccpmc/APOORVA_CCPMC_DMA/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"

//#include "/home/hruthik/Downloads/ccpmc/APOORVA_CCPMC_DMA/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
//#include "/home/acl/Desktop/APOORVA_CCPMC_DMA/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
//#include "/home/ccpmc/Desktop/APOORVA_CCPMC_DMA/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
#include "/home/ccpmc/Desktop/WORKING/APOORVA_CCPMC_DMA_Interrupt_Handler/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
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

#define C4A00_ADDR  0xC4A00
#define BIT1_MASK (1 << 1)

#define BIT2_MASK (1 << 2)

#define         CONFIG_REG_INIT_VAL       0x00000078

#define         ENABLE_BC_INT       0x00004029  //bc
#define         ENABLE_RT_INT       0x00000000   //rt
#define         ENABLE_MT_INT       0x00004001 //mt

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

typedef struct {
    unsigned char  MTModId;
    unsigned char  BCModId;
    unsigned long  expectedMessages;
} MTThreadArgs;

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
void* StartMTThread_NEW(void* arg);
void* RTThread_Bus(void* arg);
typedef long long LONGLONG;
pthread_mutex_t mutexObj = PTHREAD_MUTEX_INITIALIZER;
unsigned short txBuffer[BUFFER_SIZE];
int DefMajorFrame_new(UCHAR Bus,USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[]);
unsigned long RunBcMajorFrame_new(UCHAR Bus,USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],unsigned long times);

short ReadMsg1553(UCHAR ModId,unsigned short stack_entry,RT_MSG *message,unsigned long MsgCnt);

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
int extMT_global;
struct timeval t_struct;

void ActivateMc(unsigned char ModId,unsigned short Mode);

#define MSG_COUNT 1024
FILE* logFile = NULL;
FILE* RtlogFile=NULL;
FILE* MtlogFile=NULL;


void handle_sigint(int sig) {
    printf("\nExiting...\n");
    if (logFile) fclose(logFile);
     if (RtlogFile) fclose(RtlogFile);
     if (MtlogFile) fclose(MtlogFile);
    exit(0);
}
void ConfigureMt(unsigned char MTModId)
{
  ClearRam(MTModId);
  ResetAce(MTModId); 
  
  // Initialize descriptor pointer from hardware (0xB register)      
  RegWrite(MTModId,0x03, 0x1, 1);
  RegWrite(MTModId, 0x3, 0x2, 1);
  RegWrite(MTModId, CONFIG_REG, 0x5000, 1);
  RegWrite(MTModId, 0x07, 0x8000, 1);
  RegWrite(MTModId, 0x07, 0x8000, 1);
  printf("From Configure MT\n");
  printf("regadd=%x regread=%x\n",CONFIG_REG,RegRead(MTModId,CONFIG_REG,1));
  SelMsgMt(MTModId);
  //sel_subaddr(MTModId,0xa83e);
  //desel_subaddr(MTModId,0xa83e);
  //MemWrite(3,0x202e5,0x0000,1);
  printf("address=0x%08x  data=0x%04x\n",0x202e5,MemRead(MTModId,0x202e5,1));
  
  RunMt(MTModId); 
}
void ConfigureBCMt(unsigned char MTModId)
{
  ClearRam(MTModId);
  ResetAce(MTModId); 
  
  // Initialize descriptor pointer from hardware (0xB register)      
  RegWrite(MTModId,0x03, 0x1, 1);
  RegWrite(MTModId, 0x3, 0x2, 1);
  RegWrite(MTModId, CONFIG_REG, 0xd000, 1);
  RegWrite(MTModId, 0x07, 0x8000, 1);
  RegWrite(MTModId, 0x07, 0x8000, 1);
  printf("regadd=%x regread=%x\n",0x03,RegRead(MTModId,0x03,1));
  printf("regadd=%x regread=%x\n",0x7,RegRead(MTModId,0x7,1));
  printf("regadd=%x regread=%x\n",0x1,RegRead(MTModId,0x1,1));
  printf("regadd=%x regread=%x\n",CONFIG_REG,RegRead(MTModId,CONFIG_REG,1));
  SelMsgMt(MTModId);
  //MemWrite(3,0x202e5,0x0000,1);
  printf("address=0x%08x  data=0x%04x\n",0x202e5,MemRead(3,0x202e5,1));
  
  RunMt(MTModId);
  
  
}
volatile int bc_done = 0;
int main(void)
{
    int NoOfTimes1 = 0, rtAddr = 0, RtStatusBits = 0;
    int BcMod = 1,rtModId = 2, mtModId = 2;
    char input[32];
    fd_set set;
    struct timeval timeout;
    
    logFile = fopen("mt_log.txt", "w");
    if (!logFile)
    {
        perror("Failed to open log file");
        return -1;
    }
    
    MtlogFile=fopen("mt_log_new.txt","w");
    if(!MtlogFile)
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
        fclose(MtlogFile);
        return -1;
    }
  
   printf("***********************reading data************************\n");
   
   InitInterrupts(BcMod,CONFIG_REG_INIT_VAL,CARD1);
   InitInterrupts(mtModId,CONFIG_REG_INIT_VAL,CARD1);
   //InitInterrupts(rtModId,CONFIG_REG_INIT_VAL,CARD1);
   
   getchar();
   
  EnableInterruptsBC(BcMod,ENABLE_BC_INT,CARD1);
  EnableInterruptsMT(mtModId,ENABLE_MT_INT,CARD1);
 //EnableInterruptsRT(rtModId,ENABLE_RT_INT,CARD1);
   
   SetBCSOMCnt(BcMod,0,CARD1);
   SetBCEOMCnt(BcMod,0,CARD1);
   SetBCEOFCnt(BcMod,0,CARD1);
   SetBCEOMMINORFRAMECnt(BcMod,0,CARD1);
   
   
   
    SetMTSOMCnt(mtModId,0,CARD1);
    SetMTEOMCnt(mtModId,0,CARD1);
   
   //SetRTSOMCnt(rtModId,0,CARD1);
   //SetRTEOMCnt(rtModId,0,CARD1);
   getchar();
   
  
 
      /* ClearRam(1);
    read_phys(1, 1);*/
  #if 0
    write_via_dma1(1,1);
    
    Write_control_registers_for_Write(1,1);
    write_dma_and_valr_regs(0x100,0x4000);
    write_dma_and_valr_reg(1);
    check_bit_status(1, BIT1_MASK);
    check_bit_status(1, BIT2_MASK);
    
 // second transfer
 #if 1
  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs(0x7d0 + 0x100,0x4000 + 0x3e8);//0x3FE
  write_dma_and_valr_reg(1);
 

  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs((0x7d0 * 0x02) + 0x100,0x4000 + 0x3e8 * 0x02);//0x7FE
  write_dma_and_valr_reg(1);
  

  //fourth transfer
  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs((0x7d0 * 0x03) + 0x100,0x4000 + 0x3e8 * 0x03);
  write_dma_and_valr_reg(1); 
  

  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs((0x7d0 * 0x04) + 0x100,0x4000 + 0x3e8 * 0x04);
  write_dma_and_valr_reg(1);
 

  //sixth transfer
  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs((0x7d0 * 0x05) + 0x100,0x4000 + 0x3e8 * 0x05);
  write_dma_and_valr_reg(1); 
 
  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs((0x7d0 * 0x06) + 0x100,0x4000 + 0x3e8 * 0x06);
  write_dma_and_valr_reg(1);
 

  //eighth transfer
  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs((0x7d0 * 0x07) + 0x100,0x4000 + 0x3e8 * 0x07);
  write_dma_and_valr_reg(1); 


  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs((0x7d0 * 0x08) + 0x100,0x4000 + 0x3e8 * 0x08);
  write_dma_and_valr_reg(1);
  

  //tenth transfer
  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs((0x7d0 * 0x09) + 0x100,0x4000 + 0x3e8 * 0x09);
  write_dma_and_valr_reg(1); 
  

  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs((0x7d0 * 0x0A) + 0x100,0x4000 + 0x3e8 * 0x0a);
  write_dma_and_valr_reg(1);
 
  //tweleveth transfer
  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs((0x7d0 * 0x0B) + 0x100,0x4000 + 0x3e8 * 0x0b);
  write_dma_and_valr_reg(1); 
 

  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs((0x7d0 * 0x0C) + 0x100,0x4000 + 0x3e8 * 0x0c);
  write_dma_and_valr_reg(1);
  

  usleep(1000);
  Write_control_registers_for_Write(1,1);

  write_dma_and_valr_regs((0x7d0 * 0x0D) + 0x100,0x4000 + 0x3e8 * 0x0d);
  write_dma_and_valr_reg(1); 
  

  usleep(1000);
  Write_control_registers_for_Write(1,1);
  write_dma_and_valr_regs((0x7d0 * 0x0E) + 0x100,0x4000 + 0x3e8 * 0x0e);
  write_dma_and_valr_reg(1);

  #endif 
  
  /*getchar();//usleep(1000);
  Write_control_registers_for_read(1,1);
  //Read_control_registers(1,1);
  write_dma_and_valr_reg(3);
  getchar();*/
  //check_bit_status(1, BIT2_MASK);
  //Write_control_registers_for_read2(1,1);
  
  
        /* for(int i = 0x0 ; i <  0x28 ; i++)
          {
         printf("Address 0x%08x  Data = 0x%04x\n",0x4000+ 0x28*0 +i,MemRead(1, 0x4000+0x28*0 + i,1));
          }*/
 
 //read_dma_block(1,1);
  
  #endif
  
     ResetAce(1);
     ResetAce(2);
     ResetAce(3);
     
     //ClearRam(1); ClearRam(2); ClearRam(3);
     RegWrite(2,0x00a2,0x0001,1); //top start 
     // cfg reg BC-0,RT-8000,MT-4000 BC-MT-F000, RT-MT-F000  
    
    int choice;
    int channel,NoOfTimes;
     MTThreadArgs mtArgs;
    while(1)
    {
        printf("\n1.BC 2.MT 3.RT 4.BC+MT 5.BC+RT 6.RT+MT 7.RT Status 8.BC-MT 9.RT-MT 0.Exit\n");
        scanf("%d",&choice);

        if(choice == 0)
            break;

        switch(choice)
        {
            case 1:
                bc_done=0;
                printf("enter channel CHA : 1  CHB : 2  :");
                scanf("%d",&channel);
                printf("no of times to run:");
                scanf("%d",&NoOfTimes);
                BC_Mode(BcMod,channel,NoOfTimes);
                BCSchedule_Bus(NoOfTimes,BcMod,channel);
                bc_done = 1;
                break;

            case 2:
              MT_Mode(mtModId);              
              ThreadRunFlag = 0;              
              pthread_create(&mtThread, NULL, StartMTThread_NEW, &mtModId);              
              // Flush leftover newline from scanf
              while(getchar() != '\n');   // ← flush buffer              
              printf("MT running... Press ENTER to stop\n");
              getchar();                  // ← now actually waits              
              ThreadRunFlag = 1;
              pthread_join(mtThread, NULL);
              ThreadRunFlag = 0;             
              ResetAce(mtModId);
              SetMTEOMCnt(mtModId, 0, CARD1);
              break;

            case 3:
                printf("enter the rt address:");
                scanf("%d",&rtAddr);
                RT_Mode(BcMod,rtModId,rtAddr);
                pthread_create(&thread, NULL, StartMTThread_NEW, &rtModId);
                break;

            case 4:
                bc_done=0;
                printf("enter channel CHA : 1  CHB : 2  :");
                scanf("%d",&channel);
                printf("no of times to run:");
                scanf("%d",&NoOfTimes);
                BC_MT_Mode(BcMod,mtModId,channel,NoOfTimes);
                
                SetBCSOMCnt(BcMod, 0, CARD1);
                SetBCEOMCnt(BcMod, 0, CARD1);
                SetBCEOFCnt(BcMod, 0, CARD1);
                SetBCEOMMINORFRAMECnt(BcMod, 0, CARD1);
                
                mtArgs.MTModId          = mtModId;
                mtArgs.expectedMessages = NoOfTimes * 100; 
                pthread_create(&mtThread, NULL, StartMTThread, &mtArgs); 
                
                BCSchedule_Bus(NoOfTimes,BcMod,channel);
                bc_done = 1;
                pthread_join(mtThread, NULL);
                ResetAce(mtModId);// stop MT hardware
                ResetAce(BcMod);
                SetMTEOMCnt(mtModId, 0, CARD1);           // reset counter for next run
                printf("MT hardware stopped.\n");
                break;

            case 5:
                bc_done=0;
                printf("enter the rt address:");
                scanf("%d",&rtAddr);
                printf("enter channel CHA : 1  CHB : 2  :");
                scanf("%d",&channel);
                printf("no of times to run:");
                scanf("%d",&NoOfTimes);
                BC_RT_Mode(BcMod,rtModId,rtAddr,channel,NoOfTimes);
                pthread_create(&thread, NULL, StartMTThread_NEW, &rtModId);
                BCSchedule_Bus(10,1,1);
                bc_done = 1;
                break;

            case 6:
                printf("enter the rt address:");
                scanf("%d",&rtAddr);
                RT_MT_Mode(rtModId,mtModId,rtAddr);
                pthread_create(&mtThread, NULL, StartMTThread, &mtModId);
                pthread_create(&thread, NULL, RTThread_Bus, &rtModId);
                break;

            case 7:
                printf("enter the rt address:");
                scanf("%d",&rtAddr);
                printf("enter rt status bit:");
                scanf("%d",&RtStatusBits);
                RT_Status_Mode(rtModId,rtAddr,RtStatusBits);
                break;

            case 8:
                bc_done=0;
                printf("enter channel CHA : 1  CHB : 2  :");
                scanf("%d",&channel);
                printf("no of times to run:");
                scanf("%d",&NoOfTimes);
                BC_MT_Mode_snode(BcMod,mtModId,channel,NoOfTimes);
                
                SetBCSOMCnt(BcMod, 0, CARD1);
                SetBCEOMCnt(BcMod, 0, CARD1);
                SetBCEOFCnt(BcMod, 0, CARD1);
                SetBCEOMMINORFRAMECnt(BcMod, 0, CARD1);
                SetMTEOMCnt(mtModId, 0, CARD1);  
                
                mtArgs.MTModId          = BcMod;  // ← MT is also BcMod
                mtArgs.BCModId          = BcMod;
                mtArgs.expectedMessages = NoOfTimes * 100; 
                pthread_create(&mtThread, NULL, StartMTThread, &mtArgs);   
                BCSchedule_Bus(NoOfTimes, BcMod, channel);
                bc_done = 1;   
                pthread_join(mtThread, NULL);
                ResetAce(BcMod);
                SetMTEOMCnt(BcMod, 0, CARD1);
                SetBCSOMCnt(BcMod, 0, CARD1);
                SetBCEOMCnt(BcMod, 0, CARD1);
                printf("MT hardware stopped.\n");
                break;

            case 9:
                printf("enter the rt address:");
                scanf("%d",&rtAddr);
                RT_MT_Mode_snode(rtModId,rtAddr);
                pthread_create(&mtThread, NULL, StartMTThread_NEW, &rtModId);
                break;
        
                
                
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
        

        // Clean up threads
        pthread_join(thread, NULL);
        if (testNum != 1) pthread_join(mtThread, NULL);
        disableNonCanonicalMode();
     }
    }
             
    printf("\n***************************************** OVER ****************************************\n");
    pthread_mutex_destroy(&mutexObj);
    fclose(logFile);
    fclose(RtlogFile);
    printf("\nProgram terminated normally.\n");

    return 0;
}

void MTConfigure(unsigned char MTModId,unsigned short Config1 ,unsigned short Config3)
{
  ClearRam(MTModId);
  ResetAce(MTModId); 
  for(i=0; i < 128; i++)    //SELMESG MT LOOK-UP-TABLE
    {
        MemWrite(MTModId,(0x20280+i),0xffff,1); ///changed add 20280 29/01/26
    }
  MemWrite(MTModId,(0x20102),0x1000,1); //COMMAND STACK POINTER
  MemWrite(MTModId,(0x20103),0x4000,1); //DATA STACK POINTER
 // RegWrite(MTModId,0x1,0x5000,1);  //CONFIGURATION 12-BIT MUST BE HIGH
   RegWrite(MTModId,0x1,Config1,1); 
  RegWrite(MTModId,0x7,Config3,1); //CONFIGURATION  15-BIT MUST BE HIGH
  RegWrite(MTModId,0x3,0x2,1);  //START REGISTER
 
  
}
#if 0
void ConfigureRT(UCHAR rtModId, unsigned char rtAddr)
{
  int Sa=0,sa=0;
  
    SetRtAddrM(rtModId, rtAddr, 1);
    //DefLookUp(rtModId, LOOKUP_1);   
    //DefMode(rtModId, RT);
     RegWrite(rtModId,0x21,0xdf8f,1);
     RegWrite(rtModId,0x23,0xa000,1);
     static unsigned short dataCounter = 0;
     
      for(i=0 ; i < 30; i++)
      {
         for(sa =0 ;sa<32;sa++)
         {
           MemWrite(rtModId, 0x4000+0x20*i+sa,dataCounter++,1);
         }
        
      }
      for(i=0;i<30;i++)
      {
        MemWrite(rtModId, 0x0160+i,0x4000+0x20*i,1);
      }
      
      for(i=0;i<30;i++)
      {
        MemWrite(rtModId, 0x0140+i,0x5000+0x20*i,1);
      }
      for(i = 0; i < 64; i++)
     {
        printf("look up table address %04x  memread=0x%04x\n",0x0140+i,MemRead(rtModId, 0x0140 + i,  1));
     }
    #if 0 
    unsigned short sacw = 0;

    sacw |= (1 << 9);  // RX EOM interrupt
   sacw |= (1 << 8);  // Circular buffer interrupt
    sacw |= (1 << 7) | (1 << 6) | (1 << 5); // MM2 MM1 MM0 = 111 (8192 words)
    sacw |= (1 << 14);  // TX EOM interrupt
    sacw |= (1 << 13);  // TX circular buffer interrupt
    // TX memory mode = 8192-word circular (MM2 MM1 MM0 = 111)
    sacw |= (1 << 12) | (1 << 11) | (1 << 10);
    
    for(sa = 0; sa < 32; sa++)
    {
        MemWrite(rtModId, 0x01A0 + sa, sacw , 1); //0x360
    }
     for(sa = 0; sa < 32; sa++)
     {
        printf(" sa=%d address=%04x SACW memread=0x%04x\n",sa,0x01A0+sa,MemRead(rtModId, 0x01A0 + sa,  1));
     }
     #endif
     
   /* for (Sa = 1; Sa <31; Sa++)
    {
      for (i = 0; i < BUFFER_SIZE; ++i)
      {
          txBuffer[i] = Sa+i;      
      }
       printf("SA-%d %x\n",Sa,MemRead(modId,0x160+Sa,1));    
      WriteDataBlock(rtModId, Sa, txBuffer);
    }*/
    //MemWrite(rtModId,0x120,0xa5a5,1);//Enable ServiceReq(TxVectorWord)
   // RunRt(rtModId); 
}

#endif
void ActivateMc(unsigned char ModId,unsigned short Mode)
{ 
    int RetVal=0;
    //RetVal = ClearRam(ModId);	 
    ResetAce(ModId); 
    printf("bc=%x\n",Mode);
    RetVal = DefMode(ModId,Mode);
    printf("ModuleId of DefMode is :%x\n",RetVal);	 
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
    int msg_count=0;
    UBIT16 data[20][32], sub_addr[10], wordcnt[20];
    printf("\tBus Controller Scheduling Preparation Starts...!\n");
 
    /*************Total 350 Messages*********************/
	
  for(MC_minor_cycle_number=0;MC_minor_cycle_number<64;MC_minor_cycle_number++)
  {    
    switch(MC_minor_cycle_number)
          {
           case 0 :	       
        /* ================= FRAME 0 ================= */
              for (msg_no = 1; msg_no <= 10; msg_no++)
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
                  // printf("MsCoun %d\n",MsCoun);
                   
              DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);                                   
              }
              break;
             
            case 1 :
             /* ==================== FRAME 1 MESSAGE DEFINITION ==================== */
              for (msg_no = 1; msg_no <= 10; msg_no++)
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
        /* ================= FRAME 0 ================= */
              for (msg_no = 1; msg_no <= 10; msg_no++)
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
                   //printf("MsCoun %d\n",MsCoun);
                   
              DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);                                   
              }
              break;
              
            case 3 :
             /* ==================== FRAME 1 MESSAGE DEFINITION ==================== */
              for (msg_no = 1; msg_no <= 10; msg_no++)
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
       case 4 :	       
        /* ================= FRAME 0 ================= */
              for (msg_no = 1; msg_no <= 10; msg_no++)
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
                //   printf("MsCoun %d\n",MsCoun);
                   
              DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);                                   
              }
              break;
              
            case 5 :
             /* ==================== FRAME 1 MESSAGE DEFINITION ==================== */
              for (msg_no = 1; msg_no <= 10; msg_no++)
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
       case 6 :	       
        /* ================= FRAME 0 ================= */
              for (msg_no = 1; msg_no <= 10; msg_no++)
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
                 //  printf("MsCoun %d\n",MsCoun);
                   
              DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);                                   
              }
              break;
              
            case 7 :
             /* ==================== FRAME 1 MESSAGE DEFINITION ==================== */
              for (msg_no = 1; msg_no <= 10; msg_no++)
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
      
            case 8 :
             /* ================= FRAME 2 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
             
            case 9 :
             /* ================= FRAME 3 ================= */
              for (msg_no = 1; msg_no <= 10; msg_no++)
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
           
            case 10 :
             /* ================= FRAME 4 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
            
            case 11 :
             /* ================= FRAME 5 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
       
            case 12 :
              /* ================= FRAME 6 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
     
         case 13 :
              /* ================= FRAME 7 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
      
            case 14 :
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
        
         case 15 :
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
       
           case 16 :
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
        
         case 17 :
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 18 :
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 19:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
       
          case 20:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         
          case 21:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 22:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 23:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 24:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 25:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 26:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 27:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 28:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 29:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 30:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 31:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
       case 32:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
       case 33:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 34:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 35:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 36:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 37:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 38:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
         case 39:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 40:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 41:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 42:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 43:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 44:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 45:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 46:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 47:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 48:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 49:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 50:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 51:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 52:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 53:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 54:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 55:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 56:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 57:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 58:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 59:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 60:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 61:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 62:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
          case 63:
              /* ================= FRAME 8 ================= */
            for (msg_no = 1; msg_no <= 10; msg_no++)
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
                
             msg_count = MsCoun - prevPos;  // number of messages for this minor cycle
             int idx;         
             
              for (idx = 0; idx < msg_count; idx++)
              {
                  major_frames[MC_minor_cycle_number].frame_items[idx] = prevPos+idx+1;
              /*   printf("MC[%d]idx[%d]: MsgCount=%d \n",MC_minor_cycle_number,
                   idx+1,
                   msg_count);*/
              }
              major_frames[MC_minor_cycle_number].Mess_Count      = msg_count;
              major_frames[MC_minor_cycle_number].MinorFrame_Time = 20;
              prevPos = MsCoun;         
         }
      
        
       // DefFrame(module_id, 350, msg_sequence);          
       // RunBc(module_id, 0, 1, 350, Times);
	
	DefMajorFrame_new(module_id,0,10, major_frames);
        
        printf("\tBC Scheduling Started wait........!\n");
        
        RunBcMajorFrame_new(module_id, 0,10, major_frames, Times);
        
   printf("reg read mt %04x-%d \n",0x00F9,RegRead(1,0x00F9,1));
   printf("reg read mt %04x-%d \n",0x00FB,RegRead(1,0x00FB,1));///changed add 20280 29/01/26
   printf("reg read mt %04x-%d \n",0x00F1,RegRead(1,0x00F1,1));
   printf("reg read mt %04x-%d \n",0x00F3,RegRead(1,0x00F3,1));
   
        GetBCSOMCnt(1,CARD1);
        GetBCEOMCnt(1,CARD1);
        GetBCEOFCnt(1,CARD1);
        GetBCEOMINFRAMECnt(1,CARD1);
        
        
        GetMTSOMCnt(2,CARD1);
        GetMTEOMCnt(2,CARD1);
        
        
       
         
        printf("\tEnter to Reset BC........! \n");
        getchar(); getchar();
        ResetAce(module_id);
        printf("\tBC Reset ........Success! \n");
       
}

#if 0
void ProcessMTDescriptor(
    unsigned char MTModId,
    unsigned short desc,
    unsigned short pos,
    unsigned long *msgCount,
    struct timeval *prev_tv)
{
    MNTPACK buffer;
    char dir = 'R';
    unsigned short temp, cmd, rtAddr, subAddr, wc_or_mode, trBit;
    unsigned long ptr;
    struct timeval tv;
    struct tm *t;

    /* Read descriptor header from descriptor slot 'pos' */
    unsigned long Offset = 0x21000 + ((pos * 4) - 4);//21000
    buffer.BlockSts.value  = MemRead(MTModId, Offset, 1);
   // printf("offset=%08x bsw=%04x\n",Offset,buffer.BlockSts.value);
    buffer.TimeTag         = MemRead(MTModId, Offset + 1, 1);
    //printf("off=%08x tt=%04x\n",Offset,buffer.TimeTag);
    buffer.DataBlkPtr      = MemRead(MTModId, Offset + 2, 1);
  //  printf("off=%08x dbr=%04x\n",Offset,buffer.DataBlkPtr);
    ptr                    = 0x20000+buffer.DataBlkPtr;
   // printf("ptr=%08x\n",ptr);
    buffer.RecCmdWrd.value = MemRead(MTModId, Offset + 3, 1);
    //printf("off=%08x cmd=%04x\n",Offset,buffer.RecCmdWrd.value);
   
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
                fprintf(logFile, "%04X ", buffer.DataPack[i] );
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
                    fprintf(logFile, "%04X ", buffer.DataPack[i] );
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
                fprintf(logFile, "%04X ", buffer.DataPack[i] );
                if ((i + 1) % 8 == 0) fprintf(logFile, "\n");
            }
            fprintf(logFile, "\n");            
        }
    }    
    if ((*msgCount % 1000) == 0)
        fflush(logFile);
}
#endif
void ProcessMTDescriptor_NEW(
    unsigned char MTModId,
    unsigned short desc,
    unsigned short pos,
    unsigned long *msgCount,
    struct timeval *prev_tv)
{
    MNTPACK buffer;
    char dir = 'R';
    unsigned short  temp, cmd, rtAddr, subAddr, wc_or_mode, trBit;
    unsigned long ptr;
    struct timeval tv;
    struct tm *t;

    /* Read descriptor header from descriptor slot 'pos' */
    unsigned long Offset = 0x21000 + ((pos * 4) - 4);
    buffer.BlockSts.value  = MemRead(MTModId, Offset, 1);
    buffer.TimeTag         = MemRead(MTModId, Offset + 1, 1);
    buffer.DataBlkPtr      = MemRead(MTModId, Offset + 2, 1);
    ptr                    = 0x20000 + buffer.DataBlkPtr;
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
    if (buffer.BlockSts.value == 0x0000 || buffer.RecCmdWrd.value == 0x0000)
    {
        return;
    }

    fprintf(MtlogFile, "\nTime [%02d:%02d:%02d.%06ld] Date [%02d-%02d-%04d] Msg#%lu \n",
            t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec,t->tm_mday,t->tm_mon + 1,t->tm_year + 1900,*msgCount);
    fprintf(MtlogFile, "BlkSts=0x%04X  Cmd=0x%04X\n",
            buffer.BlockSts.value, buffer.RecCmdWrd.value);

    *prev_tv = tv;

    /* Flags for clarity */
    int is_rt_to_rt = (buffer.BlockSts.value & 0x0800) ? 1 : 0;
    int no_resp     = (buffer.BlockSts.value & 0x0200) ? 1 : 0;

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
            fprintf(MtlogFile,
                    "Decoded RT->RT: %u-%u-X (No Response!)  2ndCmd=0x%04X TxRT=%u SA=%u TxStatus=0x%04X\n",
                    rtAddr, subAddr, secondCmd, rt_tx, sa_tx, txStatus);
        }
        else
        {
            if (wc_tx == 0) wc_tx = 32;

            fprintf(MtlogFile,
                    "Decoded: %u-%u-X-%u  2ndCmd=0x%04X TxRT=%u SA=%u TxStatus=0x%04X\n",
                    rtAddr, subAddr, wc_tx, secondCmd, rt_tx, sa_tx, txStatus);

            /* Move pointer to first data word for the second command */
            ptr += 2;

            /* Read data words for the second command */
            fprintf(MtlogFile, "Data (RT->RT second cmd):\n");

            for (int i = 0; i < (int)wc_tx; i++)
            {
                buffer.DataPack[i] = MemRead(MTModId, ptr++, 1);
                fprintf(MtlogFile, "%04X ", buffer.DataPack[i]);

                if ((i + 1) % 8 == 0)
                {
                    fprintf(MtlogFile, "\n");
                    fprintf(MtlogFile, "\n");
                }
            }

            fprintf(MtlogFile, "\n");
        }
    }
    else
    {
        /* Normal RT/BC case */
        if (no_resp)
        {
            fprintf(MtlogFile, "Decoded: %u-%u-%c-%u (No Response!)\n",
                    rtAddr, subAddr, dir, wc_or_mode);
        }
        else
        {
            fprintf(MtlogFile, "Decoded: %u-%u-%c-%u\n",
                    rtAddr, subAddr, dir, wc_or_mode);
        }

        temp = wc_or_mode;
        if (subAddr == 0)
        {
            if ((temp == 16) || (temp == 17) || (temp == 18))
                temp = 1; /* Mode command fix */
            else
                temp = 0;
        }

        /* If TrnRecv==1, hardware places an extra word (pointer needs advance) */
        if (buffer.RecCmdWrd.bitwise.TrnRecv == 1)
        {
            ptr++;

            if (!no_resp)
            {
                fprintf(MtlogFile, "Data:\n");

                for (int i = 0; i < (int)temp; i++)
                {
                    buffer.DataPack[i] = MemRead(MTModId, ptr++, 1);
                    fprintf(MtlogFile, "%04X ", buffer.DataPack[i]);

                    if ((i + 1) % 8 == 0)
                    {
                        fprintf(MtlogFile, "\n");
                    }
                }

                fprintf(MtlogFile, "\n");
            }
        }
        else
        {
            fprintf(MtlogFile, "Data:\n");

            if (buffer.RecCmdWrd.value == 0xF811)
                temp = 1; /* special-case */

            for (int i = 0; i < (int)temp; i++)
            {
                buffer.DataPack[i] = MemRead(MTModId, ptr++, 1);
                fprintf(MtlogFile, "%04X ", buffer.DataPack[i]);

                if ((i + 1) % 8 == 0)
                {
                    fprintf(MtlogFile, "\n");
                }
            }

            fprintf(MtlogFile, "\n");
        }
    }

    if ((*msgCount % 1000) == 0)
        fflush(MtlogFile);
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
    unsigned short  temp, cmd, rtAddr, subAddr, wc_or_mode, trBit;
    unsigned long ptr;
    struct timeval tv;
    struct tm *t;

    /* Read descriptor header from descriptor slot 'pos' */
    unsigned long Offset = 0x21000 + ((pos * 4) - 4);
    buffer.BlockSts.value  = MemRead(MTModId, Offset, 1);
    //printf("offset=%08x bsw=%04x\n",Offset,buffer.BlockSts.value);
    buffer.TimeTag         = MemRead(MTModId, Offset + 1, 1);
    buffer.DataBlkPtr      = MemRead(MTModId, Offset + 2, 1);
    ptr                    =0x20000 + buffer.DataBlkPtr;
    //printf("offset=%08x ptr=%04x\n",Offset,ptr);
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
   if (buffer.BlockSts.value == 0x0000 || buffer.RecCmdWrd.value == 0x0000)
   {  
     return;
   }

    fprintf(logFile, "\nTime [%02d:%02d:%02d.%06ld] Date [%02d-%02d-%04d] Msg#%lu \n",
            t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec,t->tm_mday,t->tm_mon + 1,t->tm_year + 1900,*msgCount);
    fprintf(logFile, "BlkSts=0x%04X  Cmd=0x%04X\n", buffer.BlockSts.value, buffer.RecCmdWrd.value);
    
   
    
    
    
    *prev_tv = tv;

    /* Flags for clarity */
    int is_rt_to_rt = (buffer.BlockSts.value & 0x0800) ? 1 : 0;
    int no_resp    = (buffer.BlockSts.value & 0x0200) ? 1 : 0;
   //printf("Msg#%lu\t",*msgCount);
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
                   // printf( "Decoded RT->RT: %u-%u-X (No Response!)  2ndCmd=0x%04X TxRT=%u SA=%u TxStatus=0x%04X\n",
                    //rtAddr, subAddr, secondCmd, rt_tx, sa_tx, txStatus);
                    
             
        }
        else
        {
            if (wc_tx == 0) wc_tx = 32;
            fprintf(logFile, "Decoded: %u-%u-X-%u  2ndCmd=0x%04X TxRT=%u SA=%u TxStatus=0x%04X\n",
                    rtAddr, subAddr, wc_tx, secondCmd, rt_tx, sa_tx, txStatus);
                   // printf( "Decoded: %u-%u-X-%u  2ndCmd=0x%04X TxRT=%u SA=%u TxStatus=0x%04X\n",
                   // rtAddr, subAddr, wc_tx, secondCmd, rt_tx, sa_tx, txStatus);
                   
            

            /* Move pointer to first data word for the second command */
            ptr += 2;
            /* Read data words for the second command */
            fprintf(logFile, "Data (RT->RT second cmd):\n");
            
            for (int i = 0; i < (int)wc_tx; i++)
            {
                buffer.DataPack[i] = MemRead(MTModId, ptr++, 1);
                fprintf(logFile, "%04X ", buffer.DataPack[i]);
                  //  printf("%04X ", buffer.DataPack[i]);
              
                if ((i + 1) % 8 == 0) 
                {
                    fprintf(logFile, "\n");
                    fprintf(logFile, "\n");
                    
                }
            }
            
            fprintf(logFile, "\n");
           
            //printf( "\n");
        }

        /* Done with RT->RT: do NOT fall through to generic data-read below */
    }
    else
    {
        /* Normal RT/BC case */
        if (no_resp)
        {
            fprintf(logFile, "Decoded: %u-%u-%c-%u (No Response!)\n", rtAddr, subAddr, dir, wc_or_mode);
          //  printf( "Decoded: %u-%u-%c-%u (No Response!)\n", rtAddr, subAddr, dir, wc_or_mode);
            
        }
        else
        {
            fprintf(logFile, "Decoded: %u-%u-%c-%u\n", rtAddr, subAddr, dir, wc_or_mode);
            //printf( "Decoded: %u-%u-%c-%u\n", rtAddr, subAddr, dir, wc_or_mode);
            
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
                    
                    //printf("%04X ", buffer.DataPack[i]);
                    if ((i + 1) % 8 == 0)
                    {
                     /*printf("\n");*/
                     fprintf(logFile, "\n");
                    
                    }
                    
                }
                fprintf(logFile, "\n");
                
            }
        }
        else
        {          
            fprintf(logFile, "Data:\n");
            
           // printf( "Data:\n");
            if (buffer.RecCmdWrd.value == 0xF811) temp = 1; /* special-case */
            for (int i = 0; i < (int)temp; i++)
            {
                buffer.DataPack[i] = MemRead(MTModId, ptr++, 1);
                fprintf(logFile, "%04X ", buffer.DataPack[i]);
                
                // printf( "%04X ", buffer.DataPack[i]);
                if ((i + 1) % 8 == 0) {fprintf(logFile, "\n");
               //  printf("\n");
               }
            }
            fprintf(logFile, "\n");
            
            //printf("\n");      
        }
    }    
    if ((*msgCount % 1000) == 0)
        fflush(logFile);
}


#if 0
void* StartMTThread(void* arg)
{
    unsigned char MTModId = *((unsigned char*)arg);
    unsigned short current, last = 0;
    unsigned long msgCount = 0;
    unsigned short currentDesc = 0, lastDesc = 0, rt_msgCount=0;
    unsigned short pos = 0;
    struct timeval prev_tv;
    
    signal(SIGINT, handle_sigint);
    gettimeofday(&prev_tv, NULL);
    lastDesc =RegRead(MTModId, 0x30, 1);                             //ReadMsgRtCounter(MTModId);
    while (ThreadRunFlag == 0)
    {
        current = RegRead(MTModId, 0x4E, 1) & 0x3FF;                 // 0–1023 only
        currentDesc=RegRead(MTModId, 0x30, 1);                       //ReadMsgRtCounter(MTModId);
        
        /*************************************RT LOG************************************/
        if (lastDesc != currentDesc)
        {

               ++rt_msgCount;
               RT_MSG rtMsg;
               ReadMsg1553(MTModId, pos, &rtMsg,rt_msgCount);
               lastDesc = currentDesc;

        }
        
        /*************************************MT LOG************************************/
        if (current != last)
        {
            unsigned short idx = last;

            while (idx != current)
            {
                ProcessMTDescriptor(MTModId, idx, pos, &msgCount, &prev_tv);
                msgCount++;
                idx = (idx + 1) & 0x3FF; //1023
                pos = (pos + 1) & 0x3FF;//1023
                //if (msgCount > 100000000) break; 
            }

            last = current;
        }

        usleep(1);   
    }

    printf("MT Reading Over. Total messages = %lu\n", msgCount);
    
    if(rt_msgCount)
    {
      printf("RT Reading Over. Total messages = %lu\n", rt_msgCount);
    }
    
    return NULL;
}
#endif
void* StartMTThread(void* arg)
{
    MTThreadArgs  *args             = (MTThreadArgs*)arg;
    unsigned char  MTModId          = args->BCModId;//MTModId
    unsigned long  expectedMessages = args->expectedMessages;

    unsigned short current, last;
    unsigned long  msgCount = 0;
    struct timeval prev_tv;
    gettimeofday(&prev_tv, NULL);
    int idlecnt = 0;

    last = GetMTEOMCnt_dummy(MTModId, CARD1);
    printf("MT Thread Started. Initial Ptr = %u\n",last);

    while (ThreadRunFlag == 0)
    {
        current = GetMTEOMCnt_dummy(MTModId, CARD1);

        if (current != last)
        {
            

            int diff;
            diff=(current - last + 1024) % 1024;


            if (diff > 1024)
            {
               // printf("WARNING: MT overrun! Skipping %d descriptors\n", diff);
                last = current;
                continue;
            }

            while (diff--)
            {
                ProcessMTDescriptor(MTModId, last, last, &msgCount, &prev_tv);
                msgCount++;
                last = (last + 1) % 1024;

                if (msgCount >= expectedMessages)
                {
                    //printf("All %lu messages received.\n", msgCount);
                    goto done;
                }
            }
            idlecnt = 0;
            
        }
        else
        {
           idlecnt++;
           if(bc_done && idlecnt>2000)
           {
              printf("bc done but only got %lu / %lu messages\n",msgCount,expectedMessages);
              break;
           }
    }
    usleep(1);
}
done:
    RegWrite(MTModId, 0x0049, 0x2, 1);
    ResetAce(MTModId);
    printf("MT Reading Over. Total MT messages = %lu\n", msgCount);
    return NULL;
}
#if 0
void* StartMTThread(void* arg)
{
    unsigned char MTModId = *((unsigned char*)arg);

    unsigned short current;
    unsigned short last;
    unsigned long  msgCount = 0;

    struct timeval prev_tv;
    gettimeofday(&prev_tv, NULL);
    int idlecnt=0;
    /* Read initial pointer */
    last = RegRead(MTModId, 0x4E, 1) & 0x3FF;

    printf("MT Thread Started. Initial Ptr = %u\n", last);
    
#if 1
    while (ThreadRunFlag == 0)
    {
    
        current = RegRead(MTModId, 0x4E, 1) & 0x3FF;
       // printf("Loop running... current=%u last=%u\n", current, last);
        /* If pointer wrapped */
        if (current != last)
        {
            int diff;
            
            if (current > last)
                diff = current - last;
            else
                diff = (1024 - last) + current;

            /* If too large jump, assume overflow and resync */
            if (diff > 1024)
            {
                printf("Resyncing MT pointer\n");
                last = current;
                continue;
            }

            while (diff--)
            {
                ProcessMTDescriptor(MTModId,
                                    last,
                                    last,
                                    &msgCount,
                                    &prev_tv);

                msgCount++;
                last = (last + 1) % 1024;
            }
        }
        else
    {
        idlecnt++;

        if (bc_done && idlecnt > 2000)
        {
            printf("BC done and MT idle → exiting\n");
            break;
        }
    }

        usleep(50);
    }
    #endif
    RegWrite(MTModId,0x0049,0x2,1);
    printf("MT Reading Over. Total MT messages = %lu\n", msgCount);
  
 
    return NULL;
}
#endif
#if 0
void* StartMTThread_NEW(void* arg)
{
    unsigned char MTModId = *((unsigned char*)arg);
    unsigned short current, last = 0;
    unsigned long msgCount = 0;
    unsigned short currentDesc = 0, lastDesc = 0, rt_msgCount=0;
    unsigned short pos = 0;
    struct timeval prev_tv;

    signal(SIGINT, handle_sigint);
    gettimeofday(&prev_tv, NULL);
    lastDesc =RegRead(MTModId, 0x30, 1);                             //ReadMsgRtCounter(MTModId);
    while (ThreadRunFlag == 0)
    {
        current = RegRead(MTModId, 0x4E, 1) & 0x3FF;                 // 0–1023 only
        currentDesc= RegRead(MTModId, 0x30, 1);                       //ReadMsgRtCounter(MTModId);
        
        /*************************************RT LOG************************************/
        if (lastDesc != currentDesc)
        {

               ++rt_msgCount;
               RT_MSG rtMsg;
               ReadMsg1553(MTModId, pos, &rtMsg,rt_msgCount);
               lastDesc = currentDesc;

        }
        
        
        /*************************************MT LOG************************************/
        if (current != last)
        {
            unsigned short idx = last;

            while (idx != current)
            {
                ProcessMTDescriptor_NEW(MTModId, idx, pos, &msgCount, &prev_tv);
                msgCount++;
                idx = (idx + 1) & 0x3FF;
                pos = (pos + 1) & 0x3FF;
                if (msgCount > 100000000) break; 
            }

            last = current;
        }

        usleep(150);   
    }

    printf("NEW MT Reading Over. Total messages = %lu\n", msgCount);
    
   
    {
      printf("RT Reading Over. Total messages = %lu\n", rt_msgCount);
    }
    
    return NULL;
}
#endif
void* StartMTThread_NEW(void* arg)
{
    unsigned char  MTModId = *(unsigned char*)arg;
    unsigned short current, last;
    unsigned long  msgCount = 0;
    struct timeval prev_tv;
    gettimeofday(&prev_tv, NULL);

    last = GetMTEOMCnt_dummy(MTModId, CARD1);
    printf("MT Thread Started. Initial Ptr = %u\n", last);
    printf("Waiting for external BC messages...\n");

    while (ThreadRunFlag == 0)   // ← no expectedMessages, runs until keypress
    {
        current = GetMTEOMCnt_dummy(MTModId, CARD1);
        if (current != last)
        {
            int diff;
            if (current > last)
                diff = current - last;
            else
                diff = (1024 - last) + current;

            if (diff > 512)
            {
                printf("Resyncing MT pointer\n");
                last = current;
                continue;
            }

            while (diff--)
            {
                ProcessMTDescriptor(MTModId, last, last, &msgCount, &prev_tv);
                msgCount++;
                last = (last + 1) % 1024;
            }
        }
        usleep(10);
    }

    RegWrite(MTModId, 0x0049, 0x2, 1);
    ResetAce(MTModId);
    printf("MT Reading Over. Total MT messages = %lu\n", msgCount);
    return NULL;
}
#if 0
void* StartMTThread(void* arg)
{
    unsigned char MTModId = *((unsigned char*)arg);
    unsigned short current = 0;
    unsigned short last = 0;
    unsigned long msgCount = 0;
    struct timeval prev_tv;

    signal(SIGINT, handle_sigint);
    gettimeofday(&prev_tv, NULL);

    printf("MT Thread Started...\n");

    while (ThreadRunFlag == 0)
    {
        // Read HW MT write pointer
        current = RegRead(MTModId, 0x4E, 1) & 0x3FF;   // 0–1023

        // Process all unread messages
        while (last != current)
        {
            ProcessMTDescriptor(MTModId, last, last, &msgCount, &prev_tv);
            msgCount++;

            last = (last + 1) & 0x3FF;   // circular buffer
        }

        // VERY SMALL DELAY (or remove fully)
        usleep(1);
    }

    printf("\nMT Reading Over. Total messages = %lu\n", msgCount);
    return NULL;
}
#endif

#if 0
void* StartMTThread(void* arg)
{
    unsigned char MTModId = *((unsigned char*)arg);
    unsigned short current = 0, last = 0, prev = 0;
    unsigned short pos = 0;
    unsigned short currentDesc = 0, lastDesc = 0, rt_msgCount=0;
    unsigned long msgCount = 0;
    struct timeval prev_tv;
    
    signal(SIGINT, handle_sigint); 
    
    last = 0;//RegRead(MTModId, 0x4e, 1);//((MemRead(MTModId, 0x20102, 1) - 0x1000) / 4) % 1024;
    lastDesc =RegRead(MTModId, 0x30, 1);//ReadMsgRtCounter(MTModId);
    printf("thread data 0x4e =%4x\n",RegRead(MTModId, 0x4e, 1)); 
    gettimeofday(&prev_tv, NULL);
    while (ThreadRunFlag == 0)
      { 
          current = RegRead(MTModId, 0x4e, 1);
          currentDesc=RegRead(MTModId, 0x30, 1);//ReadMsgRtCounter(MTModId);
          //printf("current =%04x\n",current);
          
          if (lastDesc != currentDesc)
          {
               ++rt_msgCount;
               RT_MSG rtMsg;
               ReadMsg1553(MTModId, pos, &rtMsg,rt_msgCount);
               lastDesc = currentDesc;
          }
    if (current == 65535)
    {
       //MemWrite(MTModId, 0x20102, 0x1000, 1);
        last = 0;                               // reset software pointer
        pos  = 0;
        continue;
    }
    
        if (last != current)
         {
           last = current  ; //& 0x3FF;   // %1024 faster
          pos  = (pos  + 1) % 1024;//& 0x3FF;

          ProcessMTDescriptor(MTModId, last, pos, &msgCount, &prev_tv);
          msgCount++;
          printf("last =%04x\n",last);printf("current =%04x\n",current);
         }
        // usleep(10);
      }

     printf("MT Reading Over. Total messages: %lu   rt reading over=%lu \n", (msgCount),rt_msgCount);  
    return 0;
}
#endif

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
  
    //lastDesc = RegRead(modId, 0x10, 1);
    lastDesc = ReadMsgRtCounter(modId);
    //MemRead(modId, 0x100, 1);

    while (ThreadRunFlag == 0)
    {
        //currentDesc = RegRead(modId, 0x30, 1);//
        currentDesc=ReadMsgRtCounter(modId);//
       // printf("rt reg read add 30=%08x\n",currentDesc);
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
int DefMajorFrame_new(UCHAR Bus,USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[])
{	
  unsigned int i,m,r=0,number_of_messages = 0,framecounter=0;
  unsigned int msg_address,temp;
  unsigned int msg_offset,msg_offset1;
  //USHORT Cnt_Time=0x0;
  
  if(Bus < 1 || Bus > 3)
      return (-2);

  if(no_of_minor_frames < 1 || no_of_minor_frames > 64)
	  return (-24);  
  for(m=0;m<no_of_minor_frames;m++)
  {	  
    /*Cnt_Time = frame_sequence[m].Mess_Count << 9;//<<9 APOORVA PCI
    Cnt_Time = Cnt_Time | (frame_sequence[m].MinorFrame_Time) ;
    RegWrite(Bus,0x61 + m,0x1414,1);//Cnt_Time
    //printf("Data %x Addres %x\n",RegRead(Bus,0x61+m,1),0x61+m);*/
     unsigned short Cnt_Time = 0;
     unsigned short msgCount = (unsigned short)frame_sequence[m].Mess_Count;
     unsigned short frameTime = (unsigned short)frame_sequence[m].MinorFrame_Time;
     Cnt_Time = (msgCount << 9) | (frameTime & 0x1FF);  
     printf("Frame[%d]: MsgCount=%d Time=%d Cnt_Time=0x%04x\n", m, msgCount, frameTime, Cnt_Time);
     RegWrite(Bus, 0x61 + m, Cnt_Time, 1);  // ← use Cnt_Time not 0x1414
    
  } 
  RegWrite(Bus,0x0d,no_of_minor_frames,1);
  printf("Address(0x0d) %x no_of_minor_frames %d\n",RegRead(Bus,0x0d,1),no_of_minor_frames);
   
  for (m=0; m<no_of_minor_frames; m++)
    number_of_messages = number_of_messages + frame_sequence[m].Mess_Count;
  if (number_of_messages <= 1024 && number_of_messages >= 1)	
  {      
     for (m=0; m<no_of_minor_frames; m++)
     {
       for (i=0; i<10; i++) //frame_sequence[m].Mess_Count
       {
          msg_address = 0x0000;
          msg_offset  = 0x0000;
          temp = (frame_sequence[m].frame_items[i]-1);
          msg_address = ((temp) * 40) + 0x4000;
          msg_offset  = 0x1000+((framecounter) * 4) + 0x0003;
          MemWrite(Bus,msg_offset,msg_address,1);
          framecounter++;
        //   printf("DefFrame_num %d msg_offset %x  Msg  %x\n",i,msg_offset,MemRead(Bus,msg_offset,1));
        }
      } 
    MemWrite(Bus,(msg_offset+0x1000)+4,END_OF_FRAME,1);
   // MemWrite(Bus, 0x1000 + (framecounter * 4), END_OF_FRAME, 1);
    if (number_of_messages >1024 || number_of_messages < 1)
     return(-24);
  }
  else
  return(-10);
  return 1;
	
}

unsigned long RunBcMajorFrame_new(UCHAR Bus,USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],unsigned long times)
{  
    if(Bus < 1 || Bus > 3)
	  return (-2);
    if(no_of_minor_frames < 1 || no_of_minor_frames > 64)
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
        //StartReset(Bus,START);
        RegWrite(Bus,0x1,0x54f,1);RegWrite(Bus,0x3,0x8000,1);
        RegWrite(Bus,0x9,0x2,1);
        printf("BC Frame Based Continuous Scheduling\n");
      }
      else
      {     
         // RegWrite(Bus,0x3,0x8000,1);
         
         MemWrite(Bus,CmdStack,init_pos,1);
         MemWrite(Bus,MESS_COUNTER,times,1);//MESS_COUNTER 0x101
       /*  RegWrite(Bus,0x16,0x1,1);  //ENABLING GENERAL PURPOSE REGISTER
         RegWrite(Bus,0x12,0xFFFF,1); //FIRST 16(MINOR FRAME)
        RegWrite(Bus,0x13,0x1,1);*/   //NEXT ONE (17)
        
         //1-FFFF(1,3,7,F,1F,3F,7F,FF)
        // printf("BC_STATE  %d \n",RegRead(Bus,0x11,1));  
         // StartReset(Bus,START);
         
    #if 0
          RegWrite(Bus,0x9,0x2,1);
          //count=MemRead(Bus,MESS_COUNTER,1);
          printf("RegRead 0x07 = %x\n",RegRead(Bus,0x07,1));
          printf("RegRead 0x01 = %x\n",RegRead(Bus,0x01,1));
          
          count=RegRead(Bus,0x1,1);

         while (count != 0)//0x44e Completed
          {
          
            count=RegRead(Bus,0x1,1);//0x44e Incomplete
            count = (count&2);//getchar();
        //   printf("Frame Based Scheduling In Progress  %d %x\n",RegRead(Bus,0x11,1),count);   
          /* printf("BC_STATE  %d \n",RegRead(Bus,0x11,1));
           printf("BC_HP_SPR  %x \n",RegRead(Bus,0x19,1));
           printf("BC_LP_SPR  %x \n",RegRead(Bus,0x1a,1));
           printf("BC_SPR  %x \n",RegRead(Bus,0x1b,1));*/
          }
          printf("\tFrame Based Scheduling Completed\n"); 
          //printf("Frame Based Scheduling In Progress  %x %x\n",RegRead(Bus,0x1,1),count);
          #endif
          
          // Replace the while loop with this:
          printf("RegRead 0x07 = %x\n", RegRead(Bus, 0x07, 1));
          printf("RegRead 0x01 = %x\n", RegRead(Bus, 0x01, 1));

          // Reset EOF counter before starting
          //SetBCEOFCnt(Bus, 0, CARD1);

          // Start BC
          RegWrite(Bus, 0x9, 0x2, 1);

          printf("Waiting for %lu End of Frames...\n", times);

          // Wait until EOF count reaches times
          unsigned long eofCount = 0;
          while(1)
          {
              eofCount = GetBCEOFCnt_D(Bus, CARD1);
              
              if(eofCount >= times)
              {
                  printf("BC EOF count reached: %lu / %lu\n", eofCount, times);
                  break;
              }
              
              // Safety exit - if BC stopped unexpectedly
              count = RegRead(Bus, 0x1, 1);
               if((count & 2) == 0)
                {
                    printf("BC stopped early. EOF: %lu / %lu\n", eofCount, times);
                    break;
                }
              
              usleep(100);  // small delay to avoid busy loop
          }

          printf("\tFrame Based Scheduling Completed\n");
          
      }	
    }
       
    return 1;
}
