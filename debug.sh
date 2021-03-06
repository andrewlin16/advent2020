#!/bin/bash

set -eu

if [[ $# -lt 1 ]] ; then
	echo "Usage: $0 <name>"
	exit 1
fi

srcfile="src/$1.cpp"
binfile="$(mktemp)"

grep -q "#include <util.h>" "${srcfile}" && utilargs="-Isrc src/util.cpp" || utilargs=""

g++ -Wall -std=c++20 -g ${utilargs} "${srcfile}" -o "${binfile}"
gdb "${binfile}"
rm "${binfile}"
