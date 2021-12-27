/*
 * Command.h
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "Uart.h"
#include "global.h"

using namespace std;
using namespace Connectivity;

namespace Bootloader
{
    class Command
    {
    public:
        Command(const char* device, uint32_t baudRate);
       ~Command();

        int32_t Transmit(BLDR_CommandMessage_t* commandMessageTx);
        int32_t Receive(BLDR_CommandMessage_t* commandMessageRx);
        int32_t Print(BLDR_CommandMessage_t* commandMessage, string title = "");
        int32_t Close();

        virtual int32_t Execute() = 0;

    private:
        Uart* _uart;
    };
}

#endif /* COMMAND_H */
