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
#include "//home/acl/APOORVA_CCPMC/Apoorva31Rts-4NodeDll/Include/Libraries.h"

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
    else if(CardId == 2)
    {
	fd2 = open("/dev/APOORVA2553",O_RDWR);
	
	if(fd2<0)
	{
		printf("Cannot open APOORVA2553 device\n");
		return FALSE;
	}
	else
	{
		Card2 = TRUE;
		return TRUE;	
	}
    }
    else if(CardId == 3)
    {
	fd3 = open("/dev/APOORVA3553",O_RDWR);
	if(fd3<0)
	{
		printf("Cannot open APOORVA3553 device\n");
		return FALSE;
	}
	else
	{
		Card3 = TRUE;
		return TRUE;	
	}
    }
    else if(CardId == 4)
    {
	fd4 = open("/dev/APOORVA4553",O_RDWR);
	if(fd4<0)
	{
		printf("Cannot open APOORVA4553 device\n");
		return FALSE;
	}
	else
	{
		Card4 = TRUE;
		return TRUE;	
	}
    }	
    else
	return FALSE;
}

CHAR CloseDeviceHandle(UCHAR CardId)
{
	if(CardId == 1)
	{
		close(fd);
		return TRUE;
	}
	else if(CardId == 2)
	{
		close(fd2);
		return TRUE;
	}
	else if(CardId == 3)
	{
		close(fd3);
		return TRUE;
	}
	else if(CardId == 4)
	{
		close(fd4);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR SoftwareReset(UCHAR CardId)
{
	int conf;
	if(CardId == 1)
		conf = ioctl(fd,IOCTL_CARD1_SOFTWARE_RESET,NULL);
	else if(CardId == 2)
		conf = ioctl(fd2,IOCTL_CARD2_SOFTWARE_RESET,NULL);
	else if(CardId == 3)
		conf = ioctl(fd3,IOCTL_CARD3_SOFTWARE_RESET,NULL);
	else if(CardId == 4)
		conf = ioctl(fd4,IOCTL_CARD4_SOFTWARE_RESET,NULL);
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
CHAR ConfigICSR(UCHAR CardId)
{
	int conf;
	if(CardId == 1)
		conf = ioctl(fd,IOCTL_CARD1_SET_ICS_REG,NULL);
	else if(CardId == 2)
		conf = ioctl(fd2,IOCTL_CARD2_SET_ICS_REG,NULL);
	else if(CardId == 3)
		conf = ioctl(fd3,IOCTL_CARD3_SET_ICS_REG,NULL);
	else if(CardId == 4)
		conf = ioctl(fd4,IOCTL_CARD4_SET_ICS_REG,NULL);
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

#if 0
USHORT MemRead(UCHAR Bus,ULONG len,UCHAR CardId)
{
	USHORT	us;	       
	BOOL	CardFlag = FALSE;
        Card1=1;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (2);
		
		if(CardId==1)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
					us = ioctl(fd,IOCTL_CARD1_MEM_READ_MODULE1,(unsigned int)len);
				else if(Bus == 2)
					us = ioctl(fd,IOCTL_CARD1_MEM_READ_MODULE2,(unsigned int)len);
				else if(Bus == 3)
					us = ioctl(fd,IOCTL_CARD1_MEM_READ_MODULE3,(unsigned int)len);
				else if(Bus == 4)
					us = ioctl(fd,IOCTL_CARD1_MEM_READ_MODULE4,(unsigned int)len);
			}
			else
				return 3;
		}
  		else if(CardId==2)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
					us = ioctl(fd2,IOCTL_CARD2_MEM_READ_MODULE1,(unsigned int)len);
				else if(Bus == 2)
					us = ioctl(fd2,IOCTL_CARD2_MEM_READ_MODULE2,(unsigned int)len);
				else if(Bus == 3)
					us = ioctl(fd2,IOCTL_CARD2_MEM_READ_MODULE3,(unsigned int)len);
				else if(Bus == 4)
					us = ioctl(fd2,IOCTL_CARD2_MEM_READ_MODULE4,(unsigned int)len);
			}
			else
				return 3;
		}
		else if(CardId==3)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
					us = ioctl(fd3,IOCTL_CARD3_MEM_READ_MODULE1,(unsigned int)len);
				else if(Bus == 2)
					us = ioctl(fd3,IOCTL_CARD3_MEM_READ_MODULE2,(unsigned int)len);
				else if(Bus == 3)
					us = ioctl(fd3,IOCTL_CARD3_MEM_READ_MODULE3,(unsigned int)len);
				else if(Bus == 4)
					us = ioctl(fd3,IOCTL_CARD3_MEM_READ_MODULE4,(unsigned int)len);
			}
			else
				return 3;
		}
		else if(CardId==4)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
					us = ioctl(fd4,IOCTL_CARD4_MEM_READ_MODULE1,(unsigned int)len);
				else if(Bus == 2)
					us = ioctl(fd4,IOCTL_CARD4_MEM_READ_MODULE2,(unsigned int)len);
				else if(Bus == 3)
					us = ioctl(fd4,IOCTL_CARD4_MEM_READ_MODULE3,(unsigned int)len);
				else if(Bus == 4)
					us = ioctl(fd4,IOCTL_CARD4_MEM_READ_MODULE4,(unsigned int)len);
			}
			else
				return 3;
		}
   		return us;
	}
	else
		return FALSE;
}
#endif
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
                    perror("MemRead: ioctl failed");
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
	unsigned short MemData[2];
	MemData[0] = 0x1;
	MemData[1] = 0xa5a5;
	Addr	ad;
        Card1=1;
	ad.Data	=	(val & 0xFFFF);
	ad.Addr	=	(UINT)len;
       // printf("MemData:%x %x\n",ad.Data,ad.Addr);
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
	{
	      //printf("Card Fialed:\n");
		return FALSE;
	}
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
		//printf("Card Successs:\n");
	}
	if(CardFlag == TRUE)
	{
	       // printf("TrueData:%x %x\n",ad.Data,ad.Addr);
		if(Bus < 1 || Bus > 4)
			return (2);
	
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
				return 3;
		}
		else if(CardId==2)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
				{
					ioctl(fd2,IOCTL_CARD2_MEM_WRITE_MODULE1,(unsigned int *)&ad);
				}
				else if(Bus == 2)
				{
					ioctl(fd2,IOCTL_CARD2_MEM_WRITE_MODULE2,(unsigned int *)&ad);
				}
				else if(Bus == 3)
				{
					ioctl(fd2,IOCTL_CARD2_MEM_WRITE_MODULE3,(unsigned int *)&ad);
				}
				else if(Bus == 4)
				{
					ioctl(fd2,IOCTL_CARD2_MEM_WRITE_MODULE4,(unsigned int *)&ad);
				}
			}
			else
				return 3;
		}
		else if(CardId==3)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
				{
					ioctl(fd3,IOCTL_CARD3_MEM_WRITE_MODULE1,(unsigned int *)&ad);
				}
				else if(Bus == 2)
				{
					ioctl(fd3,IOCTL_CARD3_MEM_WRITE_MODULE2,(unsigned int *)&ad);
				}
				else if(Bus == 3)
				{
					ioctl(fd3,IOCTL_CARD3_MEM_WRITE_MODULE3,(unsigned int *)&ad);
				}
				else if(Bus == 4)
				{
					ioctl(fd3,IOCTL_CARD3_MEM_WRITE_MODULE4,(unsigned int *)&ad);
				}
			}
			else
				return 3;
		}
		else if(CardId==4)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
				{
					ioctl(fd4,IOCTL_CARD4_MEM_WRITE_MODULE1,(unsigned int *)&ad);
				}
				else if(Bus == 2)
				{
					ioctl(fd4,IOCTL_CARD4_MEM_WRITE_MODULE2,(unsigned int *)&ad);
				}
				else if(Bus == 3)
				{
					ioctl(fd4,IOCTL_CARD4_MEM_WRITE_MODULE3,(unsigned int *)&ad);
				}
				else if(Bus == 4)
				{
					ioctl(fd4,IOCTL_CARD4_MEM_WRITE_MODULE4,(unsigned int *)&ad);
				}
			}
			else
				return 3;
		}
		return TRUE;
	}
	else
		return FALSE;
}


USHORT RegRead(UCHAR Bus,ULONG len,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
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
		else if(CardId==2)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
					us = ioctl(fd2,IOCTL_CARD2_REG_READ_MODULE1,(unsigned int)len);
				else if(Bus == 2)
					us = ioctl(fd2,IOCTL_CARD2_REG_READ_MODULE2,(unsigned int)len);
				else if(Bus == 3)
					us = ioctl(fd2,IOCTL_CARD2_REG_READ_MODULE3,(unsigned int)len);
				else if(Bus == 4)
					us = ioctl(fd2,IOCTL_CARD2_REG_READ_MODULE4,(unsigned int)len);
			}
			else
				return 5;
		}
		else if(CardId==3)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
					us = ioctl(fd3,IOCTL_CARD3_REG_READ_MODULE1,(unsigned int)len);
				else if(Bus == 2)
					us = ioctl(fd3,IOCTL_CARD3_REG_READ_MODULE2,(unsigned int)len);
				else if(Bus == 3)
					us = ioctl(fd3,IOCTL_CARD3_REG_READ_MODULE3,(unsigned int)len);
				else if(Bus == 4)
					us = ioctl(fd3,IOCTL_CARD3_REG_READ_MODULE4,(unsigned int)len);
			}
			else
				return 5;
		}
		else if(CardId==4)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
					us = ioctl(fd4,IOCTL_CARD4_REG_READ_MODULE1,(unsigned int)len);
				else if(Bus == 2)
					us = ioctl(fd4,IOCTL_CARD4_REG_READ_MODULE2,(unsigned int)len);
				else if(Bus == 3)
					us = ioctl(fd4,IOCTL_CARD4_REG_READ_MODULE3,(unsigned int)len);
				else if(Bus == 4)
					us = ioctl(fd4,IOCTL_CARD4_REG_READ_MODULE4,(unsigned int)len);
			}
			else
				return 5;
		}
		return us;
	}
	else
		return FALSE;
}


USHORT RegWrite(UCHAR Bus,ULONG len,USHORT val,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	Addr	ad;

	ad.Data	=	(val);
	ad.Addr	=	len;


	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else 
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (2);
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
				return 5;
		}
		else if(CardId==2)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
				{
					ioctl(fd,IOCTL_CARD2_REG_WRITE_MODULE1,(unsigned int *)&ad);
				}
				else if(Bus == 2)
				{
					ioctl(fd,IOCTL_CARD2_REG_WRITE_MODULE2,(unsigned int *)&ad);
				}
				else if(Bus == 3)
				{
					ioctl(fd,IOCTL_CARD2_REG_WRITE_MODULE3,(unsigned int *)&ad);
				}
				else if(Bus == 4)
				{
					ioctl(fd,IOCTL_CARD2_REG_WRITE_MODULE4,(unsigned int *)&ad);
				}
			}
			else 
				return 5;
		}
		else if(CardId==3)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
				{
					ioctl(fd,IOCTL_CARD3_REG_WRITE_MODULE1,(unsigned int *)&ad);
				}
				else if(Bus == 2)
				{
					ioctl(fd,IOCTL_CARD3_REG_WRITE_MODULE2,(unsigned int *)&ad);
				}
				else if(Bus == 3)
				{
					ioctl(fd,IOCTL_CARD3_REG_WRITE_MODULE3,(unsigned int *)&ad);
				}
				else if(Bus == 4)
				{
					ioctl(fd,IOCTL_CARD3_REG_WRITE_MODULE4,(unsigned int *)&ad);
				}
			}
			else 
				return 5;
		}
		else if(CardId==4)
		{
			if((len>=0)&&(len<=0x7FFFF))
			{
				if(Bus == 1)
				{
					ioctl(fd,IOCTL_CARD4_REG_WRITE_MODULE1,(unsigned int *)&ad);
				}
				else if(Bus == 2)
				{
					ioctl(fd,IOCTL_CARD4_REG_WRITE_MODULE2,(unsigned int *)&ad);
				}
				else if(Bus == 3)
				{
					ioctl(fd,IOCTL_CARD4_REG_WRITE_MODULE3,(unsigned int *)&ad);
				}
				else if(Bus == 4)
				{
					ioctl(fd,IOCTL_CARD4_REG_WRITE_MODULE4,(unsigned int *)&ad);
				}
			}
			else 
				return 5;
		}
		return TRUE;
	}
	else
		return FALSE;
}

CHAR GetCardPresence(UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId == 1)
			CardFlag = Card1;
		else if(CardId == 2)
			CardFlag = Card2;
		else if(CardId == 3)
			CardFlag = Card3;
		else if(CardId == 4)
			CardFlag = Card4;
	}
	return CardFlag;
}

CHAR ClearRam(UCHAR Bus,USHORT Module,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	BOOL	Card_Type = 1;
	ULONG	i,j,m,addr,RtCntrlRegAdd,Offset,CirBufPntrA,CirBufPntrB,CirLookUpA,CirLookUpB; 
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		/*if(Module >= 0 &&  Module <=30 )				// For single RT
			addr=0x1e00 ;  */
		if(Card_Type == 1)						// if Card is 31 RTs
		{
			if(((short)Module >= 0) &&  (Module <=30))			// RT0 To RT30
				addr=0x1E00 + (0x1E00 * Module) ;
			else if(Module == 31)					// BC
			{
				addr = 0x0000;
				for(m=0;m<175;m++)
				{
					Int_MsgA[m] = 0x0;
					Int_MsgB[m] = 0x0;
				}
			}
			else if(Module == 32)					// MT
				addr = 0x3c000;
			else
				return (-6);
			
			if(Module==32)						//MT
			{
				for (i=addr ;i < (addr + 0x4000); i++)
						MemWrite(Bus,i,0xFFFF,CardId);
				i--;
				if((j=MemRead(Bus,i,CardId))==0xFFFF)
					return TRUE;
				else
					return (-7);
			}
			else 
			{
				if(addr == 0)
				{
					for (i=addr ;i < (addr + 0x1E00); i++)		// BC
						MemWrite(Bus,i,0xFFFF,CardId);
					i--;
					if((j=MemRead(Bus,i,CardId))==0xFFFF)
						return TRUE;
					else
						return (-7);
				}
				if(((short)Module >=0) && (Module <=30))
				{
					for (i=addr ;i < (addr + 0x1E00); i++)
					{
						if((i != 0x200A) && (i != 0x200B))
							MemWrite(Bus,i,0xFFFF,CardId);
					}				
					RtCntrlRegAdd=((0x1e00*Module)+0x2000);
					RtCntrlRegAdd= ((RtCntrlRegAdd & 0x3fff0)|0xc);
					MemWrite(Bus,RtCntrlRegAdd,0x206,CardId);		//StackA
					Offset = (Module * 0x1e00) + 0x3980;
					for(m=0;m<128;m++) 
					{
						MemWrite(Bus,(Offset + m),0x0,CardId); 		//Legalize ALL commands
					}

					///////////////////////////  Circular Buffer Pointer default 0x0 ///////////////////
					CirBufPntrA = ((0x1e00*Module)+0x2002);
					CirBufPntrB = ((0x1e00*Module)+0x2003);
					MemWrite(Bus,CirBufPntrA,0x0,CardId);
					MemWrite(Bus,CirBufPntrB,0x0,CardId);

					CirLookUpA = ((0x1e00*Module)+0x2080);
					CirLookUpB = ((0x1e00*Module)+0x2100);
					for(i=0x0;i<=0x3f;i++)
					{
						MemWrite(Bus,CirLookUpA+i,0x0,CardId);
						MemWrite(Bus,CirLookUpB+i,0x0,CardId);
					}
					
					////////////////////  RT Enable / Disable ///////////////////////

					MemWrite(Bus,0x2140,0xFFFF,CardId);
					MemWrite(Bus,0x2141,0xFFFF,CardId);
					MemWrite(Bus,0x2142,0xFFFF,CardId);
					MemWrite(Bus,0x2143,0xFFFF,CardId);

				}
			}
		}
		else
		{
			if(((short)Module >= 0) &&  (Module <=30))			// RT0 To RT30
				addr=0x1e00 ;  
			else if( Module == 31)					// BC
			{
				addr = 0x0000;
				for(m=0;m<175;m++)
				{
					Int_MsgA[m] = 0x0;
					Int_MsgB[m] = 0x0;
				}
			}
			else if(Module == 32)					// MT
				addr = 0x3c000;
			else
				return (-6);
			
			if(Module==32)						//MT
			{
				for (i=addr ;i < (addr + 0x4000); i++)
						MemWrite(Bus,i,0xFFFF,CardId);
				i--;
				if((j=MemRead(Bus,i,CardId))==0xFFFF)
					return TRUE;
				else
					return (-7);
			}
			else 
			{
				if(addr == 0)
				{
					for (i=addr ;i < (addr + 0x1E00); i++)		// BC
						MemWrite(Bus,i,0xFFFF,CardId);
					i--;
					if((j=MemRead(Bus,i,CardId))==0xFFFF)
						return TRUE;
					else
						return (-7);
				}
				else
				{
					for (i=addr ;i < (addr + 0x1E00); i++)		//  RT
						MemWrite(Bus,i,0xFFFF,CardId);
					for(i=0x3980;i<=0x39ff;i++)
						MemWrite(Bus,i,0x0000,CardId);

					MemWrite(Bus,0x2002,0x0,CardId);
					MemWrite(Bus,0x2003,0x0,CardId);

					for (i=0x3C00 ;i < 0x3C3F; i++)			// Changed for Circular Buffer
						MemWrite(Bus,i,0x0,CardId);
					
				/*	for (i=0x6000 ;i < 0x3BFFF; i++)
						MemWrite(Bus,i,0xFFFF,CardId);*/ 

					i--;
					if((j=MemRead(Bus,i,CardId))==0x0)
						return TRUE;
					else
						return (-7);
				}
			}
		}
		return TRUE;	
	}
	else
		return FALSE;
}


SHORT SelfTest(UCHAR CardId)
{
	BOOL	CardFlag = FALSE;

	ULONG		Offset,Wait=0;
	SHORT		Error=1;
	USHORT		frame_items[10],data_buffer[40], i,TempCnt,ErrCnt=31,Bc_Stack,Ch=1;
	UCHAR		Mod=1,Stack,Address[31];
	MSG_DEF		bc_page;
	MSG_RESULT	msg_return;	
	Address[1] 	= 1;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		
		for(Mod = 1;Mod<=2;Mod++)
		{
			for(Offset=0x0;Offset<=0x3ffff;Offset++)
				MemWrite(Mod,Offset,0xA5A5,CardId);
					
			for(Offset=0x0;Offset<=0x3ffff;Offset++)
				if(MemRead(Mod,Offset,CardId)!= 0xA5A5)
					Error = ErrCnt;
			
			if(Error != 1)
				return (-Error);
			
			for(Offset=0x0;Offset<=0x3ffff;Offset++)
				MemWrite(Mod,Offset,0x5A5A,CardId);
					
			for(Offset=0x0;Offset<=0x3ffff;Offset++)
				if(MemRead(Mod,Offset,CardId)!= 0x5A5A)
					Error = ErrCnt;
						
			if(Error != 1)
				return (-Error);	
			ErrCnt++;
		}
		for(Mod = 1;Mod<=2;Mod++)
		{
			for(Stack = 0;Stack<=1;Stack++)
			{

			///////// Configuring Module as RT   ////////////
					
				ConfigureRT(Mod,Stack,Address,CardId);
				RunRt(Mod,Stack,Address,CardId);
				if(RegRead(Mod,0x2D,CardId) != 0x2)
				     Error = ErrCnt;
				if(Error != 1)
					return (-Error);	
				ErrCnt++;
				
			//////////    Configuring Module as MT   ///////////
				
				SelMsgMt(Mod,Stack,CardId);
				RunMt(Mod,CardId);
		if(Stack ==0)
		{
			if(RegRead(Mod,0x13,CardId) != 0x1000 || 
				RegRead(Mod,0x17,CardId) != 0x8000)
			Error = ErrCnt;
		}
		else if(Stack ==1)
		{
			if(RegRead(Mod,0x13,CardId) != 0x3000 ||
		 		RegRead(Mod,0x17,CardId) != 0x8000)
			Error = ErrCnt;
		}
		if(Error != 1)
		   return (-Error);	
		ResetMt(Mod,CardId);
		  ErrCnt++;
				
		//////    Configuring Module as BC   //////////

			for(Ch = 0;Ch<=1;Ch++)
			{
				InitInterrupts(Mod,INTR_BCEOM,CardId);

					SetBCEOMCnt(Mod,0,CardId);
					if(Stack == 0)
						Bc_Stack = BCSTACK_A;
					else if(Stack == 1)
						Bc_Stack = BCSTACK_B;
					
					ClearRam(Mod,BC,CardId);
					ConfigureBC(Mod,100,CardId);
					
					bc_page.bus           = Ch;
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
					for(i=0; i<32; i++)
						data_buffer[i] = 0xaaaa;
			DefMsg(Mod,Bc_Stack,1,bc_page,data_buffer,32,CardId);
					
					bc_page.cmd_1.subaddr = 2;
					bc_page.cmd_1.wcnt    = 32;  
					for(i=0; i<32; i++)
						data_buffer[i] = 0xbbbb;
			DefMsg(Mod,Bc_Stack,2,bc_page,data_buffer,32,CardId);

					bc_page.cmd_1.subaddr = 3;
					bc_page.cmd_1.wcnt    = 32;  
					for(i=0; i<32; i++)
						data_buffer[i] = 0xcccc;
			DefMsg(Mod,Bc_Stack,3,bc_page,data_buffer,32,CardId);

					bc_page.cmd_1.subaddr = 4;
					bc_page.cmd_1.wcnt    = 32;  
					for(i=0; i<32; i++)
						data_buffer[i] = 0xdddd;
			DefMsg(Mod,Bc_Stack,4,bc_page,data_buffer,32,CardId);

					bc_page.cmd_1.subaddr = 5;
					bc_page.cmd_1.wcnt    = 32;  
					for(i=0; i<32; i++)
						data_buffer[i] = 0xeeee;
			DefMsg(Mod,Bc_Stack,5,bc_page,data_buffer,32,CardId);

					frame_items[0] = 1;
					frame_items[1] = 2;
					frame_items[2] = 3;
					frame_items[3] = 4;
					frame_items[4] = 5;
			DefFrame(Mod,Bc_Stack,5,frame_items,CardId);
		RunBc(Mod,Bc_Stack,1,5,1,CardId);			
		
			Error = ReadMsgBC(Mod,Bc_Stack,1,&msg_return,CardId);
				if(Error != 1)
				  return (-Error);	
					
	if(Ch == 1)
	{
	       if(msg_return.bsw != 0xA000 || msg_return.loop_word != 0xAAAA)
		Error = ErrCnt;
	}
	else if(Ch == 0)
	{
	  if(msg_return.bsw != 0x8000 || msg_return.loop_word != 0xAAAA)
	   Error = ErrCnt;
	}

	TempCnt= msg_return.cmd1_bits.wcnt;
					
		if(TempCnt == 0)
		 TempCnt = 32;
		for(i=0;i<TempCnt;i++)
		{
			if(msg_return.data[i] != 0xAAAA)
			Error = ErrCnt;
		}
		
		ReadMsgBC(Mod,Bc_Stack,2,&msg_return,CardId);
	if(Ch == 1)
	{
	  if(msg_return.bsw != 0xA000 || msg_return.loop_word != 0xBBBB)
	  Error = ErrCnt;
	}
	else if(Ch == 0)
	{
	  if(msg_return.bsw != 0x8000 || msg_return.loop_word != 0xBBBB)
	  Error = ErrCnt;
	}
		TempCnt= msg_return.cmd1_bits.wcnt;
					
		if(TempCnt == 0)
	   	  TempCnt = 32;
		for(i=0;i<TempCnt;i++)
		{
			if(msg_return.data[i] != 0xBBBB)
			 Error = ErrCnt;
		}
					
			ReadMsgBC(Mod,Bc_Stack,3,&msg_return,CardId);

	if(Ch == 1)
	{
	  if(msg_return.bsw != 0xA000 || msg_return.loop_word != 0xCCCC)
		Error = ErrCnt;
	}
	else if(Ch == 0)
	{
	  if(msg_return.bsw != 0x8000 || msg_return.loop_word != 0xCCCC)
	   Error = ErrCnt;
	}
	TempCnt= msg_return.cmd1_bits.wcnt;
					
	if(TempCnt == 0)
	  TempCnt = 32;
	for(i=0;i<TempCnt;i++)
	{
		if(msg_return.data[i] != 0xCCCC)
			Error = ErrCnt;
	}
					
	ReadMsgBC(Mod,Bc_Stack,4,&msg_return,CardId);
	if(Ch == 1)
	{
	  if(msg_return.bsw != 0xA000 || msg_return.loop_word != 0xDDDD)
		Error = ErrCnt;
	}
	else if(Ch == 0)
	{
	 if(msg_return.bsw != 0x8000 || msg_return.loop_word != 0xDDDD)
		Error = ErrCnt;
	}
	TempCnt= msg_return.cmd1_bits.wcnt;
					
	if(TempCnt == 0)
	 TempCnt = 32;
	
	for(i=0;i<TempCnt;i++)
	{
		if(msg_return.data[i] != 0xDDDD)
		Error = ErrCnt;
	}
	
	ReadMsgBC(Mod,Bc_Stack,5,&msg_return,CardId);
	if(Ch == 1)
	{
	  if(msg_return.bsw != 0xA000 || msg_return.loop_word != 0xEEEE)
		Error = ErrCnt;
	}
	else if(Ch == 0)
	{
	  if(msg_return.bsw != 0x8000 || msg_return.loop_word != 0xEEEE)
	  Error = ErrCnt;
	}
	 TempCnt= msg_return.cmd1_bits.wcnt;
		
	 if(TempCnt == 0)
	   TempCnt = 32;
	 for(i=0;i<TempCnt;i++)
	 {
		if(msg_return.data[i] != 0xEEEE)
			Error = ErrCnt;
	 }
					
	if(Error != 1)
	 return (-Error);	
	ErrCnt++;
					
		if(GetBCEOMCnt(Mod,CardId)!=5)
			Error = ErrCnt;
					
		if(Error != 1)
	  	  return (-Error);	
		ErrCnt++;
		DisableInterrupts(Mod,CardId);
	}
  }
	ResetBc(Mod,CardId);
	ResetMt(Mod,CardId);
	//ResetRt(Mod,CardId);
	}

		SelectTMR(US_128,0,0,CardId);
		SetTMR1Cnt(0,CardId);
		SetTMR2Cnt(0,CardId);
		SetTMR3Cnt(0,CardId);
		
		InitTimerIntr(INTR_TMR1,CardId);
		Wait = 0;
		while(GetTMR1Cnt(CardId)<50)
		{
			Wait++;
			if(Wait > 5000)
				break;
		}

		if(GetTMR1Cnt(CardId) < 50)
			Error = ErrCnt;
		if(Error != 1)
		  return (-Error);	
			ErrCnt++;
		
		StopTimerIntr(INTR_TMR1,CardId);

		SelectTMR(0,MS_32,0,CardId);
		SetTMR1Cnt(0,CardId);
		SetTMR2Cnt(0,CardId);
		SetTMR3Cnt(0,CardId);
		
		InitTimerIntr(INTR_TMR2,CardId);
		Wait = 0;
		while(GetTMR2Cnt(CardId)<30)
		{
			Wait++;
			if(Wait > 500000)
				break;
		}
		if(GetTMR2Cnt(CardId)<30)
			Error = ErrCnt;
		if(Error != 1)
		  return (-Error);	
			ErrCnt++;

		StopTimerIntr(INTR_TMR2,CardId);

		SelectTMR(0,0,MS_250,CardId);
		SetTMR1Cnt(0,CardId);
		SetTMR2Cnt(0,CardId);
		SetTMR3Cnt(0,CardId);
		
		InitTimerIntr(INTR_TMR3,CardId);

		Wait = 0;
		while(GetTMR3Cnt(CardId)<10)
		{
			Wait++;
			if(Wait > 1000000)
				break;
		}
		if(GetTMR3Cnt(CardId)<10)
			Error = ErrCnt;
		
		StopTimerIntr(INTR_TMR3,CardId);
		
		DisableInterrupts(1,CardId);
		DisableInterrupts(2,CardId);
		sleep(200);		
		SoftwareReset(CardId);
		sleep(400);
		return (-Error);
	}
	else
		return FALSE;
}

CHAR ConfigureBC(UCHAR Bus,USHORT stack_size,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		if(stack_size < 1 || stack_size > 128)
			return (-8);
		StackA = stack_size;
		StackB = 174 - StackA;
		if(StackB > 128)
			StackB = 128;

		RegWrite(Bus,0x1,0x0,CardId);	//Interrupt mask reg
		RegWrite(Bus,0x2,0x0,CardId);	//cr1
		RegWrite(Bus,0x6,0x0,CardId);	//Time tag reg

		MemWrite(Bus,MESS_COUNTER_A,0,CardId);
		MemWrite(Bus,MESS_COUNTER_B,0,CardId);
		return TRUE;
	}
	else
		return FALSE;
}
CHAR IntMsgGapLeast(UCHAR Bus,UCHAR flag,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	USHORT	val;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		
		if(flag==YES)
			val = (RegRead(Bus,0x2,CardId) & 0xFF7F) | (0x80);
		else
			val = RegRead(Bus,0x2,CardId) & 0xFF7F;

		RegWrite(Bus,0x2,val,CardId);		//cr1
		return TRUE;
	}
	else
		return FALSE;
}
SHORT DefMsg_StackA(UCHAR Bus,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words,UCHAR CardId)
{
	unsigned int	cmd=0;
	unsigned int	cntrl=0;
	unsigned int	cmd_offset=0;
	unsigned int	cntrl_offset=0;
	unsigned int	buff_offset=0;
	int		i,option=0,error=1;

	if(msg_num > StackA)
		return FALSE;

	if(def.cmd_1.t_r == RECEIVE && def.rt_rt == NO && def.mode_code == NO)
		option = 0;
	else if(def.cmd_1.t_r == TRANSMIT && def.rt_rt == NO && def.mode_code == NO)
		option = 1;
	else if (def.mode_code == YES)
		option = 3;

    	 cntrl=cntrl | def.int_msg_gap<<14; 
	 cntrl=cntrl | def.retry_enable<<8;
	 cntrl=cntrl | def.bus<<7;
	 cntrl=cntrl | def.self_test<<6;
	 cntrl=cntrl | def.mask_bcst_bit<<5;
	 cntrl=cntrl | def.mode_code<<2;
	 cntrl=cntrl | def.rt_rt;
	 cntrl=cntrl | def.broadcast<<1;
	 cntrl_offset = 0x0208+((msg_num-1)*39);
	 MemWrite(Bus,cntrl_offset,cntrl,CardId);

	cmd =cmd | def.cmd_1.taddr<<11;
	cmd =cmd | def.cmd_1.t_r << 10;
	cmd =cmd | def.cmd_1.subaddr <<5;
	cmd =cmd | (def.cmd_1.wcnt % 32);
	cmd_offset = 0x0208+((msg_num-1)*39)+0x1;
	
	MemWrite(Bus,cmd_offset,cmd,CardId);

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
			cmd_offset= 0x0208+((msg_num-1)*39)+2;
			MemWrite(Bus,cmd_offset,cmd,CardId);
		}
	 }
 	Int_MsgA[msg_num] = def.Int_Msg_Gap - 10;  

	if(Int_MsgA[msg_num] > 0xFFF5)
		Int_MsgA[msg_num] = 0x0; 
	 
	if (def.cmd_1.taddr <=31 && def.cmd_1.taddr >= 0)
	{
 	  switch (option)
	  {
	   case 0 :

			buff_offset = (0x0208 + (msg_num -1) * 39) + 2 ;
			for (i=0; i < num_of_data_words; i++)
			MemWrite(Bus,buff_offset+i,data_buffer[i],CardId);

			buff_offset = 0x0208 + ((msg_num-1)*39)+2+num_of_data_words;
			MemWrite(Bus,buff_offset,def.loop,CardId);

	     		buff_offset = 0x0208 + ((msg_num - 1)*39)+2+num_of_data_words+1;
			MemWrite(Bus,buff_offset,def.cmd_1.status,CardId);
		break;
	   case 1 :
			buff_offset = (0x0208 + (msg_num -1)*39) + 4;
			for (i=0; i<num_of_data_words;i++)
			MemWrite(Bus,buff_offset+i,data_buffer[i],CardId);

			buff_offset = 0x0208 +((msg_num-1)*39)+2;
			MemWrite(Bus,buff_offset,def.loop,CardId);

			buff_offset = 0x0208 + ((msg_num-1)*39)+3;
			MemWrite(Bus,buff_offset,def.cmd_1.status,CardId);
		break;
	  case 2 :
			buff_offset = 0x0208 + ((msg_num-1)*39)+3;
			MemWrite(Bus,buff_offset,def.loop,CardId);

			buff_offset = 0x0208 + ((msg_num-1)*39)+4;
			MemWrite(Bus,buff_offset,def.cmd_2.status,CardId);

			buff_offset = (0x0208 + (msg_num -1)*39) + 5;
			for (i=0; i<num_of_data_words;i++)
				MemWrite(Bus,buff_offset+i,data_buffer[i],CardId);

			buff_offset = 0x0208 + ((msg_num-1)*39)+5+num_of_data_words;
			MemWrite(Bus,buff_offset,def.cmd_1.status,CardId);
		break;

	case 3 :
	  
		if (def.cmd_1.subaddr == 31 || def.cmd_1.subaddr == 0)
				{
	        if ((def.cmd_1.t_r == TRANSMIT) &&(( def.cmd_1.wcnt == 16)
		  || ( def.cmd_1.wcnt == 18) || ( def.cmd_1.wcnt == 19)))
		{
			buff_offset = 0x0208 + ((msg_num-1)*39)+2;
			MemWrite(Bus,buff_offset,def.loop,CardId);
			buff_offset = 0x0208 + ((msg_num-1)*39)+3;
		        MemWrite(Bus,buff_offset,def.cmd_1.status,CardId);

			buff_offset = 0x0208 + ((msg_num-1)*39)+4;
			MemWrite(Bus,buff_offset,data_buffer[0],CardId);
		}

	      else if (def.cmd_1.t_r == RECEIVE && (( def.cmd_1.wcnt == 20)
		 || (def.cmd_1.wcnt == 17) || (def.cmd_1.wcnt == 21)))
	      {
			buff_offset = 0x0208 + ((msg_num-1)*39)+2;
			MemWrite(Bus,buff_offset,data_buffer[0],CardId);
			buff_offset = 0x0208 + ((msg_num-1)*39)+3;
			MemWrite(Bus,buff_offset,def.loop,CardId);
			buff_offset = 0x0208 + ((msg_num-1)*39)+4;
			MemWrite(Bus,buff_offset,def.cmd_1.status,CardId);
		}
		else
		{
			buff_offset = 0x0208 + ((msg_num-1)*39)+2;
			MemWrite(Bus,buff_offset,def.loop,CardId);
			buff_offset = 0x0208 + ((msg_num-1)*39)+3;
			MemWrite(Bus,buff_offset,def.cmd_1.status,CardId);
		} 
	} 
		break;
	}   
		if (def.cmd_1.taddr >31 || def.cmd_1.taddr <0)
			error = -23;
	}
   return (error);
}

SHORT DefMsg_StackB(UCHAR Bus,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words,UCHAR CardId)
{
	unsigned int	cmd=0;
	unsigned int	cntrl=0;
	unsigned int	cmd_offset=0;
	unsigned int	cntrl_offset=0;
	unsigned int	buff_offset=0;
	int		i,option=0,error=1;
	
	if(msg_num > StackB)
		return FALSE;

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
	cmd_offset = 0x0208+((msg_num-1)*39)+1+(StackA * 39);
	MemWrite(Bus,cmd_offset,cmd,CardId);

	if(def.rt_rt == YES )
	{
		if (def.cmd_1.t_r != RECEIVE)
			error = -20;
		else if (def.cmd_1.wcnt != def.cmd_2.wcnt)
			error= -21;
		else  if (def.cmd_2.t_r != TRANSMIT)
			error = -22;

		else 
		{	option = 2;
			cmd = 0;
			cmd =cmd | def.cmd_2.taddr<<11;
			cmd =cmd | def.cmd_2.t_r << 10;
			cmd =cmd | def.cmd_2.subaddr <<5;
			cmd =cmd | (def.cmd_2.wcnt % 32);
			cmd_offset= 0x0208+((msg_num-1)*39)+2+(StackA * 39);
			MemWrite(Bus,cmd_offset,cmd,CardId);
		}
	 }
	 cntrl=cntrl | def.int_msg_gap<<14;
	 cntrl=cntrl | def.retry_enable<<8;
	 cntrl=cntrl | def.bus<<7;
	 cntrl=cntrl | def.self_test<<6;
	 cntrl=cntrl | def.mask_bcst_bit<<5;
	 cntrl=cntrl | def.mode_code<<2;
	 cntrl=cntrl | def.rt_rt;
	 cntrl=cntrl | def.broadcast<<1;
	 cntrl_offset = 0x0208+((msg_num-1)*39)+(StackA * 39);
	 MemWrite(Bus,cntrl_offset,cntrl,CardId);


	Int_MsgB[msg_num] = def.Int_Msg_Gap - 10;  

	if(Int_MsgB[msg_num] > 0xFFF5)
		Int_MsgB[msg_num] = 0x0;

	if (def.cmd_1.taddr <=31 && def.cmd_1.taddr >= 0)
	{
	  switch (option)
	  {
	    case 0 :

		buff_offset = (0x0208 + (msg_num -1) * 39) + 2+(StackA * 39);
		for (i=0; i < num_of_data_words; i++)
			MemWrite(Bus,buff_offset+i,data_buffer[i],CardId);

		buff_offset = 0x0208 + ((msg_num-1)*39)+2+ (StackA * 39)
				 +num_of_data_words;
			MemWrite(Bus,buff_offset,def.loop,CardId);

		buff_offset = 0x0208 + ((msg_num - 1)*39)+2+(StackA * 39)
				 +num_of_data_words+1;
			MemWrite(Bus,buff_offset,def.cmd_1.status,CardId);
		break;

	     case 1 :

		buff_offset = (0x0208 + (msg_num -1)*39) + 4+(StackA * 39);
		for (i=0; i<num_of_data_words;i++)
			MemWrite(Bus,buff_offset+i,data_buffer[i],CardId);

		buff_offset = 0x0208 +((msg_num-1)*39)+2+(StackA * 39);
		
		MemWrite(Bus,buff_offset,def.loop,CardId);

		buff_offset = 0x0208 + ((msg_num-1)*39)+3+(StackA * 39);
			MemWrite(Bus,buff_offset,def.cmd_1.status,CardId);
		break;

	    case 2 :

		buff_offset = 0x0208 + ((msg_num-1)*39)+3+(StackA * 39);
			MemWrite(Bus,buff_offset,def.loop,CardId);

		buff_offset = 0x0208 + ((msg_num-1)*39)+4+(StackA * 39);
			MemWrite(Bus,buff_offset,def.cmd_2.status,CardId);

		buff_offset = (0x0208 + (msg_num -1)*39) + 5+(StackA * 39);
		for (i=0; i<num_of_data_words;i++)
			MemWrite(Bus,buff_offset+i,data_buffer[i],CardId);

	        buff_offset = 0x0208 + ((msg_num-1)*39)+5+(StackA*39)	
			+num_of_data_words;
		   MemWrite(Bus,buff_offset,def.cmd_1.status,CardId);
		break;

	    case 3 :
	  
		if (def.cmd_1.subaddr == 31 || def.cmd_1.subaddr == 0)
		{
		  if ((def.cmd_1.t_r == TRANSMIT) &&(( def.cmd_1.wcnt == 16)
		|| ( def.cmd_1.wcnt == 18) || ( def.cmd_1.wcnt == 19)) )
		{
		  buff_offset = 0x0208 + ((msg_num-1)*39)+2+(StackA * 39);
			MemWrite(Bus,buff_offset,def.loop,CardId);

		  buff_offset = 0x0208 + ((msg_num-1)*39)+3+(StackA * 39);
			MemWrite(Bus,buff_offset,def.cmd_1.status,CardId);

		  buff_offset = 0x0208 + ((msg_num-1)*39)+4+(StackA * 39);
			MemWrite(Bus,buff_offset,data_buffer[0],CardId);
		}

  	  else if (def.cmd_1.t_r == RECEIVE && (( def.cmd_1.wcnt == 20) ||
			 (def.cmd_1.wcnt == 17) || (def.cmd_1.wcnt == 21)) )
	  {
		buff_offset = 0x0208 + ((msg_num-1)*39)+2+(StackA * 39);
			MemWrite(Bus,buff_offset,data_buffer[0],CardId);

		buff_offset = 0x0208 + ((msg_num-1)*39)+3+(StackA * 39);
			MemWrite(Bus,buff_offset,def.loop,CardId);

		buff_offset = 0x0208 + ((msg_num-1)*39)+4+(StackA * 38);
		MemWrite(Bus,buff_offset,def.cmd_1.status,CardId);
	}
	else
	{
		buff_offset = 0x0208 + ((msg_num-1)*39)+2+(StackA * 39);
			MemWrite(Bus,buff_offset,def.loop,CardId);
		buff_offset = 0x0208 + ((msg_num-1)*39)+3+(StackA * 39);
		MemWrite(Bus,buff_offset,def.cmd_1.status,CardId);
	} 
	} 
	break;
    } 
		
		if (def.cmd_1.taddr >31 || def.cmd_1.taddr <0)
			error = -23;
	}
   return(error);
}

SHORT DefMsg(UCHAR Bus,USHORT BCSTACK_A_B,USHORT msg_num,MSG_DEF def,USHORT data_buffer[],USHORT num_of_data_words,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	SHORT	val;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		
		if(msg_num < 1 || msg_num > 128)
		   return (-10);
		
		if(((short)num_of_data_words < 0) || ((short)num_of_data_words > 32))
			return (-11);
		
		if(num_of_data_words == 0)
			num_of_data_words = 32;

		if(BCSTACK_A_B	==	BCSTACK_A)
			val=DefMsg_StackA(Bus,msg_num,def,data_buffer,num_of_data_words,CardId);
		else if(BCSTACK_A_B	==	BCSTACK_B)
			val=DefMsg_StackB(Bus,msg_num,def,data_buffer,num_of_data_words,CardId);
		else
			return (-9);
	return val;
	}
	else
		return FALSE;
}

SHORT DefFrame(UCHAR Bus,USHORT BCSTACK_A_B,USHORT number_of_messages,USHORT mess_sequence[173],UCHAR CardId)
{	
	BOOL		CardFlag = FALSE;
	int		i;
	unsigned int	msg_address,temp;
	unsigned int	msg_offset,msg_offset1=0x0;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(number_of_messages < 1 || number_of_messages > 128)
			return (24);
		
		if(BCSTACK_A_B	==	BCSTACK_A)
		{
			if (number_of_messages <= StackA && number_of_messages >= 1)	
			{
				for (i=0; i<number_of_messages; i++)
				{
					msg_offset  = 0x0000;
					temp = (mess_sequence[i]-1);
					msg_address = ((temp) * 39) + 0x0208;

					msg_offset  = ((i) * 4)  + 0x0002;
					MemWrite(Bus,msg_offset,Int_MsgA[mess_sequence[i]],CardId);

					msg_offset  = ((i) * 4)  + 0x0003;
					MemWrite(Bus,msg_offset,msg_address,CardId);
				}
				MemWrite(Bus,msg_offset+4,END_OF_FRAME,CardId);
			
			}
			else
				return 20;			
		}
		else if(BCSTACK_A_B	==	BCSTACK_B)
		{
			if (number_of_messages <= StackB && number_of_messages >= 1)	
			{
				for (i=0; i<number_of_messages; i++)
				{
					msg_offset  = 0x01c00;
					temp = (mess_sequence[i]-1);
					msg_address = ((temp) * 39) + 0x0208 + (StackA * 39);

					msg_offset1  = msg_offset + ((i) * 4)  + 0x0002;
					MemWrite(Bus,msg_offset1,Int_MsgB[mess_sequence[i]],CardId);

					msg_offset  = msg_offset + ((i) * 4) + 0x0003;
					MemWrite(Bus,msg_offset,msg_address,CardId);
				}
				MemWrite(Bus,msg_offset+4,END_OF_FRAME,CardId);			
			}
			else
				return (21);
		}
		else
			return (23);
		
		return TRUE;
	}
	else
		return FALSE;
}


/*SHORT DefMajorFrame(UCHAR Bus,USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[128],UCHAR CardId)
{
	BOOL		CardFlag = FALSE;
	unsigned int	i,m,r=0,number_of_messages = 0;
	unsigned int	msg_address,temp;
	unsigned int	msg_offset,msg_offset1=0x0;
	USHORT		Cnt_Time=0x0;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		   return (-2);
		
		if(no_of_minor_frames < 1 || no_of_minor_frames > 128)
			return (-24);
			
		for(m=0;m<no_of_minor_frames;m++)
		{
			Cnt_Time = frame_sequence[m].Mess_Count << 9;
		  Cnt_Time = Cnt_Time | frame_sequence[m].MinorFrame_Time ;
			RegWrite(Bus,0x80 + m,Cnt_Time,CardId);
		}

		for (m=0; m<no_of_minor_frames; m++)
			number_of_messages = number_of_messages +
				 frame_sequence[m].Mess_Count;

 	if(BCSTACK_A_B==BCSTACK_A)
	  {
	    if (number_of_messages <= StackA &&
		 number_of_messages >= 1)	
	   {
		for (m=0; m<no_of_minor_frames; m++)
		{
		  for (i=0; i<frame_sequence[m].Mess_Count; i++)
		  {
			msg_offset  = 0x0000;
			temp = (frame_sequence[m].frame_items[i] -1);
			msg_address = ((temp) * 39) + 0x0208;

			msg_offset  = ((r) * 4)  + 0x0002;
			MemWrite(Bus,msg_offset,		
			Int_MsgA[frame_sequence[m].frame_items[i]],CardId);

			msg_offset  = ((r) * 4)  + 0x0003;
			MemWrite(Bus,msg_offset,msg_address,CardId);
			r++;
		 }
	       }
	  MemWrite(Bus,msg_offset+4,END_OF_FRAME,CardId);
	}
	else
		return (-10);
	}
	else if(BCSTACK_A_B==BCSTACK_B)
	{
	  if (number_of_messages <= StackB && number_of_messages >= 1)	
	  {
		for (m=0; m<no_of_minor_frames; m++)
		{
	  	  for (i=0; i<frame_sequence[m].Mess_Count; i++)
		  {
			msg_offset  = 0x01c00;
			temp = (frame_sequence[m].frame_items[i]-1);
			msg_address = ((temp) * 39) + 0x0208 + (StackA * 39);
			msg_offset1  = msg_offset +  ((r) * 4)  + 0x0002;
			MemWrite(Bus,msg_offset1,Int_MsgB[frame_sequence[m]
					.frame_items[i]],CardId);
			msg_offset  = msg_offset + ((r) * 4) + 0x0003;
			MemWrite(Bus,msg_offset,msg_address,CardId);
			r++;
		    }
		  }
			MemWrite(Bus,msg_offset+4,END_OF_FRAME,CardId);
	        }
		else
	 		return(-10);
	    }
	     else
	  	   return (-9);
	 return TRUE;
	}
	else
		return FALSE;
}*/

SHORT DefMajorFrame(UCHAR Bus,USHORT BCSTACK_A_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],UCHAR CardId)
{
	
	
	BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else if(CardId ==5)
		CardFlag = Card5;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		unsigned int i,m,r=0,number_of_messages = 0;
		unsigned int msg_address,temp;
		unsigned int msg_offset,msg_offset1;
		USHORT Cnt_Time=0x0;
		
		if(Bus < 1 || Bus > 4)
			return (-2);
		
		if(no_of_minor_frames < 1 || no_of_minor_frames > 64)
			return (-24);
		
		for(m=0;m<no_of_minor_frames;m++)
		{
			/*if(frame_sequence[m].MinorFrame_Time < (frame_sequence[m].Mess_Count * 800)/1000)
				return 25;*/
			Cnt_Time = frame_sequence[m].Mess_Count << 9;
			Cnt_Time = Cnt_Time | frame_sequence[m].MinorFrame_Time ;
			RegWrite(Bus,0x80 + m,Cnt_Time,CardId);
		}

		for (m=0; m<no_of_minor_frames; m++)
			number_of_messages = number_of_messages + frame_sequence[m].Mess_Count;

		if(BCSTACK_A_B==BCSTACK_A)
		{
			if (number_of_messages <= StackA && number_of_messages >= 1)	
			{
				for (m=0; m<no_of_minor_frames; m++)
				{
					for (i=0; i<frame_sequence[m].Mess_Count; i++)
					{
						msg_offset  = 0x0000;
						temp = (frame_sequence[m].frame_items[i] -1);
						msg_address = ((temp) * 39) + 0x0208;

						msg_offset  = ((r) * 4)  + 0x0002;
						MemWrite(Bus,msg_offset,Int_MsgA[frame_sequence[m].frame_items[i]],CardId);

						msg_offset  = ((r) * 4)  + 0x0003;
						MemWrite(Bus,msg_offset,msg_address,CardId);
						r++;
					}
				}
				MemWrite(Bus,msg_offset+4,END_OF_FRAME,CardId);
				
			}
			else
				return(-10);
		}
		else if(BCSTACK_A_B==BCSTACK_B)
		{
			if (number_of_messages <= StackB && number_of_messages >= 1)	
			{
				for (m=0; m<no_of_minor_frames; m++)
				{
					for (i=0; i<frame_sequence[m].Mess_Count; i++)
					{
						msg_offset  = 0x01c00;
						temp = (frame_sequence[m].frame_items[i]-1);
						msg_address = ((temp) * 39) + 0x0208 + (StackA * 39);

						msg_offset1  = msg_offset +  ((r) * 4)  + 0x0002;
						MemWrite(Bus,msg_offset1,Int_MsgB[frame_sequence[m].frame_items[i]],CardId);

						msg_offset  = msg_offset + ((r) * 4) + 0x0003;
						MemWrite(Bus,msg_offset,msg_address,CardId);
						r++;
					}
				}
				MemWrite(Bus,msg_offset+4,END_OF_FRAME,CardId);
			
			}
			else
				return(-10);
		}
		else
			return (-9);
	
		return TRUE;
	}
	else
		return FALSE;
}


SHORT DefMajorFrameStackA(UCHAR Bus,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],UCHAR CardId)
{
		BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else if(CardId ==5)
		CardFlag = Card5;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		unsigned int i,m,r=0,number_of_messages=0;
		unsigned int msg_address,temp;
		unsigned int msg_offset,msg_offset1;
		USHORT Cnt_Time=0x0;

		if(Bus < 1 || Bus > 4)
			return (-2);
		
		if(no_of_minor_frames < 1 || no_of_minor_frames > 64)
			return (-24);
		
		for(m=0;m<no_of_minor_frames;m++)
		{
			/*if(frame_sequence[m].MinorFrame_Time < (frame_sequence[m].Mess_Count * 800)/1000)
				return 25;*/
			Cnt_Time = frame_sequence[m].Mess_Count << 9;
			Cnt_Time = Cnt_Time | frame_sequence[m].MinorFrame_Time ;
			RegWrite(Bus,0x80 + m,Cnt_Time,CardId);
		}

		RegWrite(Bus,0x02,(RegRead(Bus,0x02,CardId) | 0x8000),CardId);

		for (m=0; m<no_of_minor_frames; m++)
			number_of_messages = number_of_messages + frame_sequence[m].Mess_Count;

		if(number_of_messages <= 255 && number_of_messages >= 1)	
		{
			for (m=0; m<no_of_minor_frames; m++)
			{
				for (i=0; i<frame_sequence[m].Mess_Count; i++)
				{
					if(r <= 127)
					{
						msg_offset  = 0x0000;
						temp = (frame_sequence[m].frame_items[i] -1);
						msg_address = ((temp) * 39) + 0x0208;
						
						msg_offset  = ((r) * 4)  + 0x0002;
						MemWrite(Bus,msg_offset,Int_MsgA[frame_sequence[m].frame_items[i]],CardId);

						msg_offset  = ((r) * 4)  + 0x0003;
						MemWrite(Bus,msg_offset,msg_address,CardId);
					}
					else
					{
						msg_offset  = 0x1C00;
						temp = (frame_sequence[m].frame_items[i] -1);
						msg_address = ((temp) * 39) + 0x0208;
						
						msg_offset1  = msg_offset + (((r-128) * 4)  + 0x0002);
						MemWrite(Bus,msg_offset1,Int_MsgA[frame_sequence[m].frame_items[i]],CardId);

						msg_offset  = msg_offset + (((r-128) * 4)  + 0x0003);
						MemWrite(Bus,msg_offset,msg_address,CardId);


					}
					r++;
				}
			}
			MemWrite(Bus,msg_offset+4,END_OF_FRAME,CardId);
		}
		else
			return(-10);
				
		return TRUE;
	}
	else
		return FALSE;
}

CHAR BCAutoRepeat(UCHAR Bus,UCHAR sel,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	CR1=0x02;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		if(sel	==	YES)
			RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)|0x0100,CardId);
										//for run Autorepeat
		else if(sel	==	NO)
			RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)&0xFEFF,CardId);
										//to stop Autorepeat
		return TRUE;
	}
	else
		return FALSE;
}

SHORT RunBc(UCHAR Bus,USHORT stackA_B,USHORT Position,USHORT num_of_messages,ULONG times,UCHAR CardId)
{
	BOOL CardFlag = FALSE;

	int		i,init_pos,position=0;
	ULONG		CR1 = 0x02,STRSTR = 0x0F;
	USHORT		count,Mcnt=0,TimesValue;

	position = stackA_B;             
	init_pos = position;
	position = position-1;				//////////////////  14-2-2k6		
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		BOOL Card_Type = 1;

		/*if(position < 1 || position > 128)
			return (-10);						commented for checking Stack Merge

		if(num_of_messages < 1 || num_of_messages > 128)
			return (-10);*/

		if((RegRead(Bus,0x02,CardId) & 0x8000) == 0x8000)   // To check Stack Merged or not
		{
			if((Position < 1) || (Position > 255))		
				return (25);

			if((num_of_messages < 1) || (num_of_messages > 255))
				return (26);					
		}
		else
		{
			if((Position < 1) || (Position > 128))		
				return (27);

			if((num_of_messages < 1) || (num_of_messages > 128))
				return (28);			
		}
		
		if(Card_Type == 1)
		{
		    RegWrite(Bus,0x80,0x0,CardId);
		}
		else
		{
			MemWrite(Bus,0x5d50,0x0,CardId);
		}
		
		if(stackA_B==0x0)
		{			
			//RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)&0xDFFF,CardId);//13th bit -->0 for STACKA
			if(times == 0)
			{
				RegWrite(Bus,0x58,0x0,CardId);
				RegWrite(Bus,0x59,0x0,CardId);
				RegWrite(Bus,CR1, ((RegRead(Bus,CR1,CardId)&0xDFFF) | 0x0100),CardId);//13th bit -->0 for STACKA with Auto Repeat ON
			}
			else
			{
				TimesValue = (USHORT)(times & 0xFFFF);
				RegWrite(Bus,0x58,TimesValue,CardId);

				TimesValue = (USHORT)(times >> 16);
				RegWrite(Bus,0x59,TimesValue,CardId);
				RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)&0xDFFF,CardId);//13th bit -->0 for STACKA
			}
									
			for (i=0;i<num_of_messages;i++)			/////////////   14-2-2k6
			{
				position = position + 4 ;   
				if (MemRead(Bus,position,CardId) == 0x00FF)
					return (-28);
			}

				count=MemRead(Bus,MESS_COUNTER_A,CardId);
				Mcnt=0;
				while (count != 0x0) 
				{
					count	=	MemRead(Bus,MESS_COUNTER_A,CardId);
					Mcnt++;
					if(Mcnt >= 130)
						return (-24);
				}

				MemWrite(Bus,STACK_PTR_A,init_pos,CardId);
				MemWrite(Bus,MESS_COUNTER_A,num_of_messages,CardId);
				MemWrite(Bus,INIT_MSG_CNT_A,num_of_messages,CardId);     
				MemWrite(Bus,INIT_STK_PTR_A,init_pos,CardId);   
				RegWrite(Bus,STRSTR,0x02,CardId);//Run BC
				RegWrite(Bus,STRSTR,0,CardId);	//Stop BC
										
		}	
		
		else if(stackA_B == 0x1c00)
		{
			//RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)|0x2000,CardId);	//13th bit -->1 for STACKB
			if(times == 0)
			{
				RegWrite(Bus,0x58,0x0,CardId);
				RegWrite(Bus,0x59,0x0,CardId);
				RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)|0x2100,CardId);	//13th bit -->1 for STACKB with Auto Repeat ON
			}
			else
			{
				TimesValue = (USHORT)(times & 0xFFFF);
				RegWrite(Bus,0x58,TimesValue,CardId);

				TimesValue = (USHORT)(times >> 16);
				RegWrite(Bus,0x59,TimesValue,CardId);
				RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)|0x2000,CardId);	//13th bit -->1 for STACKB
			}

			for (i=0;i<num_of_messages;i++)			/////////////   14-2-2k6
			{
				position = position + 4 ;   
				if (MemRead(Bus,position,CardId) == 0x00FF)
					return (-28);
			}
				count	=	MemRead(Bus,MESS_COUNTER_B,CardId);
				Mcnt=0;
				while (count != 0x0) 
				{
					count=MemRead(Bus,MESS_COUNTER_B,CardId);  
					Mcnt++;
					if(Mcnt >= 130)
						return (-24);
				}
				
				MemWrite(Bus,STACK_PTR_B,init_pos,CardId);
				MemWrite(Bus,MESS_COUNTER_B,num_of_messages,CardId);
				MemWrite(Bus,INIT_MSG_CNT_B,num_of_messages,CardId);     
				MemWrite(Bus,INIT_STK_PTR_B,init_pos,CardId);   
				RegWrite(Bus,STRSTR,0x02,CardId);			//Run BC
				RegWrite(Bus,STRSTR,0,CardId);				//Stop BC		
		}
		else
			return (-9);

		return TRUE;
	}
	else
		return FALSE;
}


/*SHORT RunBcMajorFrame(UCHAR Bus,USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[128],ULONG times,UCHAR CardId)
{	
	BOOL	CardFlag = FALSE;

	int	i,m,init_pos,position;
	ULONG	CR1=0x02,STRSTR=0x0F;
	USHORT	count,num_of_messages=0,Mcnt=0,TimesValue;

	position = stackA_B;             
	init_pos = position;
	position = position - 1;  

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		if(no_of_minor_frames < 1 || no_of_minor_frames > 128)
			return (-24);

		for (m=0; m<no_of_minor_frames; m++)
			num_of_messages = num_of_messages + 
						frame_sequence[m].Mess_Count;
		

		if(stackA_B==0x0)
		{
		   if(times == 0)
		   {
			RegWrite(Bus,0x58,0x0,CardId);
			RegWrite(Bus,0x59,0x0,CardId);
			RegWrite(Bus,CR1, ((RegRead(Bus,CR1,CardId)&0xDFFF)
						 | 0x0100),CardId);
			//13th bit -->0 for STACKA with Auto Repeat ON
		   }
		    else
		    {
				TimesValue = (USHORT)(times & 0xFFFF);
				RegWrite(Bus,0x58,TimesValue,CardId);

				TimesValue = (USHORT)(times >> 16);
				RegWrite(Bus,0x59,TimesValue,CardId);
				RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)&0xDFFF,CardId);//13th bit -->0 for STACKA
		     }
									
			for (i=0;i<num_of_messages;i++)	 //Added on 7-3-2k6
			{
				position = position +4 ;   
				if (MemRead(Bus,position,CardId) == 0x00FF)
					return(-28);
			}
			count=MemRead(Bus,MESS_COUNTER_A,CardId);
			Mcnt=0;
			while (count != 0x0) 
			{
				count=MemRead(Bus,MESS_COUNTER_A,CardId);
				sleep(1);
				Mcnt++;
				if(Mcnt >= 130)
				return (-24);
			}

		MemWrite(Bus,STACK_PTR_A,init_pos,CardId);
		MemWrite(Bus,MESS_COUNTER_A,no_of_minor_frames,CardId);
		MemWrite(Bus,INIT_MSG_CNT_A,no_of_minor_frames,CardId);     
		MemWrite(Bus,INIT_STK_PTR_A,init_pos,CardId);   
		RegWrite(Bus,STRSTR,0x02,CardId);//Run BC
		RegWrite(Bus,STRSTR,0,CardId);	//Stop BC
 	    }	
		
	    else if(stackA_B == 0x1c00)
	    {
		
			if(times == 0)
			{
				RegWrite(Bus,0x58,0x0,CardId);
				RegWrite(Bus,0x59,0x0,CardId);
			RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)|0x2100
					,CardId);
			//13th bit -->1 for STACKB with Auto Repeat ON
			}
			else
			{
				TimesValue = (USHORT)(times & 0xFFFF);
				RegWrite(Bus,0x58,TimesValue,CardId);

				TimesValue = (USHORT)(times >> 16);
				RegWrite(Bus,0x59,TimesValue,CardId);
				RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)
				|0x2000,CardId);	
				//13th bit -->1 for STACKB
			}

			for (i=0;i<num_of_messages;i++)	 //Added on 7-3-2k6
			{
				position = position +4 ;   
				if (MemRead(Bus,position,CardId) == 0x00FF)
					return(-28);
			}
			count=MemRead(Bus,MESS_COUNTER_B,CardId);
			Mcnt = 0;
			while (count != 0x0) 
			{
				count=MemRead(Bus,MESS_COUNTER_B,CardId);  
				sleep(1);
				Mcnt++;
				if(Mcnt >= 130)
				return (-24);
			}
				
		MemWrite(Bus,STACK_PTR_B,init_pos,CardId);
		MemWrite(Bus,MESS_COUNTER_B,no_of_minor_frames,CardId);
		MemWrite(Bus,INIT_MSG_CNT_B,no_of_minor_frames,CardId);     
		MemWrite(Bus,INIT_STK_PTR_B,init_pos,CardId);   
		RegWrite(Bus,STRSTR,0x02,CardId);//Run BC
		RegWrite(Bus,STRSTR,0,CardId);	//Stop BC
	    }
	     else
		return (-9);
	 return TRUE;	
	}
	else
	    return FALSE;
}*/

SHORT RunBcMajorFrame(UCHAR Bus,USHORT stackA_B,USHORT no_of_minor_frames,MAJOR_FRAME frame_sequence[],ULONG times,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else if(CardId ==5)
		CardFlag = Card5;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		if(no_of_minor_frames < 1 || no_of_minor_frames > 64)
			return (-24);
	

		int i,m,init_pos,position;
		position = stackA_B;
		init_pos=position;
		ULONG CR1=0x02,STRSTR=0x0F;
		USHORT count,num_of_messages=0,Mcnt=0,TimesValue;
		position = position-1;				//////////////////  14-2-2k6

		for (m=0; m<no_of_minor_frames; m++)
			num_of_messages = num_of_messages + frame_sequence[m].Mess_Count;

		if(stackA_B==0x0)
		{
			
			if(times == 0)
			{
				RegWrite(Bus,0x58,0x0,CardId);
				RegWrite(Bus,0x59,0x0,CardId);
				RegWrite(Bus,CR1, ((RegRead(Bus,CR1,CardId)&0xDFFF) | 0x0100),CardId);//13th bit -->0 for STACKA with Auto Repeat ON
			}
			else
			{
				TimesValue = (USHORT)(times & 0xFFFF);
				RegWrite(Bus,0x58,TimesValue,CardId);

				TimesValue = (USHORT)(times >> 16);
				RegWrite(Bus,0x59,TimesValue,CardId);
				RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)&0xDFFF,CardId);//13th bit -->0 for STACKA
			}
									
			for (i=0;i<num_of_messages;i++)			/////////////   14-2-2k6
			{
				position = position + 4 ;   
				if (MemRead(Bus,position,CardId) == 0x00FF)
					return (-28);
			}

		
				count=MemRead(Bus,MESS_COUNTER_A,CardId);
				Mcnt=0;
				while (count != 0x0) 
				{
					count=MemRead(Bus,MESS_COUNTER_A,CardId);
					usleep(1000);
					Mcnt++;
					if(Mcnt >= 130)
						return (-24);
				}

				MemWrite(Bus,STACK_PTR_A,init_pos,CardId);
				MemWrite(Bus,MESS_COUNTER_A,no_of_minor_frames,CardId);
				MemWrite(Bus,INIT_MSG_CNT_A,no_of_minor_frames,CardId);     
				MemWrite(Bus,INIT_STK_PTR_A,init_pos,CardId);   
				RegWrite(Bus,STRSTR,0x02,CardId);//Run BC
				RegWrite(Bus,STRSTR,0,CardId);	//Stop BC
	
		}	
		
		else if(stackA_B == 0x1c00)
		{
			if(times == 0)
			{
				RegWrite(Bus,0x58,0x0,CardId);
				RegWrite(Bus,0x59,0x0,CardId);
				RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)|0x2100,CardId);	//13th bit -->1 for STACKB with Auto Repeat ON
			}
			else
			{
				TimesValue = (USHORT)(times & 0xFFFF);
				RegWrite(Bus,0x58,TimesValue,CardId);

				TimesValue = (USHORT)(times >> 16);
				RegWrite(Bus,0x59,TimesValue,CardId);
				RegWrite(Bus,CR1, RegRead(Bus,CR1,CardId)|0x2000,CardId);	//13th bit -->1 for STACKB
			}

			for (i=0;i<num_of_messages;i++)			/////////////   14-2-2k6
			{
				position = position + 4 ;   
				if (MemRead(Bus,position,CardId) == 0x00FF)
					return (-28);
			}

				count=MemRead(Bus,MESS_COUNTER_B,CardId);
				Mcnt = 0;
				while (count != 0x0) 
				{
					count=MemRead(Bus,MESS_COUNTER_B,CardId);  
					usleep(1000);
					Mcnt++;
					if(Mcnt >= 130)
						return (-24);
				}
				
				MemWrite(Bus,STACK_PTR_B,init_pos,CardId);
				MemWrite(Bus,MESS_COUNTER_B,no_of_minor_frames,CardId);
				MemWrite(Bus,INIT_MSG_CNT_B,no_of_minor_frames,CardId);     
				MemWrite(Bus,INIT_STK_PTR_B,init_pos,CardId);   
				RegWrite(Bus,STRSTR,0x02,CardId);//Run BC
				RegWrite(Bus,STRSTR,0,CardId);	//Stop BC
			
			}
		else
			return (-9);
		
		return TRUE;	
	}
	else
		return FALSE;
}


CHAR BCRetry(UCHAR Bus,USHORT sel,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	USHORT	val;
	ULONG	CR1=0x02;	

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		 return (-2);
		
		val = RegRead(Bus,CR1,CardId);
		val = val&0xFFF8;
		val = val|sel;
		RegWrite(Bus,CR1,val,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

SHORT ReadMsgBC(UCHAR Bus,USHORT stackA_B,USHORT frame_item,MSG_RESULT *result,UCHAR CardId)
{
	BOOL		CardFlag = FALSE;
	MSG_DEF		data;
	unsigned int	position,cntrl_wrd,msg_addr;
	unsigned int	i,temp;
	int		option;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(frame_item < 1 || frame_item > 128)
			return (-10);
	
		position = stackA_B +((frame_item-1)*4);
		result->bsw  = MemRead(Bus,position,CardId);
		if(((result->bsw & 0x8000) != 0x8000) || 
						(result->bsw == 0xFFFF))
		   return (-24);
		
		result->bsw_bits = ReadBswBits(result->bsw);
		result->time_tag = MemRead(Bus,position+1,CardId);
		msg_addr = MemRead(Bus,position+3,CardId);  

		cntrl_wrd = MemRead(Bus,msg_addr,CardId);
		result->cntrl_word = cntrl_wrd;

		data = ReturnCntrlFields(cntrl_wrd);
		result->cmd_1 = MemRead(Bus,msg_addr+1,CardId);
		if(result->cmd_1 == 0xFFFF)
			return (-24);

		if (data.rt_rt == 0 && data.broadcast == 0 && 
							data.mode_code == 0)
		{
			result->cmd1_bits = ReturnCmdFields(result->cmd_1);
			if (result->cmd1_bits.t_r == RECEIVE )
				option = 0;
			else if (result->cmd1_bits.t_r == TRANSMIT)
				option = 1;
		}

		else if (data.rt_rt == 1 )
			option = 2;  

		else if ( data.mode_code == 1 && data.broadcast == 0 )
		{
			result->cmd1_bits = ReturnCmdFields(result->cmd_1);
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
			result->cmd1_bits = ReturnCmdFields(result->cmd_1);
			if(data.rt_rt == 1)
				option = 9;
			else
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
			result->data[i] = MemRead(Bus,msg_addr + 2 +i,CardId);
		result->loop_word	= MemRead(Bus,msg_addr + 1 + temp + 1,CardId);
		result->status_1	= MemRead(Bus,msg_addr + 1 + temp + 2,CardId);
		result->stat1_bits	= ReadStatusBits(result->status_1);
		break;

	 case 1 :   
		result->loop_word	= MemRead(Bus,msg_addr + 2,CardId);
		result->status_1	= MemRead(Bus,msg_addr + 3,CardId);
		result->stat1_bits	= ReadStatusBits(result->status_1);
		temp = result->cmd1_bits.wcnt;
		if(temp == 0)
			temp = 32;
		for (i=0;i< temp;i++)
			result->data[i] = MemRead(Bus,msg_addr + 4 + i,CardId);
		break;

	 case 2 :
		result->cmd_1		= MemRead(Bus,msg_addr+1,CardId);
		result->cmd1_bits	= ReturnCmdFields(result->cmd_1);
		result->cmd_2		= MemRead(Bus,msg_addr + 2,CardId);
		result->cmd2_bits	= ReturnCmdFields(result->cmd_2);
		result->loop_word	= MemRead(Bus,msg_addr + 3,CardId);
		result->status_1	= MemRead(Bus,msg_addr +  4,CardId);
		result->stat1_bits	= ReadStatusBits(result->status_1);
		temp = result->cmd1_bits.wcnt;
		if(temp == 0)
	 	  temp = 32;
		for(i=0;i<temp;i++)
			result->data[i] = MemRead(Bus,msg_addr + 5 + i,CardId);
		result->status_2	= MemRead(Bus,msg_addr + 5 + i ,CardId );
		result->stat2_bits	= ReadStatusBits(result->status_2);
		break;

	 case 3 :   
		result->data[0]		= MemRead(Bus,msg_addr + 2 ,CardId);
		result->loop_word	= MemRead(Bus,msg_addr + 3,CardId);
		result->status_1	= MemRead(Bus,msg_addr + 4,CardId);
		result->stat1_bits	= ReadStatusBits(result->status_1);
		break;
	 case 4 :
		result->loop_word	= MemRead(Bus,msg_addr + 2,CardId);
		result->status_1	= MemRead(Bus,msg_addr + 3,CardId);
		result->stat1_bits	= ReadStatusBits(result->status_1);
		result->data[0]		= MemRead(Bus,msg_addr + 4,CardId);
		break;
	 case 5 :
		result->loop_word	= MemRead(Bus,msg_addr + 2,CardId);
		result->status_1	= MemRead(Bus,msg_addr + 3,CardId);
		result->stat1_bits	= ReadStatusBits(result->status_1);
		break;
	 case 6 :
		temp = result->cmd1_bits.wcnt;
		if(temp == 0)
		   temp = 32;
		for (i=0;i<temp;i++)
		{
		   result->data[i]	= MemRead(Bus,msg_addr + 2 +i,CardId);
		}
		result->loop_word	= MemRead(Bus,msg_addr + 1 + temp + 1,CardId);
		result->status_1	= MemRead(Bus,msg_addr + 1 + temp + 2,CardId);
		break;
	 case 7 :
		result->loop_word	= MemRead(Bus,msg_addr + 2,CardId);
		result->status_1	= MemRead(Bus,msg_addr + 3,CardId);
		break;

	 case 8 :
		result->data[0]		= MemRead(Bus,msg_addr + 2 ,CardId);
		result->loop_word	= MemRead(Bus,msg_addr + 3,CardId);
		result->status_1	= MemRead(Bus,msg_addr + 4,CardId);
		break;

	 case 9 :
		result->cmd_1		= MemRead(Bus,msg_addr+1,CardId);
		result->cmd1_bits	= ReturnCmdFields(result->cmd_1);
		result->cmd_2		= MemRead(Bus,msg_addr + 2,CardId);
		result->cmd2_bits	= ReturnCmdFields(result->cmd_2);
		result->loop_word	= MemRead(Bus,msg_addr + 3,CardId);
		result->status_1	= MemRead(Bus,msg_addr +  4,CardId);
		result->stat1_bits	= ReadStatusBits(result->status_1);
		temp			= result->cmd1_bits.wcnt;
		if(temp == 0)
			temp = 32;
		for(i=0;i<temp;i++)
			result->data[i] = MemRead(Bus,msg_addr + 5 + i,CardId);
		result->status_2	= MemRead(Bus,msg_addr + 5 + i ,CardId );
		break;
	}  
	  return TRUE;
	}
	else
		return FALSE;
}

BSW_BITS ReadBswBits(USHORT blk_statwrd)
{
	BSW_BITS info;

	info.Invalid_Word			= 0x01 & blk_statwrd ;
	info.Sync_err				= 0x01 & blk_statwrd>>1 ;
	info.WordCnt_err			= 0x01 & blk_statwrd>>2 ;
	info.Incorrect_Status_Add		= 0x01 & blk_statwrd>>3 ;
	info.loop_tstfail			= 0x01 & blk_statwrd>>8 ;
	info.resp_timeout			= 0x01 & blk_statwrd>>9 ;
	info.msg_fmt_err			= 0x01 & blk_statwrd>>10 ;
	info.stat_wrd_err			= 0x01 & blk_statwrd>>11 ;
	info.err_flag				= 0x01 & blk_statwrd>>12 ;
	info.bus				= 0x01 & blk_statwrd>>13 ;
	info.som				= 0x01 & blk_statwrd>>14;
	info.eom				= 0x01 & blk_statwrd>>15;
	return(info);
}

STATUS_BITS ReadStatusBits(USHORT status)
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

MSG_DEF ReturnCntrlFields(USHORT control)
{
	MSG_DEF info;
	info.rt_rt		= control & 0x01 ;
	info.broadcast		= control>>1 & 0x01;
	info.mode_code		= control>>2 & 0x01;
	info.mask_bcst_bit	= control>>5 & 0x01;
	info.bus		= control>>7 & 0x01;
	return(info);
}
CMD ReturnCmdFields(USHORT command)
{
	CMD data;
	data.wcnt	=  0x001f & command;
	data.subaddr	= 0x001f & command>>5;
	data.t_r	= 0x0001 & command>>10;
	data.taddr	= 0x001f & command>>11;
	return(data);
}

CHAR ResetBc(UCHAR Bus,UCHAR CardId)
{
	BOOL	CardFlag	= FALSE;
	ULONG	STRSTR		= 0x0F;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		RegWrite(Bus,STRSTR,0x1,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR ConfigureRT(UCHAR Bus,UCHAR A_B,UCHAR Address[],UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		int i;
		ULONG CR5=0x25,LOOKUPA_B,Cmd_Stack_Pointer;
		UINT Desp_Stack_Addr,DataBlkPtr,RtCntrlRegDat;
		//USHORT 	RtSelReg1Dat=0,RtSelReg2Dat=0;//,RtSelReg1Add,RtSelReg2Add;
		ULONG RtCntrlRegAdd;
	
		/*IMR=0x26;*/
			
		RtCntrlRegAdd=0x200c;
		RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);	
			
		if(A_B==0)
		{
			LOOKUPA_B = 0x02040;	
			DataBlkPtr=0x02150;
			Desp_Stack_Addr = 0x01E00; 
			Cmd_Stack_Pointer = 0x02000;
			RtCntrlRegDat=(RtCntrlRegDat & 0x3ffe) | 0x8;
			MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
			MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
			for(i=0;i<64;i++)			
			{
				MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
				LOOKUPA_B = LOOKUPA_B + 1;
				DataBlkPtr = DataBlkPtr + 0x20;
			}
		}
		else
		{
			
			LOOKUPA_B = 0x020c0;	
			DataBlkPtr=0x02950;
			Desp_Stack_Addr = 0x03a00; 
			Cmd_Stack_Pointer = 0x02001;
			RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x9);
			MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
			MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
			
			for(i=0;i<64;i++)			
			{
				MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
				LOOKUPA_B = LOOKUPA_B + 1;
				DataBlkPtr = DataBlkPtr + 0x20;
			}			
		}
		
		
		if(Address[1]==1)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x0002;
			RtCntrlRegAdd=0x3e0c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x03e40;	
				DataBlkPtr=0x03f50;
				Desp_Stack_Addr = 0x03c00; 
				Cmd_Stack_Pointer = 0x03e00;
				RtCntrlRegDat=(RtCntrlRegDat & 0x3ffe) | 0x8;
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x03ec0;	
				DataBlkPtr=0x04750;
				Desp_Stack_Addr = 0x05800; 
				Cmd_Stack_Pointer = 0x03e01;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x9);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
			
				for(i=0;i<64;i++)			
					{
						MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
						LOOKUPA_B = LOOKUPA_B + 1;
						DataBlkPtr = DataBlkPtr + 0x20;
					}			
			}
		}

		if(Address[2]==2)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x0004;
			RtCntrlRegAdd=0x5c0c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x05c40;	
				DataBlkPtr=0x05d50;
				Desp_Stack_Addr = 0x05a00; 
				Cmd_Stack_Pointer = 0x05c00;
				RtCntrlRegDat=(RtCntrlRegDat & 0x3ffe) | 0x8;
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x05cc0;	
				DataBlkPtr=0x06550;
				Desp_Stack_Addr = 0x07600; 
				Cmd_Stack_Pointer = 0x05c01;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x9);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
					for(i=0;i<64;i++)			
					{
						MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
						LOOKUPA_B = LOOKUPA_B + 1;
						DataBlkPtr = DataBlkPtr + 0x20;
					}			
			}
		}

		if(Address[3]==3)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x0008;
			RtCntrlRegAdd=0x7a0c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x07a40;	
				DataBlkPtr=0x07b50;
				Desp_Stack_Addr = 0x07800; 
				Cmd_Stack_Pointer = 0x07a00;
				RtCntrlRegDat=(RtCntrlRegDat & 0x3ffe) | 0x8;
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x07ac0;	
				DataBlkPtr=0x08350;
				Desp_Stack_Addr = 0x09400; 
				Cmd_Stack_Pointer = 0x07a01;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x9);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
					for(i=0;i<64;i++)			
					{
						MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
						LOOKUPA_B = LOOKUPA_B + 1;
						DataBlkPtr = DataBlkPtr + 0x20;
					}			
			}
		}

		if(Address[4]==4)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x0010;
			RtCntrlRegAdd=0x980c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x09840;	
				DataBlkPtr=0x09950;
				Desp_Stack_Addr = 0x09600; 
				Cmd_Stack_Pointer = 0x09800;
				RtCntrlRegDat=(RtCntrlRegDat & 0x3ffe) | 0x8;
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x098c0;	
				DataBlkPtr=0x0a150;
				Desp_Stack_Addr = 0x0b200; 
				Cmd_Stack_Pointer = 0x09801;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x9);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
					for(i=0;i<64;i++)			
					{
						MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
						LOOKUPA_B = LOOKUPA_B + 1;
						DataBlkPtr = DataBlkPtr + 0x20;
					}			
			}
		}

		if(Address[5]==5)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x0020;
			RtCntrlRegAdd=0xb60c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x0b640;	
				DataBlkPtr=0x0b750;
				Desp_Stack_Addr = 0x0b400; 
				Cmd_Stack_Pointer = 0x0b600;
				RtCntrlRegDat=(RtCntrlRegDat & 0x3ffe) | 0x8;
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x0b6c0;	
				DataBlkPtr=0x0bf50;
				Desp_Stack_Addr = 0x0d000; 
				Cmd_Stack_Pointer = 0x0b601;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x9);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
					for(i=0;i<64;i++)			
					{
						MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
						LOOKUPA_B = LOOKUPA_B + 1;
						DataBlkPtr = DataBlkPtr + 0x20;
					}			
			}
		}
	
		if(Address[6]==6)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x0040;
			RtCntrlRegAdd=0xd40c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x0d440;	
				DataBlkPtr=0x0d550;
				Desp_Stack_Addr = 0x0d200; 
				Cmd_Stack_Pointer = 0x0d400;
				RtCntrlRegDat=(RtCntrlRegDat & 0x3ffe) | 0x8;
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x0d4c0;	
				DataBlkPtr=0x0dd50;
				Desp_Stack_Addr = 0x0ee00; 
				Cmd_Stack_Pointer = 0x0d401;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x9);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
					for(i=0;i<64;i++)			
					{
						MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
						LOOKUPA_B = LOOKUPA_B + 1;
						DataBlkPtr = DataBlkPtr + 0x20;
					}			
			}
		}
	
		if(Address[7]==7)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x0080;
			RtCntrlRegAdd=0xf20c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x0f240;	
				DataBlkPtr=0x0f350;
				Desp_Stack_Addr = 0x0f000; 
				Cmd_Stack_Pointer = 0x0f200;
				RtCntrlRegDat=(RtCntrlRegDat & 0x3ffe) | 0x8;
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x0f2c0;	
				DataBlkPtr=0x0fb50;
				Desp_Stack_Addr = 0x10c00; 
				Cmd_Stack_Pointer = 0x0f201;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
					
				}
				
			}
		}
	
		if(Address[8]==8)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x0100;
			RtCntrlRegAdd=0x1100c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x11040;	
				DataBlkPtr=0x11150;
				Desp_Stack_Addr = 0x10e00; 
				Cmd_Stack_Pointer = 0x11000;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x110c0;	
				DataBlkPtr=0x11950;
				Desp_Stack_Addr = 0x12a00; 
				Cmd_Stack_Pointer = 0x11001;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
					for(i=0;i<64;i++)			
					{
						MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
						LOOKUPA_B = LOOKUPA_B + 1;
						DataBlkPtr = DataBlkPtr + 0x20;
					}			
			}
		}
	
		if(Address[9]==9)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x0200;
			RtCntrlRegAdd=0x12e0c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x12e40;	
				DataBlkPtr=0x12f50;
				Desp_Stack_Addr = 0x12c00; 
				Cmd_Stack_Pointer = 0x12e00;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x12ec0;	
				DataBlkPtr=0x13750;
				Desp_Stack_Addr = 0x14800; 
				Cmd_Stack_Pointer = 0x12e01;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
					for(i=0;i<64;i++)			
					{
						MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
						LOOKUPA_B = LOOKUPA_B + 1;
						DataBlkPtr = DataBlkPtr + 0x20;
					}			
			}
		}
	
		if(Address[10]==10)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x0400;
			RtCntrlRegAdd=0x14c0c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x14c40;	
				DataBlkPtr=0x14d50;
				Desp_Stack_Addr = 0x14a00; 
				Cmd_Stack_Pointer = 0x14c00;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x14cc0;	
				DataBlkPtr=0x15550;
				Desp_Stack_Addr = 0x16600; 
				Cmd_Stack_Pointer = 0x14c01;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[11]==11)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x0800;
			RtCntrlRegAdd=0x16a0c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x16a40;	
				DataBlkPtr=0x16b50;
				Desp_Stack_Addr = 0x16800; 
				Cmd_Stack_Pointer = 0x16a00;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x16ac0;	
				DataBlkPtr=0x17350;
				Desp_Stack_Addr = 0x18400; 
				Cmd_Stack_Pointer = 0x16a01;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	

		if(Address[12]==12)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x1000;
			RtCntrlRegAdd=0x1880c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x18840;	
				DataBlkPtr=0x18950;
				Desp_Stack_Addr = 0x18600; 
				Cmd_Stack_Pointer = 0x18800;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x188c0;	
				DataBlkPtr=0x19150;
				Desp_Stack_Addr = 0x1a200; 
				Cmd_Stack_Pointer = 0x18801;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[13]==13)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x2000;
			RtCntrlRegAdd=0x1a60c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x1a640;	
				DataBlkPtr=0x1a750;
				Desp_Stack_Addr = 0x1a400; 
				Cmd_Stack_Pointer = 0x1a600;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x1a6c0;	
				DataBlkPtr=0x1af50;
				Desp_Stack_Addr = 0x1c000; 
				Cmd_Stack_Pointer = 0x1a601;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackB
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackB
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[14]==14)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x4000;
			RtCntrlRegAdd=0x1c40c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x1c440;	
				DataBlkPtr=0x1c550;
				Desp_Stack_Addr = 0x1c200; 
				Cmd_Stack_Pointer = 0x1c400;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x1c4c0;	
				DataBlkPtr=0x1cd50;
				Desp_Stack_Addr = 0x1de00; 
				Cmd_Stack_Pointer = 0x1c401;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[15]==15)
		{
			//RtSelReg1Dat=RtSelReg1Dat|0x8000;
			RtCntrlRegAdd=0x1e20c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x1e240;	
				DataBlkPtr=0x1e350;
				Desp_Stack_Addr = 0x1e000; 
				Cmd_Stack_Pointer = 0x1e200;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x1e2c0;	
				DataBlkPtr=0x1eb50;
				Desp_Stack_Addr = 0x1fc00; 
				Cmd_Stack_Pointer = 0x1e201;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[16]==16)
		{
			//RtSelReg2Dat=0x0001;
			RtCntrlRegAdd=0x2000c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x20040;	
				DataBlkPtr=0x20150;
				Desp_Stack_Addr = 0x1fe00; 
				Cmd_Stack_Pointer = 0x20000;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x4008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x200c0;	
				DataBlkPtr=0x20950;
				Desp_Stack_Addr = 0x21a00; 
				Cmd_Stack_Pointer = 0x20001;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[17]==17)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x0002;
			RtCntrlRegAdd=0x21e0c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x21e40;	
				DataBlkPtr=0x21f50;
				Desp_Stack_Addr = 0x21c00; 
				Cmd_Stack_Pointer = 0x21e00;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x21ec0;	
				DataBlkPtr=0x22750;
				Desp_Stack_Addr = 0x23800; 
				Cmd_Stack_Pointer = 0x21e01;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[18]==18)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x0004;
			RtCntrlRegAdd=0x23c0c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x23c40;	
				DataBlkPtr=0x23d50;
				Desp_Stack_Addr = 0x23a00; 
				Cmd_Stack_Pointer = 0x23c00;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x23cc0;	
				DataBlkPtr=0x24550;
				Desp_Stack_Addr = 0x25600; 
				Cmd_Stack_Pointer = 0x23c01;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[19]==19)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x0008;
			RtCntrlRegAdd=0x25a0c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x25a40;	
				DataBlkPtr=0x25b50;
				Desp_Stack_Addr = 0x25800; 
				Cmd_Stack_Pointer = 0x25a00;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x25ac0;	
				DataBlkPtr=0x26350;
				Desp_Stack_Addr = 0x27400; 
				Cmd_Stack_Pointer = 0x25a01;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[20]==20)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x0010;
			RtCntrlRegAdd=0x2780c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x27840;	
				DataBlkPtr=0x27950;
				Desp_Stack_Addr = 0x27600; 
				Cmd_Stack_Pointer = 0x27800;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x278c0;	
				DataBlkPtr=0x28150;
				Desp_Stack_Addr = 0x29200; 
				Cmd_Stack_Pointer = 0x27801;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[21]==21)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x0020;
			RtCntrlRegAdd=0x2960c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x29640;	
				DataBlkPtr=0x29750;
				Desp_Stack_Addr = 0x29400; 
				Cmd_Stack_Pointer = 0x29600;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x296c0;	
				DataBlkPtr=0x29f50;
				Desp_Stack_Addr = 0x2b000; 
				Cmd_Stack_Pointer = 0x29601;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[22]==22)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x0040;
			RtCntrlRegAdd=0x2b40c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x2b440;	
				DataBlkPtr=0x2b550;
				Desp_Stack_Addr = 0x2b200; 
				Cmd_Stack_Pointer = 0x2b400;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x2b4c0;	
				DataBlkPtr=0x2bd50;
				Desp_Stack_Addr = 0x2ce00; 
				Cmd_Stack_Pointer = 0x2b401;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[23]==23)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x0080;
			RtCntrlRegAdd=0x2d20c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x2d240;	
				DataBlkPtr=0x2d350;
				Desp_Stack_Addr = 0x2d000; 
				Cmd_Stack_Pointer = 0x2d200;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x2d2c0;	
				DataBlkPtr=0x2db50;
				Desp_Stack_Addr = 0x2ec00; 
				Cmd_Stack_Pointer = 0x2d201;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
						for(i=0;i<64;i++)			
					{
						MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
						LOOKUPA_B = LOOKUPA_B + 1;
						DataBlkPtr = DataBlkPtr + 0x20;
					}			
			}
		}
	
		if(Address[24]==24)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x0100;
			RtCntrlRegAdd=0x2f00c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x2f040;	
				DataBlkPtr=0x2f150;
				Desp_Stack_Addr = 0x2ee00; 
				Cmd_Stack_Pointer = 0x2f000;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0x8008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x2f0c0;	
				DataBlkPtr=0x2f950;
				Desp_Stack_Addr = 0x30a00; 
				Cmd_Stack_Pointer = 0x2f001;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
		if(Address[25]==25)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x0200;
			RtCntrlRegAdd=0x30e0c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x30e40;	
				DataBlkPtr=0x30f50;
				Desp_Stack_Addr = 0x30c00; 
				Cmd_Stack_Pointer = 0x30e00;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x30ec0;	
				DataBlkPtr=0x31750;
				Desp_Stack_Addr = 0x32800; 
				Cmd_Stack_Pointer = 0x30e01;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[26]==26)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x0400;
			RtCntrlRegAdd=0x32c0c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x32c40;	
				DataBlkPtr=0x32d50;
				Desp_Stack_Addr = 0x32a00; 
				Cmd_Stack_Pointer = 0x32c00;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x32cc0;	
				DataBlkPtr=0x33550;
				Desp_Stack_Addr = 0x34600; 
				Cmd_Stack_Pointer = 0x32c01;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[27]==27)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x0800;
			RtCntrlRegAdd=0x34a0c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x34a40;	
				DataBlkPtr=0x34b50;
				Desp_Stack_Addr = 0x34800; 
				Cmd_Stack_Pointer = 0x34a00;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x34ac0;	
				DataBlkPtr=0x35350;
				Desp_Stack_Addr = 0x36400; 
				Cmd_Stack_Pointer = 0x34a01;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[28]==28)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x1000;
			RtCntrlRegAdd=0x3680c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x36840;	
				DataBlkPtr=0x36950;
				Desp_Stack_Addr = 0x36600; 
				Cmd_Stack_Pointer = 0x36800;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x368c0;	
				DataBlkPtr=0x37150;
				Desp_Stack_Addr = 0x38200; 
				Cmd_Stack_Pointer = 0x36801;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[29]==29)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x2000;
			RtCntrlRegAdd=0x3860c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x38640;	
				DataBlkPtr=0x38750;
				Desp_Stack_Addr = 0x38400; 
				Cmd_Stack_Pointer = 0x38600;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x386c0;	
				DataBlkPtr=0x38f50;
				Desp_Stack_Addr = 0x3a000; 
				Cmd_Stack_Pointer = 0x38601;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		if(Address[30]==30)
		{
			//RtSelReg2Dat=RtSelReg2Dat|0x4000;
			RtCntrlRegAdd=0x3a40c;
			RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
			if(A_B==0)
			{
				LOOKUPA_B = 0x3a440;	
				DataBlkPtr=0x3a550;
				Desp_Stack_Addr = 0x3a200; 
				Cmd_Stack_Pointer = 0x3a400;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc008);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}
			}
			else 
			{
				LOOKUPA_B = 0x3a4c0;	
				DataBlkPtr=0x3ad50;
				Desp_Stack_Addr = 0x3be00; 
				Cmd_Stack_Pointer = 0x3a401;
				RtCntrlRegDat=((RtCntrlRegDat & 0x3ffe)|0xc009);
				MemWrite(Bus,RtCntrlRegAdd,RtCntrlRegDat,CardId);//StackA
				MemWrite(Bus,Cmd_Stack_Pointer,Desp_Stack_Addr,CardId);//StackA
				for(i=0;i<64;i++)			
				{
					MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);
					LOOKUPA_B = LOOKUPA_B + 1;
					DataBlkPtr = DataBlkPtr + 0x20;
				}			
			}
		}
	
		//MemWrite(Bus,RtSelReg1Add,RtSelReg1Dat);
		//MemWrite(Bus,RtSelReg2Add,RtSelReg2Dat);
		return TRUE;
	}
	else
		return FALSE;
	
}

CHAR RunRt(UCHAR Bus,UCHAR RtStack_A_B,UCHAR Address[],UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		ULONG CR1=0x2D;
		USHORT m,RtAddrs = 0x0;
		for(m=0;m<=15;m++)
		{
			if(Address[m] == m)
				RtAddrs = RtAddrs | (1 << m);
		}
		MemWrite(Bus,0x200A,RtAddrs,CardId);
		RtAddrs = 0x0;
		for(m=16;m<=31;m++)
		{
			if(Address[m] == m)
				RtAddrs = RtAddrs | (1 << (m - 16));
		}
		MemWrite(Bus,0x200B,RtAddrs,CardId);

		if(RtStack_A_B == 0)
			RegWrite(Bus,CR1,0x0002,CardId);
		else if(RtStack_A_B == 1)
			RegWrite(Bus,CR1,0x2002,CardId);
		else
			return (-9);

		return TRUE;
	}
	else
		return FALSE;
}

CHAR SetSrvReq(UCHAR Bus,UCHAR RtNo,UCHAR Sr,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	RtCntrlRegAdd;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);

		if(((char)RtNo>=0) && (RtNo <31))
		{
			RtCntrlRegAdd=((RtNo*0x1e00)+0x200c);
			if(Sr==1)
		        	MemWrite(Bus,RtCntrlRegAdd,(MemRead(Bus,RtCntrlRegAdd,CardId)|0x0040),CardId);
		 	else if(Sr==0)
 		   		MemWrite(Bus,RtCntrlRegAdd,(MemRead(Bus,RtCntrlRegAdd,CardId)&0xffbf),CardId);
		  	else
				return (-25);
		return TRUE;
		}
		else
			return (-23);
	}
	else
		return FALSE;
}

CHAR SetTrFlag(UCHAR Bus,UCHAR RtNo,UCHAR Tr,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	RtCntrlRegAdd;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);

		if(((char)RtNo>=0) && (RtNo <31))
		{
			RtCntrlRegAdd=((RtNo*0x1e00)+0x200c);
		   if(Tr==1)
			MemWrite(Bus,RtCntrlRegAdd,(MemRead(Bus
				,RtCntrlRegAdd,CardId)|0x0100),CardId);
		   else if(Tr==0)
			MemWrite(Bus,RtCntrlRegAdd,(MemRead(Bus
				,RtCntrlRegAdd,CardId)&0xFEFF),CardId);
		    else				
			return (-25);
		return TRUE;
		}
		else
			return (-23);
	}
	else
		return FALSE;
}

CHAR SetSubSys(UCHAR Bus,UCHAR RtNo,UCHAR SSFlag,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	RtCntrlRegAdd;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		
		if(((char)RtNo>=0) && (RtNo <31))
		{
			RtCntrlRegAdd=((RtNo*0x1e00)+0x200c);
			if(SSFlag==1)
			    MemWrite(Bus,RtCntrlRegAdd,(MemRead(Bus,RtCntrlRegAdd,CardId)|0x0080),CardId);
			else if(SSFlag==0)
			    MemWrite(Bus,RtCntrlRegAdd,(MemRead(Bus,RtCntrlRegAdd,CardId)&0xff7f),CardId);
			else
			     return (-25);	
			return TRUE;
		}
		else
			return (-23);
	}
	else
		return FALSE;
}

CHAR SetBusy(UCHAR Bus,UCHAR RtNo,UCHAR BFlag,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	RtCntrlRegAdd;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		
	   if(((char)RtNo>=0) && (RtNo <31))
	   {
			RtCntrlRegAdd=((RtNo*0x1e00)+0x200c);
		if(BFlag==1)
			MemWrite(Bus,RtCntrlRegAdd,(MemRead(Bus,RtCntrlRegAdd,CardId)|0x0020),CardId);
		else if(BFlag==0)
			MemWrite(Bus,RtCntrlRegAdd,(MemRead(Bus,RtCntrlRegAdd,CardId)&0xffdf),CardId);
		else
			return (-25);	
		return TRUE;
	   }
	   else
		return (-23);
	}	
	else
		return FALSE;
}

CHAR SetDBA(UCHAR Bus,UCHAR RtNo,UCHAR DBAFlag,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	RtCntrlRegAdd;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		
		if(((char)RtNo>=0) && (RtNo <31))
		{
			RtCntrlRegAdd=((RtNo*0x1e00)+0x200c);
			if(DBAFlag==1)
			   MemWrite(Bus,RtCntrlRegAdd,(MemRead(Bus,RtCntrlRegAdd,CardId)&0xfff7),CardId);
			else if(DBAFlag==0)
			   MemWrite(Bus,RtCntrlRegAdd,(MemRead(Bus,RtCntrlRegAdd,CardId)|0x0008),CardId);
			else
				return (-25);
		 return TRUE;
		}
		else
			return (-23);
	}
	else
		return FALSE;
}

CHAR ClrTTROnSync(UCHAR Bus,UCHAR RtNo,UCHAR TTRFlag,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	RtCntrlRegAdd;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		
		if(((char)RtNo>=0) && ((char)RtNo <31))
		{
			RtCntrlRegAdd=((RtNo*0x1e00)+0x200c);
		   if(TTRFlag==1)
			MemWrite(Bus,RtCntrlRegAdd,(MemRead(Bus
				,RtCntrlRegAdd,CardId)|0x0010),CardId);
		   else  if(TTRFlag==0)
			MemWrite(Bus,RtCntrlRegAdd,(MemRead(Bus
				,RtCntrlRegAdd,CardId)&0xffef),CardId);
		   else
			return(-25);	
		return TRUE;
		}
		else
			return (-23);
	}
	else
		return FALSE;
}

CHAR LoadTTROnSync(UCHAR Bus,UCHAR RtNo,UCHAR TTRFlag,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	USHORT	CR2=0x2F,Temp;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(((char)RtNo>=0) && ((char)RtNo <31))
		{
			Temp = RegRead(Bus,CR2,CardId);
			if(TTRFlag==1)
				RegWrite(Bus,CR2,(Temp | 0x20),CardId);
			else if(TTRFlag==0)
				RegWrite(Bus,CR2,(Temp & 0xFFDF),CardId);
			else
			    return (-25);	
			return TRUE;
		}
		else
			return (-23);
	}	
	else
		return FALSE;
	
}

CHAR WriteDataBlock(UCHAR Bus,UCHAR RtNo,UCHAR SubAddr,USHORT data_buff[],UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	USHORT	i;
	ULONG	LOOKUP_AB,DataBlkPtr;
	ULONG	RtCntrlRegAdd,RtCntrlRegDat;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);

		if(((char)RtNo<0) || ((char)RtNo>30))
			return (-23);

		if(SubAddr<1 || SubAddr>30)
			return (-13);

		RtCntrlRegAdd=((RtNo*0x1e00)+0x200c);
		RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
	
		if( (RtCntrlRegDat & 0x0001)==0x0001)
		{
			LOOKUP_AB = 0x020E0;	
			LOOKUP_AB=((RtNo*0x1e00)+LOOKUP_AB);
			DataBlkPtr = MemRead(Bus,LOOKUP_AB+SubAddr,CardId);
			RtCntrlRegAdd=RtCntrlRegAdd & 0x30000;
			if(RtNo == 7 && SubAddr >=6)
				RtCntrlRegAdd = 0x10000;
		}
		else
		{
			LOOKUP_AB = 0x02060;		
			LOOKUP_AB=((RtNo*0x1e00)+LOOKUP_AB);
			DataBlkPtr = MemRead(Bus,LOOKUP_AB+SubAddr,CardId);
			RtCntrlRegAdd=RtCntrlRegAdd & 0x30000;
		}
		DataBlkPtr=DataBlkPtr|RtCntrlRegAdd;
		for (i=0;i<32;i++)
			MemWrite(Bus,(DataBlkPtr + i),data_buff[i],CardId);
	
		return TRUE;
	}
	else
		return FALSE;
}

/*CHAR ReadMsgRT(UCHAR Bus,UCHAR MsgNo,UCHAR RtNo,RT_MSG *message,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	USHORT	i;
	UINT	Desp_Stack_Addr,offset,temp;
	ULONG	RtCntrlRegAdd,RtCntrlRegDat;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(RtNo<0 || RtNo>30)
			return (-23);
		if((MsgNo >128) && (MsgNo <1))
			return (-10);

		RtCntrlRegAdd=((RtNo*0x1e00)+0x200c);
		RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
		
		if( (RtCntrlRegDat & 0x0001)==0x0001)
			  Desp_Stack_Addr=((RtNo*0x1e00)+0x3a00);
		else
			  Desp_Stack_Addr=((RtNo*0x1e00)+0x01e00);
		
		message->bsw = MemRead(Bus,Desp_Stack_Addr + ((MsgNo * 4)-4),CardId);

		//if(((message->bsw)& 0xC000)==0xC000)
		if((message->bsw >= 0xDE00) && (message->bsw < 0xFC00)) 
                                                        ///Added for Bcst by mahi on 13-05-2k6
		{
				 
			//temp = MemRead(Bus,Desp_Stack_Addr + ((MsgNo * 4) - 3),CardId);
			temp = message->bsw & 0x3FFF;

			message->bsw = MemRead(Bus,temp,CardId);
			message->time_tag  = MemRead(Bus,temp+1,CardId);
			offset = MemRead(Bus,temp+2,CardId);
			message->cmd = MemRead(Bus,Desp_Stack_Addr + 
						((MsgNo * 4) - 1),CardId);
			message->wcnt    = (message->cmd) & 0x1f;
			message->subaddr = (message->cmd>>5) & 0x1f;
			message->t_r     = (message->cmd>>10) & 0x01;
		
			temp	 = message->wcnt;
			if(temp == 0)
				temp = 32;
			for (i=0;i<temp;i++)
			message->data[i] = MemRead(Bus,offset+i,CardId);
			return TRUE;
		}
		message->time_tag  = MemRead(Bus,Desp_Stack_Addr + 
						((MsgNo * 4)-3),CardId);
		message->cmd = MemRead(Bus,Desp_Stack_Addr + 
						    ((MsgNo * 4)-1),CardId);
		message->wcnt    = (message->cmd) & 0x1f;
		message->subaddr = (message->cmd>>5) & 0x1f;
		message->t_r     = (message->cmd>>10) & 0x01;
		
		offset = MemRead(Bus,Desp_Stack_Addr + 
						((MsgNo * 4)-2),CardId);
		if( (RtCntrlRegDat & 0x0001)==0x0001)
		{
			if(RtNo == 7 && message->subaddr >= 6 && 
							message->t_r ==1)
				offset = 0x10000 | offset;
			else
				offset=((RtCntrlRegAdd & 0x30000)|offset);
		}
		else
			offset=((RtCntrlRegAdd & 0x30000)|offset);
		temp = message->wcnt;
		if(temp == 0)
			temp = 32;
		for (i=0;i<temp;i++)
			message->data[i] = MemRead(Bus,offset+i,CardId);
			
		return TRUE;
	}
	else
		return FALSE;
}*/

CHAR ReadMsgRT(UCHAR Bus,UCHAR MsgNo,UCHAR RtNo,RT_MSG *message,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		USHORT i,CirSub;
		UINT Desp_Stack_Addr,offset,temp;
		ULONG RtCntrlRegAdd,RtCntrlRegDat,CmdStackPtr,RxCBBase,TxCBBase,CbTemp=0xF0000;
		BOOL CirSubFlag = FALSE;

		if(((char)RtNo<0) || ((char)RtNo>30))
			return (-23);
		if((MsgNo > 128) || (MsgNo <1))
			return (-10);

		RtCntrlRegAdd=((RtNo*0x1e00)+0x200c);
		RtCntrlRegDat=MemRead(Bus,RtCntrlRegAdd,CardId);
		
		if( (RtCntrlRegDat & 0x0001)==0x0001)
			  Desp_Stack_Addr=((RtNo*0x1e00)+0x3a00);
		else
			  Desp_Stack_Addr=((RtNo*0x1e00)+0x01e00);
		
		message->bsw = MemRead(Bus,Desp_Stack_Addr + ((MsgNo * 4)-4),CardId);

		//if(((message->bsw)& 0xC000)==0xC000)
		if((message->bsw >= 0xDE00) && (message->bsw < 0xFC00))   ///Added for Bcst by mahi on 13-05-2k6
		{
			
			//temp = MemRead(Bus,Desp_Stack_Addr + ((MsgNo * 4) - 3),CardId);
			temp = message->bsw & 0x3FFF;

			message->bsw = MemRead(Bus,temp,CardId);
			message->time_tag  = MemRead(Bus,temp+1,CardId);
			offset = MemRead(Bus,temp+2,CardId);
			message->cmd = MemRead(Bus,Desp_Stack_Addr + ((MsgNo * 4) - 1),CardId);
			//message->cmd = MemRead(Bus,temp+3,CardId);
			message->wcnt    = (message->cmd) & 0x1f;
			message->subaddr = (message->cmd>>5) & 0x1f;
			message->t_r     = (message->cmd>>10) & 0x01;
		
			temp	 = message->wcnt;
			if(temp == 0)
				temp = 32;
			for (i=0;i<temp;i++)
				message->data[i] = MemRead(Bus,offset+i,CardId);
			return TRUE;
		}
		message->time_tag  = MemRead(Bus,Desp_Stack_Addr + ((MsgNo * 4)-3),CardId);
		message->cmd = MemRead(Bus,Desp_Stack_Addr + ((MsgNo * 4)-1),CardId);
		message->wcnt    = (message->cmd) & 0x1f;
		message->subaddr = (message->cmd>>5) & 0x1f;
		message->t_r     = (message->cmd>>10) & 0x01;

///////////////////////////////// ADDED  by Shaheen on 11-07-2k8 //////////

		if(message->subaddr > 0 && message->subaddr < 16)
		{
			CmdStackPtr = 0x2002 + (RtNo * 0x1E00);
			CirSub = MemRead(Bus,CmdStackPtr,CardId);			
			if(((CirSub >> message->subaddr) & 0x1) == 0x1)
				CirSubFlag = TRUE;	
		}
		else if(message->subaddr > 15 && message->subaddr < 31)
		{
			CmdStackPtr = 0x2003 + (RtNo * 0x1E00);
			CirSub = MemRead(Bus,CmdStackPtr,CardId);
			if(((CirSub >> (message->subaddr-16)) & 0x1) == 0x1)
				CirSubFlag = TRUE;	
		}
		offset = MemRead(Bus,Desp_Stack_Addr + ((MsgNo * 4)-2),CardId);
		if(CirSubFlag == TRUE)
		{				
			if(message->t_r == 0)
			{
				RxCBBase = MulRtCircSubRxBase[RtNo][message->subaddr] & CbTemp;
				offset = offset +  RxCBBase;
			}
			else
			{
				TxCBBase = MulRtCircSubTxBase[RtNo][message->subaddr] & CbTemp;
				offset = offset + TxCBBase;
			}
		}
		else
		{
			if( (RtCntrlRegDat & 0x0001)==0x0001)
			{
				if(RtNo == 7 && message->subaddr >= 6 && message->t_r ==1)
					offset = 0x10000 | offset;
				else
					offset=((RtCntrlRegAdd & 0x30000)|offset);
			}
			else
				offset=((RtCntrlRegAdd & 0x30000)|offset);
		}			

		temp = message->wcnt;
		if(temp == 0)
			temp = 32;
		for (i=0;i<temp;i++)
			message->data[i] = MemRead(Bus,offset+i,CardId);
			
		 return TRUE;
	}
	else
		return FALSE;
}


CHAR ConfigureRTCB(UCHAR Bus,USHORT RtAddr,USHORT BlockSize,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	CircBuff_Ptr,Init_CircBuff_Ptr,End_CircBuff_Ptr,CntrlRegAddr;
	USHORT	End_Offset,Start_Offset;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);

		if(((short)RtAddr<0) || ((short)RtAddr>30))
			return (-23);
		
		CntrlRegAddr = (RtAddr * 0x1e00) + 0x200C;
		MemWrite(Bus,CntrlRegAddr,(MemRead(Bus,CntrlRegAddr,CardId)
						 | 0x0400),CardId);
		
		if(RtAddr == 7 || RtAddr == 24)
			End_Offset = 0x0FFFF;
		else
			End_Offset = ((RtAddr * 0x1e00) + 0x397F) & 0x0FFFF;
						
		Start_Offset = End_Offset - (BlockSize + 2);

		//RECEIVE
		CircBuff_Ptr = (RtAddr * 0x1e00) + 0x2002;
		Init_CircBuff_Ptr = (RtAddr * 0x1e00) + 0x2008;
		End_CircBuff_Ptr = (RtAddr * 0x1e00) + 0x2010;
		
		MemWrite(Bus,CircBuff_Ptr,Start_Offset,CardId);
		MemWrite(Bus,Init_CircBuff_Ptr,Start_Offset,CardId);
		MemWrite(Bus,End_CircBuff_Ptr,End_Offset,CardId);

		//TRANSMIT
		CircBuff_Ptr = (RtAddr * 0x1e00) + 0x2003;
		Init_CircBuff_Ptr = (RtAddr * 0x1e00) + 0x2009;
		End_CircBuff_Ptr = (RtAddr * 0x1e00) + 0x2011;
		
		MemWrite(Bus,CircBuff_Ptr,Start_Offset,CardId);
		MemWrite(Bus,Init_CircBuff_Ptr,Start_Offset,CardId);
		MemWrite(Bus,End_CircBuff_Ptr,End_Offset,CardId);

		return TRUE;
	}
	else
		return FALSE;
}


CHAR IllegalizeAll(UCHAR Bus,UCHAR RtNo,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	int	i;
	ULONG	Offset;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		
		Offset = (RtNo * 0x1e00) + 0x3980;
		for(i=0;i<128;i++) 
		{
			MemWrite(Bus,(Offset + i),0xFFFF,CardId); 
						//Illegal ALL commands
		}
		return TRUE;
	}
	else
		return FALSE;
}

CHAR LegalizeAll(UCHAR Bus,UCHAR RtNo,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	int	i;
	ULONG	Offset;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		Offset = (RtNo * 0x1e00) + 0x3980;
		for(i=0;i<128;i++) 
		{
			MemWrite(Bus,(Offset + i),0x0,CardId); 
					//Legal ALL commands
		}
		return TRUE;
	}
	else
		return FALSE;
}

CHAR IllegalizeCmd(UCHAR Bus,CMD_WRD *temp,UCHAR CardId) 
{
	BOOL	CardFlag = FALSE;
	USHORT	val,shift;
	ULONG	offset;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		
		offset = (temp->cmd.taddr * 0x1e00) + 0x3980;
		val = (temp->wrd & 0x7FF)>> 4;
		shift = temp->wrd & 0x0f;
		offset = val | offset;
		val = 0x0001 << shift;
		shift = MemRead(Bus,offset,CardId);
		val = val | shift;
		MemWrite(Bus,offset,val,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR LegalizeCmd(UCHAR Bus,CMD_WRD *temp,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	USHORT	val,shift;
	ULONG	offset;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		offset = (temp->cmd.taddr * 0x1e00) + 0x3980;
		val = (temp->wrd & 0x07ff) >> 4;
		shift = temp->wrd & 0x0f;
		offset = val + offset;
		val = ~(0x0001 << shift);
		shift = MemRead(Bus,offset,CardId);
		val = val & shift;
		MemWrite(Bus,offset,val,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR BcTimeTagReset(UCHAR Bus,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	STRSTR=0x0F;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		RegWrite(Bus,STRSTR,0x04,CardId);
		RegWrite(Bus,STRSTR,0x0,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR RtTimeTagReset(UCHAR Bus,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	STRSTR=0x2F;
	USHORT	RegValue;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		RegValue = RegRead(Bus,STRSTR,CardId);
		RegWrite(Bus,STRSTR,RegValue | 0x0400,CardId);
		RegWrite(Bus,STRSTR,RegValue,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR MtTimeTagReset(UCHAR Bus,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	STRSTR=0x1F;
	USHORT	RegValue;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		RegValue = RegRead(Bus,STRSTR,CardId);
		RegWrite(Bus,STRSTR,RegValue & 0xFFFB,CardId);
		RegWrite(Bus,STRSTR,RegValue,CardId);
		return TRUE;
	}
	else
		return FALSE;
}
CHAR BcTTRResolution(UCHAR Bus,USHORT TimeRes,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	USHORT	RegValue;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		if(((short)TimeRes < 0) || ((short)TimeRes > 7))
			return (-27);

		TimeRes = TimeRes << 9;
		RegValue = RegRead(Bus,0x02,CardId);
		RegValue = (RegValue & 0xF1FF) | TimeRes;
		RegWrite(Bus,0x02,RegValue,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR RtTTRResolution(UCHAR Bus,USHORT TimeRes,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	USHORT	RegValue;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		if(((short)TimeRes < 0) || ((short)TimeRes > 7))
			return (-27);
		
		TimeRes = TimeRes << 7;
		RegValue = RegRead(Bus,0x2F,CardId);
		RegValue = (RegValue & 0xFC7F) | TimeRes;
		RegWrite(Bus,0x2F,RegValue,CardId);
		return TRUE;
	}
	else
		return FALSE;
}
CHAR MtTTRResolution(UCHAR Bus,USHORT TimeRes,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	USHORT	RegValue;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		if(((short)TimeRes < 0) || ((short)TimeRes > 7))
			return (-27);

		TimeRes = TimeRes << 7;
		RegValue = RegRead(Bus,0x12,CardId);
		RegValue = (RegValue & 0xFC7F) | TimeRes;
		RegWrite(Bus,0x12,RegValue,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR SelMsgMt(UCHAR Bus,UCHAR MTSTACK_A_B,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	USHORT	i;
	ULONG	CR1=0x13,CR3=0x17;


	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		MemWrite(Bus,0x3C080,0xC084,CardId);
					//Descriptor STACKA/CMD STACKA
		MemWrite(Bus,0x3C081,0xCB19,CardId);//Data Stack PtrA(DBPTRA)
		MemWrite(Bus,0x3C082,0xE042,CardId);
					//Descriptor STACKB/CMD STACKB
		MemWrite(Bus,0x3C083,0xEAD7,CardId);//Data Stack PtrB(DBPTRB)
				
		if(MTSTACK_A_B==0)
			RegWrite(Bus,CR1,0x1000,CardId);//AreaA
		else if(MTSTACK_A_B==1)
			RegWrite(Bus,CR1,0x3000,CardId);//AreaB
		else
			return (-9);

		RegWrite(Bus,CR3,0x8000,CardId);   //Select Message monitor

		for(i=0;i<128;i++) //selective monitor lookup table
		{
			MemWrite(Bus,(0x3C000+i),0xFFFF,CardId); 
					//select all types of messages
		}

		return  TRUE;
	}
	else
		return FALSE;
}

CHAR RunMt(UCHAR Bus,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	STRSTR=0x1F;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		RegWrite(Bus,STRSTR,0x6,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR ResetMt(UCHAR Bus,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	STRSTR	= 0x1F;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		RegWrite(Bus,STRSTR,1,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR ReadMonPacket(UCHAR Bus,UCHAR MTSTACK_A_B,USHORT DespNo,MNTPACK *MontDataBuff,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	USHORT	i,t_r;
	USHORT	temp;
	UINT	Offset,ptr,MStatus = 0x0;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		if(MTSTACK_A_B==0)
			Offset = ( 0x3C084 + ((DespNo * 4) - 4));
		else if(MTSTACK_A_B==1)
			Offset = ( 0x3E042 + ((DespNo * 4) - 4));
		else
			return (-9);

		MontDataBuff->BlockSts.value = MemRead(Bus,Offset,CardId);
		MontDataBuff->TimeTag = MemRead(Bus,Offset + 1,CardId);
		MontDataBuff->DataBlkPtr = MemRead(Bus,Offset + 2,CardId);
		ptr = MontDataBuff->DataBlkPtr;
		ptr=ptr+0x30000;
		MontDataBuff->RecCmdWrd.value = 
					MemRead(Bus,Offset + 3,CardId);
		t_r = MontDataBuff->RecCmdWrd.value;
		temp=MontDataBuff->RecCmdWrd.bitwise.WrdCntOrMde;
		if(temp==0) temp=32;

		if( MontDataBuff->RecCmdWrd.bitwise.TrnRecv == 1 )//transmit
		{
		MontDataBuff->TxCmdWrd.value = MontDataBuff->RecCmdWrd.value;
			MontDataBuff->TxStatus = MemRead(Bus,ptr,CardId);
			ptr++;
			
			for(i=0; i<temp;i++)
			{
			MontDataBuff->DataPack[i] = MemRead(Bus,ptr,CardId);
				ptr=ptr+1;
				
			}
			MStatus = MontDataBuff->TxStatus;
		}

		else if( MontDataBuff->RecCmdWrd.bitwise.TrnRecv == 0 )
								//receive
		{
		if(((MontDataBuff->BlockSts.value & 0x0800)>>11) == 0x1)
		{
		   MontDataBuff->TxCmdWrd.value = MemRead(Bus,ptr,CardId);
			MontDataBuff->TxStatus = MemRead(Bus,ptr+1,CardId);
				ptr = ptr + 2;
			for(i=0; i < temp;i++)
			{
			MontDataBuff->DataPack[i] = MemRead(Bus,ptr,CardId);
			ptr= ptr+1;
			}
		if(((MontDataBuff->RecCmdWrd.value & 0xf800)>>11) == 0x1f)
			MontDataBuff->RecStatus = 0xcccc;
		else
			MontDataBuff->RecStatus = MemRead(Bus,ptr,CardId);
			MStatus = MontDataBuff->RecStatus;
		}
		else
		{
			for(i=0; i < temp;i++)
			{
			MontDataBuff->DataPack[i] = MemRead(Bus,ptr,CardId);
			ptr= ptr+1;
			}
		  if(((MontDataBuff->RecCmdWrd.value & 0xf800)>>11) == 0x1f)
			MontDataBuff->RecStatus = 0xcccc;
		  else
			MontDataBuff->RecStatus = MemRead(Bus,ptr,CardId);
 		      if(MontDataBuff->RecCmdWrd.bitwise.SubAddr == 0 ||
			 MontDataBuff->RecCmdWrd.bitwise.SubAddr == 31)
		      {
				ptr = (ptr - temp) + 1;
			  MontDataBuff->RecStatus = MemRead(Bus,ptr,CardId);
		       }
			MStatus = MontDataBuff->RecStatus;
		}//end of else
		}
		if(((MontDataBuff->RecCmdWrd.value & 0xF800) != 
			(MStatus & 0xF800)) && 
			((MontDataBuff->RecCmdWrd.value & 0xF800) != 0xF800))
		{
		 MontDataBuff->BlockSts.value = MontDataBuff->BlockSts.value
								 | 0x1000;
		}
		return TRUE;
	}
	else
		return FALSE;
}

CHAR SelWordMt(UCHAR Bus,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	CR1=0x13,CR3=0x17;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		
		RegWrite(Bus,CR1,0x0000,CardId);
		RegWrite(Bus,CR3,0x0000,CardId);//Select Word monitor
		MemWrite(Bus,0x3C000,0xC001,CardId);//pointing word pointer
		return TRUE;
	}
	else
		return FALSE;
}

CHAR DeSelAll(UCHAR Bus,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	int	i;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		for(i=0;i<128;i++) //selective monitor lookup table
		{
			MemWrite(Bus,(0x3C000+i),0x0000,CardId); 
					//deselect all types of messages
		}
		return TRUE;
	}
	else
		return FALSE;
}

CHAR SelAll(UCHAR Bus,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	int	i;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		for(i=0;i<128;i++) //selective monitor lookup table
		{
			MemWrite(Bus,(0x3C000+i),0xFFFF,CardId); 
				//select all types of messages
		}

		return TRUE;
	}
	else
		return FALSE;
}

CHAR SelCmd(UCHAR Bus,CMD_WRD *temp,UCHAR CardId) 
		//Code not written properly, only copied from DDC
{	
	BOOL	CardFlag = FALSE;
	USHORT	val,shift;
	ULONG	offset;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		temp->cmd.wcnt = 0;
		val = (temp->wrd & 0xfff0) >> 9;
		shift = temp->cmd.subaddr & 0x0f;
		offset = val + 0x3C000;
		val = 0x0001 << shift;
		shift = MemRead(Bus,offset,CardId);
		val = val | shift;
		MemWrite(Bus,offset,val,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR DeSelCmd(UCHAR Bus,CMD_WRD *temp,UCHAR CardId)
			//Code not written properly, only copied from DDC
{	
	BOOL	CardFlag = FALSE;
	USHORT	val,shift;
	ULONG	offset;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		temp->cmd.wcnt = 0;
		val = (temp->wrd & 0xfff0) >> 9;
		shift = temp->cmd.subaddr & 0x0f;
		offset = val + 0x3C000;
		val = ~(0x0001 << shift);
		shift = MemRead(Bus,offset,CardId);
		val = val & shift;
		MemWrite(Bus,offset,val,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR ReadMtWord(UCHAR Bus,MT_WORD_DEF *result,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		result -> rx_word = MemRead(Bus,WrdMonStkPtr,CardId);
		result -> idword.value= MemRead(Bus,WrdMonStkPtr + 1,CardId);
		WrdMonStkPtr = WrdMonStkPtr+2;
		if(WrdMonStkPtr >= 0x3ffff)
			WrdMonStkPtr = 0x3c001;
		return TRUE;
	}
	else
		return FALSE;
}

void DefErrors()
{
	*(err_buff+0) =  "INVALID CARD_ID OR CARD IS NOT PRESENT";
	*(err_buff+1) =  "NO ERROR";
	*(err_buff+2) =  "ERROR_IN_SELECTING_MODULE";
	*(err_buff+3) =  "INVALID_MEMORY_OFFSET";
	*(err_buff+4) =  "INVALID_MEMORY_HANDLE";
	*(err_buff+5) = "INVALID_REGISTER_OFFSET";
	*(err_buff+6) = "INVALID_MODULE_SELECTION";
	*(err_buff+7) = "INVALID_MEMORY_OPERATION";
	*(err_buff+8) = "INVALID_STACK_SIZE";
	*(err_buff+9) = "INVALID_STACK_SELECTION";
	*(err_buff+10) = "INVALID_MESSAGE_NUMBER";
	*(err_buff+11) = "INVALID_DATAWORD_COUNT";
	*(err_buff+12) = "MODULE_CONFIGURED_AGAIN";
	*(err_buff+13) = "INVALID_SUB-ADDRESS";
	*(err_buff+14) = "INVALID_CIRCULAR-BUFFER_BLOCK_SIZE";
	*(err_buff+15) = "INVALID_CIRCULAR-BUFFER_DATA_BLOCK_NUMBER";
	*(err_buff+16) = "INVALID_INTR_SET_VALUE";
	*(err_buff+17) = "INVALID_INTR_GET_VALUE";
	*(err_buff+18) = "RESERVED";
	*(err_buff+19) = "RESERVED";
	*(err_buff+20) = "RT_RT-TRANSFER_CMD-#1_MUST_BE_A_RECEIVE_CMD";
	*(err_buff+21) = "RT_RT-TRANSFER_WORD-COUNT_NOT_THE_SAME";
	*(err_buff+22) = "RT_RT-TRANSFER_CMD-#2_MUST_BE_A_TRANSMIT_CMD";
	*(err_buff+23) = "RT_ADDRESS_OUT_OF_RANGE-(i.e. 0-31 valid)";
	*(err_buff+24) = "INCOMPLETE MESSAGE OPERATION";
	*(err_buff+25) = "INVALID INPUT PARAMETER (ALLOWS ONLY YES/NO)";
	*(err_buff+26) = "INVALID RETRY SELECTION";
	*(err_buff+27) = "INVALID TIME TAG RESOLUTION VALUE";
	*(err_buff+28) = 		"END_OF_FRAME_REACHED_BEFORE_DEFINITION_OF_MESSAGES_COMPLETED";

	*(err_buff+31) = "SELF TEST : MODULE 1 MEMORY ERROR";
	*(err_buff+32) = "SELF TEST : MODULE 2 MEMORY ERROR";
	*(err_buff+33) = "SELF TEST : MODULE 1 STACK A RT REGISTERS ERROR";
	*(err_buff+34) = "SELF TEST : MODULE 1 STACK A MT REGISTERS ERROR";
	*(err_buff+35) = 
		"SELF TEST : MODULE 1 STACK A CH B BC MESSAGE ERROR";
	*(err_buff+36) = 
		"SELF TEST : MODULE 1 STACK A CH B BC EOM INTERRUPT ERROR";
	*(err_buff+37) = 
			"SELF TEST : MODULE 1 STACK A CH A BC MESSAGE ERROR";
	*(err_buff+38) =
		 "SELF TEST : MODULE 1 STACK A CH A BC EOM INTERRUPT ERROR";
	*(err_buff+39) = "SELF TEST : MODULE 1 STACK B RT REGISTERS ERROR";
	*(err_buff+40) = "SELF TEST : MODULE 1 STACK B MT REGISTERS ERROR";
	*(err_buff+41) = 
			"SELF TEST : MODULE 1 STACK B CH B BC MESSAGE ERROR";
	*(err_buff+42) = 
		"SELF TEST : MODULE 1 STACK B CH B BC EOM INTERRUPT ERROR";
	*(err_buff+43) = 
			"SELF TEST : MODULE 1 STACK B CH A BC MESSAGE ERROR";
	*(err_buff+44) = 
		"SELF TEST : MODULE 1 STACK B CH A BC EOM INTERRUPT ERROR";
	*(err_buff+45) = "SELF TEST : MODULE 2 STACK A RT REGISTERS ERROR";
	*(err_buff+46) = "SELF TEST : MODULE 2 STACK A MT REGISTERS ERROR";
	*(err_buff+47) = 
			"SELF TEST : MODULE 2 STACK A CH B BC MESSAGE ERROR";
	*(err_buff+48) = 
		"SELF TEST : MODULE 2 STACK A CH B BC EOM INTERRUPT ERROR";
	*(err_buff+49) = 
			"SELF TEST : MODULE 2 STACK A CH A BC MESSAGE ERROR";
	*(err_buff+50) = 
		"SELF TEST : MODULE 2 STACK A CH A BC EOM INTERRUPT ERROR";
	*(err_buff+51) = "SELF TEST : MODULE 2 STACK B RT REGISTERS ERROR";
	*(err_buff+52) = "SELF TEST : MODULE 2 STACK B MT REGISTERS ERROR";
	*(err_buff+53) = 
			"SELF TEST : MODULE 2 STACK B CH B BC MESSAGE ERROR";
	*(err_buff+54) = 
		"SELF TEST : MODULE 2 STACK B CH B BC EOM INTERRUPT ERROR";
	*(err_buff+55) = 
		"SELF TEST : MODULE 2 STACK B CH A BC MESSAGE ERROR";
	*(err_buff+56) = 
		"SELF TEST : MODULE 2 STACK B CH A BC EOM INTERRUPT ERROR";

	*(err_buff+57) = "SELF TEST : TIMER 1 INTERRUPT ERROR";
	*(err_buff+58) = "SELF TEST : TIMER 2 INTERRUPT ERROR";
	*(err_buff+59) = "SELF TEST : TIMER 3 INTERRUPT ERROR";
}
PCHAR PrintErrorMsg( int code)
{
	if(code !=1)
		return err_buff[-(code)];
	else
		return "No Error";
}

PCHAR GetMfgId_VerId(UCHAR CardId)
{
	char	str[1000];
	UCHAR	CardVer,CardType;
	
	CardVer		=	ReadPort(0x2,CardId);
	CardType	=	ReadPort(0x1,CardId);

	switch (CardType)
	{
	   case 0x0 :
	     	sprintf(str,"APOORVA - 1553B - SINGLE NODE BCRTMT CARD, DRIVER & DLL SW VER %d %d",(CardVer >> 4) , (CardVer & 0x0F));
		break;
	   case 0x1 :
	     	sprintf(str,"APOORVA - 1553B - SINGLE NODE BC31RTsMT CARD, DRIVER & DLL SW VER %d %d",(CardVer >> 4) , (CardVer & 0x0F));
		break;
	   case 0x2 :
	     	sprintf(str,"APOORVA - 1553B - TWO NODE BCRTMT CARD, DRIVER & DLL SW VER %d %d",(CardVer >> 4) , (CardVer & 0x0F));
		break;	
	   case 0x3 :
	     	sprintf(str,"APOORVA - 1553B - TWO NODE BC31RTsMT CARD, DRIVER & DLL SW VER %d %d",(CardVer >> 4) , (CardVer & 0x0F));
		break;
	   case 0x4 :
	     	sprintf(str,"APOORVA - 1553B - THREE NODE BCRTMT CARD, DRIVER & DLL SW VER %d %d",(CardVer >> 4) , (CardVer & 0x0F));
		break;	
	   case 0x5 :
	     	sprintf(str,"APOORVA - 1553B - THREE NODE BC31RTsMT CARD, DRIVER & DLL SW VER %d %d",	(CardVer >> 4) , (CardVer & 0x0F));
		break;
           case 0x8 :
	     	sprintf(str,"APOORVA - 1553B - FOUR NODE BCRTMT CARD, DRIVER & DLL SW VER %d %d",(CardVer >> 4) , (CardVer & 0x0F));
		break;
	   case 0x9 :
	     	sprintf(str,"APOORVA - 1553B - FOUR NODE BCRTMT CARD, DRIVER & DLL SW VER %d %d",(CardVer >> 4) , (CardVer & 0x0F));
		break;
   	   case 0x80 :
	     	sprintf(str,"APOORVA - 1553B - 8 MTs CARD, DRIVER & DLL SW VER %d %d",(CardVer >> 4) , (CardVer & 0x0F));
		break;
	   default :
	     	sprintf(str,"APOORVA - 1553B - CARD, DRIVER & DLL SW VER %d %d",(CardVer >> 4) , (CardVer & 0x0F));
		break;	
	}
	return (&str[0]);	
	//return ((char *)&str[0]);	
}

/*CHAR InitInterrupts(UCHAR Bus,UCHAR Value,UCHAR CardId)
{
	
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(CardId==1)
		{	printf("card1");
			ioctl(fd,IOCTL_CARD1_ENABLE_INT,NULL);
		}
		else if(CardId==2)
		{	printf("card---------------------------2");
			ioctl(fd,IOCTL_CARD2_ENABLE_INT,NULL);
		}
		else if(CardId==3)
			ioctl(fd,IOCTL_CARD3_ENABLE_INT,NULL);
		else if(CardId==4)
			ioctl(fd,IOCTL_CARD4_ENABLE_INT,NULL);

		if(Bus == 1)
		{
			Value = ReadPort(0x18,CardId) | Value ;
			printf("BUS-1 INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",Value);
			WritePort(0x08,Value,CardId);
			
			//Value = ReadPort(0x18,CardId) | Value ;
			printf("INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x18,CardId));
		}
		else if(Bus == 2)
		{
			Value = ReadPort(0x38,CardId) | Value ;
			WritePort(0x28,Value,CardId);
			printf("Bus-2 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x38,CardId));
			
		}
		else if(Bus == 3)
		{
			
			Value = ReadPort(0x58,CardId) | Value ;
			WritePort(0x48,Value,CardId);
			printf("Bus-3 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x58,CardId));
		}
		else if(Bus == 4)
		{
			
			Value = ReadPort(0x78,CardId) | Value ;
			WritePort(0x68,Value,CardId);
			printf("Bus-4 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x78,CardId));
		}
		else
			return (-2);
				
		return TRUE;
	}
	else
		return FALSE;
}*/


CHAR InitInterrupts(UCHAR Bus,USHORT Value,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
	   if(Bus < 1 || Bus > 4)
		  return (-2);
		if(CardId==1)
		{
			if(Bus==1)
			{
				Value = ReadPort(0x18,CardId) | Value ;
				//printf("BUS-1 INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",Value);
				WritePort(0x08,Value,CardId);
				
				//Value = ReadPort(0x18,CardId) | Value ;
				//printf("INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x18,CardId));
			}
			else if(Bus==2)
			{
				Value = ReadPort(0x38,CardId) | Value ;
				WritePort(0x28,Value,CardId);
				//printf("Bus-2 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x38,CardId));
			}
			else if(Bus==3)
			{
				Value = ReadPort(0x58,CardId) | Value ;
				WritePort(0x48,Value,CardId);
				//printf("Bus-3 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x58,CardId));
			}
			else if(Bus==4)
			{
				Value = ReadPort(0x78,CardId) | Value ;
				WritePort(0x68,Value,CardId);
				//printf("Bus-4 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x78,CardId));
			}
			else
				return (-2);
		}
		else if(CardId==2)
		{

			if(Bus==1)
			{
				IntrValue2 = ReadPort(0x18,CardId) | Value;
				//printf("BUS-1 INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",Value);
				WritePort(0x08,IntrValue2,CardId);
				//printf("INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x18,CardId));

			}
			else if(Bus==2)
			{
				Mod2IntrValue2 = ReadPort(0x38,CardId) | Value;
				WritePort(0x28,Mod2IntrValue2,CardId);
				//printf("Bus-2 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x38,CardId));
			
			}
			else if(Bus==3)
			{
				Value = ReadPort(0x58,CardId) | Value;
				WritePort(0x48,Value,CardId);
				//printf("Bus-3 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x58,CardId));
			}
			else if(Bus==4)
			{
				Value = ReadPort(0x78,CardId) | Value;
				WritePort(0x68,Value,CardId);
				//printf("Bus-4 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x78,CardId));
			}
			else
				return (-2);
		}
		else if(CardId==3)
		{
			if(Bus==1)
			{
				IntrValue3 = IntrValue3 | Value ;
				WritePort(0x08,IntrValue3,CardId);
			}
			else if(Bus==2)
			{
				Mod2IntrValue3 = Mod2IntrValue3 | Value ;
				WritePort(0x48,Mod2IntrValue3,CardId);
			}
			else if(Bus==3)
			{
				Value = ReadPort(0x58,CardId) | Value ;
				WritePort(0x48,Value,CardId);
				//printf("Bus-3 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x58,CardId));
			}
			else if(Bus==4)
			{
				Value = ReadPort(0x78,CardId) | Value ;
				WritePort(0x68,Value,CardId);
				//printf("Bus-4 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x78,CardId));
			}
			else
				return (-2);
		}
		else if(CardId==4)
		{
			if(Bus==1)
			{
				IntrValue4 = IntrValue4 | Value ;
				WritePort(0x08,IntrValue4,CardId);
			}
			else if(Bus==2)
			{
				Mod2IntrValue4 = Mod2IntrValue4 | Value ;
				WritePort(0x48,Mod2IntrValue4,CardId);
			}
			else if(Bus==3)
			{
				Value = ReadPort(0x58,CardId) | Value ;
				WritePort(0x48,Value,CardId);
				//printf("Bus-3 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x58,CardId));
			}
			else if(Bus==4)
			{
				Value = ReadPort(0x78,CardId) | Value ;
				WritePort(0x68,Value,CardId);
				//printf("Bus-4 --> INITINTERRUPTS  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x78,CardId));
			}
			else
				return (-2);
		}
		ConfigICSR(CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR InitTimerIntr(USHORT Value,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(CardId==1)
		{
			/*IntrValue1 = IntrValue1 | Value;
			WritePort(0x08,IntrValue1,CardId);
			Mod3IntrValue1 = Mod3IntrValue1 | Value;
			WritePort(0x09,Mod3IntrValue1,CardId);*/

			Value = ReadPort(0x18,CardId) | Value;
			WritePort(0x08,Value,CardId);
			
			Value = Value & 0xE0;
	
			Value = ReadPort(0x19,CardId) | Value;
			WritePort(0x09,Value,CardId);		
		
		}
		else if(CardId==2)
		{
			IntrValue2 = IntrValue2 | Value;
			WritePort(0x08,IntrValue2,CardId);
			Mod3IntrValue2 = Mod3IntrValue2 | Value;
			WritePort(0x09,Mod3IntrValue2,CardId);
		}
		else if(CardId==3)
		{
			IntrValue3 = IntrValue3 | Value;
			WritePort(0x08,IntrValue3,CardId);
			Mod3IntrValue3 = Mod3IntrValue3 | Value;
			WritePort(0x09,Mod3IntrValue3,CardId);
		}
		else if(CardId==4)
		{
			IntrValue4 = IntrValue4 | Value;
			WritePort(0x08,IntrValue4,CardId);
			Mod3IntrValue4 = Mod3IntrValue4 | Value;
			WritePort(0x09,Mod3IntrValue4,CardId);
		}
		ConfigICSR(CardId);
		return TRUE;
	}
	else
		return FALSE;
}
CHAR StopTimerIntr(USHORT Value,UCHAR CardId)
{	
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(CardId==1)
		{
			IntrValue1 = IntrValue1 & (~Value);
			WritePort(0x08,IntrValue1,CardId);
			Mod3IntrValue1 = Mod3IntrValue1 & (~Value);
			WritePort(0x09,Mod3IntrValue1,CardId);
		}
		else if(CardId==2)
		{
			IntrValue2 = IntrValue2 & (~Value);
			WritePort(0x08,IntrValue2,CardId);
			Mod3IntrValue2 = Mod3IntrValue2 & (~Value);
			WritePort(0x09,Mod3IntrValue2,CardId);
		}
		else if(CardId==3)
		{
			IntrValue3 = IntrValue3 & (~Value);
			WritePort(0x08,IntrValue3,CardId);
			Mod3IntrValue3 = Mod3IntrValue3 & (~Value);
			WritePort(0x09,Mod3IntrValue3,CardId);
		}
		else if(CardId==4)
		{
			IntrValue4 = IntrValue4 & (~Value);
			WritePort(0x08,IntrValue4,CardId);
			Mod3IntrValue4 = Mod3IntrValue4 & (~Value);
			WritePort(0x09,Mod3IntrValue4,CardId);
		}
		ConfigICSR(CardId);
		return TRUE;
	}
	else
		return FALSE;
}
CHAR SelectTMR(UCHAR Timer1,UCHAR Timer2,UCHAR Timer3,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	USHORT	TimerValue = 0x0;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		TimerValue = Timer3 << 6;
		TimerValue = TimerValue | (Timer2 << 3);
		TimerValue = TimerValue | (Timer1);
		RegWrite(1,0x1B,TimerValue,CardId);

		return TRUE;
	}
	else
		return FALSE;
}
CHAR DisableInterrupts(UCHAR Bus,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(CardId==1)
		{
			if(Bus==1)
				WritePort(0x08,0,CardId);
			else if(Bus==2)
				WritePort(0x28,0,CardId);
			else if(Bus==3)
				WritePort(0x48,0,CardId);
			else if(Bus==4)
				WritePort(0x68,0,CardId);
			else
				return (-2);		
		}
		else if(CardId==2)
		{
			if(Bus==1)
				WritePort(0x08,0,CardId);
			else if(Bus==2)
				WritePort(0x28,0,CardId);
			else if(Bus==3)
				WritePort(0x48,0,CardId);
			else if(Bus==4)
				WritePort(0x68,0,CardId);
			else
				return (-2);		
		}
		else if(CardId==3)
		{
			if(Bus==1)
				WritePort(0x08,0,CardId);
			else if(Bus==2)
				WritePort(0x28,0,CardId);
			else if(Bus==3)
				WritePort(0x48,0,CardId);
			else if(Bus==4)
				WritePort(0x68,0,CardId);
			else
				return (-2);		
		}
		else if(CardId==4)
		{
			if(Bus==1)
				WritePort(0x08,0,CardId);
			else if(Bus==2)
				WritePort(0x28,0,CardId);
			else if(Bus==3)
				WritePort(0x48,0,CardId);
			else if(Bus==4)
				WritePort(0x68,0,CardId);
			else
				return (-2);		
		}
		return TRUE;
	}
	else
		return FALSE;
}

LONG GetBCEOMCnt(UCHAR Bus,UCHAR CardId)//Get BC End of Msg Interrupt count
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		
		if(CardId==1)
		{
		if(Bus==1)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_BCEOMCNT,NULL);
		else if(Bus==2)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_BCEOMCNT,NULL);
		else if(Bus==3)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_BCEOMCNT,NULL);
		else if(Bus==4)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_BCEOMCNT,NULL);
		else
			return (-2);
		}
		else if(CardId==2)
		{
		if(Bus==1)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD1_GET_BCEOMCNT,NULL);
		else if(Bus==2)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD2_GET_BCEOMCNT,NULL);
		else if(Bus==3)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD3_GET_BCEOMCNT,NULL);
		else if(Bus==4)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD4_GET_BCEOMCNT,NULL);
		else
			return (-2);
		}
		else if(CardId==3)
		{
		if(Bus==1)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD1_GET_BCEOMCNT,NULL);
		else if(Bus==2)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD2_GET_BCEOMCNT,NULL);
		else if(Bus==3)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD3_GET_BCEOMCNT,NULL);
		else if(Bus==4)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD4_GET_BCEOMCNT,NULL);
		else
			return (-2);
		}
		else if(CardId==4)
		{
		if(Bus==1)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD1_GET_BCEOMCNT,NULL);
		else if(Bus==2)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD2_GET_BCEOMCNT,NULL);
		else if(Bus==3)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD3_GET_BCEOMCNT,NULL);
		else if(Bus==4)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD4_GET_BCEOMCNT,NULL);
		else
			return (-2);
		}
		else
			return FALSE;
		
		return Cnt;
	}
	else
		return FALSE;
}

LONG GetRTEOMCnt(UCHAR Bus,USHORT RtNo,UCHAR CardId)//Get RT End of Msg Interrupt count
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		if(CardId==1)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
			
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
		else
			return (-2);
		}
		else if(CardId==2)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
			
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
		else
			return (-2);
		}
		else if(CardId==3)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
			
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
		else
			return (-2);
		}
		else if(CardId==4)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
			
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT0_EOMCNT,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT1_EOMCNT,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT2_EOMCNT,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT3_EOMCNT,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT4_EOMCNT,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT5_EOMCNT,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT6_EOMCNT,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT7_EOMCNT,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT8_EOMCNT,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT9_EOMCNT,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT10_EOMCNT,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT11_EOMCNT,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT12_EOMCNT,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT13_EOMCNT,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT14_EOMCNT,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT15_EOMCNT,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT16_EOMCNT,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT17_EOMCNT,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT18_EOMCNT,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT19_EOMCNT,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT20_EOMCNT,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT21_EOMCNT,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT22_EOMCNT,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT23_EOMCNT,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT24_EOMCNT,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT25_EOMCNT,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT26_EOMCNT,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT27_EOMCNT,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT28_EOMCNT,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT29_EOMCNT,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT30_EOMCNT,NULL);
					break;
			}
		}
		else
			return (-2);
		}
		else
			return FALSE;
		return Cnt;
	}
	else
		return FALSE;
}



LONG GetMTEOMCnt(UCHAR Bus,UCHAR CardId)  //Get MT End of Msg Interrupt count
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		
		if(CardId==1)
		{
		if(Bus==1)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_MTEOMCNT,NULL);
		else if(Bus==2)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_MTEOMCNT,NULL);
		else if(Bus==3)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_MTEOMCNT,NULL);
		else if(Bus==4)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_MTEOMCNT,NULL);
		else
			return (-2);
		}
		else if(CardId==2)
		{
		if(Bus==1)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD1_GET_MTEOMCNT,NULL);
		else if(Bus==2)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD2_GET_MTEOMCNT,NULL);
		else if(Bus==3)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD3_GET_MTEOMCNT,NULL);
		else if(Bus==4)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD4_GET_MTEOMCNT,NULL);
		else
			return (-2);
		}
		else if(CardId==3)
		{
		if(Bus==1)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD1_GET_MTEOMCNT,NULL);
		else if(Bus==2)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD2_GET_MTEOMCNT,NULL);
		else if(Bus==3)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD3_GET_MTEOMCNT,NULL);
		else if(Bus==4)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD4_GET_MTEOMCNT,NULL);
		else
			return (-2);
		}
		else if(CardId==4)
		{
		if(Bus==1)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD1_GET_MTEOMCNT,NULL);
		else if(Bus==2)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD2_GET_MTEOMCNT,NULL);
		else if(Bus==3)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD3_GET_MTEOMCNT,NULL);
		else if(Bus==4)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD4_GET_MTEOMCNT,NULL);
		else
			return (-2);
		}
		else
			return FALSE;
		
		return Cnt;
	}
	else
		return FALSE;
}

LONG GetRTCIRCCnt(UCHAR Bus,USHORT RtNo,UCHAR CardId)	//Get RT CIRC of Msg Interrupt count
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		
		if(CardId==1)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else
			return (-2);
		}
		else if(CardId==2)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else
			return (-2);
		}
		else if(CardId==3)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else
			return (-2);
		}
		else if(CardId==4)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT0_CIRC,NULL);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT1_CIRC,NULL);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT2_CIRC,NULL);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT3_CIRC,NULL);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT4_CIRC,NULL);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT5_CIRC,NULL);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT6_CIRC,NULL);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT7_CIRC,NULL);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT8_CIRC,NULL);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT9_CIRC,NULL);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT10_CIRC,NULL);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT11_CIRC,NULL);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT12_CIRC,NULL);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT13_CIRC,NULL);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT14_CIRC,NULL);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT15_CIRC,NULL);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT16_CIRC,NULL);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT17_CIRC,NULL);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT18_CIRC,NULL);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT19_CIRC,NULL);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT20_CIRC,NULL);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT21_CIRC,NULL);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT22_CIRC,NULL);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT23_CIRC,NULL);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT24_CIRC,NULL);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT25_CIRC,NULL);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT26_CIRC,NULL);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT27_CIRC,NULL);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT28_CIRC,NULL);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT29_CIRC,NULL);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_GET_RT30_CIRC,NULL);
					break;
			}
		}
		else
			return (-2);
		}
		else
			return FALSE;
		
		return Cnt;
	}
	else
		return FALSE;
}

LONG GetRTDBCACnt(UCHAR Bus,UCHAR CardId) //Get RT DBCA of Msg Interruptcount
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		
		if(CardId==1)
		{
		if(Bus==1)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_RTDBCA,NULL);
		else if(Bus==2)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_RTDBCA,NULL);
		else if(Bus==3)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_RTDBCA,NULL);
		else if(Bus==4)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_RTDBCA,NULL);
		else
			return (-2);
		}
		else if(CardId==2)
		{
		if(Bus==1)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD1_GET_RTDBCA,NULL);
		else if(Bus==2)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD2_GET_RTDBCA,NULL);
		else if(Bus==3)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD3_GET_RTDBCA,NULL);
		else if(Bus==4)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD4_GET_RTDBCA,NULL);
		else
			return (-2);
		}
		else if(CardId==3)
		{
		if(Bus==1)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD1_GET_RTDBCA,NULL);
		else if(Bus==2)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD2_GET_RTDBCA,NULL);
		else if(Bus==3)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD3_GET_RTDBCA,NULL);
		else if(Bus==4)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD4_GET_RTDBCA,NULL);
		else
			return (-2);
		}
		else if(CardId==4)
		{
		if(Bus==1)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD1_GET_RTDBCA,NULL);
		else if(Bus==2)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD2_GET_RTDBCA,NULL);
		else if(Bus==3)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD3_GET_RTDBCA,NULL);
		else if(Bus==4)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD4_GET_RTDBCA,NULL);
		else
			return (-2);
		}
		else
			return FALSE;
		
		return Cnt;
	}
	else
		return FALSE;
}

LONG GetTMR1Cnt(UCHAR CardId)		//Get TMR1 Interrupt count
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(CardId==1)
			Cnt = ioctl(fd,IOCTL_CARD1_GET_RTC1,NULL);
		else if(CardId==2)
			Cnt = ioctl(fd2,IOCTL_CARD2_GET_RTC1,NULL);
		else if(CardId==3)
			Cnt = ioctl(fd3,IOCTL_CARD3_GET_RTC1,NULL);
		else if(CardId==4)
			Cnt = ioctl(fd4,IOCTL_CARD4_GET_RTC1,NULL);
		else
			return FALSE;
		return Cnt;
	}
	else
		return FALSE;
}

LONG GetTMR2Cnt(UCHAR CardId)		//Get TMR2 Interrupt count
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(CardId==1)
			Cnt = ioctl(fd,IOCTL_CARD1_GET_RTC2,NULL);
		else if(CardId==2)
			Cnt = ioctl(fd2,IOCTL_CARD2_GET_RTC2,NULL);
		else if(CardId==3)
			Cnt = ioctl(fd3,IOCTL_CARD3_GET_RTC2,NULL);
		else if(CardId==4)
			Cnt = ioctl(fd4,IOCTL_CARD4_GET_RTC2,NULL);
		else
			return FALSE;
	}
	else
		return FALSE;
}

LONG GetTMR3Cnt(UCHAR CardId)		//Get TMR3 Interrupt count
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(CardId==1)
			Cnt = ioctl(fd,IOCTL_CARD1_GET_RTC3,NULL);
		else if(CardId==2)
			Cnt = ioctl(fd2,IOCTL_CARD2_GET_RTC3,NULL);
		else if(CardId==3)
			Cnt = ioctl(fd3,IOCTL_CARD3_GET_RTC3,NULL);
		else if(CardId==4)
			Cnt = ioctl(fd4,IOCTL_CARD4_GET_RTC3,NULL);
		else
			return FALSE;
	}
	else
		return FALSE;
}

CHAR SetBCEOMCnt(UCHAR Bus,ULONG value,UCHAR CardId)	
				//Set BC Interrupt Count to given Value;
{
    	UCHAR Cnt;
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(CardId==1)
		{
		if(Bus==1)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_BCEOMCNT,value);
		else if(Bus==2)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_BCEOMCNT,value);
		else if(Bus==3)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_BCEOMCNT,value);
		else if(Bus==4)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_BCEOMCNT,value);
		else
			return (-2);
		}
		else if(CardId==2)
		{
		if(Bus==1)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD1_SET_BCEOMCNT,value);
		else if(Bus==2)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD2_SET_BCEOMCNT,value);
		else if(Bus==3)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD3_SET_BCEOMCNT,value);
		else if(Bus==4)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD4_SET_BCEOMCNT,value);
		else
			return (-2);
		}
		else if(CardId==3)
		{
		if(Bus==1)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD1_SET_BCEOMCNT,value);
		else if(Bus==2)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD2_SET_BCEOMCNT,value);
		else if(Bus==3)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD3_SET_BCEOMCNT,value);
		else if(Bus==4)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD4_SET_BCEOMCNT,value);
		else
			return (-2);
		}
		else if(CardId==4)
		{
		if(Bus==1)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD1_SET_BCEOMCNT,value);
		else if(Bus==2)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD2_SET_BCEOMCNT,value);
		else if(Bus==3)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD3_SET_BCEOMCNT,value);
		else if(Bus==4)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD4_SET_BCEOMCNT,value);
		else
			return (-2);
		}
		else
			return FALSE;
		return Cnt;	
	}	
	else
		return FALSE;
}

CHAR SetRTEOMCnt(UCHAR Bus,USHORT RtNo,ULONG value,UCHAR CardId)	
				//Set RT Interrupt Count to given Value;
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{	if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		if(CardId==1)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else
			return (-2);
		}
		else if(CardId==2)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else
			return (-2);
		}
		else if(CardId==3)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else
			return (-2);
		}
		else if(CardId==4)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT0_EOMCNT,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT1_EOMCNT,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT2_EOMCNT,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT3_EOMCNT,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT4_EOMCNT,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT5_EOMCNT,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT6_EOMCNT,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT7_EOMCNT,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT8_EOMCNT,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT9_EOMCNT,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT10_EOMCNT,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT11_EOMCNT,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT12_EOMCNT,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT13_EOMCNT,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT14_EOMCNT,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT15_EOMCNT,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT16_EOMCNT,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT17_EOMCNT,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT18_EOMCNT,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT19_EOMCNT,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT20_EOMCNT,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT21_EOMCNT,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT22_EOMCNT,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT23_EOMCNT,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT24_EOMCNT,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT25_EOMCNT,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT26_EOMCNT,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT27_EOMCNT,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT28_EOMCNT,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT29_EOMCNT,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT30_EOMCNT,value);
					break;
			}
			
		}
		else
			return (-2);
		}
		else
			return FALSE;
		return Cnt;	
   	}	
	else
		return FALSE;
}


CHAR SetMTEOMCnt(UCHAR Bus,ULONG value,UCHAR CardId)	
				//Set MT Interrupt Count to given Value;
{
    	UCHAR Cnt;
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);

		if(CardId==1)
		{
		if(Bus==1)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_MTEOMCNT,value);
		else if(Bus==2)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_MTEOMCNT,value);
		else if(Bus==3)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_MTEOMCNT,value);
		else if(Bus==4)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_MTEOMCNT,value);
		else
			return (-2);
		}
		else if(CardId==2)
		{
		if(Bus==1)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD1_SET_MTEOMCNT,value);
		else if(Bus==2)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD2_SET_MTEOMCNT,value);
		else if(Bus==3)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD3_SET_MTEOMCNT,value);
		else if(Bus==4)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD4_SET_MTEOMCNT,value);
		else
			return (-2);
		}
		else if(CardId==3)
		{
		if(Bus==1)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD1_SET_MTEOMCNT,value);
		else if(Bus==2)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD2_SET_MTEOMCNT,value);
		else if(Bus==3)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD3_SET_MTEOMCNT,value);
		else if(Bus==4)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD4_SET_MTEOMCNT,value);
		else
			return (-2);
		}
		else if(CardId==4)
		{
		if(Bus==1)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD1_SET_MTEOMCNT,value);
		else if(Bus==2)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD2_SET_MTEOMCNT,value);
		else if(Bus==3)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD3_SET_MTEOMCNT,value);
		else if(Bus==4)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD4_SET_MTEOMCNT,value);
		else
			return (-2);
		}
		else
			return FALSE;
		return Cnt;	
	}
	else
		return FALSE;
}

CHAR SetRTCIRCCnt(UCHAR Bus,USHORT RtNo,ULONG value,UCHAR CardId)	
				//Set RTCIRC Intr Count to given Value;
{
    	UCHAR	Cnt;
	BOOL	CardFlag = FALSE;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		if(CardId==1)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RT30_CIRC,value);
					break;
			}		
		}
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RT30_CIRC,value);
					break;
			}		
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RT30_CIRC,value);
					break;
			}		
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RT30_CIRC,value);
					break;
			}		
		}
		else
			return (-2);
		}
		else if(CardId==2)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD1_SET_RT30_CIRC,value);
					break;
			}		
		}
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD2_SET_RT30_CIRC,value);
					break;
			}		
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD3_SET_RT30_CIRC,value);
					break;
			}		
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD2_MOD4_SET_RT30_CIRC,value);
					break;
			}		
		}
		else
			return (-2);
		}
		else if(CardId==3)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD1_SET_RT30_CIRC,value);
					break;
			}		
		}
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD2_SET_RT30_CIRC,value);
					break;
			}		
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD3_SET_RT30_CIRC,value);
					break;
			}		
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD3_MOD4_SET_RT30_CIRC,value);
					break;
			}		
		}
		else
			return (-2);
		}
		else if(CardId==4)
		{
		if(Bus==1)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD1_SET_RT30_CIRC,value);
					break;
			}		
		}
		else if(Bus==2)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD2_SET_RT30_CIRC,value);
					break;
			}		
		}
		else if(Bus==3)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD3_SET_RT30_CIRC,value);
					break;
			}		
		}
		else if(Bus==4)
		{
			switch(RtNo)
			{
				case 0:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT0_CIRC,value);
					break;
				case 1:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT1_CIRC,value);
					break;
				case 2:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT2_CIRC,value);
					break;
				case 3:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT3_CIRC,value);
					break;
				case 4:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT4_CIRC,value);
					break;
				case 5:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT5_CIRC,value);
					break;
				case 6:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT6_CIRC,value);
					break;
				case 7:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT7_CIRC,value);
					break;
				case 8:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT8_CIRC,value);
					break;
				case 9:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT9_CIRC,value);
					break;
				case 10:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT10_CIRC,value);
					break;
				case 11:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT11_CIRC,value);
					break;
				case 12:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT12_CIRC,value);
					break;
				case 13:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT13_CIRC,value);
					break;
				case 14:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT14_CIRC,value);
					break;
				case 15:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT15_CIRC,value);
					break;
				case 16:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT16_CIRC,value);
					break;
				case 17:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT17_CIRC,value);
					break;
				case 18:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT18_CIRC,value);
					break;
				case 19:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT19_CIRC,value);
					break;
				case 20:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT20_CIRC,value);
					break;
				case 21:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT21_CIRC,value);
					break;
				case 22:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT22_CIRC,value);
					break;
				case 23:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT23_CIRC,value);
					break;
				case 24:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT24_CIRC,value);
					break;
				case 25:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT25_CIRC,value);
					break;
				case 26:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT26_CIRC,value);
					break;
				case 27:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT27_CIRC,value);
					break;
				case 28:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT28_CIRC,value);
					break;
				case 29:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT29_CIRC,value);
					break;
				case 30:
					Cnt = ioctl(fd,IOCTL_CARD4_MOD4_SET_RT30_CIRC,value);
					break;
			}		
		}
		else
			return (-2);
		}
		else
			return FALSE;

		return Cnt;	
	}	
	else
		return FALSE;

}

CHAR SetRTDBCACnt(UCHAR Bus,ULONG value,UCHAR CardId)	
					//Set BC Intr Count to given Value;
{	
	UCHAR	Cnt;
	BOOL	CardFlag = FALSE;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(CardId==1)
		{
		if(Bus==1)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD1_SET_RTDBCA,value);
		else if(Bus==2)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD2_SET_RTDBCA,value);
		else if(Bus==3)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD3_SET_RTDBCA,value);
		else if(Bus==4)
			Cnt = ioctl(fd,IOCTL_CARD1_MOD4_SET_RTDBCA,value);
		else
			return (-2);
		}
		else if(CardId==2)
		{
		if(Bus==1)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD1_SET_RTDBCA,value);
		else if(Bus==2)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD2_SET_RTDBCA,value);
		else if(Bus==3)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD3_SET_RTDBCA,value);
		else if(Bus==4)
			Cnt = ioctl(fd2,IOCTL_CARD2_MOD4_SET_RTDBCA,value);
		else
			return (-2);
		}
		else if(CardId==3)
		{
		if(Bus==1)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD1_SET_RTDBCA,value);
		else if(Bus==2)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD2_SET_RTDBCA,value);
		else if(Bus==3)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD3_SET_RTDBCA,value);
		else if(Bus==4)
			Cnt = ioctl(fd3,IOCTL_CARD3_MOD4_SET_RTDBCA,value);
		else
			return (-2);
		}
		else if(CardId==4)
		{
		if(Bus==1)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD1_SET_RTDBCA,value);
		else if(Bus==2)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD2_SET_RTDBCA,value);
		else if(Bus==3)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD3_SET_RTDBCA,value);
		else if(Bus==4)
			Cnt = ioctl(fd4,IOCTL_CARD4_MOD4_SET_RTDBCA,value);
		else
			return (-2);
		}
		else
			return FALSE;

		return Cnt;	
	}
	else
		return FALSE;
}
CHAR SetTMR1Cnt(ULONG value,UCHAR CardId)
				//Set TMR1 Interrupt Count to given Value;
{
    	UCHAR	Cnt;
	BOOL	CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(CardId==1)
			Cnt = ioctl(fd,IOCTL_CARD1_SET_RTC1,value);
		else if(CardId==2)
			Cnt = ioctl(fd2,IOCTL_CARD2_SET_RTC1,value);
		else if(CardId==3)
			Cnt = ioctl(fd3,IOCTL_CARD3_SET_RTC1,value);
		else if(CardId==4)
			Cnt = ioctl(fd4,IOCTL_CARD4_SET_RTC1,value);
		else
			return FALSE;
		return Cnt;		
	}
	else
		return FALSE;
}

CHAR SetTMR2Cnt(ULONG value,UCHAR CardId)
				//Set TMR2 Interrupt Count to given Value;
{
	UCHAR	Cnt;
	BOOL	CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(CardId==1)
			Cnt = ioctl(fd,IOCTL_CARD1_SET_RTC2,value);
		else if(CardId==2)
			Cnt = ioctl(fd2,IOCTL_CARD2_SET_RTC2,value);
		else if(CardId==3)
			Cnt = ioctl(fd3,IOCTL_CARD3_SET_RTC2,value);
		else if(CardId==4)
			Cnt = ioctl(fd4,IOCTL_CARD4_SET_RTC2,value);
		else
			return FALSE;
		return Cnt;		
	}
	else
		return FALSE;
}

CHAR SetTMR3Cnt(ULONG value,UCHAR CardId)
				//Set TMR3 Interrupt Count to given Value;
{
    	UCHAR	Cnt;
	BOOL	CardFlag = FALSE;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(CardId==1)
			Cnt = ioctl(fd,IOCTL_CARD1_SET_RTC3,value);
		else if(CardId==2)
			Cnt = ioctl(fd2,IOCTL_CARD2_SET_RTC3,value);
		else if(CardId==3)
			Cnt = ioctl(fd3,IOCTL_CARD3_SET_RTC3,value);
		else if(CardId==4)
			Cnt = ioctl(fd4,IOCTL_CARD4_SET_RTC3,value);
		else
			return FALSE;
		return Cnt;		
	}
	else
		return FALSE;
}
CHAR WritePort(ULONG offset,UCHAR val,UCHAR CardId) 
{
	Addr ad;
	ad.Data	=	val;
	ad.Addr	=	offset;

	if(CardId == 1)
	{
		ioctl(fd,IOCTL_CARD1_WRITE_PORT,(unsigned int *)&ad);
		return TRUE;	
	}
	else if(CardId == 2)
	{
		ioctl(fd,IOCTL_CARD2_WRITE_PORT,(unsigned int *)&ad);
		return TRUE;	
	}
	else if(CardId == 3)
	{
		ioctl(fd,IOCTL_CARD3_WRITE_PORT,(unsigned int *)&ad);
		return TRUE;	
	}
	else if(CardId == 4)
	{
		ioctl(fd,IOCTL_CARD4_WRITE_PORT,(unsigned int *)&ad);
		return TRUE;		
	}
	else
		return (-1);
}

UCHAR ReadPort(ULONG offset,UCHAR CardId)
{
	UCHAR us;

	if(CardId == 1)
	{
		us = ioctl(fd,IOCTL_CARD1_READ_PORT,(unsigned int)offset);
		return (us);
	}
	else if(CardId == 2)
	{
		us = ioctl(fd,IOCTL_CARD2_READ_PORT,(unsigned int)offset);
		return (us);
	}
	else if(CardId == 3)
	{
		us = ioctl(fd,IOCTL_CARD3_READ_PORT,(unsigned int)offset);
		return (us);
	}
	else if(CardId == 4)
	{
		us = ioctl(fd,IOCTL_CARD4_READ_PORT,(unsigned int)offset);
		return (us);
	}
	else
		return (-1);
}

CHAR ErrorInjection(UCHAR Bus,ERR_INJ Err,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	ErrReg=0xCA;
	USHORT	ErrRegValue=0x0;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		ErrRegValue = ErrRegValue | Err.Error_Inject << 15;
		ErrRegValue = ErrRegValue | Err.Bi_Phase_EncErr  << 14;
		ErrRegValue = ErrRegValue | Err.Word_Length  << 13;
		ErrRegValue = ErrRegValue | Err.Sync_Err  << 12;
		ErrRegValue = ErrRegValue | Err.Parity_Err << 11;
		ErrRegValue = ErrRegValue | Err.Cmd_Data << 10;
		ErrRegValue = ErrRegValue | Err.DataCnt_Err << 9;
		ErrRegValue = ErrRegValue | Err.Err_Info << 4;
		ErrRegValue = ErrRegValue | Err.WordLen_Size << 2;
		ErrRegValue = ErrRegValue | Err.Sync_Info;
		
		RegWrite(Bus,ErrReg,ErrRegValue,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR ErrorInjectReset(UCHAR Bus,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	ErrReg=0xCA;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		RegWrite(Bus,ErrReg,0x0,CardId);
		return TRUE;
	}
	else
		return FALSE;
}

SHORT GetMinorFrameCnt(UCHAR Bus,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
	 	USHORT MinFrCnt = 0x0;
		RegWrite(Bus,0xC5,0x1,CardId);
		MinFrCnt = RegRead(Bus,0xC6,CardId);
		RegWrite(Bus,0xC5,0x0,CardId);
		return MinFrCnt;
	}
	else
		return FALSE;
}

CHAR MsgEnableDisable(UCHAR Bus,UCHAR MsgNos[],UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	UCHAR	i;
	USHORT	MsgValue = 0x0;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		
		for(i=1;i<=16;i++)
		{
			if(MsgNos[i] == TRUE)
				MsgValue = MsgValue | (1 << (i-1)); 
		}
		RegWrite(Bus,0x60,MsgValue,CardId);
		MsgValue = 0x0;

		for(i=17;i<=32;i++)
		{
			if(MsgNos[i] == TRUE)
				MsgValue = MsgValue | (1 << (i-17)); 
		}
		RegWrite(Bus,0x61,MsgValue,CardId);
		MsgValue = 0x0;

		for(i=33;i<=48;i++)
		{
			if(MsgNos[i] == TRUE)
				MsgValue = MsgValue | (1 << (i-33)); 
		}
		RegWrite(Bus,0x62,MsgValue,CardId);
		MsgValue = 0x0;

		for(i=49;i<=64;i++)
		{
			if(MsgNos[i] == TRUE)
				MsgValue = MsgValue | (1 << (i-49)); 
		}
		RegWrite(Bus,0x63,MsgValue,CardId);
		MsgValue = 0x0;

		for(i=65;i<=80;i++)
		{
			if(MsgNos[i] == TRUE)
				MsgValue = MsgValue | (1 << (i-65)); 
		}
		RegWrite(Bus,0x64,MsgValue,CardId);
		MsgValue = 0x0;

		for(i=81;i<=96;i++)
		{
			if(MsgNos[i] == TRUE)
				MsgValue = MsgValue | (1 << (i-81)); 
		}
		RegWrite(Bus,0x65,MsgValue,CardId);
		MsgValue = 0x0;

		for(i=97;i<=112;i++)
		{
			if(MsgNos[i] == TRUE)
				MsgValue = MsgValue | (1 << (i-97)); 
		}
		RegWrite(Bus,0x66,MsgValue,CardId);
		MsgValue = 0x0;

		for(i=113;i<=128;i++)
		{
			if(MsgNos[i] == TRUE)
				MsgValue = MsgValue | (1 << (i-113)); 
		}
		RegWrite(Bus,0x67,MsgValue,CardId);
		
		return TRUE;
	}
	else
		return FALSE;
}

CHAR InterMsgGap(UCHAR Bus,UCHAR MsgNo,USHORT Inter_Msg_Gap,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	STRSTR=0x0F,Offset=0x0;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
				
		if(MsgNo < 1 || MsgNo > 128)
			return (-10);
	
		Offset = (MsgNo * 4) - 2;
		MemWrite(Bus,Offset,Inter_Msg_Gap,CardId);
		return TRUE;
	}
	else
		return FALSE;
}
 
CHAR InitEnhancedInterrupts(UCHAR Bus,UCHAR Value,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);

		if(CardId==1)
		{
			if(Bus == 1)
			{
				Value = ReadPort(0x19,CardId) | Value ;
				WritePort(0x09,Value,CardId);
				//printf("Enhanced  ---- > Value in INIT_INTERRUPTS is :%04X\n",ReadPort(0x19,CardId));
			}
			else if(Bus == 2)
			{
				Value = ReadPort(0x39,CardId) | Value ;
				WritePort(0x29,Value,CardId);
			}
			else if(Bus == 3)
			{
				Value = ReadPort(0x59,CardId) | Value ;
				WritePort(0x49,Value,CardId);
			}
			else if(Bus == 4)
			{
				Value = ReadPort(0x79,CardId) | Value ;
				WritePort(0x69,Value,CardId);
			}
			else
				return (-2);
		}
		else if(CardId==2)
		{
			if(Bus == 1)
			{
				Value = ReadPort(0x19,CardId) | Value ;
				WritePort(0x09,Value,CardId);
			}
			else if(Bus == 2)
			{
				Value = ReadPort(0x39,CardId) | Value ;
				WritePort(0x29,Value,CardId);
			}
			else if(Bus == 3)
			{
				Value = ReadPort(0x59,CardId) | Value ;
				WritePort(0x49,Value,CardId);
			}
			else if(Bus == 4)
			{
				Value = ReadPort(0x79,CardId) | Value ;
				WritePort(0x69,Value,CardId);
			}
			else
				return (-2);
		}
		else if(CardId==3)
		{
			if(Bus == 1)
			{
				Value = ReadPort(0x19,CardId) | Value ;
				WritePort(0x09,Value,CardId);
			}
			else if(Bus == 2)
			{
				Value = ReadPort(0x39,CardId) | Value ;
				WritePort(0x29,Value,CardId);
			}
			else if(Bus == 3)
			{
				Value = ReadPort(0x59,CardId) | Value ;
				WritePort(0x49,Value,CardId);
			}
			else if(Bus == 4)
			{
				Value = ReadPort(0x79,CardId) | Value ;
				WritePort(0x69,Value,CardId);
			}
			else
				return (-2);
		}
		else if(CardId==4)
		{
			if(Bus == 1)
			{
				Value = ReadPort(0x19,CardId) | Value ;
				WritePort(0x09,Value,CardId);
			}
			else if(Bus == 2)
			{
				Value = ReadPort(0x39,CardId) | Value ;
				WritePort(0x29,Value,CardId);
			}
			else if(Bus == 3)
			{
				Value = ReadPort(0x59,CardId) | Value ;
				WritePort(0x49,Value,CardId);
			}
			else if(Bus == 4)
			{
				Value = ReadPort(0x79,CardId) | Value ;
				WritePort(0x69,Value,CardId);
			}
			else
				return (-2);
		}
		return TRUE;
	}
	else
		return FALSE;
}

CHAR DisableEnhancedInterrupts(UCHAR Bus,UCHAR Value,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(CardId==1)
		{
			if(Bus == 1)
			{
				Value = ReadPort(0x19,CardId) & (~Value) ;
				WritePort(0x09,Value,CardId);
			}
			else if(Bus == 2)
			{
				Value = ReadPort(0x39,CardId) & (~Value);
				WritePort(0x29,Value,CardId);
			}
			else if(Bus == 3)
			{
				Value = ReadPort(0x59,CardId) & (~Value) ;
				WritePort(0x49,Value,CardId);
			}
			else if(Bus == 4)
			{
				Value = ReadPort(0x79,CardId) & (~Value);
				WritePort(0x69,Value,CardId);
			}
			else
				return (-2);
		}
		else if(CardId==2)
		{
			if(Bus == 1)
			{
				Value = ReadPort(0x19,CardId) & (~Value) ;
				WritePort(0x09,Value,CardId);
			}
			else if(Bus == 2)
			{
				Value = ReadPort(0x39,CardId) & (~Value);
				WritePort(0x29,Value,CardId);
			}
			else if(Bus == 3)
			{
				Value = ReadPort(0x59,CardId) & (~Value) ;
				WritePort(0x49,Value,CardId);
			}
			else if(Bus == 4)
			{
				Value = ReadPort(0x79,CardId) & (~Value);
				WritePort(0x69,Value,CardId);
			}
			else
				return (-2);
		}
		else if(CardId==3)
		{
			if(Bus == 1)
			{
				Value = ReadPort(0x19,CardId) & (~Value) ;
				WritePort(0x09,Value,CardId);
			}
			else if(Bus == 2)
			{
				Value = ReadPort(0x39,CardId) & (~Value);
				WritePort(0x29,Value,CardId);
			}
			else if(Bus == 3)
			{
				Value = ReadPort(0x59,CardId) & (~Value) ;
				WritePort(0x49,Value,CardId);
			}
			else if(Bus == 4)
			{
				Value = ReadPort(0x79,CardId) & (~Value);
				WritePort(0x69,Value,CardId);
			}
			else
				return (-2);
		}
		else if(CardId==4)
		{
			if(Bus == 1)
			{
				Value = ReadPort(0x19,CardId) & (~Value) ;
				WritePort(0x09,Value,CardId);
			}
			else if(Bus == 2)
			{
				Value = ReadPort(0x39,CardId) & (~Value);
				WritePort(0x29,Value,CardId);
			}
			else if(Bus == 3)
			{
				Value = ReadPort(0x59,CardId) & (~Value) ;
				WritePort(0x49,Value,CardId);
			}
			else if(Bus == 4)
			{
				Value = ReadPort(0x79,CardId) & (~Value);
				WritePort(0x69,Value,CardId);
			}
			else
				return (-2);
		}
		return TRUE;
	}
	else
		return FALSE;
} 


LONG GetEndOfFrameCnt(UCHAR Bus,UCHAR CardId) 
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(CardId==1)
		{
			if(Bus==1)
		 		Cnt = ioctl(fd,IOCTL_CARD1_MOD1_GET_ENDOF_FRAME_COUNT,NULL);
			else if(Bus==2)
		 		Cnt = ioctl(fd,IOCTL_CARD1_MOD2_GET_ENDOF_FRAME_COUNT,NULL);
			else if(Bus==3)
		 		Cnt = ioctl(fd,IOCTL_CARD1_MOD3_GET_ENDOF_FRAME_COUNT,NULL);
			else if(Bus==4)
		 		Cnt = ioctl(fd,IOCTL_CARD1_MOD4_GET_ENDOF_FRAME_COUNT,NULL);
			else
				return (-2);
		}
		else if(CardId==2)
		{
			if(Bus==1)
				Cnt = ioctl(fd2,IOCTL_CARD2_MOD1_GET_ENDOF_FRAME_COUNT,NULL);
			else if(Bus==2)
				Cnt = ioctl(fd2,IOCTL_CARD2_MOD2_GET_ENDOF_FRAME_COUNT,NULL);
			else if(Bus==3)
		 		Cnt = ioctl(fd2,IOCTL_CARD2_MOD3_GET_ENDOF_FRAME_COUNT,NULL);
			else if(Bus==4)
		 		Cnt = ioctl(fd2,IOCTL_CARD2_MOD4_GET_ENDOF_FRAME_COUNT,NULL);
			else
				return (-2);
		}
		else if(CardId==3)
		{
			if(Bus==1)
				Cnt = ioctl(fd3,IOCTL_CARD3_MOD1_GET_ENDOF_FRAME_COUNT,NULL);
			else if(Bus==2)
				Cnt = ioctl(fd3,IOCTL_CARD3_MOD2_GET_ENDOF_FRAME_COUNT,NULL);
			else if(Bus==3)
		 		Cnt = ioctl(fd3,IOCTL_CARD3_MOD3_GET_ENDOF_FRAME_COUNT,NULL);
			else if(Bus==4)
		 		Cnt = ioctl(fd3,IOCTL_CARD3_MOD4_GET_ENDOF_FRAME_COUNT,NULL);
			else
				return (-2);
		}
		else if(CardId==4)
		{
			if(Bus==1)
				Cnt = ioctl(fd4,IOCTL_CARD4_MOD1_GET_ENDOF_FRAME_COUNT,NULL);
			else if(Bus==2)
				Cnt = ioctl(fd4,IOCTL_CARD4_MOD2_GET_ENDOF_FRAME_COUNT,NULL);
			else if(Bus==3)
		 		Cnt = ioctl(fd4,IOCTL_CARD4_MOD3_GET_ENDOF_FRAME_COUNT,NULL);
			else if(Bus==4)
		 		Cnt = ioctl(fd4,IOCTL_CARD4_MOD4_GET_ENDOF_FRAME_COUNT,NULL);
			else
				return (-2);

		}
		else
			return FALSE;
		
		return Cnt;
	}
	else
		return FALSE;
}

LONG GetMT_TTR_Roll_OverCnt(UCHAR Bus,UCHAR CardId)		
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(CardId==1)
		{		 
			if(Bus==1)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD1_GET_MT_TTR_ROLL_COUNT,NULL);                
			else if(Bus==2)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD2_GET_MT_TTR_ROLL_COUNT,NULL);       
			else if(Bus==3)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD3_GET_MT_TTR_ROLL_COUNT,NULL);       
			else if(Bus==4)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD4_GET_MT_TTR_ROLL_COUNT,NULL);       
		    	else
				return (-2);
		}
		else if(CardId==2)
		{
			if(Bus==1)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD1_GET_MT_TTR_ROLL_COUNT,NULL);                
			else if(Bus==2)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD2_GET_MT_TTR_ROLL_COUNT,NULL);
			else if(Bus==3)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD3_GET_MT_TTR_ROLL_COUNT,NULL);       
			else if(Bus==4)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD4_GET_MT_TTR_ROLL_COUNT,NULL);              
		   	else
				return (-2);
		}
		else if(CardId==3)
		{
			if(Bus==1)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD1_GET_MT_TTR_ROLL_COUNT,NULL);                
			else if(Bus==2)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD2_GET_MT_TTR_ROLL_COUNT,NULL);
			else if(Bus==3)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD3_GET_MT_TTR_ROLL_COUNT,NULL);       
			else if(Bus==4)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD4_GET_MT_TTR_ROLL_COUNT,NULL);              
		   	else
				return (-2);
		}
		else if(CardId==4)
		{
			if(Bus==1)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD1_GET_MT_TTR_ROLL_COUNT,NULL);                
			else if(Bus==2)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD2_GET_MT_TTR_ROLL_COUNT,NULL);
			else if(Bus==3)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD3_GET_MT_TTR_ROLL_COUNT,NULL);       
			else if(Bus==4)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD4_GET_MT_TTR_ROLL_COUNT,NULL);              
		   	else
				return (-2);
		}
		else
			return FALSE;
		return Cnt;
	}
	else
		return FALSE;

}

CHAR SetEndOfFrameCnt(UCHAR Bus,ULONG value,UCHAR CardId) 
						//Set End of Frame Interrupt Count to given Value;
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;
	
	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(CardId==1)
		{
			if(Bus==1)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD1_SET_ENDOF_FRAME_COUNT,value);                
			else if(Bus==2)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD2_SET_ENDOF_FRAME_COUNT,value);       
			else if(Bus==3)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD3_SET_ENDOF_FRAME_COUNT,value);       
			else if(Bus==4)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD4_SET_ENDOF_FRAME_COUNT,value);       
			else
				return (-2);
		}
		else if(CardId==2)
		{
			if(Bus==1)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD1_SET_ENDOF_FRAME_COUNT,value);                
			else if(Bus==2)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD2_SET_ENDOF_FRAME_COUNT,value);       
			else if(Bus==3)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD3_SET_ENDOF_FRAME_COUNT,value);       
			else if(Bus==4)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD4_SET_ENDOF_FRAME_COUNT,value);       
			else
				return (-2);
		}
		else if(CardId==3)
		{
			if(Bus==1)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD1_SET_ENDOF_FRAME_COUNT,value);                
			else if(Bus==2)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD2_SET_ENDOF_FRAME_COUNT,value);       
			else if(Bus==3)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD3_SET_ENDOF_FRAME_COUNT,value);       
			else if(Bus==4)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD4_SET_ENDOF_FRAME_COUNT,value);       
			else
				return (-2);
		}
		else if(CardId==4)
		{
			if(Bus==1)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD1_SET_ENDOF_FRAME_COUNT,value);                
			else if(Bus==2)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD2_SET_ENDOF_FRAME_COUNT,value);       
			else if(Bus==3)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD3_SET_ENDOF_FRAME_COUNT,value);       
			else if(Bus==4)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD4_SET_ENDOF_FRAME_COUNT,value);       
			else
				return (-2);
		}
		else
			return FALSE;
		return Cnt;	
	}
	else
		return FALSE;
}

CHAR SetMT_TTR_Roll_OverCnt(UCHAR Bus,ULONG value,UCHAR CardId)	
		//Set MT TTR Roll Over Interrupt Count to given Value;
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(CardId==1)
		{
			if(Bus==1)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD1_SET_MT_TTR_ROLL_COUNT,value);                
			else if(Bus==2)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD2_SET_MT_TTR_ROLL_COUNT,value);       
			else if(Bus==3)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD3_SET_MT_TTR_ROLL_COUNT,value);       
			else if(Bus==4)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD4_SET_MT_TTR_ROLL_COUNT,value);       
			else
				return (-2);
		}
		else if(CardId==2)
		{
			if(Bus==1)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD1_SET_MT_TTR_ROLL_COUNT,value);                
			else if(Bus==2)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD2_SET_MT_TTR_ROLL_COUNT,value);       
			else if(Bus==3)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD3_SET_MT_TTR_ROLL_COUNT,value);       
			else if(Bus==4)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD4_SET_MT_TTR_ROLL_COUNT,value);       
			else
				return (-2);
		}
		else if(CardId==3)
		{
			if(Bus==1)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD1_SET_MT_TTR_ROLL_COUNT,value);                
			else if(Bus==2)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD2_SET_MT_TTR_ROLL_COUNT,value);       
			else if(Bus==3)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD3_SET_MT_TTR_ROLL_COUNT,value);       
			else if(Bus==4)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD4_SET_MT_TTR_ROLL_COUNT,value);       
			else
				return (-2);

		}
		else if(CardId==4)
		{
			if(Bus==1)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD1_SET_MT_TTR_ROLL_COUNT,value);                
			else if(Bus==2)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD2_SET_MT_TTR_ROLL_COUNT,value);       
			else if(Bus==3)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD3_SET_MT_TTR_ROLL_COUNT,value);       
			else if(Bus==4)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD4_SET_MT_TTR_ROLL_COUNT,value);       
			else
				return (-2);
		}
		else
			return FALSE;
		return Cnt;
	}
	else
		return FALSE;
}

/************ Functions Added From Here	***********************/

CHAR ResetRt(UCHAR Bus,UCHAR Address,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(CardId ==1)
		{
			if(Bus < 1 || Bus > No_Mod_CARD1)
				return (-2);
		}
		else if(CardId ==2)
		{
			if(Bus < 1 || Bus > No_Mod_CARD2)
				return (-2);
		}
		else if(CardId ==3)
		{
			if(Bus < 1 || Bus > No_Mod_CARD3)
				return (-2);
		}
		else
		{
			if(Bus < 1 || Bus > No_Mod_CARD4)
				return (-2);
		}
		
		USHORT m,RtAddrs;

		if(((char)Address >= 0) && ((char)Address <= 15))
		{
			/*m = MemRead(Bus,0x200A,CardId);
			RtAddrs = m & (~(1<<Address));
			MemWrite(Bus,0x200A,RtAddrs,CardId);*/

			MemWrite(Bus,0x200a,0x0,CardId);
			MemWrite(Bus,0x200b,0x0,CardId);
			RegWrite(Bus,0x2d,0x1,CardId);
				
						
		}
		else if(Address >= 16 && Address <= 30)
		{
			/*Address = Address - 16;
			m = MemRead(Bus,0x200B,CardId);
			RtAddrs = m & (~(1<<Address));
			MemWrite(Bus,0x200B,RtAddrs,CardId);*/

			MemWrite(Bus,0x200a,0x0,CardId);
			MemWrite(Bus,0x200b,0x0,CardId);
			RegWrite(Bus,0x2d,0x1,CardId);
		}
		else
			return (-23);

		return TRUE;
	}
	else
		return FALSE;	
}

CHAR SetMT_CmdStack_Roll_OverCnt(UCHAR Bus,ULONG value,UCHAR CardId)			//Set MT CmdStack_Roll_Over Interrupt Count to given Value;
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		if(CardId==1)
		{
			if(Bus==1)
		 		Cnt =ioctl(fd,IOCTL_CARD1_MOD1_SET_MT_CMDSTK_ROLL_COUNT,value);                
		  	else if(Bus==2)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD2_SET_MT_CMDSTK_ROLL_COUNT,value);       
		  	else if(Bus==3)
		 		Cnt =ioctl(fd,IOCTL_CARD1_MOD3_SET_MT_CMDSTK_ROLL_COUNT,value);                
		  	else if(Bus==4)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD4_SET_MT_CMDSTK_ROLL_COUNT,value);       
		 	else
				return (-2);
		}
		else if(CardId==2)
		{
			if(Bus==1)
		 		Cnt =ioctl(fd2,IOCTL_CARD2_MOD1_SET_MT_CMDSTK_ROLL_COUNT,value);                
		  	else if(Bus==2)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD2_SET_MT_CMDSTK_ROLL_COUNT,value);       
		  	else if(Bus==3)
		 		Cnt =ioctl(fd2,IOCTL_CARD2_MOD3_SET_MT_CMDSTK_ROLL_COUNT,value);                
		  	else if(Bus==4)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD4_SET_MT_CMDSTK_ROLL_COUNT,value);       
		 	else
				return (-2);
		}
		else if(CardId==3)
		{
			if(Bus==1)
		 		Cnt =ioctl(fd3,IOCTL_CARD3_MOD1_SET_MT_CMDSTK_ROLL_COUNT,value);                
		  	else if(Bus==2)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD2_SET_MT_CMDSTK_ROLL_COUNT,value);       
		  	else if(Bus==3)
		 		Cnt =ioctl(fd3,IOCTL_CARD3_MOD3_SET_MT_CMDSTK_ROLL_COUNT,value);                
		  	else if(Bus==4)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD4_SET_MT_CMDSTK_ROLL_COUNT,value);       
		 	else
				return (-2);
		}
		else if(CardId==4)
		{
			if(Bus==1)
		 		Cnt =ioctl(fd4,IOCTL_CARD4_MOD1_SET_MT_CMDSTK_ROLL_COUNT,value);                
		  	else if(Bus==2)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD2_SET_MT_CMDSTK_ROLL_COUNT,value);       
		  	else if(Bus==3)
		 		Cnt =ioctl(fd4,IOCTL_CARD4_MOD3_SET_MT_CMDSTK_ROLL_COUNT,value);                
		  	else if(Bus==4)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD4_SET_MT_CMDSTK_ROLL_COUNT,value);       
		 	else
				return (-2);
		}
		else
			return FALSE;
		return Cnt;
	}
	else
		return FALSE;
}

CHAR SetMT_DataStack_Roll_OverCnt(UCHAR Bus,ULONG value,UCHAR CardId)			//Set MT DataStack_Roll_Over Interrupt Count to given Value;
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(CardId==1)
		{
		  if(Bus==1)
		 	Cnt =ioctl(fd,IOCTL_CARD1_MOD1_SET_MT_DATASTK_ROLL_COUNT,value);                
		  else if(Bus==2)
			Cnt =ioctl(fd,IOCTL_CARD1_MOD2_SET_MT_DATASTK_ROLL_COUNT,value);       
		  else if(Bus==3)
		 	Cnt =ioctl(fd,IOCTL_CARD1_MOD3_SET_MT_DATASTK_ROLL_COUNT,value);                
		  else if(Bus==4)
			Cnt =ioctl(fd,IOCTL_CARD1_MOD4_SET_MT_DATASTK_ROLL_COUNT,value);       
		  else
			return (-2);
		}
		else if(CardId==2)
		{
		  if(Bus==1)
		 	Cnt =ioctl(fd2,IOCTL_CARD2_MOD1_SET_MT_DATASTK_ROLL_COUNT,value);                
		  else if(Bus==2)
			Cnt =ioctl(fd2,IOCTL_CARD2_MOD2_SET_MT_DATASTK_ROLL_COUNT,value);       
		  else if(Bus==3)
		 	Cnt =ioctl(fd2,IOCTL_CARD2_MOD3_SET_MT_DATASTK_ROLL_COUNT,value);                
		  else if(Bus==4)
			Cnt =ioctl(fd2,IOCTL_CARD2_MOD4_SET_MT_DATASTK_ROLL_COUNT,value);       
		  else
			return (-2);
		}
		else if(CardId==3)
		{
		  if(Bus==1)
		 	Cnt =ioctl(fd3,IOCTL_CARD3_MOD1_SET_MT_DATASTK_ROLL_COUNT,value);                
		  else if(Bus==2)
			Cnt =ioctl(fd3,IOCTL_CARD3_MOD2_SET_MT_DATASTK_ROLL_COUNT,value);       
		  else if(Bus==3)
		 	Cnt =ioctl(fd3,IOCTL_CARD3_MOD3_SET_MT_DATASTK_ROLL_COUNT,value);                
		  else if(Bus==4)
			Cnt =ioctl(fd3,IOCTL_CARD3_MOD4_SET_MT_DATASTK_ROLL_COUNT,value);       
		  else
			return (-2);
		}
		else if(CardId==4)
		{
		  if(Bus==1)
		 	Cnt =ioctl(fd4,IOCTL_CARD4_MOD1_SET_MT_DATASTK_ROLL_COUNT,value);                
		  else if(Bus==2)
			Cnt =ioctl(fd4,IOCTL_CARD4_MOD2_SET_MT_DATASTK_ROLL_COUNT,value);       
		  else if(Bus==3)
		 	Cnt =ioctl(fd4,IOCTL_CARD4_MOD3_SET_MT_DATASTK_ROLL_COUNT,value);                
		  else if(Bus==4)
			Cnt =ioctl(fd4,IOCTL_CARD4_MOD4_SET_MT_DATASTK_ROLL_COUNT,value);       
		  else
			return (-2);
		}

		else
			return FALSE;
		return Cnt;
	}
	else
		return FALSE;
}


LONG GetMT_CmdStack_Roll_OverCnt(UCHAR Bus,UCHAR CardId)		//Get MT CmdStack Roll Over Interrupt count
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
		  return (-2);
		if(CardId==1)
		{		 
			if(Bus==1)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD1_GET_MT_CMDSTK_ROLL_COUNT,NULL);                
			else if(Bus==2)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD2_GET_MT_CMDSTK_ROLL_COUNT,NULL);       
			else if(Bus==3)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD3_GET_MT_CMDSTK_ROLL_COUNT,NULL);       
			else if(Bus==4)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD4_GET_MT_CMDSTK_ROLL_COUNT,NULL);       
			else
				return (-2);
		}
		else if(CardId==2)
		{		 
			if(Bus==1)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD1_GET_MT_CMDSTK_ROLL_COUNT,NULL);                
			else if(Bus==2)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD2_GET_MT_CMDSTK_ROLL_COUNT,NULL);       
			else if(Bus==3)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD3_GET_MT_CMDSTK_ROLL_COUNT,NULL);       
			else if(Bus==4)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD4_GET_MT_CMDSTK_ROLL_COUNT,NULL);       
			else
				return (-2);
		}
		else if(CardId == 3)
		{		 
			if(Bus == 1)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD1_GET_MT_CMDSTK_ROLL_COUNT,NULL);                
			else if(Bus == 2)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD2_GET_MT_CMDSTK_ROLL_COUNT,NULL);       
			else if(Bus == 3)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD3_GET_MT_CMDSTK_ROLL_COUNT,NULL);       
			else if(Bus == 4)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD4_GET_MT_CMDSTK_ROLL_COUNT,NULL);       
			else
				return (-2);
		}
		else if(CardId==4)
		{		 
			if(Bus==1)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD1_GET_MT_CMDSTK_ROLL_COUNT,NULL);                
			else if(Bus==2)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD2_GET_MT_CMDSTK_ROLL_COUNT,NULL);       
			else if(Bus==3)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD3_GET_MT_CMDSTK_ROLL_COUNT,NULL);       
			else if(Bus==4)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD4_GET_MT_CMDSTK_ROLL_COUNT,NULL);       
			else
				return (-2);
		}
		else
			return FALSE;
		return Cnt;
	}
	else
		return FALSE;

}

LONG GetMT_DataStack_Roll_OverCnt(UCHAR Bus,UCHAR CardId)		//Get MT DataStack Roll Over Interrupt count
{
	BOOL	CardFlag = FALSE;
	ULONG	Cnt;

	if(CardId !=1 && CardId !=2 && CardId !=3 && CardId !=4)
		return FALSE;
	else
	{
		if(CardId ==1)
			CardFlag = Card1;
		else if(CardId ==2)
			CardFlag = Card2;
		else if(CardId ==3)
			CardFlag = Card3;
		else if(CardId ==4)
			CardFlag = Card4;
	}
	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);
		if(CardId==1)
		{		 
			if(Bus==1)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD1_GET_MT_DATASTK_ROLL_COUNT,NULL);                
			else if(Bus==2)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD2_GET_MT_DATASTK_ROLL_COUNT,NULL);
			else if(Bus==3)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD3_GET_MT_DATASTK_ROLL_COUNT,NULL);       
			else if(Bus==4)
				Cnt =ioctl(fd,IOCTL_CARD1_MOD4_GET_MT_DATASTK_ROLL_COUNT,NULL);              
			else
				return (-2);
		}
		else if(CardId==2)
		{		 
			if(Bus==1)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD1_GET_MT_DATASTK_ROLL_COUNT,NULL);                
			else if(Bus==2)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD2_GET_MT_DATASTK_ROLL_COUNT,NULL);
			else if(Bus==3)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD3_GET_MT_DATASTK_ROLL_COUNT,NULL);       
			else if(Bus==4)
				Cnt =ioctl(fd2,IOCTL_CARD2_MOD4_GET_MT_DATASTK_ROLL_COUNT,NULL);              
			else
				return (-2);
		}
		else if(CardId==3)
		{		 
			if(Bus==1)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD1_GET_MT_DATASTK_ROLL_COUNT,NULL);                
			else if(Bus==2)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD2_GET_MT_DATASTK_ROLL_COUNT,NULL);
			else if(Bus==3)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD3_GET_MT_DATASTK_ROLL_COUNT,NULL);       
			else if(Bus==4)
				Cnt =ioctl(fd3,IOCTL_CARD3_MOD4_GET_MT_DATASTK_ROLL_COUNT,NULL);              
			else
				return (-2);
		}
		else if(CardId==4)
		{		 
			if(Bus==1)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD1_GET_MT_DATASTK_ROLL_COUNT,NULL);                
			else if(Bus==2)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD2_GET_MT_DATASTK_ROLL_COUNT,NULL);
			else if(Bus==3)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD3_GET_MT_DATASTK_ROLL_COUNT,NULL);       
			else if(Bus==4)
				Cnt =ioctl(fd4,IOCTL_CARD4_MOD4_GET_MT_DATASTK_ROLL_COUNT,NULL);              
			else
				return (-2);
		}
		else
			return FALSE;
		return Cnt;
	}
	else
		return FALSE;
}

/************************** Circular Buffer	***************************/

SHORT DefBuffSize(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,UCHAR t_r,USHORT size,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else if(CardId ==5)
		CardFlag = Card5;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);	

		USHORT CirSub,CntrlRegData;
		ULONG  CirOffset = 0x0,CmdStackPtr=0x0,CntrlReg;
		
		if(((char)RtNo<0) || ((char)RtNo>30))
			return (-23);
		
		if(subaddr > 0 && subaddr < 16)
		{
			CmdStackPtr = 0x2002 + (RtNo * 0x1E00);
			CirSub = MemRead(Bus,CmdStackPtr,CardId);
			CirSub = CirSub | (1 << subaddr);
			MemWrite(Bus,CmdStackPtr,CirSub,CardId);
		}
		else if(subaddr > 15 && subaddr < 31)
		{
			CmdStackPtr = 0x2003 + (RtNo * 0x1E00);
			CirSub = MemRead(Bus,CmdStackPtr,CardId);
			CirSub = CirSub | (1 << (subaddr - 16));
			MemWrite(Bus,CmdStackPtr,CirSub,CardId);
		}
		else
			return (-13);

		CntrlReg = 0x200c + (RtNo * 0x1E00);
		CntrlRegData = MemRead(Bus,CntrlReg,CardId);
		CntrlRegData = CntrlRegData | 0x0400;
		MemWrite(Bus,CntrlReg,CntrlRegData,CardId);

		if(t_r == 0)
			CirOffset =  0x2010 + (RtNo * 0x1E00);
		else if(t_r == 1)
			CirOffset =  0x2011 + (RtNo * 0x1E00);

		size = size-1;
		
		MemWrite(Bus,CirOffset,size,CardId);

		
		/*if(t_r == 0)
			CirOffset = CirOffset + subaddr;
		else if(t_r == 1)
			CirOffset = CirOffset + 0x20 + subaddr;

		switch(size)
		{
		case 128:
			MemWrite(Bus,CirOffset,0x1,CardId);
			break;
		
		case 256:
			MemWrite(Bus,CirOffset,0x2,CardId);
			break;

		case 512:
			MemWrite(Bus,CirOffset,0x3,CardId);
			break;

		case 1024:
			MemWrite(Bus,CirOffset,0x4,CardId);
			break;

		case 2048:
			MemWrite(Bus,CirOffset,0x5,CardId);
			break;

		case 4096:
			MemWrite(Bus,CirOffset,0x6,CardId);
			break;

		case 8192:
			MemWrite(Bus,CirOffset,0x7,CardId);
			break;

		default:
			MemWrite(Bus,CirOffset,0x0,CardId);
			break;
		}		*/
		return TRUE;
	}
	else
		return FALSE;
}

SHORT DefDataBlkCB(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,UCHAR t_r,USHORT block_num,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else if(CardId ==5)
		CardFlag = Card5;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		ULONG LOOKUPA_B,CirBlock=0x0,RxCBInitialLoct=0x0,TxCBInitialLoct=0x0,CirOffset;
		USHORT DataBlkPtr,Size;
		

		if(((char)RtNo<0) || ((char)RtNo>30))
				return (-23);

		if(subaddr<1 || subaddr>30)
			return (-13);

		if(t_r == 0)
			CirOffset =  0x2010 + (RtNo * 0x1E00);
		else if(t_r == 1)
			CirOffset =  0x2011 + (RtNo * 0x1E00);

		Size = MemRead(Bus,CirOffset,CardId);

		Size = Size+1;

		CirBlock = 0x40000 + (RtNo * 0x2000) + (Size * block_num);
		
		DataBlkPtr = (USHORT)(CirBlock >> 3);

		if(t_r == 0)
		{
			LOOKUPA_B = 0x02040 + (0x1E00 * RtNo);	
			MulRtCircSubRxBase[RtNo][subaddr] = CirBlock;

			RxCBInitialLoct = 0x02008 + (0x1E00 * RtNo);
			MemWrite(Bus,RxCBInitialLoct,(USHORT)CirBlock,CardId);
		}
		else if(t_r == 1)
		{
			LOOKUPA_B = 0x02060 + (0x1E00 * RtNo);
			MulRtCircSubTxBase[RtNo][subaddr] = CirBlock;
			
			TxCBInitialLoct = 0x02009 + (0x1E00 * RtNo);
			MemWrite(Bus,TxCBInitialLoct,(USHORT)CirBlock,CardId);
		}
		else
			return (-25);
		
		LOOKUPA_B = LOOKUPA_B + subaddr;
		MemWrite(Bus,LOOKUPA_B,DataBlkPtr,CardId);	

		return TRUE;
	}
	else
		return FALSE;
}

CHAR WriteDataBlockCB(UCHAR Bus,USHORT RtAddr,USHORT datablock,USHORT data_buff[],USHORT BlockSize,UCHAR CardId)
{
	
	BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else if(CardId ==5)
		CardFlag = Card5;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		ULONG DataBlkPtr;	
		USHORT i;
		if(((char)RtAddr<0) || ((char)RtAddr>30))
			return (-23);

		if(datablock<1 || datablock>480)
			return (-15);
		
		if(RtAddr == 7 || RtAddr == 24)
		{
			if(RtAddr == 7)
				DataBlkPtr = 0xFFFF - BlockSize;
			if(RtAddr == 24)
				DataBlkPtr = 0x2FFFF - BlockSize;
		}
		else
			DataBlkPtr = ((RtAddr * 0x1e00) + 0x397F) - BlockSize;

		DataBlkPtr = DataBlkPtr + (datablock-1)*0x20;
			
		for (i=0;i<32;i++)
			MemWrite(Bus,(DataBlkPtr + i),data_buff[i],CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR ReadDataBlockCB(UCHAR Bus,USHORT RtAddr,USHORT datablock,USHORT *data_buff,USHORT BlockSize,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else if(CardId ==5)
		CardFlag = Card5;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		ULONG DataBlkPtr;	
		USHORT i;

		if(((char)RtAddr<0) || ((char)RtAddr>30))
			return (-23);

		if(datablock<1 || datablock>480) //later changes the no. of blocks for new card
			return (-15);
		
		if(RtAddr == 7 || RtAddr == 24)
		{
			if(RtAddr == 7)
				DataBlkPtr = 0xFFFF - BlockSize;
			if(RtAddr == 24)
				DataBlkPtr = 0x2FFFF - BlockSize;
		}
		else
			DataBlkPtr = ((RtAddr * 0x1e00) + 0x397F) - BlockSize;
		DataBlkPtr = DataBlkPtr + (datablock-1)*0x20;
		
		for(i=0;i<32;i++)
			*(data_buff + i) = MemRead(Bus,(DataBlkPtr + i),CardId);
		return TRUE;
	}
	else
		return FALSE;
}

CHAR ReadDataBufferCB(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,USHORT *data_buff,USHORT Buff_Size,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else if(CardId ==5)
		CardFlag = Card5;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		ULONG i,DataBlkPtr;

		if(((char)RtNo<0) || ((char)RtNo>30))
				return (-23);

		if(subaddr<1 || subaddr>30)
			return (-13);
		
		if(Buff_Size == 128 || Buff_Size == 256 || Buff_Size == 512 || Buff_Size == 1024 || Buff_Size == 2048 || Buff_Size == 4096 || Buff_Size == 8192)
		{
		
			DataBlkPtr = MulRtCircSubRxBase[RtNo][subaddr];
				
			for(i=0;i<Buff_Size;i++)
				*(data_buff+i) = MemRead(Bus,(DataBlkPtr + i),CardId);

			return TRUE;
		}
		else
		  return (-14);
	}
	else
		return FALSE;
}

CHAR WriteDataBufferCB(UCHAR Bus,UCHAR RtNo,UCHAR subaddr,USHORT data_buff[],USHORT Buff_Size,UCHAR CardId)
{
	BOOL CardFlag = FALSE;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else if(CardId ==5)
		CardFlag = Card5;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		ULONG i,DataBlkPtr;	
		
		if(((char)RtNo<0) || ((char)RtNo>30))
				return (-23);

		if(subaddr<1 || subaddr>30)
			return (-13);
						
		if(Buff_Size == 128 || Buff_Size == 256 || Buff_Size == 512 || Buff_Size == 1024 || Buff_Size == 2048 || Buff_Size == 4096 || Buff_Size == 8192)
		{			
			DataBlkPtr = MulRtCircSubTxBase[RtNo][subaddr];

			for (i=0;i<Buff_Size;i++)
				MemWrite(Bus,(DataBlkPtr + i),data_buff[i],CardId);
		
			return TRUE;
		}
		else
			return (-14);
	}
	else
		return FALSE;

}


CHAR EnableRtChannel(UCHAR Bus,UCHAR RtNo,UCHAR Channel,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	EnableChannelA1=0x2142,EnableChannelA2=0x2143,EnableChannelB1=0x2140,EnableChannelB2=0x2141,
			ChannelA1Value,ChannelA2Value,ChannelB1Value,ChannelB2Value;

	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		if(((char)RtNo<0) || ((char)RtNo>30))
			return (-23);
		if(((char)Channel<0) || ((char)Channel>1))
			return (-25);
		
		if(Channel)		//ChannelA
		{
			if(((char)RtNo >= 0) || ((char)RtNo <= 15))
			{
				ChannelA1Value = MemRead(Bus,EnableChannelA1,CardId);
				ChannelA1Value = ChannelA1Value | (1 << RtNo);
				MemWrite(Bus,EnableChannelA1,(USHORT)ChannelA1Value,CardId);			
			}
			else
			{
				ChannelA2Value = MemRead(Bus,EnableChannelA2,CardId);
				ChannelA2Value = ChannelA2Value | (1 << RtNo);
				MemWrite(Bus,EnableChannelA2,(USHORT)ChannelA2Value,CardId);			
			}		
		}
		else			//ChannelB
		{
			if(((char)RtNo >= 0) || ((char)RtNo <= 15))
			{
				ChannelB1Value = MemRead(Bus,EnableChannelB1,CardId);
				ChannelB1Value = ChannelB1Value | (1 << RtNo);
				MemWrite(Bus,EnableChannelB1,(USHORT)ChannelB1Value,CardId);			
			}
			else
			{
				ChannelB2Value = MemRead(Bus,EnableChannelB2,CardId);
				ChannelB2Value = ChannelB2Value | (1 << RtNo);
				MemWrite(Bus,EnableChannelB2,(USHORT)ChannelB2Value,CardId);			
			}	
		}
		return TRUE;
	}
	else
		return FALSE;
}

CHAR DisableRtChannel(UCHAR Bus,UCHAR RtNo,UCHAR Channel,UCHAR CardId)
{
	BOOL	CardFlag = FALSE;
	ULONG	EnableChannelA1=0x2142,EnableChannelA2=0x2143,EnableChannelB1=0x2140,EnableChannelB2=0x2141,
			ChannelA1Value,ChannelA2Value,ChannelB1Value,ChannelB2Value,ActValue=0x0;
	
	if(CardId ==1)
		CardFlag = Card1;
	else if(CardId ==2)
		CardFlag = Card2;
	else if(CardId ==3)
		CardFlag = Card3;
	else if(CardId ==4)
		CardFlag = Card4;
	else
		return FALSE;

	if(CardFlag == TRUE)
	{
		if(Bus < 1 || Bus > 4)
			return (-2);

		if(((char)RtNo<0) || ((char)RtNo>30))
			return (-23);
		if(((char)Channel<0) || ((char)Channel>1))
			return (-25);
		
		if(Channel)		//ChannelA
		{
			if(((char)RtNo >= 0) || ((char)RtNo <= 15))
			{
				ChannelA1Value = MemRead(Bus,EnableChannelA1,CardId);
				ActValue = (1 << RtNo);
				ChannelA1Value = (~ActValue) & ChannelA1Value;				
				MemWrite(Bus,EnableChannelA1,(USHORT)ChannelA1Value,CardId);			
			}
			else
			{
				ChannelA2Value = MemRead(Bus,EnableChannelA2,CardId);
				ActValue = (1 << RtNo);
				ChannelA2Value = (~ActValue) & ChannelA2Value;				
				MemWrite(Bus,EnableChannelA2,(USHORT)ChannelA2Value,CardId);	
			}		
		}
		else			//ChannelB
		{
			if(((char)RtNo >= 0) || ((char)RtNo <= 15))
			{
				ChannelB1Value = MemRead(Bus,EnableChannelB1,CardId);
				ActValue = (1 << RtNo);
				ChannelB1Value = (~ActValue) & ChannelB1Value;				
				MemWrite(Bus,EnableChannelB1,(USHORT)ChannelB1Value,CardId);			
			}
			else
			{
				ChannelB2Value = MemRead(Bus,EnableChannelB2,CardId);
				ActValue = (1 << RtNo);
				ChannelB2Value = (~ActValue) & ChannelB2Value;				
				MemWrite(Bus,EnableChannelB2,(USHORT)ChannelB2Value,CardId);			
			}	
		}

		return TRUE;
	}
	else
		return FALSE;
}
void	EnableDriverDebug(UCHAR CardId)
{
	if(CardId==1)
		ioctl(fd,IOCTL_CARD1_DEBUG_ENABLE,NULL);
	else if(CardId==2)
		ioctl(fd2,IOCTL_CARD2_DEBUG_ENABLE,NULL);
	else if(CardId==3)
		ioctl(fd3,IOCTL_CARD3_DEBUG_ENABLE,NULL);
	else if(CardId==4)
		ioctl(fd4,IOCTL_CARD4_DEBUG_ENABLE,NULL);
}

void	DisableDriverDebug(UCHAR CardId)
{
	if(CardId==1)
		ioctl(fd,IOCTL_CARD1_DEBUG_DISABLE,NULL);
	else if(CardId==2)
		ioctl(fd2,IOCTL_CARD2_DEBUG_DISABLE,NULL);
	else if(CardId==3)
		ioctl(fd3,IOCTL_CARD3_DEBUG_DISABLE,NULL);
	else if(CardId==4)
		ioctl(fd4,IOCTL_CARD4_DEBUG_DISABLE,NULL);
}

