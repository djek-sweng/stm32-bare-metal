/*
 * CommandSystemReset.cpp
 *
 *  Created on: Dec 26, 2021
 *      Author: djek-sweng
 */

#include "CommandSystemReset.h"
#include "global.h"

namespace Bootloader
{
    CommandSystemReset::CommandSystemReset(
        const char* device,
        uint32_t baudRate
    ) : Command { device, baudRate }
    {
        _delay = 0;
    }

    CommandSystemReset::~CommandSystemReset()
    {
        Command::Close();
    }

    int32_t CommandSystemReset::Execute()
    {
        BLDR_CommandMessage_t commandMessageTx;
        BLDR_CommandMessage_t commandMessageRx;

        BLDR_InitCommandMessageStruct(&commandMessageTx);
        BLDR_InitCommandMessageStruct(&commandMessageRx);

        commandMessageTx.Id     = BLDR_IDC_SYSTEM_RESET;
        commandMessageTx.Length = 4;

        uint32_t* ptr32 = (uint32_t*)&commandMessageTx.Payload[0];
        *ptr32 = _delay;

        Command::Transmit(&commandMessageTx);
        Command::Receive(&commandMessageRx);

        Command::Print(&commandMessageTx, "----TX----");
        Command::Print(&commandMessageRx, "----RX----");

        return NO_FAILURE;
    }

    CommandSystemReset* CommandSystemReset::SetDelay(uint32_t delay)
    {
        _delay = delay;

        return this;
    }
}
