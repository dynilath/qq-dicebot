$startloc = Resolve-Path .

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

Write-Host "Building denpendecies..."
Set-Location -Path $VCPKG_RT
.\vcpkg install sqlite3 gtest
Set-Location -Path $startloc
Write-Host "Building of dependencies completed." -ForegroundColor Green