/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_D_CODE_BUILD_DEMO_PARSER_BISON_TAB_H_INCLUDED
# define YY_YY_D_CODE_BUILD_DEMO_PARSER_BISON_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"

    #include "ast.h"
    #include <string>
    #include <vector>

#line 55 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    T_IDENTIFIER = 258,            /* T_IDENTIFIER  */
    T_INTEGER = 259,               /* T_INTEGER  */
    T_REAL = 260,                  /* T_REAL  */
    T_CHAR = 261,                  /* T_CHAR  */
    T_STRING = 262,                /* T_STRING  */
    T_BOOLEAN = 263,               /* T_BOOLEAN  */
    T_UNKNOWN = 264,               /* T_UNKNOWN  */
    T_PROGRAM = 265,               /* T_PROGRAM  */
    T_CONST = 266,                 /* T_CONST  */
    T_TYPE = 267,                  /* T_TYPE  */
    T_VAR = 268,                   /* T_VAR  */
    T_PROCEDURE = 269,             /* T_PROCEDURE  */
    T_FUNCTION = 270,              /* T_FUNCTION  */
    T_BEGIN = 271,                 /* T_BEGIN  */
    T_END = 272,                   /* T_END  */
    T_IF = 273,                    /* T_IF  */
    T_THEN = 274,                  /* T_THEN  */
    T_ELSE = 275,                  /* T_ELSE  */
    T_CASE = 276,                  /* T_CASE  */
    T_WHILE = 277,                 /* T_WHILE  */
    T_REPEAT = 278,                /* T_REPEAT  */
    T_UNTIL = 279,                 /* T_UNTIL  */
    T_FOR = 280,                   /* T_FOR  */
    T_TO = 281,                    /* T_TO  */
    T_DOWNTO = 282,                /* T_DOWNTO  */
    T_DO = 283,                    /* T_DO  */
    T_READ = 284,                  /* T_READ  */
    T_READLN = 285,                /* T_READLN  */
    T_WRITE = 286,                 /* T_WRITE  */
    T_WRITELN = 287,               /* T_WRITELN  */
    T_BREAK = 288,                 /* T_BREAK  */
    T_CONTINUE = 289,              /* T_CONTINUE  */
    T_EXIT = 290,                  /* T_EXIT  */
    T_RECORD = 291,                /* T_RECORD  */
    T_ARRAY = 292,                 /* T_ARRAY  */
    T_OF = 293,                    /* T_OF  */
    T_TYPE_INTEGER = 294,          /* T_TYPE_INTEGER  */
    T_TYPE_REAL = 295,             /* T_TYPE_REAL  */
    T_TYPE_BOOLEAN = 296,          /* T_TYPE_BOOLEAN  */
    T_TYPE_CHAR = 297,             /* T_TYPE_CHAR  */
    T_PLUS = 298,                  /* T_PLUS  */
    T_MINUS = 299,                 /* T_MINUS  */
    T_MULTIPLY = 300,              /* T_MULTIPLY  */
    T_DIVIDE = 301,                /* T_DIVIDE  */
    T_DIV = 302,                   /* T_DIV  */
    T_MOD = 303,                   /* T_MOD  */
    T_AND = 304,                   /* T_AND  */
    T_OR = 305,                    /* T_OR  */
    T_NOT = 306,                   /* T_NOT  */
    T_ASSIGN = 307,                /* T_ASSIGN  */
    T_EQUAL = 308,                 /* T_EQUAL  */
    T_NOT_EQUAL = 309,             /* T_NOT_EQUAL  */
    T_LESS = 310,                  /* T_LESS  */
    T_LESS_EQUAL = 311,            /* T_LESS_EQUAL  */
    T_GREATER = 312,               /* T_GREATER  */
    T_GREATER_EQUAL = 313,         /* T_GREATER_EQUAL  */
    T_LPAREN = 314,                /* T_LPAREN  */
    T_RPAREN = 315,                /* T_RPAREN  */
    T_LBRACKET = 316,              /* T_LBRACKET  */
    T_RBRACKET = 317,              /* T_RBRACKET  */
    T_COMMA = 318,                 /* T_COMMA  */
    T_SEMICOLON = 319,             /* T_SEMICOLON  */
    T_COLON = 320,                 /* T_COLON  */
    T_DOT = 321,                   /* T_DOT  */
    T_DOTDOT = 322                 /* T_DOTDOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 15 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"

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

#line 155 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_D_CODE_BUILD_DEMO_PARSER_BISON_TAB_H_INCLUDED  */
