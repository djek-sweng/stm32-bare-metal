/*
 * bldr_api.h
 *
 *  Created on: Jul 20, 2021
 *      Author: djek-sweng
 */

#ifndef BLDR_API_H_
#define BLDR_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/* includes */
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdint.h>

/*--------------------------------------------------------------------------------------------------------------------*/
/* defines */
/*--------------------------------------------------------------------------------------------------------------------*/
#define BLDR_COMMAND_PAYLOAD_BYTES            (64)

/*--------------------------------------------------------------------------------------------------------------------*/
/* custom types */
/*--------------------------------------------------------------------------------------------------------------------*/
typedef enum CommandIdentifier
{
  BLDR_IDC_GET_VERSION                        = 0x30,
  BLDR_IDC_GET_SESSION_ID                     = 0x31,
  BLDR_IDC_FLASH_UNLOCK                       = 0x32,
  BLDR_IDC_FLASH_LOCK                         = 0x33,
  BLDR_IDC_FLASH_ERASE                        = 0x34,
  BLDR_IDC_MEMORY_READ                        = 0x35,
  BLDR_IDC_MEMORY_WRITE                       = 0x36,
  BLDR_IDC_MEMORY_GET_CRC16                   = 0x37,
  BLDR_IDC_SYSTEM_RESET                       = 0x38,
  BLDR_IDC_JUMP                               = 0x39,
} BLDR_CommandIdentifier_t;

typedef struct CommandMessage
{
  uint8_t   Id;
  uint8_t   Length;
  uint8_t   Payload [BLDR_COMMAND_PAYLOAD_BYTES];
  uint16_t  Crc16;
} BLDR_CommandMessage_t;

typedef struct SystemState
{
  uint32_t  CommandCounter;
  uint32_t  ErrorRegister;
} BLDR_SystemState_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/* prototypes */
/*--------------------------------------------------------------------------------------------------------------------*/
int32_t BLDR_ExecuteCommand                   (BLDR_CommandMessage_t* messageRx, BLDR_CommandMessage_t* messageTx,
                                               BLDR_SystemState_t* systemState);

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* BLDR_API_H_ */
