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

using namespace Bootloader;

/*
 * $> ./bin/bldr_uart /dev/cu.usbmodem144303 0
 */
int main(int argc, char** argv)
{
    for (int i=0; i<argc; ++i)
    {
        cout << argv[i] << endl;
    }

    char* device = argv[1];
    int select = atoi(argv[2]);

    int baudRate = 115200;

    Command* command;
    int result;

    switch (select)
    {
    case 1:
        command = new CommandGetSessionId(device, baudRate);
        break;

    case 2:
        command = new CommandFlashUnlock(device, baudRate);
        break;

    case 3:
        command = new CommandFlashLock(device, baudRate);
        break;

    case 0:
    default:
        command = new CommandGetVersion(device, baudRate);
        break;
    }

    result = command->Execute();
    command->Close();

    return result;
}
