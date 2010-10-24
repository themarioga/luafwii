@echo off
:makea
cls
IF EXIST *.dol rm *.dol
IF EXIST *.elf rm *.elf
make
pause
goto makea