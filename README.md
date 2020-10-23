# CLEO library for GTA San Andreas

## About
CLEO is a hugely popular extensible library plugin which brings new possibilities in scripting for the game Grand Theft Auto: San Andreas by Rockstar Games, allowing the use of thousands of unique mods which change or expand the gameplay. You may find more information about CLEO on the official website https://cleo.li

## Installation
CLEO requires an 'ASI Loader' installed to run which is provided with the release. The ASI Loader requires overwriting one original game file: vorbisFile.dll - be sure to make a backup of this file.
No additional files are replaced, however the following files and folders are added:
- cleo\ (CLEO script directory)
- cleo\FileSystemOperations.cleo (file system plugin)
- cleo\IniFiles.cleo (ini config plugin)
- cleo\IntOperations.cleo (int operations plugin)
- cleo\cleo_save\ (CLEO save directory)
- cleo.asi (core library)
- bass.dll (audio engine library)
- vorbisHooked.dll (Silent's ASI Loader)
All plugins are optional, however they may be required by various CLEO scripts.

## CLEO Scripts
CLEO allows the installation of 'CLEO scripts', which often use the extension '.cs'. These third-party scripts are entirely user-made and are in no way supported by the developers of this library. While CLEO itself should work in a wide range of game installations, individual scripts are known to have their own compatibility restrictions and can not be guaranteed to work.
A huge range of CLEO scripts can be found on the web, notably via Grand Theft Auto fansites and modding sites such as:
http://hotmist.ddo.jp/cleomod/index.html
http://www.gtagarage.com/
http://gtainside.com/en/download.php?do=cat&id=322
http://zazmahall.de/CLEO.htm

## Compatibility Mode
CLEO is continually being improved and extended over time. In very rare circumstances, some scripts written for CLEO 3 may not work while using CLEO 4. However, since CLEO 4.3 you are able to enable a 'legacy mode' to increase compatibility with CLEO 3 scripts by naming them with the extension '.cs3'. CLEO 4.3 will load '.cs' and '.cs4' scripts normally and load '.cs3' scripts in CLEO 3 compatibility mode, in which certain small behaviours of the CLEO library will change to achieve better compatibility with that script. However, most CLEO 3 scripts will work without the need for compatibility mode being set as CLEO 4.3 also detects certain necessary CLEO 3 behaviours. Specifically, scripts which use the uninitialized storage data after a SCM function call to work.

## Credits
The author and original developer of the CLEO library is Seemann. Development of CLEO 4 was led by Alien and Deji. Today the CLEO library is an open-source project being maintained at https://github.com/cleolibrary

The author of the ASI Loader is Silent. Find out more at: http://gtaforums.com/topic/523982-relopensrc-silents-asi-loader/

Special thanks to:
 * Stanislav Golovin (a.k.a. listener) for his great work in exploration of the GTA series.
 * NTAuthority and LINK/2012 for additional support with CLEO 4.3.
 * mfisto for the alpha-testing of CLEO 4, his support and advices.

The developers have no connection with Take 2 Interactive or Rockstar Games.
By using this product or any of the additional products included you take your own personal responsibility for any negative consequences should they arise.
