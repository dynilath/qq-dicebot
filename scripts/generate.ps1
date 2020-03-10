$config_type = $args[0]

$start_location = Resolve-Path .

If (-not (Test-Path ".\CMakeLists.txt")) {
    Write-Warning "This script should be called from project root folder."
    exit
}

Set-Location $PSScriptRoot\..\  # enter the parent folder

$build_folder = Resolve-Path ".\build"
if (-not (Test-Path -Path "$build_folder")) {
    New-Item -Path "$build_folder" -ItemType "directory"
}
Set-Location $build_folder  # enter the build folder

$vcpkg_root = $env:VCPKG_ROOT

cmake `
    -DCMAKE_TOOLCHAIN_FILE="$vcpkg_root\scripts\buildsystems\vcpkg.cmake" `
    -DCMAKE_CONFIGURATION_TYPES="$config_type" `
    -DCMAKE_BUILD_TYPE="$config_type" `
    -DBUILD_FOLDER="$build_folder" `
    ..

Set-Location $start_location