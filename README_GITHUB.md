# OBS Starter Plugin

[![Build Status](https://github.com/Mortygotvr/obs-starter-plugin/workflows/Build%20Plugin/badge.svg)](https://github.com/Mortygotvr/obs-starter-plugin/actions)
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![Release](https://img.shields.io/github/v/release/Mortygotvr/obs-starter-plugin)](https://github.com/Mortygotvr/obs-starter-plugin/releases)

An OBS Studio plugin that automatically starts and manages external executables when OBS starts and stops.

## ?? Features

- **Automatic Startup**: Start multiple executables when OBS Studio launches
- **Automatic Shutdown**: Optionally terminate executables when OBS Studio closes
- **Start Minimized**: Launch executables in minimized window state (Windows)
- **GUI Configuration**: Easy-to-use configuration dialog accessible via Tools menu
- **Scrollable Interface**: Manage multiple executables with a clean, scrollable interface
- **Individual Control**: Each executable has its own configuration section with remove button

## ?? Installation

### Quick Install (Windows)

1. Download the latest release from [Releases](https://github.com/Mortygotvr/obs-starter-plugin/releases)
2. Extract the ZIP file
3. Run `install_plugin.ps1` as Administrator
4. Restart OBS Studio

### Manual Installation

1. Copy `obs-starter.dll` to `C:\Program Files\obs-studio\obs-plugins\64bit\`
2. Restart OBS Studio

## ?? Building from Source

### Prerequisites

- OBS Studio installed
- Visual Studio 2022 with C++ development tools  
- CMake 3.28 or later

### Build Steps

```powershell
git clone https://github.com/Mortygotvr/obs-starter-plugin.git
cd obs-starter-plugin
cmake --preset windows-x64
cmake --build build_x64 --config Release
```

## ?? Usage

1. Start OBS Studio
2. Go to **Tools** ? **OBS Starter Config**
3. Click **Add Executable** to add a new executable configuration
4. Browse and select the executable file you want to start with OBS
5. Configure options:
   - **Auto-shutdown when OBS closes**: Terminate when OBS exits
   - **Start minimized**: Launch in minimized window state
6. Click **Save** to save your configuration

## ??? Configuration Options

- **Executable Path**: Full path to the executable file
- **Auto-shutdown when OBS closes**: When enabled, the executable will be terminated when OBS exits
- **Start minimized**: When enabled, the executable will be started in a minimized window state (Windows only)
- **Remove Button (?)**: Click to remove that executable configuration

## ?? Troubleshooting

<details>
<summary>Plugin Not Loading</summary>

- Ensure OBS Studio is closed when installing the plugin
- Verify the plugin DLL is in the correct folder: `C:\Program Files\obs-studio\obs-plugins\64bit\`
- Check that you have the Visual C++ Redistributable installed
</details>

<details>
<summary>Executables Not Starting</summary>

- Verify the executable paths are correct and accessible
- Check that the executables don't require administrator privileges
- Ensure the executables are not blocked by antivirus software
</details>

<details>
<summary>Configuration Not Saving</summary>

- Check that OBS has write permissions to its configuration directory
- Verify that the config file is being created in the OBS config folder
</details>

## ?? License

This project is licensed under the GNU General Public License v2.0. See the [LICENSE](LICENSE) file for details.

## ?? Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## ?? Version History

- **v1.0.5**: Added "Start minimized" option to launch executables in minimized window state
- **v1.0.4**: Ultra-safe shutdown with comprehensive error handling and crash prevention
- **v1.0.3**: Added exception handling, extra safety checks, and detailed logging
- **v1.0.2**: Major crash fix - eliminated Qt object deletion during module unload
- **v1.0.1**: Fixed configuration saving and reduced shutdown crashes
- **v1.0.0**: Initial release with basic functionality

## ?? Acknowledgments

- [OBS Studio](https://obsproject.com/) - The amazing streaming software this plugin extends
- [Qt Framework](https://www.qt.io/) - For the cross-platform GUI components

## ? Support

If you find this plugin helpful, please consider:
- ? Starring this repository
- ?? Reporting issues
- ?? Suggesting new features
- ?? Contributing code

---

**Note**: The GitHub username has been updated to 'Mortygotvr' in all URLs above.