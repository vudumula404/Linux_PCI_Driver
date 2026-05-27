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

#define BUS_A                1
#define BUS_B                0
#define RECEIVE              0
#define TRANSMIT             1
#define YES                  1
#define NO                   0
#define CARD1                1
#define CONFIG_REG_INIT_VAL  0x00000078
#define ENABLE_BC_INT        0x00004029
#define ENABLE_MT_INT        0x00004001
#define ENABLE_RT_INT        0x00000001

#define MT_RING_SIZE         4096
#define MT_OVERRUN_THRESHOLD 2048

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

/* ── Forward declarations ── */
void  ConfigureRT_ForTx(unsigned char rtModId, unsigned char rtAddr,
                         unsigned char sub_addr, unsigned char word_count);
void  ConfigureRT_ForRx(unsigned char rtModId, unsigned char rtAddr,
                         unsigned char sub_addr, unsigned char word_count);
void  BCSchedule_RT_to_RT(int Times, unsigned char module_id, unsigned char channel,
                            unsigned char tx_rt_addr, unsigned char tx_sub_addr,
                            unsigned char rx_rt_addr, unsigned char rx_sub_addr,
                            unsigned char word_count);
void* StartMTThread(void* arg);
void  ProcessMTDescriptor(unsigned char MTModId, unsigned short desc,
                           unsigned short pos, unsigned long *msgCount,
                           struct timeval *prev_tv);
int   DefMajorFrame_new(UCHAR Bus, USHORT BCSTACK_A_B, USHORT no_of_minor_frames,
                         MAJOR_FRAME frame_sequence[]);
unsigned long RunBcMajorFrame_new(UCHAR Bus, USHORT stackA_B, USHORT no_of_minor_frames,
                                   MAJOR_FRAME frame_sequence[], unsigned long times);

/* =========================================================================
 * ConfigureRT_ForTx  –  TX RT (Module 3)
 * ======================================================================= */
/* =========================================================================
 * BCSchedule_RT_to_RT  –  RT-to-RT transfer via BC
 * ======================================================================= */
void BCSchedule_RT_to_RT(int Times,
                          unsigned char module_id,
                          unsigned char channel,
                          unsigned char tx_rt_addr,
                          unsigned char tx_sub_addr,
                          unsigned char rx_rt_addr,
                          unsigned char rx_sub_addr,
                          unsigned char word_count)
{
    unsigned short inter_msg_gap = 20;

    printf("\tBC Scheduling (RT→RT): TX_RT=%u SA=%u  RX_RT=%u SA=%u  WC=%u  Times=%d\n",
           tx_rt_addr, tx_sub_addr, rx_rt_addr, rx_sub_addr, word_count, Times);

    memset(&bc_page, 0, sizeof(bc_page));
    bc_page.bus           = (channel == 1) ? BUS_A : BUS_B;
    bc_page.self_test     = NO;
    bc_page.mask_bcst_bit = NO;
    bc_page.broadcast     = YES;
    bc_page.mode_code     = NO;
    bc_page.rt_rt         = YES;          /* ← RT-to-RT flag */
    bc_page.loop          = 0x0000;

    /* CW1 = Receive command (RX RT) */
    bc_page.cmd_1.taddr   = rx_rt_addr;
    bc_page.cmd_1.subaddr = rx_sub_addr;
    bc_page.cmd_1.wcnt    = word_count;
    bc_page.cmd_1.t_r     = RECEIVE;      /* RX RT receives */
    bc_page.cmd_1.status  = 0xCCCC;

    /* CW2 = Transmit command (TX RT) */
    bc_page.cmd_2.taddr   = tx_rt_addr;
    bc_page.cmd_2.subaddr = tx_sub_addr;
    bc_page.cmd_2.wcnt    = word_count;
    bc_page.cmd_2.t_r     = TRANSMIT;     /* TX RT transmits */
    bc_page.cmd_2.status  = 0xCCCC;

    memset(data_buffer, 0, sizeof(data_buffer));
    DefMsg(module_id, 1, bc_page, data_buffer, word_count, inter_msg_gap);

    major_frames[0].frame_items[0]  = 1;
    major_frames[0].Mess_Count      = 1;
    major_frames[0].MinorFrame_Time = 20;

    DefMajorFrame_new(module_id, 0, 1, major_frames);
    printf("\tBC Scheduling Started...\n");
    RunBcMajorFrame_new(module_id, 0, 1, major_frames, (unsigned long)Times);

    /* Read RX RT received data from BC descriptor */
    printf("\n--- RT-to-RT Data (read from BC descriptor, RX side) ---\n");
    unsigned int  msg_base    = 0x4000;
    unsigned short rx_status  = MemRead(module_id, msg_base + 3, 1);
    unsigned short tx_status  = MemRead(module_id, msg_base + 4, 1);
    printf("RX RT Status: 0x%04X\n", rx_status);
    printf("TX RT Status: 0x%04X\n", tx_status);
    if (rx_status != 0xCCCC && rx_status != 0x0000)
    {
        printf("Data words (TX RT → RX RT):\n");
        for (int w = 0; w < word_count; w++)
        {
            unsigned short dw = MemRead(module_id, msg_base + 5 + w, 1);
           // printf("  Word[%2d] = 0x%04X\n", w, dw);
        }

        /* Also read what RX RT actually stored in its RX buffer */
        printf("\n--- RX RT Memory (Module 4, SA=%u) ---\n", rx_sub_addr);
        unsigned long rx_buf = 0x5000 + 0x20 * rx_sub_addr;
        for (int w = 0; w < word_count; w++)
        {
            unsigned short dw = MemRead(4, rx_buf + w, 1);
           // printf("  RX_RT Word[%2d] = 0x%04X\n", w, dw);
        }
    }
    else
    {
        printf("WARNING: No valid RT response (rx_status=0x%04X)\n", rx_status);
    }

    GetBCSOMCnt(module_id, CARD1);
    GetBCEOMCnt(module_id, CARD1);
    GetBCEOFCnt(module_id, CARD1);
    GetRTSOMCnt(3, CARD1); GetRTEOMCnt(3, CARD1);
    GetMTSOMCnt(module_id, CARD1);
    GetMTEOMCnt(module_id, CARD1);
    

    printf("\tEnter to Reset BC...\n");
    getchar(); getchar();
    ResetAce(module_id);
    printf("\tBC Reset Success!\n");
}

/* =========================================================================
 * ProcessMTDescriptor  –  handles RT-to-RT block correctly
 * ======================================================================= */
void ProcessMTDescriptor(unsigned char  MTModId,
                          unsigned short desc,
                          unsigned short pos,
                          unsigned long *msgCount,
                          struct timeval *prev_tv)
{
    MNTPACK        buffer;
    unsigned short cmd, rtAddr, subAddr, wc_or_mode, trBit;
    unsigned long  ptr;
    struct timeval tv;
    struct tm     *t;

    unsigned long Offset   = 0x21000 + ((pos * 4) - 4);
    buffer.BlockSts.value  = MemRead(MTModId, Offset,     1);
    buffer.TimeTag         = MemRead(MTModId, Offset + 1, 1);
    buffer.DataBlkPtr      = MemRead(MTModId, Offset + 2, 1);
    ptr                    = 0x20000 + buffer.DataBlkPtr;
    buffer.RecCmdWrd.value = MemRead(MTModId, Offset + 3, 1);

    if (buffer.BlockSts.value == 0x0000 || buffer.RecCmdWrd.value == 0x0000)
        return;

    cmd        = buffer.RecCmdWrd.value;
    rtAddr     = (cmd >> 11) & 0x1F;
    trBit      = (cmd >> 10) & 0x01;
    subAddr    = (cmd >>  5) & 0x1F;
    wc_or_mode = cmd & 0x1F;
    if (wc_or_mode == 0) wc_or_mode = 32;

    gettimeofday(&tv, NULL);
    t = localtime(&tv.tv_sec);

    printf("\nTime [%02d:%02d:%02d.%06ld] Date [%02d-%02d-%04d] Msg#%lu\n",
           t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec,
           t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, *msgCount);
    printf("BlkSts=0x%04X  Cmd=0x%04X\n",
           buffer.BlockSts.value, buffer.RecCmdWrd.value);
    fprintf(logFile, "\nTime [%02d:%02d:%02d.%06ld] Date [%02d-%02d-%04d] Msg#%lu\n",
            t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec,
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, *msgCount);
    *prev_tv = tv;

    int is_rt_to_rt = (buffer.BlockSts.value & 0x0800) ? 1 : 0;
    int no_resp     = (buffer.BlockSts.value & 0x0200) ? 1 : 0;

    if (is_rt_to_rt)
    {
        /* RT-to-RT descriptor layout in MT memory:
         *   ptr+0 : TX command word (second CW)
         *   ptr+1 : TX RT status word
         *   ptr+2 : RX RT status word
         *   ptr+3 .. ptr+2+WC : data words
         */
        unsigned short tx_cmd    = MemRead(MTModId, ptr,     1);
        unsigned short tx_status = MemRead(MTModId, ptr + 1, 1);
        unsigned short rx_status = MemRead(MTModId, ptr + 2, 1);
        unsigned short tx_rt     = (tx_cmd >> 11) & 0x1F;
        unsigned short tx_sa     = (tx_cmd >>  5) & 0x1F;
        unsigned short wc        = tx_cmd & 0x1F;
        if (wc == 0) wc = 32;

        printf("RT→RT: RX_RT=%u SA=%u  TX_RT=%u SA=%u  WC=%u\n",
               rtAddr, subAddr, tx_rt, tx_sa, wc);
        printf("TX_CmdWrd=0x%04X  TX_Status=0x%04X  RX_Status=0x%04X\n",
               tx_cmd, tx_status, rx_status);
        fprintf(logFile, "RT→RT: RX_RT=%u SA=%u  TX_RT=%u SA=%u  WC=%u\n",
                rtAddr, subAddr, tx_rt, tx_sa, wc);

        if (!no_resp)
        {
            ptr += 3;
            printf("Data (TX_RT → RX_RT):\n");
            fprintf(logFile, "Data (TX_RT → RX_RT):\n");
            for (int ii = 0; ii < (int)wc; ii++)
            {
                buffer.DataPack[ii] = MemRead(MTModId, ptr++, 1);
                printf("%04X ", buffer.DataPack[ii]);
                fprintf(logFile, "%04X ", buffer.DataPack[ii]);
                if ((ii + 1) % 8 == 0) { printf("\n"); fprintf(logFile, "\n"); }
            }
            printf("\n"); fprintf(logFile, "\n");
        }
        else
        {
            printf("No Response from one or both RTs!\n");
            fprintf(logFile, "No Response!\n");
        }
    }
    else
    {
        /* Should not appear in RT-to-RT test, but handle gracefully */
        printf("Unexpected BC→RT/RT→BC message in RT-RT test. Skipping.\n");
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
int DefMajorFrame_new(UCHAR Bus, USHORT BCSTACK_A_B, USHORT no_of_minor_frames,
                       MAJOR_FRAME frame_sequence[])
{
    unsigned int i, m, number_of_messages = 0, framecounter = 0;
    unsigned int msg_address, temp, msg_offset;

    if (Bus < 1 || Bus > 3) return -2;
    if (no_of_minor_frames < 1 || no_of_minor_frames > 64) return -24;

    for (m = 0; m < no_of_minor_frames; m++)
    {
        unsigned short Cnt_Time = ((unsigned short)frame_sequence[m].Mess_Count << 9)
                                 | ((unsigned short)frame_sequence[m].MinorFrame_Time & 0x1FF);
        RegWrite(Bus, 0x61 + m, Cnt_Time, 1);
    }
    RegWrite(Bus, 0x0d, no_of_minor_frames, 1);

    for (m = 0; m < no_of_minor_frames; m++)
        number_of_messages += frame_sequence[m].Mess_Count;

    if (number_of_messages >= 1 && number_of_messages <= 1024)
    {
        for (m = 0; m < no_of_minor_frames; m++)
            for (i = 0; i < 10; i++)
            {
                temp        = frame_sequence[m].frame_items[i] - 1;
                msg_address = (temp * 40) + 0x4000;
                msg_offset  = 0x1000 + (framecounter * 4) + 0x0003;
                MemWrite(Bus, msg_offset, msg_address, 1);
                framecounter++;
            }
        MemWrite(Bus, (msg_offset + 0x1000) + 4, END_OF_FRAME, 1);
    }
    else return -10;
    return 1;
}

/* =========================================================================
 * RunBcMajorFrame_new
 * ======================================================================= */
unsigned long RunBcMajorFrame_new(UCHAR Bus, USHORT stackA_B, USHORT no_of_minor_frames,
                                   MAJOR_FRAME frame_sequence[], unsigned long times)
{
    if (Bus < 1 || Bus > 3) return -2;
    if (no_of_minor_frames < 1 || no_of_minor_frames > 64) return -24;

    unsigned int CmdStack = 0x1000;
    int init_pos = stackA_B + CmdStack;

    if (stackA_B == 0x0)
    {
        if (times == 0)
        {
            RegWrite(Bus, 0x1, 0x54f,  1);
            RegWrite(Bus, 0x3, 0x8000, 1);
            RegWrite(Bus, 0x9, 0x2,    1);
        }
        else
        {
            MemWrite(Bus, CmdStack,     init_pos, 1);
            MemWrite(Bus, MESS_COUNTER, times,    1);
            RegWrite(Bus, 0x9, 0x2, 1);
            printf("Waiting for %lu End of Frames...\n", times);
            unsigned long eofCount = 0;
            USHORT count;
            while (1)
            {
                eofCount = GetBCEOFCnt_D(Bus, CARD1);
                if (eofCount >= times) { printf("BC EOF reached: %lu\n", eofCount); break; }
                count = RegRead(Bus, 0x1, 1);
                if ((count & 2) == 0) { printf("BC stopped early. EOF: %lu\n", eofCount); break; }
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
    int BcMod    = 2;
    int mtModId  = 2;
    int rtmod  = 3;   
   

    int choice, channel, NoOfTimes;
    int tx_rt_addr, tx_sub_addr, rx_rt_addr, rx_sub_addr, word_count;

    fd_set         set;
    struct timeval timeout;
    MTThreadArgs   mtArgs;

    logFile = fopen("rt_to_rt_log.txt", "w");
    if (!logFile) { perror("fopen rt_to_rt_log.txt"); return -1; }
    MtlogFile = fopen("mt_log.txt", "w");
    if (!MtlogFile) { perror("fopen mt_log.txt"); return -1; }

    if (!InitCard(CARD1))
    {
        printf("\n\tINVALID CARD ID\n\n");
        fclose(logFile); fclose(MtlogFile); return -1;
    }

    printf("*********************** RT → RT Test ************************\n");

    InitInterrupts(BcMod,   CONFIG_REG_INIT_VAL, CARD1);
    InitInterrupts(rtmod, CONFIG_REG_INIT_VAL, CARD1);
    InitInterrupts(mtModId, CONFIG_REG_INIT_VAL, CARD1);
    getchar();

    EnableInterruptsBC(BcMod,   ENABLE_BC_INT, CARD1);
    EnableInterruptsMT(mtModId, ENABLE_MT_INT, CARD1);
    EnableInterruptsRT(rtmod, ENABLE_RT_INT, CARD1);

    SetBCSOMCnt(BcMod, 0, CARD1); SetBCEOMCnt(BcMod, 0, CARD1);
    SetBCEOFCnt(BcMod, 0, CARD1); SetBCEOMMINORFRAMECnt(BcMod, 0, CARD1);
    SetMTSOMCnt(mtModId, 0, CARD1); SetMTEOMCnt(mtModId, 0, CARD1);
    SetRTSOMCnt(rtmod, 0, CARD1); SetRTEOMCnt(rtmod, 0, CARD1);
    getchar();

    ResetAce(1); ResetAce(2); ResetAce(3); ResetAce(4);
    RegWrite(2, 0x00a2, 0x0001, 1);

    while (1)
    {
        printf("\n1. RT→RT Test   0. Exit\n");
        scanf("%d", &choice);
        if (choice == 0) break;

        if (choice == 1)
        {
            bc_done = 0;

            printf("Enter channel (CHA=1 / CHB=2): ");           scanf("%d", &channel);
            printf("Number of times to run: ");                   scanf("%d", &NoOfTimes);
            printf("Enter TX RT address (1-30): ");               scanf("%d", &tx_rt_addr);
            printf("Enter TX Sub-address (1-30): ");              scanf("%d", &tx_sub_addr);
            printf("Enter RX RT address (31): ");   scanf("%d", &rx_rt_addr);
            printf("Enter RX Sub-address (1-30): ");              scanf("%d", &rx_sub_addr);
            printf("Enter Word count (1-32): ");                  scanf("%d", &word_count);
            if (word_count < 1 || word_count > 32) word_count = 1;

            if (tx_rt_addr == rx_rt_addr)
            { 
              printf("ERROR: TX and RX RT addresses must be different!\n"); break; 
            }
            ConfigureRT(rtmod,tx_rt_addr);
            ConfigureRT(rtmod,rx_rt_addr);
            BC_MT_Mode_snode(BcMod, mtModId, channel, NoOfTimes);
            SetBCSOMCnt(BcMod, 0, CARD1); SetBCEOMCnt(BcMod, 0, CARD1);
            SetBCEOFCnt(BcMod, 0, CARD1); SetBCEOMMINORFRAMECnt(BcMod, 0, CARD1);
            SetMTEOMCnt(mtModId, 0, CARD1);

            /* Step 4: Start MT thread */
            printf("\n[STEP 4] Starting MT thread...\n");
            mtArgs.MTModId = BcMod; 
            mtArgs.BCModId = BcMod;
            mtArgs.expectedMessages = (unsigned long)NoOfTimes;
            pthread_create(&mtThread, NULL, StartMTThread, &mtArgs);

            /* Step 5: Run BC */
            printf("\n[STEP 5] Starting BC (RT→RT command)...\n");
            BCSchedule_RT_to_RT(NoOfTimes, (unsigned char)BcMod,
                                (unsigned char)channel,
                                (unsigned char)tx_rt_addr, (unsigned char)tx_sub_addr,
                                (unsigned char)rx_rt_addr, (unsigned char)rx_sub_addr,
                                (unsigned char)word_count);
            bc_done = 1;

            pthread_join(mtThread, NULL);
            ResetAce(rtmod); 
            ResetAce(mtModId); ResetAce(BcMod);
            SetMTEOMCnt(mtModId, 0, CARD1);
            SetRTEOMCnt(rtmod, 0, CARD1);
            printf("All modules stopped.\n");
        }

        ThreadRunFlag = 0;
        while (ThreadRunFlag == 0)
        {
            FD_ZERO(&set); FD_SET(STDIN_FILENO, &set);
            timeout.tv_sec = 0; timeout.tv_usec = 100000;
            if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0)
            {
                char ch; read(STDIN_FILENO, &ch, 1);
                printf("\nReturning to menu...\n");
                ResetAce(BcMod); ResetAce(mtModId);
                ResetAce(rtmod); 
                ThreadRunFlag = 1;
            }
            pthread_join(thread, NULL);
            pthread_join(mtThread, NULL);
        }
    }

    printf("\n********** OVER **********\n");
    pthread_mutex_destroy(&mutexObj);
    fclose(logFile); fclose(MtlogFile);
    return 0;
}
