/*
 * CommandGetSessionId.h
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#ifndef COMMAND_GET_SESSION_ID_H
#define COMMAND_GET_SESSION_ID_H

#include "Command.h"
#include "Uart.h"

using namespace Connectivity;

namespace Bootloader
{
    class CommandGetSessionId : public Command
    {
    public:
        CommandGetSessionId(const char* device, int baudRate);
       ~CommandGetSessionId();

        int Execute();

    private:
    };
}

#endif /* COMMAND_GET_SESSION_ID_H */
