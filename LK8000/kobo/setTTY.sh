#!/bin/sh
export BAUD_RATE=19200
export TTY_DEV=/dev/ttymxc0


###########################################################
# init tty
###########################################################
stty -F $TTY_DEV $BAUD_RATE


