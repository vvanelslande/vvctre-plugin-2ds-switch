@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
cl /LD /Fe:2ds-switch.dll plugin.c

del 2ds-switch.lib
del 2ds-switch.exp
del plugin.obj

echo Useless files deleted
pause
