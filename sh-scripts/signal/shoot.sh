#!/bin/sh

echo "$$ is starting..."
trap 'echo oh no!; exit' SIGINT


for i in `jot 10`; do echo $$; sleep 1; done
