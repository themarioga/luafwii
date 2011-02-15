@echo off
set PATH=C:\devkitPro\msys\bin;C:\devkitPro\devkitPPC\bin;%PATH%
set DEVKITPPC=/c/devkitPro/devkitPPC
set DEVKITPRO=/c/devkitPro
:makea
cls
IF EXIST *.dol rm *.dol
IF EXIST *.elf rm *.elf
make
pause
goto makea