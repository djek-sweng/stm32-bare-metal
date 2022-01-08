/*
 * CommandFlashErase.cpp
 *
 *  Created on: Dec 26, 2021
 *      Author: djek-sweng
 */

#include "CommandFlashErase.h"
#include "global.h"

namespace Bootloader
{
    CommandFlashErase::CommandFlashErase(
        const char* device,
        uint32_t baudRate
    ) : Command { device, baudRate }
    {
        _sector = 0;
        _totalNumber = 0;
    }

    CommandFlashErase::~CommandFlashErase()
    {
        Command::Close();
    }

    int32_t CommandFlashErase::Execute()
    {
        BLDR_CommandMessage_t commandMessageTx;
        BLDR_CommandMessage_t commandMessageRx;

        BLDR_InitCommandMessageStruct(&commandMessageTx);
        BLDR_InitCommandMessageStruct(&commandMessageRx);

        commandMessageTx.Id     = BLDR_IDC_FLASH_ERASE;
        commandMessageTx.Length = 8;

        uint32_t* ptr32 = (uint32_t*)&commandMessageTx.Payload[0];
        *ptr32 = _sector;

        ptr32 = (uint32_t*)&commandMessageTx.Payload[4];
        *ptr32 = _totalNumber;

        Command::Transmit(&commandMessageTx);
        Command::Receive(&commandMessageRx);

        Command::Print(&commandMessageTx, "----TX----");
        Command::Print(&commandMessageRx, "----RX----");

        return NO_FAILURE;
    }

    CommandFlashErase* CommandFlashErase::SetSector(uint32_t sector)
    {
        _sector = sector;

        return this;
    }

    CommandFlashErase* CommandFlashErase::SetTotalNumber(uint32_t totalNumber)
    {
        _totalNumber = totalNumber;

        return this;
    }
}
