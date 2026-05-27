#include <linux/ioctl.h>
#include <asm/ioctl.h>

typedef struct _Addr
{
	unsigned int Addr;
	unsigned short Data;
}Addr;

#define MAJOR_NUM_CARD1 1553
#define MAJOR_NUM_CARD2 2553
#define MAJOR_NUM_CARD3 3553
#define MAJOR_NUM_CARD4 4553
#define DEVICE_NAME "APOORVA1553"

static int MOD1_EOMCNT		=	0;
static int MOD2_EOMCNT		=	0;
static int MOD3_EOMCNT		=	0;

static int MOD1_EOFRAME		=	0;
static int MOD2_EOFRAME		=	0;
static int MOD3_EOFRAME		=	0;
static int MOD4_EOFRAME		=	0;

static int MOD1_MTTTROLL	=	0;
static int MOD2_MTTTROLL	=	0;
static int MOD3_MTTTROLL	=	0;
static int MOD4_MTTTROLL	=	0;

static int MOD1_MTDATASTKROLL	=	0;
static int MOD2_MTDATASTKROLL	=	0;
static int MOD3_MTDATASTKROLL	=	0;
static int MOD4_MTDATASTKROLL	=	0;

static int MOD1_MTCMDSTKROLL	=	0;
static int MOD2_MTCMDSTKROLL	=	0;
static int MOD3_MTCMDSTKROLL	=	0;
static int MOD4_MTCMDSTKROLL	=	0;

static int MOD1RT0EOMCnt	=	0;
static int MOD1RT1EOMCnt	=	0;
static int MOD1RT2EOMCnt	=	0;
static int MOD1RT3EOMCnt	=	0;
static int MOD1RT4EOMCnt	=	0;
static int MOD1RT5EOMCnt	=	0;
static int MOD1RT6EOMCnt	=	0;
static int MOD1RT7EOMCnt	=	0;
static int MOD1RT8EOMCnt	=	0;
static int MOD1RT9EOMCnt	=	0;
static int MOD1RT10EOMCnt	=	0;
static int MOD1RT11EOMCnt	=	0;
static int MOD1RT12EOMCnt	=	0;
static int MOD1RT13EOMCnt	=	0;
static int MOD1RT14EOMCnt	=	0;
static int MOD1RT15EOMCnt	=	0;
static int MOD1RT16EOMCnt	=	0;
static int MOD1RT17EOMCnt	=	0;
static int MOD1RT18EOMCnt	=	0;
static int MOD1RT19EOMCnt	=	0;
static int MOD1RT20EOMCnt	=	0;
static int MOD1RT21EOMCnt	=	0;
static int MOD1RT22EOMCnt	=	0;
static int MOD1RT23EOMCnt	=	0;
static int MOD1RT24EOMCnt	=	0;
static int MOD1RT25EOMCnt	=	0;
static int MOD1RT26EOMCnt	=	0;
static int MOD1RT27EOMCnt	=	0;
static int MOD1RT28EOMCnt	=	0;
static int MOD1RT29EOMCnt	=	0;
static int MOD1RT30EOMCnt	=	0;

static int MOD2RT0EOMCnt	=	0;
static int MOD2RT1EOMCnt	=	0;
static int MOD2RT2EOMCnt	=	0;
static int MOD2RT3EOMCnt	=	0;
static int MOD2RT4EOMCnt	=	0;
static int MOD2RT5EOMCnt	=	0;
static int MOD2RT6EOMCnt	=	0;
static int MOD2RT7EOMCnt	=	0;
static int MOD2RT8EOMCnt	=	0;
static int MOD2RT9EOMCnt	=	0;
static int MOD2RT10EOMCnt	=	0;
static int MOD2RT11EOMCnt	=	0;
static int MOD2RT12EOMCnt	=	0;
static int MOD2RT13EOMCnt	=	0;
static int MOD2RT14EOMCnt	=	0;
static int MOD2RT15EOMCnt	=	0;
static int MOD2RT16EOMCnt	=	0;
static int MOD2RT17EOMCnt	=	0;
static int MOD2RT18EOMCnt	=	0;
static int MOD2RT19EOMCnt	=	0;
static int MOD2RT20EOMCnt	=	0;
static int MOD2RT21EOMCnt	=	0;
static int MOD2RT22EOMCnt	=	0;
static int MOD2RT23EOMCnt	=	0;
static int MOD2RT24EOMCnt	=	0;
static int MOD2RT25EOMCnt	=	0;
static int MOD2RT26EOMCnt	=	0;
static int MOD2RT27EOMCnt	=	0;
static int MOD2RT28EOMCnt	=	0;
static int MOD2RT29EOMCnt	=	0;
static int MOD2RT30EOMCnt	=	0;


static int MOD3RT0EOMCnt	=	0;
static int MOD3RT1EOMCnt	=	0;
static int MOD3RT2EOMCnt	=	0;
static int MOD3RT3EOMCnt	=	0;
static int MOD3RT4EOMCnt	=	0;
static int MOD3RT5EOMCnt	=	0;
static int MOD3RT6EOMCnt	=	0;
static int MOD3RT7EOMCnt	=	0;
static int MOD3RT8EOMCnt	=	0;
static int MOD3RT9EOMCnt	=	0;
static int MOD3RT10EOMCnt	=	0;
static int MOD3RT11EOMCnt	=	0;
static int MOD3RT12EOMCnt	=	0;
static int MOD3RT13EOMCnt	=	0;
static int MOD3RT14EOMCnt	=	0;
static int MOD3RT15EOMCnt	=	0;
static int MOD3RT16EOMCnt	=	0;
static int MOD3RT17EOMCnt	=	0;
static int MOD3RT18EOMCnt	=	0;
static int MOD3RT19EOMCnt	=	0;
static int MOD3RT20EOMCnt	=	0;
static int MOD3RT21EOMCnt	=	0;
static int MOD3RT22EOMCnt	=	0;
static int MOD3RT23EOMCnt	=	0;
static int MOD3RT24EOMCnt	=	0;
static int MOD3RT25EOMCnt	=	0;
static int MOD3RT26EOMCnt	=	0;
static int MOD3RT27EOMCnt	=	0;
static int MOD3RT28EOMCnt	=	0;
static int MOD3RT29EOMCnt	=	0;
static int MOD3RT30EOMCnt	=	0;


static int MOD4RT0EOMCnt	=	0;
static int MOD4RT1EOMCnt	=	0;
static int MOD4RT2EOMCnt	=	0;
static int MOD4RT3EOMCnt	=	0;
static int MOD4RT4EOMCnt	=	0;
static int MOD4RT5EOMCnt	=	0;
static int MOD4RT6EOMCnt	=	0;
static int MOD4RT7EOMCnt	=	0;
static int MOD4RT8EOMCnt	=	0;
static int MOD4RT9EOMCnt	=	0;
static int MOD4RT10EOMCnt	=	0;
static int MOD4RT11EOMCnt	=	0;
static int MOD4RT12EOMCnt	=	0;
static int MOD4RT13EOMCnt	=	0;
static int MOD4RT14EOMCnt	=	0;
static int MOD4RT15EOMCnt	=	0;
static int MOD4RT16EOMCnt	=	0;
static int MOD4RT17EOMCnt	=	0;
static int MOD4RT18EOMCnt	=	0;
static int MOD4RT19EOMCnt	=	0;
static int MOD4RT20EOMCnt	=	0;
static int MOD4RT21EOMCnt	=	0;
static int MOD4RT22EOMCnt	=	0;
static int MOD4RT23EOMCnt	=	0;
static int MOD4RT24EOMCnt	=	0;
static int MOD4RT25EOMCnt	=	0;
static int MOD4RT26EOMCnt	=	0;
static int MOD4RT27EOMCnt	=	0;
static int MOD4RT28EOMCnt	=	0;
static int MOD4RT29EOMCnt	=	0;
static int MOD4RT30EOMCnt	=	0;

/*********** CIRC COUNT INITIALIZATION	*********/

static int MOD1RT0CIRCCnt	=	0;
static int MOD1RT1CIRCCnt	=	0;
static int MOD1RT2CIRCCnt	=	0;
static int MOD1RT3CIRCCnt	=	0;
static int MOD1RT4CIRCCnt	=	0;
static int MOD1RT5CIRCCnt	=	0;
static int MOD1RT6CIRCCnt	=	0;
static int MOD1RT7CIRCCnt	=	0;
static int MOD1RT8CIRCCnt	=	0;
static int MOD1RT9CIRCCnt	=	0;
static int MOD1RT10CIRCCnt	=	0;
static int MOD1RT11CIRCCnt	=	0;
static int MOD1RT12CIRCCnt	=	0;
static int MOD1RT13CIRCCnt	=	0;
static int MOD1RT14CIRCCnt	=	0;
static int MOD1RT15CIRCCnt	=	0;
static int MOD1RT16CIRCCnt	=	0;
static int MOD1RT17CIRCCnt	=	0;
static int MOD1RT18CIRCCnt	=	0;
static int MOD1RT19CIRCCnt	=	0;
static int MOD1RT20CIRCCnt	=	0;
static int MOD1RT21CIRCCnt	=	0;
static int MOD1RT22CIRCCnt	=	0;
static int MOD1RT23CIRCCnt	=	0;
static int MOD1RT24CIRCCnt	=	0;
static int MOD1RT25CIRCCnt	=	0;
static int MOD1RT26CIRCCnt	=	0;
static int MOD1RT27CIRCCnt	=	0;
static int MOD1RT28CIRCCnt	=	0;
static int MOD1RT29CIRCCnt	=	0;
static int MOD1RT30CIRCCnt	=	0;

static int MOD2RT0CIRCCnt	=	0;
static int MOD2RT1CIRCCnt	=	0;
static int MOD2RT2CIRCCnt	=	0;
static int MOD2RT3CIRCCnt	=	0;
static int MOD2RT4CIRCCnt	=	0;
static int MOD2RT5CIRCCnt	=	0;
static int MOD2RT6CIRCCnt	=	0;
static int MOD2RT7CIRCCnt	=	0;
static int MOD2RT8CIRCCnt	=	0;
static int MOD2RT9CIRCCnt	=	0;
static int MOD2RT10CIRCCnt	=	0;
static int MOD2RT11CIRCCnt	=	0;
static int MOD2RT12CIRCCnt	=	0;
static int MOD2RT13CIRCCnt	=	0;
static int MOD2RT14CIRCCnt	=	0;
static int MOD2RT15CIRCCnt	=	0;
static int MOD2RT16CIRCCnt	=	0;
static int MOD2RT17CIRCCnt	=	0;
static int MOD2RT18CIRCCnt	=	0;
static int MOD2RT19CIRCCnt	=	0;
static int MOD2RT20CIRCCnt	=	0;
static int MOD2RT21CIRCCnt	=	0;
static int MOD2RT22CIRCCnt	=	0;
static int MOD2RT23CIRCCnt	=	0;
static int MOD2RT24CIRCCnt	=	0;
static int MOD2RT25CIRCCnt	=	0;
static int MOD2RT26CIRCCnt	=	0;
static int MOD2RT27CIRCCnt	=	0;
static int MOD2RT28CIRCCnt	=	0;
static int MOD2RT29CIRCCnt	=	0;
static int MOD2RT30CIRCCnt	=	0;

static int MOD3RT0CIRCCnt	=	0;
static int MOD3RT1CIRCCnt	=	0;
static int MOD3RT2CIRCCnt	=	0;
static int MOD3RT3CIRCCnt	=	0;
static int MOD3RT4CIRCCnt	=	0;
static int MOD3RT5CIRCCnt	=	0;
static int MOD3RT6CIRCCnt	=	0;
static int MOD3RT7CIRCCnt	=	0;
static int MOD3RT8CIRCCnt	=	0;
static int MOD3RT9CIRCCnt	=	0;
static int MOD3RT10CIRCCnt	=	0;
static int MOD3RT11CIRCCnt	=	0;
static int MOD3RT12CIRCCnt	=	0;
static int MOD3RT13CIRCCnt	=	0;
static int MOD3RT14CIRCCnt	=	0;
static int MOD3RT15CIRCCnt	=	0;
static int MOD3RT16CIRCCnt	=	0;
static int MOD3RT17CIRCCnt	=	0;
static int MOD3RT18CIRCCnt	=	0;
static int MOD3RT19CIRCCnt	=	0;
static int MOD3RT20CIRCCnt	=	0;
static int MOD3RT21CIRCCnt	=	0;
static int MOD3RT22CIRCCnt	=	0;
static int MOD3RT23CIRCCnt	=	0;
static int MOD3RT24CIRCCnt	=	0;
static int MOD3RT25CIRCCnt	=	0;
static int MOD3RT26CIRCCnt	=	0;
static int MOD3RT27CIRCCnt	=	0;
static int MOD3RT28CIRCCnt	=	0;
static int MOD3RT29CIRCCnt	=	0;
static int MOD3RT30CIRCCnt	=	0;

static int MOD4RT0CIRCCnt	=	0;
static int MOD4RT1CIRCCnt	=	0;
static int MOD4RT2CIRCCnt	=	0;
static int MOD4RT3CIRCCnt	=	0;
static int MOD4RT4CIRCCnt	=	0;
static int MOD4RT5CIRCCnt	=	0;
static int MOD4RT6CIRCCnt	=	0;
static int MOD4RT7CIRCCnt	=	0;
static int MOD4RT8CIRCCnt	=	0;
static int MOD4RT9CIRCCnt	=	0;
static int MOD4RT10CIRCCnt	=	0;
static int MOD4RT11CIRCCnt	=	0;
static int MOD4RT12CIRCCnt	=	0;
static int MOD4RT13CIRCCnt	=	0;
static int MOD4RT14CIRCCnt	=	0;
static int MOD4RT15CIRCCnt	=	0;
static int MOD4RT16CIRCCnt	=	0;
static int MOD4RT17CIRCCnt	=	0;
static int MOD4RT18CIRCCnt	=	0;
static int MOD4RT19CIRCCnt	=	0;
static int MOD4RT20CIRCCnt	=	0;
static int MOD4RT21CIRCCnt	=	0;
static int MOD4RT22CIRCCnt	=	0;
static int MOD4RT23CIRCCnt	=	0;
static int MOD4RT24CIRCCnt	=	0;
static int MOD4RT25CIRCCnt	=	0;
static int MOD4RT26CIRCCnt	=	0;
static int MOD4RT27CIRCCnt	=	0;
static int MOD4RT28CIRCCnt	=	0;
static int MOD4RT29CIRCCnt	=	0;
static int MOD4RT30CIRCCnt	=	0;


/************************************  CARD-1 IOCTL's	******************************/

#define IOCTL_CARD1_DEBUG_ENABLE   			_IOWR(MAJOR_NUM_CARD1,0x1,int)
#define IOCTL_CARD1_DEBUG_DISABLE   			_IOWR(MAJOR_NUM_CARD1,0x2,int)

#define IOCTL_CARD1_SET_ICS_REG   			_IOWR(MAJOR_NUM_CARD1,0x3,int)
#define IOCTL_CARD1_SOFTWARE_RESET   			_IOWR(MAJOR_NUM_CARD1,0x4,int)
#define IOCTL_CARD1_MOD1_GET_BCEOMCNT  			_IOR(MAJOR_NUM_CARD1,0x5,int)
#define IOCTL_CARD1_MOD2_GET_BCEOMCNT  			_IOR(MAJOR_NUM_CARD1,0x6,int)
#define IOCTL_CARD1_MOD3_GET_BCEOMCNT  			_IOR(MAJOR_NUM_CARD1,0x7,int)
#define IOCTL_CARD1_MOD4_GET_BCEOMCNT  			_IOR(MAJOR_NUM_CARD1,0x8,int)

#define IOCTL_CARD1_MOD1_GET_MTEOMCNT 	 		_IOR(MAJOR_NUM_CARD1,0x9,int)
#define IOCTL_CARD1_MOD2_GET_MTEOMCNT  			_IOR(MAJOR_NUM_CARD1,0xA,int)
#define IOCTL_CARD1_MOD3_GET_MTEOMCNT  			_IOR(MAJOR_NUM_CARD1,0xB,int)
#define IOCTL_CARD1_MOD4_GET_MTEOMCNT  			_IOR(MAJOR_NUM_CARD1,0xC,int)

#define IOCTL_CARD1_MOD1_GET_RTDBCA    			_IOR(MAJOR_NUM_CARD1,0x11,int)
#define IOCTL_CARD1_MOD2_GET_RTDBCA    			_IOR(MAJOR_NUM_CARD1,0x12,int)
#define IOCTL_CARD1_MOD3_GET_RTDBCA    			_IOR(MAJOR_NUM_CARD1,0x13,int)
#define IOCTL_CARD1_MOD4_GET_RTDBCA    			_IOR(MAJOR_NUM_CARD1,0x14,int)

#define IOCTL_CARD1_GET_RTC1      			_IOR(MAJOR_NUM_CARD1,0x15,int)
#define IOCTL_CARD1_GET_RTC2      			_IOR(MAJOR_NUM_CARD1,0x16,int)
#define IOCTL_CARD1_GET_RTC3      			_IOR(MAJOR_NUM_CARD1,0x17,int)


#define IOCTL_CARD1_MOD1_SET_BCEOMCNT  			_IOR(MAJOR_NUM_CARD1,0x18,int)
#define IOCTL_CARD1_MOD2_SET_BCEOMCNT  			_IOR(MAJOR_NUM_CARD1,0x19,int)
#define IOCTL_CARD1_MOD3_SET_BCEOMCNT  			_IOR(MAJOR_NUM_CARD1,0x1A,int)
#define IOCTL_CARD1_MOD4_SET_BCEOMCNT  			_IOR(MAJOR_NUM_CARD1,0x1B,int)

#define IOCTL_CARD1_MOD1_SET_MTEOMCNT  			_IOR(MAJOR_NUM_CARD1,0x1C,int)
#define IOCTL_CARD1_MOD2_SET_MTEOMCNT  			_IOR(MAJOR_NUM_CARD1,0x1D,int)
#define IOCTL_CARD1_MOD3_SET_MTEOMCNT  			_IOR(MAJOR_NUM_CARD1,0x1E,int)
#define IOCTL_CARD1_MOD4_SET_MTEOMCNT  			_IOR(MAJOR_NUM_CARD1,0x1F,int)

#define IOCTL_CARD1_MOD1_SET_RTDBCA    			_IOR(MAJOR_NUM_CARD1,0x24,int)
#define IOCTL_CARD1_MOD2_SET_RTDBCA    			_IOR(MAJOR_NUM_CARD1,0x25,int)
#define IOCTL_CARD1_MOD3_SET_RTDBCA    			_IOR(MAJOR_NUM_CARD1,0x26,int)
#define IOCTL_CARD1_MOD4_SET_RTDBCA    			_IOR(MAJOR_NUM_CARD1,0x27,int)

#define IOCTL_CARD1_SET_RTC1      			_IOWR(MAJOR_NUM_CARD1,0x28,int)
#define IOCTL_CARD1_SET_RTC2      			_IOW(MAJOR_NUM_CARD1,0x29,int)
#define IOCTL_CARD1_SET_RTC3      			_IOW(MAJOR_NUM_CARD1,0x2A,int)

#define IOCTL_CARD1_MOD1_GET_ENDOF_FRAME_COUNT  	_IOR(MAJOR_NUM_CARD1,0x2B,int)
#define IOCTL_CARD1_MOD2_GET_ENDOF_FRAME_COUNT  	_IOR(MAJOR_NUM_CARD1,0x2C,int)
#define IOCTL_CARD1_MOD3_GET_ENDOF_FRAME_COUNT 		_IOR(MAJOR_NUM_CARD1,0x2D,int)
#define IOCTL_CARD1_MOD4_GET_ENDOF_FRAME_COUNT  	_IOR(MAJOR_NUM_CARD1,0x2E,int)

#define IOCTL_CARD1_MOD1_GET_MT_TTR_ROLL_COUNT  	_IOR(MAJOR_NUM_CARD1,0x2F,int)
#define IOCTL_CARD1_MOD2_GET_MT_TTR_ROLL_COUNT  	_IOR(MAJOR_NUM_CARD1,0x30,int)
#define IOCTL_CARD1_MOD3_GET_MT_TTR_ROLL_COUNT  	_IOR(MAJOR_NUM_CARD1,0x31,int)
#define IOCTL_CARD1_MOD4_GET_MT_TTR_ROLL_COUNT  	_IOR(MAJOR_NUM_CARD1,0x32,int)

#define IOCTL_CARD1_MOD1_SET_ENDOF_FRAME_COUNT  	_IOW(MAJOR_NUM_CARD1,0x33,int)
#define IOCTL_CARD1_MOD2_SET_ENDOF_FRAME_COUNT  	_IOW(MAJOR_NUM_CARD1,0x34,int)
#define IOCTL_CARD1_MOD3_SET_ENDOF_FRAME_COUNT  	_IOW(MAJOR_NUM_CARD1,0x35,int)
#define IOCTL_CARD1_MOD4_SET_ENDOF_FRAME_COUNT  	_IOW(MAJOR_NUM_CARD1,0x36,int)

#define IOCTL_CARD1_MOD1_SET_MT_TTR_ROLL_COUNT  	_IOW(MAJOR_NUM_CARD1,0x37,int)
#define IOCTL_CARD1_MOD2_SET_MT_TTR_ROLL_COUNT  	_IOW(MAJOR_NUM_CARD1,0x38,int)
#define IOCTL_CARD1_MOD3_SET_MT_TTR_ROLL_COUNT  	_IOW(MAJOR_NUM_CARD1,0x39,int)
#define IOCTL_CARD1_MOD4_SET_MT_TTR_ROLL_COUNT  	_IOW(MAJOR_NUM_CARD1,0x3A,int)


#define IOCTL_CARD1_MEM_READ_MODULE1  			_IOR(MAJOR_NUM_CARD1,0x3B,int)
#define IOCTL_CARD1_MEM_WRITE_MODULE1  			_IOW(MAJOR_NUM_CARD1,0x3C,int)

#define IOCTL_CARD1_MEM_READ_MODULE2  			_IOR(MAJOR_NUM_CARD1,0x3D,int)
#define IOCTL_CARD1_MEM_WRITE_MODULE2  			_IOW(MAJOR_NUM_CARD1,0x3E,int)

#define IOCTL_CARD1_MEM_READ_MODULE3  			_IOR(MAJOR_NUM_CARD1,0x3F,int)
#define IOCTL_CARD1_MEM_WRITE_MODULE3  			_IOW(MAJOR_NUM_CARD1,0x40,int)

#define IOCTL_CARD1_MEM_READ_MODULE4  			_IOR(MAJOR_NUM_CARD1,0x41,int)
#define IOCTL_CARD1_MEM_WRITE_MODULE4  			_IOW(MAJOR_NUM_CARD1,0x42,int)



#define IOCTL_CARD1_REG_READ_MODULE1  			_IOR(MAJOR_NUM_CARD1,0x43,int)
#define IOCTL_CARD1_REG_WRITE_MODULE1  			_IOW(MAJOR_NUM_CARD1,0x44,int)

#define IOCTL_CARD1_REG_READ_MODULE2  			_IOR(MAJOR_NUM_CARD1,0x45,int)
#define IOCTL_CARD1_REG_WRITE_MODULE2  			_IOW(MAJOR_NUM_CARD1,0x46,int)

#define IOCTL_CARD1_REG_READ_MODULE3  			_IOR(MAJOR_NUM_CARD1,0x47,int)
#define IOCTL_CARD1_REG_WRITE_MODULE3  			_IOW(MAJOR_NUM_CARD1,0x48,int)

#define IOCTL_CARD1_REG_READ_MODULE4  			_IOR(MAJOR_NUM_CARD1,0x49,int)
#define IOCTL_CARD1_REG_WRITE_MODULE4  			_IOW(MAJOR_NUM_CARD1,0x4A,int)


#define IOCTL_CARD1_READ_PORT	  			_IOR(MAJOR_NUM_CARD1,0x4B,int)
#define IOCTL_CARD1_WRITE_PORT	 			_IOW(MAJOR_NUM_CARD1,0x4C,int)
#define IOCTL_SET_M1CNT  			        _IOR(MAJOR_NUM_CARD1,0x55,int)
#define IOCTL_GET_M1CNT  			        _IOR(MAJOR_NUM_CARD1,0x56,int)

#define IOCTL_CARD1_ENABLE_INT				_IOWR(MAJOR_NUM_CARD1,0x4D,int)

#define IOCTL_UART_GET_INT				_IOWR(MAJOR_NUM_CARD1,0x51,int)
#define IOCTL_UART_SET_INT				_IOWR(MAJOR_NUM_CARD1,0x52,int)

#define IOCTL_UART_READ					_IOWR(MAJOR_NUM_CARD1,0x53,int)
#define IOCTL_UART_WRITE				_IOWR(MAJOR_NUM_CARD1,0x54,int)




