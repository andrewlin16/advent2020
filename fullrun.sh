#!/bin/bash

set -eu

if [[ $# -lt 1 ]] ; then
	echo "Usage: $0 <name>"
	exit 1
fi

./make.sh "$1" && ./test.sh "$1" && ./run.sh "$1"
