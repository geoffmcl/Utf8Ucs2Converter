@setlocal
@set TMPEXE=Release\Utf8Ucs2Converter.exe
@if NOT EXIST %TMPEXE% (
@echo Error: Can NOT locate '%TMPEXE%'! *** FIX ME ***
@exit /b 1
)

@set TMPTXT1=temp1.txt
@set TMPTXT2=temp2.txt
@set TMPTXT3=temp3.txt
@set TMPTXT4=temp4.txt

@REM Write a utf-8, actually just ANSI text file
@echo Line 1 > %TMPTXT1%
@echo Line 2 >> %TMPTXT1%
@echo Line 3 >> %TMPTXT1%

@REM Convert to UCS2/UNICODE, with BOM
@echo Doing: '%TMPEXE% -o %TMPTXT2% %TMPTXT1%'
@%TMPEXE% -o %TMPTXT2% %TMPTXT1%
@if ERRORLEVEL 1 (
@echo Got ERROR!
@exit /b 1
)
@echo Doing: %TMPEXE% -r -o %TMPTXT3% %TMPTXT2%
@%TMPEXE% -r -o %TMPTXT3% %TMPTXT2%
@if ERRORLEVEL 1 (
@echo Got ERROR!
@exit /b 1
)

fc4 %TMPTXT1% %TMPTXT3%
@if ERRORLEVEL 1 (
@echo Got ERROR!
@exit /b 1
)

@REM Convert to UCS2/UNICODE, with BOM
@echo Doing: '%TMPEXE% -o %TMPTXT4% %TMPTXT3%'
@%TMPEXE% -o %TMPTXT4% %TMPTXT3%
@if ERRORLEVEL 1 (
@echo Got ERROR!
@exit /b 1
)

fc4 -b %TMPTXT2% %TMPTXT4%
@if ERRORLEVEL 1 (
@echo Got ERROR!
@exit /b 1
)

@echo.
@echo Congrats! Appear a fully successful 2-way conversion UTF-8 to UCS-2 and in reverse..
@echo.

@REM eof
