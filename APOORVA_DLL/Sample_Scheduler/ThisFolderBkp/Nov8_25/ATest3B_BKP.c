	// mt_stanalone_test.cpp : Defines the entry point for the console application.
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
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
//// TODO: Provide equivalent header in Linux
// TODO: Replace with Linux-compatible header if needed
#ifndef TRUE
#define TRUE 1
#endif
#define BUS_A 1
#define BUS_B 0
#ifndef FALSE
#define FALSE 0
#endif
#define CARD1 1
#define RECEIVE 0
#define TRANSMIT 1
#define YES 1
#define NO 0
#define CH_A 0
#define CH_B 1
bool Card_Type = true;
#define   EXTERN   extern
#define EXTN extern
#define MESS_COUNTER 0x0101
#define BUFFER_SIZE 32
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
char chr;
UCHAR CardId=1,Mod=0,Bc1Mod=1,Rt1Mod=1,Mt1Mod=1,Saddr;
MSG_DEF bc_page,bc_sync,bc_page_msg[32];
MSG_RESULT	msg_return; 
RT_MSG RtBuff;
MNTPACK buffer;
MAJOR_FRAME major_frames[128];
unsigned short frame_items[128],data_buffer[40],temp_data,BCDataCounter=0;
unsigned short Temp_Buff[40],i,Counter=0;
int TempCnt=0,RtNo_G=0;;
ULONG m,temp,testNum=0;
void  ReadResult(int mCount);
SHORT RunBcNowait(UCHAR Bus,USHORT stackA_B,USHORT position,USHORT num_of_messages,ULONG times,UCHAR CARD);
void ConfigureRT(UCHAR rtModId, unsigned char rtAddr);
void clearRamMT(int MtMod);
void ModuleTest();
int BC_MOD=0,RT_MOD=0,MT_MOD=0;
void PrepareBCmessage(int BUS,int channel,int interGap,int Taddrs ,int SubAddr,int t_r,int Wcnt,USHORT data ,int MessageNum,int CARD);
int ThreadRunFlag=0;
void *BCThread_Bus(void *lpParameter);
void BCSchedule_Bus();
void* StartMTThread(void* arg);
void* RTThread_Bus(void* arg);
int freqR1,freqR2,freqR3,freqR4,CountResponse=0;
LARGE_INTEGER Freq1,Freq2,Freq3,Freq4,EndCounter,StartCounter,Freq;
long long diffC;
double diffT;
typedef long long LONGLONG;
pthread_mutex_t mutexObj = PTHREAD_MUTEX_INITIALIZER;
unsigned short txBuffer[BUFFER_SIZE];
/*=============================================*/
#define	MC_ADDR			0
#define	MRT_ADDR		2
#define	FSDU_RS_ADDR	3
#define	RADAR_ADDR		3 
#define	FSDU_RF_ADDR	4
#define	INS_ADDR		5
#define	SGU1_ADDR		5
#define VUHF_ADDR		5
#define IFFG_ADDR		6
#define SGU2_ADDR		6
#define MPRU_ADDR		7 
#define HMD_ADDR		7
#define VUHF2_ADDR		7 
#define FSDU_FS_ADDR	8
#define FSP_ADDR		9
#define FSDU_FF_ADDR	9
#define BIFU_ADDR		9
#define SSCDR_ADDR		10
#define CCU1_ADDR		11
#define CCU2_ADDR		11
#define LDP_ADDR		11
#define RADALT_ADDR		12
#define FADEC1_ADDR		13
#define FADEC2_ADDR		13
#define OAC2_ADDR		14
#define BHEEM1_ADDR		15
#define HEEM2_ADDR		15
#define BHEEM2_ADDR		15
#define ECFM1_ADDR		16
#define ECFM2_ADDR		16
#define VORILS_ADDR		17 	 
#define TACAN_ADDR		18 
#define DFCC11_ADDR		21
#define DFCC32_ADDR		21
#define DFCC21_ADDR		22
#define DFCC42_ADDR		22
#define VSU_ADDR		23
#define	RWR_ADDR		24
#define ECMR_ADDR		24
#define SIB1_ADDR		25
#define SIB2_ADDR		26
#define CMDS_ADDR		27 
#define BVR_ADDR		28	 

#define MT_20msec_COUNT	20000
# define STATUS 0xcccc
# define LOOP   0x0000
/*=============================================*/

#define	HALF_DAY_HR				12

#define MODULE_ID_0		1
#define MODULE_ID_1		2
#define MODULE_ID_2		3

typedef struct
{
	 UBIT16			bit		:1;
	 UBIT16			unused	:15;

}SIGN_BIT;

struct TIME
{
	UBIT16  hour;
	UBIT16  min;
	UBIT16  sec;
	SIGN_BIT time_offset_sign; 																
};
TIME  MCD_current_time;


typedef struct  
{
	UBIT16 hour   : 5  ;
	UBIT16 min    : 6  ;
	UBIT16 sec    : 6  ;
	UBIT16 millisec :10 ;
	UBIT16 unused : 6 ;

}SYSTEM_TIME_HUD;
SYSTEM_TIME_HUD system_time_hud;
unsigned short ASGI_Sys_time_millisec;

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

UBIT16 Mc1Cardno,Mc2Cardno,Mc3Cardno;
unsigned char Mc1Modno, Mc2Modno,Mc3Modno;
bool Mc1LruInitialised = FALSE, Mc2LruInitialised = FALSE, Mc3LruInitialised = FALSE;
unsigned short index1=0,index2=0,index3=0;
short MC_minor_cycle_number=-1,major_cycle=-1;
unsigned short Bus1_address[32],Bus1_sub_address[32],Bus1_wordcnt[32],Bus1_data[32][32],Bus1_poll[32];
unsigned short Bus2_address[32],Bus2_sub_address[32],Bus2_wordcnt[32],Bus2_data[32][32],Bus2_poll[32];
unsigned short Bus3_address[32],Bus3_sub_address[32],Bus3_wordcnt[32],Bus3_data[32][32],Bus3_poll[32];
unsigned short Pos1 = 0,Pos2 = 0,Pos3 = 0;
unsigned short M1_count=0,M2_count=0,M3_count=0;
UBIT16 data[32];

UBIT16 DBM_active_channel_selected_b1 = 1,DBM_active_channel_selected_b2 = 1,DBM_active_channel_selected_b3 = 1;
unsigned short frame_items1[100], frame_items2[100], frame_items3[100];
unsigned long MT1PrevCnt,TempCnt1=0,Lcnt1=1,MT2PrevCnt,TempCnt2=0,Lcnt2=1,MT3PrevCnt,TempCnt3=0,Lcnt3=1;

UCHAR Mt1Modno=1,Mt2Modno=2,Mt3Modno=3;
MNTPACK buffer1,buffer2,buffer3;
HANDLE hMutexObj;

UBIT16	  MT1Initialised,MT2Initialised ,MT3Initialised;
UBIT16		Mt1Cardno=1,Mt2Cardno=1,Mt3Cardno=1;

HANDLE hthread1,hthread2,hthread3,hthread4;

FILE *Fp1_1=NULL,*Fp2_1=NULL,*Fp3_1=NULL;
FILE *Fp1_B=NULL,*Fp2_B=NULL,*Fp3_B=NULL;

errno_t err1,err2,err3,Mterr1,Mterr2,Mterr3;
//struct timeb t_struct;
struct timeval t_struct;
int minor_cycle1=-1, minor_cycle2=-1,minor_cycle3=-1;
UCHAR RT_Address;
ULONG msg_count[4][32];
ULONG m_count[4];
FILE *fp1_rm;

/*************************************************/

void Find_minor_cycleno_and_mccount();
void  Send20msSynch(UBIT16  Module_no ,UBIT16 Card_no,UBIT16 Minorcycle_no ,UBIT16 Active_channel);
void Bus_Scheduel(void);
void B1553_Bus_Scheduling(void);
void Msg_counterRead(void);

void Bus_Data_Recv(void);
void InitCard_init(void);
UBIT16 IntOpenBC1(UBIT16 ModuleId ,UBIT16 CardId);
UBIT16 IntOpenBC2(UBIT16 ModuleId ,UBIT16 CardId);
UBIT16 IntOpenBC3(UBIT16 ModuleId ,UBIT16 CardId);
void IntSyncT(UBIT16 syncval,UBIT16 ModuleId,UBIT16 CardId);
void BroadcastMc1_12(void);
void Do_Ofp_Rt2Rt_MinorCycle0(void);
void Receive_Ofp_Data_MinorCycle0_bc(void);
void Send_Ofp_Data_MinorCycle0_bc(void);
void Do_Ofp_Rt2Rt_MinorCycle1(void);
void Receive_Ofp_Data_MinorCycle1_bc(void);
void Send_Ofp_Data_MinorCycle1_bc(void);
void Do_Ofp_Rt2Rt_MinorCycle2(void);
void Receive_Ofp_Data_MinorCycle2_bc(void);
void Send_Ofp_Data_MinorCycle2_bc(void);
void Do_Ofp_Rt2Rt_MinorCycle3(void);
void Receive_Ofp_Data_MinorCycle3_bc(void);
void Send_Ofp_Data_MinorCycle3_bc(void);
void Do_Ofp_Rt2Rt_MinorCycle4(void);
void Receive_Ofp_Data_MinorCycle4_bc(void);
void Send_Ofp_Data_MinorCycle4_bc(void);
void Do_Ofp_Rt2Rt_MinorCycle5(void);
void Receive_Ofp_Data_MinorCycle5_bc(void);
void Send_Ofp_Data_MinorCycle5_bc(void);
void Do_Ofp_Rt2Rt_MinorCycle6(void);
void Receive_Ofp_Data_MinorCycle6_bc(void);
void Send_Ofp_Data_MinorCycle6_bc(void);
void Do_Ofp_Rt2Rt_MinorCycle7(void); 
void Receive_Ofp_Data_MinorCycle7_bc(void);
void Send_Ofp_Data_MinorCycle7_bc(void);
void IntBCbcst(UBIT16 no_of_mess,UBIT16 *sub_add,UBIT16 buffer[][32],UBIT16 *cnt,UBIT16 bus ,UBIT16 ModuleId,UBIT16 CardId);
void IntBCrt2rt(UBIT16 no_of_mess,UBIT16 *dest_addr,UBIT16 *source_addr,UBIT16 *dest_sub_addr,UBIT16 *source_sub_addr,UBIT16 *cnt,UBIT16 Bus,UBIT16 ModuleId,UBIT16 CardId);
UBIT16 IntBCrec(UBIT16 no_of_mess,UBIT16 *address,UBIT16 *sub_add,UBIT16 buffer[32][32],UBIT16 *cnt,UBIT16 Bus ,UBIT16 ModuleId ,UBIT16 CardId);
UBIT16 IntBCsend(UBIT16 no_of_mess,UBIT16 *address,UBIT16 *sub_add,UBIT16 buffer[32][32],UBIT16 *cnt,UBIT16 Bus ,UBIT16 ModuleId,UBIT16 CardId);
void IntBCrt2rt1(UBIT16 no_of_mess,UBIT16 *dest_addr,UBIT16 *source_addr,UBIT16 *dest_sub_addr,UBIT16 *source_sub_addr,UBIT16 *cnt,UBIT16 Bus ,UBIT16 ModuleId,UBIT16 buffer[32][32],UBIT16 CardId);
void Receive_Data_MinorCycle0(void);
void Rt2Rt_Data_Receive_MinorCycle1(void);
void Receive_Data_MinorCycle1(void);
void Receive_Data_MinorCycle2(void);
void Receive_Data_MinorCycle3(void);
void Receive_Data_MinorCycle4(void);
void Receive_Data_MinorCycle5(void);
void Receive_Data_MinorCycle6(void);
void Receive_Data_MinorCycle7(void);
void Rt2Rt_Data_Receive_MinorCycle5(void);
void Rt2Rt_Data_Receive_MinorCycle6(void);
UBIT16 IntBCrec_recv(UBIT16 buffer[32][32],UBIT16 MCyc,UBIT16 ModuleId,UBIT16 CardId);

void ActivateMc(void);
void Sdef_Scheduler(void);
void DeActivateMT(void);
void ActivateMT(void);
void Init_RT(void);
void Init_MT(void);
void RUNALL_MT(void);
void BC_FILE(void);
UBIT16 ActivateMT1(UBIT16 ModuleId ,UBIT16 CardId);
UBIT16 ActivateMT2(UBIT16 ModuleId ,UBIT16 CardId);
UBIT16 ActivateMT3(UBIT16 ModuleId ,UBIT16 CardId);
void CloseMT1Module(void);
void CloseMT2Module(void);
void CloseMT3Module(void);
int chanNum = 0,TxchanNum=0,RxchanNum=0;
	UCHAR rxData[64] ;
	UCHAR txData[64] ;
unsigned int ChNo=0,addr=0,setting,bytcnt = 1;
int cnt=0;
#define MSG_COUNT 1024
FILE* logFile = NULL;
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
   MemWrite(ModId, CmdStack + 2 + ((msg_num - 1) * 4), IntMsgGap, 1);	
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
            buff_offset = DataStack + ((msg_num-1)*40)+3;
            MemWrite(ModId,buff_offset,def.cmd_1.status,1);
            buff_offset = DataStack + ((msg_num-1)*40)+4;
            MemWrite(ModId,buff_offset,data_buffer[0],1);              
          }

          else if (def.cmd_1.t_r == RECEIVE && (( def.cmd_1.wcnt == 20) || (def.cmd_1.wcnt == 17) || (def.cmd_1.wcnt == 21)) )
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
void enableNonCanonicalMode() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_lflag &= ~ECHO;
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

void handle_sigint(int sig) {
    printf("\nExiting...\n");
    if (logFile) fclose(logFile);
    exit(0);
}
void SelMsgMt1553(UCHAR ModId)
{	
  unsigned short i;	
  RegWrite(ModId,CONFIG_REG3,0x8000,1);
  RegWrite(ModId,CONFIG_REG3,0x8700,1);
  printf(" CONFIG_REG3 -> %x CONFIG_REG %x\n",RegRead(ModId, 0x07,1),RegRead(ModId, 0x01,1));
  RegWrite(ModId,CONFIG_REG,0x5000,1);
  MemWrite(ModId,0x102,0x1000,1);				
  MemWrite(ModId,0x103,0x4000,1);	
  for(i=0; i < 128; i++)
  {
	  MemWrite(ModId,(0x280+i),0xffff,1);
  }	
    return;
}
void ConfigureMt(unsigned char MTModId)
{
  ClearRam(MTModId);
  ResetAce(MTModId);  
  // Initialize descriptor pointer from hardware (0xB register)      
  RegWrite(MTModId, 0x3, 0x1, 1);                               
  RegWrite(MTModId, 0x3, 0x2, 1);
  RegWrite(MTModId, CONFIG_REG, 0x5000, 1);
  SelMsgMt1553(MTModId);
  RunMt(MTModId);
}
void* StartMTThread(void* arg)
{
    unsigned char MTModId = *((unsigned char*)arg);
    MNTPACK buffer;
    unsigned short ptr = 0, temp,msgNo=0,pos=0;
    unsigned short cmd = 0, rtAddr = 0, trBit = 0, subAddr = 0, wc_or_mode = 0;
    unsigned short current = 0, last = 0, prev = 0;
    unsigned long msgCount = 0;
    int diff = 0;
    struct timeval tv, prev_tv;
    struct tm* t;
    char dir = 'R'; // default direction

    signal(SIGINT, handle_sigint); // Ctrl+C handling  
    last = RegRead(MTModId, 0xB, 1);
    gettimeofday(&prev_tv, NULL); // initialize previous timestamp
    printf("Monitor thread running... startDesc=%04X\n", last);
    while (1)
    {
        current = RegRead(MTModId, 0xB, 1); // hardware descriptor pointer
       
        if (current != 0)
        {
            if (last != current)
            {
                last++;              
                diff = last - prev;                
            
                if (diff != 1/* && prevDesc != 0*/)
                {
                    fprintf(logFile, "Warning: Descriptor jump! prev=%04X last=%04X curr=%04X (diff=%d)\n",
                            prev, last, current, diff);
                }
                 if (msgCount % 2000 == 0)
                    printf("Message Count: %lu LastC %04x CurrentC %04x\n", msgCount,last,current);
                // ---- Read Block Header ----
                unsigned short Offset = 0x1000 + ((pos * 4) - 4);                
                
                // --- Read Descriptor ---
                buffer.BlockSts.value   = MemRead(MTModId, Offset,     1);
                buffer.TimeTag          = MemRead(MTModId, Offset + 1, 1);
                buffer.DataBlkPtr       = MemRead(MTModId, Offset + 2, 1);
                ptr                     = buffer.DataBlkPtr;
                buffer.RecCmdWrd.value  = MemRead(MTModId, Offset + 3, 1);               

                // ---- Timestamp and time difference ----
                gettimeofday(&tv, NULL);
                double delta_ms = (tv.tv_sec - prev_tv.tv_sec) * 1000.0 +
                                  (tv.tv_usec - prev_tv.tv_usec) / 1000.0;

                t = localtime(&tv.tv_sec);
                fprintf(logFile,"\nTime [%02d:%02d:%02d.%06ld] Msg#%lu LastC=%04X CurrentC=%04X pos=%d Offset=%04X \n",t->tm_hour, t->tm_min, t->tm_sec,
                        tv.tv_usec, msgCount, last,current,pos,Offset);
                printf("\nTime [%02d:%02d:%02d.%06ld] Msg#%lu LastC=%04X CurrentC=%04X pos=%d Offset=%04X \n",t->tm_hour, t->tm_min, t->tm_sec,
                        tv.tv_usec, msgCount, last,current,pos,Offset);
                
                prev_tv = tv; // update previous timestamp

                // ---- Log header ----
                fprintf(logFile,"BlkSts=0x%04X  TimeTag=0x%04X  Cmd=0x%04X\n",buffer.BlockSts.value, buffer.TimeTag, buffer.RecCmdWrd.value);

                // ---- Decode Command Word ----
                cmd        = buffer.RecCmdWrd.value;
                rtAddr     = (cmd >> 11) & 0x1F;
                trBit      = (cmd >> 10) & 0x01;
                subAddr    = (cmd >> 5)  & 0x1F;
                wc_or_mode = cmd & 0x1F;
                if (wc_or_mode == 0) wc_or_mode = 32;

                dir = (trBit == 1) ? 'T' : 'R';

                // ---- RT-to-RT Special Handling ----
                if (buffer.BlockSts.value & 0x0800)
                {
                    unsigned short secondCmd = MemRead(MTModId, ptr, 1);
                    unsigned short txStatus  = MemRead(MTModId, ptr + 1, 1);

                    unsigned short rt_tx  = (secondCmd >> 11) & 0x1F;
                    unsigned short sa_tx  = (secondCmd >> 5)  & 0x1F;
                    unsigned short wc_tx  = secondCmd & 0x1F;
                    if (wc_tx == 0) wc_tx = 32;

                    fprintf(logFile,
                            "Decoded: %u-%u-X-%u  2ndCmd=0x%04X TxRT=%u SA=%u TxStatus=0x%04X\n",
                            rtAddr, subAddr, wc_tx, secondCmd, rt_tx, sa_tx, txStatus);

                    ptr += 2;      // skip 2 words
                    temp = wc_tx;  // number of data words
                }
                else
                {                  
                   if (buffer.BlockSts.value & 0x0200) 
                   {  // If RT no-response bit
                        fprintf(logFile,"Decoded: %u-%u-%c-%u (No Response!)\n",rtAddr, subAddr, dir, wc_or_mode);
                   } 
                   else 
                   {
                      fprintf(logFile, "Decoded: %u-%u-%c-%u\n",rtAddr, subAddr, dir, wc_or_mode);
                   }

                    temp = wc_or_mode;
                }

                if (subAddr == 0) temp = 1; // Mode command fix
                
                if (buffer.RecCmdWrd.bitwise.TrnRecv == 1)
                {
                     ptr++;
                     if ((buffer.BlockSts.value & 0x0200)==0)
                     {
                        // ---- Read & Log Data ----
                        fprintf(logFile, "Data:\n");
                        char line[512] = {0};
                        int len = 0;
                        for (int i = 0; i < temp; i++)
                        {
                            buffer.DataPack[i] = MemRead(MTModId, ptr++, 1);
                             fprintf(logFile,"%04X ", buffer.DataPack[i]);
                             /*if(buffer.DataPack[i]!=0)
                               fprintf(logFile,"MsgN %d TxErr:[%d] Data[%04X] \n",lastDesc,i,buffer.DataPack[i]);*/
                            if ((i + 1) % 8 == 0)
                              fprintf(logFile, "\n");
                        }
                       fprintf(logFile, "\n");
                    }
                }
                else
                {              
                    fprintf(logFile, "Data:\n");
                    int len = 0;
                    for (int i = 0; i < temp; i++)
                    {
                      buffer.DataPack[i] = MemRead(MTModId, ptr++, 1);
                      fprintf(logFile,"%04X ", buffer.DataPack[i]);
                      if ((i + 1) % 8 == 0)
                        fprintf(logFile, "\n");                       
                    }
                    fprintf(logFile, "\n");                
                }
                // Flush periodically
                if ((msgCount % 1000) == 0)
                    fflush(logFile);

                prev = last;
                msgCount++;
                pos = (pos >= MSG_COUNT) ? 1 : pos + 1;        
            }
            /*else
            {
#if defined(__x86_64__) || defined(__i386__)
                _mm_pause();    // Intel/AMD pause (~10–50ns)
#else
                asm volatile("yield");  // ARM or other (no-op if unsupported)
#endif
            }*/
        }
    }
    return 0;
}

void main()
{
	MSG_DEF bc_page;
	int RtNo=0,subbAdd=0,trbit=0,opt=0,RtNo2=0,subbAdd2=0,wCOUNTER2=0,wCOUNTER1=0,timerResol=0,Wcnt=0,j=0,cnt=0;
	unsigned short data_buf[32],ResVal=0;
	UCHAR Address[31];
	int Ch=0,IntGap=10,timesToRun=1,RetryOption=0,i_subAdd=0;
	int Large=0,MessageNum=1,smal=0,a=1,b=20,rev=0,Present=0,Num=0,rtAddr=0,rtModId=2,mtModId=3;
        fd_set set;
        struct timeval timeout;
	logFile = fopen("mt_log.txt", "w");	
        //setvbuf(logFile, NULL, _IOFBF, 65536); 
        if (!logFile) {
            perror("Failed to open log file");
            return -1;
        }
	if(InitCard(CARD1))
	{	
		
		for(i=0;i<32;i++)
		{
			for(j=0;j<32;j++)
			{
				Bus1_data[i][j] = 0;
				Bus2_data[i][j] = 0;
				Bus3_data[i][j] = 0;
			}
			data[i] = 0;
		}
		while(!kbhit()) 
		{
			MessageNum=1;
		        //InitInterrupts(3,1);
		       // WriteMemA(3,1,4);  
		       // WritePort(0x0,(Val | ReadPort(0x0)));
		        //RegWrite(3,0x0000,0x0000,1);
		        
		       
			printf("\n\n\t***********APOORVA 1553B BC/RT/MT CARD ATP*************************\n");
			
			printf("\n\n\tSelect Option for ");
			printf("\n\t 1.BC & RT Mode with HAL Scheduler \n");
                        printf("\n\t 2.BC & MT Mode with HAL Scheduler \n");
                        printf("\n\t 3.RT & MT Mode \n");

			printf("\n\t to exit give zero (0)\t");
		
			scanf("%d",&testNum);
			if((testNum==1)||(testNum==3))
			{
			  printf("\n\tSelect Any RT Address Between 1 <-> 30) \n");
			  scanf("%d",&rtAddr);
			  rtModId=2;
			  mtModId=3;
			}
			
			switch (testNum)
			{
			case 1:
			      printf("\n\t Selected Option 1.BC & RT Mode \n");
			      ConfigureRT(rtModId, rtAddr);
			      enableNonCanonicalMode();
			      InitCard_init();
			      ActivateMc();
					
                                //pthread_create(&thread, NULL, BCThread_Bus, NULL); 
                                pthread_create(&thread, NULL, RTThread_Bus,  &rtModId); 
                                BCSchedule_Bus();
                                while (ThreadRunFlag == 0) 
                                {
                                      FD_ZERO(&set);
                                      FD_SET(STDIN_FILENO, &set);

                                      timeout.tv_sec = 0;
                                      timeout.tv_usec = 100000; // 100ms polling

                                      if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0) {
                                          char ch;
                                          read(STDIN_FILENO, &ch, 1);
                                          ResetAce(1);
                                          ResetAce(rtAddr);
                                          printf("Key pressed: exiting...\n");
                                          ThreadRunFlag = 1;
                                      }
                                  }                                                    
                                pthread_join(thread, NULL);
                                break;
			case 2:
			        enableNonCanonicalMode();
				printf("\n\t Selected Option 2.BC & MT Mode \n");
				ConfigureMt(mtModId);				
				InitCard_init();				
				ActivateMc();				
			        pthread_create(&mtThread, NULL, StartMTThread, &mtModId);
                               // pthread_create(&thread, NULL, BCThread_Bus, NULL);
                                BCSchedule_Bus();
                                
                                while (ThreadRunFlag == 0) 
                                {
                                      FD_ZERO(&set);
                                      FD_SET(STDIN_FILENO, &set);

                                      timeout.tv_sec = 0;
                                      timeout.tv_usec = 100000; // 100ms polling

                                      if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0) {
                                          char ch;
                                          read(STDIN_FILENO, &ch, 1);
                                          ResetAce(1);
                                          //ResetAce(mtModId);
                                          printf("Key pressed: exiting...\n");
                                          ThreadRunFlag = 1;
                                      }
                                  }
                                /*while (1) {
                                    sleep(1);
                                }     */                        
                                pthread_join(thread, NULL);
                                pthread_join(mtThread, NULL);
			      break;
			case 3:			
			        enableNonCanonicalMode();
			        printf("\n\tSelected Option 3.RT & MT mode \n");
			      	//ConfigureRT(rtModId, rtAddr);
			      	ConfigureMt(mtModId);
                                /* for(j=0;j<10;j++)
			         printf("Addres %04X, Data %04X \n",j,RegRead(mtModId,j,1));*/
			        
                                pthread_create(&mtThread, NULL, StartMTThread, &mtModId);
                                
                                while (ThreadRunFlag == 0) 
                                {
                                      FD_ZERO(&set);
                                      FD_SET(STDIN_FILENO, &set);

                                      timeout.tv_sec = 0;
                                      timeout.tv_usec = 100000; // 100ms polling

                                      if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0) {
                                          char ch;
                                          read(STDIN_FILENO, &ch, 1);
                                          ResetAce(rtModId);
                                          ResetAce(mtModId);
                                          printf("Key pressed: exiting...\n");
                                          ThreadRunFlag = 1;
                                      }
                                  }                                         
                                pthread_join(mtThread, NULL);
				break;
				
			default:
				printf("\n enter the valid test case num \n");
				break;
			}
		
			if(testNum==0)
			{
				printf("\n  exit test 0 \n");
				break;
			}
		}//while all
		printf("******************OVER*****************************\n");
		pthread_mutex_destroy(&mutexObj);
 		getchar();				
		//CloseDeviceHandle();//(CARD1);		
	}
	else
		printf("\n\tINVALID CARD ID OR CARD MAY NOT BE PRESENT\n\n");

	printf("\n entered return \n");	
	exit(0);
}
void ConfigureRT(UCHAR rtModId, unsigned char rtAddr)
{
//rtModId=3;
  int Sa=0;
 // Basic RT setup
    ClearRam(rtModId);
    ResetAce(rtModId);
    ResetDba(rtModId);
    ResetSvcReq(rtModId);
    ResetBusy(rtModId);
    ResetSsflag(rtModId);
    SetRtAddrM(rtModId, rtAddr, 1);  // Set RT address = 2
    DefLookUp(rtModId, LOOKUP_1);   
    // Enable RT mode
    DefMode(rtModId, RT);

   //SetTerFlag(modId);
   //ResetTerFlag(modId);
    //SetDba(modId);
    //SetSvcReq(modId);
    RegWrite(rtModId,CONFIG_REG3,0x8000,1);
    RegWrite(rtModId,CONFIG_REG3,0x8001,1);
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
      WriteDataBlock(rtModId, Sa, txBuffer);
    }
    MemWrite(rtModId,0x120,0xa5a5,1);
    RunRt(rtModId); 
}
void get_time_hud()
{	
	time_t ltime;
	//struct timeb tstruct;//struct _timeb tstruct;
	struct tm *today;
	//_tzset();
	time(&ltime);
	//ftime(&tstruct);//_ftime(&tstruct);
	
        struct timeval t_struct;
        gettimeofday(&t_struct, NULL);
	today = localtime(&ltime);
	
	if(today->tm_hour > HALF_DAY_HR)
	{
		today->tm_hour = today->tm_hour - HALF_DAY_HR;
	}
	
	system_time_hud.hour = today->tm_hour ;
        system_time_hud.min  = today->tm_min  ;
	system_time_hud.sec  = today->tm_sec  ;
	
	//system_time_hud.millisec  = tstruct.millitm;
	system_time_hud.millisec = t_struct.tv_usec / 1000;
        //system_time_hud.millisec  = t_struct.millitm;
	if(system_time_hud.millisec < 1000) 
		ASGI_Sys_time_millisec = system_time_hud.millisec;
	else
		ASGI_Sys_time_millisec = 999;
}
void InitCard_init(void) /*PC Based OAC*/
{
   Mc1Cardno = 1;
   Mc1Modno  = MODULE_ID_0;

   Mc2Cardno = 1;
   Mc2Modno  = MODULE_ID_1;

   Mc3Cardno = 1; 
   Mc3Modno  = MODULE_ID_2;
}
void ActivateMc(void)
{
  if(Mc1LruInitialised == FALSE) 
  {
	  Mc1LruInitialised = IntOpenBC1(Mc1Modno,Mc1Cardno);	
	  if(Mc1LruInitialised == TRUE)
	    printf(" BC1 INITIALISED ");   
  }
}

UBIT16 IntOpenBC1(UBIT16 ModuleId ,UBIT16 CardId)
{
          int RetVal=0;
	  RetVal = ClearRam(ModuleId);	 
	 // printf("Retval of ClearRam is :%d ModId %d\n",RetVal,ModuleId);
	  ResetAce(ModuleId); 
	  RetVal = DefMode(ModuleId,BC);
	  //printf("Retval of DefMode is :%x\n",RetVal);
    return TRUE;
}
void Find_minor_cycleno_and_mccount()
{
 	MC_minor_cycle_number++; 
	
 	if(MC_minor_cycle_number == 8)
	{
		MC_minor_cycle_number= 0;
		major_cycle++;
	}
}

void Send20msSynch(UBIT16  Module_no ,UBIT16 Card_no,UBIT16 Minorcycle_no ,UBIT16 Active_channel)
{
	UBIT16   Minor;
	Minor = Minorcycle_no;
	IntSyncT(Minor,Module_no,Card_no);
}

void IntSyncT(UBIT16 syncval,UBIT16 ModuleId,UBIT16 CardId)
{
	UBIT16 err;
	
	for(int i=0;i<32;i++)
		data[i] = 0x00;
         int RetVal=0;
	  
	bc_sync.bus           = BUS_A;
	bc_sync.self_test     = NO;
	bc_sync.mask_bcst_bit = NO;
	bc_sync.broadcast     = YES;
	bc_sync.rt_rt         = NO; 
	bc_sync.mode_code     = YES;
	bc_sync.cmd_1.taddr   = 31;
	bc_sync.cmd_1.t_r     = RECEIVE;
	bc_sync.cmd_1.subaddr = 0x00;
	bc_sync.cmd_1.wcnt    = 0x11;
	bc_sync.cmd_1.status  = 0xcccc;
	bc_sync.loop          = 0x0;

	data[0] = syncval<<12;
	
	//if(ModuleId == MODULE_ID_0)
	{
		Pos1++;		
	        err =DefMsg1553(1,Pos1,bc_sync,data,17,10);
		//printf("ModuleId %d Pos1 %d\n",ModuleId,Pos1);
	}

}
void BroadcastMc1_12(void)
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
    MC1_12_broadcast_mesg.sec_20ms.count_ms20 = (UBIT16)(ms / 20); // 0 to 49

    sub_addr[0] = 29;
    wordcnt[0] = 2;

    memcpy(data[0], &MC1_12_broadcast_mesg, sizeof(TIME_BROADCAST));
    IntBCbcst(1, sub_addr, data, wordcnt, 1, Mc1Modno, Mc1Cardno);
}

void IntBCbcst(UBIT16 no_of_mess,UBIT16 *sub_add,UBIT16 buffer[][32],UBIT16 *cnt,UBIT16 bus ,UBIT16 ModuleId,UBIT16 CardId)
{
	SBIT16 i;
	for(i=0;i<no_of_mess;i++)
	{			
		bc_page_msg[i].bus           = BUS_A;
		bc_page_msg[i].self_test     = NO;
		bc_page_msg[i].mask_bcst_bit = NO; 
		bc_page_msg[i].broadcast     = YES;
		bc_page_msg[i].rt_rt         = NO; 
		bc_page_msg[i].mode_code     = NO;
		bc_page_msg[i].cmd_1.taddr   = 31; 
		bc_page_msg[i].cmd_1.t_r     = RECEIVE;
		bc_page_msg[i].cmd_1.subaddr = sub_add[i];
		bc_page_msg[i].cmd_1.wcnt    = cnt[i];
		bc_page_msg[i].cmd_1.status  = 0xcccc;
		bc_page_msg[i].loop          = 0x0000;
	}	
	for(i=0;i<no_of_mess;i++)
	{			
		Pos1++;		
		DefMsg1553(1,Pos1,bc_page_msg[i],(UBIT16 *)&buffer[i][0],cnt[i],10);		
	}
	msg_count[0][0] += no_of_mess;
	
}

void Do_Ofp_Rt2Rt_MinorCycle0(void)
{
      UBIT16 Bus1_dest_addr[9],Bus1_src_addr[9],
      Bus1_dest_sub_addr[9],Bus1_src_sub_addr[9],
      Bus1_wordcnt[9]; 
      index1=0;	
      Bus1_src_addr[index1] =1 ; Bus1_src_sub_addr[index1] = 5; 
      Bus1_dest_addr[index1] = 2; Bus1_dest_sub_addr[index1] = 5;	Bus1_wordcnt[index1] = 2;
      index1++;

      Bus1_src_addr[index1] = 3;	Bus1_src_sub_addr[index1] = 8; 
      Bus1_dest_addr[index1] = 4; Bus1_dest_sub_addr[index1] = 8;	Bus1_wordcnt[index1] = 32;
      index1++;

      Bus1_src_addr[index1] = 5;	Bus1_src_sub_addr[index1] = 5; 
      Bus1_dest_addr[index1] = 6;	Bus1_dest_sub_addr[index1] = 9;		Bus1_wordcnt[index1] = 4;
      index1++;

      msg_count[0][0] += index1;
                IntBCrt2rt(index1,Bus1_dest_addr,Bus1_src_addr,Bus1_dest_sub_addr,Bus1_src_sub_addr,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);
	

	
}

void IntBCrt2rt(UBIT16 no_of_mess,UBIT16 *dest_addr,UBIT16 *source_addr,
			   UBIT16 *dest_sub_addr,UBIT16 *source_sub_addr,UBIT16 *cnt,UBIT16 Bus ,UBIT16 ModuleId,UBIT16 CardId )
{
	SBIT16 i,err;
	UBIT16 buffer[15][32];

	for(i=0;i<15;i++)
	{
		for(int j=0;j<32;j++)
			buffer[i][j]=0;
	}
		
	if(no_of_mess > 0)
	{
		for(i=0;i<no_of_mess;i++)
		{
			/*SelectModule(3,1);
			for(UCHAR p=0;p<cnt[i];p++)
				data_buffer[p] = 0x9999;
			WriteDataBlock(source_addr[i],source_sub_addr[i],data_buffer,CARD1);*/
			//WriteDataBlock(source_sub_addr[i],data_buffer);

			//SelectModule(2,1);
			bc_page_msg[i].bus           = BUS_A/*Bus*/;//change
			bc_page_msg[i].self_test     = NO;
			bc_page_msg[i].mask_bcst_bit = NO;
			bc_page_msg[i].broadcast     = NO; /*Gbroadcast;*/
			bc_page_msg[i].mode_code     = NO;
			bc_page_msg[i].rt_rt         = YES;
			bc_page_msg[i].cmd_1.taddr   = dest_addr[i];
			bc_page_msg[i].cmd_1.t_r     = RECEIVE;
			bc_page_msg[i].cmd_1.status  = STATUS;
			bc_page_msg[i].loop          = 0x0000/*LOOP*/;
			bc_page_msg[i].cmd_1.subaddr = dest_sub_addr[i];
			bc_page_msg[i].cmd_1.wcnt    = cnt[i];
		
			bc_page_msg[i].cmd_2.taddr   = source_addr[i];
			bc_page_msg[i].cmd_2.t_r     = TRANSMIT;
			bc_page_msg[i].cmd_2.status  = STATUS;
			bc_page_msg[i].loop          = 0x0000;
			bc_page_msg[i].cmd_2.subaddr = source_sub_addr[i];
			bc_page_msg[i].cmd_2.wcnt    = cnt[i];
			
			Pos1++;			
			err =DefMsg1553(1,Pos1,bc_page_msg[i],(UBIT16 *)&buffer[i][0],cnt[i],10);
			
		}
	}
}

void Receive_Ofp_Data_MinorCycle0_bc(void)
{
	index1=0;index2=0;index3=0;
	int k=0;

	if( Mc1LruInitialised  == TRUE )
	{
	        Bus1_address[index1]= 21 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 26 ;
	        index1++;
	        Bus1_address[index1]= 22 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 26 ;
	        index1++;
		Bus1_address[index1]= 11 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 14 ;
		index1++;
		Bus1_address[index1]= 5 ; Bus1_sub_address[index1] = 2 ;Bus1_wordcnt[index1] = 4 ;
		index1++;
		Bus1_address[index1]= 26 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 24 ;
		index1++;
		Bus1_address[index1] = 26;  Bus1_sub_address[index1] =  5;  Bus1_wordcnt[index1] =  1;
		index1++;
		Bus1_address[index1] = 7; Bus1_sub_address[index1] = 1; Bus1_wordcnt[index1] = 7;
		index1++;
		Bus1_address[index1] = 26; Bus1_sub_address[index1] = 10; Bus1_wordcnt[index1] = 32;
		index1++;
		Bus1_address[index1] = 26; Bus1_sub_address[index1] = 2; Bus1_wordcnt[index1] = 11;
		index1++;
		Bus1_address[index1]= 5 ; Bus1_sub_address[index1] = 11 ;Bus1_wordcnt[index1] = 4 ;
		index1++;
		Bus1_address[index1]= 7 ; Bus1_sub_address[index1] = 11 ;Bus1_wordcnt[index1] = 4 ;
		index1++;
		Bus1_address[index1] = 26; Bus1_sub_address[index1] =  18; Bus1_wordcnt[index1] = 10;
		index1++;
		Bus1_address[index1] = 26; Bus1_sub_address[index1] =  19; Bus1_wordcnt[index1] = 10;	
		index1++;
		
		msg_count[0][0] += index1;
		IntBCrec(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1 ,Mc1Modno,Mc1Cardno);
		index1=0;
	}	
}

UBIT16 IntBCrec(UBIT16 no_of_mess,UBIT16 *address,UBIT16 *sub_add,UBIT16 buffer[32][32],UBIT16 *cnt,UBIT16 Bus ,UBIT16 ModuleId ,UBIT16 CardId)
{
	SBIT16 err;
	UBIT16 i; 
	
	if(no_of_mess != 0)
	{
		for(i=0;i<no_of_mess;i++)
		{
			/*SelectModule(3,1);
			for(UCHAR p=0;p<cnt[i];p++)
				data_buffer[p] = 0x4444;
			//WriteDataBlock(sub_add[i],data_buffer);
			WriteDataBlock( address[i],sub_add[i],data_buffer,CARD1);*/

			//SelectModule(2,1);
			bc_page_msg[i].bus           = BUS_A;//change
			bc_page_msg[i].self_test     = NO;
			bc_page_msg[i].mask_bcst_bit = NO;
			bc_page_msg[i].broadcast     = NO; 
			bc_page_msg[i].rt_rt         = NO; 
			bc_page_msg[i].mode_code     = NO;
			bc_page_msg[i].cmd_1.taddr   = address[i];
			bc_page_msg[i].cmd_1.t_r     = TRANSMIT;
			bc_page_msg[i].cmd_1.subaddr = sub_add[i];
			bc_page_msg[i].cmd_1.wcnt    = cnt[i];
			bc_page_msg[i].cmd_1.status  = 0xcccc;
			bc_page_msg[i].loop          = 0x0000;

			for(int k=0;k<cnt[i];k++)
				buffer[i][k] = 0x0000;			

			Pos1++;
			//SelectModule(MODULE_ID_0,CARD1);
			err = DefMsg1553(1,Pos1,bc_page_msg[i],(UBIT16 *)&buffer[i][0],cnt[i],10);
			
		}
		return(msg_return.bsw_bits.err_flag);
	}
	else	return 0;
}

void Send_Ofp_Data_MinorCycle0_bc(void)
{
	index1=0;index2=0;index3 =0;
	int k=0;

	if( Mc1LruInitialised  == TRUE )
	{
		Bus1_address[index1]= 25 ; Bus1_sub_address[index1] = 16 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		Bus1_address[index1]= 21 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 30 ;
		index1++;
		Bus1_address[index1]= 22 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 30 ;
		index1++;
		Bus1_address[index1]= 25 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 24 ;
		index1++;
		Bus1_address[index1] = 25;  Bus1_sub_address[index1] =  1;  Bus1_wordcnt[index1] =  10;
		index1++;
		Bus1_address[index1] = 25; Bus1_sub_address[index1] = 3; Bus1_wordcnt[index1] = 11;
		index1++;
		Bus1_address[index1] = 25; Bus1_sub_address[index1] = 2; Bus1_wordcnt[index1] = 5;
		index1++;
		Bus1_address[index1] = 25; Bus1_sub_address[index1] = 12; Bus1_wordcnt[index1] = 25;
		index1++;
		Bus1_address[index1]= 25 ; Bus1_sub_address[index1] = 12 ;Bus1_wordcnt[index1] = 14 ;
		index1++;
		Bus1_address[index1]= 25 ; Bus1_sub_address[index1] = 9 ;Bus1_wordcnt[index1] = 10 ;
		index1++;
		Bus1_address[index1] = 15; Bus1_sub_address[index1] =  6; Bus1_wordcnt[index1] = 4;
		index1++;
		Bus1_address[index1] = 25; Bus1_sub_address[index1] =  14; Bus1_wordcnt[index1] = 14;
		index1++;
		Bus1_address[index1] = 15; Bus1_sub_address[index1] =  2; Bus1_wordcnt[index1] = 24;
		index1++;
		Bus1_address[index1] = 7; Bus1_sub_address[index1] =  2; Bus1_wordcnt[index1] = 29;
		index1++;
		Bus1_address[index1] = 5; Bus1_sub_address[index1] =  1; Bus1_wordcnt[index1] = 8;
		index1++;
		Bus1_address[index1] = 10; Bus1_sub_address[index1] =  7; Bus1_wordcnt[index1] = 1;
		index1++;
		Bus1_address[index1] = 22; Bus1_sub_address[index1] =  2; Bus1_wordcnt[index1] = 21;
		index1++;
		Bus1_address[index1] = 26; Bus1_sub_address[index1] =  15; Bus1_wordcnt[index1] = 14;
		index1++;
		Bus1_address[index1] = 7; Bus1_sub_address[index1] =  1; Bus1_wordcnt[index1] = 17;
		index1++;
		Bus1_address[index1] = 10; Bus1_sub_address[index1] =  1; Bus1_wordcnt[index1] = 16;
		index1++;
		Bus1_address[index1] = 25; Bus1_sub_address[index1] =  7; Bus1_wordcnt[index1] = 16;
		index1++;
		Bus1_address[index1] = 25; Bus1_sub_address[index1] =  26; Bus1_wordcnt[index1] = 16;
		index1++;
		Bus1_address[index1] = 26; Bus1_sub_address[index1] =  5; Bus1_wordcnt[index1] = 1;
		index1++;
		msg_count[0][0] += index1;
		IntBCsend(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);	
		
	}	
}

UBIT16 IntBCsend(UBIT16 no_of_mess,UBIT16 *address,UBIT16 *sub_add,UBIT16 buffer[32][32],UBIT16 *cnt,UBIT16 Bus ,UBIT16 ModuleId,UBIT16 CardId)
{
	SBIT16 err;
	UBIT16 i,data[32];

	for(i=0;i < no_of_mess;i++)
	{
		bc_page_msg[i].bus           = BUS_A;    
		bc_page_msg[i].self_test     = NO;//Gself_test;
		bc_page_msg[i].mask_bcst_bit = NO;//Gmask_bcst_bit;
		bc_page_msg[i].broadcast     = NO; /*Gbroadcast;*/
		bc_page_msg[i].rt_rt         = NO; /*Grt_rt;*/
		bc_page_msg[i].mode_code     = NO;
		bc_page_msg[i].cmd_1.taddr   = address[i];
		bc_page_msg[i].cmd_1.t_r     = RECEIVE;
		bc_page_msg[i].cmd_1.subaddr = sub_add[i];
		bc_page_msg[i].cmd_1.wcnt    = cnt[i];
		bc_page_msg[i].cmd_1.status  = STATUS;
		bc_page_msg[i].loop          = LOOP;		
		
		for(int k=0;k<cnt[i];k++)
			data[k] = BCDataCounter++;
			/*if((bc_page_msg[i].cmd_1.taddr==25)&&(bc_page_msg[i].cmd_1.subaddr==16))
			  printf("RTADDDRESS %d SAddress %d\n",bc_page_msg[i].cmd_1.taddr,bc_page_msg[i].cmd_1.subaddr);*/
		  /*  if(BCDataCounter>=0xffff)
		      BCDataCounter = 0x0000;*/
		
		/*if((address[i]==11)&&(sub_add[i]==1))
		{
			for(int k=0;k<cnt[i];k++)
				buffer[i][k] = Counter++;
		}
		else
		{
			for(int k=0;k<cnt[i];k++)
				buffer[i][k] = ((i+1) << 8) | (i+1);
		}*/
		
		Pos1++;
		//SelectModule(MODULE_ID_0,CARD1);
		err =DefMsg1553(1,Pos1,bc_page_msg[i],/*(UBIT16 *)&buffer[i][0]*/data,cnt[i],10);
		
	}
	return(msg_return.bsw_bits.err_flag);
}

void Do_Ofp_Rt2Rt_MinorCycle1(void)
{
	UBIT16 Bus1_dest_addr[13],Bus1_src_addr[13],
		Bus1_dest_sub_addr[13],Bus1_src_sub_addr[13],
		Bus1_wordcnt[13];

	index1=0; index2=0;index3 =0;

	//if( Mc1LruInitialised  == TRUE )
	{
		Bus1_src_addr[index1] = 15;	Bus1_src_sub_addr[index1] = 5; 
		Bus1_dest_addr[index1] = 13;	Bus1_dest_sub_addr[index1] = 9; Bus1_wordcnt[index1] = 4;
		index1++;
		Bus1_src_addr[index1] = 13;	Bus1_src_sub_addr[index1] = 13; 
		Bus1_dest_addr[index1] = 15;	Bus1_dest_sub_addr[index1] = 3; Bus1_wordcnt[index1] = 24;
		index1++;
		msg_count[0][1] += index1;
		IntBCrt2rt(index1,Bus1_dest_addr,Bus1_src_addr,Bus1_dest_sub_addr,Bus1_src_sub_addr,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);		
	}	
}

void IntBCrt2rt1(UBIT16 no_of_mess,UBIT16 *dest_addr,UBIT16 *source_addr,
			   UBIT16 *dest_sub_addr,UBIT16 *source_sub_addr,UBIT16 *cnt,UBIT16 Bus ,UBIT16 ModuleId,UBIT16 buffer[32][32],UBIT16 CardId)
{
	SBIT16 i,err;
	
	for(i=0;i<32;i++)
	{
		for(int j=0;j<32;j++)
			buffer[i][j]=0;
	}

	for(i=0;i<no_of_mess;i++)
	{
		/*SelectModule(3,1);
		for(UCHAR p=0;p<cnt[i];p++)
			data_buffer[p] = 0x5555;
		//WriteDataBlock(source_sub_addr[i],data_buffer);
		WriteDataBlock(source_addr[i],source_sub_addr[i],data_buffer,CARD1);*/

		//SelectModule(2,1);
		bc_page_msg[i].bus           = BUS_A/*Bus*/;
		bc_page_msg[i].self_test     = NO;
		bc_page_msg[i].mask_bcst_bit = NO;
		bc_page_msg[i].broadcast     = NO; 
		bc_page_msg[i].mode_code     = NO;
		bc_page_msg[i].rt_rt         = YES;
		bc_page_msg[i].cmd_1.taddr   = dest_addr[i];
		bc_page_msg[i].cmd_1.t_r     = RECEIVE;
		bc_page_msg[i].cmd_1.status  = 0xcccc/*STATUS*/;
		bc_page_msg[i].loop          = 0x0000/*LOOP*/;
		bc_page_msg[i].cmd_1.subaddr = dest_sub_addr[i];
		bc_page_msg[i].cmd_1.wcnt    = cnt[i];
			
		bc_page_msg[i].cmd_2.taddr   = source_addr[i];
		bc_page_msg[i].cmd_2.t_r     = TRANSMIT;
		bc_page_msg[i].cmd_2.status  = 0xcccc/*STATUS*/;
		bc_page_msg[i].loop          = 0x0000/*LOOP*/;
		bc_page_msg[i].cmd_2.subaddr = source_sub_addr[i];
		bc_page_msg[i].cmd_2.wcnt    = cnt[i];
		
		Pos1++;			
		err =DefMsg1553(1,Pos1,bc_page_msg[i],(UBIT16 *)&buffer[i][0],cnt[i],10);
		
	}
}

void Receive_Ofp_Data_MinorCycle1_bc(void)
{
	int k=0;
	index1=0;index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_address[index1]= 21 ; Bus1_sub_address[index1] = 6 ;Bus1_wordcnt[index1] = 1 ;
		index1++;
		Bus1_address[index1]= 22 ; Bus1_sub_address[index1] = 6 ;Bus1_wordcnt[index1] = 1 ;
		index1++;
	        Bus1_address[index1]= 11 ; Bus1_sub_address[index1] = 3 ;Bus1_wordcnt[index1] = 2 ;
	        index1++;
		Bus1_address[index1]= 16 ; Bus1_sub_address[index1] = 2 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		Bus1_address[index1]= 15 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 19 ;
		index1++;
		Bus1_address[index1]= 6 ; Bus1_sub_address[index1] = 15 ;Bus1_wordcnt[index1] = 26 ;
		index1++;
		Bus1_address[index1] = 15;  Bus1_sub_address[index1] =  11;  Bus1_wordcnt[index1] =  9;
		index1++;
		Bus1_address[index1] = 25;  Bus1_sub_address[index1] =  11;  Bus1_wordcnt[index1] =  27;
		index1++;
		Bus1_address[index1] = 26;  Bus1_sub_address[index1] =  11;  Bus1_wordcnt[index1] =  27;
		index1++;
		Bus1_address[index1] = 6;  Bus1_sub_address[index1] =  11;  Bus1_wordcnt[index1] =  4;
		index1++;
		Bus1_address[index1] = 10;  Bus1_sub_address[index1] =  11;  Bus1_wordcnt[index1] =  4;
		index1++;
		Bus1_address[index1] = 26;  Bus1_sub_address[index1] =  8;  Bus1_wordcnt[index1] =  32;
		index1++;
		Bus1_address[index1] = 21;  Bus1_sub_address[index1] =  8;  Bus1_wordcnt[index1] =  2;
		index1++;
		msg_count[0][1] += index1;
		IntBCrec(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1 ,Mc1Modno,Mc1Cardno);
		index1=0;
	}	
}

void Send_Ofp_Data_MinorCycle1_bc(void)
{
	int k=0;
	index1=0;index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{		
		Bus1_address[index1]= 25 ; Bus1_sub_address[index1] = 16 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		Bus1_address[index1]= 21 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 30 ;
		index1++;
		Bus1_address[index1]= 22 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 30 ;
		index1++;
		Bus1_address[index1]= 25 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 24 ;
		index1++;
		Bus1_address[index1] = 25;  Bus1_sub_address[index1] =  1;  Bus1_wordcnt[index1] =  10;
		index1++;
		Bus1_address[index1] = 16; Bus1_sub_address[index1] = 3; Bus1_wordcnt[index1] = 5;
		index1++;		
		Bus1_address[index1] = 15; Bus1_sub_address[index1] = 1; Bus1_wordcnt[index1] = 24;
		index1++;
		Bus1_address[index1] = 6; Bus1_sub_address[index1] = 8; Bus1_wordcnt[index1] = 27;
		index1++;
		Bus1_address[index1]= 7 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		Bus1_address[index1]= 26 ; Bus1_sub_address[index1] = 11 ;Bus1_wordcnt[index1] = 6 ;
		index1++;
		Bus1_address[index1] = 6; Bus1_sub_address[index1] =  10; Bus1_wordcnt[index1] = 10;
		index1++;
		Bus1_address[index1] = 13; Bus1_sub_address[index1] =  7; Bus1_wordcnt[index1] = 12; 
		index1++;
		Bus1_address[index1] = 26; Bus1_sub_address[index1] =  8; Bus1_wordcnt[index1] = 27;
		index1++;
		Bus1_address[index1] = 26; Bus1_sub_address[index1] =  10; Bus1_wordcnt[index1] = 27;
		index1++;
		Bus1_address[index1] = 22; Bus1_sub_address[index1] =  2; Bus1_wordcnt[index1] = 21;
		index1++;
		msg_count[0][1] += index1;
		      IntBCsend(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);

		Bus1_poll[0]=RADALT_ADDR;
		Bus1_poll[1]=FADEC1_ADDR;/*VORILS_ADDR;*/
	}	
}

void Do_Ofp_Rt2Rt_MinorCycle2(void)
{
	UBIT16 Bus1_dest_addr[5],Bus1_src_addr[5],
		Bus1_dest_sub_addr[5],Bus1_src_sub_addr[5],
		Bus1_wordcnt[5]; 

	index1=0; 

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_src_addr[index1] =15 ;	Bus1_src_sub_addr[index1] = 6; 
		Bus1_dest_addr[index1] = 2;	Bus1_dest_sub_addr[index1] = 11; Bus1_wordcnt[index1] = 19;
		index1++;
		msg_count[0][2] += index1;
		IntBCrt2rt(index1,Bus1_dest_addr,Bus1_src_addr,Bus1_dest_sub_addr,Bus1_src_sub_addr,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);
 
	}	
}

void Receive_Ofp_Data_MinorCycle2_bc(void)
{
	int k=0;
	index1=0;index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_address[index1]= 16 ; Bus1_sub_address[index1] = 25 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		Bus1_address[index1]= 7 ; Bus1_sub_address[index1] = 2 ;Bus1_wordcnt[index1] = 3 ;
		index1++;
	        Bus1_address[index1]= 22 ; Bus1_sub_address[index1] = 7 ;Bus1_wordcnt[index1] = 21 ;
	        index1++;
	        Bus1_address[index1]= 21 ; Bus1_sub_address[index1] = 7 ;Bus1_wordcnt[index1] = 21 ;
	        index1++;
		msg_count[0][2] += index1;
		IntBCrec(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1 ,Mc1Modno,Mc1Cardno);
		index1=0;
	}		
}

void Send_Ofp_Data_MinorCycle2_bc(void)
{
	int k=0;
	index1=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_address[index1]= 11 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 5 ;
		index1++;
		Bus1_address[index1]= 26 ; Bus1_sub_address[index1] = 6 ;Bus1_wordcnt[index1] = 11 ;
		index1++;
		Bus1_address[index1]= 7 ; Bus1_sub_address[index1] = 3 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		Bus1_address[index1]= 16 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 9 ;
		index1++;
		Bus1_address[index1] = 10;  Bus1_sub_address[index1] =  4;  Bus1_wordcnt[index1] =  5;
		index1++;
		Bus1_address[index1] = 26; Bus1_sub_address[index1] = 17; Bus1_wordcnt[index1] = 30;
		index1++;
		Bus1_address[index1] = 26; Bus1_sub_address[index1] = 18; Bus1_wordcnt[index1] = 25;	
		index1++;
		msg_count[0][2] += index1;
		IntBCsend(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);
		
	}
}

void Do_Ofp_Rt2Rt_MinorCycle3(void)
{
	UBIT16 Bus1_dest_addr[13],Bus1_src_addr[13],
		Bus1_dest_sub_addr[13],Bus1_src_sub_addr[13],
		Bus1_wordcnt[13];	
	index1=0; 
	//if(Mc1LruInitialised  == TRUE)
	{
		Bus1_src_addr[index1] = 15;	Bus1_src_sub_addr[index1] = 10;
		Bus1_dest_addr[index1] = 2;	Bus1_dest_sub_addr[index1] = 10; Bus1_wordcnt[index1] = 32;
		index1++;
		msg_count[0][3] += index1;
		IntBCrt2rt(index1,Bus1_dest_addr,Bus1_src_addr,Bus1_dest_sub_addr,Bus1_src_sub_addr,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);
	}	
}

void Receive_Ofp_Data_MinorCycle3_bc(void)
{
	int k=0;
	index1=0 ,index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_address[index1]= 16 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
	        Bus1_address[index1]= 26 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 32 ;
	        index1++;
		Bus1_address[index1]= 7 ; Bus1_sub_address[index1] = 3 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		Bus1_address[index1]= 25 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		Bus1_address[index1]= 25 ; Bus1_sub_address[index1] = 3 ;Bus1_wordcnt[index1] = 19 ;
		index1++;
		Bus1_address[index1] = 11;  Bus1_sub_address[index1] =  11;  Bus1_wordcnt[index1] =  4;
		index1++;
		Bus1_address[index1] = 25; Bus1_sub_address[index1] = 9; Bus1_wordcnt[index1] = 32;
		index1++;
		Bus1_address[index1] = 16; Bus1_sub_address[index1] = 11; Bus1_wordcnt[index1] = 17;
		index1++;
		
		msg_count[0][3] += index1;
		IntBCrec(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1 ,Mc1Modno,Mc1Cardno);
		index1=0;
	}	
}

void Send_Ofp_Data_MinorCycle3_bc(void)
{
	int k=0;
	index1=0;index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_address[index1]= 6 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 17 ;
		index1++;
		Bus1_address[index1]= 10 ; Bus1_sub_address[index1] = 5 ;Bus1_wordcnt[index1] = 9 ;
		index1++;
		Bus1_address[index1]= 7 ; Bus1_sub_address[index1] = 6 ;Bus1_wordcnt[index1] = 30 ;
		index1++;
		Bus1_address[index1]= 7 ; Bus1_sub_address[index1] = 8 ;Bus1_wordcnt[index1] = 30 ;
		index1++;
		Bus1_address[index1] = 6;  Bus1_sub_address[index1] =  10;  Bus1_wordcnt[index1] =  10;
		index1++;
		msg_count[0][3] += index1;
		IntBCsend(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);
		Bus1_poll[0]=VORILS_ADDR;/*CCU1_ADDR;*/
	}	
}

void Do_Ofp_Rt2Rt_MinorCycle4(void)
{
	UBIT16 Bus1_dest_addr[13],Bus1_src_addr[13],
		Bus1_dest_sub_addr[13],Bus1_src_sub_addr[13],
		Bus1_wordcnt[13];  

	UBIT16 Bus3_dest_addr[9],Bus3_src_addr[9],
		Bus3_dest_sub_addr[9],Bus3_src_sub_addr[9],
		Bus3_wordcnt[9]; 

	index1=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_src_addr[index1] =5 ;	Bus1_src_sub_addr[index1] = 5;
		Bus1_dest_addr[index1] = 7;	Bus1_dest_sub_addr[index1] = 5; Bus1_wordcnt[index1] = 2;
		index1++;
		Bus1_src_addr[index1] = 8;	Bus1_src_sub_addr[index1] = 5;
		Bus1_dest_addr[index1] = 9;	Bus1_dest_sub_addr[index1] = 9; Bus1_wordcnt[index1] = 4;
		index1++;
		Bus1_src_addr[index1] = 10;	Bus1_src_sub_addr[index1] = 10;
		Bus1_dest_addr[index1] = 11;		Bus1_dest_sub_addr[index1] = 10; Bus1_wordcnt[index1] = 32;
		index1++;
		msg_count[0][4] += index1;
		IntBCrt2rt(index1,Bus1_dest_addr,Bus1_src_addr,Bus1_dest_sub_addr,Bus1_src_sub_addr,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);
	}	
}

void Receive_Ofp_Data_MinorCycle4_bc(void)
{
	int k=0;
	index1=0,index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_address[index1]= 21 ; Bus1_sub_address[index1] = 5 ;Bus1_wordcnt[index1] = 18 ;
		index1++;
		Bus1_address[index1]= 22 ; Bus1_sub_address[index1] = 5 ;Bus1_wordcnt[index1] = 18 ;
		index1++;
	        Bus1_address[index1]= 15 ; Bus1_sub_address[index1] = 20 ;Bus1_wordcnt[index1] = 32 ;
	        index1++;
		Bus1_address[index1]= 15 ; Bus1_sub_address[index1] = 21 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		Bus1_address[index1]= 26 ; Bus1_sub_address[index1] = 6 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		Bus1_address[index1]= 13 ; Bus1_sub_address[index1] = 11 ;Bus1_wordcnt[index1] = 4 ;
		index1++;
		Bus1_address[index1] = 26;  Bus1_sub_address[index1] =  7;  Bus1_wordcnt[index1] =  16;
		index1++;
		Bus1_address[index1] = 26; Bus1_sub_address[index1] = 9; Bus1_wordcnt[index1] = 16;
		index1++;	
		msg_count[0][4] += index1;
		IntBCrec(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1 ,Mc1Modno,Mc1Cardno);
		index1=0;
	}  	
}

void Send_Ofp_Data_MinorCycle4_bc(void)
{
	int k=0;
	index1=0;index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_address[index1]= 6 ; Bus1_sub_address[index1] = 2 ;Bus1_wordcnt[index1] = 30 ;
		index1++;
		Bus1_address[index1]= 5 ; Bus1_sub_address[index1] = 30 ;Bus1_wordcnt[index1] = 1 ;
		index1++;
		Bus1_address[index1]= 6 ; Bus1_sub_address[index1] = 9 ;Bus1_wordcnt[index1] = 17 ;
		index1++;
		Bus1_address[index1]= 10 ; Bus1_sub_address[index1] = 6 ;Bus1_wordcnt[index1] = 10 ;		
		index1++;
		msg_count[0][4] += index1;
		IntBCsend(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);
	}	
}

void Do_Ofp_Rt2Rt_MinorCycle5(void)
{
	UBIT16 Bus1_dest_addr[5],Bus1_src_addr[5],
		Bus1_dest_sub_addr[5],Bus1_src_sub_addr[5],
		Bus1_wordcnt[5]; 

	index1=0;

	//if(Mc1LruInitialised  == TRUE)
	{
		Bus1_src_addr[index1] = 15;	Bus1_src_sub_addr[index1] = 8;
		Bus1_dest_addr[index1] = 2;		Bus1_dest_sub_addr[index1] = 8; Bus1_wordcnt[index1] = 32;
	        index1++;	IntBCrt2rt(index1,Bus1_dest_addr,Bus1_src_addr,Bus1_dest_sub_addr,Bus1_src_sub_addr,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);   
		
	}	
}

void Receive_Ofp_Data_MinorCycle5_bc(void)
{
	int k=0;
	index1=0;index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_address[index1]= 22 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 7 ;
		index1++;
		Bus1_address[index1]= 6 ; Bus1_sub_address[index1] = 16 ;Bus1_wordcnt[index1] = 22 ;
		index1++;
		msg_count[0][5] += index1;
		IntBCrec(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1 ,Mc1Modno,Mc1Cardno);
		index1=0;
	}	
}

void Send_Ofp_Data_MinorCycle5_bc(void)
{
	int k=0;
	index1=0;index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_address[index1]= 16 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 2 ;
		index1++;
		Bus1_address[index1]= 16 ; Bus1_sub_address[index1] = 2 ;Bus1_wordcnt[index1] = 13 ;
		index1++;
		Bus1_address[index1]= 6 ; Bus1_sub_address[index1] = 15 ;Bus1_wordcnt[index1] = 12 ;
		index1++;
		Bus1_address[index1]= 10 ; Bus1_sub_address[index1] = 3 ;Bus1_wordcnt[index1] = 14 ;
		index1++;
		
		msg_count[0][5] += index1;
		IntBCsend(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);

		Bus1_poll[0]=TACAN_ADDR;/*CMDS_ADDR;*/
	}

}

void Do_Ofp_Rt2Rt_MinorCycle6(void)
{
	UBIT16 Bus1_dest_addr[5],Bus1_src_addr[5],
		Bus1_dest_sub_addr[5],Bus1_src_sub_addr[5],
		Bus1_wordcnt[5];  
	index1=0;

	//if(Mc1LruInitialised  == TRUE)
	{
		Bus1_src_addr[index1] = 13 ;	Bus1_src_sub_addr[index1] = 15;
		Bus1_dest_addr[index1] = 15;	Bus1_dest_sub_addr[index1] = 5; Bus1_wordcnt[index1] = 25;
		index1++;
		IntBCrt2rt(index1,Bus1_dest_addr,Bus1_src_addr,Bus1_dest_sub_addr,Bus1_src_sub_addr,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);
		msg_count[0][6] += index1;
	}
	

}
void Receive_Ofp_Data_MinorCycle6_bc(void)
{
	int k=0;
	index1=0;index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_address[index1]= 5 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 6 ;
		index1++;
		Bus1_address[index1]= 7 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 7 ;
		index1++;
	        Bus1_address[index1]= 15 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 20 ;
	        index1++;
		Bus1_address[index1]= 6 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 16 ;
		index1++;
		Bus1_address[index1]= 7 ; Bus1_sub_address[index1] = 5 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		Bus1_address[index1]= 16 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 20 ;
		index1++;
		Bus1_address[index1] = 16;  Bus1_sub_address[index1] =  26;  Bus1_wordcnt[index1] =  32;
		index1++;
		Bus1_address[index1] = 10; Bus1_sub_address[index1] = 4; Bus1_wordcnt[index1] = 7;
		index1++;

		msg_count[0][6] += index1;
		IntBCrec(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1 ,Mc1Modno,Mc1Cardno);
		index1=0;
	}	
}

void Send_Ofp_Data_MinorCycle6_bc(void)
{
	int k=0;
	index1=0;index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_address[index1]= 15 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 25 ;
		index1++;
		Bus1_address[index1]= 25 ; Bus1_sub_address[index1] = 9 ;Bus1_wordcnt[index1] = 10 ;
		index1++;
		Bus1_address[index1]= 26 ; Bus1_sub_address[index1] = 2 ;Bus1_wordcnt[index1] = 5 ;
		index1++;
		msg_count[0][6] += index1;
		IntBCsend(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);

		Bus1_poll[0]=DFCC11_ADDR;
		Bus1_poll[1]=DFCC21_ADDR;
		Bus1_poll[2]=CMDS_ADDR;
	}	
}

void Do_Ofp_Rt2Rt_MinorCycle7(void)
{
	UBIT16 Bus1_dest_addr[13],Bus1_src_addr[13],
		Bus1_dest_sub_addr[13],Bus1_src_sub_addr[13],
		Bus1_wordcnt[13];  

	index1=0;
	//if(Mc1LruInitialised  == TRUE)
	{
		Bus1_src_addr[index1] = 21;	Bus1_src_sub_addr[index1] = 2;
		Bus1_dest_addr[index1] = 22;	Bus1_dest_sub_addr[index1] = 15; Bus1_wordcnt[index1] = 13;
		index1++;
		Bus1_src_addr[index1] = 22;	Bus1_src_sub_addr[index1] = 2;
		Bus1_dest_addr[index1] = 2;		Bus1_dest_sub_addr[index1] = 16; Bus1_wordcnt[index1] = 13;
		index1++;
		msg_count[0][7] += index1;
		IntBCrt2rt(index1,Bus1_dest_addr,Bus1_src_addr,Bus1_dest_sub_addr,Bus1_src_sub_addr,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);
	}	
}

void Receive_Ofp_Data_MinorCycle7_bc(void)
{
	int k=0;
	index1=0;index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{                
		Bus1_address[index1]= 6 ; Bus1_sub_address[index1] = 4 ;Bus1_wordcnt[index1] = 9 ;
		index1++;
		Bus1_address[index1]= 5 ; Bus1_sub_address[index1] = 1 ;Bus1_wordcnt[index1] = 7 ;
		index1++;
	        Bus1_address[index1]= 6 ; Bus1_sub_address[index1] = 17 ;Bus1_wordcnt[index1] = 6 ;
	        index1++;
		Bus1_address[index1]= 7 ; Bus1_sub_address[index1] = 6 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		Bus1_address[index1]= 21 ; Bus1_sub_address[index1] = 3 ;Bus1_wordcnt[index1] = 12 ;
		index1++;
		Bus1_address[index1] = 22;  Bus1_sub_address[index1] =  4;  Bus1_wordcnt[index1] =  12;
		index1++;
		Bus1_address[index1]= 26 ; Bus1_sub_address[index1] = 14 ;Bus1_wordcnt[index1] = 1 ;
		index1++;
		Bus1_address[index1]= 15 ; Bus1_sub_address[index1] = 3 ;Bus1_wordcnt[index1] = 20 ;
		index1++;
		Bus1_address[index1] = 16;  Bus1_sub_address[index1] =  3;  Bus1_wordcnt[index1] =  21;
		index1++;
		Bus1_address[index1] = 26;  Bus1_sub_address[index1] =  10;  Bus1_wordcnt[index1] =  16;
		index1++;
		msg_count[0][7] += index1;
		IntBCrec(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1 ,Mc1Modno,Mc1Cardno);
		index1=0;
	}	
}	

void Send_Ofp_Data_MinorCycle7_bc(void)
{
	int k=0;
	index1=0;index2=0;index3=0;

	if(Mc1LruInitialised  == TRUE)
	{
		Bus1_address[index1]= 21 ; Bus1_sub_address[index1] = 6 ;Bus1_wordcnt[index1] = 2 ;
		index1++;
		Bus1_address[index1]= 22 ; Bus1_sub_address[index1] = 6 ;Bus1_wordcnt[index1] = 2 ;		
		index1++;
		Bus1_address[index1]= 10 ; Bus1_sub_address[index1] = 10 ;Bus1_wordcnt[index1] = 10 ;
		index1++;
		Bus1_address[index1]= 5 ; Bus1_sub_address[index1] = 23 ;Bus1_wordcnt[index1] = 4 ;
		index1++;
		Bus1_address[index1]= 10 ; Bus1_sub_address[index1] = 2 ;Bus1_wordcnt[index1] = 27 ;
		index1++;
		Bus1_address[index1]= 7 ; Bus1_sub_address[index1] = 10 ;Bus1_wordcnt[index1] = 14 ;
		index1++;
		Bus1_address[index1]= 7 ; Bus1_sub_address[index1] = 7 ;Bus1_wordcnt[index1] = 30 ;
		index1++;
		Bus1_address[index1]= 5 ; Bus1_sub_address[index1] = 2 ;Bus1_wordcnt[index1] = 32 ;
		index1++;
		
		msg_count[0][7] += index1;
		IntBCsend(index1,Bus1_address,Bus1_sub_address,Bus1_data,Bus1_wordcnt,DBM_active_channel_selected_b1,Mc1Modno,Mc1Cardno);

		Bus1_poll[0]=FSDU_FF_ADDR;
		Bus1_poll[1]=OAC2_ADDR;
	}	
}

void B1553_Bus_Scheduling(void)
{
	unsigned int CPos=0,cyc=0,count;
	
	USHORT minor_cycle = MC_minor_cycle_number+1;
	if(Mc1LruInitialised == TRUE)
	{		
		for(CPos=1;CPos<=Pos1;CPos++)
		  frame_items[CPos-1] = CPos;
		DefFrame(1,Pos1,frame_items);
	}	
	if(Mc1LruInitialised == TRUE)
	{ 
		//SelectModule(0,CARD1);		
		RunBc(1,STACK_1,1,Pos1,1);
		//printf("RunBc..\n");
	}	
}
void Bus_Data_Recv(void)
{
	switch(MC_minor_cycle_number)
	{
	case 0 :
		Receive_Data_MinorCycle0();
		break;
	case 1 :
		Receive_Data_MinorCycle1();
		break;
	case 2 :
		Receive_Data_MinorCycle2();
		break;
	case 3 :
		Receive_Data_MinorCycle3();
		break;
	case 4 :
		Receive_Data_MinorCycle4();
		break;				
	case 5 :
		Receive_Data_MinorCycle5();
		break;
	case 6 :
		Receive_Data_MinorCycle6();
		break;	
	case 7 :
		Receive_Data_MinorCycle7();
		break;
	default :break;
	}
}
UBIT16 IntBCrec_recv(UBIT16 buffer[32][32],UBIT16 MCyc,UBIT16 ModuleId,UBIT16 CardId )
{
	SBIT16 err;
	UBIT16 i ,TempCnt,j=0,k=0;
	//if(ModuleId == MODULE_ID_0)
	{
		for(j = 1;j <= msg_count[0][MCyc];j++)
		{
			//SelectModule(MODULE_ID_0,CARD1);
			err=ReadMsgResult(STACK_1,j,&msg_return,CARD1);
			fprintf(Fp1_B,"\n%d\t\t%04x\t\t%04x\t\t%04x\t\t%04x\t",j,msg_return.cmd_1,msg_return.cmd_2,msg_return.bsw,msg_return.status_1);//msg_return.time_tag);
			TempCnt= msg_return.cmd1_bits.wcnt;
			if(TempCnt == 0)
				TempCnt = 32;

			for(i=0;i<TempCnt;i++)
			{
				buffer[j][i] = msg_return.data[i];
				fprintf(Fp1_B,"%04x\t",buffer[j][i]);
			}
		}
	}
	return(msg_return.bsw_bits.err_flag);
}

void Receive_Data_MinorCycle0()
{
	if(Mc1LruInitialised == TRUE)	
	{
		IntBCrec_recv(Bus1_data,0,Mc1Modno,Mc1Cardno);
	}
	if(Mc2LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus2_data,0,Mc2Modno,Mc2Cardno);
	}
	if(Mc3LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus3_data,0,Mc3Modno,Mc3Cardno);
	}
}
void Receive_Data_MinorCycle1()
{
	if(Mc1LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus1_data,1,Mc1Modno,Mc1Cardno);
	}
	if(Mc2LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus2_data,1,Mc2Modno,Mc2Cardno);
	}
	/*if(Mc3LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus3_data,1,Mc3Modno,Mc3Cardno);
	}*/
}

void Receive_Data_MinorCycle2()
{
	if(Mc1LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus1_data,2,Mc1Modno,Mc1Cardno);
	}
	if(Mc2LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus2_data,2,Mc2Modno,Mc2Cardno);
	}
	/*if(Mc3LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus3_data,2,Mc3Modno,Mc3Cardno);
	}*/
}

void Receive_Data_MinorCycle3()
{
	if(Mc1LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus1_data,3,Mc1Modno,Mc1Cardno);
	}
	if(Mc2LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus2_data,3,Mc2Modno,Mc2Cardno);
	}
	/*if(Mc3LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus3_data,3,Mc3Modno,Mc3Cardno);
	}*/
}

void Receive_Data_MinorCycle4()
{
	if(Mc1LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus1_data,4,Mc1Modno,Mc1Cardno);
	}
	if(Mc2LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus2_data,4,Mc2Modno,Mc2Cardno);
	}
	/*if(Mc3LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus3_data,4,Mc3Modno,Mc3Cardno);
	}*/
}

void Receive_Data_MinorCycle5()
{
	if(Mc1LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus1_data,5,Mc1Modno,Mc1Cardno);
	}
	if(Mc2LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus2_data,5,Mc2Modno,Mc2Cardno);
	}
	/*if(Mc3LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus3_data,5,Mc3Modno,Mc3Cardno);
	}*/
}

void Receive_Data_MinorCycle6()
{
	if(Mc1LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus1_data,6,Mc1Modno,Mc1Cardno);
	}
	if(Mc2LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus2_data,6,Mc2Modno,Mc2Cardno);
	}
	/*if(Mc3LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus3_data,6,Mc3Modno,Mc3Cardno);
	}*/
}

void Receive_Data_MinorCycle7()
{
	if(Mc1LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus1_data,7,Mc1Modno,Mc1Cardno);
	}
	if(Mc2LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus2_data,7,Mc2Modno,Mc2Cardno);
	}
	/*if(Mc3LruInitialised == TRUE)
	{
		IntBCrec_recv(Bus3_data,7,Mc3Modno,Mc3Cardno);
	}*/
}
void BCSchedule_Bus()
{
    struct timeval start, stop;
    long long llTimeDiff = 0,counter=0;
    int globalIncre=0,PreC=0;
    double dftDuration = 0.0;
    int prevPos = 0;  
    printf("Bus thread Start...\n");
	 for(MC_minor_cycle_number=0;MC_minor_cycle_number<8;MC_minor_cycle_number++)
	 {	  
	    //Find_minor_cycleno_and_mccount();  
	    Send20msSynch((UCHAR)Mc1Modno,CARD1,MC_minor_cycle_number, 1);
	    //msg_count[0][MC_minor_cycle_number] = Pos1;
	    
	    switch(MC_minor_cycle_number)
            {
              case 0 :	       
	              BroadcastMc1_12();		
	              //Do_Ofp_Rt2Rt_MinorCycle0();		
	              Send_Ofp_Data_MinorCycle0_bc();
	              Receive_Ofp_Data_MinorCycle0_bc();
	              break;
              case 1 :
	              Receive_Ofp_Data_MinorCycle1_bc(); 
	              Do_Ofp_Rt2Rt_MinorCycle1();
	              Send_Ofp_Data_MinorCycle1_bc();
	              break;
              case 2 :
	              Do_Ofp_Rt2Rt_MinorCycle2();
	              Receive_Ofp_Data_MinorCycle2_bc();
	              Send_Ofp_Data_MinorCycle2_bc();			
	              break;
              case 3 :
	              Do_Ofp_Rt2Rt_MinorCycle3();
	              Receive_Ofp_Data_MinorCycle3_bc();
	              Send_Ofp_Data_MinorCycle3_bc();
	              break;  
              case 4 :
	              //Do_Ofp_Rt2Rt_MinorCycle4();
	              Receive_Ofp_Data_MinorCycle4_bc();
	              Send_Ofp_Data_MinorCycle4_bc();
	              break;				
              case 5 :
	              Do_Ofp_Rt2Rt_MinorCycle5();
	              Receive_Ofp_Data_MinorCycle5_bc();
	              Send_Ofp_Data_MinorCycle5_bc();
	              break;				
              case 6 :
	              Do_Ofp_Rt2Rt_MinorCycle6();
	              Receive_Ofp_Data_MinorCycle6_bc();
	              Send_Ofp_Data_MinorCycle6_bc();
	              break;	
              case 7 :
	              Do_Ofp_Rt2Rt_MinorCycle7(); 
	              Receive_Ofp_Data_MinorCycle7_bc();
	              Send_Ofp_Data_MinorCycle7_bc();
	              break;
              default :break;
              }           
            
               int msg_count = Pos1 - prevPos;  // number of messages for this minor cycle
               int idx;

                for (idx = 0; idx < msg_count; idx++)
                {
                    major_frames[MC_minor_cycle_number].frame_items[idx] = prevPos + idx + 1;
                }

                major_frames[MC_minor_cycle_number].Mess_Count      = msg_count;
                major_frames[MC_minor_cycle_number].MinorFrame_Time = 20;

                printf("MC[%d]: MsgCount=%d, FrameItems:[%d..%d]\n",
                       MC_minor_cycle_number,
                       msg_count,
                       prevPos + 1,
                       Pos1);
                prevPos = Pos1;              
           }	
  
	DefMajorFrame(1, 0, 8, major_frames);
        //PriorityHighLow(module_id, 2, 1);
        printf("BC Major Frame Started Wait........! \n");
        RunBcMajorFrame(1, 0, 8, major_frames, 1);
        getchar();
        printf("BC Major Frame Running........! \n");
        getchar();  
        ResetAce(1);
        printf("BC Reset ........! \n");
}
void* RTThread_Bus(void* arg)
{
    unsigned char modId = *((unsigned char*)arg);
   int ret, i, msgIdx,RtAdres=0,Sa=0,TimeB=0;
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
      while(ThreadRunFlag==0)
      {
        currentDesc = MemRead(modId, 0x100,1);//(((MemRead(modId, 0x0,1))-0x1000)/4);//MtMsgCnt(MTModId);
         //if (lastDesc != currentDesc) 
          {
            while (lastDesc != currentDesc ) 
            {
                ReadMsg(modId, pos, &rtMsg);
                if(rtMsg.cmd==0xfba2)
                {
                  TimeB++;
                  unsigned short dynamic_data[32],idx=0;
                  for (idx = 0; idx < 2; idx++)
                      dynamic_data[idx] = TimeB + idx; /* some dynamic pattern */
                  UpdateMsgData(1, 2, dynamic_data, 2, RECEIVE);
                }
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
                /* printf("Data     :\n\t");

                for (i = 0; i < wordCount; ++i) {
                    //fprintf(logFile,"%04X ", rtMsg.data[i]);
                    printf("%04X ", rtMsg.data[i]);
                    if ((i + 1) % 8 == 0) //fprintf(logFile,"\n\t");
                    printf("\n\t");
                }*/
               
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
#if 0
void *BCThread_Bus(void *lpParameter)
{
    struct timeval start, stop;
    long long llTimeDiff = 0,counter=0;
    double dftDuration = 0.0;
    printf("Bus thread Start...\n");
	 for(MC_minor_cycle_number=0;MC_minor_cycle_number<8;MC_minor_cycle_number++)
	 {	  
	    //Find_minor_cycleno_and_mccount();  
	    Send20msSynch((UCHAR)Mc1Modno,CARD1,MC_minor_cycle_number, 1);
	    //msg_count[0][MC_minor_cycle_number] = Pos1;
	    
	    switch(MC_minor_cycle_number)
            {
              case 0 :	       
	              BroadcastMc1_12();		
	              //Do_Ofp_Rt2Rt_MinorCycle0();		
	              Send_Ofp_Data_MinorCycle0_bc();
	              Receive_Ofp_Data_MinorCycle0_bc();
	              break;
              case 1 :
	              Receive_Ofp_Data_MinorCycle1_bc(); 
	              Do_Ofp_Rt2Rt_MinorCycle1();
	              Send_Ofp_Data_MinorCycle1_bc();
	              break;
              case 2 :
	              Do_Ofp_Rt2Rt_MinorCycle2();
	              Receive_Ofp_Data_MinorCycle2_bc();
	              Send_Ofp_Data_MinorCycle2_bc();				
	              break;
              case 3 :
	              Do_Ofp_Rt2Rt_MinorCycle3();
	              Receive_Ofp_Data_MinorCycle3_bc();
	              Send_Ofp_Data_MinorCycle3_bc();
	              break;  
              case 4 :
	              //Do_Ofp_Rt2Rt_MinorCycle4();
	              Receive_Ofp_Data_MinorCycle4_bc();
	              Send_Ofp_Data_MinorCycle4_bc();
	              break;				
              case 5 :
	              Do_Ofp_Rt2Rt_MinorCycle5();
	              Receive_Ofp_Data_MinorCycle5_bc();
	              Send_Ofp_Data_MinorCycle5_bc();
	              break;				
              case 6 :
	              Do_Ofp_Rt2Rt_MinorCycle6();
	              Receive_Ofp_Data_MinorCycle6_bc();
	              Send_Ofp_Data_MinorCycle6_bc();
	              break;	
              case 7 :
	              Do_Ofp_Rt2Rt_MinorCycle7(); 
	              Receive_Ofp_Data_MinorCycle7_bc();
	              Send_Ofp_Data_MinorCycle7_bc();
	              break;
              default :break;
              }           
             
              int idx=0;//start = MC_minor_cycle_number * 100;
              //int end = start + 100;

              for (idx = 0; idx < Pos1; idx++)
              {
                  major_frames[MC_minor_cycle_number].frame_items[idx] = idx + 1;
              }

              major_frames[MC_minor_cycle_number].Mess_Count = Pos1;
              major_frames[MC_minor_cycle_number].MinorFrame_Time = 20;
              printf("MC_minor_cycle_number[%d %d]........! %d \n",MC_minor_cycle_number,major_frames[MC_minor_cycle_number].MinorFrame_Time,Pos1);
              Pos1=0;Pos2=0;Pos3=0;
              
           }
		//get_time_hud();
	while(ThreadRunFlag==0)
	{
	  pthread_mutex_lock(&mutexObj); 
          gettimeofday(&start, NULL); // Start time         
	       	
	 // Bus_Scheduel();//scheduel
			

	 DefMajorFrame(Mc1Modno, 0, 1, major_frames);
        //PriorityHighLow(module_id, 2, 1);
        printf("BC Major Frame Started Wait........! Mod[%d] \n",Mc1Modno);
        RunBcMajorFrame(Mc1Modno, 0, 1, major_frames, 1);
	 
	   do 
	   {
                    gettimeofday(&stop, NULL);
                    llTimeDiff = (stop.tv_sec - start.tv_sec) * 1000000LL +
                                 (stop.tv_usec - start.tv_usec);
                    dftDuration = llTimeDiff / 1000.0; // in milliseconds
            } while (dftDuration < 20.0);
            counter++;
          //printf("\rBus1 Number Of Minor Cycle Time...Time %f", dftDuration);
         // printf("\rBus1 Number Of Minor Cycles...%d Time %f", counter,dftDuration);
          fflush(stdout); 
         pthread_mutex_unlock(&mutexObj); 	
         Pos1=0;Pos2=0;Pos3=0;
	   
	}
    return NULL;
}
#endif
void Init_RT()
{
	//ActivateRT();
}
void ActivateMT(void)
{
	////////////////BUS1
	if(MT1Initialised == FALSE)
	{
		MT1Initialised = ActivateMT1(Mt1Modno,Mt1Cardno); 
		if(MT1Initialised == TRUE)
			printf(" MT1 INITIALISED ");
	}
	 
	////////////////BUS2
	if(MT2Initialised == FALSE)
	{
		MT2Initialised = ActivateMT2(Mt2Modno,Mt2Cardno);
		if(MT2Initialised == TRUE)
			printf(" MT2 INITIALISED ");
	}
	
}

/*******************Initialize  module1 as MT****************************/

UBIT16 ActivateMT1(UBIT16 ModuleId ,UBIT16 CardId) 
{
	ModuleId=1;
	return TRUE;
}

/*******************Initialize  module2 as MT****************************/

UBIT16 ActivateMT2(UBIT16 ModuleId ,UBIT16 CardId) 
{     
	ModuleId=2;
	return TRUE;
}

/*******************Initialize  module3 as MT****************************/

UBIT16 ActivateMT3(UBIT16 ModuleId ,UBIT16 CardId) 
{   
	ModuleId=3;
	return TRUE;
}

void DeActivateMT(void)
{
	printf("\n MT Deactivated \n");
	CloseMT1Module();
	/*CloseMT2Module();
	CloseMT3Module();*/
}

void CloseMT1Module()
{	
	MT1Initialised = FALSE;
	MT2Initialised = FALSE;
	MT3Initialised = FALSE;	
	/*SelectModule(1,CARD1);*/
	ResetAce(1);
	printf("\t\tWait.!\n");
	usleep(500);
	//CloseHandle(hthread1);//CloseDeviceHandle(CARD1);
}

