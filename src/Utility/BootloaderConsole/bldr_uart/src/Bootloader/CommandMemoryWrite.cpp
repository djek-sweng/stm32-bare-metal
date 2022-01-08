/*
 * CommandMemoryWrite.cpp
 *
 *  Created on: Dec 26, 2021
 *      Author: djek-sweng
 */

#include "CommandMemoryWrite.h"
#include "global.h"

namespace Bootloader
{
    CommandMemoryWrite::CommandMemoryWrite(
        const char* device,
        uint32_t baudRate
    ) : Command { device, baudRate }
    {
        _address = 0;
        _length = 0;
        _buffer = 0;
    }

    CommandMemoryWrite::~CommandMemoryWrite()
    {
        Command::Close();
    }

    int32_t CommandMemoryWrite::Execute()
    {
        BLDR_CommandMessage_t commandMessageTx;
        BLDR_CommandMessage_t commandMessageRx;

        BLDR_InitCommandMessageStruct(&commandMessageTx);
        BLDR_InitCommandMessageStruct(&commandMessageRx);

        commandMessageTx.Id     = BLDR_IDC_MEMORY_READ;
        commandMessageTx.Length = 5;

        uint32_t* ptr32 = (uint32_t*)&commandMessageTx.Payload[0];
        *ptr32 = _address;

        uint8_t* ptr8 = (uint8_t*)&commandMessageTx.Payload[4];
        *ptr8 = _length;

        if (NULL == memcpy(&commandMessageTx.Payload[5], _buffer, _length))
        {
            return FAILURE;
        }

        Command::Transmit(&commandMessageTx);
        Command::Receive(&commandMessageRx);

        Command::Print(&commandMessageTx, "----TX----");
        Command::Print(&commandMessageRx, "----RX----");

        return NO_FAILURE;
    }

    CommandMemoryWrite* CommandMemoryWrite::SetAddress(uint32_t address)
    {
        _address = address;

        return this;
    }

    CommandMemoryWrite* CommandMemoryWrite::SetLength(uint8_t length)
    {
        _length = length;

        return this;
    }

    CommandMemoryWrite* CommandMemoryWrite::SetBuffer(uint8_t* buffer)
    {
        _buffer = buffer;

        return this;
    }
}
