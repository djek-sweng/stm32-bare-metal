#!/bin/bash

export SERIAL_PORT=/dev/$1
export SERIAL_BAUDRATE=115200

screen $SERIAL_PORT $SERIAL_BAUDRATE
# Kill process : Ctrl+A K
