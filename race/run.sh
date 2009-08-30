#!/bin/sh

SECS=${1:-5}

./a.out &
sleep $SECS
pkill -INT a.out
