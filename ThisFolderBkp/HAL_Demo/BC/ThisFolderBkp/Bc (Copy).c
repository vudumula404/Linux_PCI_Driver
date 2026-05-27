#include <stdio.h>
#include <stdint.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"

#define BUFFER_SIZE 32
#define HIGH  1
#define LOW  0
/*
 * Kept globals that are actively used by PCI API calls and message definition.
 * Removed unused temporaries and redundant buffers.
 */
MSG_DEF bc_page;
unsigned short data_buffer[40];
MAJOR_FRAME major_frames[128];
/*******************************************************************
*  This Program gives code for All types of BC Message Schedulers
********************************************************************/
int main(void)
{    
    int idx = 0;
    unsigned short msg_sequence[1024];
    unsigned short minor_cycle_index = 0;
    unsigned short msg_count = 0;
    unsigned short minor_cycle_count = 0;
    unsigned short minor_cycle_time = 0;
    unsigned short inter_msg_gap = 0;
    unsigned short msg_no = 0;
    unsigned short data_value = 0,Retry=0,BCResponseTime=0;
    unsigned char card_id = 0;
    unsigned char fail_count = 0;
    unsigned char module_id = 0;
    unsigned char mt_module_id = 3;
    unsigned char mode_select = 0,Enable,BcRetryEnable=0;
    int channel = 0; /* 0 = CH-B, 1 = CH-A */
    int run_times = 1;
    int use_frame_scheduler = 0;
    unsigned short dynamic_data[32];
    
    int init_status = InitCard(1);
    ResetAce(1);
    if (init_status)
        printf("Card Init Success %d\n", init_status);
    else
        printf("Card Init Failed %d\n", init_status);

    printf("\n\t\tSelect BC ModuleId:1/2/3 :");
    scanf("%hhu", &module_id);

    printf("\n\t\tEnter Channel (CH-A:1, CH-B:0) :");
    scanf("%d", &channel);
    printf("\n\t\tEnable BcRetryEnable :Enable[1]Disable[0]:");
    scanf("%d", &BcRetryEnable);
    if(BcRetryEnable)
    {
      printf("\n\t\tSingleRetry Option : Same[1] OR Alter[2]");
      printf("\n\t\tDoubleRetry Option : SS[3] OR AA[4] OR SA[5] OR AS[6]\t");
      scanf("%d",&Retry);
    }
    printf("\n\t\tEnable BCResponseTime :Enable[1]Disable[0]:");
    scanf("%d", &Enable);
    if(Enable)
    {
      printf("\n\t\tEnter BCResponseTime :1/2/3/4\t");
      scanf("%d", &BCResponseTime);
    }
    printf("\n\t\tEnter Number Of Messages Count :");
    scanf("%hu", &msg_count);
    printf("\n\t\tEnter Messages/Frame Based Scheduler (0Message/1Frame):");
    scanf("%d", &use_frame_scheduler);

    if (use_frame_scheduler == 1)
    {
        printf("\n\t\tEnter Number Of MinorCycle Count :");
        scanf("%hu", &minor_cycle_count);
        printf("\n\t\tEnter Number Of MinorCycle Time :");
        scanf("%hu", &minor_cycle_time);
    }
    
    printf("\n\t\tEnter Number Of Messages To Run (Times*Messages):");
    scanf("%d", &run_times);
 
    (void)ClearRam(module_id);
    ResetAce(module_id);
     //RegWrite(module_id,0x7,0x8000,1);
    (void)DefMode(module_id, BC);
    
    BCResponseTimeE(module_id,Enable,BCResponseTime);
    
    if (channel == 1)
        bc_page.bus = BUS_A;
    else
        bc_page.bus = BUS_B;

    for (msg_no = 1; msg_no <= msg_count; msg_no++)
    {
        /* Reset defaults for every message */
        static int rt_address = 0;
        if (rt_address >= 30)
            rt_address = 0;

        bc_page.self_test = NO;
        bc_page.mask_bcst_bit = NO;
        bc_page.broadcast = NO;
        bc_page.mode_code = NO;
        bc_page.rt_rt = NO;
        bc_page.retry_enable=YES;
        bc_page.cmd_1.taddr = ++rt_address;
        bc_page.cmd_1.t_r = RECEIVE; /* default */

        if (msg_no <= 100)
        {
            bc_page.cmd_1.t_r = RECEIVE;//RECEIVE;        /* 0–100 → BC TO RT */
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            bc_page.loop = 0x0000;
            for (idx = 0; idx < 32; idx++)
                data_buffer[idx] = ++data_value;

            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no <= 200)
        {
            bc_page.cmd_1.t_r = TRANSMIT;        /* 100–200 → RT TO BC */
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            bc_page.loop = 0x0000;
            for (idx = 0; idx < 32; idx++)
                data_buffer[idx] = 0;

            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no <= 300)
        {
            /* Broadcast */
            bc_page.cmd_1.taddr = 31;
            bc_page.broadcast = YES;
            bc_page.cmd_1.t_r = RECEIVE;
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            bc_page.loop = 0x0000;
            for (idx = 0; idx < 32; idx++)
                data_buffer[idx] = ++data_value;

            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no <= 400)
        {
            /* RT-to-RT */
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            bc_page.loop = 0x0000;
            bc_page.cmd_1.t_r = RECEIVE;
            bc_page.cmd_2.t_r = TRANSMIT;
            bc_page.rt_rt = YES;
            bc_page.cmd_2.taddr = (rt_address == 30) ? 1 : 30;
            bc_page.cmd_2.status = 0xCCCC;
            bc_page.cmd_2.subaddr = 1;
            bc_page.cmd_2.wcnt = 32;
            //bc_page.cmd_2.loop = 0x0000;
            for (idx = 0; idx < 32; idx++)
                data_buffer[idx] = 0;

            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no <= 500)
        {
            bc_page.broadcast = YES;
            bc_page.cmd_1.taddr = 31;
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            bc_page.loop = 0x0000;
            bc_page.cmd_1.t_r = RECEIVE;
            bc_page.cmd_2.t_r = TRANSMIT;
            bc_page.rt_rt = YES;
            bc_page.cmd_2.taddr = (rt_address == 30) ? 1 : rt_address;
            bc_page.cmd_2.status = 0xCCCC;
            bc_page.cmd_2.subaddr = 1;
            bc_page.cmd_2.wcnt = 32;
            //bc_page.cmd_2.loop = 0x0000;
            for (idx = 0; idx < 32; idx++)
                data_buffer[idx] = 0;

            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
        else if (msg_no >= 601 && msg_no <= 619)
        {
            int datacount = 0;
            int mode_code = msg_no - 601; /* 0..31 */

            /* Initialize common mode code settings */
            bc_page.self_test = NO;
            bc_page.mask_bcst_bit = NO;
            bc_page.broadcast = NO;
            bc_page.rt_rt = NO;
            bc_page.mode_code = YES;
            bc_page.cmd_1.taddr = 1; /* Default to RT address 1 */
            bc_page.cmd_1.subaddr = 0x00; /* Mode codes always subaddr 0 */
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.loop = 0x0000;

            switch (mode_code)
            {
                case 0: /* Dynamic Bus Control Acceptance (DBCA) W/O Data */
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 0;
                    datacount = 0;
                    break;

                case 1: /* Sync W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 1;
                    datacount = 0;
                    break;

                case 2: /* Transmit Status W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 2;
                    datacount = 0;
                    break;

                case 3: /* Initiate Self Test W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 3;
                    datacount = 0;
                    break;

                case 4: /* Transmitter Shut Down W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 4;
                    datacount = 0;
                    break;

                case 5: /* Override Transmitter Shut Down W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 5;
                    datacount = 0;
                    break;

                case 6: /* Inhibit Terminal Flag W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 6;
                    datacount = 0;
                    break;

                case 7: /* Override Inhibit Terminal Flag W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 7;
                    datacount = 0;
                    break;

                case 8: /* Reset Remote Terminal W/O Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 8;
                    datacount = 0;
                    break;

                case 16: /* Transmit Vector Word With Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 16;
                    datacount = 1;
                    data_buffer[0] = 0x0000;
                    break;

                case 17: /* Sync With Data (Broadcast Receive) */
                    bc_page.broadcast = YES;
                    bc_page.cmd_1.taddr = 31; /* Broadcast address */
                    bc_page.cmd_1.t_r = RECEIVE;
                    bc_page.cmd_1.wcnt = 17;
                    datacount = 1;
                    data_buffer[0] = ++data_value;
                    break;

                case 18: /* Transmit Last Command With Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.subaddr = 0;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 18;
                    datacount = 1;
                    data_buffer[0] = 0x0000;
                    break;
                case 20: /* Transmit Built-In-Test With Data */                   
                    bc_page.broadcast = NO;                  
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 19;
                    datacount = 1;
                    data_buffer[0] = 0x0000;
                    break;

                case 19: /* Selected Transmitter Shutdown With Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = RECEIVE;
                    bc_page.cmd_1.wcnt = 20;
                    datacount = 1;
                    data_buffer[0] = ++data_value;
                    break;

                case 21: /* Override-Selected Tx Shutdown With Data */
                    bc_page.broadcast = NO;
                    bc_page.cmd_1.taddr = 1;
                    bc_page.cmd_1.t_r = RECEIVE;
                    bc_page.cmd_1.wcnt = 21;
                    datacount = 1;
                    data_buffer[0] = ++data_value;
                    break;
                  
                default: /* Undefined mode codes 9-15, 22-31 */
                    bc_page.cmd_1.t_r = TRANSMIT;
                    bc_page.cmd_1.wcnt = 0;
                    datacount = 0;
                    data_buffer[0] = 0;
                    break;
            }

            /* Define the Mode Code message */          
            DefMsg(module_id, msg_no, bc_page, data_buffer, datacount, inter_msg_gap);
        }
        else
        {
            bc_page.cmd_1.t_r = RECEIVE;        /* 500–1000 → BC TO RT */
            bc_page.cmd_1.status = 0xCCCC;
            bc_page.cmd_1.subaddr = 1;
            bc_page.cmd_1.wcnt = 32;
            bc_page.loop = 0x0000;
            for (idx = 0; idx < 32; idx++)
                data_buffer[idx] = ++data_value;

            DefMsg(module_id, msg_no, bc_page, data_buffer, 32, inter_msg_gap);
        }
    }
    
    if (use_frame_scheduler == 1)
    {
        for (minor_cycle_index = 0; minor_cycle_index < minor_cycle_count; minor_cycle_index++)
        {
            int start = minor_cycle_index * 100;
            int end = start + 100;

            for (idx = start; idx < end; idx++)
            {
                major_frames[minor_cycle_index].frame_items[idx - start] = idx + 1;
            }

            major_frames[minor_cycle_index].Mess_Count = 100;
            major_frames[minor_cycle_index].MinorFrame_Time = minor_cycle_time;
        }

        BCRetry(module_id,Retry);       
           
        DefMajorFrame(module_id, 0, minor_cycle_count, major_frames);
     
        printf("BC Major Frame Started Wait........! \n");
        RunBcMajorFrame(module_id, 0, minor_cycle_count, major_frames, run_times);
        if(run_times==0)
        {
          printf("Enter For HIGH Priority Messages \n");
          getchar();
          getchar();     
          printf("HIGH Priority Mode Initiated\n");
          PriorityHighLow(module_id, 50, 1);
           for (idx = 0; idx < 32; idx++)
              dynamic_data[idx] = 0xAAAA + idx; 
          UpdateMsgData(module_id, 50, dynamic_data, 32, RECEIVE);          
          printf("Enter For LOW Priority Messages \n");
          getchar();            
          printf("Low Priority Mode Initiated\n");
          PriorityHighLow(module_id, 50, 2);
           for (idx = 0; idx < 32; idx++)
              dynamic_data[idx] = 0xBBBB + idx; 
          UpdateMsgData(module_id, 50, dynamic_data, 32, RECEIVE);  
          printf("Enter For Dynamic Message updation 95 to 99 \n");
          getchar();         
          unsigned short cmd_update_data[32];
              
          for (idx = 0; idx < 32; idx++)
              cmd_update_data[idx] = 0xa5a6 + idx;
          UpdateMsgCmdData(module_id,
                           95,
                           0x0820,
                           0x0000,
                           cmd_update_data,
                           32,
                           RECEIVE,
                           channel);

          for (idx = 0; idx < 32; idx++)
              cmd_update_data[idx] = 0xa5a7 + idx;
          UpdateMsgCmdData(module_id,
                           96,
                           0xf811,
                           0x0000,
                           cmd_update_data,
                           17,
                           3, /* ModeCode */
                           channel);

          for (idx = 0; idx < 32; idx++)
              cmd_update_data[idx] = 0xa5a8 + idx;
          UpdateMsgCmdData(module_id,
                           97,
                           0x1420,
                           0x0000,
                           cmd_update_data,
                           32,
                           TRANSMIT,
                           channel);

          for (idx = 0; idx < 32; idx++)
              cmd_update_data[idx] = 0xa5a9 + idx;
          UpdateMsgCmdData(module_id,
                           98,
                           0xfba2,
                           0x0000,
                           cmd_update_data,
                           32,
                           RECEIVE,
                           channel);

          for (idx = 0; idx < 32; idx++)
              cmd_update_data[idx] = 0xa5a9 + idx;
          UpdateMsgCmdData(module_id,
                           99,
                           0x0820,
                           0x1420,
                           cmd_update_data,
                           32,
                           2, /* RT-RT */
                           channel);
           
         /* RegWrite(module_id,0x3,0x4000,1);
          printf("BC Stopped Enter To Restart \n");
           printf("ResetSameBus(S) Reg(0x1) %x Reg(0x4) %x Reg(0x3) %x\n",RegRead(module_id,CONFIG_REG,1),RegRead(module_id,CONFIG_REG4,1),RegRead(module_id,CONFIG_REG3,1));
          getchar();   
          printf("BC Started\n");
          StartReset(module_id,START);
          printf("Wait.>>>>!\n");
          printf("StartSameBus(S) Reg(0x1) %x Reg(0x4) %x Reg(0x3) %x\n",RegRead(module_id,CONFIG_REG,1),RegRead(module_id,CONFIG_REG4,1),RegRead(module_id,CONFIG_REG3,1));
          getchar();   
          */     
          printf("Dynamic Message updation 95 to 99 Completed Enter to Reset \n");
        }
        getchar();       
    }
    else
    {
        for (idx = 0; idx < (int)msg_count; idx++)
            msg_sequence[idx] = idx + 1;
        BCRetry(module_id,Retry);
        DefFrame(module_id, msg_count, msg_sequence);   
        RunBc(module_id, 0, 1, msg_count, run_times);        
      /*  getchar();
        getchar();*/
       /* RegWrite(module_id,0x3,0x4000,1);
        printf("BC Stopped Enter To Restart \n");
        // printf("ResetSameBus(S) Reg(0x1) %x Reg(0x4) %x Reg(0x3) %x\n",RegRead(module_id,CONFIG_REG,1),RegRead(module_id,CONFIG_REG4,1),RegRead(module_id,CONFIG_REG3,1));
        getchar();   
        printf("BC Started\n");
        StartReset(module_id,START);	*/
        printf("Wait...!\n");
        if(run_times==0)
        {
            printf("Enter For Dynamic Message updation 95 to 99 \n");
            getchar();
            getchar();
            unsigned short cmd_update_data[32];

            for (idx = 0; idx < 32; idx++)
                cmd_update_data[idx] = 0xa5a6 + idx;
            UpdateMsgCmdData(module_id,
                             95,
                             0x0820,
                             0x0000,
                             cmd_update_data,
                             32,
                             RECEIVE,
                             channel);

            for (idx = 0; idx < 32; idx++)
                cmd_update_data[idx] = 0xa5a7 + idx;
            UpdateMsgCmdData(module_id,
                             96,
                             0xf811,
                             0x0000,
                             cmd_update_data,
                             17,
                             3, /* ModeCode */
                             channel);

            for (idx = 0; idx < 32; idx++)
                cmd_update_data[idx] = 0xa5a8 + idx;
            UpdateMsgCmdData(module_id,
                             97,
                             0x1420,
                             0x0000,
                             cmd_update_data,
                             32,
                             TRANSMIT,
                             channel);

            for (idx = 0; idx < 32; idx++)
                cmd_update_data[idx] = 0xa5a9 + idx;
            UpdateMsgCmdData(module_id,
                             98,
                             0xfba2,
                             0x0000,
                             cmd_update_data,
                             32,
                             RECEIVE,
                             channel);

            for (idx = 0; idx < 32; idx++)
                cmd_update_data[idx] = 0xa5a9 + idx;
            UpdateMsgCmdData(module_id,
                             99,
                             0x0820,
                             0x1420,
                             cmd_update_data,
                             32,
                             2, /* RT-RT */
                             channel);
        }
        else
        {
         printf("Enter to Device Reset..!\n");
          getchar();
        }
    }
    
    getchar();
    RegWrite(module_id, 0x07, 0x0000, 1);
    ResetAce(module_id);
    printf("Device Reset.........!\n");
    getchar();

    return 0;
}

