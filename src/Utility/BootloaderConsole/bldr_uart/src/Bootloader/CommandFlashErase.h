/*
 * CommandFlashErase.h
 *
 *  Created on: Dec 26, 2021
 *      Author: djek-sweng
 */

#ifndef COMMAND_FLASH_ERASE_H
#define COMMAND_FLASH_ERASE_H

#include "Command.h"
#include "Uart.h"

using namespace Connectivity;

namespace Bootloader
{
    class CommandFlashErase : public Command
    {
    public:
        CommandFlashErase(const char* device, uint32_t baudRate);
       ~CommandFlashErase();

        int32_t Execute();

        CommandFlashErase* SetSector(uint32_t sector);
        CommandFlashErase* SetTotalNumber(uint32_t totalNumber);

    private:
        uint32_t _sector;
        uint32_t _totalNumber;
    };
}

#endif /* COMMAND_FLASH_ERASE_H */
