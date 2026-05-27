#include <linux/ioctl.h>
#include <asm/ioctl.h>
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif


typedef struct _Addr
{
	unsigned int Addr;
	unsigned short Data;
}Addr;


struct reg_op1 {
    uint32_t offset;
    uint32_t value;   /* value to write */
};

struct reg_op2 {
    uint32_t offset;
    uint32_t value;   /* returned value for read */
};

struct dma_rw {
    void *buf;        /* user-space pointer */
    uint32_t len;     /* number of bytes to transfer */
    uint32_t offset;  /* offset into DMA kernel buffer */
};

/* DMA physical read (kernel -> user) */
struct dma_phys_rw {
    void *buf;        /* user-space pointer where kernel will copy data */
    uint32_t len;     /* number of bytes to copy */
    uint32_t offset;  /* offset inside kernel DMA buffer */
};

#define MAJOR_NUM_CARD1 1553
#define MAJOR_NUM_CARD2 2553
#define MAJOR_NUM_CARD3 3553
#define MAJOR_NUM_CARD4 4553
#define DEVICE_NAME "APOORVA1553"


static int MOD1_EOMCNT		=	0;
static int MOD2_EOMCNT		=	0;
static int MOD3_EOMCNT		=	0;

/************************************  CARD-1 IOCTL's	******************************/

#define IOCTL_CARD1_DEBUG_ENABLE   			_IOWR(MAJOR_NUM_CARD1,0x1,int)
#define IOCTL_CARD1_DEBUG_DISABLE   			_IOWR(MAJOR_NUM_CARD1,0x2,int)

#define IOCTL_CARD1_SET_ICS_REG   			_IOWR(MAJOR_NUM_CARD1,0x3,int)
#define IOCTL_CARD1_SOFTWARE_RESET   			_IOWR(MAJOR_NUM_CARD1,0x4,int)

#define IOCTL_CARD1_MEM_READ_MODULE1  			_IOR(MAJOR_NUM_CARD1,0x5,int)
#define IOCTL_CARD1_MEM_WRITE_MODULE1  			_IOW(MAJOR_NUM_CARD1,0x6,int)

#define IOCTL_CARD1_MEM_READ_MODULE2  			_IOR(MAJOR_NUM_CARD1,0x7,int)
#define IOCTL_CARD1_MEM_WRITE_MODULE2  			_IOW(MAJOR_NUM_CARD1,0x8,int)

#define IOCTL_CARD1_MEM_READ_MODULE3  			_IOR(MAJOR_NUM_CARD1,0x9,int)
#define IOCTL_CARD1_MEM_WRITE_MODULE3  			_IOW(MAJOR_NUM_CARD1,0xa,int)

#define IOCTL_CARD1_MEM_READ_MODULE4  			_IOR(MAJOR_NUM_CARD1,0xb,int)
#define IOCTL_CARD1_MEM_WRITE_MODULE4  			_IOW(MAJOR_NUM_CARD1,0xc,int)



#define IOCTL_CARD1_REG_READ_MODULE1  			_IOR(MAJOR_NUM_CARD1,0xd,int)
#define IOCTL_CARD1_REG_WRITE_MODULE1  			_IOW(MAJOR_NUM_CARD1,0xe,int)

#define IOCTL_CARD1_REG_READ_MODULE2  			_IOR(MAJOR_NUM_CARD1,0xf,int)
#define IOCTL_CARD1_REG_WRITE_MODULE2  			_IOW(MAJOR_NUM_CARD1,0x10,int)

#define IOCTL_CARD1_REG_READ_MODULE3  			_IOR(MAJOR_NUM_CARD1,0x11,int)
#define IOCTL_CARD1_REG_WRITE_MODULE3  			_IOW(MAJOR_NUM_CARD1,0x12,int)

#define IOCTL_CARD1_REG_READ_MODULE4  			_IOR(MAJOR_NUM_CARD1,0x13,int)
#define IOCTL_CARD1_REG_WRITE_MODULE4  			_IOW(MAJOR_NUM_CARD1,0x14,int)


#define IOCTL_CARD1_READ_PORT	  			_IOR(MAJOR_NUM_CARD1,0x15,int)
#define IOCTL_CARD1_WRITE_PORT	 			_IOW(MAJOR_NUM_CARD1,0x16,int)


#define IOCTL_CARD1_ENABLE_INT				_IOWR(MAJOR_NUM_CARD1,0x17,int)

#define IOCTL_UART_GET_INT				_IOWR(MAJOR_NUM_CARD1,0x18,int)
#define IOCTL_UART_SET_INT				_IOWR(MAJOR_NUM_CARD1,0x19,int)

#define IOCTL_UART_READ					_IOWR(MAJOR_NUM_CARD1,0x1a,int)
#define IOCTL_UART_WRITE				_IOWR(MAJOR_NUM_CARD1,0x1b,int)
#define IOCTL_SET_M1CNT                                 _IOW(MAJOR_NUM_CARD1,0x1c,int)
#define IOCTL_GET_M1CNT                                 _IOR(MAJOR_NUM_CARD1,0x1d,int)
#define IOCTL_CARD1_WRITE_PORT_A                        _IOW(MAJOR_NUM_CARD1,0x1e,int)
#define IOCTL_CARD1_READ_PORT_A                         _IOR(MAJOR_NUM_CARD1,0x1f,int)

#define IOCTL_WRITE_REG       _IOW(MAJOR_NUM_CARD1, 0x20, struct reg_op1)
#define IOCTL_READ_REG        _IOR(MAJOR_NUM_CARD1, 0x21, struct reg_op2)
#define IOCTL_DMA_ALLOC       _IOW(MAJOR_NUM_CARD1, 0x23, size_t)
#define IOCTL_DMA_FREE        _IO(MAJOR_NUM_CARD1,  0x24)
#define IOCTL_DMA_GET_PHYS    _IOR(MAJOR_NUM_CARD1, 0x25, uint64_t)
#define IOCTL_DMA_WRITE_USER  _IOW(MAJOR_NUM_CARD1, 0x26, struct dma_rw)   /* user -> dma buf */
#define IOCTL_DMA_READ_USER   _IOR(MAJOR_NUM_CARD1, 0x27, struct dma_rw)   /* dma buf -> user */
#define IOCTL_DMA_READ_PHYS   _IOWR(MAJOR_NUM_CARD1, 0x28, struct dma_phys_rw)


