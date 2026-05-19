# pascc 项目详细设计 PPT 制作提示词

> 适用于任意支持 officecli 的 AI agent。将以下内容完整交付给 AI 即应产出无歧义、可复现的 28 页 .pptx。
> 输入文件：`00-项目详细设计文档.md`；模板文件：`北京邮电大学科技风格PPT模板.pptx`；配图目录：`picture/`

---

## 一、模板预分析（AI 必须先执行以下命令获取精确形状 ID）

```
officecli view "${TEMPLATE}" stats
officecli view "${TEMPLATE}" outline
officecli view "${TEMPLATE}" text
officecli get "${TEMPLATE}" '/slide[1]' --depth 2
officecli get "${TEMPLATE}" '/slide[2]' --depth 2
officecli get "${TEMPLATE}" '/slide[3]' --depth 2
officecli get "${TEMPLATE}" '/slide[4]' --depth 2
officecli get "${TEMPLATE}" '/slide[5]' --depth 2
officecli get "${TEMPLATE}" '/slide[6]' --depth 2
officecli get "${TEMPLATE}" '/slide[7]' --depth 2
officecli get "${TEMPLATE}" '/slide[8]' --depth 2
officecli get "${TEMPLATE}" '/slide[9]' --depth 2
officecli get "${TEMPLATE}" '/slide[10]' --depth 2
```

从此输出中提取每一页中**含有中文占位符文本的形状的 @id**，记录其 `font`、`size`、`color`、`x`、`y`、`width`、`height`。这些形状就是后续要修改文字的目标。装饰用的 Freeform（`geometry=custom` 的自由形）和 Group 内的纯色圆形**只读不改**。

模板设计常量（硬编码，不可偏离）：

| 属性 | 值 |
|---|---|
| 背景色 | `#F4F5FA`（所有幻灯片统一） |
| 中文正文字体 | `微软雅黑` |
| 英文标题数字字体 | `Montserrat Ultra-Bold`（大号数字如 "01""02"） |
| 英文副标题字体 | `Montserrat` |
| 代码/技术标注字体 | `Consolas` |
| 主色（标题/重点） | `#356AFF` |
| 辅助色（副标题/小字） | `#8497B0` |
| 正文深色 | `#333333` |
| 白色卡片文字 | `#FFFFFF` |
| 技术摘要底色 | `#EEF0FF` |
| 幻灯片尺寸 | 33.87 × 19.05 cm（16:9） |
| 章节分隔大数字 | 80pt, bold, `#356AFF`, x=3cm, y=3cm |
| 章节英文标题 | 36pt, bold, `#356AFF`, x=3cm, y=7.5cm |
| 章节副标题 | 18pt, `#8497B0`, x=3cm, y=10cm |
| 正文页标题 | 36pt, bold, `#356AFF`, x=2cm, y=1.5cm, width=25cm, height=2.2cm |
| 正文页副标题 | 18pt, `#8497B0`, x=2cm, y=4.2cm, width=28cm, height=1.5cm |
| 正文内容块 | 16pt, `#333333`, x=2cm, y=6.5cm, width=29cm, lineSpacing=1.5x |
| 技术摘要框 | 14pt, `#356AFF`, fill=`#EEF0FF`, Consolas |

---

## 二、操作流程总则

### 2.1 模板处理铁律
- **绝不删除、移动、修改**任何模板原有的 `Freeform`（自由形）、`Group`（装饰圆组）、`Connector`（连接线）、`Picture`（模板装饰图）。这些元素的 `geometry=custom`、`image=true`、`zorder` 等属性原封不动。
- 仅通过 `set` 命令修改文字形状的 `text` 属性。对于有多段落的形状（如封面汇报人信息有 `paragraph[1]` 和 `paragraph[2]`），使用 `text="第一行\n第二行"` 格式。
- 模板原有的 10 张图片保留，其缺失 alt text 属模板遗留问题，不去修改。

### 2.2 操作顺序（严格按此执行）
1. `cp "${TEMPLATE}" "${OUTPUT}"`
2. `officecli open "${OUTPUT}"`
3. 阶段 A：用 `set` + `find=/replace=` 修改第 1-9 页模板原有占位符文字
4. 阶段 B：用 `batch --input file.json` 创建 17 张新幻灯片（第 11-27 页，因模板原有 10 页）
5. 阶段 C：用 `batch --input file.json` 在新幻灯片上添加标题和正文形状
6. 阶段 D：用 `batch --input file.json` 在关键页插入 `picture/` 目录中的架构图
7. 阶段 E：用 `batch --input file.json` 添加演讲者备注
8. 阶段 F：`officecli move "${OUTPUT}" /slide[10] --after /slide[28]` 将原第 10 页（感谢观看）移至末尾
9. 阶段 G：`officecli view "${OUTPUT}" text | grep ...` 检查占位符清零
10. `officecli validate "${OUTPUT}"` + `officecli view "${OUTPUT}" issues`
11. `officecli close "${OUTPUT}"`

### 2.3 关键技术限制
- **officecli 1.0.94 在 Windows 下 `add slide` CLI 直接调用存在路径解析 bug**（报错 `Invalid path 'c:/program Files/git/': trailing '/' is not allowed.`），因此**所有 `add slide`、`add shape`、`add picture`、`add notes` 操作必须通过 batch JSON 文件 + `--input` 模式执行**。`set`/`query`/`get`/`view`/`move`/`validate` 无此限制，可 CLI 直接调用。
- `set /slide[N] --prop find=X --prop replace=Y` 会替换该页**所有**匹配文本。若同一页内同一占位符出现多次且需要不同替换内容，必须先 `query` 定位各形状的 `@id`，再逐个 `set /slide[N]/shape[@id=ID] --prop text="..."` 精确修改。
- 所有路径参数必须用单引号括起（`'/slide[1]'`），防止 shell glob 展开 `[N]`。
- JSON batch 文件中的中文字符串直接书写即可，无需转义；唯一需要转义的是 JSON 双引号（`"`→`\"`）和反斜杠（`\`→`\\`）。
- 多行文本在 JSON 中用 `\n` 表示换行。

---

## 三、逐页内容规格（28 页完整文字 + 技术细节）

### 第 1 页 — 封面

此页模板原有 6 个可见文本形状 + 5 个装饰 Group + 1 个 connector。仅修改以下文本形状：

**形状定位**：模板中大标题 shape（`size=108pt, color=#356AFF`）、副标题 shape（`size=54.95pt, color=#8497B0`）、关键词 shape（`size=24pt, color=#8497B0`）、汇报人 shape（`size=36pt, color=#356AFF`，含两个段落）。通过 `find=/replace=` 修改：

- 主标题 → `pascc 项目详细设计`
- 副标题 → `Pascal-S to C Compiler`
- 关键词 → `编译原理与技术课程设计 | 四阶段编译流水线 | C++17 + Flex + Bison`
- 汇报人（两段）→ `编译课设项目组` / `2026年5月`

**演讲者备注**（`add --type notes`）：`"Welcome to the pascc project detailed design presentation. pascc is a Pascal-S to C compiler built with C++17, Flex, and Bison for the Compiler Principles course design."`

---

### 第 2 页 — 目录（CONTENTS）

模板为四栏目录：左侧大标题"目录"（81pt）+ "CONTENTS"（27pt, Montserrat Ultra-Bold）+ 四个条目，每个由编号（01-04, 27pt, #356AFF）+ 章节标题（36pt bold, #356AFF）+ 章节描述（18pt, #8497B0）组成。

分别将四个条目的占位符"添加小标题"→ 章节标题、"请输入您的文本/文字是您思想的提炼"→ 章节描述，修改为：

| 编号 | 章节标题 | 章节描述 |
|------|---------|---------|
| 01 | `项目概述与架构设计` | `技术栈、模块划分、编译流水线、核心数据结构` |
| 02 | `词法分析与语法分析` | `Token体系、Flex状态机、Bison文法、AST节点体系` |
| 03 | `语义分析与代码生成` | `符号表体系、类型检查规则、AST遍历翻译、C代码发射` |
| 04 | `驱动、测试与示例` | `CompilerDriver流水线编排、错误处理、测试体系、端到端数据流` |

---

### 第 3 页 — 章节分隔页：第一章 项目概述

模板为章节分隔页，含大数字（80pt）、标题（36pt）、副标题（18pt）。修改占位符：

- 标题 → `01`
- 英文标题 → `Project Overview`
- 副标题 → `技术栈、模块划分与文件映射`

**演讲者备注**：`"Chapter 1 covers project overview: technology stack including C++17, CMake 3.16+, WinFlexBison 2.5+, and the modular file structure with strict include/src separation. Design philosophy: module independence and pipeline orchestration."`

---

### 第 4 页 — 技术栈与模块划分

模板为两栏图文布局。其中有两个标题位（均含"请输入标题"）、两个小标题位（均含"添加小标题"）、两个描述位（均含"请输入您的文本，文字是您思想的提炼"）、一个长正文位（含"单击此处输入您的正文..."）。

**步骤一**：用 `find=/replace=` 替换唯一文本：
- `find="请输入标题" replace="技术栈与模块划分"`（2 matched）
- `find="添加小标题" replace="C++17 + Flex + Bison"`（2 matched）
- `find="请输入您的文本，文字是您思想的提炼" replace="CMake 3.16+ 跨平台构建"`（2 matched）
- `find="单击此处输入您的正文……（完整长句）" replace="项目采用 C++17 为核心语言，集成 Flex 词法生成器和 Bison 语法生成器（WinFlexBison 工具链），使用 CMake 3.16+ 作为跨平台构建系统。四个编译阶段（词法分析、语法分析、语义分析、代码生成）各自作为独立模块运行，由 CompilerDriver 统一编排调用顺序，阶段之间通过明确的数据结构（LexerResult、ParserResult、SemanticResult、CodegenResult）传递信息和错误。"`

**步骤二**：左侧副标题 shape（第二个"添加小标题"被替换为"C++17 + Flex + Bison"的那个）和描述形状需要精确区分。查询 `query '/slide[4]/shape:contains("C++17")'` 和 `query '/slide[4]/shape:contains("CMake")'` 取得 shape @id，将位于不同 y 坐标的描述形状分别改为：
- 上半部描述 → `C++17 核心编程语言，使用 if constexpr、结构化绑定等特性。Flex 从 203 行 .l 文件生成 DFA 扫描器，Bison 从 848 行 .y 文件生成 LALR(1) 解析器。`
- 下半部描述 → `CMake 管理 Flex/Bison 自定义代码生成命令和 12 个 CTest 测试目标的注册。Linux 下通过 -static 生成可移植二进制。项目严格遵循 include/ 头文件与 src/ 实现分离的 C++ 工程规范。`

**步骤三**：存在一个副标题形状位于 `x≈27cm, y≈7cm`（`font=微软雅黑, size=30pt, color=#356AFF`），其文本被替换为了"C++17 + Flex + Bison"。将其改为 `模块独立性`。其下方的描述形状（`size=18pt, color=#8497B0`）改为 `流水线可编排性`。

---

### 第 5 页 — 编译流水线架构

模板为三列卡片布局，每列含标题（30pt）和描述（18pt）。两个页面主标题、一个副标题、一个长正文。

**步骤一**：`find=/replace=` 唯一文本：
- `find="请输入标题" replace="编译流水线架构"`（2 matched）
- `find="请输入文本" replace="四阶段编译流水线"`（1 matched）
- `find="单击此处输入您的正文……（完整长句）" replace="词法分析（Lexer）→ 语法分析（Parser）→ 语义分析（Semantic）→ 代码生成（CodeGen），各阶段由 CompilerDriver 统一编排，阶段之间通过明确的 Result 结构体传递信息和错误。输入 .pas 文件，输出同名 .c 文件，命令行：pascc -i <input.pas>。"`（1 matched）

**步骤二**：用 `query '/slide[5]/shape:contains("添加小标题")'` 找出三个卡片标题 shape（@id=8 y≈7.5cm, @id=13 y≈14.6cm, @id=18 y≈21.8cm），分别改为：
- `词法分析 Lexer`
- `语法分析 Parser`
- `语义分析 Semantic`

**步骤三**：`find="请输入您的文本，文字是您思想的提炼"` 会将三个卡片的描述全部替换为同一文本。随后用 `query` 找出描述形状（@id=9 y≈9.4cm, @id=14 y≈16.5cm, @id=19 y≈23.7cm），分别精确改为：
- @id=9：`Flex 生成的 DFA 扫描器，正则匹配 → Token 流（std::vector<Token>），维护 g_line/g_col 全局状态`
- @id=14：`Bison 生成的 LALR(1) 解析器，自底向上移进-归约分析，%union 12 种语义值类型，归约时同步构造 AST 节点`
- @id=19：`独立递归下降遍历 Token 流，构建 ScopeStack 作用域栈 + TableSet 链式查找，标识符声明前使用检测 + 类型兼容性检查`

**步骤四**：插入配图。在页面底部（x=2cm, y=12cm, width=15cm, height=6cm）添加 `picture/01-architecture-pipeline.png`，alt=`"Compilation Pipeline Architecture: Lexer→Parser→Semantic→CodeGen"`。

---

### 第 6 页 — 核心数据结构

模板为两栏图文布局。含两个标题、两个"请输入文本"副标题、一个正文。

**步骤一**：`find=/replace=`：
- `find="请输入标题" replace="核心数据结构"`（2 matched）
- `find="请输入文本" replace="模块间通信的通用语言"`（2 matched）
- `find="请输入您的文本，文字是您思想的提炼" replace="Token 流 · AST 节点 · 错误列表"`（1 matched）
- `find="单击此处输入您的正文……" replace="从 SourcePosition 原子类型（token.h:76-80，含 line/column 两个 int 字段）到 ProgramNode 顶层节点，五大类 AST 节点（Type/Expr/Stmt/Decl/辅助，共 32 种 AstKind + 20+ 节点类型）构成完整程序表示。各阶段 Result 结构体封装输出与错误信息：LexerResult{tokens+errors}、ParserResult{root+errors}、SemanticResult{errors}、CodegenResult{ok+message+cSource}。注意所有错误结构体共享 code+message+pos 字段模式，但独立定义——保持各模块完全独立性。"`

---

### 第 7 页 — 重要架构决策

模板为四列卡片布局，含两个主标题、一个长副标题、四个小标题、四个描述。

**步骤一**：`find=/replace=` 主标题：
- `find="请输入标题" replace="重要架构决策"`（2 matched）

**步骤二**：`find="请输入您的文本"` 会匹配副标题（30pt, #356AFF）和四个描述文字（18pt, #8497B0）。用 `query` 分别定位后逐个修改。

副标题 shape（@id=27, x≈4cm, y≈9cm, 30pt, #356AFF）改为：
- `Bison 构建 AST → 代码生成器遍历翻译`

四个卡片标题（@id=25 x≈5cm, @id=19 x≈16cm, @id=21 x≈27cm, @id=23 x≈37cm）分别改为：
- `AST 中心架构` → 描述：`Bison 解析器的每条产生式语义动作在归约时同步构造 AST 节点（BinaryExprNode/IfStmtNode/RoutineNode 等），最终在 program 起始规则归约时产出完整 ProgramNode 树。代码生成器通过 emitExpr/emitStmt 递归遍历 AST 翻译，语法验证与代码翻译通过 AST 接口完全解耦。`
- `独立语义分析` → 描述：`SemanticDeclarationAnalyzer（semantic_declaration.cpp 992 行）采用独立递归下降策略——自己维护 Token 流游标 pos_ 和 advance()/match()/peekType() 辅助函数，拥有独立的 ScopeStack + TableSet + BuiltinTypePool 体系，通过 ParserResult.root!=nullptr 验证语法通过但不访问 AST。`
- `直译代码生成` → 描述：`对 AST 做递归遍历，逐节点模式匹配翻译为等效 C 语法。类型映射：integer/boolean→int, real→float, char→char。控制流映射：for i:=A to B do→for(i=A;i<=B;++i), repeat S until C→do{S}while(!(C))。不再需要两遍 Token 扫描——类型信息全部从 AST 声明节点提取。`
- `Fail-Fast 错误策略` → 描述：`CompilerDriver 在每个阶段完成后立即检查 errors.empty()，若不为空则遍历错误列表通过 logutil::error() 逐条输出到 stderr，然后 return 对应退出码（7-10）。任一阶段失败，后续阶段均不执行——用户可一次性看到最早阶段的全部错误。`

---

### 第 8 页 — 错误处理与退出码体系

模板为两栏百分比卡片布局。含两个主标题、两个百分比数字（40%）、两个小标题、一个副标题、一个正文。

**步骤一**：`find=/replace=`：
- `find="请输入标题" replace="错误处理与退出码"`（2 matched）
- `find="40%" replace=" "`（清空百分比数字）
- `find="请输入您的文本" replace="分层错误追踪体系"`（1 matched）
- `find="文字是您思想的提炼" replace="E/S 两级错误码体系"`（1 matched）
- `find="Click to add a subtitle" replace="四阶段分层 + 11 种退出码"`（1 matched）

**步骤二**：正文 shape（长文本）的 find+replace 结果需要检查——模板正文可能未完全替换。query `shape:contains("单击此处")` 找到该 shape @id 后精确 set：
- 正文 → `四个错误码系列：E1xx（词法层：E106 嵌套注释、E107 未闭合注释、E109 未终止字符串、E110 未知字符）；E2xx（语法层：E201 Bison LALR(1) 解析冲突）；S1xx（语义声明层：S101 重定义、S102 未定义类型、S103 声明前使用、S199 防御性检查）；S2xx（语义类型层：S201 类型不兼容、S202 例程调用参数错误）。所有编译阶段的错误上报遵循统一三步模式：模块内部累积到 result.errors → Driver 检查 errors.empty() → 失败时逐条 logutil::error(code, "(line:col) " + message) 输出到 stderr。`

**步骤三**：两个卡片小标题 shape（@id=21 #356AFF 和 @id=18 #8497B0）分别改为：
- `Fail-Fast 策略`
- `统一日志输出`

---

### 第 9 页 — 测试体系与构建系统

模板为两大卡片 + 关键词条的布局。含两个主标题、两个卡片标题、两个卡片正文、三个关键词。

**步骤一**：`find=/replace=`：
- `find="请输入标题" replace="测试体系与构建系统"`（2 matched）
- `find="添加关键词" replace="CTest"`（3 matched）
- `find="请输入您的文本，文字是您思想的提炼" replace="12 个测试目标覆盖四层测试金字塔"`（1 matched）
- `find="Click to add a subtitle" replace="CMake + CTest + Flex/Bison 集成"`（3 matched）
- `find="单击此处输入您的正文……" replace="CMake 构建系统通过 add_custom_command() 管理 Flex/Bison 代码生成依赖链：parser_bison.y → parser_bison.tab.h → lexer_flex.l → lexer_flex.cpp。测试分四层：L1 单元测试（semantic_tableset_tests 测 TableSet::insert/searchEntry 链式查找、semantic_typepool_tests 测 BuiltinTypePool）；L2 词法/语法模块测试（lexer_smoke_tests, lexer_tests, parser_tests, bison_parser_tests）；L3 语义分析模块测试（semantic_declaration_tests, semantic_usage_tests, semantic_expression_tests, semantic_param_tests）；L4 端到端集成测试（semantic_gate_tests 完整调用 CompilerDriver::run(), codegen_template_tests 验证 samples/*.pas 的 C 翻译输出）。"`（3 matched）

**步骤二**：query 两个卡片小标题 shape，分别改为：
- `测试金字塔结构`
- `CMake 构建集成`

---

### 第 10 页 — 章节分隔页：第三章 词法分析

此为新增空白幻灯片（通过 batch JSON 创建），非模板原有。三个形状：

```json
{"command":"add","parent":"/slide[10]","type":"shape","props":{"text":"02","x":"3cm","y":"3cm","width":"6cm","height":"4cm","font":"Montserrat Ultra-Bold","size":"80","bold":"true","color":"#356AFF"}},
{"command":"add","parent":"/slide[10]","type":"shape","props":{"text":"Lexical Analysis","x":"3cm","y":"7.5cm","width":"20cm","height":"2cm","font":"Montserrat","size":"36","bold":"true","color":"#356AFF"}},
{"command":"add","parent":"/slide[10]","type":"shape","props":{"text":"Token Type System · Flex DFA · Error Detection","x":"3cm","y":"10cm","width":"20cm","height":"1.5cm","font":"Montserrat","size":"18","color":"#8497B0"}}
```

**演讲者备注**：`"Chapter 3: Lexical Analysis. TokenType enum with 50+ values in 5 categories. Flex scanner with INITIAL/COMMENT/COMMENT2 exclusive states. Lexer class wraps Flex C API with mapBisonTokenToInternal() bidirectional mapping."`

---

### 第 11 页 — Token 类型体系

标题：`Token 类型体系`
副标题：`TokenType enum class：5 大类 50+ 种 Token 取值，按语义分类`
正文块一：
```
特殊类型（2 种）：EndOfFile — 流结束哨兵，由 Lexer::tokenizeDetailed() 在末尾手动追加，非 Flex 规则产生，下游阶段依赖此哨兵判断输入末尾。Unknown — Flex 最末规则 . 匹配到任何未捕获字符时返回，扫描器继续运行不中断，错误通过 addError("E110", ...) 记录。

字面量类型（6 种）：Identifier（正则 [A-Za-z_][A-Za-z0-9_]*，toLower() ASCII 小写化后查 keywordOrIdentifier() 哈希表）；IntegerLiteral（[0-9]+，无符号，-5 被分析为 Minus + IntegerLiteral(5)）；RealLiteral（[0-9]+"."[0-9]+，必须含小数点）；CharLiteral（'([^\\'\n\r])'，仅单字符）；StringLiteral（'([^'\n\r])*\'，支持多字符和空字符串，Flex 动作中提取引号内容作为 lexeme）；BooleanLiteral（true/false）。

关键字类型（30+ 种 KwXxx）：keywordOrIdentifier() 通过 std::unordered_map<std::string, int> 实现 O(1) 查找。五组——程序结构（Program/Const/Type/Var/Begin/End）、子程序（Procedure/Function）、控制流（If/Then/Else/Case/While/Repeat/Until/For/To/DownTo/Do/Break/Continue/Exit）、I/O（Read/ReadLn/Write/WriteLn）、类型与运算符（Record/Array/Of/Integer/Real/Boolean/Char/Div/Mod/And/Or/Not）。

运算符类型（11 种）：遵循 Flex 最长匹配优先原则，多字符运算符（:=、<=、>=、<>、..）规则排在单字符之前。
```

技术摘要框（Consolas 14pt, #356AFF, fill=#EEF0FF, x=2cm, y=16cm, width=29cm, height=1.5cm）：
```
struct SourcePosition { int line = 1; int column = 1; };
struct Token { TokenType type; std::string lexeme; SourcePosition pos; };
```

插入配图 `picture/02-lexer-token-tree.png`（x=2cm, y=6.5cm, width=12cm, height=10cm），alt=`"Token Type Classification Tree"`。

---

### 第 12 页 — Flex 规则设计与状态转移

标题：`Flex 规则设计`
副标题：`DFA 有限自动机驱动：正则规则匹配 · 状态转移管理 · 全局状态变量`
正文块一（18pt bold, #356AFF）：`核心状态：INITIAL（默认）+ COMMENT（{ } 块注释）+ COMMENT2（(* *) 块注释）`
正文块二：
```
INITIAL 状态：空白字符（空格/制表符/回车）仅累加列号 g_col，不产生 Token；换行符递增 g_line 并复位 g_col=1；遇 { 进入 COMMENT 状态并记录注释起始位置（g_comment_start_line/g_comment_start_col）；遇 (* 进入 COMMENT2 状态。标识符和关键字通过 keywordOrIdentifier() 在 unordered_map 中 O(1) 查表；运算符和界符按最长匹配优先原则直接返回；字面量通过 EMIT_TOKEN 宏返回。最末规则 . 匹配任何未捕获字符，产生 E110 错误并返回 T_UNKNOWN。

COMMENT 状态：} 回到 INITIAL；{ 触发 E106（嵌套注释）；换行正常递增 g_line；其他字符静默消耗。EOF 未闭合触发 E107。COMMENT2 状态处理 (* ... *) 块注释，逻辑对称。

全局状态变量（因 yylex() 是无参函数，使用 C 全局变量模式，副作用是不可重入——但单文件编译器无影响）：static std::vector<LexError>* g_errors; static int g_line=1; static int g_col=1; static int g_comment_start_line=1; static int g_comment_start_col=1; static std::string g_last_lexeme; static int g_last_line=1; static int g_last_col=1。

Flex 扫描器使用 %option prefix="pascclex" 避免符号冲突，%x COMMENT 和 %x COMMENT2 定义独占状态。
```

---

### 第 13 页 — Lexer 类封装与错误处理

标题：`Lexer 类封装`
副标题：`tokenizeDetailed() 主流程 · mapBisonTokenToInternal() 双向映射 · 4 种词法错误码`
正文块一：
```
tokenizeDetailed(const std::string& source) 主流程：
① pasccLexerResetState(&errors) — 重置 Flex 全局状态（g_line/g_col/g_errors 等）
② pascclex_scan_string(source.c_str()) — 从内存字符串创建扫描缓冲区，避免文件 I/O
③ while ((tokenCode = pascclexlex()) != 0) 循环 — 每次调用 yylex() 返回 Bison Token 整型码
④ mapBisonTokenToInternal(tokenCode) — 将 Bison T_XXX 宏转换为内部 TokenType 枚举
⑤ 构造 Token{type, lexeme, pos} 追加到 result.tokens
⑥ pascclex_delete_buffer(buf) — 释放 Flex 缓冲区
⑦ 手动追加 Token{TokenType::EndOfFile, "<EOF>", ...} — EOF 哨兵
⑧ result.errors = std::move(errors) — 转移错误列表所有权，避免拷贝
```

正文块二：
```
为什么需要 mapBisonTokenToInternal() 双向映射？
Flex 规则动作必须返回 Bison Token 码（yyparse() 只认识 parser_bison.tab.h 中的 T_XXX 宏），但下游模块不应依赖 Bison 内部编号——应使用 token.h 中的 TokenType 枚举。因此 lexer.cpp 在 Flex 返回后立即转换，对外只暴露 TokenType。正向映射 mapToken(TokenType→int) 定义在 parser_bison_bridge.cpp:23-89，反向映射 mapBisonTokenToInternal(int→TokenType) 定义在 lexer.cpp:17-82。两个映射表必须严格 1:1 对应。
```

技术摘要框（Consolas 14pt, #356AFF, fill=#EEF0FF）：
```
E106 嵌套注释 | E107 未闭合注释（到 EOF）| E109 未终止字符串字面量 | E110 无法识别字符
错误恢复策略：词法错误不中断扫描——E106 仅记录继续；E107 强制终止；E109/E110 返回 T_UNKNOWN（由 Bison 错误恢复产生式跳过）
```

---

### 第 14 页 — 章节分隔页：第四章 语法分析

```json
{"command":"add","parent":"/slide[14]","type":"shape","props":{"text":"03","x":"3cm","y":"3cm","width":"6cm","height":"4cm","font":"Montserrat Ultra-Bold","size":"80","bold":"true","color":"#356AFF"}},
{"command":"add","parent":"/slide[14]","type":"shape","props":{"text":"Syntax Analysis","x":"3cm","y":"7.5cm","width":"20cm","height":"2cm","font":"Montserrat","size":"36","bold":"true","color":"#356AFF"}},
{"command":"add","parent":"/slide[14]","type":"shape","props":{"text":"Bison LALR(1) Parser · Grammar Design · AST Construction","x":"3cm","y":"10cm","width":"20cm","height":"1.5cm","font":"Montserrat","size":"18","color":"#8497B0"}}
```

**演讲者备注**：`"Chapter 4: Syntax Analysis. Bison LALR(1) parser with %union of 12 semantic types. Bridge layer connects C++ vector<Token> to Bison yyparse(). ~70 productions construct complete AST during reduction with typed semantic actions."`

---

### 第 15 页 — Bison 桥接层数据流

标题：`Bison 桥接层`
副标题：`C++ vector<Token> → Bison yyparse() → ParserResult（含完整 AST）`
正文块一（18pt bold, #356AFF）：`五个全局状态变量驱动整个桥接层：`
正文块二：
```
const std::vector<Token>* g_tokens = nullptr;  // 输入 Token 流指针
size_t g_pos = 0;                               // 当前读取位置（游标）
Token g_lastToken;                              // 最近被 yylex() 返回的 Token（用于 yyerror 报错定位）
ParserResult* g_result = nullptr;               // 结果累积指针
AstNode* g_ast_root = nullptr;                  // Bison 归约产出的 AST 根节点

数据流转 9 步路径：
① parseWithBison() 设置 g_tokens 指向输入向量，g_pos=0，g_ast_root=nullptr
② 调用 yyparse() 启动 Bison LALR(1) 自动机
③ Bison 每次需要下一 Token 时回调 extern "C" int yylex(void)
④ yylex() 从 (*g_tokens)[g_pos++] 取出 Token，保存到 g_lastToken
⑤ 通过 mapToken(TokenType→int) 将枚举转换为 Bison T_XXX 整型码
⑥ 为 6 种携带语义值的 Token（Identifier/CharLiteral/StringLiteral/IntegerLiteral/RealLiteral/BooleanLiteral）设置 yylval（str 或 ival）
⑦ Bison 按文法规则移进-归约，每条产生式的 {} 动作在归约时同步构造 AST 节点，通过 $$ 将子节点向上传递
⑧ program 起始规则归约时，完整 ProgramNode* 赋值给 g_ast_root
⑨ parseWithBison() 返回后将 g_ast_root 转移为 ParserResult.root（shared_ptr<ProgramNode>）

yyerror() 实现：取 g_lastToken.pos → 构造 ParseError("E201", "Syntax error: " + msg, pos) 追加到 g_result->errors。
```

---

### 第 16 页 — 文法设计与产生式体系

标题：`文法设计`
副标题：`%union（12 种语义值类型）· %type 声明 · %left/%right 优先级 · 约 70 条产生式 · 4 处错误恢复`
正文块一：
```
parser_bison.y（848 行）使用 %union 定义了 12 种语义值类型，确保归约过程中类型安全地传递 AST 节点：
AstNode*/TypeNode*/StmtNode*/ExprNode*/VarExprNode*/VarPartNode*（6 种单节点指针）
vector<AstNode*>* / vector<ExprNode*>* / vector<VarExprNode*>* / vector<VarPartNode*>* / vector<string>*（5 种列表指针）
string* / int（2 种标量）

%type 为所有非终结符声明语义值类型（如 %type<stmtNode> if_stmt while_stmt for_stmt），%token 为携带语义值的终结符声明类型（如 %token<str> T_IDENTIFIER T_INTEGER T_REAL T_CHAR T_STRING），形成完整的类型化文法体系，Bison 在编译期即可检测类型不匹配错误。

优先级声明（按 %left/%right 指令，从低到高共 4 层）：
层级 1 — %left T_OR                            or           左结合   最低
层级 2 — %left T_PLUS T_MINUS                  + -          左结合
层级 3 — %left T_MULTIPLY T_DIVIDE T_DIV T_MOD T_AND   * / div mod and  左结合
层级 4 — %right T_NOT                          not          右结合   最高

关系运算符（=、<>、<、<=、>、>=）不声明结合性——这意味着 a=b=c 这样的链式比较在 Pascal-S 中是语法错误。not 声明为右结合是因为 not not x 应解析为 not (not x)。

程序顶层结构：program → program_head program_body T_DOT。program_body 包含 const/type/var_declarations + subprogram_declarations + compound_statement。常量/类型/变量声明均通过左递归支持多个声明。子程序体不含 subprogram_declarations——这意味着 Pascal-S 不支持嵌套子程序定义。

12 种语句备选 + 空语句备选（允许连续分号）。identifier_stmt 的歧义（赋值 vs 过程调用）在语法层不做区分，由 Bison 前瞻 Token 自动归约。

表达式子体系通过产生式嵌套层次 + 优先级声明共同保证正确性：
expression → simple_expression relop simple_expression  （关系运算，无结合性）
simple_expression → simple_expression addop term          （加减 or，左结合）
term → term mulop factor                                  （乘除 and，左结合）
factor → T_NOT factor | T_MINUS factor | literal | identifier | (expression)  （一元 not/-，右结合）

4 处错误恢复产生式（每处核心是 yyerrok 宏，告知 Bison 错误恢复已完成可继续正常解析）：
const_declaration: const_declaration T_SEMICOLON error { yyerrok; }
var_declaration: var_declaration T_SEMICOLON error { yyerrok; }
statement_list: statement_list error T_SEMICOLON { yyerrok; }
stmt: error { yyerrok; }
```

插入配图 `picture/03-parser-grammar-tree.png`（x=2cm, y=6.5cm, width=12cm, height=9cm），alt=`"Grammar Production Hierarchy Tree"`。

---

### 第 17 页 — AST 节点体系

标题：`AST 节点体系`
副标题：`ast.h（283 行）：32 种 AstKind 枚举 + 20+ 节点类型，五大类覆盖全部 Pascal-S 语言构造`
正文块一：
```
类型节点（TypeNode 继承体系，5 种 AstKind）：
BasicTypeNode — name（"integer"/"real"/"boolean"/"char"），直接映射到语言内建基本类型
ArrayTypeNode — periods（vector<PeriodNode*>，每维的上下界）+ elemType（TypeNode*，递归引用自身以支持多维数组）
RecordTypeNode — fields（vector<AstNode*>，实际为 VarDeclNode* 列表，表示 record 的字段声明）
NamedTypeNode — name（类型别名引用，如 type T = array[1..10] of integer 中的 T）
PeriodNode — low + high（数组下标界的 C 表示字符串，如 "1" 和 "10"）

表达式节点（ExprNode 继承体系，5 种 AstKind）：
BinaryExprNode — op（运算符字符串 "+""-""and"等）+ left（ExprNode*）+ right（ExprNode*）
UnaryExprNode — op（"-""+""not"）+ operand（ExprNode*），处理一元正负号和逻辑非
LiteralNode — litKind（Int/Real/Char/Str/Bool 枚举）+ raw（词素原始字符串）
VarExprNode — name（变量名）+ parts（vector<VarPartNode*>，支持下标和字段访问链）
CallExprNode — name（函数名）+ args（vector<ExprNode*>，实参列表）
VarPartNode — isField（布尔歧义消除：true=字段访问如 r.x，false=下标访问如 a[i]）+ fieldName 或 indices 向量

语句节点（StmtNode 继承体系，13 种 AstKind）：
CompoundStmtNode — stmts（vector<StmtNode*>，花括号块的语句列表）
AssignStmtNode — varName + varParts（左值路径）+ rhs（ExprNode*，右侧表达式）
CallStmtNode — name + args（过程调用语句，无返回值接收方）
IfStmtNode — cond + then_ + else_（else_ 可为空，表示无 else 分支）
CaseStmtNode — expr + branches（vector<CaseBranchNode*>，每分支含 values 列表 + body）
WhileStmtNode / RepeatStmtNode / ForStmtNode — 标准循环结构（ForStmtNode 含 isTo 布尔区分 TO/DOWNTO）
ReadStmtNode — vars（vector<VarExprNode*>）+ withLn（布尔，区分 Read/ReadLn）
WriteStmtNode — exprs（vector<ExprNode*>）+ withLn（布尔，区分 Write/WriteLn）
BreakStmtNode / ContinueStmtNode / ExitStmtNode — ExitStmtNode 含 value（可为空，区分 exit 和 exit(expr)）

声明节点（7 种 AstKind）：
ConstDeclNode — name + cType（"int""float""char""string"）+ cValue（C 字面量字符串）
VarDeclNode — names（vector<string>，支持逗号分隔的多变量声明）+ typeNode
TypeDeclNode — name + typeNode
ParamGroupNode — byRef（布尔）+ names（vector<string>）+ typeName
RoutineNode — isFunction + name + params + returnTypeName + body（BlockNode*）
BlockNode — consts + types + vars + routines + compound（compound 即 CompoundStmtNode*）
ProgramNode — name + params + body（BlockNode*）

辅助节点（1 种）：IdentTailNode — 消除 identifier_stmt_tail 文法中的 Assign/Call/BareCall 三路歧义，仅 Bison 内部使用。
```

---

### 第 18 页 — 章节分隔页：第五章 语义分析

```json
{"command":"add","parent":"/slide[18]","type":"shape","props":{"text":"04","x":"3cm","y":"3cm","width":"6cm","height":"4cm","font":"Montserrat Ultra-Bold","size":"80","bold":"true","color":"#356AFF"}},
{"command":"add","parent":"/slide[18]","type":"shape","props":{"text":"Semantic Analysis","x":"3cm","y":"7.5cm","width":"20cm","height":"2cm","font":"Montserrat","size":"36","bold":"true","color":"#356AFF"}},
{"command":"add","parent":"/slide[18]","type":"shape","props":{"text":"Symbol Table System · Scope Chain · Type Checking","x":"3cm","y":"10cm","width":"20cm","height":"1.5cm","font":"Montserrat","size":"18","color":"#8497B0"}}
```

**演讲者备注**：`"Chapter 5: Semantic Analysis. Independent recursive descent traversal of Token stream. Symbol table system with TypeTemplate/BasicType inheritance, ObjectSymbol/VariableSymbol/RoutineSymbol hierarchy, TableSet linked-list lookup, ScopeStack management, and BuiltinTypePool singletons."`

---

### 第 19 页 — 符号表体系与作用域

标题：`符号表体系`
副标题：`semantic_tables.h（261 行）：TypeTemplate→BasicType 类型系统 + ObjectSymbol→VariableSymbol/RoutineSymbol 符号系统`
正文块一：
```
类型系统（左侧继承树）：
TypeTemplate（抽象基类，Kind 枚举：Basic/Array/Record）→ 当前实现仅 BasicType 子类被实际使用
BasicType（BasicKind 枚举：Int/Real/Bool/Char/None，五种基本类型）— 类型比较通过枚举值实现 O(1) 等价判断

符号系统（右侧继承树）：
ObjectSymbol（抽象基类，name_ 标识符字符串）
├─ VariableSymbol — 通过组合关系持有 shared_ptr<TypeTemplate>，表示变量的类型引用
└─ RoutineSymbol — RoutineKind 枚举（Procedure/Function）+ parameters_（vector<RoutineParameter，每元素含 byRef 布尔 + type 指针）+ returnType_

作用域管理核心 — TableSet：
每个 TableSet 代表一层作用域，包含 symbols_（SymbolTableTemplate<ObjectSymbol>，底层为 unordered_map）+ defTypes_（TypeTable 类型表）+ prevSet_ 指针（形成单向链表，指向外层作用域）
insert<T>(name, symbol) — C++17 if constexpr 编译期模板分发，根据 T 的类型自动选择插入 symbols_ 还是 defTypes_
searchEntry<T>(name, &local) — 链式查找：先在当前层 unordered_map 查询；若未命中则沿 prevSet_ 指针向外层递归搜索

ScopeStack：
持有 vector<unique_ptr<TableSet>>，拥有所有作用域层的所有权。进入子程序时 push() 新建 TableSet（prevSet_ 指向当前顶层），退出时 pop() 销毁。current() 返回最内层 TableSet*。

BuiltinTypePool（内建类型单例池）：
持有四种内建类型的 shared_ptr<BasicType> 单例（integer_/real_/boolean_/character_）。installInto(TableSet*) 将四种类型全部注入指定作用域，在语义分析开始时调用——注入 global 作用域。
```

---

### 第 20 页 — 类型检查规则

标题：`类型检查规则`
副标题：`运算符类型约束 · 形参/实参匹配算法 · 语义错误码 S101-S202`
正文块一：
```
核心类型判断辅助函数（O(1) 枚举比较）：
isNumeric(const shared_ptr<BasicType>& t)    // Int 或 Real → true
isBooleanType(const shared_ptr<BasicType>& t) // Bool → true
isIntegerType(const shared_ptr<BasicType>& t) // Int → true
promoteNumeric(lhs, rhs)                     // Int+Real → Real，Int+Int → Int
isAssignable(lhs, rhs)                       // 同类型直接兼容；Int→Real 拓宽兼容

运算符类型约束汇总：
+ / - / *        两边均为 numeric             结果 = promoteNumeric(lhs, rhs)
/                两边均为 numeric             结果始终为 Real
div / mod        两边均为 Integer             结果 = Integer
and / or         两边均为 Boolean            结果 = Boolean
not              Boolean→Boolean; Integer→Integer  结果与操作数相同
= / <> / < / <= / > / >=  两边同类型 OR 同为 numeric  结果 = Boolean

形参/实参匹配算法 checkRoutineCallArguments(routine, args, callPos)：
① if (args.size() != routine->parameters().size()) → 报告 S202（参数数量不匹配）
② for i = 0..args.size()-1：
   if (param.byRef)：
     // VAR 引用参数：要求类型完全一致 + 实参必须是左值
     if (!args[i].isLvalue) → S202（VAR 参数位置传入了非左值）
     if (args[i].type ≠ param.type) → S202（VAR 参数类型必须严格一致，不允许 Int→Real 拓宽）
   else：
     // 值参数：允许 Int→Real 拓宽
     if (!isAssignable(param.type, args[i].type)) → S202

语义错误码体系：
S101 — 同一作用域内重复定义（var x: integer; var x: real;）
S102 — 未定义的类型引用（var x: unknownType;）
S103 — 未定义标识符的使用/声明前使用（x := 5; 但 x 未声明）
S199 — 语法阶段未产出 AST 根（防御性检查，ParserResult.root == nullptr）
S201 — 类型不兼容（赋值类型不匹配、运算符操作数类型错误、控制流条件非 Boolean）
S202 — 例程调用参数错误（参数数量/类型/左值不匹配）
```

---

### 第 21 页 — 章节分隔页：第六章 代码生成

```json
{"command":"add","parent":"/slide[21]","type":"shape","props":{"text":"05","x":"3cm","y":"3cm","width":"6cm","height":"4cm","font":"Montserrat Ultra-Bold","size":"80","bold":"true","color":"#356AFF"}},
{"command":"add","parent":"/slide[21]","type":"shape","props":{"text":"Code Generation","x":"3cm","y":"7.5cm","width":"20cm","height":"2cm","font":"Montserrat","size":"36","bold":"true","color":"#356AFF"}},
{"command":"add","parent":"/slide[21]","type":"shape","props":{"text":"AST Traversal · Expression & Statement Emit · C Translation","x":"3cm","y":"10cm","width":"20cm","height":"1.5cm","font":"Montserrat","size":"18","color":"#8497B0"}}
```

**演讲者备注**：`"Chapter 6: Code Generation. AST-based recursive traversal translation to C. CodeGenContext extracted from AST declaration nodes. Direct mapping strategy: Pascal types to C types, control flow structures to C equivalents, VAR parameters as pointers, 8 runtime I/O helper functions."`

---

### 第 22 页 — 代码生成主流程与类型映射

标题：`代码生成主流程`
副标题：`code_generator.cpp（791 行）：buildContext → emitPreamble → ... → emitMain 7 步管线`
正文块一：
```
CodeGenContext 结构体（从 AST 声明节点提取，不再手动扫描 Token 流）：
std::string currentFuncName;       // 当前函数名（主程序/过程中为空）
std::string currentFuncRetVar;     // 返回值变量名（"__ret_" + funcname）
unordered_map<string, string> typeMap;          // 标识符 → C 类型（如 "a1"→"int"）
unordered_set<string> allRoutines;              // 所有例程名（用于区分变量引用 vs 无参函数调用）
unordered_map<string, string> routineRetType;   // 函数名 → C 返回类型
unordered_map<string, vector<bool>> routineByRef; // 例程名 → 各参数是否为 VAR 的布尔列表
unordered_set<string> byRefParams;               // 当前作用域内的 VAR 参数名集合

buildContext(ProgramNode*, CodeGenContext&) 遍历 AST 声明部分填充上下文：
① 遍历 body->consts → typeMap[name] = cType（"int""float""char""string"）
② 遍历 body->vars → emitTypeNode(vd->typeNode) 得到 C 类型和数组后缀，typeMap[name] = cType
③ 遍历 body->routines → allRoutines.insert(name)，若为函数则 routineRetType[name] = basicTypeToCType(returnTypeName)，遍历 params 收集 routineByRef[name]

generate() 主流程 7 步管线：
① buildContext(prog, ctx)          — 从 AST 提取全部类型/函数签名信息
② emitPreamble(out)                 — #include <stdio.h> + 8 个 static I/O helper 函数
③ emitGlobalConsts(body, out)       — #define 或 static const float（整数/实数常量）
④ emitGlobalVars(body, ctx, out)    — 类型 变量名[数组后缀] = 初值;
⑤ emitForwardDecl(routine × N)      — 前向声明所有例程（支持相互递归调用）
⑥ emitRoutine(routine × N, ctx)     — 逐例程定义（含嵌套子例程作为独立 C 函数）
⑦ emitMain(body, ctx, out)          — int main(void) { 主程序体 return 0; }

类型映射 basicTypeToCType(name)：
"integer" → "int"    "real" → "float"    "boolean" → "int"    "char" → "char"

emitTypeNode(TypeNode*) 递归处理：
BasicTypeNode → 直接映射；ArrayTypeNode → 递归处理 elemType，逐维追加 C 数组后缀 [high+1]（high+1 确保 Pascal 1-based 索引在 C 0-based 数组中安全访问）；RecordTypeNode → 当前返回 {"int", ""} 占位；NamedTypeNode → 默认退化为 {"int", ""}
```

插入配图 `picture/05-codegen-main-flow.png`（x=2cm, y=6.5cm, width=14cm, height=9cm），alt=`"Code Generation Main Flow"`。

---

### 第 23 页 — 表达式与语句发射

标题：`表达式与语句翻译`
副标题：`emitExpr() 5 种 dynamic_cast 分发 · emitStmt() 13 种分发 · 完整运算符映射表`
正文块一：
```
emitExpr(ExprNode*, ctx) 五种分发：

BinaryExprNode — 二元表达式 left op right：
运算符映射："="→"=="，"<>"→"!="，"and"→"&&"，"or"→"||"，"div"→"/"，"mod"→"%"
表达式整体加括号以保留 Pascal 优先级语义："(" + emitExpr(left) + " " + cOp + " " + emitExpr(right) + ")"

UnaryExprNode — 一元表达式 op operand：
not 运算符：根据操作数是否为 Boolean 表达式选择 "!"（逻辑非）或 "~"（位取反）
一元 +/- 后加空格防止 C 的 --/++ 歧义

LiteralNode — 字面量：
Int → 直接输出 raw 值；Real → 直接输出 raw 值；Char → 保留单引号输出
Str → 转换为 C 双引号字符串（内部转义 " 和 \）；Bool → "true"→"1"，"false"→"0"

VarExprNode — 变量引用：
若引用当前函数名且存在 currentFuncRetVar → 重定向到返回值变量（函数返回值的读取）
若无子部分且是已知函数名 → 自动补 "()" 调用（无参函数调用语法糖）
若无子部分且是 VAR 参数 → 解除引用："(*name)"
遍历 parts：字段访问（".fieldName"）或下标访问（"[idx1][idx2]..."）

CallExprNode — 函数调用：
发射为 "name(arg1, arg2)"；查询 routineByRef 判断各参数位置是否为 VAR，若为 VAR 则实参前加 "&"

emitStmt(StmtNode*, lvl, ctx, out) 十三种分发：

Pascal → C 完整映射表：
if cond then S1 else S2    →  if (cond) { S1 } else { S2 }          （else 可能为空）
while cond do S            →  while (cond) { S }                     
for i:=A to B do S         →  for (i=A; i<=B; ++i) { S }            （isTo=true → <= / ++）
for i:=A downto B do S     →  for (i=A; i>=B; --i) { S }            （isTo=false → >= / --）
repeat S until C           →  do { S } while (!(C))                  （until 条件取反）
case x of 1,2:S1; 3:S2 end →  switch(x){case 1:case 2:S1;break;case 3:S2;break;}  （逗号多值→case穿透）
read(a,b)                  →  scanf("%d",&a); scanf("%d",&b)         （根据 typeMap 选 %d/%f/%c）
readln(a)                  →  scanf(...)+消耗行尾（withLn=true→追加 getchar() 循环）
write(a)                   →  pas_write_int(a)                       （通过 inferExprType() 选 helper）
writeln                    →  pas_writeln()                          （支持无括号形式）
break / continue           →  break; / continue;                     
exit / exit(expr)          →  return; / return expr;                 

函数返回值模式：emitRoutine() 在函数体开头自动声明 "Type __ret_funcname = 0;"，函数体内 "funcname := expr" 被识别为返回赋值（比对 AssignStmtNode::varName 与 currentFuncName），翻译为 "__ret_funcname = expr;"，函数末尾自动追加 "return __ret_funcname;"
```

技术摘要框（Consolas 14pt, #356AFF, fill=#EEF0FF）：
```
inferExprType(ExprNode*) ← LiteralNode(Real→float, Char→char, Str→string) | VarExprNode(查typeMap) | CallExprNode(查routineRetType) | BinaryExprNode(任一float→float) | UnaryExprNode(递归)
```

---

### 第 24 页 — VAR 参数与输出管线

标题：`VAR 参数指针转换`
副标题：`四种 VAR 处理场景 · 8 个 static I/O helper 函数 · 完整生成管线`
正文块一：
```
VAR 参数通过 C 指针模拟——四种场景的翻译对照：

场景一（声明）— ParamGroupNode.byRef == true：
emitForwardDecl() / emitRoutine() 中将参数类型声明为 T*（如 Pascal "procedure inc(var x: integer)" → C "void inc(int* x)"）

场景二（写入/左值）— emitStmt(AssignStmtNode) 检测左值是否为 byRefParams 成员：
若左值是无子部分（parts 为空）的 VAR 参数 → 发射 "*param = rhs"
若左值有子部分（下标/字段访问）→ 发射 "param[idx] = rhs" 或 "param.field = rhs"

场景三（读取/表达式内）— emitExpr(VarExprNode) 检测标识符是否在 byRefParams 中：
若在且无子部分 → 发射 "(*param)"（加括号保证优先级）
若在且有子部分 → 发射 "param[idx1][idx2]" 或 "param.fieldName"

场景四（传参调用）— emitExpr(CallExprNode) 查询 routineByRef 判断实参位置是否为 VAR：
若对应形参为 VAR → 实参前加 "&"（取地址）
若对应形参为值参数 → 实参直接传递

8 个 static I/O helper 函数（emitPreamble() 自动生成，位于 #include <stdio.h> 之后）：
static int    pas_read_int(void)    { int v=0; (void)scanf("%d",&v); return v; }
static float  pas_read_real(void)   { float v=0.0f; (void)scanf("%f",&v); return v; }
static char   pas_read_char(void)   { char v=0; (void)scanf(" %c",&v); return v; }
static void   pas_write_int(int v)  { (void)printf("%d",v); }
static void   pas_write_real(float v)  { (void)printf("%f",v); }
static void   pas_write_char(char v)   { (void)printf("%c",v); }
static void   pas_write_str(const char* v) { (void)printf("%s",v); }
static void   pas_writeln(void)     { (void)printf("\n"); }

完整生成管线（7 步顺序）：
buildContext(从 AST 提取类型/签名) → emitPreamble(#include + 8 helpers) → emitGlobalConsts(#define/static const) → emitGlobalVars(类型 变量名 = 初值) → emitForwardDecl(前向声明所有例程) → emitRoutine × N(递归处理嵌套子例程为独立 C 函数) → emitMain(int main(void){...return 0;})
```

---

### 第 25 页 — 编译驱动与流水线时序

标题：`编译驱动与流水线`
副标题：`CompilerDriver::run() 10 步编排 · Fail-Fast 错误策略 · 11 种退出码`
正文块一（18pt bold, #356AFF）：`main.cpp（22 行）→ CompilerDriver::run(inputPath) 全流程：`
正文块二：
```
STEP 1 — 扩展名检查：验证输入文件后缀为 .pas（大小写不敏感）→ E001 / 退出码 3
STEP 2 — 文件可读性检查 → E002 / 退出码 4
STEP 3 — 读取全部源码到内存字符串（std::string）
STEP 4 — Lexer::tokenizeDetailed(source) → LexerResult{tokens + errors}
        若 !errors.empty() → 逐条 logutil::error() 输出词法错误 → return 退出码 7 (E199)
STEP 5 — Parser::parse(lexResult.tokens) → ParserResult{root + errors}
        若 !errors.empty() → 逐条输出语法错误 → return 退出码 8 (E299)
STEP 6 — SemanticDeclarationAnalyzer::analyze(tokens, parserResult) → SemanticResult{errors}
        若 !errors.empty() → 逐条输出语义错误 → return 退出码 9 (E399)
STEP 7 — 打开输出 .c 文件（同名替换扩展名）→ 失败则 E004 / 退出码 6
STEP 8 — CodeGenerator::generate(parserResult) → CodegenResult{ok, message, cSource}
        若 !ok → return 退出码 10 (E499)
STEP 9 — 写入 cSource 到输出 .c 文件
STEP 10 — 输出成功统计信息（Token 数量、AST 节点数、语义检查通过、.c 文件大小）→ return 退出码 0

Fail-Fast 策略含义：每个阶段完成后 Driver 立即检查 errors.empty()，若不为空则遍历错误列表通过 logutil::error() 逐条输出到 stderr，然后直接 return 对应退出码。用户一次编译只看到最早失败阶段的全部错误，后续阶段均不执行。

11 种退出码全表：
0=Ok | 2=Usage（命令行参数错误，如未提供 -i） | 3=InvalidExtension | 4=InputNotFound | 5=InputUnreadable（预留） | 6=OutputCreateFailed | 7=LexicalError | 8=SyntaxError | 9=SemanticError | 10=GenerationError

错误处理与日志模块极简设计：error_codes.h（21 行）定义 ErrorCode 枚举 + toExitCode() 转换函数；log.h（17 行）定义 logutil::info(stdout) 和 logutil::error(stderr) 两个 inline 函数，零外部依赖，零编译开销，全部 inline。
```

---

### 第 26 页 — 端到端数据流与语言覆盖

标题：`端到端数据流与语言覆盖`
副标题：`6 个示例程序覆盖全部语言特性 · samples 目录 5 级复杂度 · Pascal-S 覆盖矩阵`
正文块一：
```
samples 目录 6 组 .pas/.c 文件（按功能复杂度从低到高排列）：

00_main.pas — 空程序 program sample; begin end. → C: int main(void) { return 0; }（验证最小编译流水线通路，空的 program_body 五部分全部为 ε 产生式）

01_features.pas — 变量声明、赋值、write 输出：var a1,b2:integer; a1:=12; b2:=34; write(a1) → C: int a1=0; int b2=0; a1=12; b2=34; pas_write_int(a1);（验证 Pascal var 声明→C 变量初始化、:=→=、write→pas_write_xxx 选择）

02_lex_error.pas — 词法错误用例（预期编译失败，验证四种词法错误码 E106-E110 的检测和报告）

03_controlflow.pas — 全部 7 种控制流结构：while/if-then-else/for-to/for-downto/repeat-until/case-of/break+continue+exit
   关键映射：while i<3 do→while(i<3); if i=1 then...else→if(i==1)...else; for i:=1 to 2 do→for(i=1;i<=2;++i); for i:=3 downto 1→for(i=3;i>=1;--i); repeat...until cond→do{...}while(!(cond)); case x of 1,2:s1; 3:s2→switch(x){case 1:case 2:s1;break;case 3:s2;break;}

04_routines.pas — procedure/function 声明、调用、返回值、VAR 参数：
   procedure inc1(a:integer); begin x:=x+a end → void inc1(int a)
   function add1(v:integer):integer; begin add1:=v+1 end → int add1(int v){__ret_add1=v+1;return __ret_add1;}
   全局变量 x 在过程体中被修改——前向声明确保函数定义前被调用的正确性

05_realmap.pas — float 类型映射、实数函数参数/返回、实数 I/O：
   Pascal real→C float；x:=1.25→x=1.25（实数字面量直接透传）；write(x)→pas_write_real(x)
   实数常量：const pi=3.14→static const float pi=3.14f

Pascal-S 语言覆盖矩阵（热力矩阵形式，19 项全部词法+语法+代码生成覆盖）：
程序结构/基本类型/数组(多维)/常量/赋值/算术运算/逻辑运算/关系运算/if-then-else/case-of/while-do/for-to-downto/repeat-until/procedure/function/read-readln/write-writeln/break-continue-exit/字符串字面量 — 全部 ✓✓✓✓

未实现/部分支持：record 类型（AST 有 RecordTypeNode，代码生成退化为 int 占位）、type 别名（AST 有 NamedTypeNode，代码生成退化为 int）、程序参数列表（文法解析但忽略，如 program demo(input,output) 中的 (input,output) 不产生代码）
```

插入配图 `picture/09-e2e-dataflow.png`（x=2cm, y=6.5cm, width=12cm, height=10cm），alt=`"End-to-End Data Flow: Pascal Source → Lexer → Parser → Semantic → CodeGen → C Output"`。

---

### 第 27 页 — 架构演进与未来方向

标题：`架构演进与未来方向`
副标题：`AST 从占位骨架到完整富语义树的演进 · 代码量减少 47% · 五个后续优化方向`
正文块一：
```
AST 完整实现带来的架构收益（四项量化指标）：

① Bison 成为 AST 构建器 — parser_bison.y（848 行）中每条产生式的语义动作在归约时同步构造 AST 节点，通过 %union（12 种语义值类型）+ %type 声明实现类型安全的自底向上树构造。不再是空的 {} 动作——每个归约步骤都在建树。

② 代码生成器基于 AST 遍历 — code_generator.cpp（791 行）通过 emitExpr(ExprNode*) / emitStmt(StmtNode*) 递归遍历 AST，代码量较旧版 Token 流直译减少 47%，逻辑清晰度显著提升。所有语法结构信息已编码在 AST 节点的类型和字段中，不再需要解析 Token 序列来判断"当前是什么语句"。

③ 编译流水线单次词法分析 — CompilerDriver 中代码生成阶段直接接收 ParserResult（含完整 AST），不再重复执行词法分析。Token 流仅在词法→语法→语义三个阶段间传递；代码生成阶段需要的类型信息、函数签名、变量声明等全部从 AST 声明节点中直接获取。

④ 类型信息从 AST 声明节点提取 — buildContext() 遍历 BlockNode 中的 ConstDeclNode / VarDeclNode / RoutineNode 填充 CodeGenContext，替代了旧版的两遍 Token 扫描策略（旧版需要第一遍收集类型和签名信息，第二遍才做翻译）。

五个后续优化方向：
1. 语义分析器接入 AST — 当前语义分析器仍独立遍历 Token 流，改为遍历 AST 可消除 Token 流的重复遍历，并利用 AST 的结构化信息（如直接获取表达式节点类型）简化类型推导逻辑
2. 中间表示（IR）层 — 在语义分析和代码生成之间插入与目标语言无关的中间表示，支持优化遍（如常量折叠、死代码消除、公共子表达式消除）
3. 错误恢复增强 — 在语义分析阶段引入类似 Bison yyerror/yyerrok 的错误恢复机制，使语义分析可以在一轮中报告多个错误而非 fail-fast 仅报第一个
4. 代码优化 — 常量折叠（编译期计算常量表达式）、死代码消除（移除永不执行的 if(false) 分支）、record 类型的完整 C 代码生成（struct 定义 + .field 访问）
5. 多目标后端 — 抽象代码生成接口（ICodeGenerator），支持除 C 以外的目标语言：LLVM IR、WebAssembly、Python、JavaScript 等
```

插入配图 `picture/10-ast-future-design.png`（x=2cm, y=6cm, width=14cm, height=10cm），alt=`"AST Full Node Design: 32 AstKind + 20+ Node Types in 5 Categories"`。

---

### 第 28 页 — 感谢观看（由原第 10 页移至末尾）

通过 `officecli move "${OUTPUT}" /slide[10] --after /slide[28]` 命令将模板原有的结束页移至末尾。修改其文本形状：

- 主文字 → `感谢观看`
- 副文字 → `pascc — Pascal-S to C Compiler`
- 底部信息 → `编译课设项目组 | 2026年5月`

---

## 四、batch JSON 生成命令汇总

### 4.1 创建 17 张空白幻灯片（s11-s27）

```json
[
  {"command":"add","parent":"/","type":"slide","props":{"layout":"Blank","background":"#F4F5FA"}},
  ...共 17 条
]
```

### 4.2 章节分隔页形状（s10, s14, s18, s21）
每页 3 个 shape：大号数字（80pt Montserrat Ultra-Bold #356AFF）+ 英文标题（36pt Montserrat Bold #356AFF）+ 副标题（18pt Montserrat #8497B0）

### 4.3 正文页形状（s11-s13, s15-s17, s19-s20, s22-s25, s26-s27）
每页 3-5 个 shape：标题（36pt 微软雅黑 Bold #356AFF）+ 副标题（18pt 微软雅黑 #8497B0）+ 正文 1-N（16pt 微软雅黑 #333333 lineSpacing=1.5x）+ 可选技术摘要框（14pt Consolas #356AFF fill=#EEF0FF）

### 4.4 配图（6 张）
```json
{"command":"add","parent":"/slide[5]","type":"picture","props":{"src":"<ABSPATH>/picture/01-architecture-pipeline.png","x":"2cm","y":"12cm","width":"15cm","height":"6cm","alt":"Compilation Pipeline Architecture"}},
{"command":"add","parent":"/slide[11]","type":"picture","props":{"src":"<ABSPATH>/picture/02-lexer-token-tree.png","x":"2cm","y":"6.5cm","width":"12cm","height":"10cm","alt":"Token Type Classification Tree"}},
{"command":"add","parent":"/slide[16]","type":"picture","props":{"src":"<ABSPATH>/picture/03-parser-grammar-tree.png","x":"2cm","y":"6.5cm","width":"12cm","height":"9cm","alt":"Grammar Production Hierarchy"}},
{"command":"add","parent":"/slide[22]","type":"picture","props":{"src":"<ABSPATH>/picture/05-codegen-main-flow.png","x":"2cm","y":"6.5cm","width":"14cm","height":"9cm","alt":"Code Generation Main Flow"}},
{"command":"add","parent":"/slide[26]","type":"picture","props":{"src":"<ABSPATH>/picture/09-e2e-dataflow.png","x":"2cm","y":"6.5cm","width":"12cm","height":"10cm","alt":"End-to-End Data Flow"}},
{"command":"add","parent":"/slide[27]","type":"picture","props":{"src":"<ABSPATH>/picture/10-ast-future-design.png","x":"2cm","y":"6cm","width":"14cm","height":"10cm","alt":"AST Node Design"}}
```

### 4.5 演讲者备注（9 条）
在 s1, s3, s10, s14, s18, s21, s25, s27 上各一条 `add --type notes`。

---

## 五、最终 QA 检查清单

```bash
# Gate 1: Schema 验证
officecli validate "${OUTPUT}" && echo "PASS" || echo "FAIL"

# Gate 2: 占位符清零（输出为空 = PASS）
officecli view "${OUTPUT}" text | grep -inE 'xxxx|lorem|ipsum|<todo>|placeholder|请输入|添加小标题|添加关键词|单击此处|Click to add|文字是您'

# Gate 3: 结构完整性
officecli view "${OUTPUT}" stats   # 确认 Slides: 28
officecli view "${OUTPUT}" outline # 确认 28 页，第 28 页为感谢观看

# Gate 4: 模板一致性
# 确认背景色、主色 #356AFF、辅助色 #8497B0、正文色 #333333
# 确认模板原有 Freeform/Group/Connector 均未被删除
officecli view "${OUTPUT}" issues  # 仅有模板遗留的 alt text 问题是可接受的
```
