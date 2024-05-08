@echo off
set filename=%date:~0,4%%date:~5,2%%date:~8,2%
cd ../ReleaseN32G45X
del FW_MERG.bin
cd ../Project
fsutil  file createnew ../ReleaseN32G45X/FW_MERG.bin 0x4000
cat ../ReleaseN32G45X/USERAPP.bin >>../ReleaseN32G45X/FW_MERG.bin
hbin ../BootLoader/N32G45XBoot.bin ../ReleaseN32G45X/FW_MERG.bin
copy ..\ReleaseN32G45X\FW_MERG.bin ..\ReleaseN32G45X\FW_MERG_%filename%.bin
echo %date:~0,4%-%date:~5,2%-%date:~8,2% %time:~0,2%:%time:~3,2%:%time:~6,2%