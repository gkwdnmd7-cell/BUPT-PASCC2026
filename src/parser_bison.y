%code requires {
    #include "ast.h"
    #include <string>
    #include <vector>
}

/* %code {} is placed AFTER #include "parser_bison.tab.h", so AstNode is defined */
%code {
    extern AstNode* g_ast_root;
    int yylex(void);
    void yyerror(const char* msg);
}

/* ---- Semantic value union ---- */
%union {
    AstNode*                    node;
    TypeNode*                   typeNode;
    StmtNode*                   stmtNode;
    ExprNode*                   exprNode;
    VarExprNode*                varExprNode;
    VarPartNode*                varPartNode;
    std::vector<AstNode*>*      nodeList;
    std::vector<ExprNode*>*     exprList;
    std::vector<VarExprNode*>*  varExprList;
    std::vector<VarPartNode*>*  varPartList;
    std::vector<std::string>*   strList;
    std::string*                str;
    int                         ival;
}

/* ---- Tokens with semantic values ---- */
%token <str>  T_IDENTIFIER T_INTEGER T_REAL T_CHAR T_STRING
%token <ival> T_BOOLEAN

/* ---- Plain tokens ---- */
%token T_UNKNOWN
%token T_PROGRAM T_CONST T_TYPE T_VAR T_PROCEDURE T_FUNCTION T_BEGIN T_END T_IF T_THEN T_ELSE
%token T_CASE T_WHILE T_REPEAT T_UNTIL T_FOR T_TO T_DOWNTO T_DO
%token T_READ T_READLN T_WRITE T_WRITELN
%token T_BREAK T_CONTINUE T_EXIT
%token T_RECORD T_ARRAY T_OF T_TYPE_INTEGER T_TYPE_REAL T_TYPE_BOOLEAN T_TYPE_CHAR
%token T_PLUS T_MINUS T_MULTIPLY T_DIVIDE T_DIV T_MOD T_AND T_OR T_NOT
%token T_ASSIGN T_EQUAL T_NOT_EQUAL T_LESS T_LESS_EQUAL T_GREATER T_GREATER_EQUAL
%token T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_COMMA T_SEMICOLON T_COLON T_DOT T_DOTDOT

/* ---- Operator precedence (low to high) ---- */
%left T_OR
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE T_DIV T_MOD T_AND
%right T_NOT

/* ---- Non-terminal types ---- */
%type <node>        program program_head program_body
%type <node>        num const_value period
%type <node>        subprogram_declaration subprogram_head subprogram_body
%type <node>        parameter_list var_parameter value_parameter
%type <node>        identifier_stmt_tail branch

%type <typeNode>    type_spec

%type <stmtNode>    compound_statement stmt
%type <stmtNode>    break_stmt continue_stmt exit_stmt
%type <stmtNode>    identifier_stmt if_stmt else_part
%type <stmtNode>    case_stmt while_stmt repeat_stmt for_stmt
%type <stmtNode>    read_stmt write_stmt

%type <exprNode>    expression simple_expression term factor

%type <varExprNode> variable
%type <varPartNode> id_varpart

%type <nodeList>    const_declarations const_declaration
%type <nodeList>    type_declarations  type_declaration record_body
%type <nodeList>    var_declarations   var_declaration
%type <nodeList>    subprogram_declarations
%type <nodeList>    formal_parameter parameter_lists
%type <nodeList>    periods
%type <nodeList>    statement_list_opt statement_list
%type <nodeList>    case_body branch_list

%type <exprList>    expression_list_opt expression_list
%type <varExprList> variable_list
%type <varPartList> id_varparts
%type <strList>     id_list const_list

%type <str>         relop addop mulop basic_type
%type <ival>        updown

%%

/* ============================================================
   Program structure
   ============================================================ */

program
    : program_head program_body T_DOT
      {
          ProgramNode* prog = static_cast<ProgramNode*>($1);
          prog->body = static_cast<BlockNode*>($2);
          g_ast_root = prog;
          $$ = prog;
      }
    ;

program_head
    : T_PROGRAM T_IDENTIFIER T_LPAREN id_list T_RPAREN T_SEMICOLON
      {
          auto* p = new ProgramNode();
          p->name = *$2; delete $2;
          p->params = *$4; delete $4;
          $$ = p;
      }
    | T_PROGRAM T_IDENTIFIER T_LPAREN T_RPAREN T_SEMICOLON
      {
          auto* p = new ProgramNode();
          p->name = *$2; delete $2;
          $$ = p;
      }
    | T_PROGRAM T_IDENTIFIER T_SEMICOLON
      {
          auto* p = new ProgramNode();
          p->name = *$2; delete $2;
          $$ = p;
      }
    ;

program_body
    : const_declarations type_declarations var_declarations subprogram_declarations compound_statement
      {
          auto* b = new BlockNode();
          if ($1) { for (auto* n : *$1) b->consts.push_back(static_cast<ConstDeclNode*>(n)); delete $1; }
          if ($2) { for (auto* n : *$2) b->types.push_back(static_cast<TypeDeclNode*>(n));  delete $2; }
          if ($3) { for (auto* n : *$3) b->vars.push_back(static_cast<VarDeclNode*>(n));    delete $3; }
          if ($4) { b->routines = *$4; delete $4; }
          b->compound = static_cast<CompoundStmtNode*>($5);
          $$ = b;
      }
    ;

/* ---- Identifier lists ---- */

id_list
    : T_IDENTIFIER
      { $$ = new std::vector<std::string>(); $$->push_back(*$1); delete $1; }
    | id_list T_COMMA T_IDENTIFIER
      { $1->push_back(*$3); delete $3; $$ = $1; }
    ;

/* ============================================================
   Const declarations
   ============================================================ */

const_declarations
    : /* empty */                          { $$ = nullptr; }
    | T_CONST const_declaration T_SEMICOLON { $$ = $2;    }
    ;

const_declaration
    : T_IDENTIFIER T_EQUAL const_value
      {
          auto* cv = static_cast<ConstDeclNode*>($3);
          cv->name = *$1; delete $1;
          $$ = new std::vector<AstNode*>();
          $$->push_back(cv);
      }
    | const_declaration T_SEMICOLON T_IDENTIFIER T_EQUAL const_value
      {
          auto* cv = static_cast<ConstDeclNode*>($5);
          cv->name = *$3; delete $3;
          $1->push_back(cv);
          $$ = $1;
      }
    | const_declaration T_SEMICOLON error { yyerrok; $$ = $1; }
    ;

const_value
    : T_PLUS num
      {
          auto* cv = static_cast<ConstDeclNode*>($2);
          if (!cv->cValue.empty() && cv->cValue[0] != '-') cv->cValue = "+" + cv->cValue;
          $$ = cv;
      }
    | T_MINUS num
      {
          auto* cv = static_cast<ConstDeclNode*>($2);
          cv->cValue = "-" + cv->cValue;
          $$ = cv;
      }
    | num { $$ = $1; }
    | T_CHAR
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "char";
          cv->cValue = *$1; delete $1;
          $$ = cv;
      }
    | T_STRING
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "string";
          std::string escaped = "\"";
          for (char c : *$1) {
              if (c == '"')  escaped += "\\\"";
              else if (c == '\\') escaped += "\\\\";
              else escaped += c;
          }
          escaped += "\"";
          cv->cValue = escaped; delete $1;
          $$ = cv;
      }
    | T_BOOLEAN
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "int";
          cv->cValue = ($1 ? "1" : "0");
          $$ = cv;
      }
    | T_IDENTIFIER
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "int";
          cv->cValue = *$1; delete $1;
          $$ = cv;
      }
    | T_PLUS T_IDENTIFIER
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "int";
          cv->cValue = "+" + *$2; delete $2;
          $$ = cv;
      }
    | T_MINUS T_IDENTIFIER
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "int";
          cv->cValue = "-" + *$2; delete $2;
          $$ = cv;
      }
    ;

num
    : T_INTEGER
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "int";
          cv->cValue = *$1; delete $1;
          $$ = cv;
      }
    | T_REAL
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "float";
          cv->cValue = *$1 + "f"; delete $1;
          $$ = cv;
      }
    ;

/* ============================================================
   Type declarations
   ============================================================ */

type_declarations
    : /* empty */                         { $$ = nullptr; }
    | T_TYPE type_declaration T_SEMICOLON { $$ = $2;      }
    ;

type_declaration
    : T_IDENTIFIER T_EQUAL type_spec
      {
          auto* td = new TypeDeclNode();
          td->name     = *$1; delete $1;
          td->typeNode = $3;
          $$ = new std::vector<AstNode*>();
          $$->push_back(td);
      }
    | type_declaration T_SEMICOLON T_IDENTIFIER T_EQUAL type_spec
      {
          auto* td = new TypeDeclNode();
          td->name     = *$3; delete $3;
          td->typeNode = $5;
          $1->push_back(td);
          $$ = $1;
      }
    ;

type_spec
    : basic_type
      { $$ = new BasicTypeNode(*$1); delete $1; }
    | T_ARRAY T_LBRACKET periods T_RBRACKET T_OF type_spec
      {
          auto* at = new ArrayTypeNode();
          if ($3) {
              for (auto* n : *$3) at->periods.push_back(static_cast<PeriodNode*>(n));
              delete $3;
          }
          at->elemType = $6;
          $$ = at;
      }
    | T_RECORD record_body T_END
      {
          auto* rt = new RecordTypeNode();
          if ($2) {
              rt->fields = *$2; delete $2;
          }
          $$ = rt;
      }
    | T_IDENTIFIER
      { $$ = new NamedTypeNode(*$1); delete $1; }
    ;

record_body
    : /* empty */                   { $$ = nullptr; }
    | var_declaration               { $$ = $1;      }
    | var_declaration T_SEMICOLON   { $$ = $1;      }
    ;

basic_type
    : T_TYPE_INTEGER  { $$ = new std::string("integer"); }
    | T_TYPE_REAL     { $$ = new std::string("real");    }
    | T_TYPE_BOOLEAN  { $$ = new std::string("boolean"); }
    | T_TYPE_CHAR     { $$ = new std::string("char");    }
    ;

periods
    : period
      { $$ = new std::vector<AstNode*>(); $$->push_back($1); }
    | periods T_COMMA period
      { $1->push_back($3); $$ = $1; }
    ;

period
    : const_value T_DOTDOT const_value
      {
          auto* lo = static_cast<ConstDeclNode*>($1);
          auto* hi = static_cast<ConstDeclNode*>($3);
          auto* p  = new PeriodNode(lo->cValue, hi->cValue);
          delete lo; delete hi;
          $$ = p;
      }
    ;

/* ============================================================
   Variable declarations
   ============================================================ */

var_declarations
    : /* empty */                       { $$ = nullptr; }
    | T_VAR var_declaration T_SEMICOLON { $$ = $2;      }
    ;

var_declaration
    : id_list T_COLON type_spec
      {
          auto* vd = new VarDeclNode();
          vd->names    = *$1; delete $1;
          vd->typeNode = $3;
          $$ = new std::vector<AstNode*>();
          $$->push_back(vd);
      }
    | var_declaration T_SEMICOLON id_list T_COLON type_spec
      {
          auto* vd = new VarDeclNode();
          vd->names    = *$3; delete $3;
          vd->typeNode = $5;
          $1->push_back(vd);
          $$ = $1;
      }
    | var_declaration T_SEMICOLON error { yyerrok; $$ = $1; }
    ;

/* ============================================================
   Subprogram declarations
   ============================================================ */

subprogram_declarations
    : /* empty */ { $$ = nullptr; }
    | subprogram_declarations subprogram_declaration T_SEMICOLON
      {
          if (!$1) $1 = new std::vector<AstNode*>();
          $1->push_back($2);
          $$ = $1;
      }
    ;

subprogram_declaration
    : subprogram_head subprogram_body
      {
          auto* r = static_cast<RoutineNode*>($1);
          r->body = static_cast<BlockNode*>($2);
          $$ = r;
      }
    ;

subprogram_head
    : T_FUNCTION T_IDENTIFIER formal_parameter T_COLON basic_type T_SEMICOLON
      {
          auto* r = new RoutineNode();
          r->isFunction    = true;
          r->name          = *$2; delete $2;
          r->returnTypeName = *$5; delete $5;
          if ($3) {
              for (auto* n : *$3) r->params.push_back(static_cast<ParamGroupNode*>(n));
              delete $3;
          }
          $$ = r;
      }
    | T_PROCEDURE T_IDENTIFIER formal_parameter T_SEMICOLON
      {
          auto* r = new RoutineNode();
          r->isFunction = false;
          r->name       = *$2; delete $2;
          if ($3) {
              for (auto* n : *$3) r->params.push_back(static_cast<ParamGroupNode*>(n));
              delete $3;
          }
          $$ = r;
      }
    ;

formal_parameter
    : /* empty */                           { $$ = nullptr; }
    | T_LPAREN parameter_lists T_RPAREN     { $$ = $2;      }
    | T_LPAREN T_RPAREN                     { $$ = nullptr; }
    ;

parameter_lists
    : parameter_list
      { $$ = new std::vector<AstNode*>(); $$->push_back($1); }
    | parameter_lists T_SEMICOLON parameter_list
      { $1->push_back($3); $$ = $1; }
    ;

parameter_list
    : var_parameter   { $$ = $1; }
    | value_parameter { $$ = $1; }
    ;

var_parameter
    : T_VAR value_parameter
      { static_cast<ParamGroupNode*>($2)->byRef = true; $$ = $2; }
    ;

value_parameter
    : id_list T_COLON basic_type
      {
          auto* pg = new ParamGroupNode();
          pg->byRef    = false;
          pg->names    = *$1; delete $1;
          pg->typeName = *$3; delete $3;
          $$ = pg;
      }
    ;

subprogram_body
    : const_declarations type_declarations var_declarations compound_statement
      {
          auto* b = new BlockNode();
          if ($1) { for (auto* n : *$1) b->consts.push_back(static_cast<ConstDeclNode*>(n)); delete $1; }
          if ($2) { for (auto* n : *$2) b->types.push_back(static_cast<TypeDeclNode*>(n));  delete $2; }
          if ($3) { for (auto* n : *$3) b->vars.push_back(static_cast<VarDeclNode*>(n));    delete $3; }
          b->compound = static_cast<CompoundStmtNode*>($4);
          $$ = b;
      }
    ;

/* ============================================================
   Compound statement and statement list
   ============================================================ */

compound_statement
    : T_BEGIN statement_list_opt T_END
      {
          auto* cs = new CompoundStmtNode();
          if ($2) {
              for (auto* n : *$2) if (n) cs->stmts.push_back(static_cast<StmtNode*>(n));
              delete $2;
          }
          $$ = cs;
      }
    ;

statement_list_opt
    : /* empty */   { $$ = new std::vector<AstNode*>(); }
    | statement_list { $$ = $1; }
    ;

statement_list
    : stmt
      { $$ = new std::vector<AstNode*>(); if ($1) $$->push_back($1); }
    | statement_list T_SEMICOLON stmt
      { if ($3) $1->push_back($3); $$ = $1; }
    | statement_list error T_SEMICOLON { yyerrok; $$ = $1; }
    ;

/* ============================================================
   Statements
   ============================================================ */

stmt
    : identifier_stmt  { $$ = $1; }
    | compound_statement { $$ = $1; }
    | if_stmt          { $$ = $1; }
    | case_stmt        { $$ = $1; }
    | while_stmt       { $$ = $1; }
    | repeat_stmt      { $$ = $1; }
    | for_stmt         { $$ = $1; }
    | read_stmt        { $$ = $1; }
    | write_stmt       { $$ = $1; }
    | break_stmt       { $$ = $1; }
    | continue_stmt    { $$ = $1; }
    | exit_stmt        { $$ = $1; }
    | error            { yyerrok; $$ = nullptr; }
    | /* empty */      { $$ = nullptr; }
    ;

break_stmt
    : T_BREAK { $$ = new BreakStmtNode(); }
    ;

continue_stmt
    : T_CONTINUE { $$ = new ContinueStmtNode(); }
    ;

exit_stmt
    : T_EXIT
      { auto* n = new ExitStmtNode(); $$ = n; }
    | T_EXIT T_LPAREN expression T_RPAREN
      { auto* n = new ExitStmtNode(); n->value = $3; $$ = n; }
    ;

identifier_stmt
    : T_IDENTIFIER identifier_stmt_tail
      {
          auto* tail = static_cast<IdentTailNode*>($2);
          std::string name = *$1; delete $1;
          StmtNode* result;
          if (tail->tailKind == IdentTailNode::Kind::Assign) {
              auto* a = new AssignStmtNode();
              a->varName  = name;
              a->varParts = tail->varParts;
              a->rhs      = tail->rhs;
              result = a;
          } else {
              result = new CallStmtNode(name, tail->args);
          }
          delete tail;
          $$ = result;
      }
    ;

identifier_stmt_tail
    : id_varparts T_ASSIGN expression
      {
          auto* t = new IdentTailNode();
          t->tailKind = IdentTailNode::Kind::Assign;
          if ($1) { t->varParts = *$1; delete $1; }
          t->rhs = $3;
          $$ = t;
      }
    | T_LPAREN expression_list_opt T_RPAREN
      {
          auto* t = new IdentTailNode();
          t->tailKind = IdentTailNode::Kind::Call;
          if ($2) { t->args = *$2; delete $2; }
          $$ = t;
      }
    | /* empty */
      {
          auto* t = new IdentTailNode();
          t->tailKind = IdentTailNode::Kind::BareCall;
          $$ = t;
      }
    ;

if_stmt
    : T_IF expression T_THEN stmt else_part
      { $$ = new IfStmtNode($2, $4, $5); }
    ;

else_part
    : /* empty */ { $$ = nullptr; }
    | T_ELSE stmt { $$ = $2; }
    ;

case_stmt
    : T_CASE expression T_OF case_body T_END
      {
          auto* cs = new CaseStmtNode();
          cs->expr = $2;
          if ($4) {
              for (auto* n : *$4) cs->branches.push_back(static_cast<CaseBranchNode*>(n));
              delete $4;
          }
          $$ = cs;
      }
    ;

case_body
    : /* empty */  { $$ = nullptr; }
    | branch_list  { $$ = $1; }
    ;

branch_list
    : branch
      { $$ = new std::vector<AstNode*>(); $$->push_back($1); }
    | branch_list T_SEMICOLON branch
      { $1->push_back($3); $$ = $1; }
    ;

branch
    : const_list T_COLON stmt
      {
          auto* cb = new CaseBranchNode();
          cb->values = *$1; delete $1;
          cb->body   = $3;
          $$ = cb;
      }
    ;

const_list
    : const_value
      {
          auto* cv = static_cast<ConstDeclNode*>($1);
          $$ = new std::vector<std::string>();
          $$->push_back(cv->cValue);
          delete cv;
      }
    | const_list T_COMMA const_value
      {
          auto* cv = static_cast<ConstDeclNode*>($3);
          $1->push_back(cv->cValue);
          delete cv;
          $$ = $1;
      }
    ;

while_stmt
    : T_WHILE expression T_DO stmt
      { $$ = new WhileStmtNode($2, $4); }
    ;

repeat_stmt
    : T_REPEAT statement_list_opt T_UNTIL expression
      {
          auto* body = new CompoundStmtNode();
          if ($2) {
              for (auto* n : *$2) if (n) body->stmts.push_back(static_cast<StmtNode*>(n));
              delete $2;
          }
          $$ = new RepeatStmtNode(body, $4);
      }
    ;

for_stmt
    : T_FOR T_IDENTIFIER T_ASSIGN expression updown expression T_DO stmt
      {
          auto* fs = new ForStmtNode();
          fs->var  = *$2; delete $2;
          fs->from_ = $4;
          fs->isTo  = ($5 == 1);
          fs->to_   = $6;
          fs->body  = $8;
          $$ = fs;
      }
    ;

updown
    : T_TO     { $$ = 1; }
    | T_DOWNTO { $$ = 0; }
    ;

read_stmt
    : T_READ T_LPAREN variable_list T_RPAREN
      {
          auto* rs = new ReadStmtNode();
          rs->withLn = false;
          if ($3) { rs->vars = *$3; delete $3; }
          $$ = rs;
      }
    | T_READLN T_LPAREN variable_list T_RPAREN
      {
          auto* rs = new ReadStmtNode();
          rs->withLn = true;
          if ($3) { rs->vars = *$3; delete $3; }
          $$ = rs;
      }
    ;

write_stmt
    : T_WRITE T_LPAREN expression_list T_RPAREN
      {
          auto* ws = new WriteStmtNode();
          ws->withLn = false;
          if ($3) { ws->exprs = *$3; delete $3; }
          $$ = ws;
      }
    | T_WRITELN T_LPAREN expression_list T_RPAREN
      {
          auto* ws = new WriteStmtNode();
          ws->withLn = true;
          if ($3) { ws->exprs = *$3; delete $3; }
          $$ = ws;
      }
    | T_WRITELN T_LPAREN T_RPAREN
      {
          auto* ws = new WriteStmtNode();
          ws->withLn = true;
          $$ = ws;
      }
    | T_WRITELN
      {
          auto* ws = new WriteStmtNode();
          ws->withLn = true;
          $$ = ws;
      }
    ;

/* ============================================================
   Variable access
   ============================================================ */

variable_list
    : variable
      { $$ = new std::vector<VarExprNode*>(); $$->push_back($1); }
    | variable_list T_COMMA variable
      { $1->push_back($3); $$ = $1; }
    ;

variable
    : T_IDENTIFIER id_varparts
      {
          std::vector<VarPartNode*> parts;
          if ($2) { parts = *$2; delete $2; }
          $$ = new VarExprNode(*$1, parts); delete $1;
      }
    ;

id_varparts
    : /* empty */ { $$ = nullptr; }
    | id_varparts id_varpart
      {
          if (!$1) $1 = new std::vector<VarPartNode*>();
          $1->push_back($2);
          $$ = $1;
      }
    ;

id_varpart
    : T_LBRACKET expression_list T_RBRACKET
      {
          auto* vp = new VarPartNode();
          vp->isField  = false;
          if ($2) { vp->indices = *$2; delete $2; }
          $$ = vp;
      }
    | T_DOT T_IDENTIFIER
      {
          auto* vp = new VarPartNode();
          vp->isField   = true;
          vp->fieldName = *$2; delete $2;
          $$ = vp;
      }
    ;

/* ============================================================
   Expressions
   ============================================================ */

expression_list_opt
    : /* empty */    { $$ = nullptr; }
    | expression_list { $$ = $1; }
    ;

expression_list
    : expression
      { $$ = new std::vector<ExprNode*>(); $$->push_back($1); }
    | expression_list T_COMMA expression
      { $1->push_back($3); $$ = $1; }
    ;

expression
    : simple_expression
      { $$ = $1; }
    | simple_expression relop simple_expression
      { $$ = new BinaryExprNode(*$2, $1, $3); delete $2; }
    ;

relop
    : T_EQUAL         { $$ = new std::string("=");  }
    | T_NOT_EQUAL     { $$ = new std::string("<>"); }
    | T_LESS          { $$ = new std::string("<");  }
    | T_LESS_EQUAL    { $$ = new std::string("<="); }
    | T_GREATER       { $$ = new std::string(">");  }
    | T_GREATER_EQUAL { $$ = new std::string(">="); }
    ;

simple_expression
    : term { $$ = $1; }
    | simple_expression addop term
      { $$ = new BinaryExprNode(*$2, $1, $3); delete $2; }
    ;

addop
    : T_PLUS  { $$ = new std::string("+");  }
    | T_MINUS { $$ = new std::string("-");  }
    | T_OR    { $$ = new std::string("or"); }
    ;

term
    : factor { $$ = $1; }
    | term mulop factor
      { $$ = new BinaryExprNode(*$2, $1, $3); delete $2; }
    ;

mulop
    : T_MULTIPLY { $$ = new std::string("*");   }
    | T_DIVIDE   { $$ = new std::string("/");   }
    | T_DIV      { $$ = new std::string("div"); }
    | T_MOD      { $$ = new std::string("mod"); }
    | T_AND      { $$ = new std::string("and"); }
    ;

factor
    : T_INTEGER
      { $$ = new LiteralNode(LiteralKind::Int,  *$1); delete $1; }
    | T_REAL
      { $$ = new LiteralNode(LiteralKind::Real, *$1); delete $1; }
    | variable
      { $$ = $1; }
    | T_IDENTIFIER T_LPAREN expression_list_opt T_RPAREN
      {
          std::vector<ExprNode*> args;
          if ($3) { args = *$3; delete $3; }
          $$ = new CallExprNode(*$1, args); delete $1;
      }
    | T_LPAREN expression T_RPAREN { $$ = $2; }
    | T_NOT factor   { $$ = new UnaryExprNode("not", $2); }
    | T_MINUS factor { $$ = new UnaryExprNode("-",   $2); }
    | T_PLUS factor  { $$ = new UnaryExprNode("+",   $2); }
    | T_CHAR
      { $$ = new LiteralNode(LiteralKind::Char, *$1); delete $1; }
    | T_STRING
      { $$ = new LiteralNode(LiteralKind::Str,  *$1); delete $1; }
    | T_BOOLEAN
      { $$ = new LiteralNode(LiteralKind::Bool, ($1 ? std::string("true") : std::string("false"))); }
    ;

%%
