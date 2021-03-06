/*
 * tim.c
 *
 *  Created on: Jan 15, 2022
 *      Author: djek-sweng
 */

/*--------------------------------------------------------------------------------------------------------------------*/
/* includes */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "global.h"

/*--------------------------------------------------------------------------------------------------------------------*/
/* functions */
/*--------------------------------------------------------------------------------------------------------------------*/
int32_t TIM_Run(void)
{
  HAL_TIM_Base_Start_IT(&htim7);

  return NO_FAILURE;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/* callback functions */
/*--------------------------------------------------------------------------------------------------------------------*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  NO_WARNING(htim);

  /* Do something here. */
}
