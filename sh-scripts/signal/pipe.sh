#!/bin/sh

echo "$$ is starting..."
trap 'echo oh no!; exit' SIGINT

cat -u
