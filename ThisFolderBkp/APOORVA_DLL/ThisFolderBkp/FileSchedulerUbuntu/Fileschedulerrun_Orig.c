#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"
#define MAX_SOURCE_ADDRESS              31
#define MAX_DEST_ADDRESS                31
#define MAX_SOURCE_SUBADDRESS           31
#define MAX_DEST_SUBADDRESS             31
#define MAX_WORD_COUNT                  32
#define MAX_MINOR_FRAME_NUMBER          7
#define TIMEBROADCASTCHNNLBMSGID        17
#define MINORFRAMETIME                  20 // ms
#define TIMEBROADCASTCHNNLAMSGID        16

#define CARD1 1
#define RECEIVE 0
#define TRANSMIT 1
#define YES 1
#define NO 0
#define CH_A 0
#define CH_B 1
#define MINOR_CYCLE_ZERO 0
#define MINOR_CYCLE_SEVEN 7
#define MAX_MESSAGE_NAME_LEN 100
#define MAX_MESSAGES 200
UCHAR data_buffer[32];

//typedef unsigned short USHORT;
//typedef unsigned char  UCHAR;

/*struct MSG_DEF {
    int Int_Msg_Gap;
    int bus;
    int self_test;
    int retry_enable;
    int mask_bcst_bit;
    int broadcast;
    int mode_code;
    int rt_rt;
    int loop;
    struct {
        int taddr;
        int status;
        int subaddr;
        int wcnt;
        int t_r;
    } cmd_1, cmd_2;
};*/
 MSG_DEF_DDC	bc_page;
struct MSG {
   
   // MSG_DEF_DDC MsgDef;
    unsigned short databuffer[32];
    char szMessageName[MAX_MESSAGE_NAME_LEN];
    unsigned char ubyRxMsgCount;
    unsigned char ubyTxMsgCount;
};

struct MAJOR_FRAME {
    unsigned short frame_items[128];
    unsigned short Mess_Count;
};
struct MSG m_stMsg[MAX_MESSAGES];
struct MAJOR_FRAME m_MinorFrame[10];

unsigned short m_urgswTimeBroadcastData[3];
char m_rgszMessagesDatabase[MAX_MESSAGES][256];
unsigned char m_ubyMessageNumber = 0;
/*
void InitCard_A(int card) {
    printf("[Stub] InitCard_A(%d)\n", card);
}

void DefMsg_A(int card, MSG_DEF* msg) {
    printf("[Stub] DefMsg_A for Addr %d SubAddr %d Wcnt %d\n", msg->cmd_1.taddr, msg->cmd_1.subaddr, msg->cmd_1.wcnt);
}

void RunBc_A(int card) {
    printf("[Stub] RunBc_A on card %d\n", card);
}*/

void fnGetMessageName(unsigned char n, char* name)
{
  sscanf(m_rgszMessagesDatabase[n], "%s", name); 
}
unsigned char fnGetMinorFrame(unsigned char n) 
{
  char b[4]; sscanf(m_rgszMessagesDatabase[n], "%s", b); int v = atoi(b); 
  return (v < 1) ?  1 : ((v > MAX_MINOR_FRAME_NUMBER) ? MAX_MINOR_FRAME_NUMBER + 1 : v); 
}
unsigned char fnGetWordCount(unsigned char n)
{ 
char b[25];
sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%*s%*s%*s%*s%*s%s", b); 
int v = atoi(b); 
return (v < 1) ? 1 : ((v > MAX_WORD_COUNT) ? MAX_WORD_COUNT : v); 
}
unsigned char fnGetChannelSel(unsigned char n)
{ 
char b[25]; 
sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%*s%*s%*s%*s%*s%s", b);
return (strcmp(b, "A") == 0) ? CH_A : CH_B; 
}
unsigned char fnGetDestSubAddress(unsigned char n) { char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%*s%*s%*s%s", b); if (strcmp(b, "--") == 0) return 32; int v = atoi(b); return (v < 0) ? 0 : ((v > MAX_DEST_SUBADDRESS) ? MAX_DEST_SUBADDRESS : v); }
unsigned char fnGetSourceSubAddress(unsigned char n) { char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%*s%*s%s", b); if (strcmp(b, "--") == 0) return 32; int v = atoi(b); return (v < 0) ? 0 : ((v > MAX_SOURCE_SUBADDRESS) ? MAX_SOURCE_SUBADDRESS : v); }
unsigned char fnGetDestAddress(unsigned char n) { char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%*s%s", b); if (strcmp(b, "--") == 0) return 32; int v = atoi(b); return (v < 0) ? 0 : ((v > MAX_DEST_ADDRESS) ? MAX_DEST_ADDRESS : v); }
unsigned char fnGetSourceAddress(unsigned char n) { char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%s", b); if (strcmp(b, "--") == 0) return 32; int v = atoi(b); return (v < 0) ? 0 : ((v > MAX_SOURCE_ADDRESS) ? MAX_SOURCE_ADDRESS : v); }
unsigned char fnGetFrequency(unsigned char n) { char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%*s%*s%*s%*s%*s%*s%s", b); if (strcmp(b, "6.25") == 0) return 160; if (strcmp(b, "12.5") == 0) return 80; if (strcmp(b, "25") == 0) return 40; if (strcmp(b, "50") == 0) return 20; return 160; }

void fnUpdateTimeBroadcastInfo()
{
    unsigned char ms = m_urgswTimeBroadcastData[1] & 0xFF;
    unsigned char sec = m_urgswTimeBroadcastData[1] >> 8;
    unsigned char min = m_urgswTimeBroadcastData[0] & 0xFF;
    unsigned char hr  = m_urgswTimeBroadcastData[0] >> 8;
    ms += 8;
    if (ms > 49) { ms %= 50; if (++sec == 60) { sec = 0; if (++min == 60) { min = 0; if (++hr == 255) hr = 0; } } }
    m_urgswTimeBroadcastData[1] = (sec << 8) | ms;
    m_urgswTimeBroadcastData[0] = (hr << 8) | min;
}

void fnExtractMessages(const char* fname) {
    FILE* fp = fopen(fname, "r"); if (!fp) { printf("Cannot open %s\n", fname); exit(1); }
    char line[256]; for (int i = 0; i < 3; ++i) fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        if (strlen(line) < 3) continue;
        strcpy(m_rgszMessagesDatabase[m_ubyMessageNumber], line);
        unsigned char mf = fnGetMinorFrame(m_ubyMessageNumber);
        unsigned char freq = fnGetFrequency(m_ubyMessageNumber);
        unsigned char da = fnGetDestAddress(m_ubyMessageNumber);
        unsigned char dsa = fnGetDestSubAddress(m_ubyMessageNumber);
        unsigned char ch = fnGetChannelSel(m_ubyMessageNumber);
        unsigned char wc = fnGetWordCount(m_ubyMessageNumber);
        fnGetMessageName(m_ubyMessageNumber, m_stMsg[m_ubyMessageNumber].szMessageName);
       // MSG* msg = &m_stMsg[m_ubyMessageNumber];
        struct MSG* msg = &m_stMsg[m_ubyMessageNumber];
        //msg->MsgDef.Int_Msg_Gap = 10;
        bc_page.bus = ch;
        bc_page.retry_enable = YES;
        bc_page.broadcast = NO;
        bc_page.rt_rt = NO;
        bc_page.cmd_1.taddr = da;
        bc_page.cmd_1.subaddr = dsa;
        bc_page.cmd_1.wcnt = wc;
        //DefMsg_DDC(CARD1, &msg->MsgDef);
        DefMsg_DDC(1,m_ubyMessageNumber,bc_page,data_buffer,32);
        switch (freq) {
            case 160: m_MinorFrame[mf].frame_items[m_MinorFrame[mf].Mess_Count++] = m_ubyMessageNumber; break;
            case 80: for (int i = 0; i < 2; ++i) m_MinorFrame[(mf + i * 4) % 8].frame_items[m_MinorFrame[(mf + i * 4) % 8].Mess_Count++] = m_ubyMessageNumber; break;
            case 40: for (int i = 0; i < 4; ++i) m_MinorFrame[(mf + i * 2) % 8].frame_items[m_MinorFrame[(mf + i * 2) % 8].Mess_Count++] = m_ubyMessageNumber; break;
            case 20: for (int i = 0; i < 8; ++i) m_MinorFrame[i].frame_items[m_MinorFrame[i].Mess_Count++] = m_ubyMessageNumber; break;
        }
        m_ubyMessageNumber++;
    }
    fclose(fp);
    printf("Loaded %d messages from file.\n", m_ubyMessageNumber);
}

void fnHandle1553BCommunication() {
    for (int i = 0; i < 8; ++i) {
        printf("\n[Minor Frame %d Execution]\n", i);
        if (i == 0) fnUpdateTimeBroadcastInfo();
        for (int j = 0; j < m_MinorFrame[i].Mess_Count; ++j) {
            int msgId = m_MinorFrame[i].frame_items[j];
            //MSG* msg = &m_stMsg[msgId];
            struct MSG* msg = &m_stMsg[msgId];
            typedef struct MSG_DEF* def = &msg->MsgDef;
            printf("  Msg %d: %s | Bus %d | Addr %d SubAddr %d WC %d\n",
                   msgId, msg->szMessageName, def->bus,
                   def->cmd_1.taddr, def->cmd_1.subaddr, def->cmd_1.wcnt);
            usleep(MINORFRAMETIME * 1000);
        }
    }
    printf("\n[Schedule Complete]\n");
}

void* ThreadProc_BC(void* arg) {
    fnHandle1553BCommunication();
    return NULL;
}

int main() {
    printf("Initializing MIL-STD-1553B Schedule Simulation (Linux Threaded)\n");
    InitCard_A(CARD1);
    fnExtractMessages("pmc_b1.txt");
    RunBc_A(CARD1);

    pthread_t threadBC;
    if (pthread_create(&threadBC, NULL, ThreadProc_BC, NULL) != 0) {
        perror("Failed to create BC thread");
        return 1;
    }

    pthread_join(threadBC, NULL);
    printf("Simulation thread completed.\n");
    return 0;
}
