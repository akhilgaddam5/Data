@echo off
rem Runs the built executable. Build first with build.bat.
if not exist build\dccollections.exe (
    echo [run] build\dccollections.exe not found - run build.bat first.
    exit /b 1
)
build\dccollections.exe
