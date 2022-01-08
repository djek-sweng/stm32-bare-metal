/*
 * Uart.h
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>

namespace Connectivity
{
    class Uart
    {
    public:
        Uart(const char* device, uint32_t baudRate);
       ~Uart();

        int32_t Init();
        int32_t DeInit();
        int32_t Receive(uint8_t* buffer, uint8_t size);
        int32_t Transmit(uint8_t* buffer, uint8_t size);

    private:
        const char* _device;
        uint32_t _baudRate;
        int32_t _fid;
    };
}

#endif /* UART_H */
