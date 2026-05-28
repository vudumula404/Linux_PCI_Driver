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
#include <linux/ioctl.h>
#include <linux/dma-mapping.h>
#include <linux/atomic.h>

//atomic_t BCEOMCnt = ATOMIC_INIT(0);
//atomic_t BCEOFCnt = ATOMIC_INIT(0);
//atomic_t BCEOMMINOR = ATOMIC_INIT(0);

#include "ApoorvaIoctl.h"
static void __iomem *reg_base;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35) )
#include <linux/config.h>
#endif

#define		Mem_Range	  0x400000
//NODE 1        BASE_ADDR         0x00180000
//NODE 2        BASE_ADDR         0x00181000
//NODE 3        BASE_ADDR         0x00182000
#define         NODE_SEL          0x00180000
#define         CONFIG_REG_MOD1        0x1801EC
#define         CONFIG_REG_MOD2        0x1811EC
#define         CONFIG_REG_MOD3        0x1821EC


#define         BC_INT_EN_REG       0x1801E0
#define         RT_INT_EN_REG       0x1801E4
#define         MT_INT_EN_REG       0x1801E8

#define         BC_INT_MASK_WR_MOD1     0x1801F4
#define         BC_INT_MASK_WR_MOD2     0x1811F4
#define         BC_INT_MASK_WR_MOD3     0x1821F4

#define         RT_INT_MASK_WR_MOD1     0x1801F8
#define         RT_INT_MASK_WR_MOD2     0x1811F8
#define         RT_INT_MASK_WR_MOD3     0x1821F8

#define         MT_INT_MASK_WR_MOD1     0x1801FC
#define         MT_INT_MASK_WR_MOD2     0x1811FC
#define         MT_INT_MASK_WR_MOD3     0x1821FC

#define         BC_INT_MASK_RD_REG     0x1801E0
#define         RT_INT_MASK_RD_REG     0x1801E4
#define         MT_INT_MASK_RD_REG     0x1811E8


#define         INT_STS_BC_MOD1      0x1801F0
#define         INT_STS_RT_MOD1      0x1801F4
#define         INT_STS_MT_MOD1      0x1801F8

#define         INT_STS_BC_MOD2      0x1811F0
#define         INT_STS_RT_MOD2      0x1811F4
#define         INT_STS_MT_MOD2      0x1811F8

#define         INT_STS_BC_MOD3      0x1821F0
#define         INT_STS_RT_MOD3      0x1821F4
#define         INT_STS_MT_MOD3      0x1821F8


#define         INT_CLR_REG_MOD1       0x1801F0
#define         INT_CLR_REG_MOD2       0x1811F0
#define         INT_CLR_REG_MOD3       0x1821F0

#define         CONFIG_REG_RST_VAL        0x00000003
#define         CONFIG_REG_INIT_VAL       0x00000078

#define         BC_INT_EN_VAL       0x0000ffbf  //bc
#define         RT_INT_EN_VAL       0x0000ffbf
#define         MT_INT_EN_VAL       0x0000CC45

#define         BC_INT_MASK_VAL     0x00000000
#define         RT_INT_MASK_VAL     0x00000000 //rt
#define         MT_INT_MASK_VAL     0x00000000


#define         INT_CLR_BC_VAL    0x0000027C
#define         INT_CLR_RT_VAL    0x000002F8
#define         INT_CLR_MT_VAL    0x00000378

#define         DMA_COMPLETE      0x189480
#define         DMA_CLEAR         0x189400

#define DMA_ALLOC_SIZE (512 * 1024) 

#define VENDOR_ID 0x01b5
#define DEVICE_ID 0x9050

static          u8 *dma_cpu_buf;           
static dma_addr_t   dma_handle;     
static size_t       dma_size = 0;  
 

static 		int		BCEOMCnt=0,MTEOMCnt=0,RTDBCACnt=0,RTEOMCnt=0,BCEOMMINOR =0;
static 		int		BCEOM2Cnt=0,MTEOMCnt2=0,RTDBCACnt2=0;
static 		int		BCEOM3Cnt=0,MTEOMCnt3=0,RTDBCACnt3=0;
static 		int		BCSOM1=0,BCSOM2=0,BCSOM3=0,BCBCR1=0,BCBCR2=0,BCBCR3=0;
static 		int		MTSOM1=0,MTSOM2=0,MTSOM3=0,RTSOM1=0,RTSOM2=0,RTSOM3=0;

static          int             SOMCnt=0,SOMCnt2=0,SOMCnt3=0;
static 		int		MTTTRollOver1=0,MTTTRollOver2=0,MTTTRollOver3=0,TTRollOver4=0;
static 		int		RTTTRollOver1=0,RTTTRollOver2=0,RTTTRollOver3=0;


static          int             RTCMDCnt=0,RTCIRCnt=0,RTMDCnt=0,RTFRMCnt=0,RTPARCnt=0,RXEOMCnt=0;
static          int             RTCMDCnt2=0,RTCIRCnt2=0,RTMDCnt2=0,RTFRMCnt2=0,RTPARCnt2=0,RXEOMCnt2=0,RTEOMCnt2=0;
static          int             RTCMDCnt3=0,RTCIRCnt3=0,RTMDCnt3=0,RTFRMCnt3=0,RTPARCnt3=0,RXEOMCnt3=0,RTEOMCnt3=0;

static          int             MTCMDCnt=0,MTDATACnt=0,MTFRMCnt=0;
static          int             MTCMDCnt2=0,MTDATACnt2=0,MTFRMCnt2=0;
static          int             MTCMDCnt3=0,MTDATACnt3=0,MTFRMCnt3=0;

static          int             BCStatusSet1=0,BCStatusSet2=0,BCStatusSet3=0;       
static          int             BCEOFCnt=0,BCEOF2Cnt=0,BCEOF3Cnt=0;
static          int             BCEOMFCnt=0,BCEOMF2Cnt=0,BCEOMF3Cnt=0,BCFMT1=0,BCFMT2=0,BCFMT3=0;
static          int             BCTTRollOver1=0,BCTTRollOver2=0,BCTTRollOver3=0,BCCWEOM1=0,BCCWEOM2=0,BCCWEOM3=0;
static          int             BCCMDSRO1=0,BCCMDSRO2=0,BCCMDSRO3=0;
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
 struct mem_ioctl {
                    unsigned long  u_addr;
                    unsigned short u_value;
                  };
struct	pci_dev	*dev_APOORVA = NULL;

static int device_open(struct inode *inode,struct file *filp)
{
	
	return 0;
}

static int device_close(struct inode *inode,struct file *filp)
{
	return 0;
}

static int my_mmap(struct file *filp,
                   struct vm_area_struct *vma)
{
    int ret;

    /* DMA buffer must be allocated */
    if (dma_cpu_buf == NULL)
        return -ENOMEM;

    /* Optional size check */
    if ((vma->vm_end - vma->vm_start) > dma_size)
        return -EINVAL;

    /* Map coherent DMA buffer into user space */
    ret = dma_mmap_coherent(&dev_APOORVA->dev,   /* or your device pointer */
                            vma,
                            dma_cpu_buf,
                            dma_handle,
                            dma_size);

    /* Always return a value */
    return ret;
}
static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{	
	unsigned int	*SETINTR;
	unsigned char	plx;
	struct reg_op1 w;  
        struct reg_op2 r;  
	
	switch(ioctl_num)
	{
	
    case GET_DMA_ADDR:
    {
        unsigned long long cpu_buf_addr;

        pr_err("DMA CPU BUFFER ADDRESS CALLED\n");

        
        cpu_buf_addr = (unsigned long long)(uintptr_t)dma_cpu_buf;

        if (copy_to_user((void __user *)ioctl_param,
                         &cpu_buf_addr,
                         sizeof(cpu_buf_addr)))
        {
            return -EFAULT;
        }

        pr_err("Returned DMA CPU buffer address: 0x%llx\n", cpu_buf_addr);
        return 0;
    }
        
    case IOCTL_WRITE_REG:
    {
        //printk(KERN_INFO "IOCTL: ioctl_num = 0x%X\n", ioctl_num); 
        struct reg_op1 w;

        if (ioctl_param == 0) {
            printk(KERN_ERR "IOCTL_WRITE_REG: Invalid user-space pointer (NULL)\n");
            return -EINVAL;
        }

        if (copy_from_user(&w, (void __user *)ioctl_param, sizeof(w))) {
            printk(KERN_ERR "IOCTL_WRITE_REG: Failed to copy data from user-space\n");
            return -EFAULT;
        }
        
       
        if (!BaseDDC) {
            printk(KERN_ERR "IOCTL_WRITE_REG: Invalid BaseDDC pointer (device not initialized)\n");
            return -ENODEV;
        }
          //printk("BaseDDC offset: %08X\n", BaseDDC);
      
        if (w.offset < 0xC4000 || w.offset > 0x1EFFFF) {
            printk(KERN_ERR "IOCTL_WRITE_REG: Invalid register offset: %08X\n", w.offset);
            return -EINVAL;
         
    }
        iowrite32(w.value, BaseDDC + w.offset);

        //printk(KERN_INFO "IOCTL_WRITE_REG: Written value 0x%08X to register at offset 0x%08X\n", w.value, w.offset);

        return 0;
    }
    break;

        case IOCTL_READ_REG:
      {
          struct reg_op2 r;

          if (ioctl_param == 0) {
              printk(KERN_ERR "IOCTL_READ_REG: Invalid user-space pointer (NULL)\n");
              return -EINVAL;
          }

         
          if (copy_from_user(&r, (void __user *)ioctl_param, sizeof(r))) {
              printk(KERN_ERR "IOCTL_READ_REG: Failed to copy data from user-space\n");
              return -EFAULT;
          }

         
          if (!BaseDDC) {
              printk(KERN_ERR "IOCTL_READ_REG: Invalid BaseDDC pointer (device not initialized)\n");
              return -ENODEV;
          }

          if (r.offset < 0x188000 || r.offset > 0x6FFFF) {
              printk(KERN_ERR "IOCTL_READ_REG: Invalid register offset: %08X\n", r.offset);
              return -EINVAL;
          }

         
          r.value = ioread32(BaseDDC + r.offset);

          
         // printk(KERN_INFO "IOCTL_READ_REG: Read value 0x%08X from register at offset 0x%08X\n", r.value, r.offset);

          
          if (copy_to_user((void __user *)ioctl_param, &r, sizeof(r))) {
              printk(KERN_ERR "IOCTL_READ_REG: Failed to copy data to user-space\n");
              return -EFAULT;
          }

          return 0;
      }
      break;

        
        case IOCTL_DMA_FREE:
        
        if (dma_cpu_buf) {
            dma_free_coherent(&dev_APOORVA->dev, dma_size, dma_cpu_buf, dma_handle);
            pr_info("%s: dma buffer freed by IOCTL\n", DEVICE_NAME);
            dma_cpu_buf = NULL;
            dma_handle = 0;
            dma_size = 0;
        }
       
        return 0;
        break;

      case IOCTL_DMA_GET_PHYS:
      {
          uint64_t phys = 0;
         
          if (!dma_cpu_buf) {
              pr_err("DMA buffer not allocated (dma_cpu_buf is NULL)\n");
              return -ENODEV;
          }

        
          phys = (uint64_t)dma_handle;

        
          //pr_info("IOCTL_DMA_GET_PHYS: Physical address = 0x%016llX\n", phys);
         
         if (copy_to_user((void __user *)ioctl_param, &phys, sizeof(phys))) {
              pr_err("Failed to copy physical address to user space\n");
              return -EFAULT;
          }

          return 0;
      }
      break;

case IOCTL_DMA_WRITE_USER:
{
    struct dma_rw rw;
    u8 *dst8;
    static int i = 0;
    int j;

    i++;
    //pr_info("\n[KERNEL] DMA WRITE CALLED %d\n", i);

    if (copy_from_user(&rw,
                       (void __user *)ioctl_param,
                       sizeof(rw))) {
        pr_err("copy_from_user failed\n");
        return -EFAULT;
    }

    //pr_info("Offset=%zu Len=%zu\n", rw.offset, rw.len);

    if (!dma_cpu_buf)
        return -ENOMEM;

    if (rw.offset + rw.len > dma_size)
        return -EINVAL;

    if (rw.offset % 2 || rw.len % 2) {
        pr_err("unaligned access\n");
        return -EINVAL;
    }

    dst8 = (u8 *)dma_cpu_buf + rw.offset;

    /* clear old data debug */
    //pr_info("OLD BUFFER:\n");
    for (j = 0; j < rw.len; j += 2)
        //pr_info("%04x ", *(u16 *)(dst8 + j));
    //pr_info("\n");

    /* SAFE COPY: keep BYTE COPY */
    if (copy_from_user(dst8,
                       rw.buf,
                       rw.len)) {
        pr_err("DMA copy failed\n");
        return -EFAULT;
    }

    /* memory barrier */
    wmb();

    //pr_info("NEW BUFFER:\n");
    for (j = 0; j < rw.len; j += 2)
        //pr_info("%04x ", *(u16 *)(dst8 + j));
    //pr_info("\n");

    /*pr_info("DMA BASE=%p OFFSET=%zu LEN=%zu\n",
            dma_cpu_buf, rw.offset, rw.len);*/

    return 0;
}

case IOCTL_DMA_READ_USER:
{
    //pr_info("DMA READ CALLED\n");

    struct dma_rw rw;

    u8 *src8;

    uint32_t j;

    if (copy_from_user(&rw,
                       (void __user *)ioctl_param,
                       sizeof(rw)))
    {
        pr_err("copy_from_user failed\n");
        return -EFAULT;
    }

    if (!dma_cpu_buf)
    {
        pr_err("DMA buffer NULL\n");
        return -ENOMEM;
    }

    if ((rw.offset + rw.len) > dma_size)
    {
        pr_err("Invalid DMA range\n");
        return -EINVAL;
    }

    /* 32-bit alignment check */
    if ((rw.offset % 4) || (rw.len % 4))
    {
        pr_err("Alignment error\n");
        return -EINVAL;
    }

    src8 = (u8 *)dma_cpu_buf + rw.offset;
    //pr_info("DMA Address = 0x%08x\n",src8);

    /*pr_info("\n[KERNEL] DMA READ\n");
    pr_info("OFFSET = 0x%X\n", rw.offset);
    pr_info("LEN    = %u\n", rw.len);*/

   // pr_info("[KERNEL] DMA DATA:\n");

    for (j = 0; j < rw.len; j += 4)
    {
        uint32_t val;

        memcpy(&val,
               src8 + j,
               sizeof(uint32_t));

        //pr_info("0x%08X ", val);

        //if (((j / 4) + 1) % 8 == 0)
           // pr_info("\n");
    }

    //pr_info("\n");

   

    if (copy_to_user(rw.buf,
                     src8,
                     rw.len))
    {
        pr_err("copy_to_user failed\n");
        return -EFAULT;
    }

    //pr_info("DMA READ SUCCESS\n");

    return 0;
}
        
        case IOCTL_CARD1_MEM_READ_MODULE1:
            {
                unsigned short Data;
                unsigned long addr;
                unsigned short __user *user_ptr;

                if (ioctl_param == 0) {
                    printk(KERN_ERR "*MEDefMsg1M_READ_APOORVA_PCI: Invalid user-space pointer (NULL)\n");
                    return -EINVAL;
                }

                user_ptr = (unsigned short __user *) ioctl_param;

                
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**MEM_READ_APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

               
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) 
                {
                
                    printk(KERN_ERR "***MEM_READ_APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }
              //  printk(" addr=%08x",addr);

                if (addr < 0x0 || addr > 0x3FFFF) {
                    printk(KERN_ERR "****MEM_READ_APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }

                Data = readw(BaseDDC + addr);
               // printk(KERN_INFO "*****MEM_READ_APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);
                 
             
                struct mem_ioctl var;  //30-10-26
                var.u_value=Data;
                var.u_addr=addr;
      
                if (copy_to_user(user_ptr, &var, sizeof(var))) 
                {
                    printk(KERN_ERR "******MEM_READ_APOORVA_PCI: Failed to copy data to user\n");
                    return -EFAULT;
                }
                return 0;
            }       
          break; 

        
        
        case IOCTL_GET_BIT_STATUS:
                int status = 0;
                u32 val;
                reg_base = ioremap(C4A00_ADDR, REG_SIZE);
        if (!reg_base) {
            printk(KERN_ERR "Failed to map register at 0x%x\n", C4A00_ADDR);
            return -ENOMEM;
        }
                val = readl(reg_base);
                 
          printk(KERN_INFO "Register value at 0xC4A00 = 0x%08X\n", val);
                if (val & BIT1_MASK)
                    status = 1;   // bit HIGH
                else
                    status = 0;   // bit LOW

                if (copy_to_user((int __user *)ioctl_param,
                                  &status,
                                  sizeof(status)))
                    return -EFAULT;
                break;

   
          case IOCTL_CARD1_MEM_WRITE_MODULE1:
          {
              Addr user_data;

              if (copy_from_user(&user_data, (Addr __user *)ioctl_param, sizeof(Addr))) {
                  printk(KERN_ERR "MEM_WR_APOORVA_PCI: Failed to copy data from user\n");
                  return -EFAULT;
              }

              if ((user_data.Addr >= 0x0) && (user_data.Addr <= 0x3FFFF)) {
                  writel(user_data.Data, (BaseDDC) + (user_data.Addr));
                 /*printk(KERN_INFO "MEM_WR_APOORVA_PCI: CARD1_MEM_WRITE_MOD_1 : ADDR: %08X, Data: %X\n",
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

              
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**REG_READ_APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

               
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "***REG_READ_APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                if (addr < 0x0 || addr > 0x1FFFF) {
                    printk(KERN_ERR "****REG_READ_APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }
               // addr = addr<<1;
                Data = readw((BaseDDC+(0xc0000)) + addr);
               // printk(KERN_INFO "*****REG_READ_APOORVA_PCI: CARD1_REG_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

                //  Copy read data back to user-space
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
                    writew(user_data.Data, (BaseDDC+(0xc0000)) + (user_data.Addr));
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

              
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**MEM_READ_APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

              
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "***MEM_READ_APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                if (addr < 0x0 || addr > 0x3FFFF) {
                    printk(KERN_ERR "****MEM_READ_APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }

                Data = readw((BaseDDC)+(0x40000) + addr);
               // printk(KERN_INFO "*****MEM_READ_APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

                
                         struct mem_ioctl var;
                          var.u_value=Data;
                          var.u_addr=addr;
                
                if (copy_to_user(user_ptr, &var, sizeof(var))) {
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

              if ((user_data.Addr >= 0x0) && (user_data.Addr <= 0x3FFFF)) {
                  writew(user_data.Data, (BaseDDC)+(0x40000) + (user_data.Addr));
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

               
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**REG_READ_APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "***REG_READ_APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                if (addr < 0x0 || addr > 0x1FFFF) {
                    printk(KERN_ERR "****REG_READ_APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }
               // addr = addr<<1;
                Data = readw((BaseDDC+(0xc0800)) + addr);
               // printk(KERN_INFO "*****REG_READ_APOORVA_PCI: CARD1_REG_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

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
                  writew(user_data.Data, (BaseDDC+(0xc0800)) + (user_data.Addr));
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

                
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**MEM_READ_APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

               
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "***MEM_READ_APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                if (addr < 0x0 || addr > 0x3FFFF) { 
                printk(KERN_ERR "****MEM_READ_APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }
                Data = readw((BaseDDC)+(0x80000) + addr);
               // printk(KERN_INFO "*****MEM_READ_APOORVA_PCI: CARD1_MEM_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

                          struct mem_ioctl var;
                          var.u_value=Data;
                          var.u_addr=addr;
                
               
                if (copy_to_user(user_ptr, &var, sizeof(var))) {
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

              if ((user_data.Addr >= 0x0) && (user_data.Addr <= 0x3FFFF)) {
                  writew(user_data.Data, (BaseDDC)+(0x80000) + (user_data.Addr));
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

             
                if (!access_ok(user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "**REG_READ_APOORVA_PCI: User-space pointer is invalid\n");
                    return -EFAULT;
                }

                
                if (copy_from_user(&addr, user_ptr, sizeof(addr))) {
                    printk(KERN_ERR "***REG_READ_APOORVA_PCI: Failed to copy address from user\n");
                    return -EFAULT;
                }

                if (addr < 0x0 || addr > 0x1FFFF) {
                    printk(KERN_ERR "****REG_READ_APOORVA_PCI: Invalid memory address: %08lX\n", addr);
                    return -EINVAL;
                }
               // addr = addr<<1;
                Data = readw((BaseDDC+(0xc1000)) + addr);
               // printk(KERN_INFO "*****REG_READ_APOORVA_PCI: CARD1_REG_READ_MOD_1 : ADDR: %08lX, Data: %X\n", addr, Data);

                
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
                  writew(user_data.Data, (BaseDDC+(0xc1000)) + (user_data.Addr));
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

                   
                    if (user_data.Addr > 0x1FFFF) {
                        printk(KERN_ERR "IOCTL_CARD1_WRITE_PORT: Invalid I/O offset: 0x%X\n", user_data.Addr);
                        return -EINVAL;
                    }
                    
                   
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
			
	
		//Interrupts  
			 
	case IOCTL_CARD1_INIT_INT_MODULE1:
	            iowrite16(CONFIG_REG_INIT_VAL,(BaseDDC+(CONFIG_REG_MOD1 >>1)));
	            break;
	            
	case IOCTL_CARD1_ENB_INT_MODULE1:
	            iowrite16(BC_INT_EN_VAL,(BaseDDC+(BC_INT_EN_REG >>1))); 
	            break;
	            
	case IOCTL_CARD1_MEM_INTR_WRITE:
            {
                INTR intr;

                if (copy_from_user(&intr, (INTR __user *)ioctl_param, sizeof(INTR)))
                    return -EFAULT;

                //printk("Reg: 0x%x, Val: 0x%x\n",intr.reg, intr.val);

                iowrite16(intr.val, BaseDDC + intr.reg);

                break;
            }
            
        case IOCTL_CARD1_MOD1_SET_BCEOMCNT:
          {
              int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

            
              BCEOMCnt = val;
              return 0;
          }
          case IOCTL_CARD1_MOD2_SET_BCEOMCNT:
          {
              int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              
              BCEOM2Cnt = val;
              return 0;
          }
           case IOCTL_CARD1_MOD3_SET_BCEOMCNT:
          {
              int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

          
              BCEOM3Cnt = val;
              return 0;
          }
          case IOCTL_CARD1_MOD1_SET_BCSOMCNT:
          {
             int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              BCSOM1=val;   

              return 0;
          }
          break;
           case IOCTL_CARD1_MOD2_SET_BCSOMCNT:
          {
             int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              BCSOM2=val;   

              return 0;
          }
          break;
           case IOCTL_CARD1_MOD3_SET_BCSOMCNT:
          {
             int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              BCSOM3=val;   

              return 0;
          }
          case IOCTL_CARD1_MOD1_SET_MTSOMCNT:
          {
             int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              MTSOM1=val;   

              return 0;
          }
          case IOCTL_CARD1_MOD2_SET_MTSOMCNT:
          {
             int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              MTSOM2=val;   

              return 0;
          }
          case IOCTL_CARD1_MOD3_SET_MTSOMCNT:
          {
             int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              MTSOM3=val;   

              return 0;
          }
          case IOCTL_CARD1_MOD1_SET_RTSOMCNT:
          {
             int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              RTSOM1=val;   

              return 0;
          }
          case IOCTL_CARD1_MOD2_SET_RTSOMCNT:
          {
             int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              RTSOM2=val;   

              return 0;
          }
          case IOCTL_CARD1_MOD3_SET_RTSOMCNT:
          {
             int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              RTSOM2=val;   

              return 0;
          }
          case IOCTL_CARD1_MOD1_SET_MTEOMCNT:
          {
              int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              MTEOMCnt=val;   
              MTSOM1 = val;
              return 0;
          }
          case IOCTL_CARD1_MOD2_SET_MTEOMCNT:
          {
              int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              MTEOMCnt2=val;
               MTSOM2 = val;

              return 0;
          }
          case IOCTL_CARD1_MOD3_SET_MTEOMCNT:
          {
              int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              MTEOMCnt3=val;   
              MTSOM3 = val;
              return 0;
          }
          case IOCTL_CARD1_MOD2_SET_RTEOMCNT:
          {
             int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              RTEOMCnt2=val;   

              return 0;
          }
           case IOCTL_CARD1_MOD3_SET_RTEOMCNT:
          {
             int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              RTEOMCnt3=val;   

              return 0;
          }
          case IOCTL_CARD1_MOD1_SET_RTEOMCNT:
          {
             int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              RTEOMCnt=val;   

              return 0;
          }
            case IOCTL_CARD1_MOD1_SET_BCEOFCNT:
          {
              int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;
  
             BCEOFCnt=val;
              return 0;
          }
            case IOCTL_CARD1_MOD2_SET_BCEOFCNT:
          {
              int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

             BCEOF2Cnt=val;
              return 0;
          }
            case IOCTL_CARD1_MOD3_SET_BCEOFCNT:
          {
              int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;
  
             BCEOF3Cnt=val;
              return 0;
          }
           case IOCTL_CARD1_MOD1_SET_BCEOFMINCNT:
          {
              int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

              BCEOMFCnt=val;

              return 0;
          }
          
            case IOCTL_CARD1_MOD2_SET_BCEOFMINCNT:
          {
              int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

             BCEOMF2Cnt=val;

              return 0;
          }
          
            case IOCTL_CARD1_MOD3_SET_BCEOFMINCNT:
          {
              int val;

              if (copy_from_user(&val, (int __user *)ioctl_param, sizeof(val)))
                  return -EFAULT;

             BCEOMF3Cnt=0;

              return 0;
          }
          
        case IOCTL_CARD1_MOD1_GET_BCEOMCNT:
        {
          int count;
            count = BCEOMCnt;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        case IOCTL_CARD1_MOD2_GET_BCEOMCNT:
        {
          int count;
            count = BCEOM2Cnt;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        
         case IOCTL_CARD1_MOD3_GET_BCEOMCNT:
        {
          int count;
            count = BCEOM3Cnt;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        
         case IOCTL_CARD1_MOD1_GET_BCSOMCNT:
        {
          int count;
            count = BCSOM1++;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
         case IOCTL_CARD1_MOD2_GET_BCSOMCNT:
        {
          int count;
            count = BCSOM2++;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        case IOCTL_CARD1_MOD3_GET_BCSOMCNT:
        {
          int count;
            count = BCSOM3++;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        case IOCTL_CARD1_MOD1_GET_MTSOMCNT:
        {
          int count;
            count = MTSOM1;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        case IOCTL_CARD1_MOD2_GET_MTSOMCNT:
        {
          int count;
            count = MTSOM2++;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        case IOCTL_CARD1_MOD3_GET_MTSOMCNT:
        {
          int count;
            count = MTSOM3++;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        case IOCTL_CARD1_MOD1_GET_RTSOMCNT:
        {
          int count;
            count = RTSOM1++;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        case IOCTL_CARD1_MOD2_GET_RTSOMCNT:
        {
          int count;
            count = RTSOM2++;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        case IOCTL_CARD1_MOD3_GET_RTSOMCNT:
        {
          int count;
            count = RTSOM3++;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
         case IOCTL_CARD1_MOD1_GET_MTEOMCNT:
        {
          int count;
            count = MTEOMCnt;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        case IOCTL_CARD1_MOD2_GET_MTEOMCNT:
        {
          int count;
            count = MTEOMCnt2;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
         case IOCTL_CARD1_MOD3_GET_MTEOMCNT:
        {
          int count;
            count = MTEOMCnt3;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        case IOCTL_CARD1_MOD2_GET_RTEOMCNT:
        {
          int count;
            count = RTEOMCnt2++;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
         case IOCTL_CARD1_MOD3_GET_RTEOMCNT:
        {
          int count;
            count = RTEOMCnt3++;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        case IOCTL_CARD1_MOD1_GET_RTEOMCNT:
        {
          int count;
            count = RTEOMCnt++;

            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        case IOCTL_CARD1_MOD1_GET_BCEOFCNT:
        {
          int count;
            count=BCEOFCnt;
            
            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
         case IOCTL_CARD1_MOD2_GET_BCEOFCNT:
        {
          int count;
            count=BCEOF2Cnt;
            
            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
         case IOCTL_CARD1_MOD3_GET_BCEOFCNT:
        {
          int count;
            count=BCEOF3Cnt;
            
            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;

            return 0;
        }
        
        case IOCTL_CARD1_MOD1_GET_BCEOFMINCNT:
        {
          int count;
          count=BCEOMFCnt++;
            
            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;
            
            return 0;
        }
        
        case IOCTL_CARD1_MOD2_GET_BCEOFMINCNT:
        {
          int count;
          count=BCEOMF2Cnt++;
            
            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;
            
            return 0;
        }
        case IOCTL_CARD1_MOD3_GET_BCEOFMINCNT:
        {
          int count;
          count=BCEOMF3Cnt++;
            
            if (copy_to_user((int __user *)ioctl_param, &count, sizeof(count)))
                return -EFAULT;
            return 0;
        }
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
release : device_close,
mmap    : my_mmap,
};

static int msg_active =0;
static int mt_pattern_err_cnt = 0;
static int prev_intr1 = 0;

irqreturn_t ISR(int irq, void *dev_id)
{
    u16 pci_status;
    
    pci_read_config_word(dev_APOORVA, 0x06, &pci_status);
   
    if(((pci_status & 0x0008) >> 3) == 0x1)
    {
      
      /****************** DMA INTERRUPTS *******************/
      #if 1
      u32 DMACOMPLETE = ioread32(BaseDDC + (DMA_COMPLETE >> 1));
      pr_info("DMA Complete Status = 0x%08x\n",DMACOMPLETE);
      if(DMACOMPLETE & 0x00000002)
      {
      pr_info("Reg: 0x%08X\n",DMACOMPLETE);
      pr_info("DMA Read Complete Interrupt \n");
      }
      if(DMACOMPLETE & 0x00000004)
      {
      pr_info("Reg: 0x%08X\n",DMACOMPLETE);
      pr_info("DMA Write Complete Interrupt \n");
      }
      
      iowrite32(0x00000008 , BaseDDC + (DMA_CLEAR >> 1));
      #endif 
      /************** Module1 Interrupts **************/
      
      u16 BCINTR1 = ioread16(BaseDDC + (INT_STS_BC_MOD1 >> 1));
      u16 MTINTR1 = ioread16(BaseDDC + (INT_STS_MT_MOD1 >> 1));
      u16 RTINTR1 = ioread16(BaseDDC + (INT_STS_RT_MOD1 >> 1));
   
       /************ BC INTERRUPTS ******************/
    if(BCINTR1 != 0x0)
    {
      if(BCINTR1 & 0x01 )
      {
          BCEOMCnt++;                                                    /*** BC END OF MESSAGE ***/
      }
      if(BCINTR1 & 0x4000)
      {
         BCSOM1++;                                                    /*** BC START OF MESSAGE ***/
      }
      if(BCINTR1 & 0x100)
      {
        BCBCR1++;
      }
      if(BCINTR1 & 0x02)
      {
          BCStatusSet1++;                                                 /*** STATUS SET ***/
      }
      if(BCINTR1 & 0x04)
      {
          BCFMT1++;
      }
      if(BCINTR1 & 0x08)
      {   
          BCEOFCnt++;                                                   /*** BC END OF MAJOR FRAME ***/
      }
      if(BCINTR1 & 0x20)
      {
         BCEOMFCnt++;                                                  /*** BC END OF MINOR FRAME ***/
      }
      if(BCINTR1 & 0x40)
      {
          BCTTRollOver1++;                                             /*** TIME TAG ROLLOVER ***/
      }
      
      if(BCINTR1 & 0x10)
      {
        BCCWEOM1++;
      }
      
      if(BCINTR1 & 0x1000)
      {
        BCCMDSRO1++;
      }
        
    iowrite16(BCINTR1, BaseDDC + (INT_CLR_REG_MOD1 >> 1)); 
    iowrite16(INT_CLR_BC_VAL  , BaseDDC + (CONFIG_REG_MOD1 >> 1));
  }
   
  /********************** MT INTERRUPTS ******************/
    if(MTINTR1 != 0x0)
    {
      if(MTINTR1 & 0x01 )
      {
          MTEOMCnt++;
          msg_active =0;
      }
      
      if(MTINTR1 & (1 << 10))
      {
          MTDATACnt++;
      }

      if(MTINTR1 & (1 << 11))
      {
          MTCMDCnt++;
      }
      if((MTINTR1 & 0x0040) )//&& !(prev_intr1 & 0x0040)
      { 
        MTTTRollOver1++;
        u16 time;
        time =ioread16(BaseDDC + (0x181084 >> 1));//(0x181090 >> 1));
      }
      if(MTINTR1 & 0x04)
      {
          MTFRMCnt++;
      }
     if(MTINTR1 & 0x4000)
      {
          MTSOM1++;
      }
   

    iowrite16(MTINTR1, BaseDDC + (INT_CLR_REG_MOD1 >> 1));
  
    iowrite16(INT_CLR_MT_VAL  , BaseDDC + (CONFIG_REG_MOD1 >> 1));

  }
  /********************* RT INTERRUPTS ********************/
  #if 1
  //u16 RTINTR1 = ioread16(BaseDDC + (INT_STS_RT_MOD1 >> 1));
  
  if(RTINTR1 != 0x0)
  {
    
     if(RTINTR1 & 0x4000)
     {
        RTSOM1++;     
     }
     if(RTINTR1 & 0x01 )
      {
          RTEOMCnt++;
      }
     //  Circular Buffer Rollover (bit 5)
    if(RTINTR1 & 0x0020)
    {
        RTCIRCnt++;
    }

    // Command Stack Rollover (bit 12)
    if(RTINTR1 & 0x1000)
    {
        RTCMDCnt++;
    }
    if(RTINTR1 & 0x0002)
    {
        RTMDCnt++;
    }
    if(RTINTR1 & 0x04)
      {
          RTFRMCnt++;
      }
     if(RTINTR1 & 0x10)
      {
         RXEOMCnt++;
      }
     // Parity Error Interrupt (bit 7)
      if(RTINTR1 & 0x0080)
      {
          RTPARCnt++;   
      }
      if((RTINTR1 & 0x0040) )//&& !(prev_intr1 & 0x0040)
      { 
        RTTTRollOver1++;
      }
      
        iowrite16(RTINTR1,(BaseDDC+(RT_INT_MASK_WR_MOD1 >>1))); 
  }
  #endif
  /********************** Module2 Interrupts ************************/
  u16 BCINTR2 = ioread16(BaseDDC + (INT_STS_BC_MOD2 >> 1));
  u16 MTINTR2 = ioread16(BaseDDC + (INT_STS_MT_MOD2 >> 1));
  u16 RTINTR2 = ioread16(BaseDDC + (INT_STS_RT_MOD2 >> 1));
 
      /************ BC INTERRUPTS ******************/
  
    if(BCINTR2 != 0x0)
    {
     
    
      if(BCINTR2 & 0x01 )
      {
          BCEOM2Cnt++;                                                    /*** BC END OF MESSAGE ***/
      }
      if(BCINTR2 & 0x4000)
      {
         BCSOM2++;                                                    /*** BC START OF MESSAGE ***/
      }
      if(BCINTR2 & 0x100)
      {
        BCBCR2++;
      }
      if(BCINTR2 & 0x02)
      {
          BCStatusSet2++;                                                 /*** STATUS SET ***/
      }
      if(BCINTR2 & 0x04)
      {
          BCFMT2++;
      }
      if(BCINTR2 & 0x08)
      {
          BCEOF2Cnt++;                                                   /*** BC END OF MAJOR FRAME ***/
      }
      if(BCINTR2 & 0x20)
      { 
         BCEOMF2Cnt++;                                                  /*** BC END OF MINOR FRAME ***/
      }
      if(BCINTR2 & 0x40)
      {
          BCTTRollOver2++;                                             /*** TIME TAG ROLLOVER ***/
      }
      
      if(BCINTR2 & 0x10)
      {
        BCCWEOM2++;
      }
      
      if(BCINTR2 & 0x1000)
      {
        BCCMDSRO2++;
      }
      
    iowrite16(BCINTR2, BaseDDC + (INT_CLR_REG_MOD2 >> 1)); 
    iowrite16(INT_CLR_BC_VAL  , BaseDDC + (CONFIG_REG_MOD2 >> 1));
  }
   
  
  /********************** MT INTERRUPTS ******************/

   if(MTINTR2 != 0x0)
    {
    
      if(MTINTR2 & 0x01 )
      {
          MTEOMCnt2++; 
      }
      
      if(MTINTR2 & (1 << 10))
      {
          MTDATACnt2++;
      }

      if(MTINTR2 & (1 << 11))
      {
          MTCMDCnt2++;
      }
      if((MTINTR2 & 0x0040) )//&& !(prev_intr1 & 0x0040)
      { 
        MTTTRollOver2++;
      }
      if(MTINTR2 & 0x04)
      {
          MTFRMCnt2++;
      }
     if(MTINTR2 & 0x4000)
      {
          MTSOM2++;
      }
      
    iowrite16(MTINTR2, BaseDDC + (INT_CLR_REG_MOD2 >> 1));
    iowrite16(INT_CLR_MT_VAL  , BaseDDC + (CONFIG_REG_MOD2 >> 1));

  }
   
   /********************* RT INTERRUPTS ********************/
  #if 1
  if(RTINTR2 != 0x0)
  {
    
     if(RTINTR2 & 0x4000)
     {
        RTSOM2++;   
     }
     if(RTINTR2 & 0x01 )
      {
          RTEOMCnt2++;
      }
     //  Circular Buffer Rollover (bit 5)
    if(RTINTR2 & 0x0020)
    {
        RTCIRCnt2++;
    }

    // Command Stack Rollover (bit 12)
    if(RTINTR2 & 0x1000)
    {
        RTCMDCnt2++;
    }
    if(RTINTR2 & 0x0002)
    {
        RTMDCnt2++;
    }
    if(RTINTR2 & 0x04)
      {
          RTFRMCnt2++;
      }
     if(RTINTR2 & 0x10)
      {
         RXEOMCnt2++;
      }
     // Parity Error Interrupt (bit 7)
      if(RTINTR2 & 0x0080)
      {
          RTPARCnt2++;
      }
      if((RTINTR2 & 0x0040) )//&& !(prev_intr1 & 0x0040)
      { 
        RTTTRollOver2++;
      }
      
     iowrite16(RTINTR2,(BaseDDC+(RT_INT_MASK_WR_MOD2 >>1)));
  }
  #endif
  
  /********************** Module3 Interrupts ************************/
  u16 BCINTR3 = ioread16(BaseDDC + (INT_STS_BC_MOD3 >> 1));
  u16 MTINTR3 = ioread16(BaseDDC + (INT_STS_MT_MOD3 >> 1));
  u16 RTINTR3 = ioread16(BaseDDC + (INT_STS_RT_MOD3 >> 1));
  
  /************ BC INTERRUPTS ******************/
     
  if(BCINTR3 != 0x0)
    {
      if(BCINTR3 & 0x01 )
      {
          BCEOM3Cnt++;
      }
      if(BCINTR3 & 0x4000)
      {
        BCSOM3++;
      }
      if(BCINTR3 & 0x02)
      {
          BCStatusSet3++;
      }
      if(BCINTR3 & 0x04)
      {
          BCFMT3++;
      }
      if(BCINTR3 & 0x08)
      {
          BCEOF3Cnt++; 
      }
      if(BCINTR3& 0x20)
      {
         BCEOMF3Cnt++;  
      }
      if(BCINTR3 & 0x40)
      {
          BCTTRollOver3++;
      }
     
      if(BCINTR3 & 0x10)
      {
        BCCWEOM3++;
      }
      if(BCINTR3 & 0x100)
      {
        BCBCR3++;
      }
      if(BCINTR3 & 0x1000)
      {
        BCCMDSRO3++;
      } 
      
    iowrite16(BCINTR3, BaseDDC + (INT_CLR_REG_MOD3 >> 1));
    iowrite16(INT_CLR_BC_VAL  , BaseDDC + (CONFIG_REG_MOD3 >> 1));
  } 
  
  /********************** MT INTERRUPTS ******************/

   if(MTINTR3 != 0x0)
    {
      if(MTINTR3 & 0x01 )
      {
          MTEOMCnt3++;
          msg_active =0;
      }
      
      if(MTINTR3 & (1 << 10))
      {
          MTDATACnt3++;
      }

      if(MTINTR3 & (1 << 11))
      {
          MTCMDCnt3++;
      }
      if((MTINTR3 & 0x0040) )//&& !(prev_intr1 & 0x0040)
      { 
        MTTTRollOver3++;
      }
      if(MTINTR3 & 0x04)
      {
          MTFRMCnt3++;
      }
     if(MTINTR3 & 0x4000)
      {
        if(msg_active == 0)
        {
          MTSOM3++;
          msg_active = 1;
        }
      }
      prev_intr1 = MTINTR3;   

    iowrite16(MTINTR3, BaseDDC + (INT_CLR_REG_MOD3 >> 1));
    iowrite16(INT_CLR_MT_VAL  , BaseDDC + (CONFIG_REG_MOD3 >> 1));

  }
   
   /********************* RT INTERRUPTS ********************/
  #if 1
  if(RTINTR3 != 0x0)
  {
    
     if(RTINTR3 & 0x4000)
     {
        RTSOM3++;     
     }
     if(RTINTR3 & 0x01 )
      {
          RTEOMCnt3++;
      }
     //  Circular Buffer Rollover (bit 5)
    if(RTINTR3 & 0x0020)
    {
        RTCIRCnt3++;
    }

    // Command Stack Rollover (bit 12)
    if(RTINTR3 & 0x1000)
    {
        RTCMDCnt3++;
    }
    if(RTINTR3 & 0x0002)
    {
        RTMDCnt3++;
    }
    if(RTINTR3 & 0x04)
      {
          RTFRMCnt3++;
      }
     if(RTINTR3 & 0x10)
      {
         RXEOMCnt3++;
      }
     // Parity Error Interrupt (bit 7)
      if(RTINTR3 & 0x0080)
      {
          RTPARCnt3++;  
      }
      if((RTINTR3 & 0x0040) )//&& !(prev_intr1 & 0x0040)
      { 
        RTTTRollOver3++;
      }
      
     iowrite16(RTINTR3,(BaseDDC+(RT_INT_MASK_WR_MOD3 >>1)));
  }
  #endif  
 }
    return IRQ_HANDLED;
}

/***************Anil******************/
static int __init APOORVA_CARD1_INIT(void)
{
    int ret;
    u16 val;
    u16 val1;
    void *irqdevid = (void *)0x12;

    pr_info("========================================\n");
    pr_info("APOORVA1553: Driver Init Start\n");
    pr_info("Vendor ID: 0x%X Device ID: 0x%X\n", Vendor_ID, Device_ID);

    /* Find PCI device */
    dev_APOORVA = pci_get_device(Vendor_ID, Device_ID, NULL);
    if (!dev_APOORVA) {
        pr_err("APOORVA1553: Device not found\n");
        return -ENODEV;
    }

    pr_info("PCI Device Found!\n");
    pr_info("Bus: %d Slot: %d Function: %d\n",
            dev_APOORVA->bus->number,
            PCI_SLOT(dev_APOORVA->devfn),
            PCI_FUNC(dev_APOORVA->devfn));

    
    ret = pci_enable_device_mem(dev_APOORVA);
    pr_info("pci_enable_device_mem returned: %d\n", ret);
    if (ret) {
        pr_err("pci_enable_device_mem failed\n");
        goto err_put_dev;
    }

    pci_set_master(dev_APOORVA);
    pr_info("Bus Master Enabled\n");

    
    ret = dma_set_mask_and_coherent(&dev_APOORVA->dev, DMA_BIT_MASK(32));
    pr_info("dma_set_mask_and_coherent returned: %d\n", ret);
    if (ret) {
        pr_err("32-bit DMA not supported\n");
        goto err_disable;
    }

    
    pr_info("BAR0 Physical Start: 0x%llX\n",(unsigned long long)pci_resource_start(dev_APOORVA, 0));
    pr_info("BAR0 Length: 0x%llX\n",(unsigned long long)pci_resource_len(dev_APOORVA, 0));
    pr_info("BAR0 Flags: 0x%lX\n",pci_resource_flags(dev_APOORVA, 0));

    ret = pci_request_regions(dev_APOORVA, "APOORVA1553");
    pr_info("pci_request_regions returned: %d\n", ret);
    if (ret) {
        pr_err("pci_request_regions failed\n");
        goto err_disable;
    }

 
    BaseDDC = pci_iomap(dev_APOORVA, 0, 0);
    pr_info("pci_iomap returned virtual address: %p\n", BaseDDC);
    if (!BaseDDC) {
        pr_err("BAR0 iomap failed\n");
        ret = -ENOMEM;
        goto err_release_regions;
    }

   
    IntrLine = dev_APOORVA->irq;
    pr_info("Assigned IRQ Number: %d\n", IntrLine);

    
    ret = request_irq(IntrLine, ISR, IRQF_SHARED,
                      "APOORVA1553", irqdevid);
    pr_info("request_irq returned: %d\n", ret);
    if (ret) {
        pr_err("IRQ request failed\n");
        goto err_iounmap;
    }
  
    pr_info("IRQ Registered Successfully\n");
    u16 status;
    
     //Interrupt Controller Configuration
     iowrite16(CONFIG_REG_RST_VAL,(BaseDDC+(CONFIG_REG_MOD1 >>1)));  //reset
     
     iowrite16(CONFIG_REG_RST_VAL,(BaseDDC+(CONFIG_REG_MOD2 >>1)));  //reset
      
    iowrite16(CONFIG_REG_RST_VAL,(BaseDDC+(CONFIG_REG_MOD3 >>1)));  //reset
   //iowrite16(CONFIG_REG_INIT_VAL,(BaseDDC+((NODE_SEL + CONFIG_REG) >>1))); //initilaization 
   
    
    // Mask Registers
    iowrite16(BC_INT_MASK_VAL,(BaseDDC+(BC_INT_MASK_WR_MOD1 >>1))); 
    iowrite16(BC_INT_MASK_VAL,(BaseDDC+(BC_INT_MASK_WR_MOD2 >>1))); 
    iowrite16(BC_INT_MASK_VAL,(BaseDDC+(BC_INT_MASK_WR_MOD3 >>1))); 
   
    
    iowrite16(RT_INT_MASK_VAL,(BaseDDC+(RT_INT_MASK_WR_MOD2>>1)));
    
    iowrite16(MT_INT_MASK_VAL,(BaseDDC+(MT_INT_MASK_WR_MOD1 >>1)));
    iowrite16(MT_INT_MASK_VAL,(BaseDDC+(MT_INT_MASK_WR_MOD2 >>1)));
    
    iowrite16(0x0100,(BaseDDC+(0x181084 >>1)));
    u16 time_read;
    time_read =ioread16(BaseDDC + (0x181084 >> 1));
    pr_info("time_read : 0x%04x\n",time_read);
    
//    iowrite16(0x1234,(BaseDDC+(0x181090 >>1)));
    
     u16 Rt_mask;
     Rt_mask = ioread16((BaseDDC+(RT_INT_MASK_RD_REG >>1)));
     pr_info("Rt_mask = 0x%04x\n", Rt_mask);
     pr_info("hello");
       
    //iowrite16(BC_INT_EN_VAL,(BaseDDC+(BC_INT_EN_REG >>1)));  //BC interrupts Enable
    
    //iowrite16(RT_INT_EN_VAL,(BaseDDC+(RT_INT_EN_REG >>1)));  //RT Interrupts Enable
    
    //iowrite16(MT_INT_EN_VAL,(BaseDDC+(MT_INT_EN_REG >>1)));  //MT Interrupts Enable
    
    pci_read_config_word(dev_APOORVA, 0x06, &status);
    pr_info("PCI Status Register = 0x%04x\n", status);

    
   dma_size = DMA_ALLOC_SIZE;
  pr_info("Requesting DMA size: %zu bytes\n", dma_size);

  dma_cpu_buf = dma_alloc_coherent(&dev_APOORVA->dev,
                                   dma_size,
                                   &dma_handle,
                                   GFP_KERNEL);

  if (!dma_cpu_buf) {
      pr_err("dma_alloc_coherent failed\n");
      ret = -ENOMEM;
      goto err_free_irq;
  }

  pr_info("DMA Allocation Successful\n");
  pr_info("DMA Virtual Address : %p\n", dma_cpu_buf);
  pr_info("DMA Handle          : %pad\n", &dma_handle);
  pr_info("DMA Size            : %zu\n", dma_size);

  /* 🔴 IMPORTANT FIX: clear stale DMA data */
  memset(dma_cpu_buf, 0, dma_size);

  
    MajorNo = register_chrdev(0, "APOORVA1553", &fops);
    pr_info("register_chrdev returned MajorNo: %d\n", MajorNo);
    if (MajorNo < 0) {
        pr_err("register_chrdev failed\n");
        ret = MajorNo;
        goto err_dma_free;
    }

    pr_info("Character Device Registered Successfully\n");
    pr_info("APOORVA1553: Driver loaded successfully\n");
    pr_info("========================================\n");

    return 0;


err_dma_free:
    pr_info("Freeing DMA memory\n");
    dma_free_coherent(&dev_APOORVA->dev,dma_size,dma_cpu_buf,dma_handle);

err_free_irq:
    pr_info("Freeing IRQ %d\n", IntrLine);
    free_irq(IntrLine, irqdevid);

err_iounmap:
    pr_info("Unmapping BAR0\n");
    pci_iounmap(dev_APOORVA, BaseDDC);

err_release_regions:
    pr_info("Releasing PCI regions\n");
    pci_release_regions(dev_APOORVA);

err_disable:
    pr_info("Disabling PCI device\n");
    pci_disable_device(dev_APOORVA);

err_put_dev:
    pr_info("Putting PCI device\n");
    pci_dev_put(dev_APOORVA);

    pr_info("APOORVA1553: Driver Init Failed with ret=%d\n", ret);
    pr_info("========================================\n");

    return ret;
}

static void __exit APOORVA_CARD1_EXIT(void)
{
    void *irqdevid = (void *)0x12;

    pr_info("APOORVA1553: Driver Exit Start\n");

    unregister_chrdev(MajorNo, "APOORVA1553");

    
    if (dma_cpu_buf) {
        dma_free_coherent(&dev_APOORVA->dev,
                          dma_size,
                          dma_cpu_buf,
                          dma_handle);
    }

   
    free_irq(IntrLine, irqdevid);

   
    if (BaseDDC)
        pci_iounmap(dev_APOORVA, BaseDDC);

  
    pci_release_regions(dev_APOORVA);

    
    pci_disable_device(dev_APOORVA);

 
    pci_dev_put(dev_APOORVA);

    pr_info("APOORVA1553: Driver unloaded successfully\n");
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


module_init(APOORVA_CARD1_INIT);
module_exit(APOORVA_CARD1_EXIT);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MMR/GMR");


