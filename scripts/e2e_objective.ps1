param(
    [string]$ProjectRoot = ".",
    [string]$BuildDir = "build_fresh",
    [string]$Config = "Debug",
    [switch]$SkipConfigure,
    [switch]$SkipBuild,
    [switch]$SkipCTest
)

$ErrorActionPreference = "Stop"

function Write-Stage {
    param([string]$Name)
    Write-Host "`n=== $Name ===" -ForegroundColor Cyan
}

function Run-PasccCase {
    param(
        [string]$PasccExe,
        [string]$InputPas,
        [int]$ExpectExit,
        [bool]$ExpectGenerated
    )

    $fullPas = (Resolve-Path $InputPas).Path
    $outC = [System.IO.Path]::ChangeExtension($fullPas, ".c")
    if (Test-Path $outC) { Remove-Item $outC -Force }

    & $PasccExe -i $fullPas
    $code = $LASTEXITCODE
    if ($code -ne $ExpectExit) {
        throw "[FAIL] ExitCode mismatch. actual=$code expected=$ExpectExit file=$fullPas"
    }

    $exists = Test-Path $outC
    if ($ExpectGenerated -and -not $exists) {
        throw "[FAIL] Expected generated C missing: $outC"
    }
    if (-not $ExpectGenerated -and $exists) {
        throw "[FAIL] Expected no generated C but found: $outC"
    }
    return $outC
}

$root = (Resolve-Path $ProjectRoot).Path
$build = Join-Path $root $BuildDir
$pascc = Join-Path $build "$Config\pascc.exe"

Write-Stage "Objective E2E Start"
Write-Host "Root   : $root"
Write-Host "Build  : $build"
Write-Host "Config : $Config"

if (-not $SkipConfigure) {
    Write-Stage "Configure"
    cmake -S $root -B $build -G "Visual Studio 17 2022" -A x64
}

if (-not $SkipBuild) {
    Write-Stage "Build"
    cmake --build $build --config $Config
}

if (-not (Test-Path $pascc)) {
    throw "[FAIL] pascc not found: $pascc"
}

if (-not $SkipCTest) {
    Write-Stage "CTests"
    ctest --test-dir $build -C $Config --output-on-failure
    if ($LASTEXITCODE -ne 0) { throw "[FAIL] CTest failed." }
}

Write-Stage "Compiler E2E Cases"
$okC = Run-PasccCase -PasccExe $pascc -InputPas (Join-Path $root "samples\03_controlflow.pas") -ExpectExit 0 -ExpectGenerated $true
Run-PasccCase -PasccExe $pascc -InputPas (Join-Path $root "samples\02_lex_error.pas") -ExpectExit 7 -ExpectGenerated $false | Out-Null

$tmpDir = Join-Path $build "e2e_tmp"
if (-not (Test-Path $tmpDir)) { New-Item -ItemType Directory -Path $tmpDir | Out-Null }
$semPas = Join-Path $tmpDir "semantic_fail_case.pas"
@"
program demo;
procedure p(a: integer);
begin
end;
begin
  p(true)
end.
"@ | Set-Content -Path $semPas -Encoding ASCII
Run-PasccCase -PasccExe $pascc -InputPas $semPas -ExpectExit 9 -ExpectGenerated $false | Out-Null

Write-Stage "Generated C Compile Check"
$cl = Get-Command cl -ErrorAction SilentlyContinue
if ($null -ne $cl) {
    $exe = Join-Path $tmpDir "e2e_controlflow.exe"
    & cl /nologo ("/Fe" + $exe) $okC | Out-Null
    if ($LASTEXITCODE -ne 0) { throw "[FAIL] compile generated C failed: $okC" }
    & $exe | Out-Null
    if ($LASTEXITCODE -ne 0) { throw "[FAIL] generated exe run failed: $exe" }
    Write-Host "[PASS] cl compile+run check passed."
} else {
    Write-Host "[SKIP] cl not found, skip compile+run check." -ForegroundColor Yellow
}

Write-Stage "Objective E2E Summary"
Write-Host "[PASS] CTest regression"
Write-Host "[PASS] success path exit=0 and emits .c"
Write-Host "[PASS] lexical error path exit=7 and no .c"
Write-Host "[PASS] semantic error path exit=9 and no .c"
exit 0
