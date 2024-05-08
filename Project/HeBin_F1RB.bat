@echo off
cd ../Release
del FW_MERG.bin
cd ../Project
fsutil  file createnew ../Release/FW_MERG.bin 0x5000
cat ../Release/USERAPP_RB.bin >>../Release/FW_MERG.bin
hbin ../BootLoader/BootLoader_103RB.bin ../Release/FW_MERG.bin
echo %date:~0,4%-%date:~5,2%-%date:~8,2% %time:~0,2%:%time:~3,2%:%time:~6,2%