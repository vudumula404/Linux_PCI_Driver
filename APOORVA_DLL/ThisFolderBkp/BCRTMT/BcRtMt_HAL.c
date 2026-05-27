// Simplified and cleaned up version of the original 1553B BC/RT/MT communication test application

#include <stdio.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"

#define BUFFER_SIZE 32
#define MSG_COUNT 4

MSG_DEF_DDC bc_page;
RT_MSG_DDC rtMsg;
MSG_RESULT_DDC msg_return;
MNTPACK_DDC mtBuffer;
unsigned short txBuffer[BUFFER_SIZE], data_buffer[BUFFER_SIZE], frame_items[MSG_COUNT];

void ConfigureRT(UCHAR rtModId, unsigned char rtAddr)
{
    ClearRam_DDC(rtModId);
    ResetAce_DDC(rtModId);
    SetRtAddrM_DDC(rtModId, rtAddr, 1);
    DefLookUp_DDC(rtModId, LOOKUP_1_DDC);
    for (int i = 0; i < BUFFER_SIZE; ++i) txBuffer[i] = 0x1553 + i;
    WriteDataBlock_DDC(rtModId, 1, txBuffer);
    DefMode_DDC(rtModId, RT_DDC);
    RunRt_DDC(rtModId);
}

void ConfigureMT(UCHAR mtModId)
{
    ClearRam_DDC(mtModId);
    ResetAce_DDC(mtModId);
    DefMode_DDC(mtModId, MT_DDC);
    SelMsgMt_DDC(mtModId);
    RunMt_DDC(mtModId);
}

void ConfigureBC(UCHAR bcModId,UCHAR rtAddr, unsigned short data)
{
    ClearRam_DDC(bcModId);
    ResetAce_DDC(bcModId);
    DefMode_DDC(bcModId, BC_DDC);

    for (int i = 0; i < BUFFER_SIZE; ++i) data_buffer[i] = data;

    for (int i = 0; i < MSG_COUNT; ++i) {
        bc_page.bus       = BUS_A_DDC;
        //bc_page.retry_enable =YES;
        bc_page.self_test = NO_DDC;
        bc_page.broadcast = NO;
        bc_page.mode_code = NO;
        bc_page.rt_rt     = NO_DDC;
        bc_page.cmd_1.taddr = /*i +*/ rtAddr+i;
        bc_page.cmd_1.t_r = /*(i < 2) ? */RECEIVE_DDC /*: TRANSMIT_DDC*/;
        bc_page.cmd_1.status = 0xcccc;
        bc_page.loop = 0x0000;
        bc_page.cmd_1.subaddr = 31/* i + 1*/;
        bc_page.cmd_1.wcnt = 2;//BUFFER_SIZE;       
       //BCRetry_DDC(bcModId,1);
        AutoRetry_DDC(bcModId,i + 1,YES);
        DefMsg_DDC(bcModId, i + 1, bc_page, data_buffer, BUFFER_SIZE);
        
        frame_items[i] = i + 1;
    }
     bc_page.bus       = BUS_A_DDC;
        //bc_page.retry_enable =YES;
        bc_page.self_test = NO_DDC;
        bc_page.broadcast = NO;
        bc_page.mode_code = YES;
        bc_page.rt_rt     = NO_DDC;
        bc_page.cmd_1.taddr = /*i +*/ rtAddr+i;
        bc_page.cmd_1.t_r = /*(i < 2) ? */RECEIVE_DDC /*: TRANSMIT_DDC*/;
        bc_page.cmd_1.status = 0xcccc;
        bc_page.loop = 0x0000;
        bc_page.cmd_1.subaddr = 31/* i + 1*/;
        bc_page.cmd_1.wcnt = 2;//BUFFER_SIZE;       
       //BCRetry_DDC(bcModId,1);
        AutoRetry_DDC(bcModId,i + 1,YES);
        DefMsg_DDC(bcModId, i + 1, bc_page, data_buffer, BUFFER_SIZE);
        
        frame_items[i] = i + 1;
   
   
    DefFrame_DDC(bcModId, MSG_COUNT, frame_items);
}

void PrintRTMessages(UCHAR rtModId)
{
    for (int i = 1; i <= MSG_COUNT; ++i) {
        ReadMsg_DDC(rtModId, i, &rtMsg);
        printf("\nRT Msg #%d\nCMD: 0x%X WCnt: %d SubAddr: %d TR: %d\nData:\n", i, rtMsg.cmd, rtMsg.wcnt, rtMsg.subaddr, rtMsg.t_r);
        for (int j = 0; j < (rtMsg.wcnt ? rtMsg.wcnt : BUFFER_SIZE); ++j) {
            printf("%04X ", rtMsg.data[j]);
            if ((j + 1) % 8 == 0) printf("\n");
        }
        printf("\n");
    }
}

void PrintBCResults(UCHAR bcModId)
{
    for (int i = 1; i <= MSG_COUNT; ++i) {
        ReadMsgResult_DDC(bcModId, STACK_1_DDC, i, &msg_return);
        printf("\nBC Msg #%d\nCMD: 0x%X SubAddr: %d WCnt: %d T/R: %d\nData:\n", i, msg_return.cmd_1, msg_return.cmd1_bits.subaddr, msg_return.cmd1_bits.wcnt, msg_return.cmd1_bits.t_r);
        for (int j = 0; j < (msg_return.cmd1_bits.wcnt ? msg_return.cmd1_bits.wcnt : BUFFER_SIZE); ++j) {
            printf("%04X ", msg_return.data[j]);
            if ((j + 1) % 8 == 0) printf("\n");
        }
        printf("\nLoopWord: %04X Status: %04X\n", msg_return.loop_word, msg_return.status_1);
    }
}
void PollMtMessages(UCHAR MTModId)
{
    MNTPACK_DDC mtPacket;
    unsigned short descriptor;
    char temp;

    //printf("\nPolling MT messages (Press Ctrl+C to exit):\n");
    //while (1)
    {
        for (descriptor = 1; descriptor <= MSG_COUNT; descriptor++)
        {
            ReadMonPacket_DDC(MTModId, descriptor, &mtPacket);
            //if (mtPacket.BlockSts.bitwise.Som && mtPacket.BlockSts.bitwise.Eom)
            {
                printf("\n[MT] Packet at descriptor %d\n", descriptor);
                printf("Cmd Word    : 0x%04X\n", mtPacket.RecCmdWrd.value);
                printf("Time Tag    : 0x%04X\n", mtPacket.TimeTag);
                int words = mtPacket.RecCmdWrd.bitwise.WrdCntOrMde;
                if (words == 0) words = 32;
                printf("Data:\n\t");
                for (int i = 0; i < words; i++)
                {
                    printf("%04X ", mtPacket.DataPack[i]);
                    if ((i + 1) % 8 == 0) printf("\n\t");
                }
                printf("\n");
            }
        }
       // usleep(10000); // 10 ms delay
    }
}

int main()
{
    UCHAR bcModId, rtModId, mtModId, rtAddr;
    unsigned short data;

    if (!InitCard(1)) {
        printf("Device initialization failed.\n");
        return -1;
    }
    /*InitInterrupts_DDC( 0,1);
    RegWrite(1,INTMASK_REG_DDC,0x1,1);*/
    printf("Enter BC ModuleId (1-3): "); scanf("%hhu", &bcModId);
    printf("Enter RT ModuleId (1-3): "); scanf("%hhu", &rtModId);
    printf("Enter MT ModuleId (1-3): "); scanf("%hhu", &mtModId);
    printf("Enter RT Address (1-30): "); scanf("%hhu", &rtAddr);
    printf("Enter 16-bit data to send (hex): "); scanf("%hx", &data);

    ConfigureRT(rtModId, rtAddr);
    ConfigureMT(mtModId);
    ConfigureBC(bcModId,rtAddr, data);

    printf("Press Enter to Run BC Frame..."); getchar(); getchar();
    RunBc_DDC(bcModId, STACK_1_DDC, 1, MSG_COUNT, 1);
    getchar();
    printf("\nBC Results: %d",GetM1Cnt_DDC());
    
    PrintBCResults(bcModId);
    printf("\nMT Results:");
    PollMtMessages(mtModId);
    printf("\nRT Messages:");
    PrintRTMessages(rtModId);

    return 0;
}


