$config_type = $args[0]
$ci_config = $args[1]

Set-Location $PSScriptRoot\..\  # enter the parent folder
if(-not (Test-Path -Path "build\$config_type")){
    New-Item -Path "build\$config_type" -ItemType "directory"
}
if(-not (Test-Path -Path "build\test_db")){
    New-Item -Path "build\test_db" -ItemType "directory"
}
Set-Location .\build\$config_type  # enter the build folder

$vcpkg_root = $env:VCPKG_ROOT
$vcpkg_triplet = "cqdicebot-x86-windows-static"

cmake -G "Visual Studio 15 2017" -T "v141" `
    -DCMAKE_TOOLCHAIN_FILE="$vcpkg_root\scripts\buildsystems\vcpkg.cmake" `
    -DVCPKG_TARGET_TRIPLET="$vcpkg_triplet" `
    -DCMAKE_CONFIGURATION_TYPES="$config_type" `
    -DCMAKE_BUILD_TYPE="$config_type" `
    -DCMAKE_CI_CONFIG="$ci_config" `
    ..\..
