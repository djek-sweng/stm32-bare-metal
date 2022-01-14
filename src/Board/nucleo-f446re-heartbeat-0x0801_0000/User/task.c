/*
 * task.c
 *
 *  Created on: Jan 15, 2022
 *      Author: djek-sweng
 */

/*--------------------------------------------------------------------------------------------------------------------*/
/* includes */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "global.h"

/*--------------------------------------------------------------------------------------------------------------------*/
/* defines */
/*--------------------------------------------------------------------------------------------------------------------*/
#define TASK_DELAY  (1000)  /* in ms */

/*--------------------------------------------------------------------------------------------------------------------*/
/* functions */
/*--------------------------------------------------------------------------------------------------------------------*/
A_NORETURN void TASK_Run(void* param)
{
  NO_WARNING(param);

  for (int i=0;;i++)
  {
    printf("Hello STM32 .. %d\n\r", i);

    DelayMs(TASK_DELAY);
  }
}
