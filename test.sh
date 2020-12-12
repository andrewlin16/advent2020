#!/bin/bash

set -eu

if [[ $# -lt 1 ]] ; then
	echo "Usage: $0 <name>"
	exit 1
fi

diff "data/$1_sample-output.txt" <(bin/$1 < "data/$1_sample-input.txt") && echo "pass" || echo "fail"
