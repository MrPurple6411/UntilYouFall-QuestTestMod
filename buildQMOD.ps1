Param(
    [String] $qmodname = "questtestmod",

    [Parameter(Mandatory = $false)]
    [Switch] $clean,

    [Parameter(Mandatory = $false)]
    [Switch] $help
)

if ($help -eq $true) {
    Write-Output "`"BuildQmod <qmodName>`" - Copies your mod into a `".so`" or a `".a`" library"
    Write-Output "`n-- Parameters --`n"
    Write-Output "qmodName `t The file name of your qmod"

    Write-Output "`n-- Arguments --`n"

    Write-Output "-Clean `t`t Performs a clean build on both your library and the qmod"

    exit
}

if ($qmodName -eq "") {
    Write-Output "Give a proper qmod name and try again"
    exit
}

& $PSScriptRoot/build.ps1 -clean:$clean

if ($LASTEXITCODE -ne 0) {
    Write-Output "Failed to build, exiting..."
    exit
}

Write-Output "Creating qmod from mod.json"

$mod = "./mod.json"
$modJson = Get-Content $mod -Raw | ConvertFrom-Json

$filelist = @($mod)

$cover = "./" + $modJson.coverImage
if ((-not ($cover -eq "./")) -and (Test-Path $cover)) { 
    $filelist += , $cover
}

foreach ($mod in $modJson.modFiles) {
    $path = "./build/" + $mod
    if (-not (Test-Path $path)) {
        $path = "./extern/libs/" + $mod
    }
    $filelist += $path
}

foreach ($lib in $modJson.libraryFiles) {
    $path = "./extern/libs/" + $lib
    if (-not (Test-Path $path)) {
        $path = "./build/" + $lib
    }
    $filelist += $path
}

$zip = $qmodName + ".zip"
$qmod = $qmodName + ".qmod"

if ((-not ($clean.IsPresent)) -and (Test-Path $qmod)) {
    Write-Output "Making Clean Qmod"
    Move-Item $qmod $zip -Force
}

Compress-Archive -Path $filelist -DestinationPath $zip -Update
Move-Item $zip $qmod -Force