/*
*************************************************************************************************************
*                                APPLICATION SPECIFIC  CONFIGURATION FILE
*
*                                (c) Copyright 2007, Micrium, Weston, FL
*                                          All Rights Reserved
*
* File : APP_CFG.H
* By   : Fabiano Kovalski
*************************************************************************************************************
*/

/*
**************************************************************************************************************
*                                               STACK SIZES
**************************************************************************************************************
*/

#define  OS_TASK_START_STK_SIZE          400
#define  OS_TASK_1_STK_SIZE              200
#define  OS_TASK_2_STK_SIZE              140
#define  OS_TASK_3_STK_SIZE              140

#define  OS_TASK_STK_SIZE_HARD            64

/*
**************************************************************************************************************
*                                             TASK PRIORITIES
**************************************************************************************************************
*/

#define  OS_TASK_START_PRIO                0

#define  OS_TASK_1_PRIO                    1
#define  OS_TASK_2_PRIO                    2
#define  OS_TASK_3_PRIO                    3

#define  OS_TASK_TMR_PRIO                  4

/*       OS_TASK_STAT_PRIO       OS_LOWEST_PRIO - 1                                                         */
/*       OS_TASK_IDLE_PRIO       OS_LOWEST_PRIO                                                             */
