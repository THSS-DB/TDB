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

#ifndef YY_YY_YACC_SQL_HPP_INCLUDED
# define YY_YY_YACC_SQL_HPP_INCLUDED
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
    SEMICOLON = 258,               /* SEMICOLON  */
    CREATE = 259,                  /* CREATE  */
    DROP = 260,                    /* DROP  */
    VIEW = 261,                    /* VIEW  */
    TABLE = 262,                   /* TABLE  */
    TABLES = 263,                  /* TABLES  */
    INDEX = 264,                   /* INDEX  */
    UNIQUE = 265,                  /* UNIQUE  */
    CALC = 266,                    /* CALC  */
    SELECT = 267,                  /* SELECT  */
    ASC = 268,                     /* ASC  */
    DESC = 269,                    /* DESC  */
    ORDER = 270,                   /* ORDER  */
    BY = 271,                      /* BY  */
    IS = 272,                      /* IS  */
    NULL_T = 273,                  /* NULL_T  */
    SHOW = 274,                    /* SHOW  */
    SYNC = 275,                    /* SYNC  */
    INSERT = 276,                  /* INSERT  */
    DELETE = 277,                  /* DELETE  */
    UPDATE = 278,                  /* UPDATE  */
    LBRACE = 279,                  /* LBRACE  */
    RBRACE = 280,                  /* RBRACE  */
    COMMA = 281,                   /* COMMA  */
    TRX_BEGIN = 282,               /* TRX_BEGIN  */
    TRX_COMMIT = 283,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 284,            /* TRX_ROLLBACK  */
    INT_T = 285,                   /* INT_T  */
    STRING_T = 286,                /* STRING_T  */
    FLOAT_T = 287,                 /* FLOAT_T  */
    DATE_T = 288,                  /* DATE_T  */
    TEXT_T = 289,                  /* TEXT_T  */
    NOT_T = 290,                   /* NOT_T  */
    LIKE_T = 291,                  /* LIKE_T  */
    COUNT_T = 292,                 /* COUNT_T  */
    MIN_T = 293,                   /* MIN_T  */
    MAX_T = 294,                   /* MAX_T  */
    AVG_T = 295,                   /* AVG_T  */
    SUM_T = 296,                   /* SUM_T  */
    HELP = 297,                    /* HELP  */
    EXIT = 298,                    /* EXIT  */
    DOT = 299,                     /* DOT  */
    INTO = 300,                    /* INTO  */
    VALUES = 301,                  /* VALUES  */
    FROM = 302,                    /* FROM  */
    WHERE = 303,                   /* WHERE  */
    AND = 304,                     /* AND  */
    OR = 305,                      /* OR  */
    SET = 306,                     /* SET  */
    INNER = 307,                   /* INNER  */
    JOIN = 308,                    /* JOIN  */
    ON = 309,                      /* ON  */
    LOAD = 310,                    /* LOAD  */
    DATA = 311,                    /* DATA  */
    INFILE = 312,                  /* INFILE  */
    EXPLAIN = 313,                 /* EXPLAIN  */
    GROUP = 314,                   /* GROUP  */
    HAVING = 315,                  /* HAVING  */
    AS = 316,                      /* AS  */
    IN_T = 317,                    /* IN_T  */
    EXISTS_T = 318,                /* EXISTS_T  */
    EQ = 319,                      /* EQ  */
    LT = 320,                      /* LT  */
    GT = 321,                      /* GT  */
    LE = 322,                      /* LE  */
    GE = 323,                      /* GE  */
    NE = 324,                      /* NE  */
    NUMBER = 325,                  /* NUMBER  */
    FLOAT = 326,                   /* FLOAT  */
    ID = 327,                      /* ID  */
    SSS = 328,                     /* SSS  */
    DATE_STR = 329                 /* DATE_STR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 132 "yacc_sql.y"

  ParsedSqlNode *                   sql_node;
  ConditionSqlNode *                condition;
  Value *                           value;
  enum CompOp                       comp;
  RelAttrSqlNode *                  rel_attr;
  std::vector<RelAttrSqlNode> *     rel_attr_list;
  RelationSqlNode *                 relation;
  std::vector<AttrInfoSqlNode> *    attr_infos;
  AttrInfoSqlNode *                 attr_info;
  std::vector<UpdateUnit> *         update_infos;
  UpdateUnit *                      update_info;
  std::vector<OrderByNode> *        order_infos;
  OrderByNode *                     order_info;
  Expression *                      expression;
  std::vector<Expression *> *       expression_list;
  std::vector<Value> *              value_list;
  std::vector<Value> *              value_list_body;
  std::vector<std::vector<Value>> * multi_value_list;
  std::vector<std::string> *        multi_attribute_names;
  WhereConditions * 		    condition_list;
  std::vector<JoinSqlNode> *        join_list;
  std::vector<RelationSqlNode> *    relation_list;
  char *                            string;
  int                               number;
  float                             floats;

#line 166 "yacc_sql.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner);


#endif /* !YY_YY_YACC_SQL_HPP_INCLUDED  */
