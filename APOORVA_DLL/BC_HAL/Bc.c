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
#include "/home/acl/APOORVA_CCPMC/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
int ThreadRunFlag=0;
pthread_t thread;
FILE* logFile = NULL;
#define BUFFER_SIZE 32
#define HIGH  1
#define LOW   0
void* RTThread_Bus(void* arg);
#define MSG_COUNT 1024
FILE* RtlogFile=NULL;
pthread_t mtThread;
void* StartMTThread(void* arg);
MSG_DEF bc_page;
unsigned short data_buffer[40];
MAJOR_FRAME major_frames[128];
unsigned long m_ulTimeTag=0,prevTime=0;
RT_MSG rtMsg;
/* ---------- Helper prototypes ---------- */
void setup_default_bc_page(void);
void define_standard_messages(unsigned char module_id, unsigned short msg_count, unsigned char channel, unsigned short inter_msg_gap);
unsigned short build_major_frames_auto(unsigned short msg_count, MAJOR_FRAME *frames, unsigned short minor_frame_time);
void update_dynamic_messages(unsigned char module_id, int channel);
void ConfigureRT(UCHAR rtModId, unsigned char rtAddr);
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
        bc_page.cmd_1.taddr = 5;//(unsigned char)rt_address;
        bc_page.cmd_1.subaddr = 5;
        bc_page.cmd_1.wcnt = 32;
        bc_page.cmd_1.t_r = RECEIVE;
       //if (msg_no >= 601 && msg_no <= 621)
        {
            /* Mode codes 0..31 mapped to 601..632 (you used 601..619) */
            int mode_code = msg_no - 601; /* 0..18 (in your range) */
           bc_page.broadcast = YES;
           // bc_page.cmd_1.taddr = 31;
            //setup_default_bc_page();
            /*if (channel == 1)
              bc_page.bus = BUS_A;
            else
             bc_page.bus = BUS_B;*/
            bc_page.mode_code = YES;
            bc_page.cmd_1.subaddr = 0x00; /* mode codes use subaddr 0 */
            bc_page.cmd_1.taddr = 31; /* default target RT for mode codes  */
            bc_page.loop = 0x0000;
            bc_page.broadcast = YES; bc_page.cmd_1.t_r = RECEIVE; bc_page.cmd_1.wcnt = 17; datacount = 1; data_buffer[0] = 0xa6c3;
           // unsigned short datacount = 0;
          
          //  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; datacount = 0; 
             //bc_page.broadcast = NO; bc_page.cmd_1.taddr = 7; bc_page.cmd_1.t_r = RECEIVE; bc_page.cmd_1.wcnt = 17; datacount = 1; data_buffer[0] = 0xa6c3;
                 /* RT-to-RT */
          
            /*if (channel == 1)
             bc_page.bus = BUS_A;
            else
             bc_page.bus = BUS_B;
            bc_page.cmd_1.t_r = RECEIVE;
            bc_page.cmd_2.t_r = TRANSMIT;
            bc_page.rt_rt = YES;
            bc_page.cmd_2.taddr = 7;//(rt_address == 30) ? 1 : 30;
            bc_page.cmd_1.subaddr = bc_page.cmd_2.subaddr = 5;
            bc_page.cmd_1.wcnt = bc_page.cmd_2.wcnt = 32;
            for (idx = 0; idx < 32; ++idx)
                data_buffer[idx] = 0;
            DefMsg(module_id, 1, bc_page, data_buffer, 32, inter_msg_gap);*/
         DefMsg(module_id, 1, bc_page, data_buffer, datacount, inter_msg_gap);
         
        }
        bc_page.bus = BUS_B;
        /* defaults for both simple BC->RT and RT->BC */
        bc_page.cmd_1.taddr = 5;//(unsigned char)rt_address;
        bc_page.cmd_1.subaddr = 5;
        bc_page.cmd_1.wcnt = 32;
        bc_page.cmd_1.t_r = RECEIVE;
       //if (msg_no >= 601 && msg_no <= 621)
        {
            /* Mode codes 0..31 mapped to 601..632 (you used 601..619) */
            int mode_code = msg_no - 601; /* 0..18 (in your range) */
           bc_page.broadcast = YES;
           // bc_page.cmd_1.taddr = 31;
            //setup_default_bc_page();
            /*if (channel == 1)
              bc_page.bus = BUS_A;
            else
             bc_page.bus = BUS_B;*/
            bc_page.mode_code = YES;
            bc_page.cmd_1.subaddr = 0x00; /* mode codes use subaddr 0 */
            bc_page.cmd_1.taddr = 31; /* default target RT for mode codes  */
            bc_page.loop = 0x0000;
            bc_page.broadcast = YES; bc_page.cmd_1.t_r = RECEIVE; bc_page.cmd_1.wcnt = 17; datacount = 1; data_buffer[0] = 0xbaba;
           // unsigned short datacount = 0;
          
          //  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; datacount = 0; 
             //bc_page.broadcast = NO; bc_page.cmd_1.taddr = 7; bc_page.cmd_1.t_r = RECEIVE; bc_page.cmd_1.wcnt = 17; datacount = 1; data_buffer[0] = 0xa6c3;
                 /* RT-to-RT */
          
            /*if (channel == 1)
             bc_page.bus = BUS_A;
            else
             bc_page.bus = BUS_B;
            bc_page.cmd_1.t_r = RECEIVE;
            bc_page.cmd_2.t_r = TRANSMIT;
            bc_page.rt_rt = YES;
            bc_page.cmd_2.taddr = 7;//(rt_address == 30) ? 1 : 30;
            bc_page.cmd_1.subaddr = bc_page.cmd_2.subaddr = 5;
            bc_page.cmd_1.wcnt = bc_page.cmd_2.wcnt = 32;
            for (idx = 0; idx < 32; ++idx)
                data_buffer[idx] = 0;
            DefMsg(module_id, 1, bc_page, data_buffer, 32, inter_msg_gap);*/
         DefMsg(module_id, 2, bc_page, data_buffer, datacount, inter_msg_gap);
         }
        #if 0
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
            bc_page.cmd_1.taddr = 7; /* default target RT for mode codes */
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
            case 0:  bc_page.cmd_1.t_r = TRANSMIT; bc_page.cmd_1.wcnt = 0; datacount = 0; break;
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
        #endif
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
    unsigned char module_id = 0,rtModId=0,mtModId=2;
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
    //ConfigureMt(mtModId);
    // RegWrite(mtModId,0x5,0x0000,1);
    //pthread_create(&mtThread, NULL, StartMTThread, &mtModId);
   
    printf("\n\t\tSelect BC ModuleId:1/2/3 :");
    scanf("%hhu", &module_id);

    printf("\n\t\tEnter Channel (CH-A:1, CH-B:0) :");
    scanf("%d", &channel);

    printf("\n\t\tEnable BcRetryEnable :Enable[1]Disable[0]:");
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

    printf("\n\t\tEnter Number Of Messages Count :");
    scanf("%hu", &msg_count);

    printf("\n\t\tMessages/Frame Based Scheduler (0=Message, 1=Frame): ");
    scanf("%d", &use_frame_scheduler);
//rtModId=2;
    /* In this optimized version we ALWAYS use 100-message minor cycles if frame scheduler chosen.
       If user chose message scheduler, we fall back to message-mode. */
    printf("\n\t\tEnter Number Of Messages To Run (Times*Messages):");
    scanf("%d", &run_times);
   /* ConfigureRT(2,5);
     pthread_create(&thread, NULL, RTThread_Bus, &rtModId);
        printf("\n\t\tRt receibing :");
     getchar();
        getchar();*/
        module_id=1;
    (void)ClearRam(module_id);
    ResetAce(module_id);
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
              PriorityHighLow(module_id, 40, 1);
           //   msleep(2);
              for (int i = 0; i < 32; ++i) dynamic_data[i] = 0xAAAA + i;
              UpdateMsgData(module_id, 40, dynamic_data, 32, RECEIVE);
            }
            else
            {
              printf("Enter For LOW Priority Messages \n");
              getchar();
              getchar();
              printf("Low Priority Mode Initiated\n");
              PriorityHighLow(module_id, 2, 2);
              //msleep(2);
              for (int i = 0; i < 32; ++i) dynamic_data[i] = 0xBBBB + i;
              UpdateMsgData(module_id, 2, dynamic_data, 32, RECEIVE);

              printf("Enter For Dynamic Message updation 95 to 99 \n");
              getchar();
              update_dynamic_messages(module_id, channel);
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
msg_count=2;
        BCRetry(module_id, Retry);
        DefFrame(module_id, msg_count, msg_sequence);          
        RunBc(module_id, 0, 1, msg_count, run_times);

        if (run_times == 0)
        {
            printf("Enter For Dynamic Message updation 95 to 99 \n");
            getchar(); getchar();
            update_dynamic_messages(module_id, channel);
        }
        
        printf("Enter to Stop BC..!\n");
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
    unsigned short ptr, temp, cmd, rtAddr, subAddr, wc_or_mode, trBit,reg=0,TempReg=0,RollOver=0;
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
    TempReg=RegRead(MTModId,0x5,1);
      if(TempReg!=0)
      {
        if(reg>TempReg)
        {
          RollOver = (0xFFFF-reg) + TempReg;
          m_ulTimeTag = m_ulTimeTag +((RollOver-1) * 65535);
        }
        else
        {
          RollOver =TempReg-reg;
          if(RollOver!=0)
          {
            m_ulTimeTag = m_ulTimeTag+((RollOver-1) * 65535);
            reg = TempReg;
          }
        }
      }
       if(RollOver==0)
       {
         m_ulTimeTag = m_ulTimeTag + (buffer.TimeTag - prevTime);
         if(m_ulTimeTag>999999)
          m_ulTimeTag = (0xFFFF-prevTime)+buffer.TimeTag;
       }
      prevTime = buffer.TimeTag ;
    
    gettimeofday(&tv, NULL);
    t = localtime(&tv.tv_sec);
    fprintf(logFile, "\nTimeSpan [%d][%x][%x][%d][%d][%x][%d] Time [%02d:%02d:%02d.%06ld] Msg#%lu \n",
            RegRead(MTModId,0x5,1), buffer.TimeTag ,prevTime,m_ulTimeTag,buffer.TimeTag,buffer.TimeTag  ,t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec, *msgCount);
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
  // Initialize descriptor pointer from hardware (0xB register)      
  RegWrite(MTModId, 0x3, 0x1, 1);                               
  RegWrite(MTModId, 0x3, 0x2, 1);
  RegWrite(MTModId, CONFIG_REG, 0x5000, 1);
  SelMsgMt(MTModId);
  RunMt(MTModId);
}
void ConfigureRT(UCHAR rtModId, unsigned char rtAddr)
{
  int Sa=0,i=0;
  unsigned short txBuffer[BUFFER_SIZE];
 // Basic RT setup
    ClearRam(rtModId);
    ResetAce(rtModId);    
    SetRtAddrM(rtModId, rtAddr, 1);
    DefLookUp(rtModId, LOOKUP_1);   
    // Enable RT mode
    DefMode(rtModId, RT);   
  //  RegWrite(rtModId,CONFIG_REG3,0x8000,1);
    
    for (Sa = 1; Sa <31; Sa++)
    {
      for (i = 0; i < BUFFER_SIZE; ++i)
      {
          txBuffer[i] = 0x1553+i;      
      }
      // printf("SA-%d %x\n",Sa,MemRead(modId,0x160+Sa,1));    
      WriteDataBlock(rtModId, Sa, txBuffer);
    }
    //MemWrite(rtModId,0x120,0xa5a5,1);//Enable ServiceReq(TxVectorWord)
    RunRt(rtModId); 
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
            fprintf(RtlogFile, "BSW      : 0x%X\n", message->bsw );

           printf("CMD      : 0x%X\n", rtMsg.cmd);
            fprintf(RtlogFile, "CMD      : 0x%X\n", message->cmd);

           printf("Word Cnt : 0x%X\n", rtMsg.wcnt);
            fprintf(RtlogFile, "Word Cnt : 0x%X\n", message->wcnt) ;

           printf("SubAddr  : %d\n", rtMsg.subaddr);
            fprintf(RtlogFile, "SubAddr  : %d\n", message->subaddr);

            printf("T/R      : 0x%X\n", rtMsg.t_r);
            fprintf(RtlogFile, "T/R      : 0x%X\n", message->t_r);            

            printf("Data     :\n\t");

            fprintf(RtlogFile, "Data     :\n\t");      
	   
            temp = message->wcnt;
            if(temp == 0)
	            temp = 32;
             if(message->subaddr==0)
             {
	       message->data[0] = MemRead(ModId,offset+0,1);
	       fprintf(RtlogFile, "%04X ",message->data[0]);
	       printf("%04X ",message->data[0]);
             }
             else
             {
                for (i=0;i<temp;i++)
                {
                    message->data[i] = MemRead(ModId,offset+i,1);
                    fprintf(RtlogFile, "%04X ",message->data[i]);
                   printf( "%04X ",message->data[i]);
                    if ((i + 1) % 8 == 0)
                    {   printf("\n\t");
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
            // RT_MSG rtMsg;
             ReadMsg1553(modId, pos, &rtMsg,msgCount);
             fprintf(RtlogFile, "\nMessage #%lu\n", msgCount);

             printf("\nMessage #%lu\n", msgCount);
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

