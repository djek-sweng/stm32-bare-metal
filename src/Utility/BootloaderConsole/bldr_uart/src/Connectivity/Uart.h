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
        Uart(
            const char* device,
            uint32_t baudRate
        );
       ~Uart();

        int Init        ();
        int DeInit      ();
        int Receive     (uint8_t* buffer, uint8_t size);
        int Transmit    (uint8_t* buffer, uint8_t size);

    private:
        const char*     _device;
        uint32_t        _baudRate;
        int             _fid;
    };
}

#endif /* UART_H */
