#!/bin/bash

set -eu

if [[ $# -lt 1 ]] ; then
	echo "Usage: $0 <name>"
	exit 1
fi

g++ -std=c++20 "src/$1.cpp" -o "bin/$1"
