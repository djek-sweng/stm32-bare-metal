/*
 * CommandFlashUnlock.cpp
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#include "CommandFlashUnlock.h"
#include "global.h"

namespace Bootloader
{
    CommandFlashUnlock::CommandFlashUnlock(
        const char* device,
        uint32_t baudRate
    ) : Command { device, baudRate }
    {
    }

    CommandFlashUnlock::~CommandFlashUnlock()
    {
        Command::Close();
    }

    int32_t CommandFlashUnlock::Execute()
    {
        BLDR_CommandMessage_t commandMessageTx;
        BLDR_CommandMessage_t commandMessageRx;

        BLDR_InitCommandMessageStruct(&commandMessageTx);
        BLDR_InitCommandMessageStruct(&commandMessageRx);

        commandMessageTx.Id     = BLDR_IDC_FLASH_UNLOCK;
        commandMessageTx.Length = 0;

        Command::Transmit(&commandMessageTx);
        Command::Receive(&commandMessageRx);

        Command::Print(&commandMessageTx, "----TX----");
        Command::Print(&commandMessageRx, "----RX----");

        return NO_FAILURE;
    }
}
