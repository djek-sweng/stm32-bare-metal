/*
 * CommandFlashUnlock.h
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#ifndef COMMAND_FLASH_UNLOCK_H
#define COMMAND_FLASH_UNLOCK_H

#include "Command.h"
#include "Uart.h"

using namespace Connectivity;

namespace Bootloader
{
    class CommandFlashUnlock : public Command
    {
    public:
        CommandFlashUnlock(const char* device, int baudRate);
       ~CommandFlashUnlock();

        int Execute();

    private:
    };
}

#endif /* COMMAND_FLASH_UNLOCK_H */
