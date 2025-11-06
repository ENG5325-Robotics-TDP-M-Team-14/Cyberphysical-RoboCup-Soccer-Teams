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
#line 26 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"

#include "pcombuilder.h"
#include "pcomparser.h"


#define    yyparse    RCSS_PCOM_parse

void yyerror( rcss::pcom::Parser::Param& param, const char* s );
int yyerror( rcss::pcom::Parser::Param& param, char* s );

namespace
{
  inline rcss::pcom::Builder& getBuilder( rcss::pcom::Parser::Param& param )
  {
    return param.getBuilder();
  }

#define YYSTYPE rcss::pcom::Parser::Lexer::Holder

  inline int yylex( YYSTYPE* holder, rcss::pcom::Parser::Param& param )
  {
    int rval = param.getLexer().lex( *holder );
//    cout << rval << endl;
    return rval;
  }

}

#define BUILDER getBuilder( param )


#line 103 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"

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

#include "player_command_parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_RCSS_PCOM_INT = 3,              /* RCSS_PCOM_INT  */
  YYSYMBOL_RCSS_PCOM_REAL = 4,             /* RCSS_PCOM_REAL  */
  YYSYMBOL_RCSS_PCOM_STR = 5,              /* RCSS_PCOM_STR  */
  YYSYMBOL_RCSS_PCOM_LP = 6,               /* "("  */
  YYSYMBOL_RCSS_PCOM_RP = 7,               /* ")"  */
  YYSYMBOL_RCSS_PCOM_DASH = 8,             /* "dash"  */
  YYSYMBOL_RCSS_PCOM_TURN = 9,             /* "turn"  */
  YYSYMBOL_RCSS_PCOM_TURN_NECK = 10,       /* "turn_neck"  */
  YYSYMBOL_RCSS_PCOM_CHANGE_FOCUS = 11,    /* "change_focus"  */
  YYSYMBOL_RCSS_PCOM_KICK = 12,            /* "kick"  */
  YYSYMBOL_RCSS_PCOM_LONG_KICK = 13,       /* "long_kick"  */
  YYSYMBOL_RCSS_PCOM_CATCH = 14,           /* "catch"  */
  YYSYMBOL_RCSS_PCOM_SAY = 15,             /* "say"  */
  YYSYMBOL_RCSS_PCOM_UNQ_SAY = 16,         /* "unquoted say"  */
  YYSYMBOL_RCSS_PCOM_SENSE_BODY = 17,      /* "sense_body"  */
  YYSYMBOL_RCSS_PCOM_SCORE = 18,           /* "score"  */
  YYSYMBOL_RCSS_PCOM_MOVE = 19,            /* "move"  */
  YYSYMBOL_RCSS_PCOM_CHANGE_VIEW = 20,     /* "change_view"  */
  YYSYMBOL_RCSS_PCOM_COMPRESSION = 21,     /* "compression"  */
  YYSYMBOL_RCSS_PCOM_BYE = 22,             /* "bye"  */
  YYSYMBOL_RCSS_PCOM_DONE = 23,            /* "done"  */
  YYSYMBOL_RCSS_PCOM_POINTTO = 24,         /* "pointto"  */
  YYSYMBOL_RCSS_PCOM_ATTENTIONTO = 25,     /* "attentionto"  */
  YYSYMBOL_RCSS_PCOM_TACKLE = 26,          /* "tackle"  */
  YYSYMBOL_RCSS_PCOM_CLANG = 27,           /* "clang"  */
  YYSYMBOL_RCSS_PCOM_EAR = 28,             /* "ear"  */
  YYSYMBOL_RCSS_PCOM_SYNCH_SEE = 29,       /* "synch_see"  */
  YYSYMBOL_RCSS_PCOM_GAUSSIAN_SEE = 30,    /* "gaussian_see"  */
  YYSYMBOL_RCSS_PCOM_VIEW_WIDTH_NARROW = 31, /* "narrow"  */
  YYSYMBOL_RCSS_PCOM_VIEW_WIDTH_NORMAL = 32, /* "normal"  */
  YYSYMBOL_RCSS_PCOM_VIEW_WIDTH_WIDE = 33, /* "wide"  */
  YYSYMBOL_RCSS_PCOM_VIEW_QUALITY_LOW = 34, /* "low"  */
  YYSYMBOL_RCSS_PCOM_VIEW_QUALITY_HIGH = 35, /* "high"  */
  YYSYMBOL_RCSS_PCOM_ON = 36,              /* "on"  */
  YYSYMBOL_RCSS_PCOM_OFF = 37,             /* "off"  */
  YYSYMBOL_RCSS_PCOM_TRUE = 38,            /* "true"  */
  YYSYMBOL_RCSS_PCOM_FALSE = 39,           /* "false"  */
  YYSYMBOL_RCSS_PCOM_OUR = 40,             /* "our"  */
  YYSYMBOL_RCSS_PCOM_OPP = 41,             /* "opp"  */
  YYSYMBOL_RCSS_PCOM_LEFT = 42,            /* RCSS_PCOM_LEFT  */
  YYSYMBOL_43_l_ = 43,                     /* 'l'  */
  YYSYMBOL_RCSS_PCOM_RIGHT = 44,           /* RCSS_PCOM_RIGHT  */
  YYSYMBOL_45_r_ = 45,                     /* 'r'  */
  YYSYMBOL_RCSS_PCOM_EAR_PARTIAL = 46,     /* "partial"  */
  YYSYMBOL_RCSS_PCOM_EAR_COMPLETE = 47,    /* "complete"  */
  YYSYMBOL_RCSS_PCOM_CLANG_VERSION = 48,   /* "ver"  */
  YYSYMBOL_RCSS_PCOM_ERROR = 49,           /* RCSS_PCOM_ERROR  */
  YYSYMBOL_YYACCEPT = 50,                  /* $accept  */
  YYSYMBOL_command_list = 51,              /* command_list  */
  YYSYMBOL_command = 52,                   /* command  */
  YYSYMBOL_dash_com = 53,                  /* dash_com  */
  YYSYMBOL_dash_params = 54,               /* dash_params  */
  YYSYMBOL_dash_left = 55,                 /* dash_left  */
  YYSYMBOL_dash_right = 56,                /* dash_right  */
  YYSYMBOL_turn_com = 57,                  /* turn_com  */
  YYSYMBOL_turn_neck_com = 58,             /* turn_neck_com  */
  YYSYMBOL_change_focus_com = 59,          /* change_focus_com  */
  YYSYMBOL_kick_com = 60,                  /* kick_com  */
  YYSYMBOL_long_kick_com = 61,             /* long_kick_com  */
  YYSYMBOL_catch_com = 62,                 /* catch_com  */
  YYSYMBOL_say_com = 63,                   /* say_com  */
  YYSYMBOL_sense_body_com = 64,            /* sense_body_com  */
  YYSYMBOL_score_com = 65,                 /* score_com  */
  YYSYMBOL_move_com = 66,                  /* move_com  */
  YYSYMBOL_change_view_com = 67,           /* change_view_com  */
  YYSYMBOL_view_width = 68,                /* view_width  */
  YYSYMBOL_view_quality = 69,              /* view_quality  */
  YYSYMBOL_compression_com = 70,           /* compression_com  */
  YYSYMBOL_bye_com = 71,                   /* bye_com  */
  YYSYMBOL_done_com = 72,                  /* done_com  */
  YYSYMBOL_pointto_com = 73,               /* pointto_com  */
  YYSYMBOL_attentionto_com = 74,           /* attentionto_com  */
  YYSYMBOL_tackle_com = 75,                /* tackle_com  */
  YYSYMBOL_clang_com = 76,                 /* clang_com  */
  YYSYMBOL_ear_com = 77,                   /* ear_com  */
  YYSYMBOL_synch_see_com = 78,             /* synch_see_com  */
  YYSYMBOL_gaussian_see_com = 79,          /* gaussian_see_com  */
  YYSYMBOL_on_off = 80,                    /* on_off  */
  YYSYMBOL_boolean_value = 81,             /* boolean_value  */
  YYSYMBOL_team_side = 82,                 /* team_side  */
  YYSYMBOL_partial_complete = 83,          /* partial_complete  */
  YYSYMBOL_floating_point_number = 84      /* floating_point_number  */
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
#define YYFINAL  49
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   147

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  85
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  165

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   302


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
       2,     2,     2,     2,     2,     2,     2,     2,    43,     2,
       2,     2,     2,     2,    45,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    44,    46,
      47,    48,    49
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   130,   130,   131,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   158,   162,   166,   169,
     170,   171,   172,   175,   181,   187,   193,   199,   205,   211,
     217,   223,   227,   233,   239,   245,   251,   255,   261,   265,
     269,   275,   279,   285,   291,   297,   303,   307,   313,   317,
     321,   327,   332,   338,   344,   348,   352,   356,   360,   364,
     370,   376,   382,   386,   392,   396,   400,   404,   410,   414,
     418,   422,   428,   432,   438,   442
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
  "\"end of file\"", "error", "\"invalid token\"", "RCSS_PCOM_INT",
  "RCSS_PCOM_REAL", "RCSS_PCOM_STR", "\"(\"", "\")\"", "\"dash\"",
  "\"turn\"", "\"turn_neck\"", "\"change_focus\"", "\"kick\"",
  "\"long_kick\"", "\"catch\"", "\"say\"", "\"unquoted say\"",
  "\"sense_body\"", "\"score\"", "\"move\"", "\"change_view\"",
  "\"compression\"", "\"bye\"", "\"done\"", "\"pointto\"",
  "\"attentionto\"", "\"tackle\"", "\"clang\"", "\"ear\"", "\"synch_see\"",
  "\"gaussian_see\"", "\"narrow\"", "\"normal\"", "\"wide\"", "\"low\"",
  "\"high\"", "\"on\"", "\"off\"", "\"true\"", "\"false\"", "\"our\"",
  "\"opp\"", "RCSS_PCOM_LEFT", "'l'", "RCSS_PCOM_RIGHT", "'r'",
  "\"partial\"", "\"complete\"", "\"ver\"", "RCSS_PCOM_ERROR", "$accept",
  "command_list", "command", "dash_com", "dash_params", "dash_left",
  "dash_right", "turn_com", "turn_neck_com", "change_focus_com",
  "kick_com", "long_kick_com", "catch_com", "say_com", "sense_body_com",
  "score_com", "move_com", "change_view_com", "view_width", "view_quality",
  "compression_com", "bye_com", "done_com", "pointto_com",
  "attentionto_com", "tackle_com", "clang_com", "ear_com", "synch_see_com",
  "gaussian_see_com", "on_off", "boolean_value", "team_side",
  "partial_complete", "floating_point_number", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-121)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      12,    55,  -121,    11,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,    85,    27,    27,
      27,    27,    27,    27,     2,    17,    35,    27,    61,    43,
      64,    90,    22,    16,    27,    94,    95,    96,    97,  -121,
    -121,  -121,  -121,   -22,    98,   100,   101,    83,   102,   103,
      27,    27,    27,   104,   105,  -121,  -121,    27,  -121,  -121,
    -121,     6,   106,  -121,  -121,   107,    27,    99,   108,  -121,
    -121,  -121,  -121,   113,    -1,    60,    59,  -121,  -121,    27,
      27,  -121,    73,  -121,    76,  -121,  -121,   112,  -121,  -121,
     114,   115,   116,  -121,  -121,   117,  -121,  -121,  -121,   118,
    -121,  -121,   119,   120,  -121,   121,  -121,  -121,  -121,  -121,
    -121,   122,   127,  -121,  -121,     3,    27,    27,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,   128,     5,
     125,  -121,  -121,     8,   126,   129,   130,   131,   132,   133,
    -121,   134,   135,   136,  -121,  -121,   137,  -121,   138,  -121,
     139,  -121,  -121,  -121,  -121
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    41,     0,     2,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       3,    84,    85,     0,     0,    29,    30,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    44,     0,    48,    49,
      50,     0,     0,    54,    55,     0,     0,     0,     0,    78,
      79,    80,    81,     0,     0,     0,     0,    70,    71,     0,
       0,    28,     0,    31,     0,    32,    26,     0,    35,    36,
       0,     0,     0,    40,    42,     0,    47,    51,    52,     0,
      53,    57,     0,     0,    60,     0,    61,    74,    75,    76,
      77,     0,     0,    72,    73,     0,     0,     0,    27,    37,
      38,    39,    45,    46,    56,    59,    58,    62,     0,     0,
       0,    82,    83,     0,     0,     0,     0,     0,     0,     0,
      69,     0,     0,     0,    33,    34,     0,    67,     0,    66,
       0,    68,    63,    65,    64
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -121,  -121,   144,  -121,  -121,    78,    65,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,    10,  -120,   -28
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,     4,     5,    54,    55,    56,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    71,   109,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     125,   121,    83,   144,    57
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      58,    59,    60,    61,    62,    63,   116,    64,   139,    67,
     140,    49,   148,   106,    76,   151,    84,     1,     1,   149,
      89,    77,    90,   152,    65,    51,    52,     2,     2,    97,
      51,    52,   100,   101,   102,   117,   118,   119,   120,   105,
     107,   108,    66,    79,    80,    81,    72,    82,   112,   141,
     142,   141,   142,    78,   141,   142,    79,    80,    81,    75,
      82,   126,   127,    27,    28,    29,    30,    31,    32,    33,
      34,    73,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    51,    52,    51,    52,
      96,    53,    68,    69,    70,   123,   124,    74,   145,   146,
      85,    86,   113,    87,    88,    91,    92,    94,   122,    98,
      99,   103,   104,   110,   111,   114,   115,    90,    89,   128,
      93,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   147,   150,   153,    95,   143,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    50
};

static const yytype_uint8 yycheck[] =
{
      28,    29,    30,    31,    32,    33,     7,     5,     5,    37,
       7,     0,     7,     7,    42,     7,    44,     6,     6,   139,
      42,     5,    44,   143,     7,     3,     4,    16,    16,    57,
       3,     4,    60,    61,    62,    36,    37,    38,    39,    67,
      34,    35,     7,    40,    41,    42,     3,    44,    76,    46,
      47,    46,    47,    37,    46,    47,    40,    41,    42,    37,
      44,    89,    90,     8,     9,    10,    11,    12,    13,    14,
      15,     7,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,     3,     4,     3,     4,
       7,     6,    31,    32,    33,    36,    37,     7,   126,   127,
       6,     6,     3,     7,     7,     7,     6,     6,    48,     7,
       7,     7,     7,     7,     7,     7,     3,    44,    42,     7,
      55,     7,     7,     7,     7,     7,     7,     7,     7,     7,
       3,     3,     7,     7,    56,   125,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,     7,     3
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,    16,    51,    52,    53,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,     8,     9,    10,
      11,    12,    13,    14,    15,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,     0,
      52,     3,     4,     6,    54,    55,    56,    84,    84,    84,
      84,    84,    84,    84,     5,     7,     7,    84,    31,    32,
      33,    68,     3,     7,     7,    37,    84,     5,    37,    40,
      41,    42,    44,    82,    84,     6,     6,     7,     7,    42,
      44,     7,     6,    56,     6,    55,     7,    84,     7,     7,
      84,    84,    84,     7,     7,    84,     7,    34,    35,    69,
       7,     7,    84,     3,     7,     3,     7,    36,    37,    38,
      39,    81,    48,    36,    37,    80,    84,    84,     7,     7,
       7,     7,     7,     7,     7,     7,     7,     7,     3,     5,
       7,    46,    47,    82,    83,    84,    84,     3,     7,    83,
       7,     7,    83,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    51,    51,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    53,    53,    53,    54,
      54,    54,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    63,    64,    65,    66,    67,    67,    68,    68,
      68,    69,    69,    70,    71,    72,    73,    73,    74,    74,
      74,    75,    75,    76,    77,    77,    77,    77,    77,    77,
      78,    79,    80,    80,    81,    81,    81,    81,    82,    82,
      82,    82,    83,    83,    84,    84
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     5,     4,     1,
       1,     2,     2,     5,     5,     4,     4,     5,     5,     5,
       4,     1,     4,     3,     3,     5,     5,     4,     1,     1,
       1,     1,     1,     4,     3,     3,     5,     4,     5,     5,
       4,     4,     5,     8,     8,     8,     7,     7,     7,     6,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
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
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, rcss::pcom::Parser::Param& param)
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, rcss::pcom::Parser::Param& param)
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
                 int yyrule, rcss::pcom::Parser::Param& param)
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, rcss::pcom::Parser::Param& param)
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
yyparse (rcss::pcom::Parser::Param& param)
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
  case 26: /* dash_com: "(" "dash" floating_point_number ")"  */
#line 159 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
           {
             BUILDER.dash( (yyvsp[-1]. m_double ) );
           }
#line 1298 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 27: /* dash_com: "(" "dash" floating_point_number floating_point_number ")"  */
#line 163 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
           {
	           BUILDER.dash( (yyvsp[-2]. m_double ), (yyvsp[-1]. m_double ) );
           }
#line 1306 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 33: /* dash_left: "(" RCSS_PCOM_LEFT floating_point_number floating_point_number ")"  */
#line 176 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
            {
              BUILDER.dashLeftLeg( (yyvsp[-2]. m_double ), (yyvsp[-1]. m_double ) );
            }
#line 1314 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 34: /* dash_right: "(" RCSS_PCOM_RIGHT floating_point_number floating_point_number ")"  */
#line 182 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
             {
               BUILDER.dashRightLeg( (yyvsp[-2]. m_double ), (yyvsp[-1]. m_double ) );
             }
#line 1322 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 35: /* turn_com: "(" "turn" floating_point_number ")"  */
#line 188 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
           {
             BUILDER.turn( (yyvsp[-1]. m_double ) );
           }
#line 1330 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 36: /* turn_neck_com: "(" "turn_neck" floating_point_number ")"  */
#line 194 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                {
                  BUILDER.turn_neck( (yyvsp[-1]. m_double ) );
                }
#line 1338 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 37: /* change_focus_com: "(" "change_focus" floating_point_number floating_point_number ")"  */
#line 200 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                {
                  BUILDER.change_focus( (yyvsp[-2]. m_double ), (yyvsp[-1]. m_double ) );
                }
#line 1346 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 38: /* kick_com: "(" "kick" floating_point_number floating_point_number ")"  */
#line 206 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
           {
             BUILDER.kick( (yyvsp[-2]. m_double ), (yyvsp[-1]. m_double ) );
           }
#line 1354 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 39: /* long_kick_com: "(" "long_kick" floating_point_number floating_point_number ")"  */
#line 212 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
           {
             BUILDER.long_kick( (yyvsp[-2]. m_double ), (yyvsp[-1]. m_double ) );
           }
#line 1362 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 40: /* catch_com: "(" "catch" floating_point_number ")"  */
#line 218 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
            {
              BUILDER.goalieCatch( (yyvsp[-1]. m_double ) );
            }
#line 1370 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 41: /* say_com: "unquoted say"  */
#line 224 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
          {
            BUILDER.say( (yyvsp[0]. m_str ) );
          }
#line 1378 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 42: /* say_com: "(" "say" RCSS_PCOM_STR ")"  */
#line 228 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
          {
            BUILDER.say( rcss::stripQuotes( (yyvsp[-1]. m_str ) ) );
          }
#line 1386 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 43: /* sense_body_com: "(" "sense_body" ")"  */
#line 234 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                 {
                   BUILDER.sense_body();
                 }
#line 1394 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 44: /* score_com: "(" "score" ")"  */
#line 240 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
            {
              BUILDER.score();
            }
#line 1402 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 45: /* move_com: "(" "move" floating_point_number floating_point_number ")"  */
#line 246 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
           {
             BUILDER.move( (yyvsp[-2]. m_double ), (yyvsp[-1]. m_double ) );
           }
#line 1410 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 46: /* change_view_com: "(" "change_view" view_width view_quality ")"  */
#line 252 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                 {
                   BUILDER.change_view( (yyvsp[-2]. m_view_w ), (yyvsp[-1]. m_view_q ) );
                 }
#line 1418 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 47: /* change_view_com: "(" "change_view" view_width ")"  */
#line 256 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                 {
                   BUILDER.change_view( (yyvsp[-1]. m_view_w ) );
                 }
#line 1426 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 48: /* view_width: "narrow"  */
#line 262 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
             {
               (yyval. m_view_w ) = rcss::pcom::NARROW;
             }
#line 1434 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 49: /* view_width: "normal"  */
#line 266 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
             {
               (yyval. m_view_w ) = rcss::pcom::NORMAL;
             }
#line 1442 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 50: /* view_width: "wide"  */
#line 270 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
             {
               (yyval. m_view_w ) = rcss::pcom::WIDE;
             }
#line 1450 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 51: /* view_quality: "low"  */
#line 276 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
               {
                 (yyval. m_view_q ) = rcss::pcom::LOW;
               }
#line 1458 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 52: /* view_quality: "high"  */
#line 280 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
               {
                 (yyval. m_view_q ) = rcss::pcom::HIGH;
               }
#line 1466 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 53: /* compression_com: "(" "compression" RCSS_PCOM_INT ")"  */
#line 286 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                  {
                    BUILDER.compression( (yyvsp[-1]. m_int ) );
                  }
#line 1474 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 54: /* bye_com: "(" "bye" ")"  */
#line 292 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
          {
            BUILDER.bye();
          }
#line 1482 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 55: /* done_com: "(" "done" ")"  */
#line 298 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
           {
             BUILDER.done();
           }
#line 1490 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 56: /* pointto_com: "(" "pointto" floating_point_number floating_point_number ")"  */
#line 304 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
              {
                BUILDER.pointto( true, (yyvsp[-2]. m_double ), (yyvsp[-1]. m_double ) );
              }
#line 1498 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 57: /* pointto_com: "(" "pointto" "off" ")"  */
#line 308 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
              {
                BUILDER.pointto( false, 0.0, 0.0 );
              }
#line 1506 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 58: /* attentionto_com: "(" "attentionto" team_side RCSS_PCOM_INT ")"  */
#line 314 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                  {
                    BUILDER.attentionto( true, (yyvsp[-2]. m_team ), "", (yyvsp[-1]. m_int ) );
                  }
#line 1514 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 59: /* attentionto_com: "(" "attentionto" RCSS_PCOM_STR RCSS_PCOM_INT ")"  */
#line 318 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                  {
                    BUILDER.attentionto( true, rcss::pcom::UNKNOWN_TEAM, (yyvsp[-2]. m_str ), (yyvsp[-1]. m_int ) );
                  }
#line 1522 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 60: /* attentionto_com: "(" "attentionto" "off" ")"  */
#line 322 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                  {
                    BUILDER.attentionto( false, rcss::pcom::UNKNOWN_TEAM, "", 0 );
                  }
#line 1530 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 61: /* tackle_com: "(" "tackle" floating_point_number ")"  */
#line 328 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
             {
               BUILDER.tackle( (yyvsp[-1]. m_double ) );
             }
#line 1538 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 62: /* tackle_com: "(" "tackle" floating_point_number boolean_value ")"  */
#line 333 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
             {
               BUILDER.tackle( (yyvsp[-2]. m_double ), (yyvsp[-1]. m_bool ) );
             }
#line 1546 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 63: /* clang_com: "(" "clang" "(" "ver" RCSS_PCOM_INT RCSS_PCOM_INT ")" ")"  */
#line 339 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
           {
             BUILDER.clang( (yyvsp[-3]. m_int ), (yyvsp[-2]. m_int ) );
           }
#line 1554 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 64: /* ear_com: "(" "ear" "(" on_off team_side partial_complete ")" ")"  */
#line 345 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
          {
            BUILDER.ear( (yyvsp[-4]. m_bool ), (yyvsp[-3]. m_team ), "", (yyvsp[-2]. m_ear ) );
          }
#line 1562 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 65: /* ear_com: "(" "ear" "(" on_off RCSS_PCOM_STR partial_complete ")" ")"  */
#line 349 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
          {
            BUILDER.ear( (yyvsp[-4]. m_bool ), rcss::pcom::UNKNOWN_TEAM, (yyvsp[-3]. m_str ), (yyvsp[-2]. m_ear ) );
          }
#line 1570 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 66: /* ear_com: "(" "ear" "(" on_off team_side ")" ")"  */
#line 353 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
          {
            BUILDER.ear( (yyvsp[-3]. m_bool ), (yyvsp[-2]. m_team ), "", rcss::pcom::UNKNOWN_EAR_MODE );
          }
#line 1578 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 67: /* ear_com: "(" "ear" "(" on_off RCSS_PCOM_STR ")" ")"  */
#line 357 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
          {
            BUILDER.ear( (yyvsp[-3]. m_bool ), rcss::pcom::UNKNOWN_TEAM, (yyvsp[-2]. m_str ), rcss::pcom::UNKNOWN_EAR_MODE );
          }
#line 1586 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 68: /* ear_com: "(" "ear" "(" on_off partial_complete ")" ")"  */
#line 361 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
          {
            BUILDER.ear( (yyvsp[-3]. m_bool ), rcss::pcom::UNKNOWN_TEAM, "", (yyvsp[-2]. m_ear ) );
          }
#line 1594 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 69: /* ear_com: "(" "ear" "(" on_off ")" ")"  */
#line 365 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
          {
            BUILDER.ear( (yyvsp[-2]. m_bool ), rcss::pcom::UNKNOWN_TEAM, "", rcss::pcom::UNKNOWN_EAR_MODE );
          }
#line 1602 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 70: /* synch_see_com: "(" "synch_see" ")"  */
#line 371 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                {
                   BUILDER.synch_see();
                }
#line 1610 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 71: /* gaussian_see_com: "(" "gaussian_see" ")"  */
#line 377 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                {
                   BUILDER.gaussian_see();
                }
#line 1618 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 72: /* on_off: "on"  */
#line 383 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
         {
           (yyval. m_bool ) = true;
         }
#line 1626 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 73: /* on_off: "off"  */
#line 387 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
         {
           (yyval. m_bool ) = false;
         }
#line 1634 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 74: /* boolean_value: "on"  */
#line 393 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                {
                  (yyval. m_bool ) = true;
                }
#line 1642 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 75: /* boolean_value: "off"  */
#line 397 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                {
                  (yyval. m_bool ) = false;
                }
#line 1650 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 76: /* boolean_value: "true"  */
#line 401 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                {
                  (yyval. m_bool ) = true;
                }
#line 1658 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 77: /* boolean_value: "false"  */
#line 405 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                {
                  (yyval. m_bool ) = false;
                }
#line 1666 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 78: /* team_side: "our"  */
#line 411 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
            {
              (yyval. m_team ) = rcss::pcom::OUR;
            }
#line 1674 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 79: /* team_side: "opp"  */
#line 415 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
            {
              (yyval. m_team ) = rcss::pcom::OPP;
            }
#line 1682 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 80: /* team_side: RCSS_PCOM_LEFT  */
#line 419 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
            {
              (yyval. m_team ) = rcss::pcom::LEFT_SIDE;
            }
#line 1690 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 81: /* team_side: RCSS_PCOM_RIGHT  */
#line 423 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
            {
              (yyval. m_team ) = rcss::pcom::RIGHT_SIDE;
            }
#line 1698 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 82: /* partial_complete: "partial"  */
#line 429 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                   {
                     (yyval. m_ear ) = rcss::pcom::PARTIAL;
                   }
#line 1706 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 83: /* partial_complete: "complete"  */
#line 433 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                   {
                     (yyval. m_ear ) = rcss::pcom::COMPLETE;
                   }
#line 1714 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 84: /* floating_point_number: RCSS_PCOM_INT  */
#line 439 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                        {
                          (yyval. m_double ) = static_cast< double >( (yyvsp[0]. m_int ) );
                        }
#line 1722 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;

  case 85: /* floating_point_number: RCSS_PCOM_REAL  */
#line 443 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"
                        {
                          (yyval. m_double ) = (yyvsp[0]. m_double );
                        }
#line 1730 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"
    break;


#line 1734 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/src/player_command_parser.cpp"

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

#line 448 "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/src/player_command_parser.ypp"



void yyerror (rcss::pcom::Parser::Param& /*param*/, const char* s)
{
  std::cerr << s << std::endl;
  //do nothing
}

int yyerror (rcss::pcom::Parser::Param& param, char* s)
{
  yyerror ( param, (const char*)s );
  return 0;
}
