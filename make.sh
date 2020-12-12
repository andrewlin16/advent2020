#!/bin/bash

set -eu

if [[ $# -lt 1 ]] ; then
	echo "Usage: $0 <name>"
	exit 1
fi

srcfile="src/$1.cpp"
binfile="bin/$1"

grep -q "#include <util.h>" "${srcfile}" && utilargs="-Isrc src/util.cpp" || utilargs=""

g++ -std=c++20 ${utilargs} "${srcfile}" -o "${binfile}"
