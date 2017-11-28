#!/bin/bash

#
# Use lock-unlock.c (build it and name it "lock-unlock") for file locking.
# This simple shell script simulates a worker. If holding a lock, it simulates
# some work by printing its PID. You will see that only one process at a time
# is working. Run it like this to see something:
#
#	$ ./run.sh xxx & ./run.sh xxx & ./run.sh xxx & ./run.sh xxx &
#
# We need bash for this script to get $RANDOM stuff. Also note that we use a
# busy waiting with "sleep 1". In production environment, that would not be
# the best way to wait.
#
# (c) jp@devnull.cz
#

lock=./lock-unlock

[ $# -ne 1 ] && echo "usage: $0 <lock-file>" && exit 1
trap "rm -f $1; exit" INT TERM

[ -x $lock ] || { echo "program '$lock' does not exist"; exit 1; }

while true; do
	# loop until lock acquired
	while true; do
		$lock $1 lock
		ret=$?

		# something bad happened
		[ $ret -eq 2 ] && echo "UNEXPECTED FAILURE, exiting..." && \
		    return 1

		# already locked
		[ $ret -eq 1 ] && sleep 1

		# lock OK
		[ $ret -eq 0 ] && break
	done

	# simulate some work by printing its PID every second
	n=`expr $RANDOM % 3 + 1`
	echo "lock acquired by $$, will do some work"
	for i in `yes | head -$n`; do
		echo "  $$"
		sleep 1
	done
	echo "$$ is releasing the lock"
	$lock $1 unlock
	sleep 1
done
