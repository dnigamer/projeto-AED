@ECHO off


REM Verificar tipo de build passado
if "%~1"=="" (
    echo Tipo de build não passado. Verifique: script.bat [Debug ou Release]
    exit /b 1
)
set "BUILD_TYPE=%~1"
if /i not "%BUILD_TYPE%"=="Debug" if /i not "%BUILD_TYPE%"=="Release" (
    echo Tipo de build inválido. Escolha entre "Debug" ou "Release"
    exit /b 1
)


REM Instalar vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
echo "set(VCPKG_BUILD_TYPE release)" > vcpkg/triplets/x64-windows.cmake
vcpkg install qtbase:x64-windows --clean-after-build


REM Configurar visual studio
"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -property catalog_productDisplayVersion > vs_version.tmp
set /p VS_VERSION=<vs_version.tmp
del vs_version.tmp
echo Versão Visual Studio: %VS_VERSION%
if "%VS_VERSION:~0,2%"=="16" (
    set "GENERATOR=Visual Studio 16 2019"
) else if "%VS_VERSION:~0,2%"=="17" (
    set "GENERATOR=Visual Studio 17 2022"
) else (
    echo Unsupported Visual Studio version.
)
cmake -Bbuild_windows -S. -G "%GENERATOR%" -A x64


REM Fazer a build - script.bat [Release|Debug]
cmake --build build_windows --config %BUILD_TYPE%


REM se houver problema com plugin de iniciar a window (improvável):
REM set QT_QPA_PLATFORM_PLUGIN_PATH=\vcpkg\installed\x64-windows\Qt6\plugins\platforms


REM Copiar DLLs em falta para a pasta do executável
echo A copiar DLLs em falta
if exist "build_windows\%BUILD_TYPE%\"  (
    copy "vcpkg\installed\x64-windows\bin\double-conversion.dll" "build_windows\%BUILD_TYPE%"
    copy "vcpkg\installed\x64-windows\bin\pcre2-16.dll" "build_windows\%BUILD_TYPE%"
    copy "vcpkg\installed\x64-windows\bin\zstd.dll" "build_windows\%BUILD_TYPE%"
    copy "vcpkg\installed\x64-windows\bin\libpng16.dll" "build_windows\%BUILD_TYPE%"
    copy "vcpkg\installed\x64-windows\bin\harfbuzz.dll" "build_windows\%BUILD_TYPE%"
)
