/*
 * stm32f4xx_driver.c
 *
 *  Created on: Jul 20, 2021
 *      Author: djek-sweng
 */

/*--------------------------------------------------------------------------------------------------------------------*/
/* includes */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stm32f4xx_driver.h"

/*--------------------------------------------------------------------------------------------------------------------*/
/* globals */
/*--------------------------------------------------------------------------------------------------------------------*/
#if defined(STM32F446xx)
  CONST uint32_t FLASH_SectorMapping[] = { FLASH_ADDRESS_S0,
                                           FLASH_ADDRESS_S1,
                                           FLASH_ADDRESS_S2,
                                           FLASH_ADDRESS_S3,
                                           FLASH_ADDRESS_S4,
                                           FLASH_ADDRESS_S5,
                                           FLASH_ADDRESS_S6,
                                           FLASH_ADDRESS_S7 };
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/* functions */
/*--------------------------------------------------------------------------------------------------------------------*/
#if defined(HAL_MODULE_ENABLED)

int32_t GetTickMs(uint32_t* ms)
{
  *ms = HAL_GetTick();

  return NO_FAILURE;
}

int32_t DelayMs(uint32_t ms)
{
  HAL_Delay(ms);

  return NO_FAILURE;
}

#endif /* HAL_MODULE_ENABLED */

/*--------------------------------------------------------------------------------------------------------------------*/
#if defined(HAL_FLASH_MODULE_ENABLED)

int32_t FLASH_Unlock(void)
{
  if (HAL_OK != HAL_FLASH_Unlock())
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

int32_t FLASH_Lock(void)
{
  if (HAL_OK != HAL_FLASH_Lock())
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

int32_t FLASH_IsLocked(void)
{
  if (RESET != READ_BIT(FLASH->CR, FLASH_CR_LOCK))
  {
    return TRUE;
  }

  return FALSE;
}

int32_t FLASH_FindSector(uint32_t address, uint32_t* sector)
{
  if (!IS_VALID_FLASH_ADDRESS(address))
  {
    return FAILURE;
  }

  uint32_t i = NUMEL(FLASH_SectorMapping);
  do
  {
    i--;
    if (address >= FLASH_SectorMapping[i])
    {
      break;
    }
  } while (i != 0);

  *sector = i;

  return NO_FAILURE;
}

int32_t FLASH_EraseSector(uint32_t sector, uint32_t totalNumber)
{
  FLASH_EraseInitTypeDef eraseInit;
  uint32_t               sectorError;

  /* Select mass or sector erase. */
  eraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;

  /* Select banks to erase, when mass erase is enabled. */
  eraseInit.Banks        = 0;

  /* Select initial sector to erase, when mass erase is disabled. */
  eraseInit.Sector       = sector;

  /* Select number of sectors to be erased. */
  eraseInit.NbSectors    = totalNumber;

  /* Select device voltage range, which defines the erase parallelism. */
  eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_2;

  /* Erase FLASH sectors. */
  if (HAL_OK != HAL_FLASHEx_Erase(&eraseInit, &sectorError))
  {
    return FAILURE;
  }

  /* Check for faulty sector. */
  if (0xFFFFFFFFU != sectorError)
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

int32_t FLASH_ProgramByte(uint32_t address, uint8_t data)
{
  if (!IS_VALID_FLASH_ADDRESS(address))
  {
    return FAILURE;
  }

  if (HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address, data))
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

int32_t FLASH_ProgramByteBuffer(uint32_t address, uint8_t* data, uint32_t length)
{
  uint32_t flashAddress = address;
  uint8_t* sramData     = data;

  for (int i=0; i<length; i++)
  {
    if (NO_FAILURE != FLASH_ProgramByte(flashAddress, *sramData))
    {
      return FAILURE;
    }

    /* Increment address and data pointer for next iteration. */
    flashAddress++;
    sramData++;
  }

  /* Compare FLASH and SRAM memory. */
  if (0 != memcmp((uint8_t*) address, data, length))
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

#endif /* HAL_FLASH_MODULE_ENABLED */

/*--------------------------------------------------------------------------------------------------------------------*/
#if defined(HAL_IWDG_MODULE_ENABLED)

int32_t IWDG_Reload(IWDG_HandleTypeDef* iwdg)
{
  if (HAL_OK != HAL_IWDG_Refresh(iwdg))
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

int32_t IWDG_StopDebugMCU(void)
{
  __HAL_DBGMCU_FREEZE_IWDG();

  return NO_FAILURE;
}

#endif /* HAL_IWDG_MODULE_ENABLED */

/*--------------------------------------------------------------------------------------------------------------------*/
#if defined(HAL_GPIO_MODULE_ENABLED)

int32_t GPIO_ReadPin(GPIO_TypeDef* port, uint16_t pin, LogicalValue_t* level)
{
  *level = LOW;

  if (GPIO_PIN_RESET != HAL_GPIO_ReadPin(port, pin))
  {
    *level = HIGH;
  }

  return NO_FAILURE;
}

#endif /* HAL_GPIO_MODULE_ENABLED */

/*--------------------------------------------------------------------------------------------------------------------*/
#if defined(HAL_UART_MODULE_ENABLED)

int32_t UART_Transmit(UART_HandleTypeDef* uart, uint8_t* data, uint16_t size, uint32_t timeout)
{
  switch (HAL_UART_Transmit(uart, data, size, timeout))
  {
    case HAL_OK:      return NO_FAILURE;
    case HAL_TIMEOUT: return TIMEOUT;
    default:          return FAILURE;
  }
}

int32_t UART_Receive(UART_HandleTypeDef* uart, uint8_t* data, uint16_t size, uint32_t timeout)
{
  switch (HAL_UART_Receive(uart, data, size, timeout))
  {
    case HAL_OK:      return NO_FAILURE;
    case HAL_TIMEOUT: return TIMEOUT;
    default:          return FAILURE;
  }
}

#endif /* HAL_UART_MODULE_ENABLED */
