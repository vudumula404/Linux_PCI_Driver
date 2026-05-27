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
#include <linux/version.h>
#include <linux/io.h>
#include "ApoorvaIoctl.h"

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35) )
#include <linux/config.h>
#endif

#define		Mem_Range	0x800000

typedef struct _AddrBy
{
	unsigned int Addr;
	unsigned char Data;
}AddrBy;
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
        unsigned char		CARD1_DEBUG_FLAG = 0x0;

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
	   
            case IOCTL_CARD1_MEM_READ_MODULE1:
            {
                unsigned short Data;
                unsigned long addr;
                unsigned short __user *user_ptr;

                if (ioctl_param == 0) {
                    printk(KERN_ERR "*MEM_READ_APOORVA_PCI: Invalid user-space pointer (NULL)\n");
                    return -EINVAL;
                }

                user_ptr = (unsigned short __user *) ioctl_param;

                // ✅ Check if user-space pointer is valid
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**MEM_READ_APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

                // ✅ Copy address from user-space
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "***MEM_READ_APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                if (addr < 0x0 || addr > 0x1FFFF) {
                    printk(KERN_ERR "****MEM_READ_APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }

                Data = readw(BaseDDC + addr);
                //printk(KERN_INFO "*****MEM_READ_APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

                // ✅ Copy read data back to user-space
                if (copy_to_user(user_ptr, &Data, sizeof(Data))) {
                    printk(KERN_ERR "******MEM_READ_APOORVA_PCI: Failed to copy data to user\n");
                    return -EFAULT;
                }

                return 0;
            }       
          break;         
          case IOCTL_CARD1_MEM_WRITE_MODULE1:
          {
              Addr user_data;

              if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr))) {
                  printk(KERN_ERR "MEM_WR_APOORVA_PCI: Failed to copy data from user\n");
                  return -EFAULT;
              }

              if ((user_data.Addr >= 0x0) && (user_data.Addr <= 0x1FFFF)) {
                  writew(user_data.Data, (BaseDDC) + (user_data.Addr));
                 /* printk(KERN_INFO "MEM_WR_APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %08X, Data: %X\n",
                         (unsigned int)(BaseDDC + user_data.Addr), user_data.Data);*/
              } else {
                  return -EINVAL;
              }
          }
          break;
          case IOCTL_CARD1_REG_READ_MODULE1:
	  {
	        unsigned short Data;
                unsigned long addr;
                unsigned short __user *user_ptr;

                if (ioctl_param == 0) {
                    printk(KERN_ERR "*REG_READ_APOORVA_PCI: Invalid user-space pointer (NULL)\n");
                    return -EINVAL;
                }

                user_ptr = (unsigned short __user *) ioctl_param;

                // ✅ Check if user-space pointer is valid
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**REG_READ_APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

                // ✅ Copy address from user-space
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "***REG_READ_APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                if (addr < 0x0 || addr > 0x1FFFF) {
                    printk(KERN_ERR "****REG_READ_APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }
               // addr = addr<<1;
                Data = readw((BaseDDC+(0x30000)) + addr);
               // printk(KERN_INFO "*****REG_READ_APOORVA_PCI: CARD1_REG_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

                // ✅ Copy read data back to user-space
                if (copy_to_user(user_ptr, &Data, sizeof(Data))) {
                    printk(KERN_ERR "******REG_READ_APOORVA_PCI: Failed to copy data to user\n");
                    return -EFAULT;
                }	 
	  }
	  break;
	  case IOCTL_CARD1_REG_WRITE_MODULE1:
	  {
	        Addr user_data;

                if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr))) {
                    printk(KERN_ERR "REG_WR_APOORVA_PCI: Failed to copy data from user\n");
                    return -EFAULT;
                }

                if ((user_data.Addr >= 0x0) && (user_data.Addr <= 0x1FFFF)) {
                    writew(user_data.Data, (BaseDDC+(0x30000)) + (user_data.Addr));
                    /*printk(KERN_INFO "REG_WR_APOORVA_PCI: CARD1_REG_WRITE_MOD_1 : ADDR: %08X, Data: %X\n",
                           (unsigned int)(BaseDDC+(0x30000) + user_data.Addr), user_data.Data);*/
                } else {
                    return -EINVAL;
                }
		  /*copy_from_user(&str,(buf *)ioctl_param,sizeof(buf));
		  //printk("Arg.i APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %X %x\n",str.Addr,str.Data);
		  if((str.Addr >= 0x0) && (str.Addr <= 0x1FFFF))
		  {
			  if(CARD1_DEBUG_FLAG)
				  printk("APOORVA_PCI: CARD1_REG_WRITE_MOD_1 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+str.Addr)),(str.Data));
			  writew(str.Data,(BaseDDC+(0x60000))+(str.Addr));
		  }*/			
	  }
	  break;	
           case IOCTL_CARD1_MEM_READ_MODULE2:
            {
                unsigned short Data;
                unsigned long addr;
                unsigned short __user *user_ptr;

                if (ioctl_param == 0) {
                    printk(KERN_ERR "*MEM_READ_APOORVA_PCI: Invalid user-space pointer (NULL)\n");
                    return -EINVAL;
                }

                user_ptr = (unsigned short __user *) ioctl_param;

                // ✅ Check if user-space pointer is valid
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**MEM_READ_APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

                // ✅ Copy address from user-space
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "***MEM_READ_APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                if (addr < 0x0 || addr > 0x1FFFF) {
                    printk(KERN_ERR "****MEM_READ_APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }

                Data = readw((BaseDDC)+(0x10000) + addr);
               // printk(KERN_INFO "*****MEM_READ_APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

                // ✅ Copy read data back to user-space
                if (copy_to_user(user_ptr, &Data, sizeof(Data))) {
                    printk(KERN_ERR "******MEM_READ_APOORVA_PCI: Failed to copy data to user\n");
                    return -EFAULT;
                }

                return 0;
            }       
          break;         
          case IOCTL_CARD1_MEM_WRITE_MODULE2:
          {
              Addr user_data;

              if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr))) {
                  printk(KERN_ERR "MEM_WR_APOORVA_PCI: Failed to copy data from user\n");
                  return -EFAULT;
              }

              if ((user_data.Addr >= 0x0) && (user_data.Addr <= 0x1FFFF)) {
                  writew(user_data.Data, (BaseDDC)+(0x10000) + (user_data.Addr));
                  /*printk(KERN_INFO "MEM_WR_APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %08X, Data: %X\n",
                         (unsigned int)(BaseDDC + user_data.Addr), user_data.Data);*/
              } else {
                  return -EINVAL;
              }
          }
          break;
          case IOCTL_CARD1_REG_READ_MODULE2:
	  {
	        unsigned short Data;
                unsigned long addr;
                unsigned short __user *user_ptr;

                if (ioctl_param == 0) {
                    printk(KERN_ERR "*REG_READ_APOORVA_PCI: Invalid user-space pointer (NULL)\n");
                    return -EINVAL;
                }

                user_ptr = (unsigned short __user *) ioctl_param;

                // ✅ Check if user-space pointer is valid
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**REG_READ_APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

                // ✅ Copy address from user-space
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "***REG_READ_APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                if (addr < 0x0 || addr > 0x1FFFF) {
                    printk(KERN_ERR "****REG_READ_APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }
               // addr = addr<<1;
                Data = readw((BaseDDC+(0x30800)) + addr);
               // printk(KERN_INFO "*****REG_READ_APOORVA_PCI: CARD1_REG_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

                // ✅ Copy read data back to user-space
                if (copy_to_user(user_ptr, &Data, sizeof(Data))) {
                    printk(KERN_ERR "******REG_READ_APOORVA_PCI: Failed to copy data to user\n");
                    return -EFAULT;
                }	 
	  }
	  break;
	case IOCTL_CARD1_REG_WRITE_MODULE2:
	{
	      Addr user_data;

              if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr))) {
                  printk(KERN_ERR "REG_WR_APOORVA_PCI: Failed to copy data from user\n");
                  return -EFAULT;
              }

              if ((user_data.Addr >= 0x0) && (user_data.Addr <= 0x1FFFF)) {
                  writew(user_data.Data, (BaseDDC+(0x30800)) + (user_data.Addr));
                 /* printk(KERN_INFO "REG_WR_APOORVA_PCI: CARD1_REG_WRITE_MOD_1 : ADDR: %08X, Data: %X\n",
                         (unsigned int)(BaseDDC+(0x30800) + user_data.Addr), user_data.Data);*/
              } else {
                  return -EINVAL;
              }
		/*copy_from_user(&str,(buf *)ioctl_param,sizeof(buf));
		//printk("Arg.i APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %X %x\n",str.Addr,str.Data);
		if((str.Addr >= 0x0) && (str.Addr <= 0x1FFFF))
		{
			if(CARD1_DEBUG_FLAG)
				printk("APOORVA_PCI: CARD1_REG_WRITE_MOD_1 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+str.Addr)),(str.Data));
			writew(str.Data,(BaseDDC+(0x60000))+(str.Addr));
		}*/			
	}
	break;
	case IOCTL_CARD1_MEM_READ_MODULE3:
            {
                unsigned short Data;
                unsigned long addr;
                unsigned short __user *user_ptr;

                if (ioctl_param == 0) {
                    printk(KERN_ERR "*MEM_READ_APOORVA_PCI: Invalid user-space pointer (NULL)\n");
                    return -EINVAL;
                }

                user_ptr = (unsigned short __user *) ioctl_param;

                // ✅ Check if user-space pointer is valid
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**MEM_READ_APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

                // ✅ Copy address from user-space
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "***MEM_READ_APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                if (addr < 0x0 || addr > 0x1FFFF) {
                    printk(KERN_ERR "****MEM_READ_APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }

                Data = readw((BaseDDC)+(0x20000) + addr);
               // printk(KERN_INFO "*****MEM_READ_APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

                // ✅ Copy read data back to user-space
                if (copy_to_user(user_ptr, &Data, sizeof(Data))) {
                    printk(KERN_ERR "******MEM_READ_APOORVA_PCI: Failed to copy data to user\n");
                    return -EFAULT;
                }

                return 0;
            }       
          break;         
          case IOCTL_CARD1_MEM_WRITE_MODULE3:
          {
              Addr user_data;

              if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr))) {
                  printk(KERN_ERR "MEM_WR_APOORVA_PCI: Failed to copy data from user\n");
                  return -EFAULT;
              }

              if ((user_data.Addr >= 0x0) && (user_data.Addr <= 0x1FFFF)) {
                  writew(user_data.Data, (BaseDDC)+(0x20000) + (user_data.Addr));
                  /*printk(KERN_INFO "MEM_WR_APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %08X, Data: %X\n",
                         (unsigned int)(BaseDDC + user_data.Addr), user_data.Data);*/
              } else {
                  return -EINVAL;
              }
          }
          break;
          case IOCTL_CARD1_REG_READ_MODULE3:
	  {
	        unsigned short Data;
                unsigned long addr;
                unsigned short __user *user_ptr;

                if (ioctl_param == 0) {
                    printk(KERN_ERR "*REG_READ_APOORVA_PCI: Invalid user-space pointer (NULL)\n");
                    return -EINVAL;
                }

                user_ptr = (unsigned short __user *) ioctl_param;

                // ✅ Check if user-space pointer is valid
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**REG_READ_APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

                // ✅ Copy address from user-space
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "***REG_READ_APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                if (addr < 0x0 || addr > 0x1FFFF) {
                    printk(KERN_ERR "****REG_READ_APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }
               // addr = addr<<1;
                Data = readw((BaseDDC+(0x31000)) + addr);
               // printk(KERN_INFO "*****REG_READ_APOORVA_PCI: CARD1_REG_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

                // ✅ Copy read data back to user-space
                if (copy_to_user(user_ptr, &Data, sizeof(Data))) {
                    printk(KERN_ERR "******REG_READ_APOORVA_PCI: Failed to copy data to user\n");
                    return -EFAULT;
                }	 
	  }
	  break;
	case IOCTL_CARD1_REG_WRITE_MODULE3:
	{
	      Addr user_data;

              if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr))) {
                  printk(KERN_ERR "REG_WR_APOORVA_PCI: Failed to copy data from user\n");
                  return -EFAULT;
              }

              if ((user_data.Addr >= 0x0) && (user_data.Addr <= 0x1FFFF)) {
                  writew(user_data.Data, (BaseDDC+(0x31000)) + (user_data.Addr));
                  /*printk(KERN_INFO "REG_WR_APOORVA_PCI: CARD1_REG_WRITE_MOD_1 : ADDR: %08X, Data: %X\n",
                         (unsigned int)(BaseDDC+(0x31000) + user_data.Addr), user_data.Data);*/
              } else {
                  return -EINVAL;
              }
		/*copy_from_user(&str,(buf *)ioctl_param,sizeof(buf));
		//printk("Arg.i APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %X %x\n",str.Addr,str.Data);
		if((str.Addr >= 0x0) && (str.Addr <= 0x1FFFF))
		{
			if(CARD1_DEBUG_FLAG)
				printk("APOORVA_PCI: CARD1_REG_WRITE_MOD_1 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+str.Addr)),(str.Data));
			writew(str.Data,(BaseDDC+(0x60000))+(str.Addr));
		}*/			
	}
	break;
	#if 0
	case IOCTL_WRITE_MEM_W:
			{
			  Addr user_data;

                          if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr)))
                          {
                              printk(KERN_ERR "REG_WR_APOORVA_PCI: Failed to copy data from user\n");
                              return -EFAULT;
                          }

                          if ((user_data.Addr >= 0x0) && (user_data.Addr <= 0x1FFFF)) 
                          {
                              writew(user_data.Data, (BaseDDC+(0x31000)) + (user_data.Addr));
                              printk(KERN_INFO "REG_WR_APOORVA_PCI: CARD1_REG_WRITE_MOD_1 : ADDR: %08X, Data: %X\n",
                                     (unsigned int)(BaseDDC+(0x31000) + user_data.Addr), user_data.Data);
                          } 
                          else
                          {
                              return -EINVAL;
                          }
				/*CCSC_DATA_ADDR	*Ptr	=	(CCSC_DATA_ADDR	*)ioctl_param;
				//printk("Addr is %08X\n",Ptr->Addr);

				if((Ptr->Addr >= 0x00) && (Ptr->Addr <= (IO_MEM_SIZE-0x2)))
				{
					if(DRVDBG_ENABLED)
						printk("RGDAS: IOCTL_WRITE__MEM_WORD: ADDR: 0x%08X , DATA: 0x%04X\n",((unsigned int)(Base2+(RESOURCES)+Ptr->Addr)),(Ptr->Data & 0x0000FFFF));
					writew((Ptr->Data & 0x0000FFFF),(Base2+(RESOURCES)+Ptr->Addr));
					return 0;
				}
				else
					return -1;*/
			}
			break;
			#endif
#if 0
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
		        printk("1APOORVA_PCI: CARD1_PORT_WRITE_MOD_1\n");
			copy_from_user(&str,(buf *)ioctl_param,sizeof(buf));
			 printk("2APOORVA_PCI: CARD1_PORT_WRITE_MOD_1\n");
			if((str.Addr >= 0x0) && (str.Addr <= 0x7FFFF))
			{
			        printk("3APOORVA_PCI: CARD1_PORT_WRITE_MOD_1\n");
				/*if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_PORT_WRITE_MOD_1 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+str.Addr)),(str.Data));*/
				outb(str.Data,Start1+(str.Addr));
				printk("4APOORVA_PCI: CARD1_PORT_WRITE_MOD1 Data %x Address %x Start1 %x\n",str.Data,(str.Addr),Start1);
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
		#endif
		/*case IOCTL_CARD1_WRITE_PORT:
                {
                    AddrBy user_data;

                    if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr))) {
                        printk(KERN_ERR "IOCTL_CARD1_WRITE_PORT: Failed to copy data from user\n");
                        return -EFAULT;
                    }

                    // Address boundary check (optional but good practice)
                    if (user_data.Addr > 0x1FFFF) {
                        printk(KERN_ERR "IOCTL_CARD1_WRITE_PORT: Invalid I/O offset: 0x%X\n", user_data.Addr);
                        return -EINVAL;
                    }

                    // Use outb only if BAR[2] points to an I/O port region
                    outb(user_data.Data, (unsigned int)(BAR[2] + user_data.Addr));

                    printk(KERN_INFO "IOCTL_CARD1_WRITE_PORT: I/O write - Addr: 0x%X (BAR[2]=0x%X, offset=0x%X), Data: 0x%02X\n",
                           (unsigned int)(BAR[2] + user_data.Addr), BAR[2], user_data.Addr, user_data.Data);

                    break;
                }*/
              case IOCTL_CARD1_READ_PORT:
              {
                  AddrBy user_data;

                  if (copy_from_user(&user_data, (AddrBy __user *)ioctl_param, sizeof(AddrBy))) {
                      printk(KERN_ERR "IOCTL_CARD1_READ_PORT: Failed to copy data from user\n");
                      return -EFAULT;
                  }

                  if (user_data.Addr > 0x1FFFF) {
                      printk(KERN_ERR "IOCTL_CARD1_READ_PORT: Invalid I/O offset: 0x%X\n", user_data.Addr);
                      return -EINVAL;
                  }

                  user_data.Data = inb((unsigned int)(PLX_IO + user_data.Addr));

                  if (copy_to_user((AddrBy __user *)ioctl_param, &user_data, sizeof(AddrBy))) {
                      printk(KERN_ERR "IOCTL_CARD1_READ_PORT: Failed to copy data to user\n");
                      return -EFAULT;
                  }

                  if (CARD1_DEBUG_FLAG)
                      printk(KERN_INFO "IOCTL_CARD1_READ_PORT: I/O read - Addr: 0x%X, Data: 0x%02X\n",
                             user_data.Addr, user_data.Data);

                  return 0;
              }
            case IOCTL_CARD1_WRITE_PORT:
                {
                    AddrBy user_data;

                    if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr))) {
                        printk(KERN_ERR "IOCTL_CARD1_WRITE_PORT: Failed to copy data from user\n");
                        return -EFAULT;
                    }

                    // Address boundary check (optional but good practice)
                    if (user_data.Addr > 0x1FFFF) {
                        printk(KERN_ERR "IOCTL_CARD1_WRITE_PORT: Invalid I/O offset: 0x%X\n", user_data.Addr);
                        return -EINVAL;
                    }
                    
                    // Use outb only if BAR[1] points to an I/O port region
                    outb(user_data.Data, (unsigned int)(PLX_IO+ user_data.Addr));

                    printk(KERN_INFO "IOCTL_CARD1_WRITE_PORT: I/O write - Addr: 0x%X (PLX_IO=0x%X, offset=0x%X), Data: 0x%02X\n",
                           (unsigned int)(PLX_IO + user_data.Addr), PLX_IO, user_data.Addr, user_data.Data);
                   /* u8 readback = inb(PLX_IO + 0x10);
                    printk("Wrote RT Addr 0x01, Read back 0x%02X\n", readback);*/
                    break;
                }
                case IOCTL_CARD1_SET_ICS_REG:
                 {
                    // Use outb only if BAR[1] points to an I/O port region
                   // outb(user_data.Data, (unsigned int)(PLX_IO+user_data.Addr));
                    outb(0x09,(unsigned int)(PLX_IO+0x69));

                   /* printk(KERN_INFO "IOCTL_CARD1_WRITE_PORT: I/O write - Addr: 0x%X (BAR[1]=0x%X, offset=0x%X), Data: 0x%02X\n",
                           (unsigned int)(PLX_IO + user_data.Addr), PLX_IO, user_data.Addr, user_data.Data);*/
                    u8 readback = inb(PLX_IO + 0x69);
                    printk("IOCTL_CARD1_SET_ICS_REG, Read back 0x%02X\n", readback);
                    break;                  	
                }
              case IOCTL_CARD1_READ_PORT_A:
              {
                  AddrBy user_data;

                  if (copy_from_user(&user_data, (AddrBy __user *)ioctl_param, sizeof(AddrBy))) {
                      printk(KERN_ERR "IOCTL_CARD1_READ_PORT: Failed to copy data from user\n");
                      return -EFAULT;
                  }

                  if (user_data.Addr > 0x1FFFF) {
                      printk(KERN_ERR "IOCTL_CARD1_READ_PORT: Invalid I/O offset: 0x%X\n", user_data.Addr);
                      return -EINVAL;
                  }

                  user_data.Data = inb((unsigned int)(Start1 + user_data.Addr));

                  if (copy_to_user((AddrBy __user *)ioctl_param, &user_data, sizeof(AddrBy))) {
                      printk(KERN_ERR "IOCTL_CARD1_READ_PORT: Failed to copy data to user\n");
                      return -EFAULT;
                  }

                  if (CARD1_DEBUG_FLAG)
                      printk(KERN_INFO "IOCTL_CARD1_READ_PORT: I/O read - Addr: 0x%X, Data: 0x%02X\n",
                             user_data.Addr, user_data.Data);

                  return 0;
              }
            case IOCTL_CARD1_WRITE_PORT_A:
                {
                    AddrBy user_data;
                    int portread=0;
                    if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr))) {
                        printk(KERN_ERR "IOCTL_CARD1_WRITE_PORT_A: Failed to copy data from user\n");
                        return -EFAULT;
                    }

                    // Address boundary check (optional but good practice)
                    if (user_data.Addr > 0x1FFFF) {
                        printk(KERN_ERR "IOCTL_CARD1_WRITE_PORT_A: Invalid I/O offset: 0x%X\n", user_data.Addr);
                        return -EINVAL;
                    }

                    // Use outb only if BAR[1] points to an I/O port region
                   // user_data.Data = 1;
                    /*user_data.Addr = 0x16;*/
                     portread = inb((unsigned int)(Start1 + 0x0000));
                    printk("portread(Start1 + 0x0000) 0x%02X, user_data.Addr 0x%02X\n", portread,user_data.Addr);
                    // Use outb only if BAR[1] points to an I/O port region
                    outb(user_data.Data, (unsigned int)(Start1+ 0x12+(user_data.Addr | portread)));

                    /*printk(KERN_INFO "IOCTL_CARD1_WRITE_PORT_A: I/O write - Addr: 0x%X (Start1=0x%X, offset=0x%X), Data: 0x%02X\n",
                           (unsigned int)(Start1 + user_data.Addr), Start1, user_data.Addr, user_data.Data);*/
                    u8 readback = inb(Start1 +0x12+ (user_data.Addr | portread));
                    printk("Inter Addr 0x01, Read back 0x%02X\n", readback);
                    break;
                }
                
		/*case IOCTL_CARD1_SET_ICS_REG:
		writeb(0x5b,Base0+0x4c);
		writeb(0x0f,Base0+0x4d);
		//writeb(0x00,Base0+0x4d);
		//printk("ICS-REG-0x4C :%04X\n",readb(Base0+0x4c));
		//printk("ICS-REG-0x4D :%04X\n",readb(Base0+0x4d));
		break;*/

	      /*case IOCTL_CARD1_WRITE_PORT:
	      {
		    Addr user_data;

                  if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr))) {
                      printk(KERN_ERR "IOCTL_CARD1_WRITE_PORT: Failed to copy data from user\n");
                      return -EFAULT;
                  }

                  if ((user_data.Addr >= 0x0) && (user_data.Addr <= 0x1FFFF)) {
                      outb((user_data.Data & 0x0000FFFF),(unsigned int)(BAR[2]+(user_data.Addr)));                    
                     // writew(user_data.Data,((unsigned int)(BAR[3]+user_data.Addr)));
                      printk(KERN_INFO "IOCTL_CARD1_WRITE_PORT: PLX_IO: ADDR: %08X, Data: %X\n",
                             (unsigned int)(BAR[2]+user_data.Addr), user_data.Data);
                  } else 
                  {
                      return -EINVAL;
                  }
              }
            break;*/
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
			
			copy_from_user(&str,(buf *)ioctl_param,sizeof(buf));
			if((str.Addr >= 0x0) && (str.Addr <= 0x7FFFF))
			{
				if(CARD1_DEBUG_FLAG)
					printk("APOORVA_PCI: CARD1_REG_WRITE_MOD_2 : ADDR: %X, DATA: %X\n",((unsigned int)(Base0+str.Addr)),(str.Data));
				writew(str.Data,(Base0+(0x180000))+(str.Addr));
			}

		}
		break;
		*/
	case IOCTL_CARD1_DEBUG_ENABLE:
		CARD1_DEBUG_FLAG = 0xFF;
		break;		

	case IOCTL_CARD1_DEBUG_DISABLE:
		CARD1_DEBUG_FLAG = 0x0;
		break;		
	case IOCTL_CARD1_SOFTWARE_RESET:
		plx = readb(Base0+0x53);
		//printk("Before---plx :%04X\n",plx);
		writeb((plx | 0x40),Base0+0x53);
		//printk("After---plx :%04X\n",plx);
		writeb(plx,Base0+0x53);
		break;
	case IOCTL_SET_M1CNT:
	      MOD1_EOMCNT=0;
			//if(CARD1_DEBUG_FLAG)
			//printk("IOCTL_CARD1_MOD1_GET_BCEOMCNT :%04X\n",BCEOMCnt);
			break;
	case IOCTL_GET_M1CNT:
			//if(CARD1_DEBUG_FLAG)
			//printk("IOCTL_CARD1_MOD1_GET_MTEOMCNT :%04X\n",MTEOMCnt);
			return MOD1_EOMCNT;
			
	/*case IOCTL_CARD1_MOD1_GET_BCEOMCNT:
			//if(CARD1_DEBUG_FLAG)
			//printk("IOCTL_CARD1_MOD1_GET_BCEOMCNT :%04X\n",BCEOMCnt);
			return BCEOMCnt;
	case IOCTL_CARD1_MOD1_GET_MTEOMCNT:
			//if(CARD1_DEBUG_FLAG)
			//printk("IOCTL_CARD1_MOD1_GET_MTEOMCNT :%04X\n",MTEOMCnt);
			return MTEOMCnt;

	case IOCTL_CARD1_MOD1_GET_RT0_EOMCNT:
			return MOD1RT0EOMCnt;*/
	
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
/*static irqreturn_t ISR(int irq, void *dev_id)
{
    unsigned short Temp = 0, LINT = 0;
    unsigned char Mod_1_3 = 0;
    
     printk("Before ISR triggered. LINT = 0x%02X\n", LINT);

    LINT = readb(PLX_IO + 0x4c);
    printk("ISR triggered. LINT = 0x%02X\n", LINT);

    if ((LINT & 0x20) != 0x20)
        return IRQ_NONE;

    Mod_1_3 = inb(Start1 + 0x80);

    if (Mod_1_3 & 0x01) {
        if (Mod_1_3 & 0x88) MOD1_EOMCNT++;
        if (Mod_1_3 & 0x8A) MOD2_EOMCNT++;
        if (Mod_1_3 & 0x8C) MOD3_EOMCNT++;
        printk("MOD1_EOMCNT: %d\n", MOD1_EOMCNT);
    }

    outb(0x1, Start1 + 0x16); // Ack local device
    Temp = inb(PLX_IO + 0x4d);
    outb(Temp | 0x8, PLX_IO + 0x4d); // Clear PLX INT
    outb(0x0, Start1 + 0x16); // Deassert local device ACK

    return IRQ_HANDLED;
}*/
irqreturn_t ISR(int irq, void *dev_id)
{
    printk("APOORVA_PCI: ISR called for IRQ %d with dev_id %p\n", irq, dev_id);

    // Add additional interrupt handling logic here if needed

    return IRQ_HANDLED;
}
#if 0
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
	unsigned char		Intr1=0,Mod_1_3=0;
	

	dev_id			= NULL;
        printk("Before ISR1  LINT \n");
	LINT = readb(PLX_IO+0x4c);

	printk("ISR1  LINT : %x\n",LINT);

	if( (LINT&0x20)==0x20) //LINT Status
	{
		Mod_1_3		=	inb(Start1+0x80);				//0th bit is '1'---- >Module 1 and 1st bit is '1' ---- >Module 3

		//printk("Addr :0x80 -----> Value: %04X\n",Mod_1_3);

		if((Mod_1_3 & 0x01)	==	0x01)
		{
			//printk("Module 1 Has been selected\n");

			/*Intr1			=	inb(Start1+0x11);		//read Normal interrupt status
			EnhancedIntr1		=	inb(Start1+0x12);		//read Enhanced interrupts status
			RTMod1			=	inb(Start1+0x15);		//read RT-Address
			RTMod1			&=	0x1F;*/

			/*printk("Intr1 :0x11 -----> Value: %04X\n",Intr1);
			printk("EnhancedIntr1 :0x12 -----> Value: %04X\n",EnhancedIntr1);
			printk("RTMod1 :0x15 -----> Value: %04X\n",RTMod1);*/

			//if(Intr1 != 0x0)
			{
		    		if(Mod_1_3 & 0x88)
				   	MOD1_EOMCNT++;				
				if(Mod_1_3 & 0x8A)
				   	MOD2_EOMCNT++;
		    		if(Mod_1_3 & 0x8C)
			      		MOD3_EOMCNT++;			
			}
			printk("Intr1 :0x11 -----> Value: %04d\n",MOD1_EOMCNT);
			
		}		
		outb(0x1,Start1+0x16);      			//Acknowledge To Loc. Device
 		Temp = inb((PLX_IO)+(0x4d));
		outb(Temp|0x8,(PLX_IO)+(0x4d));			//Clears LINT2 ICSR 
		outb(0x0,Start1+0x16);				//Acknowledge To Loc.Device	

	}	
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35) )
// void return
 #else
    return IRQ_HANDLED;
#endif
}
#endif
#if 0
static irqreturn_t ISR_FOR_CARD1(int irq, void *dev_id)
{
    unsigned short Temp = 0, LINT = 0;
    unsigned char Mod_1_3 = 0;

    dev_id = NULL;

    LINT = readb(PLX_IO + 0x4c);

    if ((LINT & 0x20) == 0x20) // LINT Status
    {
        Mod_1_3 = inb(Start1 + 0x80); // 0th bit = Module 1, 1st bit = Module 3

        if ((Mod_1_3 & 0x01) == 0x01)
        {
            if (Mod_1_3 & 0x88)
                MOD1_EOMCNT++;
            if (Mod_1_3 & 0x8A)
                MOD2_EOMCNT++;
            if (Mod_1_3 & 0x8C)
                MOD3_EOMCNT++;

            printk("MOD1_EOMCNT = %d\n", MOD1_EOMCNT);
        }

        outb(0x1, Start1 + 0x16);       // Acknowledge local device
        Temp = inb(PLX_IO + 0x4d);
        outb(Temp | 0x08, PLX_IO + 0x4d); // Clear LINT2 ICSR
        outb(0x0, Start1 + 0x16);       // Final acknowledge
    }

    return IRQ_HANDLED;
}

#endif
#if 0
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
#endif
#if 0
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
	IntrLine = dev_APOORVA->irq;
        printk("APOORVA_PCI: IRQ assigned by kernel: %d\n", IntrLine);
	BAR[0] = BAR[0] & PCI_BASE_ADDRESS_MEM_MASK;
   	BAR[2] = BAR[2] & PCI_BASE_ADDRESS_MEM_MASK;
 	//Start1 = BAR[3] & PCI_BASE_ADDRESS_IO_MASK;
 	Start1 = BAR[2] & PCI_BASE_ADDRESS_IO_MASK;
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
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35) )
	 if(request_irq(IntrLine,&ISR,SA_SHIRQ,"APOORVA1553",irqdevid)) //For Non SMP	
#else
        if(request_irq(IntrLine, (irq_handler_t)ISR, IRQF_SHARED, "APOORVA1553", irqdevid))
	//if(request_irq(IntrLine,(void *)&ISR,IRQF_SHARED,"APOORVA1553",irqdevid)) //For Non SMP

#endif
	{
		printk("\tIrq Allocation Failure\n");
		return 0;
	}

	BaseDDC = (unsigned short *)CheckAndAllocateMemory(BAR[0],Mem_Range/*0x60*/);
	request_region(Start1,Len1,"APOORVA1553");
	request_region(PLX_IO,Len2,"APOORVA1553");
	//outb(0x00, Start1 + 0x1); 
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
#endif
static int __init APOORVA_CARD1_INIT(void)
{
	int ret;
	unsigned char intcsr_val;
	void *irqdevid = (void *)0x12;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
	dev_APOORVA = pci_find_device(Vendor_ID, Device_ID, dev_APOORVA);
#else
	dev_APOORVA = pci_get_device(Vendor_ID, Device_ID, dev_APOORVA);
#endif

	if (dev_APOORVA == NULL) {
		printk("\n\nAPOORVA1553:Device not found .... \n\n ");
		return 1;
	}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,18))
	ret = pci_enable_device(dev_APOORVA);
	if (ret) {
		printk("Warning--------> Problem While Enabling the Device \n ret  is :%04X\n", ret);
	}
#endif

	MajorNo = register_chrdev(0, "APOORVA1553", &fops);
	if (MajorNo < 0)
		printk("\nAPOORVA_PCI: APOORVA1553 Device Registration Failure\n");
	else
		printk("\nAPOORVA_PCI: APOORVA1553 Device Has Been Registered With MajorNo :[%d] .....[OK]\n", MajorNo);

        printk("APOORVA_PCI: READING PCI BAR0................");	pci_read_config_dword(dev_APOORVA ,PCI_BASE_ADDRESS_0,&BAR[0]);		
        printk("[0x%8X]\n",BAR[0]);
	printk("APOORVA_PCI: READING PCI BAR1................");	pci_read_config_dword(dev_APOORVA ,PCI_BASE_ADDRESS_1,&BAR[1]);	
	printk("[0x%4X]\n",BAR[1]);
	printk("APOORVA_PCI: READING PCI BAR2................");	pci_read_config_dword(dev_APOORVA ,PCI_BASE_ADDRESS_2,&BAR[2]);
	printk("[0x%8X]\n",BAR[2]);
	printk("APOORVA_PCI: READING PCI BAR3................");	pci_read_config_dword(dev_APOORVA ,PCI_BASE_ADDRESS_3,&BAR[3]);
	printk("[0x%4X]\n",BAR[3]);
	printk("APOORVA_PCI: READING PCI IRQL................");	pci_read_config_byte(dev_APOORVA ,PCI_INTERRUPT_LINE,&IntrLine);
	//printk("APOORVA_PCI: READING PCI IRQL................");	pci_read_config_byte(dev_APOORVA ,0x3c,&IntrLine);
	printk("[0x%1X]\n",IntrLine);
	printk("APOORVA_PCI: READING PCI IRQ PIN.............");	pci_read_config_byte(dev_APOORVA ,PCI_INTERRUPT_PIN,&IntrPin);		
	printk("[0x%02X]\n",IntrPin);
	IntrLine = dev_APOORVA->irq;
        printk("APOORVA_PCI: IRQ assigned by kernel: %d\n", IntrLine);	

	BAR[0] &= PCI_BASE_ADDRESS_MEM_MASK;
	BAR[2] &= PCI_BASE_ADDRESS_MEM_MASK;
	Start1 = BAR[2] & PCI_BASE_ADDRESS_IO_MASK;
	Len1 = 0xff;
	PLX_IO = BAR[1] & PCI_BASE_ADDRESS_IO_MASK;
	Len2 = 0x50;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18))
	if (request_irq(IRQL, ISR_FOR_CARD1, SA_SHIRQ, "APOORVA1553", irqdevid))
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
	if (request_irq(IntrLine, &ISR, SA_SHIRQ, "APOORVA1553", irqdevid))
#else
	if (request_irq(IntrLine, (irq_handler_t)ISR, IRQF_SHARED, "APOORVA1553", irqdevid))
#endif
	{
		printk("\tIrq Allocation Failure\n");
		return 0;
	}

	BaseDDC = (unsigned short *)CheckAndAllocateMemory(BAR[0], Mem_Range);
	request_region(Start1, Len1, "APOORVA1553");
	request_region(PLX_IO, Len2, "APOORVA1553");
	if (BaseDDC == '\0') {
		printk("ERROR !!!\n");
		free_irq(IntrLine, irqdevid);
		release_region(Start1, Len1);
		release_region(PLX_IO, Len2);
		return -1;
	}
	/*// PLX 9050 INTCSR register (offset 0x4C) - send software interrupt and enable bits
	outb(0xC0, Start1 + 0x4C);
	printk("APOORVA_PCI: Test interrupt signal sent using outb at offset 0x4C.\n");
        outb(0x1, Start1 + 0x4C);
	// Read back INTCSR to verify
	intcsr_val = inb(Start1 + 0x4C);
	printk("APOORVA_PCI: INTCSR value after read: 0x%02X\n", intcsr_val);    */  
        
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
static unsigned int *CheckAndAllocateMemory(unsigned int MemAddr, unsigned int MemRange)
{
    unsigned int *TempAddr = NULL;

    printk("APOORVA_PCI: IO-MEMORY-PHY.0x%08X-0x%08X", MemAddr, (MemAddr + MemRange - 1));

    // Request memory region to ensure it's not in use
    /*if (!request_mem_region(MemAddr, MemRange, "APOORVA1553")) {
        printk(" .....MEMORY REQUEST FAILED - ALREADY IN USE.......ERROR !!!\n");
        return NULL;
    }*/

    // Use standard ioremap for mapping PCI memory
    TempAddr = (unsigned int *)ioremap(MemAddr, MemRange);
    if (TempAddr != NULL)
        printk("  ---  0x%08X-0x%08X.[OK]\n", (unsigned int)TempAddr, ((unsigned int)TempAddr + MemRange - 1));
    else
        printk("................REMAPPING-ERROR !!!\n");

    return TempAddr;
}

#if 0
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
#endif
module_init(APOORVA_CARD1_INIT);
module_exit(APOORVA_CARD1_EXIT);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MMR/GMR");


