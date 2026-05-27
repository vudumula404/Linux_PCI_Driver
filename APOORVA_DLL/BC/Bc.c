#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
//#include "/home/desktop/Downloads/APOORVA_CCPMC/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
#include "/home/ccpmc/Documents/CCPMC_ANIL/NEW/Working_DMA_1553/APOORVA_CCPMC_DMA/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
int ThreadRunFlag=0;
FILE* logFile = NULL;
#define BUFFER_SIZE 32
#define HIGH  1
#define LOW   0

#define MSG_COUNT 1024
pthread_t mtThread;
void* StartMTThread(void* arg);
MSG_DEF bc_page;
unsigned short data_buffer[40];
MAJOR_FRAME major_frames[128];
unsigned long m_ulTimeTag=0,prevTime=0;
/* ---------- Helper prototypes ---------- */
void setup_default_bc_page(void);
void define_standard_messages(unsigned char module_id, unsigned short msg_count, unsigned char channel, unsigned short inter_msg_gap);
unsigned short build_major_frames_auto(unsigned short msg_count, MAJOR_FRAME *frames, unsigned short minor_frame_time);
void update_dynamic_messages(unsigned char module_id, int channel);
void msleep(unsigned int milliseconds) {
    struct timespec req, rem;

    req.tv_sec = milliseconds / 1000;
    req.tv_nsec = (milliseconds % 1000) * 1000000L;  // convert ms to ns

    while (nanosleep(&req, &rem) == -1) {
        req = rem;  // if interrupted by a signal, continue sleeping
    }
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
    
    exit(0);
}
/* ---------- Helper implementations ---------- */
void setup_default_bc_page(void)
{
    memset(&bc_page, 0, sizeof(bc_page));
    bc_page.self_test = NO;
    bc_page.mask_bcst_bit = NO;
    bc_page.broadcast = NO;
    bc_page.mode_code = NO;
    bc_page.rt_rt = NO;
    bc_page.retry_enable = YES;
    bc_page.loop = 0x0000;
    bc_page.cmd_1.status = 0xCCCC;
    bc_page.cmd_2.status = 0xCCCC;
}
void define_standard_messages(unsigned char module_id, unsigned short msg_count, unsigned char channel, unsigned short inter_msg_gap)
{
    int idx;
    unsigned short data_value = 0;
    static int rt_address = 0; /* keeps incrementing across calls, wrap at 30 */
   unsigned short datacount = 0;

    for (unsigned short msg_no = 1; msg_no <= msg_count; ++msg_no)
    {
        if (++rt_address > 30) rt_address = 1; /* addresses 1..30 */
        setup_default_bc_page();
        if (channel == 1)
          bc_page.bus = BUS_A;
        else
            bc_page.bus = BUS_B;
        /* defaults for both simple BC->RT and RT->BC */
        bc_page.cmd_1.taddr = (unsigned char)rt_address;
        bc_page.cmd_1.subaddr = 1;
        bc_page.cmd_1.wcnt = 32;
        bc_page.cmd_1.t_r = RECEIVE;

        if (msg_no <= 100)
        {
            /* BC -> RT (receive) */
            bc_page.cmd_1.t_r = RECEIVE;
            for (idx = 0; idx < 32; ++idx)
                data_buffer[idx] = ++data_value;           
             DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no <= 200)
        {
            /* RT -> BC */
            bc_page.cmd_1.t_r = TRANSMIT;
            for (idx = 0; idx < 32; ++idx)
                data_buffer[idx] = 0;            
             DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no <= 300)
        {
            /* Broadcast */
            bc_page.broadcast = YES;
            bc_page.cmd_1.taddr = 31;
            bc_page.cmd_1.t_r = RECEIVE;
            for (idx = 0; idx < 32; ++idx)
                data_buffer[idx] = ++data_value;
            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no <= 400)
        {
            /* RT-to-RT */
            setup_default_bc_page();
            if (channel == 1)
             bc_page.bus = BUS_A;
            else
             bc_page.bus = BUS_B;
            bc_page.cmd_1.t_r = RECEIVE;
            bc_page.cmd_2.t_r = TRANSMIT;
            bc_page.rt_rt = YES;
            bc_page.cmd_2.taddr = (rt_address == 30) ? 1 : 30;
            bc_page.cmd_1.subaddr = bc_page.cmd_2.subaddr = 1;
            bc_page.cmd_1.wcnt = bc_page.cmd_2.wcnt = 32;
            for (idx = 0; idx < 32; ++idx)
                data_buffer[idx] = 0;
            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no <= 500)
        {
            /* Broadcast RT-RT mix (as in original) */
            setup_default_bc_page();
            if (channel == 1)
              bc_page.bus = BUS_A;
            else
              bc_page.bus = BUS_B;
            bc_page.broadcast = YES;
            bc_page.cmd_1.taddr = 31;
            bc_page.cmd_1.t_r = RECEIVE;
            bc_page.cmd_2.t_r = TRANSMIT;
            bc_page.rt_rt = YES;
            bc_page.cmd_2.taddr = (rt_address == 30) ? 1 : rt_address;
            bc_page.cmd_1.subaddr = bc_page.cmd_2.subaddr = 1;
            bc_page.cmd_1.wcnt = bc_page.cmd_2.wcnt = 32;
            for (idx = 0; idx < 32; ++idx)
                data_buffer[idx] = 0;
            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no >= 601 && msg_no <= 621)
        {
            /* Mode codes 0..31 mapped to 601..632 (you used 601..619) */
            int mode_code = msg_no - 601; /* 0..18 (in your range) */
            setup_default_bc_page();
            if (channel == 1)
              bc_page.bus = BUS_A;
            else
             bc_page.bus = BUS_B;
            bc_page.mode_code = YES;
            bc_page.cmd_1.subaddr = 0x00; /* mode codes use subaddr 0 */
            bc_page.cmd_1.taddr = 1; /* default target RT for mode codes */
            bc_page.loop = 0x0000;
           // unsigned short datacount = 0;

            switch (mode_code)
            {
                case 0:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; datacount = 0; break;
                case 1:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 1; datacount = 0; break;
                case 2:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 2; datacount = 0; break;
                case 3:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 3; datacount = 0; break;
                case 4:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 4; datacount = 0; break;
                case 5:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 5; datacount = 0; break;
                case 6:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 6; datacount = 0; break;
                case 7:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 7; datacount = 0; break;
                case 8:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 8; datacount = 0; break;
                case 9:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 9; datacount = 0; break;
                case 10:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 10; datacount = 0; break;
                case 11:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 11; datacount = 0; break;
                case 12:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 12; datacount = 0; break;
                case 13:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 13; datacount = 0; break;
                case 14:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 14; datacount = 0; break;
                case 15:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 15; datacount = 0; break;    
                case 16: bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 16; datacount = 1; data_buffer[0] = 0x0000; break;
                case 17: bc_page.broadcast = YES; bc_page.cmd_1.taddr = 31; bc_page.cmd_1.t_r = RECEIVE; bc_page.cmd_1.wcnt = 17; datacount = 1; data_buffer[0] = ++data_value; break;
                case 18: bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 18; datacount = 1; data_buffer[0] = 0x0000; break;             
                case 19: bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 19; datacount = 1; data_buffer[0] = 0x0000; break;
                case 20: bc_page.cmd_1.t_r = RECEIVE; bc_page.cmd_1.wcnt = 20; datacount = 1; data_buffer[0] = ++data_value; break;
                case 21: bc_page.cmd_1.t_r = RECEIVE; bc_page.cmd_1.wcnt = 21; datacount = 1; data_buffer[0] = ++data_value; break;
                default: bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; datacount = 0; data_buffer[0] = 0; break;
            }

            DefMsg(module_id, msg_no, bc_page, data_buffer, datacount, inter_msg_gap);
        }
        else
        {
            /* Default BC->RT for higher message numbers */
            bc_page.cmd_1.t_r = RECEIVE;
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            for (idx = 0; idx < 32; ++idx)
                data_buffer[idx] = ++data_value;
            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
    }
}

/*
 * Build major frames automatically using 100-message minor cycles.
 * Returns the number of minor cycles constructed.
 */
unsigned short build_major_frames_auto(unsigned short msg_count, MAJOR_FRAME *frames, unsigned short minor_frame_time)
{
    unsigned short minor_cycle_count = (msg_count + 99) / 100; /* ceil(msg_count/100) */
    for (unsigned short mc = 0; mc < minor_cycle_count; ++mc)
    {
        int start = mc * 100;
        int end = start + 100;
        int items = 0;
        /* if last minor frame has fewer messages, add only available ones */
        for (int i = start; i < end && (i < (int)msg_count); ++i)
        {
            frames[mc].frame_items[items++] = (unsigned short)(i + 1); /* message numbers are 1-based */
        }
        frames[mc].Mess_Count = items;
        frames[mc].MinorFrame_Time = minor_frame_time; /* 100 ms as requested */
    }
    return minor_cycle_count;
}

void update_dynamic_messages(unsigned char module_id, int channel)
{
    /* your existing dynamic updates (95..99) kept here for convenience */
    int idx;
    unsigned short cmd_update_data[32];
    for (idx = 0; idx < 32; idx++) cmd_update_data[idx] = 0xa5a6 + idx;
    UpdateMsgCmdData(module_id, 95, 0x0820, 0x0000, cmd_update_data, 32, RECEIVE, channel);

    for (idx = 0; idx < 32; idx++) cmd_update_data[idx] = 0xa5a7 + idx;
    UpdateMsgCmdData(module_id, 96, 0xf811, 0x0000, cmd_update_data, 17, 3, channel);

    for (idx = 0; idx < 32; idx++) cmd_update_data[idx] = 0xa5a8 + idx;
    UpdateMsgCmdData(module_id, 97, 0x1420, 0x0000, cmd_update_data, 32, TRANSMIT, channel);

    for (idx = 0; idx < 32; idx++) cmd_update_data[idx] = 0xa5a9 + idx;
    UpdateMsgCmdData(module_id, 98, 0xfba2, 0x0000, cmd_update_data, 32, RECEIVE, channel);

    for (idx = 0; idx < 32; idx++) cmd_update_data[idx] = 0xa5a9 + idx;
    UpdateMsgCmdData(module_id, 99, 0x0820, 0x1420, cmd_update_data, 32, 2, channel);
}

/* ---------- Main ---------- */
int main(void)
{
    unsigned short msg_sequence[1024];
    unsigned short minor_cycle_count = 0;
    unsigned short minor_cycle_time = 100; /* fixed 100 ms per minor frame */
    unsigned short inter_msg_gap = 0;
    unsigned short msg_count = 0;
    unsigned short msg_no,HighLow;
    unsigned char module_id = 0,mtModId=1;
    int channel = 0; /* 0 = CH-B, 1 = CH-A */
    int run_times = 1;
    int use_frame_scheduler = 0;
    int Retry = 0;
    int BcRetryEnable = 0;
    int Enable = 0;
    int BCResponseTime = 0;

    int init_status = InitCard(1);
    ResetAce(1);
    logFile = fopen("mt_log.txt", "w");
    if (!logFile)
    {
        perror("Failed to open log file");
        return -1;
    }
    if (init_status)
        printf("Card Init Success %d\n", init_status);
    else
        printf("Card Init Failed %d\n", init_status);
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
    (void)ClearRam(1);
    ResetAce(1);
    ConfigureMt(mtModId);
    pthread_create(&mtThread, NULL, StartMTThread, &mtModId);
    getchar();
    
    printf("\n\t\tSelect BC ModuleId:1/2/3 :");
    scanf("%hhu", &module_id);

    printf("\n\t\tEnter Channel (CH-A:1, CH-B:0) :");
    scanf("%d", &channel);

  /*  printf("\n\t\tEnable BcRetryEnable :Enable[1]Disable[0]:");
    scanf("%d", &BcRetryEnable);
    if (BcRetryEnable)
    {
        printf("\n\t\tSingleRetry Option : Same[1] OR Alter[2]");
        printf("\n\t\tDoubleRetry Option : SS[3] OR AA[4] OR SA[5] OR AS[6]\t");
        scanf("%d", &Retry);
    }

    printf("\n\t\tEnable BCResponseTime :Enable[1]Disable[0]:");
    scanf("%d", &Enable);
    if (Enable)
    {
        printf("\n\t\tEnter BCResponseTime :1/2/3/4\t");
        scanf("%d", &BCResponseTime);
    }
    printf("\n\t\tEnter inter_msg_gap :Max:65535_micSec\t");
    scanf("%d", &inter_msg_gap);
*/
    printf("\n\t\tEnter Number Of Messages Count :");
    scanf("%hu", &msg_count);

    printf("\n\t\tMessages/Frame Based Scheduler (0=Message, 1=Frame): ");
    scanf("%d", &use_frame_scheduler);

    /* In this optimized version we ALWAYS use 100-message minor cycles if frame scheduler chosen.
       If user chose message scheduler, we fall back to message-mode. */
    printf("\n\t\tEnter Number Of Messages To Run (Times*Messages):");
    scanf("%d", &run_times);


    (void)DefMode(module_id, BC);

    BCResponseTimeE(module_id, Enable, BCResponseTime);
    
    if (channel == 1) bc_page.bus = BUS_A;
    else bc_page.bus = BUS_B;

    /* Define messages once (keeps previous functionality) */
    define_standard_messages(module_id, msg_count, channel, inter_msg_gap);

    /* If user selected frame scheduler, auto-build major frames using 100-message minor cycles */
    if (use_frame_scheduler == 1)
    {
        printf("\n\t\tEnter High[1]/Low[0] Priority Mode ");
        scanf("%d", &HighLow);
        /* compute minor cycle count automatically based on msg_count and 100-msg minor frames */
        minor_cycle_count = build_major_frames_auto(msg_count, major_frames, minor_cycle_time);
        
        BCRetry(module_id, Retry);

        /* Define and run major frame */
        DefMajorFrame(module_id, 0, minor_cycle_count, major_frames);
        printf("BC Major Frame Started ... minor cycles=%hu, minor time=%hu ms\n", minor_cycle_count, minor_cycle_time);

        RunBcMajorFrame(module_id, 0, minor_cycle_count, major_frames, run_times);

        if (run_times == 0)
        {
            unsigned short dynamic_data[32];
            /* dynamic updates and priority switching maintained */
            if(HighLow)
            {
              printf("Enter For HIGH Priority Messages \n");
              getchar(); getchar();
              printf("HIGH Priority Mode Initiated\n");
              PriorityHighLow(module_id, 10, 1);
           //   msleep(2);
             unsigned short cmd_update_data[32];
              for (int idx = 0; idx < 32; idx++) cmd_update_data[idx] = 0xbaba + idx;
              UpdateMsgCmdData(module_id, 10, 0x0840, 0x0000, cmd_update_data, 32, RECEIVE, channel);
             /* printf("Enter For Dynamic Message updation 95 to 99 \n");
              for (int i = 0; i < 32; ++i) dynamic_data[i] = 0xAAAA + i;
              UpdateMsgData(module_id, 10, dynamic_data, 32, RECEIVE);*/
            }
            else
            {
              printf("Enter For LOW Priority Messages \n");
              getchar();
              getchar();
              printf("Low Priority Mode Initiated\n");
              PriorityHighLow(module_id, 2, 2);
              //msleep(2);
              for (int i = 0; i < 32; ++i) dynamic_data[i] = 0x1553+ i;
              UpdateMsgData(module_id, 2, dynamic_data, 32, RECEIVE);
                 unsigned short cmd_update_data[32];
              for (int idx = 0; idx < 32; idx++) cmd_update_data[idx] = 0xbaba + idx;
              UpdateMsgCmdData(module_id, 10, 0x0860, 0x0000, cmd_update_data, 32, RECEIVE, channel);
              getchar();
              //update_dynamic_messages(module_id, channel);
              printf("Dynamic Message updation 95 to 99 Completed\n");
            }
        }
         printf("Enter to Stop BC..!\n");
         //getchar();
    }
    else /* message-based scheduler */
    {
        for (int idx = 0; idx < (int)msg_count; ++idx)
            msg_sequence[idx] = idx + 1;

        //BCRetry(module_id, Retry);
        DefFrame(module_id, msg_count, msg_sequence);          
        RunBc(module_id, 0, 1, msg_count, run_times);

        if (run_times == 0)
        {
            printf("Enter For Dynamic Message updation 95 to 99 \n");
            getchar(); getchar();
            update_dynamic_messages(module_id, channel);
        }
        
        printf("Enter to Stop BC..!\n");
        for (int i=0;i<100;i++)
                {
                    printf("MT READ\n");
                    printf("ADDRESS=%08x \t DATA=%04x",0x24000+i,MemRead(module_id,0x24000+i,1));
          
                }
       // getchar();
      
    }

    /* cleanup and reset */
    getchar();
     getchar();
    RegWrite(module_id, 0x07, 0x0000, 1);
    printf("BC Stopped..!\n");
    ResetAce(1);
    ResetAce(2);
    ResetAce(3);
    printf("Device Reset Over.........!\n");
    getchar();

    return 0;
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


void* StartMTThread(void* arg)
{
    unsigned char MTModId = *((unsigned char*)arg);
    unsigned short current = 0, last = 0, prev = 0;
    unsigned short pos = 0;
    unsigned long msgCount = 0;
    struct timeval prev_tv;

    signal(SIGINT, handle_sigint); 
    last = ReadMsgMtCounter(MTModId);
    //RegRead(MTModId, 0xB, 1); 
    gettimeofday(&prev_tv, NULL);

    while (ThreadRunFlag == 0)
    {
        current = ReadMsgMtCounter(MTModId);
        //RegRead(MTModId, 0xB, 1);
     
        if (last != current)
        {
            ++last;          
            if (pos != 0)
            {            
                ProcessMTDescriptor(MTModId, last, pos, &msgCount, &prev_tv);
            }
            prev = last;
            ++msgCount;          
            pos = (pos >= MSG_COUNT) ? 1 : pos + 1;
        }       
    }    
   // ProcessMTDescriptor(MTModId, ++last, ++pos, &msgCount, &prev_tv);

    printf("MT Reading Over. Total messages: %lu\n", (msgCount-1));
    return 0;
}
void ConfigureMt(unsigned char MTModId)
{
  ClearRam(MTModId);
  ResetAce(MTModId);  
  for(int i=0; i < 128; i++)    //SELMESG MT LOOK-UP-TABLE
    {
        MemWrite(MTModId,(0x20280+i),0xffff,1); ///changed add 20280 29/01/26
    }
  MemWrite(MTModId,(0x20102),0x1000,1); //COMMAND STACK POINTER
  MemWrite(MTModId,(0x20103),0x4000,1); //DATA STACK POINTER
  RegWrite(MTModId,0x1,0x5000,1);  //CONFIGURATION 12-BIT MUST BE HIGH
  RegWrite(MTModId,0x7,0x8000,1); //CONFIGURATION  15-BIT MUST BE HIGH
  RegWrite(MTModId,0x3,0x2,1);  //START REGISTER
  //pthread_create(&mtThread, NULL, StartMTThread, &MTModId);
}
