# 端到端测试脚本（T-036 基础框架）
# 用法: ./scripts/run_e2e_test.ps1 <test_case_dir> <pascc_exe_path>

param(
    [string]$TestDir = ".\tests\e2e_cases",
    [string]$PasccExe = ".\build_fresh\Debug\pascc.exe"
)

function Write-Color {
    param([string]$Text, [string]$Color = "White")
    $colors = @{Green = "Green"; Red = "Red"; Yellow = "Yellow"; Cyan = "Cyan"}
    $host.ui.Write($colors[$Color], $host.ui.RawUI.ForegroundColor, $Text)
}

function Test-Compilation {
    param([string]$PascalFile, [string]$PasccExe)
    
    Write-Host "`n--- 测试: $(Split-Path $PascalFile -Leaf) ---" -ForegroundColor Cyan
    
    # 1. 运行 pascc
    $pasccArgs = @("-i", $PascalFile)
    Write-Host "执行: $PasccExe $pasccArgs"
    $pasccResult = & $PasccExe $pasccArgs 2>&1
    $exitCode = $LASTEXITCODE
    
    if ($exitCode -ne 0) {
        Write-Color "  ✗ pascc 失败 (退出码: $exitCode)`n" "Red"
        Write-Host $pasccResult -ForegroundColor Red
        return $false
    }
    
    # 2. 检查生成的 C 文件
    $cFile = [System.IO.Path]::ChangeExtension($PascalFile, ".c")
    if (-not (Test-Path $cFile)) {
        Write-Color "  ✗ 未找到生成的 C 文件: $cFile" "Red"
        return $false
    }
    
    $cContent = Get-Content $cFile -Raw
    if (-not $cContent -or $cContent.Trim().Length -eq 0) {
        Write-Color "  ✗ 生成的 C 文件为空" "Red"
        return $false
    }
    
    # 3. 尝试编译 C 文件
    $objFile = "$env:TEMP\temp_e2e_$(Get-Random).obj"
    $compileArgs = @("/c", "/nologo", "/W1", "/Od", "/Zi", "/Fe", $objFile, $cFile)
    Write-Host "编译 C: cl $compileArgs"
    $compileResult = & cl @compileArgs 2>&1
    $compileExit = $LASTEXITCODE
    
    if ($compileExit -ne 0) {
        Write-Color "  ✗ C 文件编译失败 (cl 退出码: $compileExit)`n" "Red"
        Write-Host $compileResult -ForegroundColor Red
        Remove-Item $objFile -ErrorAction SilentlyContinue
        return $false
    }
    
    Remove-Item $objFile -ErrorAction SilentlyContinue
    
    # 4. 基本语法检查
    $requiredPatterns = @(
        "#include <stdio\.h>",
        "int main\(void\)",
        "return 0;"
    )
    
    foreach ($pattern in $requiredPatterns) {
        if ($cContent -notmatch $pattern) {
            Write-Color "  ⚠ 缺少模式: $pattern" "Yellow"
        }
    }
    
    Write-Color "  ✓ 通过 (生成 + 编译成功)" "Green"
    return $true
}

# 主流程
Write-Host "`n=== Pascal-S 编译器端到端测试 (T-036 框架) ===" -ForegroundColor Green
Write-Host "测试目录: $TestDir"
Write-Host "编译器: $PasccExe`n"

if (-not (Test-Path $PasccExe)) {
    Write-Color "错误: 找不到 pascc 可执行文件: $PasccExe" "Red"
    exit 1
}

if (-not (Test-Path $TestDir)) {
    New-Item -ItemType Directory -Path $TestDir | Out-Null
    Write-Host "创建测试目录: $TestDir" -ForegroundColor Yellow
}

# 确保有测试用例
$testCases = @(
    "01_ok_simple.pas",
    "02_ok_with_procedure.pas",
    "03_lex_error.pas",
    "04_semantic_error.pas"
)

foreach ($testCase in $testCases) {
    $testPath = Join-Path $TestDir $testCase
    if (-not (Test-Path $testPath)) {
        # 创建默认测试文件
        switch ($testCase) {
            "01_ok_simple.pas" {
                @"
program simple;
var x: integer;
begin
  x := 42
end.
"@ | Out-File $testPath -Encoding UTF8
            }
            "02_ok_with_procedure.pas" {
                @"
program withproc;
procedure inc;
begin end;
begin
  inc
end.
"@ | Out-File $testPath -Encoding UTF8
            }
            "03_lex_error.pas" {
                @"
program lexerr;
var x: integer;
begin
  x := @  // 非法字符
end.
"@ | Out-File $testPath -Encoding UTF8
            }
            "04_semantic_error.pas" {
                @"
program semerr;
var x: integer;
begin
  y := 1  // 未定义变量
end.
"@ | Out-File $testPath -Encoding UTF8
            }
        }
        Write-Host "创建测试用例: $testCase" -ForegroundColor Yellow
    }
}

# 运行测试
$passed = 0
$failed = 0
$total = 0

Get-ChildItem $TestDir -Filter "*.pas" | ForEach-Object {
    $total++
    if (Test-Compilation $_.FullName $PasccExe) {
        $passed++
    } else {
        $failed++
    }
}

# 结果汇总
Write-Host "`n=== 测试结果汇总 ===" -ForegroundColor Cyan
Write-Host "总测试数: $total"
Write-Host "通过: $passed" -ForegroundColor Green
Write-Host "失败: $failed" -ForegroundColor $(if ($failed -gt 0) { "Red" } else { "Green" })
Write-Host "成功率: $([math]::Round($passed/$total*100, 1))%`n"

exit $(if ($failed -eq 0) { 0 } else { 1 })
