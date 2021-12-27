/*
 * Command.cpp
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#include "Command.h"
#include "global.h"

using namespace std;

namespace Bootloader
{
    Command::Command(
        const char* device,
        uint32_t baudRate
    )
    {
        _uart = new Uart(device, baudRate);
        _uart->Init();
    }

    Command::~Command()
    {
        Close();
    }

    int32_t Command::Transmit(BLDR_CommandMessage_t* commandMessageTx)
    {
        /* First update Crc16 of transmit message. */
        BLDR_UpdateCrc16(commandMessageTx);

        /* Transmit message. */
        _uart->Transmit((uint8_t*)&commandMessageTx->Id, 1);
        _uart->Transmit((uint8_t*)&commandMessageTx->Length, 1);

        if (commandMessageTx->Length > 0)
        {
            _uart->Transmit((uint8_t*)&commandMessageTx->Payload, commandMessageTx->Length);
        }

        _uart->Transmit((uint8_t*)&commandMessageTx->Crc16, 2);

        return NO_FAILURE;
    }

    int32_t Command::Receive(BLDR_CommandMessage_t* commandMessageRx
    )
    {
        /* Receive message. */
        BLDR_InitCommandMessageStruct(commandMessageRx);

        _uart->Receive((uint8_t*)&commandMessageRx->Id, 1);
        _uart->Receive((uint8_t*)&commandMessageRx->Length, 1);

        if (commandMessageRx->Length > 0)
        {
            _uart->Receive((uint8_t*)&commandMessageRx->Payload, commandMessageRx->Length);
        }

        _uart->Receive((uint8_t*)&commandMessageRx->Crc16, 2);

        return NO_FAILURE;
    }

    int32_t Command::Print(BLDR_CommandMessage_t* commandMessage, string title)
    {
        char text[32];

        cout << title << endl;   

        cout << "Id:      ";
        SPRINTF(text, "%02X", commandMessage->Id);
        cout << text << endl;

        cout << "Length:  ";
        SPRINTF(text, "%02X", commandMessage->Length);
        cout << text << endl;

        cout << "Payload: ";
        for (int i=0; i<commandMessage->Length; i++)
        {
            SPRINTF(text, "%02X", commandMessage->Payload[i]);
            cout << text << " ";

            /* Break line after 16 bytes. */
            if (0 == (i+1)%16)
            {
                cout << endl;
                cout << "         ";
            }
        }
        cout << endl;

        cout << "Crc16:   ";
        SPRINTF(text, "%02X", commandMessage->Crc16);
        cout << text << " " << endl;

        return NO_FAILURE;
    }

    int32_t Command::Close()
    {
        return _uart->DeInit();
    }
}
