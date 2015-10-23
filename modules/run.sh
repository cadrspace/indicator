#!/bin/sh

# XXX: This hack is aimed to configure serial port properly before starting
# of the Scheme code.
#
# XXX: Too hacky, need to fix ASAP.
python ind.py

# Call the main Scheme program.
./indicator.scm --device=$1
