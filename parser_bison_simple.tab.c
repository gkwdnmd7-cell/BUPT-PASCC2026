/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src\\parser_bison_simple.y"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include "token.h"
#include "log.h"

extern "C" int yylex(void);
extern "C" void yyerror(const char* s);

// Semantic value type
union YYSTYPE {
    int intVal;
    double realVal;
    char* strVal;
    bool boolVal;
    char charVal;
};


#line 94 "parser_bison_simple.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser_bison_simple.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_PROGRAM = 3,                  /* T_PROGRAM  */
  YYSYMBOL_T_CONST = 4,                    /* T_CONST  */
  YYSYMBOL_T_TYPE = 5,                     /* T_TYPE  */
  YYSYMBOL_T_VAR = 6,                      /* T_VAR  */
  YYSYMBOL_T_PROCEDURE = 7,                /* T_PROCEDURE  */
  YYSYMBOL_T_FUNCTION = 8,                 /* T_FUNCTION  */
  YYSYMBOL_T_BEGIN = 9,                    /* T_BEGIN  */
  YYSYMBOL_T_END = 10,                     /* T_END  */
  YYSYMBOL_T_IF = 11,                      /* T_IF  */
  YYSYMBOL_T_THEN = 12,                    /* T_THEN  */
  YYSYMBOL_T_ELSE = 13,                    /* T_ELSE  */
  YYSYMBOL_T_CASE = 14,                    /* T_CASE  */
  YYSYMBOL_T_WHILE = 15,                   /* T_WHILE  */
  YYSYMBOL_T_REPEAT = 16,                  /* T_REPEAT  */
  YYSYMBOL_T_UNTIL = 17,                   /* T_UNTIL  */
  YYSYMBOL_T_FOR = 18,                     /* T_FOR  */
  YYSYMBOL_T_TO = 19,                      /* T_TO  */
  YYSYMBOL_T_DOWNTO = 20,                  /* T_DOWNTO  */
  YYSYMBOL_T_DO = 21,                      /* T_DO  */
  YYSYMBOL_T_READ = 22,                    /* T_READ  */
  YYSYMBOL_T_READLN = 23,                  /* T_READLN  */
  YYSYMBOL_T_WRITE = 24,                   /* T_WRITE  */
  YYSYMBOL_T_WRITELN = 25,                 /* T_WRITELN  */
  YYSYMBOL_T_RECORD = 26,                  /* T_RECORD  */
  YYSYMBOL_T_ARRAY = 27,                   /* T_ARRAY  */
  YYSYMBOL_T_OF = 28,                      /* T_OF  */
  YYSYMBOL_T_TYPE_INTEGER = 29,            /* T_TYPE_INTEGER  */
  YYSYMBOL_T_TYPE_REAL = 30,               /* T_TYPE_REAL  */
  YYSYMBOL_T_TYPE_BOOLEAN = 31,            /* T_TYPE_BOOLEAN  */
  YYSYMBOL_T_TYPE_CHAR = 32,               /* T_TYPE_CHAR  */
  YYSYMBOL_T_DIV = 33,                     /* T_DIV  */
  YYSYMBOL_T_MOD = 34,                     /* T_MOD  */
  YYSYMBOL_T_AND = 35,                     /* T_AND  */
  YYSYMBOL_T_OR = 36,                      /* T_OR  */
  YYSYMBOL_T_NOT = 37,                     /* T_NOT  */
  YYSYMBOL_T_IDENTIFIER = 38,              /* T_IDENTIFIER  */
  YYSYMBOL_T_INTEGER = 39,                 /* T_INTEGER  */
  YYSYMBOL_T_REAL = 40,                    /* T_REAL  */
  YYSYMBOL_T_CHAR = 41,                    /* T_CHAR  */
  YYSYMBOL_T_BOOLEAN = 42,                 /* T_BOOLEAN  */
  YYSYMBOL_T_PLUS = 43,                    /* T_PLUS  */
  YYSYMBOL_T_MINUS = 44,                   /* T_MINUS  */
  YYSYMBOL_T_MULTIPLY = 45,                /* T_MULTIPLY  */
  YYSYMBOL_T_DIVIDE = 46,                  /* T_DIVIDE  */
  YYSYMBOL_T_ASSIGN = 47,                  /* T_ASSIGN  */
  YYSYMBOL_T_EQUAL = 48,                   /* T_EQUAL  */
  YYSYMBOL_T_NOT_EQUAL = 49,               /* T_NOT_EQUAL  */
  YYSYMBOL_T_LESS = 50,                    /* T_LESS  */
  YYSYMBOL_T_LESS_EQUAL = 51,              /* T_LESS_EQUAL  */
  YYSYMBOL_T_GREATER = 52,                 /* T_GREATER  */
  YYSYMBOL_T_GREATER_EQUAL = 53,           /* T_GREATER_EQUAL  */
  YYSYMBOL_T_LPAREN = 54,                  /* T_LPAREN  */
  YYSYMBOL_T_RPAREN = 55,                  /* T_RPAREN  */
  YYSYMBOL_T_LBRACKET = 56,                /* T_LBRACKET  */
  YYSYMBOL_T_RBRACKET = 57,                /* T_RBRACKET  */
  YYSYMBOL_T_COMMA = 58,                   /* T_COMMA  */
  YYSYMBOL_T_SEMICOLON = 59,               /* T_SEMICOLON  */
  YYSYMBOL_T_COLON = 60,                   /* T_COLON  */
  YYSYMBOL_T_DOT = 61,                     /* T_DOT  */
  YYSYMBOL_T_DOTDOT = 62,                  /* T_DOTDOT  */
  YYSYMBOL_T_UNKNOWN = 63,                 /* T_UNKNOWN  */
  YYSYMBOL_T_EOF = 64,                     /* T_EOF  */
  YYSYMBOL_YYACCEPT = 65,                  /* $accept  */
  YYSYMBOL_program = 66,                   /* program  */
  YYSYMBOL_program_head = 67,              /* program_head  */
  YYSYMBOL_program_body = 68,              /* program_body  */
  YYSYMBOL_block = 69,                     /* block  */
  YYSYMBOL_declaration_part = 70,          /* declaration_part  */
  YYSYMBOL_constant_declaration_part = 71, /* constant_declaration_part  */
  YYSYMBOL_constant_declaration_list = 72, /* constant_declaration_list  */
  YYSYMBOL_constant_declaration = 73,      /* constant_declaration  */
  YYSYMBOL_constant = 74,                  /* constant  */
  YYSYMBOL_type_declaration_part = 75,     /* type_declaration_part  */
  YYSYMBOL_type_declaration_list = 76,     /* type_declaration_list  */
  YYSYMBOL_type_declaration = 77,          /* type_declaration  */
  YYSYMBOL_type = 78,                      /* type  */
  YYSYMBOL_simple_type = 79,               /* simple_type  */
  YYSYMBOL_array_type = 80,                /* array_type  */
  YYSYMBOL_index_range = 81,               /* index_range  */
  YYSYMBOL_record_type = 82,               /* record_type  */
  YYSYMBOL_field_list = 83,                /* field_list  */
  YYSYMBOL_field_declaration = 84,         /* field_declaration  */
  YYSYMBOL_identifier_list = 85,           /* identifier_list  */
  YYSYMBOL_variable_declaration_part = 86, /* variable_declaration_part  */
  YYSYMBOL_variable_declaration_list = 87, /* variable_declaration_list  */
  YYSYMBOL_variable_declaration = 88,      /* variable_declaration  */
  YYSYMBOL_subprogram_declaration_part = 89, /* subprogram_declaration_part  */
  YYSYMBOL_subprogram_declaration = 90,    /* subprogram_declaration  */
  YYSYMBOL_subprogram_head = 91,           /* subprogram_head  */
  YYSYMBOL_procedure_head = 92,            /* procedure_head  */
  YYSYMBOL_function_head = 93,             /* function_head  */
  YYSYMBOL_formal_parameter_list = 94,     /* formal_parameter_list  */
  YYSYMBOL_parameter_list = 95,            /* parameter_list  */
  YYSYMBOL_parameter_declaration = 96,     /* parameter_declaration  */
  YYSYMBOL_statement_part = 97,            /* statement_part  */
  YYSYMBOL_compound_statement = 98,        /* compound_statement  */
  YYSYMBOL_statement_sequence = 99,        /* statement_sequence  */
  YYSYMBOL_statement = 100,                /* statement  */
  YYSYMBOL_assignment_statement = 101,     /* assignment_statement  */
  YYSYMBOL_if_statement = 102,             /* if_statement  */
  YYSYMBOL_case_statement = 103,           /* case_statement  */
  YYSYMBOL_case_list = 104,                /* case_list  */
  YYSYMBOL_case_element = 105,             /* case_element  */
  YYSYMBOL_constant_list = 106,            /* constant_list  */
  YYSYMBOL_while_statement = 107,          /* while_statement  */
  YYSYMBOL_repeat_statement = 108,         /* repeat_statement  */
  YYSYMBOL_for_statement = 109,            /* for_statement  */
  YYSYMBOL_read_statement = 110,           /* read_statement  */
  YYSYMBOL_write_statement = 111,          /* write_statement  */
  YYSYMBOL_variable = 112,                 /* variable  */
  YYSYMBOL_variable_list = 113,            /* variable_list  */
  YYSYMBOL_expression_list = 114,          /* expression_list  */
  YYSYMBOL_expression = 115,               /* expression  */
  YYSYMBOL_simple_expression = 116,        /* simple_expression  */
  YYSYMBOL_term = 117,                     /* term  */
  YYSYMBOL_factor = 118,                   /* factor  */
  YYSYMBOL_relational_operator = 119,      /* relational_operator  */
  YYSYMBOL_adding_operator = 120,          /* adding_operator  */
  YYSYMBOL_multiplying_operator = 121,     /* multiplying_operator  */
  YYSYMBOL_sign = 122                      /* sign  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   216

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  58
/* YYNRULES -- Number of rules.  */
#define YYNRULES  124
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  225

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   319


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    65,    65,    72,    80,    84,    87,    89,    90,    91,
      92,    93,    94,    95,    96,   100,   104,   105,   109,   117,
     118,   119,   120,   121,   125,   129,   130,   134,   142,   143,
     144,   148,   149,   150,   151,   152,   156,   160,   164,   168,
     169,   173,   177,   178,   182,   186,   187,   191,   198,   199,
     203,   207,   208,   212,   220,   227,   229,   233,   234,   238,
     242,   246,   250,   251,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   268,   275,   276,   280,   284,   285,
     289,   293,   294,   298,   302,   306,   307,   311,   312,   316,
     317,   321,   322,   323,   327,   328,   332,   333,   337,   338,
     342,   343,   344,   348,   349,   353,   354,   355,   356,   360,
     361,   362,   363,   364,   365,   369,   370,   371,   375,   376,
     377,   378,   379,   383,   384
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "T_PROGRAM", "T_CONST",
  "T_TYPE", "T_VAR", "T_PROCEDURE", "T_FUNCTION", "T_BEGIN", "T_END",
  "T_IF", "T_THEN", "T_ELSE", "T_CASE", "T_WHILE", "T_REPEAT", "T_UNTIL",
  "T_FOR", "T_TO", "T_DOWNTO", "T_DO", "T_READ", "T_READLN", "T_WRITE",
  "T_WRITELN", "T_RECORD", "T_ARRAY", "T_OF", "T_TYPE_INTEGER",
  "T_TYPE_REAL", "T_TYPE_BOOLEAN", "T_TYPE_CHAR", "T_DIV", "T_MOD",
  "T_AND", "T_OR", "T_NOT", "T_IDENTIFIER", "T_INTEGER", "T_REAL",
  "T_CHAR", "T_BOOLEAN", "T_PLUS", "T_MINUS", "T_MULTIPLY", "T_DIVIDE",
  "T_ASSIGN", "T_EQUAL", "T_NOT_EQUAL", "T_LESS", "T_LESS_EQUAL",
  "T_GREATER", "T_GREATER_EQUAL", "T_LPAREN", "T_RPAREN", "T_LBRACKET",
  "T_RBRACKET", "T_COMMA", "T_SEMICOLON", "T_COLON", "T_DOT", "T_DOTDOT",
  "T_UNKNOWN", "T_EOF", "$accept", "program", "program_head",
  "program_body", "block", "declaration_part", "constant_declaration_part",
  "constant_declaration_list", "constant_declaration", "constant",
  "type_declaration_part", "type_declaration_list", "type_declaration",
  "type", "simple_type", "array_type", "index_range", "record_type",
  "field_list", "field_declaration", "identifier_list",
  "variable_declaration_part", "variable_declaration_list",
  "variable_declaration", "subprogram_declaration_part",
  "subprogram_declaration", "subprogram_head", "procedure_head",
  "function_head", "formal_parameter_list", "parameter_list",
  "parameter_declaration", "statement_part", "compound_statement",
  "statement_sequence", "statement", "assignment_statement",
  "if_statement", "case_statement", "case_list", "case_element",
  "constant_list", "while_statement", "repeat_statement", "for_statement",
  "read_statement", "write_statement", "variable", "variable_list",
  "expression_list", "expression", "simple_expression", "term", "factor",
  "relational_operator", "adding_operator", "multiplying_operator", "sign", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-120)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-92)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      13,   -21,    26,   131,   -29,  -120,    63,    85,    87,   116,
     117,  -120,    99,   143,  -120,  -120,  -120,    64,  -120,   106,
    -120,  -120,  -120,     3,    63,  -120,   113,    85,  -120,  -120,
     -10,    87,  -120,   112,   112,  -120,     9,  -120,  -120,  -120,
      64,  -120,  -120,  -120,   131,   103,  -120,   102,  -120,   130,
     102,  -120,    87,  -120,   109,    25,    25,    25,     9,   132,
     118,   119,   120,   121,  -120,  -120,    -1,  -120,  -120,  -120,
    -120,  -120,  -120,  -120,  -120,  -120,   -18,   122,  -120,  -120,
    -120,  -120,  -120,   123,    87,   124,  -120,  -120,  -120,  -120,
    -120,   125,  -120,  -120,  -120,  -120,   128,    18,   -40,  -120,
      89,    68,    -7,  -120,  -120,    25,  -120,    14,   164,    47,
      11,  -120,    68,   149,   157,    -6,   136,   141,   141,    25,
      25,  -120,     9,    25,    25,   150,  -120,  -120,     0,  -120,
      29,   151,  -120,  -120,   102,  -120,    87,  -120,  -120,   134,
       9,  -120,  -120,  -120,  -120,  -120,  -120,  -120,  -120,  -120,
      25,    68,  -120,  -120,  -120,  -120,  -120,    68,    11,   103,
       9,    25,    25,    14,   -16,    22,    59,  -120,    98,  -120,
    -120,   135,  -120,  -120,    87,   102,   129,   137,  -120,  -120,
    -120,   180,    -8,    11,  -120,  -120,     2,  -120,    55,  -120,
    -120,    74,  -120,   141,  -120,  -120,    25,  -120,  -120,  -120,
    -120,   156,   168,     9,  -120,   103,   103,     9,    25,    25,
      14,  -120,  -120,   102,  -120,  -120,  -120,  -120,   176,   177,
    -120,     9,     9,  -120,  -120
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     6,     0,     1,     0,     0,     0,     0,
       0,     2,     0,     0,     7,     8,     9,    10,    48,     0,
      51,    52,     3,     0,    15,    16,     0,    24,    25,    42,
       0,    44,    45,    55,    55,     4,    73,    11,    12,    13,
      14,     5,    60,    49,     6,     0,    17,     0,    26,     0,
       0,    46,     0,    53,     0,     0,     0,     0,    73,     0,
       0,     0,     0,     0,    91,    72,     0,    62,    64,    65,
      66,    67,    68,    69,    70,    71,     0,     0,    23,    19,
      20,    21,    22,     0,     0,     0,    31,    32,    33,    34,
      35,     0,    28,    29,    30,    43,     0,     0,     0,    57,
       0,     0,    23,   123,   124,     0,   106,   105,     0,    98,
     100,   103,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    73,     0,     0,     0,    50,    18,     0,    39,
       0,     0,    27,    47,     0,    56,     0,    54,   108,     0,
      73,   117,   115,   116,   109,   110,   111,   112,   113,   114,
       0,     0,   120,   121,   122,   118,   119,     0,   101,     0,
      73,     0,     0,    94,     0,     0,     0,    96,     0,    63,
      74,     0,    93,    38,     0,     0,     0,     0,    59,    58,
     107,    75,    99,   102,   104,    81,     0,    78,     0,    83,
      84,     0,    87,     0,    88,    89,     0,    90,    92,    40,
      41,     0,     0,    73,    77,     0,     0,    73,     0,     0,
      95,    97,    37,     0,    76,    79,    82,    80,     0,     0,
      36,    73,    73,    85,    86
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -120,  -120,  -120,  -120,   155,  -120,   187,  -120,   178,   -43,
     188,  -120,   179,   -49,   104,  -120,  -120,  -120,  -120,    31,
     -44,   190,  -120,   181,   194,    -3,  -120,  -120,  -120,   174,
    -120,    73,  -120,   197,   153,  -119,  -120,  -120,  -120,  -120,
       8,  -120,  -120,  -120,  -120,  -120,  -120,   -36,    96,    95,
     -50,    66,   -99,   -97,  -120,  -120,  -120,  -120
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,    11,    12,    13,    14,    24,    25,   106,
      15,    27,    28,    91,    92,    93,   177,    94,   128,   129,
      30,    16,    31,    32,    17,    18,    19,    20,    21,    53,
      98,    99,    41,    65,    66,    67,    68,    69,    70,   186,
     187,   188,    71,    72,    73,    74,    75,   107,   164,   166,
     167,   109,   110,   111,   150,   151,   157,   112
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,    96,    83,   169,   138,   108,   113,   114,    97,   121,
     173,   161,   204,   158,    43,   135,     1,     4,    36,   136,
      55,   181,    76,    56,    57,    58,     5,    59,   141,   123,
      22,    60,    61,    62,    63,   142,   143,    43,   124,   192,
     130,   189,   193,   125,   152,   153,   154,    64,    49,   -91,
      50,    45,   183,   122,   -91,   139,   155,   156,   122,   174,
     184,   205,   101,   102,    79,    80,    81,    82,   103,   104,
     124,     9,    10,   170,   171,   125,    49,   194,   134,   105,
     193,   163,   163,   141,   214,   178,    76,    49,   217,   175,
     142,   143,    97,   208,   209,   144,   145,   146,   147,   148,
     149,    23,   223,   224,    76,   101,   102,    79,    80,    81,
      82,   190,   191,   206,   195,   207,   185,   196,    86,    87,
      88,    89,   105,    26,    76,    29,   200,    90,    84,    85,
     130,    86,    87,    88,    89,     6,     7,     8,     9,    10,
      90,    78,    79,    80,    81,    82,   211,     6,     7,     8,
       9,    10,    36,   197,    33,    34,   196,   210,   218,   219,
      35,    47,   185,   216,   220,    44,    52,    76,    95,   100,
     116,    76,   117,   118,   119,   120,   140,   159,   160,    64,
     131,   126,   127,   162,   132,    76,    76,   133,   172,   180,
     176,   201,   198,   203,   202,   212,   213,   221,   222,    77,
      37,    38,    46,    39,   137,   199,    48,    40,    54,   179,
      42,   115,    51,   215,   165,   168,   182
};

static const yytype_uint8 yycheck[] =
{
      36,    50,    45,   122,   101,    55,    56,    57,    52,    10,
      10,    17,    10,   112,    17,    55,     3,    38,     9,    59,
      11,   140,    58,    14,    15,    16,     0,    18,    36,    47,
      59,    22,    23,    24,    25,    43,    44,    40,    56,    55,
      84,   160,    58,    61,    33,    34,    35,    38,    58,    56,
      60,    48,   151,    59,    61,   105,    45,    46,    59,    59,
     157,    59,    37,    38,    39,    40,    41,    42,    43,    44,
      56,     7,     8,   123,   124,    61,    58,    55,    60,    54,
      58,   117,   118,    36,   203,   134,   122,    58,   207,    60,
      43,    44,   136,    19,    20,    48,    49,    50,    51,    52,
      53,    38,   221,   222,   140,    37,    38,    39,    40,    41,
      42,   161,   162,    58,    55,    60,   159,    58,    29,    30,
      31,    32,    54,    38,   160,    38,   175,    38,    26,    27,
     174,    29,    30,    31,    32,     4,     5,     6,     7,     8,
      38,    38,    39,    40,    41,    42,   196,     4,     5,     6,
       7,     8,     9,    55,    38,    38,    58,   193,   208,   209,
      61,    48,   205,   206,   213,    59,    54,   203,    38,    60,
      38,   207,    54,    54,    54,    54,    12,    28,    21,    38,
      56,    59,    59,    47,    59,   221,   222,    59,    38,    55,
      39,    62,    57,    13,    57,    39,    28,    21,    21,    44,
      13,    13,    24,    13,   100,   174,    27,    13,    34,   136,
      13,    58,    31,   205,   118,   120,   150
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    66,    67,    38,     0,     4,     5,     6,     7,
       8,    68,    69,    70,    71,    75,    86,    89,    90,    91,
      92,    93,    59,    38,    72,    73,    38,    76,    77,    38,
      85,    87,    88,    38,    38,    61,     9,    71,    75,    86,
      89,    97,    98,    90,    59,    48,    73,    48,    77,    58,
      60,    88,    54,    94,    94,    11,    14,    15,    16,    18,
      22,    23,    24,    25,    38,    98,    99,   100,   101,   102,
     103,   107,   108,   109,   110,   111,   112,    69,    38,    39,
      40,    41,    42,    74,    26,    27,    29,    30,    31,    32,
      38,    78,    79,    80,    82,    38,    78,    85,    95,    96,
      60,    37,    38,    43,    44,    54,    74,   112,   115,   116,
     117,   118,   122,   115,   115,    99,    38,    54,    54,    54,
      54,    10,    59,    47,    56,    61,    59,    59,    83,    84,
      85,    56,    59,    59,    60,    55,    59,    79,   118,   115,
      12,    36,    43,    44,    48,    49,    50,    51,    52,    53,
     119,   120,    33,    34,    35,    45,    46,   121,   117,    28,
      21,    17,    47,   112,   113,   113,   114,   115,   114,   100,
     115,   115,    38,    10,    59,    60,    39,    81,    78,    96,
      55,   100,   116,   117,   118,    74,   104,   105,   106,   100,
     115,   115,    55,    58,    55,    55,    58,    55,    57,    84,
      78,    62,    57,    13,    10,    59,    58,    60,    19,    20,
     112,   115,    39,    28,   100,   105,    74,   100,   115,   115,
      78,    21,    21,   100,   100
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    65,    66,    67,    68,    69,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    71,    72,    72,    73,    74,
      74,    74,    74,    74,    75,    76,    76,    77,    78,    78,
      78,    79,    79,    79,    79,    79,    80,    81,    82,    83,
      83,    84,    85,    85,    86,    87,    87,    88,    89,    89,
      90,    91,    91,    92,    93,    94,    94,    95,    95,    96,
      97,    98,    99,    99,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   101,   102,   102,   103,   104,   104,
     105,   106,   106,   107,   108,   109,   109,   110,   110,   111,
     111,   112,   112,   112,   113,   113,   114,   114,   115,   115,
     116,   116,   116,   117,   117,   118,   118,   118,   118,   119,
     119,   119,   119,   119,   119,   120,   120,   120,   121,   121,
     121,   121,   121,   122,   122
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     3,     2,     2,     0,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     1,     2,     4,     1,
       1,     1,     1,     1,     2,     1,     2,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     6,     3,     3,     1,
       3,     3,     1,     3,     2,     1,     2,     4,     1,     2,
       4,     1,     1,     3,     5,     0,     3,     1,     3,     3,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     4,     6,     5,     1,     3,
       3,     1,     3,     4,     4,     8,     8,     4,     4,     4,
       4,     1,     4,     3,     1,     3,     1,     3,     1,     3,
       1,     2,     3,     1,     3,     1,     1,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: program_head program_body  */
#line 66 "src\\parser_bison_simple.y"
    {
        LOG_INFO("Program parsed successfully");
    }
#line 1377 "parser_bison_simple.tab.c"
    break;

  case 3: /* program_head: T_PROGRAM T_IDENTIFIER T_SEMICOLON  */
#line 73 "src\\parser_bison_simple.y"
    {
        LOG_INFO("Program header: %s", (yyvsp[-1].strVal));
        free((yyvsp[-1].strVal));
    }
#line 1386 "parser_bison_simple.tab.c"
    break;

  case 18: /* constant_declaration: T_IDENTIFIER T_EQUAL constant T_SEMICOLON  */
#line 110 "src\\parser_bison_simple.y"
    {
        LOG_INFO("Constant declaration: %s", (yyvsp[-3].strVal));
        free((yyvsp[-3].strVal));
    }
#line 1395 "parser_bison_simple.tab.c"
    break;

  case 27: /* type_declaration: T_IDENTIFIER T_EQUAL type T_SEMICOLON  */
#line 135 "src\\parser_bison_simple.y"
    {
        LOG_INFO("Type declaration: %s", (yyvsp[-3].strVal));
        free((yyvsp[-3].strVal));
    }
#line 1404 "parser_bison_simple.tab.c"
    break;

  case 47: /* variable_declaration: identifier_list T_COLON type T_SEMICOLON  */
#line 192 "src\\parser_bison_simple.y"
    {
        LOG_INFO("Variable declaration");
    }
#line 1412 "parser_bison_simple.tab.c"
    break;

  case 53: /* procedure_head: T_PROCEDURE T_IDENTIFIER formal_parameter_list  */
#line 213 "src\\parser_bison_simple.y"
    {
        LOG_INFO("Procedure declaration: %s", (yyvsp[-1].strVal));
        free((yyvsp[-1].strVal));
    }
#line 1421 "parser_bison_simple.tab.c"
    break;

  case 54: /* function_head: T_FUNCTION T_IDENTIFIER formal_parameter_list T_COLON simple_type  */
#line 221 "src\\parser_bison_simple.y"
    {
        LOG_INFO("Function declaration: %s", (yyvsp[-3].strVal));
        free((yyvsp[-3].strVal));
    }
#line 1430 "parser_bison_simple.tab.c"
    break;

  case 74: /* assignment_statement: variable T_ASSIGN expression  */
#line 269 "src\\parser_bison_simple.y"
    {
        LOG_INFO("Assignment statement");
    }
#line 1438 "parser_bison_simple.tab.c"
    break;


#line 1442 "parser_bison_simple.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 387 "src\\parser_bison_simple.y"


void yyerror(const char* s) {
    fprintf(stderr, "Parser error: %s\n", s);
}

int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "Cannot open file: %s\n", argv[1]);
            return 1;
        }
        yyin = input;
    }
    
    LOG_INFO("Starting parser...");
    int result = yyparse();
    
    if (result == 0) {
        LOG_INFO("Parse successful");
    } else {
        LOG_ERROR("Parse failed");
    }
    
    return result;
}
