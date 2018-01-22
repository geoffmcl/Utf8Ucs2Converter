@setlocal
@set TMPFIL=Utf8Ucs2Converter.exe
@set TMPSRC=Release\%TMPFIL%
@if NOT EXIST %TMPSRC% (
	@echo Error: Unable to locate source '%TMPSRC%'! *** FIX ME ***
	@exit /b 1
)
@set TMPDIR=C:\MDOS
@if NOT EXIST %TMPDIR%\nul (
	@echo Error: Unable to locate destination dir '%TMPDIR%'! *** FIX ME ***
	@exit /b 1
)
@set TMPDST=%TMPDIR%\%TMPFIL%
@if NOT EXIST %TMPDST% goto DOCOPY
@fc4 -B -Q -V0 %TMPSRC% %TMPDST% > nul 2>&1
@if ERRORLEVEL 4 goto NOFC4
@if ERRORLEVEL 2 goto NOFC4
@if ERRORLEVEL 1 goto CHKUPD
@echo.
@echo Appears 
@echo src %TMPSRC% and 
@echo dst %TMPDST% are the *** SAME ***
@echo *** NO update required ***
@echo.
@goto END
:CHKUPD
@echo Appears src %TMPSRC% and dst %TMPDST% different
@echo Continue with UPDATE?
@pause
@echo Doing update...
:DOCOPY
copy %TMPSRC% %TMPDST%
@goto END
:NOFC4
@echo.
@echo Error: seem unable to run 'fc4' utility
@echo So not way to compare source and destination!
@echo.
@goto END


:END

	