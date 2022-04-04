#!/bin/sh

while true; do
	if test "$1" = ""; then
		break
	fi
	cat "$1"  | sed -e 's/fltk/fltkutf8/' > .tmp
	rm -f "$1"
	mv .tmp "$1"
	shift
done

rm -f .tmp

