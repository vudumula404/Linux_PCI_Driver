
#include <stdio.h>
//#include "/root/Include/PciAce.h"
#include "/DDCapoorva/C/Include.h"

MSG_DEF bc_page;
RT_MSG RtBuff;
MSG_RESULT msg_return;
unsigned short frame_items[10],data_buffer[40],temp_data;
unsigned short Temp_Buff[40];
int ind;

/*******************************************************************
*  This Program gives code for BC and MT 
********************************************************************/
int main()
{

	int i,TempCnt;
	unsigned char CardId;

	ResetCard();

	SelectModule(0,CARD1);
	ClearRam(CARD1);
	ResetAce(CARD1); 
	DefMode(BC,CARD1);

	/* Set up the 1553 messages and descriptor stack */
	/* SET UP MESSAGE BLOCKS */
	/* Broadcast Messege */	
	printf(" enter any key to see run bc\n");
	getchar();
	
	bc_page.bus           = BUS_A;
	bc_page.self_test     = NO;
	bc_page.mask_bcst_bit = NO;
	bc_page.broadcast     = YES;
	bc_page.mode_code     = NO;
	bc_page.rt_rt         = NO;
	bc_page.cmd_1.taddr   = 31;
	bc_page.cmd_1.t_r     = RECEIVE;
	bc_page.cmd_1.status  = 0xcccc;
	bc_page.loop          = 0x0000;
	bc_page.cmd_1.subaddr = 1;
	bc_page.cmd_1.wcnt    = 32;

	for(i=0; i< 32; i++)
	data_buffer[i] = 0x5aa5;
		
	DefMsg(1,bc_page,data_buffer,32,CARD1);
	
	/* Modecode Message (Transmit StatusWord) */
	frame_items[0] = 1;
	DefFrame(1,frame_items,CARD1);
	printf("Before RUNBC\n");

	RunBc(STACK_1,1,1,1,CARD1);/*issue start Command*/

	getchar();
	getchar();
	getchar();

	printf("After RUNBC\n");
	/*print Bc results */

	printf("\nFirst Message Result from BC Module");
	ReadMsgResult(STACK_1,1,&msg_return,CARD1);

	printf("\n COMMAND WORD IS %x \n",msg_return.cmd_1);
	printf("THE  MESSAGE WORD COUNT = %x \n",msg_return.cmd1_bits.wcnt);
	printf("THE  MESSAGE SUBADDRESS = %x \n",msg_return.cmd1_bits.subaddr);
	printf("THE T/R BIT OF MSG#1 IS SET TO %x \n",msg_return.cmd1_bits.t_r);
	printf("THE  MESSAGE TADDRESS IS= %x \n",msg_return.cmd1_bits.taddr);
	printf("\n THE STATUS BITS OF THE MESSAGE ARE AS FOLLLOWS \n");
	printf("THE BLOCK STATUS WORD IS %x \n",msg_return.bsw);
	printf("\nLOOP TEST FAIL     = %x\n",msg_return.bsw_bits.loop_tstfail);
	printf("RESP TIME OUT      = %x \n",msg_return.bsw_bits.resp_timeout);
	printf("MESSAGE FORMAT     = %x \n",msg_return.bsw_bits.msg_fmt_err);
	printf("STATUS WORD ERROR  = %x \n",msg_return.bsw_bits.stat_wrd_err);
	printf("ERROR FLAG         = %x \n",msg_return.bsw_bits.err_flag);
	printf("BUS                = %x \n",msg_return.bsw_bits.bus);
	printf("START OF MESSAGE   = %x \n",msg_return.bsw_bits.som);
	printf("END OF MESSAGE     = %x \n",msg_return.bsw_bits.eom);
	printf("\nDATA RECEIVED \n");
	TempCnt= msg_return.cmd1_bits.wcnt;
	
	if(TempCnt == 0)
	TempCnt = 32;
	for(i=0;i<TempCnt;i++)
	printf("%x ",msg_return.data[i]);
	
	printf("LOOPBACK WORD      = %x \n",msg_return.loop_word);
	printf("STATUS             = %x \n",msg_return.status_1);
    	CloseDeviceHandle();
	getchar();

	return 0;

}  
