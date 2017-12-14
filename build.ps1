[CmdletBinding()]
Param(
    [ValidateSet('Vs2015', 'Vs2017')]
    [String] $VisualStudioVersion = 'Vs2015',
    [ValidateSet('All', 'Static', 'Dynamic')]
    [String] $Runtime = 'All',
    [ValidateSet('All', 'Debug', 'Release')]
    [String] $Config = 'All',
    [ValidateSet('All', 'Win32', 'x64')]
    [String] $Platform = 'All'
)

$cmake = 'cmake.exe'

function Invoke-CMakeGenerator(
    [ValidateSet('Vs2015', 'Vs2017')]
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

    if ($Platform -eq 'x64') {
        $Generator = $Generator + ' Win64'
    }

    # Configure build projects using CMake
    if (!(Test-Path $Directory)) {
        New-Item -ItemType Directory -Path $Directory | Out-Null
    }

    Push-Location $Directory
    Write-Output "$cmake -G $Generator "-DMSVC_RUNTIME=$Runtime" .."
    & $cmake -G $Generator "-DMSVC_RUNTIME=$Runtime" ..
    Pop-Location
    if ($LastExitCode -ne 0) { exit }

    Write-Output "$cmake --build $Directory --config $Config"
    & $cmake --build $Directory --config $Config
    if ($LastExitCode -ne 0) { exit }
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

foreach ($r in $Runtimes) {
    foreach ($p in $Platforms) {
        foreach ($c in $Configs) {
            Invoke-CMakeGenerator -VisualStudioVersion $VisualStudioVersion -Runtime $r -Platform $p -Config $c
        }
    }
}
