/*
 * bldr_config.template.h
 *
 *  Created on: Jul 28, 2021
 *      Author: djek-sweng
 */

#ifndef BLDR_CONFIG_TEMPLATE_H_
#define BLDR_CONFIG_TEMPLATE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/* includes */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "bldr_global.h"

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
  __NOP();
}

STATIC INLINE void cb_BLDR_SlowDownIwdg(void)
{
  __NOP();
}

STATIC INLINE void cb_BLDR_JumpDeInit(void)
{
  __NOP();
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* BLDR_CONFIG_TEMPLATE_H_ */
