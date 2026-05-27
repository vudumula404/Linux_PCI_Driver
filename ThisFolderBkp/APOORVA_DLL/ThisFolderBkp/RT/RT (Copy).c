#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"
RT_MSG_DDC	RtBuff;
/*******************************************************************

********************************************************************/
int main()
{
        int i,RetVal=0,RTModId=0,RtCnt=0;
        unsigned short TempCnt=0,Temp_Buff[32];

	RetVal = InitCard(1);
	if(RetVal < 0)
        {
	        printf("\n\n\t\t!!! ERROR IN ACQUIRING DEVICE HANDLE !!!\n\n\t\tTerminating Application.\n\n");
	        return 0;
        }
        else		
          printf("CCPMC DEVICE OPEN SUCCESS :%d\n",RetVal);
        
        printf("\n\t\tSelect RT ModuleId:1/2/3\n");
        scanf("%d",&RTModId);	         
	 
        RetVal = ClearRam_DDC(RTModId);          
        ResetAce_DDC(RTModId);	  
        //RetVal = DefMode_DDC(RTModId,RT_DDC);
        //printf("RetVal %x\n",RetVal);
	SetRtAddrM_DDC(RTModId,2,1);
	DefLookUp_DDC(RTModId,LOOKUP_1_DDC);
	for(i=0; i < 32; i++)
	  Temp_Buff[i] = 0xfeda;
	WriteDataBlock_DDC(RTModId,1,Temp_Buff);
	//DefMode_DDC(RTModId,RT_DDC);	
	 RetVal = DefMode_DDC(RTModId,RT_DDC);
	RunRt_DDC(RTModId);
	printf("RunBC...Rt Now Running.........Wait..!\n");
	getchar();
	getchar();
	for(RtCnt=1;RtCnt<10;RtCnt++)
	{
          ReadMsg_DDC(RTModId,RtCnt,&RtBuff); 
          printf("MsgNo %d\n",RtCnt);
          printf("message->bsw %x \n",RtBuff.bsw);
	  printf("message->CMD %x \n",RtBuff.cmd);
	  printf("message->wcnt %x \n",RtBuff.wcnt);
	  printf("message->SubAddress %x \n",RtBuff.subaddr);
	  printf("message->T/R %x \n",RtBuff.t_r);
          TempCnt = RtBuff.wcnt;
          if(TempCnt == 0)
           TempCnt = 32;
          for(i =0; i < TempCnt; i++)
          {
	          if(i%8==0)
	          {
	           puts("\n");
	           puts("\t");
	          }
	          printf("%0.4X ",RtBuff.data[i]);
          }       
        }
	getchar();
	printf("\nStopped RT");	
	return 0;		
} //close for main



