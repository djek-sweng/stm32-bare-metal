/*
 * CommandMemoryRead.h
 *
 *  Created on: Dec 26, 2021
 *      Author: djek-sweng
 */

#ifndef COMMAND_MEMORY_READ_H
#define COMMAND_MEMORY_READ_H

#include "Command.h"
#include "Uart.h"

using namespace Connectivity;

namespace Bootloader
{
    class CommandMemoryRead : public Command
    {
    public:
        CommandMemoryRead(const char* device, uint32_t baudRate);
       ~CommandMemoryRead();

        int32_t Execute();

        CommandMemoryRead* SetAddress(uint32_t address);
        CommandMemoryRead* SetLength(uint8_t length);

    private:
        uint32_t _address;
        uint8_t _length;
    };
}

#endif /* COMMAND_MEMORY_READ_H */
