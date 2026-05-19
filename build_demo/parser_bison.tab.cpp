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

#include "parser_bison.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_IDENTIFIER = 3,               /* T_IDENTIFIER  */
  YYSYMBOL_T_INTEGER = 4,                  /* T_INTEGER  */
  YYSYMBOL_T_REAL = 5,                     /* T_REAL  */
  YYSYMBOL_T_CHAR = 6,                     /* T_CHAR  */
  YYSYMBOL_T_STRING = 7,                   /* T_STRING  */
  YYSYMBOL_T_BOOLEAN = 8,                  /* T_BOOLEAN  */
  YYSYMBOL_T_UNKNOWN = 9,                  /* T_UNKNOWN  */
  YYSYMBOL_T_PROGRAM = 10,                 /* T_PROGRAM  */
  YYSYMBOL_T_CONST = 11,                   /* T_CONST  */
  YYSYMBOL_T_TYPE = 12,                    /* T_TYPE  */
  YYSYMBOL_T_VAR = 13,                     /* T_VAR  */
  YYSYMBOL_T_PROCEDURE = 14,               /* T_PROCEDURE  */
  YYSYMBOL_T_FUNCTION = 15,                /* T_FUNCTION  */
  YYSYMBOL_T_BEGIN = 16,                   /* T_BEGIN  */
  YYSYMBOL_T_END = 17,                     /* T_END  */
  YYSYMBOL_T_IF = 18,                      /* T_IF  */
  YYSYMBOL_T_THEN = 19,                    /* T_THEN  */
  YYSYMBOL_T_ELSE = 20,                    /* T_ELSE  */
  YYSYMBOL_T_CASE = 21,                    /* T_CASE  */
  YYSYMBOL_T_WHILE = 22,                   /* T_WHILE  */
  YYSYMBOL_T_REPEAT = 23,                  /* T_REPEAT  */
  YYSYMBOL_T_UNTIL = 24,                   /* T_UNTIL  */
  YYSYMBOL_T_FOR = 25,                     /* T_FOR  */
  YYSYMBOL_T_TO = 26,                      /* T_TO  */
  YYSYMBOL_T_DOWNTO = 27,                  /* T_DOWNTO  */
  YYSYMBOL_T_DO = 28,                      /* T_DO  */
  YYSYMBOL_T_READ = 29,                    /* T_READ  */
  YYSYMBOL_T_READLN = 30,                  /* T_READLN  */
  YYSYMBOL_T_WRITE = 31,                   /* T_WRITE  */
  YYSYMBOL_T_WRITELN = 32,                 /* T_WRITELN  */
  YYSYMBOL_T_BREAK = 33,                   /* T_BREAK  */
  YYSYMBOL_T_CONTINUE = 34,                /* T_CONTINUE  */
  YYSYMBOL_T_EXIT = 35,                    /* T_EXIT  */
  YYSYMBOL_T_RECORD = 36,                  /* T_RECORD  */
  YYSYMBOL_T_ARRAY = 37,                   /* T_ARRAY  */
  YYSYMBOL_T_OF = 38,                      /* T_OF  */
  YYSYMBOL_T_TYPE_INTEGER = 39,            /* T_TYPE_INTEGER  */
  YYSYMBOL_T_TYPE_REAL = 40,               /* T_TYPE_REAL  */
  YYSYMBOL_T_TYPE_BOOLEAN = 41,            /* T_TYPE_BOOLEAN  */
  YYSYMBOL_T_TYPE_CHAR = 42,               /* T_TYPE_CHAR  */
  YYSYMBOL_T_PLUS = 43,                    /* T_PLUS  */
  YYSYMBOL_T_MINUS = 44,                   /* T_MINUS  */
  YYSYMBOL_T_MULTIPLY = 45,                /* T_MULTIPLY  */
  YYSYMBOL_T_DIVIDE = 46,                  /* T_DIVIDE  */
  YYSYMBOL_T_DIV = 47,                     /* T_DIV  */
  YYSYMBOL_T_MOD = 48,                     /* T_MOD  */
  YYSYMBOL_T_AND = 49,                     /* T_AND  */
  YYSYMBOL_T_OR = 50,                      /* T_OR  */
  YYSYMBOL_T_NOT = 51,                     /* T_NOT  */
  YYSYMBOL_T_ASSIGN = 52,                  /* T_ASSIGN  */
  YYSYMBOL_T_EQUAL = 53,                   /* T_EQUAL  */
  YYSYMBOL_T_NOT_EQUAL = 54,               /* T_NOT_EQUAL  */
  YYSYMBOL_T_LESS = 55,                    /* T_LESS  */
  YYSYMBOL_T_LESS_EQUAL = 56,              /* T_LESS_EQUAL  */
  YYSYMBOL_T_GREATER = 57,                 /* T_GREATER  */
  YYSYMBOL_T_GREATER_EQUAL = 58,           /* T_GREATER_EQUAL  */
  YYSYMBOL_T_LPAREN = 59,                  /* T_LPAREN  */
  YYSYMBOL_T_RPAREN = 60,                  /* T_RPAREN  */
  YYSYMBOL_T_LBRACKET = 61,                /* T_LBRACKET  */
  YYSYMBOL_T_RBRACKET = 62,                /* T_RBRACKET  */
  YYSYMBOL_T_COMMA = 63,                   /* T_COMMA  */
  YYSYMBOL_T_SEMICOLON = 64,               /* T_SEMICOLON  */
  YYSYMBOL_T_COLON = 65,                   /* T_COLON  */
  YYSYMBOL_T_DOT = 66,                     /* T_DOT  */
  YYSYMBOL_T_DOTDOT = 67,                  /* T_DOTDOT  */
  YYSYMBOL_YYACCEPT = 68,                  /* $accept  */
  YYSYMBOL_program = 69,                   /* program  */
  YYSYMBOL_program_head = 70,              /* program_head  */
  YYSYMBOL_program_body = 71,              /* program_body  */
  YYSYMBOL_id_list = 72,                   /* id_list  */
  YYSYMBOL_const_declarations = 73,        /* const_declarations  */
  YYSYMBOL_const_declaration = 74,         /* const_declaration  */
  YYSYMBOL_const_value = 75,               /* const_value  */
  YYSYMBOL_num = 76,                       /* num  */
  YYSYMBOL_type_declarations = 77,         /* type_declarations  */
  YYSYMBOL_type_declaration = 78,          /* type_declaration  */
  YYSYMBOL_type_spec = 79,                 /* type_spec  */
  YYSYMBOL_record_body = 80,               /* record_body  */
  YYSYMBOL_basic_type = 81,                /* basic_type  */
  YYSYMBOL_periods = 82,                   /* periods  */
  YYSYMBOL_period = 83,                    /* period  */
  YYSYMBOL_var_declarations = 84,          /* var_declarations  */
  YYSYMBOL_var_declaration = 85,           /* var_declaration  */
  YYSYMBOL_subprogram_declarations = 86,   /* subprogram_declarations  */
  YYSYMBOL_subprogram_declaration = 87,    /* subprogram_declaration  */
  YYSYMBOL_subprogram_head = 88,           /* subprogram_head  */
  YYSYMBOL_formal_parameter = 89,          /* formal_parameter  */
  YYSYMBOL_parameter_lists = 90,           /* parameter_lists  */
  YYSYMBOL_parameter_list = 91,            /* parameter_list  */
  YYSYMBOL_var_parameter = 92,             /* var_parameter  */
  YYSYMBOL_value_parameter = 93,           /* value_parameter  */
  YYSYMBOL_subprogram_body = 94,           /* subprogram_body  */
  YYSYMBOL_compound_statement = 95,        /* compound_statement  */
  YYSYMBOL_statement_list_opt = 96,        /* statement_list_opt  */
  YYSYMBOL_statement_list = 97,            /* statement_list  */
  YYSYMBOL_stmt = 98,                      /* stmt  */
  YYSYMBOL_break_stmt = 99,                /* break_stmt  */
  YYSYMBOL_continue_stmt = 100,            /* continue_stmt  */
  YYSYMBOL_exit_stmt = 101,                /* exit_stmt  */
  YYSYMBOL_identifier_stmt = 102,          /* identifier_stmt  */
  YYSYMBOL_identifier_stmt_tail = 103,     /* identifier_stmt_tail  */
  YYSYMBOL_if_stmt = 104,                  /* if_stmt  */
  YYSYMBOL_else_part = 105,                /* else_part  */
  YYSYMBOL_case_stmt = 106,                /* case_stmt  */
  YYSYMBOL_case_body = 107,                /* case_body  */
  YYSYMBOL_branch_list = 108,              /* branch_list  */
  YYSYMBOL_branch = 109,                   /* branch  */
  YYSYMBOL_const_list = 110,               /* const_list  */
  YYSYMBOL_while_stmt = 111,               /* while_stmt  */
  YYSYMBOL_repeat_stmt = 112,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 113,                 /* for_stmt  */
  YYSYMBOL_updown = 114,                   /* updown  */
  YYSYMBOL_read_stmt = 115,                /* read_stmt  */
  YYSYMBOL_write_stmt = 116,               /* write_stmt  */
  YYSYMBOL_variable_list = 117,            /* variable_list  */
  YYSYMBOL_variable = 118,                 /* variable  */
  YYSYMBOL_id_varparts = 119,              /* id_varparts  */
  YYSYMBOL_id_varpart = 120,               /* id_varpart  */
  YYSYMBOL_expression_list_opt = 121,      /* expression_list_opt  */
  YYSYMBOL_expression_list = 122,          /* expression_list  */
  YYSYMBOL_expression = 123,               /* expression  */
  YYSYMBOL_relop = 124,                    /* relop  */
  YYSYMBOL_simple_expression = 125,        /* simple_expression  */
  YYSYMBOL_addop = 126,                    /* addop  */
  YYSYMBOL_term = 127,                     /* term  */
  YYSYMBOL_mulop = 128,                    /* mulop  */
  YYSYMBOL_factor = 129                    /* factor  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 8 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"

    extern AstNode* g_ast_root;
    int yylex(void);
    void yyerror(const char* msg);

#line 239 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"

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
typedef yytype_int16 yy_state_t;

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
#define YYLAST   343

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  62
/* YYNRULES -- Number of rules.  */
#define YYNRULES  154
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  270

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   322


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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    96,    96,   106,   113,   119,   128,   143,   145,   154,
     155,   159,   166,   173,   177,   183,   189,   190,   197,   211,
     218,   225,   232,   242,   249,   263,   264,   268,   276,   287,
     289,   299,   307,   312,   313,   314,   318,   319,   320,   321,
     325,   327,   332,   347,   348,   352,   360,   368,   376,   377,
     386,   395,   407,   421,   422,   423,   427,   429,   434,   435,
     439,   444,   455,   471,   483,   484,   488,   490,   492,   500,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     511,   512,   513,   517,   521,   525,   527,   532,   552,   560,
     568,   576,   581,   582,   586,   599,   600,   604,   606,   611,
     621,   628,   638,   643,   655,   668,   669,   673,   680,   690,
     697,   704,   710,   723,   725,   730,   739,   740,   749,   756,
     770,   771,   775,   777,   782,   784,   789,   790,   791,   792,
     793,   794,   798,   799,   804,   805,   806,   810,   811,   816,
     817,   818,   819,   820,   824,   826,   828,   830,   836,   837,
     838,   839,   840,   842,   844
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
  "\"end of file\"", "error", "\"invalid token\"", "T_IDENTIFIER",
  "T_INTEGER", "T_REAL", "T_CHAR", "T_STRING", "T_BOOLEAN", "T_UNKNOWN",
  "T_PROGRAM", "T_CONST", "T_TYPE", "T_VAR", "T_PROCEDURE", "T_FUNCTION",
  "T_BEGIN", "T_END", "T_IF", "T_THEN", "T_ELSE", "T_CASE", "T_WHILE",
  "T_REPEAT", "T_UNTIL", "T_FOR", "T_TO", "T_DOWNTO", "T_DO", "T_READ",
  "T_READLN", "T_WRITE", "T_WRITELN", "T_BREAK", "T_CONTINUE", "T_EXIT",
  "T_RECORD", "T_ARRAY", "T_OF", "T_TYPE_INTEGER", "T_TYPE_REAL",
  "T_TYPE_BOOLEAN", "T_TYPE_CHAR", "T_PLUS", "T_MINUS", "T_MULTIPLY",
  "T_DIVIDE", "T_DIV", "T_MOD", "T_AND", "T_OR", "T_NOT", "T_ASSIGN",
  "T_EQUAL", "T_NOT_EQUAL", "T_LESS", "T_LESS_EQUAL", "T_GREATER",
  "T_GREATER_EQUAL", "T_LPAREN", "T_RPAREN", "T_LBRACKET", "T_RBRACKET",
  "T_COMMA", "T_SEMICOLON", "T_COLON", "T_DOT", "T_DOTDOT", "$accept",
  "program", "program_head", "program_body", "id_list",
  "const_declarations", "const_declaration", "const_value", "num",
  "type_declarations", "type_declaration", "type_spec", "record_body",
  "basic_type", "periods", "period", "var_declarations", "var_declaration",
  "subprogram_declarations", "subprogram_declaration", "subprogram_head",
  "formal_parameter", "parameter_lists", "parameter_list", "var_parameter",
  "value_parameter", "subprogram_body", "compound_statement",
  "statement_list_opt", "statement_list", "stmt", "break_stmt",
  "continue_stmt", "exit_stmt", "identifier_stmt", "identifier_stmt_tail",
  "if_stmt", "else_part", "case_stmt", "case_body", "branch_list",
  "branch", "const_list", "while_stmt", "repeat_stmt", "for_stmt",
  "updown", "read_stmt", "write_stmt", "variable_list", "variable",
  "id_varparts", "id_varpart", "expression_list_opt", "expression_list",
  "expression", "relop", "simple_expression", "addop", "term", "mulop",
  "factor", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-145)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-117)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      31,    56,    63,    75,   -25,  -145,    87,    42,    99,    25,
    -145,    62,    58,  -145,   143,   152,  -145,   105,    43,   257,
      30,   123,   121,   202,  -145,  -145,   160,   223,  -145,  -145,
    -145,  -145,  -145,  -145,   198,   211,  -145,  -145,  -145,   175,
     122,   233,   -44,   184,   252,  -145,  -145,  -145,  -145,  -145,
    -145,   257,  -145,   202,   178,  -145,  -145,  -145,  -145,  -145,
    -145,   219,   122,    86,   266,   270,   165,   210,    75,  -145,
    -145,   258,   212,   257,   122,  -145,  -145,    -7,   218,   218,
    -145,    -4,   227,   227,   227,   188,   276,   225,   226,   230,
     238,  -145,  -145,   239,  -145,   265,    12,  -145,  -145,  -145,
    -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,
      99,  -145,  -145,    23,   229,   130,  -145,  -145,   122,    24,
     235,   240,   227,  -145,   -36,   243,  -145,  -145,  -145,  -145,
    -145,   227,   227,   227,   227,  -145,   264,   237,    81,  -145,
     268,   275,   280,   255,   305,   305,   227,    88,   227,  -145,
     245,   120,   152,   257,   272,   257,  -145,   202,  -145,    70,
     -42,  -145,  -145,  -145,  -145,   133,   251,   249,  -145,   227,
     227,   310,  -145,   227,   -14,  -145,  -145,  -145,   254,    48,
    -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,   227,
     227,  -145,  -145,  -145,  -145,  -145,   227,   257,    48,   227,
     227,  -145,    53,  -145,    57,    97,  -145,   107,   256,  -145,
    -145,   299,  -145,   122,  -145,  -145,   133,  -145,    94,   253,
    -145,   227,  -145,   145,  -145,   259,  -145,   298,    55,    81,
    -145,  -145,   303,   260,  -145,   114,  -145,  -145,   217,  -145,
     305,  -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,
    -145,  -145,  -145,    48,  -145,  -145,   257,   257,   224,  -145,
    -145,   227,  -145,  -145,  -145,  -145,  -145,   293,    48,  -145
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     9,     0,     1,     0,     0,    25,     0,
       5,     0,     0,     2,     0,    43,     7,     0,     0,     0,
       0,     0,     0,     0,    48,     4,     0,     0,    20,    23,
      24,    17,    18,    19,     0,     0,    11,    16,    13,     0,
       0,    26,     0,     0,     0,     3,     8,    21,    14,    22,
      15,     0,    32,    33,     0,    36,    37,    38,    39,    27,
      29,     0,     0,     0,     0,     0,     0,     0,     9,     6,
      12,     0,    34,     0,     0,    45,    47,     0,    53,    53,
      81,    90,     0,     0,     0,     0,     0,     0,     0,     0,
     112,    83,    84,    85,    70,     0,     0,    66,    78,    79,
      80,    69,    71,    72,    73,    74,    75,    76,    77,    49,
      25,    50,    31,     0,     0,     0,    40,    28,     0,     0,
       0,     0,   120,    87,     0,   116,   144,   145,   152,   153,
     154,     0,     0,     0,     0,   146,     0,   124,   132,   137,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,    43,     0,     0,     0,    46,     0,    55,     0,
       0,    56,    58,    59,    52,     0,     0,   121,   122,     0,
       0,     0,   117,   120,   115,   151,   150,   149,     0,     0,
     134,   135,   136,   126,   127,   128,   129,   130,   131,     0,
       0,   139,   140,   141,   142,   143,     0,    95,     0,     0,
       0,   116,     0,   113,     0,     0,   111,     0,     0,    68,
      67,     0,    42,     0,    41,    60,     0,    54,     0,     0,
      89,     0,    88,     0,   119,     0,   148,    92,   125,   133,
     138,   100,     0,    96,    97,     0,   102,   103,     0,   107,
       0,   108,   109,   110,    86,    62,    30,    61,    57,    51,
     123,   118,   147,     0,    91,    94,     0,     0,     0,   105,
     106,     0,   114,    93,    98,   101,    99,     0,     0,   104
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -145,  -145,  -145,  -145,    -3,   261,  -145,   -19,   216,   213,
    -145,   -57,  -145,  -142,  -145,   167,   173,   273,  -145,  -145,
    -145,   248,  -145,   110,  -145,   174,  -145,   -40,   247,  -145,
    -144,  -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,
    -145,    74,  -145,  -145,  -145,  -145,  -145,  -145,  -145,   189,
    -136,   262,  -145,   162,  -132,   -81,  -145,   144,  -145,   146,
    -145,  -121
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     2,     3,     7,   159,     8,    12,   114,    37,    15,
      22,    59,    71,    60,   115,   116,    24,    43,    44,    67,
      68,   120,   160,   161,   162,   163,   111,    94,    95,    96,
      97,    98,    99,   100,   101,   123,   102,   254,   103,   232,
     233,   234,   235,   104,   105,   106,   261,   107,   108,   202,
     135,   174,   172,   166,   167,   168,   189,   137,   190,   138,
     196,   139
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,   136,   140,   141,    69,    75,    18,   210,   203,   203,
     175,   176,   177,   150,   205,   207,   169,   117,   217,    27,
      42,    62,   218,   219,    76,   170,    16,    16,    16,   -65,
     171,    38,    70,    39,     9,   227,   -65,   157,   223,    10,
     -35,     1,   -10,   -10,   -10,   -10,   -10,   170,  -116,    80,
      42,    81,   171,   178,   236,   122,    27,  -116,   118,     4,
      77,   156,  -116,     5,    66,   -82,    82,   208,   -82,    83,
      84,    85,   -82,    86,   247,   230,   151,    87,    88,    89,
      90,    91,    92,    93,   158,    17,     6,    76,   222,    16,
      11,   125,   126,   127,   128,   129,   130,    16,   180,   181,
     -44,   -44,   -44,    26,   262,   182,    27,   157,    13,   263,
      77,    14,   -82,   239,   266,    19,   240,   241,   237,   238,
     240,    80,    20,    81,   269,    52,   191,   192,   193,   194,
     195,   131,   132,    27,   212,   216,    66,   -82,    82,   133,
     250,    83,    84,    85,   -82,    86,    21,   134,   206,    87,
      88,    89,    90,    91,    92,    93,   246,   242,    53,    54,
     221,    55,    56,    57,    58,    23,    80,   243,    81,    25,
     221,   245,    55,    56,    57,    58,    40,   257,   231,   258,
     267,    66,   -64,    82,   -82,    41,    83,    84,    85,    80,
      86,    81,   154,   155,    87,    88,    89,    90,    91,    92,
      93,    47,    29,    30,    66,    16,    82,   251,   221,    83,
      84,    85,   -64,    86,    49,    29,    30,    87,    88,    89,
      90,    91,    92,    93,    45,    80,    46,    81,    51,   -82,
     125,   126,   127,   128,   129,   130,    61,   231,   265,    73,
      66,   -82,    82,   259,   260,    83,    84,    85,    63,    86,
      48,    50,   -82,    87,    88,    89,    90,    91,    92,    93,
      28,    29,    30,    31,    32,    33,    64,    65,    66,    78,
     131,   132,    74,    79,   109,   112,   113,   119,   133,   143,
     180,   181,   149,   179,   144,   145,   134,   182,   -82,   146,
     183,   184,   185,   186,   187,   188,   153,   147,   148,   164,
      34,    35,   173,   198,   199,   165,   197,   200,   201,   209,
     213,   220,   221,   224,   226,    66,   244,   249,   253,   252,
     255,   268,   214,   152,   256,   211,    72,   121,   248,   110,
     264,   215,   142,   228,   204,   225,   229,     0,     0,     0,
       0,     0,     0,   124
};

static const yytype_int16 yycheck[] =
{
      19,    82,    83,    84,    44,    62,     9,   151,   144,   145,
     131,   132,   133,     1,   146,   147,    52,    74,    60,    63,
      23,    65,    64,   165,     1,    61,     3,     3,     3,    17,
      66,     1,    51,     3,    59,   179,    24,    13,   170,    64,
      17,    10,    12,    13,    14,    15,    16,    61,    52,     1,
      53,     3,    66,   134,   198,    59,    63,    61,    65,     3,
      63,   118,    66,     0,    16,    17,    18,   148,    20,    21,
      22,    23,    24,    25,   216,   196,    64,    29,    30,    31,
      32,    33,    34,    35,    60,    60,    11,     1,   169,     3,
       3,     3,     4,     5,     6,     7,     8,     3,    43,    44,
      14,    15,    16,    60,   240,    50,    63,    13,    66,   253,
     113,    12,    64,    60,   258,    53,    63,    60,   199,   200,
      63,     1,    64,     3,   268,     3,    45,    46,    47,    48,
      49,    43,    44,    63,   153,    65,    16,    17,    18,    51,
     221,    21,    22,    23,    24,    25,     3,    59,    60,    29,
      30,    31,    32,    33,    34,    35,   213,    60,    36,    37,
      63,    39,    40,    41,    42,    13,     1,    60,     3,    64,
      63,   211,    39,    40,    41,    42,    53,    63,   197,    65,
     261,    16,    17,    18,    64,    64,    21,    22,    23,     1,
      25,     3,    62,    63,    29,    30,    31,    32,    33,    34,
      35,     3,     4,     5,    16,     3,    18,    62,    63,    21,
      22,    23,    24,    25,     3,     4,     5,    29,    30,    31,
      32,    33,    34,    35,    64,     1,     3,     3,    53,    64,
       3,     4,     5,     6,     7,     8,     3,   256,   257,    61,
      16,    17,    18,    26,    27,    21,    22,    23,    64,    25,
      34,    35,    64,    29,    30,    31,    32,    33,    34,    35,
       3,     4,     5,     6,     7,     8,    14,    15,    16,     3,
      43,    44,    53,     3,    64,    17,    64,    59,    51,     3,
      43,    44,    17,    19,    59,    59,    59,    50,    64,    59,
      53,    54,    55,    56,    57,    58,    67,    59,    59,    64,
      43,    44,    59,    28,    24,    65,    38,    52,     3,    64,
      38,    60,    63,     3,    60,    16,    60,    64,    20,    60,
      17,    28,   155,   110,    64,   152,    53,    79,   218,    68,
     256,   157,    85,   189,   145,   173,   190,    -1,    -1,    -1,
      -1,    -1,    -1,    81
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    10,    69,    70,     3,     0,    11,    71,    73,    59,
      64,     3,    74,    66,    12,    77,     3,    60,    72,    53,
      64,     3,    78,    13,    84,    64,    60,    63,     3,     4,
       5,     6,     7,     8,    43,    44,    75,    76,     1,     3,
      53,    64,    72,    85,    86,    64,     3,     3,    76,     3,
      76,    53,     3,    36,    37,    39,    40,    41,    42,    79,
      81,     3,    65,    64,    14,    15,    16,    87,    88,    95,
      75,    80,    85,    61,    53,    79,     1,    72,     3,     3,
       1,     3,    18,    21,    22,    23,    25,    29,    30,    31,
      32,    33,    34,    35,    95,    96,    97,    98,    99,   100,
     101,   102,   104,   106,   111,   112,   113,   115,   116,    64,
      73,    94,    17,    64,    75,    82,    83,    79,    65,    59,
      89,    89,    59,   103,   119,     3,     4,     5,     6,     7,
       8,    43,    44,    51,    59,   118,   123,   125,   127,   129,
     123,   123,    96,     3,    59,    59,    59,    59,    59,    17,
       1,    64,    77,    67,    62,    63,    79,    13,    60,    72,
      90,    91,    92,    93,    64,    65,   121,   122,   123,    52,
      61,    66,   120,    59,   119,   129,   129,   129,   123,    19,
      43,    44,    50,    53,    54,    55,    56,    57,    58,   124,
     126,    45,    46,    47,    48,    49,   128,    38,    28,    24,
      52,     3,   117,   118,   117,   122,    60,   122,   123,    64,
      98,    84,    75,    38,    83,    93,    65,    60,    64,    81,
      60,    63,   123,   122,     3,   121,    60,    98,   125,   127,
     129,    75,   107,   108,   109,   110,    98,   123,   123,    60,
      63,    60,    60,    60,    60,    95,    79,    81,    91,    64,
     123,    62,    60,    20,   105,    17,    64,    63,    65,    26,
      27,   114,   118,    98,   109,    75,    98,   123,    28,    98
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    68,    69,    70,    70,    70,    71,    72,    72,    73,
      73,    74,    74,    74,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    76,    76,    77,    77,    78,    78,    79,
      79,    79,    79,    80,    80,    80,    81,    81,    81,    81,
      82,    82,    83,    84,    84,    85,    85,    85,    86,    86,
      87,    88,    88,    89,    89,    89,    90,    90,    91,    91,
      92,    93,    94,    95,    96,    96,    97,    97,    97,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      98,    98,    98,    99,   100,   101,   101,   102,   103,   103,
     103,   104,   105,   105,   106,   107,   107,   108,   108,   109,
     110,   110,   111,   112,   113,   114,   114,   115,   115,   116,
     116,   116,   116,   117,   117,   118,   119,   119,   120,   120,
     121,   121,   122,   122,   123,   123,   124,   124,   124,   124,
     124,   124,   125,   125,   126,   126,   126,   127,   127,   128,
     128,   128,   128,   128,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     6,     5,     3,     5,     1,     3,     0,
       3,     3,     5,     3,     2,     2,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     0,     3,     3,     5,     1,
       6,     3,     1,     0,     1,     2,     1,     1,     1,     1,
       1,     3,     3,     0,     3,     3,     5,     3,     0,     3,
       2,     6,     4,     0,     3,     2,     1,     3,     1,     1,
       2,     3,     4,     3,     0,     1,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     1,     1,     4,     2,     3,     3,
       0,     5,     0,     2,     5,     0,     1,     1,     3,     3,
       1,     3,     4,     4,     8,     1,     1,     4,     4,     4,
       4,     3,     1,     1,     3,     2,     0,     2,     3,     2,
       0,     1,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     2,
       2,     2,     1,     1,     1
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
  case 2: /* program: program_head program_body T_DOT  */
#line 97 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          ProgramNode* prog = static_cast<ProgramNode*>((yyvsp[-2].node));
          prog->body = static_cast<BlockNode*>((yyvsp[-1].node));
          g_ast_root = prog;
          (yyval.node) = prog;
      }
#line 1419 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 3: /* program_head: T_PROGRAM T_IDENTIFIER T_LPAREN id_list T_RPAREN T_SEMICOLON  */
#line 107 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* p = new ProgramNode();
          p->name = *(yyvsp[-4].str); delete (yyvsp[-4].str);
          p->params = *(yyvsp[-2].strList); delete (yyvsp[-2].strList);
          (yyval.node) = p;
      }
#line 1430 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 4: /* program_head: T_PROGRAM T_IDENTIFIER T_LPAREN T_RPAREN T_SEMICOLON  */
#line 114 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* p = new ProgramNode();
          p->name = *(yyvsp[-3].str); delete (yyvsp[-3].str);
          (yyval.node) = p;
      }
#line 1440 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 5: /* program_head: T_PROGRAM T_IDENTIFIER T_SEMICOLON  */
#line 120 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* p = new ProgramNode();
          p->name = *(yyvsp[-1].str); delete (yyvsp[-1].str);
          (yyval.node) = p;
      }
#line 1450 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 6: /* program_body: const_declarations type_declarations var_declarations subprogram_declarations compound_statement  */
#line 129 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* b = new BlockNode();
          if ((yyvsp[-4].nodeList)) { for (auto* n : *(yyvsp[-4].nodeList)) b->consts.push_back(static_cast<ConstDeclNode*>(n)); delete (yyvsp[-4].nodeList); }
          if ((yyvsp[-3].nodeList)) { for (auto* n : *(yyvsp[-3].nodeList)) b->types.push_back(static_cast<TypeDeclNode*>(n));  delete (yyvsp[-3].nodeList); }
          if ((yyvsp[-2].nodeList)) { for (auto* n : *(yyvsp[-2].nodeList)) b->vars.push_back(static_cast<VarDeclNode*>(n));    delete (yyvsp[-2].nodeList); }
          if ((yyvsp[-1].nodeList)) { b->routines = *(yyvsp[-1].nodeList); delete (yyvsp[-1].nodeList); }
          b->compound = static_cast<CompoundStmtNode*>((yyvsp[0].stmtNode));
          (yyval.node) = b;
      }
#line 1464 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 7: /* id_list: T_IDENTIFIER  */
#line 144 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.strList) = new std::vector<std::string>(); (yyval.strList)->push_back(*(yyvsp[0].str)); delete (yyvsp[0].str); }
#line 1470 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 8: /* id_list: id_list T_COMMA T_IDENTIFIER  */
#line 146 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyvsp[-2].strList)->push_back(*(yyvsp[0].str)); delete (yyvsp[0].str); (yyval.strList) = (yyvsp[-2].strList); }
#line 1476 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 9: /* const_declarations: %empty  */
#line 154 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                           { (yyval.nodeList) = nullptr; }
#line 1482 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 10: /* const_declarations: T_CONST const_declaration T_SEMICOLON  */
#line 155 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                            { (yyval.nodeList) = (yyvsp[-1].nodeList);    }
#line 1488 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 11: /* const_declaration: T_IDENTIFIER T_EQUAL const_value  */
#line 160 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = static_cast<ConstDeclNode*>((yyvsp[0].node));
          cv->name = *(yyvsp[-2].str); delete (yyvsp[-2].str);
          (yyval.nodeList) = new std::vector<AstNode*>();
          (yyval.nodeList)->push_back(cv);
      }
#line 1499 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 12: /* const_declaration: const_declaration T_SEMICOLON T_IDENTIFIER T_EQUAL const_value  */
#line 167 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = static_cast<ConstDeclNode*>((yyvsp[0].node));
          cv->name = *(yyvsp[-2].str); delete (yyvsp[-2].str);
          (yyvsp[-4].nodeList)->push_back(cv);
          (yyval.nodeList) = (yyvsp[-4].nodeList);
      }
#line 1510 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 13: /* const_declaration: const_declaration T_SEMICOLON error  */
#line 173 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                          { yyerrok; (yyval.nodeList) = (yyvsp[-2].nodeList); }
#line 1516 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 14: /* const_value: T_PLUS num  */
#line 178 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = static_cast<ConstDeclNode*>((yyvsp[0].node));
          if (!cv->cValue.empty() && cv->cValue[0] != '-') cv->cValue = "+" + cv->cValue;
          (yyval.node) = cv;
      }
#line 1526 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 15: /* const_value: T_MINUS num  */
#line 184 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = static_cast<ConstDeclNode*>((yyvsp[0].node));
          cv->cValue = "-" + cv->cValue;
          (yyval.node) = cv;
      }
#line 1536 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 16: /* const_value: num  */
#line 189 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1542 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 17: /* const_value: T_CHAR  */
#line 191 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "char";
          cv->cValue = *(yyvsp[0].str); delete (yyvsp[0].str);
          (yyval.node) = cv;
      }
#line 1553 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 18: /* const_value: T_STRING  */
#line 198 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "string";
          std::string escaped = "\"";
          for (char c : *(yyvsp[0].str)) {
              if (c == '"')  escaped += "\\\"";
              else if (c == '\\') escaped += "\\\\";
              else escaped += c;
          }
          escaped += "\"";
          cv->cValue = escaped; delete (yyvsp[0].str);
          (yyval.node) = cv;
      }
#line 1571 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 19: /* const_value: T_BOOLEAN  */
#line 212 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "int";
          cv->cValue = ((yyvsp[0].ival) ? "1" : "0");
          (yyval.node) = cv;
      }
#line 1582 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 20: /* const_value: T_IDENTIFIER  */
#line 219 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "int";
          cv->cValue = *(yyvsp[0].str); delete (yyvsp[0].str);
          (yyval.node) = cv;
      }
#line 1593 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 21: /* const_value: T_PLUS T_IDENTIFIER  */
#line 226 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "int";
          cv->cValue = "+" + *(yyvsp[0].str); delete (yyvsp[0].str);
          (yyval.node) = cv;
      }
#line 1604 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 22: /* const_value: T_MINUS T_IDENTIFIER  */
#line 233 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "int";
          cv->cValue = "-" + *(yyvsp[0].str); delete (yyvsp[0].str);
          (yyval.node) = cv;
      }
#line 1615 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 23: /* num: T_INTEGER  */
#line 243 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "int";
          cv->cValue = *(yyvsp[0].str); delete (yyvsp[0].str);
          (yyval.node) = cv;
      }
#line 1626 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 24: /* num: T_REAL  */
#line 250 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = new ConstDeclNode();
          cv->cType  = "float";
          cv->cValue = *(yyvsp[0].str) + "f"; delete (yyvsp[0].str);
          (yyval.node) = cv;
      }
#line 1637 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 25: /* type_declarations: %empty  */
#line 263 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                          { (yyval.nodeList) = nullptr; }
#line 1643 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 26: /* type_declarations: T_TYPE type_declaration T_SEMICOLON  */
#line 264 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                          { (yyval.nodeList) = (yyvsp[-1].nodeList);      }
#line 1649 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 27: /* type_declaration: T_IDENTIFIER T_EQUAL type_spec  */
#line 269 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* td = new TypeDeclNode();
          td->name     = *(yyvsp[-2].str); delete (yyvsp[-2].str);
          td->typeNode = (yyvsp[0].typeNode);
          (yyval.nodeList) = new std::vector<AstNode*>();
          (yyval.nodeList)->push_back(td);
      }
#line 1661 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 28: /* type_declaration: type_declaration T_SEMICOLON T_IDENTIFIER T_EQUAL type_spec  */
#line 277 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* td = new TypeDeclNode();
          td->name     = *(yyvsp[-2].str); delete (yyvsp[-2].str);
          td->typeNode = (yyvsp[0].typeNode);
          (yyvsp[-4].nodeList)->push_back(td);
          (yyval.nodeList) = (yyvsp[-4].nodeList);
      }
#line 1673 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 29: /* type_spec: basic_type  */
#line 288 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.typeNode) = new BasicTypeNode(*(yyvsp[0].str)); delete (yyvsp[0].str); }
#line 1679 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 30: /* type_spec: T_ARRAY T_LBRACKET periods T_RBRACKET T_OF type_spec  */
#line 290 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* at = new ArrayTypeNode();
          if ((yyvsp[-3].nodeList)) {
              for (auto* n : *(yyvsp[-3].nodeList)) at->periods.push_back(static_cast<PeriodNode*>(n));
              delete (yyvsp[-3].nodeList);
          }
          at->elemType = (yyvsp[0].typeNode);
          (yyval.typeNode) = at;
      }
#line 1693 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 31: /* type_spec: T_RECORD record_body T_END  */
#line 300 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* rt = new RecordTypeNode();
          if ((yyvsp[-1].nodeList)) {
              rt->fields = *(yyvsp[-1].nodeList); delete (yyvsp[-1].nodeList);
          }
          (yyval.typeNode) = rt;
      }
#line 1705 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 32: /* type_spec: T_IDENTIFIER  */
#line 308 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.typeNode) = new NamedTypeNode(*(yyvsp[0].str)); delete (yyvsp[0].str); }
#line 1711 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 33: /* record_body: %empty  */
#line 312 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                    { (yyval.nodeList) = nullptr; }
#line 1717 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 34: /* record_body: var_declaration  */
#line 313 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                    { (yyval.nodeList) = (yyvsp[0].nodeList);      }
#line 1723 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 35: /* record_body: var_declaration T_SEMICOLON  */
#line 314 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                    { (yyval.nodeList) = (yyvsp[-1].nodeList);      }
#line 1729 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 36: /* basic_type: T_TYPE_INTEGER  */
#line 318 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.str) = new std::string("integer"); }
#line 1735 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 37: /* basic_type: T_TYPE_REAL  */
#line 319 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.str) = new std::string("real");    }
#line 1741 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 38: /* basic_type: T_TYPE_BOOLEAN  */
#line 320 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.str) = new std::string("boolean"); }
#line 1747 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 39: /* basic_type: T_TYPE_CHAR  */
#line 321 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.str) = new std::string("char");    }
#line 1753 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 40: /* periods: period  */
#line 326 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.nodeList) = new std::vector<AstNode*>(); (yyval.nodeList)->push_back((yyvsp[0].node)); }
#line 1759 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 41: /* periods: periods T_COMMA period  */
#line 328 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyvsp[-2].nodeList)->push_back((yyvsp[0].node)); (yyval.nodeList) = (yyvsp[-2].nodeList); }
#line 1765 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 42: /* period: const_value T_DOTDOT const_value  */
#line 333 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* lo = static_cast<ConstDeclNode*>((yyvsp[-2].node));
          auto* hi = static_cast<ConstDeclNode*>((yyvsp[0].node));
          auto* p  = new PeriodNode(lo->cValue, hi->cValue);
          delete lo; delete hi;
          (yyval.node) = p;
      }
#line 1777 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 43: /* var_declarations: %empty  */
#line 347 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                        { (yyval.nodeList) = nullptr; }
#line 1783 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 44: /* var_declarations: T_VAR var_declaration T_SEMICOLON  */
#line 348 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                        { (yyval.nodeList) = (yyvsp[-1].nodeList);      }
#line 1789 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 45: /* var_declaration: id_list T_COLON type_spec  */
#line 353 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* vd = new VarDeclNode();
          vd->names    = *(yyvsp[-2].strList); delete (yyvsp[-2].strList);
          vd->typeNode = (yyvsp[0].typeNode);
          (yyval.nodeList) = new std::vector<AstNode*>();
          (yyval.nodeList)->push_back(vd);
      }
#line 1801 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 46: /* var_declaration: var_declaration T_SEMICOLON id_list T_COLON type_spec  */
#line 361 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* vd = new VarDeclNode();
          vd->names    = *(yyvsp[-2].strList); delete (yyvsp[-2].strList);
          vd->typeNode = (yyvsp[0].typeNode);
          (yyvsp[-4].nodeList)->push_back(vd);
          (yyval.nodeList) = (yyvsp[-4].nodeList);
      }
#line 1813 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 47: /* var_declaration: var_declaration T_SEMICOLON error  */
#line 368 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                        { yyerrok; (yyval.nodeList) = (yyvsp[-2].nodeList); }
#line 1819 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 48: /* subprogram_declarations: %empty  */
#line 376 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                  { (yyval.nodeList) = nullptr; }
#line 1825 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 49: /* subprogram_declarations: subprogram_declarations subprogram_declaration T_SEMICOLON  */
#line 378 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          if (!(yyvsp[-2].nodeList)) (yyvsp[-2].nodeList) = new std::vector<AstNode*>();
          (yyvsp[-2].nodeList)->push_back((yyvsp[-1].node));
          (yyval.nodeList) = (yyvsp[-2].nodeList);
      }
#line 1835 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 50: /* subprogram_declaration: subprogram_head subprogram_body  */
#line 387 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* r = static_cast<RoutineNode*>((yyvsp[-1].node));
          r->body = static_cast<BlockNode*>((yyvsp[0].node));
          (yyval.node) = r;
      }
#line 1845 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 51: /* subprogram_head: T_FUNCTION T_IDENTIFIER formal_parameter T_COLON basic_type T_SEMICOLON  */
#line 396 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* r = new RoutineNode();
          r->isFunction    = true;
          r->name          = *(yyvsp[-4].str); delete (yyvsp[-4].str);
          r->returnTypeName = *(yyvsp[-1].str); delete (yyvsp[-1].str);
          if ((yyvsp[-3].nodeList)) {
              for (auto* n : *(yyvsp[-3].nodeList)) r->params.push_back(static_cast<ParamGroupNode*>(n));
              delete (yyvsp[-3].nodeList);
          }
          (yyval.node) = r;
      }
#line 1861 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 52: /* subprogram_head: T_PROCEDURE T_IDENTIFIER formal_parameter T_SEMICOLON  */
#line 408 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* r = new RoutineNode();
          r->isFunction = false;
          r->name       = *(yyvsp[-2].str); delete (yyvsp[-2].str);
          if ((yyvsp[-1].nodeList)) {
              for (auto* n : *(yyvsp[-1].nodeList)) r->params.push_back(static_cast<ParamGroupNode*>(n));
              delete (yyvsp[-1].nodeList);
          }
          (yyval.node) = r;
      }
#line 1876 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 53: /* formal_parameter: %empty  */
#line 421 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                            { (yyval.nodeList) = nullptr; }
#line 1882 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 54: /* formal_parameter: T_LPAREN parameter_lists T_RPAREN  */
#line 422 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                            { (yyval.nodeList) = (yyvsp[-1].nodeList);      }
#line 1888 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 55: /* formal_parameter: T_LPAREN T_RPAREN  */
#line 423 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                            { (yyval.nodeList) = nullptr; }
#line 1894 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 56: /* parameter_lists: parameter_list  */
#line 428 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.nodeList) = new std::vector<AstNode*>(); (yyval.nodeList)->push_back((yyvsp[0].node)); }
#line 1900 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 57: /* parameter_lists: parameter_lists T_SEMICOLON parameter_list  */
#line 430 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyvsp[-2].nodeList)->push_back((yyvsp[0].node)); (yyval.nodeList) = (yyvsp[-2].nodeList); }
#line 1906 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 58: /* parameter_list: var_parameter  */
#line 434 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1912 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 59: /* parameter_list: value_parameter  */
#line 435 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1918 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 60: /* var_parameter: T_VAR value_parameter  */
#line 440 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { static_cast<ParamGroupNode*>((yyvsp[0].node))->byRef = true; (yyval.node) = (yyvsp[0].node); }
#line 1924 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 61: /* value_parameter: id_list T_COLON basic_type  */
#line 445 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* pg = new ParamGroupNode();
          pg->byRef    = false;
          pg->names    = *(yyvsp[-2].strList); delete (yyvsp[-2].strList);
          pg->typeName = *(yyvsp[0].str); delete (yyvsp[0].str);
          (yyval.node) = pg;
      }
#line 1936 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 62: /* subprogram_body: const_declarations type_declarations var_declarations compound_statement  */
#line 456 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* b = new BlockNode();
          if ((yyvsp[-3].nodeList)) { for (auto* n : *(yyvsp[-3].nodeList)) b->consts.push_back(static_cast<ConstDeclNode*>(n)); delete (yyvsp[-3].nodeList); }
          if ((yyvsp[-2].nodeList)) { for (auto* n : *(yyvsp[-2].nodeList)) b->types.push_back(static_cast<TypeDeclNode*>(n));  delete (yyvsp[-2].nodeList); }
          if ((yyvsp[-1].nodeList)) { for (auto* n : *(yyvsp[-1].nodeList)) b->vars.push_back(static_cast<VarDeclNode*>(n));    delete (yyvsp[-1].nodeList); }
          b->compound = static_cast<CompoundStmtNode*>((yyvsp[0].stmtNode));
          (yyval.node) = b;
      }
#line 1949 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 63: /* compound_statement: T_BEGIN statement_list_opt T_END  */
#line 472 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cs = new CompoundStmtNode();
          if ((yyvsp[-1].nodeList)) {
              for (auto* n : *(yyvsp[-1].nodeList)) if (n) cs->stmts.push_back(static_cast<StmtNode*>(n));
              delete (yyvsp[-1].nodeList);
          }
          (yyval.stmtNode) = cs;
      }
#line 1962 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 64: /* statement_list_opt: %empty  */
#line 483 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                    { (yyval.nodeList) = new std::vector<AstNode*>(); }
#line 1968 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 65: /* statement_list_opt: statement_list  */
#line 484 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                     { (yyval.nodeList) = (yyvsp[0].nodeList); }
#line 1974 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 66: /* statement_list: stmt  */
#line 489 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.nodeList) = new std::vector<AstNode*>(); if ((yyvsp[0].stmtNode)) (yyval.nodeList)->push_back((yyvsp[0].stmtNode)); }
#line 1980 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 67: /* statement_list: statement_list T_SEMICOLON stmt  */
#line 491 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { if ((yyvsp[0].stmtNode)) (yyvsp[-2].nodeList)->push_back((yyvsp[0].stmtNode)); (yyval.nodeList) = (yyvsp[-2].nodeList); }
#line 1986 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 68: /* statement_list: statement_list error T_SEMICOLON  */
#line 492 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                       { yyerrok; (yyval.nodeList) = (yyvsp[-2].nodeList); }
#line 1992 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 69: /* stmt: identifier_stmt  */
#line 500 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 1998 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 70: /* stmt: compound_statement  */
#line 501 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                         { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 2004 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 71: /* stmt: if_stmt  */
#line 502 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 2010 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 72: /* stmt: case_stmt  */
#line 503 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 2016 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 73: /* stmt: while_stmt  */
#line 504 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 2022 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 74: /* stmt: repeat_stmt  */
#line 505 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 2028 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 75: /* stmt: for_stmt  */
#line 506 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 2034 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 76: /* stmt: read_stmt  */
#line 507 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 2040 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 77: /* stmt: write_stmt  */
#line 508 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 2046 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 78: /* stmt: break_stmt  */
#line 509 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 2052 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 79: /* stmt: continue_stmt  */
#line 510 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 2058 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 80: /* stmt: exit_stmt  */
#line 511 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 2064 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 81: /* stmt: error  */
#line 512 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { yyerrok; (yyval.stmtNode) = nullptr; }
#line 2070 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 82: /* stmt: %empty  */
#line 513 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                       { (yyval.stmtNode) = nullptr; }
#line 2076 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 83: /* break_stmt: T_BREAK  */
#line 517 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
              { (yyval.stmtNode) = new BreakStmtNode(); }
#line 2082 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 84: /* continue_stmt: T_CONTINUE  */
#line 521 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                 { (yyval.stmtNode) = new ContinueStmtNode(); }
#line 2088 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 85: /* exit_stmt: T_EXIT  */
#line 526 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { auto* n = new ExitStmtNode(); (yyval.stmtNode) = n; }
#line 2094 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 86: /* exit_stmt: T_EXIT T_LPAREN expression T_RPAREN  */
#line 528 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { auto* n = new ExitStmtNode(); n->value = (yyvsp[-1].exprNode); (yyval.stmtNode) = n; }
#line 2100 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 87: /* identifier_stmt: T_IDENTIFIER identifier_stmt_tail  */
#line 533 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* tail = static_cast<IdentTailNode*>((yyvsp[0].node));
          std::string name = *(yyvsp[-1].str); delete (yyvsp[-1].str);
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
          (yyval.stmtNode) = result;
      }
#line 2121 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 88: /* identifier_stmt_tail: id_varparts T_ASSIGN expression  */
#line 553 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* t = new IdentTailNode();
          t->tailKind = IdentTailNode::Kind::Assign;
          if ((yyvsp[-2].varPartList)) { t->varParts = *(yyvsp[-2].varPartList); delete (yyvsp[-2].varPartList); }
          t->rhs = (yyvsp[0].exprNode);
          (yyval.node) = t;
      }
#line 2133 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 89: /* identifier_stmt_tail: T_LPAREN expression_list_opt T_RPAREN  */
#line 561 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* t = new IdentTailNode();
          t->tailKind = IdentTailNode::Kind::Call;
          if ((yyvsp[-1].exprList)) { t->args = *(yyvsp[-1].exprList); delete (yyvsp[-1].exprList); }
          (yyval.node) = t;
      }
#line 2144 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 90: /* identifier_stmt_tail: %empty  */
#line 568 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* t = new IdentTailNode();
          t->tailKind = IdentTailNode::Kind::BareCall;
          (yyval.node) = t;
      }
#line 2154 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 91: /* if_stmt: T_IF expression T_THEN stmt else_part  */
#line 577 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.stmtNode) = new IfStmtNode((yyvsp[-3].exprNode), (yyvsp[-1].stmtNode), (yyvsp[0].stmtNode)); }
#line 2160 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 92: /* else_part: %empty  */
#line 581 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                  { (yyval.stmtNode) = nullptr; }
#line 2166 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 93: /* else_part: T_ELSE stmt  */
#line 582 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                  { (yyval.stmtNode) = (yyvsp[0].stmtNode); }
#line 2172 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 94: /* case_stmt: T_CASE expression T_OF case_body T_END  */
#line 587 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cs = new CaseStmtNode();
          cs->expr = (yyvsp[-3].exprNode);
          if ((yyvsp[-1].nodeList)) {
              for (auto* n : *(yyvsp[-1].nodeList)) cs->branches.push_back(static_cast<CaseBranchNode*>(n));
              delete (yyvsp[-1].nodeList);
          }
          (yyval.stmtNode) = cs;
      }
#line 2186 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 95: /* case_body: %empty  */
#line 599 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                   { (yyval.nodeList) = nullptr; }
#line 2192 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 96: /* case_body: branch_list  */
#line 600 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                   { (yyval.nodeList) = (yyvsp[0].nodeList); }
#line 2198 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 97: /* branch_list: branch  */
#line 605 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.nodeList) = new std::vector<AstNode*>(); (yyval.nodeList)->push_back((yyvsp[0].node)); }
#line 2204 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 98: /* branch_list: branch_list T_SEMICOLON branch  */
#line 607 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyvsp[-2].nodeList)->push_back((yyvsp[0].node)); (yyval.nodeList) = (yyvsp[-2].nodeList); }
#line 2210 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 99: /* branch: const_list T_COLON stmt  */
#line 612 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cb = new CaseBranchNode();
          cb->values = *(yyvsp[-2].strList); delete (yyvsp[-2].strList);
          cb->body   = (yyvsp[0].stmtNode);
          (yyval.node) = cb;
      }
#line 2221 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 100: /* const_list: const_value  */
#line 622 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = static_cast<ConstDeclNode*>((yyvsp[0].node));
          (yyval.strList) = new std::vector<std::string>();
          (yyval.strList)->push_back(cv->cValue);
          delete cv;
      }
#line 2232 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 101: /* const_list: const_list T_COMMA const_value  */
#line 629 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* cv = static_cast<ConstDeclNode*>((yyvsp[0].node));
          (yyvsp[-2].strList)->push_back(cv->cValue);
          delete cv;
          (yyval.strList) = (yyvsp[-2].strList);
      }
#line 2243 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 102: /* while_stmt: T_WHILE expression T_DO stmt  */
#line 639 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.stmtNode) = new WhileStmtNode((yyvsp[-2].exprNode), (yyvsp[0].stmtNode)); }
#line 2249 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 103: /* repeat_stmt: T_REPEAT statement_list_opt T_UNTIL expression  */
#line 644 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* body = new CompoundStmtNode();
          if ((yyvsp[-2].nodeList)) {
              for (auto* n : *(yyvsp[-2].nodeList)) if (n) body->stmts.push_back(static_cast<StmtNode*>(n));
              delete (yyvsp[-2].nodeList);
          }
          (yyval.stmtNode) = new RepeatStmtNode(body, (yyvsp[0].exprNode));
      }
#line 2262 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 104: /* for_stmt: T_FOR T_IDENTIFIER T_ASSIGN expression updown expression T_DO stmt  */
#line 656 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* fs = new ForStmtNode();
          fs->var  = *(yyvsp[-6].str); delete (yyvsp[-6].str);
          fs->from_ = (yyvsp[-4].exprNode);
          fs->isTo  = ((yyvsp[-3].ival) == 1);
          fs->to_   = (yyvsp[-2].exprNode);
          fs->body  = (yyvsp[0].stmtNode);
          (yyval.stmtNode) = fs;
      }
#line 2276 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 105: /* updown: T_TO  */
#line 668 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
               { (yyval.ival) = 1; }
#line 2282 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 106: /* updown: T_DOWNTO  */
#line 669 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
               { (yyval.ival) = 0; }
#line 2288 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 107: /* read_stmt: T_READ T_LPAREN variable_list T_RPAREN  */
#line 674 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* rs = new ReadStmtNode();
          rs->withLn = false;
          if ((yyvsp[-1].varExprList)) { rs->vars = *(yyvsp[-1].varExprList); delete (yyvsp[-1].varExprList); }
          (yyval.stmtNode) = rs;
      }
#line 2299 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 108: /* read_stmt: T_READLN T_LPAREN variable_list T_RPAREN  */
#line 681 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* rs = new ReadStmtNode();
          rs->withLn = true;
          if ((yyvsp[-1].varExprList)) { rs->vars = *(yyvsp[-1].varExprList); delete (yyvsp[-1].varExprList); }
          (yyval.stmtNode) = rs;
      }
#line 2310 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 109: /* write_stmt: T_WRITE T_LPAREN expression_list T_RPAREN  */
#line 691 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* ws = new WriteStmtNode();
          ws->withLn = false;
          if ((yyvsp[-1].exprList)) { ws->exprs = *(yyvsp[-1].exprList); delete (yyvsp[-1].exprList); }
          (yyval.stmtNode) = ws;
      }
#line 2321 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 110: /* write_stmt: T_WRITELN T_LPAREN expression_list T_RPAREN  */
#line 698 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* ws = new WriteStmtNode();
          ws->withLn = true;
          if ((yyvsp[-1].exprList)) { ws->exprs = *(yyvsp[-1].exprList); delete (yyvsp[-1].exprList); }
          (yyval.stmtNode) = ws;
      }
#line 2332 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 111: /* write_stmt: T_WRITELN T_LPAREN T_RPAREN  */
#line 705 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* ws = new WriteStmtNode();
          ws->withLn = true;
          (yyval.stmtNode) = ws;
      }
#line 2342 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 112: /* write_stmt: T_WRITELN  */
#line 711 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* ws = new WriteStmtNode();
          ws->withLn = true;
          (yyval.stmtNode) = ws;
      }
#line 2352 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 113: /* variable_list: variable  */
#line 724 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.varExprList) = new std::vector<VarExprNode*>(); (yyval.varExprList)->push_back((yyvsp[0].varExprNode)); }
#line 2358 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 114: /* variable_list: variable_list T_COMMA variable  */
#line 726 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyvsp[-2].varExprList)->push_back((yyvsp[0].varExprNode)); (yyval.varExprList) = (yyvsp[-2].varExprList); }
#line 2364 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 115: /* variable: T_IDENTIFIER id_varparts  */
#line 731 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          std::vector<VarPartNode*> parts;
          if ((yyvsp[0].varPartList)) { parts = *(yyvsp[0].varPartList); delete (yyvsp[0].varPartList); }
          (yyval.varExprNode) = new VarExprNode(*(yyvsp[-1].str), parts); delete (yyvsp[-1].str);
      }
#line 2374 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 116: /* id_varparts: %empty  */
#line 739 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                  { (yyval.varPartList) = nullptr; }
#line 2380 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 117: /* id_varparts: id_varparts id_varpart  */
#line 741 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          if (!(yyvsp[-1].varPartList)) (yyvsp[-1].varPartList) = new std::vector<VarPartNode*>();
          (yyvsp[-1].varPartList)->push_back((yyvsp[0].varPartNode));
          (yyval.varPartList) = (yyvsp[-1].varPartList);
      }
#line 2390 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 118: /* id_varpart: T_LBRACKET expression_list T_RBRACKET  */
#line 750 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* vp = new VarPartNode();
          vp->isField  = false;
          if ((yyvsp[-1].exprList)) { vp->indices = *(yyvsp[-1].exprList); delete (yyvsp[-1].exprList); }
          (yyval.varPartNode) = vp;
      }
#line 2401 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 119: /* id_varpart: T_DOT T_IDENTIFIER  */
#line 757 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          auto* vp = new VarPartNode();
          vp->isField   = true;
          vp->fieldName = *(yyvsp[0].str); delete (yyvsp[0].str);
          (yyval.varPartNode) = vp;
      }
#line 2412 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 120: /* expression_list_opt: %empty  */
#line 770 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                     { (yyval.exprList) = nullptr; }
#line 2418 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 121: /* expression_list_opt: expression_list  */
#line 771 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 2424 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 122: /* expression_list: expression  */
#line 776 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.exprList) = new std::vector<ExprNode*>(); (yyval.exprList)->push_back((yyvsp[0].exprNode)); }
#line 2430 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 123: /* expression_list: expression_list T_COMMA expression  */
#line 778 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyvsp[-2].exprList)->push_back((yyvsp[0].exprNode)); (yyval.exprList) = (yyvsp[-2].exprList); }
#line 2436 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 124: /* expression: simple_expression  */
#line 783 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.exprNode) = (yyvsp[0].exprNode); }
#line 2442 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 125: /* expression: simple_expression relop simple_expression  */
#line 785 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.exprNode) = new BinaryExprNode(*(yyvsp[-1].str), (yyvsp[-2].exprNode), (yyvsp[0].exprNode)); delete (yyvsp[-1].str); }
#line 2448 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 126: /* relop: T_EQUAL  */
#line 789 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.str) = new std::string("=");  }
#line 2454 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 127: /* relop: T_NOT_EQUAL  */
#line 790 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.str) = new std::string("<>"); }
#line 2460 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 128: /* relop: T_LESS  */
#line 791 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.str) = new std::string("<");  }
#line 2466 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 129: /* relop: T_LESS_EQUAL  */
#line 792 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.str) = new std::string("<="); }
#line 2472 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 130: /* relop: T_GREATER  */
#line 793 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.str) = new std::string(">");  }
#line 2478 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 131: /* relop: T_GREATER_EQUAL  */
#line 794 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                      { (yyval.str) = new std::string(">="); }
#line 2484 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 132: /* simple_expression: term  */
#line 798 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
           { (yyval.exprNode) = (yyvsp[0].exprNode); }
#line 2490 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 133: /* simple_expression: simple_expression addop term  */
#line 800 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.exprNode) = new BinaryExprNode(*(yyvsp[-1].str), (yyvsp[-2].exprNode), (yyvsp[0].exprNode)); delete (yyvsp[-1].str); }
#line 2496 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 134: /* addop: T_PLUS  */
#line 804 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
              { (yyval.str) = new std::string("+");  }
#line 2502 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 135: /* addop: T_MINUS  */
#line 805 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
              { (yyval.str) = new std::string("-");  }
#line 2508 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 136: /* addop: T_OR  */
#line 806 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
              { (yyval.str) = new std::string("or"); }
#line 2514 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 137: /* term: factor  */
#line 810 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
             { (yyval.exprNode) = (yyvsp[0].exprNode); }
#line 2520 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 138: /* term: term mulop factor  */
#line 812 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.exprNode) = new BinaryExprNode(*(yyvsp[-1].str), (yyvsp[-2].exprNode), (yyvsp[0].exprNode)); delete (yyvsp[-1].str); }
#line 2526 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 139: /* mulop: T_MULTIPLY  */
#line 816 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                 { (yyval.str) = new std::string("*");   }
#line 2532 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 140: /* mulop: T_DIVIDE  */
#line 817 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                 { (yyval.str) = new std::string("/");   }
#line 2538 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 141: /* mulop: T_DIV  */
#line 818 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                 { (yyval.str) = new std::string("div"); }
#line 2544 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 142: /* mulop: T_MOD  */
#line 819 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                 { (yyval.str) = new std::string("mod"); }
#line 2550 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 143: /* mulop: T_AND  */
#line 820 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                 { (yyval.str) = new std::string("and"); }
#line 2556 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 144: /* factor: T_INTEGER  */
#line 825 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.exprNode) = new LiteralNode(LiteralKind::Int,  *(yyvsp[0].str)); delete (yyvsp[0].str); }
#line 2562 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 145: /* factor: T_REAL  */
#line 827 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.exprNode) = new LiteralNode(LiteralKind::Real, *(yyvsp[0].str)); delete (yyvsp[0].str); }
#line 2568 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 146: /* factor: variable  */
#line 829 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.exprNode) = (yyvsp[0].varExprNode); }
#line 2574 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 147: /* factor: T_IDENTIFIER T_LPAREN expression_list_opt T_RPAREN  */
#line 831 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      {
          std::vector<ExprNode*> args;
          if ((yyvsp[-1].exprList)) { args = *(yyvsp[-1].exprList); delete (yyvsp[-1].exprList); }
          (yyval.exprNode) = new CallExprNode(*(yyvsp[-3].str), args); delete (yyvsp[-3].str);
      }
#line 2584 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 148: /* factor: T_LPAREN expression T_RPAREN  */
#line 836 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                                   { (yyval.exprNode) = (yyvsp[-1].exprNode); }
#line 2590 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 149: /* factor: T_NOT factor  */
#line 837 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                     { (yyval.exprNode) = new UnaryExprNode("not", (yyvsp[0].exprNode)); }
#line 2596 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 150: /* factor: T_MINUS factor  */
#line 838 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                     { (yyval.exprNode) = new UnaryExprNode("-",   (yyvsp[0].exprNode)); }
#line 2602 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 151: /* factor: T_PLUS factor  */
#line 839 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
                     { (yyval.exprNode) = new UnaryExprNode("+",   (yyvsp[0].exprNode)); }
#line 2608 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 152: /* factor: T_CHAR  */
#line 841 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.exprNode) = new LiteralNode(LiteralKind::Char, *(yyvsp[0].str)); delete (yyvsp[0].str); }
#line 2614 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 153: /* factor: T_STRING  */
#line 843 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.exprNode) = new LiteralNode(LiteralKind::Str,  *(yyvsp[0].str)); delete (yyvsp[0].str); }
#line 2620 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;

  case 154: /* factor: T_BOOLEAN  */
#line 845 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"
      { (yyval.exprNode) = new LiteralNode(LiteralKind::Bool, ((yyvsp[0].ival) ? std::string("true") : std::string("false"))); }
#line 2626 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"
    break;


#line 2630 "D:/金介然/大三下/编译课设/CODE/build_demo/parser_bison.tab.cpp"

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

#line 848 "D:/金介然/大三下/编译课设/CODE/src/parser_bison.y"

