@ECHO off

@ECHO FAIL > REPO_STATUS

CD ..\Source

IF NOT EXIST "%cd%\AI" GOTO NO_AI

@RD /Q /S "%cd%\AI"

:NO_AI
:: AI FOLDER MISSING

git clone https://github.com/Arktiks/Dalmuti-AI.git --quiet AI --depth=1 --branch=master

IF NOT EXIST "%cd%\AI" GOTO FAIL

CD AI

@RD /S /Q "%cd%\.git"
DEL /Q "%cd%\.gitignore"

SET count = 0
FOR %%x in (*) do set /a count+=1
ECHO %count%

IF /I "%count%" GEQ "3" GOTO ALL_OK

:ALL_OK
cd ..\..\Scripts
@ECHO OK > REPO_STATUS

:FAIL
:: DONE FUCKED UP