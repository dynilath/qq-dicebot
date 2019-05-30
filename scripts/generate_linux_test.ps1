$config_type = 'Debug'

Set-Location $PSScriptRoot\..\  # enter the parent folder
if (-not (Test-Path -Path "build\$config_type")) {
    New-Item -Path "build\$config_type" -ItemType "directory"
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
Set-Location .\build\$config_type  # enter the build folder

if (-not (Test-Path -Path "$env:VCPKG_ROOT")) {
    Write-Host "vcpkg_root not found" -ForegroundColor Red
    Exit 1
}
$vcpkg_root = $env:VCPKG_ROOT
Write-Host "vcpkg_root at $env:VCPKG_ROOT" -ForegroundColor Green

$vcpkg_triplet = "x64-linux"

cmake -DCMAKE_TOOLCHAIN_FILE="$vcpkg_root\scripts\buildsystems\vcpkg.cmake" `
    -DVCPKG_TARGET_TRIPLET="$vcpkg_triplet" `
    -DCMAKE_CONFIGURATION_TYPES="$config_type" `
    -DCMAKE_BUILD_TYPE="$config_type" `
    -DCMAKE_TEST_SIGNATURE="linux64_test" `
    ..\..
