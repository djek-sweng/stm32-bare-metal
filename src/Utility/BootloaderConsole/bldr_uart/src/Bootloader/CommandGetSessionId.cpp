/*
 * CommandGetSessionId.cpp
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#include "CommandGetSessionId.h"
#include "global.h"

namespace Bootloader
{
    CommandGetSessionId::CommandGetSessionId(
        const char* device,
        int baudRate
    ) : Command { device, baudRate }
    {
    }

    CommandGetSessionId::~CommandGetSessionId()
    {
        Command::Close();
    }

    int CommandGetSessionId::Execute()
    {
        BLDR_CommandMessage_t commandMessageTx;
        BLDR_CommandMessage_t commandMessageRx;

        BLDR_InitCommandMessageStruct(&commandMessageTx);
        BLDR_InitCommandMessageStruct(&commandMessageRx);

        commandMessageTx.Id     = BLDR_IDC_GET_SESSION_ID;
        commandMessageTx.Length = 0;

        Command::TransmitReceive(&commandMessageTx, &commandMessageRx);

        Command::PrintMessage(&commandMessageTx, "----TX----");
        Command::PrintMessage(&commandMessageRx, "----RX----");

        return 0;
    }
}
