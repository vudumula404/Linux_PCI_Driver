#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
#define MAX_MESSAGE_NAME_LEN 100
#define MAX_MESSAGES 200
char RtRtFlag=0;
USHORT data_buffer[32];
UCHAR MsgPos1=0,MsgPos2=0,MsgPos3=0,MsgPos4=0,MsgPos5=0,MsgPos6=0,MsgPos7=0,MsgPos8=0;
UCHAR POS1=0,wc=32;
typedef struct {
    MSG_DEF_DDC MsgDef;
    unsigned short databuffer[32];
    char szMessageName[MAX_MESSAGE_NAME_LEN];
    unsigned char ubyRxMsgCount;
    unsigned char ubyTxMsgCount;
} MSG;
 MSG_DEF_DDC	bc_page;
typedef struct {
    unsigned short frame_items[128];
    unsigned short Mess_Count;
} MAJOR_FRAME;
unsigned short frame_items[128];
MSG m_stMsg[MAX_MESSAGES];
MAJOR_FRAME m_MinorFrame[10];
MSG* msg = &m_stMsg[200];
unsigned short m_urgswTimeBroadcastData[3];
char m_rgszMessagesDatabase[MAX_MESSAGES][256];
unsigned char m_ubyMessageNumber = 0,ModId=1;

// ---- EXTRACTORS ----
void fnGetMessageName(unsigned char n, char* name) {
    sscanf(m_rgszMessagesDatabase[n], "%s", name);
}
unsigned char fnGetMinorFrame(unsigned char n) {
    char b[4]; sscanf(m_rgszMessagesDatabase[n], "%s", b);
    int v = atoi(b); return (v < 1) ? 1 : ((v > MAX_MINOR_FRAME_NUMBER) ? MAX_MINOR_FRAME_NUMBER + 1 : v);
}
unsigned char fnGetWordCount(unsigned char n) 
{   
    char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%*s%*s%*s%*s%s", b);
    int v = atoi(b); 
     return (v < 1) ? 1 : ((v > MAX_WORD_COUNT) ? MAX_WORD_COUNT : v);
}
unsigned char fnGetChannelSel(unsigned char n) 
{
     char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%*s%*s%*s%s", b);
   // printf("value of b %s\n",b);
    return (strcmp(b, "A") == 0) ? CH_A : CH_B;
}
unsigned char fnGetDestSubAddress(unsigned char n) 
{
    char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%*s%*s%s", b);
     if (strcmp(b, "--") == 0) return 0/*32*/;
    int v = atoi(b); 
   // printf("destsubaddrsvalue of v %d\n",v);
    return (v < 0) ? 0 : ((v > MAX_DEST_SUBADDRESS) ? MAX_DEST_SUBADDRESS : v);
}
unsigned char fnGetSourceSubAddress(unsigned char n) {
     char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%*s%s", b);
    if (strcmp(b, "--") == 0)
    return 0/*32*/;
    int v = atoi(b);
    //printf("Sourcesubaddrsvalue of v %d\n",v);
    return (v < 0) ? 0 : ((v > MAX_SOURCE_SUBADDRESS) ? MAX_SOURCE_SUBADDRESS : v);
}
unsigned char fnGetDestAddress(unsigned char n) {
    char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%s", b);
   if (strcmp(b, "--") == 0) return 0/*32*/;
    int v = atoi(b);
    //printf("value of v %d\n",v);
    return (v < 0) ? 0 : ((v > MAX_DEST_ADDRESS) ? MAX_DEST_ADDRESS : v);
}
unsigned char fnGetSourceAddress(unsigned char n) {
    //char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%s", b);
    char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%s", b);
    if (strcmp(b, "--") == 0) return 0/*32*/;
    int v = atoi(b); return (v < 0) ? 0 : ((v > MAX_SOURCE_ADDRESS) ? MAX_SOURCE_ADDRESS : v);
}
unsigned char fnGetFrequency(unsigned char n)
{   
    char b[25]; sscanf(m_rgszMessagesDatabase[n], "%*s%*s%*s%*s%*s%*s%*s%*s%s", b);
    if (strcmp(b, "6.25") == 0) return 160;
    if (strcmp(b, "12.5") == 0) return 80;
    if (strcmp(b, "25") == 0) return 40;
    if (strcmp(b, "50") == 0) return 20;
    return 160;
}

// ---- TIME ----
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

// ---- LOADER ----
void fnExtractMessages(const char* fname) {
    FILE* fp = fopen(fname, "r");
    if (!fp) { printf("Cannot open %s\n", fname); exit(1); }
    char line[256];  
    
    for (int i = 0; i < 3; ++i) 
      fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) 
    {
        if (strlen(line) < 3) continue;
        strcpy(m_rgszMessagesDatabase[m_ubyMessageNumber], line);
        MSG* msg = &m_stMsg[m_ubyMessageNumber];
        unsigned char MF = fnGetMinorFrame(m_ubyMessageNumber);
        unsigned char freq = fnGetFrequency(m_ubyMessageNumber);
        unsigned char DA = fnGetDestAddress(m_ubyMessageNumber);
        unsigned char SA = fnGetSourceAddress(m_ubyMessageNumber);
        unsigned char SSA = fnGetSourceSubAddress(m_ubyMessageNumber);
        unsigned char DSA = fnGetDestSubAddress(m_ubyMessageNumber);
        unsigned char ch = fnGetChannelSel(m_ubyMessageNumber);
        unsigned char wc = fnGetWordCount(m_ubyMessageNumber);
         //printf("ModeCode--RT %d SA %d MsgNumber %d Ch %d\n",DA,DSA,m_ubyMessageNumber,ch);
        RtRtFlag=0;
        fnGetMessageName(m_ubyMessageNumber, m_stMsg[m_ubyMessageNumber].szMessageName);
       // BCRetry_DDC(1,2);
         if(ch	==	0)
	     msg->MsgDef.bus       = BUS_A_DDC;		
	  else if(ch	==	1)
	      msg->MsgDef.bus       = BUS_B_DDC;
       // msg->MsgDef.retry_enable = YES_DDC;
	if((DA==31)&&((DSA>0)&&(DSA<31)))
        {          
         //printf("BroadCast--RT %d SA %d MsgNumber %d Ch %d\n",DA,DSA,m_ubyMessageNumber,ch);
          msg->MsgDef.self_test     = NO_DDC;
          msg->MsgDef.mask_bcst_bit = NO_DDC;
          msg->MsgDef.broadcast = YES_DDC;  
          msg->MsgDef.mode_code     = NO_DDC;
          msg->MsgDef.rt_rt         = NO_DDC;
          msg->MsgDef.cmd_1.taddr   = 31;
          msg->MsgDef.cmd_1.t_r     = RECEIVE_DDC;
          msg->MsgDef.cmd_1.status  = 0xcccc;
          msg->MsgDef.loop          = 0x0000;
          msg->MsgDef.cmd_1.subaddr = DSA;
          msg->MsgDef.cmd_1.wcnt    = wc;
        }       
        if((DA==31)&&((DSA==31)||(DSA==0)))
        {
         //printf("ModeCode--RT %d SA %d MsgNumber %d Ch %d\n",DA,DSA,m_ubyMessageNumber,ch);
          msg->MsgDef.mode_code     = YES;
          msg->MsgDef.self_test     = NO_DDC;
          msg->MsgDef.mask_bcst_bit = NO_DDC;
          msg->MsgDef.broadcast     = YES;  
          msg->MsgDef.mode_code     = NO_DDC;
          msg->MsgDef.rt_rt         = NO_DDC;
          msg->MsgDef.cmd_1.taddr   = 31;
          msg->MsgDef.cmd_1.t_r     = RECEIVE_DDC;
          msg->MsgDef.cmd_1.status  = 0xcccc;
          msg->MsgDef.loop          = 0x0000;
          msg->MsgDef.cmd_1.subaddr = DSA;
          msg->MsgDef.cmd_1.wcnt    = wc;
          
            for(int i=0; i< 32; i++)
		  data_buffer[i] = 0xa5a5;
        }     
	if((DA>0)&&(SA>0))
        {
          RtRtFlag=1;
         // printf("RT--->>*****<<----RT\n");
          
          msg->MsgDef.self_test     = NO_DDC;
	  msg->MsgDef.mask_bcst_bit = NO_DDC;
	  msg->MsgDef.broadcast     = NO_DDC;
	  msg->MsgDef.mode_code     = NO_DDC;
	  msg->MsgDef.rt_rt         = YES_DDC;	  
	  msg->MsgDef.cmd_1.status  = 0xcccc;
	  msg->MsgDef.cmd_2.status  = 0xcccc;
	  msg->MsgDef.loop          = 0x0000;	  
          msg->MsgDef.cmd_1.t_r= RECEIVE_DDC;
          msg->MsgDef.cmd_2.t_r= TRANSMIT_DDC;
          msg->MsgDef.cmd_1.taddr = DA;
          msg->MsgDef.cmd_1.subaddr = DSA;
          msg->MsgDef.cmd_2.taddr = SA;
          msg->MsgDef.cmd_2.subaddr = SSA;    
          msg->MsgDef.cmd_1.wcnt    = wc;
          msg->MsgDef.cmd_2.wcnt    = wc;
	  for(int i=0; i< 32; i++)
		  data_buffer[i] = 0xa5a5;	
        }
        if(RtRtFlag!=1)
        {
        
	  if ((DA>0)&&(DA<31))
	  {	  
	    msg->MsgDef.self_test     = NO_DDC;
	    msg->MsgDef.mask_bcst_bit = NO_DDC;
	    msg->MsgDef.broadcast     = NO_DDC;
	    msg->MsgDef.mode_code     = NO_DDC;
	    msg->MsgDef.rt_rt         = NO_DDC;
	    msg->MsgDef.cmd_1.taddr   = DA;
	    msg->MsgDef.cmd_1.t_r     = RECEIVE_DDC;
	    msg->MsgDef.cmd_1.status  = 0xcccc;
	    msg->MsgDef.loop          = 0x0000;
	    msg->MsgDef.cmd_1.subaddr = DSA;
	    msg->MsgDef.cmd_1.wcnt    = wc;

	    for(int i=0; i< 32; i++)
		    data_buffer[i] = 0xa6a8;
	    }
	    if ((SA>0)&&(SA<31))
	    {
	      /*printf("Tx********MinorFrame %d  freq %d Msg %d: %s | Bus %d | RxDest Addr %d Dest SubAddr %d TxSource Addr %d Source SubAddr %d WC %d\n",
                   MF,freq,m_ubyMessageNumber, msg->szMessageName, msg->MsgDef.bus,
                   DA, DSA,SA,SSA, wc);*/
	      msg->MsgDef.self_test     = NO_DDC;
	      msg->MsgDef.mask_bcst_bit = NO_DDC;
	      msg->MsgDef.broadcast     = NO_DDC;
	      msg->MsgDef.mode_code     = NO_DDC;
	      msg->MsgDef.rt_rt         = NO_DDC;
	      msg->MsgDef.cmd_1.taddr   = SA;
	      msg->MsgDef.cmd_1.t_r     = TRANSMIT_DDC;
	      msg->MsgDef.cmd_1.status  = 0xcccc;
	      msg->MsgDef.loop          = 0x0000;
	      msg->MsgDef.cmd_1.subaddr = SSA;
	      msg->MsgDef.cmd_1.wcnt    = wc;

	      for(int i=0; i< 32; i++)
		      data_buffer[i] = 0x1234;
	    }
	  }
	 
	 #if 0
        //msg->MsgDef.Int_Msg_Gap = 10;
        EnableInterMsgGap_DDC(1);
        InterMsgGap_DDC(1,m_ubyMessageNumber+1,100);       
        #endif
       switch (MF)
        {
            case 1:MsgPos1++;  
            for (int i = 0; i < 32; ++i)
             data_buffer[i]=0x1111+i;
            DefMsg_DDC(1,MsgPos1,msg->MsgDef,data_buffer,32);
            //DefMsg_DDC(1,MsgPos1,m_stMsg[MsgPos1-1].MsgDef,data_buffer,wc);  
            break;
            case 2:MsgPos2++;
            for (int i = 0; i < 32; ++i)
             data_buffer[i]=0x2222+i;
            DefMsg_DDC(1,MsgPos2,msg->MsgDef,data_buffer,32);
            break;
            case 3:MsgPos3++; 
            for (int i = 0; i < 32; ++i)
             data_buffer[i]=0x3333+i;
            DefMsg_DDC(1,MsgPos3,msg->MsgDef,data_buffer,32);
            break;
            case 4:MsgPos4++;   
            for (int i = 0; i < 32; ++i)
             data_buffer[i]=0x4444+i;
            DefMsg_DDC(1,MsgPos4,msg->MsgDef,data_buffer,32);
            break;
            case 5:MsgPos5++;  
            for (int i = 0; i < 32; ++i)
             data_buffer[i]=0x5555+i;
            DefMsg_DDC(1,MsgPos5,msg->MsgDef,data_buffer,32);
            break;
            case 6:MsgPos6++;
            for (int i = 0; i < 32; ++i)
             data_buffer[i]=0x6666+i;
            DefMsg_DDC(1,MsgPos6,msg->MsgDef,data_buffer,32);
            break;
            case 7:MsgPos7++;
            for (int i = 0; i < 32; ++i)
             data_buffer[i]=0x7777+i;
            DefMsg_DDC(1,MsgPos7,msg->MsgDef,data_buffer,32);
            break;
            case 8:MsgPos8++;  
            for (int i = 0; i < 32; ++i)
             data_buffer[i]=0x8888+i;
            DefMsg_DDC(1,MsgPos8,msg->MsgDef,data_buffer,32);
            break;
        }     
        switch (freq)
        {
            case 160: m_MinorFrame[MF].frame_items[m_MinorFrame[MF].Mess_Count++] = m_ubyMessageNumber;
            break;
            case 80:  for (int i = 0; i < 2; ++i) m_MinorFrame[(MF + i * 4) % 8].frame_items[m_MinorFrame[(MF + i * 4) % 8].Mess_Count++] = m_ubyMessageNumber;
            break;
            case 40:  for (int i = 0; i < 4; ++i) m_MinorFrame[(MF + i * 2) % 8].frame_items[m_MinorFrame[(MF + i * 2) % 8].Mess_Count++] = m_ubyMessageNumber; 
            break;
            case 20:  for (int i = 0; i < 8; ++i) m_MinorFrame[i].frame_items[m_MinorFrame[i].Mess_Count++] = m_ubyMessageNumber; 
            break;
        }
        m_ubyMessageNumber++;
    }
    fclose(fp);
    printf("Loaded %d messages from file.\n", m_ubyMessageNumber);
}

// ---- EXECUTION ----
void fnHandle1553BCommunication() {

    for (int i = 0; i < 8; ++i) {
        printf("\n[Minor Frame %d Execution]\n", i);
        if (i == 0) 
        fnUpdateTimeBroadcastInfo();
         switch (i)
        {
            case 0:
           
             for(POS1=0;POS1<MsgPos1;POS1++)
             {
                //DefMsg_DDC(1,POS1+1,m_stMsg[POS1].MsgDef,data_buffer,wc);  
                printf("\n[POS1 %d MsgPos1 %d RtAddres %d]\n", POS1,MsgPos1,m_stMsg[POS1].MsgDef.cmd_1.taddr);
	        frame_items[POS1] = POS1+1;     
	     }
	    DefFrame_DDC(1,MsgPos1,frame_items);
	    printf("\nModId %d Frame Completed MsgPos1 %d %d\n", ModId,MsgPos1,POS1);
	    RunBc_DDC(1,STACK_1_DDC,1,MsgPos1,1);
	    printf("\nRunBc Completed \n");
	    usleep(MINORFRAMETIME * 10000);
            MsgPos1=0;
            break;
            #if 1
            case 1:
            //DefMsg_DDC(1,MsgPos2,msg->MsgDef,data_buffer,wc);
            
              for(POS1=0;POS1<=MsgPos2;POS1++)
              {
	        frame_items[POS1] = POS1+1;
	        //DefMsg_DDC(1,MsgPos2,m_stMsg[POS1].MsgDef,data_buffer,wc);
	      }
	    DefFrame_DDC(ModId,MsgPos2,frame_items);
	    RunBc_DDC(1,STACK_1_DDC,1,MsgPos2,1);
	     usleep(MINORFRAMETIME * 10000);
            MsgPos2=0;
            break;
            case 2://DefMsg_DDC(1,MsgPos3,msg->MsgDef,data_buffer,wc);
             for(POS1=0;POS1<=MsgPos3;POS1++)
             {
	      frame_items[POS1] = POS1+1;
	      //DefMsg_DDC(1,MsgPos3,m_stMsg[POS1].MsgDef,data_buffer,wc);
	    }
	    DefFrame_DDC(ModId,MsgPos3,frame_items);
	    RunBc_DDC(1,STACK_1_DDC,1,MsgPos3,1);
	     usleep(MINORFRAMETIME * 10000);
            MsgPos3=0;
            break;
            case 3://DefMsg_DDC(1,MsgPos4,msg->MsgDef,data_buffer,wc);
              for(POS1=0;POS1<=MsgPos4;POS1++)
              {
	        frame_items[POS1] = POS1+1;
	        //DefMsg_DDC(1,MsgPos4,m_stMsg[POS1].MsgDef,data_buffer,wc);
	      }
	    DefFrame_DDC(ModId,MsgPos4,frame_items);
	    RunBc_DDC(1,STACK_1_DDC,1,MsgPos4,1);
	    usleep(MINORFRAMETIME * 10000);
            MsgPos4=0;
            break;
            case 4://DefMsg_DDC(1,MsgPos5,msg->MsgDef,data_buffer,wc);
              for(POS1=0;POS1<=MsgPos5;POS1++)
              {
	        frame_items[POS1] = POS1+1;
	        //DefMsg_DDC(1,MsgPos5,m_stMsg[POS1].MsgDef,data_buffer,wc);
	      }
	    DefFrame_DDC(ModId,MsgPos5,frame_items);
	    RunBc_DDC(1,STACK_1_DDC,1,MsgPos5,1);
	    usleep(MINORFRAMETIME * 10000);
            MsgPos5=0;
            break;
            case 5://DefMsg_DDC(1,MsgPos6,msg->MsgDef,data_buffer,wc);
             for(POS1=0;POS1<=MsgPos6;POS1++)
             {
	      frame_items[POS1] = POS1+1;
	      //DefMsg_DDC(1,MsgPos6,m_stMsg[POS1].MsgDef,data_buffer,wc);
	     }
	    DefFrame_DDC(ModId,MsgPos6,frame_items);
	    RunBc_DDC(1,STACK_1_DDC,1,MsgPos6,1);
	     usleep(MINORFRAMETIME * 10000);
            MsgPos6=0;
            break;
            case 6://DefMsg_DDC(1,MsgPos7,msg->MsgDef,data_buffer,wc); 
             for(POS1=0;POS1<=MsgPos7;POS1++)
             {
	      frame_items[POS1] = POS1+1;
	     // DefMsg_DDC(1,MsgPos7,m_stMsg[POS1].MsgDef,data_buffer,wc);
	     }
	    DefFrame_DDC(ModId,MsgPos7,frame_items);
	    RunBc_DDC(1,STACK_1_DDC,1,MsgPos7,1);
	    usleep(MINORFRAMETIME * 10000);
            MsgPos7=0;
            break;
            case 7://DefMsg_DDC(1,MsgPos8,msg->MsgDef,data_buffer,wc); 
              for(POS1=0;POS1<=MsgPos8;POS1++)
              {
	        frame_items[POS1] = POS1+1;
	        //DefMsg_DDC(1,MsgPos8,m_stMsg[POS1].MsgDef,data_buffer,wc);
	      }
	    DefFrame_DDC(ModId,MsgPos8,frame_items);
	    RunBc_DDC(1,STACK_1_DDC,1,MsgPos8,1);
	    usleep(MINORFRAMETIME * 10000);
            MsgPos8=0;
            break;
            #endif
        } 
        #if 1
        for (int j = 0; j < m_MinorFrame[i].Mess_Count; ++j) 
        {
            int msgId = m_MinorFrame[i].frame_items[j];
           /* for(POS=0;POS<6;POS++)
	       frame_items[POS] = POS+1;*/
            MSG* msg = &m_stMsg[msgId];
            MSG_DEF_DDC* def = &msg->MsgDef;
            /*printf("  Msg %d: %s | Bus %d | Dest Addr %d Dest SubAddr %d Source Addr %d Source SubAddr %d WC %d\n",
                   msgId, msg->szMessageName, def->bus,
                   def->cmd_1.taddr, def->cmd_1.subaddr,def->cmd_2.taddr,def->cmd_2.subaddr, def->cmd_1.wcnt);*/
            //usleep(MINORFRAMETIME * 1000);
            //RunBc_DDC(1,STACK_1_DDC,1,1,1);
        }
        #endif
    }
    printf("\n[Schedule Complete]\n");
}

// ---- THREAD START ----
void* ThreadProc_BC(void* arg) {
    fnHandle1553BCommunication();
    return NULL;
}

// ---- MAIN ----
int main()
{
   
    unsigned char i=0;
    int RetVal=0;
    printf("Initializing MIL-STD-1553B Schedule Simulation (Full C Threaded)\n");
    RetVal = InitCard(CARD1);
    printf("Retval of InitCard is :%d\n",RetVal);
    RetVal = ClearRam_DDC(1);
    printf("Retval of ClearRam is :%d\n",RetVal);
    ResetAce_DDC(1); 
    //printf("Retval of ResetAce is :%d\n",RetVal);
    RetVal = DefMode_DDC(1,BC_DDC);
    printf("Retval of ResetAce is :%x\n",RetVal);
    
     
	 /* bc_page.bus       = BUS_A_DDC;	 	
	  bc_page.self_test     = NO_DDC;
	  bc_page.mask_bcst_bit = NO_DDC;
	  bc_page.broadcast     = NO_DDC;
	  bc_page.mode_code     = NO_DDC;
	  bc_page.rt_rt         = NO_DDC;
	  bc_page.cmd_1.taddr   = 1;
	  bc_page.cmd_1.t_r     = RECEIVE_DDC;
	  bc_page.cmd_1.status  = 0xcccc;
	  bc_page.loop          = 0x0000;
	  bc_page.cmd_1.subaddr = 1;
	  bc_page.cmd_1.wcnt    = 31;

	  for(i=0; i< 32; i++)
		  data_buffer[i] = 0xa5a5+i;
		  
	  DefMsg_DDC(1,1,bc_page,data_buffer,32);
	   printf("Defmsg.\n");
	  frame_items[0] = 1;
	  DefFrame_DDC(1,1,frame_items);
	   printf("DefFrame.\n");
	  RunBc_DDC(1,STACK_1_DDC,1,1,1);
	   printf("RunBc.\n");
    getchar();
    getchar();*/
    //fnExtractMessages("/home/acl/APOORVA_CCPMC/DDC_DLL/FileSchedulerUbuntu/pmc_b1.txt");

    fnExtractMessages("PMC_B1.txt");
   // RunBc(CARD1);    
    pthread_t threadBC;
    if (pthread_create(&threadBC, NULL, ThreadProc_BC, NULL) != 0) {
        perror("Failed to create BC thread");
        return 1;
    }
    pthread_join(threadBC, NULL);
    printf("Simulation thread completed.\n");
    return 0;
}
