#pragma once

#include <stddef.h>
#include <memory>
#include <vector>
#include <string>

#include "value.h"

#define MAX_FIELD_AMOUNT 20
#define MAX_TEXT_VALUE_LENGTH_IN_RECORD 100
#define MIN_TEXT_LENGTH 1024

class Expression;

/**
 * @defgroup SQLParser SQL Parser 
 */

/**
 * @brief 描述一个属性
 * @ingroup SQLParser
 * @details 属性，或者说字段(column, field)
 * Rel -> Relation
 * Attr -> Attribute
 */
struct RelAttrSqlNode {
  std::string relation_name;   ///< relation name (may be NULL) 表名
  std::string attribute_name;  ///< attribute name              属性名
};

struct RelationSqlNode {
  std::string relation_name;
  std::string alias;
};

/**
 * @brief 描述比较运算符
 * @ingroup SQLParser
 */
enum CompOp 
{
  EQUAL_TO,     ///< "="
  LESS_EQUAL,   ///< "<="
  NOT_EQUAL,    ///< "<>"
  LESS_THAN,    ///< "<"
  GREAT_EQUAL,  ///< ">="
  GREAT_THAN,   ///< ">"
  LIKE_OP,      //"like"
  NOT_LIKE_OP,  //"not like"
  IS_NULL,      //"is null"
  IS_NOT_NULL,  //"is not null"
  IN,           //"in"
  NOT_IN,       //"not in"
  EXISTS,        //"exist"
  NOT_EXISTS,    //"not exist"
  NO_OP
};

/**
 * @brief 表示一个条件比较
 * @ingroup SQLParser
 * @details 条件比较就是SQL查询中的 where a>b 这种。
 * 一个条件比较是有两部分组成的，称为左边和右边。
 * 左边和右边理论上都可以是任意的数据，比如是字段（属性，列），也可以是数值常量。
 * 这个结构中记录的仅仅支持字段和值。
 */
struct ConditionSqlNode {
  Expression*     left_expr = nullptr;       ///< left-hand side expression
  CompOp          comp = CompOp::NO_OP;            ///< comparison operator
  Expression*     right_expr = nullptr;      ///< right-hand side expression
};

enum class ConjunctionType {
  AND,
  OR,
};

struct WhereConditions {
  ConjunctionType           type = ConjunctionType::AND;
  std::vector<ConditionSqlNode>   conditions;
};

/**
 * @brief 描述一个 order by 结构
 * @ingroup SQLParser
 * @details 包含一个 sort_attr 和 is_asc
 */
struct OrderByNode
{
  RelAttrSqlNode sort_attr;  // order by attribute
  int is_asc = 1;            // sort type
};

/**
 * @brief 描述一个 join 结构
 * @ingroup SQLParser
 * @details 包含一个 condition 和 relation
 */
struct JoinSqlNode
{
  RelationSqlNode relation; // join 的表
  std::vector<ConditionSqlNode> join_conditions; //join condition
};

enum AggrType
{
  AGGR_COUNT,         ///< count
  AGGR_MIN,           ///< min
  AGGR_MAX,           ///< max
  AGGR_AVG,           ///< avg
  AGGR_SUM,           ///< sum
  AGGR_UNDEFINED     ///< undefined
};

/**
 * @brief 描述一个select语句
 * @ingroup SQLParser
 * @details 一个正常的select语句描述起来比这个要复杂很多，这里做了简化。
 * 一个select语句由三部分组成，分别是select, from, where。
 * select部分表示要查询的字段，from部分表示要查询的表，where部分表示查询的条件。
 * 比如 from 中可以是多个表，也可以是另一个查询语句，这里仅仅支持表，也就是 relations。
 * where 条件 conditions，这里表示使用AND串联起来多个条件。正常的SQL语句会有OR，NOT等，
 * 甚至可以包含复杂的表达式。
 */

struct FuncResult
{
  std::string alia_name_;
  std::string res_value_;
};

enum FunctionType
{
  LENGTH_TYPE,
  ROUND_TYPE,
  DATE_FORMAT_TYPE,
};
struct FunctionUnit
{
  FunctionType function_type;
  Value value;
  int round_precision;
  std::string date_formation;
  std::string alia_name;
};
struct SelectSqlNode
{
  std::vector<Expression *>       attributes;    ///< attributes in select clause
  std::vector<RelationSqlNode>    relations;     ///< 查询的表
  std::vector<JoinSqlNode>        join_lists;    ///< join 列表
  WhereConditions                 where_conditions; ///< 查询条件，使用 AND/OR 串联起来多个条件
  std::vector<RelAttrSqlNode>     group_by_attributes; ///< group by 属性
  WhereConditions                 having_conditions; /// < group by 条件
  std::vector<OrderByNode>        order_lists;   ///< order 列表
  std::vector<FunctionUnit>       functions;     ///< function 列表
};
//struct SelectFunctionNode
//{
//  std::vector<FunctionUnit> functions;
//};

/**
 * @brief 算术表达式计算的语法树
 * @ingroup SQLParser
 */
struct CalcSqlNode
{
  std::vector<Expression *> expressions;  ///< calc clause

  ~CalcSqlNode();
};

/**
 * @brief 描述一个insert语句
 * @ingroup SQLParser
 * @details 于Selects类似，也做了很多简化
 */
struct InsertSqlNode
{
  std::string        relation_name;  ///< Relation to insert into
  std::vector<std::vector<Value>> multi_values;
};

/**
 * @brief 描述一个delete语句
 * @ingroup SQLParser
 */
struct DeleteSqlNode
{
  std::string                   relation_name;  ///< Relation to delete from
  std::vector<ConditionSqlNode> conditions;
};

/**
 * @brief 描述一个 update unit
 * @ingroup SQLParser
 */
struct UpdateUnit
{
  std::string                   attribute_name;
  Expression *                  value = nullptr;
};

/**
 * @brief 描述一个update语句
 * @ingroup SQLParser
 */
struct UpdateSqlNode
{
  std::string                   relation_name;         ///< Relation to update
  std::vector<UpdateUnit>       update_units;          ///< 更新的字段
  std::vector<ConditionSqlNode> conditions;
};

/**
 * @brief 描述一个属性
 * @ingroup SQLParser
 * @details 属性，或者说字段(column, field)
 * Rel -> Relation
 * Attr -> Attribute
 */
struct AttrInfoSqlNode
{
  AttrType    type;       ///< Type of attribute
  std::string name;       ///< Attribute name
  size_t      length;     ///< Length of attribute
  bool        nullable;   ///< nullable of attribute
};

/**
 * @brief 描述一个create table语句
 * @ingroup SQLParser
 * @details 这里也做了很多简化。
 */
struct CreateTableSqlNode
{
  std::string                  relation_name;         ///< Relation name
  std::vector<AttrInfoSqlNode> attr_infos;            ///< attributes
};

struct CreateTableSelectNode
{
  std::string table_name_;
  Expression* select_exp_;
};

struct CreateViewSqlNode {
  std::string                  view_name;             ///< view name
  SelectSqlNode                select_sql_node;       ///< select
};

/**
 * @brief 描述一个drop table语句
 * @ingroup SQLParser
 */
struct DropTableSqlNode
{
  std::string relation_name;  ///< 要删除的表名
};

/**
 * @brief 描述一个create index语句
 * @ingroup SQLParser
 * @details 创建索引时，需要指定索引名，表名，字段名。
 * 正常的SQL语句中，一个索引可能包含了多个字段，这里仅支持一个字段。
 */
struct CreateIndexSqlNode
{
  std::string index_name;      ///< Index name
  std::string relation_name;   ///< Relation name
  std::vector<std::string> multi_attribute_names;
  bool is_unique_;
};

/**
 * @brief 描述一个drop index语句
 * @ingroup SQLParser
 */
struct DropIndexSqlNode
{
  std::string index_name;     ///< Index name
  std::string relation_name;  ///< Relation name
};

/**
 * @brief 描述一个desc table语句
 * @ingroup SQLParser
 * @details desc table 是查询表结构信息的语句
 */
struct DescTableSqlNode
{
  std::string relation_name;
};

/**
 * @brief 描述一个load data语句
 * @ingroup SQLParser
 * @details 从文件导入数据到表中。文件中的每一行就是一条数据，每行的数据类型、字段个数都与表保持一致
 */
struct LoadDataSqlNode
{
  std::string relation_name;
  std::string file_name;
};

/**
 * @brief 设置变量的值
 * @ingroup SQLParser
 * @note 当前还没有查询变量
 */
struct SetVariableSqlNode
{
  std::string name;
  Value       value;
};

class ParsedSqlNode;

/**
 * @brief 描述一个explain语句
 * @ingroup SQLParser
 * @details 会创建operator的语句，才能用explain输出执行计划。
 * 一个command就是一个语句，比如select语句，insert语句等。
 * 可能改成SqlCommand更合适。
 */
struct ExplainSqlNode
{
  std::unique_ptr<ParsedSqlNode> sql_node;
};

/**
 * @brief 解析SQL语句出现了错误
 * @ingroup SQLParser
 * @details 当前解析时并没有处理错误的行号和列号
 */
struct ErrorSqlNode
{
  std::string error_msg;
  int         line;
  int         column;
};

/**
 * @brief 表示一个SQL语句的类型
 * @ingroup SQLParser
 */
enum SqlCommandFlag
{
  SCF_ERROR = 0,
  SCF_CALC,
  SCF_SELECT,
//  SCF_SELECT_FUNCTION,
  SCF_INSERT,
  SCF_UPDATE,
  SCF_DELETE,
  SCF_CREATE_TABLE,
  SCF_CREATE_TABLE_SELECT,
  SCF_CREATE_VIEW,
  SCF_DROP_TABLE,
  SCF_CREATE_INDEX,
  SCF_DROP_INDEX,
  SCF_SYNC,
  SCF_SHOW_TABLES,
  SCF_DESC_TABLE,
  SCF_BEGIN,        ///< 事务开始语句，可以在这里扩展只读事务
  SCF_COMMIT,
  SCF_CLOG_SYNC,
  SCF_ROLLBACK,
  SCF_LOAD_DATA,
  SCF_HELP,
  SCF_EXIT,
  SCF_EXPLAIN,
  SCF_SET_VARIABLE, ///< 设置变量
};
/**
 * @brief 表示一个SQL语句
 * @ingroup SQLParser
 */
class ParsedSqlNode
{
public:
  enum SqlCommandFlag       flag;
  ErrorSqlNode              error;
  CalcSqlNode               calc;
  SelectSqlNode             selection;
  InsertSqlNode             insertion;
  DeleteSqlNode             deletion;
  UpdateSqlNode             update;
  CreateTableSqlNode        create_table;
  CreateTableSelectNode     create_table_select;
  CreateViewSqlNode         create_view;
  DropTableSqlNode          drop_table;
  CreateIndexSqlNode        create_index;
  DropIndexSqlNode          drop_index;
  DescTableSqlNode          desc_table;
  LoadDataSqlNode           load_data;
  ExplainSqlNode            explain;
  SetVariableSqlNode        set_variable;

public:
  ParsedSqlNode();
  explicit ParsedSqlNode(SqlCommandFlag flag);
};

/**
 * @brief 表示语法解析后的数据
 * @ingroup SQLParser
 */
class ParsedSqlResult
{
public:
  void add_sql_node(std::unique_ptr<ParsedSqlNode> sql_node);
  std::vector<std::unique_ptr<ParsedSqlNode>> &sql_nodes()
  {
    return sql_nodes_;
  }

private:
  std::vector<std::unique_ptr<ParsedSqlNode>> sql_nodes_;  ///< 这里记录SQL命令。虽然看起来支持多个，但是当前仅处理一个
};
