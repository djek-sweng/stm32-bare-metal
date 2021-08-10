/*
 * stm32f4xx_driver.h
 *
 *  Created on: Jul 20, 2021
 *      Author: djek-sweng
 */

#ifndef STM32F4XX_DRIVER_H_
#define STM32F4XX_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/* includes */
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "basedefs.h"

/*--------------------------------------------------------------------------------------------------------------------*/
/* defines */
/*--------------------------------------------------------------------------------------------------------------------*/
#if defined(STM32F446xx) /*-------------------------------------------------------------------------------------------*/

/* FLASH memory -------------------------------------------------------------*/
#define FLASH_NBR_SECTORS                     (8)
#define FLASH_SIZE_S0                         (SIZE_16K)
#define FLASH_SIZE_S1                         (SIZE_16K)
#define FLASH_SIZE_S2                         (SIZE_16K)
#define FLASH_SIZE_S3                         (SIZE_16K)
#define FLASH_SIZE_S4                         (SIZE_64K)
#define FLASH_SIZE_S5                         (SIZE_128K)
#define FLASH_SIZE_S6                         (SIZE_128K)
#define FLASH_SIZE_S7                         (SIZE_128K)
#define FLASH_SIZE                            (FLASH_SIZE_S0 + FLASH_SIZE_S1 + \
                                               FLASH_SIZE_S2 + FLASH_SIZE_S3 + \
                                               FLASH_SIZE_S4 + FLASH_SIZE_S5 + \
                                               FLASH_SIZE_S6 + FLASH_SIZE_S7)

#if (SIZE_512K != FLASH_SIZE)
  #error "FLASH != 512 Kbytes"
#endif

#define FLASH_ADDRESS_START                   (FLASH_BASE)
#define FLASH_ADDRESS_S0                      (FLASH_ADDRESS_START)
#define FLASH_ADDRESS_S1                      (FLASH_ADDRESS_S0 + FLASH_SIZE_S0)
#define FLASH_ADDRESS_S2                      (FLASH_ADDRESS_S1 + FLASH_SIZE_S1)
#define FLASH_ADDRESS_S3                      (FLASH_ADDRESS_S2 + FLASH_SIZE_S2)
#define FLASH_ADDRESS_S4                      (FLASH_ADDRESS_S3 + FLASH_SIZE_S3)
#define FLASH_ADDRESS_S5                      (FLASH_ADDRESS_S4 + FLASH_SIZE_S4)
#define FLASH_ADDRESS_S6                      (FLASH_ADDRESS_S5 + FLASH_SIZE_S5)
#define FLASH_ADDRESS_S7                      (FLASH_ADDRESS_S6 + FLASH_SIZE_S6)
#define FLASH_ADDRESS_END                     (FLASH_ADDRESS_S7 + FLASH_SIZE_S7)

#if (SIZE_512K != (FLASH_ADDRESS_END-FLASH_ADDRESS_START))
  #error "FLASH != 512 Kbytes"
#endif

/* SRAM1 memory -------------------------------------------------------------*/
#define SRAM1_SIZE                            (SIZE_112K)

#define SRAM1_ADDRESS_START                   (SRAM1_BASE)
#define SRAM1_ADDRESS_END                     (SRAM1_ADDRESS_START + SRAM1_SIZE)

#if (SIZE_112K != (SRAM1_ADDRESS_END-SRAM1_ADDRESS_START))
  #error "SRAM1 != 112 Kbytes"
#endif

/* SRAM2 memory -------------------------------------------------------------*/
#define SRAM2_SIZE                            (SIZE_16K)

#define SRAM2_ADDRESS_START                   (SRAM2_BASE)
#define SRAM2_ADDRESS_END                     (SRAM2_ADDRESS_START + SRAM2_SIZE)

#if (SIZE_16K != (SRAM2_ADDRESS_END-SRAM2_ADDRESS_START))
  #error "SRAM2 != 16 Kbytes"
#endif

/* SRAM memory --------------------------------------------------------------*/
#define SRAM_SIZE                             (SRAM1_SIZE + SRAM2_SIZE)

#define SRAM_ADDRESS_START                    (SRAM1_ADDRESS_START)
#define SRAM_ADDRESS_END                      (SRAM2_ADDRESS_END)

#if (SIZE_128K != (SRAM_ADDRESS_END-SRAM_ADDRESS_START))
  #error "SRAM != 128 Kbytes"
#endif

#endif

#define IS_VALID_FLASH_ADDRESS(addr)          ((addr >= FLASH_ADDRESS_START) && \
                                               (addr <= FLASH_ADDRESS_END))

#define IS_VALID_SRAM_ADDRESS(addr)           ((addr >= SRAM_ADDRESS_START) && \
                                               (addr <= SRAM_ADDRESS_END))

/*--------------------------------------------------------------------------------------------------------------------*/
/* prototypes */
/*--------------------------------------------------------------------------------------------------------------------*/
#if defined(HAL_MODULE_ENABLED)
int32_t   GetTickMs                           (uint32_t* ms);
int32_t   DelayMs                             (uint32_t ms);
#endif

#if defined(HAL_FLASH_MODULE_ENABLED)
int32_t   FLASH_Unlock                        (void);
int32_t   FLASH_Lock                          (void);
int32_t   FLASH_IsLocked                      (void);
int32_t   FLASH_FindSector                    (uint32_t address, uint32_t* sector);
int32_t   FLASH_EraseSector                   (uint32_t sector, uint32_t totalNumber);
int32_t   FLASH_ProgramByte                   (uint32_t address, uint8_t data);
int32_t   FLASH_ProgramByteBuffer             (uint32_t address, uint8_t* data, uint32_t length);
#endif

#if defined(HAL_IWDG_MODULE_ENABLED)
int32_t   IWDG_Reload                         (IWDG_HandleTypeDef* iwdg);
#endif

#if defined(HAL_UART_MODULE_ENABLED)
int32_t   UART_Transmit                       (UART_HandleTypeDef* uart, uint8_t* data, uint16_t size, uint32_t timeout);
int32_t   UART_Receive                        (UART_HandleTypeDef* uart, uint8_t* data, uint16_t size, uint32_t timeout);
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* STM32F4XX_DRIVER_H_ */
