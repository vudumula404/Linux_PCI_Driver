#include <windows.h>
#include <stdio.h>
#include <winioctl.h>
#include <conio.h>
#include "C:\Apoorva_General\Include\Libraries.h"

MSG_DEF bc_page;
MSG_RESULT msg_return;
UCHAR Mod=0;

unsigned short frame_items[10];
/*******************************************************************
  This Program gives code for BC Broadcast 
	
********************************************************************/
void main()
{
	int TempCnt,ret=0;
	ULONG i;
	USHORT data_buffer[32]; 
	
	
	InitCard(CARD1);
	SoftwareReset();

	printf("\nEnter Module ID -->>1/2/3:\t");
	scanf("%d",&Mod);

	
	for(i=0;i<0xffff;i++)
	{
		MemWrite(Mod,i,i,CARD1);
		if(MemRead(Mod,i,CARD1)!=i)
		{
			printf("\n Location -->> %x\tData -->> %x",i,MemRead(Mod,i,CARD1));
		}		
	}	
	printf("\n Mem Check ok");	
	getchar();
	CloseDeviceHandle(CARD1);
	getchar();
}  
