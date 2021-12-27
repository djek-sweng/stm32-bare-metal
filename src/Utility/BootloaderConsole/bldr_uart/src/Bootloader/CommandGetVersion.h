/*
 * CommandGetVersion.h
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#ifndef COMMAND_GET_VERSION_H
#define COMMAND_GET_VERSION_H

#include "Command.h"
#include "Uart.h"

using namespace Connectivity;

namespace Bootloader
{
    class CommandGetVersion : public Command
    {
    public:
        CommandGetVersion(const char* device, uint32_t baudRate);
       ~CommandGetVersion();

        int32_t Execute();

    private:
    };
}

#endif /* COMMAND_GET_VERSION_H */
