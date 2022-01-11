/*
 * bldr_api.c
 *
 *  Created on: Jul 20, 2021
 *      Author: djek-sweng
 */

/*--------------------------------------------------------------------------------------------------------------------*/
/* includes */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "bldr_global.h"

/*--------------------------------------------------------------------------------------------------------------------*/
/* defines */
/*--------------------------------------------------------------------------------------------------------------------*/
/* supervisor call number 0 (software interrupt to SVC_Handler) */
#define SVC_0_EnablePrivilegedMode() __ASM("SVC #0")

/*--------------------------------------------------------------------------------------------------------------------*/
/* globals */
/*--------------------------------------------------------------------------------------------------------------------*/
/* Bootloader FLASH sectors (none erasable)       16K 16K 16K 16K */
STATIC CONST uint8_t FlashSectorsNoneErasable[] = { 0,  1,  2,  3 };

/*--------------------------------------------------------------------------------------------------------------------*/
/* prototypes */
/*--------------------------------------------------------------------------------------------------------------------*/
STATIC int32_t prv_BLDR_CommandGetVersion                 (uint8_t* data, uint8_t* size);
STATIC int32_t prv_BLDR_CommandGetSessionId               (uint8_t* data, uint8_t* size);
STATIC int32_t prv_BLDR_CommandFlashUnlock                (uint8_t* data, uint8_t* size);
STATIC int32_t prv_BLDR_CommandFlashLock                  (uint8_t* data, uint8_t* size);
STATIC int32_t prv_BLDR_CommandFlashErase                 (uint8_t* data, uint8_t* size, uint32_t sector, uint32_t totalNumber);
STATIC int32_t prv_BLDR_CommandMemoryRead                 (uint8_t* data, uint8_t* size, uint32_t address, uint8_t length);
STATIC int32_t prv_BLDR_CommandMemoryWrite                (uint8_t* data, uint8_t* size, uint32_t address, uint8_t length, uint8_t* buffer);
STATIC int32_t prv_BLDR_CommandMemoryGetCrc16             (uint8_t* data, uint8_t* size, uint32_t address, uint32_t length);

A_NORETURN
STATIC void    prv_BLDR_CommandReset                      (uint32_t delay);

A_NORETURN
STATIC void    prv_BLDR_CommandJump                       (uint32_t address, uint32_t delay);

A_NORETURN A_NAKED
STATIC void    prv_BLDR_Jump_ASM                          (uint32_t sp, uint32_t rv);

EXTERN void    cb_BLDR_ExecuteBeforeCommandSystemReset    (BLDR_CommandMessage_t* messageRx, BLDR_CommandMessage_t* messageTx,
                                                           BLDR_SystemState_t* systemState);
EXTERN void    cb_BLDR_ExecuteBeforeCommandJump           (BLDR_CommandMessage_t* messageRx, BLDR_CommandMessage_t* messageTx,
                                                           BLDR_SystemState_t* systemState);

/*--------------------------------------------------------------------------------------------------------------------*/
/* functions */
/*--------------------------------------------------------------------------------------------------------------------*/
void BLDR_Init(void)
{
  if (TRUE == cb_BLDR_IsBootloaderRequest())
  {
    return;
  }

  STOP_HERE();

  prv_BLDR_CommandJump(FLASH_ADDRESS_S4, 0);
}

int32_t BLDR_ExecuteCommand(BLDR_CommandMessage_t* messageRx, BLDR_CommandMessage_t* messageTx,
                            BLDR_SystemState_t* systemState)
{
  uint16_t crcHost;
  uint16_t crcDevice;

  /* Check CRC16 calculated on host and device. */
  crcHost = messageRx->Crc16;

  BLDR_UpdateCrc16(messageRx);
  crcDevice = messageRx->Crc16;

  /* CRC16 mismatch? */
  if (crcDevice != crcHost)
  {
    systemState->ErrorRegister = FAILURE;
  }
  /* Error register is not empty? An error occurred before command execution. */
  else
  if (NO_FAILURE != systemState->ErrorRegister)
  {
    __NOP();
  }
  /* Execute command. */
  else
  {
    switch (messageRx->Id)
    {
    case BLDR_IDC_GET_VERSION:
      systemState->ErrorRegister = prv_BLDR_CommandGetVersion           (&messageTx->Payload[0], &messageTx->Length);
      break;

    case BLDR_IDC_GET_SESSION_ID:
      systemState->ErrorRegister = prv_BLDR_CommandGetSessionId         (&messageTx->Payload[0], &messageTx->Length);
      break;

    case BLDR_IDC_FLASH_UNLOCK:
      systemState->ErrorRegister = prv_BLDR_CommandFlashUnlock          (&messageTx->Payload[0], &messageTx->Length);
      break;

    case BLDR_IDC_FLASH_LOCK:
      systemState->ErrorRegister = prv_BLDR_CommandFlashLock            (&messageTx->Payload[0], &messageTx->Length);
      break;

    case BLDR_IDC_FLASH_ERASE:
      systemState->ErrorRegister = prv_BLDR_CommandFlashErase           (&messageTx->Payload[0], &messageTx->Length,
                                                                         /*sector*/           READ_U32( &(messageRx->Payload[0]) ),
                                                                         /*totalNumber*/      READ_U32( &(messageRx->Payload[4]) ));
      break;

    case BLDR_IDC_MEMORY_READ:
      systemState->ErrorRegister = prv_BLDR_CommandMemoryRead           (&messageTx->Payload[0], &messageTx->Length,
                                                                         /*address*/          READ_U32( &(messageRx->Payload[0]) ),
                                                                         /*length*/           READ_U8 ( &(messageRx->Payload[4]) ));
      break;

    case BLDR_IDC_MEMORY_WRITE:
      systemState->ErrorRegister = prv_BLDR_CommandMemoryWrite          (&messageTx->Payload[0], &messageTx->Length,
                                                                         /*address*/          READ_U32( &(messageRx->Payload[0]) ),
                                                                         /*length*/           READ_U8 ( &(messageRx->Payload[4]) ),
                                                                         /*buffer*/                   ( &(messageRx->Payload[5]) ));
      break;

    case BLDR_IDC_MEMORY_GET_CRC16:
      systemState->ErrorRegister = prv_BLDR_CommandMemoryGetCrc16       (&messageTx->Payload[0], &messageTx->Length,
                                                                         /*address*/          READ_U32( &(messageRx->Payload[0]) ),
                                                                         /*length*/           READ_U32( &(messageRx->Payload[4]) ));
      break;

    case BLDR_IDC_SYSTEM_RESET:
      cb_BLDR_ExecuteBeforeCommandSystemReset(
          messageRx,
          messageTx,
          systemState);
      prv_BLDR_CommandReset                                             (/*delay*/            READ_U32( &(messageRx->Payload[0]) ));
      STOP_HERE();
      break;

    case BLDR_IDC_JUMP:
      cb_BLDR_ExecuteBeforeCommandJump(
          messageRx,
          messageTx,
          systemState);
      prv_BLDR_CommandJump                                              (/*address*/          READ_U32( &(messageRx->Payload[0]) ),
                                                                         /*delay*/            READ_U32( &(messageRx->Payload[4]) ));
      STOP_HERE();
      break;

    default:
      systemState->ErrorRegister = FAILURE;

      messageTx->Length     = SIZE(uint8_t);
      messageTx->Payload[0] = 0;
      break;
    }
  }

  /* Answer with received command id. */
  messageTx->Id = messageRx->Id;

  /* Error occurred?
   * If yes, then flip MSB of the command identifier
   * in bootloader response (transmission message).
   */
  if (NO_FAILURE != systemState->ErrorRegister)
  {
    messageTx->Id |= 0x80;

    messageTx->Length = SIZE(uint32_t);

    WRITE_U32(&messageTx->Payload[0], systemState->ErrorRegister);
  }

  BLDR_UpdateCrc16(messageTx);

  return NO_FAILURE;
}

STATIC int32_t prv_BLDR_CommandGetVersion(uint8_t* data, uint8_t* size)
{
  CONST uint32_t version = BLDR_VERSION;

  *size = SIZE(version);

  if (NULL == memcpy(data, &version, *size))
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

STATIC int32_t prv_BLDR_CommandGetSessionId(uint8_t* data, uint8_t* size)
{
  STATIC uint8_t  isFirst_n = 0;
  STATIC uint32_t sessionId = 0;

  /* Calculate session id in case of first call. */
  if (0 == isFirst_n)
  {
    uint32_t tick;
    GetTickMs(&tick);

    srand(tick);
    sessionId = rand();

    isFirst_n = 1;
  }

  *size = SIZE(sessionId);

  if (NULL == memcpy(data, &sessionId, *size))
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

STATIC int32_t prv_BLDR_CommandFlashUnlock(uint8_t* data, uint8_t* size)
{
  *data = 0;
  *size = 0;

  return FLASH_Unlock();
}

STATIC int32_t prv_BLDR_CommandFlashLock(uint8_t* data, uint8_t* size)
{
  *data = 0;
  *size = 0;

  return FLASH_Lock();
}

STATIC int32_t prv_BLDR_CommandFlashErase(uint8_t* data, uint8_t* size, uint32_t sector, uint32_t totalNumber)
{
  *data = 0;
  *size = 0;

  /* FLASH is locked? */
  if (TRUE == FLASH_IsLocked())
  {
    return FAILURE;
  }

  /* Avoid erasing FLASH sectors containing the bootloader program. */

  /* Is FLASH sector member of none erasable sectors? */
  for (int i=0; i<totalNumber; i++)
  {
    for (int j=0; j<NUMEL(FlashSectorsNoneErasable); j++)
    {
      if ((sector+i) == FlashSectorsNoneErasable[j])
      {
        /* This FLASH sector is not erasable. */
        return FAILURE;
      }
    }
  }

  if (HAL_OK != FLASH_EraseSector(sector, totalNumber))
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

STATIC int32_t prv_BLDR_CommandMemoryRead(uint8_t* data, uint8_t* size, uint32_t address, uint8_t length)
{
  uint8_t* ptr = (uint8_t*) address;
  *size = length;

  /* Avoid buffer overflow here.
   * Length must not be greater than payload's byte size. */
  if (length > BLDR_COMMAND_PAYLOAD_BYTES)
  {
    return FAILURE;
  }

  if (NULL == memcpy(data, ptr, *size))
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

STATIC int32_t prv_BLDR_CommandMemoryWrite(uint8_t* data, uint8_t* size, uint32_t address, uint8_t length, uint8_t* buffer)
{
  *data = 0;
  *size = 0;

  if (IS_VALID_FLASH_ADDRESS(address) &&
      IS_VALID_FLASH_ADDRESS(address+length))
  {
    if (TRUE == FLASH_IsLocked())
    {
      return FAILURE;
    }

    if (NO_FAILURE != FLASH_ProgramByteBuffer(address, buffer, length))
    {
      return FAILURE;
    }
  }
  else
  if (IS_VALID_SRAM_ADDRESS(address) &&
      IS_VALID_SRAM_ADDRESS(address+length))
  {
    /* SRAM access is not implemented. */
    return FAILURE;
  }
  else
  {
    /* Invalid memory address. */
    return FAILURE;
  }

  return NO_FAILURE;
}

STATIC int32_t prv_BLDR_CommandMemoryGetCrc16(uint8_t* data, uint8_t* size, uint32_t address, uint32_t length)
{
  uint16_t* crc16 = (uint16_t*) data;

  /* Check arguments address and length.
   * Is CRC calculated over valid memory area? */
  if (FALSE)
  {
    return FAILURE;
  }

  *crc16 = BLDR_CalculateCrc16((uint8_t*) address, length, BLDR_CRC16_INIT, BLDR_CRC16_XOR);
  *size  = SIZE(uint16_t);

  return NO_FAILURE;
}

A_NORETURN
STATIC void prv_BLDR_CommandReset(uint32_t delay)
{
  DelayMs(delay);

  NVIC_SystemReset();
}

A_NORETURN
STATIC void prv_BLDR_CommandJump(uint32_t address, uint32_t delay)
{
  /* See ARM: How to Write a Bootloader
   * http://www.keil.com/support/docs/3913.htm */
  uint32_t* ptr = (uint32_t*) address;

  /* Slow down IWDG. */
  cb_BLDR_SlowDownIwdg();

  /* Make sure, that the core is in privileged mode. */
  if ( __get_CONTROL() & CONTROL_nPRIV_Msk )
  {
    /* NOT in privileged mode, when enter here.
     *
     * Trigger a SVC and enter handler mode (which can only run in privileged mode).
     * The nPRIV bit in the CONTROL register is cleared which can only be done in privileged mode.
     *
     * See ARM: How to write an SVC function
     * http://www.keil.com/support/docs/4063.htm */
    SVC_0_EnablePrivilegedMode();
  }

  /* Disable all enabled interrupts in NVIC. */
  for (int i=0; i<NUMEL(NVIC->ICER); i++)
  {
    NVIC->ICER[i] = 0xFFFFFFFF;
  }

  /* Disable all enabled peripherals which might generate interrupt requests,
   * and clear all pending interrupt flags in those peripherals. */
  cb_BLDR_JumpDeInit();

  /* Clear all pending interrupt requests in NVIC. */
  for (int i=0; i<NUMEL(NVIC->ICPR); i++)
  {
    NVIC->ICPR[i] = 0xFFFFFFFF;
  }

  /* Disable SysTick and clear its exception pending bit,
   * if it is used in the bootloader, e.g. by HAL or RTOS. */
  SysTick->CTRL = 0;
  SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;

  /* Disable individual fault handlers if the bootloader used them. */
  SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk | \
                   SCB_SHCSR_BUSFAULTENA_Msk | \
                   SCB_SHCSR_MEMFAULTENA_Msk ) ;

  /* Activate the MSP, if the core is found to currently run with the PSP.
   * As the compiler might still uses the stack, the PSP needs to be copied to the MSP before this. */
  if ( __get_CONTROL() & CONTROL_SPSEL_Msk )
  {
    /* MSP is NOT active, when enter here. */
    __set_MSP( __get_PSP() );
    __set_CONTROL( __get_CONTROL() & ~CONTROL_SPSEL_Msk );
  }

  /* Load the vector table address of the user application into SCB->VTOR register.
   * Make sure the address meets the alignment requirements. */
  SCB->VTOR = (uint32_t) ptr;

  /* Set the MSP to the value found in the user application vector table and then load
   * the PC with the reset vector value of the user application. */
  prv_BLDR_Jump_ASM( ptr[0], ptr[1] );

  /* The program flow will never return to this point. */
  STOP_HERE();
}

A_NORETURN A_NAKED
STATIC void prv_BLDR_Jump_ASM(uint32_t sp, uint32_t rv)
{
  /* Core registers are set via function arguments.
   *  R0 _ SP (stack pointer)
   *  R1 _ RV (reset vector) */
  __ASM
  (
    "msr  msp, r0 \n" /* set MSP */
    "bx   r1      \n" /* jump to reset vector */
  );
}
