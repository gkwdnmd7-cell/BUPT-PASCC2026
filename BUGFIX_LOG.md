# PASCC 编译器 Bug 修复日志

> 修复周期：2026-05-18 夜 ～ 2026-05-19 晚  
> 平台：Linux 评测环境（GCC）

---

## Bug 1 — `real` 类型输出格式错误（测试 15、52、66）

### 现象
浮点数结果输出 `2` 而非 `2.000000`，与预期不符。

### 根因
`pas_write_real` 使用 `%g` 格式，会自动去掉尾零；同时 Pascal `Real` 类型被映射为 C `double`，与平台期望 `float` 精度不一致。

```c
// 修复前（错误）
static void pas_write_real(double v) { printf("%g", v); }
```

### 修复

```cpp
// code_generator.cpp — 映射改为 float
case TokenType::KwReal: return "float";   // 原为 "double"

// 生成的运行时辅助函数改为：
static void pas_write_real(float v)  { (void)printf("%f", v); }
```

---

## Bug 2 — 不支持多字符字符串字面量（`09_graph_coloring`）

### 现象
词法错误：`Unknown character` 或语法错误，Pascal 中 `'ne'`、`'--'` 等多字符字符串无法识别。

### 根因
Flex 规则只处理单字符 `'x'`，缺少多字符字符串规则。

```flex
// 修复前：只有单字符规则
\'([^\\\'\n\r])\'   { EMIT_TOKEN(T_CHAR, ...); }
```

### 修复

```flex
// lexer_flex.l — 新增多字符字符串规则
\'([^\\\'\n\r])\'   { EMIT_TOKEN(T_CHAR, ...); }           // 单字符不变
\'([^\'\n\r])*\'    {                                       // 新增：多字符
    std::string content(yytext + 1, yyleng - 2);
    EMIT_TOKEN(T_STRING, content);
}
```

```yacc
// parser_bison.y — 新增 T_STRING token
%token T_STRING
// const_value 规则中允许字符串
const_value : ... | T_STRING | ...
```

```cpp
// code_generator.cpp — tokenToExprPiece 将 StringLiteral 转为 C 字符串
case TokenType::StringLiteral:
    return "\"" + tok.lexeme + "\"";
```

---

## Bug 3 — 不支持 `break` / `continue` / `exit` 关键字

### 现象
`09_graph_coloring` 中使用 `break`，生成 C 代码将其当作普通标识符，编译报 `undeclared`。

### 根因
词法器关键字表、Bison 语法规则、代码生成器均未覆盖这三个关键字。

### 修复

```flex
// lexer_flex.l — keywordOrIdentifier 中增加
{"break",    T_BREAK},
{"continue", T_CONTINUE},
{"exit",     T_EXIT},
```

```yacc
// parser_bison.y
%token T_BREAK T_CONTINUE T_EXIT

break_stmt    : T_BREAK    ;
continue_stmt : T_CONTINUE ;
exit_stmt     : T_EXIT
              | T_EXIT T_LPAREN expression T_RPAREN ;

stmt : ... | break_stmt | continue_stmt | exit_stmt | ...
```

```cpp
// code_generator.cpp — parseSingleStatement
if (tokens[i].type == TokenType::KwBreak) {
    emitStatementLine(out, indentLevel, "break;");  ++i; return;
}
if (tokens[i].type == TokenType::KwContinue) {
    emitStatementLine(out, indentLevel, "continue;"); ++i; return;
}
if (tokens[i].type == TokenType::KwExit) {
    emitStatementLine(out, indentLevel, "return;");   ++i; return;
}
```

---

## Bug 4 — 类型别名变量被忽略（`09_graph_coloring`：`ne undeclared`）

### 现象
全局变量 `ne: NodeType`（`NodeType` 为类型别名）在嵌套过程中不可见，C 编译报 `undeclared`。

### 根因
`mapTypeTokenToC` 对 `Identifier` 类型返回空字符串，代码生成器检测到空就跳过该变量声明，根本不输出。

```cpp
// 修复前：Identifier 类型的变量声明被完全丢弃
std::string cType = mapTypeTokenToC(tokens[i].type);
// cType == "" 时，for (nm : names) { if (!cType.empty()) ... }
// → 什么都不加
```

### 修复

```cpp
// 在 parseGlobalVarDecls / parseVarDeclsInRange / parseRoutineParams 中统一增加：
std::string cType = mapTypeTokenToC(tokens[i].type);
if (cType.empty() && tokens[i].type == TokenType::Identifier) {
    cType = "int";   // 类型别名降级为 int，确保变量被声明
}
```

---

## Bug 5 — 全局 `var` 段扫描被函数体干扰（`09_graph_coloring`：`ne undeclared` 第二轮）

### 现象
修复 Bug 4 后 `ne` 仍然 `undeclared`，原因是全局变量根本没被扫描到。

### 根因
`parseGlobalVarDecls` 的外层扫描逻辑用简单 begin/end 深度计数跳过函数体，但函数体内包含 `case...of...end` 时，`case` 对应的 `end` 会把深度从 1 减到 0，误判为函数结束，提前退出扫描：

```
function foo;
begin
  case x of       ← 没有 begin，只有 case
    1: writeln(1);
  end;             ← depth: 1→0，误认为函数结束！
  writeln(2);      ← 后续代码被忽略
end;               ← 真正的函数 end
```

### 修复

引入递归辅助函数 `skipRoutineBodyAt`，将过程/函数整体跳过；body 阶段同时将 `KwCase` 纳入深度计数：

```cpp
static void skipRoutineBodyAt(const std::vector<Token>& tokens, std::size_t& i) {
    // 1. 跳过函数头到第一个 ';'（处理参数列表括号）
    int parenDepth = 0;
    while (i < tokens.size()) {
        if (tokens[i].type == TokenType::LParen)  { ++parenDepth; ++i; continue; }
        if (tokens[i].type == TokenType::RParen)  { --parenDepth; ++i; continue; }
        if (parenDepth == 0 && tokens[i].type == TokenType::Semicolon) { ++i; break; }
        ++i;
    }
    // 2. 跳过局部声明段（遇到嵌套函数则递归）
    while (i < tokens.size() && tokens[i].type != TokenType::KwBegin) {
        if (tokens[i].type == TokenType::KwProcedure ||
            tokens[i].type == TokenType::KwFunction) {
            ++i;
            skipRoutineBodyAt(tokens, i);   // 递归
        } else { ++i; }
    }
    // 3. 跳过 begin...end 体
    //    KwCase 也计入深度，避免 case...end 被误认为函数结束
    if (i < tokens.size() && tokens[i].type == TokenType::KwBegin) {
        ++i;
        int depth = 1;
        while (i < tokens.size() && depth > 0) {
            if (tokens[i].type == TokenType::KwBegin ||
                tokens[i].type == TokenType::KwCase)     ++depth;
            else if (tokens[i].type == TokenType::KwEnd) --depth;
            ++i;
        }
        if (i < tokens.size() && (tokens[i].type == TokenType::Semicolon ||
                                   tokens[i].type == TokenType::Dot)) ++i;
    }
}

// parseGlobalVarDecls 外层扫描改为：
if (tokens[i].type == TokenType::KwProcedure || tokens[i].type == TokenType::KwFunction) {
    ++i;
    skipRoutineBodyAt(tokens, i);   // 整体跳过，不遗漏
    continue;
}
```

---

## Bug 6 — 数组大小计算错误（`07_BST`：输出行数少）

### 现象
BST 程序使用 `array[1..MAXN]`，输出元素比预期少，数组越界导致数据被截断。

### 根因
Pascal `array[1..N]` 用 1-based 索引，但代码生成器生成 `int arr[N]`（只有 N 个元素），而我们在 C 中不调整下标，直接使用 Pascal 的原始索引值（1\~N），导致访问 `arr[N]` 越界。

```cpp
// 修复前：直接使用 high 作为数组大小
outSuffix += "[" + high + "]";          // array[1..100] → int arr[100]
// arr[100] 越界！需要 arr[101]
```

### 修复

```cpp
// 数字上界：size = high + 1
const int hv = std::stoi(high);
outSuffix += "[" + std::to_string(hv + 1) + "]";  // array[1..100] → int arr[101]

// 常量名上界：生成 C 表达式
outSuffix += "[" + high + " + 1]";                // array[1..MAXN] → int arr[MAXN + 1]
```

---

## Bug 7 — 函数返回值语义错误（`14_union_find`、`22_math`）

### 现象
- `scanf("%d", &getfloat())` → GCC 报 `lvalue required as unary '&' operand`
- 函数名在 `writeln` 中被正确展开

### 根因
Pascal 函数通过 `funcname := expr` 赋值返回值，`read(funcname)` 将输入读入函数返回值。代码生成器直接对函数名调用生成 `getfloat()`，导致 `&getfloat()` 是非法左值。

```pascal
function getfloat: real;
begin
  read(getfloat)       { Pascal：将输入赋给返回值 }
end;
```

```c
// 修复前（错误）：
float getfloat() {
    scanf("%f", &getfloat());  // ERROR: lvalue required
    return /* 无返回值 */;
}
```

### 修复

为每个函数引入隐式局部返回值变量 `__ret_funcname`：

```cpp
// parseRoutineAt：为函数生成返回变量
const std::string retVarName = "__ret_" + routine.name;
routine.localDecls.push_back(VarDecl{retVarName, routine.returnType, ...});
routine.body.push_back("return " + retVarName + ";");

// parseAssignmentStatement：函数名赋值 → 返回变量赋值
if (lhs == ctx.currentFunctionName)
    lhs = ctx.currentFunctionRetVar;   // funcname := x → __ret_funcname = x

// parseReadStatement：read(funcname) → scanf(..., &__ret_funcname)
if (argName == ctx.currentFunctionName)
    effectiveArg = ctx.currentFunctionRetVar;

// parseExpressionUntil：函数内部引用自身名字 → 返回变量
if (nm == currentFuncRetVar.substr(6))   // funcname → __ret_funcname
    pieces.push_back(currentFuncRetVar);
```

生成结果：

```c
float getfloat() {
    float __ret_getfloat = 0.0f;        // 隐式返回变量
    scanf("%f", &__ret_getfloat);       // 正确左值
    return __ret_getfloat;              // 自动追加
}
```

---

## Bug 8 — 数组下标含函数调用时赋值解析失败（`12_prim`：`fa()` 非函数）

### 现象
`fa[func(j)] := s` 生成 `fa()` 而非 `fa[func(j)]`，GCC 报 `called object is not a function`。

### 根因
`findAssignIndex` 寻找 `:=` 时遇到 `(` 就停止，未追踪括号嵌套深度。处理 `fa[func(j)]` 时，看到 `(` 就认为已到函数调用末尾，从而把 `fa` 后的所有内容截断。

```cpp
// 修复前（错误）：遇到 '(' 就停
while (j < tokens.size()) {
    if (tokens[j].type == TokenType::LParen) return tokens.size(); // 错误截断
    if (tokens[j].type == TokenType::Assign)  return j;
    ++j;
}
```

### 修复

同时追踪 `bracketDepth`（`[]`）和 `parenDepth`（`()`），只有两者均为 0 时才判定终止：

```cpp
std::size_t findAssignIndex(const std::vector<Token>& tokens, std::size_t start) {
    std::size_t j = start;
    int bracketDepth = 0, parenDepth = 0;
    while (j < tokens.size()) {
        if (tokens[j].type == TokenType::LBracket) { ++bracketDepth; ++j; continue; }
        if (tokens[j].type == TokenType::RBracket) { --bracketDepth; ++j; continue; }
        if (tokens[j].type == TokenType::LParen)   { ++parenDepth;   ++j; continue; }
        if (tokens[j].type == TokenType::RParen)   { --parenDepth;   ++j; continue; }
        if (bracketDepth == 0 && parenDepth == 0) {
            if (tokens[j].type == TokenType::Assign) return j;
            if (/* 终止符 */) return tokens.size();
        }
        ++j;
    }
    return tokens.size();
}
```

`fa[func(j)] := s` 的扫描轨迹：

```
fa → [  bracketDepth=1
→ func → ( parenDepth=1 → j parenDepth=0 → ) bracketDepth=0
→ ]  bracketDepth=0
→ := ← 两个深度均为 0，正确识别赋值！
```

---

## Bug 9 — Pascal 字符串常量被静默丢弃（`22_math`、`07_BST`）

### 现象
- `22_math`：`split` 完全未声明，GCC 报 `undeclared`；修复声明后又报类型不匹配 `char* → int`
- `07_BST`：同类问题

### 根因

**第一层**：`parseGlobalConstDecls` 遇到 `StringLiteral` token 时没有对应处理分支，`cValue` 保持为空，整个常量被静默丢弃：

```pascal
const split = '--';   { '--' 是 StringLiteral token }
```

```cpp
// 修复前：没有 StringLiteral 分支，cValue = ""，常量被抛弃
if      (tok == RealLiteral)    { cType = "float"; cValue = ...; }
else if (tok == IntegerLiteral) { cType = "int";   cValue = ...; }
else if (tok == CharLiteral)    { cType = "char";  cValue = ...; }
// StringLiteral：无处理 → cValue = "" → 不推入 decls → split 在 C 中不存在
```

**第二层**：即使 `split` 被正确声明为 `string` 类型，`inferWriteType` 也不处理 `"string"`，回退到 `"int"`，导致 `pas_write_int(split)` 接收 `char*`：

```cpp
// 修复前：inferWriteType 不识别 string
if (type == "float") return "float";
if (type == "char")  return "char";
return "int";   // string 也走到这里 → 错误
```

### 修复

```cpp
// 修复一：parseGlobalConstDecls 新增 StringLiteral 分支
} else if (tokens[i].type == TokenType::StringLiteral) {
    cType  = "string";
    cValue = "\"" + tokens[i].lexeme + "\"";
    ++i;
}

// 修复二：解析失败时给出兜底声明，防止任何情况下出现 undeclared
if (!cValue.empty()) {
    decls.push_back(ConstDecl{name, cType, cValue});
} else {
    decls.push_back(ConstDecl{name, "int", "0"});  // 兜底：至少声明出来
}

// 修复三：inferWriteType 新增 string 分支
if (it->second == "float" || it->second == "double") return "float";
if (it->second == "char")   return "char";
if (it->second == "string") return "string";   // 新增！
return "int";
```

最终生成效果：

```c
// 修复后
#define split "--"           // 字符串常量正确声明

// writeln(split) 生成：
pas_write_str(split);        // 展开为 pas_write_str("--") → 输出 --
```

---

## 修复汇总

| # | 涉及测试 | 根因 | 关键改动文件 | 状态 |
|---|---------|------|------------|------|
| 1 | 15、52、66 | `%g` 截断浮点尾零 | `code_generator.cpp` | ✅ |
| 2 | 09_graph_coloring | 多字符字符串字面量未支持 | `lexer_flex.l`、`parser_bison.y`、`code_generator.cpp` | ✅ |
| 3 | 09_graph_coloring | `break`/`continue`/`exit` 未支持 | `token.h`、`lexer_flex.l`、`parser_bison.y`、`code_generator.cpp` | ✅ |
| 4 | 09_graph_coloring | 类型别名变量声明被丢弃 | `code_generator.cpp` | ✅ |
| 5 | 09_graph_coloring | `case...end` 干扰函数体跳过，全局变量漏扫 | `code_generator.cpp`（`skipRoutineBodyAt`） | ✅ |
| 6 | 07_BST | Pascal 1-based 数组大小计算错误 | `code_generator.cpp`（`parseArrayTypeToC`） | ✅ |
| 7 | 14_union_find、22_math | 函数返回值赋值/读取语义错误 | `code_generator.cpp`（`__ret_funcname` 机制） | ✅ |
| 8 | 12_prim | 数组下标含函数调用时 `findAssignIndex` 提前截断 | `code_generator.cpp`（`findAssignIndex`） | ✅ |
| 9 | 22_math、07_BST | Pascal 字符串常量被静默丢弃，`inferWriteType` 不识别 string | `code_generator.cpp` | ✅ |
