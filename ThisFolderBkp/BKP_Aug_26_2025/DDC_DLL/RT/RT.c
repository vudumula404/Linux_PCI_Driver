#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"


#define BUFFER_SIZE 32

volatile sig_atomic_t stop = 0;

void handle_sigint(int sig) {
    stop = 1;
    printf("\n Stopping RT polling loop...\n");
}

int main() {
    int ret, modId, i, msgIdx,RtAdres=0;
    unsigned short wordCount;
    unsigned short txBuffer[BUFFER_SIZE];
    RT_MSG_DDC rtMsg;

    // Initialize DDC card
    ret = InitCard(1);
    if (ret < 0) {
        printf("\nERROR: Failed to acquire device handle. Exiting.\n");
        return 1;
    }
    printf("CCPMC device opened successfully (Handle: %d)\n", ret);

    // Select RT module
    printf("Enter RT Module ID (1/2/3): ");
    scanf("%d", &modId);
    printf("Enter Any RT Address (between 1 to 30): ");
    scanf("%d", &RtAdres);

    // Basic RT setup
    ClearRam_DDC(modId);
    ResetAce_DDC(modId);

    SetRtAddrM_DDC(modId, RtAdres, 1);  // Set RT address = 2
    DefLookUp_DDC(modId, LOOKUP_1_DDC);

    // Fill transmit buffer
    for (i = 0; i < BUFFER_SIZE; ++i)
        txBuffer[i] = 0x1553+i;
    WriteDataBlock_DDC(modId, 1, txBuffer);

    // Enable RT mode
    ret = DefMode_DDC(modId, RT_DDC);
    RunRt_DDC(modId);
    printf("RT is now running. Press Enter to continue...\n");    
    int msgNo = 1;
    while (!stop) {
        ret = ReadMsg_DDC(modId, msgNo, &rtMsg);
        if (ret == 0 && rtMsg.bsw != 0) {  // BSW != 0 indicates a valid message
            printf("\n Message #%d\n", msgNo);
            printf("BSW      : 0x%X\n", rtMsg.bsw);
            printf("CMD      : 0x%X\n", rtMsg.cmd);
            printf("Word Cnt : 0x%X\n", rtMsg.wcnt);
            printf("SubAddr  : 0x%X\n", rtMsg.subaddr);
            printf("T/R      : 0x%X\n", rtMsg.t_r);

            wordCount = rtMsg.wcnt ? rtMsg.wcnt : BUFFER_SIZE;
            printf("Data     :\n\t");

            for (i = 0; i < wordCount; ++i) {
                printf("%04X ", rtMsg.data[i]);
                if ((i + 1) % 8 == 0) printf("\n\t");
            }
            fflush(stdout);
        }

        msgNo++;
        if (msgNo > 64) msgNo = 1;  // wrap around or limit

        //usleep(50000);  // poll every 50ms
        usleep(20000);  // poll every 1ms
    }
    printf("\n RT stopped.\n");
    return 0;
}

