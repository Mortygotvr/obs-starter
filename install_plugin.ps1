# OBS Starter Plugin Installation Script
# Run this script as Administrator

Write-Host "Installing OBS Starter Plugin..." -ForegroundColor Green

# Check if running as administrator
if (-NOT ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Host "ERROR: This script requires administrator privileges." -ForegroundColor Red
    Write-Host "Please run PowerShell as Administrator and try again." -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit 1
}

# Get current directory (where the script is located)
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# Set paths
$sourceDir = Join-Path $scriptDir "build_x64\rundir\Release"
$obsPluginsDir = "C:\Program Files\obs-studio\obs-plugins\64bit"
$obsDataDir = "C:\Program Files\obs-studio\data\obs-plugins"

# Check if source files exist
if (-not (Test-Path "$sourceDir\obs-starter.dll")) {
    Write-Host "ERROR: Plugin DLL not found at $sourceDir\obs-starter.dll" -ForegroundColor Red
    Write-Host "Please build the plugin first using: cmake --build build_x64 --config Release" -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit 1
}

# Check if OBS is installed
if (-not (Test-Path $obsPluginsDir)) {
    Write-Host "ERROR: OBS Studio not found at expected location." -ForegroundColor Red
    Write-Host "Please install OBS Studio or check the installation path." -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit 1
}

try {
    # Copy main plugin DLL
    Write-Host "Copying plugin DLL..." -ForegroundColor Cyan
    Copy-Item "$sourceDir\obs-starter.dll" "$obsPluginsDir\" -Force
    
    # Copy locale files if they exist
    if (Test-Path "$sourceDir\obs-starter\locale") {
        Write-Host "Copying locale files..." -ForegroundColor Cyan
        $localeDestDir = "$obsDataDir\obs-starter\locale"
        if (-not (Test-Path $localeDestDir)) {
            New-Item -ItemType Directory -Path $localeDestDir -Force | Out-Null
        }
        Copy-Item "$sourceDir\obs-starter\locale\*" $localeDestDir -Recurse -Force
    }
    
    Write-Host "`nOBS Starter Plugin installed successfully!" -ForegroundColor Green
    Write-Host "`nThe plugin will be available in OBS Studio under:" -ForegroundColor White
    Write-Host "  Tools > OBS Starter Config" -ForegroundColor Yellow
    Write-Host "`nPlease restart OBS Studio to load the plugin." -ForegroundColor White
    
} catch {
    Write-Host "ERROR: Installation failed: $($_.Exception.Message)" -ForegroundColor Red
}

Read-Host "`nPress Enter to exit"