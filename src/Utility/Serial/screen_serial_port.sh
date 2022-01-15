#!/bin/bash

# List devices:
#   $ ls /dev
#   $ lsof | grep /dev/cu.*
#
# Open serial port:
#   $ ./screen_serial_port.sh /dev/cu.usbmodem144303
#
# Kill process:
#   Ctrl+A K

export SERIAL_PORT=/dev/$1
export SERIAL_BAUDRATE=115200

screen $SERIAL_PORT $SERIAL_BAUDRATE
