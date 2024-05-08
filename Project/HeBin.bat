@echo off
set filename=%date:~0,4%%date:~5,2%%date:~8,2%
cd ../Release
del FW_MERG.bin
cd ../Project
fsutil  file createnew ../Release/FW_MERG.bin 0x5000
cat ../Release/USERAPP.bin >>../Release/FW_MERG.bin
hbin ../BootLoader/BootLoader.bin ../Release/FW_MERG.bin
copy ..\Release\FW_MERG.bin ..\Release\FW_MERG_%filename%.bin
copy ..\Release\USERAPP.bin ..\Release\USERAPP_%filename%.bin
echo %date:~0,4%-%date:~5,2%-%date:~8,2% %time:~0,2%:%time:~3,2%:%time:~6,2%