/*
 * bldr_config.h
 *
 *  Created on: 9 Sep 2021
 *      Author: djek
 */

#ifndef BLDR_CONFIG_H_
#define BLDR_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/* includes */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "bldr_global.h"
#include "led.h"

/*--------------------------------------------------------------------------------------------------------------------*/
/* defines */
/*--------------------------------------------------------------------------------------------------------------------*/
#define BLDR_UART       (&huart2)
#define BLDR_IWDG       (&hiwdg)

/*--------------------------------------------------------------------------------------------------------------------*/
/* callback functions */
/*--------------------------------------------------------------------------------------------------------------------*/
STATIC INLINE void cb_BLDR_ReloadIwdg(void)
{
  IWDG_Reload(BLDR_IWDG);
}

STATIC INLINE void cb_BLDR_ToggleAliveLed(void)
{
  LED_GREEN_TOGGLE();
}

STATIC INLINE void cb_BLDR_SlowDownIwdg(void)
{
  __NOP();
}

STATIC INLINE void cb_BLDR_JumpDeInit(void)
{
  __NOP();
}

STATIC INLINE BooleanValue_t cb_BLDR_IsBootloaderRequest(void)
{
  LogicalValue_t level;
  uint32_t reg = LOW;

  /* Debouncing level of input pin. */
  for (int i=0; i<8; i++)
  {
    GPIO_ReadPin(PB_BLUE_GPIO_Port, PB_BLUE_Pin, &level);

    reg |= (level << i);

    DelayMs(10);
  }

  /* Is the blue user push button hold down? */
  if (LOW == reg)
  {
    return TRUE;
  }

  return FALSE;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* BLDR_CONFIG_H_ */
