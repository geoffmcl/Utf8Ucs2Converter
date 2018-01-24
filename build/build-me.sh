#!/bin/sh
#< build-me.sh - utf8ucs2 - 20180124
BN=`basename $0`
TMPPRJ="utf8ucs2converter"
TMPLOG="bldlog-1.txt"
TMPOPTS=""
# Possible options to help with problems
# TMPOPTS="$TMPOPTS -DCMAKE_VERBOSE_MAKEFILE=ON"

for arg in $@; do
	TMPOPTS="$TMPOPTS $arg"
done

if [ -z "$TMPOPTS" ]; then
	TMPOPTS="-DCMAKE_INSTALL_PREFIX=$HOME"
fi
if [ -f "$TMPLOG" ]; then
    rm -f $TMPLOG
fi

echo "Begin $TMPPRJ project build" > $TMPLOG

echo "Doing 'cmake .. $TMPOPTS'"
echo "Doing 'cmake .. $TMPOPTS'" >> $TMPLOG
cmake .. $TMPOPTS >> $TMPLOG 2>&1
if [ ! "$?" =  "0" ]; then
	echo "cmake config, gen error $?"
	echo "See $TMPLOG for details..."
	exit 1
fi

echo "Doing 'make'"
echo "Doing 'make'" >>$TMPLOG
make >>$TMPLOG 2>&1
if [ ! "$?" =  "0" ]; then
	echo "make error $?"
	echo "See $TMPLOG for details..."
	exit 1
fi

echo "Appears a successful build..."

echo "Perhaps follow with 'make install', to install to $HONE/bin unless changed..."


# eof

