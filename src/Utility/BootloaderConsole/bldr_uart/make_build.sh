#!/bin/bash

# Build progamm (with make)
make clean
make create
make

exit 0

# Build program (with VS Code)
#   1) $ make create
#   2) Select file "main.cpp"
#   3) Click Terminal > Run Build Task... (Shift+Command+B)
#
# Run program
#   1) $ ls /dev
#   2) $ lsof | grep /dev/cu.*
#   3) $ ./bin/bldr_uart /dev/cu.usbmodem144303 0
#
# Clean program
#   1) $ make clean
