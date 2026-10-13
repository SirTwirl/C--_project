@echo off
set "PATH=D:\CodeBlocks\share\CodeBlocks\compilers\mingw64\bin;%PATH%"

echo ======================================================================
echo    Kompilacja projektu symulatora w C++20 (GCC 14.2.0 MinGW-w64)
echo    Tryb: Statyczna konsolidacja (Standalone EXE)
echo ======================================================================

g++ -std=c++20 -O3 -Wall -Wextra -pedantic ^
    -static -static-libgcc -static-libstdc++ ^
    input.cpp ^
    Upload.cpp ^
    Usluga.cpp ^
    KatalogUslug.cpp ^
    Pracownik.cpp ^
    Zgloszenie.cpp ^
    AlgorytmA.cpp ^
    AlgorytmB.cpp ^
    AlgorytmC.cpp ^
    AlgorytmAging.cpp ^
    Statystyki.cpp ^
    SilnikSymulacji.cpp ^
    Output.cpp ^
    GeneratorZgloszen.cpp ^
    main.cpp ^
    -o symulator.exe

if %ERRORLEVEL% EQU 0 (
    echo [SUKCES] Kompilacja powiodla sie! Utworzono w pelni samodzielny symulator.exe
) else (
    echo [BLAD] Wystapily bledy kompilacji!
)
