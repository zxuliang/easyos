#ifndef  OS_CPU_H
#define  OS_CPU_H

#define OS_CPU_ARM_CONTROL_INT_DIS		0xC0
#define OS_CPU_ARM_CONTROL_FIQ_DIS		0x40
#define OS_CPU_ARM_CONTROL_IRQ_DIS		0x80
#define OS_CPU_ARM_CONTROL_THUMB		0x20
#define OS_CPU_ARM_CONTROL_ARM			0x00

#define OS_CPU_ARM_MODE_MASK			0x1F
#define OS_CPU_ARM_MODE_USR			0x10
#define OS_CPU_ARM_MODE_FIQ			0x11
#define OS_CPU_ARM_MODE_IRQ			0x12
#define OS_CPU_ARM_MODE_SVC			0x13
#define OS_CPU_ARM_MODE_ABT			0x17
#define OS_CPU_ARM_MODE_UND			0x1B
#define OS_CPU_ARM_MODE_SYS			0x1F

#define OS_CPU_ARM_EXCEPT_RESET			0x00
#define OS_CPU_ARM_EXCEPT_UNDEF_INSTR		0x01
#define OS_CPU_ARM_EXCEPT_SWI			0x02
#define OS_CPU_ARM_EXCEPT_PREFETCH_ABORT	0x03
#define OS_CPU_ARM_EXCEPT_DATA_ABORT		0x04
#define OS_CPU_ARM_EXCEPT_IRQ			0x06
#define OS_CPU_ARM_EXCEPT_FIQ			0x07

/* ARM exception IDs */
#define  OS_CPU_ARM_EXCEPT_RESET		0x00
#define  OS_CPU_ARM_EXCEPT_UNDEF_INSTR		0x01
#define  OS_CPU_ARM_EXCEPT_SWI			0x02
#define  OS_CPU_ARM_EXCEPT_PREFETCH_ABORT	0x03
#define  OS_CPU_ARM_EXCEPT_DATA_ABORT		0x04
#define  OS_CPU_ARM_EXCEPT_ADDR_ABORT		0x05
#define  OS_CPU_ARM_EXCEPT_IRQ			0x06
#define  OS_CPU_ARM_EXCEPT_FIQ			0x07
#define  OS_CPU_ARM_EXCEPT_NBR			0x08

#define NO_IRQ_MASK             (0x80)
#define NO_FIQ_MASK             (0x40)
#define NO_INTRPT_MASK          (0xC0)

#define USR_MODE                (0x10)
#define FIQ_MODE                (0x11)
#define IRQ_MODE                (0x12)
#define SVC_MODE                (0x13)
#define ABT_MODE                (0x1B)
#define SYS_MODE                (0x1F)

#define CPU_CFG_DATA_SIZE	4



#ifndef __ASM__

typedef unsigned char		CPU_INT08U;
typedef unsigned short		CPU_INT16U;
typedef unsigned int		CPU_INT32U;
typedef unsigned long		ulong;
typedef unsigned long long	CPU_INT64U;

typedef CPU_INT32U		CPU_STK;
typedef CPU_INT32U		CPU_TS;
typedef CPU_INT32U		CPU_STK_SIZE;
typedef CPU_INT32U		CPU_DATA;
typedef	CPU_INT32U		CPU_BOOLEAN;
typedef char			CPU_CHAR;
typedef unsigned long		CPU_ADDR;

#define CPU_TYPE_CREATE(a, b, c, d)	(((a) << 24) | ((b) << 16) | ((c) << 8) | (d))



typedef   CPU_INT16U      OS_CPU_USAGE;                /* CPU Usage 0..10000                                  <16>/32 */

typedef   CPU_INT32U      OS_CTR;                      /* Counter,                                                 32 */

typedef   CPU_INT32U      OS_CTX_SW_CTR;               /* Counter of context switches,                             32 */

typedef   CPU_INT32U      OS_CYCLES;                   /* CPU clock cycles,                                   <32>/64 */

typedef   CPU_INT32U      OS_FLAGS;                    /* Event flags,                                      8/16/<32> */

typedef   CPU_INT32U      OS_IDLE_CTR;                 /* Holds the number of times the idle task runs,       <32>/64 */

typedef   CPU_INT16U      OS_MEM_QTY;                  /* Number of memory blocks,                            <16>/32 */
typedef   CPU_INT16U      OS_MEM_SIZE;                 /* Size in bytes of a memory block,                    <16>/32 */

typedef   CPU_INT16U      OS_MSG_QTY;                  /* Number of OS_MSGs in the msg pool,                  <16>/32 */
typedef   CPU_INT16U      OS_MSG_SIZE;                 /* Size of messages in number of bytes,                <16>/32 */

typedef   CPU_INT08U      OS_NESTING_CTR;              /* Interrupt and scheduler nesting,                  <8>/16/32 */

typedef   CPU_INT16U      OS_OBJ_QTY;                  /* Number of kernel objects counter,                   <16>/32 */
typedef   CPU_INT32U      OS_OBJ_TYPE;                 /* Special flag to determine object type,                   32 */

typedef   CPU_INT16U      OS_OPT;                      /* Holds function options,                             <16>/32 */

typedef   CPU_INT08U      OS_PRIO;                     /* Priority of a task,                               <8>/16/32 */

typedef   CPU_INT16U      OS_QTY;                      /* Quantity                                            <16>/32 */

typedef   CPU_INT32U      OS_RATE_HZ;                  /* Rate in Hertz                                            32 */

#if (CPU_CFG_ADDR_SIZE == CPU_WORD_SIZE_64)            /* Task register                                  8/16/<32/64> */
typedef   CPU_INT64U      OS_REG;
#else
typedef   CPU_INT32U      OS_REG;
#endif
typedef   CPU_INT08U      OS_REG_ID;                   /* Index to task register                            <8>/16/32 */

typedef   CPU_INT32U      OS_SEM_CTR;                  /* Semaphore value                                     16/<32> */

typedef   CPU_INT08U      OS_STATE;                    /* State variable                                    <8>/16/32 */

typedef   CPU_INT08U      OS_STATUS;                   /* Status                                            <8>/16/32 */

typedef   CPU_INT32U      OS_TICK;                     /* Clock tick counter                                  <32>/64 */


/*
*********************************************************************************************************
*                                       TIMESTAMP CONFIGURATION
*
* Note(s) : (1) OS_TS_GET() is generally defined as CPU_TS_Get32() to allow CPU timestamp timer to be of
*               any data type size.
*
*           (2) For architectures that provide 32-bit or higher precision free running counters
*               (i.e. cycle count registers):
*
*               (a) OS_TS_GET() may be defined as CPU_TS_TmrRd() to improve performance when retrieving
*                   the timestamp.
*
*               (b) CPU_TS_TmrRd() MUST be configured to be greater or al to 32-bits to avoid
*                   truncation of TS.
*********************************************************************************************************
*/

#if      OS_CFG_TS_EN == 1u
#define  OS_TS_GET()               (CPU_TS)CPU_TS_TmrRd()   /* See Note #2a.                                          */
#else
#define  OS_TS_GET()               (CPU_TS)0u
#endif

/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern CPU_STK  *OS_CPU_ExceptStkBase;
extern CPU_STK  OS_CPU_ExceptStkPtr;

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void  OSCtxSw                            (void);
void  OSIntCtxSw                         (void);
void  OSStartHighRdy                     (void);
void  OS_CPU_InitExceptVect              (void);
void  OS_CPU_ARM_ExceptUndefInstrHndlr   (void);
void  OS_CPU_ARM_ExceptSwiHndlr          (void);
void  OS_CPU_ARM_ExceptPrefetchAbortHndlr(void);
void  OS_CPU_ARM_ExceptDataAbortHndlr    (void);
void  OS_CPU_ARM_ExceptAddrAbortHndlr    (void);
void  OS_CPU_ARM_ExceptIrqHndlr          (void);
void  OS_CPU_ARM_ExceptFiqHndlr          (void);

#define  OS_TASK_SW()               	OSCtxSw()


#endif

#endif
