/*
 * CommandGetVersion.cpp
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#include "CommandGetVersion.h"
#include "global.h"

namespace Bootloader
{
    CommandGetVersion::CommandGetVersion(
        const char* device,
        int baudRate
    ) : Command { device, baudRate }
    {
    }

    CommandGetVersion::~CommandGetVersion()
    {
        Command::Close();
    }

    int CommandGetVersion::Execute()
    {
        BLDR_CommandMessage_t commandMessageTx;
        BLDR_CommandMessage_t commandMessageRx;

        BLDR_InitCommandMessageStruct(&commandMessageTx);
        BLDR_InitCommandMessageStruct(&commandMessageRx);

        commandMessageTx.Id     = BLDR_IDC_GET_VERSION;
        commandMessageTx.Length = 0;

        Command::TransmitReceive(&commandMessageTx, &commandMessageRx);

        Command::PrintMessage(&commandMessageTx, "----TX----");
        Command::PrintMessage(&commandMessageRx, "----RX----");

        return 0;
    }
}
