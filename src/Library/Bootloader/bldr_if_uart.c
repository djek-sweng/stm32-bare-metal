/*
 * bldr_if_uart.c
 *
 *  Created on: Jul 28, 2021
 *      Author: djek-sweng
 */

/*--------------------------------------------------------------------------------------------------------------------*/
/* includes */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "bldr_global.h"

/*--------------------------------------------------------------------------------------------------------------------*/
/* defines */
/*--------------------------------------------------------------------------------------------------------------------*/
#define BLDR_IF_UART_TRANSMIT_TIMEOUT           (0xffffffff)    /* maximal timeout (in ms) */
#define BLDR_IF_UART_RECEIVE_TIMEOUT            (1 *1000)       /* timeout (in ms) */

/*--------------------------------------------------------------------------------------------------------------------*/
/* prototypes */
/*--------------------------------------------------------------------------------------------------------------------*/
STATIC int32_t prv_BLDR_IfUartTransmit          (uint8_t* data, uint16_t size);
STATIC int32_t prv_BLDR_IfUartReceive           (uint8_t* data, uint16_t size);
STATIC int32_t prv_BLDR_IfUartCommandTransmit   (BLDR_CommandMessage_t* message);
STATIC int32_t prv_BLDR_IfUartCommandReceive    (BLDR_CommandMessage_t* message);
STATIC void    prv_BLDR_IfUartBeforeNoReturn    (BLDR_CommandMessage_t* messageRx, BLDR_CommandMessage_t* messageTx);

/*--------------------------------------------------------------------------------------------------------------------*/
/* functions */
/*--------------------------------------------------------------------------------------------------------------------*/
int32_t BLDR_IfUartInit(void* arg)
{
  NO_WARNING(arg);

  BLDR_Init();

  return NO_FAILURE;
}

A_NORETURN
void BLDR_IfUartStart(void* arg)
{
  NO_WARNING(arg);

  /* initialization */
  BLDR_SystemState_t systemState;
  BLDR_CommandMessage_t messageRx;
  BLDR_CommandMessage_t messageTx;

  BLDR_InitSystemStateStruct(&systemState);
  BLDR_InitCommandMessageStruct(&messageRx);
  BLDR_InitCommandMessageStruct(&messageTx);

  /* running state */
  for (;;)
  {
    cb_BLDR_ReloadIwdg();

    systemState.ErrorRegister = NO_FAILURE;
    BLDR_InitCommandMessageStruct(&messageRx);
    BLDR_InitCommandMessageStruct(&messageTx);

    systemState.ErrorRegister |= prv_BLDR_IfUartCommandReceive(&messageRx);
    systemState.ErrorRegister |= BLDR_ExecuteCommand(&messageRx, &messageTx, &systemState);
    systemState.ErrorRegister |= prv_BLDR_IfUartCommandTransmit(&messageTx);

    systemState.CommandCounter++;
  }

  /* Should never come here. */
  STOP_HERE();
}

STATIC int32_t prv_BLDR_IfUartTransmit(uint8_t* data, uint16_t size)
{
  if (NO_FAILURE != UART_Transmit(BLDR_UART, data, size, BLDR_IF_UART_TRANSMIT_TIMEOUT))
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

STATIC int32_t prv_BLDR_IfUartReceive(uint8_t* data, uint16_t size)
{
  int32_t status;

  do
  {
    cb_BLDR_ReloadIwdg();
    cb_BLDR_ToggleAliveLed();

    status = UART_Receive(BLDR_UART, data, size, BLDR_IF_UART_RECEIVE_TIMEOUT);

    /* If a UART receive timeout occurred, then stay in loop. */
  } while (TIMEOUT == status);

  if (NO_FAILURE != status)
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

STATIC int32_t prv_BLDR_IfUartCommandTransmit(BLDR_CommandMessage_t* message)
{
  uint32_t status = NO_FAILURE;
  uint16_t size;

  /* Id */
  status |= prv_BLDR_IfUartTransmit(&message->Id, 1);

  /* Length */
  status |= prv_BLDR_IfUartTransmit(&message->Length, 1);

  /* Payload (saturate size) */
  size = SIZE(message->Payload);
  size = GET_MIN(size, message->Length);

  if (size > 0)
  {
    status |= prv_BLDR_IfUartTransmit(&message->Payload[0], size);
  }

  /* Crc16 */
  status |= prv_BLDR_IfUartTransmit((uint8_t*)&message->Crc16, 2);

  /* Status */
  if (NO_FAILURE != status)
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

STATIC int32_t prv_BLDR_IfUartCommandReceive(BLDR_CommandMessage_t* message)
{
  uint32_t status = NO_FAILURE;
  uint16_t crcHost;
  uint16_t crcDevice;
  uint16_t size;

  /* Id */
  status |= prv_BLDR_IfUartReceive(&message->Id, 1);

  /* Length */
  status |= prv_BLDR_IfUartReceive(&message->Length, 1);

  /* Payload (saturate size) */
  size = SIZE(message->Payload);
  size = GET_MIN(size, message->Length);

  if (size > 0)
  {
    status |= prv_BLDR_IfUartReceive(&message->Payload[0], size);
  }

  /* Crc16 (received from host) */
  status |= prv_BLDR_IfUartReceive((uint8_t*)&message->Crc16, 2);
  crcHost = message->Crc16;

  /* Crc16 (calculated on device) */
  status |= BLDR_UpdateCrc16(message);
  crcDevice = message->Crc16;

  /* Status */
  if ((NO_FAILURE != status) ||
      (crcHost != crcDevice))
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

STATIC void prv_BLDR_IfUartBeforeNoReturn(BLDR_CommandMessage_t* messageRx, BLDR_CommandMessage_t* messageTx)
{
  messageTx->Id     = messageRx->Id;
  messageTx->Length = 0;

  BLDR_UpdateCrc16(messageTx);

  prv_BLDR_IfUartCommandTransmit(messageTx);
}

void cb_BLDR_ExecuteBeforeCommandSystemReset(BLDR_CommandMessage_t* messageRx, BLDR_CommandMessage_t* messageTx,
                                             BLDR_SystemState_t* systemState)
{
  NO_WARNING(systemState);

  prv_BLDR_IfUartBeforeNoReturn(messageRx, messageTx);
}

void cb_BLDR_ExecuteBeforeCommandJump(BLDR_CommandMessage_t* messageRx, BLDR_CommandMessage_t* messageTx,
                                      BLDR_SystemState_t* systemState)
{
  NO_WARNING(systemState);

  prv_BLDR_IfUartBeforeNoReturn(messageRx, messageTx);
}
