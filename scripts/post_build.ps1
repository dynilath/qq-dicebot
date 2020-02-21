$lib_name = $args[0]
$out_dir = $args[1]
$json_name = "${lib_name}.json"
$json_path = "${out_dir}\${json_name}"

Copy-Item -Force "$PSScriptRoot\..\${json_name}" $json_path

if ([System.IO.File]::Exists("$PSScriptRoot\install.ps1")) {
    powershell.exe -ExecutionPolicy Bypass -NoProfile -File "$PSScriptRoot\install.ps1" $args
}
