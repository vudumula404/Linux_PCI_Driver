#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"

#define MSG_COUNT 128

FILE* logFile = NULL;

void handle_sigint(int sig) {
    printf("\nExiting...\n");
    if (logFile) fclose(logFile);
    exit(0);
}

void ReadMonPacket1553_DDC(UCHAR ModId, unsigned short DespNo, MNTPACK_DDC* MontDataBuff) {
    unsigned short Offset, i, ptr, temp;

    Offset = 0x400 + ((DespNo * 4) - 4);
    MontDataBuff->BlockSts.value = MemRead(ModId, Offset, 1);
    MontDataBuff->TimeTag = MemRead(ModId, Offset + 1, 1);
    MontDataBuff->DataBlkPtr = MemRead(ModId, Offset + 2, 1);
    ptr = MontDataBuff->DataBlkPtr;
    MontDataBuff->RecCmdWrd.value = MemRead(ModId, Offset + 3, 1);

    temp = MontDataBuff->RecCmdWrd.bitwise.WrdCntOrMde;
    if (temp == 0) temp = 32;

    if (MontDataBuff->RecCmdWrd.bitwise.TrnRecv == 1) {
        ptr++;
        if (ptr == 0x1000) ptr = 0x800;

        for (i = 0; i < temp; i++) {
            MontDataBuff->DataPack[i] = MemRead(ModId, ptr, 1);
            ptr = (ptr + 1 == 0x1000) ? 0x800 : ptr + 1;
        }
    } else {
        for (i = 0; i < temp; i++) {
            MontDataBuff->DataPack[i] = MemRead(ModId, ptr, 1);
            ptr = (ptr + 1 == 0x1000) ? 0x800 : ptr + 1;
        }
    }
}
void MtCmdStackRollOver(unsigned char ModId,unsigned short RollOver)
{
  if(RollOver==64)
  {
    RegWrite(ModId, CONFIG_REG2_DDC, 0x8000, 1);
    RegWrite(ModId, INTMASK_REG_DDC, 0x0800, 1);
    RegWrite(ModId, CONFIG_REG3_DDC, 0x8000, 1);
  }
}
void SelMsgMt(UCHAR ModId) {
    RegWrite(ModId, CONFIG_REG3_DDC, 0x8000, 1);
   // RegWrite(ModId, CONFIG_REG3_DDC, 0x8500, 1);
    RegWrite(ModId, CONFIG_REG3_DDC, 0x9100, 1);
    RegWrite(ModId, CONFIG_REG_DDC, 0x5000, 1);
    MemWrite(ModId, 0x102, 0x400, 1);  // CMD stack pointer
    MemWrite(ModId, 0x103, 0x800, 1);  // Data stack pointer

    for (unsigned short i = 0; i < 128; i++) {
        MemWrite(ModId, 0x280 + i, 0xFFFF, 1);
    }
}

int MtMsgCnt(unsigned char MTModId) {
    unsigned short CMDDesc = MemRead(MTModId, 0x102, 1);
    return ((CMDDesc - 0x1000) / 4);
}

int main() {
    MNTPACK_DDC buffer;
    unsigned char MTModId = 0;
    unsigned short currentDesc = 0, lastDesc = 0, pos = 1, Lcount = 0;
    unsigned short cmd=0,rtAddr=0,trBit=0,subAddr=0,wc_or_mode=0;

    signal(SIGINT, handle_sigint); // Graceful Ctrl+C handling

    if (!InitCard(1)) {
        printf("Card Init Failed\n");
        return -1;
    }
    printf("Card Init Success\n");

    printf("Select MT ModuleId (1/2/3): ");
    scanf("%hhu", &MTModId);
    getchar(); // to consume leftover newline

    ClearRam_DDC(MTModId);
    ResetAce_DDC(MTModId);
    DefMode_DDC(MTModId, MT_DDC);
    SelMsgMt(MTModId);
    MtCmdStackRollOver(MTModId,64);
    RunMt_DDC(MTModId);
    /*ClearRam_DDC(MTModId);
    ResetAce_DDC(MTModId);
    DefMode_DDC(MTModId, MT_DDC);
    SelMsgMt(MTModId);
    RunMt_DDC(MTModId);*/

    printf("Press Enter to display MT results...\n");
    getchar();

    logFile = fopen("mt_log.txt", "w");
    if (!logFile) {
        perror("Failed to open log file");
        return -1;
    }

    lastDesc = MtMsgCnt(MTModId);

    while (1) {
        currentDesc = MtMsgCnt(MTModId);

        if (lastDesc != currentDesc) 
        {
            ReadMonPacket1553_DDC(MTModId, pos, &buffer);
            
             //ReadMonPacket_DDC(MTModId, pos, &buffer);
           /*  for(unsigned short MemD=0x400;MemD<0x420;MemD++)
              printf("Address %x MeMD %x\n",MemD,MemRead(MTModId, MemD, 1));*/
              Lcount++;
             
              printf("\n*************** pos %d MT Packet #%d Desc %d CommandTack %x***************\n", pos,Lcount, currentDesc,MemRead(MTModId, 0x400+((pos*4)-4)+3, 1));
             /* printf("Block Status Word: 0x%04X\n", buffer.BlockSts.value);
              printf("Cmd Word         : 0x%04X\n", buffer.RecCmdWrd.value);
              printf("Time Tag         : 0x%04X\n", buffer.TimeTag);*/

              unsigned short words = buffer.RecCmdWrd.bitwise.WrdCntOrMde;
              if (words == 0) words = 32;

             /* printf("Data Words       :\n");
              for (int i = 0; i < words; i++) {
                  printf("%04X ", buffer.DataPack[i]);
                  if ((i + 1) % 8 == 0) printf("\n");
              }
              printf("\n");*/

              // Logging
              time_t now = time(NULL);
              struct tm* t = localtime(&now);
              fprintf(logFile, "\n[%02d:%02d:%02d] Packet #%d (Desc %d)\n",
                      t->tm_hour, t->tm_min, t->tm_sec, Lcount, currentDesc);
              fprintf(logFile, "Block Status Word: 0x%04X\n", buffer.BlockSts.value);             
              fprintf(logFile, "Time Tag         : 0x%04X\n", buffer.TimeTag);
              fprintf(logFile, "Cmd Word         : 0x%04X\n", buffer.RecCmdWrd.value);
              
              cmd = buffer.RecCmdWrd.value;
              rtAddr = (cmd >> 11) & 0x1F;
              trBit  = (cmd >> 10) & 0x01;
              subAddr = (cmd >> 5) & 0x1F;
              wc_or_mode = cmd & 0x1F;

              if (wc_or_mode == 0) 
                wc_or_mode = 32;

              char dir = (trBit == 1) ? 'T' : 'R';  // T = Transmit, R = Receive
              
              fprintf(logFile,"Decoded Cmd      : %u-%u-%c-%u\n", rtAddr, subAddr, dir, wc_or_mode);
              fprintf(logFile, "Data             : ");
              for (int i = 0; i < words; i++) {
                  fprintf(logFile, "%04X ", buffer.DataPack[i]);
                  if ((i + 1) % 8 == 0) fprintf(logFile, "\n");
              }
              fprintf(logFile, "\n");
              fflush(logFile);           
            if(pos==128)
            {
              ClearRam_DDC(MTModId);
              ResetAce_DDC(MTModId);
              DefMode_DDC(MTModId, MT_DDC);
              SelMsgMt(MTModId);
              //MtCmdStackRollOver(MTModId,64);
              RunMt_DDC(MTModId);
              printf("Memory Cleared\n");
            }
            pos = (pos >= MSG_COUNT) ? 1 : pos + 1;           
            lastDesc = currentDesc;
        }
    }

    return 0;
}

