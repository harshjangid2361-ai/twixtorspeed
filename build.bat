@echo off
setlocal enabledelayedexpansion

echo.
echo ===== TWIXTOR SPEED - BUILD SCRIPT =====
echo.

cmake --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake not installed!
    pause
    exit /b 1
)

set "ADOBE_SDK_PATH=C:\Users\Harshitjangid\Downloads\AfterEffectsSDK_25.6_61_win"

echo [STEP 1] Checking Adobe SDK...
if not exist "%ADOBE_SDK_PATH%" (
    echo ERROR: SDK path not found!
    echo Expected: %ADOBE_SDK_PATH%
    pause
    exit /b 1
)
echo SDK found: %ADOBE_SDK_PATH%
echo.

echo [STEP 2] Creating build directory...
if exist "build" rmdir /s /q build
mkdir build
cd build

echo [STEP 3] Generating project...
cmake -G "Visual Studio 16 2019" -DADOBE_SDK_PATH="%ADOBE_SDK_PATH%" ..
if errorlevel 1 (
    echo CMAKE FAILED!
    pause
    exit /b 1
)

echo [STEP 4] Building plugin...
cmake --build . --config Release
if errorlevel 1 (
    echo BUILD FAILED!
    pause
    exit /b 1
)

echo.
if exist "output\TwixtorSpeed.aex" (
    echo ===== SUCCESS! =====
    echo Plugin: %CD%\output\TwixtorSpeed.aex
    echo.
    echo Copy to: C:\Program Files\Adobe\Common\Plug-ins\7.0\MediaCore\
    echo Restart After Effects
    echo.
    pause
) else (
    echo ERROR: Plugin not created!
    pause
    exit /b 1
)

endlocal
