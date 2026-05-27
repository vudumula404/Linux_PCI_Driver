/*++

	(C) 2004, Apollo Computing Laboratories (P) Ltd.  All rights reserved.

	NO PART  OF  THIS  PROGRAM OR PUBLICATION MAY BE REPRODUCED, TRANSMITTED,
	TRANSCRIBED, STORED IN A RETRIEVAL  SYSTEM,  OR  TRANSLATED INTO ANY
	LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
	MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE,
	WITHOUT THE PRIOR WRITTEN PERMISSION OF :
                                                  
		APOLLO COMPUTING LABORATORIES (P) LTD.
                                                  
		12-13-197/403-405,Pavani Anasuya Tower,
                                                   
		Tarnaka, Hyderabad - 500 017. INDIA.   
                                                   
		E-mail : mktg@apollocomputing.com  

Module Name:

    Apoorva31Rts_4NodeDll.cpp

Abstract:

	Apoorva31Rts_4NodeDll.cpp : Defines the entry point for the DLL application.

	Dynamic Link Library to control the all APOORVA_1553B  cards. This version contains
	Library Routines for the configuration of BUS CONTROLLER, REMOTE TERMINALS, MONITOR TERMINAL and
	INTERRUPTS.

	Using this DLL, APOORVA Card can be configured as BC, RTs AND MT simultaneously.
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
//#include <curses.h>
#include <errno.h>
#include <math.h>

#include "ApoorvaIoctl.h"

//#include "/home/brahmos/Apoorva31Rts-4NodeDll/Include/Libraries.h"
//#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/ace.h"
//#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/Libraries.h"
#include "/home/acl/APOORVA_CCPMC/APOORVA_DLL/Include/DDC_INCLUDE/PciAce.h"
//#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/mess.h"
//#include "/home/acl/APOORVA_CCPMC/DDC_DLL/Include/DDC_INCLUDE/Pci1553.h"
//#include "//home/acl/APOORVA_CCPMC/Apoorva31Rts-4NodeDll/Include/Libraries.h"

#define USHORT	unsigned short
#define SHORT	unsigned short
#define UCHAR	unsigned char
#define CHAR	char
#define PCHAR	char*
#define ULONG	unsigned long
#define LONG	long
#define UINT	unsigned int
#define TRUE	1
#define FALSE	0
#define BOOL	unsigned short

int		fd,fd2,fd3,fd4;
char		*err_buff[100];
USHORT		StackA,StackB;
ULONG		WrdMonStkPtr=0x3c001;
BOOL		Card1 = FALSE,Card2 = FALSE,Card3 = FALSE,Card4 = FALSE,Card5 = FALSE;
USHORT		CR1BUFF=0x0F80,Int_MsgA[175],Int_MsgB[175];
USHORT		IntrValue1 = 0x0,IntrValue2 = 0x0,IntrValue3 = 0x0,IntrValue4 = 0x0;
ULONG		CircSubRxBase[32],CircSubTxBase[32],MulRtCircSubRxBase[32][32],MulRtCircSubTxBase[32][32];	// Added on 03-12-2k8   
USHORT		Mod2IntrValue1 = 0x0,Mod2IntrValue2 = 0x0,Mod2IntrValue3 = 0x0,Mod2IntrValue4 = 0x0;
USHORT		Mod3IntrValue1 = 0x0,Mod3IntrValue2 = 0x0,Mod3IntrValue3 = 0x0,Mod3IntrValue4 = 0x0;
USHORT		Mod4IntrValue1 = 0x0,Mod4IntrValue2 = 0x0,Mod4IntrValue3 = 0x0,Mod4IntrValue4 = 0x0;
UCHAR		No_Mod_CARD1=0,No_Mod_CARD2=0,No_Mod_CARD3=0,No_Mod_CARD4=0;
BOOL		Card1_Type = FALSE,Card2_Type = FALSE,Card3_Type = FALSE,Card4_Type = FALSE;
unsigned short		enhanced_lookup[32][3][2];

int InitCard(UCHAR CardId)
{
    if(CardId == 1)
    {
	fd = open("/dev/APOORVA1553",O_RDWR);
	
	if(fd<0)
	{
		printf("Cannot open APOORVA1553 device\n");
		return FALSE;
	}
	else
	{
		Card1 = TRUE;
		return TRUE;	
	}
    }   	
    else
	return FALSE;
}

void CloseDeviceHandle()
{	
      close(fd);	
}

CHAR SoftwareReset(UCHAR CardId)
{
	int conf;
	if(CardId == 1)
	    conf = ioctl(fd,IOCTL_CARD1_SOFTWARE_RESET,NULL);	
	else
	    return FALSE;
	if(conf<0)
	{
	  printf("Software Reset failure\n");
	  return FALSE;
	}
	else
	    return TRUE;
}
USHORT MemRead(UCHAR Bus, ULONG len, UCHAR CardId)
{
    USHORT us = 0;  // Variable to store read data
    BOOL CardFlag = FALSE;
    Card1 = 1;

    if (CardId < 1 || CardId > 4)
        return FALSE;

    if (CardId == 1) CardFlag = Card1;
    else if (CardId == 2) CardFlag = Card2;
    else if (CardId == 3) CardFlag = Card3;
    else if (CardId == 4) CardFlag = Card4;

    if (CardFlag == TRUE)
    {
        if (Bus < 1 || Bus > 4)
            return 2;

        if (CardId == 1)
        {
            if ((len >= 0) && (len <= 0x1FFFF)) // ✅ Check valid address range
            {
                struct {
                    ULONG addr;
                    USHORT value;
                } ioctl_data;

                ioctl_data.addr = len;  // ✅ Assign address
                ioctl_data.value = 0;   // ✅ Initialize value

                int ret = -1;

                if (Bus == 1)
                    ret = ioctl(fd, IOCTL_CARD1_MEM_READ_MODULE1, &ioctl_data);
                else if (Bus == 2)
                    ret = ioctl(fd, IOCTL_CARD1_MEM_READ_MODULE2, &ioctl_data);
                else if (Bus == 3)
                    ret = ioctl(fd, IOCTL_CARD1_MEM_READ_MODULE3, &ioctl_data);
                else if (Bus == 4)
                    ret = ioctl(fd, IOCTL_CARD1_MEM_READ_MODULE4, &ioctl_data);

                if (ret < 0)
                {
                 //   perror("MemRead: ioctl failed");
                    printf("MemRead: Read Data = 0x%X from Address = 0x%lX\n",
                        ioctl_data.value, ioctl_data.addr);
                    return 0;
                }

                /*printf("MemRead: Read Data = 0x%X from Address = 0x%lX\n",
                        ioctl_data.value, ioctl_data.addr);*/

                return ioctl_data.addr;  // ✅ Correctly return read value
            }
            else
                return 3;
        }
    }
    return 0;  // Default return value
}
USHORT MemWrite(UCHAR Bus,ULONG len, USHORT val,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	Addr	ad;
        Card1=1;
	ad.Data	=	(val & 0xFFFF);
	ad.Addr	=	(UINT)len;
       // printf("MemData:%x %x\n",ad.Data,ad.Addr);
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
	{
	      //printf("Card Fialed:\n");
		return 4;
	}
	else
	{
	      if(CardId ==1)
		CardFlag = Card1;
	}
	if(CardFlag == TRUE)
	{
	       // printf("TrueData:%x %x\n",ad.Data,ad.Addr);
		if(Bus < 1 || Bus > 4)
			return (5);
	
		if(CardId==1)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
				{
					//printf("B1WriteMemData:%x %x\n",ad.Data,ad.Addr);
					ioctl(fd,IOCTL_CARD1_MEM_WRITE_MODULE1,(unsigned int *)&ad);
				}
				else if(Bus == 2)
				{
					ioctl(fd,IOCTL_CARD1_MEM_WRITE_MODULE2,(unsigned int *)&ad);
				}
				else if(Bus == 3)
				{
					ioctl(fd,IOCTL_CARD1_MEM_WRITE_MODULE3,(unsigned int *)&ad);
				}
				else if(Bus == 4)
				{
					ioctl(fd,IOCTL_CARD1_MEM_WRITE_MODULE4,(unsigned int *)&ad);
				}
			}
			else
				return 6;
		}		
		return TRUE;
	}
	else
		return FALSE;
}
USHORT RegRead(UCHAR Bus, ULONG len, UCHAR CardId)
{
    USHORT us = 0;  // Variable to store read data
    BOOL CardFlag = FALSE;
    Card1 = 1;

    if (CardId < 1 || CardId > 4)
        return 7;

    if (CardId == 1) CardFlag = Card1;
    else if (CardId == 2) CardFlag = Card2;
    else if (CardId == 3) CardFlag = Card3;
    else if (CardId == 4) CardFlag = Card4;

    if (CardFlag == TRUE)
    {
        if (Bus < 1 || Bus > 4)
            return 8;

        if (CardId == 1)
        {
            if ((len >= 0) && (len <= 0x1FFFF)) // ✅ Check valid address range
            {
                struct {
                    ULONG addr;
                    USHORT value;
                } ioctl_data;

                ioctl_data.addr = len;  // ✅ Assign address
                ioctl_data.value = 0;   // ✅ Initialize value

                int ret = -1;

                if (Bus == 1)
                    ret = ioctl(fd, IOCTL_CARD1_REG_READ_MODULE1, &ioctl_data);
                else if (Bus == 2)
                    ret = ioctl(fd, IOCTL_CARD1_REG_READ_MODULE2, &ioctl_data);
                else if (Bus == 3)
                    ret = ioctl(fd, IOCTL_CARD1_REG_READ_MODULE3, &ioctl_data);
                else if (Bus == 4)
                    ret = ioctl(fd, IOCTL_CARD1_REG_READ_MODULE4, &ioctl_data);

                if (ret < 0)
                {
                    //perror("RegRead: ioctl failed");
                    printf("RegRead:ret %x ioctl failed ioctl_data.addr %x ioctl_data.value %x",ret,ioctl_data.addr,ioctl_data.value);
                    return 0;
                }

                /*printf("RegRead: Read Data = 0x%X from Address = 0x%lX\n",
                        ioctl_data.value, ioctl_data.addr);*/

                return ioctl_data.addr;  // ✅ Correctly return read value
            }
            else
                return 9;
        }
    }
    return 0;  // Default return value
}
/*USHORT RegRead(UCHAR Bus,ULONG len,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;

	if(CardId !=1)
	    return FALSE;
	else
	{
          if(CardId ==1)
	      CardFlag = Card1;		
	}
	if(CardFlag == TRUE)
	{	
		USHORT us;
		if(Bus < 1 || Bus > 4)
			return (2);
		
		if(CardId==1)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
					us = ioctl(fd,IOCTL_CARD1_REG_READ_MODULE1,(unsigned int)len);
				else if(Bus == 2)
					us = ioctl(fd,IOCTL_CARD1_REG_READ_MODULE2,(unsigned int)len);
				else if(Bus == 3)
					us = ioctl(fd,IOCTL_CARD1_REG_READ_MODULE3,(unsigned int)len);
				else if(Bus == 4)
					us = ioctl(fd,IOCTL_CARD1_REG_READ_MODULE4,(unsigned int)len);
			}
			else
				return 5;
		}	
		return us;
	}
	else
		return FALSE;
}*/


USHORT RegWrite(UCHAR Bus,ULONG len,USHORT val,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	Addr	ad;

	ad.Data	=	(val);
	ad.Addr	=	len;


	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return 10;
	else 
	{
	  if(CardId ==1)
	      CardFlag = Card1;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (11);
		if(CardId==1)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
				{
					ioctl(fd,IOCTL_CARD1_REG_WRITE_MODULE1,(unsigned int *)&ad);
				}
				else if(Bus == 2)
				{
					ioctl(fd,IOCTL_CARD1_REG_WRITE_MODULE2,(unsigned int *)&ad);
				}
				else if(Bus == 3)
				{
					ioctl(fd,IOCTL_CARD1_REG_WRITE_MODULE3,(unsigned int *)&ad);
				}
				else if(Bus == 4)
				{
					ioctl(fd,IOCTL_CARD1_REG_WRITE_MODULE4,(unsigned int *)&ad);
				}
			}
			else 
				return 12;
		}		
		return TRUE;
	}
	else
		return FALSE;
}
int WriteMemW(UCHAR ModId,unsigned short Data,unsigned int Offset)
{
	Addr	da;

       
        da.Data	=	Data;
        da.Addr	=	Offset;        
        return ioctl(fd,IOCTL_CARD1_WRITE_PORT,(unsigned int *)&da);
	
	/*if(Offset <= (IO_MEM_SIZE-0x2))
	{
		da.Data	=	Data;
		da.Addr	=	Offset;
		return ioctl(fd,IOCTL_WRITE_MEM_W,(unsigned int)&da);
	}
	else
		return -1;*/
}
int WriteMemA(UCHAR ModId,unsigned short Data,unsigned int Offset)
{
	Addr	da;

       
        da.Data	=	Data;
        da.Addr	=	Offset;        
        return ioctl(fd,IOCTL_CARD1_WRITE_PORT_A,(unsigned int *)&da);
	
	/*if(Offset <= (IO_MEM_SIZE-0x2))
	{
		da.Data	=	Data;
		da.Addr	=	Offset;
		return ioctl(fd,IOCTL_WRITE_MEM_W,(unsigned int)&da);
	}
	else
		return -1;*/
}
////////////////////////////////  DDC---------------Functions	///////////////
/*unsigned short RegRead(UCHAR ModId,unsigned short offset)
{
	unsigned short us;
  
	us = ioctl(fd,IOCTL_REG_READ_MODULE,(unsigned int)offset);
	return us;
}

void RegWrite(UCHAR ModId,unsigned short  offset,unsigned short  DataValue)
{
	CCSC_DATA_ADDR ad;
	ad.Data	=	(DataValue);
	ad.Addr	=	(unsigned int)offset;

	ioctl(fd,IOCTL_REG_WRITE_MODULE,(unsigned int *)&ad);
}*/

unsigned int ClearRam(UCHAR ModId)
{
   unsigned int i,j;

	for (i=0 ;i < MEMORY_SIZE; i++)
	{
		MemWrite(ModId,i,0x0,1);
	}

	if((j=MemRead(ModId,i,1))==0)
		return 1;
	else
		return 0;
}

void ResetAce(UCHAR ModId)	
{
      StartReset(ModId,RESET);
}

void StartReset(UCHAR ModId,unsigned short  command)
{
	
	if (command == 0x0002 || command== 0x0001)
	{
    		RegWrite(ModId,START_REG,command,1);
	}
}

unsigned short DefMode(UCHAR ModId, unsigned short mode)
 {
	unsigned short Data;
	
	switch(mode)
	{
	  case 0x044f:
  	  RegWrite(ModId,CONFIG_REG,mode,1);  
  	  MemWrite(ModId,0X100,0X1000,1);//CmdSatck
          MemWrite(ModId,0X1003,0X4000,1);//DataStack
	  break;
	  case 0x8f8f:
	  RegWrite(ModId,CONFIG_REG,mode,1); //RT 
	  break;
	  case 0x4f4f:
	  RegWrite(ModId,CONFIG_REG,mode,1); //MT 
	  break;
	  case 0x0000:
          RegWrite(ModId,CONFIG_REG,0x0000,1);  
          break;
	}
	Data	=	RegRead(ModId,CONFIG_REG,1);
	return (Data);
}
/*
////////////////////////////////RUN BC INTRRUPT///////////////////////////

short RunBcInt(unsigned short frame,unsigned short position,unsigned short num_of_messages,unsigned short times)
{
        int i,msg_addr,temp,init_pos,error=0;
        position = frame +(position-1)*4;
        init_pos=position;

	    for (i=position;i<num_of_messages;i++)
	       {
		     msg_addr = position +3 ;
		     if (msg_addr == 0x00FF)
			 error= -25;
	       }
	
             if (error == 0)
	           
               {
		        for (i=0;i<times;i++)
		          
                    {
			          MemWrite(STACKA_PTR,init_pos);
			          temp = 0xffff - num_of_messages;
			          MemWrite(MESS_COUNTER,temp);
			          MemWrite(TMP_CNTR,temp);     
			          MemWrite(TMP_PTR,init_pos);  
			          StartReset(ModId,START);

			      temp=0;
		           }
	
                }
	         return(error);
               }


*/
  //////////////////////////CODE THAT READS STATUS WORD BITS///////////////


BSW_BITS ReadBswBits(UCHAR ModId,unsigned short blk_statwrd)
{
	 BSW_BITS info;
	 info.loop_tstfail  = 0x01 & blk_statwrd>>8 ;
	 info.resp_timeout  = 0x01 & blk_statwrd>>9 ;
	 info.msg_fmt_err   = 0x01 & blk_statwrd>>10 ;
	 info.stat_wrd_err  = 0x01 & blk_statwrd>>11 ;
	 info.err_flag      = 0x01 & blk_statwrd>>12 ;
	 info.bus           = 0x01 & blk_statwrd>>13 ;
	 info.som           = 0x01 & blk_statwrd>>14;
	 info.eom           = 0x01 & blk_statwrd>>15;
	 return(info);
}

 ///////////////////////CODE THAT READS STATUS BITS////////////////////////

STATUS_BITS ReadStatusBits(UCHAR ModId,unsigned short status)
{
         STATUS_BITS info;
 	     info.term_flag     = status & 0x01;
	     info.db_accept     = status>>1 & 0x01;
	     info.subsys_flag   = status>>2 & 0x01;
	     info.busy          = status>>3 & 0x01;
	     info.bcst_recv     = status>>4 & 0x01;
	     info.svc_request   = status>>8 & 0x01;
	     info.instrument    = status>>9 & 0x01;
	     info.msg_error     = status>>10 & 0x01;
	     info.rt_addr       = status>>11 & 0x1f;

	return(info);
}

/////////////MESSAGE DEFINITION THAT RETURNS CONTROL FIELDS/////////////////

MSG_DEF ReturnCntrlFields(UCHAR ModId,unsigned short control)
{
     MSG_DEF info;
	 info.rt_rt = control & 0x01 ;
	 info.broadcast =  control>>1 & 0x01;
	 info.mode_code =  control>>2 & 0x01;
	 info.mask_bcst_bit = control>>5 & 0x01;
	 info.bus = control>>7 & 0x01;
	 
	 return(info);
}

//////////////// COMMAND THAT RETURNS COMMAND FIELDS///////////////////////


CMD ReturnCmdFields(UCHAR ModId,unsigned short command)
{
    CMD data;
    data.wcnt	 =  0x001f & command;
    data.subaddr = 0x001f & command>>5;
    data.t_r 	 = 0x0001 & command>>10;
    data.taddr 	 = 0x001f & command>>11;
    return(data);
}

///////////////CODE TO LOOK UP THE TABLE///////////////////////////////

short DefLookUp(UCHAR ModId,unsigned short look_up_num)
{
      int error=0;
      if (look_up_num == LOOKUP_1 || look_up_num == LOOKUP_2)
	
           {
		   unsigned int i,temp,uplimit;
		   uplimit=64;
		   temp=RegRead(ModId,CONFIG_REG2,1);

     	if ( (temp & 2) != 0)
			uplimit=96;
		 for (i=0;i<uplimit;i++)
			MemWrite(ModId,look_up_num + i,0x0400 + (i * 32),1);
	       }
	else
		
        error = -33;
	    return (error);
}

/////////////////////////DEF FEATURES///////////////////////////////////////

void DefFeatures(UCHAR ModId,CFG2 options)
{
	
	int cfg_reg2; 
	if(options.b_disable)
	{
		cfg_reg2=RegRead(ModId,CONFIG_REG2,1);
		RegWrite(ModId,CONFIG_REG2,(cfg_reg2 | 1024),1);
	}
	if(options.ttag_res)
	{
		if(options.ttag_res == 64)
		
          {
			cfg_reg2=RegRead(ModId,CONFIG_REG2,1); 
			RegWrite(ModId,CONFIG_REG2,(cfg_reg2 & 0xfc7f),1);
		  }
		if(options.ttag_res == 32 )
		
          {
			cfg_reg2=RegRead(ModId,CONFIG_REG2,1); 
			cfg_reg2 =(cfg_reg2 & 0xfcff) | 128 ;
			RegWrite(ModId,CONFIG_REG2,cfg_reg2,1);
		  }
		
         if(options.ttag_res == 16)
		    
          {
			cfg_reg2=RegRead(ModId,CONFIG_REG2,1); 
			cfg_reg2 = (cfg_reg2 & 0xfd7f) | 256 ;
			RegWrite(ModId,CONFIG_REG2,cfg_reg2,1);
		  }
		
         if(options.ttag_res == 8)
		
         {
			cfg_reg2=RegRead(ModId,CONFIG_REG2,1); 
			cfg_reg2 = (cfg_reg2 & 0xfdff) | 384 ;
			RegWrite(ModId,CONFIG_REG2,cfg_reg2,1);
		 }
		

         if (options.ttag_res == 4 )
		
             {
			 cfg_reg2=RegRead(ModId,CONFIG_REG2,1); 
			 cfg_reg2= (cfg_reg2 & 0xfe7f) | 512;
			 RegWrite(ModId,CONFIG_REG2,cfg_reg2,1);
		     }
		
        if(options.ttag_res == 2 )
		
             {
			 cfg_reg2=RegRead(ModId,CONFIG_REG2,1); 
			 cfg_reg2= (cfg_reg2 & 0xfeff) | 640 ;
			 RegWrite(ModId,CONFIG_REG2,cfg_reg2,1);
		     }
      
		if(options.ttag_res == TST_MODE)
		
             {
			 cfg_reg2=RegRead(ModId,CONFIG_REG2,1); 
			 cfg_reg2= (cfg_reg2 & 0xf7ff) | 768 ;
			 RegWrite(ModId,CONFIG_REG2,cfg_reg2,1);
		     }

		if(options.ttag_res == EXT_CLK)
		     
             {
			 cfg_reg2=RegRead(ModId,CONFIG_REG2,1); 
			 cfg_reg2 = (cfg_reg2 | 896);
			 RegWrite(ModId,CONFIG_REG2,cfg_reg2,1);
		     }

	}

	   if(options.clr_ttag_on_sync)
	        
             {
		     cfg_reg2=RegRead(ModId,CONFIG_REG2,1); 
		     RegWrite(ModId,CONFIG_REG2,(cfg_reg2 | 64),1);
	         }
	
       if(options.load_ttag_on_sync)
	       
             {
		     cfg_reg2=RegRead(ModId,CONFIG_REG2,1); 
		     RegWrite(ModId,CONFIG_REG2,(cfg_reg2 | 32),1);
	         }
 
	   if(options.intstat_autoclear)
	
             {
		     cfg_reg2=RegRead(ModId,CONFIG_REG2,1);
		     RegWrite(ModId,CONFIG_REG2,(cfg_reg2 | 16),1);
	         }

	   if(options.lev_pulse_select)
	
             {
		     cfg_reg2=RegRead(ModId,CONFIG_REG2,1);
		     RegWrite(ModId,CONFIG_REG2,(cfg_reg2 | 8),1);
	         }

	   if(options.clr_servreq)
	
             {
		     cfg_reg2=RegRead(ModId,CONFIG_REG2,1);
		     RegWrite(ModId,CONFIG_REG2,(cfg_reg2 | 4),1);
	         }

	   if(options.enhanced_rtmm)
	   
             {
		      cfg_reg2=RegRead(ModId,CONFIG_REG2,1);
		      RegWrite(ModId,CONFIG_REG2,(cfg_reg2 | 2),1);
	         }

	   if(options.sep_bcst_data)
	
             {
		     cfg_reg2=RegRead(ModId,CONFIG_REG2,1);
		     RegWrite(ModId,CONFIG_REG2,(cfg_reg2 | 1),1);
	         }

}

   //////////////////////////ENABLES ENHANCED RT///////////////////

void EnableEnhancedRt(UCHAR ModId)
{
	int temp;
	temp = RegRead(ModId,CONFIG_REG2,1);
	temp = temp | 2;
	RegWrite(ModId,CONFIG_REG2,temp,1);

}

/////////////////THIS IS HOW AN ILLEGAL MESSAGE LOOKS LIKE ///////////////////////


short DefMsgIlegal(UCHAR ModId,unsigned short subaddr,unsigned short wcl,unsigned short wcu,unsigned short bcst,unsigned short tx_recv)
{
	int offset,value,set_bit,wcnt,i,times,error =0;
	unsigned int loop;
	int increment =0;
	if(subaddr <1 || subaddr > 31)
		error = -35;
	if (wcl < 1  || wcl > 32)
		error = -39;
	if (wcu < 1  || wcu > 32)
		error = -39;
	if(bcst < 1 || bcst > 2)
		error = -51;
	if (tx_recv < 1|| tx_recv > 1)
		error = -34;
	if (!error)
	{
		if (bcst==BOTH )
			times=2;
		else
			times=1;
		if (wcl == 32)
			wcl = 0;
		if (wcu == 32)
			wcu = 0;

		for(i=1;i<=times;i++)
		{
			if (times == 2 && i == 1)
				bcst=0;
			else if(times == 2 && i==2)
			{
				bcst=1;
				increment=0;
			}
			if (wcu-wcl >= 0)
			{
				for(loop=wcl;loop<=wcu;loop++)
				{

					wcnt = loop;
					if(wcnt >= 16)
					{
						wcnt=wcnt-16;
						increment =1;
					}
					offset=(0x300 + (bcst * 128) + (tx_recv * 64 ) + (subaddr *2) ) + increment;
					if (loop != ALL)
					{
						value=MemRead(ModId,offset,1);
						set_bit=wcnt*wcnt;
						value = value | set_bit;

					}
					else if (wcu-wcl==31)
						value = ALL;
					MemWrite(ModId,offset,value,1);
				}
			}
		} 
	}
	return (error);
}


///////////////////////THIS IS HOW AN ILLEGAL MODE CODE IS/////////////////////

short DefModeCodeIllegal(UCHAR ModId,int modecode,int bcst)
{
	int error=0;
	int err_flag =1;

	switch(modecode)
	
     {
		case DYNAMIC_BUS :
			DefMsgIlegal(ModId,0,0,0,NBCAST,TRANSMIT);
			DefMsgIlegal(ModId,31,0,0,NBCAST,TRANSMIT);
			err_flag=0;
			break;

		case SYNC :
			DefMsgIlegal(ModId,0,1,1,bcst,TRANSMIT);
			DefMsgIlegal(ModId,31,1,1,bcst,TRANSMIT);
			err_flag=0;
			break;

		case XMIT_STATUS :
			DefMsgIlegal(ModId,0,2,2,NBCAST,TRANSMIT);
			DefMsgIlegal(ModId,31,2,2,NBCAST,TRANSMIT);
			err_flag=0;
			break;

		case INIT_SLFTST :
			DefMsgIlegal(ModId,0,3,3,bcst,TRANSMIT);
			DefMsgIlegal(ModId,31,3,3,bcst,TRANSMIT);
			err_flag=0;
			break;

		case TX_SHUTDOWN :
			DefMsgIlegal(ModId,0,4,4,bcst,TRANSMIT);
			DefMsgIlegal(ModId,31,4,4,bcst,TRANSMIT);
			err_flag=0;
			break;

		case OVRRD_TX_SHUTDOWN :
			DefMsgIlegal(ModId,0,5,5,bcst,TRANSMIT);
			DefMsgIlegal(ModId,31,5,5,bcst,TRANSMIT);
			err_flag=0;
			break;

		case INHIBIT_TFLAG :
			DefMsgIlegal(ModId,0,6,6,bcst,TRANSMIT);
			DefMsgIlegal(ModId,31,6,6,bcst,TRANSMIT);
			err_flag=0;
			break;

		case OVRRD_INHB_TFLAG :
			DefMsgIlegal(ModId,0,7,7,bcst,TRANSMIT);
			DefMsgIlegal(ModId,31,7,7,bcst,TRANSMIT);
			err_flag=0;
			break;
		case RESET_RT :
			DefMsgIlegal(ModId,0,8,8,bcst,TRANSMIT);
			DefMsgIlegal(ModId,31,8,8,bcst,TRANSMIT);
			err_flag=0;
			break;

		case TX_VECTORWRD :
			DefMsgIlegal(ModId,0,16,16,NBCAST,TRANSMIT);
			DefMsgIlegal(ModId,31,16,16,NBCAST,TRANSMIT);
			err_flag=0;
			break;

		case SYNC_WDATA :
			DefMsgIlegal(ModId,0,17,17,bcst,RECEIVE);
			DefMsgIlegal(ModId,31,17,17,bcst,RECEIVE);
			err_flag=0;
			break;

		case TX_LASTCMD :
			DefMsgIlegal(ModId,0,18,18,NBCAST,TRANSMIT);
			DefMsgIlegal(ModId,31,18,18,NBCAST,TRANSMIT);
			err_flag=0;
			break;

		case TX_BITWORD :
			DefMsgIlegal(ModId,0,19,19,NBCAST,TRANSMIT);
			DefMsgIlegal(ModId,31,19,19,NBCAST,TRANSMIT);
			err_flag=0;
			break;

	}
	if(err_flag)
		error = -52;
	return (error);
}

///////////////////DEFINITION OF THE BUFFER SIZE////////////////

/*

short DefBuffSize(int subaddr,int mode,int size)

      {
	  unsigned short offset,value,data,exp,check_bits,error=0;
	  double op;
	  int err_flag=1;

	   offset= (0x01A0 + subaddr);
	   value = MemRead(offset);

	  if(subaddr < 0  || subaddr > 30)
		 error = -35;

	  if(mode == TRANSMIT)
	   {
		   op = 10;
		   check_bits = 0x1c00;
		}
	  else if(mode== RECEIVE)
		 {
		   op = 5;
		   check_bits = 0x00e0;
		 }
	  else if (mode == BCST)
		  {
		   op = 0;
		   check_bits =0x7;
		  }
	   else error= -37;
	 if (!error)
	  {
	 switch(size)
		   {

			 case 0:
					data = value & check_bits;
					data = value - data;
					err_flag=NO;
					break;


		   case 128:
					 data = value & check_bits;
					 data = value - data;
					 exp = (unsigned short)pow(2,op+0);
					 data = data | exp;
					 err_flag=NO;
					 break;

		   case 256:
					data = value & check_bits;
					data = value - data;
					exp = (unsigned short)pow(2,op + 1);
					data = data | exp;
					err_flag=NO;
					break;

		   case 512:
					data = value & check_bits;
					data = value - data;
					exp  = (unsigned short)(pow(2,op + 0) + pow(2,op + 1));
					data = data | exp;
					err_flag=NO;
					break;

		   case 1024:
					data = value & check_bits;
					data = value - data;
					exp  = (unsigned short)pow(2,op + 2);
					data = data | exp ;
					err_flag=NO;
					break;

		   case 2048:
					 data = value & check_bits;
					 data = value - data;
					 exp  = (unsigned short)(pow(2,op + 0) + pow(2,op + 2));
					 data = data  | exp;
					 err_flag=NO;
					 break;

		   case 4096:
					 data = value & check_bits;
					 data = value - data;
					 exp  =(unsigned short)(pow(2,op + 1) + pow(2,op + 2));
					 data = data | exp;
					 err_flag=NO;
					 break;

		   case 8192:
					   data = value | check_bits;
					   err_flag=NO;
					  break;

		 }  // end switch size 

		MemWrite (offset,data);
	} // end if not error 
	  if (err_flag)
		 error = -38;
	return(error);
}

*/

/////////////////DEFNITION OF ERROR IN MINT/////////////////////////

short DefEomInt(UCHAR ModId,int subaddr,int mode)
{
	int value,offset,error=0;
	int err_flag = 1;
	if(subaddr <0 || subaddr > 30)
		error = -35;
	offset = 0x01A0 + subaddr;
	value = MemRead(ModId,offset,1);
	if(!error)
	{
		switch (mode)
		{
			case TRANSMIT:
				value = value | 0x4000;
				err_flag=NO;
				break;
			
			case RECEIVE:
				value = value | 0x0200;
				err_flag=NO;
				break;
			
			case BCST:
				value = value | 0x0010;
				err_flag=NO;
				break;

		}
		MemWrite(ModId,offset,value,1);
	}
	if(err_flag)
		error = -37;
	return(error);
}
////////////////////INITIIALISING THE BUFFER//////////////////////////////

short DefBuffInt(UCHAR ModId,int subaddr,int mode)
{
	int value,offset,error=0;
	int err_flag = 1;
	if(subaddr <0 || subaddr > 30)
		error = -35;
	offset = 0x01A0 + subaddr;
		value = MemRead(ModId,offset,1);
	if(!error)
	{
		switch (mode)
		{
			case TRANSMIT:
				value = value | 0x2000;
				err_flag=NO;
				break;

			case RECEIVE:
				value = value | 0x0100;
				err_flag=NO;
				break;

			case BCST:
				value = value | 0x0008;
				err_flag=NO;
				break;
		}
		MemWrite(ModId,offset,value,1);
	}
	if(err_flag)
		error = -37;
	return (error);
}

//////////////////READS THE ENHANCED MESSAGE////////////////////////////////
#if 1
short ReadEnhancedMsg(UCHAR ModId,unsigned short stack_entry, RT_ENHCD_MSG *message)
{
	 unsigned short i,total,mode,tmp_data;
	 unsigned short offset_upper,offset_lower,count_1,msg;
	 int error=0;
	 total=0;
	 total=0x0420;
	 for(mode=0;mode <=2;mode++)
	
        {
		enhanced_lookup[0][mode][0] = enhanced_lookup[31][mode][0];
		enhanced_lookup[0][mode][1] = enhanced_lookup[31][mode][1];
	    }
	
    if ((stack_entry <=64 || stack_entry == LAST_MSG) && stack_entry >=1)
	
      {
		if(stack_entry == LAST_MSG)
		  
          {
			msg = MemRead(ModId,STACKA_PTR,1);
			message->bsw = MemRead(ModId,STACK_1 + (msg - 4),1);
			message->time_tag = MemRead(ModId,STACK_1 + (msg - 3),1);
			message->address  = MemRead(ModId,STACK_1 + (msg-2),1 );
			message->cmd = MemRead(ModId,STACK_1 + (msg-1),1);
		  }
		
          else if(stack_entry != LAST_MSG)
		
          {
			message->bsw = MemRead(ModId,STACK_1 + ((stack_entry * 4)-4),1);
		message->time_tag = MemRead(ModId,STACK_1 + ((stack_entry * 4)-3),1);
		message->address = MemRead(ModId,STACK_1 + ((stack_entry * 4)-2),1);
		message->cmd = MemRead(ModId,STACK_1 + ((stack_entry * 4)-1),1);
	  }

		    message->wcnt    = message->cmd  & 0x001f;
		    message->subaddr = (message->cmd & 0x03e0) >>5;
		    message->t_r     = (message->cmd & 0x0400) >>10;
		    message->rt      = (message->cmd & 0xf800) >>11;
		   tmp_data = RegRead(ModId,CONFIG_REG2,1);
		
       if((tmp_data & 1) == 0)
       {
         offset_upper = enhanced_lookup[message->subaddr][message->t_r][0];
	 offset_lower = enhanced_lookup[message->subaddr][message->t_r][1];
       }
		
         else if((tmp_data & 1) !=0 && message->rt == 31)
		

           {
		offset_upper = enhanced_lookup[message->subaddr][BCST][0];
		offset_lower = enhanced_lookup[message->subaddr][BCST][1];
	   }

		if(message->address + message->wcnt <= offset_lower)
		
           {
			if (message->wcnt == 0)
				message->wcnt = 32;
			for (i=0;i<message->wcnt;i++)
				message->data[i] = MemRead(ModId,message->address+i,1);
		   }
		
          else if(message->address + message->wcnt > offset_lower)
		   
           {
			count_1 = offset_lower - message->address;
			for(i=0;i<= count_1;i++)
				message->data[i]=MemRead(ModId,message->address + i,1);
		for(i=count_1 + 1;i<=message->wcnt;i++)
		message->data[i] = MemRead(ModId,offset_upper - count_1 + i -1,1);
	   }
	 }
	else
		error = -36;
	return(error);
}
#endif

/////////////////RETURN INT STATUS///////////////////

INTSTAT_REG ReturnIntStatus(UCHAR ModId,unsigned short value)
   {
	INTSTAT_REG result;
	result.master             = value >> 15;
	result.rt_parity          = (value & 0x0080) >> 7;
	result.ttag_rllovr        = (value & 0x0040) >> 6;
	result.rt_cirbuff_rllovr  = (value & 0x0020) >> 5;
	result.rt_sacntrlwrd_eom  = (value & 0x0010) >> 4;
	result.bc_eof             = (value & 0x0008) >> 3;
	result.fmt_err            = (value & 0x0004) >> 2;
	result.status_set         = (value & 0x0002) >> 1;
	result.eom                = (value & 0x0001);
	return(result);
}

///////////////////////WRITES TO DATA BLOCK//////////////////////////////////

short WriteDataBlock(UCHAR ModId,unsigned short subaddr,unsigned short data_buff[])
{
	unsigned short i,error=0;
	unsigned short offset;
	if(subaddr<=31 && subaddr>=1)
	{
		offset=0x160+subaddr;
		offset=MemRead(ModId,offset,1);
		for (i=0;i<32;i++)
			MemWrite(ModId,(offset + i),data_buff[i],1);
	}
	else if (subaddr>31 || subaddr<1)
		error = 31;
	return(error);
}
///////////////////////READS THE DATA BLOCK//////////////////////////////

short ReadDataBlock(UCHAR ModId,unsigned short data_block,unsigned short *data_buff)
{
	int i,error=0;
	if(data_block <=64 && data_block >=1)
	{
		for (i=0;i<32;i++)
	       *(data_buff+i) =  MemRead(ModId,DATA_BLK1 + (((data_block - 1)*32)+i),1);
	}
	else if (data_block >64 || data_block <1)
		error= -32;
	return(error);
}

/////////////////////////SETS THE SYSTEM FLAG//////////////////////////////
void SetSsflag(UCHAR ModId)
{
	unsigned int config;
	config = RegRead(ModId,CONFIG_REG,1);
	config = config & 0xFEFE;
	RegWrite(ModId,CONFIG_REG,config,1);
}

//////////////////////////SETS THE SVC REGISTER////////////////////////////
void SetSvcReq(UCHAR ModId)
{
	unsigned int config;
	config = RegRead(ModId,CONFIG_REG,1);
	config = config & 0xFDFD;
	RegWrite(ModId,CONFIG_REG,config,1);
}
////////////////////////SETS THE BUSY FLAG////////////////////////////////


void SetBusy(UCHAR ModId)
{
	unsigned int config;
	config = RegRead(ModId,CONFIG_REG,1);
	config = config & 0xFBFB;
	RegWrite(ModId,CONFIG_REG,config,1);
}

////////////////////////////////SETS THE DBA//////////////////////////
void SetDba(UCHAR ModId)

{
	unsigned int config;
	config = RegRead(ModId,CONFIG_REG,1);
	config = config & 0xF7F7;
	RegWrite(ModId,CONFIG_REG,config,1);
}


////////////////////////////RESTES THE SYSTEM FLAG////////////////////////

void ResetSsflag(UCHAR ModId)
  {
	unsigned int config;
	config = RegRead(ModId,CONFIG_REG,1);
	config = config | 0x0101;
	RegWrite(ModId,CONFIG_REG,config,1);
  }
///////////////////////////////RESETS THE SERVICE REQUEST///////////////////
void ResetSvcReq(UCHAR ModId)
{
	unsigned int config;
	config = RegRead(ModId,CONFIG_REG,1);
	config = config | 0x0202;
	RegWrite(ModId,CONFIG_REG,config,1);
}
////////////////////////////////////////////RESTES BUSY BIT//////////////////

void ResetBusy(UCHAR ModId)
{
	unsigned int config;
	config = RegRead(ModId,CONFIG_REG,1);
	config = config | 0x0404;
	RegWrite(ModId,CONFIG_REG,config,1);
}
//////////////////////////////////////////////RESETS THE DBA/////////////////
void ResetDba(UCHAR ModId)
{
	unsigned int config;
	config = RegRead(ModId,CONFIG_REG,1);
	config = config | 0x0808;

	RegWrite(ModId,CONFIG_REG,config,1);
}
//////////////////////////////////////////////READS THE CONFIGIURE BIT//////
CONFIG_BITS ReadConfigBits(UCHAR ModId)
{
	unsigned short config;
	CONFIG_BITS info;
	config = RegRead(ModId,CONFIG_REG,1);
	info.subsys_flag  = config;
	info.svc_request  = config>>1;
	info.busy         = config>>2;
	info.db_accept    = config>>3;
	info.stop_on_err  = config>>4;
	info.current_area = config>>5;
	info.mode         = config>>6;
	return(info);
}
/////////////////////////////////////////////////RUNS THE RT//////////
void RunRt(UCHAR ModId)
{
	MemWrite(ModId,STACK_1,0X0000,1);
	MemWrite(ModId,LAST_STK_PTR,0X0000,1);
	MemWrite(ModId,LAST_MSG_READ,0X0000,1);
	MemWrite(ModId,STACKA_PTR,0X0000,1);
}
///////////////////////////CHECKS THE MESSAGE ACTIVITY//////////////////

unsigned short CheckMsgActivity(UCHAR ModId)
{
	unsigned int previous_ptr,current_ptr,flag=0;

	previous_ptr = MemRead(ModId,LAST_STK_PTR,1);
	current_ptr  = MemRead(ModId,STACKA_PTR,1);

	if (previous_ptr != current_ptr)
	{
		MemWrite(ModId,LAST_STK_PTR,current_ptr,1);
		MemWrite(ModId,LAST_MSG_READ,(current_ptr/4),1);
		flag = 1;
	}
	return (flag);
}
//////////////////////////READ MESSAGE///////////////////////////

short ReadMsg(UCHAR ModId,unsigned short stack_entry,RT_MSG *message)
{
	CONFIG_BITS config;
	unsigned short msg,look_up,i,offset,temp;
	unsigned short error=0;
	if ((stack_entry <=64 || stack_entry == LAST_MSG) && stack_entry >=1)
	{
		if(stack_entry == LAST_MSG)
		{
			msg = MemRead(ModId,STACKA_PTR,1);
			message->bsw = MemRead(ModId,STACK_1 + (msg - 4),1 );
			message->cmd = MemRead(ModId,STACK_1 + (msg-1),1);
		}
		else if(stack_entry != LAST_MSG)
		{
			temp = MemRead(ModId,STACK_1 + ((stack_entry * 4) - 4),1);
			message->bsw = MemRead(ModId,STACK_1 + ((stack_entry * 4)-4),1);
			message->cmd = MemRead(ModId,STACK_1 + ((stack_entry * 4)-1),1);
		}
		message->wcnt    = (message->cmd) & 0x1f;
		message->subaddr = (message->cmd>>5) & 0x1f;
		message->t_r     =  (message->cmd>>10) & 0x01;
		config = ReadConfigBits(ModId);
		if (config.current_area == 0)
			look_up= LOOKUP_1;
		else
			look_up = LOOKUP_2;
		if (message->t_r == TRANSMIT)
			look_up = look_up+32;
		offset = MemRead(ModId,look_up + message->subaddr,1);
		temp = message->wcnt;
		if(temp == 0)
			temp = 32;
		for (i=0;i<temp;i++)
			message->data[i] = MemRead(ModId,offset+i,1);
	}
	else
		error = 36;
	return(error);
}
//////////////////////////DEFINITION PF DATA BLOCK////////////////////////

short DefDataBlk(UCHAR ModId,unsigned short t_r,unsigned short subaddr,unsigned short block_num)
{
	unsigned int look_up=LOOKUP_1,val,sub_cntrlwrd;
	int error = 0;
	if (subaddr <1 || subaddr >31)  error = -35;
	if (subaddr >=1 && subaddr <=31 )
	{
		val=block_num * 32  + 0x0400;
		switch(t_r)
		{
			case 0:
				MemWrite(ModId,look_up + subaddr,val,1);
				sub_cntrlwrd = MemRead(ModId,0x01a0 + subaddr,1);
				sub_cntrlwrd = sub_cntrlwrd & 0x00e0;
				sub_cntrlwrd = sub_cntrlwrd >> 5;
				break;
			case 1:
				look_up=look_up+32;
				MemWrite(ModId,look_up + subaddr,val,1);
				sub_cntrlwrd = MemRead(ModId,0x01a0 + subaddr,1);
				sub_cntrlwrd = sub_cntrlwrd & 0x1c00;
				sub_cntrlwrd = sub_cntrlwrd >> 10;
				break;
			case 2:
				look_up=look_up+64;
				MemWrite(ModId,look_up + subaddr,val,1);
				sub_cntrlwrd = MemRead(ModId,0x01a0 + subaddr,1);
				sub_cntrlwrd = sub_cntrlwrd & 0x0007;
				break;
		}
		enhanced_lookup[subaddr][t_r][0]=val;
		switch(sub_cntrlwrd)
		{
			case 0:
				 enhanced_lookup[subaddr][t_r][1]=val + 31;
				 break;
			 case 1:
				 enhanced_lookup[subaddr][t_r][1]=val+127;
				 break;

			 case 2:
				 enhanced_lookup[subaddr][t_r][1]=val+255;
				 break;
			 case 3:
				 enhanced_lookup[subaddr][t_r][1]=val+511;
				 break;
			 case 4:
				 enhanced_lookup[subaddr][t_r][1]=val+1023;
				 break;
			 case 5:
				 enhanced_lookup[subaddr][t_r][1]=val+2047;
				 break;
			 case 6:
				 enhanced_lookup[subaddr][t_r][1]=val+4095;
				 break;
			 case 7:
				 enhanced_lookup[subaddr][t_r][1]=val+8191;
				 break;
		}
	}
	return(error);
}

//////////////////////////////DEFNITION PF BC ENHANCE MODE///////////////////
void DefBcEnhanceMode(UCHAR ModId)
{
	RegWrite(ModId,CONFIG_REG3,0x8000,1);
}
/////////////////////////////ENABLES INTER MSG GAPE/////////////////////////

void EnableInterMsgGap(UCHAR ModId)
{
	unsigned short temp;
	temp = RegRead(ModId,CONFIG_REG,1);
	temp |=  0x0020;
	RegWrite(ModId,CONFIG_REG,temp,1);
}
///////////////////////////////INTER MESSAGE GAP/////////////////////////
void InterMsgGap(UCHAR ModId,unsigned short MsgNo,unsigned short Gap)
{
	unsigned short Offset;
	Offset =  STACK_1 +  (((MsgNo * 4) - 4) + 2);
	MemWrite(ModId,Offset, Gap,1);

}
//////////////////////ENABLES FRAME TO AUTO REPEAT//////////////////////
void EnableFrameAutoRepeat(UCHAR ModId,unsigned char IntTrigger)
{
	WORD temp;
	temp = RegRead(ModId,CONFIG_REG,1);
	if(IntTrigger == 1)
	temp |=  0x0140;
	if(IntTrigger==0)
	temp |= 0x0100;
}
//////////////////////FRAME TIME OF BC///////////////////////////////

void BcFrameTime(UCHAR ModId,unsigned short FrameTime)
{
	RegWrite(ModId,0x000d,FrameTime,1);
}
///////////////////////CODE TO AUTO RETRY////////////////////////////
void AutoRetry(UCHAR ModId,unsigned short MsgNo,unsigned char SinOrDub)
{
	short temp,Offset;
	Offset =  0x140+((MsgNo-1)*64);
	temp = RegRead(ModId,CONFIG_REG,1);
	if(SinOrDub)
	{
		MemWrite(ModId,Offset, MemRead(ModId,Offset,1)|0x0100,1);
	
		temp |= 0x0018; 
	}
	else
		temp |= 0x0010;
}
/////////////////////////DISABLES ILLEGAL LOGIC////////////////////
void DisableIllegLogic(UCHAR ModId)
{
	RegWrite(ModId,CONFIG_REG3,0x8000,1);
	RegWrite(ModId,CONFIG_REG3,0x8080,1);
}
//////////////////CALICULATES PARITY//////////////////////////
unsigned char CalculateParity(UCHAR ModId,unsigned char WithoutParity)
{
	unsigned char ParityMask = 1;
	int 		i;
	int 	Count = 0;
	WithoutParity = (WithoutParity << 1) & MASK_RT_ADDR;	//MASK_RT_ADDR = 0x3E
	for(i = 1; i < 6; i++)
		if((WithoutParity >> i) & ParityMask) 
			Count++;
		if(Count % 2) 
			return(WithoutParity);
		else 
			return(WithoutParity | PARITY_RT_ADDR);
}

////////////////TO SET THE RT ADDRESS/////////////////
/*amar to be enable*/
#if 1
char SetRtAddrM(UCHAR ModId,unsigned short RtAddr,unsigned char CardId)
{
	unsigned short Rt;
	char Temp;
	Rt = RtAddr;
        RtAddr = CalculateParity(ModId,RtAddr);
        Temp = RtAddr;
        RtAddr = (Temp >> 1) | ((Temp << 5) & 0x3f);
	switch(ModId)
	{
		case 1:
		WriteMemW(ModId,RtAddr,0x10);
		break;
		case 2:
		WriteMemW(ModId,RtAddr,0x14);
		break;
		case 3:
		WriteMemW(ModId,RtAddr,0x18);		
		break;		
		default:
		printf("You have Entered Invalid Mod ID\n");
		break;
	}
	return Rt;

}
#endif

//////////////////////DEFINE DOUBLE BUFFER/////////////////////////////////////////////
void DefDouBuff(UCHAR ModId)
{
	RegWrite(ModId,CONFIG_REG3,0x8000,1);
	RegWrite(ModId,CONFIG_REG2,0x1002,1);

}
//////////       DEFINITION OF DATA BLK CB////////////////////////

short DefDataBlkCB(UCHAR ModId,unsigned short t_r,
                    unsigned short subaddr,unsigned short block_num)
{
	unsigned int look_up=LOOKUP_1,val,sub_cntrlwrd;
	int error = 0;
	if (subaddr <1 || subaddr >31)  error = -35;
	if (subaddr >=1 && subaddr <=31 )
	{
		val = block_num * 8192+ 0x0400; // for 32 or 64k memory 
		
		switch(t_r)
		{
			case 0:
				MemWrite(ModId,look_up + subaddr,val,1);
				sub_cntrlwrd = MemRead(ModId,0x01a0 + subaddr,1);
				sub_cntrlwrd = sub_cntrlwrd & 0x00e0;
				sub_cntrlwrd = sub_cntrlwrd >> 5;
				break;
			case 1:
				look_up=look_up+32;
				MemWrite(ModId,look_up + subaddr,val,1);
				sub_cntrlwrd = MemRead(ModId,0x01a0 + subaddr,1);
				sub_cntrlwrd = sub_cntrlwrd & 0x1c00;
				sub_cntrlwrd = sub_cntrlwrd >> 10;
				break;
			case 2:
				look_up=look_up+64;
				MemWrite(ModId,look_up + subaddr,val,1);
				sub_cntrlwrd = MemRead(ModId,0x01a0 + subaddr,1);
				sub_cntrlwrd = sub_cntrlwrd & 0x0007;
				break;
		}
		enhanced_lookup[subaddr][t_r][0]=val;
		switch(sub_cntrlwrd)
		{
			case 0:
				enhanced_lookup[subaddr][t_r][1]=val + 31;
				break;
			case 1:
				enhanced_lookup[subaddr][t_r][1]=val+127;
				break;
			case 2:
				enhanced_lookup[subaddr][t_r][1]=val+255;
				break;
			case 3:
				enhanced_lookup[subaddr][t_r][1]=val+511;
				break;
			case 4:
				enhanced_lookup[subaddr][t_r][1]=val+1023;
				break;
			case 5:
				enhanced_lookup[subaddr][t_r][1]=val+2047;
				break;
			case 6:
				enhanced_lookup[subaddr][t_r][1]=val+4095;
				break;
			case 7:
				enhanced_lookup[subaddr][t_r][1]=val+8191;
				break;
		}
	}
	return(error);
}
////////////////////ENABLE IM GAP/////////////////////////////////
void EnableIMGap(UCHAR ModId)
{
	WORD temp;
	temp = RegRead(ModId,CONFIG_REG,1);
	temp |=  0x0020;
	RegWrite(ModId,CONFIG_REG,temp,1);
}
//////////////////DEFINITION OF DATA BLK DB//////////////////////////////
short DefDataBlkDB(UCHAR ModId,unsigned short t_r,unsigned short subaddr,unsigned short block_num)
{
	unsigned int look_up=LOOKUP_1,val;
	int error = 0;
	t_r = 0; 
	if (subaddr <1 || subaddr >31)  error = -35;
	if (subaddr >=1 && subaddr <=31 )
	{
		val = block_num * 64+ 0x0400;
		MemWrite(ModId,look_up + subaddr,val,1);
		MemWrite(ModId,0x01a0 + subaddr, 0x8000,1);
		enhanced_lookup[subaddr][t_r][0]=val;
		enhanced_lookup[subaddr][t_r][1]=val + 63;
	}
	return(error);
}
/////////////////FUNCTION BC RETRY ///////////////////////////////////

void BCRetry(UCHAR ModId,USHORT sel)
{
	USHORT temp=0,val;	
	if(sel)
	{
		/*RegWrite(ModId,CONFIG_REG3,0x8000,1);
		temp=RegRead(ModId,CONFIG_REG,1);
		RegWrite(ModId,CONFIG_REG,(temp&0xffe7)|(0x0013),1);	*/	
		//(0x458 Double Retry Enale 0x450 for Single retry)
		printf("BCRetry sel %d \n",sel);
		switch(sel)
		{

		case 1://S		       
		        RegWrite(ModId,CONFIG_REG,0x450,1);
			RegWrite(ModId,CONFIG_REG3,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4,1);
			RegWrite(ModId,CONFIG_REG4,val | 0x1200 ,1);
			printf("SameBus(S)\n");
			break;

		case 2://A
		        RegWrite(ModId,CONFIG_REG,0x450/*RegRead(ModId,CONFIG_REG,1) | 0x0*/,1);		       
			RegWrite(ModId,CONFIG_REG3,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4,1);			
			RegWrite(ModId,CONFIG_REG4,val | 0x1300 ,1);
			printf("AlterBus(A)\n");
			break;
		case 3://SS
		        RegWrite(ModId,CONFIG_REG,0x458,1);
		        RegWrite(ModId,CONFIG_REG,RegRead(ModId,CONFIG_REG,1) | 0x8,1);
			RegWrite(ModId,CONFIG_REG3,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4,1);
			RegWrite(ModId,CONFIG_REG4,val | 0x1200 ,1);
			printf("DoubleRetry Same(SS)\n");
			break;
	      	case 4://AA
	      	        RegWrite(ModId,CONFIG_REG,0x458,1);
		        RegWrite(ModId,CONFIG_REG,RegRead(ModId,CONFIG_REG,1) | 0x8,1);
			RegWrite(ModId,CONFIG_REG3,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4,1);
			RegWrite(ModId,CONFIG_REG4,val | 0x1300 ,1);
			printf("DoubleRetry Alter(AA)\n");
			break;
		case 5://SA
		        RegWrite(ModId,CONFIG_REG,0x458,1);
		        RegWrite(ModId,CONFIG_REG,RegRead(ModId,CONFIG_REG,1) | 0x8,1);
			RegWrite(ModId,CONFIG_REG3,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4,1);			
			RegWrite(ModId,CONFIG_REG4,val | 0x1280 ,1);
			printf("Same Alter(SA)\n");
			break;
		case 6://AS
		        RegWrite(ModId,CONFIG_REG,0x458,1);
		        RegWrite(ModId,CONFIG_REG,RegRead(ModId,CONFIG_REG,1) | 0x8,1);
			RegWrite(ModId,CONFIG_REG3,0x8000,1);
			val=RegRead(ModId,CONFIG_REG4,1);
			RegWrite(ModId,CONFIG_REG4,val | 0x1380 ,1);
			printf("Alter Same(AS)\n",RegRead(ModId,CONFIG_REG3,1),RegRead(ModId,CONFIG_REG4,1));
			break;
		}
	}
	else
	{
	}

}
/////////////////FUNCTION BC RETRY ///////////////////////////////////
/*void BCRetry(UCHAR ModId,unsigned short sel)
{
	unsigned short tp=0,mask;
	if(sel)
	{
		RegWrite(ModId,CONFIG_REG3,0x8000,1);
		tp=RegRead(ModId,CONFIG_REG);
		RegWrite(ModId,CONFIG_REG,(tp&0xffe7)|(0x0010|0x3));
		mask=0x180;
	}
	else
		RegWrite(ModId,CONFIG_REG,(tp&0xffe7));
}*/
///////////////////FUNCTION TO AUTO REPEAT BC//////////////////////////
void BCAutoRepeat(UCHAR ModId,unsigned short Sel)
{
	RegWrite(ModId,CONFIG_REG3,0x8000,1);
	if(Sel)
		RegWrite(ModId,CONFIG_REG,RegRead(ModId,CONFIG_REG,1)|0x0140,1);
	else
		RegWrite(ModId,CONFIG_REG,RegRead(ModId,CONFIG_REG,1)&0xfeff,1);
}
/////////////FUNCTION THAT DEFINES BC TIMEOUT////////////////
void BCTimeout(UCHAR ModId,unsigned short Value)
{
	RegWrite(ModId,CONFIG_REG5,(RegRead(ModId,CONFIG_REG5,1)&0xf9ff)|(Value<<9),1);
}
//////////////FUNCTION THAT SETS THE RT BUSY///////////////////
void RTSetBusy(UCHAR ModId,unsigned short sel)
{
	RegWrite(ModId,CONFIG_REG,(RegRead(ModId,CONFIG_REG,1)&0xFBFF)|((!sel)<<10),1);
}
////////////////FUNCTION TO OPEN RTMON/////////////////////
/*void RTMONOpen(UCHAR ModId)
{
	unsigned short i;
	RegWrite(ModId,CONFIG_REG3,0x8000,1);

	RegWrite(ModId,CONFIG_REG,(RegRead(ModId,CONFIG_REG,1)|0x9000),1);

	MemWrite(STACK_1,0X0000);
	MemWrite(STACKA_PTR,0X0000);

	//MemWrite(0x102, 0x1000);
	//MemWrite(0x103, 0x2000);
	MemWrite(0x102, 0x200);
	MemWrite(0x103, 0x400);

	for(i=0; i < 128; i++)
		MemWrite(0x280 + i, 0xffff);

	MsgMonStkPtr = 0x400;
	return;
}*/
/////////////////FUNCTION TO RUN THE RT MON////////////////////////
void RunRTMon(UCHAR ModId)
{
	RegWrite(ModId,START_REG,START,1);
}
/////////////////////FUNCTION TO SELECT THE WORD OF MT/////////////////
void SelWordMt(UCHAR ModId)
{
	unsigned short WrdMonStkPtr1;
	RegWrite(ModId,CONFIG_REG,0x4000,1);
	WrdMonStkPtr1 = 0;
	MemWrite(ModId,STACKA_PTR, WrdMonStkPtr1,1);
	//return;
}
//////////////FUNCTION TO READ THE WORD OF MT//////
short ReadMtWord(UCHAR ModId,MT_WORD_DEF *result)
{
	unsigned short temp;
	unsigned short WrdMonStkPtr1=0;
	temp = MemRead(ModId,WrdMonStkPtr1 + 1,1);
	if (temp & 0x0080)
	{
		result -> tag_word.gap_time = (temp & 0xff00) >> 8;
		result -> tag_word.word_flag = (temp & 0x0080) >> 7;
		result -> tag_word.this_rt = (temp & 0x0040) >> 6;
		result -> tag_word.broadcast = (temp & 0x0020) >> 5;
		result -> tag_word.error = (temp & 0x0010) >> 4;
		result -> tag_word.command_data = (temp & 0x0008) >> 3;
		result -> tag_word.channel_b_a = (temp & 0x0004) >> 2;
		result -> tag_word.contiguous = (temp & 0x0002) >> 1;
		result -> tag_word.mode_code = (temp & 0x0001);
		result -> rx_word = MemRead(ModId,WrdMonStkPtr1,1);

		MemWrite(ModId,WrdMonStkPtr1 + 1, temp &0xff7f,1);

		WrdMonStkPtr = (WrdMonStkPtr1+2);

		return(1);
	}
	else
	{
		return(0);
	}
}
///////////////FUNCTION TO SELECT MESSAGE OF MT//////////////////////////
/*void SelMsgMt(UCHAR ModId)
{
	extern unsigned short MsgMonStkPtr;
	unsigned short	i;

	RegWrite(ModId,CONFIG_REG3,0x8000);
	RegWrite(ModId,CONFIG_REG3,0x9200);
	RegWrite(ModId,CONFIG_REG,0x5000);
	MemWrite (0x102,0x200);				//CMD word Stk Ptr
	MemWrite (0x103,0x400);				//Data word Stk Ptr
	for(i=0; i < 128; i++)
	{	
		MemWrite ((0x280+i),0xffff);
	}
	MsgMonStkPtr = 0x400;
	return;
}*/

///////////////FUNCTION TO SELECT MESSAGE OF MT//////////////////////////
/*void SelMsgMt(UCHAR ModId)
{
	//extern unsigned short MsgMonStkPtr;
	unsigned short	i;
	
	RegWrite(ModId,CONFIG_REG3,0x8000,1);
	RegWrite(ModId,CONFIG_REG3,(RegRead(ModId,CONFIG_REG3,1)|0x1500),1);
	RegWrite(ModId,CONFIG_REG,0x5000,1);
	MemWrite(ModId,0x102,0x400,1);				//CMD word Stk Ptr//0x400
	MemWrite(ModId,0x103,0x800,1);				//Data word Stk Ptr//0x800
	for(i=0; i < 128; i++)
	{
		MemWrite(ModId,(0x280+i),0xffff,1);
	}
	//MsgMonStkPtr = 0x800;
	return;
}*/
void SelMsgMt(UCHAR ModId)
{	
  unsigned short i;	
  RegWrite(ModId,CONFIG_REG3,0x8000,1);
  RegWrite(ModId,CONFIG_REG3,0x8700,1);
 // printf(" CONFIG_REG3 -> %x CONFIG_REG %x\n",RegRead(ModId, 0x07,1),RegRead(ModId, 0x01,1));
  RegWrite(ModId,CONFIG_REG,0x5000,1);
  MemWrite(ModId,0x102,0x1000,1);				
  MemWrite(ModId,0x103,0x4000,1);	
  for(i=0; i < 128; i++)
  {
      MemWrite(ModId,(0x280+i),0xffff,1);
  }	
    return;
}

////////////////FUNCTION TO OPEN RTMON/////////////////////

void RTMONOpen(UCHAR ModId)
{
	unsigned short	i;

	RegWrite(ModId,CONFIG_REG3,0x8000,1);
	RegWrite(ModId,CONFIG_REG3,(RegRead(ModId,CONFIG_REG3,1)|0x0E00),1);
	RegWrite(ModId,CONFIG_REG,(RegRead(ModId,CONFIG_REG,1)|0x9000),1);		//Runs RT and MT At a Time

	MemWrite(ModId,STACK_1,0X0000,1);		//STACK_1		== 0x000
	MemWrite(ModId,STACKA_PTR,0X0000,1);		//STACKA_PTR	== 0x0100

	MemWrite(ModId,0x102, 0x400,1);			//MT Cmd Stk Pointer -A
	MemWrite(ModId,0x103, 0x800,1);			//MT Data Stk Pointer -A

	MemWrite(ModId,0x106, 0x400,1);			//MT Cmd Stk Pointer -B
	MemWrite(ModId,0x107, 0x800,1);			//MT Data Stk Pointer -B

	for(i=0; i < 128; i++)
		MemWrite(ModId,0x280 + i, 0xFFFF,1);

	return;
}


////////////FUNCTION TO RUN THE MT//////////////////////
void RunMt(UCHAR ModId)
{
	RegWrite(ModId,START_REG,START,1);
	return;
}
/////////////FUNCTION TO READ THE MONITOR PACKET/////////////

void ReadMonPacket(UCHAR ModId,unsigned short DespNo,MNTPACK *MontDataBuff)
{
    unsigned short ptr = 0, temp,msgNo=0,pos=0;
    unsigned short cmd = 0, rtAddr = 0, trBit = 0, subAddr = 0, wc_or_mode = 0;
    unsigned short current = 0, last = 0, prev = 0;
    unsigned long msgCount = 0;
    int diff = 0;
    struct timeval tv, prev_tv;
    struct tm* t;
    char dir = 'R'; // default direction
    unsigned short Offset = 0x1000 + ((DespNo * 4) - 4);                
                
                // --- Read Descriptor ---
                MontDataBuff->BlockSts.value   = MemRead(ModId, Offset,     1);
                MontDataBuff->TimeTag          = MemRead(ModId, Offset + 1, 1);
                MontDataBuff->DataBlkPtr       = MemRead(ModId, Offset + 2, 1);
                ptr                     = MontDataBuff->DataBlkPtr;
                MontDataBuff->RecCmdWrd.value  = MemRead(ModId, Offset + 3, 1);  

                
                // ---- Decode Command Word ----
                cmd        = MontDataBuff->RecCmdWrd.value;
                rtAddr     = (cmd >> 11) & 0x1F;
                trBit      = (cmd >> 10) & 0x01;
                subAddr    = (cmd >> 5)  & 0x1F;
                wc_or_mode = cmd & 0x1F;
                if (wc_or_mode == 0) wc_or_mode = 32;

                dir = (trBit == 1) ? 'T' : 'R';

                // ---- RT-to-RT Special Handling ----
                if (MontDataBuff->BlockSts.value & 0x0800)
                {
                    MontDataBuff->TxCmdWrd.value=MemRead(ModId, ptr, 1);
                    unsigned short secondCmd = MemRead(ModId, ptr, 1);
                    MontDataBuff->TxStatus  = MemRead(ModId, ptr + 1, 1);

                    unsigned short rt_tx  = (secondCmd >> 11) & 0x1F;
                    unsigned short sa_tx  = (secondCmd >> 5)  & 0x1F;
                    unsigned short wc_tx  = secondCmd & 0x1F;
                    if (wc_tx == 0) wc_tx = 32;

                  /*  fprintf(logFile,
                            "Decoded: %u-%u-X-%u  2ndCmd=0x%04X TxRT=%u SA=%u TxStatus=0x%04X\n",
                            rtAddr, subAddr, wc_tx, secondCmd, rt_tx, sa_tx, txStatus);*/

                    ptr += 2;      // skip 2 words
                    temp = wc_tx;  // number of data words
                }
                else
                {                  
                   /*if (buffer.BlockSts.value & 0x0200) 
                   {  // If RT no-response bit
                        fprintf(logFile,"Decoded: %u-%u-%c-%u (No Response!)\n",rtAddr, subAddr, dir, wc_or_mode);
                   } 
                   else 
                   {
                      fprintf(logFile, "Decoded: %u-%u-%c-%u\n",rtAddr, subAddr, dir, wc_or_mode);
                   }*/

                    temp = wc_or_mode;
                }

                if (subAddr == 0) temp = 1; // Mode command fix
                
                if (MontDataBuff->RecCmdWrd.bitwise.TrnRecv == 1)
                {
                     ptr++;
                     if ((MontDataBuff->BlockSts.value & 0x0200)==0)
                     {
                        // ---- Read & Log Data ----                       
                        for (int i = 0; i < temp; i++)
                        {
                          MontDataBuff->DataPack[i] = MemRead(ModId, ptr++, 1);                          
                        }                      
                    }
                }
                else
                {                    
                    for (int i = 0; i < temp; i++)
                    {
                      MontDataBuff->DataPack[i] = MemRead(ModId, ptr++, 1);                                            
                    }                               
                }
}
//code of the interrupt routines 
CHAR ConfigICSR(UCHAR CardId)
{
	int conf;
	if(CardId == 1)
		conf = ioctl(fd,IOCTL_CARD1_SET_ICS_REG,NULL);	
	else 
		return FALSE;
	if(conf<0)
	{
		printf("ICSR failure\n");
		return FALSE;
	}
	else
		return TRUE;	
}

void InitInterrupts(unsigned char Intr,unsigned char ModId)
{
    switch(ModId)
    {
        case 1:
          WriteMemA(ModId,Intr,1);  	
          break;
        case 2:
          WriteMemA(ModId,Intr,2);  
          break;
        case 3:
          WriteMemA(ModId,Intr,4);  
          break;
        case 0x0000:
	  WriteMemA(ModId,Intr,1);  
	  break;
    }
    /*IntrValue = Intr;
    ConfigICSR();
    WritePort(0xe0,0x1);
    WritePort(0x40,IntrValue); */  
}
void DefIntMask(UCHAR ModId, INT_DEF mask)
{
	unsigned short value=0;

	value = value | mask.bc_eom<<3;
	value = value | mask.fmterr_statset<<1;
	value = value | mask.eom;
	RegWrite(ModId,INTMASK_REG,value,1);
	//printf("MaskReg :%x, Value :%x",0x0,RegRead(INTMASK_REG));
}


int GetM1Cnt()
{

  int Cnt;

	Cnt = ioctl(fd,IOCTL_GET_M1CNT,NULL);

	if(Cnt<0)
	{
		printf("Invalid M1Cnt\n");
	}
	else
		return Cnt;
}	
void SetM1Cnt()
{
      int CntS;

	CntS = ioctl(fd,IOCTL_SET_M1CNT,NULL);
	if(CntS<0)
	{	
		printf("Invalid Setting\n");
	}
		
}
short ReadMsgResult(UCHAR ModId,unsigned short frame_num,unsigned short frame_item,MSG_RESULT *result)
{
         MSG_DEF	data;
         unsigned int position,cntrl_wrd,msg_addr;
         unsigned int  CmdStack=0x1000;
        unsigned int  DataStack=0x4000;
         unsigned int i,temp;
         int		option,error=0;

        if (frame_num != STACK_1 )
	  error = -26;
       
        if (frame_item >1024 || frame_item <1 )
	  error = -27;

        position = CmdStack+frame_num +((frame_item-1)*4);
        result->bsw  = MemRead(ModId,position,1);
        result->bsw_bits = ReadBswBits(ModId,result->bsw);
        result->time_tag = MemRead(ModId,position+1,1);
        msg_addr = MemRead(ModId,position+3,1);  

        cntrl_wrd = MemRead(ModId,msg_addr,1);
        result->cntrl_word = cntrl_wrd;
        data = ReturnCntrlFields(ModId,cntrl_wrd);
        result->cmd_1 = MemRead(ModId,msg_addr+1,1);
	
        if (data.rt_rt == 0 && data.broadcast == 0 && data.mode_code == 0)
        {
          result->cmd1_bits = ReturnCmdFields(ModId,result->cmd_1);
          if (result->cmd1_bits.t_r == RECEIVE )
             option = 0;
          else if (result->cmd1_bits.t_r == TRANSMIT)
          option = 1;
	          
         }
         else if (data.rt_rt == 1 )
	  option = 2;  
         else if ( data.mode_code == 1 && data.broadcast == 0 )
         {
            result->cmd1_bits = ReturnCmdFields(ModId,result->cmd_1);
            if ( (result->cmd1_bits.wcnt & 0x0010) != 0 ) 
            {
               if (result->cmd1_bits.t_r == RECEIVE)
                option = 3;    
               else if (result->cmd1_bits.t_r == TRANSMIT )
                option = 4;
            }
            else if ((result->cmd1_bits.wcnt & 0x0010) == 0)
            option = 5;      
          }
          if(data.broadcast == 1)
          {
            result->cmd1_bits = ReturnCmdFields(ModId,result->cmd_1);
            option = 6;
            if(data.mode_code == 1)
            {
            if ((result->cmd1_bits.wcnt & 0x0010) == 0)
            option = 7;
            else
            option = 8;
            }
          }
	switch (option)
	{
		case 0 :   
			temp = result->cmd1_bits.wcnt;
			if(temp == 0)
				temp = 32;
			for (i=0;i<temp;i++)
			result->data[i] = MemRead(ModId,msg_addr + 2 +i,1);
			result->loop_word = MemRead(ModId,msg_addr + 1 + temp + 1,1);
			result->status_1 = MemRead(ModId,msg_addr + 1 + temp + 2,1);
			result->stat1_bits = ReadStatusBits(ModId,result->status_1);
			break;
		case 1 :   
			result->loop_word = MemRead(ModId,msg_addr + 2,1);
			result->status_1 = MemRead(ModId,msg_addr + 3,1);
			result->stat1_bits = ReadStatusBits(ModId,result->status_1);
			temp = result->cmd1_bits.wcnt;
			if(temp == 0)
				temp = 32;
			for (i=0;i< temp;i++)
				result->data[i] = MemRead(ModId,msg_addr + 4 + i,1);
			break;
		case 2 :
			result->cmd_1 = MemRead(ModId,msg_addr+1,1);
			result->cmd1_bits = ReturnCmdFields(ModId,result->cmd_1);
			result->cmd_2 = MemRead(ModId,msg_addr + 2,1);
			result->cmd2_bits = ReturnCmdFields(ModId,result->cmd_2);
			result->loop_word = MemRead(ModId,msg_addr + 3,1);
			result->status_1 = MemRead(ModId,msg_addr +  4,1);
			result->stat1_bits = ReadStatusBits(ModId,result->status_1);
			temp = result->cmd1_bits.wcnt;
			if(temp == 0)
				temp = 32;
			for(i=0;i<temp;i++)
				result->data[i] = MemRead(ModId,msg_addr + 5 + i,1);
			result->status_2 = MemRead(ModId,msg_addr + 5 + i,1);
			result->stat2_bits = ReadStatusBits(ModId,result->status_2);
			break;
		case 3 :   
			result->data[0] = MemRead(ModId,msg_addr + 2 ,1);
			result->loop_word = MemRead(ModId,msg_addr + 3,1);
			result->status_1 = MemRead(ModId,msg_addr + 4,1);
			break;
		case 4 :
			result->loop_word = MemRead(ModId,msg_addr + 2,1);
			result->status_1 = MemRead(ModId,msg_addr + 3,1);
			result->stat1_bits = ReadStatusBits(ModId,result->status_1);
			result->data[0] = MemRead(ModId,msg_addr + 4,1);
			break;
		case 5 :

			result->loop_word = MemRead(ModId,msg_addr + 2,1);
			result->status_1 = MemRead(ModId,msg_addr + 3,1);
			result->stat1_bits = ReadStatusBits(ModId,result->status_1);
			break;
		case 6 :

			temp = result->cmd1_bits.wcnt;
			if(temp == 0)
			temp = 32;
			for (i=0;i<temp;i++)
			{
				result->data[i] = MemRead(ModId,msg_addr + 2 +i,1);
			}
			result->loop_word = MemRead(ModId,msg_addr + 1 + temp + 1,1);
			break;
		case 7 :
			result->loop_word = MemRead(ModId,msg_addr + 2,1);
			break;
		case 8 :
			result->data[0] = MemRead(ModId,msg_addr + 2,1);
			result->loop_word = MemRead(ModId,msg_addr + 3,1);
			break;

	}  
	return (error);
}
short DefMsg(UCHAR ModId,unsigned short msg_num,MSG_DEF def,unsigned short data_buffer[],unsigned short num_of_data_words,unsigned short IntMsgGap)
{
  unsigned int cmd=0;
  unsigned int cntrl=0;
  unsigned int cmd_offset=0;
  unsigned int cntrl_offset=0;
  unsigned int buff_offset=0;
  int i,option=0,error=0;
  unsigned int  CmdStack=0x1000;
  unsigned int  DataStack=0x4000;

  if (def.cmd_1.t_r == RECEIVE && def.rt_rt == NO && def.mode_code == NO)
    option = 0;
  else if (def.cmd_1.t_r == TRANSMIT && def.rt_rt == NO && def.mode_code == NO)
    option = 1;
  else if (def.mode_code == YES)
    option = 3;

  cmd =cmd | def.cmd_1.taddr<<11;
  cmd =cmd | def.cmd_1.t_r << 10;
  cmd =cmd | def.cmd_1.subaddr <<5;
  cmd =cmd | (def.cmd_1.wcnt % 32);
  cmd_offset = DataStack+((msg_num-1)*40)+1;
  MemWrite(ModId,cmd_offset,cmd,1);
               
  if(def.rt_rt == YES )
  {
    if (def.cmd_1.t_r != RECEIVE)
	    error = -20;
    else if (def.cmd_1.wcnt != def.cmd_2.wcnt)
	    error= -21;
    else  if (def.cmd_2.t_r != TRANSMIT)
	    error = -22;

    else 
    {
      option = 2;
      cmd = 0;
      cmd =cmd | def.cmd_2.taddr<<11;
      cmd =cmd | def.cmd_2.t_r << 10;
      cmd =cmd | def.cmd_2.subaddr <<5;
      cmd =cmd | (def.cmd_2.wcnt % 32);
      cmd_offset= DataStack+((msg_num-1)*40)+2;
      MemWrite(ModId,cmd_offset,cmd,1);
    }
   }
   cntrl=cntrl | def.retry_enable<<8;
   cntrl=cntrl | def.bus<<7;
   cntrl=cntrl | def.self_test<<6;
   cntrl=cntrl | def.mask_bcst_bit<<5;
   cntrl=cntrl | def.mode_code<<2;
   cntrl=cntrl | def.rt_rt;
   cntrl=cntrl | def.broadcast<<1;
   cntrl_offset = DataStack+((msg_num-1)*40);
   MemWrite(ModId,cntrl_offset,cntrl,1);
   MemWrite(ModId, CmdStack + 2 + ((msg_num - 1) * 4), IntMsgGap, 1);	
    switch (option)
    {
      case 0 :
      buff_offset = (DataStack + (msg_num -1) * 40) + 2 ;

      for (i=0; i < num_of_data_words; i++)
        MemWrite(ModId,buff_offset+i,data_buffer[i],1);
      buff_offset = DataStack + ((msg_num-1)*40)+2+num_of_data_words;
      MemWrite(ModId,buff_offset,def.loop,1);
      buff_offset = DataStack + (msg_num - 1)*40+2+num_of_data_words+1;
      MemWrite(ModId,buff_offset,def.cmd_1.status,1);
      break;
      case 1 :
      buff_offset = (DataStack + (msg_num -1)*40) + 4;
      for (i=0; i<num_of_data_words;i++)
        MemWrite(ModId,buff_offset+i,data_buffer[i],1);
      buff_offset = DataStack +((msg_num-1)*40)+2;
      MemWrite(ModId,buff_offset,def.loop,1);
      buff_offset = DataStack + ((msg_num-1)*40)+3;
      MemWrite(ModId,buff_offset,def.cmd_1.status,1);
      break;
      case 2 :
      buff_offset = DataStack + ((msg_num-1)*40)+3;
      MemWrite(ModId,buff_offset,def.loop,1);
      buff_offset = DataStack + ((msg_num-1)*40)+4;
      MemWrite(ModId,buff_offset,def.cmd_2.status,1);
      buff_offset = (DataStack + (msg_num -1)*40) + 5;
      for (i=0; i<num_of_data_words;i++)
	  MemWrite(ModId,buff_offset+i,data_buffer[i],1);
      buff_offset = DataStack + ((msg_num-1)*40)+5+num_of_data_words;
      MemWrite(ModId,buff_offset,def.cmd_1.status,1);
      break;
      case 3 :	  
      if (def.cmd_1.subaddr == 31 || def.cmd_1.subaddr == 0)
      {
          if ((def.cmd_1.t_r == TRANSMIT) &&(( def.cmd_1.wcnt == 16) ||( def.cmd_1.wcnt == 18) || ( def.cmd_1.wcnt == 19)) )
          {
            buff_offset = DataStack + ((msg_num-1)*40)+2;
            MemWrite(ModId,buff_offset,def.loop,1);
            buff_offset = DataStack + ((msg_num-1)*40)+3;
            MemWrite(ModId,buff_offset,def.cmd_1.status,1);
            buff_offset = DataStack + ((msg_num-1)*40)+4;
            MemWrite(ModId,buff_offset,data_buffer[0],1);              
          }

          else if (def.cmd_1.t_r == RECEIVE && (( def.cmd_1.wcnt == 20) || (def.cmd_1.wcnt == 17) || (def.cmd_1.wcnt == 21)) )
          {
            buff_offset = DataStack + ((msg_num-1)*40)+2;
            MemWrite(ModId,buff_offset,data_buffer[0],1);
            buff_offset = DataStack + ((msg_num-1)*40)+3;
            MemWrite(ModId,buff_offset,def.loop,1);
            buff_offset = DataStack + ((msg_num-1)*40)+4;
            MemWrite(ModId,buff_offset,def.cmd_1.status,1);           
          }
          else
          {
            buff_offset = DataStack + ((msg_num-1)*40)+2;
            MemWrite(ModId,buff_offset,def.loop,1);
            buff_offset = DataStack + ((msg_num-1)*40)+3;
            MemWrite(ModId,buff_offset,def.cmd_1.status,1);           
          } 
        } 
        break;
      } 
      return(error);
}
short DefFrame(UCHAR ModId,unsigned short number_of_messages,unsigned short mess_sequence[1024])
{
   int i,error=0;
   unsigned int msg_address,temp;
   unsigned int msg_offset;
   unsigned int CmdStack=0x1000;
   unsigned int DataStack=0x4000;
  if (number_of_messages <= 1024 && number_of_messages >= 1)	
  {
    for (i=0; i<number_of_messages; i++)
    {
        msg_address = 0x0000;
        msg_offset  = 0x0000;
        temp = (mess_sequence[i]-1);
        msg_address = ((temp) * 40) + DataStack;
        msg_offset  = CmdStack+((i) * 4)  + 0x0003;
        MemWrite(ModId,msg_offset,msg_address,1);		  
    }

    MemWrite(ModId,(msg_offset+CmdStack)+4,END_OF_FRAME,1);
    if (number_of_messages >1024 || number_of_messages < 1)
	    error = -24;
  }
  return(error);
}
short RunBc(UCHAR ModId,unsigned short frame,unsigned short position,unsigned short num_of_messages,unsigned short times)
{
	int				i,msg_addr,temp,init_pos,error=0;
	unsigned int	count;
	unsigned int CmdStack=0x1000;	
	position	=	frame +(position-1)*4;
	init_pos	=	position+CmdStack;
  if (num_of_messages <= 1024 && num_of_messages >= 1)	
  {
    if(frame==0x0)
     {
        RegWrite(ModId,CONFIG_REG,RegRead(ModId,CONFIG_REG,1)&0xdfff,1);
	if(times==0)
        {
         RegWrite(ModId,0x7,0x8000,1);
         RegWrite(ModId,0x1,RegRead(ModId,0x1,1)|0x0100,1);   
         RegWrite(ModId,0x1E,num_of_messages,1);
         MemWrite(ModId,MESS_COUNTER,times,1);
         StartReset(ModId,START);
         printf("Message Based Continuous Scheduling\n");
        }
	else
        { 
          RegWrite(ModId,0x1E,num_of_messages,1);
          MemWrite(ModId,MESS_COUNTER,times,1);//MESS_COUNTER 0x101
          count=MemRead(ModId,MESS_COUNTER,1);
         // printf("RegMsg_Cntr:(0x11) %x MemMsg_Cntr:(0x101) %x\n",RegRead(ModId,0x1E,1),count);			
          StartReset(ModId,START);
          count=RegRead(ModId,0x1,1);
           while (count != 0x0)//0x54c Completed
           {
              count=RegRead(ModId,0x1,1);//0x54d Incomplete
              count = count&1;
           }
           printf("Message based Scheduling Completed\n");          
        }	
      }
        return(error);
    }
    else
      return(-23);  
}
int DefMajorFrame(UCHAR Bus,USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[])
{	
  unsigned int i,m,r=0,number_of_messages = 0,framecounter=0;
  unsigned int msg_address,temp;
  unsigned int msg_offset,msg_offset1;
  USHORT Cnt_Time=0x0;
  
  if(Bus < 1 || Bus > 3)
      return (-2);

  if(no_of_minor_frames < 1 || no_of_minor_frames > 32)
	  return (-24);  
  for(m=0;m<no_of_minor_frames;m++)
  {	  
    Cnt_Time = frame_sequence[m].Mess_Count << 9;//<<9 APOORVA PCI
    Cnt_Time = Cnt_Time | (frame_sequence[m].MinorFrame_Time) ;
    RegWrite(Bus,0x3,0x8000,1);
    RegWrite(Bus,0x24 + m,Cnt_Time,1);
   // printf("Data %x Addres %x\n",RegRead(Bus,0x24+m,1),0x24+m);
  } 
  RegWrite(Bus,0x3,0x8000,1);
  RegWrite(Bus,0x10,no_of_minor_frames,1);
  //printf("Address(0x10) %x no_of_minor_frames %d\n",RegRead(Bus,0x10,1),no_of_minor_frames);
   
  for (m=0; m<no_of_minor_frames; m++)
    number_of_messages = number_of_messages + frame_sequence[m].Mess_Count;
  if (number_of_messages <= 1024 && number_of_messages >= 1)	
  {      
     for (m=0; m<no_of_minor_frames; m++)
     {
       for (i=0; i<frame_sequence[m].Mess_Count; i++)
       {
          msg_address = 0x0000;
          msg_offset  = 0x0000;
          temp = (frame_sequence[m].frame_items[i]-1);
          msg_address = ((temp) * 40) + 0x4000;
          msg_offset  = 0x1000+((framecounter) * 4)  + 0x0003;
          MemWrite(Bus,msg_offset,msg_address,1);
          framecounter++;
           //printf("DefFrame_num %d msg_offset %x  Msg  %x\n",i,msg_offset,MemRead(Bus,msg_offset,1));
        }
      } 
    MemWrite(Bus,(msg_offset+0x1000)+4,END_OF_FRAME,1);
    if (number_of_messages >1024 || number_of_messages < 1)
     return(-24);
  }
  else
    return(-10);
  return 1;
	
}
unsigned long RunBcMajorFrame(UCHAR Bus,USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],unsigned long times)
{  
    if(Bus < 1 || Bus > 3)
	  return (-2);
    if(no_of_minor_frames < 1 || no_of_minor_frames > 32)
	    return (-24);
          int i,m,init_pos,position,temp;
    position = stackA_B;
    USHORT count,num_of_messages=0,Mcnt=0,TimesValue;
    //position = position-1;				//////////////////  14-2-2k6
    unsigned int CmdStack=0x1000;
    position	=	stackA_B +(position-1)*4;
    init_pos	=	position+CmdStack;
    for (m=0; m<no_of_minor_frames; m++)
      num_of_messages = num_of_messages + frame_sequence[m].Mess_Count;

    if(stackA_B==0x0)
    {		
      if(times == 0)
      {      
        
         RegWrite(Bus,0x07,0x8000,1); 
         RegWrite(Bus,0x01,0x054f,1);            
        // RegWrite(Bus,0x1,RegRead(Bus,0x1,1)|0x0100,1);
         //printf("RegMsg_Cntr:(0x1) %x\n",RegRead(Bus,0x1,1));
       // RegWrite(Bus,0x07,0x3000,1);
        StartReset(Bus,START);		 
        printf("BC Frame Based Continuous Scheduling\n");
      }
      else
      {     
          RegWrite(Bus,0x3,0x8000,1);
          MemWrite(Bus,CmdStack,init_pos,1);
          MemWrite(Bus,MESS_COUNTER,times,1);//MESS_COUNTER 0x101
         // MemWrite(Bus,TMP_CNTR,temp,1);     
         // MemWrite(Bus,TMP_PTR,init_pos,1);   
          RegWrite(Bus,0x3,0x8000,1);
         // RegWrite(Bus,0x01,0x044f,1);
         // RegWrite(Bus,0x7,0x0000,1);
          StartReset(Bus,START);
          //count=MemRead(Bus,MESS_COUNTER,1);
          //printf("RegRead 0x07 = %x\n",RegRead(Bus,0x07,1));
          count=RegRead(Bus,0x1,1);
          while (count != 0)//0x44e Completed
          {
            count=RegRead(Bus,0x1,1);//0x44e Incomplete
            count = (count&2);//getchar();
             //printf("Frame Based Scheduling Completed %x %x\n",RegRead(Bus,0x1,1),count);   
          }
          printf("\tFrame Based Scheduling Completed\n");        			  
      }	
    }				
    return 1;
}
void UpdateMsgData(UCHAR ModId, unsigned short msg_num, unsigned short *new_data, int num_words, int direction)
{
    unsigned int data_stack_base = 0x4000;
    unsigned int msg_base = data_stack_base + (msg_num - 1) * 40;
    unsigned int buff_offset;
    int idx;

    if (direction == RECEIVE)   // BC → RT
        buff_offset = msg_base + 2;
    else if (direction == TRANSMIT) // RT → BC
        buff_offset = msg_base + 4;
    else
        return;

    for (idx = 0; idx < num_words; idx++)
    {
        MemWrite(ModId, buff_offset + idx, new_data[idx], 1);
    }
}
int UpdateMsgCmdData(UCHAR ModId,
                     unsigned short msg_num,
                     unsigned short cmd1,
                     unsigned short cmd2,
                     unsigned short *payload_buffer,
                     unsigned short num_of_data_words,
                     int direction,       // RECEIVE / TRANSMIT / RT_RT / MODE_CODE
                     int bus)             // 0 = Bus A, 1 = Bus B
{
    unsigned int data_stack_base = 0x4000;
    unsigned int base = data_stack_base + (msg_num - 1) * 40;
    unsigned int offset, control_word;
    int idx;

    /* Update command words when provided */
    if (cmd1)
        MemWrite(ModId, base + 1, cmd1, 1);

    if (cmd2 && direction == 2)   /* RT–RT */
        MemWrite(ModId, base + 2, cmd2, 1);

    /* Build control word */
    control_word = 0;
    control_word |= (0 << 8);                 /* retry_enable placeholder */
    control_word |= (bus & 0x1) << 7;         /* bus: 0 = A, 1 = B */
    control_word |= (0 << 6);                 /* self_test */
    control_word |= (0 << 5);                 /* mask_bcst_bit */
    control_word |= ((direction == 3) ? 1 : 0) << 2; /* mode_code flag */
    control_word |= ((direction == 2) ? 1 : 0);      /* rt_rt flag */
    control_word |= (0 << 1);                 /* broadcast */

    MemWrite(ModId, base + 0, control_word, 1);

    /* Update payload area depending on message type */
    switch (direction)
    {
        case RECEIVE:   /* BC → RT */
            offset = base + 2;
            for (idx = 0; idx < num_of_data_words; idx++)
                MemWrite(ModId, offset + idx, payload_buffer[idx], 1);
            break;

        case TRANSMIT:  /* RT → BC */
            offset = base + 4;
            for (idx = 0; idx < num_of_data_words; idx++)
                MemWrite(ModId, offset + idx, payload_buffer[idx], 1);
            break;

        case 2:   /* RT–RT */
            offset = base + 5;
            for (idx = 0; idx < num_of_data_words; idx++)
                MemWrite(ModId, offset + idx, payload_buffer[idx], 1);
            break;

        case 3:   /* Mode Code */
        {
            if (cmd1) MemWrite(ModId, base + 1, cmd1, 1);

            unsigned short word_count = cmd1 & 0x1F;       /* 5 bits word count/mode code */
            unsigned short tr_flag    = (cmd1 >> 10) & 0x1;

            if (tr_flag == TRANSMIT && (word_count == 16 || word_count == 18 || word_count == 19)) {
                MemWrite(ModId, base + 2, 0x0001, 1);
                MemWrite(ModId, base + 3, 0x0000, 1);
                if (num_of_data_words > 0)
                    MemWrite(ModId, base + 4, payload_buffer[0], 1);
            }
            else if (tr_flag == RECEIVE && (word_count == 17 || word_count == 20 || word_count == 21)) {
                if (num_of_data_words > 0)
                    MemWrite(ModId, base + 2, payload_buffer[0], 1);
                MemWrite(ModId, base + 3, 0x0001, 1);
                MemWrite(ModId, base + 4, 0x0000, 1);
            }
            else {
                MemWrite(ModId, base + 2, 0x0001, 1);
                MemWrite(ModId, base + 3, 0x0000, 1);
            }
        }
        break;

        default:
            return -1; /* invalid */
    }

    return 0;
}
void SetTerFlag(UCHAR ModId)
{
    unsigned int config;
    config = RegRead(ModId,CONFIG_REG,1);
    config = config & 0xFF2F;
    RegWrite(ModId,CONFIG_REG,config,1);
}
void ResetTerFlag(UCHAR ModId)
{
  unsigned int config;
  config = RegRead(ModId,CONFIG_REG,1);
  config = config | 0xFFCF;
  RegWrite(ModId,CONFIG_REG,config,1);
}
void PriorityHighLow(UCHAR ModId, unsigned short msg_num, unsigned short HighLow)
{
    unsigned int data_stack_base = 0x4000;
    unsigned short PriorityRegAddress=0x1d;
    unsigned int msg_base = data_stack_base + (msg_num - 1) * 40; 
    RegWrite(ModId, PriorityRegAddress, HighLow, 1);
    if(HighLow==1)
      MemWrite(ModId, 0xff80, msg_base, 1);
    else
      MemWrite(ModId, 0xffc0, msg_base, 1);   
}
void BCResponseTimeE(UCHAR ModId,UCHAR Enable, unsigned short BCResponseTime)
{
    if(Enable==1)
    {
      switch (BCResponseTime)
      {
        case 1:
        RegWrite(ModId,0x7,0x8000,1);
        RegWrite(ModId,0x9,(RegRead(ModId,0x9,1))|0x0000,1);
        //printf("(1BCWaits 24 Microseconds (If NoResponse) %x",(RegRead(module_id,0x9,1)));
        printf("(1BCWaits 24 Microseconds (If NoResponse)\n");
        break;
        case 2:
        RegWrite(ModId,0x7,0x8000,1);
        RegWrite(ModId,0x9,(RegRead(ModId,0x9,1))|0x0200,1);
        //printf("2BCWaits 28 Microseconds (If NoResponse) %x",(RegRead(module_id,0x9,1)));
        printf("2BCWaits 28 Microseconds (If NoResponse)\n");
        break;
        case 3:
        RegWrite(ModId,0x7,0x8000,1);
        RegWrite(ModId,0x9,(RegRead(ModId,0x9,1))|0x0400,1);
        //printf("3BCWaits 56 Microseconds (If NoResponse) %x",(RegRead(module_id,0x9,1)));
        printf("3BCWaits 56 Microseconds (If NoResponse)\n");
        break;
        case 4:
        RegWrite(ModId,0x7,0x8000,1);
        RegWrite(ModId,0x9,(RegRead(ModId,0x9,1))|0x0600,1);
       // printf("4BCWaits 136 Microseconds (If NoResponse) %x",(RegRead(module_id,0x9,1)));
        printf("4BCWaits 136 Microseconds (If NoResponse)\n");
        break;
      }
    }
}
void BCStartStop(UCHAR ModId,UCHAR Stop)
{
    if(Stop)
    {
      RegWrite(ModId,0x3,0x4000,1);
      printf("BC Stopped\n");
    }
    else
    {
      printf("BC Started\n");
      RegWrite(ModId,START_REG,START,1);      
    }
}
int ReadMsgRtCounter(UCHAR ModId)
{
  return MemRead(ModId, 0x100, 1);
}
int ReadMsgMtCounter(UCHAR ModId)
{
  return RegRead(ModId, 0xB, 1); 
}
