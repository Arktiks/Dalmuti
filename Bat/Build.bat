@ECHO off

@ECHO FAIL > BUILD_STATUS

call "F:\Visual Studio 2015\VC\vcvarsall.bat" x86

cd ..

devenv "%cd%\Dalmuti.sln" /rebuild debug 
devenv "%cd%\Dalmuti.sln" /rebuild release

IF NOT EXIST "%cd%\Build" GOTO FAIL

cd Build

IF NOT EXIST "%cd%\Debug" GOTO FAIL

cd Debug

IF NOT EXIST "%cd%\Dalmuti-Debug.exe" GOTO FAIL

MOVE /Y "%cd%\Dalmuti-Debug.exe" "%cd%\..\..\Website\"

cd ..

IF NOT EXIST "%cd%\Release" GOTO FAIL

cd Release

IF NOT EXIST "%cd%\Dalmuti-Release.exe" GOTO FAIL

MOVE /Y "%cd%\Dalmuti-Release.exe" "%cd%\..\..\Website\"

cd ..\..\Scripts

:ALL_OK
@ECHO OK > BUILD_STATUS

:FAIL