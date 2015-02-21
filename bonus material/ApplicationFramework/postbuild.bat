REM Note: Sometimes xcopy seems to hang forever. Manually deleting destination files then could help.
REM ... or using robocopy

REM xcopy /d from to

REM cd sdks\Toolib\lib
REM IF [%1] == [--debug] (
REM robocopy /XO . ..\..\..\bin Toolib_dbg.dll
REM ) ELSE (
REM robocopy /XO . ..\..\..\bin Toolib.dll
REM )
REM cd ..\..\..
