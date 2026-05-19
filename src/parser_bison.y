%{
#include <stdio.h>

int yylex(void);
void yyerror(const char* msg);
%}

%token T_UNKNOWN
%token T_IDENTIFIER T_INTEGER T_REAL T_CHAR T_STRING T_BOOLEAN
%token T_PROGRAM T_CONST T_TYPE T_VAR T_PROCEDURE T_FUNCTION T_BEGIN T_END T_IF T_THEN T_ELSE
%token T_CASE T_WHILE T_REPEAT T_UNTIL T_FOR T_TO T_DOWNTO T_DO
%token T_READ T_READLN T_WRITE T_WRITELN
%token T_RECORD T_ARRAY T_OF T_TYPE_INTEGER T_TYPE_REAL T_TYPE_BOOLEAN T_TYPE_CHAR
%token T_PLUS T_MINUS T_MULTIPLY T_DIVIDE T_DIV T_MOD T_AND T_OR T_NOT
%token T_ASSIGN T_EQUAL T_NOT_EQUAL T_LESS T_LESS_EQUAL T_GREATER T_GREATER_EQUAL
%token T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_COMMA T_SEMICOLON T_COLON T_DOT T_DOTDOT

%left T_OR
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE T_DIV T_MOD T_AND
%right T_NOT

%%

program
    : program_head program_body T_DOT
    ;

program_head
    : T_PROGRAM T_IDENTIFIER T_LPAREN id_list T_RPAREN T_SEMICOLON
    | T_PROGRAM T_IDENTIFIER T_LPAREN T_RPAREN T_SEMICOLON
    | T_PROGRAM T_IDENTIFIER T_SEMICOLON
    ;

program_body
    : const_declarations type_declarations var_declarations subprogram_declarations compound_statement
    ;

id_list
    : T_IDENTIFIER
    | id_list T_COMMA T_IDENTIFIER
    ;

const_declarations
    : /* empty */
    | T_CONST const_declaration T_SEMICOLON
    ;

const_declaration
    : T_IDENTIFIER T_EQUAL const_value
    | const_declaration T_SEMICOLON T_IDENTIFIER T_EQUAL const_value
    | const_declaration T_SEMICOLON error { yyerrok; }
    ;

const_value
    : T_PLUS num
    | T_MINUS num
    | num
    | T_CHAR
    | T_STRING
    | T_BOOLEAN
    | T_IDENTIFIER
    | T_PLUS T_IDENTIFIER
    | T_MINUS T_IDENTIFIER
    ;

num
    : T_INTEGER
    | T_REAL
    ;

type_declarations
    : /* empty */
    | T_TYPE type_declaration T_SEMICOLON
    ;

type_declaration
    : T_IDENTIFIER T_EQUAL type_spec
    | type_declaration T_SEMICOLON T_IDENTIFIER T_EQUAL type_spec
    ;

type_spec
    : basic_type
    | T_ARRAY T_LBRACKET periods T_RBRACKET T_OF type_spec
    | T_RECORD record_body T_END
    | T_IDENTIFIER
    ;

record_body
    : /* empty */
    | var_declaration
    | var_declaration T_SEMICOLON
    ;

basic_type
    : T_TYPE_INTEGER
    | T_TYPE_REAL
    | T_TYPE_BOOLEAN
    | T_TYPE_CHAR
    ;

periods
    : period
    | periods T_COMMA period
    ;

period
    : const_value T_DOTDOT const_value
    ;

var_declarations
    : /* empty */
    | T_VAR var_declaration T_SEMICOLON
    ;

var_declaration
    : id_list T_COLON type_spec
    | var_declaration T_SEMICOLON id_list T_COLON type_spec
    | var_declaration T_SEMICOLON error { yyerrok; }
    ;

subprogram_declarations
    : /* empty */
    | subprogram_declarations subprogram_declaration T_SEMICOLON
    ;

subprogram_declaration
    : subprogram_head subprogram_body
    ;

subprogram_head
    : T_FUNCTION T_IDENTIFIER formal_parameter T_COLON basic_type T_SEMICOLON
    | T_PROCEDURE T_IDENTIFIER formal_parameter T_SEMICOLON
    ;

formal_parameter
    : /* empty */
    | T_LPAREN parameter_lists T_RPAREN
    | T_LPAREN T_RPAREN
    ;

parameter_lists
    : parameter_list
    | parameter_lists T_SEMICOLON parameter_list
    ;

parameter_list
    : var_parameter
    | value_parameter
    ;

var_parameter
    : T_VAR value_parameter
    ;

value_parameter
    : id_list T_COLON basic_type
    ;

subprogram_body
    : const_declarations type_declarations var_declarations compound_statement
    ;

compound_statement
    : T_BEGIN statement_list_opt T_END
    ;

statement_list_opt
    : /* empty */
    | statement_list
    ;

statement_list
    : stmt
    | statement_list T_SEMICOLON stmt
    | statement_list error T_SEMICOLON { yyerrok; }
    ;

stmt
    : identifier_stmt
    | compound_statement
    | if_stmt
    | case_stmt
    | while_stmt
    | repeat_stmt
    | for_stmt
    | read_stmt
    | write_stmt
    | error { yyerrok; }
    | /* empty */
    ;

identifier_stmt
    : T_IDENTIFIER identifier_stmt_tail
    ;

identifier_stmt_tail
    : id_varparts T_ASSIGN expression
    | T_LPAREN expression_list_opt T_RPAREN
    | /* empty */
    ;

if_stmt
    : T_IF expression T_THEN stmt else_part
    ;

else_part
    : /* empty */
    | T_ELSE stmt
    ;

case_stmt
    : T_CASE expression T_OF case_body T_END
    ;

case_body
    : /* empty */
    | branch_list
    ;

branch_list
    : branch
    | branch_list T_SEMICOLON branch
    ;

branch
    : const_list T_COLON stmt
    ;

const_list
    : const_value
    | const_list T_COMMA const_value
    ;

while_stmt
    : T_WHILE expression T_DO stmt
    ;

repeat_stmt
    : T_REPEAT statement_list_opt T_UNTIL expression
    ;

for_stmt
    : T_FOR T_IDENTIFIER T_ASSIGN expression updown expression T_DO stmt
    ;

updown
    : T_TO
    | T_DOWNTO
    ;

read_stmt
    : T_READ T_LPAREN variable_list T_RPAREN
    | T_READLN T_LPAREN variable_list T_RPAREN
    ;

write_stmt
    : T_WRITE T_LPAREN expression_list T_RPAREN
    | T_WRITELN T_LPAREN expression_list T_RPAREN
    | T_WRITELN T_LPAREN T_RPAREN
    | T_WRITELN
    ;

variable_list
    : variable
    | variable_list T_COMMA variable
    ;

variable
    : T_IDENTIFIER id_varparts
    ;

id_varparts
    : /* empty */
    | id_varparts id_varpart
    ;

id_varpart
    : T_LBRACKET expression_list T_RBRACKET
    | T_DOT T_IDENTIFIER
    ;

expression_list_opt
    : /* empty */
    | expression_list
    ;

expression_list
    : expression
    | expression_list T_COMMA expression
    ;

expression
    : simple_expression
    | simple_expression relop simple_expression
    ;

relop
    : T_EQUAL
    | T_NOT_EQUAL
    | T_LESS
    | T_LESS_EQUAL
    | T_GREATER
    | T_GREATER_EQUAL
    ;

simple_expression
    : term
    | simple_expression addop term
    ;

addop
    : T_PLUS
    | T_MINUS
    | T_OR
    ;

term
    : factor
    | term mulop factor
    ;

mulop
    : T_MULTIPLY
    | T_DIVIDE
    | T_DIV
    | T_MOD
    | T_AND
    ;

factor
    : num
    | variable
    | T_IDENTIFIER T_LPAREN expression_list_opt T_RPAREN
    | T_LPAREN expression T_RPAREN
    | T_NOT factor
    | T_MINUS factor
    | T_PLUS factor
    | T_CHAR
    | T_STRING
    | T_BOOLEAN
    ;

%%
