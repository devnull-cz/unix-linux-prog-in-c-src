#!/usr/local/bin/bash

true | false | true | false | false

echo ${PIPESTATUS[*]}
