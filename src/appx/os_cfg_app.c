/*
*********************************************************************************************************
*                                              uC/OS-III
*                                        The Real-Time Kernel
*
*                    Copyright 2009-2020 Silicon Laboratories Inc. www.silabs.com
*
*                                 SPDX-License-Identifier: APACHE-2.0
*
*               This software is subject to an open source license and is distributed by
*                Silicon Laboratories Inc. pursuant to the terms of the Apache License,
*                    Version 2.0 available at www.apache.org/licenses/LICENSE-2.0.
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                               OS CONFIGURATION (APPLICATION SPECIFICS)
*
* File    : os_cfg_app.c
* Version : V3.08.00
*********************************************************************************************************
* Note(s) : DO NOT CHANGE THIS FILE!
*********************************************************************************************************
*/

#define  MICRIUM_SOURCE
#include <os_cfg_app.h>
#include <os.h>

#if (OS_CFG_TASK_IDLE_EN > 0u)
#define  OS_CFG_IDLE_TASK_STK_LIMIT      ((OS_CFG_IDLE_TASK_STK_SIZE  * OS_CFG_TASK_STK_LIMIT_PCT_EMPTY) / 100u)
#endif

#if (OS_CFG_STAT_TASK_EN > 0u)
#define  OS_CFG_STAT_TASK_STK_LIMIT      ((OS_CFG_STAT_TASK_STK_SIZE  * OS_CFG_TASK_STK_LIMIT_PCT_EMPTY) / 100u)
#endif

#if (OS_CFG_TMR_EN > 0u)
#define  OS_CFG_TMR_TASK_STK_LIMIT       ((OS_CFG_TMR_TASK_STK_SIZE   * OS_CFG_TASK_STK_LIMIT_PCT_EMPTY) / 100u)
#endif

/*
************************************************************************************************************************
*                                                    DATA STORAGE
************************************************************************************************************************
*/

#if (OS_CFG_TASK_IDLE_EN > 0u)
CPU_STK        OSCfg_IdleTaskStk   [OS_CFG_IDLE_TASK_STK_SIZE];
#endif

#if (OS_CFG_ISR_STK_SIZE > 0u)
CPU_STK        OSCfg_ISRStk       [OS_CFG_ISR_STK_SIZE];
#endif

#if (OS_MSG_EN > 0u)
OS_MSG         OSCfg_MsgPool       [OS_CFG_MSG_POOL_SIZE];
#endif

#if (OS_CFG_STAT_TASK_EN > 0u)
CPU_STK        OSCfg_StatTaskStk   [OS_CFG_STAT_TASK_STK_SIZE];
#endif

#if (OS_CFG_TMR_EN > 0u)
CPU_STK        OSCfg_TmrTaskStk    [OS_CFG_TMR_TASK_STK_SIZE];
#endif

/*
************************************************************************************************************************
*                                                      CONSTANTS
************************************************************************************************************************
*/

#if (OS_CFG_TASK_IDLE_EN > 0u)
CPU_STK      * const  OSCfg_IdleTaskStkBasePtr   = &OSCfg_IdleTaskStk[0];
CPU_STK_SIZE   const  OSCfg_IdleTaskStkLimit     =  OS_CFG_IDLE_TASK_STK_LIMIT;
CPU_STK_SIZE   const  OSCfg_IdleTaskStkSize      =  OS_CFG_IDLE_TASK_STK_SIZE;
CPU_INT32U     const  OSCfg_IdleTaskStkSizeRAM   =  sizeof(OSCfg_IdleTaskStk);
#else
CPU_STK      * const  OSCfg_IdleTaskStkBasePtr   = (CPU_STK *)0;
CPU_STK_SIZE   const  OSCfg_IdleTaskStkLimit     =            0u;
CPU_STK_SIZE   const  OSCfg_IdleTaskStkSize      =            0u;
CPU_INT32U     const  OSCfg_IdleTaskStkSizeRAM   =            0u;
#endif

#if (OS_CFG_ISR_STK_SIZE > 0u)
CPU_STK      * const  OSCfg_ISRStkBasePtr        = &OSCfg_ISRStk[0];
CPU_STK_SIZE   const  OSCfg_ISRStkSize           =  OS_CFG_ISR_STK_SIZE;
CPU_INT32U     const  OSCfg_ISRStkSizeRAM        =  sizeof(OSCfg_ISRStk);

#else
CPU_STK      * const  OSCfg_ISRStkBasePtr        =  (CPU_STK *)0;
CPU_STK_SIZE   const  OSCfg_ISRStkSize           =             0u;
CPU_INT32U     const  OSCfg_ISRStkSizeRAM        =             0u;
#endif


#if (OS_MSG_EN > 0u)
OS_MSG_SIZE    const  OSCfg_MsgPoolSize          =  OS_CFG_MSG_POOL_SIZE;
CPU_INT32U     const  OSCfg_MsgPoolSizeRAM       =  sizeof(OSCfg_MsgPool);
OS_MSG       * const  OSCfg_MsgPoolBasePtr       = &OSCfg_MsgPool[0];
#else
OS_MSG_SIZE    const  OSCfg_MsgPoolSize          =           0u;
CPU_INT32U     const  OSCfg_MsgPoolSizeRAM       =           0u;
OS_MSG       * const  OSCfg_MsgPoolBasePtr       = (OS_MSG *)0;
#endif


#if (OS_CFG_STAT_TASK_EN > 0u)
OS_PRIO        const  OSCfg_StatTaskPrio         =  OS_CFG_STAT_TASK_PRIO;
OS_RATE_HZ     const  OSCfg_StatTaskRate_Hz      =  OS_CFG_STAT_TASK_RATE_HZ;
CPU_STK      * const  OSCfg_StatTaskStkBasePtr   = &OSCfg_StatTaskStk[0];
CPU_STK_SIZE   const  OSCfg_StatTaskStkLimit     =  OS_CFG_STAT_TASK_STK_LIMIT;
CPU_STK_SIZE   const  OSCfg_StatTaskStkSize      =  OS_CFG_STAT_TASK_STK_SIZE;
CPU_INT32U     const  OSCfg_StatTaskStkSizeRAM   =  sizeof(OSCfg_StatTaskStk);
#else
OS_PRIO        const  OSCfg_StatTaskPrio         =            0u;
OS_RATE_HZ     const  OSCfg_StatTaskRate_Hz      =            0u;
CPU_STK      * const  OSCfg_StatTaskStkBasePtr   = (CPU_STK *)0;
CPU_STK_SIZE   const  OSCfg_StatTaskStkLimit     =            0u;
CPU_STK_SIZE   const  OSCfg_StatTaskStkSize      =            0u;
CPU_INT32U     const  OSCfg_StatTaskStkSizeRAM   =            0u;
#endif


CPU_STK_SIZE   const  OSCfg_StkSizeMin           =  OS_CFG_STK_SIZE_MIN;


#if (OS_CFG_TICK_EN > 0u)
OS_RATE_HZ     const  OSCfg_TickRate_Hz          =  OS_CFG_TICK_RATE_HZ;
#else
OS_RATE_HZ     const  OSCfg_TickRate_Hz          =  0u;
#endif


#if (OS_CFG_TMR_EN > 0u)
OS_PRIO        const  OSCfg_TmrTaskPrio          =  OS_CFG_TMR_TASK_PRIO;
OS_RATE_HZ     const  OSCfg_TmrTaskRate_Hz       =  OS_CFG_TMR_TASK_RATE_HZ;
CPU_STK      * const  OSCfg_TmrTaskStkBasePtr    = &OSCfg_TmrTaskStk[0];
CPU_STK_SIZE   const  OSCfg_TmrTaskStkLimit      =  OS_CFG_TMR_TASK_STK_LIMIT;
CPU_STK_SIZE   const  OSCfg_TmrTaskStkSize       =  OS_CFG_TMR_TASK_STK_SIZE;
CPU_INT32U     const  OSCfg_TmrTaskStkSizeRAM    =  sizeof(OSCfg_TmrTaskStk);
#else
OS_PRIO        const  OSCfg_TmrTaskPrio          =             0u;
OS_RATE_HZ     const  OSCfg_TmrTaskRate_Hz       =             0u;
CPU_STK      * const  OSCfg_TmrTaskStkBasePtr    =  (CPU_STK *)0;
CPU_STK_SIZE   const  OSCfg_TmrTaskStkLimit      =             0u;
CPU_STK_SIZE   const  OSCfg_TmrTaskStkSize       =             0u;
CPU_INT32U     const  OSCfg_TmrTaskStkSizeRAM    =             0u;
#endif


/*
************************************************************************************************************************
*                                         TOTAL SIZE OF APPLICATION CONFIGURATION
************************************************************************************************************************
*/

CPU_INT32U     const  OSCfg_DataSizeRAM          = 0u

#if (OS_CFG_TASK_IDLE_EN > 0u)
                                                 + sizeof(OSCfg_IdleTaskStk)
#endif

#if (OS_MSG_EN > 0u)
                                                 + sizeof(OSCfg_MsgPool)
#endif

#if (OS_CFG_STAT_TASK_EN > 0u)
                                                 + sizeof(OSCfg_StatTaskStk)
#endif

#if (OS_CFG_TMR_EN > 0u)
                                                 + sizeof(OSCfg_TmrTaskStk)
#endif

#if (OS_CFG_ISR_STK_SIZE > 0u)
                                                 + sizeof(OSCfg_ISRStk)
#endif

                                                 + 0u;



/*
************************************************************************************************************************
*                                             OS CONFIGURATION INITIALIZATION
*
* Description: This function is used to make sure that debug variables that are unused in the application are not
*              optimized away.  This function might not be necessary for all compilers.  In this case, you should simply
*              DELETE the code in this function while still leaving the declaration of the function itself.
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : (1) This code doesn't do anything, it simply prevents the compiler from optimizing out the 'const'
*                  variables which are declared in this file.
*              (2) You may decide to 'compile out' the code (by using #if 0/#endif) INSIDE the function if your compiler
*                  DOES NOT optimize out the 'const' variables above.
************************************************************************************************************************
*/

void  OSCfg_Init (void)
{
    (void)OSCfg_DataSizeRAM;

#if (OS_CFG_TASK_IDLE_EN > 0u)
    (void)OSCfg_IdleTaskStkBasePtr;
    (void)OSCfg_IdleTaskStkLimit;
    (void)OSCfg_IdleTaskStkSize;
    (void)OSCfg_IdleTaskStkSizeRAM;
#endif

    (void)OSCfg_ISRStkBasePtr;
    (void)OSCfg_ISRStkSize;
    (void)OSCfg_ISRStkSizeRAM;

#if (OS_MSG_EN > 0u)
    (void)OSCfg_MsgPoolSize;
    (void)OSCfg_MsgPoolSizeRAM;
    (void)OSCfg_MsgPoolBasePtr;
#endif

#if (OS_CFG_STAT_TASK_EN > 0u)
    (void)OSCfg_StatTaskPrio;
    (void)OSCfg_StatTaskRate_Hz;
    (void)OSCfg_StatTaskStkBasePtr;
    (void)OSCfg_StatTaskStkLimit;
    (void)OSCfg_StatTaskStkSize;
    (void)OSCfg_StatTaskStkSizeRAM;
#endif

    (void)OSCfg_StkSizeMin;

#if (OS_CFG_TICK_EN > 0u)
    (void)OSCfg_TickRate_Hz;
#endif

#if (OS_CFG_TMR_EN > 0u)
    (void)OSCfg_TmrTaskPrio;
    (void)OSCfg_TmrTaskRate_Hz;
    (void)OSCfg_TmrTaskStkBasePtr;
    (void)OSCfg_TmrTaskStkLimit;
    (void)OSCfg_TmrTaskStkSize;
    (void)OSCfg_TmrTaskStkSizeRAM;
#endif
}

static  const uint8_t  CPU_CntLeadZerosTbl[256] = {                             /* Data vals :                      */
/*   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F   */
    8u,  7u,  6u,  6u,  5u,  5u,  5u,  5u,  4u,  4u,  4u,  4u,  4u,  4u,  4u,  4u,  /*   0x00 to 0x0F                   */
    3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  /*   0x10 to 0x1F                   */
    2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  /*   0x20 to 0x2F                   */
    2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  /*   0x30 to 0x3F                   */
    1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x40 to 0x4F                   */
    1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x50 to 0x5F                   */
    1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x60 to 0x6F                   */
    1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x70 to 0x7F                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0x80 to 0x8F                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0x90 to 0x9F                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xA0 to 0xAF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xB0 to 0xBF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xC0 to 0xCF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xD0 to 0xDF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xE0 to 0xEF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u   /*   0xF0 to 0xFF                   */
};


unsigned int  CPU_CntLeadZeros (unsigned int  val)
{
    unsigned int  ix;
    unsigned int  nbr_lead_zeros;

    if (val > 0x0000FFFFu) {
        if (val > 0x00FFFFFFu) {

            ix             = (unsigned int)(val >> 24u);
            nbr_lead_zeros = (unsigned int)(CPU_CntLeadZerosTbl[ix] +  0u);

        } else {

            ix             = (unsigned int)(val >> 16u);
            nbr_lead_zeros = (unsigned int)(CPU_CntLeadZerosTbl[ix] +  8u);
        }

    } else {
        if (val > 0x000000FFu) {

            ix             = (unsigned int)(val >>  8u);
            nbr_lead_zeros = (unsigned int)(CPU_CntLeadZerosTbl[ix] + 16u);

        } else {

            ix             = (unsigned int)(val >>  0u);
            nbr_lead_zeros = (unsigned int)(CPU_CntLeadZerosTbl[ix] + 24u);
        }
    }

    return (nbr_lead_zeros);
}
