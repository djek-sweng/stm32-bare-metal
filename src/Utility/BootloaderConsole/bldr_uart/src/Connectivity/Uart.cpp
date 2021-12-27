/*
 * Uart.cpp
 *
 *  Created on: Sep 10, 2021
 *      Author: djek-sweng
 */

#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "basedefs.h"
#include "Uart.h"

namespace Connectivity
{
    Uart::Uart(
        const char* device,
        uint32_t baudRate
    )
    {
        _device = device;
        _baudRate = baudRate;
        _fid = -1;
    }

    Uart::~Uart()
    {
        DeInit();
    }

    int32_t Uart::Init()
    {
        struct termios options;

        /* Open device. */
        _fid = open(_device, O_RDWR | O_NOCTTY);

        if (-1 == _fid)
        {
            perror("Opening serial port failed.");
            return FAILURE;
        }

        /* Flush buffers of device. */
        if (0 != tcflush(_fid, TCIOFLUSH))
        {
            perror("Flushing serial port failed.");
        }

        /* Port configuration. */
        if (0 != tcgetattr(_fid, &options))
        {
            perror("Getting options of serial port failed.");
            close(_fid);
            return FAILURE;
        }

        /* Clear options. */
        REG_CLR(options.c_iflag,
            INLCR
            | IGNCR
            | ICRNL
            | IXON
            | IXOFF
        );

        REG_CLR(options.c_oflag,
            ONLCR
            | OCRNL
        );

        REG_CLR(options.c_lflag,
            ECHO
            | ECHONL
            | ICANON
            | ISIG
            | IEXTEN
        );

        /* Setup timeouts. */
        options.c_cc[VTIME] = 1;
        options.c_cc[VMIN]  = 0;

        /* Setup baud rate. */
        switch (_baudRate)
        {
        case   9600: _baudRate =   B9600; break;
        case  19200: _baudRate =  B19200; break;
        case  38400: _baudRate =  B57600; break;
        case 115200: _baudRate = B115200; break;
        default:
            fprintf(stderr,
                "Warning: Baud rate %u is not supported, using 9600.\n", _baudRate);
            _baudRate = B9600;
            break;
        }

        if (0 != cfsetospeed(&options, _baudRate))
        {
            perror("Setting output baud rate failed.");
            close(_fid);
            return FAILURE;
        }

        if (0 != cfsetispeed(&options, cfgetospeed(&options)))
        {
            perror("Setting input baud rate failed.");
            close(_fid);
            return FAILURE;
        }

        /* Setup attributes. */
        if (0 != tcsetattr(_fid, TCSANOW, &options))
        {
            perror("Setting attributes failed.");
            close(_fid);
            return FAILURE;
        }

        return _fid;
    }

    int32_t Uart::DeInit()
    {
        return close(_fid);
    }

    int32_t Uart::Receive(uint8_t* buffer, uint8_t size)
    {
        int rest = size;

        while (rest > 0)
        {
            /* Read single byte. */
            int r = read(_fid, buffer, 1);

            /* Failure. */
            if (r < 0)
            {
                perror("Reading from port failed.");
                return FAILURE;
            }

            /* Timeout. */
            if (r == 0)
            {
                continue;
            }

            buffer++;
            rest--;
        }

        return size;
    }

    int32_t Uart::Transmit(uint8_t* buffer, uint8_t size)
    {
        if (size != write(_fid, buffer, size))
        {
            perror("Writing to port failed.");
            return FAILURE;
        }

        return NO_FAILURE;
    }
}
