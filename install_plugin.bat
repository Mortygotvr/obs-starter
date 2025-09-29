@echo off
echo Installing OBS Starter Plugin...
echo.

:: Check if running as administrator
net session >nul 2>&1
if %errorLevel% == 0 (
    echo Running with administrator privileges.
) else (
    echo This script requires administrator privileges.
    echo Please run as administrator.
    pause
    exit /b 1
)

:: Set source and destination paths
set "SOURCE_DIR=C:\Users\death\source\repos\obs-Starter\build_x64\rundir\Release"
set "OBS_PLUGINS_DIR=C:\Program Files\obs-studio\obs-plugins\64bit"
set "OBS_DATA_DIR=C:\Program Files\obs-studio\data\obs-plugins"

:: Copy the main plugin DLL
echo Copying plugin DLL...
copy "%SOURCE_DIR%\obs-starter.dll" "%OBS_PLUGINS_DIR%\" /Y
if %errorLevel% neq 0 (
    echo ERROR: Failed to copy plugin DLL
    pause
    exit /b 1
)

:: Copy locale files if they exist
if exist "%SOURCE_DIR%\obs-starter\locale" (
    echo Copying locale files...
    if not exist "%OBS_DATA_DIR%\obs-starter" mkdir "%OBS_DATA_DIR%\obs-starter"
    xcopy "%SOURCE_DIR%\obs-starter\locale" "%OBS_DATA_DIR%\obs-starter\locale" /E /I /Y
)

echo.
echo OBS Starter Plugin installed successfully!
echo.
echo The plugin will be available in OBS Studio under:
echo   Tools ^> OBS Starter Config
echo.
echo Please restart OBS Studio to load the plugin.
echo.
pause