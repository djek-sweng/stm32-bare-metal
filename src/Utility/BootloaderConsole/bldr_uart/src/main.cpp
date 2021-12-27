/*
 * main.cpp
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#include "Command.h"
#include "CommandGetVersion.h"
#include "CommandGetSessionId.h"
#include "CommandFlashUnlock.h"
#include "CommandFlashLock.h"
#include "CommandFlashErase.h"
#include "CommandMemoryRead.h"
#include "CommandMemoryWrite.h"
#include "CommandSystemReset.h"

using namespace Bootloader;

int main(int argc, char** argv)
{
    for (int i=0; i<argc; ++i)
    {
        cout << argv[i] << endl;
    }

    char* device = argv[1];
    int select = atoi(argv[2]);
    int param_0 = 0;/* argv[3] */
    int param_1 = 0;/* argv[4] */

    uint32_t baudRate = 115200;

    Command* command;
    int result;

    switch (select)
    {
    case 1:
    {
        command = new CommandGetSessionId(device, baudRate);
        break;
    }        

    case 2:
    {
        command = new CommandFlashUnlock(device, baudRate);
        break;
    }

    case 3:
    {
        command = new CommandFlashLock(device, baudRate);
        break;
    }

    case 4:
    {
        param_0 = 0x20000000;
        param_1 = 0x40;

        command = (Command*)(new CommandMemoryRead(device, baudRate))
            ->SetAddress(param_0)
            ->SetLength(param_1);
        break;
    }

    case 5:
    {
        param_0 = 4;
        param_1 = 4;

        command = (Command*)(new CommandFlashErase(device, baudRate))
            ->SetSector(param_0)
            ->SetTotalNumber(param_1);
        break;
    }

    case 6:
    {
        param_0 = 2000;

        command = (Command*)(new CommandSystemReset(device, baudRate))
            ->SetDelay(param_0);
        break;
    }

    case 7:
    {
        param_0 = 0x08010000;
        param_1 = 4;
        uint8_t buffer[4] = {0xde, 0xad, 0xbe, 0xef};

        command = (Command*)(new CommandMemoryWrite(device, baudRate))
            ->SetAddress(param_0)
            ->SetLength(param_1)
            ->SetBuffer(&buffer[0]);
        break;
    }

    case 0:
    default:
    {
        command = new CommandGetVersion(device, baudRate);
        break;
    }
    }

    result = command->Execute();
    command->Close();

    return result;
}
