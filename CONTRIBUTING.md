# Contributing to OBS Starter Plugin

Thank you for your interest in contributing to the OBS Starter Plugin! This document provides guidelines for contributing to the project.

## Code of Conduct

Be respectful and considerate in all interactions. We're all here to improve the plugin and help the community.

## How to Contribute

### Reporting Bugs

1. Check if the bug has already been reported in [Issues](../../issues)
2. Use the Bug Report template
3. Include as much detail as possible:
   - Steps to reproduce
   - Expected vs actual behavior
   - Environment details (OS, OBS version, plugin version)
   - OBS log files if relevant

### Suggesting Features

1. Check if the feature has already been requested in [Issues](../../issues)
2. Use the Feature Request template
3. Clearly describe the feature and why it would be useful

### Code Contributions

1. Fork the repository
2. Create a new branch for your feature/fix:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. Make your changes following the coding standards below
4. Test thoroughly
5. Commit with clear, descriptive messages
6. Push to your fork and submit a Pull Request

## Coding Standards

### C++ Style

- Follow the existing code style in the project
- Use meaningful variable and function names
- Add comments for complex logic
- Include error handling where appropriate

### CMake

- Follow existing CMake patterns
- Update CMakeLists.txt if adding new files
- Test builds on Windows (primary platform)

### Git Commit Messages

- Use present tense ("Add feature" not "Added feature")
- Use imperative mood ("Fix bug" not "Fixes bug")
- Limit first line to 50 characters
- Reference issues when applicable

Example:
```
Add command line argument support

- Add support for passing arguments to executables
- Update UI to include argument input field
- Fixes #123
```

## Development Setup

### Prerequisites

- Windows 10/11
- Visual Studio 2022 with C++ development tools
- CMake 3.28+
- OBS Studio installed

### Building

```powershell
git clone https://github.com/yourusername/obs-starter-plugin.git
cd obs-starter-plugin
cmake --preset windows-x64
cmake --build build_x64 --config Release
```

### Testing

1. Build the plugin
2. Run the installation script as Administrator:
   ```powershell
   .\install_plugin.ps1
   ```
3. Test in OBS Studio
4. Verify all features work as expected

## Pull Request Process

1. Ensure your code builds without warnings
2. Test your changes thoroughly
3. Update documentation if needed
4. Ensure your branch is up to date with main
5. Submit PR with:
   - Clear title and description
   - Reference any related issues
   - List of changes made

## Getting Help

- Check existing [Issues](../../issues) and [Discussions](../../discussions)
- Feel free to ask questions in a new issue
- Be patient and respectful when seeking help

Thank you for contributing! ??