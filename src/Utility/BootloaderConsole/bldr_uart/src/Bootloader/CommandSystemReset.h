/*
 * CommandSystemReset.h
 *
 *  Created on: Dec 26, 2021
 *      Author: djek-sweng
 */

#ifndef COMMAND_SYSTEM_RESET_H
#define COMMAND_SYSTEM_RESET_H

#include "Command.h"
#include "Uart.h"

using namespace Connectivity;

namespace Bootloader
{
    class CommandSystemReset : public Command
    {
    public:
        CommandSystemReset(const char* device, uint32_t baudRate);
       ~CommandSystemReset();

        int32_t Execute();

        CommandSystemReset* SetDelay(uint32_t delay);

    private:
        uint32_t _delay;
    };
}

#endif /* COMMAND_SYSTEM_RESET_H */
