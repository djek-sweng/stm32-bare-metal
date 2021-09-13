/*
 * CommandFlashLock.cpp
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#include "CommandFlashLock.h"
#include "global.h"

namespace Bootloader
{
    CommandFlashLock::CommandFlashLock(
        const char* device,
        int baudRate
    ) : Command { device, baudRate }
    {
    }

    CommandFlashLock::~CommandFlashLock()
    {
        Command::Close();
    }

    int CommandFlashLock::Execute()
    {
        BLDR_CommandMessage_t commandMessageTx;
        BLDR_CommandMessage_t commandMessageRx;

        BLDR_InitCommandMessageStruct(&commandMessageTx);
        BLDR_InitCommandMessageStruct(&commandMessageRx);

        commandMessageTx.Id     = BLDR_IDC_FLASH_LOCK;
        commandMessageTx.Length = 0;

        Command::TransmitReceive(&commandMessageTx, &commandMessageRx);

        Command::PrintMessage(&commandMessageTx, "----TX----");
        Command::PrintMessage(&commandMessageRx, "----RX----");

        return 0;
    }
}
