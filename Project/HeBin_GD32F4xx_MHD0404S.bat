@echo off
set filename=%date:~0,4%%date:~5,2%%date:~8,2%
cd ../Release_GD32F4XX
del FW_MERGE_0404S.bin
cd ../Project
fsutil  file createnew ../Release_GD32F4XX/FW_MERGE_0404S.bin 0x10000
cat ../Release_GD32F4XX/USERAPP_0404S.bin >>../Release_GD32F4XX/FW_MERGE_0404S.bin
hbin ../BootLoader/GD32F4xxBoot.bin ../Release_GD32F4XX/FW_MERGE_0404S.bin
copy ..\Release_GD32F4XX\FW_MERGE_0404S.bin ..\Release_GD32F4XX\FW_MERGE_0404S_%filename%.bin
echo %date:~0,4%-%date:~5,2%-%date:~8,2% %time:~0,2%:%time:~3,2%:%time:~6,2%