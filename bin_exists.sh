#!/usr/bin/env bash

for BIN in "$@"
do
	if ! type $BIN > /dev/null; then
		echo "ERROR: $BIN could not be found"
		exit 1
	fi
done
