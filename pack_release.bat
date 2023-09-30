@echo off

SET zip="C:\Program Files\7-Zip\7z.exe"

echo Preparing GTA SA CLEO
FOR /F "USEBACKQ" %%F IN (`powershell -NoLogo -NoProfile -Command ^(Get-Item ".output\Release\CLEO.asi"^).VersionInfo.FileVersion`) DO (SET fileVersion=%%F)
echo Detected version: %fileVersion%
SET outputFile=".\CLEO.SA_v%fileVersion%.zip"
if exist %outputFile% del %outputFile% /q

%zip% a -tzip %outputFile% ".\Changelog.md" -bb2 | findstr "+" 
%zip% rn %outputFile% "Changelog.md" "cleo_readme\Changelog.txt" -bso0

%zip% a -tzip %outputFile% ".\Readme.md" -bb2 | findstr "+" 
%zip% rn %outputFile% "Readme.md" "cleo_readme\Readme.txt" -bso0

%zip% a -tzip %outputFile% ".\.output\Release\CLEO.asi" -bb2 | findstr "+"

%zip% a -tzip %outputFile% "cleo_plugins\.output\*.cleo" -bb2 | findstr "+"
%zip% rn %outputFile% "cleo_plugins\.output" "cleo\cleo_plugins" -bso0

pause
