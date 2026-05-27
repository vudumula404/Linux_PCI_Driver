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

#include "/home/ccpmc/Desktop/APOORVA_CCPMC_DMA_Interrupt_Handler/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define BUS_A                0
#define BUS_B                1
#define RECEIVE              0
#define TRANSMIT             1
#define YES                  1
#define NO                   0
#define CARD1                1
#define CONFIG_REG_INIT_VAL  0x00000078
#define ENABLE_BC_INT        0x00004029
#define ENABLE_MT_INT        0x00004001

/* ── Ring-buffer constants ── */
#define MT_RING_SIZE         4096
#define MT_OVERRUN_THRESHOLD 2048

typedef unsigned char  BYTE;
typedef unsigned long  DWORD;
typedef unsigned short UBIT16;
typedef short          SBIT16;
typedef void*          HANDLE;
typedef long long      LARGE_INTEGER;
typedef int            errno_t;

typedef struct {
    unsigned char  MTModId;
    unsigned char  BCModId;
    unsigned long  expectedMessages;
} MTThreadArgs;

/* ── Globals ── */
pthread_t       thread;
pthread_t       mtThread;
pthread_mutex_t mutexObj      = PTHREAD_MUTEX_INITIALIZER;
volatile int    bc_done       = 0;
int             ThreadRunFlag = 0;

MSG_DEF         bc_page;
MAJOR_FRAME     major_frames[128];
unsigned short  data_buffer[40];

FILE *logFile   = NULL;
FILE *MtlogFile = NULL;
FILE *RtlogFile = NULL;

/* ── Forward declarations ── */
void  BCSchedule_ModeCode17(int Times, unsigned char module_id,
                             unsigned char channel, unsigned char rt_addr,unsigned char sub_addr,
                                        unsigned char w_cnt);
void* StartMTThread(void* arg);
void  ProcessMTDescriptor(unsigned char MTModId, unsigned short desc,
                           unsigned short pos, unsigned long *msgCount,
                           struct timeval *prev_tv);
int   DefMajorFrame_new(UCHAR Bus, USHORT BCSTACK_A_B, USHORT no_of_minor_frames,
                         MAJOR_FRAME frame_sequence[]);
unsigned long RunBcMajorFrame_new(UCHAR Bus, USHORT stackA_B, USHORT no_of_minor_frames,
                                   MAJOR_FRAME frame_sequence[], unsigned long times);

/* =========================================================================
 * SIGNAL HANDLER
 * ======================================================================= */
void handle_sigint(int sig)
{
    printf("\nExiting...\n");
    if (logFile)   fclose(logFile);
    if (MtlogFile) fclose(MtlogFile);
    if (RtlogFile) fclose(RtlogFile);
    exit(0);
}

void BCSchedule_ModeCode17(int Times, unsigned char module_id,
                            unsigned char channel, unsigned char rt_addr,unsigned char sub_addr,unsigned char w_cnt)
{
    unsigned short inter_msg_gap = 20;
    printf("\tBus      : %s\n", (channel == 1) ? "A" : "B");
    printf("\tFrames   : %d\n\n", Times);

    memset(&bc_page, 0, sizeof(bc_page));

    bc_page.bus            = (channel == 1) ? BUS_A : BUS_B;
    bc_page.self_test      = NO;
    bc_page.mask_bcst_bit  = NO;
    bc_page.broadcast      = YES;         /* unicast to specific RT           */
    bc_page.mode_code      = YES;        /* ★ mode-code message              */
    bc_page.rt_rt          = NO;
    bc_page.loop           = 0x0000;
    bc_page.cmd_1.status   = 0xCCCC;
    bc_page.cmd_1.taddr    = rt_addr;    /* specific RT address              */
    bc_page.cmd_1.subaddr  = sub_addr;   /* 0 → mode-code             */
    bc_page.cmd_1.wcnt     = w_cnt;  /* 17 = Sync with Data Word  */
    bc_page.cmd_1.t_r      = RECEIVE;    /* BC→RT: RT receives               */

    memset(data_buffer, 0, sizeof(data_buffer));
    data_buffer[0] = 0x0001;             /* sync data word                   */

    DefMsg(module_id, 1, bc_page, data_buffer, w_cnt, inter_msg_gap);

    major_frames[0].frame_items[0]  = 1;
    major_frames[0].Mess_Count      = 1;
    major_frames[0].MinorFrame_Time = 20;

    DefMajorFrame_new(module_id, 0, 1, major_frames);

    printf("\tBC Mode Code Scheduling Started – waiting...\n");

    RunBcMajorFrame_new(module_id, 0, 1, major_frames, (unsigned long)Times);

    printf("reg read mt %04x-%d\n", 0x00F9, RegRead(1, 0x00F9, 1));
    printf("reg read mt %04x-%d\n", 0x00FB, RegRead(1, 0x00FB, 1));
    printf("reg read mt %04x-%d\n", 0x00F1, RegRead(1, 0x00F1, 1));
    printf("reg read mt %04x-%d\n", 0x00F3, RegRead(1, 0x00F3, 1));

    GetBCSOMCnt(2,  CARD1);
    GetBCEOMCnt(2,  CARD1);
    GetBCEOFCnt(2,  CARD1);
    GetBCEOMINFRAMECnt(2, CARD1);

    GetMTSOMCnt(2, CARD1);
    GetMTEOMCnt(2, CARD1);

    printf("\tPress Enter to Reset BC...\n");
    getchar(); getchar();
    ResetAce(module_id);
    printf("\tBC Reset – Success!\n");
}

/* =========================================================================
 * ProcessMTDescriptor
 * ======================================================================= */
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
    buffer.RecCmdWrd.value     = MemRead(MTModId, Offset + 3, 1);

    cmd        = buffer.RecCmdWrd.value;
    rtAddr     = (cmd >> 11) & 0x1F;
    trBit      = (cmd >> 10) & 0x01;
    subAddr    = (cmd >> 5)  & 0x1F;
    wc_or_mode = cmd & 0x1F;
    if (wc_or_mode == 0) wc_or_mode = 32;
    dir = (trBit == 1) ? 'T' : 'R';

    gettimeofday(&tv, NULL);
    t = localtime(&tv.tv_sec);

    if (buffer.BlockSts.value == 0x0000 || buffer.RecCmdWrd.value == 0x0000)
        return;

    printf("\nTime [%02d:%02d:%02d.%06ld] Date [%02d-%02d-%04d] Msg#%lu\n",
            t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec,
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, *msgCount);
    printf("BlkSts=0x%04X  Cmd=0x%04X\n",
            buffer.BlockSts.value, buffer.RecCmdWrd.value);
    *prev_tv = tv;

    int is_rt_to_rt = (buffer.BlockSts.value & 0x0800) ? 1 : 0;
    int no_resp     = (buffer.BlockSts.value & 0x0200) ? 1 : 0;

    if (is_rt_to_rt)
    {
        unsigned short secondCmd = MemRead(MTModId, ptr,     1);
        unsigned short txStatus  = MemRead(MTModId, ptr + 1, 1);
        unsigned short rt_tx     = (secondCmd >> 11) & 0x1F;
        unsigned short sa_tx     = (secondCmd >> 5)  & 0x1F;
        unsigned short wc_tx     = secondCmd & 0x1F;

        if (no_resp)
        {
            printf("Decoded RT->RT: %u-%u-X (No Response!) 2ndCmd=0x%04X TxRT=%u SA=%u TxSts=0x%04X\n",
                    rtAddr, subAddr, secondCmd, rt_tx, sa_tx, txStatus);
        }
        else
        {
            if (wc_tx == 0) wc_tx = 32;
            printf("Decoded: %u-%u-X-%u  2ndCmd=0x%04X TxRT=%u SA=%u TxSts=0x%04X\n",
                    rtAddr, subAddr, wc_tx, secondCmd, rt_tx, sa_tx, txStatus);
            ptr += 2;
            printf("Data (RT->RT):\n");
            for (int ii = 0; ii < (int)wc_tx; ii++)
            {
                buffer.DataPack[ii] = MemRead(MTModId, ptr++, 1);
                printf("%04X ", buffer.DataPack[ii]);
                if ((ii + 1) % 8 == 0) printf("\n");
            }
            printf("\n");
        }
    }
    else
    {
        if (no_resp)
            printf("Decoded: %u-%u-%c-%u (No Response!)\n",
                    rtAddr, subAddr, dir, wc_or_mode);
        else
            printf("Decoded: %u-%u-%c-%u\n",
                    rtAddr, subAddr, dir, wc_or_mode);

        temp = wc_or_mode;
        if (subAddr == 0)
        {
            temp = ((temp == 16) || (temp == 17) || (temp == 18)) ? 1 : 0;
        }

        if (buffer.RecCmdWrd.bitwise.TrnRecv == 1)
        {
            ptr++;
            if (!no_resp)
            {
                printf("Data:\n");
                for (int ii = 0; ii < (int)temp; ii++)
                {
                    buffer.DataPack[ii] = MemRead(MTModId, ptr++, 1);
                    printf("%04X ", buffer.DataPack[ii]);
                    if ((ii + 1) % 8 == 0) printf("\n");
                }
                printf("\n");
            }
        }
        else
        {
            printf("Data:\n");
            if (buffer.RecCmdWrd.value == 0xF811) temp = 1;
            for (int ii = 0; ii < (int)temp; ii++)
            {
                buffer.DataPack[ii] = MemRead(MTModId, ptr++, 1);
                printf("%04X ", buffer.DataPack[ii]);
                if ((ii + 1) % 8 == 0) printf("\n");
            }
            printf("\n");
        }
    }

    if ((*msgCount % 1000) == 0) fflush(logFile);
}
/* =========================================================================
 * StartMTThread
 * ======================================================================= */
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

/* =========================================================================
 * DefMajorFrame_new
 * ======================================================================= */
int DefMajorFrame_new(UCHAR  Bus, USHORT BCSTACK_A_B,
                       USHORT no_of_minor_frames, MAJOR_FRAME frame_sequence[])
{
    unsigned int i, m, number_of_messages = 0, framecounter = 0;
    unsigned int msg_address, temp, msg_offset;

    if (Bus < 1 || Bus > 3)                                return -2;
    if (no_of_minor_frames < 1 || no_of_minor_frames > 64) return -24;

    for (m = 0; m < no_of_minor_frames; m++)
    {
        unsigned short Cnt_Time  = 0;
        unsigned short msgCount  = (unsigned short)frame_sequence[m].Mess_Count;
        unsigned short frameTime = (unsigned short)frame_sequence[m].MinorFrame_Time;
        Cnt_Time = (msgCount << 9) | (frameTime & 0x1FF);
        printf("Frame[%d]: MsgCount=%d Time=%d Cnt_Time=0x%04x\n",
               m, msgCount, frameTime, Cnt_Time);
        RegWrite(Bus, 0x61 + m, Cnt_Time, 1);
    }

    RegWrite(Bus, 0x0d, no_of_minor_frames, 1);

    for (m = 0; m < no_of_minor_frames; m++)
        number_of_messages += frame_sequence[m].Mess_Count;

    if (number_of_messages >= 1 && number_of_messages <= 1024)
    {
        for (m = 0; m < no_of_minor_frames; m++)
        {
            for (i = 0; i < 10; i++)
            {
                temp        = frame_sequence[m].frame_items[i] - 1;
                msg_address = (temp * 40) + 0x4000;
                msg_offset  = 0x1000 + (framecounter * 4) + 0x0003;
                MemWrite(Bus, msg_offset, msg_address, 1);
                framecounter++;
            }
        }
        MemWrite(Bus, (msg_offset + 0x1000) + 4, END_OF_FRAME, 1);
    }
    else return -10;

    return 1;
}

/* =========================================================================
 * RunBcMajorFrame_new
 * ======================================================================= */
unsigned long RunBcMajorFrame_new(UCHAR Bus, USHORT stackA_B,
                                   USHORT no_of_minor_frames,
                                   MAJOR_FRAME frame_sequence[],
                                   unsigned long times)
{
    if (Bus < 1 || Bus > 3)                                return -2;
    if (no_of_minor_frames < 1 || no_of_minor_frames > 64) return -24;

    int          m;
    USHORT       count, num_of_messages = 0;
    unsigned int CmdStack = 0x1000;
    int          position = stackA_B;
    int          init_pos = position + CmdStack;

    for (m = 0; m < no_of_minor_frames; m++)
        num_of_messages += frame_sequence[m].Mess_Count;

    if (stackA_B == 0x0)
    {
        if (times == 0)
        {
            RegWrite(Bus, 0x1, 0x54f,  1);
            RegWrite(Bus, 0x3, 0x8000, 1);
            RegWrite(Bus, 0x9, 0x2,    1);
            printf("BC Frame Based Continuous Scheduling\n");
        }
        else
        {
            MemWrite(Bus, CmdStack,     init_pos, 1);
            MemWrite(Bus, MESS_COUNTER, times,    1);

            printf("RegRead 0x07 = %x\n", RegRead(Bus, 0x07, 1));
            printf("RegRead 0x01 = %x\n", RegRead(Bus, 0x01, 1));

            RegWrite(Bus, 0x9, 0x2, 1);

            printf("Waiting for %lu End of Frames...\n", times);

            unsigned long eofCount = 0;
            while (1)
            {
                eofCount = GetBCEOFCnt_D(Bus, CARD1);
                if (eofCount >= times)
                {
                    printf("BC EOF count reached: %lu / %lu\n", eofCount, times);
                    break;
                }
                count = RegRead(Bus, 0x1, 1);
                if ((count & 2) == 0) break;
                usleep(100);
            }
            printf("\tFrame Based Scheduling Completed\n");
        }
    }
    return 1;
}

/* =========================================================================
 * main
 * ======================================================================= */
int main(void)
{
    int BcMod   = 2;
    int mtModId = 2;

    int choice, channel, NoOfTimes, rt_addr,sub_addr,w_cnt;

    fd_set         set;
    struct timeval timeout;
    MTThreadArgs   mtArgs;

    signal(SIGINT, handle_sigint);

    logFile = fopen("mc17_mt_log.txt", "w");
    if (!logFile) { perror("Failed to open mc17_mt_log.txt"); return -1; }

    MtlogFile = fopen("mc17_mt_log_new.txt", "w");
    if (!MtlogFile) { perror("Failed to open mc17_mt_log_new.txt"); return -1; }

    RtlogFile = fopen("mc17_rt_log.txt", "w");
    if (!RtlogFile) { perror("Failed to open mc17_rt_log.txt"); return -1; }

    if (!InitCard(CARD1))
    {
        printf("\n\tINVALID CARD ID OR CARD MAY NOT BE PRESENT\n\n");
        fclose(logFile); fclose(MtlogFile); fclose(RtlogFile);
        return -1;
    }

    InitInterrupts(BcMod,   CONFIG_REG_INIT_VAL, CARD1);
    InitInterrupts(mtModId, CONFIG_REG_INIT_VAL, CARD1);
    getchar();

    EnableInterruptsBC(BcMod,   ENABLE_BC_INT, CARD1);
    EnableInterruptsMT(mtModId, ENABLE_MT_INT, CARD1);

    SetBCSOMCnt(BcMod, 0, CARD1);
    SetBCEOMCnt(BcMod, 0, CARD1);
    SetBCEOFCnt(BcMod, 0, CARD1);
    SetBCEOMMINORFRAMECnt(BcMod, 0, CARD1);
    SetMTSOMCnt(mtModId, 0, CARD1);
    SetMTEOMCnt(mtModId, 0, CARD1);
    getchar();

    ResetAce(1); ResetAce(2); ResetAce(3);
    RegWrite(2, 0x00a2, 0x0001, 1);

    while (1)
    {
        printf("\n1. BC->RT Mode Code  0. Exit\n");
        scanf("%d", &choice);
        if (choice == 0) break;

        switch (choice)
        {
            case 1:
                bc_done = 0;

                printf("Enter channel (CHA=1 / CHB=2): ");
                scanf("%d", &channel);

                printf("No of times to run: ");
                scanf("%d", &NoOfTimes);

                printf("Enter RT address : ");
                scanf("%d", &rt_addr);
                
                 printf("Enter sub address (0 or 31): ");
                scanf("%d", &sub_addr);
                
                 printf("Enter w_cnt  ");
                scanf("%d", &w_cnt);
                
              

                BC_MT_Mode_snode(BcMod, mtModId, channel, NoOfTimes);

                SetBCSOMCnt(BcMod, 0, CARD1);
                SetBCEOMCnt(BcMod, 0, CARD1);
                SetBCEOFCnt(BcMod, 0, CARD1);
                SetBCEOMMINORFRAMECnt(BcMod, 0, CARD1);

                mtArgs.MTModId          = BcMod;
                mtArgs.BCModId          = BcMod;
                mtArgs.expectedMessages = (unsigned long)NoOfTimes;

                pthread_create(&mtThread, NULL, StartMTThread, &mtArgs);

                BCSchedule_ModeCode17(NoOfTimes, BcMod,
                                      (unsigned char)channel,
                                      (unsigned char)rt_addr,
                                       (unsigned char)sub_addr,
                                        (unsigned char)w_cnt);
                bc_done = 1;

                pthread_join(mtThread, NULL);
                ResetAce(mtModId);
                ResetAce(BcMod);
                SetMTEOMCnt(mtModId, 0, CARD1);
                printf("MT hardware stopped.\n");
                break;

            default:
                printf("\n\tInvalid choice.\n");
                break;
        }

        ThreadRunFlag = 0;
        while (ThreadRunFlag == 0)
        {
            FD_ZERO(&set);
            FD_SET(STDIN_FILENO, &set);
            timeout.tv_sec  = 0;
            timeout.tv_usec = 100000;

            if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0)
            {
                char ch;
                read(STDIN_FILENO, &ch, 1);
                printf("\nKey pressed: Exiting mode...\n");
                ResetAce(BcMod);
                ResetAce(mtModId);
                ThreadRunFlag = 1;
            }
            pthread_join(thread,   NULL);
            pthread_join(mtThread, NULL);
        }
    }

    printf("\n********* OVER *********\n");
    pthread_mutex_destroy(&mutexObj);
    fclose(logFile); fclose(MtlogFile); fclose(RtlogFile);
    printf("\nProgram terminated normally.\n");
    return 0;
}
