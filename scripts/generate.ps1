$config_type = $args[0]

Set-Location $PSScriptRoot\..\  # enter the parent folder
if (-not (Test-Path -Path ".\build")) {
    New-Item -Path ".\build" -ItemType "directory"
}
if (-not (Test-Path -Path "build\test_db")) {
    New-Item -Path "build\test_db" -ItemType "directory"
}
Set-Location .\build  # enter the build folder

$vcpkg_root = $env:VCPKG_ROOT

cmake -G "Visual Studio 15 2017" -T "v141" `
    -DCMAKE_TOOLCHAIN_FILE="$vcpkg_root\scripts\buildsystems\vcpkg.cmake" `
    -DCMAKE_CONFIGURATION_TYPES="$config_type" `
    -DCMAKE_BUILD_TYPE="$config_type" `
    ..
