/*
 * CommandMemoryWrite.h
 *
 *  Created on: Dec 26, 2021
 *      Author: djek-sweng
 */

#ifndef COMMAND_MEMORY_WRITE_H
#define COMMAND_MEMORY_WRITE_H

#include "Command.h"
#include "Uart.h"

using namespace Connectivity;

namespace Bootloader
{
    class CommandMemoryWrite : public Command
    {
    public:
        CommandMemoryWrite(const char* device, uint32_t baudRate);
       ~CommandMemoryWrite();

        int32_t Execute();

        CommandMemoryWrite* SetAddress(uint32_t address);
        CommandMemoryWrite* SetLength(uint8_t length);
        CommandMemoryWrite* SetBuffer(uint8_t* buffer);

    private:
        uint32_t _address;
        uint8_t _length;
        uint8_t* _buffer;
    };
}

#endif /* COMMAND_MEMORY_WRITE_H */
