@echo off
rem Builds the project on Windows using MinGW g++ or MSVC cl (whichever is on PATH).
setlocal

where g++ >nul 2>nul
if %errorlevel%==0 (
    echo [build] Found MinGW g++ - compiling...
    if not exist build mkdir build
    g++ -std=c++17 -Wall -Wextra -Iinclude src\main.cpp -o build\dccollections.exe
    if errorlevel 1 (
        echo [build] FAILED
        exit /b 1
    )
    echo [build] OK - executable: build\dccollections.exe
    exit /b 0
)

where cl >nul 2>nul
if %errorlevel%==0 (
    echo [build] Found MSVC cl - compiling...
    if not exist build mkdir build
    cl /nologo /EHsc /W4 /std:c++17 /Iinclude /Fe:build\dccollections.exe src\main.cpp
    if errorlevel 1 (
        echo [build] FAILED
        exit /b 1
    )
    echo [build] OK - executable: build\dccollections.exe
    exit /b 0
)

echo [build] No C++ compiler found on PATH.
echo.
echo Install ONE of these, then re-run build.bat:
echo   1. WinLibs MinGW-w64 ^(recommended, simplest^):
echo      winget install BrechtSanders.WinLibs.POSIX.UCRT
echo      or download from https://winlibs.com/ and add its bin\ folder to PATH
echo   2. Visual Studio Build Tools ^(MSVC^):
echo      https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio
echo      ^(run from the "x64 Native Tools Command Prompt"^)
echo   3. Or open this folder in VS Code with the C/C++ extension.
exit /b 1
