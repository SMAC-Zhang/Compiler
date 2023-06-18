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
#line 1 "./src/parser.yacc"

#include <stdio.h>
#include <string.h>
#include "fdmjast.h"
#include "util.h"

extern A_prog root; 
extern int line, pos;

extern int yylex();
extern void yyerror(char*);
extern int  yywrap();

// extern int yydebug = 1;


#line 88 "src/y.tab.c"

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

#include "y.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_OP_PLUS = 3,                    /* OP_PLUS  */
  YYSYMBOL_OP_MULTIPLY = 4,                /* OP_MULTIPLY  */
  YYSYMBOL_OP_MINUS = 5,                   /* OP_MINUS  */
  YYSYMBOL_OP_DIV = 6,                     /* OP_DIV  */
  YYSYMBOL_UMINUS = 7,                     /* UMINUS  */
  YYSYMBOL_OR = 8,                         /* OR  */
  YYSYMBOL_AND = 9,                        /* AND  */
  YYSYMBOL_LESS = 10,                      /* LESS  */
  YYSYMBOL_LE = 11,                        /* LE  */
  YYSYMBOL_EQ = 12,                        /* EQ  */
  YYSYMBOL_NOT = 13,                       /* NOT  */
  YYSYMBOL_GREATER = 14,                   /* GREATER  */
  YYSYMBOL_GE = 15,                        /* GE  */
  YYSYMBOL_NEQ = 16,                       /* NEQ  */
  YYSYMBOL_BRACE = 17,                     /* BRACE  */
  YYSYMBOL_BRACKET = 18,                   /* BRACKET  */
  YYSYMBOL_PARENTHESIS = 19,               /* PARENTHESIS  */
  YYSYMBOL_PUBLIC = 20,                    /* PUBLIC  */
  YYSYMBOL_INT = 21,                       /* INT  */
  YYSYMBOL_MAIN = 22,                      /* MAIN  */
  YYSYMBOL_CLASS = 23,                     /* CLASS  */
  YYSYMBOL_EXTENDS = 24,                   /* EXTENDS  */
  YYSYMBOL_IF = 25,                        /* IF  */
  YYSYMBOL_ELSE = 26,                      /* ELSE  */
  YYSYMBOL_WHILE = 27,                     /* WHILE  */
  YYSYMBOL_CONTINUE = 28,                  /* CONTINUE  */
  YYSYMBOL_BREAK = 29,                     /* BREAK  */
  YYSYMBOL_RETURN = 30,                    /* RETURN  */
  YYSYMBOL_PUTINT = 31,                    /* PUTINT  */
  YYSYMBOL_PUTCH = 32,                     /* PUTCH  */
  YYSYMBOL_PUTARRAY = 33,                  /* PUTARRAY  */
  YYSYMBOL_GETINT = 34,                    /* GETINT  */
  YYSYMBOL_GETCH = 35,                     /* GETCH  */
  YYSYMBOL_GETARRAY = 36,                  /* GETARRAY  */
  YYSYMBOL_STARTTIME = 37,                 /* STARTTIME  */
  YYSYMBOL_STOPTIME = 38,                  /* STOPTIME  */
  YYSYMBOL_True = 39,                      /* True  */
  YYSYMBOL_False = 40,                     /* False  */
  YYSYMBOL_LENGTH = 41,                    /* LENGTH  */
  YYSYMBOL_THIS = 42,                      /* THIS  */
  YYSYMBOL_NEW = 43,                       /* NEW  */
  YYSYMBOL_INT_CONST = 44,                 /* INT_CONST  */
  YYSYMBOL_IDENTIFIER = 45,                /* IDENTIFIER  */
  YYSYMBOL_46_ = 46,                       /* '.'  */
  YYSYMBOL_47_ = 47,                       /* ')'  */
  YYSYMBOL_48_ = 48,                       /* '}'  */
  YYSYMBOL_49_ = 49,                       /* ';'  */
  YYSYMBOL_50_ = 50,                       /* '='  */
  YYSYMBOL_51_ = 51,                       /* ','  */
  YYSYMBOL_52_ = 52,                       /* ']'  */
  YYSYMBOL_YYACCEPT = 53,                  /* $accept  */
  YYSYMBOL_PROG = 54,                      /* PROG  */
  YYSYMBOL_MAIN_METHOD = 55,               /* MAIN_METHOD  */
  YYSYMBOL_CLASS_DEC = 56,                 /* CLASS_DEC  */
  YYSYMBOL_CLASS_DEC_LIST = 57,            /* CLASS_DEC_LIST  */
  YYSYMBOL_VAR = 58,                       /* VAR  */
  YYSYMBOL_VAR_LIST = 59,                  /* VAR_LIST  */
  YYSYMBOL_IntConst = 60,                  /* IntConst  */
  YYSYMBOL_INT_CONST_LIST = 61,            /* INT_CONST_LIST  */
  YYSYMBOL_INT_CONST_REST = 62,            /* INT_CONST_REST  */
  YYSYMBOL_METHOD = 63,                    /* METHOD  */
  YYSYMBOL_METHOD_LIST = 64,               /* METHOD_LIST  */
  YYSYMBOL_FORMAL_LIST = 65,               /* FORMAL_LIST  */
  YYSYMBOL_FORMAL_REST = 66,               /* FORMAL_REST  */
  YYSYMBOL_TYPE = 67,                      /* TYPE  */
  YYSYMBOL_STM = 68,                       /* STM  */
  YYSYMBOL_STM_LIST = 69,                  /* STM_LIST  */
  YYSYMBOL_EXP = 70,                       /* EXP  */
  YYSYMBOL_EXP_LIST = 71,                  /* EXP_LIST  */
  YYSYMBOL_EXP_REST = 72                   /* EXP_REST  */
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
#define YYLAST   705

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  216

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   300


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
       2,    47,     2,     2,    51,     2,    46,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    49,
       2,    50,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    52,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    48,     2,     2,     2,     2,
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
      45
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    88,    88,    94,   100,   105,   111,   116,   121,   126,
     131,   137,   142,   147,   152,   158,   163,   168,   173,   178,
     184,   189,   194,   199,   204,   209,   214,   219,   224,   230,
     235,   240,   245,   250,   255,   260,   265,   270,   275,   280,
     285,   290,   295,   300,   305,   311,   316,   321,   326,   331,
     336,   341,   346,   351,   356,   361,   366,   371,   376,   381,
     386,   391,   396,   401,   406,   411,   416,   421,   426,   431,
     436,   441,   446,   451,   456,   461,   466,   472,   477,   482,
     487
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
  "\"end of file\"", "error", "\"invalid token\"", "OP_PLUS",
  "OP_MULTIPLY", "OP_MINUS", "OP_DIV", "UMINUS", "OR", "AND", "LESS", "LE",
  "EQ", "NOT", "GREATER", "GE", "NEQ", "BRACE", "BRACKET", "PARENTHESIS",
  "PUBLIC", "INT", "MAIN", "CLASS", "EXTENDS", "IF", "ELSE", "WHILE",
  "CONTINUE", "BREAK", "RETURN", "PUTINT", "PUTCH", "PUTARRAY", "GETINT",
  "GETCH", "GETARRAY", "STARTTIME", "STOPTIME", "True", "False", "LENGTH",
  "THIS", "NEW", "INT_CONST", "IDENTIFIER", "'.'", "')'", "'}'", "';'",
  "'='", "','", "']'", "$accept", "PROG", "MAIN_METHOD", "CLASS_DEC",
  "CLASS_DEC_LIST", "VAR", "VAR_LIST", "IntConst", "INT_CONST_LIST",
  "INT_CONST_REST", "METHOD", "METHOD_LIST", "FORMAL_LIST", "FORMAL_REST",
  "TYPE", "STM", "STM_LIST", "EXP", "EXP_LIST", "EXP_REST", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-153)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -4,     0,    38,    22,    25,  -153,    29,    22,  -153,    35,
      13,  -153,     8,    10,    40,    12,    31,    12,    41,    32,
      12,    68,    49,    78,   606,    44,  -153,  -153,    12,    68,
      50,    -9,    12,   660,   660,   606,   647,    91,    95,    48,
      67,   660,    98,    99,   100,   103,   104,   106,   111,   117,
    -153,  -153,   118,  -153,    -8,  -153,  -153,   606,    93,   191,
    -153,    94,  -153,  -153,  -153,    87,    68,    -7,    -7,    96,
     606,   339,   660,   660,  -153,  -153,   253,   660,   660,   660,
     101,   113,   660,   120,   123,   660,   124,   127,  -153,  -153,
     660,   660,   660,   660,   660,   660,   660,   660,   660,   660,
     660,   660,   208,   119,   660,   150,   128,     1,  -153,   138,
     143,   660,   126,  -153,   156,  -153,   355,   400,  -153,   416,
     461,   147,  -153,  -153,   477,   159,   161,   522,   660,   164,
     122,    -7,   122,    -7,   561,    14,   129,   129,   377,   129,
     129,   377,   162,    75,   157,   269,    12,  -153,   163,   167,
    -153,  -153,   197,   660,   606,   292,   168,   169,   660,  -153,
    -153,  -153,  -153,    97,  -153,   202,  -153,   660,  -153,   175,
     178,     1,  -153,   177,   660,   538,   198,  -153,  -153,  -153,
    -153,   583,  -153,   660,   174,   181,   212,   179,   163,  -153,
     184,  -153,   606,   183,   185,   660,  -153,   186,    12,    12,
    -153,  -153,  -153,  -153,  -153,   187,   174,  -153,   606,   189,
    -153,  -153,   190,   179,  -153,  -153
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     7,     0,     1,     0,     7,     2,     0,
       0,     6,     0,     0,     0,    12,     0,    12,    27,     0,
      12,    21,     0,     0,    46,     0,    26,    11,     0,    21,
       0,     0,    12,     0,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,     0,    67,     0,    62,    66,    46,     0,     0,
      28,     0,    20,     4,     8,     0,    21,    71,    70,     0,
      46,     0,     0,     0,    37,    38,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    45,     3,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    16,    13,     0,
       0,     0,     0,    29,     0,    72,     0,     0,    39,     0,
       0,     0,    74,    75,     0,     0,     0,     0,     0,     0,
      47,    49,    48,    50,    52,    51,    53,    54,    57,    55,
      56,    58,     0,     0,    61,     0,    23,    14,    18,     0,
       9,     5,    61,     0,     0,     0,     0,     0,     0,    76,
      43,    44,    65,     0,    69,     0,    59,    78,    34,     0,
       0,     0,    15,     0,    78,     0,    31,    33,    32,    40,
      41,     0,    68,    78,    80,     0,     0,    25,    18,    10,
       0,    73,     0,     0,     0,     0,    77,    60,    12,     0,
      22,    17,    60,    30,    42,     0,    80,    36,    46,     0,
      35,    79,     0,    25,    19,    24
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -153,  -153,  -153,  -153,   232,  -153,   -17,   -97,  -153,    52,
    -153,   -15,  -153,    33,   -26,  -146,   -34,   -29,  -152,    39
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     7,     8,    20,    21,   109,   149,   172,
      29,    30,   169,   200,    22,    57,    58,    59,   185,   196
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      24,    69,    61,    27,    67,    68,   106,    71,   176,   178,
     148,   111,    76,    86,    62,    66,     1,    90,    91,    92,
      93,     4,   190,    88,    96,    97,    98,    15,    99,   100,
     101,   194,   111,    18,    16,    19,   114,    87,     5,   112,
      64,    65,     6,   116,   117,   108,   203,     9,   119,   120,
     121,   110,    10,   124,    12,    14,   127,    17,    13,    25,
     112,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   143,   188,   145,    23,    26,    90,    91,
      92,    93,   143,    94,    95,    96,    97,    98,    28,    99,
     100,   101,   106,   111,    31,    32,    60,    74,    63,   163,
      90,    91,    92,    93,   107,    94,    95,    96,    97,    98,
      72,    99,   100,   101,    73,   111,    75,    77,    78,    79,
     170,   112,    80,    81,   175,    82,    91,   166,    93,   181,
      83,   108,    90,    91,    92,    93,    84,    85,   184,   105,
     111,    89,   128,   112,   113,   184,   129,   111,   122,   182,
      90,    91,    92,    93,   184,    94,    95,    96,    97,    98,
     123,    99,   100,   101,   144,   111,   206,   125,   112,   146,
     126,   152,   147,   209,   212,   112,   167,    90,    91,    92,
      93,   208,    94,    95,    96,    97,    98,   150,    99,   100,
     101,   151,   111,   112,    90,    91,    92,    93,   158,    94,
      95,    96,    97,    98,   153,    99,   100,   101,   160,   102,
     161,   164,   165,    33,   171,   173,   174,   179,   180,   183,
     112,    34,   186,   187,   192,   195,   189,    36,   197,   198,
     199,   202,   204,   205,   213,   207,   210,   103,   214,    11,
     201,   104,    45,    46,    47,   211,   215,    50,    51,    52,
      53,    54,    55,    56,     0,     0,    90,    91,    92,    93,
     142,    94,    95,    96,    97,    98,     0,    99,   100,   101,
       0,   111,    90,    91,    92,    93,     0,    94,    95,    96,
      97,    98,     0,    99,   100,   101,     0,   111,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    33,     0,   112,
       0,     0,   118,     0,     0,    34,     0,     0,     0,    35,
       0,    36,     0,     0,     0,   112,     0,    37,   168,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,     0,     0,
       0,   177,    90,    91,    92,    93,     0,    94,    95,    96,
      97,    98,     0,    99,   100,   101,     0,   111,    90,    91,
      92,    93,     0,    94,    95,    96,    97,    98,     0,    99,
     100,   101,     0,   111,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,     0,   112,   115,    96,    97,     0,
       0,    99,   100,     0,     0,   111,     0,     0,     0,     0,
       0,   112,   154,    90,    91,    92,    93,     0,    94,    95,
      96,    97,    98,     0,    99,   100,   101,     0,   111,    90,
      91,    92,    93,   112,    94,    95,    96,    97,    98,     0,
      99,   100,   101,     0,   111,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   112,   155,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   112,   156,    90,    91,    92,    93,     0,    94,
      95,    96,    97,    98,     0,    99,   100,   101,     0,   111,
      90,    91,    92,    93,     0,    94,    95,    96,    97,    98,
       0,    99,   100,   101,     0,   111,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   112,   157,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   112,   159,    90,    91,    92,    93,     0,
      94,    95,    96,    97,    98,     0,    99,   100,   101,     0,
     111,    90,    91,    92,    93,     0,    94,    95,    96,    97,
      98,     0,    99,   100,   101,     0,   111,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,   112,   162,
      95,    96,    97,    98,     0,    99,   100,   101,     0,   111,
       0,     0,     0,     0,   112,   191,    90,    91,    92,    93,
       0,    94,    95,    96,    97,    98,     0,    99,   100,   101,
       0,   111,     0,     0,     0,     0,     0,   112,     0,     0,
       0,    33,     0,     0,     0,     0,     0,     0,     0,    34,
       0,     0,     0,    35,     0,    36,     0,     0,     0,   112,
     193,    37,     0,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    33,     0,     0,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    70,    33,    36,     0,     0,     0,
       0,     0,     0,    34,     0,     0,     0,     0,     0,    36,
       0,    45,    46,    47,     0,     0,    50,    51,    52,    53,
      54,    55,    56,     0,    45,    46,    47,     0,     0,    50,
      51,    52,    53,    54,    55,    56
};

static const yytype_int16 yycheck[] =
{
      17,    35,    28,    20,    33,    34,     5,    36,   154,   155,
     107,    18,    41,    21,    29,    32,    20,     3,     4,     5,
       6,    21,   174,    57,    10,    11,    12,    17,    14,    15,
      16,   183,    18,    21,    24,    23,    70,    45,     0,    46,
      49,    50,    20,    72,    73,    44,   192,    22,    77,    78,
      79,    66,    23,    82,    19,    47,    85,    17,    45,    18,
      46,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   171,   104,    45,    45,     3,     4,
       5,     6,   111,     8,     9,    10,    11,    12,    20,    14,
      15,    16,     5,    18,    45,    17,    52,    49,    48,   128,
       3,     4,     5,     6,    17,     8,     9,    10,    11,    12,
      19,    14,    15,    16,    19,    18,    49,    19,    19,    19,
     146,    46,    19,    19,   153,    19,     4,    52,     6,   158,
      19,    44,     3,     4,     5,     6,    19,    19,   167,    45,
      18,    48,    18,    46,    48,   174,    19,    18,    47,    52,
       3,     4,     5,     6,   183,     8,     9,    10,    11,    12,
      47,    14,    15,    16,    45,    18,   195,    47,    46,    19,
      47,    45,    44,   199,   208,    46,    19,     3,     4,     5,
       6,   198,     8,     9,    10,    11,    12,    49,    14,    15,
      16,    48,    18,    46,     3,     4,     5,     6,    51,     8,
       9,    10,    11,    12,    48,    14,    15,    16,    49,    18,
      49,    47,    50,     5,    51,    48,    19,    49,    49,    17,
      46,    13,    47,    45,    26,    51,    49,    19,    47,    17,
      51,    47,    49,    48,    45,    49,    49,    46,    48,     7,
     188,    50,    34,    35,    36,   206,   213,    39,    40,    41,
      42,    43,    44,    45,    -1,    -1,     3,     4,     5,     6,
      52,     8,     9,    10,    11,    12,    -1,    14,    15,    16,
      -1,    18,     3,     4,     5,     6,    -1,     8,     9,    10,
      11,    12,    -1,    14,    15,    16,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,    -1,    46,
      -1,    -1,    49,    -1,    -1,    13,    -1,    -1,    -1,    17,
      -1,    19,    -1,    -1,    -1,    46,    -1,    25,    49,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    -1,
      -1,    49,     3,     4,     5,     6,    -1,     8,     9,    10,
      11,    12,    -1,    14,    15,    16,    -1,    18,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    -1,    14,
      15,    16,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    46,    47,    10,    11,    -1,
      -1,    14,    15,    -1,    -1,    18,    -1,    -1,    -1,    -1,
      -1,    46,    47,     3,     4,     5,     6,    -1,     8,     9,
      10,    11,    12,    -1,    14,    15,    16,    -1,    18,     3,
       4,     5,     6,    46,     8,     9,    10,    11,    12,    -1,
      14,    15,    16,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    47,     3,     4,     5,     6,    -1,     8,
       9,    10,    11,    12,    -1,    14,    15,    16,    -1,    18,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      -1,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    47,     3,     4,     5,     6,    -1,
       8,     9,    10,    11,    12,    -1,    14,    15,    16,    -1,
      18,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    -1,    14,    15,    16,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    46,    47,
       9,    10,    11,    12,    -1,    14,    15,    16,    -1,    18,
      -1,    -1,    -1,    -1,    46,    47,     3,     4,     5,     6,
      -1,     8,     9,    10,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    17,    -1,    19,    -1,    -1,    -1,    46,
      47,    25,    -1,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    17,     5,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    34,    35,    36,    -1,    -1,    39,    40,    41,    42,
      43,    44,    45,    -1,    34,    35,    36,    -1,    -1,    39,
      40,    41,    42,    43,    44,    45
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    20,    54,    55,    21,     0,    20,    56,    57,    22,
      23,    57,    19,    45,    47,    17,    24,    17,    21,    23,
      58,    59,    67,    45,    59,    18,    45,    59,    20,    63,
      64,    45,    17,     5,    13,    17,    19,    25,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    68,    69,    70,
      52,    67,    64,    48,    49,    50,    59,    70,    70,    69,
      17,    70,    19,    19,    49,    49,    70,    19,    19,    19,
      19,    19,    19,    19,    19,    19,    21,    45,    69,    48,
       3,     4,     5,     6,     8,     9,    10,    11,    12,    14,
      15,    16,    18,    46,    50,    45,     5,    17,    44,    60,
      64,    18,    46,    48,    69,    47,    70,    70,    49,    70,
      70,    70,    47,    47,    70,    47,    47,    70,    18,    19,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    52,    70,    45,    70,    19,    44,    60,    61,
      49,    48,    45,    48,    47,    47,    47,    47,    51,    47,
      49,    49,    47,    70,    47,    50,    52,    19,    49,    65,
      67,    51,    62,    48,    19,    70,    68,    49,    68,    49,
      49,    70,    52,    17,    70,    71,    47,    45,    60,    49,
      71,    47,    26,    47,    71,    51,    72,    47,    17,    51,
      66,    62,    47,    68,    49,    48,    70,    49,    59,    67,
      49,    72,    69,    45,    48,    66
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    53,    54,    55,    56,    56,    57,    57,    58,    58,
      58,    59,    59,    60,    60,    61,    61,    62,    62,    63,
      64,    64,    65,    65,    66,    66,    67,    67,    67,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    69,    69,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    71,    71,    72,
      72
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     9,     7,     9,     2,     0,     3,     5,
       7,     2,     0,     1,     2,     2,     0,     3,     0,    10,
       2,     0,     3,     0,     4,     0,     2,     1,     3,     3,
       7,     5,     5,     5,     4,     8,     7,     2,     2,     3,
       5,     5,     7,     4,     4,     2,     0,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       6,     3,     1,     1,     1,     4,     1,     1,     5,     4,
       2,     2,     3,     6,     3,     3,     4,     2,     0,     3,
       0
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
  case 2: /* PROG: MAIN_METHOD CLASS_DEC_LIST  */
#line 89 "./src/parser.yacc"
        {
		root = A_Prog((yyvsp[-1].main)->pos, (yyvsp[-1].main), (yyvsp[0].classdeclist));
		(yyval.prog) = A_Prog((yyvsp[-1].main)->pos, (yyvsp[-1].main), (yyvsp[0].classdeclist));
	}
#line 1379 "src/y.tab.c"
    break;

  case 3: /* MAIN_METHOD: PUBLIC INT MAIN PARENTHESIS ')' BRACE VAR_LIST STM_LIST '}'  */
#line 95 "./src/parser.yacc"
        {
		(yyval.main) = A_MainMethod((yyvsp[-8].token), (yyvsp[-2].varlist), (yyvsp[-1].stmlist));
	}
#line 1387 "src/y.tab.c"
    break;

  case 4: /* CLASS_DEC: PUBLIC CLASS IDENTIFIER BRACE VAR_LIST METHOD_LIST '}'  */
#line 101 "./src/parser.yacc"
        {
		(yyval.classdec) = A_ClassDecl((yyvsp[-6].token), (yyvsp[-4].id), NULL, (yyvsp[-2].varlist), (yyvsp[-1].methodlist));
	}
#line 1395 "src/y.tab.c"
    break;

  case 5: /* CLASS_DEC: PUBLIC CLASS IDENTIFIER EXTENDS IDENTIFIER BRACE VAR_LIST METHOD_LIST '}'  */
#line 106 "./src/parser.yacc"
        {
		(yyval.classdec) = A_ClassDecl((yyvsp[-8].token), (yyvsp[-6].id), (yyvsp[-4].id), (yyvsp[-2].varlist), (yyvsp[-1].methodlist));
	}
#line 1403 "src/y.tab.c"
    break;

  case 6: /* CLASS_DEC_LIST: CLASS_DEC CLASS_DEC_LIST  */
#line 112 "./src/parser.yacc"
        {
		(yyval.classdeclist) = A_ClassDeclList((yyvsp[-1].classdec), (yyvsp[0].classdeclist));
	}
#line 1411 "src/y.tab.c"
    break;

  case 7: /* CLASS_DEC_LIST: %empty  */
#line 116 "./src/parser.yacc"
        {
		(yyval.classdeclist) = NULL;
	}
#line 1419 "src/y.tab.c"
    break;

  case 8: /* VAR: TYPE IDENTIFIER ';'  */
#line 122 "./src/parser.yacc"
        {
		(yyval.var) = A_VarDecl((yyvsp[-2].type)->pos, (yyvsp[-2].type), (yyvsp[-1].id), NULL);
	}
#line 1427 "src/y.tab.c"
    break;

  case 9: /* VAR: TYPE IDENTIFIER '=' IntConst ';'  */
#line 127 "./src/parser.yacc"
        {
		(yyval.var) = A_VarDecl((yyvsp[-4].type)->pos, (yyvsp[-4].type), (yyvsp[-3].id), A_ExpList((yyvsp[-1].exp), NULL));
	}
#line 1435 "src/y.tab.c"
    break;

  case 10: /* VAR: TYPE IDENTIFIER '=' BRACE INT_CONST_LIST '}' ';'  */
#line 132 "./src/parser.yacc"
        {
		(yyval.var) = A_VarDecl((yyvsp[-6].type)->pos, (yyvsp[-6].type), (yyvsp[-5].id), (yyvsp[-2].explist));
	}
#line 1443 "src/y.tab.c"
    break;

  case 11: /* VAR_LIST: VAR VAR_LIST  */
#line 138 "./src/parser.yacc"
        {
		(yyval.varlist) = A_VarDeclList((yyvsp[-1].var), (yyvsp[0].varlist));
	}
#line 1451 "src/y.tab.c"
    break;

  case 12: /* VAR_LIST: %empty  */
#line 142 "./src/parser.yacc"
        {
		(yyval.varlist) = NULL;
	}
#line 1459 "src/y.tab.c"
    break;

  case 13: /* IntConst: INT_CONST  */
#line 148 "./src/parser.yacc"
        {
		(yyval.exp) = (yyvsp[0].exp);
	}
#line 1467 "src/y.tab.c"
    break;

  case 14: /* IntConst: OP_MINUS INT_CONST  */
#line 153 "./src/parser.yacc"
        {
		(yyval.exp) = A_NumConst(A_Pos((yyvsp[0].exp)->pos->line, (yyvsp[0].exp)->pos->pos - 1), -(yyvsp[0].exp)->u.num);
	}
#line 1475 "src/y.tab.c"
    break;

  case 15: /* INT_CONST_LIST: IntConst INT_CONST_REST  */
#line 159 "./src/parser.yacc"
        {
		(yyval.explist) = A_ExpList((yyvsp[-1].exp), (yyvsp[0].explist));
	}
#line 1483 "src/y.tab.c"
    break;

  case 16: /* INT_CONST_LIST: %empty  */
#line 163 "./src/parser.yacc"
        {
		(yyval.explist) = NULL;
	}
#line 1491 "src/y.tab.c"
    break;

  case 17: /* INT_CONST_REST: ',' IntConst INT_CONST_REST  */
#line 169 "./src/parser.yacc"
        {
		(yyval.explist) = A_ExpList((yyvsp[-1].exp), (yyvsp[0].explist));
	}
#line 1499 "src/y.tab.c"
    break;

  case 18: /* INT_CONST_REST: %empty  */
#line 173 "./src/parser.yacc"
        {
		(yyval.explist) = NULL;
	}
#line 1507 "src/y.tab.c"
    break;

  case 19: /* METHOD: PUBLIC TYPE IDENTIFIER PARENTHESIS FORMAL_LIST ')' BRACE VAR_LIST STM_LIST '}'  */
#line 179 "./src/parser.yacc"
        {
		(yyval.method) = A_MethodDecl((yyvsp[-9].token), (yyvsp[-8].type), (yyvsp[-7].id), (yyvsp[-5].formallist), (yyvsp[-2].varlist), (yyvsp[-1].stmlist));
	}
#line 1515 "src/y.tab.c"
    break;

  case 20: /* METHOD_LIST: METHOD METHOD_LIST  */
#line 185 "./src/parser.yacc"
        {
		(yyval.methodlist) = A_MethodDeclList((yyvsp[-1].method), (yyvsp[0].methodlist));
	}
#line 1523 "src/y.tab.c"
    break;

  case 21: /* METHOD_LIST: %empty  */
#line 189 "./src/parser.yacc"
        {
		(yyval.methodlist) = NULL;
	}
#line 1531 "src/y.tab.c"
    break;

  case 22: /* FORMAL_LIST: TYPE IDENTIFIER FORMAL_REST  */
#line 195 "./src/parser.yacc"
        {
		(yyval.formallist) = A_FormalList(A_Formal((yyvsp[-2].type)->pos, (yyvsp[-2].type), (yyvsp[-1].id)), (yyvsp[0].formallist));
	}
#line 1539 "src/y.tab.c"
    break;

  case 23: /* FORMAL_LIST: %empty  */
#line 199 "./src/parser.yacc"
        {
		(yyval.formallist) = NULL;
	}
#line 1547 "src/y.tab.c"
    break;

  case 24: /* FORMAL_REST: ',' TYPE IDENTIFIER FORMAL_REST  */
#line 205 "./src/parser.yacc"
        {
		(yyval.formallist) = A_FormalList(A_Formal((yyvsp[-2].type)->pos, (yyvsp[-2].type), (yyvsp[-1].id)), (yyvsp[0].formallist));
	}
#line 1555 "src/y.tab.c"
    break;

  case 25: /* FORMAL_REST: %empty  */
#line 209 "./src/parser.yacc"
        {
		(yyval.formallist) = NULL;
	}
#line 1563 "src/y.tab.c"
    break;

  case 26: /* TYPE: CLASS IDENTIFIER  */
#line 215 "./src/parser.yacc"
        {
		(yyval.type) = A_Type((yyvsp[-1].token), A_idType, (yyvsp[0].id));
	}
#line 1571 "src/y.tab.c"
    break;

  case 27: /* TYPE: INT  */
#line 220 "./src/parser.yacc"
        {
		(yyval.type) = A_Type((yyvsp[0].token), A_intType, NULL);
	}
#line 1579 "src/y.tab.c"
    break;

  case 28: /* TYPE: INT BRACKET ']'  */
#line 225 "./src/parser.yacc"
        {
		(yyval.type) = A_Type((yyvsp[-2].token), A_intArrType, NULL);
	}
#line 1587 "src/y.tab.c"
    break;

  case 29: /* STM: BRACE STM_LIST '}'  */
#line 231 "./src/parser.yacc"
        {
		(yyval.stm) = A_NestedStm((yyvsp[-2].token), (yyvsp[-1].stmlist));
	}
#line 1595 "src/y.tab.c"
    break;

  case 30: /* STM: IF PARENTHESIS EXP ')' STM ELSE STM  */
#line 236 "./src/parser.yacc"
        {
		(yyval.stm) = A_IfStm((yyvsp[-6].token), (yyvsp[-4].exp), (yyvsp[-2].stm), (yyvsp[0].stm));
	}
#line 1603 "src/y.tab.c"
    break;

  case 31: /* STM: IF PARENTHESIS EXP ')' STM  */
#line 241 "./src/parser.yacc"
        {
		(yyval.stm) = A_IfStm((yyvsp[-4].token), (yyvsp[-2].exp), (yyvsp[0].stm), NULL);
	}
#line 1611 "src/y.tab.c"
    break;

  case 32: /* STM: WHILE PARENTHESIS EXP ')' STM  */
#line 246 "./src/parser.yacc"
        {
		(yyval.stm) = A_WhileStm((yyvsp[-4].token), (yyvsp[-2].exp), (yyvsp[0].stm));
	}
#line 1619 "src/y.tab.c"
    break;

  case 33: /* STM: WHILE PARENTHESIS EXP ')' ';'  */
#line 251 "./src/parser.yacc"
        {
		(yyval.stm) = A_WhileStm((yyvsp[-4].token), (yyvsp[-2].exp), NULL);
	}
#line 1627 "src/y.tab.c"
    break;

  case 34: /* STM: EXP '=' EXP ';'  */
#line 256 "./src/parser.yacc"
        {
		(yyval.stm) = A_AssignStm((yyvsp[-3].exp)->pos, (yyvsp[-3].exp), (yyvsp[-1].exp));
	}
#line 1635 "src/y.tab.c"
    break;

  case 35: /* STM: EXP BRACKET ']' '=' BRACE EXP_LIST '}' ';'  */
#line 261 "./src/parser.yacc"
        {
		(yyval.stm) = A_ArrayInit((yyvsp[-7].exp)->pos, (yyvsp[-7].exp), (yyvsp[-2].explist));
	}
#line 1643 "src/y.tab.c"
    break;

  case 36: /* STM: EXP '.' IDENTIFIER PARENTHESIS EXP_LIST ')' ';'  */
#line 266 "./src/parser.yacc"
        {
		(yyval.stm) = A_CallStm((yyvsp[-6].exp)->pos, (yyvsp[-6].exp), (yyvsp[-4].id), (yyvsp[-2].explist));
	}
#line 1651 "src/y.tab.c"
    break;

  case 37: /* STM: CONTINUE ';'  */
#line 271 "./src/parser.yacc"
        {
		(yyval.stm) = A_Continue((yyvsp[-1].token));
	}
#line 1659 "src/y.tab.c"
    break;

  case 38: /* STM: BREAK ';'  */
#line 276 "./src/parser.yacc"
        {
		(yyval.stm) = A_Break((yyvsp[-1].token));
	}
#line 1667 "src/y.tab.c"
    break;

  case 39: /* STM: RETURN EXP ';'  */
#line 281 "./src/parser.yacc"
        {
		(yyval.stm) = A_Return((yyvsp[-2].token), (yyvsp[-1].exp));
	}
#line 1675 "src/y.tab.c"
    break;

  case 40: /* STM: PUTINT PARENTHESIS EXP ')' ';'  */
#line 286 "./src/parser.yacc"
        {
		(yyval.stm) = A_Putint((yyvsp[-4].token), (yyvsp[-2].exp));
	}
#line 1683 "src/y.tab.c"
    break;

  case 41: /* STM: PUTCH PARENTHESIS EXP ')' ';'  */
#line 291 "./src/parser.yacc"
        {
		(yyval.stm) = A_Putch((yyvsp[-4].token), (yyvsp[-2].exp));
	}
#line 1691 "src/y.tab.c"
    break;

  case 42: /* STM: PUTARRAY PARENTHESIS EXP ',' EXP ')' ';'  */
#line 296 "./src/parser.yacc"
        {
		(yyval.stm) = A_Putarray((yyvsp[-6].token), (yyvsp[-4].exp), (yyvsp[-2].exp));
	}
#line 1699 "src/y.tab.c"
    break;

  case 43: /* STM: STARTTIME PARENTHESIS ')' ';'  */
#line 301 "./src/parser.yacc"
        {
		(yyval.stm) = A_Starttime((yyvsp[-3].token));
	}
#line 1707 "src/y.tab.c"
    break;

  case 44: /* STM: STOPTIME PARENTHESIS ')' ';'  */
#line 306 "./src/parser.yacc"
        {
		(yyval.stm) = A_Stoptime((yyvsp[-3].token));
	}
#line 1715 "src/y.tab.c"
    break;

  case 45: /* STM_LIST: STM STM_LIST  */
#line 312 "./src/parser.yacc"
        {
		(yyval.stmlist) = A_StmList((yyvsp[-1].stm), (yyvsp[0].stmlist));
	}
#line 1723 "src/y.tab.c"
    break;

  case 46: /* STM_LIST: %empty  */
#line 316 "./src/parser.yacc"
        {
		(yyval.stmlist) = NULL;
	}
#line 1731 "src/y.tab.c"
    break;

  case 47: /* EXP: EXP OP_PLUS EXP  */
#line 322 "./src/parser.yacc"
        {
		(yyval.exp) = A_OpExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), A_plus, (yyvsp[0].exp));
	}
#line 1739 "src/y.tab.c"
    break;

  case 48: /* EXP: EXP OP_MINUS EXP  */
#line 327 "./src/parser.yacc"
        {
		(yyval.exp) = A_OpExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), A_minus, (yyvsp[0].exp));
	}
#line 1747 "src/y.tab.c"
    break;

  case 49: /* EXP: EXP OP_MULTIPLY EXP  */
#line 332 "./src/parser.yacc"
        {
		(yyval.exp) = A_OpExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), A_times, (yyvsp[0].exp));
	}
#line 1755 "src/y.tab.c"
    break;

  case 50: /* EXP: EXP OP_DIV EXP  */
#line 337 "./src/parser.yacc"
        {
		(yyval.exp) = A_OpExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), A_div, (yyvsp[0].exp));
	}
#line 1763 "src/y.tab.c"
    break;

  case 51: /* EXP: EXP AND EXP  */
#line 342 "./src/parser.yacc"
        {
		(yyval.exp) = A_OpExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), A_and, (yyvsp[0].exp));
	}
#line 1771 "src/y.tab.c"
    break;

  case 52: /* EXP: EXP OR EXP  */
#line 347 "./src/parser.yacc"
        {
		(yyval.exp) = A_OpExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), A_or, (yyvsp[0].exp));
	}
#line 1779 "src/y.tab.c"
    break;

  case 53: /* EXP: EXP LESS EXP  */
#line 352 "./src/parser.yacc"
        {
		(yyval.exp) = A_OpExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), A_less, (yyvsp[0].exp));
	}
#line 1787 "src/y.tab.c"
    break;

  case 54: /* EXP: EXP LE EXP  */
#line 357 "./src/parser.yacc"
        {
		(yyval.exp) = A_OpExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), A_le, (yyvsp[0].exp));
	}
#line 1795 "src/y.tab.c"
    break;

  case 55: /* EXP: EXP GREATER EXP  */
#line 362 "./src/parser.yacc"
        {
		(yyval.exp) = A_OpExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), A_greater, (yyvsp[0].exp));
	}
#line 1803 "src/y.tab.c"
    break;

  case 56: /* EXP: EXP GE EXP  */
#line 367 "./src/parser.yacc"
        {
		(yyval.exp) = A_OpExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), A_ge, (yyvsp[0].exp));
	}
#line 1811 "src/y.tab.c"
    break;

  case 57: /* EXP: EXP EQ EXP  */
#line 372 "./src/parser.yacc"
        {
		(yyval.exp) = A_OpExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), A_eq, (yyvsp[0].exp));
	}
#line 1819 "src/y.tab.c"
    break;

  case 58: /* EXP: EXP NEQ EXP  */
#line 377 "./src/parser.yacc"
        {
		(yyval.exp) = A_OpExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), A_ne, (yyvsp[0].exp));
	}
#line 1827 "src/y.tab.c"
    break;

  case 59: /* EXP: EXP BRACKET EXP ']'  */
#line 382 "./src/parser.yacc"
        {
		(yyval.exp) = A_ArrayExp((yyvsp[-3].exp)->pos, (yyvsp[-3].exp), (yyvsp[-1].exp));
	}
#line 1835 "src/y.tab.c"
    break;

  case 60: /* EXP: EXP '.' IDENTIFIER PARENTHESIS EXP_LIST ')'  */
#line 387 "./src/parser.yacc"
        {
		(yyval.exp) = A_CallExp((yyvsp[-5].exp)->pos, (yyvsp[-5].exp), (yyvsp[-3].id), (yyvsp[-1].explist));
	}
#line 1843 "src/y.tab.c"
    break;

  case 61: /* EXP: EXP '.' IDENTIFIER  */
#line 392 "./src/parser.yacc"
        {
		(yyval.exp) = A_ClassVarExp((yyvsp[-2].exp)->pos, (yyvsp[-2].exp), (yyvsp[0].id));
	}
#line 1851 "src/y.tab.c"
    break;

  case 62: /* EXP: INT_CONST  */
#line 397 "./src/parser.yacc"
        {
		(yyval.exp) = (yyvsp[0].exp);
	}
#line 1859 "src/y.tab.c"
    break;

  case 63: /* EXP: True  */
#line 402 "./src/parser.yacc"
        {
		(yyval.exp) = A_BoolConst((yyvsp[0].token), TRUE);
	}
#line 1867 "src/y.tab.c"
    break;

  case 64: /* EXP: False  */
#line 407 "./src/parser.yacc"
        {
		(yyval.exp) = A_BoolConst((yyvsp[0].token), FALSE);
	}
#line 1875 "src/y.tab.c"
    break;

  case 65: /* EXP: LENGTH PARENTHESIS EXP ')'  */
#line 412 "./src/parser.yacc"
        {
		(yyval.exp) = A_LengthExp((yyvsp[-3].token), (yyvsp[-1].exp));
	}
#line 1883 "src/y.tab.c"
    break;

  case 66: /* EXP: IDENTIFIER  */
#line 417 "./src/parser.yacc"
        {
		(yyval.exp) = A_IdExp(A_Pos(line, pos - strlen((yyvsp[0].id))), (yyvsp[0].id));
	}
#line 1891 "src/y.tab.c"
    break;

  case 67: /* EXP: THIS  */
#line 422 "./src/parser.yacc"
        {
		(yyval.exp) = A_ThisExp((yyvsp[0].token));
	}
#line 1899 "src/y.tab.c"
    break;

  case 68: /* EXP: NEW INT BRACKET EXP ']'  */
#line 427 "./src/parser.yacc"
        {
		(yyval.exp) = A_NewIntArrExp((yyvsp[-4].token), (yyvsp[-1].exp));
	}
#line 1907 "src/y.tab.c"
    break;

  case 69: /* EXP: NEW IDENTIFIER PARENTHESIS ')'  */
#line 432 "./src/parser.yacc"
        {
		(yyval.exp) = A_NewObjExp((yyvsp[-3].token), (yyvsp[-2].id));
	}
#line 1915 "src/y.tab.c"
    break;

  case 70: /* EXP: NOT EXP  */
#line 437 "./src/parser.yacc"
        {
		(yyval.exp) = A_NotExp((yyvsp[-1].token), (yyvsp[0].exp));
	}
#line 1923 "src/y.tab.c"
    break;

  case 71: /* EXP: OP_MINUS EXP  */
#line 442 "./src/parser.yacc"
        {
		(yyval.exp) = A_MinusExp((yyvsp[-1].token), (yyvsp[0].exp));
	}
#line 1931 "src/y.tab.c"
    break;

  case 72: /* EXP: PARENTHESIS EXP ')'  */
#line 447 "./src/parser.yacc"
        {
		(yyval.exp) = A_EscExp((yyvsp[-2].token), NULL, (yyvsp[-1].exp));
	}
#line 1939 "src/y.tab.c"
    break;

  case 73: /* EXP: PARENTHESIS BRACE STM_LIST '}' EXP ')'  */
#line 452 "./src/parser.yacc"
        {
		(yyval.exp) = A_EscExp((yyvsp[-5].token), (yyvsp[-3].stmlist), (yyvsp[-1].exp));
	}
#line 1947 "src/y.tab.c"
    break;

  case 74: /* EXP: GETINT PARENTHESIS ')'  */
#line 457 "./src/parser.yacc"
        {
		(yyval.exp) = A_Getint((yyvsp[-2].token));
	}
#line 1955 "src/y.tab.c"
    break;

  case 75: /* EXP: GETCH PARENTHESIS ')'  */
#line 462 "./src/parser.yacc"
        {
		(yyval.exp) = A_Getch((yyvsp[-2].token));
	}
#line 1963 "src/y.tab.c"
    break;

  case 76: /* EXP: GETARRAY PARENTHESIS EXP ')'  */
#line 467 "./src/parser.yacc"
        {
		(yyval.exp) = A_Getarray((yyvsp[-3].token), (yyvsp[-1].exp));
	}
#line 1971 "src/y.tab.c"
    break;

  case 77: /* EXP_LIST: EXP EXP_REST  */
#line 473 "./src/parser.yacc"
        {
		(yyval.explist) = A_ExpList((yyvsp[-1].exp), (yyvsp[0].explist));
	}
#line 1979 "src/y.tab.c"
    break;

  case 78: /* EXP_LIST: %empty  */
#line 477 "./src/parser.yacc"
        {
		(yyval.explist) = NULL;
	}
#line 1987 "src/y.tab.c"
    break;

  case 79: /* EXP_REST: ',' EXP EXP_REST  */
#line 483 "./src/parser.yacc"
        {
		(yyval.explist) = A_ExpList((yyvsp[-1].exp), (yyvsp[0].explist));
	}
#line 1995 "src/y.tab.c"
    break;

  case 80: /* EXP_REST: %empty  */
#line 487 "./src/parser.yacc"
        {
		(yyval.explist) = NULL;
	}
#line 2003 "src/y.tab.c"
    break;


#line 2007 "src/y.tab.c"

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

#line 492 "./src/parser.yacc"


void yyerror(s)
char *s;
{
  fprintf(stderr, "%s\n",s);
}

int yywrap()
{
  return(1);
}
