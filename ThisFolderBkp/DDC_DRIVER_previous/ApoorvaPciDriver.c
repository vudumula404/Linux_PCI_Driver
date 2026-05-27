/* 
	This driver is developed for 4-NODE APOORVA 1553B PCI based PC 	Add OnCard (CARD -->> 1)
        This driver is developed by Apollo Computing Labs(P) Ltd.Hyderabad.
*/   

#ifdef CONFIG_SMP
#endif

#ifndef __SMP__
	#define __SMP__
#endif

  
#ifndef __KERNEL__
	#define __KERNEL__
#endif

#ifndef __MODULE__
	#define __MODULE__
#endif


#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <asm/irq.h>
#include <linux/sched.h>

#include "ApoorvaIoctl.h"

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35) )
#include <linux/config.h>
#endif

#define		Mem_Range	0x800000


unsigned short Vendor_ID	=	0x10b5;
unsigned short Device_ID	=	0x9050;

//static 		int		IRQL=201;
//static 		int		IRQL=11;
typedef struct _buf
			{
				unsigned int Addr;
				unsigned short Data;
			}buf;
int				MajorNo,Val=-1,i;	
static	unsigned short		*Base2=NULL,*BaseDDC=NULL;
	unsigned int		BAR[5];
	unsigned char		IntrLine=201,IntrPin,*Base0;
	unsigned		Start1=0,Len1=0,PLX_IO=0,Len2=0;
static 		int		BCEOMCnt=0,MTEOMCnt=0,RTDBCACnt=0;
static 		int		BCEOMCnt2=0,MTEOMCnt2=0,RTDBCACnt2=0;
static 		int		BCEOMCnt3=0,MTEOMCnt3=0,RTDBCACnt3=0;
static 		int		BCEOMCnt4=0,MTEOMCnt4=0,RTDBCACnt4=0;
static 		int		RTC1=0,RTC2=0,RTC3=0;


unsigned	char		CARD1_DEBUG_FLAG = 0x0;

static	unsigned int		*CheckAndAllocateMemory(unsigned int MemAddr,unsigned int MemRange);

struct	pci_dev	*dev_APOORVA = NULL;
static int device_open(struct inode *inode,struct file *filp)
{
	return 0;
}

static int device_close(struct inode *inode,struct file *filp)
{
	return 0;
}

static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{

	unsigned int	*SETINTR;
	unsigned char	plx;
         //printk("ioctl_num: %x  ioctl_param(addr and Data): %lx",ioctl_num,ioctl_param);
	/*void __user *arg_user;
	arg_user = (void __user *)ioctl_param;
	union {
	int i;
	//lkmc_ioctl_struct s;
	}arg_kernel;*/
	//unsigned short buf;
	buf str;		
	
	switch(ioctl_num)
	{
	   /* case IOCTL_CARD1_MEM_READ_MODULE1:
            {
                unsigned short Data;

                if ((ioctl_param >= 0x0) && (ioctl_param <= 0x1FFFF)) {
                    Data = readw((BaseDDC) + ioctl_param);
                    printk(KERN_INFO "APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08X, Data: %X\n",
                           ((unsigned int)(BaseDDC + ioctl_param)), Data);

                    if (copy_to_user((void __user *)ioctl_param, &Data, sizeof(Data))) {
                        printk(KERN_ERR "APOORVA_PCI: Failed to copy data to user\n");
                        return -EFAULT;
                    }
                    return 0;
                } else {
                    return -EINVAL;
                }
            }
            break;*/
            /*case IOCTL_CARD1_MEM_READ_MODULE1:
            {
                unsigned short Data;
                unsigned long addr;    // Address to read from
                unsigned short __user *user_ptr;

                if (ioctl_param == 0) {
                    printk(KERN_ERR "APOORVA_PCI: Invalid user-space pointer (NULL)\n");
                    return -EINVAL;
                }

                user_ptr = (unsigned short __user *) ioctl_param;

                // ✅ First, copy the address from user-space
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                // ✅ Validate the address range
                if (addr < 0x0 || addr > 0x1FFFF) {
                    printk(KERN_ERR "APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }

                // ✅ Read the memory
                Data = readw(BaseDDC + addr);
                printk(KERN_INFO "APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

                // ✅ Copy the read data back to user-space
                if (copy_to_user(user_ptr, &Data, sizeof(Data))) {
                    printk(KERN_ERR "APOORVA_PCI: Failed to copy data to user\n");
                    return -EFAULT;
                }

                return 0;
            }*/
            case IOCTL_CARD1_MEM_READ_MODULE1:
            {
                unsigned short Data;
                unsigned long addr;
                unsigned short __user *user_ptr;

                if (ioctl_param == 0) {
                    printk(KERN_ERR "*APOORVA_PCI: Invalid user-space pointer (NULL)\n");
                    return -EINVAL;
                }

                user_ptr = (unsigned short __user *) ioctl_param;

                // ✅ Check if user-space pointer is valid
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

                // ✅ Copy address from user-space
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "***APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                if (addr < 0x0 || addr > 0x1FFFF) {
                    printk(KERN_ERR "A****POORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }

                Data = readw(BaseDDC + addr);
                printk(KERN_INFO "*****APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

                // ✅ Copy read data back to user-space
                if (copy_to_user(user_ptr, &Data, sizeof(Data))) {
                    printk(KERN_ERR "******APOORVA_PCI: Failed to copy data to user\n");
                    return -EFAULT;
                }

                return 0;
            }

        /*case IOCTL_CARD1_MEM_READ_MODULE1:
        {
         unsigned short Data;
         unsigned short __user *user_ptr; 
          if (ioctl_param == 0) {
              printk(KERN_ERR "APOORVA_PCI: Invalid user-space pointer (NULL)\n");
              return -EINVAL;
          }

          user_ptr = (unsigned short __user *) ioctl_param;  // Convert argument to user-space pointer

          if ((ioctl_param >= 0x0) && (ioctl_param <= 0x1FFFF))
          {
              Data = readw((BaseDDC) + ioctl_param);  // Read from memory

              printk(KERN_INFO "APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08X, Data: %X\n",
                     (unsigned int)(BaseDDC + ioctl_param), Data);

              // ✅ FIX: Properly copy data to user-space
              if (copy_to_user(user_ptr, &Data, sizeof(Data)))
              {
                  printk(KERN_ERR "APOORVA_PCI: Failed to copy data to user\n");
                  return -EFAULT;
              }
              return 0;
          }
          else
          {
              return -EINVAL; // Invalid address
          }
      }*/
      break;
	    case IOCTL_CARD1_MEM_WRITE_MODULE1:
            {
                Addr user_data;

                if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr))) {
                    printk(KERN_ERR "APOORVA_PCI: Failed to copy data from user\n");
                    return -EFAULT;
                }

                if ((user_data.Addr >= 0x0) && (user_data.Addr <= 0x1FFFF)) {
                    writew(user_data.Data, (BaseDDC) + (user_data.Addr));
                    printk(KERN_INFO "APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %08X, Data: %X\n",
                           (unsigned int)(BaseDDC + user_data.Addr), user_data.Data);
                } else {
                    return -EINVAL;
                }
            }
            break;
	#if 0
	case IOCTL_CARD1_MEM_READ_MODULE1:
		{
		            unsigned short Data,counter=0;
			      for(counter=0x0;counter<0x1ff;counter++)
			      {
				Data	=	readw((BaseDDC)+counter);
				printk("APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08X,Data: %X\n",((unsigned int)(Base0+counter)),Data);
			      }	
		#if 0
			if((ioctl_param >= 0x0) && (ioctl_param <= 0x1FFFF))
			{
				unsigned short Data;
				Data	=	readw((Base0)+ioctl_param);
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08X,Data: %X\n",((unsigned int)(Base0+ioctl_param)),Data);
				return (Data);
			}
			else
				return (-1);
		  #endif
		}
		break;

	case IOCTL_CARD1_MEM_WRITE_MODULE1:
		{			
		        unsigned long counter=0;
			Addr *ptr = (Addr *)ioctl_param;
			printk("APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %X\n",(Addr *)ioctl_param);
			//if((ptr->Addr >= 0x0) && (ptr->Addr <= 0x1FFFF))
			{
			 // printk("APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+ptr->Addr)),(ptr->Data));
			        printk("APOORVA_PCI: Before WRite\n");
				//writew(ptr->Data,(Base0)+(ptr->Addr));
				for(counter=0x0;counter<0x1ff;counter++)
				  writew(0x1553+counter,(BaseDDC)+counter);
				
				/*for(counter=0x0;counter<0xfffff;counter++);
				unsigned short Data;
			      for(counter=0x0;counter<0xf;counter++)
			      {
				Data	=	readw((Base0)+counter);
				printk("APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08X,Data: %X\n",((unsigned int)(Base0+counter)),Data);
			      }	*/			
				
			}			
		}
		break;
#endif
	case IOCTL_CARD1_READ_PORT:
		{
			if((ioctl_param >= 0x0) && (ioctl_param <= 0x7FFFF))
			{
				unsigned char Data;
				Data	=	inb(Start1+ioctl_param);
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_READ_PORT : ADDR: %0X,Data: %02X\n",((unsigned int)(Start1+ioctl_param)),Data);
				return (Data);
			}
			else 
				return -1;
		}
		break;

	case IOCTL_CARD1_WRITE_PORT:
		{
			copy_from_user(&str,(buf *)ioctl_param,sizeof(buf));
			if((str.Addr >= 0x0) && (str.Addr <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_PORT_WRITE_MOD_1 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+str.Addr)),(str.Data));
				outb(str.Data,Start1+(str.Addr));
			}


			/*Addr *ptr = (Addr *)ioctl_param;
	
			if((ptr->Addr >= 0x0) && (ptr->Addr <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_WRITE_PORT : ADDR: %X, DATA: %02X\n",((unsigned int)(Start1+ptr->Addr)),(ptr->Data));
				outb(ptr->Data,Start1+(ptr->Addr));
			}*/
		}
		break;

	/*case IOCTL_CARD1_MEM_READ_MODULE1:
		{
			if((ioctl_param >= 0x0) && (ioctl_param <= 0x7FFFF))
			{
				unsigned short Data;
				Data	=	readw((Base0)+ioctl_param);
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08X,Data: %X\n",((unsigned int)(Base0+ioctl_param)),Data);
				return (Data);
			}
			else
				return (-1);
		}
		break;

	case IOCTL_CARD1_MEM_WRITE_MODULE1:
		{
			unsigned short MemData[2];
			MemData[0] = ioctl_param[0];
			MemData[1] = ioctl_param[1];
			Addr *ptr = (Addr *)ioctl_param;
				printk("APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %X, DATA: %X\n",(MemData[0] ),(MemData[1] ));
			if((ptr->Addr >= 0x0) && (ptr->Addr <= 0x7FFFF))
			{
				//if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+ptr->Addr)),(ptr->Data));
				writew(ptr->Data,(Base0)+(ptr->Addr));
			}
		}
		break;*/
		
	case IOCTL_CARD1_REG_READ_MODULE1:
		{
			if((ioctl_param >= 0x0) && (ioctl_param <= 0x1FFFF))
			{
				unsigned short Data;
				Data	=	readw(Base0+(0x20000)+ioctl_param);
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_REG_READ_MOD_1: Addr :%08X,Data :%04X",((unsigned int)(Base0+(0x80000)+ioctl_param)),Data);
				return (Data);
			}
			else 
				return (-1);
		}
		break;

	case IOCTL_CARD1_REG_WRITE_MODULE1:
		{
			copy_from_user(&str,(buf *)ioctl_param,sizeof(buf));
			//printk("Arg.i APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %X %x\n",str.Addr,str.Data);
			if((str.Addr >= 0x0) && (str.Addr <= 0x1FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_REG_WRITE_MOD_1 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+str.Addr)),(str.Data));
				writew(str.Data,(Base0+(0x20000))+(str.Addr));
			}
			/*Addr *ptr = (Addr *)ioctl_param;
			
			if((ptr->Addr >= 0x0) && (ptr->Addr <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_REG_WRITE_MOD_1: Addr :%08X,Data :%04X",((unsigned int)(Base0+(0x80000)+ptr->Addr)),(ptr->Data));
				writew(ptr->Data,Base0+(0x80000)+(ptr->Addr));
			}*/
		}
		break;
		
	case IOCTL_CARD1_MEM_READ_MODULE2:
		{
			unsigned short Data;
			Data	=	readw(Base0+(0x100000)+ioctl_param);
			if((ioctl_param >= 0x0) && (ioctl_param <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_MEM_READ_MOD_2 : ADDR: %X,Data: %X\n",((unsigned int)(Base0+(0x100000)+ioctl_param)),Data);
				return (Data);
			}
			else 
				return -1;
		}
		break;
	
	case IOCTL_CARD1_MEM_WRITE_MODULE2:
		{
			copy_from_user(&str,(buf *)ioctl_param,sizeof(buf));
			if((str.Addr >= 0x0) && (str.Addr <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_MEM_WRITE_MOD_2 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+str.Addr)),(str.Data));
				writew(str.Data,(Base0+(0x100000))+(str.Addr));
			}

			/*Addr *ptr = (Addr *)ioctl_param;

			if((ptr->Addr >= 0x0) && (ptr->Addr <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_MEM_WRITE_MOD_2 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+(0x100000)+ptr->Addr)),(ptr->Data));
				writew(ptr->Data,Base0+(0x100000)+(ptr->Addr));
			}*/
		}
		break;

	case IOCTL_CARD1_REG_READ_MODULE2:
		{
			unsigned short Data;
			Data	=	readw(Base0+(0x180000)+ioctl_param);

			if((ioctl_param >= 0x0) && (ioctl_param <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_REG_READ_MOD_2 : Addr :%08X,Data :%04X",((unsigned int)(Base0+(0x180000)+ioctl_param)),Data);
				return (Data);
			}
			else 
				return -1;
		}
		break;
	
	case IOCTL_CARD1_REG_WRITE_MODULE2:
		{
			/*Addr *ptr = (Addr *)ioctl_param;
	
			if((ptr->Addr >= 0x0) && (ptr->Addr <= 0x7FFFF))
				writew(ptr->Data,Base0+(0x180000)+(ptr->Addr));*/
			copy_from_user(&str,(buf *)ioctl_param,sizeof(buf));
			if((str.Addr >= 0x0) && (str.Addr <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_REG_WRITE_MOD_2 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+str.Addr)),(str.Data));
				writew(str.Data,(Base0+(0x180000))+(str.Addr));
			}

		}
		break;
		
	case IOCTL_CARD1_MEM_READ_MODULE3:
		{
			unsigned short Data;
			Data	=	readw(Base0+(0x200000)+ioctl_param);

			if((ioctl_param >= 0x0) && (ioctl_param <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_MEM_READ_MOD_3 : ADDR: %X,Data: %X\n",((unsigned int)(Base0+0x200000+ioctl_param)),Data);
				return (Data);
			}
			else 
				return -1;
		}
		break;

	case IOCTL_CARD1_MEM_WRITE_MODULE3:
		{
			Addr *ptr = (Addr *)ioctl_param;
	
			if((ptr->Addr >= 0x0) && (ptr->Addr <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_MEM_WRITE_MOD_3 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+(0x200000)+ptr->Addr)),(ptr->Data));
				writew(ptr->Data,Base0+(0x200000)+(ptr->Addr));
			}
		}
		break;

	case IOCTL_CARD1_REG_READ_MODULE3:
		{
			unsigned short Data;
			Data	=	readw(Base0+(0x280000)+ioctl_param);

			if((ioctl_param >= 0x0) && (ioctl_param <= 0x7FFFF))
				return (Data);
			else 
				return -1;
		}
		break;

	case IOCTL_CARD1_REG_WRITE_MODULE3:
		{
			Addr *ptr = (Addr *)ioctl_param;

			if((ptr->Addr >= 0x0) && (ptr->Addr <= 0x7FFFF))
				writew(ptr->Data,Base0+(0x280000)+(ptr->Addr));
		}
		break;

	case IOCTL_CARD1_MEM_READ_MODULE4:
		{
			unsigned short Data;
			Data	=	readw(Base0+(0x300000)+ioctl_param);

			if((ioctl_param >= 0x0) && (ioctl_param <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_MEM_READ_MOD_4 : ADDR: %X,Data: %X\n",((unsigned int)(Base0+0x300000+ioctl_param)),Data);
				return (Data);
			}
			else 
				return -1;
		}
		break;

	case IOCTL_CARD1_MEM_WRITE_MODULE4:
		{
			Addr *ptr = (Addr *)ioctl_param;

			if((ptr->Addr >= 0x0) && (ptr->Addr <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_MEM_WRITE_MOD_4 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+(0x300000)+ptr->Addr)),(ptr->Data));					
				writew(ptr->Data,Base0+(0x300000)+(ptr->Addr));
			}
		}
		break;

	case IOCTL_CARD1_REG_READ_MODULE4:
		{
			unsigned short Data;
			Data	=	readw(Base0+(0x380000)+ioctl_param);

			if((ioctl_param >= 0x0) && (ioctl_param <= 0x7FFFF))
				return (Data);
			else 
				return -1;
		}
		break;

	case IOCTL_CARD1_REG_WRITE_MODULE4:
		{
			Addr *ptr = (Addr *)ioctl_param;
	
			if((ptr->Addr >= 0x0) && (ptr->Addr <= 0x7FFFF))
				writew(ptr->Data,Base0+(0x380000)+(ptr->Addr));
		}
		break;

	case IOCTL_CARD1_DEBUG_ENABLE:
		CARD1_DEBUG_FLAG = 0xFF;
		break;		

	case IOCTL_CARD1_DEBUG_DISABLE:
		CARD1_DEBUG_FLAG = 0x0;
		break;		

	case IOCTL_CARD1_SET_ICS_REG:
		writeb(0x5b,Base0+0x4c);
		writeb(0x0f,Base0+0x4d);
		//writeb(0x00,Base0+0x4d);
		//printk("ICS-REG-0x4C :%04X\n",readb(Base0+0x4c));
		//printk("ICS-REG-0x4D :%04X\n",readb(Base0+0x4d));
		break;

	case IOCTL_CARD1_SOFTWARE_RESET:
		plx = readb(Base0+0x53);
		//printk("Before---plx :%04X\n",plx);
		writeb((plx | 0x40),Base0+0x53);
		//printk("After---plx :%04X\n",plx);
		writeb(plx,Base0+0x53);
		break;
	
	case IOCTL_CARD1_MOD1_GET_BCEOMCNT:
			//if(CARD1_DEBUG_FLAG)
			//printk("IOCTL_CARD1_MOD1_GET_BCEOMCNT :%04X\n",BCEOMCnt);
			return BCEOMCnt;
	case IOCTL_CARD1_MOD1_GET_MTEOMCNT:
			//if(CARD1_DEBUG_FLAG)
			//printk("IOCTL_CARD1_MOD1_GET_MTEOMCNT :%04X\n",MTEOMCnt);
			return MTEOMCnt;

	case IOCTL_CARD1_MOD1_GET_RT0_EOMCNT:
			return MOD1RT0EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT1_EOMCNT:
			return MOD1RT1EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT2_EOMCNT:
			return MOD1RT2EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT3_EOMCNT:
			return MOD1RT3EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT4_EOMCNT:
			return MOD1RT4EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT5_EOMCNT:
			return MOD1RT5EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT6_EOMCNT:
			return MOD1RT6EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT7_EOMCNT:
			return MOD1RT7EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT8_EOMCNT:
			return MOD1RT8EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT9_EOMCNT:
			return MOD1RT9EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT10_EOMCNT:
			return MOD1RT10EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT11_EOMCNT:
			return MOD1RT11EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT12_EOMCNT:
			return MOD1RT12EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT13_EOMCNT:
			return MOD1RT13EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT14_EOMCNT:
			return MOD1RT14EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT15_EOMCNT:
			return MOD1RT15EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT16_EOMCNT:
			return MOD1RT16EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT17_EOMCNT:
			return MOD1RT17EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT18_EOMCNT:
			return MOD1RT18EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT19_EOMCNT:
			return MOD1RT19EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT20_EOMCNT:
			return MOD1RT20EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT21_EOMCNT:
			return MOD1RT21EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT22_EOMCNT:
			return MOD1RT22EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT23_EOMCNT:
			return MOD1RT23EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT24_EOMCNT:
			return MOD1RT24EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT25_EOMCNT:
			return MOD1RT25EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT26_EOMCNT:
			return MOD1RT26EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT27_EOMCNT:
			return MOD1RT27EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT28_EOMCNT:
			return MOD1RT28EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT29_EOMCNT:
			return MOD1RT29EOMCnt;
	case IOCTL_CARD1_MOD1_GET_RT30_EOMCNT:
			return MOD1RT30EOMCnt;

	case IOCTL_CARD1_MOD2_GET_RT0_EOMCNT:
			return MOD2RT0EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT1_EOMCNT:
			return MOD2RT1EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT2_EOMCNT:
			return MOD2RT2EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT3_EOMCNT:
			return MOD2RT3EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT4_EOMCNT:
			return MOD2RT4EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT5_EOMCNT:
			return MOD2RT5EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT6_EOMCNT:
			return MOD2RT6EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT7_EOMCNT:
			return MOD2RT7EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT8_EOMCNT:
			return MOD2RT8EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT9_EOMCNT:
			return MOD2RT9EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT10_EOMCNT:
			return MOD2RT10EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT11_EOMCNT:
			return MOD2RT11EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT12_EOMCNT:
			return MOD2RT12EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT13_EOMCNT:
			return MOD2RT13EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT14_EOMCNT:
			return MOD2RT14EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT15_EOMCNT:
			return MOD2RT15EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT16_EOMCNT:
			return MOD2RT16EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT17_EOMCNT:
			return MOD2RT17EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT18_EOMCNT:
			return MOD2RT18EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT19_EOMCNT:
			return MOD2RT19EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT20_EOMCNT:
			return MOD2RT20EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT21_EOMCNT:
			return MOD2RT21EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT22_EOMCNT:
			return MOD2RT22EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT23_EOMCNT:
			return MOD2RT23EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT24_EOMCNT:
			return MOD2RT24EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT25_EOMCNT:
			return MOD2RT25EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT26_EOMCNT:
			return MOD2RT26EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT27_EOMCNT:
			return MOD2RT27EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT28_EOMCNT:
			return MOD2RT28EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT29_EOMCNT:
			return MOD2RT29EOMCnt;
	case IOCTL_CARD1_MOD2_GET_RT30_EOMCNT:
			return MOD2RT30EOMCnt;

	case IOCTL_CARD1_MOD3_GET_RT0_EOMCNT:
			return MOD3RT0EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT1_EOMCNT:
			return MOD3RT1EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT2_EOMCNT:
			return MOD3RT2EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT3_EOMCNT:
			return MOD3RT3EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT4_EOMCNT:
			return MOD3RT4EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT5_EOMCNT:
			return MOD3RT5EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT6_EOMCNT:
			return MOD3RT6EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT7_EOMCNT:
			return MOD3RT7EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT8_EOMCNT:
			return MOD3RT8EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT9_EOMCNT:
			return MOD3RT9EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT10_EOMCNT:
			return MOD3RT10EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT11_EOMCNT:
			return MOD3RT11EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT12_EOMCNT:
			return MOD3RT12EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT13_EOMCNT:
			return MOD3RT13EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT14_EOMCNT:
			return MOD3RT14EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT15_EOMCNT:
			return MOD3RT15EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT16_EOMCNT:
			return MOD3RT16EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT17_EOMCNT:
			return MOD3RT17EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT18_EOMCNT:
			return MOD3RT18EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT19_EOMCNT:
			return MOD3RT19EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT20_EOMCNT:
			return MOD3RT20EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT21_EOMCNT:
			return MOD3RT21EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT22_EOMCNT:
			return MOD3RT22EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT23_EOMCNT:
			return MOD3RT23EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT24_EOMCNT:
			return MOD3RT24EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT25_EOMCNT:
			return MOD3RT25EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT26_EOMCNT:
			return MOD3RT26EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT27_EOMCNT:
			return MOD3RT27EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT28_EOMCNT:
			return MOD3RT28EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT29_EOMCNT:
			return MOD3RT29EOMCnt;
	case IOCTL_CARD1_MOD3_GET_RT30_EOMCNT:
			return MOD3RT30EOMCnt;

	case IOCTL_CARD1_MOD4_GET_RT0_EOMCNT:
			return MOD4RT0EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT1_EOMCNT:
			return MOD4RT1EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT2_EOMCNT:
			return MOD4RT2EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT3_EOMCNT:
			return MOD4RT3EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT4_EOMCNT:
			return MOD4RT4EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT5_EOMCNT:
			return MOD4RT5EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT6_EOMCNT:
			return MOD4RT6EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT7_EOMCNT:
			return MOD4RT7EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT8_EOMCNT:
			return MOD4RT8EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT9_EOMCNT:
			return MOD4RT9EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT10_EOMCNT:
			return MOD4RT10EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT11_EOMCNT:
			return MOD4RT11EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT12_EOMCNT:
			return MOD4RT12EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT13_EOMCNT:
			return MOD4RT13EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT14_EOMCNT:
			return MOD4RT14EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT15_EOMCNT:
			return MOD4RT15EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT16_EOMCNT:
			return MOD4RT16EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT17_EOMCNT:
			return MOD4RT17EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT18_EOMCNT:
			return MOD4RT18EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT19_EOMCNT:
			return MOD4RT19EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT20_EOMCNT:
			return MOD4RT20EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT21_EOMCNT:
			return MOD4RT21EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT22_EOMCNT:
			return MOD4RT22EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT23_EOMCNT:
			return MOD4RT23EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT24_EOMCNT:
			return MOD4RT24EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT25_EOMCNT:
			return MOD4RT25EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT26_EOMCNT:
			return MOD4RT26EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT27_EOMCNT:
			return MOD4RT27EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT28_EOMCNT:
			return MOD4RT28EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT29_EOMCNT:
			return MOD4RT29EOMCnt;
	case IOCTL_CARD1_MOD4_GET_RT30_EOMCNT:
			return MOD4RT30EOMCnt;

	case IOCTL_CARD1_MOD1_GET_RT0_CIRC:
			return MOD1RT0CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT1_CIRC:
			return MOD1RT1CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT2_CIRC:
			return MOD1RT2CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT3_CIRC:
			return MOD1RT3CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT4_CIRC:
			return MOD1RT4CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT5_CIRC:
			return MOD1RT5CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT6_CIRC:
			return MOD1RT6CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT7_CIRC:
			return MOD1RT7CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT8_CIRC:
			return MOD1RT8CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT9_CIRC:
			return MOD1RT9CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT10_CIRC:
			return MOD1RT10CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT11_CIRC:
			return MOD1RT11CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT12_CIRC:
			return MOD1RT12CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT13_CIRC:
			return MOD1RT13CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT14_CIRC:
			return MOD1RT14CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT15_CIRC:
			return MOD1RT15CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT16_CIRC:
			return MOD1RT16CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT17_CIRC:
			return MOD1RT17CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT18_CIRC:
			return MOD1RT18CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT19_CIRC:
			return MOD1RT19CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT20_CIRC:
			return MOD1RT20CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT21_CIRC:
			return MOD1RT21CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT22_CIRC:
			return MOD1RT22CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT23_CIRC:
			return MOD1RT23CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT24_CIRC:
			return MOD1RT24CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT25_CIRC:
			return MOD1RT25CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT26_CIRC:
			return MOD1RT26CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT27_CIRC:
			return MOD1RT27CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT28_CIRC:
			return MOD1RT28CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT29_CIRC:
			return MOD1RT29CIRCCnt;
	case IOCTL_CARD1_MOD1_GET_RT30_CIRC:
			return MOD1RT30CIRCCnt;

	case IOCTL_CARD1_MOD2_GET_RT0_CIRC:
			return MOD2RT0CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT1_CIRC:
			return MOD2RT1CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT2_CIRC:
			return MOD2RT2CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT3_CIRC:
			return MOD2RT3CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT4_CIRC:
			return MOD2RT4CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT5_CIRC:
			return MOD2RT5CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT6_CIRC:
			return MOD2RT6CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT7_CIRC:
			return MOD2RT7CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT8_CIRC:
			return MOD2RT8CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT9_CIRC:
			return MOD2RT9CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT10_CIRC:
			return MOD2RT10CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT11_CIRC:
			return MOD2RT11CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT12_CIRC:
			return MOD2RT12CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT13_CIRC:
			return MOD2RT13CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT14_CIRC:
			return MOD2RT14CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT15_CIRC:
			return MOD2RT15CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT16_CIRC:
			return MOD2RT16CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT17_CIRC:
			return MOD2RT17CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT18_CIRC:
			return MOD2RT18CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT19_CIRC:
			return MOD2RT19CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT20_CIRC:
			return MOD2RT20CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT21_CIRC:
			return MOD2RT21CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT22_CIRC:
			return MOD2RT22CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT23_CIRC:
			return MOD2RT23CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT24_CIRC:
			return MOD2RT24CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT25_CIRC:
			return MOD2RT25CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT26_CIRC:
			return MOD2RT26CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT27_CIRC:
			return MOD2RT27CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT28_CIRC:
			return MOD2RT28CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT29_CIRC:
			return MOD2RT29CIRCCnt;
	case IOCTL_CARD1_MOD2_GET_RT30_CIRC:
			return MOD2RT30CIRCCnt;

	case IOCTL_CARD1_MOD3_GET_RT0_CIRC:
			return MOD3RT0CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT1_CIRC:
			return MOD3RT1CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT2_CIRC:
			return MOD3RT2CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT3_CIRC:
			return MOD3RT3CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT4_CIRC:
			return MOD3RT4CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT5_CIRC:
			return MOD3RT5CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT6_CIRC:
			return MOD3RT6CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT7_CIRC:
			return MOD3RT7CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT8_CIRC:
			return MOD3RT8CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT9_CIRC:
			return MOD3RT9CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT10_CIRC:
			return MOD3RT10CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT11_CIRC:
			return MOD3RT11CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT12_CIRC:
			return MOD3RT12CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT13_CIRC:
			return MOD3RT13CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT14_CIRC:
			return MOD3RT14CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT15_CIRC:
			return MOD3RT15CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT16_CIRC:
			return MOD3RT16CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT17_CIRC:
			return MOD3RT17CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT18_CIRC:
			return MOD3RT18CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT19_CIRC:
			return MOD3RT19CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT20_CIRC:
			return MOD3RT20CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT21_CIRC:
			return MOD3RT21CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT22_CIRC:
			return MOD3RT22CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT23_CIRC:
			return MOD3RT23CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT24_CIRC:
			return MOD3RT24CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT25_CIRC:
			return MOD3RT25CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT26_CIRC:
			return MOD3RT26CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT27_CIRC:
			return MOD3RT27CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT28_CIRC:
			return MOD3RT28CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT29_CIRC:
			return MOD3RT29CIRCCnt;
	case IOCTL_CARD1_MOD3_GET_RT30_CIRC:
			return MOD3RT30CIRCCnt;

	case IOCTL_CARD1_MOD4_GET_RT0_CIRC:
			return MOD4RT0CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT1_CIRC:
			return MOD4RT1CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT2_CIRC:
			return MOD4RT2CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT3_CIRC:
			return MOD4RT3CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT4_CIRC:
			return MOD4RT4CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT5_CIRC:
			return MOD4RT5CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT6_CIRC:
			return MOD4RT6CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT7_CIRC:
			return MOD4RT7CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT8_CIRC:
			return MOD4RT8CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT9_CIRC:
			return MOD4RT9CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT10_CIRC:
			return MOD4RT10CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT11_CIRC:
			return MOD4RT11CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT12_CIRC:
			return MOD4RT12CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT13_CIRC:
			return MOD4RT13CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT14_CIRC:
			return MOD4RT14CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT15_CIRC:
			return MOD4RT15CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT16_CIRC:
			return MOD4RT16CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT17_CIRC:
			return MOD4RT17CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT18_CIRC:
			return MOD4RT18CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT19_CIRC:
			return MOD4RT19CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT20_CIRC:
			return MOD4RT20CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT21_CIRC:
			return MOD4RT21CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT22_CIRC:
			return MOD4RT22CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT23_CIRC:
			return MOD4RT23CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT24_CIRC:
			return MOD4RT24CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT25_CIRC:
			return MOD4RT25CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT26_CIRC:
			return MOD4RT26CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT27_CIRC:
			return MOD4RT27CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT28_CIRC:
			return MOD4RT28CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT29_CIRC:
			return MOD4RT29CIRCCnt;
	case IOCTL_CARD1_MOD4_GET_RT30_CIRC:
			return MOD4RT30CIRCCnt;

	case IOCTL_CARD1_MOD1_GET_RTDBCA:
			return RTDBCACnt;

	case IOCTL_CARD1_MOD1_GET_ENDOF_FRAME_COUNT:
			return MOD1_EOFRAME;

	case IOCTL_CARD1_MOD1_GET_MT_TTR_ROLL_COUNT:
			return MOD1_MTTTROLL;

	case IOCTL_CARD1_MOD1_GET_MT_DATASTK_ROLL_COUNT:
			return MOD1_MTDATASTKROLL;

	case IOCTL_CARD1_MOD1_GET_MT_CMDSTK_ROLL_COUNT:
			return MOD1_MTCMDSTKROLL;

	case IOCTL_CARD1_MOD2_GET_BCEOMCNT:
			return BCEOMCnt2;

	case IOCTL_CARD1_MOD2_GET_MTEOMCNT:
			return MTEOMCnt2;

	case IOCTL_CARD1_MOD2_GET_RTDBCA:
			return RTDBCACnt2;
	
	case IOCTL_CARD1_MOD2_GET_ENDOF_FRAME_COUNT:
			return MOD2_EOFRAME;

	case IOCTL_CARD1_MOD2_GET_MT_TTR_ROLL_COUNT:
			return MOD2_MTTTROLL;

	case IOCTL_CARD1_MOD2_GET_MT_DATASTK_ROLL_COUNT:
			return MOD2_MTDATASTKROLL;

	case IOCTL_CARD1_MOD2_GET_MT_CMDSTK_ROLL_COUNT:
			return MOD2_MTCMDSTKROLL;

	case IOCTL_CARD1_MOD3_GET_BCEOMCNT:
			return BCEOMCnt3;
	case IOCTL_CARD1_MOD3_GET_MTEOMCNT:
			return MTEOMCnt3;

	case IOCTL_CARD1_MOD3_GET_RTDBCA:
			return RTDBCACnt3;

	case IOCTL_CARD1_MOD3_GET_ENDOF_FRAME_COUNT:
			return MOD3_EOFRAME;

	case IOCTL_CARD1_MOD3_GET_MT_TTR_ROLL_COUNT:
			return MOD3_MTTTROLL;

	case IOCTL_CARD1_MOD3_GET_MT_DATASTK_ROLL_COUNT:
			return MOD3_MTDATASTKROLL;

	case IOCTL_CARD1_MOD3_GET_MT_CMDSTK_ROLL_COUNT:
			return MOD3_MTCMDSTKROLL;
	
	case IOCTL_CARD1_MOD4_GET_BCEOMCNT:
			return BCEOMCnt4;
	case IOCTL_CARD1_MOD4_GET_MTEOMCNT:
			return MTEOMCnt4;

	case IOCTL_CARD1_MOD4_GET_RTDBCA:
			return RTDBCACnt4;

	case IOCTL_CARD1_MOD4_GET_ENDOF_FRAME_COUNT:
			return MOD4_EOFRAME;

	case IOCTL_CARD1_MOD4_GET_MT_TTR_ROLL_COUNT:
			return MOD4_MTTTROLL;
	
	case IOCTL_CARD1_MOD4_GET_MT_DATASTK_ROLL_COUNT:
			return MOD4_MTDATASTKROLL;

	case IOCTL_CARD1_MOD4_GET_MT_CMDSTK_ROLL_COUNT:
			return MOD4_MTCMDSTKROLL;
	
	case IOCTL_CARD1_GET_RTC1:
			return RTC1;
	case IOCTL_CARD1_GET_RTC2:
			return RTC2;
	case IOCTL_CARD1_GET_RTC3:
			return RTC3;


	case IOCTL_CARD1_MOD1_SET_BCEOMCNT:
			 SETINTR  = (unsigned int *)ioctl_param;
			 BCEOMCnt = (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD1_SET_MTEOMCNT:
			 SETINTR  = (unsigned int *)ioctl_param;
			 MTEOMCnt = (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD1_SET_RT0_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT0EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT1_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT1EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT2_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT2EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT3_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT3EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT4_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT4EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT5_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT5EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT6_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT6EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT7_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT7EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT8_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT8EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT9_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT9EOMCnt	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD1_SET_RT10_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT10EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT11_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT11EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT12_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT12EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT13_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT13EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT14_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT14EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT15_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT15EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT16_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT16EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT17_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT17EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT18_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT18EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT19_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT19EOMCnt	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD1_SET_RT20_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT20EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT21_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT21EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT22_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT22EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT23_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT23EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT24_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT24EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT25_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT25EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT26_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT26EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT27_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT27EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT28_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT28EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT29_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT29EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT30_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD1RT30EOMCnt	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD2_SET_RT0_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT0EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT1_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT1EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT2_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT2EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT3_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT3EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT4_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT4EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT5_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT5EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT6_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT6EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT7_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT7EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT8_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT8EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT9_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT9EOMCnt	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD2_SET_RT10_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT10EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT11_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT11EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT12_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT12EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT13_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT13EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT14_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT14EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT15_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT15EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT16_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT16EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT17_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT17EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT18_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT18EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT19_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT19EOMCnt	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD2_SET_RT20_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT20EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT21_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT21EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT22_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT22EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT23_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT23EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT24_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT24EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT25_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT25EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT26_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT26EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT27_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT27EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT28_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT28EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT29_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT29EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT30_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD2RT30EOMCnt	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD3_SET_RT0_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT0EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT1_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT1EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT2_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT2EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT3_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT3EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT4_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT4EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT5_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT5EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT6_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT6EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT7_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT7EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT8_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT8EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT9_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT9EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT10_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT10EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT11_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT11EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT12_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT12EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT13_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT13EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT14_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT14EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT15_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT15EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT16_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT16EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT17_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT17EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT18_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT18EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT19_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT19EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT20_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT20EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT21_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT21EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT22_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT22EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT23_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT23EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT24_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT24EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT25_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT25EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT26_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT26EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT27_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT27EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT28_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT28EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT29_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT29EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT30_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD3RT30EOMCnt	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD4_SET_RT0_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT0EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT1_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT1EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT2_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT2EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT3_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT3EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT4_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT4EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT5_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT5EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT6_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT6EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT7_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT7EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT8_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT8EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT9_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT9EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT10_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT10EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT11_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT11EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT12_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT12EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT13_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT13EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT14_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT14EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT15_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT15EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT16_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT16EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT17_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT17EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT18_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT18EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT19_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT19EOMCnt	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD4_SET_RT20_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT20EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT21_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT21EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT22_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT22EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT23_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT23EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT24_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT24EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT25_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT25EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT26_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT26EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT27_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT27EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT28_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT28EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT29_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT29EOMCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT30_EOMCNT:
			 SETINTR  	= (unsigned int *)ioctl_param;
			 MOD4RT30EOMCnt	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD1_SET_RT0_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT0CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT1_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT1CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT2_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT2CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT3_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT3CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT4_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT4CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT5_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT5CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT6_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT6CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT7_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT7CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT8_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT8CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT9_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT9CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT10_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT10CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT11_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT11CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT12_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT12CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT13_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT13CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT14_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT14CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT15_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT15CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT16_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT16CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT17_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT17CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT18_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT18CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT19_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT19CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT20_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT20CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT21_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT21CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT22_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT22CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT23_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT23CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT24_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT24CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT25_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT25CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT26_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT26CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT27_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT27CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT28_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT28CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT29_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT29CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD1_SET_RT30_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1RT30CIRCCnt= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD2_SET_RT0_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT0CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT1_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT1CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT2_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT2CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT3_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT3CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT4_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT4CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT5_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT5CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT6_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT6CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT7_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT7CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT8_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT8CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT9_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT9CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT10_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT10CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT11_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT11CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT12_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT12CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT13_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT13CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT14_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT14CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT15_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT15CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT16_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT16CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT17_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT17CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT18_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT18CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT19_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT19CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT20_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT20CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT21_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT21CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT22_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT22CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT23_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT23CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT24_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT24CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT25_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT25CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT26_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT26CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT27_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT27CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT28_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT28CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT29_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT29CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD2_SET_RT30_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2RT30CIRCCnt= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD3_SET_RT0_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT0CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT1_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT1CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT2_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT2CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT3_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT3CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT4_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT4CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT5_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT5CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT6_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT6CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT7_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT7CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT8_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT8CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT9_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT9CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT10_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT10CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT11_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT11CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT12_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT12CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT13_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT13CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT14_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT14CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT15_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT15CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT16_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT16CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT17_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT17CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT18_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT18CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT19_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT19CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT20_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT20CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT21_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT21CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT22_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT22CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT23_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT23CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT24_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT24CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT25_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT25CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT26_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT26CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT27_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT27CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT28_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT28CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT29_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT29CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD3_SET_RT30_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3RT30CIRCCnt= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD4_SET_RT0_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT0CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT1_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT1CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT2_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT2CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT3_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT3CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT4_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT4CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT5_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT5CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT6_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT6CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT7_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT7CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT8_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT8CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT9_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT9CIRCCnt	= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT10_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT10CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT11_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT11CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT12_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT12CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT13_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT13CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT14_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT14CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT15_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT15CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT16_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT16CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT17_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT17CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT18_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT18CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT19_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT19CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT20_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT20CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT21_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT21CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT22_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT22CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT23_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT23CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT24_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT24CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT25_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT25CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT26_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT26CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT27_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT27CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT28_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT28CIRCCnt= (unsigned int  )SETINTR;	
			 break;
	case IOCTL_CARD1_MOD4_SET_RT29_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT29CIRCCnt= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD4_SET_RT30_CIRC:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4RT30CIRCCnt= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD1_SET_RTDBCA:
			 SETINTR   	= (unsigned int *)ioctl_param;
			 RTDBCACnt 	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD1_SET_ENDOF_FRAME_COUNT:
			 SETINTR   	= (unsigned int *)ioctl_param;
			 MOD1_EOFRAME	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD1_SET_MT_TTR_ROLL_COUNT:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD1_MTTTROLL	= (unsigned int  )SETINTR;	
			 break;	
	
	case IOCTL_CARD1_MOD1_SET_MT_DATASTK_ROLL_COUNT:
			 SETINTR		= (unsigned int *)ioctl_param;
			 MOD1_MTDATASTKROLL	= (unsigned int  )SETINTR;	
			 break;	

	case IOCTL_CARD1_MOD1_SET_MT_CMDSTK_ROLL_COUNT:
			 SETINTR		= (unsigned int *)ioctl_param;
			 MOD1_MTCMDSTKROLL	= (unsigned int  )SETINTR;	
			 break;		

	case IOCTL_CARD1_MOD2_SET_BCEOMCNT:
			 SETINTR   	= (unsigned int *)ioctl_param;
			 BCEOMCnt2 	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD2_SET_MTEOMCNT:
			 SETINTR   	= (unsigned int *)ioctl_param;
			 MTEOMCnt2 	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD2_SET_RTDBCA:
			 SETINTR    	= (unsigned int *)ioctl_param;
			 RTDBCACnt2 	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD2_SET_ENDOF_FRAME_COUNT:
			 SETINTR   	= (unsigned int *)ioctl_param;
			 MOD2_EOFRAME	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD2_SET_MT_TTR_ROLL_COUNT:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD2_MTTTROLL	= (unsigned int  )SETINTR;	
			 break;	

	case IOCTL_CARD1_MOD2_SET_MT_DATASTK_ROLL_COUNT:
			 SETINTR		= (unsigned int *)ioctl_param;
			 MOD2_MTDATASTKROLL	= (unsigned int  )SETINTR;	
			 break;	

	case IOCTL_CARD1_MOD2_SET_MT_CMDSTK_ROLL_COUNT:
			 SETINTR		= (unsigned int *)ioctl_param;
			 MOD2_MTCMDSTKROLL	= (unsigned int  )SETINTR;	
			 break;		
	
	case IOCTL_CARD1_MOD3_SET_BCEOMCNT:
			 SETINTR  = (unsigned int *)ioctl_param;
			 BCEOMCnt3 = (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD3_SET_MTEOMCNT:
			 SETINTR  = (unsigned int *)ioctl_param;
			 MTEOMCnt3 = (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD3_SET_RTDBCA:
			 SETINTR   = (unsigned int *)ioctl_param;
			 RTDBCACnt3 = (unsigned int  )SETINTR;	
			 break;
	
	case IOCTL_CARD1_MOD3_SET_ENDOF_FRAME_COUNT:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3_EOFRAME	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD3_SET_MT_TTR_ROLL_COUNT:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD3_MTTTROLL	= (unsigned int  )SETINTR;	
			 break;	

	case IOCTL_CARD1_MOD3_SET_MT_DATASTK_ROLL_COUNT:
			 SETINTR		= (unsigned int *)ioctl_param;
			 MOD3_MTDATASTKROLL	= (unsigned int  )SETINTR;	
			 break;	
	
	case IOCTL_CARD1_MOD3_SET_MT_CMDSTK_ROLL_COUNT:
			 SETINTR		= (unsigned int *)ioctl_param;
			 MOD3_MTCMDSTKROLL	= (unsigned int  )SETINTR;	
			 break;		

	case IOCTL_CARD1_MOD4_SET_BCEOMCNT:
			 SETINTR   = (unsigned int *)ioctl_param;
			 BCEOMCnt4 = (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD4_SET_MTEOMCNT:
			 SETINTR   = (unsigned int *)ioctl_param;
			 MTEOMCnt4 = (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD4_SET_RTDBCA:
			 SETINTR    = (unsigned int *)ioctl_param;
			 RTDBCACnt4 = (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD4_SET_ENDOF_FRAME_COUNT:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4_EOFRAME	= (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_MOD4_SET_MT_TTR_ROLL_COUNT:
			 SETINTR	= (unsigned int *)ioctl_param;
			 MOD4_MTTTROLL	= (unsigned int  )SETINTR;	
			 break;
	
	case IOCTL_CARD1_MOD4_SET_MT_DATASTK_ROLL_COUNT:
			 SETINTR		= (unsigned int *)ioctl_param;
			 MOD4_MTDATASTKROLL	= (unsigned int  )SETINTR;	
			 break;		

	case IOCTL_CARD1_MOD4_SET_MT_CMDSTK_ROLL_COUNT:
			 SETINTR		= (unsigned int *)ioctl_param;
			 MOD4_MTCMDSTKROLL	= (unsigned int  )SETINTR;	
			 break;		

	case IOCTL_CARD1_SET_RTC1:
			 SETINTR   = (unsigned int *)ioctl_param;
			 RTC1	   = (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_SET_RTC2:
			 SETINTR   = (unsigned int *)ioctl_param;
			 RTC2      = (unsigned int  )SETINTR;	
			 break;

	case IOCTL_CARD1_SET_RTC3:
			 SETINTR   = (unsigned int *)ioctl_param;
			 RTC3      = (unsigned int  )SETINTR;	
			 break;

	}

	return 0;
}

static struct file_operations fops = {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35) )
  ioctl	: device_ioctl,
#else
  unlocked_ioctl: device_ioctl,
#endif
open 	: device_open,
release : device_close
};
/*static struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = device_ioctl,  // Correct for kernels 2.6.35 and later
    .open = device_open,
    .release = device_close
};*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18) )
  int ISR_FOR_CARD1(int irq,void *dev_id,struct pt_regs *regs)	
#elseif (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35) )
  static irqreturn_t ISR(int irq,void *dev_id)
#else 
 static irqreturn_t ISR(int irq,void *dev_id,struct pt_regs *regs)	
#endif
//void ISR(int irq,void *dev_id,struct pt_regs *regs)	
{
	unsigned short		Temp=0,LINT=0;
	unsigned char		Intr1=0,Intr2,Intr3=0,Intr4=0,Mod_1_3=0,Mod_2_4=0;
	unsigned char		RTMod1=0,EnhancedIntr1=0,RTMod2=0,EnhancedIntr2=0,RTMod3=0,EnhancedIntr3=0,RTMod4=0,EnhancedIntr4=0;

	dev_id			= NULL;

	LINT = readb(Base0+0x4c);

	//printk("ISR1  LINT : %x\n",LINT));

	if( (LINT&0x20)==0x20) //LINT Status
	{
		Mod_1_3		=	inb(Start1+0x10);				//0th bit is '1'---- >Module 1 and 1st bit is '1' ---- >Module 3

		//printk("Addr :0x10 -----> Value: %04X\n",Mod_1_3);

		if((Mod_1_3 & 0x01)	==	0x01)
		{
			//printk("Module 1 Has been selected\n");

			Intr1			=	inb(Start1+0x11);		//read Normal interrupt status
			EnhancedIntr1		=	inb(Start1+0x12);		//read Enhanced interrupts status
			RTMod1			=	inb(Start1+0x15);		//read RT-Address
			RTMod1			&=	0x1F;

			/*printk("Intr1 :0x11 -----> Value: %04X\n",Intr1);
			printk("EnhancedIntr1 :0x12 -----> Value: %04X\n",EnhancedIntr1);
			printk("RTMod1 :0x15 -----> Value: %04X\n",RTMod1);*/

			if(Intr1 != 0x0)
			{
		    		if(Intr1 & 0x01)
				   	BCEOMCnt++;
				
				if(Intr1 & 0x02)
				   	MTEOMCnt++;

		    		if(Intr1 & 0x04)
				{
					switch(RTMod1)
					{
						case 0x0:
							MOD1RT0EOMCnt++;
							break;
						case 0x1:
							MOD1RT1EOMCnt++;
							break;
						case 0x2:
							MOD1RT2EOMCnt++;
							break;
						case 0x3:
							MOD1RT3EOMCnt++;
							break;
						case 0x4:
							MOD1RT4EOMCnt++;
							break;
						case 0x5:
							MOD1RT5EOMCnt++;
							break;
						case 0x6:
							MOD1RT6EOMCnt++;
							break;
						case 0x7:
							MOD1RT7EOMCnt++;
							break;
						case 0x8:
							MOD1RT8EOMCnt++;
							break;
						case 0x9:
							MOD1RT9EOMCnt++;
							break;
						case 0xA:
							MOD1RT10EOMCnt++;
							break;
						case 0xB:
							MOD1RT11EOMCnt++;
							break;
						case 0xC:
							MOD1RT12EOMCnt++;
							break;
						case 0xD:
							MOD1RT13EOMCnt++;
							break;
						case 0xE:
							MOD1RT14EOMCnt++;
							break;
						case 0xF:
							MOD1RT15EOMCnt++;
							break;
						case 0x10:
							MOD1RT16EOMCnt++;
							break;
						case 0x11:
							MOD1RT17EOMCnt++;
							break;
						case 0x12:
							MOD1RT18EOMCnt++;
							break;
						case 0x13:
							MOD1RT19EOMCnt++;
							break;
				 		case 0x14:
							MOD1RT20EOMCnt++;
							break;
						case 0x15:
							MOD1RT21EOMCnt++;
							break;
						case 0x16:
							MOD1RT22EOMCnt++;
							break;
						case 0x17:
							MOD1RT23EOMCnt++;
							break;
						case 0x18:
							MOD1RT24EOMCnt++;
							break;
						case 0x19:
							MOD1RT25EOMCnt++;
							break;
						case 0x1A:
							MOD1RT26EOMCnt++;
							break;
						case 0x1B:
							MOD1RT27EOMCnt++;
							break;
						case 0x1C:
							MOD1RT28EOMCnt++;
							break;
						case 0x1D:
							MOD1RT29EOMCnt++;
							break;
						case 0x1E:
							MOD1RT30EOMCnt++;
							break;
						case 0x1F:
							MOD1RT0EOMCnt++;
							MOD1RT1EOMCnt++;
							MOD1RT2EOMCnt++;
							MOD1RT3EOMCnt++;
							MOD1RT4EOMCnt++;
							MOD1RT5EOMCnt++;
							MOD1RT6EOMCnt++;
							MOD1RT7EOMCnt++;
							MOD1RT8EOMCnt++;
							MOD1RT9EOMCnt++;
							MOD1RT10EOMCnt++;
							MOD1RT11EOMCnt++;
							MOD1RT12EOMCnt++;
							MOD1RT13EOMCnt++;
							MOD1RT14EOMCnt++;
							MOD1RT15EOMCnt++;
							MOD1RT16EOMCnt++;
							MOD1RT17EOMCnt++;
							MOD1RT18EOMCnt++;
							MOD1RT19EOMCnt++;
							MOD1RT20EOMCnt++;
							MOD1RT21EOMCnt++;
							MOD1RT22EOMCnt++;
							MOD1RT23EOMCnt++;
							MOD1RT24EOMCnt++;
							MOD1RT25EOMCnt++;
							MOD1RT26EOMCnt++;
							MOD1RT27EOMCnt++;
							MOD1RT28EOMCnt++;
							MOD1RT29EOMCnt++;
							MOD1RT30EOMCnt++;
							break;
					}
				
				}
	
				if(Intr1 & 0x08)
				{
					switch(RTMod1)
					{
						case 0x0:
							MOD1RT0CIRCCnt++;
							break;
						case 0x1:
							MOD1RT1CIRCCnt++;
							break;
						case 0x2:
							MOD1RT2CIRCCnt++;
							break;
						case 0x3:
							MOD1RT3CIRCCnt++;
							break;
						case 0x4:
							MOD1RT4CIRCCnt++;
							break;
						case 0x5:
							MOD1RT5CIRCCnt++;
							break;
						case 0x6:
							MOD1RT6CIRCCnt++;
							break;
						case 0x7:
							MOD1RT7CIRCCnt++;
							break;
						case 0x8:
							MOD1RT8CIRCCnt++;
							break;
						case 0x9:
							MOD1RT9CIRCCnt++;
							break;
						case 0xA:
							MOD1RT10CIRCCnt++;
							break;
						case 0xB:
							MOD1RT11CIRCCnt++;
							break;
						case 0xC:
							MOD1RT12CIRCCnt++;
							break;
						case 0xD:
							MOD1RT13CIRCCnt++;
							break;
						case 0xE:
							MOD1RT14CIRCCnt++;
							break;
						case 0xF:
							MOD1RT15CIRCCnt++;
							break;
						case 0x10:
							MOD1RT16CIRCCnt++;
							break;
						case 0x11:
							MOD1RT17CIRCCnt++;
							break;
						case 0x12:
							MOD1RT18CIRCCnt++;
							break;
						case 0x13:
							MOD1RT19CIRCCnt++;
							break;
						case 0x14:
							MOD1RT20CIRCCnt++;
							break;
						case 0x15:
							MOD1RT21CIRCCnt++;
							break;
						case 0x16:
							MOD1RT22CIRCCnt++;
							break;
						case 0x17:
							MOD1RT23CIRCCnt++;
							break;
						case 0x18:
							MOD1RT24CIRCCnt++;
							break;
						case 0x19:
							MOD1RT25CIRCCnt++;
							break;
						case 0x1A:
							MOD1RT26CIRCCnt++;
							break;
						case 0x1B:
							MOD1RT27CIRCCnt++;
							break;
						case 0x1C:
							MOD1RT28CIRCCnt++;
							break;
						case 0x1D:
							MOD1RT29CIRCCnt++;
							break;
						case 0x1E:
							MOD1RT30CIRCCnt++;
							break;
						case 0x1F:
							MOD1RT0CIRCCnt++;
							MOD1RT1CIRCCnt++;
							MOD1RT2CIRCCnt++;
							MOD1RT3CIRCCnt++;
							MOD1RT4CIRCCnt++;
							MOD1RT5CIRCCnt++;
							MOD1RT6CIRCCnt++;
							MOD1RT7CIRCCnt++;
							MOD1RT8CIRCCnt++;
							MOD1RT9CIRCCnt++;
							MOD1RT10CIRCCnt++;
							MOD1RT11CIRCCnt++;
							MOD1RT12CIRCCnt++;
							MOD1RT13CIRCCnt++;
							MOD1RT14CIRCCnt++;
							MOD1RT15CIRCCnt++;
							MOD1RT16CIRCCnt++;
							MOD1RT17CIRCCnt++;
							MOD1RT18CIRCCnt++;
							MOD1RT19CIRCCnt++;
							MOD1RT20CIRCCnt++;
							MOD1RT21CIRCCnt++;
							MOD1RT22CIRCCnt++;
							MOD1RT23CIRCCnt++;
							MOD1RT24CIRCCnt++;
							MOD1RT25CIRCCnt++;
							MOD1RT26CIRCCnt++;
							MOD1RT27CIRCCnt++;
							MOD1RT28CIRCCnt++;
							MOD1RT29CIRCCnt++;
							MOD1RT30CIRCCnt++;
							break;
					}
				}
				if(Intr1 & 0x10)
					RTDBCACnt++;

				if(Intr1 & 0x20) 
					RTC1++;

				if(Intr1 & 0x40)
					RTC2++;

				if(Intr1 & 0x80)
					RTC3++;
			
			}
			if(EnhancedIntr1 != 0x0)
			{
				if(EnhancedIntr1 & 0x01)
				{
					//printk("EOF\n");
					MOD1_EOFRAME++;	
				}
					
				if(EnhancedIntr1 & 0x02)
				{
					//printk("TTTRROLL\n");
					MOD1_MTTTROLL++;
				}
					
				if(EnhancedIntr1 & 0x04)
				{
					//printk("DATA\n");
					MOD1_MTDATASTKROLL++;
				}
					
				if(EnhancedIntr1 & 0x08)
				{
					//printk("CMD\n");
					MOD1_MTCMDSTKROLL++;
				}
			}
		}
		if((Mod_1_3 & 0x02)	==	0x02)
		{
			//printk("Module 3 Has been selected\n");

			Intr3			=	inb(Start1+0x51);		//read Normal interrupt status
			EnhancedIntr3		=	inb(Start1+0x52);		//read Enhanced interrupts status
			RTMod3			=	inb(Start1+0x55);		//read RT-Address
			RTMod3			&= 0x1F;
	
			//printk("Intr3 :0x51 -----> Value: %04X\n",Intr3);
			//printk("EnhancedIntr3 :0x52 -----> Value: %04X\n",EnhancedIntr3);
			//printk("RTMod3 :0x55 -----> Value: %04X\n",RTMod3);

			if(Intr3 != 0x0)
			{
		    		if(Intr3 & 0x01)
				   	BCEOMCnt3++;
				
				if(Intr3 & 0x02)
				   	MTEOMCnt3++;

		    		if(Intr3 & 0x04)
				{
					switch(RTMod3)
					{
						case 0x0:
							MOD3RT0EOMCnt++;
							break;
						case 0x1:
							MOD3RT1EOMCnt++;
							break;
						case 0x2:
							MOD3RT2EOMCnt++;
							break;
						case 0x3:
							MOD3RT3EOMCnt++;
							break;
						case 0x4:
							MOD3RT4EOMCnt++;
							break;
						case 0x5:
							MOD3RT5EOMCnt++;
							break;
						case 0x6:
							MOD3RT6EOMCnt++;
							break;
						case 0x7:
							MOD3RT7EOMCnt++;
							break;
						case 0x8:
							MOD3RT8EOMCnt++;
							break;
						case 0x9:
							MOD3RT9EOMCnt++;
							break;
						case 0xA:
							MOD3RT10EOMCnt++;
							break;
						case 0xB:
							MOD3RT11EOMCnt++;
							break;
						case 0xC:
							MOD3RT12EOMCnt++;
							break;
						case 0xD:
							MOD3RT13EOMCnt++;
							break;
						case 0xE:
							MOD3RT14EOMCnt++;
							break;
						case 0xF:
							MOD3RT15EOMCnt++;
							break;
						case 0x10:
							MOD3RT16EOMCnt++;
							break;
						case 0x11:
							MOD3RT17EOMCnt++;
							break;
						case 0x12:
							MOD3RT18EOMCnt++;
							break;
						case 0x13:
							MOD3RT19EOMCnt++;
							break;
						case 0x14:
							MOD3RT20EOMCnt++;
							break;
						case 0x15:
							MOD3RT21EOMCnt++;
							break;
						case 0x16:
							MOD3RT22EOMCnt++;
							break;
						case 0x17:
							MOD3RT23EOMCnt++;
							break;
						case 0x18:
							MOD3RT24EOMCnt++;
							break;
						case 0x19:
							MOD3RT25EOMCnt++;
							break;
						case 0x1A:
							MOD3RT26EOMCnt++;
							break;
						case 0x1B:
							MOD3RT27EOMCnt++;
							break;
						case 0x1C:
							MOD3RT28EOMCnt++;
							break;
						case 0x1D:
							MOD3RT29EOMCnt++;
							break;
						case 0x1E:
							MOD3RT30EOMCnt++;
							break;
						case 0x1F:
							MOD3RT0EOMCnt++;
							MOD3RT1EOMCnt++;
							MOD3RT2EOMCnt++;
							MOD3RT3EOMCnt++;
							MOD3RT4EOMCnt++;
							MOD3RT5EOMCnt++;
							MOD3RT6EOMCnt++;
							MOD3RT7EOMCnt++;
							MOD3RT8EOMCnt++;
							MOD3RT9EOMCnt++;
							MOD3RT10EOMCnt++;
							MOD3RT11EOMCnt++;
							MOD3RT12EOMCnt++;
							MOD3RT13EOMCnt++;
							MOD3RT14EOMCnt++;
							MOD3RT15EOMCnt++;
							MOD3RT16EOMCnt++;
							MOD3RT17EOMCnt++;
							MOD3RT18EOMCnt++;
							MOD3RT19EOMCnt++;
							MOD3RT20EOMCnt++;
							MOD3RT21EOMCnt++;
							MOD3RT22EOMCnt++;
							MOD3RT23EOMCnt++;
							MOD3RT24EOMCnt++;
							MOD3RT25EOMCnt++;
							MOD3RT26EOMCnt++;
							MOD3RT27EOMCnt++;
							MOD3RT28EOMCnt++;
							MOD3RT29EOMCnt++;
							MOD3RT30EOMCnt++;
							break;
					}
				}
	
				if(Intr3 & 0x08)
				{
				    	switch(RTMod3)
					{
						case 0x0:
							MOD3RT0CIRCCnt++;
							break;
						case 0x1:
							MOD3RT1CIRCCnt++;
							break;
						case 0x2:
							MOD3RT2CIRCCnt++;
							break;
						case 0x3:
							MOD3RT3CIRCCnt++;
							break;
						case 0x4:
							MOD3RT4CIRCCnt++;
							break;
						case 0x5:
							MOD3RT5CIRCCnt++;
							break;
						case 0x6:
							MOD3RT6CIRCCnt++;
							break;
						case 0x7:
							MOD3RT7CIRCCnt++;
							break;
						case 0x8:
							MOD3RT8CIRCCnt++;
							break;
						case 0x9:
							MOD3RT9CIRCCnt++;
							break;
						case 0xA:
							MOD3RT10CIRCCnt++;
							break;
						case 0xB:
							MOD3RT11CIRCCnt++;
							break;
						case 0xC:
							MOD3RT12CIRCCnt++;
							break;
						case 0xD:
							MOD3RT13CIRCCnt++;
							break;
						case 0xE:
							MOD3RT14CIRCCnt++;
							break;
						case 0xF:
							MOD3RT15CIRCCnt++;
							break;
						case 0x10:
							MOD3RT16CIRCCnt++;
							break;
						case 0x11:
							MOD3RT17CIRCCnt++;
							break;
						case 0x12:
							MOD3RT18CIRCCnt++;
							break;
						case 0x13:
							MOD3RT19CIRCCnt++;
							break;
						case 0x14:
							MOD3RT20CIRCCnt++;
							break;
						case 0x15:
							MOD3RT21CIRCCnt++;
							break;
						case 0x16:
							MOD3RT22CIRCCnt++;
							break;
						case 0x17:
							MOD3RT23CIRCCnt++;
							break;
						case 0x18:
							MOD3RT24CIRCCnt++;
							break;
						case 0x19:
							MOD3RT25CIRCCnt++;
							break;
						case 0x1A:
							MOD3RT26CIRCCnt++;
							break;
						case 0x1B:
							MOD3RT27CIRCCnt++;
							break;
						case 0x1C:
							MOD3RT28CIRCCnt++;
							break;
						case 0x1D:
							MOD3RT29CIRCCnt++;
							break;
						case 0x1E:
							MOD3RT30CIRCCnt++;
							break;
						case 0x1F:
							MOD3RT0CIRCCnt++;
							MOD3RT1CIRCCnt++;
							MOD3RT2CIRCCnt++;
							MOD3RT3CIRCCnt++;
							MOD3RT4CIRCCnt++;
							MOD3RT5CIRCCnt++;
							MOD3RT6CIRCCnt++;
							MOD3RT7CIRCCnt++;
							MOD3RT8CIRCCnt++;
							MOD3RT9CIRCCnt++;
							MOD3RT10CIRCCnt++;
							MOD3RT11CIRCCnt++;
							MOD3RT12CIRCCnt++;
							MOD3RT13CIRCCnt++;
							MOD3RT14CIRCCnt++;
							MOD3RT15CIRCCnt++;
							MOD3RT16CIRCCnt++;
							MOD3RT17CIRCCnt++;
							MOD3RT18CIRCCnt++;
							MOD3RT19CIRCCnt++;
							MOD3RT20CIRCCnt++;
							MOD3RT21CIRCCnt++;
							MOD3RT22CIRCCnt++;
							MOD3RT23CIRCCnt++;
							MOD3RT24CIRCCnt++;
							MOD3RT25CIRCCnt++;
							MOD3RT26CIRCCnt++;
							MOD3RT27CIRCCnt++;
							MOD3RT28CIRCCnt++;
							MOD3RT29CIRCCnt++;
							MOD3RT30CIRCCnt++;
							break;
					}
				}
				if(Intr3 & 0x10)
				    	RTDBCACnt3++;
			}
			if(EnhancedIntr3 != 0x0)
			{
				if(EnhancedIntr3 & 0x01)
					MOD3_EOFRAME++;
					
				if(EnhancedIntr3 & 0x02)
					MOD3_MTTTROLL++;
					
				if(EnhancedIntr3 & 0x04)
					MOD3_MTDATASTKROLL++;
					
				if(EnhancedIntr3 & 0x08)
					MOD3_MTCMDSTKROLL++;
			}
			outb(0x1,Start1+0x56); 			//Ack to local device (Apoorva)
			outb(0x0,Start1+0x56); 			//Ack to local device (Apoorva)
		}
		
		outb(0x1,Start1+0x16);      			//Acknowledge To Loc. Device
 		Temp = inb((PLX_IO)+(0x4d));
		outb(Temp|0x8,(PLX_IO)+(0x4d));			//Clears LINT2 ICSR 
		outb(0x0,Start1+0x16);				//Acknowledge To Loc.Device	

	}

	if( (LINT&0x04)==0x04)			//LINT Status
	{
		Mod_2_4			=	inb(Start1+0x30);				//0th bit is '1'---- >Module 2 and 1st bit is '1' ---- >Module 4
		//printk("Addr :0x30 -----> Value: %04X\n",Mod_2_4);
				 
		if((Mod_2_4 & 0x01)	==	0x01)
		{
			//printk("Module 2 Has been selected\n");

			Intr2			=	inb(Start1+0x31);		//read Normal interrupt status
			EnhancedIntr2		=	inb(Start1+0x32);		//read Enhanced interrupts status
			RTMod2			=	inb(Start1+0x35);		//read RT-Address
			RTMod2			&=	0x1F;

			//printk("Intr2 :0x31 -----> Value: %04X\n",Intr2);
			//printk("EnhancedIntr2 :0x32 -----> Value: %04X\n",EnhancedIntr2);
			//printk("RTMod2 :0x35 -----> Value: %04X\n",RTMod2);

			if(Intr2 != 0x0)
			{
		    		if(Intr2 & 0x01)
				   	BCEOMCnt2++;
				
				if(Intr2 & 0x02)
				   	MTEOMCnt2++;

		    		if(Intr2 & 0x04)
				{
					//printk("Intr2222-----%d\n",Intr2);
					switch(RTMod2)
					{
						case 0x0:
							MOD2RT0EOMCnt++;
							//printk("MOD2RT0EOMCnt-----%d\n",MOD2RT0EOMCnt);
							break;
						case 0x1:
							MOD2RT1EOMCnt++;
							break;
						case 0x2:
							MOD2RT2EOMCnt++;
							break;
						case 0x3:
							MOD2RT3EOMCnt++;
							break;
						case 0x4:
							MOD2RT4EOMCnt++;
							break;
						case 0x5:
							MOD2RT5EOMCnt++;
							break;
						case 0x6:
							MOD2RT6EOMCnt++;
							break;
						case 0x7:
							MOD2RT7EOMCnt++;
							break;
						case 0x8:
							MOD2RT8EOMCnt++;
							break;
						case 0x9:
							MOD2RT9EOMCnt++;
							break;
						case 0xA:
							MOD2RT10EOMCnt++;
							break;
						case 0xB:
							MOD2RT11EOMCnt++;
							break;
						case 0xC:
							MOD2RT12EOMCnt++;
							break;
						case 0xD:
							MOD2RT13EOMCnt++;
							break;
						case 0xE:
							MOD2RT14EOMCnt++;
							break;
						case 0xF:
							MOD2RT15EOMCnt++;
							break;
						case 0x10:
							MOD2RT16EOMCnt++;
							break;
						case 0x11:
							MOD2RT17EOMCnt++;
							break;
						case 0x12:
							MOD2RT18EOMCnt++;
							break;
						case 0x13:
							MOD2RT19EOMCnt++;
							break;
						case 0x14:
							MOD2RT20EOMCnt++;
							break;
						case 0x15:
							MOD2RT21EOMCnt++;
							break;
						case 0x16:
							MOD2RT22EOMCnt++;
							break;
						case 0x17:
							MOD2RT23EOMCnt++;
							break;
						case 0x18:
							MOD2RT24EOMCnt++;
							break;
						case 0x19:
							MOD2RT25EOMCnt++;
							break;
						case 0x1A:
							MOD2RT26EOMCnt++;
							break;
						case 0x1B:
							MOD2RT27EOMCnt++;
							break;
						case 0x1C:
							MOD2RT28EOMCnt++;
							break;
						case 0x1D:
							MOD2RT29EOMCnt++;
							break;
						case 0x1E:
							MOD2RT30EOMCnt++;
							break;
						case 0x1F:
							MOD2RT0EOMCnt++;
							MOD2RT1EOMCnt++;
							MOD2RT2EOMCnt++;
							MOD2RT3EOMCnt++;
							MOD2RT4EOMCnt++;
							MOD2RT5EOMCnt++;
							MOD2RT6EOMCnt++;
							MOD2RT7EOMCnt++;
							MOD2RT8EOMCnt++;
							MOD2RT9EOMCnt++;
							MOD2RT10EOMCnt++;
							MOD2RT11EOMCnt++;
							MOD2RT12EOMCnt++;
							MOD2RT13EOMCnt++;
							MOD2RT14EOMCnt++;
							MOD2RT15EOMCnt++;
							MOD2RT16EOMCnt++;
							MOD2RT17EOMCnt++;
							MOD2RT18EOMCnt++;
							MOD2RT19EOMCnt++;
							MOD2RT20EOMCnt++;
							MOD2RT21EOMCnt++;
							MOD2RT22EOMCnt++;
							MOD2RT23EOMCnt++;
							MOD2RT24EOMCnt++;
							MOD2RT25EOMCnt++;
							MOD2RT26EOMCnt++;
							MOD2RT27EOMCnt++;
							MOD2RT28EOMCnt++;
							MOD2RT29EOMCnt++;
							MOD2RT30EOMCnt++;
							//printk("RTMod2-----%X\n",RTMod2);
							break;
					}

				}
	
				if(Intr2 & 0x08)
				{
				    	switch(RTMod2)
					{
						case 0x0:
							MOD2RT0CIRCCnt++;
							break;
						case 0x1:
							MOD2RT1CIRCCnt++;
							break;
						case 0x2:
							MOD2RT2CIRCCnt++;
							break;
						case 0x3:
							MOD2RT3CIRCCnt++;
							break;
						case 0x4:
							MOD2RT4CIRCCnt++;
							break;
						case 0x5:
							MOD2RT5CIRCCnt++;
							break;
						case 0x6:
							MOD2RT6CIRCCnt++;
							break;
						case 0x7:
							MOD2RT7CIRCCnt++;
							break;
						case 0x8:
							MOD2RT8CIRCCnt++;
							break;
						case 0x9:
							MOD2RT9CIRCCnt++;
							break;
						case 0xA:
							MOD2RT10CIRCCnt++;
							break;
						case 0xB:
							MOD2RT11CIRCCnt++;
							break;
						case 0xC:
							MOD2RT12CIRCCnt++;
							break;
						case 0xD:
							MOD2RT13CIRCCnt++;
							break;
						case 0xE:
							MOD2RT14CIRCCnt++;
							break;
						case 0xF:
							MOD2RT15CIRCCnt++;
							break;
						case 0x10:
							MOD2RT16CIRCCnt++;
							break;
						case 0x11:
							MOD2RT17CIRCCnt++;
							break;
						case 0x12:
							MOD2RT18CIRCCnt++;
							break;
						case 0x13:
							MOD2RT19CIRCCnt++;
							break;
						case 0x14:
							MOD2RT20CIRCCnt++;
							break;
						case 0x15:
							MOD2RT21CIRCCnt++;
							break;
						case 0x16:
							MOD2RT22CIRCCnt++;
							break;
						case 0x17:
							MOD2RT23CIRCCnt++;
							break;
						case 0x18:
							MOD2RT24CIRCCnt++;
							break;
						case 0x19:
							MOD2RT25CIRCCnt++;
							break;
						case 0x1A:
							MOD2RT26CIRCCnt++;
							break;
						case 0x1B:
							MOD2RT27CIRCCnt++;
							break;
						case 0x1C:
							MOD2RT28CIRCCnt++;
							break;
						case 0x1D:
							MOD2RT29CIRCCnt++;
							break;
						case 0x1E:
							MOD2RT30CIRCCnt++;
							break;
						case 0x1F:
							MOD2RT0CIRCCnt++;
							MOD2RT1CIRCCnt++;
							MOD2RT2CIRCCnt++;
							MOD2RT3CIRCCnt++;
							MOD2RT4CIRCCnt++;
							MOD2RT5CIRCCnt++;
							MOD2RT6CIRCCnt++;
							MOD2RT7CIRCCnt++;
							MOD2RT8CIRCCnt++;
							MOD2RT9CIRCCnt++;
							MOD2RT10CIRCCnt++;
							MOD2RT11CIRCCnt++;
							MOD2RT12CIRCCnt++;
							MOD2RT13CIRCCnt++;
							MOD2RT14CIRCCnt++;
							MOD2RT15CIRCCnt++;
							MOD2RT16CIRCCnt++;
							MOD2RT17CIRCCnt++;
							MOD2RT18CIRCCnt++;
							MOD2RT19CIRCCnt++;
							MOD2RT20CIRCCnt++;
							MOD2RT21CIRCCnt++;
							MOD2RT22CIRCCnt++;
							MOD2RT23CIRCCnt++;
							MOD2RT24CIRCCnt++;
							MOD2RT25CIRCCnt++;
							MOD2RT26CIRCCnt++;
							MOD2RT27CIRCCnt++;
							MOD2RT28CIRCCnt++;
							MOD2RT29CIRCCnt++;
							MOD2RT30CIRCCnt++;
							break;
					}
				}
	
				if(Intr2 & 0x10)
				    	RTDBCACnt2++;
			}

			if(EnhancedIntr2 != 0x0)
			{
				if(EnhancedIntr2 & 0x01)
					MOD2_EOFRAME++;
					
				if(EnhancedIntr2 & 0x02)
					MOD2_MTTTROLL++;
					
				if(EnhancedIntr2 & 0x04)
					MOD2_MTDATASTKROLL++;
					
				if(EnhancedIntr2 & 0x08)
					MOD2_MTCMDSTKROLL++;
			}
		}
	
		if((Mod_2_4 & 0x02)	==	0x02)
		{
			//printk("Module 4 Has been selected\n");

			Intr4			=	inb(Start1+0x71);		//read Normal interrupt status
			EnhancedIntr4		=	inb(Start1+0x72);		//read Enhanced interrupts status
			RTMod4			=	inb(Start1+0x75);		//read RT-Address
			RTMod4			&= 0x1F;

			//printk("Intr4 :0x71 -----> Value: %04X\n",Intr4);
			//printk("EnhancedIntr4 :0x72 -----> Value: %04X\n",EnhancedIntr4);
			//printk("RTMod4 :0x75 -----> Value: %04X\n",RTMod4);

			if(Intr4 != 0x0)
			{
		    		if(Intr4 & 0x01)
				   	BCEOMCnt4++;
				
				if(Intr4 & 0x02)
				   	MTEOMCnt4++;

		    		if(Intr4 & 0x04)
				{
					switch(RTMod4)
					{
						case 0x0:
							MOD4RT0EOMCnt++;
							break;
						case 0x1:
							MOD4RT1EOMCnt++;
							break;
						case 0x2:
							MOD4RT2EOMCnt++;
							break;
						case 0x3:
							MOD4RT3EOMCnt++;
							break;
						case 0x4:
							MOD4RT4EOMCnt++;
							break;
						case 0x5:
							MOD4RT5EOMCnt++;
							break;
						case 0x6:
							MOD4RT6EOMCnt++;
							break;
						case 0x7:
							MOD4RT7EOMCnt++;
							break;
						case 0x8:
							MOD4RT8EOMCnt++;
							break;
						case 0x9:
							MOD4RT9EOMCnt++;
							break;
						case 0xA:
							MOD4RT10EOMCnt++;
							break;
						case 0xB:
							MOD4RT11EOMCnt++;
							break;
						case 0xC:
							MOD4RT12EOMCnt++;
							break;
						case 0xD:
							MOD4RT13EOMCnt++;
							break;
						case 0xE:
							MOD4RT14EOMCnt++;
							break;
						case 0xF:
							MOD4RT15EOMCnt++;
							break;
						case 0x10:
							MOD4RT16EOMCnt++;
							break;
						case 0x11:
							MOD4RT17EOMCnt++;
							break;
						case 0x12:
							MOD4RT18EOMCnt++;
							break;
						case 0x13:
							MOD4RT19EOMCnt++;
							break;
						case 0x14:
							MOD4RT20EOMCnt++;
							break;
						case 0x15:
							MOD4RT21EOMCnt++;
							break;
						case 0x16:
							MOD4RT22EOMCnt++;
							break;
						case 0x17:
							MOD4RT23EOMCnt++;
							break;
						case 0x18:
							MOD4RT24EOMCnt++;
							break;
						case 0x19:
							MOD4RT25EOMCnt++;
							break;
						case 0x1A:
							MOD4RT26EOMCnt++;
							break;
						case 0x1B:
							MOD4RT27EOMCnt++;
							break;
						case 0x1C:
							MOD4RT28EOMCnt++;
							break;
						case 0x1D:
							MOD4RT29EOMCnt++;
							break;
						case 0x1E:
							MOD4RT30EOMCnt++;
							break;
						case 0x1F:
							MOD4RT0EOMCnt++;
							MOD4RT1EOMCnt++;
							MOD4RT2EOMCnt++;
							MOD4RT3EOMCnt++;
							MOD4RT4EOMCnt++;
							MOD4RT5EOMCnt++;
							MOD4RT6EOMCnt++;
							MOD4RT7EOMCnt++;
							MOD4RT8EOMCnt++;
							MOD4RT9EOMCnt++;
							MOD4RT10EOMCnt++;
							MOD4RT11EOMCnt++;
							MOD4RT12EOMCnt++;
							MOD4RT13EOMCnt++;
							MOD4RT14EOMCnt++;
							MOD4RT15EOMCnt++;
							MOD4RT16EOMCnt++;
							MOD4RT17EOMCnt++;
							MOD4RT18EOMCnt++;
							MOD4RT19EOMCnt++;
							MOD4RT20EOMCnt++;
							MOD4RT21EOMCnt++;
							MOD4RT22EOMCnt++;
							MOD4RT23EOMCnt++;
							MOD4RT24EOMCnt++;
							MOD4RT25EOMCnt++;
							MOD4RT26EOMCnt++;
							MOD4RT27EOMCnt++;
							MOD4RT28EOMCnt++;
							MOD4RT29EOMCnt++;
							MOD4RT30EOMCnt++;
							break;
					}
				}
	
				if(Intr4 & 0x08)
				{
				    	switch(RTMod4)
					{
						case 0x0:
							MOD4RT0CIRCCnt++;
							break;
						case 0x1:
							MOD4RT1CIRCCnt++;
							break;
						case 0x2:
							MOD4RT2CIRCCnt++;
							break;
						case 0x3:
							MOD4RT3CIRCCnt++;
							break;
						case 0x4:
							MOD4RT4CIRCCnt++;
							break;
						case 0x5:
							MOD4RT5CIRCCnt++;
							break;
						case 0x6:
							MOD4RT6CIRCCnt++;
							break;
						case 0x7:
							MOD4RT7CIRCCnt++;
							break;
						case 0x8:
							MOD4RT8CIRCCnt++;
							break;
						case 0x9:
							MOD4RT9CIRCCnt++;
							break;
						case 0xA:
							MOD4RT10CIRCCnt++;
							break;
						case 0xB:
							MOD4RT11CIRCCnt++;
							break;
						case 0xC:
							MOD4RT12CIRCCnt++;
							break;
						case 0xD:
							MOD4RT13CIRCCnt++;
							break;
						case 0xE:
							MOD4RT14CIRCCnt++;
							break;
						case 0xF:
							MOD4RT15CIRCCnt++;
							break;
						case 0x10:
							MOD4RT16CIRCCnt++;
							break;
						case 0x11:
							MOD4RT17CIRCCnt++;
							break;
						case 0x12:
							MOD4RT18CIRCCnt++;
							break;
						case 0x13:
							MOD4RT19CIRCCnt++;
							break;
						case 0x14:
							MOD4RT20CIRCCnt++;
							break;
						case 0x15:
							MOD4RT21CIRCCnt++;
							break;
						case 0x16:
							MOD4RT22CIRCCnt++;
							break;
						case 0x17:
							MOD4RT23CIRCCnt++;
							break;
						case 0x18:
							MOD4RT24CIRCCnt++;
							break;
						case 0x19:
							MOD4RT25CIRCCnt++;
							break;
						case 0x1A:
							MOD4RT26CIRCCnt++;
							break;
						case 0x1B:
							MOD4RT27CIRCCnt++;
							break;
						case 0x1C:
							MOD4RT28CIRCCnt++;
							break;
						case 0x1D:
							MOD4RT29CIRCCnt++;
							break;
						case 0x1E:
							MOD4RT30CIRCCnt++;
							break;
						case 0x1F:
							MOD4RT0CIRCCnt++;
							MOD4RT1CIRCCnt++;
							MOD4RT2CIRCCnt++;
							MOD4RT3CIRCCnt++;
							MOD4RT4CIRCCnt++;
							MOD4RT5CIRCCnt++;
							MOD4RT6CIRCCnt++;
							MOD4RT7CIRCCnt++;
							MOD4RT8CIRCCnt++;
							MOD4RT9CIRCCnt++;
							MOD4RT10CIRCCnt++;
							MOD4RT11CIRCCnt++;
							MOD4RT12CIRCCnt++;
							MOD4RT13CIRCCnt++;
							MOD4RT14CIRCCnt++;
							MOD4RT15CIRCCnt++;
							MOD4RT16CIRCCnt++;
							MOD4RT17CIRCCnt++;
							MOD4RT18CIRCCnt++;
							MOD4RT19CIRCCnt++;
							MOD4RT20CIRCCnt++;
							MOD4RT21CIRCCnt++;
							MOD4RT22CIRCCnt++;
							MOD4RT23CIRCCnt++;
							MOD4RT24CIRCCnt++;
							MOD4RT25CIRCCnt++;
							MOD4RT26CIRCCnt++;
							MOD4RT27CIRCCnt++;
							MOD4RT28CIRCCnt++;
							MOD4RT29CIRCCnt++;
							MOD4RT30CIRCCnt++;
							break;
					}
				}
				if(Intr4 & 0x10)
				    	RTDBCACnt4++;
			}

			if(EnhancedIntr4 != 0x0)
			{
				if(EnhancedIntr4 & 0x01)
					MOD4_EOFRAME++;
					
				if(EnhancedIntr4 & 0x02)
					MOD4_MTTTROLL++;
					
				if(EnhancedIntr4 & 0x04)
					MOD4_MTDATASTKROLL++;
					
				if(EnhancedIntr4 & 0x08)
					MOD4_MTCMDSTKROLL++;
			}

			outb(0x1,Start1+0x76);			//Ack to local device (Apoorva)
			outb(0x0,Start1+0x76);			//Ack to local device (Apoorva)
		}
		outb(0x1,Start1+0x36);				//Acknowledge To Loc. Device
 		Temp = inb(PLX_IO+(0x4d));
		outb(Temp|0x4,PLX_IO+(0x4d));			//Clears LINT1 ICSR 
		outb(0x0,Start1+0x36);				//Acknowledge To Loc.Device	
		//return 0;
	}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35) )
// void return
 #else
    return IRQ_HANDLED;
#endif
}

static int __init APOORVA_CARD1_INIT(void)
{

	//struct pci_dev *dev_APOORVA = NULL;
	
	int	ret;
	//int   mi=0;
	void	*irqdevid=(void *)0x12;

	/*This function is used to scan the list of 
    	installed devices looking for a device featuring a specific signature.*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35) )
	dev_APOORVA  = pci_find_device(Vendor_ID,Device_ID,dev_APOORVA );
#else
	dev_APOORVA  = pci_get_device(Vendor_ID,Device_ID,dev_APOORVA );
#endif

	if(dev_APOORVA==NULL){
		printk("\n\nAPOORVA1553:Device not found .... \n\n ");
		return 1;
	}	
	
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,18) )
 	ret = pci_enable_device(dev_APOORVA );
	if(ret)
	{
		printk("Warning--------> Problem While Enabling the Device \n ret  is :%04X\n",ret);
	} 
#endif

	 /*Registers a charecter device driver.If the major number is not zero,it is used 	
	unchanged,if the number is 0,then a dynamic number is assigned for this device.*/
 
	MajorNo = register_chrdev(0,"APOORVA1553",&fops);
	
	if(MajorNo<0)
		printk("\nAPOORVA_PCI: APOORVA1553 Device Registration Failure\n");
	else
		printk("\nAPOORVA_PCI: APOORVA1553 Device Has Been Registered With MajorNo :[%d] .....[OK]\n",MajorNo);
	//MKDEV( MajorNo, MINOR((dev_APOORVA)));
	/* Reads And Prints 4 Base Address Registers of PCI Configuration in kernel ring buffer */

	printk("APOORVA_PCI: READING PCI BAR0................");	pci_read_config_dword(dev_APOORVA ,PCI_BASE_ADDRESS_0,&BAR[0]);		printk("[0x%8X]\n",BAR[0]);
	printk("APOORVA_PCI: READING PCI BAR1................");	pci_read_config_dword(dev_APOORVA ,PCI_BASE_ADDRESS_1,&BAR[1]);		printk("[0x%4X]\n",BAR[1]);
	printk("APOORVA_PCI: READING PCI BAR2................");	pci_read_config_dword(dev_APOORVA ,PCI_BASE_ADDRESS_2,&BAR[2]);		printk("[0x%8X]\n",BAR[2]);
	printk("APOORVA_PCI: READING PCI BAR3................");	pci_read_config_dword(dev_APOORVA ,PCI_BASE_ADDRESS_3,&BAR[3]);	
	printk("[0x%4X]\n",BAR[3]);
	printk("APOORVA_PCI: READING PCI IRQL................");	pci_read_config_byte(dev_APOORVA ,PCI_INTERRUPT_LINE,&IntrLine);
	//printk("APOORVA_PCI: READING PCI IRQL................");	pci_read_config_byte(dev_APOORVA ,0x3c,&IntrLine);
	printk("[0x%1X]\n",IntrLine);
	printk("APOORVA_PCI: READING PCI IRQ PIN.............");	pci_read_config_byte(dev_APOORVA ,PCI_INTERRUPT_PIN,&IntrPin);		
	printk("[0x%02X]\n",IntrPin);
	BAR[0] = BAR[0] & PCI_BASE_ADDRESS_MEM_MASK;
   	BAR[2] = BAR[2] & PCI_BASE_ADDRESS_MEM_MASK;
 	Start1 = BAR[3] & PCI_BASE_ADDRESS_IO_MASK;
	Len1   = 0xff ;
 	PLX_IO = BAR[1] & PCI_BASE_ADDRESS_IO_MASK;
	Len2   = 0x50 ;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18) )
	IntrLine=dev_APOORVA->irq;
	printk("APOORVA_PCI:Irq Allocation...................[%d] \n",dev_APOORVA->irq);
#endif	
	//if(request_irq(IntrLine,(void *)&ISR,SA_SAMPLE_RANDOM,"APOORVA1553",irqdevid)) //For Non SMP
	//if(request_irq(IntrLine,(void *)&ISR,SA_SHIRQ,"APOORVA1553",irqdevid)) //For Non SMP
	//if(request_irq(IntrLine,(void *)&ISR,SA_INTERRUPT,"APOORVA1553",irqdevid)) //For Non SMP
	//if(request_irq(IntrLine,&ISR,SA_SHIRQ,"APOORVA1553",irqdevid)) //For Non SMP	
	
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18) )
	if(request_irq(IRQL,ISR_FOR_CARD1,SA_SHIRQ,"APOORVA1553",irqdevid))  //For SMP	
#elseif (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35) )
	 if(request_irq(IntrLine,&ISR,SA_SHIRQ,"APOORVA1553",irqdevid)) //For Non SMP	
#else
	if(request_irq(IntrLine,(void *)&ISR,IRQF_SHARED,"APOORVA1553",irqdevid)) //For Non SMP

#endif
	{
		printk("\tIrq Allocation Failure\n");
		return 0;
	}

	BaseDDC = (unsigned short *)CheckAndAllocateMemory(BAR[0],Mem_Range/*0x60*/);
	request_region(Start1,Len1,"APOORVA1553");
	request_region(PLX_IO,Len2,"APOORVA1553");
	//Base2 = (unsigned short *)CheckAndAllocateMemory(BAR[2],Mem_Range);
	if(BaseDDC == '\0')
	{
		printk("ERROR !!!\n");
		free_irq(IntrLine,irqdevid);
		release_region(Start1,Len1);
		release_region(PLX_IO,Len2);
		return -1;
	}
	return 0;
}

static void __exit APOORVA_CARD1_EXIT(void)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35) )
	int   device;
#endif	
	
	void *irqdevid=(void *)0x12;

	/*pci_destroy_dev(dev_APOORVA );
	release_pcibus_dev(dev_APOORVA );//added vsr
	pci_free_resources(dev_APOORVA );*/
	pci_disable_device(dev_APOORVA);
	/* Unregisters an interrupt handler */
	free_irq(IntrLine,irqdevid);//needs
	
	/* Unregisters I/O Ports */
	release_region(Start1,Len1);
	release_region(PLX_IO,Len2);	
	
	/* Unregisters Memory	*/
	release_mem_region(BAR[0],Mem_Range/*0x60*/);
	//release_mem_region(BAR[2],Mem_Range);
	//iounmap(Base0);
	iounmap(BaseDDC);
	iounmap(Base2);

	/*Unregisters the driver at unload time.Both major and the name string
	must contain the same values that were used to register the driver.*/
	
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35) )
	device =unregister_chrdev(MajorNo,"APOORVA1553");

	if(device<0)
		printk("\nAPOORVA_PCI: APOORVA1553 Unregistration Failure\n");
	else
		printk("\nAPOORVA_PCI: APOORVA1553 Device Has Been Unregistered .......................[OK]\n");
#else	
	unregister_chrdev(MajorNo,"APOORVA1553");
	printk("\nAPOORVA_PCI: APOORVA1553 Device Has Been Unregistered .......................[OK]\n");
#endif	

}

static unsigned int *CheckAndAllocateMemory(unsigned int MemAddr,unsigned int MemRange)
{
	unsigned int *TempAddr	=	'\0';

	printk("APOORVA_PCI: IO-MEMORY-PHY.0x%08X-0x%08X",MemAddr,(MemAddr+MemRange-1));

	/*if(check_mem_region(MemAddr,MemRange))
		printk(".....ALREAD_MEM_Y-IN-USE.......ERROR !!!\n");
	else*/
	{
		request_mem_region(MemAddr,MemRange,"APOORVA1553");

		//TempAddr	=	(unsigned int *) ioremap_nocache(MemAddr,MemRange);
                TempAddr = (unsigned int *)ioremap_cache(MemAddr, MemRange);
                  //TempAddr = (unsigned int *)ioremap(MemAddr, MemRange);
		if(TempAddr != 0)
			printk("  ---  0x%08X-0x%08X.[OK]\n",(unsigned int)TempAddr,(((unsigned int)TempAddr)+MemRange-1));
		else
			printk("................REMAPPING-ERROR !!!\n");
	}

	return TempAddr;
}

module_init(APOORVA_CARD1_INIT);
module_exit(APOORVA_CARD1_EXIT);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MMR/GMR");


