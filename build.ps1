[CmdletBinding()]
Param(
    [ValidateSet('All', 'Vs2015', 'Vs2017', 'Vs2019')]
    [String] $VisualStudioVersion = 'Vs2015',
    [ValidateSet('All', 'Static', 'Dynamic')]
    [String] $Runtime = 'All',
    [ValidateSet('All', 'Debug', 'Release')]
    [String] $Config = 'All',
    [ValidateSet('All', 'Win32', 'x64')]
    [String] $Platform = 'All',
    [switch] $Package
)

$cmake = 'cmake.exe'
$package_dir = 'package'

function Invoke-CMakeGenerator(
    [ValidateSet('Vs2015', 'Vs2017', 'Vs2019')]
    [String] $VisualStudioVersion,
    [ValidateSet('Static', 'Dynamic')]
    [String] $Runtime,
    [ValidateSet('Debug', 'Release')]
    [String] $Config,
    [ValidateSet('Win32', 'x64')]
    [String] $Platform
) {
    $Runtime = $Runtime.ToLower()
    $Directory = 'build_' + $VisualStudioVersion + '_' + $Platform + '_' + $Runtime
    if ($VisualStudioVersion -eq 'Vs2015') {
        $Generator = 'Visual Studio 14 2015'
    }
    elseif ($VisualStudioVersion -eq 'Vs2017') {
        $Generator = 'Visual Studio 15 2017'
    }
    elseif ($VisualStudioVersion -eq 'Vs2019') {
        $Generator = 'Visual Studio 16 2019'
    }

    if ($Platform -eq 'x64' -and $VisualStudioVersion -in ('Vs2015', 'Vs2017')) {
        $Generator = $Generator + ' Win64'
    }

    # Configure build projects using CMake
    if (!(Test-Path $Directory)) {
        New-Item -ItemType Directory -Path $Directory | Out-Null
    }

    Push-Location $Directory

    if ($VisualStudioVersion -in ('Vs2015', 'Vs2017')) {
        Write-Output "$cmake -G $Generator "-DMSVC_RUNTIME=$Runtime" .."
        & $cmake -G $Generator "-DMSVC_RUNTIME=$Runtime" ..
    }
    else {
        Write-Output "$cmake -G $Generator -A $Platform "-DMSVC_RUNTIME=$Runtime" .."
        & $cmake -G $Generator -A $Platform "-DMSVC_RUNTIME=$Runtime" .. 
    }

    Pop-Location
    if ($LastExitCode -ne 0) { exit -1 }

    Write-Output "$cmake --build $Directory --config $Config"
    & $cmake --build $Directory --config $Config
    if ($LastExitCode -ne 0) { exit -1 }

    # Copy output somewhere interesting for packaging
    if ($Package) {
        $lib_dir = $package_dir + '\lib'
        if ($Runtime -eq 'static') {
            $lib_dir += '\crt'
        }
        else {
            $lib_dir += '\crtdll'
        }
        $lib_dir += '\' + $Platform + '\' + $Config

        New-Item -ItemType Directory -Path $lib_dir | Out-Null
        Copy-Item -Path ($Directory + "\\src\\lib\\$Config\\*") -Destination $lib_dir
    }
}

if ($VisualStudioVersion -eq 'All') {
    $VisualStudioVersions = 'Vs2015', 'Vs2017', 'Vs2019'
}
else {
    $VisualStudioVersions = @($VisualStudioVersion)
}

if ($Runtime -eq 'All') {
    $Runtimes = 'Static', 'Dynamic'
}
else {
    $Runtimes = @($Runtime)
}

if ($Platform -eq 'All') {
    $Platforms = 'Win32', 'x64'
}
else {
    $Platforms = @($Platform)
}

if ($Config -eq 'All') {
    $Configs = 'Debug', 'Release'
}
else {
    $Configs = @($Config)
}

if ($Package) {
    if (Test-Path $package_dir) {
        Remove-Item -Recurse $package_dir | Out-Null
    }

    # Copy headers to 'package\include\win32cpp'
    $include_dir = $package_dir + '\include\win32cpp'
    New-Item -ItemType Directory -Path $include_dir | Out-Null
    Copy-Item -Path 'src\lib\*.h' -Destination $include_dir -Exclude 'pch.h'
}

foreach ($v in $VisualStudioVersions) {
    foreach ($r in $Runtimes) {
        foreach ($p in $Platforms) {
            foreach ($c in $Configs) {
                Invoke-CMakeGenerator -VisualStudioVersion $VisualStudioVersion -Runtime $r -Platform $p -Config $c
            }
        }
    }
}