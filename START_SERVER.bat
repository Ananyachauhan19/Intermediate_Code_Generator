@echo off
echo ========================================
echo  Intermediate Code Generator Server
echo ========================================
echo.

cd /d "%~dp0"

echo Checking if Node.js is installed...
where node >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Node.js is not installed!
    echo Please install Node.js from https://nodejs.org/
    echo.
    pause
    exit /b 1
)

echo Node.js found: 
node --version
echo npm version: 
npm --version
echo.

cd backend

if not exist "node_modules\" (
    echo Installing dependencies...
    echo This may take a few minutes...
    call npm install
    echo.
)

echo Starting server...
echo.
echo The web interface will be available at:
echo http://localhost:3000
echo.
echo Press Ctrl+C to stop the server
echo ========================================
echo.

node server.js

pause
