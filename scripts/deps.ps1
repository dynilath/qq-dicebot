$startloc = $PSScriptRoot

$VCPKG_RT = ${env:VCPKG_ROOT}
if ($null -eq $VCPKG_RT) {
	Write-Warning "Could not find VCPKG_ROOT. Please set up vcpkg environment."
	exit
}

$VCPKG_EXE = "$VCPKG_RT\vcpkg.exe"
if (!(Test-Path $VCPKG_EXE)) {
	Write-Warning "Invalid VCPKG_ROOT. Please check vcpkg environment."
	exit
}

Write-Host "Building denpendecies..."
Set-Location -Path $VCPKG_RT
.\vcpkg install sqlite3 gtest --triplet "x86-windows-static"
Set-Location -Path $startloc
Write-Host "Building of dependencies completed." -ForegroundColor Green