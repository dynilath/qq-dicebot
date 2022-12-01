$config_type = $args[0]

$start_location = Resolve-Path .

if ($null -eq $config_type) {
    Write-Warning "No configuration specified, using Debug as default, if you want release, use `"bootstrap.ps1 Release`" instead."
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

$target_triplet = "x64-windows-static-md"
$target_arch = "x64"

function Test-And-Install-Packages {
    param (
        $_module_name
    )
    $_start_loc = Resolve-Path .
    Set-Location $VCPKG_RT

    Write-Host "Checking $_module_name"

    $_val = (& .\vcpkg list $_module_name)
    $module_good = $false
    if ($_val -match 'No packages are installed') {
        $module_good = $false
    }
    else {
        foreach ($_ in $_val) {
            if ($_.SubString($_.IndexOf(":") + 1, $_.IndexOf(" ")).Trim() -match "^$target_triplet$") {
                $module_good = $true
                break
            }
        }
    }

    if (-not $module_good) {

        Write-Host "run .\vcpkg install $_module_name --triplet $target_triplet"

        (& .\vcpkg install $_module_name --triplet $target_triplet)

        $_val = (& .\vcpkg list $_module_name)
        if ($null -eq $_val -or $_val -match 'No packages are installed') {
            $module_good = $false
        }
        else {
            foreach ($_ in $_val) {
                if ($_.SubString($_.IndexOf(":") + 1, $_.IndexOf(" ")).Trim() -match "^$target_triplet$") {
                    $module_good = $true
                    break
                }
            }
        }
    }

    if ($module_good) {
        Write-Host "$_module_name is installed" 
    }
    else {
        Write-Host  "$_module_name is not installed" -ForegroundColor Red 
    }
    Set-Location $_start_loc
    return $module_good
}

Write-Host "Checking denpendecies..." -ForegroundColor Green
if ( -not (Test-And-Install-Packages("sqlite3"))) {
    exit
}
if ( -not (Test-And-Install-Packages("gtest"))) {
    exit
}
Write-Host "Dependencies checked." -ForegroundColor Green

if (-not (Test-Path -Path ".\build")) {
    New-Item -Path ".\build" -ItemType "directory" | Out-Null
}
$build_dir = Resolve-Path ".\build"

Set-Location $build_dir

Write-Host "Configuration begins." -ForegroundColor Green

Write-Host "Env:OS is $Env:OS"
try {
    $is_windows = ($Env:OS -match "Win")
}
catch {
    $is_windows = $false
}

if ($is_windows) {
    Write-Host "Build for win..."
    cmake `
        -DCMAKE_TOOLCHAIN_FILE="$VCPKG_RT\scripts\buildsystems\vcpkg.cmake" `
        -DCMAKE_CONFIGURATION_TYPES="$config_type" `
        -DVCPKG_TARGET_TRIPLET="$target_triplet" `
        -DVCPKG_TARGET_ARCHITECTURE="$target_arch" `
        ..
}
else {
    Write-Host "Build for anything..."
    cmake `
        -DCMAKE_TOOLCHAIN_FILE="$VCPKG_RT\scripts\buildsystems\vcpkg.cmake" `
        -DCMAKE_BUILD_TYPE="$config_type" `
        -DVCPKG_TARGET_ARCHITECTURE="$target_arch" `
        ..
}

Write-Host "Build begins." -ForegroundColor Green
cmake --build .  --config $config_type

Set-Location $start_location

if ($is_windows) {
    $target_artifact = Resolve-Path "$build_dir/$config_type/dicebot.dll"
    $install_dir = Resolve-Path "."
    Copy-Item -LiteralPath $target_artifact -Destination $install_dir
}


