param (
    [string]$BUILD_TYPE
)

# Verificar se o tipo de build foi passado
if (-not $BUILD_TYPE) {
    Write-Host "Tipo de build nao passado. Verifique: script.ps1 [Debug ou Release]"
    exit 1
}

$BUILD_TYPE = $BUILD_TYPE.ToLower()
if ($BUILD_TYPE -ne "debug" -and $BUILD_TYPE -ne "release") {
    Write-Host "Tipo de build invalido. Escolha entre 'Debug' ou 'Release'"
    exit 1
}

# Instalar vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg integrate install
Add-Content -Path "triplets/x64-windows.cmake" -Value "set(VCPKG_BUILD_TYPE release)"
./vcpkg install qtbase:x64-windows --clean-after-build
cd ..

# Configurar visual studio
$VS_VERSION = & "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -property catalog_productDisplayVersion
Write-Host "Versao Visual Studio: $VS_VERSION"
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
Write-Host "DLLs copiadas!"

# Create a self-extracting executable
function Create-SingleExecutable {
    param (
        [string]$SourceFolder,
        [string]$OutputExe
    )

    # Load .NET assemblies
    Add-Type -AssemblyName "System.IO.Compression.FileSystem"

    # Create a temporary directory to hold the compressed files
    $tempDir = [System.IO.Path]::Combine([System.IO.Path]::GetTempPath(), [System.IO.Path]::GetRandomFileName())
    New-Item -ItemType Directory -Path $tempDir | Out-Null

    try {
        # Compress the source folder into a zip file
        $zipFile = [System.IO.Path]::Combine($tempDir, "temp.zip")
        [System.IO.Compression.ZipFile]::CreateFromDirectory($SourceFolder, $zipFile)

        # Read the zip file as byte array
        $zipBytes = [System.IO.File]::ReadAllBytes($zipFile)

        # Convert the byte array to base64 string
        $base64Content = [System.Convert]::ToBase64String($zipBytes)

        # Create a self-extracting executable with base64 encoded zip content
        $exeContent = @"
            \$zipBytes = [System.Convert]::FromBase64String('$base64Content')
            \$zipFile = [System.IO.Path]::Combine([System.IO.Path]::GetTempPath(), 'extracted.zip')
            [System.IO.File]::WriteAllBytes(\$zipFile, \$zipBytes)
            [System.IO.Compression.ZipFile]::ExtractToDirectory(\$zipFile, [System.IO.Path]::GetTempPath())
            Remove-Item \$zipFile
            Copy-Item -Path ".\build_windows\$BUILD_TYPE\*.dll" -Destination "\$env:TEMP\build_windows\$BUILD_TYPE" -Force
            Start-Process -FilePath "\$env:TEMP\build_windows\$BUILD_TYPE\Projeto.exe"
"@

        # Append the extraction script to a new executable
        Set-Content -Path $OutputExe -Value $exeContent -Encoding ASCII

        Write-Output "Single executable created successfully: $OutputExe"
    }
    finally {
        # Clean up temporary directory
        Remove-Item -Path $tempDir -Force -Recurse
    }
}

# Call the function to create the single executable
Create-SingleExecutable -SourceFolder "build_windows/Release" -OutputExe "Projeto.exe"
