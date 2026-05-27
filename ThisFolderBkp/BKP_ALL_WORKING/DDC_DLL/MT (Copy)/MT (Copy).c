#include <stdio.h>
#include <stdint.h>
#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/PciAce.h"

#define MSG_COUNT 512
#define MT_CMD_STACK_PTR_OFFSET 0x02
#define MT_CMD_STACK_PTR_MASK 0x01FF

extern volatile uint16_t* BaseDDC[];

MSG_DEF_DDC bc_page;
RT_MSG_DDC RtBuff;
MSG_RESULT_DDC msg_return;
unsigned short frame_items[10], data_buffer[40], Temp_Buff[40];
// Read memory-mapped stack pointer (hardware dependent)
unsigned short ReadCommandStackPointer(UCHAR ModId) {
    return MemRead(ModId,0x404,1);
}
void ReadMonPacket1553_DDC(UCHAR ModId,unsigned short DespNo,MNTPACK_DDC *MontDataBuff)
{

	unsigned short Offset,i,ptr;
	unsigned short temp;
	
	Offset 				= 	( 0x400 + ((DespNo * 4) - 4));
	MontDataBuff->BlockSts.value	=	MemRead(ModId,Offset,1);
	MontDataBuff->TimeTag 		=	MemRead(ModId,Offset+1,1);
	MontDataBuff->DataBlkPtr 	=	MemRead(ModId,Offset+2,1);
	ptr 				= 	MontDataBuff->DataBlkPtr;
	MontDataBuff->RecCmdWrd.value	=	MemRead(ModId,Offset+3,1);
	temp				=	MontDataBuff->RecCmdWrd.value;
	temp				=	MontDataBuff->RecCmdWrd.bitwise.WrdCntOrMde;
	if(temp==0) temp=32;

	if( MontDataBuff->RecCmdWrd.bitwise.TrnRecv == 1 )
	{
		ptr++;
		if(ptr==0x1000)
		{
			ptr=0x800;
		}
		for(i=0; i < temp;i++)
		{
			MontDataBuff->DataPack[i] =   MemRead(ModId,ptr,1);
			ptr=ptr+1;
			if(ptr==0x1000)
			{
				ptr=0x800;
			}
		}
	}

	else if( MontDataBuff->RecCmdWrd.bitwise.TrnRecv == 0 )
	{
		for(i=0; i < temp;i++)
		{
			MontDataBuff->DataPack[i] =   MemRead(ModId,ptr,1);
			if(ptr==0x1000)
			{
				ptr=0x800;
			}
			else
				ptr= ptr+1;
		}
	}
}

void SelMsgMt(UCHAR ModId) {
    RegWrite(ModId, CONFIG_REG3_DDC, 0x8000, 1);
    RegWrite(ModId, CONFIG_REG3_DDC, 0x8500, 1);
    RegWrite(ModId, CONFIG_REG_DDC,  0x5000, 1);
    MemWrite(ModId, 0x102, 0x400, 1);  // CMD stack pointer
    MemWrite(ModId, 0x103, 0x800, 1);  // Data stack pointer
    for (unsigned short i = 0; i < 128; i++) {
        MemWrite(ModId, 0x280 + i, 0xFFFF, 1);
    }
}
int MtMsgCnt(unsigned char MTModId)
{
        unsigned short CMDDesc; 
        CMDDesc = MemRead(MTModId,(0x102),1);
       // printf("\n*************** MT Packet @ Descriptor %d ***************\n", ((CMDDesc-0x1000)/4));
        return ((CMDDesc-0x1000)/4);
}
int main() {
    MNTPACK_DDC buffer;
    unsigned char MTModId = 0;
    unsigned short currentDesc = 0, lastDesc = 0,Lcount=0;

    if (!InitCard(1)) {
        printf("Card Init Failed\n");
        return -1;
    }
    printf("Card Init Success\n");

    printf("Select MT ModuleId (1/2/3): ");
    scanf("%hhu", &MTModId);

    ClearRam_DDC(MTModId);
    ResetAce_DDC(MTModId);
    DefMode_DDC(MTModId, MT_DDC);
    SelMsgMt(MTModId);
    RunMt_DDC(MTModId);

    printf("Press Enter to display MT results...\n");
    getchar(); getchar();

    lastDesc = MtMsgCnt(MTModId);
    
    unsigned short pos =1;
    while (1) {
        currentDesc = MtMsgCnt(MTModId);          
          
        while(lastDesc != currentDesc) 
        {
            Lcount++;
           // unsigned short pos = (lastDesc + 1 > MSG_COUNT) ? 1 : lastDesc + 1;
            //ReadMonPacket_DDC(MTModId, pos, &buffer);
            ReadMonPacket1553_DDC(MTModId, pos, &buffer);
            printf("\n*************** MT Packet @ Counter %d currentDesc %d***************\n", Lcount,currentDesc);
            pos++;
           
            //printf("\n*************** MT Packet @ Descriptor %d ***************\n", currentDesc);
            if (buffer.BlockSts.bitwise.Eom) 
            {
               
                printf("Block Status Word: 0x%04X\n", buffer.BlockSts.value);
                printf("Cmd Word         : 0x%04X\n", buffer.RecCmdWrd.value);
                printf("Time Tag         : 0x%04X\n", buffer.TimeTag);

                unsigned short words = buffer.RecCmdWrd.bitwise.WrdCntOrMde;
                if (words == 0) words = 32;

                for (int i = 0; i < words; i++) {
                    printf("%04X ", buffer.DataPack[i]);
                    if ((i + 1) % 8 == 0) printf("\n");
                }
                printf("\n");
                if(pos==64)
                  pos=1;
            }
            lastDesc = currentDesc;
        }
    }

    return 0;
}

