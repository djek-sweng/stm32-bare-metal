/*
 * bldr_misc.c
 *
 *  Created on: Jul 31, 2021
 *      Author: djek-sweng
 */

/*--------------------------------------------------------------------------------------------------------------------*/
/* includes */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "bldr_misc.h"
#include "bldr_crc16.h"
#include "basedefs.h"

/*--------------------------------------------------------------------------------------------------------------------*/
/* functions */
/*--------------------------------------------------------------------------------------------------------------------*/
int32_t BLDR_InitCommandMessageStruct(BLDR_CommandMessage_t* message)
{
  if (NULL == memset(message, 0, SIZE(BLDR_CommandMessage_t)))
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

int32_t BLDR_InitSystemStateStruct(BLDR_SystemState_t* systemState)
{
  if (NULL == memset(systemState, 0, SIZE(BLDR_SystemState_t)))
  {
    return FAILURE;
  }

  return NO_FAILURE;
}

int32_t BLDR_UpdateCrc16(BLDR_CommandMessage_t* message)
{
  uint16_t size;

  size = SIZE(message->Payload);
  size = GET_MIN(size, message->Length);

  message->Crc16 = BLDR_CalculateCrc16(&message->Id, (size + 2), BLDR_CRC16_INIT, BLDR_CRC16_XOR);

  return NO_FAILURE;
}
