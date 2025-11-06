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
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 26 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"

#include "clangbuilder.h"
#include "clangparser.h"
/*#include "serverparam.h"*/

#define    yymaxdepth RCSS_CLANG_maxdepth
#define    yyparse    RCSS_CLANG_parse
#define    yylex      RCSS_CLANG_lex
#define    yyerror    RCSS_CLANG_error
#define    yylval     RCSS_CLANG_lval
#define    yychar     RCSS_CLANG_char
#define    yydebug    RCSS_CLANG_debug
#define    yypact     RCSS_CLANG_pact
#define    yyr1       RCSS_CLANG_r1
#define    yyr2       RCSS_CLANG_r2
#define    yydef      RCSS_CLANG_def
#define    yychk      RCSS_CLANG_chk
#define    yypgo      RCSS_CLANG_pgo
#define    yyact      RCSS_CLANG_act
#define    yyexca     RCSS_CLANG_exca
#define    yyerrflag  RCSS_CLANG_errflag
#define    yynerrs    RCSS_CLANG_nerrs
#define    yyps       RCSS_CLANG_ps
#define    yypv       RCSS_CLANG_pv
#define    yys        RCSS_CLANG_s
#define    yy_yys     RCSS_CLANG_yys
#define    yystate    RCSS_CLANG_state
#define    yytmp      RCSS_CLANG_tmp
#define    yyv        RCSS_CLANG_v
#define    yy_yyv     RCSS_CLANG_yyv
#define    yyval      RCSS_CLANG_val
#define    yylloc     RCSS_CLANG_lloc
#define    yyreds     RCSS_CLANG_reds
#define    yytoks     RCSS_CLANG_toks
#define    yylhs      RCSS_CLANG_yylhs
#define    yylen      RCSS_CLANG_yylen
#define    yydefred   RCSS_CLANG_yydefred
#define    yydgoto    RCSS_CLANG_yydgoto
#define    yysindex   RCSS_CLANG_yysindex
#define    yyrindex   RCSS_CLANG_yyrindex
#define    yygindex   RCSS_CLANG_yygindex
#define    yytable    RCSS_CLANG_yytable
#define    yycheck    RCSS_CLANG_yycheck
#define    yyname     RCSS_CLANG_yyname
#define    yyrule     RCSS_CLANG_yyrule

#define YYERROR_VERBOSE

void yyerror (rcss::clang::Parser::Param &param, const char* s);
int yyerror (rcss::clang::Parser::Param &param, char* s);

namespace
{
  inline
  rcss::clang::Builder&
  getBuilder( rcss::clang::Parser::Param &param )
  { return param.getBuilder(); }

#define YYSTYPE rcss::clang::Parser::Lexer::Holder

  inline
  int
  yylex( YYSTYPE* holder, rcss::clang::Parser::Param& param )
  {
    int rval =  param.getLexer().lex( *holder );
//    cout << rval << endl;
    return rval;
  }

}

#define BUILDER getBuilder( param )


#line 146 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"

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

#include "coach_lang_parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_RCSS_CLANG_INT = 3,             /* RCSS_CLANG_INT  */
  YYSYMBOL_RCSS_CLANG_REAL = 4,            /* RCSS_CLANG_REAL  */
  YYSYMBOL_RCSS_CLANG_STR = 5,             /* RCSS_CLANG_STR  */
  YYSYMBOL_RCSS_CLANG_VAR = 6,             /* RCSS_CLANG_VAR  */
  YYSYMBOL_RCSS_CLANG_LP = 7,              /* "("  */
  YYSYMBOL_RCSS_CLANG_RP = 8,              /* ")"  */
  YYSYMBOL_RCSS_CLANG_LB = 9,              /* "{"  */
  YYSYMBOL_RCSS_CLANG_RB = 10,             /* "}"  */
  YYSYMBOL_RCSS_CLANG_SAY = 11,            /* "say"  */
  YYSYMBOL_RCSS_CLANG_HEAR = 12,           /* "hear"  */
  YYSYMBOL_RCSS_CLANG_COACH_SIDE = 13,     /* RCSS_CLANG_COACH_SIDE  */
  YYSYMBOL_RCSS_CLANG_META = 14,           /* "meta"  */
  YYSYMBOL_RCSS_CLANG_FREEFORM = 15,       /* "freeform"  */
  YYSYMBOL_RCSS_CLANG_UNSUPP = 16,         /* "unsupported_clang"  */
  YYSYMBOL_RCSS_CLANG_INFO = 17,           /* "info"  */
  YYSYMBOL_RCSS_CLANG_ADVICE = 18,         /* "advice"  */
  YYSYMBOL_RCSS_CLANG_DEFINE = 19,         /* "define"  */
  YYSYMBOL_RCSS_CLANG_DEFINEC = 20,        /* "definec"  */
  YYSYMBOL_RCSS_CLANG_DEFINED = 21,        /* "defined"  */
  YYSYMBOL_RCSS_CLANG_DEFINER = 22,        /* "definer"  */
  YYSYMBOL_RCSS_CLANG_DEFINEA = 23,        /* "definea"  */
  YYSYMBOL_RCSS_CLANG_VER = 24,            /* "ver"  */
  YYSYMBOL_RCSS_CLANG_NULL_REGION = 25,    /* "null"  */
  YYSYMBOL_RCSS_CLANG_QUAD = 26,           /* "quad"  */
  YYSYMBOL_RCSS_CLANG_ARC = 27,            /* "arc"  */
  YYSYMBOL_RCSS_CLANG_PT = 28,             /* "pt"  */
  YYSYMBOL_RCSS_CLANG_REG = 29,            /* "reg"  */
  YYSYMBOL_RCSS_CLANG_TRI = 30,            /* "tri"  */
  YYSYMBOL_RCSS_CLANG_REC = 31,            /* "rec"  */
  YYSYMBOL_RCSS_CLANG_BALL = 32,           /* "ball"  */
  YYSYMBOL_RCSS_CLANG_POS = 33,            /* "pos"  */
  YYSYMBOL_RCSS_CLANG_HOME = 34,           /* "home"  */
  YYSYMBOL_RCSS_CLANG_BALLTO = 35,         /* "bto"  */
  YYSYMBOL_RCSS_CLANG_MARK = 36,           /* "mark"  */
  YYSYMBOL_RCSS_CLANG_MARK_LINE = 37,      /* "markl"  */
  YYSYMBOL_RCSS_CLANG_OFFLINE = 38,        /* "oline"  */
  YYSYMBOL_RCSS_CLANG_HTYPE = 39,          /* "htype"  */
  YYSYMBOL_RCSS_CLANG_BMOVE_TOKEN = 40,    /* RCSS_CLANG_BMOVE_TOKEN  */
  YYSYMBOL_RCSS_CLANG_DO_DONT = 41,        /* RCSS_CLANG_DO_DONT  */
  YYSYMBOL_RCSS_CLANG_TEAM = 42,           /* RCSS_CLANG_TEAM  */
  YYSYMBOL_RCSS_CLANG_TRUE_KW = 43,        /* "true"  */
  YYSYMBOL_RCSS_CLANG_FALSE_KW = 44,       /* "false"  */
  YYSYMBOL_RCSS_CLANG_PPOS = 45,           /* "ppos"  */
  YYSYMBOL_RCSS_CLANG_BPOS = 46,           /* "bpos"  */
  YYSYMBOL_RCSS_CLANG_BOWNER = 47,         /* "bowner"  */
  YYSYMBOL_RCSS_CLANG_AND = 48,            /* "and"  */
  YYSYMBOL_RCSS_CLANG_OR = 49,             /* "or"  */
  YYSYMBOL_RCSS_CLANG_NOT = 50,            /* "not"  */
  YYSYMBOL_RCSS_CLANG_PLAY_MODE_KW = 51,   /* "playm"  */
  YYSYMBOL_RCSS_CLANG_PLAY_MODE = 52,      /* RCSS_CLANG_PLAY_MODE  */
  YYSYMBOL_RCSS_CLANG_CLEAR = 53,          /* "clear"  */
  YYSYMBOL_RCSS_CLANG_L = 54,              /* "<"  */
  YYSYMBOL_RCSS_CLANG_G = 55,              /* ">"  */
  YYSYMBOL_RCSS_CLANG_E = 56,              /* "=="  */
  YYSYMBOL_RCSS_CLANG_NE = 57,             /* "!="  */
  YYSYMBOL_RCSS_CLANG_LE = 58,             /* "<="  */
  YYSYMBOL_RCSS_CLANG_GE = 59,             /* ">="  */
  YYSYMBOL_RCSS_CLANG_PLUS = 60,           /* "+"  */
  YYSYMBOL_RCSS_CLANG_MINUS = 61,          /* "-"  */
  YYSYMBOL_RCSS_CLANG_MULT = 62,           /* "*"  */
  YYSYMBOL_RCSS_CLANG_DIV = 63,            /* "/"  */
  YYSYMBOL_RCSS_CLANG_TIME = 64,           /* "time"  */
  YYSYMBOL_RCSS_CLANG_OPP_GOAL = 65,       /* "opp_goals"  */
  YYSYMBOL_RCSS_CLANG_OUR_GOAL = 66,       /* "own_goals"  */
  YYSYMBOL_RCSS_CLANG_GOAL_DIFF = 67,      /* "goal_diff"  */
  YYSYMBOL_RCSS_CLANG_UNUM = 68,           /* "unum"  */
  YYSYMBOL_RCSS_CLANG_PASS = 69,           /* "pass"  */
  YYSYMBOL_RCSS_CLANG_DRIBBLE = 70,        /* "dribble"  */
  YYSYMBOL_RCSS_CLANG_SHOOT = 71,          /* "shoot"  */
  YYSYMBOL_RCSS_CLANG_HOLD = 72,           /* "hold"  */
  YYSYMBOL_RCSS_CLANG_INTERCEPT = 73,      /* "intercept"  */
  YYSYMBOL_RCSS_CLANG_TACKLE = 74,         /* "tackle"  */
  YYSYMBOL_RCSS_CLANG_RULE = 75,           /* "rule"  */
  YYSYMBOL_RCSS_CLANG_ON_OFF = 76,         /* "on|off"  */
  YYSYMBOL_RCSS_CLANG_ALL = 77,            /* "all"  */
  YYSYMBOL_RCSS_CLANG_DEL = 78,            /* "delete"  */
  YYSYMBOL_RCSS_CLANG_DEFRULE = 79,        /* "definerule"  */
  YYSYMBOL_RCSS_CLANG_MODEL = 80,          /* "model"  */
  YYSYMBOL_RCSS_CLANG_DIREC = 81,          /* "direc"  */
  YYSYMBOL_RCSS_CLANG_ERROR = 82,          /* RCSS_CLANG_ERROR  */
  YYSYMBOL_YYACCEPT = 83,                  /* $accept  */
  YYSYMBOL_command = 84,                   /* command  */
  YYSYMBOL_message = 85,                   /* message  */
  YYSYMBOL_rule_mess = 86,                 /* rule_mess  */
  YYSYMBOL_activation_list = 87,           /* activation_list  */
  YYSYMBOL_activation_element = 88,        /* activation_element  */
  YYSYMBOL_id_list = 89,                   /* id_list  */
  YYSYMBOL_id_list2 = 90,                  /* id_list2  */
  YYSYMBOL_del_mess = 91,                  /* del_mess  */
  YYSYMBOL_meta_mess = 92,                 /* meta_mess  */
  YYSYMBOL_meta_token_list = 93,           /* meta_token_list  */
  YYSYMBOL_meta_token = 94,                /* meta_token  */
  YYSYMBOL_define_mess = 95,               /* define_mess  */
  YYSYMBOL_define_token_list = 96,         /* define_token_list  */
  YYSYMBOL_define_token = 97,              /* define_token  */
  YYSYMBOL_define_rule = 98,               /* define_rule  */
  YYSYMBOL_rule = 99,                      /* rule  */
  YYSYMBOL_rule_list = 100,                /* rule_list  */
  YYSYMBOL_freeform_mess = 101,            /* freeform_mess  */
  YYSYMBOL_unsupp_mess = 102,              /* unsupp_mess  */
  YYSYMBOL_info_mess = 103,                /* info_mess  */
  YYSYMBOL_advice_mess = 104,              /* advice_mess  */
  YYSYMBOL_token = 105,                    /* token  */
  YYSYMBOL_token_list = 106,               /* token_list  */
  YYSYMBOL_action = 107,                   /* action  */
  YYSYMBOL_directive = 108,                /* directive  */
  YYSYMBOL_action_list = 109,              /* action_list  */
  YYSYMBOL_directive_list = 110,           /* directive_list  */
  YYSYMBOL_condition = 111,                /* condition  */
  YYSYMBOL_cond_comp = 112,                /* cond_comp  */
  YYSYMBOL_time_comp = 113,                /* time_comp  */
  YYSYMBOL_opp_goal_comp = 114,            /* opp_goal_comp  */
  YYSYMBOL_our_goal_comp = 115,            /* our_goal_comp  */
  YYSYMBOL_goal_diff_comp = 116,           /* goal_diff_comp  */
  YYSYMBOL_comp = 117,                     /* comp  */
  YYSYMBOL_condition_list = 118,           /* condition_list  */
  YYSYMBOL_region = 119,                   /* region  */
  YYSYMBOL_region_list = 120,              /* region_list  */
  YYSYMBOL_point = 121,                    /* point  */
  YYSYMBOL_point_list = 122,               /* point_list  */
  YYSYMBOL_num = 123,                      /* num  */
  YYSYMBOL_unum_set = 124,                 /* unum_set  */
  YYSYMBOL_unum_list = 125,                /* unum_list  */
  YYSYMBOL_unum = 126,                     /* unum  */
  YYSYMBOL_bmove_token_set = 127,          /* bmove_token_set  */
  YYSYMBOL_bmove_token_list = 128          /* bmove_token_list  */
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
#define YYFINAL  22
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   339

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  83
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  139
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  315

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   337


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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   225,   225,   226,   227,   237,   238,   239,   240,   241,
     242,   243,   244,   248,   255,   256,   260,   267,   280,   284,
     291,   303,   318,   327,   334,   335,   339,   347,   354,   355,
     359,   371,   383,   395,   407,   414,   426,   441,   445,   449,
     455,   456,   461,   478,   486,   494,   502,   506,   512,   513,
     517,   521,   525,   529,   533,   537,   541,   545,   549,   553,
     565,   570,   575,   580,   585,   590,   595,   600,   608,   612,
     627,   628,   631,   632,   636,   640,   644,   649,   653,   657,
     661,   665,   669,   673,   681,   685,   696,   709,   710,   711,
     712,   716,   720,   727,   731,   738,   742,   749,   753,   759,
     759,   759,   759,   760,   760,   764,   768,   775,   779,   784,
     791,   795,   807,   811,   816,   824,   825,   829,   833,   837,
     841,   846,   858,   871,   878,   883,   888,   893,   898,   905,
     909,   916,   922,   923,   927,   931,   944,   959,   963,   967
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
  "\"end of file\"", "error", "\"invalid token\"", "RCSS_CLANG_INT",
  "RCSS_CLANG_REAL", "RCSS_CLANG_STR", "RCSS_CLANG_VAR", "\"(\"", "\")\"",
  "\"{\"", "\"}\"", "\"say\"", "\"hear\"", "RCSS_CLANG_COACH_SIDE",
  "\"meta\"", "\"freeform\"", "\"unsupported_clang\"", "\"info\"",
  "\"advice\"", "\"define\"", "\"definec\"", "\"defined\"", "\"definer\"",
  "\"definea\"", "\"ver\"", "\"null\"", "\"quad\"", "\"arc\"", "\"pt\"",
  "\"reg\"", "\"tri\"", "\"rec\"", "\"ball\"", "\"pos\"", "\"home\"",
  "\"bto\"", "\"mark\"", "\"markl\"", "\"oline\"", "\"htype\"",
  "RCSS_CLANG_BMOVE_TOKEN", "RCSS_CLANG_DO_DONT", "RCSS_CLANG_TEAM",
  "\"true\"", "\"false\"", "\"ppos\"", "\"bpos\"", "\"bowner\"", "\"and\"",
  "\"or\"", "\"not\"", "\"playm\"", "RCSS_CLANG_PLAY_MODE", "\"clear\"",
  "\"<\"", "\">\"", "\"==\"", "\"!=\"", "\"<=\"", "\">=\"", "\"+\"",
  "\"-\"", "\"*\"", "\"/\"", "\"time\"", "\"opp_goals\"", "\"own_goals\"",
  "\"goal_diff\"", "\"unum\"", "\"pass\"", "\"dribble\"", "\"shoot\"",
  "\"hold\"", "\"intercept\"", "\"tackle\"", "\"rule\"", "\"on|off\"",
  "\"all\"", "\"delete\"", "\"definerule\"", "\"model\"", "\"direc\"",
  "RCSS_CLANG_ERROR", "$accept", "command", "message", "rule_mess",
  "activation_list", "activation_element", "id_list", "id_list2",
  "del_mess", "meta_mess", "meta_token_list", "meta_token", "define_mess",
  "define_token_list", "define_token", "define_rule", "rule", "rule_list",
  "freeform_mess", "unsupp_mess", "info_mess", "advice_mess", "token",
  "token_list", "action", "directive", "action_list", "directive_list",
  "condition", "cond_comp", "time_comp", "opp_goal_comp", "our_goal_comp",
  "goal_diff_comp", "comp", "condition_list", "region", "region_list",
  "point", "point_list", "num", "unum_set", "unum_list", "unum",
  "bmove_token_set", "bmove_token_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-234)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      10,    48,    42,  -234,  -234,  -234,  -234,  -234,  -234,  -234,
    -234,  -234,    54,    88,    87,    99,    90,   102,   102,   108,
     118,     9,  -234,    22,   125,   116,   117,    25,  -234,   139,
    -234,    18,  -234,    41,    61,     8,   112,  -234,    80,   204,
    -234,  -234,   154,  -234,   155,  -234,   192,    52,  -234,  -234,
    -234,    20,   218,  -234,  -234,  -234,   232,   236,   240,   244,
     259,  -234,  -234,     9,  -234,  -234,  -234,    93,  -234,    54,
    -234,  -234,   245,  -234,   121,   109,  -234,    20,   109,   168,
     251,   -57,   249,   258,  -234,  -234,   260,  -234,   151,   261,
     262,   225,   168,   229,    20,    20,    20,   220,   151,   151,
     151,   151,   146,   265,  -234,  -234,  -234,  -234,  -234,   234,
    -234,   137,   266,   268,  -234,   194,   269,  -234,  -234,   144,
     270,    13,    13,  -234,  -234,  -234,  -234,  -234,  -234,  -234,
    -234,  -234,   126,  -234,  -234,   271,   273,   271,  -234,   191,
     195,   274,   275,   276,   281,   282,   283,   271,   271,  -234,
     246,  -234,  -234,  -234,  -234,    15,   279,   284,   284,    89,
     168,   284,   284,  -234,    50,  -234,   168,   168,   127,   271,
     127,   168,   286,   168,   127,   168,   285,   287,   288,   271,
    -234,    67,  -234,  -234,  -234,  -234,  -234,  -234,  -234,   233,
     289,  -234,   290,  -234,  -234,  -234,  -234,  -234,  -234,  -234,
    -234,  -234,   291,   292,   271,  -234,   284,    52,   293,   237,
      52,  -234,   239,   284,   284,  -234,   284,   284,   284,   284,
     294,   295,   296,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,  -234,  -234,  -234,   308,     2,  -234,  -234,
    -234,   152,  -234,   314,  -234,  -234,  -234,   251,   284,    52,
    -234,   310,   311,   312,   252,  -234,  -234,   284,   313,   199,
     199,  -234,  -234,  -234,  -234,   250,   315,  -234,  -234,  -234,
    -234,  -234,  -234,  -234,  -234,  -234,  -234,  -234,   133,  -234,
       5,   243,  -234,  -234,   168,  -234,   247,   284,    52,  -234,
    -234,  -234,  -234,   316,   317,  -234,  -234,    -5,  -234,  -234,
    -234,  -234,   318,  -234,  -234,   319,    52,  -234,  -234,  -234,
    -234,  -234,  -234,   320,  -234
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     3,    11,    12,     5,     9,     6,    10,
       7,     8,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     1,     0,     0,     0,     0,     0,    25,     0,
      43,     0,    49,     0,     0,     0,     0,    29,     0,     0,
      15,    17,     0,    19,     0,     2,     0,     0,    23,    24,
      42,     0,     0,    44,    48,    45,     0,     0,     0,     0,
       0,    27,    28,     0,    13,    14,    21,     0,    22,     0,
     129,   130,     0,    83,     0,     0,    47,     0,     0,     0,
       0,     0,     0,     0,    20,    18,     0,    26,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    69,     0,
      73,     0,     0,     0,   111,     0,     0,   112,    59,     0,
       0,     0,     0,    34,    16,     4,    99,   102,   103,   104,
     100,   101,     0,    74,    75,     0,     0,     0,   106,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    84,
       0,    46,    72,    30,    31,     0,     0,     0,     0,     0,
       0,     0,     0,   128,     0,    32,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      33,     0,    39,    35,    36,    92,    94,    96,    98,     0,
       0,    77,     0,    80,   105,    81,    82,    79,    91,    93,
      95,    97,     0,     0,     0,   107,     0,     0,     0,     0,
       0,   116,     0,     0,     0,   123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,    65,    66,     0,     0,   134,   136,
     135,     0,   132,     0,    78,    86,    85,     0,     0,     0,
     119,     0,     0,     0,     0,   110,   115,     0,     0,   124,
     125,   126,   127,    50,    51,     0,     0,    53,    54,    56,
      55,    57,    58,    63,    60,    61,    62,    67,     0,    41,
       0,     0,   131,   133,     0,    71,     0,     0,     0,   120,
     122,   121,   117,     0,     0,   114,   139,     0,    52,    38,
      40,    37,     0,    68,    70,     0,     0,   118,   113,   137,
     138,    76,   108,     0,   109
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -234,  -234,     6,  -234,  -234,   255,   -11,  -234,  -234,  -234,
    -234,   277,  -234,  -234,   297,  -234,  -119,  -234,  -234,  -234,
    -234,  -234,   230,   321,  -233,   -77,  -234,    60,   -51,  -234,
    -234,  -234,  -234,  -234,   130,   227,   -90,  -234,  -111,    16,
    -153,   -20,  -234,    91,  -234,  -234
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     2,     3,     4,    39,    40,   182,    67,     5,     6,
      27,    28,     7,    36,    37,    82,   183,   280,     8,     9,
      10,    11,    32,    33,   120,   110,   286,   111,   138,   103,
     104,   105,   106,   107,   132,   139,   116,   212,   117,   164,
      72,   190,   241,   242,   266,   297
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      75,   113,   136,   184,   163,   309,   210,   108,    41,   278,
      44,    41,   181,   299,   285,    41,    42,     1,    24,    41,
     181,    51,   155,   121,   122,    73,   112,    74,    56,    57,
      58,    59,    26,    48,   152,   310,    14,    15,    16,    17,
      18,    19,    22,   159,   163,   141,   206,   207,    31,    53,
     213,   214,    83,   304,   249,    70,    71,   254,   215,    12,
      13,    23,    14,    15,    16,    17,    18,    19,    31,    55,
     211,    52,    73,    66,    74,    86,   220,   221,   222,    43,
     225,   227,    43,   229,   230,   232,    43,    60,   194,   194,
      43,    25,    70,    71,    26,   248,   288,    20,    30,    84,
      21,    85,   257,   258,    29,   163,   163,   163,   163,    31,
     216,   217,   218,   219,   108,    35,   109,   192,   279,    35,
      61,   208,   256,    20,    88,    38,    21,   202,   203,    46,
     237,   209,   114,    45,   115,   306,   189,   287,    73,    66,
      74,    47,   108,   293,   109,   151,   294,    50,   223,   224,
     226,   147,   148,   313,   231,   238,    63,   239,   240,   236,
      66,   300,   282,    68,    89,    90,    91,    92,    93,    94,
      95,    96,    97,   114,   150,   115,   305,   166,   167,   168,
     169,   170,   171,   172,   247,    98,    99,   100,   101,   102,
     185,   186,   187,   188,   302,    69,    73,   173,    74,   193,
      73,   155,    74,   195,   152,   126,   127,   128,   129,   130,
     131,    38,    64,   174,   175,   176,   177,   178,   179,   156,
     157,   158,   159,   160,   161,   162,    76,   237,   143,   144,
     145,   146,   259,   260,   261,   262,   238,    77,   239,   240,
     251,    78,   252,   253,   114,    79,   115,   255,   108,    80,
     109,   301,   118,    87,   119,   303,   118,   123,   119,   155,
     292,   218,   219,    54,    54,    81,   124,   135,   125,   133,
     134,   137,   142,   149,   153,   150,   154,   165,   180,   198,
     189,   191,   196,   197,   199,   200,   201,   205,   204,   228,
     296,   155,   243,   233,    65,   234,   235,   281,   244,   245,
     246,   250,   263,   264,    49,   265,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   284,   289,   290,
     291,   295,   140,   298,   307,   308,   311,   312,   314,     0,
       0,     0,   283,    62,     0,     0,     0,     0,     0,    34
};

static const yytype_int16 yycheck[] =
{
      51,    78,    92,   122,   115,    10,   159,     5,     6,     7,
      21,     6,     7,     8,   247,     6,     7,     7,    12,     6,
       7,     3,     7,    80,    81,     5,    77,     7,    20,    21,
      22,    23,     7,     8,   111,    40,    14,    15,    16,    17,
      18,    19,     0,    28,   155,    96,   157,   158,     7,     8,
     161,   162,    63,   286,   207,     3,     4,   210,     8,    11,
      12,     7,    14,    15,    16,    17,    18,    19,     7,     8,
     160,    53,     5,     6,     7,    69,   166,   167,   168,    77,
     170,   171,    77,   173,   174,   175,    77,    79,   139,   140,
      77,     3,     3,     4,     7,   206,   249,    75,     8,     6,
      78,     8,   213,   214,     5,   216,   217,   218,   219,     7,
      60,    61,    62,    63,     5,     7,     7,   137,   237,     7,
       8,    32,   212,    75,     3,     7,    78,   147,   148,    13,
     181,    42,     5,     8,     7,   288,     9,   248,     5,     6,
       7,    24,     5,   254,     7,     8,   257,     8,   168,   169,
     170,     5,     6,   306,   174,     3,    76,     5,     6,   179,
       6,   280,    10,     8,    43,    44,    45,    46,    47,    48,
      49,    50,    51,     5,    41,     7,   287,    33,    34,    35,
      36,    37,    38,    39,   204,    64,    65,    66,    67,    68,
      64,    65,    66,    67,   284,     3,     5,    53,     7,     8,
       5,     7,     7,     8,   281,    54,    55,    56,    57,    58,
      59,     7,     8,    69,    70,    71,    72,    73,    74,    25,
      26,    27,    28,    29,    30,    31,     8,   278,    98,    99,
     100,   101,   216,   217,   218,   219,     3,     5,     5,     6,
       3,     5,     5,     6,     5,     5,     7,     8,     5,     5,
       7,     8,     5,     8,     7,     8,     5,     8,     7,     7,
       8,    62,    63,    33,    34,     6,     8,    42,     8,     8,
       8,    42,    52,     8,     8,    41,     8,     8,     8,     3,
       9,     8,     8,     8,     3,     3,     3,     8,    42,     3,
      40,     7,     3,     8,    39,     8,     8,   237,     8,     8,
       8,     8,     8,     8,    27,     9,     8,     8,     8,     8,
       8,     8,     8,     8,     8,     8,     8,     3,     8,     8,
       8,     8,    95,     8,     8,     8,     8,     8,     8,    -1,
      -1,    -1,   241,    36,    -1,    -1,    -1,    -1,    -1,    18
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     7,    84,    85,    86,    91,    92,    95,   101,   102,
     103,   104,    11,    12,    14,    15,    16,    17,    18,    19,
      75,    78,     0,     7,    85,     3,     7,    93,    94,     5,
       8,     7,   105,   106,   106,     7,    96,    97,     7,    87,
      88,     6,     7,    77,    89,     8,    13,    24,     8,    94,
       8,     3,    53,     8,   105,     8,    20,    21,    22,    23,
      79,     8,    97,    76,     8,    88,     6,    90,     8,     3,
       3,     4,   123,     5,     7,   111,     8,     5,     5,     5,
       5,     6,    98,    89,     6,     8,    85,     8,     3,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    64,    65,
      66,    67,    68,   112,   113,   114,   115,   116,     5,     7,
     108,   110,   111,   108,     5,     7,   119,   121,     5,     7,
     107,    80,    81,     8,     8,     8,    54,    55,    56,    57,
      58,    59,   117,     8,     8,    42,   119,    42,   111,   118,
     118,   111,    52,   117,   117,   117,   117,     5,     6,     8,
      41,     8,   108,     8,     8,     7,    25,    26,    27,    28,
      29,    30,    31,   121,   122,     8,    33,    34,    35,    36,
      37,    38,    39,    53,    69,    70,    71,    72,    73,    74,
       8,     7,    89,    99,    99,    64,    65,    66,    67,     9,
     124,     8,   124,     8,   111,     8,     8,     8,     3,     3,
       3,     3,   124,   124,    42,     8,   121,   121,    32,    42,
     123,   119,   120,   121,   121,     8,    60,    61,    62,    63,
     119,   119,   119,   124,   124,   119,   124,   119,     3,   119,
     119,   124,   119,     8,     8,     8,   124,   111,     3,     5,
       6,   125,   126,     3,     8,     8,     8,   124,   121,   123,
       8,     3,     5,     6,   123,     8,   119,   121,   121,   122,
     122,   122,   122,     8,     8,     9,   127,     8,     8,     8,
       8,     8,     8,     8,     8,     8,     8,     8,     7,    99,
     100,   110,    10,   126,     3,   107,   109,   121,   123,     8,
       8,     8,     8,   121,   121,     8,    40,   128,     8,     8,
      99,     8,   119,     8,   107,   121,   123,     8,     8,    10,
      40,     8,     8,   123,     8
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    83,    84,    84,    84,    85,    85,    85,    85,    85,
      85,    85,    85,    86,    87,    87,    88,    89,    89,    89,
      90,    90,    91,    92,    93,    93,    94,    95,    96,    96,
      97,    97,    97,    97,    97,    98,    98,    99,    99,    99,
     100,   100,   101,   102,   103,   104,   105,   105,   106,   106,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   108,   108,
     109,   109,   110,   110,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   112,   112,   112,
     112,   113,   113,   114,   114,   115,   115,   116,   116,   117,
     117,   117,   117,   117,   117,   118,   118,   119,   119,   119,
     119,   119,   119,   119,   119,   120,   120,   121,   121,   121,
     121,   121,   121,   121,   122,   122,   122,   122,   122,   123,
     123,   124,   125,   125,   126,   126,   126,   127,   128,   128
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     1,     7,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     2,     1,     4,     1,     3,     1,
       2,     1,     4,     4,     2,     1,     4,     4,     2,     1,
       5,     5,     5,     5,     4,     3,     3,     4,     4,     1,
       2,     1,     4,     3,     4,     4,     5,     3,     2,     1,
       4,     4,     5,     4,     4,     4,     4,     4,     4,     1,
       4,     4,     4,     4,     3,     3,     3,     4,     6,     1,
       2,     1,     2,     1,     3,     3,     8,     4,     5,     4,
       4,     4,     4,     1,     3,     5,     5,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     2,     1,     3,     7,     8,
       4,     1,     1,     6,     5,     2,     1,     5,     6,     4,
       5,     5,     5,     3,     3,     3,     3,     3,     1,     1,
       1,     3,     1,     2,     1,     1,     1,     3,     2,     1
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
        yyerror (param, YY_("syntax error: cannot back up")); \
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
                  Kind, Value, param); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, rcss::clang::Parser::Param &param)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (param);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, rcss::clang::Parser::Param &param)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, param);
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
                 int yyrule, rcss::clang::Parser::Param &param)
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
                       &yyvsp[(yyi + 1) - (yynrhs)], param);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, param); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, rcss::clang::Parser::Param &param)
{
  YY_USE (yyvaluep);
  YY_USE (param);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (rcss::clang::Parser::Param &param)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

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
      yychar = yylex (&yylval, param);
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
  case 4: /* command: "(" "hear" RCSS_CLANG_INT RCSS_CLANG_COACH_SIDE RCSS_CLANG_INT message ")"  */
#line 229 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        yyval = yyvsp[-2];
        BUILDER.setTime( yyvsp[-4].getInt() );
        BUILDER.setSide( yyvsp[-3].getInt() );
        BUILDER.setTimeRecv( yyvsp[-2].getInt() );
    }
#line 1498 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 13: /* rule_mess: "(" "rule" activation_list ")"  */
#line 249 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildRuleMsg();
        BUILDER.setVer( 8 );
    }
#line 1507 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 16: /* activation_element: "(" "on|off" id_list ")"  */
#line 261 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActivateRules( yyvsp[-2].getBool() );
    }
#line 1515 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 17: /* id_list: RCSS_CLANG_VAR  */
#line 268 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string var = yyvsp[0].getStr();
        if ( var.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildRuleID( var );
            BUILDER.buildRuleIDList();
        }
    }
#line 1532 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 18: /* id_list: "(" id_list2 ")"  */
#line 281 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildRuleIDList();
    }
#line 1540 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 19: /* id_list: "all"  */
#line 285 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildRuleIDListALL();
    }
#line 1548 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 20: /* id_list2: id_list2 RCSS_CLANG_VAR  */
#line 292 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string var = yyvsp[0].getStr();
        if ( var.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildRuleID( var );
        }
    }
#line 1564 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 21: /* id_list2: RCSS_CLANG_VAR  */
#line 304 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string var = yyvsp[0].getStr();
        if ( var.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildRuleID( var );
        }
    }
#line 1580 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 22: /* del_mess: "(" "delete" id_list ")"  */
#line 319 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildDelMsg();
        BUILDER.setVer( 8 );
    }
#line 1589 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 23: /* meta_mess: "(" "meta" meta_token_list ")"  */
#line 328 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildMetaMsg();
        BUILDER.setVer( 7 );
    }
#line 1598 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 26: /* meta_token: "(" "ver" num ")"  */
#line 340 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
       BUILDER.buildMetaTokenVer( yyvsp[-1].getDoub() );
    }
#line 1606 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 27: /* define_mess: "(" "define" define_token_list ")"  */
#line 348 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildDefineMsg();
        BUILDER.setVer( 8, 7 );
    }
#line 1615 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 30: /* define_token: "(" "definec" RCSS_CLANG_STR condition ")"  */
#line 360 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string str = rcss::stripQuotes( yyvsp[-2].getStr() );
        if ( str.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildDefineCond( str );
        }
    }
#line 1631 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 31: /* define_token: "(" "defined" RCSS_CLANG_STR directive ")"  */
#line 372 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string str = rcss::stripQuotes( yyvsp[-2].getStr() );
        if ( str.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildDefineDir( str );
        }
    }
#line 1647 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 32: /* define_token: "(" "definer" RCSS_CLANG_STR region ")"  */
#line 384 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string str = rcss::stripQuotes( yyvsp[-2].getStr() );
        if ( str.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildDefineReg( str );
        }
    }
#line 1663 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 33: /* define_token: "(" "definea" RCSS_CLANG_STR action ")"  */
#line 396 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string str = rcss::stripQuotes( yyvsp[-2].getStr() );
        if ( str.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildDefineAct( str );
        }
    }
#line 1679 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 34: /* define_token: "(" "definerule" define_rule ")"  */
#line 408 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.setVer( 8 );
    }
#line 1687 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 35: /* define_rule: RCSS_CLANG_VAR "model" rule  */
#line 415 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string var = yyvsp[-2].getStr();
        if ( var.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildDefineModelRule( var );
        }
    }
#line 1703 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 36: /* define_rule: RCSS_CLANG_VAR "direc" rule  */
#line 427 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string var = yyvsp[-2].getStr();
        if ( var.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildDefineDirectiveRule( var );
        }
    }
#line 1719 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 37: /* rule: "(" condition directive_list ")"  */
#line 442 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildSimpleRule();
    }
#line 1727 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 38: /* rule: "(" condition rule_list ")"  */
#line 446 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildNestedRule();
    }
#line 1735 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 39: /* rule: id_list  */
#line 450 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildIDRule();
    }
#line 1743 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 42: /* freeform_mess: "(" "freeform" RCSS_CLANG_STR ")"  */
#line 462 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string str = rcss::stripQuotes( yyvsp[-1].getStr() );
        if( str.length() > BUILDER.freeformMsgSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildFreeformMsg( str );
            BUILDER.setVer( 8, 7 );
        }
    }
#line 1760 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 43: /* unsupp_mess: "(" "unsupported_clang" ")"  */
#line 479 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildUnsuppMsg();
        BUILDER.setVer( 8, 7 );
    }
#line 1769 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 44: /* info_mess: "(" "info" token_list ")"  */
#line 487 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildInfoMsg();
        BUILDER.setVer( 7 );
    }
#line 1778 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 45: /* advice_mess: "(" "advice" token_list ")"  */
#line 495 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildAdviceMsg();
        BUILDER.setVer( 7 );
    }
#line 1787 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 46: /* token: "(" RCSS_CLANG_INT condition directive_list ")"  */
#line 503 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildTokenRule( yyvsp[-3].getInt() );
    }
#line 1795 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 47: /* token: "(" "clear" ")"  */
#line 507 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildTokenClear();
    }
#line 1803 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 50: /* action: "(" "pos" region ")"  */
#line 518 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActPos();
    }
#line 1811 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 51: /* action: "(" "home" region ")"  */
#line 522 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActHome();
    }
#line 1819 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 52: /* action: "(" "bto" region bmove_token_set ")"  */
#line 526 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActBallToReg();
    }
#line 1827 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 53: /* action: "(" "bto" unum_set ")"  */
#line 530 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActBallToPlayer();
    }
#line 1835 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 54: /* action: "(" "mark" unum_set ")"  */
#line 534 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActMark();
    }
#line 1843 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 55: /* action: "(" "markl" unum_set ")"  */
#line 538 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActMarkLinePlayer();
    }
#line 1851 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 56: /* action: "(" "markl" region ")"  */
#line 542 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActMarkLineReg();
    }
#line 1859 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 57: /* action: "(" "oline" region ")"  */
#line 546 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActOffsideLine();
    }
#line 1867 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 58: /* action: "(" "htype" RCSS_CLANG_INT ")"  */
#line 550 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActHetType( yyvsp[-1].getInt() );
    }
#line 1875 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 59: /* action: RCSS_CLANG_STR  */
#line 554 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string str = rcss::stripQuotes( yyvsp[0].getStr() );
        if ( str.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildActNamed( str );
        }
    }
#line 1891 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 60: /* action: "(" "pass" region ")"  */
#line 566 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActPassReg();
        BUILDER.setVer( 8 );
    }
#line 1900 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 61: /* action: "(" "pass" unum_set ")"  */
#line 571 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActPassUNum();
        BUILDER.setVer( 8 );
    }
#line 1909 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 62: /* action: "(" "dribble" region ")"  */
#line 576 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActDribble();
        BUILDER.setVer( 8 );
    }
#line 1918 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 63: /* action: "(" "clear" region ")"  */
#line 581 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActClear();
        BUILDER.setVer( 8 );
    }
#line 1927 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 64: /* action: "(" "shoot" ")"  */
#line 586 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActShoot();
        BUILDER.setVer( 8 );
    }
#line 1936 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 65: /* action: "(" "hold" ")"  */
#line 591 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActHold();
        BUILDER.setVer( 8 );
    }
#line 1945 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 66: /* action: "(" "intercept" ")"  */
#line 596 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActIntercept();
        BUILDER.setVer( 8 );
    }
#line 1954 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 67: /* action: "(" "tackle" unum_set ")"  */
#line 601 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildActTackle();
        BUILDER.setVer( 8 );
    }
#line 1963 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 68: /* directive: "(" RCSS_CLANG_DO_DONT RCSS_CLANG_TEAM unum_set action_list ")"  */
#line 609 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildDirComm( yyvsp[-4].getBool(), yyvsp[-3].getBool() );
    }
#line 1971 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 69: /* directive: RCSS_CLANG_STR  */
#line 613 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string str = rcss::stripQuotes( yyvsp[0].getStr() );
        if ( str.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildDirNamed( str );
        }
    }
#line 1987 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 70: /* action_list: action_list action  */
#line 627 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
                                 {}
#line 1993 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 71: /* action_list: action  */
#line 628 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
                     {}
#line 1999 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 74: /* condition: "(" "true" ")"  */
#line 637 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondTrue();
    }
#line 2007 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 75: /* condition: "(" "false" ")"  */
#line 641 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondFalse();
    }
#line 2015 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 76: /* condition: "(" "ppos" RCSS_CLANG_TEAM unum_set RCSS_CLANG_INT RCSS_CLANG_INT region ")"  */
#line 646 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondPlayerPos( yyvsp[-5].getBool(), yyvsp[-3].getInt(), yyvsp[-2].getInt() );
    }
#line 2023 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 77: /* condition: "(" "bpos" region ")"  */
#line 650 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondBallPos();
    }
#line 2031 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 78: /* condition: "(" "bowner" RCSS_CLANG_TEAM unum_set ")"  */
#line 654 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondBallOwner( yyvsp[-2].getBool() );
    }
#line 2039 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 79: /* condition: "(" "playm" RCSS_CLANG_PLAY_MODE ")"  */
#line 658 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
         BUILDER.buildCondPlayMode( yyvsp[-1].getPM() );
    }
#line 2047 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 80: /* condition: "(" "and" condition_list ")"  */
#line 662 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
         BUILDER.buildCondAnd();
    }
#line 2055 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 81: /* condition: "(" "or" condition_list ")"  */
#line 666 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondOr();
    }
#line 2063 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 82: /* condition: "(" "not" condition ")"  */
#line 670 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondNot();
    }
#line 2071 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 83: /* condition: RCSS_CLANG_STR  */
#line 674 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string str = yyvsp[0].getStr();
        if ( str.length() > BUILDER.strVarSize() )
            YYERROR;
        else
            BUILDER.buildCondNamed( str );
    }
#line 2083 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 84: /* condition: "(" cond_comp ")"  */
#line 682 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.setVer( 8 );
    }
#line 2091 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 85: /* condition: "(" "unum" RCSS_CLANG_VAR unum_set ")"  */
#line 686 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string var = yyvsp[-2].getStr();
        if ( var.length() > BUILDER.strVarSize() )
            YYERROR;
        else
        {
            BUILDER.buildCondUNum( rcss::clang::UNum( yyvsp[-2].getStr() ) );
            BUILDER.setVer( 8 );
        }
    }
#line 2106 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 86: /* condition: "(" "unum" RCSS_CLANG_STR unum_set ")"  */
#line 697 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string str = yyvsp[-2].getStr();
        if ( str.length() > BUILDER.strVarSize() )
            YYERROR;
        else
        {
            BUILDER.buildCondUNum( str );
            BUILDER.setVer( 8 );
        }
    }
#line 2121 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 91: /* time_comp: "time" comp RCSS_CLANG_INT  */
#line 717 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondTime( yyvsp[0].getInt(), yyvsp[-1].getCompOp() );
    }
#line 2129 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 92: /* time_comp: RCSS_CLANG_INT comp "time"  */
#line 721 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondTime( yyvsp[-2].getInt(), yyvsp[-1].getCompOp().swap() );
    }
#line 2137 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 93: /* opp_goal_comp: "opp_goals" comp RCSS_CLANG_INT  */
#line 728 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondOppGoal( yyvsp[0].getInt(), yyvsp[-1].getCompOp() );
    }
#line 2145 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 94: /* opp_goal_comp: RCSS_CLANG_INT comp "opp_goals"  */
#line 732 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondOppGoal( yyvsp[-2].getInt(), yyvsp[-1].getCompOp().swap() );
    }
#line 2153 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 95: /* our_goal_comp: "own_goals" comp RCSS_CLANG_INT  */
#line 739 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondOurGoal( yyvsp[0].getInt(), yyvsp[-1].getCompOp() );
    }
#line 2161 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 96: /* our_goal_comp: RCSS_CLANG_INT comp "own_goals"  */
#line 743 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondOurGoal( yyvsp[-2].getInt(), yyvsp[-1].getCompOp().swap() );
    }
#line 2169 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 97: /* goal_diff_comp: "goal_diff" comp RCSS_CLANG_INT  */
#line 750 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondGoalDiff( yyvsp[0].getInt(), yyvsp[-1].getCompOp() );
    }
#line 2177 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 98: /* goal_diff_comp: RCSS_CLANG_INT comp "goal_diff"  */
#line 754 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCondGoalDiff( yyvsp[-2].getInt(), yyvsp[-1].getCompOp().swap() );
    }
#line 2185 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 105: /* condition_list: condition_list condition  */
#line 765 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
       BUILDER.buildAddToCondList();
    }
#line 2193 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 106: /* condition_list: condition  */
#line 769 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildCreateCondList();
    }
#line 2201 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 107: /* region: "(" "null" ")"  */
#line 776 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildRegNull();
    }
#line 2209 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 108: /* region: "(" "quad" point point point point ")"  */
#line 780 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildRegQuad();
        BUILDER.setVer( 7 );
    }
#line 2218 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 109: /* region: "(" "arc" point num num num num ")"  */
#line 785 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildRegArc( yyvsp[-4].getDoub(),
                             yyvsp[-3].getDoub(),
                             yyvsp[-2].getDoub(),
                             yyvsp[-1].getDoub() );
    }
#line 2229 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 110: /* region: "(" "reg" region_list ")"  */
#line 792 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildRegUnion();
    }
#line 2237 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 111: /* region: RCSS_CLANG_STR  */
#line 796 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string str = rcss::stripQuotes( yyvsp[0].getStr() );
        if ( str.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildRegNamed( str );
        }
    }
#line 2253 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 112: /* region: point  */
#line 808 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildRegPoint();
    }
#line 2261 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 113: /* region: "(" "tri" point point point ")"  */
#line 812 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildRegTri();
        BUILDER.setVer( 8 );
    }
#line 2270 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 114: /* region: "(" "rec" point point ")"  */
#line 817 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildRegRec();
        BUILDER.setVer( 8 );
    }
#line 2279 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 117: /* point: "(" "pt" num num ")"  */
#line 830 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildPointSimple( yyvsp[-2].getDoub(), yyvsp[-1].getDoub() );
    }
#line 2287 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 118: /* point: "(" "pt" num num point ")"  */
#line 834 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildPointRel( yyvsp[-3].getDoub(), yyvsp[-2].getDoub() );
    }
#line 2295 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 119: /* point: "(" "pt" "ball" ")"  */
#line 838 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildPointBall();
    }
#line 2303 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 120: /* point: "(" "pt" RCSS_CLANG_TEAM RCSS_CLANG_INT ")"  */
#line 842 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildPointPlayer( yyvsp[-2].getBool(),
                                  rcss::clang::UNum( ( rcss::clang::UNum::unum_t )yyvsp[-1].getInt() ) );
    }
#line 2312 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 121: /* point: "(" "pt" RCSS_CLANG_TEAM RCSS_CLANG_VAR ")"  */
#line 847 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string var = yyvsp[-1].getStr();
        if ( var.length() > BUILDER.strVarSize() )
            YYERROR;
        else
        {
            BUILDER.buildPointPlayer( yyvsp[-2].getBool(),
                                      rcss::clang::UNum( yyvsp[-1].getStr() ) );
            BUILDER.setVer( 8 );
        }
    }
#line 2328 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 122: /* point: "(" "pt" RCSS_CLANG_TEAM RCSS_CLANG_STR ")"  */
#line 859 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string str = yyvsp[-1].getStr();
        if ( str.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildPointPlayer( yyvsp[-2].getBool(), str );
            BUILDER.setVer( 8 );
        }
    }
#line 2345 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 123: /* point: "(" point_list ")"  */
#line 872 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {

    }
#line 2353 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 124: /* point_list: point_list "+" point_list  */
#line 879 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildPointArith( yyvsp[-1].getArithOp() );
        BUILDER.setVer( 8 );
    }
#line 2362 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 125: /* point_list: point_list "-" point_list  */
#line 884 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildPointArith( yyvsp[-1].getArithOp() );
        BUILDER.setVer( 8 );
    }
#line 2371 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 126: /* point_list: point_list "*" point_list  */
#line 889 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildPointArith( yyvsp[-1].getArithOp() );
        BUILDER.setVer( 8 );
    }
#line 2380 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 127: /* point_list: point_list "/" point_list  */
#line 894 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildPointArith( yyvsp[-1].getArithOp() );
        BUILDER.setVer( 8 );
    }
#line 2389 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 128: /* point_list: point  */
#line 899 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {

    }
#line 2397 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 129: /* num: RCSS_CLANG_INT  */
#line 906 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        yyval = (double)yyvsp[0].getInt();
    }
#line 2405 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 130: /* num: RCSS_CLANG_REAL  */
#line 910 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        yyval = yyvsp[0];
    }
#line 2413 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 131: /* unum_set: "{" unum_list "}"  */
#line 917 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildUNumSet();
    }
#line 2421 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 134: /* unum: RCSS_CLANG_INT  */
#line 928 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildUNum( rcss::clang::UNum( ( rcss::clang::UNum::unum_t )yyvsp[0].getInt() ) );
    }
#line 2429 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 135: /* unum: RCSS_CLANG_VAR  */
#line 932 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string var = yyvsp[0].getStr();
        if ( var.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildUNum( rcss::clang::UNum( var ) );
            BUILDER.setVer( 8 );
        }
    }
#line 2446 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 136: /* unum: RCSS_CLANG_STR  */
#line 945 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        std::string str = rcss::stripQuotes( yyvsp[0].getStr() );
        if ( str.length() > BUILDER.strVarSize() )
        {
            YYERROR;
        }
        else
        {
            BUILDER.buildUNum( rcss::clang::UNum( str ) );
            BUILDER.setVer( 8 );
        }
    }
#line 2463 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 138: /* bmove_token_list: bmove_token_list RCSS_CLANG_BMOVE_TOKEN  */
#line 964 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildBallMoveToken( yyvsp[0].getBMT() );
    }
#line 2471 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;

  case 139: /* bmove_token_list: RCSS_CLANG_BMOVE_TOKEN  */
#line 968 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"
    {
        BUILDER.buildBallMoveToken( yyvsp[0].getBMT() );
    }
#line 2479 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"
    break;


#line 2483 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.cpp"

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
      yyerror (param, YY_("syntax error"));
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
                      yytoken, &yylval, param);
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, param);
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
  yyerror (param, YY_("memory exhausted"));
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
                  yytoken, &yylval, param);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, param);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 973 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_parser.ypp"



extern "C" void parse()
{
  // dumby function use to make the library easy to find with autoconf
}

void yyerror (rcss::clang::Parser::Param & /*param*/, const char*)
{
/*   std::cerr << s << std::endl;*/
  //do nothing
}

int yyerror (rcss::clang::Parser::Param &param, char* s)
{

	yyerror ( param, (const char*)s );
	return 0;
}
