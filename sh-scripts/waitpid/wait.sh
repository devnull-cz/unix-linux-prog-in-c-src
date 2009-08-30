#!/bin/sh

while :; do

  sleep 6 &
  echo -n "`date +%S`: "

  n=`jot -r 1 1 5`
  for i in `jot $n`; do
    echo -n .
    sleep 1
  done

  echo ""
  wait $!
done
