[CmdletBinding()]
Param(
    [ValidateSet('All', 'Static', 'Dynamic')]
    [String] $Runtime = 'All',
    [ValidateSet('All', 'Debug', 'Release')]
    [String] $Config = 'All',
    [ValidateSet('All', 'Win32', 'x64')]
    [String] $Platform = 'All'
    )

$cmake = '\dev\utils\cmake\bin\cmake.exe'

function Invoke-CMakeGenerator(
    [ValidateSet('Static', 'Dynamic')]
    [String] $Runtime,
    [ValidateSet('Debug', 'Release')]
    [String] $Config,
    [ValidateSet('Win32', 'x64')]
    [String] $Platform
    )
{
    if ($Platform -eq 'Win32')
    {
        $Generator = 'Visual Studio 14 2015'
        $Directory = 'build32_' + $Runtime
    }
    else
    {
        $Generator = 'Visual Studio 14 2015 Win64'
        $Directory = 'build64_' + $Runtime
    }

    # Configure build projects using CMake
    if (!(Test-Path $Directory))
    {
        New-Item $Directory
    }

    Push-Location $Directory
    & $cmake -G $Generator -DMSVC_RUNTIME=$Runtime ..
    Pop-Location

    & $cmake --build $Directory --config $Config
}

if ($Runtime -eq 'All')
{
    $Runtimes = 'Static','Dynamic'
}
else
{
    $Runtimes = @($Runtime)
}

if ($Platform -eq 'All')
{
    $Platforms = 'Win32','x64'
}
else
{
    $Platforms = @($Platform)
}

if ($Config -eq 'All')
{
    $Configs = 'Debug','Release'
}
else
{
    $Configs = @($Config)
}

foreach ($r in $Runtimes)
{
    foreach ($p in $Platforms)
    {
        foreach ($c in $Configs)
        {
            Invoke-CMakeGenerator -Runtime $r -Platform $p -Config $c
        }
    }
}
