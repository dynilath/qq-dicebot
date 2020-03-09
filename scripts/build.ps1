$startloc = Resolve-Path .

$config_type = $args[0]

Set-Location $PSScriptRoot\..\build  # enter the parent folder
cmake --build . --config $config_type  # build the project

Set-Location $startloc