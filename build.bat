@echo off
REM Build script for Windows
REM Intermediate Code Generator

echo ============================================
echo Building Intermediate Code Generator...
echo ============================================

set SRC_DIR=src\compiler
set OBJ_DIR=build

if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"

g++ -std=c++11 -Wall -c "%SRC_DIR%\main.cpp" -o "%OBJ_DIR%\main.o"
g++ -std=c++11 -Wall -c "%SRC_DIR%\utils.cpp" -o "%OBJ_DIR%\utils.o"
g++ -std=c++11 -Wall -c "%SRC_DIR%\symbol_table.cpp" -o "%OBJ_DIR%\symbol_table.o"
g++ -std=c++11 -Wall -c "%SRC_DIR%\infix_postfix.cpp" -o "%OBJ_DIR%\infix_postfix.o"
g++ -std=c++11 -Wall -c "%SRC_DIR%\tac_generator.cpp" -o "%OBJ_DIR%\tac_generator.o"
g++ -std=c++11 -Wall -c "%SRC_DIR%\optimizer.cpp" -o "%OBJ_DIR%\optimizer.o"
g++ -std=c++11 -Wall -c "%SRC_DIR%\display.cpp" -o "%OBJ_DIR%\display.o"

echo.
echo Linking...
g++ -std=c++11 -Wall "%OBJ_DIR%\main.o" "%OBJ_DIR%\utils.o" "%OBJ_DIR%\symbol_table.o" "%OBJ_DIR%\infix_postfix.o" "%OBJ_DIR%\tac_generator.o" "%OBJ_DIR%\optimizer.o" "%OBJ_DIR%\display.o" -o icg.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ============================================
    echo Build successful! Run with: icg.exe
    echo ============================================
) else (
    echo.
    echo ============================================
    echo Build failed! Please check errors above.
    echo ============================================
)

echo.
pause
