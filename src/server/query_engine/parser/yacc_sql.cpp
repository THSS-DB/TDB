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
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "include/query_engine/parser/parse_defs.h"
#include "yacc_sql.hpp"
#include "lex_sql.h"
#include "include/query_engine/structor/expression/aggregation_expression.h"
#include "include/query_engine/structor/expression/arithmetic_expression.h"
#include "include/query_engine/structor/expression/comparison_expression.h"
#include "include/query_engine/structor/expression/conjunction_expression.h"
#include "include/query_engine/structor/expression/field_expression.h"
#include "include/query_engine/structor/expression/value_expression.h"
#include "include/query_engine/structor/expression/attribute_expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}


#line 121 "yacc_sql.cpp"

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

#include "yacc_sql.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_CREATE = 4,                     /* CREATE  */
  YYSYMBOL_DROP = 5,                       /* DROP  */
  YYSYMBOL_VIEW = 6,                       /* VIEW  */
  YYSYMBOL_TABLE = 7,                      /* TABLE  */
  YYSYMBOL_TABLES = 8,                     /* TABLES  */
  YYSYMBOL_INDEX = 9,                      /* INDEX  */
  YYSYMBOL_UNIQUE = 10,                    /* UNIQUE  */
  YYSYMBOL_CALC = 11,                      /* CALC  */
  YYSYMBOL_SELECT = 12,                    /* SELECT  */
  YYSYMBOL_ASC = 13,                       /* ASC  */
  YYSYMBOL_DESC = 14,                      /* DESC  */
  YYSYMBOL_ORDER = 15,                     /* ORDER  */
  YYSYMBOL_BY = 16,                        /* BY  */
  YYSYMBOL_IS = 17,                        /* IS  */
  YYSYMBOL_NULL_T = 18,                    /* NULL_T  */
  YYSYMBOL_SHOW = 19,                      /* SHOW  */
  YYSYMBOL_SYNC = 20,                      /* SYNC  */
  YYSYMBOL_INSERT = 21,                    /* INSERT  */
  YYSYMBOL_DELETE = 22,                    /* DELETE  */
  YYSYMBOL_UPDATE = 23,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 24,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 25,                    /* RBRACE  */
  YYSYMBOL_COMMA = 26,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 27,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 28,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 29,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 30,                     /* INT_T  */
  YYSYMBOL_STRING_T = 31,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 32,                   /* FLOAT_T  */
  YYSYMBOL_DATE_T = 33,                    /* DATE_T  */
  YYSYMBOL_TEXT_T = 34,                    /* TEXT_T  */
  YYSYMBOL_NOT_T = 35,                     /* NOT_T  */
  YYSYMBOL_LIKE_T = 36,                    /* LIKE_T  */
  YYSYMBOL_COUNT_T = 37,                   /* COUNT_T  */
  YYSYMBOL_MIN_T = 38,                     /* MIN_T  */
  YYSYMBOL_MAX_T = 39,                     /* MAX_T  */
  YYSYMBOL_AVG_T = 40,                     /* AVG_T  */
  YYSYMBOL_SUM_T = 41,                     /* SUM_T  */
  YYSYMBOL_HELP = 42,                      /* HELP  */
  YYSYMBOL_EXIT = 43,                      /* EXIT  */
  YYSYMBOL_DOT = 44,                       /* DOT  */
  YYSYMBOL_INTO = 45,                      /* INTO  */
  YYSYMBOL_VALUES = 46,                    /* VALUES  */
  YYSYMBOL_FROM = 47,                      /* FROM  */
  YYSYMBOL_WHERE = 48,                     /* WHERE  */
  YYSYMBOL_AND = 49,                       /* AND  */
  YYSYMBOL_OR = 50,                        /* OR  */
  YYSYMBOL_SET = 51,                       /* SET  */
  YYSYMBOL_INNER = 52,                     /* INNER  */
  YYSYMBOL_JOIN = 53,                      /* JOIN  */
  YYSYMBOL_ON = 54,                        /* ON  */
  YYSYMBOL_LOAD = 55,                      /* LOAD  */
  YYSYMBOL_DATA = 56,                      /* DATA  */
  YYSYMBOL_INFILE = 57,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 58,                   /* EXPLAIN  */
  YYSYMBOL_GROUP = 59,                     /* GROUP  */
  YYSYMBOL_HAVING = 60,                    /* HAVING  */
  YYSYMBOL_AS = 61,                        /* AS  */
  YYSYMBOL_IN_T = 62,                      /* IN_T  */
  YYSYMBOL_EXISTS_T = 63,                  /* EXISTS_T  */
  YYSYMBOL_EQ = 64,                        /* EQ  */
  YYSYMBOL_LT = 65,                        /* LT  */
  YYSYMBOL_GT = 66,                        /* GT  */
  YYSYMBOL_LE = 67,                        /* LE  */
  YYSYMBOL_GE = 68,                        /* GE  */
  YYSYMBOL_NE = 69,                        /* NE  */
  YYSYMBOL_NUMBER = 70,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 71,                     /* FLOAT  */
  YYSYMBOL_ID = 72,                        /* ID  */
  YYSYMBOL_SSS = 73,                       /* SSS  */
  YYSYMBOL_DATE_STR = 74,                  /* DATE_STR  */
  YYSYMBOL_75_ = 75,                       /* '+'  */
  YYSYMBOL_76_ = 76,                       /* '-'  */
  YYSYMBOL_77_ = 77,                       /* '*'  */
  YYSYMBOL_78_ = 78,                       /* '/'  */
  YYSYMBOL_YYACCEPT = 79,                  /* $accept  */
  YYSYMBOL_commands = 80,                  /* commands  */
  YYSYMBOL_command_wrapper = 81,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 82,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 83,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 84,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 85,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 86,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 87,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 88,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 89,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 90,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 91,         /* create_index_stmt  */
  YYSYMBOL_multi_attribute_names = 92,     /* multi_attribute_names  */
  YYSYMBOL_drop_index_stmt = 93,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 94,         /* create_table_stmt  */
  YYSYMBOL_create_view_stmt = 95,          /* create_view_stmt  */
  YYSYMBOL_attr_def_list = 96,             /* attr_def_list  */
  YYSYMBOL_attr_def = 97,                  /* attr_def  */
  YYSYMBOL_number = 98,                    /* number  */
  YYSYMBOL_type = 99,                      /* type  */
  YYSYMBOL_aggr_type = 100,                /* aggr_type  */
  YYSYMBOL_insert_stmt = 101,              /* insert_stmt  */
  YYSYMBOL_multi_value_list = 102,         /* multi_value_list  */
  YYSYMBOL_value_list = 103,               /* value_list  */
  YYSYMBOL_value_list_body = 104,          /* value_list_body  */
  YYSYMBOL_value = 105,                    /* value  */
  YYSYMBOL_delete_stmt = 106,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 107,              /* update_stmt  */
  YYSYMBOL_update_def_list = 108,          /* update_def_list  */
  YYSYMBOL_update_def = 109,               /* update_def  */
  YYSYMBOL_select_stmt = 110,              /* select_stmt  */
  YYSYMBOL_opt_group_by = 111,             /* opt_group_by  */
  YYSYMBOL_opt_having = 112,               /* opt_having  */
  YYSYMBOL_opt_order_by = 113,             /* opt_order_by  */
  YYSYMBOL_sort_def_list = 114,            /* sort_def_list  */
  YYSYMBOL_sort_def = 115,                 /* sort_def  */
  YYSYMBOL_calc_stmt = 116,                /* calc_stmt  */
  YYSYMBOL_aggr_expr = 117,                /* aggr_expr  */
  YYSYMBOL_base_expr = 118,                /* base_expr  */
  YYSYMBOL_mul_expr = 119,                 /* mul_expr  */
  YYSYMBOL_add_expr = 120,                 /* add_expr  */
  YYSYMBOL_select_attr = 121,              /* select_attr  */
  YYSYMBOL_expression_list = 122,          /* expression_list  */
  YYSYMBOL_rel_attr = 123,                 /* rel_attr  */
  YYSYMBOL_rel_attr_list = 124,            /* rel_attr_list  */
  YYSYMBOL_relation_list = 125,            /* relation_list  */
  YYSYMBOL_rel_list = 126,                 /* rel_list  */
  YYSYMBOL_join_list = 127,                /* join_list  */
  YYSYMBOL_join_conditions = 128,          /* join_conditions  */
  YYSYMBOL_where_conditions = 129,         /* where_conditions  */
  YYSYMBOL_condition_list = 130,           /* condition_list  */
  YYSYMBOL_condition = 131,                /* condition  */
  YYSYMBOL_comp_op = 132,                  /* comp_op  */
  YYSYMBOL_load_data_stmt = 133,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 134,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 135,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 136             /* opt_semicolon  */
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

#if 1

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
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  80
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   337

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  79
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  58
/* YYNRULES -- Number of rules.  */
#define YYNRULES  157
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  300

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   329


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
       2,     2,    77,    75,     2,    76,     2,    78,     2,     2,
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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   230,   230,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   262,   268,   273,   279,   285,   291,
     297,   304,   310,   318,   334,   354,   357,   369,   380,   399,
     406,   417,   420,   433,   442,   451,   460,   469,   478,   490,
     494,   495,   496,   497,   498,   503,   504,   505,   506,   507,
     511,   527,   530,   543,   558,   561,   574,   577,   580,   583,
     586,   590,   594,   602,   615,   637,   640,   653,   663,   705,
     708,   713,   716,   723,   726,   733,   738,   750,   756,   763,
     772,   782,   788,   791,   802,   806,   810,   813,   816,   827,
     829,   831,   833,   839,   841,   843,   849,   860,   871,   878,
     891,   893,   903,   914,   921,   930,   939,   953,   958,   968,
     972,   983,   994,  1006,  1021,  1023,  1034,  1046,  1063,  1066,
    1090,  1093,  1101,  1104,  1110,  1112,  1116,  1121,  1131,  1136,
    1142,  1146,  1151,  1157,  1162,  1170,  1171,  1172,  1173,  1174,
    1175,  1176,  1177,  1181,  1194,  1202,  1212,  1213
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "CREATE",
  "DROP", "VIEW", "TABLE", "TABLES", "INDEX", "UNIQUE", "CALC", "SELECT",
  "ASC", "DESC", "ORDER", "BY", "IS", "NULL_T", "SHOW", "SYNC", "INSERT",
  "DELETE", "UPDATE", "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN",
  "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T", "FLOAT_T", "DATE_T",
  "TEXT_T", "NOT_T", "LIKE_T", "COUNT_T", "MIN_T", "MAX_T", "AVG_T",
  "SUM_T", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM", "WHERE", "AND",
  "OR", "SET", "INNER", "JOIN", "ON", "LOAD", "DATA", "INFILE", "EXPLAIN",
  "GROUP", "HAVING", "AS", "IN_T", "EXISTS_T", "EQ", "LT", "GT", "LE",
  "GE", "NE", "NUMBER", "FLOAT", "ID", "SSS", "DATE_STR", "'+'", "'-'",
  "'*'", "'/'", "$accept", "commands", "command_wrapper", "exit_stmt",
  "help_stmt", "sync_stmt", "begin_stmt", "commit_stmt", "rollback_stmt",
  "drop_table_stmt", "show_tables_stmt", "desc_table_stmt",
  "create_index_stmt", "multi_attribute_names", "drop_index_stmt",
  "create_table_stmt", "create_view_stmt", "attr_def_list", "attr_def",
  "number", "type", "aggr_type", "insert_stmt", "multi_value_list",
  "value_list", "value_list_body", "value", "delete_stmt", "update_stmt",
  "update_def_list", "update_def", "select_stmt", "opt_group_by",
  "opt_having", "opt_order_by", "sort_def_list", "sort_def", "calc_stmt",
  "aggr_expr", "base_expr", "mul_expr", "add_expr", "select_attr",
  "expression_list", "rel_attr", "rel_attr_list", "relation_list",
  "rel_list", "join_list", "join_conditions", "where_conditions",
  "condition_list", "condition", "comp_op", "load_data_stmt",
  "explain_stmt", "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-205)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-65)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     279,   102,    16,    11,    11,   -58,    24,  -205,   -11,    26,
     -18,  -205,  -205,  -205,  -205,  -205,     5,    50,   279,   121,
      77,  -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,
    -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,
    -205,  -205,    71,    91,    92,   145,    94,    95,  -205,   190,
    -205,  -205,  -205,  -205,  -205,  -205,  -205,    86,  -205,  -205,
     201,   139,   147,  -205,  -205,  -205,  -205,    36,    75,  -205,
    -205,   122,  -205,  -205,    98,   105,   124,   114,   123,  -205,
    -205,  -205,  -205,    -2,   155,   127,   110,  -205,   129,   140,
      93,   -15,   -56,  -205,  -205,    64,  -205,    52,  -205,   -32,
     241,   241,   120,   190,   190,  -205,   125,   149,   138,   126,
      29,   128,   130,   181,   131,   132,   142,   134,   135,    29,
     175,  -205,  -205,   139,  -205,  -205,   165,   139,   -19,   186,
     187,   191,  -205,  -205,   139,    36,    36,    -6,   163,   194,
     150,  -205,   168,   207,  -205,   189,   209,   211,  -205,   107,
     217,   213,   172,  -205,   219,  -205,  -205,   -36,  -205,   -44,
     139,  -205,  -205,  -205,  -205,  -205,   174,   177,   221,  -205,
     197,   138,    29,   225,   192,   190,    80,  -205,   103,   190,
     126,   138,   245,   130,   193,  -205,  -205,  -205,  -205,  -205,
      -5,   131,   228,   184,   233,  -205,   139,   139,   139,  -205,
       7,   221,  -205,   195,   199,   219,   194,  -205,   190,    82,
       8,   -25,  -205,   190,  -205,  -205,  -205,  -205,  -205,  -205,
     190,   150,   150,    82,   207,  -205,   196,  -205,   181,  -205,
     200,   251,   217,  -205,   250,   214,  -205,  -205,  -205,   215,
     221,  -205,  -205,   231,   272,   229,   225,    82,  -205,   274,
    -205,   190,    82,    82,  -205,  -205,  -205,  -205,  -205,  -205,
     269,  -205,  -205,   223,   271,   250,   221,  -205,   150,   163,
     130,   150,   282,  -205,  -205,    82,     9,   250,  -205,   278,
    -205,  -205,  -205,  -205,  -205,   288,  -205,  -205,   287,  -205,
    -205,   130,  -205,  -205,   283,   146,   130,  -205,  -205,  -205
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,    27,    28,    29,    25,    24,     0,     0,     0,     0,
     156,    23,    22,    15,    16,    17,    18,    10,    11,    12,
      13,    14,     8,     9,     5,     7,     6,     4,     3,    19,
      20,    21,     0,     0,     0,     0,     0,     0,    72,     0,
      55,    56,    57,    58,    59,    66,    68,   117,    70,    71,
       0,   110,     0,    98,    94,    97,    99,   103,   110,    90,
      95,     0,    32,    31,     0,     0,     0,     0,     0,   154,
       1,   157,     2,     0,     0,     0,     0,    30,     0,   117,
      94,     0,     0,    66,    68,     0,   100,     0,   106,     0,
       0,     0,     0,     0,     0,   108,     0,     0,   132,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,   118,   110,    67,    69,   117,   110,   110,     0,
       0,     0,   101,   102,   110,   104,   105,   124,   128,     0,
     134,    73,     0,    75,   155,     0,   119,     0,    39,     0,
      41,     0,     0,    37,    64,    63,   107,     0,   111,     0,
     110,   113,    93,    91,    92,   109,     0,     0,   124,   121,
       0,   132,     0,    61,     0,     0,     0,   133,   135,     0,
       0,   132,     0,     0,     0,    50,    51,    52,    53,    54,
      44,     0,     0,     0,     0,    65,   110,   110,   110,   114,
     124,   124,   122,     0,    79,    64,     0,    60,     0,   143,
       0,     0,   151,     0,   145,   146,   147,   148,   149,   150,
       0,   134,   134,    77,    75,    74,     0,   120,     0,    48,
       0,     0,    41,    38,    35,     0,   112,   116,   115,     0,
     124,   125,   123,   130,     0,    81,    61,   144,   139,     0,
     152,     0,   141,   138,   136,   137,    76,   153,    40,    49,
       0,    46,    42,     0,     0,    35,   124,   126,   134,   128,
       0,   134,    83,    62,   140,   142,    43,    35,    34,     0,
     127,   131,   129,    80,    82,     0,    78,    47,     0,    36,
      33,     0,    45,    84,    85,    87,     0,    89,    88,    86
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -205,  -205,   292,  -205,  -205,  -205,  -205,  -205,  -205,  -205,
    -205,  -205,  -205,  -167,  -205,  -205,  -205,    84,   133,  -205,
    -205,  -205,  -205,    72,  -131,   166,   -45,  -205,  -205,    99,
     148,  -108,  -205,  -205,  -205,    23,  -205,  -205,  -205,   -29,
      58,    -3,   321,   -65,   -97,  -174,  -205,  -162,    57,  -205,
     -64,  -204,  -205,  -205,  -205,  -205,  -205,  -205
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,   264,    31,    32,    33,   192,   150,   260,
     190,    62,    34,   207,    63,   120,    64,    35,    36,   181,
     143,    37,   245,   272,   286,   293,   294,    38,    65,    66,
      67,   176,    69,    98,    70,   147,   138,   169,   171,   269,
     141,   177,   178,   220,    39,    40,    41,    82
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      68,    68,   131,   105,    90,   148,   202,    97,   173,   227,
     121,   250,   197,   229,    72,   146,   122,   254,   255,   230,
     166,   123,   112,    46,   129,    47,   248,   287,   198,    48,
     231,    96,    73,   166,    74,    49,   122,   251,   241,   242,
      89,   196,   159,   249,   288,   130,    91,    48,    50,    51,
      52,    53,    54,   160,    76,   167,   103,   104,   156,   113,
     103,   104,   158,   161,   281,   144,   168,   284,   239,   165,
      48,   132,   133,    75,   154,   246,    49,    77,   267,   240,
      81,    55,    56,    57,    58,    59,   146,    60,    61,    50,
      51,    52,    53,    54,   128,   199,   283,   210,   279,    55,
      56,    97,    58,    59,   280,    95,    78,   204,    42,    43,
     289,    44,    45,   100,   101,   211,   212,   225,   -64,   119,
     258,    80,    55,    56,   126,    58,    59,   205,    60,   127,
      92,   236,   237,   238,   124,   125,   102,   185,   186,   187,
     188,   189,   213,    83,   214,   215,   216,   217,   218,   219,
     103,   104,   221,   222,    86,   103,   104,   103,   104,   297,
     298,   135,   136,    84,    85,    97,    87,    88,    48,   106,
     107,    99,   209,   146,    49,   109,   223,   108,   110,   114,
     111,   115,   116,   117,   118,   174,   140,    50,    51,    52,
      53,    54,   134,     4,   295,   139,   152,   137,   142,   295,
     155,   145,    89,   149,   151,   247,   153,   122,    48,   157,
     252,   162,   163,   175,    49,   170,   164,   253,   172,    48,
      55,    56,    89,    58,    59,    49,    60,    50,    51,    52,
      53,    54,   179,   180,   182,   183,   184,   193,    50,    51,
      52,    53,    54,   191,   194,   119,   200,   166,   275,   201,
     203,   206,   226,   233,   228,   208,   234,   235,   244,    48,
      55,    56,    89,    58,    59,    49,    60,   243,   257,   261,
     259,    93,    94,    89,    58,    59,   263,    95,    50,    51,
      52,    53,    54,     1,     2,   268,   265,   266,   270,   271,
       3,     4,   274,     5,   276,   277,   278,   285,     6,     7,
       8,     9,    10,   290,   291,   292,    11,    12,    13,   296,
      79,    55,    56,    89,    58,    59,   262,    95,   273,   299,
     195,    14,    15,   256,   232,    71,   282,     0,   224,     0,
      16,     0,     0,     0,    17,     0,     0,    18
};

static const yytype_int16 yycheck[] =
{
       3,     4,    99,    68,    49,   113,   168,    26,   139,   183,
      25,    36,    56,    18,    72,   112,    72,   221,   222,    24,
      26,    77,    24,     7,    56,     9,    18,    18,    72,    18,
      35,    60,     8,    26,    45,    24,    72,    62,   200,   201,
      72,    77,    61,    35,    35,    77,    49,    18,    37,    38,
      39,    40,    41,    72,    72,    61,    75,    76,   123,    61,
      75,    76,   127,   128,   268,   110,    72,   271,    61,   134,
      18,   100,   101,    47,   119,   206,    24,    72,   240,    72,
       3,    70,    71,    72,    73,    74,   183,    76,    77,    37,
      38,    39,    40,    41,    97,   160,   270,    17,   265,    70,
      71,    26,    73,    74,   266,    76,    56,   171,     6,     7,
     277,     9,    10,    77,    78,    35,    36,   181,    25,    26,
     228,     0,    70,    71,    72,    73,    74,   172,    76,    77,
      44,   196,   197,   198,    70,    71,    61,    30,    31,    32,
      33,    34,    62,    72,    64,    65,    66,    67,    68,    69,
      75,    76,    49,    50,     9,    75,    76,    75,    76,    13,
      14,   103,   104,    72,    72,    26,    72,    72,    18,    47,
      72,    24,   175,   270,    24,    51,   179,    72,    64,    24,
      57,    54,    72,    54,    44,    35,    48,    37,    38,    39,
      40,    41,    72,    12,   291,    46,    54,    72,    72,   296,
      25,    73,    72,    72,    72,   208,    72,    72,    18,    44,
     213,    25,    25,    63,    24,    52,    25,   220,    24,    18,
      70,    71,    72,    73,    74,    24,    76,    37,    38,    39,
      40,    41,    64,    26,    45,    26,    25,    24,    37,    38,
      39,    40,    41,    26,    72,    26,    72,    26,   251,    72,
      53,    26,     7,    25,    61,    63,    72,    24,    59,    18,
      70,    71,    72,    73,    74,    24,    76,    72,    72,    18,
      70,    70,    71,    72,    73,    74,    26,    76,    37,    38,
      39,    40,    41,     4,     5,    54,    72,    72,    16,    60,
      11,    12,    18,    14,    25,    72,    25,    15,    19,    20,
      21,    22,    23,    25,    16,    18,    27,    28,    29,    26,
      18,    70,    71,    72,    73,    74,   232,    76,   246,   296,
     154,    42,    43,   224,   191,     4,   269,    -1,   180,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    58
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,    11,    12,    14,    19,    20,    21,    22,
      23,    27,    28,    29,    42,    43,    51,    55,    58,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    93,    94,    95,   101,   106,   107,   110,   116,   133,
     134,   135,     6,     7,     9,    10,     7,     9,    18,    24,
      37,    38,    39,    40,    41,    70,    71,    72,    73,    74,
      76,    77,   100,   103,   105,   117,   118,   119,   120,   121,
     123,   121,    72,     8,    45,    47,    72,    72,    56,    81,
       0,     3,   136,    72,    72,    72,     9,    72,    72,    72,
     105,   120,    44,    70,    71,    76,   118,    26,   122,    24,
      77,    78,    61,    75,    76,   122,    47,    72,    72,    51,
      64,    57,    24,    61,    24,    54,    72,    54,    44,    26,
     104,    25,    72,    77,    70,    71,    72,    77,   120,    56,
      77,   123,   118,   118,    72,   119,   119,    72,   125,    46,
      48,   129,    72,   109,   105,    73,   123,   124,   110,    72,
      97,    72,    54,    72,   105,    25,   122,    44,   122,    61,
      72,   122,    25,    25,    25,   122,    26,    61,    72,   126,
      52,   127,    24,   103,    35,    63,   120,   130,   131,    64,
      26,   108,    45,    26,    25,    30,    31,    32,    33,    34,
      99,    26,    96,    24,    72,   104,    77,    56,    72,   122,
      72,    72,   126,    53,   129,   105,    26,   102,    63,   120,
      17,    35,    36,    62,    64,    65,    66,    67,    68,    69,
     132,    49,    50,   120,   109,   129,     7,   124,    61,    18,
      24,    35,    97,    25,    72,    24,   122,   122,   122,    61,
      72,   126,   126,    72,    59,   111,   103,   120,    18,    35,
      36,    62,   120,   120,   130,   130,   108,    72,   110,    70,
      98,    18,    96,    26,    92,    72,    72,   126,    54,   128,
      16,    60,   112,   102,    18,   120,    25,    72,    25,    92,
     126,   130,   127,   124,   130,    15,   113,    18,    35,    92,
      25,    16,    18,   114,   115,   123,    26,    13,    14,   114
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    79,    80,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    91,    92,    92,    93,    94,    95,
      95,    96,    96,    97,    97,    97,    97,    97,    97,    98,
      99,    99,    99,    99,    99,   100,   100,   100,   100,   100,
     101,   102,   102,   103,   104,   104,   105,   105,   105,   105,
     105,   105,   105,   106,   107,   108,   108,   109,   110,   111,
     111,   112,   112,   113,   113,   114,   114,   115,   115,   115,
     116,   117,   117,   117,   118,   118,   118,   118,   118,   119,
     119,   119,   119,   120,   120,   120,   121,   121,   121,   121,
     122,   122,   122,   122,   122,   122,   122,   123,   123,   124,
     124,   125,   125,   125,   126,   126,   126,   126,   127,   127,
     128,   128,   129,   129,   130,   130,   130,   130,   131,   131,
     131,   131,   131,   131,   131,   132,   132,   132,   132,   132,
     132,   132,   132,   133,   134,   135,   136,   136
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     2,     2,    10,     9,     0,     3,     5,     7,     5,
       8,     0,     3,     5,     2,     7,     4,     6,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       6,     0,     3,     4,     0,     3,     1,     2,     1,     2,
       1,     1,     1,     4,     6,     0,     3,     3,     9,     0,
       3,     0,     2,     0,     3,     1,     3,     1,     2,     2,
       2,     4,     4,     4,     1,     1,     3,     1,     1,     1,
       2,     3,     3,     1,     3,     3,     2,     4,     2,     4,
       0,     3,     5,     3,     4,     5,     5,     1,     3,     1,
       3,     2,     3,     4,     0,     3,     4,     5,     0,     5,
       0,     2,     0,     2,     0,     1,     3,     3,     3,     3,
       4,     3,     4,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     2,     7,     2,     4,     0,     1
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
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, sql_string, sql_result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, sql_string, sql_result, scanner);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), sql_string, sql_result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, sql_string, sql_result, scanner); \
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
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
yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yychar = yylex (&yylval, &yylloc, scanner);
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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* commands: command_wrapper opt_semicolon  */
#line 231 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1871 "yacc_sql.cpp"
    break;

  case 24: /* exit_stmt: EXIT  */
#line 262 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1880 "yacc_sql.cpp"
    break;

  case 25: /* help_stmt: HELP  */
#line 268 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1888 "yacc_sql.cpp"
    break;

  case 26: /* sync_stmt: SYNC  */
#line 273 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1896 "yacc_sql.cpp"
    break;

  case 27: /* begin_stmt: TRX_BEGIN  */
#line 279 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1904 "yacc_sql.cpp"
    break;

  case 28: /* commit_stmt: TRX_COMMIT  */
#line 285 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1912 "yacc_sql.cpp"
    break;

  case 29: /* rollback_stmt: TRX_ROLLBACK  */
#line 291 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1920 "yacc_sql.cpp"
    break;

  case 30: /* drop_table_stmt: DROP TABLE ID  */
#line 297 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1930 "yacc_sql.cpp"
    break;

  case 31: /* show_tables_stmt: SHOW TABLES  */
#line 304 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1938 "yacc_sql.cpp"
    break;

  case 32: /* desc_table_stmt: DESC ID  */
#line 310 "yacc_sql.y"
             {
	(yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
	(yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
	free((yyvsp[0].string));
    }
#line 1948 "yacc_sql.cpp"
    break;

  case 33: /* create_index_stmt: CREATE UNIQUE INDEX ID ON ID LBRACE ID multi_attribute_names RBRACE  */
#line 319 "yacc_sql.y"
  {
	(yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
	CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
	create_index.index_name = (yyvsp[-6].string);
	create_index.relation_name = (yyvsp[-4].string);
	create_index.is_unique_ = true;
	if ((yyvsp[-1].multi_attribute_names) != nullptr) {
	create_index.multi_attribute_names.swap(*(yyvsp[-1].multi_attribute_names));
	}
	create_index.multi_attribute_names.emplace_back((yyvsp[-2].string));
	std::reverse(create_index.multi_attribute_names.begin(), create_index.multi_attribute_names.end());
	free((yyvsp[-6].string));
	free((yyvsp[-4].string));
	free((yyvsp[-2].string));
  }
#line 1968 "yacc_sql.cpp"
    break;

  case 34: /* create_index_stmt: CREATE INDEX ID ON ID LBRACE ID multi_attribute_names RBRACE  */
#line 335 "yacc_sql.y"
  {
	(yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
	CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
	create_index.index_name = (yyvsp[-6].string);
	create_index.relation_name = (yyvsp[-4].string);
	create_index.is_unique_ = false;
	if ((yyvsp[-1].multi_attribute_names) != nullptr) {
	create_index.multi_attribute_names.swap(*(yyvsp[-1].multi_attribute_names));
	}
	create_index.multi_attribute_names.emplace_back((yyvsp[-2].string));
	std::reverse(create_index.multi_attribute_names.begin(), create_index.multi_attribute_names.end());
	free((yyvsp[-6].string));
	free((yyvsp[-4].string));
	free((yyvsp[-2].string));
  }
#line 1988 "yacc_sql.cpp"
    break;

  case 35: /* multi_attribute_names: %empty  */
#line 354 "yacc_sql.y"
  {
	(yyval.multi_attribute_names) = nullptr;
  }
#line 1996 "yacc_sql.cpp"
    break;

  case 36: /* multi_attribute_names: COMMA ID multi_attribute_names  */
#line 357 "yacc_sql.y"
                                    {
	if ((yyvsp[0].multi_attribute_names) != nullptr) {
		(yyval.multi_attribute_names) = (yyvsp[0].multi_attribute_names);
	} else {
		(yyval.multi_attribute_names) = new std::vector<std::string>;
	}
	(yyval.multi_attribute_names)->emplace_back((yyvsp[-1].string));
	delete (yyvsp[-1].string);
  }
#line 2010 "yacc_sql.cpp"
    break;

  case 37: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 370 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2022 "yacc_sql.cpp"
    break;

  case 38: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE  */
#line 381 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-4].string);
      free((yyvsp[-4].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-1].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-2].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-2].attr_info);
    }
#line 2042 "yacc_sql.cpp"
    break;

  case 39: /* create_view_stmt: CREATE VIEW ID AS select_stmt  */
#line 399 "yacc_sql.y"
                                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_VIEW);
      CreateViewSqlNode &create_view = (yyval.sql_node)->create_view;
      create_view.view_name = (yyvsp[-2].string);
      create_view.select_sql_node = (yyvsp[0].sql_node)->selection;
      free((yyvsp[-2].string));

    }
#line 2055 "yacc_sql.cpp"
    break;

  case 40: /* create_view_stmt: CREATE VIEW ID LBRACE rel_attr_list RBRACE AS select_stmt  */
#line 406 "yacc_sql.y"
                                                                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_VIEW);
      CreateViewSqlNode &create_view = (yyval.sql_node)->create_view;
      create_view.view_name = (yyvsp[-5].string);
      create_view.select_sql_node = (yyvsp[0].sql_node)->selection;
      free((yyvsp[-5].string));
    }
#line 2067 "yacc_sql.cpp"
    break;

  case 41: /* attr_def_list: %empty  */
#line 417 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2075 "yacc_sql.cpp"
    break;

  case 42: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 421 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2089 "yacc_sql.cpp"
    break;

  case 43: /* attr_def: ID type LBRACE number RBRACE  */
#line 434 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-3].number);
      (yyval.attr_info)->name = (yyvsp[-4].string);
      (yyval.attr_info)->length = (yyvsp[-1].number);
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-4].string));
    }
#line 2102 "yacc_sql.cpp"
    break;

  case 44: /* attr_def: ID type  */
#line 443 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[0].number);
      (yyval.attr_info)->name = (yyvsp[-1].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-1].string));
    }
#line 2115 "yacc_sql.cpp"
    break;

  case 45: /* attr_def: ID type LBRACE number RBRACE NOT_T NULL_T  */
#line 452 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-5].number);
      (yyval.attr_info)->name = (yyvsp[-6].string);
      (yyval.attr_info)->length = (yyvsp[-3].number);
      (yyval.attr_info)->nullable = false;
      free((yyvsp[-6].string));
    }
#line 2128 "yacc_sql.cpp"
    break;

  case 46: /* attr_def: ID type NOT_T NULL_T  */
#line 461 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-2].number);
      (yyval.attr_info)->name = (yyvsp[-3].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = false;
      free((yyvsp[-3].string));
    }
#line 2141 "yacc_sql.cpp"
    break;

  case 47: /* attr_def: ID type LBRACE number RBRACE NULL_T  */
#line 470 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-5].string));
    }
#line 2154 "yacc_sql.cpp"
    break;

  case 48: /* attr_def: ID type NULL_T  */
#line 479 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-2].string));
    }
#line 2167 "yacc_sql.cpp"
    break;

  case 49: /* number: NUMBER  */
#line 490 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2173 "yacc_sql.cpp"
    break;

  case 50: /* type: INT_T  */
#line 494 "yacc_sql.y"
               { (yyval.number)=INTS; }
#line 2179 "yacc_sql.cpp"
    break;

  case 51: /* type: STRING_T  */
#line 495 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 2185 "yacc_sql.cpp"
    break;

  case 52: /* type: FLOAT_T  */
#line 496 "yacc_sql.y"
               { (yyval.number)=FLOATS; }
#line 2191 "yacc_sql.cpp"
    break;

  case 53: /* type: DATE_T  */
#line 497 "yacc_sql.y"
               { (yyval.number)=DATES; }
#line 2197 "yacc_sql.cpp"
    break;

  case 54: /* type: TEXT_T  */
#line 498 "yacc_sql.y"
               { (yyval.number)=TEXTS; }
#line 2203 "yacc_sql.cpp"
    break;

  case 55: /* aggr_type: COUNT_T  */
#line 503 "yacc_sql.y"
               { (yyval.number)=AGGR_COUNT; }
#line 2209 "yacc_sql.cpp"
    break;

  case 56: /* aggr_type: MIN_T  */
#line 504 "yacc_sql.y"
               { (yyval.number)=AGGR_MIN;   }
#line 2215 "yacc_sql.cpp"
    break;

  case 57: /* aggr_type: MAX_T  */
#line 505 "yacc_sql.y"
               { (yyval.number)=AGGR_MAX;   }
#line 2221 "yacc_sql.cpp"
    break;

  case 58: /* aggr_type: AVG_T  */
#line 506 "yacc_sql.y"
               { (yyval.number)=AGGR_AVG;   }
#line 2227 "yacc_sql.cpp"
    break;

  case 59: /* aggr_type: SUM_T  */
#line 507 "yacc_sql.y"
               { (yyval.number)=AGGR_SUM;   }
#line 2233 "yacc_sql.cpp"
    break;

  case 60: /* insert_stmt: INSERT INTO ID VALUES value_list multi_value_list  */
#line 512 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-3].string);
      if ((yyvsp[0].multi_value_list) != nullptr) {
        (yyval.sql_node)->insertion.multi_values.swap(*(yyvsp[0].multi_value_list));
      }
      (yyval.sql_node)->insertion.multi_values.emplace_back(*(yyvsp[-1].value_list));
      std::reverse((yyval.sql_node)->insertion.multi_values.begin(), (yyval.sql_node)->insertion.multi_values.end());
      delete (yyvsp[-1].value_list);
      free((yyvsp[-3].string));
    }
#line 2249 "yacc_sql.cpp"
    break;

  case 61: /* multi_value_list: %empty  */
#line 527 "yacc_sql.y"
    {
      (yyval.multi_value_list) = nullptr;
    }
#line 2257 "yacc_sql.cpp"
    break;

  case 62: /* multi_value_list: COMMA value_list multi_value_list  */
#line 531 "yacc_sql.y"
    {
      if ((yyvsp[0].multi_value_list) != nullptr) {
        (yyval.multi_value_list) = (yyvsp[0].multi_value_list);
      } else {
        (yyval.multi_value_list) = new std::vector<std::vector<Value>>;
      }
      (yyval.multi_value_list)->emplace_back(*(yyvsp[-1].value_list));
      delete (yyvsp[-1].value_list);
    }
#line 2271 "yacc_sql.cpp"
    break;

  case 63: /* value_list: LBRACE value value_list_body RBRACE  */
#line 544 "yacc_sql.y"
    {
      if ((yyvsp[-1].value_list_body) != nullptr) {
        (yyval.value_list) = (yyvsp[-1].value_list_body);
      } else {
        (yyval.value_list) = new std::vector<Value>;
      }
      (yyval.value_list)->emplace_back(*(yyvsp[-2].value));
      std::reverse((yyval.value_list)->begin(), (yyval.value_list)->end());
      delete (yyvsp[-2].value);
    }
#line 2286 "yacc_sql.cpp"
    break;

  case 64: /* value_list_body: %empty  */
#line 558 "yacc_sql.y"
    {
      (yyval.value_list_body) = nullptr;
    }
#line 2294 "yacc_sql.cpp"
    break;

  case 65: /* value_list_body: COMMA value value_list_body  */
#line 562 "yacc_sql.y"
    {
      if ((yyvsp[0].value_list_body) != nullptr) {
        (yyval.value_list_body) = (yyvsp[0].value_list_body);
      } else {
        (yyval.value_list_body) = new std::vector<Value>;
      }
      (yyval.value_list_body)->emplace_back(*(yyvsp[-1].value));
      delete (yyvsp[-1].value);
    }
#line 2308 "yacc_sql.cpp"
    break;

  case 66: /* value: NUMBER  */
#line 574 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2317 "yacc_sql.cpp"
    break;

  case 67: /* value: '-' NUMBER  */
#line 577 "yacc_sql.y"
                   {
      (yyval.value) = new Value(-(int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2326 "yacc_sql.cpp"
    break;

  case 68: /* value: FLOAT  */
#line 580 "yacc_sql.y"
              {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2335 "yacc_sql.cpp"
    break;

  case 69: /* value: '-' FLOAT  */
#line 583 "yacc_sql.y"
                  {
      (yyval.value) = new Value(-(float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2344 "yacc_sql.cpp"
    break;

  case 70: /* value: SSS  */
#line 586 "yacc_sql.y"
            {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
    }
#line 2354 "yacc_sql.cpp"
    break;

  case 71: /* value: DATE_STR  */
#line 590 "yacc_sql.y"
                 {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(DATES, tmp, 4, true);
      free(tmp);
    }
#line 2364 "yacc_sql.cpp"
    break;

  case 72: /* value: NULL_T  */
#line 594 "yacc_sql.y"
               {
      (yyval.value) = new Value(0);
      (yyval.value)->set_null();
      (yyloc) = (yylsp[0]);
    }
#line 2374 "yacc_sql.cpp"
    break;

  case 73: /* delete_stmt: DELETE FROM ID where_conditions  */
#line 603 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->deletion.conditions.swap((yyvsp[0].condition_list)->conditions);
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2388 "yacc_sql.cpp"
    break;

  case 74: /* update_stmt: UPDATE ID SET update_def update_def_list where_conditions  */
#line 616 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      (yyval.sql_node)->update.relation_name = (yyvsp[-4].string);
      if ((yyvsp[-1].update_infos) != nullptr) {
        (yyval.sql_node)->update.update_units.swap(*(yyvsp[-1].update_infos));
        delete (yyvsp[-1].update_infos);
      }
      (yyval.sql_node)->update.update_units.emplace_back(*(yyvsp[-2].update_info));
      std::reverse((yyval.sql_node)->update.update_units.begin(), (yyval.sql_node)->update.update_units.end());
      delete (yyvsp[-2].update_info);

      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->update.conditions.swap((yyvsp[0].condition_list)->conditions);
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-4].string));
    }
#line 2410 "yacc_sql.cpp"
    break;

  case 75: /* update_def_list: %empty  */
#line 637 "yacc_sql.y"
    {
      (yyval.update_infos) = nullptr;
    }
#line 2418 "yacc_sql.cpp"
    break;

  case 76: /* update_def_list: COMMA update_def update_def_list  */
#line 641 "yacc_sql.y"
    {
      if ((yyvsp[0].update_infos) != nullptr) {
        (yyval.update_infos) = (yyvsp[0].update_infos);
      } else {
        (yyval.update_infos) = new std::vector<UpdateUnit>;
      }
      (yyval.update_infos)->emplace_back(*(yyvsp[-1].update_info));
      delete (yyvsp[-1].update_info);
    }
#line 2432 "yacc_sql.cpp"
    break;

  case 77: /* update_def: ID EQ add_expr  */
#line 654 "yacc_sql.y"
    {
      (yyval.update_info) = new UpdateUnit;
      (yyval.update_info)->attribute_name = (yyvsp[-2].string);
      (yyval.update_info)->value = (yyvsp[0].expression);
      free((yyvsp[-2].string));
    }
#line 2443 "yacc_sql.cpp"
    break;

  case 78: /* select_stmt: SELECT select_attr FROM relation_list join_list where_conditions opt_group_by opt_having opt_order_by  */
#line 663 "yacc_sql.y"
                                                                                                          {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);

      (yyval.sql_node)->selection.attributes.swap(*(yyvsp[-7].expression_list));
      delete (yyvsp[-7].expression_list);

      (yyval.sql_node)->selection.relations.swap(*(yyvsp[-5].relation_list));
      std::reverse((yyval.sql_node)->selection.relations.begin(), (yyval.sql_node)->selection.relations.end());
      delete (yyvsp[-5].relation_list);

      if ((yyvsp[-4].join_list) != nullptr) {
        (yyval.sql_node)->selection.join_lists.swap(*(yyvsp[-4].join_list));
        std::reverse((yyval.sql_node)->selection.join_lists.begin(), (yyval.sql_node)->selection.join_lists.end());
        for (const auto& join_list : (yyval.sql_node)->selection.join_lists) {
          (yyval.sql_node)->selection.relations.emplace_back(join_list.relation);
        }
        delete (yyvsp[-4].join_list);
      }
      if ((yyvsp[-3].condition_list) != nullptr) {
        (yyval.sql_node)->selection.where_conditions.type = (yyvsp[-3].condition_list)->type;
        (yyval.sql_node)->selection.where_conditions.conditions.swap((yyvsp[-3].condition_list)->conditions);
        delete (yyvsp[-3].condition_list);
      }
      if ((yyvsp[-2].rel_attr_list) != nullptr) {
      	(yyval.sql_node)->selection.group_by_attributes.swap(*(yyvsp[-2].rel_attr_list));
	std::reverse((yyval.sql_node)->selection.group_by_attributes.begin(), (yyval.sql_node)->selection.group_by_attributes.end());
	delete (yyvsp[-2].rel_attr_list);
      }
      if ((yyvsp[-1].condition_list) != nullptr) {
	(yyval.sql_node)->selection.having_conditions.type = (yyvsp[-1].condition_list)->type;
	(yyval.sql_node)->selection.having_conditions.conditions.swap((yyvsp[-1].condition_list)->conditions);
	delete (yyvsp[-1].condition_list);
      }
      if ((yyvsp[0].order_infos) != nullptr) {
        (yyval.sql_node)->selection.order_lists.swap(*(yyvsp[0].order_infos));
        std::reverse((yyval.sql_node)->selection.order_lists.begin(), (yyval.sql_node)->selection.order_lists.end());
        delete (yyvsp[0].order_infos);
      }
    }
#line 2487 "yacc_sql.cpp"
    break;

  case 79: /* opt_group_by: %empty  */
#line 705 "yacc_sql.y"
                {
      (yyval.rel_attr_list) = nullptr;

    }
#line 2496 "yacc_sql.cpp"
    break;

  case 80: /* opt_group_by: GROUP BY rel_attr_list  */
#line 708 "yacc_sql.y"
                               {
      (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
    }
#line 2504 "yacc_sql.cpp"
    break;

  case 81: /* opt_having: %empty  */
#line 713 "yacc_sql.y"
                {
      (yyval.condition_list) = nullptr;

    }
#line 2513 "yacc_sql.cpp"
    break;

  case 82: /* opt_having: HAVING condition_list  */
#line 716 "yacc_sql.y"
                              {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 2521 "yacc_sql.cpp"
    break;

  case 83: /* opt_order_by: %empty  */
#line 723 "yacc_sql.y"
        {
      (yyval.order_infos) = nullptr;
    }
#line 2529 "yacc_sql.cpp"
    break;

  case 84: /* opt_order_by: ORDER BY sort_def_list  */
#line 727 "yacc_sql.y"
        {
      (yyval.order_infos) = (yyvsp[0].order_infos);
	}
#line 2537 "yacc_sql.cpp"
    break;

  case 85: /* sort_def_list: sort_def  */
#line 734 "yacc_sql.y"
        {
      (yyval.order_infos) = new std::vector<OrderByNode>;
      (yyval.order_infos)->emplace_back(*(yyvsp[0].order_info));
	}
#line 2546 "yacc_sql.cpp"
    break;

  case 86: /* sort_def_list: sort_def COMMA sort_def_list  */
#line 739 "yacc_sql.y"
        {
      if ((yyvsp[0].order_infos) != nullptr) {
        (yyval.order_infos) = (yyvsp[0].order_infos);
      } else {
        (yyval.order_infos) = new std::vector<OrderByNode>;
      }
      (yyval.order_infos)->emplace_back(*(yyvsp[-2].order_info));
	}
#line 2559 "yacc_sql.cpp"
    break;

  case 87: /* sort_def: rel_attr  */
#line 751 "yacc_sql.y"
    {
      (yyval.order_info) = new OrderByNode;
      (yyval.order_info)->sort_attr = *(yyvsp[0].rel_attr);
      delete((yyvsp[0].rel_attr));
    }
#line 2569 "yacc_sql.cpp"
    break;

  case 88: /* sort_def: rel_attr DESC  */
#line 757 "yacc_sql.y"
    {
      (yyval.order_info) = new OrderByNode;
      (yyval.order_info)->sort_attr = *(yyvsp[-1].rel_attr);
      (yyval.order_info)->is_asc = 0;
      delete((yyvsp[-1].rel_attr));
    }
#line 2580 "yacc_sql.cpp"
    break;

  case 89: /* sort_def: rel_attr ASC  */
#line 764 "yacc_sql.y"
    {
      (yyval.order_info) = new OrderByNode;
      (yyval.order_info)->sort_attr = *(yyvsp[-1].rel_attr);
      delete((yyvsp[-1].rel_attr));
    }
#line 2590 "yacc_sql.cpp"
    break;

  case 90: /* calc_stmt: CALC select_attr  */
#line 773 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      std::reverse((yyvsp[0].expression_list)->begin(), (yyvsp[0].expression_list)->end());
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2601 "yacc_sql.cpp"
    break;

  case 91: /* aggr_expr: aggr_type LBRACE '*' RBRACE  */
#line 782 "yacc_sql.y"
                                {
      RelAttrSqlNode *rel_attr_sql_node = new RelAttrSqlNode;
      rel_attr_sql_node->relation_name = "";
      rel_attr_sql_node->attribute_name = "*";
      RelAttrExpr *relExpr = new RelAttrExpr(*rel_attr_sql_node);
      (yyval.expression) = new AggrExpr((AggrType)(yyvsp[-3].number), relExpr);
    }
#line 2613 "yacc_sql.cpp"
    break;

  case 92: /* aggr_expr: aggr_type LBRACE rel_attr RBRACE  */
#line 788 "yacc_sql.y"
                                         {
      RelAttrExpr *relExpr = new RelAttrExpr(*(yyvsp[-1].rel_attr));
      (yyval.expression) = new AggrExpr((AggrType)(yyvsp[-3].number), relExpr);
    }
#line 2622 "yacc_sql.cpp"
    break;

  case 93: /* aggr_expr: aggr_type LBRACE DATA RBRACE  */
#line 791 "yacc_sql.y"
                                     {
      // These shit is added due to a fucking test case
      RelAttrSqlNode *rel_attr_sql_node = new RelAttrSqlNode;
      rel_attr_sql_node->relation_name = "";
      rel_attr_sql_node->attribute_name = "data";
      RelAttrExpr *relExpr = new RelAttrExpr(*rel_attr_sql_node);
      (yyval.expression) = new AggrExpr((AggrType)(yyvsp[-3].number), relExpr);
    }
#line 2635 "yacc_sql.cpp"
    break;

  case 94: /* base_expr: value  */
#line 802 "yacc_sql.y"
          {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 2645 "yacc_sql.cpp"
    break;

  case 95: /* base_expr: rel_attr  */
#line 806 "yacc_sql.y"
                 {
      (yyval.expression) = new RelAttrExpr(*(yyvsp[0].rel_attr));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].rel_attr);
    }
#line 2655 "yacc_sql.cpp"
    break;

  case 96: /* base_expr: LBRACE add_expr RBRACE  */
#line 810 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2664 "yacc_sql.cpp"
    break;

  case 97: /* base_expr: aggr_expr  */
#line 813 "yacc_sql.y"
                  {
      (yyval.expression) = (yyvsp[0].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2673 "yacc_sql.cpp"
    break;

  case 98: /* base_expr: value_list  */
#line 816 "yacc_sql.y"
                   {
      (yyval.expression) = new ValuesExpr();
      for (auto &value : *(yyvsp[0].value_list)) {
	    ((ValuesExpr*) (yyval.expression))->add_value(value);
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value_list);
    }
#line 2686 "yacc_sql.cpp"
    break;

  case 99: /* mul_expr: base_expr  */
#line 827 "yacc_sql.y"
              {
      (yyval.expression) = (yyvsp[0].expression);
    }
#line 2694 "yacc_sql.cpp"
    break;

  case 100: /* mul_expr: '-' base_expr  */
#line 829 "yacc_sql.y"
                      {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2702 "yacc_sql.cpp"
    break;

  case 101: /* mul_expr: mul_expr '*' base_expr  */
#line 831 "yacc_sql.y"
                               {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2710 "yacc_sql.cpp"
    break;

  case 102: /* mul_expr: mul_expr '/' base_expr  */
#line 833 "yacc_sql.y"
                               {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2718 "yacc_sql.cpp"
    break;

  case 103: /* add_expr: mul_expr  */
#line 839 "yacc_sql.y"
             {
      (yyval.expression) = (yyvsp[0].expression);
    }
#line 2726 "yacc_sql.cpp"
    break;

  case 104: /* add_expr: add_expr '+' mul_expr  */
#line 841 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2734 "yacc_sql.cpp"
    break;

  case 105: /* add_expr: add_expr '-' mul_expr  */
#line 843 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2742 "yacc_sql.cpp"
    break;

  case 106: /* select_attr: '*' expression_list  */
#line 849 "yacc_sql.y"
                        {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      RelAttrSqlNode* relAttrSqlNode = new RelAttrSqlNode;
      relAttrSqlNode->relation_name  = "";
      relAttrSqlNode->attribute_name = "*";
      (yyval.expression_list)->emplace_back(new RelAttrExpr(*relAttrSqlNode));
    }
#line 2758 "yacc_sql.cpp"
    break;

  case 107: /* select_attr: ID DOT '*' expression_list  */
#line 860 "yacc_sql.y"
                                 {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      RelAttrSqlNode* relAttrSqlNode = new RelAttrSqlNode;
      relAttrSqlNode->relation_name  = (yyvsp[-3].string);
      relAttrSqlNode->attribute_name = "*";
      (yyval.expression_list)->emplace_back(new RelAttrExpr(*relAttrSqlNode));
      delete (yyvsp[-3].string);
    }
#line 2775 "yacc_sql.cpp"
    break;

  case 108: /* select_attr: add_expr expression_list  */
#line 871 "yacc_sql.y"
                                 {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      (yyval.expression_list)->emplace_back((yyvsp[-1].expression));
    }
#line 2788 "yacc_sql.cpp"
    break;

  case 109: /* select_attr: add_expr AS ID expression_list  */
#line 878 "yacc_sql.y"
                                       {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      Expression *expr = (yyvsp[-3].expression);
      expr->set_alias((yyvsp[-1].string));
      (yyval.expression_list)->emplace_back(expr);
    }
#line 2803 "yacc_sql.cpp"
    break;

  case 110: /* expression_list: %empty  */
#line 891 "yacc_sql.y"
                {
      (yyval.expression_list) = nullptr;
    }
#line 2811 "yacc_sql.cpp"
    break;

  case 111: /* expression_list: COMMA '*' expression_list  */
#line 893 "yacc_sql.y"
                                  {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      RelAttrSqlNode* relAttrSqlNode = new RelAttrSqlNode;
      relAttrSqlNode->relation_name  = "";
      relAttrSqlNode->attribute_name = "*";
      (yyval.expression_list)->emplace_back(new RelAttrExpr(*relAttrSqlNode));
    }
#line 2827 "yacc_sql.cpp"
    break;

  case 112: /* expression_list: COMMA ID DOT '*' expression_list  */
#line 903 "yacc_sql.y"
                                         {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      RelAttrSqlNode* relAttrSqlNode = new RelAttrSqlNode;
      relAttrSqlNode->relation_name  = (yyvsp[-3].string);
      relAttrSqlNode->attribute_name = "*";
      (yyval.expression_list)->emplace_back(new RelAttrExpr(*relAttrSqlNode));
      delete (yyvsp[-3].string);
    }
#line 2844 "yacc_sql.cpp"
    break;

  case 113: /* expression_list: COMMA add_expr expression_list  */
#line 914 "yacc_sql.y"
                                       {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      (yyval.expression_list)->emplace_back((yyvsp[-1].expression));
    }
#line 2857 "yacc_sql.cpp"
    break;

  case 114: /* expression_list: COMMA add_expr ID expression_list  */
#line 921 "yacc_sql.y"
                                          {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      Expression *expr = (yyvsp[-2].expression);
      expr->set_alias((yyvsp[-1].string));
      (yyval.expression_list)->emplace_back(expr);
    }
#line 2872 "yacc_sql.cpp"
    break;

  case 115: /* expression_list: COMMA add_expr AS ID expression_list  */
#line 930 "yacc_sql.y"
                                             {
      if ((yyvsp[0].expression_list) != nullptr) {
	(yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
	(yyval.expression_list) = new std::vector<Expression *>;
      }
      Expression *expr = (yyvsp[-3].expression);
      expr->set_alias((yyvsp[-1].string));
      (yyval.expression_list)->emplace_back(expr);
    }
#line 2887 "yacc_sql.cpp"
    break;

  case 116: /* expression_list: COMMA add_expr AS DATA expression_list  */
#line 939 "yacc_sql.y"
                                               {
      // These shit is added due to a fucking test case
      if ((yyvsp[0].expression_list) != nullptr) {
      	(yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
      	(yyval.expression_list) = new std::vector<Expression *>;
      }
      Expression *expr = (yyvsp[-3].expression);
      expr->set_alias("data");
      (yyval.expression_list)->emplace_back(expr);
    }
#line 2903 "yacc_sql.cpp"
    break;

  case 117: /* rel_attr: ID  */
#line 953 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name = "";
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      delete (yyvsp[0].string);
    }
#line 2914 "yacc_sql.cpp"
    break;

  case 118: /* rel_attr: ID DOT ID  */
#line 958 "yacc_sql.y"
                  {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      delete (yyvsp[-2].string);
      delete (yyvsp[0].string);
    }
#line 2926 "yacc_sql.cpp"
    break;

  case 119: /* rel_attr_list: rel_attr  */
#line 968 "yacc_sql.y"
             {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[0].rel_attr));
      delete (yyvsp[0].rel_attr);
    }
#line 2936 "yacc_sql.cpp"
    break;

  case 120: /* rel_attr_list: rel_attr COMMA rel_attr_list  */
#line 972 "yacc_sql.y"
                                     {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
	(yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
	(yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-2].rel_attr));
      delete (yyvsp[-2].rel_attr);
    }
#line 2950 "yacc_sql.cpp"
    break;

  case 121: /* relation_list: ID rel_list  */
#line 983 "yacc_sql.y"
                {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<RelationSqlNode>;
      }
      RelationSqlNode *relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = (yyvsp[-1].string);
      relationSqlNode->alias = "";
      (yyval.relation_list)->push_back(*relationSqlNode);
      free((yyvsp[-1].string));
    }
#line 2967 "yacc_sql.cpp"
    break;

  case 122: /* relation_list: ID ID rel_list  */
#line 994 "yacc_sql.y"
                       {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<RelationSqlNode>;
      }
      RelationSqlNode *relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = (yyvsp[-2].string);
      relationSqlNode->alias = (yyvsp[-1].string);
      (yyval.relation_list)->push_back(*relationSqlNode);
      free((yyvsp[-2].string));
      free((yyvsp[-1].string));
    }
#line 2985 "yacc_sql.cpp"
    break;

  case 123: /* relation_list: ID AS ID rel_list  */
#line 1006 "yacc_sql.y"
                          {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<RelationSqlNode>;
      }
      RelationSqlNode *relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = (yyvsp[-3].string);
      relationSqlNode->alias = (yyvsp[-1].string);
      (yyval.relation_list)->push_back(*relationSqlNode);
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
    }
#line 3003 "yacc_sql.cpp"
    break;

  case 124: /* rel_list: %empty  */
#line 1021 "yacc_sql.y"
                {
      (yyval.relation_list) = nullptr;
    }
#line 3011 "yacc_sql.cpp"
    break;

  case 125: /* rel_list: COMMA ID rel_list  */
#line 1023 "yacc_sql.y"
                          {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<RelationSqlNode>;
      }
      RelationSqlNode *relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = (yyvsp[-1].string);
      relationSqlNode->alias = "";
      (yyval.relation_list)->push_back(*relationSqlNode);
      free((yyvsp[-1].string));
    }
#line 3028 "yacc_sql.cpp"
    break;

  case 126: /* rel_list: COMMA ID ID rel_list  */
#line 1034 "yacc_sql.y"
                             {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<RelationSqlNode>;
      }
      RelationSqlNode *relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = (yyvsp[-2].string);
      relationSqlNode->alias = (yyvsp[-1].string);
      (yyval.relation_list)->push_back(*relationSqlNode);
      free((yyvsp[-2].string));
      free((yyvsp[0].relation_list));
    }
#line 3046 "yacc_sql.cpp"
    break;

  case 127: /* rel_list: COMMA ID AS ID rel_list  */
#line 1046 "yacc_sql.y"
                                {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<RelationSqlNode>;
      }
      RelationSqlNode *relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = (yyvsp[-3].string);
      relationSqlNode->alias = (yyvsp[-1].string);
      (yyval.relation_list)->push_back(*relationSqlNode);
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
    }
#line 3064 "yacc_sql.cpp"
    break;

  case 128: /* join_list: %empty  */
#line 1063 "yacc_sql.y"
    {
      (yyval.join_list) = nullptr;
    }
#line 3072 "yacc_sql.cpp"
    break;

  case 129: /* join_list: INNER JOIN ID join_conditions join_list  */
#line 1066 "yacc_sql.y"
                                             {
      if ((yyvsp[0].join_list) != nullptr) {
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new std::vector<JoinSqlNode>;
      }
      RelationSqlNode* relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = (yyvsp[-2].string);
      relationSqlNode->alias = "";
      JoinSqlNode* joinSqlNode = new JoinSqlNode;
      joinSqlNode->relation = *relationSqlNode;
      if ((yyvsp[-1].condition_list) != nullptr) {
        joinSqlNode->join_conditions.swap((yyvsp[-1].condition_list)->conditions);
        std::reverse(joinSqlNode->join_conditions.begin(), joinSqlNode->join_conditions.end());
      }
      delete (yyvsp[-1].condition_list);
      (yyval.join_list)->emplace_back(*joinSqlNode);
      delete joinSqlNode;
      free((yyvsp[-2].string));
    }
#line 3097 "yacc_sql.cpp"
    break;

  case 130: /* join_conditions: %empty  */
#line 1090 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3105 "yacc_sql.cpp"
    break;

  case 131: /* join_conditions: ON condition_list  */
#line 1094 "yacc_sql.y"
        {
	  (yyval.condition_list) = (yyvsp[0].condition_list);
	}
#line 3113 "yacc_sql.cpp"
    break;

  case 132: /* where_conditions: %empty  */
#line 1101 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3121 "yacc_sql.cpp"
    break;

  case 133: /* where_conditions: WHERE condition_list  */
#line 1104 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);  
    }
#line 3129 "yacc_sql.cpp"
    break;

  case 134: /* condition_list: %empty  */
#line 1110 "yacc_sql.y"
                {
      (yyval.condition_list) = nullptr;
    }
#line 3137 "yacc_sql.cpp"
    break;

  case 135: /* condition_list: condition  */
#line 1112 "yacc_sql.y"
                  {
      (yyval.condition_list) = new WhereConditions;
      (yyval.condition_list)->conditions.emplace_back(*(yyvsp[0].condition));
      delete (yyvsp[0].condition);
    }
#line 3147 "yacc_sql.cpp"
    break;

  case 136: /* condition_list: condition AND condition_list  */
#line 1116 "yacc_sql.y"
                                     {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->type = ConjunctionType::AND;
      (yyval.condition_list)->conditions.emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 3158 "yacc_sql.cpp"
    break;

  case 137: /* condition_list: condition OR condition_list  */
#line 1121 "yacc_sql.y"
                                    {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->type = ConjunctionType::OR;
      (yyval.condition_list)->conditions.emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);

    }
#line 3170 "yacc_sql.cpp"
    break;

  case 138: /* condition: add_expr comp_op add_expr  */
#line 1131 "yacc_sql.y"
                              {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[-2].expression);
      (yyval.condition)->right_expr = (yyvsp[0].expression);
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 3181 "yacc_sql.cpp"
    break;

  case 139: /* condition: add_expr IS NULL_T  */
#line 1136 "yacc_sql.y"
                           {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[-2].expression);
      (yyval.condition)->comp = IS_NULL;
    }
#line 3191 "yacc_sql.cpp"
    break;

  case 140: /* condition: add_expr IS NOT_T NULL_T  */
#line 1142 "yacc_sql.y"
                             {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[-3].expression);
      (yyval.condition)->comp = IS_NOT_NULL;
    }
#line 3201 "yacc_sql.cpp"
    break;

  case 141: /* condition: add_expr IN_T add_expr  */
#line 1146 "yacc_sql.y"
                               {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[-2].expression);
      (yyval.condition)->right_expr = (yyvsp[0].expression);
      (yyval.condition)->comp = IN;
    }
#line 3212 "yacc_sql.cpp"
    break;

  case 142: /* condition: add_expr NOT_T IN_T add_expr  */
#line 1151 "yacc_sql.y"
                                     {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[-3].expression);
      (yyval.condition)->right_expr = (yyvsp[0].expression);
      (yyval.condition)->comp = NOT_IN;
    }
#line 3223 "yacc_sql.cpp"
    break;

  case 143: /* condition: EXISTS_T add_expr  */
#line 1157 "yacc_sql.y"
                        {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[0].expression);
      (yyval.condition)->comp = EXISTS;
    }
#line 3233 "yacc_sql.cpp"
    break;

  case 144: /* condition: NOT_T EXISTS_T add_expr  */
#line 1162 "yacc_sql.y"
                              {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[0].expression);
      (yyval.condition)->comp = NOT_EXISTS;
    }
#line 3243 "yacc_sql.cpp"
    break;

  case 145: /* comp_op: EQ  */
#line 1170 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3249 "yacc_sql.cpp"
    break;

  case 146: /* comp_op: LT  */
#line 1171 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3255 "yacc_sql.cpp"
    break;

  case 147: /* comp_op: GT  */
#line 1172 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3261 "yacc_sql.cpp"
    break;

  case 148: /* comp_op: LE  */
#line 1173 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3267 "yacc_sql.cpp"
    break;

  case 149: /* comp_op: GE  */
#line 1174 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3273 "yacc_sql.cpp"
    break;

  case 150: /* comp_op: NE  */
#line 1175 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3279 "yacc_sql.cpp"
    break;

  case 151: /* comp_op: LIKE_T  */
#line 1176 "yacc_sql.y"
             { (yyval.comp) = LIKE_OP; }
#line 3285 "yacc_sql.cpp"
    break;

  case 152: /* comp_op: NOT_T LIKE_T  */
#line 1177 "yacc_sql.y"
                   { (yyval.comp) = NOT_LIKE_OP; }
#line 3291 "yacc_sql.cpp"
    break;

  case 153: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1182 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);
      
      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 3305 "yacc_sql.cpp"
    break;

  case 154: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1195 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3314 "yacc_sql.cpp"
    break;

  case 155: /* set_variable_stmt: SET ID EQ value  */
#line 1203 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3326 "yacc_sql.cpp"
    break;


#line 3330 "yacc_sql.cpp"

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
  *++yylsp = yyloc;

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
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
  yyerror (&yylloc, sql_string, sql_result, scanner, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1215 "yacc_sql.y"


//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
