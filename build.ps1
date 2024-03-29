[CmdletBinding()]
Param(
    [ValidateSet('All', 'Vs2019', 'Vs2022')]
    [String] $VisualStudioVersion = 'Vs2022',
    [ValidateSet('All', 'Static', 'Dynamic')]
    [String] $Runtime = 'All',
    [ValidateSet('All', 'Debug', 'Release')]
    [String] $Config = 'All',
    [ValidateSet('All', 'Win32', 'x64')]
    [String] $Platform = 'All',
    [switch] $Package,
    [switch] $VerboseBuild
)

$cmake = 'cmake.exe'
$package_dir = 'package'

function Invoke-CMakeGenerator() {
    [CmdletBinding()]
    Param(
        [ValidateSet('Vs2019', 'Vs2022')]
        [String] $VisualStudioVersion,
        [ValidateSet('Static', 'Dynamic')]
        [String] $Runtime,
        [ValidateSet('Debug', 'Release')]
        [String] $Config,
        [ValidateSet('Win32', 'x64')]
        [String] $Platform,
        [switch] $VerboseBuild
    )

    $Runtime = $Runtime.ToLower()
    $Directory = 'build_' + $VisualStudioVersion + '_' + $Platform + '_' + $Runtime
    if ($VisualStudioVersion -eq 'Vs2019') {
        $Generator = 'Visual Studio 16 2019'
    }
    elseif ($VisualStudioVersion -eq 'Vs2022') {
        $Generator = 'Visual Studio 17 2022'
    }

    # Configure build projects using CMake
    if (!(Test-Path $Directory)) {
        New-Item -ItemType Directory -Path $Directory | Out-Null
    }

    Push-Location $Directory

    Write-Output "$cmake -G $Generator -A $Platform "-DMSVC_RUNTIME=$Runtime" .."
    & $cmake -G $Generator -A $Platform "-DMSVC_RUNTIME=$Runtime" .. 

    Pop-Location
    if ($LastExitCode -ne 0) { exit -1 }

    Write-Output "$cmake --build $Directory --config $Config"
    if ($VerboseBuild) {
        & $cmake --build $Directory --config $Config --verbose
    }
    else {
        & $cmake --build $Directory --config $Config
    }
    if ($LastExitCode -ne 0) { exit -1 }

    # Copy output somewhere interesting for packaging
    if ($Package) {
        $debugPostfix = ''
        if ($Config -eq 'Debug') {
            $debugPostfix = '-debug'
        }

        $vsver = $VisualStudioVersion.ToLower()
        $package_name = "win32cpp-$vsver-$Platform-$Runtime$debugPostfix"

        $inc_out_dir = "$package_dir\\$package_name\\include\\win32cpp"
        New-Item -Type Directory -Path $inc_out_dir | Out-Null
        Copy-Item -Path 'src\lib\*.h' -Destination $inc_out_dir -Exclude 'pch.h'

        $lib_out_dir = "$package_dir\\$package_name\\lib"
        New-Item -Type Directory -Path $lib_out_dir | Out-Null
        Copy-Item -Path "$Directory\\src\\lib\\$Config\\*" -Destination $lib_out_dir

        $package_filename = "$package_dir\\$package_name.zip"
        Compress-Archive -Path "$package_dir\\$package_name\\*" -DestinationPath $package_filename
    }
}

if ($VisualStudioVersion -eq 'All') {
    $VisualStudioVersions = 'Vs2019', 'Vs2022'
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

$timer = [Diagnostics.Stopwatch]::StartNew()

if ($Package) {
    if (Test-Path $package_dir) {
        Remove-Item -Recurse $package_dir | Out-Null
    }

    New-Item -ItemType Directory -Path $package_dir | Out-Null
}

foreach ($v in $VisualStudioVersions) {
    foreach ($r in $Runtimes) {
        foreach ($p in $Platforms) {
            foreach ($c in $Configs) {
                Invoke-CMakeGenerator -VisualStudioVersion $VisualStudioVersion -Runtime $r -Platform $p -Config $c -VerboseBuild:$VerboseBuild
            }
        }
    }
}

$timer.Stop()
Write-Output ('Total Build time: ' + $timer.Elapsed)
