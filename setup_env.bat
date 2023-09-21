@ECHO OFF

@SET GAME_DIR=C:\Program Files (x86)\Rockstar Games\GTA San Andreas
@SET PLUGIN_DIR=C:\plugin-sdk-master











@IF DEFINED GTA_SA_DIR (
	ECHO GTA_SA_DIR already set to:
	ECHO "%GTA_SA_DIR%"
	ECHO:
	ECHO New value:
	ECHO "%GAME_DIR%"
	ECHO:
	CHOICE /C YN /M "Do you want to update?"
	If ERRORLEVEL 2 GOTO SET_GAME_END
)

@SETX GTA_SA_DIR "%GAME_DIR%"
ECHO GTA_SA_DIR configured as:
ECHO "%GAME_DIR%"
:SET_GAME_END

ECHO:
ECHO:

@IF DEFINED PLUGIN_SDK_DIR (
	ECHO PLUGIN_SDK_DIR already set to:
	ECHO "%PLUGIN_SDK_DIR%"
	ECHO .
	ECHO New value:
	ECHO "%PLUGIN_DIR%"
	ECHO .
	CHOICE /C YN /M "Do you want to update?"
	If ERRORLEVEL 2 GOTO SET_PLUGIN_END
)

@SETX PLUGIN_SDK_DIR "%PLUGIN_DIR%"
ECHO PLUGIN_SDK_DIR configured as:
ECHO "%PLUGIN_DIR%"
:SET_PLUGIN_END

ECHO:
ECHO:

pause
exit
