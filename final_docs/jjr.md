# pascc 编译器验收讲解笔记

> 讲解人：GitHub Copilot  
> 记录时间：2026年5月20日  
> 对应文档：01-总体架构设计.md / 02-词法分析模块.md / 03-语法分析模块.md / 04-语义分析模块.md

---

## 一、全局鸟瞰：四份文档的关系

项目 **pascc**（Pascal-S → C 编译器）是经典的四阶段编译器：

```
.pas 源文件
  ↓
① 词法分析 (Lexer)     → Token 流               【02 文档】
  ↓
② 语法分析 (Parser)    → AST 语法树              【03 文档】
  ↓
③ 语义分析 (Semantic)  → 类型检查 & 符号表       【04 文档】
  ↓
④ 代码生成 (CodeGen)   → .c 目标文件             【05 文档，不负责】
```

**01 文档**是把这些串起来的"总指挥"——CompilerDriver 流水线编排 + 模块间接口契约 + 核心数据结构。

---

## 二、01-总体架构设计

### 2.1 项目概述

> **pascc** = 把 Pascal-S（教学用简化 Pascal）源码翻译成 C 语言源码的编译器。

| 要素 | 要记住的 |
|------|---------|
| **语言** | C++17 为核心 |
| **词法工具** | Flex（WinFlexBison 版） |
| **语法工具** | Bison（WinFlexBison 版） |
| **构建系统** | CMake 3.16+ |
| **架构理念** | **模块独立性 + 流水线可编排性** |
| **四个阶段** | 词法 → 语法 → 语义 → 代码生成 |
| **总指挥** | `CompilerDriver` 统一编排调用顺序 |
| **阶段间数据传递** | `LexerResult` → `ParserResult` → `SemanticResult` → `CodegenResult` |

> 🎯 **验收讲解要点**：强调"每个模块可以独立测试、独立调试、独立演化"。

**验收一句话概括**：
> "我们的编译器 pascc 采用经典四阶段流水线架构，C++17 实现，Flex/Bison 做词法语法生成，CMake 管理构建，CompilerDriver 统一编排，四个阶段通过明确的 Result 结构体传递数据。"

---

### 2.2 编译流水线

```
.pas 源文件
  ↓  ① 词法分析 (Lexer)
Token 流 (vector<Token>)  +  词法错误列表
  ↓  ② 语法分析 (Parser)
AST 根节点 (ProgramNode)  +  语法错误列表
  ↓  ③ 语义分析 (Semantic)
类型检查通过/失败       +  语义错误列表
  ↓  ④ 代码生成 (CodeGen)
.c 目标文件
```

#### 阶段① 词法分析（Lexer）
| 项目 | 内容 |
|------|------|
| **输入** | 原始 Pascal-S 源代码字符串（`.pas` 文件全部内容读到内存） |
| **核心机制** | Flex 生成的 DFA（确定性有限自动机）逐字符扫描 |
| **输出** | `LexerResult`：含 `tokens`（`vector<Token>`）+ `errors`（`vector<LexError>`） |

> 🔑 关键词：**DFA 扫描**、**最长匹配优先**、**行号/列号追踪**

#### 阶段② 语法分析（Parser）
| 项目 | 内容 |
|------|------|
| **输入** | 阶段①产出的 `vector<Token>` |
| **核心机制** | Bison 生成的 LALR(1) 解析器，**自底向上移进-归约** |
| **特别之处** | 归约的**同时**同步构造 AST 节点 |
| **输出** | `ParserResult`：含 `root`（`shared_ptr<ProgramNode>`）+ `errors` |

> 🔑 关键词：**LALR(1)**、**移进-归约**、**%union**、**%left/%right 优先级**

#### 阶段③ 语义分析（Semantic）
| 项目 | 内容 |
|------|------|
| **输入** | Token 流 + 语法分析结果（`ParserResult`） |
| **核心机制** | **独立递归下降**遍历 Token 流（不依赖 AST），构建作用域栈 + 符号表 |
| **检查内容** | 声明前使用？重复定义？类型兼容？参数匹配？ |
| **输出** | `SemanticResult`：只含 `errors`，不产出新的中间表示 |

> 🔑 关键词：**独立于 AST**、**ScopeStack 作用域栈**、**TableSet 链式查找**

#### 阶段④ 代码生成（CodeGen）
| 项目 | 内容 |
|------|------|
| **输入** | 原始文件路径（内部自己重新调 Lexer！） |
| **核心机制** | 对 AST 做**两次递归下降扫描** |
| **输出** | `CodegenResult`：含 `cSource`（完整 C 源代码字符串） |

> ⚠️ **特别设计**：CodeGen 不接收 Driver 传来的 tokens，自己重新调 Lexer。代价是词法分析执行两次，但换来**模块完全解耦**。

#### CompilerDriver 总指挥流程

```
1. 检查输入文件扩展名是否为 .pas
2. 读取文件全部内容到内存字符串
3. 调 Lexer::tokenizeDetailed() → LexerResult
4. 调 Parser::parse() → ParserResult
5. 调 SemanticDeclarationAnalyzer::analyze() → SemanticResult
6. 调 CodeGenerator::generateTemplate() → CodegenResult
7. 写入输出 .c 文件
```

每步之间检查 `errors.empty()`，有错就终止并返回对应退出码（7=词法错, 8=语法错, 9=语义错, 10=代码生成错）。

**验收一句话**：
> "四个阶段像工厂流水线一样串行工作，CompilerDriver 是总调度，阶段间通过 Result 结构体传递数据，任何阶段出错立即终止并报告。"

---

### 2.3 模块划分与文件映射

项目遵循严格的 **include/（头文件）+ src/（实现文件）** 分离：

| 模块 | 头文件 (include/) | 实现文件 (src/) | 特殊文件 |
|------|-------------------|-----------------|----------|
| Token 定义 | `token.h` | — | — |
| 词法分析 | `lexer.h` | `lexer.cpp`（119行） | `lexer_flex.l`（203行，Flex源文件） |
| 语法分析 | `parser.h`, `ast.h` | `parser.cpp`（7行）, `parser_bison_bridge.cpp`（163行） | `parser_bison.y`（848行，Bison文法源文件） |
| 语义分析 | `semantic_declaration.h`, `semantic_tables.h`（261行） | `semantic_declaration.cpp`（992行） | — |
| 代码生成 | `code_generator.h` | `code_generator.cpp`（791行） | — |
| 编译驱动 | `compiler_driver.h` | `compiler_driver.cpp`（106行）, `main.cpp`（22行） | — |

#### 关键记忆点

- **Flex/Bison 是"生成层"**：`.l` / `.y` 文件通过 CMake 自定义命令编译生成 `.cpp` / `.h`，产物放 `build/` 目录
- **`semantic_declaration.cpp` 最胖**（992行），承载全部语义分析逻辑
- **`parser.cpp` 最瘦**（仅7行），就是一个委托：`Parser::parse()` → `parseWithBison()`
- **`parser_bison_bridge.cpp`** 是关键的"桥接层"——连接 Flex 的 C 世界和 Bison 的 C 世界，负责 Token 类型双向映射 + `yylval` 语义值注入

#### 三层架构

- **头文件层（include/）**：公开接口和数据结构的"契约层"，模块间通信全靠它
- **实现层（src/）**：具体逻辑，用匿名 `namespace {}` 隐藏内部实现
- **生成层（Flex/Bison）**：`.l` → `lexer_flex.cpp` + `.h`，`.y` → `parser_bison.tab.c` + `.tab.h`

---

### 2.4 核心数据结构总览

流水线中模块间通信的"通用语言"：

```
SourcePosition        （原子类型）
  ├─ line: int = 1
  └─ column: int = 1

Token                 （词法最小单元）
  ├─ type: TokenType
  ├─ lexeme: string    ← 已做 ASCII 小写化
  └─ pos: SourcePosition

LexError → LexerResult
  └─ tokens: vector<Token>  +  errors: vector<LexError>

ParseError → ParserResult
  └─ root: shared_ptr<ProgramNode>  +  errors: vector<ParseError>

SemanticError → SemanticResult
  └─ errors: vector<SemanticError>   ← 只输出错误，不产出中间表示

CodegenResult
  ├─ ok: bool
  ├─ message: string
  └─ cSource: string   ← 完整的 C 源代码
```

> 🎯 所有错误结构体（`LexError`、`ParseError`、`SemanticError`）共享 `code + message + pos` 三字段，但**独立定义而非继承**——刻意保持模块独立性。

---

### 2.5 核心数据流（完整调用链）

```
main.cpp
  └→ CompilerDriver::run(inputPath)
       ├─ 1. 检查 .pas 扩展名
       ├─ 2. 读文件到内存字符串
       ├─ 3. Lexer::tokenizeDetailed(source) → LexerResult
       ├─ 4. Parser::parse(tokens) → ParserResult
       ├─ 5. Semantic::analyze(tokens, parserResult) → SemanticResult
       ├─ 6. CodeGenerator::generateTemplate(inputPath) → CodegenResult
       └─ 7. 写 .c 文件 + 打印统计
```

---

### 2.6 模块间接口契约（四个关键设计点）

1. **Token 流只读传递**：Parser 和 Semantic 通过 `const vector<Token>&` 接收，不修改
2. **CodeGen 自给自足**：不接收 tokens，内部重新调 Lexer——代价是词法分析执行两次，换来**完全解耦**
3. **错误传播**：每个阶段在 `errors` 中累积，Driver 逐阶段检查 → 非空即终止
4. **Semantic 双输入**：同时需要 tokens 和 parserResult（验证语法已通过）

---

### 2.7 重要架构决策（四大决策）

| 决策 | 内容 |
|------|------|
| **① AST 中心化** | Bison 归约时同步构建完整 AST，代码生成器遍历 AST 翻译 |
| **② AST 完整实现** | `ast.h` 283行，`AstKind` 枚举 32 种取值，覆盖全部语言构造 |
| **③ 语义分析独立工作** | 不遍历 AST，自己维护 Token 游标递归下降，独立符号表体系 |
| **④ 代码生成"直译"策略** | 逐 AST 节点翻译为等效 C 语法（如 `for i:=A to B` → `for(i=A;i<=B;++i)`） |

---

### 2.8 技术栈速查

| 技术 | 用途 | 版本 |
|------|------|------|
| C++17 | 核心语言（`if constexpr`、结构化绑定等） | C++17 |
| CMake | 跨平台构建 + 12个CTest测试目标 | ≥3.16 |
| Flex (win_flex) | 词法生成器，DFA扫描 | 2.5+ |
| Bison (win_bison) | 语法生成器，LALR(1)解析 | 2.5+ |
| CTest | 测试框架 | CMake内置 |


