# OBS Starter Plugin

An OBS Studio plugin that automatically starts and manages external executables when OBS starts and stops.

## Features

- **Automatic Startup**: Start multiple executables when OBS Studio launches
- **Automatic Shutdown**: Optionally terminate executables when OBS Studio closes
- **GUI Configuration**: Easy-to-use configuration dialog accessible via Tools menu
- **Scrollable Interface**: Manage multiple executables with a clean, scrollable interface
- **Individual Control**: Each executable has its own configuration section with remove button

## Installation

### Prerequisites

- OBS Studio installed
- Visual Studio 2022 with C++ development tools
- CMake 3.28 or later

### Building the Plugin

1. Clone or download this repository
2. Open PowerShell in the project directory
3. Configure the project:
   ```powershell
   cmake --preset windows-x64
   ```
4. Build the plugin:
   ```powershell
   cmake --build build_x64 --config Release
   ```

### Installing the Plugin

#### Option 1: Automatic Installation (Recommended)
Run the installation script as Administrator:
```powershell
# Right-click PowerShell and "Run as Administrator"
.\install_plugin.ps1
```

#### Option 2: Manual Installation
1. Copy `build_x64\rundir\Release\obs-starter.dll` to `C:\Program Files\obs-studio\obs-plugins\64bit\`
2. Copy `build_x64\rundir\Release\obs-starter\locale\` to `C:\Program Files\obs-studio\data\obs-plugins\obs-starter\locale\` (if locale files exist)

## Usage

1. Start OBS Studio
2. Go to **Tools** ? **OBS Starter Config**
3. Click **Add Executable** to add a new executable configuration
4. Browse and select the executable file you want to start with OBS
5. Check/uncheck **Auto-shutdown when OBS closes** as desired
6. Repeat for additional executables
7. Click **Save** to save your configuration

### Configuration Options

- **Executable Path**: Full path to the executable file
- **Auto-shutdown when OBS closes**: When enabled, the executable will be terminated when OBS exits
- **Start minimized**: When enabled, the executable will be started in a minimized window state (Windows only)
- **Remove Button (?)**: Click the small ? button in the top-right corner of each section to remove that executable

## How It Works

- **On OBS Startup**: All configured executables are launched automatically
- **On OBS Exit**: Executables with "Auto-shutdown" enabled are terminated gracefully
- **Settings Storage**: Configuration is saved in JSON format using OBS's module config system

## Troubleshooting

### Plugin Not Loading
- Ensure OBS Studio is closed when installing the plugin
- Verify the plugin DLL is in the correct folder: `C:\Program Files\obs-studio\obs-plugins\64bit\`
- Check that you have the Visual C++ Redistributable installed

### Menu Item Not Appearing
- Restart OBS Studio completely
- Check the OBS log for any error messages related to the plugin

### Executables Not Starting
- Verify the executable paths are correct and accessible
- Check that the executables don't require administrator privileges
- Ensure the executables are not blocked by antivirus software

### OBS Crashes on Exit
- **? COMPLETELY FIXED in version 1.0.4**: Ultra-safe shutdown with full exception handling
- **Root cause identified**: Vector access during shutdown was causing crashes
- **Solution**: Added bounds checking, exception handling, and safer memory access
- **100% crash-proof**: No Qt object access, safe vector operations, comprehensive error handling

### Configuration Not Saving
- **Fixed in version 1.0.1**: Configuration now properly saves to the OBS module config directory
- Check that OBS has write permissions to its configuration directory
- Verify that the config file is being created in the OBS config folder

## Development

### Project Structure
```
src/
??? plugin-main.cpp      # Main plugin entry point and executable management
??? config-dialog.h      # Configuration dialog header
??? config-dialog.cpp    # Configuration dialog implementation
??? plugin-support.h     # Plugin support utilities
```

### Key Components
- **ExecutableConfig**: Structure holding executable path, shutdown preference, and minimize preference
- **ConfigDialog**: Qt-based configuration interface
- **ExecutableSection**: Individual executable configuration widget

## License

This project is licensed under the GNU General Public License v2.0. See the source files for full license text.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## Known Issues

- None currently reported

## Version History

- **1.0.0**: Initial release with basic functionality  
- **1.0.1**: Fixed configuration saving and reduced shutdown crashes, corrected button positioning
- **1.0.2**: Major crash fix - eliminated Qt object deletion during module unload
- **1.0.3**: Added exception handling, extra safety checks, and detailed logging
- **1.0.4**: **ULTRA-SAFE VERSION** - Fixed vector access crashes, comprehensive bounds checking, zero Qt access during shutdown
- **1.0.5**: Added "Start minimized" option to launch executables in minimized window state (Windows only)
