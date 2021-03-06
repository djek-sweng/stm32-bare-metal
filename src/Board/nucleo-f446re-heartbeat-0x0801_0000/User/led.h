/*
 * led.h
 *
 *  Created on: Jan 15, 2022
 *      Author: djek-sweng
 */

#ifndef LED_H_
#define LED_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#include "main.h"

#define LED_GREEN_TOGGLE()      (HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin))

#define LED_HEART_BEAT(t)       do {                      \
                                  for (int i=0; i<2; i++) \
                                  {                       \
                                    LED_GREEN_TOGGLE();   \
                                    DelayMs(t);           \
                                    LED_GREEN_TOGGLE();   \
                                    DelayMs(t);           \
                                  }                       \
                                  DelayMs(t*16);          \
                                } while(0U)

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#endif /* LED_H_ */
