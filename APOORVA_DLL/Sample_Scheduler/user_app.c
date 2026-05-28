
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
#include <stdatomic.h>
#include <stdint.h>


#include "/home/ccpmc/Desktop/WORKING/APOORVA_CCPMC_DMA_Interrupt_Handler/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"


/* ── Boolean helpers ── */
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* ── Bus / direction constants ── */
#define BUS_A                1
#define BUS_B                0
#define RECEIVE              0
#define TRANSMIT             1
#define YES                  1
#define NO                   0

/* ── Card / hardware ── */
#define CARDID                1
#define CONFIG_REG_INIT_VAL  0x00000078
#define ENABLE_BC_INT        0x00000029
#define ENABLE_MT_INT        0x00000001
#define ENABLE_RT_INT        0x0000ffff

#define DISABLE  0x00000000


/* ── Messages per minor frame for each test type ──
 *   Change these if you want a different number of messages.        */
#define MSGS_PER_FRAME_BCRT   5   /* BC→RT        */
#define MSGS_PER_FRAME_RTBC   5   /* RT→BC        */
#define MSGS_PER_FRAME_RTRT   5   /* RT→RT        */
#define MSGS_PER_FRAME_MC     5   /* Mode Code    */
#define MSGS_PER_FRAME_BCAST  5   /* Broadcast    */

/* ── Module assignments ── */
#define BC_MOD    1
#define MT_MOD    1/* same module as BC in snode mode */
#define RT_MOD    3 /* separate RT module              */



/*static struct termios orig_termios;  // save original state


/* ── typedef shims ── */
typedef unsigned char  BYTE;
typedef unsigned long  DWORD;
typedef unsigned short UBIT16;
typedef short          SBIT16;
typedef void*          HANDLE;
typedef long long      LARGE_INTEGER;
typedef int            errno_t;

/* ── Thread argument struct ── */
typedef struct {
    unsigned char  MTModId;
    unsigned char  BCModId;
    unsigned long  expectedMessages;   /* NoOfTimes × msgs_per_frame */
} MTThreadArgs;

pthread_t        mtThread;
pthread_t        thread;               /* spare, kept for compat */
pthread_mutex_t  mutexObj      = PTHREAD_MUTEX_INITIALIZER;
volatile int     bc_done       = 0;
volatile int     ThreadRunFlag = 0;
volatile int     stop_mt       = 0;    /* set to 1 to stop MT thread        */
atomic_int       bc_completed  = 0;    /* set to 1 when BC finishes frames  */

MSG_DEF          bc_page;
MAJOR_FRAME      major_frames[128];
unsigned short   data_buffer[40];

FILE *logFile   = NULL;
FILE *MtlogFile = NULL;
FILE *RtlogFile = NULL;

int enable,BCResponseTime,BCRETRY,intr,intr_msg_no,msg_no;
unsigned short gap;
 int msg_no,prior,count_no,count,enable_async,Dynamic_data,dynamic_no,count1,BcRetryEnable,Retry;
unsigned long g_total_bytes_prepared = 0;
int gp_enable = 0, gp_frames = 0;


/* BC schedules */
void BCSchedule_BCRT  (int Times, unsigned char mod, unsigned char ch,
                        unsigned char rt, unsigned char sa, unsigned char wc,int,int,int,int,int,int,unsigned short);
void BCSchedule_RTBC  (int Times, unsigned char mod, unsigned char ch,
                        unsigned char rt, unsigned char sa, unsigned char wc,int,int,int,int,int,int,int);
void BCSchedule_RTRT  (int Times, unsigned char mod, unsigned char ch,
                        unsigned char tx_rt, unsigned char tx_sa,
                        unsigned char rx_rt, unsigned char rx_sa,
                        unsigned char wc,int,int,int,int,int,int,unsigned short);
void BCSchedule_RTRT_BCST  (int Times, unsigned char mod, unsigned char ch,
                        unsigned char tx_rt, unsigned char tx_sa,
                        unsigned char rx_rt, unsigned char rx_sa,
                        unsigned char wc,int,int,int);
void BCSchedule_MCode    (int Times, unsigned char mod, unsigned char ch,
                        unsigned char rt,  unsigned char sa, unsigned char wc,int,int,int);
void BCSchedule_BCAST (int Times, unsigned char mod, unsigned char ch,
                        unsigned char sa,  unsigned char wc,int,int,int,int);

/* MT */
void* StartMTThread(void *arg);
void  ProcessMTDescriptor(unsigned char MTModId, unsigned short desc,
                            unsigned short pos, unsigned long *msgCount,
                            struct timeval *prev_tv);
void GeneralPurposeReg(int ,int );
/* Frame helpers */
int           DefMajorFrame_new    (UCHAR Bus, USHORT BCSTACK_A_B,
                                    USHORT no_of_minor_frames,
                                    MAJOR_FRAME frame_sequence[],int intrmsg);
unsigned long RunBcMajorFrame_new  (UCHAR Bus, USHORT stackA_B,
                                    USHORT no_of_minor_frames,
                                    MAJOR_FRAME frame_sequence[],
                                    unsigned long times,int,int,int);
                               
void get_bcretry(void);
void get_response(void);
void get_async_config(int *msg_no, int *prior, int *count_no, int *count);
void Dynamic_Data(int *dynamic_no, int *count1);
 
 
 void get_async_config(int *msg_no, int *prior, int *count_no, int *count)
  {
      printf("\t Select msg no: ");
      scanf("%d", msg_no);

      printf("\t Select priority (1: high, 0: low): ");
      scanf("%d", prior);

      printf("\t Enter Msg No from where to inject async: ");
      scanf("%d", count_no);

      *count = *count_no;
  }
  void Dynamic_Data(int *dynamic_no,int *count1)
  {
     
      printf("\t Enter Msg No from where to Change Data: ");
      scanf("%d", dynamic_no);
      
      *count1 = *dynamic_no;
   }
  void get_bcretry()
  {
      //printf("%d\n",ret);
     printf("\n\tSingleRetry Option : Same[1] OR Alter[2]");
     printf("\n\tDoubleRetry Option : SS[3] OR AA[4] OR SA[5] OR AS[6]\t");
     scanf("%d", &BCRETRY);
    // printf("%d\n",ret);
  }
  void get_response()
  { 
     printf("enter bc response time:");
     scanf("%d",&BCResponseTime);
  }
  
void Read_DMA()
{
printf("***********************DMA Read data************************\n");
      getchar();
      getchar();
      Configure_DMA_READ(1,1);
      getchar();
      Trigger_DMA(1,1,3);
      getchar();
      getchar();
      read_dma_block();
}
 void Run_All_DMA_Transfers2(void)
{
    struct {
        unsigned long src_offset;   /* Source offset in DMA buffer */
        unsigned long dst_addr;     /* Destination device memory address */
    } dma_jobs[] =
    {
        {0x0000, 0x44000},
        {0x0FA0, 0x44000 + 0x07D0},
        {0x1F40, 0x44000 + 0x0FA0},
        {0x2EE0, 0x44000 + 0x1770},
        {0x3E80, 0x44000 + 0x1F40},
        {0x4E20, 0x44000 + 0x2710},
        {0x5DC0, 0x44000 + 0x2EE0},
        {0x6D60, 0x44000 + 0x36B0},
        {0x7D00, 0x44000 + 0x3E80},
        {0x8CA0, 0x44000 + 0x4650},
        {0x9C40, 0x44000 + 0x4E20},
        {0xABE0, 0x44000 + 0x55F0},
        {0xBB80, 0x44000 + 0x5DC0},
        {0xCB20, 0x44000 + 0x6590},
        {0xDAC0, 0x44000 + 0x6D60},
        {0xEA60, 0x44000 + 0x7530},
        {0xFA00, 0x44000 + 0x7D00},
        {0x109A0, 0x44000 + 0x84D0},
        {0x11940, 0x44000 + 0x8CA0},
        {0x128E0, 0x44000 + 0x9470}
        
    };

    int i;
    int num_jobs = sizeof(dma_jobs) / sizeof(dma_jobs[0]);

    //printf("Total bytes prepared = %lu bytes\n", g_total_bytes_prepared);
    write_via_dma(1, g_total_bytes_prepared);
    unsigned short temp=0x4000;
    for (i = 0; i < 20; i++)
    {
        /*printf("Starting DMA Transfer %d/%d\n", i + 1, num_jobs);
        printf("  Source Offset : 0x%08X\n", dma_jobs[i].src_offset);
        printf("  Destination   : 0x%08X\n", dma_jobs[i].dst_addr);*/

        /* Small delay between transfers */
       // usleep(1000);
       //  getchar();
        /* Configure DMA engine */
        Configure_DMA_Write(1, 1);
         
        /* Program source and destination */
        DMA_Source_Destination(dma_jobs[i].src_offset,
                               dma_jobs[i].dst_addr);

        /* Trigger DMA */
        Trigger_DMA(1, 1, 1);
        usleep(1000);
       

    }

    printf("All DMA transfers completed successfully.\n");
}                                   
void Run_All_DMA_Transfers(void)
{
    struct {
        unsigned long src_offset;   /* Source offset in DMA buffer */
        unsigned long dst_addr;     /* Destination device memory address */
    } dma_jobs[] =
    {
        {0x0000, 0x4000},
        {0x0FA0, 0x4000 + 0x07D0},
        {0x1F40, 0x4000 + 0x0FA0},
        {0x2EE0, 0x4000 + 0x1770},
        {0x3E80, 0x4000 + 0x1F40},
        {0x4E20, 0x4000 + 0x2710},
        {0x5DC0, 0x4000 + 0x2EE0},
        {0x6D60, 0x4000 + 0x36B0},
        {0x7D00, 0x4000 + 0x3E80},
        {0x8CA0, 0x4000 + 0x4650},
        {0x9C40, 0x4000 + 0x4E20},
        {0xABE0, 0x4000 + 0x55F0},
        {0xBB80, 0x4000 + 0x5DC0},
        {0xCB20, 0x4000 + 0x6590},
        {0xDAC0, 0x4000 + 0x6D60},
        {0xEA60, 0x4000 + 0x7530},
        {0xFA00, 0x4000 + 0x7D00},
        {0x109A0, 0x4000 + 0x84D0},
        {0x11940, 0x4000 + 0x8CA0},
        {0x128E0, 0x4000 + 0x9470}
        
    };

    int i;
    int num_jobs = sizeof(dma_jobs) / sizeof(dma_jobs[0]);

    //printf("Total bytes prepared = %lu bytes\n", g_total_bytes_prepared);
    write_via_dma(1, g_total_bytes_prepared);
    unsigned short temp=0x4000;
    for (i = 0; i < 20; i++)
    {
      
        Configure_DMA_Write(1, 1);
         
        /* Program source and destination */
        DMA_Source_Destination(dma_jobs[i].src_offset,
                               dma_jobs[i].dst_addr);

        /* Trigger DMA */
        Trigger_DMA(1, 1, 1);
        usleep(1000);
       
    }

    printf("All DMA transfers completed successfully.\n");
}
void BCResponseTimeEnable(UCHAR ModId,UCHAR Enable, unsigned short BCResponseTime)
{
    if(Enable==1)
    {
      switch (BCResponseTime)
      {
        case 1:
        RegWrite(ModId,0xA3,0x8000,1);
        RegWrite(ModId,0xA1,0xffff,1);
        RegWrite(ModId,0xA4,(RegRead(ModId,0xA5,1))|0x0000,1);
        printf("(1 BCWaits 9.25 Microseconds (If NoResponse) %x\n",(RegRead(ModId,0xA1,1)));
        printf("(1 BCWaits 9.25 Microseconds (If NoResponse) %x\n",(RegRead(ModId,0xA2,1)));
        //printf("(1 BCWaits 14 Microseconds (If NoResponse)\n");
        break;
        case 2:
        //RegWrite(ModId,0xA2,0x2,1);
        RegWrite(ModId,0xA3,0x8000,1);
        RegWrite(ModId,0xA4,0x0200,1);
        printf("2 BCWaits 11.25 Microseconds (If NoResponse) %x\n",(RegRead(ModId,0xA2,1)));
        //printf("2 BCWaits 18 Microseconds (If NoResponse)\n");
        break;
        case 3:
        RegWrite(ModId,0xA3,0x8000,1);
        RegWrite(ModId,0xA4,0x0400,1);
        printf("3 BCWaits 25.25 Microseconds (If NoResponse) %x\n",(RegRead(ModId,0xA2,1)));
        //printf("3 BCWaits 46 Microseconds (If NoResponse)\n");
        break;
        case 4:
        RegWrite(ModId,0xA3,0x8000,1);
        RegWrite(ModId,0xA4,(RegRead(ModId,0xA4,1))|0x0600,1);
        printf("4 BCWaits 65 Microseconds (If NoResponse) %x\n",(RegRead(ModId,0xA2,1)));
        //printf("4 BCWaits 126 Microseconds (If NoResponse)\n");
        break;
      }
    }
}
void get_user_config(int *enable_async, int *Dynamic_data, int *BcRetryEnable, 
                     int *enable, int *intr, int *msg_no, int *prior, 
                     int *count_no, int *count, int *dynamic_no, int *count1,
                     int *intr_msg_no, uint16_t *gap,int *gp_enable, int *gp_frames)
{
    printf("\t Do you want to enable asynchronous messages? (1: Yes, 0: No): ");
    scanf("%d", enable_async);
    printf("\t Do you want to Update Data Dynamically? (1: Yes, 0: No): ");
    scanf("%d", Dynamic_data);
    printf("\t Do you want to BC retry enable? (1: Yes, 0: No): ");
    scanf("%d", BcRetryEnable);
    printf("\t Do you want to BC response time Enable? (1: Yes, 0: No): ");
    scanf("%d", enable);
    printf("\t Do you want enable intr msg gap? (1: Yes, 0: No): ");
    scanf("%d", intr);
     printf("\t Do you want to enable General Purpose  Register? (1: Yes, 0: No): ");
    scanf("%d", gp_enable);
    if (*gp_enable)
    {
        printf("\t Enter number of frames for GP register (e.g. 3): ");
        scanf("%d", gp_frames);
    }

    if (*BcRetryEnable)
    {
        get_bcretry();
    }
    if (*enable)
    {
        get_response();
    }
    if (*enable_async)
    {
        get_async_config(msg_no, prior, count_no, count);
    }
    else
    {
        printf("\t Continuing without async messages...\n");
    }
    if (*Dynamic_data)
    {
        Dynamic_Data(dynamic_no, count1);
    }
    else
    {
        printf("\t Continuing without Dynamic Data...\n");
    }
    if (*intr)
    {
        printf("enter msg no:");
        scanf("%d", intr_msg_no);
        printf("enter gap:");
        scanf("%hu", gap);
    }
}

/* ══════════════════════════════════════════════════════════════════════════
 * SIGNAL HANDLER
 * ══════════════════════════════════════════════════════════════════════════ */
void handle_sigint(int sig)
{
    disableNonCanonicalMode();
    printf("\n\tExiting...\n");
    if (logFile)   fclose(logFile);
    if (MtlogFile) fclose(MtlogFile);
    if (RtlogFile) fclose(RtlogFile);
    exit(0);
}

/* ══════════════════════════════════════════════════════════════════════════
 * HELPER – common diagnostics printed after every BC run
 * ══════════════════════════════════════════════════════════════════════════ */
static void PrintDiagnostics(unsigned char mod)
{
    printf("\t Reg read mt %04x-%d\n", 0x00F9, RegRead(1, 0x00F9, 1));
    printf("\t Reg read mt %04x-%d\n", 0x00FB, RegRead(1, 0x00FB, 1));
    printf("\t Reg read mt %04x-%d\n", 0x00F1, RegRead(1, 0x00F1, 1));
    printf("\t Reg read mt %04x-%d\n", 0x00F3, RegRead(1, 0x00F3, 1));
    //GetBCSOMCnt(mod, CARDID);
    GetBCEOMCnt(mod, CARDID);
    GetBCEOFCnt(mod, CARDID);
    GetBCEOMINFRAMECnt(mod, CARDID);
    //GetMTSOMCnt(mod, CARDID);
    GetMTEOMCnt(mod, CARDID);
}
void GeneralPurposeReg(int BcModId,int N)
{
    unsigned int frames = N;
    unsigned int value = 0x100 | ((1 << frames) - 1);

    RegWrite(BcModId,0x16,0x1,1);  //ENABLING GENERAL PURPOSE REGISTER
    RegWrite(BcModId, 0x12, value, 1);

    //RegWrite(BcModId,0x12,0x107,1); //FIRST 16(MINOR FRAME)
           
}
/* ══════════════════════════════════════════════════════════════════════════
 * HELPER – build N messages into one minor frame, each to same RT/SA
 *          Returns actual count placed (clamped to Mess_Count limit)
 * ══════════════════════════════════════════════════════════════════════════ */
static void BuildMinorFrame_BCRT(unsigned char mod,
                                  unsigned char bus,
                                  unsigned char rt,
                                  unsigned char sa,
                                  unsigned char wc,
                                  int           n)
{
    unsigned short data_value = 0;
    g_total_bytes_prepared = 0;
    memset(major_frames, 0, sizeof(major_frames));

    for (int i = 0; i < 1000; i++)
    {
    
        memset(&bc_page, 0, sizeof(bc_page));
        bc_page.bus           = bus;
        bc_page.self_test     = NO;
        bc_page.mask_bcst_bit = NO;
        bc_page.broadcast     = NO;
        bc_page.mode_code     = NO;
        bc_page.rt_rt         = NO;
        bc_page.retry_enable =  YES;
        bc_page.loop          = 0x0000;
        bc_page.cmd_1.status  = 0xCCCC;  
        if(rt==31)
        {
           rt=1;
        }
        bc_page.cmd_1.taddr   = rt++;
        bc_page.cmd_1.subaddr = sa++;
        if(sa==31 || sa==0)
        {
        sa=1;
        }
        bc_page.cmd_1.wcnt    = wc;
        bc_page.cmd_1.t_r     = RECEIVE;

        for (int idx = 0; idx < wc; idx++)
            data_buffer[idx] = ++data_value;
            
        DEFMSG_DMA(mod, i + 1, bc_page, data_buffer, wc, 100);
        
        g_total_bytes_prepared += (unsigned long)wc * sizeof(unsigned short);
       
      //  major_frames[0].frame_items[i] = i + 1;
    }
    
for(int i =0 ;i<10;i++) //i = no of frames
{
    major_frames[i].Mess_Count      = 100;
    major_frames[i].MinorFrame_Time = 100;
    for(int j=0;j<100;j++) //no of msgs
    {
            int msg_num = (i* 100) + j + 1;  // 1-based
            major_frames[i].frame_items[j] = msg_num;
    }
  }
    printf("\t Minor frame: %d BC→RT messages  RT=%u SA=%u WC=%u\n", 100, rt, sa, wc);
    
}

static void BuildMinorFrame_RTBC(unsigned char mod,
                                  unsigned char bus,
                                  unsigned char rt,
                                  unsigned char sa,
                                  unsigned char wc,
                                  int           n)
{
    memset(major_frames, 0, sizeof(major_frames));
    rt=1;
    for (int i = 0; i < 1000; i++)
    {
        memset(&bc_page, 0, sizeof(bc_page));
        bc_page.bus           = bus;
        bc_page.self_test     = NO;
        bc_page.mask_bcst_bit = NO;
        bc_page.broadcast     = NO;
        bc_page.mode_code     = NO;
        bc_page.rt_rt         = NO;
        bc_page.loop          = 0x0000;
        bc_page.cmd_1.status  = 0xCCCC;
        if(rt==31)
        {
           rt=1;
        }
        bc_page.cmd_1.taddr   = rt++;
        bc_page.cmd_1.subaddr = sa;
        bc_page.cmd_1.wcnt    = wc;
        bc_page.cmd_1.t_r     = TRANSMIT;   /* RT transmits → BC receives */

        memset(data_buffer, 0, sizeof(data_buffer));
       
           DEFMSG_DMA(mod, i + 1, bc_page, data_buffer, wc, 100);
           g_total_bytes_prepared += (unsigned long)wc * sizeof(unsigned short);

      
       // major_frames[0].frame_items[i] = i + 1;
    }
for(int i =0 ;i<10;i++)
{
    major_frames[i].Mess_Count      = 100;
    major_frames[i].MinorFrame_Time = 100;
    for(int j=0;j<100;j++)
    {
            int msg_num = (i* 100) + j + 1;  // 1-based
            major_frames[i].frame_items[j] = msg_num;
    }
  }
    printf("\t Minor frame: %d RT→BC messages  RT=%u SA=%u WC=%u\n", n, rt, sa, wc);
    
}

static void BuildMinorFrame_RTRT(unsigned char mod,
                                  unsigned char bus,
                                  unsigned char tx_rt, unsigned char tx_sa,
                                  unsigned char rx_rt, unsigned char rx_sa,
                                  unsigned char wc,
                                  int           n)
{
    memset(major_frames, 0, sizeof(major_frames));
    rx_rt=1;
    for (int i = 0; i < 1000; i++)
    {
        memset(&bc_page, 0, sizeof(bc_page));
        bc_page.bus           = bus;
        bc_page.self_test     = NO;
        bc_page.mask_bcst_bit = NO;
        bc_page.broadcast     = NO;
        bc_page.mode_code     = NO;
        bc_page.retry_enable =  YES;
        bc_page.rt_rt         = YES;
        bc_page.loop          = 0x0000;

        /* CW1 = Receive command (RX RT) */
        if(rx_rt==31)
        {
          rx_rt=1;
        }
        bc_page.cmd_1.taddr   = rx_rt++;
        bc_page.cmd_1.subaddr = rx_sa;
        bc_page.cmd_1.wcnt    = wc;
        bc_page.cmd_1.t_r     = RECEIVE;
        bc_page.cmd_1.status  = 0xCCCC;
       
        /* CW2 = Transmit command (TX RT) */
        bc_page.cmd_2.taddr   = tx_rt;
        bc_page.cmd_2.subaddr = tx_sa;
        bc_page.cmd_2.wcnt    = wc;
        bc_page.cmd_2.t_r     = TRANSMIT;
        bc_page.cmd_2.status  = 0xCCCC;

        memset(data_buffer, 0, sizeof(data_buffer));
        
        DEFMSG_DMA(mod, i + 1, bc_page, data_buffer, wc, 20);
        g_total_bytes_prepared += (unsigned long)wc * sizeof(unsigned short);

      //  major_frames[0].frame_items[i] = i + 1;
    }
for(int i =0 ;i<10;i++)
{
    major_frames[i].Mess_Count      = 100;
    major_frames[i].MinorFrame_Time = 100;
    for(int j=0;j<100;j++)
    {
            int msg_num = (i* 100) + j + 1;  // 1-based
            major_frames[i].frame_items[j] = msg_num;
    }
  }
    printf("\t Minor frame: %d RT→RT messages  TX_RT=%u SA=%u  RX_RT=%u SA=%u  WC=%u\n",
           n, tx_rt, tx_sa, rx_rt, rx_sa, wc);
    
}
static void BuildMinorFrame_RTRT_BCST(unsigned char mod,
                                  unsigned char bus,
                                  unsigned char tx_rt, unsigned char tx_sa,
                                  unsigned char rx_rt, unsigned char rx_sa,
                                  unsigned char wc,
                                  int           n)
{
    memset(major_frames, 0, sizeof(major_frames));
    tx_rt=1;
    for (int i = 0; i < 1000; i++)
    {
        memset(&bc_page, 0, sizeof(bc_page));
        bc_page.bus           = bus;
        bc_page.self_test     = NO;
        bc_page.mask_bcst_bit = NO;
        bc_page.broadcast     = YES;
        bc_page.mode_code     = NO;
        bc_page.rt_rt         = YES;
        bc_page.loop          = 0x0000;

        /* CW1 = Receive command (RX RT) */
        bc_page.cmd_1.taddr   = rx_rt;
        bc_page.cmd_1.subaddr = rx_sa;
        bc_page.cmd_1.wcnt    = wc;
        bc_page.cmd_1.t_r     = RECEIVE;
        bc_page.cmd_1.status  = 0xCCCC;

        /* CW2 = Transmit command (TX RT) */
        if(tx_rt==31)
        {
          tx_rt=1;
        }
        bc_page.cmd_2.taddr   = tx_rt++;
        bc_page.cmd_2.subaddr = tx_sa;
        bc_page.cmd_2.wcnt    = wc;
        bc_page.cmd_2.t_r     = TRANSMIT;
        bc_page.cmd_2.status  = 0xCCCC;

        memset(data_buffer, 0, sizeof(data_buffer));
       
           DEFMSG_DMA(mod, i + 1, bc_page, data_buffer, wc, 20);
           g_total_bytes_prepared += (unsigned long)wc * sizeof(unsigned short);

       
        //major_frames[0].frame_items[i] = i + 1;
    }
for(int i=0;i<10 ;i++)
{
   
    major_frames[i].Mess_Count      = 100;
    major_frames[i].MinorFrame_Time = 100;
    for(int j=0;j<100;j++)
    {
            int msg_num = (i* 100) + j + 1;  // 1-based
            major_frames[i].frame_items[j] = msg_num;
    }
}
    printf("\t Minor frame: %d RT→RT messages  TX_RT=%u SA=%u  RX_RT=%u SA=%u  WC=%u\n",
           n, tx_rt, tx_sa, rx_rt, rx_sa, wc);
    
}

static void BuildMinorFrame_MCode(unsigned char mod,
                                unsigned char bus,
                                unsigned char rt,
                                unsigned char sa,
                                unsigned char wc,
                                int           n)
{
    
    memset(major_frames, 0, sizeof(major_frames));
    for (int i = 0; i < 1000; i++)
    {
        memset(&bc_page, 0, sizeof(bc_page));
        bc_page.bus           = bus;
        bc_page.self_test     = NO;
        bc_page.mask_bcst_bit = NO;
        bc_page.broadcast     = YES;
        bc_page.mode_code     = YES;
        bc_page.rt_rt         = NO;
        bc_page.loop          = 0x0000;
        bc_page.cmd_1.status  = 0xCCCC;
        if(rt==31)
        {
           rt=1;
        }
        bc_page.cmd_1.taddr   = 31;//rt;
        bc_page.cmd_1.subaddr = 0;   /* 0 or 31 for mode code */
        bc_page.cmd_1.wcnt    = 17;   /* e.g. 17 = Sync with Data Word */
        bc_page.cmd_1.t_r     = RECEIVE;

        memset(data_buffer, 0, sizeof(data_buffer));
         //if (sa == 0)
            data_buffer[0] = 0xa5a5+i;////(unsigned short)(i / 10); /* frame number */
        //printf("DefMsg %d  data_buffer[0]=0x%04X\n", i+1, data_buffer[0]);
         DEFMSG_DMA(mod, i + 1, bc_page, data_buffer, wc, 100);
         g_total_bytes_prepared += (unsigned long)wc * sizeof(unsigned short);

      //  major_frames[0].frame_items[i] = i + 1;
    }
for(int i=0;i<10 ;i++)
{
   
    major_frames[i].Mess_Count      = 100;
    major_frames[i].MinorFrame_Time = 100;
    for(int j=0;j<100;j++)
    {
            int msg_num = (i* 100) + j + 1;  // 1-based
            major_frames[i].frame_items[j] = msg_num;
    }
}
    printf("\t Minor frame: %d Mode Code messages  RT=%u SA=%u WC=%u\n", n, rt, sa, wc);
     
}

static void BuildMinorFrame_BCAST(unsigned char mod,
                                   unsigned char bus,
                                   unsigned char sa,
                                   unsigned char wc,
                                   int           n)
{
    unsigned short data_value = 0;

    memset(major_frames, 0, sizeof(major_frames));

    for (int i = 0; i < 1000; i++)
    {
        memset(&bc_page, 0, sizeof(bc_page));
        bc_page.bus           = bus;
        bc_page.self_test     = NO;
        bc_page.mask_bcst_bit = NO;
        bc_page.broadcast     = YES;          /* broadcast flag */
        bc_page.mode_code     = NO;
        bc_page.rt_rt         = NO;
        bc_page.retry_enable = YES;
        bc_page.loop          = 0x0000;
        bc_page.cmd_1.status  = 0xCCCC;
        bc_page.cmd_1.taddr   = 31;           /* broadcast address */
        bc_page.cmd_1.subaddr = sa;
        bc_page.cmd_1.wcnt    = wc;
        bc_page.cmd_1.t_r     = RECEIVE;

        for (int idx = 0; idx < wc; idx++)
            data_buffer[idx] = ++data_value;

      
           DEFMSG_DMA(mod, i + 1, bc_page, data_buffer, wc, 100);
           g_total_bytes_prepared += (unsigned long)wc * sizeof(unsigned short);
       
       // major_frames[0].frame_items[i] = i + 1;
    }

   for(int i =0 ;i<10;i++)
{
    major_frames[i].Mess_Count      = 100;
    major_frames[i].MinorFrame_Time = 100;
    for(int j=0;j<100;j++)
    {
            int msg_num = (i* 100) + j + 1;  // 1-based
            major_frames[i].frame_items[j] = msg_num;
    }
  }
    printf("\t Minor frame: %d Broadcast messages  SA=%u WC=%u\n", n, sa, wc);
    
}

/* ══════════════════════════════════════════════════════════════════════════
 * BC SCHEDULE FUNCTIONS
 * ══════════════════════════════════════════════════════════════════════════ */

/* ── Case 1: BC → RT (5 messages per frame) ── */
void BCSchedule_BCRT(int Times, unsigned char mod, unsigned char ch,
                      unsigned char rt, unsigned char sa, unsigned char wc,int count,int prior,int dynamic_no,int BcRetryEnable,int ResponseTime,int intr_msg_no,unsigned short gap)
{
    unsigned char bus = (ch == 1) ? BUS_A : BUS_B;
    printf("\n\t [BC→RT] RT=%u SA=%u WC=%u  Frames=%d bus=%d  \n",
           rt, sa, wc, Times,bus);
    printf("\t RegRead 0x03 = %X\n", RegRead(BC_MOD, 0x03, 1));
    printf("\t RegRead 0x01 = %X\n", RegRead(BC_MOD, 0x01, 1));
    BuildMinorFrame_BCRT(mod, bus, rt, sa, wc, MSGS_PER_FRAME_BCRT);
    
    int Retry; 
    printf("third reg=%04x\n",RegRead(mod,0x03,1));
    
    DefMajorFrame_new(mod, 0, 10, major_frames,100);
    if(ResponseTime)
    {
       BCResponseTimeEnable(BC_MOD, enable, BCResponseTime);
    }
    
    if (BcRetryEnable)
    {
        BCRetry(mod,BCRETRY);    
    }
    
    printf("\t BC Scheduling Started...\n");
    printf("\t RegRead 0x03 = %X\n", RegRead(BC_MOD, 0x03, 1));   
    printf("\t RegRead 0x01 = %X\n", RegRead(BC_MOD, 0x01, 1));
    usleep(1000);
    Run_All_DMA_Transfers();
    usleep(10000);
      printf("\t RegRead 0x03 = %X\n", RegRead(BC_MOD, 0x03, 1));
      printf("\t RegRead 0x01 = %X\n", RegRead(BC_MOD, 0x01, 1));
    RunBcMajorFrame_new(mod, 0,10, major_frames, (unsigned long)Times,count,prior,dynamic_no); 
    usleep(1000);
    getchar();getchar();
    Read_DMA();
    PrintDiagnostics(mod);
    getchar();
    getchar();
    
    printf("third reg=%04x\n",RegRead(mod,0x03,1));
    printf("\t Enter to Reset BC........!\n");
    getchar(); getchar();
    //ResetAce(mod);
    printf("\t BC Reset Success!\n");
    
    
}

/* ── Case 2: RT → BC  ── */
void BCSchedule_RTBC(int Times, unsigned char mod, unsigned char ch,
                      unsigned char rt, unsigned char sa, unsigned char wc,int count,int prior,int dynamic_no,int BcRetryEnable,int ResponseTime,int intr_msg_no,int gap)
{
    unsigned char bus = (ch == 1) ? BUS_A : BUS_B;
    printf("\n\t[RT→BC] RT=%u SA=%u WC=%u  Frames=%d  MsgsPerFrame=%d\n",
           rt, sa, wc, Times, MSGS_PER_FRAME_RTBC);

    BuildMinorFrame_RTBC(mod, bus, rt, sa, wc, MSGS_PER_FRAME_RTBC);
   //BuildMinorFrame_RTBC(mod, bus,  sa, wc, MSGS_PER_FRAME_RTBC);
    DefMajorFrame_new(mod, 0, 10, major_frames,100);  
    if(ResponseTime)
    {
       
       BCResponseTimeEnable(BC_MOD, enable, BCResponseTime);
    }
    
    if (BcRetryEnable)
    {
        BCRetry(mod,BCRETRY);
    }
    
    Run_All_DMA_Transfers();
    usleep(10000);
    printf("\t BC Scheduling Started...\n");
    RunBcMajorFrame_new(mod, 0, 10, major_frames, (unsigned long)Times,count,prior,dynamic_no);
    usleep(1000);
    getchar(); getchar();
    PrintDiagnostics(mod);

    printf("\t Enter to Reset BC...\n");
    getchar(); getchar();
    ResetAce(mod);
    printf("\t BC Reset Success!\n");
}

/* ── Case 3: RT → RT (5 messages per frame) ── */
void BCSchedule_RTRT(int Times, unsigned char mod, unsigned char ch,
                      unsigned char tx_rt, unsigned char tx_sa,
                      unsigned char rx_rt, unsigned char rx_sa,
                      unsigned char wc,int count,int prior,int dynamic_no,int BcRetryEnable,int ResponseTime,int intr_msg_no,unsigned short gap)
{
    unsigned char bus = (ch == 1) ? BUS_A : BUS_B;
    printf("\n\t[RT→RT] TX_RT=%u SA=%u  RX_RT=%u SA=%u  WC=%u  Frames=%d  MsgsPerFrame=%d\n",
           tx_rt, tx_sa, rx_rt, rx_sa, wc, Times, MSGS_PER_FRAME_RTRT);

    BuildMinorFrame_RTRT(mod, bus, tx_rt, tx_sa, rx_rt, rx_sa, wc, MSGS_PER_FRAME_RTRT);
    DefMajorFrame_new(mod, 0, 10, major_frames,100);
    if(ResponseTime)
    {
       
       BCResponseTimeEnable(BC_MOD, enable, BCResponseTime);
    }
    
    if (BcRetryEnable)
    {
        BCRetry(mod,BCRETRY);
    }
    Run_All_DMA_Transfers();
    usleep(10000);
    printf("\t BC Scheduling Started...\n");
    RunBcMajorFrame_new(mod, 0, 10, major_frames, (unsigned long)Times,count,prior,dynamic_no);
    usleep(1000);
    getchar(); getchar();
    PrintDiagnostics(mod);
  //  GetRTSOMCnt(RT_MOD, CARDID); //GetRTEOMCnt(RT_MOD, CARDID);

    printf("\t Enter to Reset BC...\n");
    getchar(); getchar();
    ResetAce(mod);
    printf("\t BC Reset Success!\n");
}
void BCSchedule_RTRT_BCST(int Times, unsigned char mod, unsigned char ch,
                      unsigned char tx_rt, unsigned char tx_sa,
                      unsigned char rx_rt, unsigned char rx_sa,
                      unsigned char wc,int count,int prior,int dynamic_no)
{
    unsigned char bus = (ch == 1) ? BUS_A : BUS_B;
    printf("\n\t[RT→RT] TX_RT=%u SA=%u  RX_RT=%u SA=%u  WC=%u  Frames=%d  MsgsPerFrame=%d\n",
           tx_rt, tx_sa, rx_rt, rx_sa, wc, Times, MSGS_PER_FRAME_RTRT);

    BuildMinorFrame_RTRT_BCST(mod, bus, tx_rt, tx_sa, rx_rt, rx_sa, wc, MSGS_PER_FRAME_RTRT);
    DefMajorFrame_new(mod, 0, 10, major_frames,100);
    Run_All_DMA_Transfers();
    usleep(10000);
    printf("\t BC Scheduling Started...\n");
    RunBcMajorFrame_new(mod, 0, 10, major_frames, (unsigned long)Times,count,prior,dynamic_no);
    usleep(1000);
    getchar(); getchar();
    PrintDiagnostics(mod);
    //GetRTSOMCnt(RT_MOD, CARDID); //GetRTEOMCnt(RT_MOD, CARDID);

    printf("\t Enter to Reset BC...\n");
    getchar(); getchar();
    ResetAce(mod);
    printf("\t BC Reset Success!\n");
}
/* ── Case 4: Mode Code (5 messages per frame) ── */
void BCSchedule_MCode(int Times, unsigned char mod, unsigned char ch,
                    unsigned char rt, unsigned char sa, unsigned char wc,int count,int prior,int dynamic_no)
{
    unsigned char bus = (ch == 1) ? BUS_A : BUS_B;
    printf("\n\t[Mode Code] RT=%u SA=%u WC=%u  Frames=%d  MsgsPerFrame=%d\n",
           rt, sa, wc, Times, MSGS_PER_FRAME_MC);

    BuildMinorFrame_MCode(mod, bus, rt, sa, wc, MSGS_PER_FRAME_MC);
    DefMajorFrame_new(mod, 0, 10, major_frames,100);
    Run_All_DMA_Transfers();
    usleep(10000);
    printf("\t BC Mode Code Scheduling Started...\n");
    RunBcMajorFrame_new(mod, 0, 10, major_frames, (unsigned long)Times,count,prior,dynamic_no);
    usleep(1000);
    getchar(); getchar();
    PrintDiagnostics(mod);

    printf("\t Press Enter to Reset BC...\n");
    getchar(); getchar();
    ResetAce(mod);
    printf("\t BC Reset Success!\n");
}

/* ── Case 5: Broadcast (5 messages per frame) ── */
void BCSchedule_BCAST(int Times, unsigned char mod, unsigned char ch,
                       unsigned char sa, unsigned char wc,int count,int prior,int dynamic_no,int BcRetryEnable)
{
    unsigned char bus = (ch == 1) ? BUS_A : BUS_B;
    printf("\n\t[Broadcast] SA=%u WC=%u  Frames=%d  MsgsPerFrame=%d\n",
           sa, wc, Times, MSGS_PER_FRAME_BCAST);

    BuildMinorFrame_BCAST(mod, bus, sa, wc, MSGS_PER_FRAME_BCAST);
    DefMajorFrame_new(mod, 0, 10, major_frames,100);
    Run_All_DMA_Transfers();
    usleep(10000);
    
    printf("\t BC Broadcast Scheduling Started...\n");
    RunBcMajorFrame_new(mod, 0, 10, major_frames, (unsigned long)Times,count,prior,dynamic_no);
    usleep(1000);
    getchar(); getchar();
    PrintDiagnostics(mod);

    printf("\t Enter to Reset BC........!\n");
    getchar(); getchar();
    ResetAce(mod);
    printf("\t BC Reset Success!\n");
}

/* ══════════════════════════════════════════════════════════════════════════
 * ProcessMTDescriptor
 * ══════════════════════════════════════════════════════════════════════════ */
void ProcessMTDescriptor(unsigned char  MTModId,
                          unsigned short desc,
                          unsigned short pos,
                          unsigned long *msgCount,
                          struct timeval *prev_tv)
{
    
    MNTPACK        buffer;
    char           dir = 'R';
    unsigned short temp, cmd, rtAddr, subAddr, wc_or_mode, trBit;
    unsigned long  ptr;
    struct timeval tv;
    struct tm     *t;

    unsigned long Offset       = 0x21000 + ((pos * 4) - 4);
    buffer.BlockSts.value      = MemRead(MTModId, Offset,     1);
    buffer.TimeTag             = MemRead(MTModId, Offset + 1, 1);
    buffer.DataBlkPtr          = MemRead(MTModId, Offset + 2, 1);
    ptr                        = 0x20000 + buffer.DataBlkPtr;
    //ptr = buffer.DataBlkPtr;
    buffer.RecCmdWrd.value     = MemRead(MTModId, Offset + 3, 1);
    //fprintf(logFile," cmd wrd =%04x \n",buffer.RecCmdWrd.value );
    if (buffer.BlockSts.value == 0x0000 || buffer.RecCmdWrd.value == 0x0000)
        return;

    cmd        = buffer.RecCmdWrd.value;
    rtAddr     = (cmd >> 11) & 0x1F;
    trBit      = (cmd >> 10) & 0x01;
    subAddr    = (cmd >>  5) & 0x1F;
    wc_or_mode = cmd & 0x1F;
    if (wc_or_mode == 0) wc_or_mode = 32;
    dir = (trBit == 1) ? 'T' : 'R';

    gettimeofday(&tv, NULL);
    t = localtime(&tv.tv_sec);

   /* printf("\nTime [%02d:%02d:%02d.%06ld] Date [%02d-%02d-%04d] Msg#%lu\n",
           t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec,
           t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, *msgCount);*/
   

    
    fprintf(logFile,
            "\nTime [%02d:%02d:%02d.%06ld] Date [%02d-%02d-%04d] Msg#%lu\n",
            t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec,
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, *msgCount);
    fprintf(logFile, "BlkSts=0x%04X  Cmd=0x%04X\n",
            buffer.BlockSts.value, buffer.RecCmdWrd.value);

    *prev_tv = tv;

    int is_rt_to_rt = (buffer.BlockSts.value & 0x0800) ? 1 : 0;
    int no_resp     = (buffer.BlockSts.value & 0x0200) ? 1 : 0;

    if (is_rt_to_rt)
    {
        unsigned short tx_cmd    = MemRead(MTModId, ptr,     1);
        unsigned short tx_status = MemRead(MTModId, ptr + 1, 1);
        unsigned short rx_status = MemRead(MTModId, ptr + 2, 1);
        unsigned short tx_rt     = (tx_cmd >> 11) & 0x1F;
        unsigned short tx_sa     = (tx_cmd >>  5) & 0x1F;
        unsigned short wc        = tx_cmd & 0x1F;
        if (wc == 0) wc = 32;

        /*printf("RT→RT: RX_RT=%u SA=%u  TX_RT=%u SA=%u  WC=%u\n",
               rtAddr, subAddr, tx_rt, tx_sa, wc);
        printf("TX_Cmd=0x%04X  TX_Sts=0x%04X  RX_Sts=0x%04X\n",
               tx_cmd, tx_status, rx_status);*/
               
        fprintf(logFile, "RT→RT: RX_RT=%u SA=%u  TX_RT=%u SA=%u  WC=%u\n",
                rtAddr, subAddr, tx_rt, tx_sa, wc);

        if (!no_resp)
        {
            ptr += 3;
           // printf("Data (TX_RT→RX_RT):\n");
            fprintf(logFile, "Data (TX_RT→RX_RT):\n");
            for (int ii = 0; ii < (int)wc; ii++)
            {
                buffer.DataPack[ii] = MemRead(MTModId, ptr++, 1);
                //printf("%04X ", buffer.DataPack[ii]);
                fprintf(logFile, "%04X ", buffer.DataPack[ii]);
                if ((ii + 1) % 8 == 0) 
                { 
                 // printf("\n"); 
                  fprintf(logFile, "\n");
                }
            }
           // printf("\n"); 
            fprintf(logFile, "\n");
        }
        else
        {
            //printf("No Response from one or both RTs!\n");
            fprintf(logFile, "No Response!\n");
        }
    }
    else
    {
        if (no_resp)
        {
           /* printf("Decoded: %u-%u-%c-%u (No Response!)\n",
                   rtAddr, subAddr, dir, wc_or_mode);*/
                   
            fprintf(logFile, "Decoded: %u-%u-%c-%u (No Response!)\n",
                    rtAddr, subAddr, dir, wc_or_mode);
        }
        else
        {
           /* printf("Decoded: %u-%u-%c-%u\n",
                   rtAddr, subAddr, dir, wc_or_mode);*/
            fprintf(logFile, "Decoded: %u-%u-%c-%u\n",
                    rtAddr, subAddr, dir, wc_or_mode);
        }

        temp = wc_or_mode;
        if (subAddr == 0)
            temp = ((temp == 16) || (temp == 17) || (temp == 18)) ? 1 : 0;

        if (buffer.RecCmdWrd.bitwise.TrnRecv == 1)
        {
           ptr++;   /* skip RT status word */
            //ptr += 2;   /* skip block_status + command_word */
            if (!no_resp)
            {
                //printf("Data:\n"); 
                fprintf(logFile, "Data:\n");
                for (int ii = 0; ii < (int)temp; ii++)
                {
                    buffer.DataPack[ii] = MemRead(MTModId, ptr++, 1);
                    //printf("%04X ", buffer.DataPack[ii]);
                    fprintf(logFile, "%04X ", buffer.DataPack[ii]);
                    if ((ii + 1) % 8 == 0) 
                    {
                   // printf("\n"); 
                    fprintf(logFile, "\n");
                    }
                }
               // printf("\n"); 
                fprintf(logFile, "\n");
            }
        }
        else
        {
   
            //printf("Data:\n");
            fprintf(logFile, "Data:\n");
            if (buffer.RecCmdWrd.value == 0xF811) temp = 1;
          //  ptr++;
            for(int ii = 0; ii < (int)temp; ii++)
            {
                buffer.DataPack[ii] = MemRead(MTModId, ptr++, 1);
                //printf("%04X ", buffer.DataPack[ii]);
                fprintf(logFile, "%04X ", buffer.DataPack[ii]);
                if ((ii + 1) % 8 == 0) 
                { 
                //printf("\n"); 
                 fprintf(logFile, "\n");
                }
            }
          //  printf("\n");
            fprintf(logFile, "\n");
        }
    }

    if ((*msgCount % 1000) == 0) fflush(logFile);
}

#if 1
void* StartMTThread(void *arg)
{
    MTThreadArgs  *args    = (MTThreadArgs *)arg;
    unsigned char  MTModId = args->MTModId;
    unsigned short current, last;
    unsigned long  msgCount  = 0;
    unsigned long  skipCount = 0;   /* track skipped messages for debug */
    struct timeval prev_tv;
    gettimeofday(&prev_tv, NULL);
    fd_set         readfds;
    struct timeval tv;

    last = GetMTEOMCnt_dummy(MTModId, CARDID);
    printf("\t MT Thread Started. Initial Ptr = %u\n", last);

    while (!stop_mt)
    {
        /* ── Keypress check ── */
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        tv.tv_sec  = 0;
        tv.tv_usec = 0;
        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0)
        {
            char ch;
            read(STDIN_FILENO, &ch, 1);
            printf("\n\t Key pressed → Stopping MT thread...\n");
            stop_mt = 1;
            break;
        }

        /* ── Normal message processing ── */
        current = GetMTEOMCnt_dummy(MTModId, CARDID);
        if (current != last)
        {
            int diff = (current - last + 1024) % 1024;
            while (diff-- > 0)
            {
                /* Keypress check inside processing loop */
                FD_ZERO(&readfds);
                FD_SET(STDIN_FILENO, &readfds);
                tv.tv_sec  = 0;
                tv.tv_usec = 0;
                if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0)
                {
                    char ch;
                    read(STDIN_FILENO, &ch, 1);
                    printf("\n\t Key pressed → Stopping inside processing\n");
                    stop_mt = 1;
                    break;
                }
                ProcessMTDescriptor(MTModId, last, last, &msgCount, &prev_tv);
                msgCount++;
                last = (last + 1) % 1024;
            }
        }

        /* ── Drain on BC completion ── */
        if (bc_completed)
        {
          //  printf("\t BC completed. Starting drain. msgCount so far = %lu\n", msgCount)
            const int DRAIN_POLL_US   = 200;
            const int IDLE_THRESHOLD  = 25;    /* 25 × 0.2ms = 5ms silence */
            const int HARD_TIMEOUT_US = 2000000;

            int            idle_count = 0;
            int            total_wait = 0;
            unsigned short prev       = last;

            while (total_wait < HARD_TIMEOUT_US)
            {
                current = GetMTEOMCnt_dummy(MTModId, CARDID);

                if (current == prev)
                {
                    idle_count++;
                    if (idle_count >= IDLE_THRESHOLD)
                    {
                       // printf("\t Hardware idle %d ms. Drain complete.\n",
                          //     (IDLE_THRESHOLD * DRAIN_POLL_US) / 1000);
                        break;
                    }
                    usleep(DRAIN_POLL_US);
                    total_wait += DRAIN_POLL_US;
                    continue;
                }

                /* Pointer moved — process all new descriptors */
                idle_count = 0;
                total_wait = 0;
                prev       = current;

                int diff = (current - last + 1024) % 1024;

                /* Only skip if diff is impossibly large (full ring overwrite) */
                if (diff == 0)
                    continue;

                while (diff-- > 0)
                {
                    ProcessMTDescriptor(MTModId, last, last, &msgCount, &prev_tv);
                    msgCount++;
                    last = (last + 1) % 1024;
                }
            }

          //  if (total_wait >= HARD_TIMEOUT_US)
                //printf("\t Drain hard timeout! msgCount = %lu\n", msgCount);
         //   else
                //printf("\t Drain complete. Total MT messages = %lu\n", msgCount);

            break;
        }

        usleep(1);
    }

    /* ── Clean exit ── */
    for(int i=0;i<0x1000;i++)
    {
    //   printf("mt read=%04x  at=%05x\n",MemRead(MTModId,0x21000+i,1),0x21000+i);
    }
    printf("\t MT Reading Over. Total MT messages = %lu\n", msgCount);
    RegWrite(MTModId, 0x0049, 0x2, 1);
    ResetAce(MTModId);
    disableNonCanonicalMode();
    printf("\t MT thread fully exited.\n");
    return NULL;
}
#endif
/* ══════════════════════════════════════════════════════════════════════════
 * DefMajorFrame_new
 * ══════════════════════════════════════════════════════════════════════════ */
int DefMajorFrame_new(UCHAR  Bus,
                       USHORT BCSTACK_A_B,
                       USHORT no_of_minor_frames,
                       MAJOR_FRAME frame_sequence[],int intrmsg)
{
    unsigned int i, m, number_of_messages = 0, framecounter = 0;
    unsigned int msg_address, temp, msg_offset = 0,Intermessage = 0;

    if (Bus < 1 || Bus > 3)                                return -2;
    if (no_of_minor_frames < 1 || no_of_minor_frames > 64) return -24;

    for (m = 0; m < no_of_minor_frames; m++)
    {
        unsigned short Cnt_Time  = 0;
        unsigned short msgCount  = (unsigned short)frame_sequence[m].Mess_Count;
        unsigned short frameTime = (unsigned short)frame_sequence[m].MinorFrame_Time;
        Cnt_Time = (msgCount << 9) | (frameTime & 0x1FF);
        //printf("Frame[%u]: MsgCount=%u Time=%u Cnt_Time=0x%04X\n",
         //      m, msgCount, frameTime, Cnt_Time);
        RegWrite(Bus, 0x61 + m, Cnt_Time, 1);
       // printf("reg read at %04x  %04x \n",0x61+m,RegRead(Bus, 0x61 + m, 1));
    }
   
    RegWrite(Bus, 0x0d, no_of_minor_frames, 1);
    printf("\t Address(0x0d)=%X  no_of_minor_frames=%u\n",
           RegRead(Bus, 0x0d, 1), no_of_minor_frames);

    for (m = 0; m < no_of_minor_frames; m++)
        number_of_messages += frame_sequence[m].Mess_Count;

    if (number_of_messages < 1 || number_of_messages > 1024)
        return -10;

    for (m = 0; m < no_of_minor_frames; m++)
    {
        for (i = 0; i < (unsigned int)frame_sequence[m].Mess_Count; i++)
        {
            temp        = frame_sequence[m].frame_items[i] - 1;
            msg_address = (temp * 40) + 0x4000;//0x5040
            msg_offset  = 0x1000 + (framecounter * 4) + 0x0003;
            Intermessage  = 0x1000 + (framecounter * 4) + 0x0002;
             //printf("Writing 0x%04X to command stack location 0x%04X\n",msg_address,msg_offset);
             MemWrite(Bus, msg_offset, msg_address, 1);
             MemWrite(Bus, Intermessage, intrmsg, 1);
             //printf("Location = 0x%04X  data = 0x%04x\n",msg_offset,MemRead(Bus,msg_offset,1));
            framecounter++;
        }
    }
    MemWrite(Bus, (msg_offset + 0x1000) + 4, END_OF_FRAME, 1);

    return 1;
}

/* ══════════════════════════════════════════════════════════════════════════
 * RunBcMajorFrame_new
 * ══════════════════════════════════════════════════════════════════════════ */
unsigned long RunBcMajorFrame_new(UCHAR  Bus,
                                   USHORT stackA_B,
                                   USHORT no_of_minor_frames,
                                   MAJOR_FRAME frame_sequence[],
                                   unsigned long times,int count,int prior,int dynamic_no)
{

    if (Bus < 1 || Bus > 3)                                return (unsigned long)-2;
    if (no_of_minor_frames < 1 || no_of_minor_frames > 64) return (unsigned long)-24;

    unsigned int CmdStack = 0x1000;
    int          init_pos = stackA_B + (int)CmdStack;
    USHORT       count_bc;
   unsigned short dyn_data[32];
    for (int i = 0; i < 32; i++)
    {
        dyn_data[i] = 0xA5A5;
    }
    if (stackA_B == 0x0)
    {
        if (times == 0)
        {
            RegWrite(Bus, 0x1, 0x54f,  1);
            RegWrite(Bus, 0x3, 0x8000, 1);
            RegWrite(Bus, 0x9, 0x2,    1);
            printf("\t BC Frame Based Continuous Scheduling\n");
        }
        else
        {
            MemWrite(Bus, CmdStack,     init_pos, 1);
            MemWrite(Bus, MESS_COUNTER, times,    1);
            RegWrite(Bus, 0x0d, no_of_minor_frames, 1);
           // RegWrite(Bus, 0x1, 0x045f, 1);
            RegWrite(Bus, 0x3, 0x8000, 1);
            /*printf("\t RegRead 0x0d = %X\n", RegRead(Bus, 0x0d, 1));
            printf("\t RegRead 0x03 = %X\n", RegRead(Bus, 0x03, 1));
            printf("\t RegRead 0x01 = %X\n", RegRead(Bus, 0x01, 1));
            printf("\t RegRead 0x61 = %X\n", RegRead(Bus, 0x61, 1));
            printf("\t RegRead 0x62 = %X\n", RegRead(Bus, 0x62, 1));
            printf("\t RegRead 0x63 = %X\n", RegRead(Bus, 0x63, 1));
            printf("\t RegRead 0x64 = %X\n", RegRead(Bus, 0x64, 1));
            printf("\t RegRead 0x65 = %X\n", RegRead(Bus, 0x65, 1));
            printf("\t RegRead 0x66 = %X\n", RegRead(Bus, 0x66, 1));
            printf("\t RegRead 0x67 = %X\n", RegRead(Bus, 0x67, 1));
            printf("\t RegRead 0x68 = %X\n", RegRead(Bus, 0x68, 1));
            printf("\t RegRead 0x69 = %X\n", RegRead(Bus, 0x69, 1));
            printf("\t RegRead 0x6A = %X\n", RegRead(Bus, 0x6A, 1));*/
            
            if (gp_enable)                     
            {
                GeneralPurposeReg(Bus, gp_frames);
            }

            MemWrite(Bus, MESS_COUNTER, times,    1);
            /*int c;
            while ((c = getchar()) != '\n' && c != EOF); // flush
            printf("Press Enter to start BC...\n");
            getchar();*/
            
            //getchar();getchar();
            RegWrite(Bus, 0x9, 0x2, 1);
            printf("\t Waiting for %lu End of Frames...\n", times);
            //USHORT active_frames =3;  // however many frames GP reg enables
            unsigned long adjusted_times = (times * gp_frames) / no_of_minor_frames;
            unsigned long eofCount = 0;
            int async_triggered=0;
            while (1)
            {
              

                eofCount = GetBCEOFCnt_D(Bus, CARDID);
                int eom;
                eom=GetBCEOMCnt(Bus, CARDID);

                if(eom == count-2 && !async_triggered)
                { 

                   if(prior==1)
                   {

                      RegWrite(Bus,0x017,0x0101,1);
                   }
                   else if(prior==0)
                   {
                     
                      RegWrite(Bus,0x018,0x0101,1);
                   }
                    async_triggered = 1;
                    printf("\t Async triggered at EOM=%d\n", eom);
                }
               
                if(eom == dynamic_no-2)
                {
                   UpdateMsgData(Bus, dynamic_no, dyn_data, 32, RECEIVE);
                   //UpdateMsgCmdData(Bus,dynamic_no,0x820,0,dyn_data,32,RECEIVE,1);       // RECEIVE / TRANSMIT / RT_RT / MODE_CODE 
                }
                 if (gp_enable)
                 {
                     if (eofCount >= adjusted_times) //times
                        {
                            printf("\t BC EOF count reached: %lu / %lu\n", eofCount, times);
                            break;
                        }
                 }
                 else 
                  {
                      if (eofCount >= times) //times
                      {
                          printf("\t BC EOF count reached: %lu / %lu\n", eofCount, times);
                          break;
                      }
                  }
                
                count_bc = RegRead(Bus, 0x1, 1);
                if ((count_bc & 2) == 0)
                {
                    printf("\t BC stopped early. EOF: %lu / %lu\n", eofCount, times);
                    break;
                }
                usleep(100);
            }
            
            printf("\t Frame Based Scheduling Completed\n");
        }
    }
   
    return 1;
}

static void ResetCounters(int bcMod, int mtMod)
{
    SetBCSOMCnt(bcMod, 0, CARDID);
    SetBCEOMCnt(bcMod, 0, CARDID);
    SetBCEOFCnt(bcMod, 0, CARDID);
    SetBCEOMMINORFRAMECnt(bcMod, 0, CARDID);
    SetMTSOMCnt(mtMod, 0, CARDID);
    SetMTEOMCnt(mtMod, 0, CARDID);
}

/* ══════════════════════════════════════════════════════════════════════════
 * HELPER – wait for user keypress to return to menu after a test
 * ══════════════════════════════════════════════════════════════════════════ */
static void WaitForKeyAndReturn(int bcMod, int mtMod, int rtMod)
{
    // mtThread already joined in each case block — don't join again
    printf("\n\t Press Enter to return to menu...\n");
    getchar();
    if (bcMod > 0) ResetAce(bcMod);
    if (mtMod > 0) ResetAce(mtMod);
    if (rtMod > 0) ResetAce(rtMod);
}

int main(void)
{
    int BcMod   = BC_MOD;
    int mtModId = MT_MOD;
    int RtModId = RT_MOD;

    int choice, channel, NoOfTimes;
    int rt_addr, sub_addr, word_count;
    int tx_rt, tx_sa, rx_rt, rx_sa;
    int mc_sa, mc_wc;

    MTThreadArgs mtArgs;
    ClearRam(1);
    ClearRam(2);
    ClearRam(3);
    signal(SIGINT, handle_sigint);

    /* ── Open log files ── */
    logFile = fopen("unified_mt_log.txt", "w");
    if (!logFile)   { perror("unified_mt_log.txt");   return -1; }
    MtlogFile = fopen("unified_mt_log2.txt", "w");
    if (!MtlogFile) { perror("unified_mt_log2.txt");  return -1; }
    RtlogFile = fopen("unified_rt_log.txt", "w");
    if (!RtlogFile) { perror("unified_rt_log.txt");   return -1; }

    /* ── Init card ── */
    if (!InitCard(CARDID))
    {
        printf("\n\tINVALID CARD ID OR CARD MAY NOT BE PRESENT\n\n");
        fclose(logFile); fclose(MtlogFile); fclose(RtlogFile);
        return -1;
    }

    printf("\n***** MIL-STD-1553 Unified Test *****\n");
#if 1
    InitInterrupts(BcMod,   CONFIG_REG_INIT_VAL, CARDID);
    InitInterrupts(mtModId, CONFIG_REG_INIT_VAL, CARDID);
    InitInterrupts(RtModId, CONFIG_REG_INIT_VAL, CARDID);
    getchar();   /* press ENTER to proceed */
    
    EnableInterruptsBC(BcMod,   ENABLE_BC_INT, CARDID);
    EnableInterruptsMT(mtModId, ENABLE_MT_INT, CARDID);
    EnableInterruptsRT(RtModId, ENABLE_RT_INT, CARDID);
    #endif 

    SetBCSOMCnt(BcMod, 0, CARDID);
    SetBCEOMCnt(BcMod, 0, CARDID);
    SetBCEOFCnt(BcMod, 0, CARDID);
    SetBCEOMMINORFRAMECnt(BcMod, 0, CARDID);
    SetMTSOMCnt(mtModId, 0, CARDID);
    SetMTEOMCnt(mtModId, 0, CARDID);
    SetRTSOMCnt(RtModId, 0, CARDID);
    SetRTEOMCnt(RtModId, 0, CARDID);
    getchar();   /* press ENTER to proceed */

    ResetAce(1); ResetAce(2); ResetAce(3); ResetAce(4);
    RegWrite(2, 0x00a2, 0x0001, 1);

    /* ══════════════════════════════════════
     * MAIN MENU
     * ══════════════════════════════════════ */
    while (1)
    {
        
        printf("   \t MIL-STD-1553 Test Menu     \n\n");
        printf("  \t 1. BC → RT  \n");
        printf("  \t 2. RT → BC    \n");
        printf("  \t 3. RT → RT     \n");
        printf("  \t 4. Mode Code  \n");
        printf("  \t 5. Broadcast  \n");
        printf("  \t 6. RT Broadcast  \n");
        printf("  \t 7. RT   \n");
        printf("  \t 8. MT   \n");
        printf("  \t 9. RT StatusBits  \n");
        printf("  \t 0. Exit     \n");
        //DMACPUBUFFER(BcMod,1);
        printf("\n\t Enter Your Choice: ");
        scanf("%d", &choice);
        if (choice == 0) break;
                       
        switch (choice)
        {
            /* ────────────────────────────────────────────
             * CASE 1 : BC → RT */
            case 1:
                /* Reset stop flags before every run */
                bc_completed = 0;
                stop_mt      = 0;
                bc_done      = 0;

                /*get_user_config(&enable_async, &Dynamic_data, &BcRetryEnable, &enable, &intr,
                &msg_no, &prior, &count_no, &count, &dynamic_no, &count1,
                &intr_msg_no, &gap, &gp_enable, &gp_frames);*/
                printf("\n\t--- BC→RT Test ---\n");
                printf("\t Channel (CHA=1 / CHB=0): ");    scanf("%d", &channel);
                printf("\t Number of times to run: ");     scanf("%d", &NoOfTimes);
                printf("\t RT address (1-30): ");          scanf("%d", &rt_addr);
                printf("\t Sub-address (1-30): ");         scanf("%d", &sub_addr);
                printf("\t Word count (1-32): ");          scanf("%d", &word_count);
                if (word_count < 1 || word_count > 32) word_count = 1;        
                BC_MT_Mode_snode(BcMod, mtModId, channel, NoOfTimes);
                //BC_MT_Mode(BcMod, mtModId, channel, NoOfTimes);
                ResetCounters(BcMod, mtModId);

                mtArgs.MTModId          = BcMod;
                mtArgs.BCModId          = BcMod;
                //mtArgs.expectedMessages = (unsigned long)NoOfTimes * MSGS_PER_FRAME_BCRT;
                SetMTEOMCnt(mtModId, 0, CARDID);
                Async(BcMod, mtModId,channel, NoOfTimes, msg_no, prior);
                pthread_create(&mtThread, NULL, StartMTThread, &mtArgs);
                BCSchedule_BCRT(NoOfTimes, BcMod, channel, (unsigned char)rt_addr,  (unsigned char)sub_addr,
                      (unsigned char)word_count,count,prior,dynamic_no,BcRetryEnable,enable,intr_msg_no,gap);
                  
                      
                
                
                /* Signal MT thread that BC is done — it will drain and exit */
                bc_completed = 1;
                bc_done      = 1;
                printf("\t RegRead 0x04 = %X\n", RegRead(BcMod, 0x04, 1));
                pthread_join(mtThread, NULL);
                bc_completed = 0;
                stop_mt      = 0;
                ResetAce(mtModId); ResetAce(BcMod);
                SetMTEOMCnt(mtModId, 0, CARDID);
                disableNonCanonicalMode();

                printf("\t BC→RT test complete.\n");
                ClearRam(1);
                ClearRam(2);
                ClearRam(3);
                
                break;
#if 1
            /* ────────────────────────────────────────────
             * CASE 2 : RT → BC */
                case 2:
                bc_completed = 0;
                stop_mt      = 0;
                bc_done      = 0;
                
                /*get_user_config(&enable_async, &Dynamic_data, &BcRetryEnable, &enable, &intr,
                &msg_no, &prior, &count_no, &count, &dynamic_no, &count1,
                &intr_msg_no, &gap, &gp_enable, &gp_frames);*/
                
                printf("\n\t--- RT→BC Test ---\n");
                printf("\t Channel (CHA=1 / CHB=2): ");    scanf("%d", &channel);
                printf("\t Number of frames to run: ");    scanf("%d", &NoOfTimes);
                printf("\t RT address (1-30): ");          scanf("%d", &rt_addr);
                printf("\t Sub-address (1-30): ");         scanf("%d", &sub_addr);
                printf("\t Word count (1-32): ");          scanf("%d", &word_count);
                if (word_count < 1 || word_count > 32) word_count = 1;
         
                // ── Step 1: configure the RT hardware ──
                BC_MT_Mode_snode(BcMod, mtModId, channel, NoOfTimes);
                
                // ── Step 3: counters & MT thread ──
                ResetCounters(BcMod, mtModId);
                
         
                mtArgs.MTModId = BcMod;
                mtArgs.BCModId = BcMod;
                SetMTEOMCnt(mtModId, 0, CARDID);
                SetRTEOMCnt(RtModId, 0, CARDID);
                Async(BcMod, mtModId, channel, NoOfTimes, msg_no, prior);
                pthread_create(&mtThread, NULL, StartMTThread, &mtArgs);
         
                // ── Step 4: run the BC ──
                BCSchedule_RTBC(NoOfTimes, BcMod, channel,
                                (unsigned char)rt_addr,
                                (unsigned char)sub_addr,
                                (unsigned char)word_count,
                                count, prior, dynamic_no,BcRetryEnable,enable,intr_msg_no,gap);
         
                bc_completed = 1;
                bc_done      = 1;
         
                pthread_join(mtThread, NULL);
                bc_completed = 0;
                stop_mt      = 0;
                ResetAce(RtModId); ResetAce(mtModId); ResetAce(BcMod);
                SetMTEOMCnt(mtModId, 0, CARDID);
                SetRTEOMCnt(RtModId, 0, CARDID);
                disableNonCanonicalMode();
                
                printf("\t RT→BC test complete.\n");
                
                break;
           #endif
            /* ────────────────────────────────────────────
             * CASE 3 : RT → RT */
            case 3:
                bc_completed = 0;
                stop_mt      = 0;
                bc_done      = 0;
                /*get_user_config(&enable_async, &Dynamic_data, &BcRetryEnable, &enable, &intr,
                &msg_no, &prior, &count_no, &count, &dynamic_no, &count1,
                &intr_msg_no, &gap, &gp_enable, &gp_frames);*/
                printf("\n\t--- RT→RT Test ---\n");
                printf("\t Channel (CHA=1 / CHB=2): ");           scanf("%d", &channel);
                printf("\t Number of frames to run: ");           scanf("%d", &NoOfTimes);
                printf("\t TX RT address (1-30): ");              scanf("%d", &tx_rt);
                printf("\t TX Sub-address (1-30): ");             scanf("%d", &tx_sa);
                printf("\t RX RT address (1-30, != TX addr): ");  scanf("%d", &rx_rt);
                printf("\t RX Sub-address (1-30): ");             scanf("%d", &rx_sa);
                printf("\t Word count (1-32): ");                 scanf("%d", &word_count);
                if (word_count < 1 || word_count > 32) word_count = 1;

                if (tx_rt == rx_rt)
                {
                    printf("\t ERROR: TX and RX RT addresses must differ!\n");
                    break;
                }
                BC_MT_Mode_snode(BcMod, mtModId, channel, NoOfTimes);
                ResetCounters(BcMod, mtModId);
                SetMTEOMCnt(mtModId, 0, CARDID);

                mtArgs.MTModId          = BcMod;
                mtArgs.BCModId          = BcMod;
              //  mtArgs.expectedMessages = (unsigned long)NoOfTimes * MSGS_PER_FRAME_RTRT;
                Async(BcMod, mtModId, channel, NoOfTimes, msg_no, prior);
                pthread_create(&mtThread, NULL, StartMTThread, &mtArgs);

                BCSchedule_RTRT(NoOfTimes, BcMod, channel,
                                (unsigned char)tx_rt, (unsigned char)tx_sa,
                                (unsigned char)rx_rt, (unsigned char)rx_sa,
                                (unsigned char)word_count,count,prior,dynamic_no,BcRetryEnable,enable,intr_msg_no,gap);

                bc_completed = 1;
                bc_done      = 1;

                pthread_join(mtThread, NULL);
                bc_completed = 0;
                stop_mt      = 0;
                ResetAce(RtModId); ResetAce(mtModId); ResetAce(BcMod);
                SetMTEOMCnt(mtModId, 0, CARDID);
                SetRTEOMCnt(RtModId, 0, CARDID);
                disableNonCanonicalMode();
                printf("\t RT→RT test complete.\n");
                break;

            /* ────────────────────────────────────────────
             * CASE 4 : Mode Code */
            case 4:
                bc_completed = 0;
                stop_mt      = 0;
                bc_done      = 0;

                printf("\n\t--- Mode Code Test ---\n");
                printf("\t Channel (CHA=1 / CHB=2): ");   scanf("%d", &channel);
                printf("\t Number of frames to run: ");    scanf("%d", &NoOfTimes);
                printf("\t RT address: ");                 scanf("%d", &rt_addr);
                printf("\t Sub-address (0 or 31): ");      scanf("%d", &mc_sa);
                printf("\t Mode code value (e.g. 17): ");  scanf("%d", &mc_wc);

                BC_MT_Mode_snode(BcMod, mtModId, channel, NoOfTimes);
                ResetCounters(BcMod, mtModId);

                mtArgs.MTModId          = BcMod;
                mtArgs.BCModId          = BcMod;
                mtArgs.expectedMessages = (unsigned long)NoOfTimes * MSGS_PER_FRAME_MC;
                Async(BcMod, mtModId, channel, NoOfTimes, msg_no, prior);
                pthread_create(&mtThread, NULL, StartMTThread, &mtArgs);

                BCSchedule_MCode(NoOfTimes, BcMod, channel,
                              (unsigned char)rt_addr,
                              (unsigned char)mc_sa,
                              (unsigned char)mc_wc,count,prior,dynamic_no);

                bc_completed = 1;
                bc_done      = 1;

                pthread_join(mtThread, NULL);
                bc_completed = 0;
                stop_mt      = 0;
                ResetAce(mtModId); ResetAce(BcMod);
                SetMTEOMCnt(mtModId, 0, CARDID);
                disableNonCanonicalMode();
                printf("\t Mode Code test complete.\n");
                break;

            /* ────────────────────────────────────────────
             * CASE 5 : Broadcast */
            case 5:
                bc_completed = 0;
                stop_mt      = 0;
                bc_done      = 0;
                
                /*get_user_config(&enable_async, &Dynamic_data, &BcRetryEnable, &enable, &intr,
                &msg_no, &prior, &count_no, &count, &dynamic_no, &count1,
                &intr_msg_no, &gap, &gp_enable, &gp_frames);*/
                
                printf("\n\t--- Broadcast Test ---\n");
                printf("\t Channel (CHA=1 / CHB=2): ");   scanf("%d", &channel);
                printf("\t Number of frames to run: ");    scanf("%d", &NoOfTimes);
                printf("\t Sub-address (1-30): ");         scanf("%d", &sub_addr);
                printf("\t Word count (1-32): ");          scanf("%d", &word_count);
                if (word_count < 1 || word_count > 32) word_count = 1;

                BC_MT_Mode_snode(BcMod, mtModId, channel, NoOfTimes);
                ResetCounters(BcMod, mtModId);

                mtArgs.MTModId          = BcMod;
                mtArgs.BCModId          = BcMod;
              //  mtArgs.expectedMessages = (unsigned long)NoOfTimes * MSGS_PER_FRAME_BCAST;
                Async(BcMod, mtModId,channel, NoOfTimes, msg_no, prior);
                pthread_create(&mtThread, NULL, StartMTThread, &mtArgs);

                BCSchedule_BCAST(NoOfTimes, BcMod, channel,
                                 (unsigned char)sub_addr,
                                 (unsigned char)word_count,count,prior,dynamic_no,BcRetryEnable);

                bc_completed = 1;
                bc_done      = 1;

                pthread_join(mtThread, NULL);
                bc_completed = 0;
                stop_mt      = 0;
                ResetAce(mtModId); ResetAce(BcMod);
                SetMTEOMCnt(mtModId, 0, CARDID);
                disableNonCanonicalMode();
                printf("\t Broadcast test complete.\n");
                break;
                
            case 6:
                bc_completed = 0;
                stop_mt      = 0;
                bc_done      = 0;
                
                /*get_user_config(&enable_async, &Dynamic_data, &BcRetryEnable, &enable, &intr,
                &msg_no, &prior, &count_no, &count, &dynamic_no, &count1,
                &intr_msg_no, &gap, &gp_enable, &gp_frames);*/
                
                printf("\t Enter channel (CHA=1 / CHB=2): ");            scanf("%d", &channel);
                printf("\t Number of times to run: ");                   scanf("%d", &NoOfTimes);
                printf("\t Enter TX RT address (1-30): ");               scanf("%d", &tx_rt);
                printf("\t Enter TX Sub-address (1-30): ");              scanf("%d", &tx_sa);
                printf("\t Enter RX RT address (31): ");                 scanf("%d", &rx_rt);
                printf("\t Enter RX Sub-address (1-30): ");              scanf("%d", &rx_sa);
                printf("\t Enter Word count (1-32): ");                  scanf("%d", &word_count);
            if (word_count < 1 || word_count > 32) word_count = 1;

            if (tx_rt == rx_rt)
            { 
              printf("\t ERROR: TX and RX RT addresses must be different!\n"); break; 
            }
            ConfigureRT(RtModId,tx_rt);
            ConfigureRT(RtModId,rx_rt);
            BC_MT_Mode_snode(BcMod, mtModId, channel, NoOfTimes);
            SetBCSOMCnt(BcMod, 0, CARDID); SetBCEOMCnt(BcMod, 0, CARDID);
            SetBCEOFCnt(BcMod, 0, CARDID); SetBCEOMMINORFRAMECnt(BcMod, 0, CARDID);
            SetMTEOMCnt(mtModId, 0, CARDID);
            mtArgs.MTModId = BcMod; 
            mtArgs.BCModId = BcMod;
            //mtArgs.expectedMessages = (unsigned long)NoOfTimes;
            
            Async(BcMod, mtModId,channel, NoOfTimes, msg_no, prior);
            pthread_create(&mtThread, NULL, StartMTThread, &mtArgs);

            /* Step 5: Run BC */
            printf("\n\t[STEP 5] Starting BC (RT→RT command)...\n");
            BCSchedule_RTRT_BCST(NoOfTimes, (unsigned char)BcMod,
                                (unsigned char)channel,
                                (unsigned char)tx_rt, (unsigned char)tx_sa,
                                (unsigned char)rx_rt, (unsigned char)rx_sa,
                                (unsigned char)word_count,count,prior,dynamic_no);
             bc_completed = 1;
             bc_done      = 1;

             pthread_join(mtThread, NULL);
             bc_completed = 0;
                stop_mt      = 0;
             ResetAce(RtModId); ResetAce(mtModId); ResetAce(BcMod);
             SetMTEOMCnt(mtModId, 0, CARDID);
             SetRTEOMCnt(RtModId, 0, CARDID);
             disableNonCanonicalMode();
             printf("\t RT→RT BC test complete.\n");
             break;   
            /* Case 2 — clean version */
        case 7:
            bc_completed = 0;
            stop_mt      = 0;
            bc_done      = 0;

            printf("\n\t--- RT Test (External BC mode) ---\n");
            printf("\t Channel (CHA=1 / CHB=2): ");  scanf("%d", &channel);
            printf("\t RT address (1-30): ");         scanf("%d", &rt_addr);

            RT_MT_Mode_snode(RtModId, rt_addr);

            
            /* Drain ALL pending stdin before thread starts — critical */
            printf("\t Press ENTER to start MT monitoring...\n");
            tcflush(STDIN_FILENO, TCIFLUSH);
            getchar();
            tcflush(STDIN_FILENO, TCIFLUSH);  /* flush again after getchar */

            ResetCounters(BcMod, RtModId);
            SetMTEOMCnt(RtModId, 0, CARDID);
            SetRTEOMCnt(RtModId, 0, CARDID);

            mtArgs.MTModId = RtModId;
            mtArgs.BCModId = RtModId;

            pthread_create(&mtThread, NULL, StartMTThread, &mtArgs);

            //printf("MT monitoring active. External BC send now.\n");
            printf("\t Press any key + ENTER in this terminal to stop.\n");

            /* Main thread just waits — MT thread owns stdin for keypress detection */
            pthread_join(mtThread, NULL);

            
            printf("\t Final RT_EOM = ");
            GetRTEOMCnt(RtModId, CARDID);

            ResetAce(RtModId);   /* reset HERE, not inside thread */
            SetMTEOMCnt(RtModId, 0, CARDID);
            SetRTEOMCnt(RtModId, 0, CARDID);
            disableNonCanonicalMode();
            printf("\t RT→BC test complete.\n");
            break;
            case 8:
            bc_completed = 0;
            stop_mt      = 0;
            bc_done      = 0;

            printf("\n\t---mt mode ---\n");
            MT_Mode(mtModId);

            
            /* Drain ALL pending stdin before thread starts — critical */
            printf("\t Press ENTER to start MT monitoring...\n");
            tcflush(STDIN_FILENO, TCIFLUSH);
            getchar();
            tcflush(STDIN_FILENO, TCIFLUSH);  /* flush again after getchar */

            ResetCounters(BcMod, RtModId);
            SetMTEOMCnt(RtModId, 0, CARDID);
            SetRTEOMCnt(RtModId, 0, CARDID);

            mtArgs.MTModId = mtModId;
            mtArgs.BCModId = mtModId;

            pthread_create(&mtThread, NULL, StartMTThread, &mtArgs);

            //printf("MT monitoring active. External BC send now.\n");
            printf("\t Press any key + ENTER in this terminal to stop.\n");
            
            

            /* Main thread just waits — MT thread owns stdin for keypress detection */
            pthread_join(mtThread, NULL);
            ResetAce(RtModId);   /* reset HERE, not inside thread */
            SetMTEOMCnt(RtModId, 0, CARDID);
            SetRTEOMCnt(RtModId, 0, CARDID);
            
            disableNonCanonicalMode();
            printf("\t RT→BC test complete.\n");
            Read_DMA();
            break;
             case 9:
                int RtStatusBits;
                printf("enter the rt address:");
                scanf("%d",&rt_addr);
                printf("enter rt status bit:");
                scanf("%d",&RtStatusBits);
                RT_MT_Mode_snode(RtModId, rt_addr);
                switch (RtStatusBits)
                {
                    case 1:
                       
                        SetSvcReq(RtModId);
                        printf("\n\tService Request set. Press ENTER to reset.\n");
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);  // clear buffer
                        getchar();  
                        ResetSvcReq(RtModId);
                        printf("\tService Request Flag Reset.\n");
                        break;

                    case 2:
                        SetBusy(RtModId);
                        printf("\n\tBusy flag set. Press ENTER to reset.\n");              
                        while ((c = getchar()) != '\n' && c != EOF);  // clear buffer
                        getchar();  
                        ResetBusy(RtModId);
                        printf("\tBusy Flag Reset.\n");
                        break;

                    case 3:
                        SetSsflag(RtModId);
                        printf("\n\tSubsystem Flag set. Press ENTER to reset.\n");  
                        while ((c = getchar()) != '\n' && c != EOF);  // clear buffer
                        getchar(); 
                        ResetSsflag(RtModId);
                        printf("\tSubsystem Flag Reset.\n");
                        break;

                    case 4:
                        SetDba(RtModId);
                        printf("\n\tDBA Flag set. Press ENTER to reset.\n");                      
                        while ((c = getchar()) != '\n' && c != EOF);  // clear buffer
                        getchar();  
                        ResetDba(RtModId);
                        printf("\tDBA Flag Reset.\n");
                        break;

                    case 5:
                        SetTerFlag(RtModId);
                        printf("\n\tTerminal Flag set. Press ENTER to reset.\n");                       
                        while ((c = getchar()) != '\n' && c != EOF);  // clear buffer
                        getchar();  
                        ResetTerFlag(RtModId);                    
                        RegWrite(RtModId, 0x23, 0x8000, 1);
                        printf("\tTerminal Flag Reset.\n");
                        break;

                    default:
                        printf("\n\tInvalid option! Please enter 1–5.\n");
                        break;
                  }
                        
                disableNonCanonicalMode();       
                break;
                    default:
                        printf("\n\tInvalid choice.\n");
                        break;
                }

                /* ── Wait for keypress then return to menu ── */
                WaitForKeyAndReturn(BcMod, mtModId, RtModId);
            }

            printf("\n\t******** OVER ********\n");
            DisableInterruptsBC(1, DISABLE, CARDID);
            DisableInterruptsMT(1, DISABLE, CARDID);
            DisableInterruptsRT(1, DISABLE, CARDID);
            
            DisableInterruptsBC(2, DISABLE, CARDID);
            DisableInterruptsMT(2, DISABLE, CARDID);
            DisableInterruptsRT(2, DISABLE, CARDID);
            

            DisableInterruptsBC(3, DISABLE, CARDID);
            DisableInterruptsMT(3, DISABLE, CARDID);
            DisableInterruptsRT(3, DISABLE, CARDID);
            
            pthread_mutex_destroy(&mutexObj);
            fclose(logFile);
            fclose(MtlogFile);
            fclose(RtlogFile);
            printf("\t Program terminated normally.\n");
            return 0;
}
