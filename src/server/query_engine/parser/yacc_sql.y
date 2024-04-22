
%{

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

%}

%define api.pure full
%define parse.error verbose
/** 启用位置标识 **/
%locations
%lex-param { yyscan_t scanner }
/** 这些定义了在yyparse函数中的参数 **/
%parse-param { const char * sql_string }
%parse-param { ParsedSqlResult * sql_result }
%parse-param { void * scanner }

//标识tokens
%token  SEMICOLON
        CREATE
        DROP
        VIEW
        TABLE
        TABLES
        INDEX
        UNIQUE
        CALC
        SELECT
        ASC
        DESC
        ORDER
        BY
        IS
        NULL_T
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        INT_T
        STRING_T
        FLOAT_T
        DATE_T
        TEXT_T
        NOT_T
        LIKE_T
        COUNT_T
        MIN_T
        MAX_T
        AVG_T
        SUM_T
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        WHERE
        AND
        OR
        SET
        INNER
        JOIN
        ON
        LOAD
        DATA
        INFILE
        EXPLAIN
        GROUP
        HAVING
        AS
        IN_T
        EXISTS_T
        EQ
        LT
        GT
        LE
        GE
        NE

/** union 中定义各种数据类型，真实生成的代码也是union类型，所以不能有非POD类型的数据 **/
%union {
  ParsedSqlNode *                   sql_node;
  ConditionSqlNode *                condition;
  Value *                           value;
  enum CompOp                       comp;
  RelAttrSqlNode *                  rel_attr;
  std::vector<RelAttrSqlNode> *     rel_attr_list;
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
}

%token <number> NUMBER
%token <floats> FLOAT
%token <string> ID
%token <string> SSS
%token <string> DATE_STR
//非终结符

/** type 定义了各种解析后的结果输出的是什么类型。类型对应了 union 中的定义的成员变量名称 **/
%type <number>              type
%type <number>              aggr_type
%type <condition>           condition
%type <value>               value
%type <number>              number
%type <comp>                comp_op
%type <rel_attr>            rel_attr
%type <rel_attr_list>	    rel_attr_list
%type <rel_attr_list>	    opt_group_by
%type <attr_infos>          attr_def_list
%type <attr_info>           attr_def
%type <update_infos>        update_def_list
%type <update_info>         update_def
%type <order_infos>         opt_order_by
%type <order_infos>         sort_def_list
%type <order_info>          sort_def
%type <value_list>          value_list
%type <value_list_body>     value_list_body
%type <multi_value_list>    multi_value_list
%type <multi_attribute_names>  multi_attribute_names
%type <condition_list>	    opt_having
%type <condition_list>      where_conditions
%type <condition_list>      join_conditions
%type <join_list>           join_list
%type <condition_list>      condition_list
%type <expression_list>     select_attr
%type <relation_list>       rel_list
%type <relation_list>       relation_list
%type <expression>	    aggr_expr
%type <expression>          base_expr
%type <expression>          mul_expr
%type <expression>	    add_expr
%type <expression_list>     expression_list
%type <sql_node>            calc_stmt
%type <sql_node>            select_stmt
%type <sql_node>            insert_stmt
%type <sql_node>            update_stmt
%type <sql_node>            delete_stmt
%type <sql_node>            create_table_stmt
%type <sql_node>            create_view_stmt  
%type <sql_node>            drop_table_stmt
%type <sql_node>            show_tables_stmt
%type <sql_node>            desc_table_stmt
%type <sql_node>            create_index_stmt
%type <sql_node>            drop_index_stmt
%type <sql_node>            sync_stmt
%type <sql_node>            begin_stmt
%type <sql_node>            commit_stmt
%type <sql_node>            rollback_stmt
%type <sql_node>            load_data_stmt
%type <sql_node>            explain_stmt
%type <sql_node>            set_variable_stmt
%type <sql_node>            help_stmt
%type <sql_node>            exit_stmt
%type <sql_node>            command_wrapper
// commands should be a list but I use a single command instead
%type <sql_node>            commands

%left '+' '-'
%left '*' '/'
// %nonassoc UMINUS
%%

commands: command_wrapper opt_semicolon  //commands or sqls. parser starts here.
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>($1);
    sql_result->add_sql_node(std::move(sql_node));
  }
  ;

command_wrapper:
    calc_stmt
  | select_stmt
  | insert_stmt
  | update_stmt
  | delete_stmt
  | create_table_stmt
  | create_view_stmt
  | drop_table_stmt
  | show_tables_stmt
  | desc_table_stmt
  | create_index_stmt
  | drop_index_stmt
  | sync_stmt
  | begin_stmt
  | commit_stmt
  | rollback_stmt
  | load_data_stmt
  | explain_stmt
  | set_variable_stmt
  | help_stmt
  | exit_stmt
    ;

exit_stmt:
    EXIT {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      $$ = new ParsedSqlNode(SCF_EXIT);
    };

help_stmt:
    HELP {
      $$ = new ParsedSqlNode(SCF_HELP);
    };

sync_stmt:
    SYNC {
      $$ = new ParsedSqlNode(SCF_SYNC);
    }
    ;

begin_stmt:
    TRX_BEGIN  {
      $$ = new ParsedSqlNode(SCF_BEGIN);
    }
    ;

commit_stmt:
    TRX_COMMIT {
      $$ = new ParsedSqlNode(SCF_COMMIT);
    }
    ;

rollback_stmt:
    TRX_ROLLBACK  {
      $$ = new ParsedSqlNode(SCF_ROLLBACK);
    }
    ;

drop_table_stmt:    /*drop table 语句的语法解析树*/
    DROP TABLE ID {
      $$ = new ParsedSqlNode(SCF_DROP_TABLE);
      $$->drop_table.relation_name = $3;
      free($3);
    };

show_tables_stmt:
    SHOW TABLES {
      $$ = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
    ;

desc_table_stmt:
    DESC ID  {
	$$ = new ParsedSqlNode(SCF_DESC_TABLE);
	$$->desc_table.relation_name = $2;
	free($2);
    }
    ;

create_index_stmt:    /*create index 语句的语法解析树*/
  CREATE UNIQUE INDEX ID ON ID LBRACE ID multi_attribute_names RBRACE
  {
	$$ = new ParsedSqlNode(SCF_CREATE_INDEX);
	CreateIndexSqlNode &create_index = $$->create_index;
	create_index.index_name = $4;
	create_index.relation_name = $6;
	create_index.is_unique_ = true;
	if ($9 != nullptr) {
	create_index.multi_attribute_names.swap(*$9);
	}
	create_index.multi_attribute_names.emplace_back($8);
	std::reverse(create_index.multi_attribute_names.begin(), create_index.multi_attribute_names.end());
	free($4);
	free($6);
	free($8);
  }
  | CREATE INDEX ID ON ID LBRACE ID multi_attribute_names RBRACE
  {
	$$ = new ParsedSqlNode(SCF_CREATE_INDEX);
	CreateIndexSqlNode &create_index = $$->create_index;
	create_index.index_name = $3;
	create_index.relation_name = $5;
	create_index.is_unique_ = false;
	if ($8 != nullptr) {
	create_index.multi_attribute_names.swap(*$8);
	}
	create_index.multi_attribute_names.emplace_back($7);
	std::reverse(create_index.multi_attribute_names.begin(), create_index.multi_attribute_names.end());
	free($3);
	free($5);
	free($7);
  }
  ;

multi_attribute_names:
  /* empty */
  {
	$$ = nullptr;
  }
  | COMMA ID multi_attribute_names  {
	if ($3 != nullptr) {
		$$ = $3;
	} else {
		$$ = new std::vector<std::string>;
	}
	$$->emplace_back($2);
	delete $2;
  }
  ;

drop_index_stmt:      /*drop index 语句的语法解析树*/
    DROP INDEX ID ON ID
    {
      $$ = new ParsedSqlNode(SCF_DROP_INDEX);
      $$->drop_index.index_name = $3;
      $$->drop_index.relation_name = $5;
      free($3);
      free($5);
    }
    ;

create_table_stmt:    /*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = $$->create_table;
      create_table.relation_name = $3;
      free($3);

      std::vector<AttrInfoSqlNode> *src_attrs = $6;

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*$5);
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete $5;
    }
    ;

create_view_stmt:    /*create view 语句的语法解析树*/
    CREATE VIEW ID AS select_stmt {
      $$ = new ParsedSqlNode(SCF_CREATE_VIEW);
      CreateViewSqlNode &create_view = $$->create_view;
      create_view.view_name = $3;
      create_view.select_sql_node = $5->selection;
      free($3);

    } | CREATE VIEW ID LBRACE rel_attr_list RBRACE AS select_stmt {
      $$ = new ParsedSqlNode(SCF_CREATE_VIEW);
      CreateViewSqlNode &create_view = $$->create_view;
      create_view.view_name = $3;
      create_view.select_sql_node = $8->selection;
      free($3);
    }
    ;

attr_def_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA attr_def attr_def_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<AttrInfoSqlNode>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;

attr_def:
    ID type LBRACE number RBRACE
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = $4;
      $$->nullable = true;
      free($1);
    }
    | ID type
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = 4;
      $$->nullable = true;
      free($1);
    }
    | ID type LBRACE number RBRACE NOT_T NULL_T
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = $4;
      $$->nullable = false;
      free($1);
    }
    | ID type NOT_T NULL_T
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = 4;
      $$->nullable = false;
      free($1);
    }
    | ID type LBRACE number RBRACE NULL_T
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = $4;
      $$->nullable = true;
      free($1);
    }
    | ID type NULL_T
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = 4;
      $$->nullable = true;
      free($1);
    }
    ;

number:
    NUMBER {$$ = $1;}
    ;

type:
    INT_T      { $$=INTS; }
    | STRING_T { $$=CHARS; }
    | FLOAT_T  { $$=FLOATS; }
    | DATE_T   { $$=DATES; }
    | TEXT_T   { $$=TEXTS; }
    ;

// TODO: add your aggr_type here
aggr_type:
    COUNT_T    { $$=AGGR_COUNT; }
    | MIN_T    { $$=AGGR_MIN;   }
    | MAX_T    { $$=AGGR_MAX;   }
    | AVG_T    { $$=AGGR_AVG;   }
    | SUM_T    { $$=AGGR_SUM;   }
    ;

insert_stmt:
    INSERT INTO ID VALUES value_list multi_value_list
    {
      $$ = new ParsedSqlNode(SCF_INSERT);
      $$->insertion.relation_name = $3;
      if ($6 != nullptr) {
        $$->insertion.multi_values.swap(*$6);
      }
      $$->insertion.multi_values.emplace_back(*$5);
      std::reverse($$->insertion.multi_values.begin(), $$->insertion.multi_values.end());
      delete $5;
      free($3);
    }
    ;

multi_value_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA value_list multi_value_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<std::vector<Value>>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;

value_list:
    LBRACE value value_list_body RBRACE
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Value>;
      }
      $$->emplace_back(*$2);
      std::reverse($$->begin(), $$->end());
      delete $2;
    }
    ;

value_list_body:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA value value_list_body
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Value>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;

value:
    NUMBER {
      $$ = new Value((int)$1);
      @$ = @1;
    } | '-' NUMBER {
      $$ = new Value(-(int)$2);
      @$ = @2;
    } | FLOAT {
      $$ = new Value((float)$1);
      @$ = @1;
    } | '-' FLOAT {
      $$ = new Value(-(float)$2);
      @$ = @2;
    } | SSS {
      char *tmp = common::substr($1,1,strlen($1)-2);
      $$ = new Value(tmp);
      free(tmp);
    } | DATE_STR {
      char *tmp = common::substr($1,1,strlen($1)-2);
      $$ = new Value(DATES, tmp, 4, true);
      free(tmp);
    } | NULL_T {
      $$ = new Value(0);
      $$->set_null();
      @$ = @1;
    }
    ;
    
delete_stmt:    /*  delete 语句的语法解析树*/
    DELETE FROM ID where_conditions
    {
      $$ = new ParsedSqlNode(SCF_DELETE);
      $$->deletion.relation_name = $3;
      if ($4 != nullptr) {
        $$->deletion.conditions.swap($4->conditions);
        delete $4;
      }
      free($3);
    }
    ;

update_stmt:      /*  update 语句的语法解析树*/
    UPDATE ID SET update_def update_def_list where_conditions
    {
      $$ = new ParsedSqlNode(SCF_UPDATE);
      $$->update.relation_name = $2;
      if ($5 != nullptr) {
        $$->update.update_units.swap(*$5);
        delete $5;
      }
      $$->update.update_units.emplace_back(*$4);
      std::reverse($$->update.update_units.begin(), $$->update.update_units.end());
      delete $4;

      if ($6 != nullptr) {
        $$->update.conditions.swap($6->conditions);
        delete $6;
      }
      free($2);
    }
    ;

update_def_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA update_def update_def_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<UpdateUnit>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;

update_def:
    ID EQ add_expr
    {
      $$ = new UpdateUnit;
      $$->attribute_name = $1;
      $$->value = $3;
      free($1);
    }
    ;

select_stmt:        /*  select 语句的语法解析树*/
    SELECT select_attr FROM relation_list join_list where_conditions opt_group_by opt_having opt_order_by {
      $$ = new ParsedSqlNode(SCF_SELECT);

      $$->selection.attributes.swap(*$2);
      delete $2;

      $$->selection.relations.swap(*$4);
      std::reverse($$->selection.relations.begin(), $$->selection.relations.end());
      delete $4;

      if ($5 != nullptr) {
        $$->selection.join_lists.swap(*$5);
        for (const auto& join_list : $$->selection.join_lists) {
          $$->selection.relations.emplace_back(join_list.relation);
        }
        std::reverse($$->selection.join_lists.begin(), $$->selection.join_lists.end());
        delete $5;
      }
      if ($6 != nullptr) {
        $$->selection.where_conditions.type = $6->type;
        $$->selection.where_conditions.conditions.swap($6->conditions);
        delete $6;
      }
      if ($7 != nullptr) {
      	$$->selection.group_by_attributes.swap(*$7);
	std::reverse($$->selection.group_by_attributes.begin(), $$->selection.group_by_attributes.end());
	delete $7;
      }
      if ($8 != nullptr) {
	$$->selection.having_conditions.type = $8->type;
	$$->selection.having_conditions.conditions.swap($8->conditions);
	delete $8;
      }
      if ($9 != nullptr) {
        $$->selection.order_lists.swap(*$9);
        std::reverse($$->selection.order_lists.begin(), $$->selection.order_lists.end());
        delete $9;
      }
    }
    ;

opt_group_by:
    /* empty */ {
      $$ = nullptr;

    } | GROUP BY rel_attr_list {
      $$ = $3;
    }

opt_having:
    /* empty */ {
      $$ = nullptr;

    } | HAVING condition_list {
      $$ = $2;
    }
    ;

opt_order_by:
	/* empty */
	{
      $$ = nullptr;
    }
	| ORDER BY sort_def_list
	{
      $$ = $3;
	}
	;

sort_def_list:
    sort_def
	{
      $$ = new std::vector<OrderByNode>;
      $$->emplace_back(*$1);
	}
	| sort_def COMMA sort_def_list
	{
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<OrderByNode>;
      }
      $$->emplace_back(*$1);
	}
	;

sort_def:
    rel_attr
    {
      $$ = new OrderByNode;
      $$->sort_attr = *$1;
      delete($1);
    }
    | rel_attr DESC
    {
      $$ = new OrderByNode;
      $$->sort_attr = *$1;
      $$->is_asc = 0;
      delete($1);
    }
    | rel_attr ASC
    {
      $$ = new OrderByNode;
      $$->sort_attr = *$1;
      delete($1);
    }
    ;

calc_stmt:
    CALC select_attr
    {
      $$ = new ParsedSqlNode(SCF_CALC);
      std::reverse($2->begin(), $2->end());
      $$->calc.expressions.swap(*$2);
      delete $2;
    }
    ;

aggr_expr:
    aggr_type LBRACE '*' RBRACE {
      RelAttrSqlNode *rel_attr_sql_node = new RelAttrSqlNode;
      rel_attr_sql_node->relation_name = "";
      rel_attr_sql_node->attribute_name = "*";
      RelAttrExpr *relExpr = new RelAttrExpr(*rel_attr_sql_node);
      $$ = new AggrExpr((AggrType)$1, relExpr);
    } | aggr_type LBRACE rel_attr RBRACE {
      RelAttrExpr *relExpr = new RelAttrExpr(*$3);
      $$ = new AggrExpr((AggrType)$1, relExpr);
    } | aggr_type LBRACE DATA RBRACE {
      // These shit is added due to a fucking test case
      RelAttrSqlNode *rel_attr_sql_node = new RelAttrSqlNode;
      rel_attr_sql_node->relation_name = "";
      rel_attr_sql_node->attribute_name = "data";
      RelAttrExpr *relExpr = new RelAttrExpr(*rel_attr_sql_node);
      $$ = new AggrExpr((AggrType)$1, relExpr);
    }
    ;

base_expr:
    value {
      $$ = new ValueExpr(*$1);
      $$->set_name(token_name(sql_string, &@$));
      delete $1;
    } | rel_attr {
      $$ = new RelAttrExpr(*$1);
      $$->set_name(token_name(sql_string, &@$));
      delete $1;
    } | LBRACE add_expr RBRACE {
      $$ = $2;
      $$->set_name(token_name(sql_string, &@$));
    } | aggr_expr {
      $$ = $1;
      $$->set_name(token_name(sql_string, &@$));
    } | value_list {
      $$ = new ValuesExpr();
      for (auto &value : *$1) {
	    ((ValuesExpr*) $$)->add_value(value);
      }
      $$->set_name(token_name(sql_string, &@$));
      delete $1;
    }
    ;

mul_expr:
    base_expr {
      $$ = $1;
    } | '-' base_expr {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, $2, nullptr, sql_string, &@$);
    } | mul_expr '*' base_expr {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::MUL, $1, $3, sql_string, &@$);
    } | mul_expr '/' base_expr {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::DIV, $1, $3, sql_string, &@$);
    }
    ;

add_expr:
    mul_expr {
      $$ = $1;
    } | add_expr '+' mul_expr {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::ADD, $1, $3, sql_string, &@$);
    } | add_expr '-' mul_expr {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::SUB, $1, $3, sql_string, &@$);
    }
    ;

select_attr:
    '*' expression_list {
      if ($2 != nullptr) {
        $$ = $2;
      } else {
        $$ = new std::vector<Expression *>;
      }
      RelAttrSqlNode* relAttrSqlNode = new RelAttrSqlNode;
      relAttrSqlNode->relation_name  = "";
      relAttrSqlNode->attribute_name = "*";
      $$->emplace_back(new RelAttrExpr(*relAttrSqlNode));
    } | 
      ID DOT '*' expression_list {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<Expression *>;
      }
      RelAttrSqlNode* relAttrSqlNode = new RelAttrSqlNode;
      relAttrSqlNode->relation_name  = $1;
      relAttrSqlNode->attribute_name = "*";
      $$->emplace_back(new RelAttrExpr(*relAttrSqlNode));
      delete $1;
    } | add_expr expression_list {
      if ($2 != nullptr) {
        $$ = $2;
      } else {
        $$ = new std::vector<Expression *>;
      }
      $$->emplace_back($1);
    } | add_expr AS ID expression_list {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<Expression *>;
      }
      Expression *expr = $1;
      expr->set_alias($3);
      $$->emplace_back(expr);
    }
    ;

expression_list:
    /* empty */ {
      $$ = nullptr;
    } | COMMA '*' expression_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Expression *>;
      }
      RelAttrSqlNode* relAttrSqlNode = new RelAttrSqlNode;
      relAttrSqlNode->relation_name  = "";
      relAttrSqlNode->attribute_name = "*";
      $$->emplace_back(new RelAttrExpr(*relAttrSqlNode));
    } | COMMA ID DOT '*' expression_list {
      if ($5 != nullptr) {
        $$ = $5;
      } else {
        $$ = new std::vector<Expression *>;
      }
      RelAttrSqlNode* relAttrSqlNode = new RelAttrSqlNode;
      relAttrSqlNode->relation_name  = $2;
      relAttrSqlNode->attribute_name = "*";
      $$->emplace_back(new RelAttrExpr(*relAttrSqlNode));
      delete $2;
    } | COMMA add_expr expression_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Expression *>;
      }
      $$->emplace_back($2);
    } | COMMA add_expr ID expression_list {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<Expression *>;
      }
      Expression *expr = $2;
      expr->set_alias($3);
      $$->emplace_back(expr);
    } | COMMA add_expr AS ID expression_list {
      if ($5 != nullptr) {
	$$ = $5;
      } else {
	$$ = new std::vector<Expression *>;
      }
      Expression *expr = $2;
      expr->set_alias($4);
      $$->emplace_back(expr);
    } | COMMA add_expr AS DATA expression_list {
      // These shit is added due to a fucking test case
      if ($5 != nullptr) {
      	$$ = $5;
      } else {
      	$$ = new std::vector<Expression *>;
      }
      Expression *expr = $2;
      expr->set_alias("data");
      $$->emplace_back(expr);
    }
    ;

rel_attr:
    ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name = "";
      $$->attribute_name = $1;
      delete $1;
    } | ID DOT ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      delete $1;
      delete $3;
    }
    ;

rel_attr_list:
    rel_attr {
      $$ = new std::vector<RelAttrSqlNode>;
      $$->emplace_back(*$1);
      delete $1;
    } | rel_attr COMMA rel_attr_list {
      if ($3 != nullptr) {
	$$ = $3;
      } else {
	$$ = new std::vector<RelAttrSqlNode>;
      }
      $$->emplace_back(*$1);
      delete $1;
    }

relation_list:
    ID rel_list {
      if ($2 != nullptr) {
        $$ = $2;
      } else {
        $$ = new std::vector<RelationSqlNode>;
      }
      RelationSqlNode *relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = $1;
      relationSqlNode->alias = "";
      $$->push_back(*relationSqlNode);
      free($1);
    } | ID ID rel_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<RelationSqlNode>;
      }
      RelationSqlNode *relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = $1;
      relationSqlNode->alias = $2;
      $$->push_back(*relationSqlNode);
      free($1);
      free($2);
    } | ID AS ID rel_list {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<RelationSqlNode>;
      }
      RelationSqlNode *relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = $1;
      relationSqlNode->alias = $3;
      $$->push_back(*relationSqlNode);
      free($1);
      free($3);
    }

rel_list:
    /* empty */ {
      $$ = nullptr;
    } | COMMA ID rel_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<RelationSqlNode>;
      }
      RelationSqlNode *relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = $2;
      relationSqlNode->alias = "";
      $$->push_back(*relationSqlNode);
      free($2);
    } | COMMA ID ID rel_list {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<RelationSqlNode>;
      }
      RelationSqlNode *relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = $2;
      relationSqlNode->alias = $3;
      $$->push_back(*relationSqlNode);
      free($2);
      free($4);
    } | COMMA ID AS ID rel_list {
      if ($5 != nullptr) {
        $$ = $5;
      } else {
        $$ = new std::vector<RelationSqlNode>;
      }
      RelationSqlNode *relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = $2;
      relationSqlNode->alias = $4;
      $$->push_back(*relationSqlNode);
      free($2);
      free($4);
    }
    ;

join_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | INNER JOIN ID join_conditions join_list{
      if ($5 != nullptr) {
        $$ = $5;
      } else {
        $$ = new std::vector<JoinSqlNode>;
      }
      RelationSqlNode* relationSqlNode = new RelationSqlNode;
      relationSqlNode->relation_name = $3;
      relationSqlNode->alias = "";
      JoinSqlNode* joinSqlNode = new JoinSqlNode;
      joinSqlNode->relation = *relationSqlNode;
      if ($4 != nullptr) {
        joinSqlNode->join_conditions.swap($4->conditions);
        std::reverse(joinSqlNode->join_conditions.begin(), joinSqlNode->join_conditions.end());
      }
      delete $4;
      $$->emplace_back(*joinSqlNode);
      delete joinSqlNode;
      free($3);
    }
    ;

join_conditions:
	/* empty */
    {
      $$ = nullptr;
    }
	| ON condition_list
	{
	  $$ = $2;
	}
	;

where_conditions:
    /* empty */
    {
      $$ = nullptr;
    }
    | WHERE condition_list {
      $$ = $2;  
    }
    ;

condition_list:
    /* empty */ {
      $$ = nullptr;
    } | condition {
      $$ = new WhereConditions;
      $$->conditions.emplace_back(*$1);
      delete $1;
    } | condition AND condition_list {
      $$ = $3;
      $$->type = ConjunctionType::AND;
      $$->conditions.emplace_back(*$1);
      delete $1;
    } | condition OR condition_list {
      $$ = $3;
      $$->type = ConjunctionType::OR;
      $$->conditions.emplace_back(*$1);
      delete $1;

    }
    ;

condition:
    add_expr comp_op add_expr {
      $$ = new ConditionSqlNode;
      $$->left_expr = $1;
      $$->right_expr = $3;
      $$->comp = $2;
    } | add_expr IS NULL_T {
      $$ = new ConditionSqlNode;
      $$->left_expr = $1;
      $$->comp = IS_NULL;
    }
    | 
    add_expr IS NOT_T NULL_T {
      $$ = new ConditionSqlNode;
      $$->left_expr = $1;
      $$->comp = IS_NOT_NULL;
    } | add_expr IN_T add_expr {
      $$ = new ConditionSqlNode;
      $$->left_expr = $1;
      $$->right_expr = $3;
      $$->comp = IN;
    } | add_expr NOT_T IN_T add_expr {
      $$ = new ConditionSqlNode;
      $$->left_expr = $1;
      $$->right_expr = $4;
      $$->comp = NOT_IN;
    }
    | EXISTS_T add_expr {
      $$ = new ConditionSqlNode;
      $$->left_expr = $2;
      $$->comp = EXISTS;
    }
    | NOT_T EXISTS_T add_expr {
      $$ = new ConditionSqlNode;
      $$->left_expr = $3;
      $$->comp = NOT_EXISTS;
    }
    ;

comp_op:
      EQ { $$ = EQUAL_TO; }
    | LT { $$ = LESS_THAN; }
    | GT { $$ = GREAT_THAN; }
    | LE { $$ = LESS_EQUAL; }
    | GE { $$ = GREAT_EQUAL; }
    | NE { $$ = NOT_EQUAL; }
    | LIKE_T { $$ = LIKE_OP; }
    | NOT_T LIKE_T { $$ = NOT_LIKE_OP; }
    ;

load_data_stmt:
    LOAD DATA INFILE SSS INTO TABLE ID 
    {
      char *tmp_file_name = common::substr($4, 1, strlen($4) - 2);
      
      $$ = new ParsedSqlNode(SCF_LOAD_DATA);
      $$->load_data.relation_name = $7;
      $$->load_data.file_name = tmp_file_name;
      free($7);
      free(tmp_file_name);
    }
    ;

explain_stmt:
    EXPLAIN command_wrapper
    {
      $$ = new ParsedSqlNode(SCF_EXPLAIN);
      $$->explain.sql_node = std::unique_ptr<ParsedSqlNode>($2);
    }
    ;

set_variable_stmt:
    SET ID EQ value
    {
      $$ = new ParsedSqlNode(SCF_SET_VARIABLE);
      $$->set_variable.name  = $2;
      $$->set_variable.value = *$4;
      free($2);
      delete $4;
    }
    ;

opt_semicolon: /*empty*/
    | SEMICOLON
    ;
%%

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
