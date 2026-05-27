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

UBIT16 IntBCrec_recv(UBIT16 buffer[32][32],UBIT16 MCyc,UBIT16 ModuleId,UBIT16 CardId);

void ActivateMc(void);
void Sdef_Scheduler(void);
void Init_RT(void);
void Init_MT(void);
void RUNALL_MT(void);
void BC_FILE(void);
int chanNum = 0,TxchanNum=0,RxchanNum=0;
	UCHAR rxData[64] ;
	UCHAR txData[64] ;
unsigned int ChNo=0,addr=0,setting,bytcnt = 1;
int cnt=0;
#define MSG_COUNT 1024
FILE* logFile = NULL;
short ReadMsg1553(UCHAR ModId,unsigned short stack_entry,RT_MSG *message)//Use for TxVectorWord
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
			//look_up= LOOKUP_1;
		if (message->t_r == TRANSMIT)
			look_up = look_up+32;
		offset = MemRead(ModId,look_up + message->subaddr,1);
		if(message->cmd==0xf811)
		{
		   message->data[0] = MemRead(ModId,STACK_1 + ((stack_entry * 4)-2),1);	  
		}
		else
		{
		  temp = message->wcnt;
		  if(temp == 0)
			  temp = 32;
		  for (i=0;i<temp;i++)
			  message->data[i] = MemRead(ModId,offset+i,1);
		}
	}
	else
		error = 36;
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
    SetRtAddrM(rtModId, rtAddr, 1);
    DefLookUp(rtModId, LOOKUP_1);   
    // Enable RT mode
    DefMode(rtModId, RT);

   //SetTerFlag(modId);
   //ResetTerFlag(modId);
    //SetDba(modId);    
    RegWrite(rtModId,CONFIG_REG3,0x8000,1);
    //RegWrite(rtModId,CONFIG_REG3,0x8001,1);//To SetServiceReq(TxVectorWord)
    //SetSvcReq(modId);
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
    //MemWrite(rtModId,0x120,0xa5a5,1);//Enable ServiceReq(TxVectorWord)
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
	  printf("ModuleId of DefMode is :%d\n",ModuleId);
    return TRUE;
}
void BCSchedule_Bus()
{
    struct timeval start, stop;
    long long llTimeDiff = 0,counter=0;
    int globalIncre=0,PreC=0;
    unsigned short MsCoun=0;
    unsigned short inter_msg_gap=10,msg_no=0,idx=0,module_id=1,data_value=0,Wcount=0;
    double dftDuration = 0.0;
    int prevPos = 0;  
    UBIT16 data[20][32], sub_addr[10], wordcnt[20];
    printf("Bus thread Start...\n");
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
                    bc_page.bus = BUS_A;
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
                           printf("MC0 %d\n",data_buffer[0]);
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
                    bc_page.bus = BUS_A;
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
                    DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);      
                    
                }
	       break;
              case 2 :
	             /* ================= FRAME 2 ================= */
              for (msg_no = 1; msg_no <= 41; msg_no++)
              {
                  ++MsCoun;
                  memset(&bc_page, 0, sizeof(bc_page));
                  bc_page.bus = BUS_A;
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
                    DefMsg(module_id, MsCoun, bc_page, data_buffer, Wcount, inter_msg_gap);       
              }
			
	              break;
              case 3 :
	             /* ================= FRAME 3 ================= */
                for (msg_no = 1; msg_no <= 42; msg_no++)
                {
                    ++MsCoun;
                    memset(&bc_page, 0, sizeof(bc_page));
                    bc_page.bus = BUS_A;
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

                    if (bc_page.cmd_1.t_r != TRANSMIT)
                    {
                        if (bc_page.cmd_1.subaddr == 0)
                        {
                            data_buffer[0] =  MC_minor_cycle_number;
                            //printf("MC3 %d\n",data_buffer[0]);
                        }
                        else
                        {
                            for (idx = 0; idx < Wcount; idx++)
                                data_buffer[idx] = ++data_value;
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
                  bc_page.bus = BUS_A;
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

                    if (bc_page.cmd_1.t_r != TRANSMIT)
                    {
                        if (bc_page.cmd_1.subaddr == 0)
                        {
                            data_buffer[0] =  MC_minor_cycle_number;
                            //printf("MC4 %d\n",data_buffer[0]);
                        }
                        else
                        {
                            for (idx = 0; idx < Wcount; idx++)
                                data_buffer[idx] = ++data_value;
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
                  bc_page.bus = BUS_A;
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

                    if (bc_page.cmd_1.t_r != TRANSMIT)
                    {
                        if (bc_page.cmd_1.subaddr == 0)
                        {
                            data_buffer[0] =  MC_minor_cycle_number;
                            //printf("MC5 %d\n",data_buffer[0]);
                        }
                        else
                        {
                            for (idx = 0; idx < Wcount; idx++)
                                data_buffer[idx] = ++data_value;
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
                   bc_page.bus = BUS_A;
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

                    if (bc_page.cmd_1.t_r != TRANSMIT)
                    {
                        if (bc_page.cmd_1.subaddr == 0)
                        {
                            data_buffer[0] =  MC_minor_cycle_number;
                            //printf("MC6 %d\n",data_buffer[0]);
                        }
                        else
                        {
                            for (idx = 0; idx < Wcount; idx++)
                                data_buffer[idx] = ++data_value;
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
                  bc_page.bus = BUS_A;
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

                    if (bc_page.cmd_1.t_r != TRANSMIT)
                    {
                        if (bc_page.cmd_1.subaddr == 0)
                        {
                            data_buffer[0] = MC_minor_cycle_number;
                            //printf("MC7 %d\n",data_buffer[0]);
                        }
                        else
                        {
                            for (idx = 0; idx < Wcount; idx++)
                                data_buffer[idx] = ++data_value;
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
  
	DefMajorFrame(1, 0, 8, major_frames);
        //PriorityHighLow(module_id, 2, 1);
        printf("BC Major Frame Started Wait........! \n");
        RunBcMajorFrame(1, 0,8, major_frames, 10);
        getchar();
        printf("Enter to Reset BC........! \n");
        getchar();  
        ResetAce(1);
        printf("BC Reset ........Success! \n");
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
            
                /*if (diff != 1/* && prevDesc != 0* /)
                {
                    fprintf(logFile, "Warning: Descriptor jump! prev=%04X last=%04X curr=%04X (diff=%d)\n",
                            prev, last, current, diff);
                }  */            
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
                fprintf(logFile,"\nTime [%02d:%02d:%02d.%06ld] Msg#%lu \n",t->tm_hour, t->tm_min, t->tm_sec,
                        tv.tv_usec, msgCount);
               // fprintf(logFile,"\nTime [%02d:%02d:%02d.%06ld] Msg#%lu LastC=%04X CurrentC=%04X pos=%d Offset=%04X \n",t->tm_hour, t->tm_min, t->tm_sec,tv.tv_usec, msgCount, last,current,pos,Offset);
               // printf("\nMsgCount#[%lu]\n",msgCount);
                
                prev_tv = tv; // update previous timestamp

                // ---- Log header ----
                fprintf(logFile,"BlkSts=0x%04X  Cmd=0x%04X\n",buffer.BlockSts.value,buffer.RecCmdWrd.value);
                 //fprintf(logFile,"BlkSts=0x%04X  TimeTag=0x%04X  Cmd=0x%04X\n",buffer.BlockSts.value, buffer.TimeTag, buffer.RecCmdWrd.value);
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
                  if(buffer.RecCmdWrd.value==0xf811)
                    temp=1;
                    for (int i = 0; i < temp; i++)
                    {
                      buffer.DataPack[i] = MemRead(MTModId, ptr++, 1);
                      fprintf(logFile,"%04X ", buffer.DataPack[i]);
                      if(buffer.RecCmdWrd.value==0xf811)
                        printf("MsgCount#[%lu] MC[%04X] \n",msgCount, buffer.DataPack[0]);
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
                #if 1
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
                  MC1_12_broadcast_mesg.sec_20ms.count_ms20 = (UBIT16)(ms / 20); // 0 to 49
                  
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
}


