#!/bin/sh
#
# wrapper for race.c
#

PROG=./a.out
SECS=${1:-5}

if [[ ! -x "$PROG" ]]; then
	echo "program $PROG not found, exiting"
	exit 1
fi

echo "running $PROG for $SECS seconds"
$PROG &
pid=$!
sleep $SECS
kill -INT $pid
