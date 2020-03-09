$config_type = $args[0]

$start_location = Resolve-Path .

Set-Location $PSScriptRoot\..\  # enter the parent folder
if (-not (Test-Path -Path ".\build")) {
    New-Item -Path ".\build" -ItemType "directory"
}
if (-not (Test-Path -Path "build\test_db")) {
    New-Item -Path "build\test_db" -ItemType "directory"
}
if (-not (Test-Path -Path "build\test_db_1")) {
    New-Item -Path "build\test_db_1" -ItemType "directory"
}
if (-not (Test-Path -Path "build\test_db_2")) {
    New-Item -Path "build\test_db_2" -ItemType "directory"
}
Set-Location .\build  # enter the build folder

$vcpkg_root = $env:VCPKG_ROOT

cmake `
    -DCMAKE_TOOLCHAIN_FILE="$vcpkg_root\scripts\buildsystems\vcpkg.cmake" `
    -DCMAKE_CONFIGURATION_TYPES="$config_type" `
    -DCMAKE_BUILD_TYPE="$config_type" `
    ..

Set-Location $start_location