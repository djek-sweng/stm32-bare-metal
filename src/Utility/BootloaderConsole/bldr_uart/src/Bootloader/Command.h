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
        Command(const char* device, int baudRate);
       ~Command();

        int TransmitReceive(BLDR_CommandMessage_t* commandMessageTx, BLDR_CommandMessage_t* commandMessageRx);
        void PrintMessage(BLDR_CommandMessage_t* commandMessage, string title="");
        void Close();

        virtual int Execute() = 0;

    private:
        Uart* _uart;
    };
}

#endif /* COMMAND_H */
