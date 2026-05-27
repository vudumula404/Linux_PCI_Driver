#include <stdio.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"


/*******************************************************************
*  This Program gives code for Memory and Register Test 
********************************************************************/
int main()
{       
	int			i,MemAdr=0,RegAdr=0,RetVal=0x0,Times=0x1;
	unsigned short		MemBuff,MsgNo=0,Data=0;
	unsigned char		CardId,FailCount=0,ModId=0,MTModId=3,Com_Or_MemTest=0;
	char temp;
       
	i = InitCard(1);
	if(i)
	  printf("Card Init Success %d \n",i);
	else
	  printf("Card Init Failed %d \n",i);
	   /*******************REGISTER TEST********************/
	printf("\n\t\tREGISTER TEST STARTS FOR:1/2/3\n");
	for(ModId=1;ModId<4;ModId++)
	{
	 /* for(RegAdr=0x0;RegAdr<=0x1f;RegAdr++)
	    RegWrite(ModId,RegAdr,0xa5a5,1);	*/
	  for(RegAdr=0x0;RegAdr<=0x1f;RegAdr++)
	  {
	      MemBuff = RegRead(ModId,RegAdr,1);    
	      printf("ModId %d RegAdr %x MemBuff %x \n",ModId,RegAdr,MemBuff);	      
	  }
	}  
	getchar();
	getchar();
	  /*******************MEMORY TEST STARTS********************/
	      
          printf("\n\t\tMEMORY TEST STARTS FOR:1/2/3\n");
	
	   for(ModId=1;ModId<4;ModId++)
	   {
              for(MemAdr=0x0;MemAdr<MEMORY_SIZE_DDC;MemAdr++)
                MemWrite(ModId,MemAdr,MemAdr,1);	          
              for(MemAdr=0x0;MemAdr<MEMORY_SIZE_DDC;MemAdr++)
              {
                  MemBuff = MemRead(ModId,MemAdr,1);
                  if(MemBuff!=(MemAdr & 0xffff))
                  {
                    printf("MemAdr %x MemBuff %x \n",MemAdr,MemBuff);
                    FailCount=1;
                  }
              }
              getchar();	
              if(FailCount)
              {
                printf("MEMORY TEST :FAIL %d ModId %d\n",FailCount,ModId);
              }
              else
                printf("MEMORY TEST :PASS %d ModId %d\n",FailCount,ModId);  
          }
        
        
	
	return 0;
}  
