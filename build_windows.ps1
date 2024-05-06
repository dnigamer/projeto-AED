param (
    [string]$BUILD_TYPE
)

# Verificar se o tipo de build foi passado
if (-not $BUILD_TYPE) {
    Write-Host "Tipo de build não passado. Verifique: script.ps1 [Debug ou Release]"
    exit 1
}

$BUILD_TYPE = $BUILD_TYPE.ToLower()
if ($BUILD_TYPE -ne "debug" -and $BUILD_TYPE -ne "release") {
    Write-Host "Tipo de build inválido. Escolha entre 'Debug' ou 'Release'"
    exit 1
}

# Instalar vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
Set-Content -Path "vcpkg\triplets\x64-windows.cmake" -Value "set(VCPKG_BUILD_TYPE release)" -Force
.\vcpkg install qtbase:x64-windows --clean-after-build

# Configurar visual studio
$VS_VERSION = & "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -property catalog_productDisplayVersion
Write-Host "Versão Visual Studio: $VS_VERSION"
if ($VS_VERSION -match "^16") {
    $GENERATOR = "Visual Studio 16 2019"
} elseif ($VS_VERSION -match "^17") {
    $GENERATOR = "Visual Studio 17 2022"
} else {
    Write-Host "Unsupported Visual Studio version."
    exit 1
}

cmake -Bbuild_windows -S. -G $GENERATOR -A x64

# Fazer a build
cmake --build build_windows --config $BUILD_TYPE

# Copiar DLLs em falta para a pasta do executável
Write-Host "A copiar DLLs em falta"
if (Test-Path "build_windows\$BUILD_TYPE\") {
    Copy-Item "vcpkg\installed\x64-windows\bin\double-conversion.dll" -Destination "build_windows\$BUILD_TYPE" -Force
    Copy-Item "vcpkg\installed\x64-windows\bin\pcre2-16.dll" -Destination "build_windows\$BUILD_TYPE" -Force
    Copy-Item "vcpkg\installed\x64-windows\bin\zstd.dll" -Destination "build_windows\$BUILD_TYPE" -Force
    Copy-Item "vcpkg\installed\x64-windows\bin\libpng16.dll" -Destination "build_windows\$BUILD_TYPE" -Force
    Copy-Item "vcpkg\installed\x64-windows\bin\harfbuzz.dll" -Destination "build_windows\$BUILD_TYPE" -Force
}
