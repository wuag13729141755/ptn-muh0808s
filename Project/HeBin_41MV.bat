@echo off
cd ../Release
del FW_MV.bin
cd ../Project
fsutil  file createnew ../Release/FW_MV.bin 0x5000
cat ../Release/USERAPP_MV.bin >>../Release/FW_MV.bin
hbin ../BootLoader/BootLoader_41MV.bin ../Release/FW_MV.bin
echo %date:~0,4%-%date:~5,2%-%date:~8,2% %time:~0,2%:%time:~3,2%:%time:~6,2%