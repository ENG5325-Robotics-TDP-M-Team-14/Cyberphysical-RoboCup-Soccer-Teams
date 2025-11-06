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

#ifndef YY_YY_USERS_NATHAN_SIDIB_CODE_VS_CODE_RCSOCCERSIM_RCSSSERVER_19_0_0_BUILD_RCSS_CLANG_COACH_LANG_PARSER_HPP_INCLUDED
# define YY_YY_USERS_NATHAN_SIDIB_CODE_VS_CODE_RCSOCCERSIM_RCSSSERVER_19_0_0_BUILD_RCSS_CLANG_COACH_LANG_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    RCSS_CLANG_INT = 258,          /* RCSS_CLANG_INT  */
    RCSS_CLANG_REAL = 259,         /* RCSS_CLANG_REAL  */
    RCSS_CLANG_STR = 260,          /* RCSS_CLANG_STR  */
    RCSS_CLANG_VAR = 261,          /* RCSS_CLANG_VAR  */
    RCSS_CLANG_LP = 262,           /* "("  */
    RCSS_CLANG_RP = 263,           /* ")"  */
    RCSS_CLANG_LB = 264,           /* "{"  */
    RCSS_CLANG_RB = 265,           /* "}"  */
    RCSS_CLANG_SAY = 266,          /* "say"  */
    RCSS_CLANG_HEAR = 267,         /* "hear"  */
    RCSS_CLANG_COACH_SIDE = 268,   /* RCSS_CLANG_COACH_SIDE  */
    RCSS_CLANG_META = 269,         /* "meta"  */
    RCSS_CLANG_FREEFORM = 270,     /* "freeform"  */
    RCSS_CLANG_UNSUPP = 271,       /* "unsupported_clang"  */
    RCSS_CLANG_INFO = 272,         /* "info"  */
    RCSS_CLANG_ADVICE = 273,       /* "advice"  */
    RCSS_CLANG_DEFINE = 274,       /* "define"  */
    RCSS_CLANG_DEFINEC = 275,      /* "definec"  */
    RCSS_CLANG_DEFINED = 276,      /* "defined"  */
    RCSS_CLANG_DEFINER = 277,      /* "definer"  */
    RCSS_CLANG_DEFINEA = 278,      /* "definea"  */
    RCSS_CLANG_VER = 279,          /* "ver"  */
    RCSS_CLANG_NULL_REGION = 280,  /* "null"  */
    RCSS_CLANG_QUAD = 281,         /* "quad"  */
    RCSS_CLANG_ARC = 282,          /* "arc"  */
    RCSS_CLANG_PT = 283,           /* "pt"  */
    RCSS_CLANG_REG = 284,          /* "reg"  */
    RCSS_CLANG_TRI = 285,          /* "tri"  */
    RCSS_CLANG_REC = 286,          /* "rec"  */
    RCSS_CLANG_BALL = 287,         /* "ball"  */
    RCSS_CLANG_POS = 288,          /* "pos"  */
    RCSS_CLANG_HOME = 289,         /* "home"  */
    RCSS_CLANG_BALLTO = 290,       /* "bto"  */
    RCSS_CLANG_MARK = 291,         /* "mark"  */
    RCSS_CLANG_MARK_LINE = 292,    /* "markl"  */
    RCSS_CLANG_OFFLINE = 293,      /* "oline"  */
    RCSS_CLANG_HTYPE = 294,        /* "htype"  */
    RCSS_CLANG_BMOVE_TOKEN = 295,  /* RCSS_CLANG_BMOVE_TOKEN  */
    RCSS_CLANG_DO_DONT = 296,      /* RCSS_CLANG_DO_DONT  */
    RCSS_CLANG_TEAM = 297,         /* RCSS_CLANG_TEAM  */
    RCSS_CLANG_TRUE_KW = 298,      /* "true"  */
    RCSS_CLANG_FALSE_KW = 299,     /* "false"  */
    RCSS_CLANG_PPOS = 300,         /* "ppos"  */
    RCSS_CLANG_BPOS = 301,         /* "bpos"  */
    RCSS_CLANG_BOWNER = 302,       /* "bowner"  */
    RCSS_CLANG_AND = 303,          /* "and"  */
    RCSS_CLANG_OR = 304,           /* "or"  */
    RCSS_CLANG_NOT = 305,          /* "not"  */
    RCSS_CLANG_PLAY_MODE_KW = 306, /* "playm"  */
    RCSS_CLANG_PLAY_MODE = 307,    /* RCSS_CLANG_PLAY_MODE  */
    RCSS_CLANG_CLEAR = 308,        /* "clear"  */
    RCSS_CLANG_L = 309,            /* "<"  */
    RCSS_CLANG_G = 310,            /* ">"  */
    RCSS_CLANG_E = 311,            /* "=="  */
    RCSS_CLANG_NE = 312,           /* "!="  */
    RCSS_CLANG_LE = 313,           /* "<="  */
    RCSS_CLANG_GE = 314,           /* ">="  */
    RCSS_CLANG_PLUS = 315,         /* "+"  */
    RCSS_CLANG_MINUS = 316,        /* "-"  */
    RCSS_CLANG_MULT = 317,         /* "*"  */
    RCSS_CLANG_DIV = 318,          /* "/"  */
    RCSS_CLANG_TIME = 319,         /* "time"  */
    RCSS_CLANG_OPP_GOAL = 320,     /* "opp_goals"  */
    RCSS_CLANG_OUR_GOAL = 321,     /* "own_goals"  */
    RCSS_CLANG_GOAL_DIFF = 322,    /* "goal_diff"  */
    RCSS_CLANG_UNUM = 323,         /* "unum"  */
    RCSS_CLANG_PASS = 324,         /* "pass"  */
    RCSS_CLANG_DRIBBLE = 325,      /* "dribble"  */
    RCSS_CLANG_SHOOT = 326,        /* "shoot"  */
    RCSS_CLANG_HOLD = 327,         /* "hold"  */
    RCSS_CLANG_INTERCEPT = 328,    /* "intercept"  */
    RCSS_CLANG_TACKLE = 329,       /* "tackle"  */
    RCSS_CLANG_RULE = 330,         /* "rule"  */
    RCSS_CLANG_ON_OFF = 331,       /* "on|off"  */
    RCSS_CLANG_ALL = 332,          /* "all"  */
    RCSS_CLANG_DEL = 333,          /* "delete"  */
    RCSS_CLANG_DEFRULE = 334,      /* "definerule"  */
    RCSS_CLANG_MODEL = 335,        /* "model"  */
    RCSS_CLANG_DIREC = 336,        /* "direc"  */
    RCSS_CLANG_ERROR = 337         /* RCSS_CLANG_ERROR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif




int yyparse (rcss::clang::Parser::Param &param);


#endif /* !YY_YY_USERS_NATHAN_SIDIB_CODE_VS_CODE_RCSOCCERSIM_RCSSSERVER_19_0_0_BUILD_RCSS_CLANG_COACH_LANG_PARSER_HPP_INCLUDED  */
