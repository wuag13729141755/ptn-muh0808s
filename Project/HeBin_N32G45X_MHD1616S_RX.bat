@echo off
set filename=%date:~0,4%%date:~5,2%%date:~8,2%
cd ../ReleaseN32G45X
del FW_MERG_MHD1616S_RX.bin
cd ../Project
fsutil  file createnew ../ReleaseN32G45X/FW_MERG_MHD1616S_RX.bin 0x4000
cat ../ReleaseN32G45X/USERAPP.bin >>../ReleaseN32G45X/FW_MERG_MHD1616S_RX.bin
hbin ../BootLoader/N32G45XBoot.bin ../ReleaseN32G45X/FW_MERG_MHD1616S_RX.bin
copy ..\ReleaseN32G45X\FW_MERG_MHD1616S_RX.bin ..\ReleaseN32G45X\FW_MERG_MHD1616S_RX_%filename%.bin
echo %date:~0,4%-%date:~5,2%-%date:~8,2% %time:~0,2%:%time:~3,2%:%time:~6,2%