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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    OP_PLUS = 258,                 /* OP_PLUS  */
    OP_MULTIPLY = 259,             /* OP_MULTIPLY  */
    OP_MINUS = 260,                /* OP_MINUS  */
    OP_DIV = 261,                  /* OP_DIV  */
    UMINUS = 262,                  /* UMINUS  */
    OR = 263,                      /* OR  */
    AND = 264,                     /* AND  */
    LESS = 265,                    /* LESS  */
    LE = 266,                      /* LE  */
    EQ = 267,                      /* EQ  */
    NOT = 268,                     /* NOT  */
    GREATER = 269,                 /* GREATER  */
    GE = 270,                      /* GE  */
    NEQ = 271,                     /* NEQ  */
    BRACE = 272,                   /* BRACE  */
    BRACKET = 273,                 /* BRACKET  */
    PARENTHESIS = 274,             /* PARENTHESIS  */
    PUBLIC = 275,                  /* PUBLIC  */
    INT = 276,                     /* INT  */
    MAIN = 277,                    /* MAIN  */
    CLASS = 278,                   /* CLASS  */
    EXTENDS = 279,                 /* EXTENDS  */
    IF = 280,                      /* IF  */
    ELSE = 281,                    /* ELSE  */
    WHILE = 282,                   /* WHILE  */
    CONTINUE = 283,                /* CONTINUE  */
    BREAK = 284,                   /* BREAK  */
    RETURN = 285,                  /* RETURN  */
    PUTINT = 286,                  /* PUTINT  */
    PUTCH = 287,                   /* PUTCH  */
    PUTARRAY = 288,                /* PUTARRAY  */
    GETINT = 289,                  /* GETINT  */
    GETCH = 290,                   /* GETCH  */
    GETARRAY = 291,                /* GETARRAY  */
    STARTTIME = 292,               /* STARTTIME  */
    STOPTIME = 293,                /* STOPTIME  */
    True = 294,                    /* True  */
    False = 295,                   /* False  */
    LENGTH = 296,                  /* LENGTH  */
    THIS = 297,                    /* THIS  */
    NEW = 298,                     /* NEW  */
    INT_CONST = 299,               /* INT_CONST  */
    IDENTIFIER = 300               /* IDENTIFIER  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define OP_PLUS 258
#define OP_MULTIPLY 259
#define OP_MINUS 260
#define OP_DIV 261
#define UMINUS 262
#define OR 263
#define AND 264
#define LESS 265
#define LE 266
#define EQ 267
#define NOT 268
#define GREATER 269
#define GE 270
#define NEQ 271
#define BRACE 272
#define BRACKET 273
#define PARENTHESIS 274
#define PUBLIC 275
#define INT 276
#define MAIN 277
#define CLASS 278
#define EXTENDS 279
#define IF 280
#define ELSE 281
#define WHILE 282
#define CONTINUE 283
#define BREAK 284
#define RETURN 285
#define PUTINT 286
#define PUTCH 287
#define PUTARRAY 288
#define GETINT 289
#define GETCH 290
#define GETARRAY 291
#define STARTTIME 292
#define STOPTIME 293
#define True 294
#define False 295
#define LENGTH 296
#define THIS 297
#define NEW 298
#define INT_CONST 299
#define IDENTIFIER 300

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 19 "parser.yacc"

    A_pos token;
    string id;
    A_prog prog;
    A_mainMethod main;
    A_stm stm;
	A_stmList stmlist;
	A_varDecl var;
	A_varDeclList varlist;
	A_classDecl classdec;
	A_classDeclList classdeclist;
	A_methodDecl method;
	A_methodDeclList methodlist;
	A_formalList formallist;
    A_exp exp;
	A_expList explist;
	A_type type;

#line 176 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
