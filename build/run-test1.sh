#!/bin/sh
BN=`basename $0`
TMPEXE="Utf8Ucs2Converter"
if [ ! -x "$TMPEXE" ]; then
    echo "$BN: Error: Can NOT locate '$TMPEXE'! *** FIX ME ***"
    exit 1
fi

TMPTXT1="temp1.txt"
TMPTXT2="temp2.txt"
TMPTXT3="temp3.txt"
TMPTXT4="temp4.txt"

# @REM Write a utf-8, actually just ANSI text file
if [ -f "$TMPTXT1" ]; then
    rm -f "$TMPTXT1"
fi
if [ -f "$TMPTXT2" ]; then
    rm -f "$TMPTXT2"
fi
if [ -f "$TMPTXT3" ]; then
    rm -f "$TMPTXT3"
fi
if [ -f "$TMPTXT4" ]; then
    rm -f "$TMPTXT4"
fi

echo "$BN:Writting: '$TMPTXT1'"
echo "Line 1" > $TMPTXT1
echo "Line 2" >> $TMPTXT1
echo "Line 3" >> $TMPTXT1

#@REM Convert to UCS2/UNICODE, with BOM
echo ""
echo "$BN:Doing: './$TMPEXE -o $TMPTXT2 $TMPTXT1'"
./$TMPEXE -o $TMPTXT2 $TMPTXT1
if [ ! "$?" = "0" ]; then
    echo "$BN: Got ERROR!"
    exit 1
fi

echo ""
echo "$BN:Doing: $TMPEXE -r -o $TMPTXT3 $TMPTXT2"
./$TMPEXE -r -o $TMPTXT3 $TMPTXT2
if [ ! "$?" = "0" ]; then
    echo "$BN: Got ERROR!"
    exit 1
fi

echo ""
echo "$BN:Doing: 'diff -u $TMPTXT1 $TMPTXT3"
diff -u $TMPTXT1 $TMPTXT3
if [ ! "$?" = "0" ]; then
    echo "$BN: Got ERROR!"
    exit 1
fi

# @REM Convert to UCS2/UNICODE, with BOM
echo ""
echo "$BN:Doing: '$TMPEXE -o $TMPTXT4 $TMPTXT3'"
/.$TMPEXE -o $TMPTXT4 $TMPTXT3
if [ ! "$?" = "0" ]; then
    echo "$BN: Got ERROR!"
    exit 1
fi

echo ""
# fc4 -b %TMPTXT2% %TMPTXT4%
if [ ! "$?" = "0" ]; then
    echo "$BN: Got ERROR!"
    exit 1
fi

echo ""
echo "$BN: Congrats! Appear a fully successful 2-way conversion UTF-8 to UCS-2 and in reverse.."
echo ""

# @REM eof

