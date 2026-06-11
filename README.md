# Twixtor Speed - After Effects Plugin

Speed control with frame interpolation for Adobe After Effects.

## Features

- Speed control (25% to 400%)
- Frame interpolation
- Quality settings

## Quick Start

### Local Build (RECOMMENDED)

1. Download Adobe SDK: https://developer.adobe.com/console
2. Extract to: `C:\Users\YourName\Downloads\AfterEffectsSDK_25.6_61_win`
3. Double-click `build.bat`
4. Wait for build to complete
5. Copy generated .aex file to After Effects plugins folder
6. Restart After Effects

### GitHub Build (EXPERIMENTAL)

See `GITHUB_SETUP.md` for instructions.

## Files

- `src/TwixtorSpeed.cpp` - Plugin source code
- `CMakeLists.txt` - Build configuration
- `build.bat` - Windows build script (local machine)
- `.github/workflows/build.yml` - GitHub Actions workflow

## Requirements

- Adobe After Effects CC 2020+
- CMake 3.12+
- Visual Studio 2019+
- Adobe After Effects SDK

## Build Time

- Local build: 10-15 minutes
- GitHub Actions: 15-20 minutes

## Support

This is an educational plugin. For production use, consider official Adobe plugins.
