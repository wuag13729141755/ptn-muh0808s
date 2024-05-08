@echo off
cd ../ReleaseF2xx
del FW_MERG.bin
cd ../Project
fsutil  file createnew ../ReleaseF2xx/FW_MERG.bin 0xD000
cat ../ReleaseF2xx/USERAPP.bin >>../ReleaseF2xx/FW_MERG.bin
hbin ../BootLoader/F2xxBoot.bin ../ReleaseF2xx/FW_MERG.bin
echo %date:~0,4%-%date:~5,2%-%date:~8,2% %time:~0,2%:%time:~3,2%:%time:~6,2%