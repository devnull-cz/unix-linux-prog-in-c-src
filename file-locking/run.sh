#!/bin/bash

#
# Use lock-unlock.c (build it and name it "lock-unlock") for file locking.
# This simple shell script simulates a single worker.  If holding a lock, it
# does some work by printing its PID.  You will see that only one process at a
# time is working.  Run it like this to see something:
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

(($# != 1)) && echo "usage: $0 <lock-file>" && exit 1
trap "rm -f $1; exit" INT TERM

[[ -x $lock ]] || { echo "program '$lock' does not exist"; exit 1; }

typeset -i rc
while true; do
	# Loop until lock acquired (active/busy waiting!).
	while true; do
		$lock $1 lock
		rc=$?

		# Something bad happened.
		((rc == 2)) && echo "UNEXPECTED FAILURE, exiting..." && \
		    return 1

		# Already locked.
		((rc == 1)) && sleep 1

		# Lock acquired now.
		((rc == 0)) && break
	done

	# Simulate some work by printing its PID every second.
	n=$(expr $RANDOM % 3 + 1)
	echo "Lock acquired by $$, will do some work."
	for i in $(yes | head -$n); do
		echo "  $$"
		sleep 1
	done
	echo "$$ is releasing the lock"
	# Does not check its return value, if the lock was removed by somebody
	# else, we ignore it.
	$lock $1 unlock
	sleep 1
done
