$config_type = $args[0]

$start_location = Resolve-Path .

if ($null -eq $config_type) {
    Write-Warning "No configuration specified, using Debug as default, if you want release, use `"confguire.ps1 Release`" instead."
    $config_type = "Debug"
}
else{
    Write-Host "Building configuration $config_type"
}

If (-not (Test-Path ".\CMakeLists.txt")) {
    Write-Warning "CMakeList.txt not found."
    exit
}

try {
    "cmake --version" | Out-Null
}
catch {
    Write-Warning "CMake not found!"
    exit
}

$VCPKG_RT = ${env:VCPKG_ROOT}
if ($null -eq $VCPKG_RT) {
    Write-Warning "Could not find VCPKG_ROOT. Please set up vcpkg environment."
    exit
}

try {
    "$VCPKG_RT\vcpkg" | Out-Null
}
catch {
    Write-Warning "Invalid VCPKG_ROOT. Please check vcpkg environment."
    exit
}

Write-Host "VCPKG_ROOT at $VCPKG_RT." -ForegroundColor Green

function Test-And-Install-Packages {
    param (
        $_module_name
    )
    $_start_loc = Resolve-Path .
    Set-Location $VCPKG_RT
    $_val = (& .\vcpkg list $_module_name)
    if ($null -eq $_val -or $_val -match '^No packages are installed') {
        Write-Host "Installing $_module_name ..."
        .\vcpkg install $_module_name
    }
    else {
        $_val = $_val.SubString(0,$_val.IndexOf("  "))
        Write-Host "$_val is installed" 
    }
    Set-Location $_start_loc
}

Write-Host "Checking denpendecies..." -ForegroundColor Green
Test-And-Install-Packages("sqlite3")
Test-And-Install-Packages("gtest")
Write-Host "Dependencies checked." -ForegroundColor Green

if (-not (Test-Path -Path ".\build")) {
    New-Item -Path ".\build" -ItemType "directory"
}
$build_dir = Resolve-Path ".\build"

Set-Location $build_dir

Write-Host "Configuration begins." -ForegroundColor Green
cmake `
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_RT\scripts\buildsystems\vcpkg.cmake" `
    -DCMAKE_CONFIGURATION_TYPES="$config_type" `
    -DCMAKE_BUILD_TYPE="$config_type" `
    ..

Write-Host "Build begins." -ForegroundColor Green
cmake --build .  --config $config_type

Set-Location $start_location



