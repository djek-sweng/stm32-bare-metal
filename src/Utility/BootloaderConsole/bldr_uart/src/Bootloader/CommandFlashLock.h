/*
 * CommandFlashLock.h
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#ifndef COMMAND_FLASH_LOCK_H
#define COMMAND_FLASH_LOCK_H

#include "Command.h"
#include "Uart.h"

using namespace Connectivity;

namespace Bootloader
{
    class CommandFlashLock : public Command
    {
    public:
        CommandFlashLock(const char* device, int baudRate);
       ~CommandFlashLock();

        int Execute();

    private:
    };
}

#endif /* COMMAND_FLASH_LOCK_H */
