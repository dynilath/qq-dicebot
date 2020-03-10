$startloc = Resolve-Path .

$config_type = $args[0]

If (-not (Test-Path ".\CMakeLists.txt")) {
    Write-Warning "This script should be called from project root folder."
    exit
}

Set-Location $PSScriptRoot\..\build  # enter the parent folder
cmake --build . --config $config_type  # build the project

Set-Location $startloc