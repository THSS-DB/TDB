#include <list>

#include "common/lang/bitmap.h"

#include "include/query_engine/planner/logical_plan_generator.h"
#include "include/query_engine/planner/node/logical_node.h"
#include "include/query_engine/planner/node/project_logical_node.h"
#include "include/query_engine/planner/node/group_by_logical_node.h"
#include "include/query_engine/planner/node/order_by_logical_node.h"
#include "include/query_engine/planner/node/predicate_logical_node.h"
#include "include/query_engine/planner/node/table_get_logical_node.h"
#include "include/query_engine/planner/node/insert_logical_node.h"
#include "include/query_engine/planner/node/delete_logical_node.h"
#include "include/query_engine/planner/node/update_logical_node.h"
#include "include/query_engine/planner/node/join_logical_node.h"
#include "include/query_engine/planner/node/aggr_logical_node.h"
#include "include/query_engine/planner/node/explain_logical_node.h"

#include "include/query_engine/analyzer/statement/stmt.h"
#include "include/query_engine/analyzer/statement/select_stmt.h"
#include "include/query_engine/analyzer/statement/filter_stmt.h"
#include "include/query_engine/analyzer/statement/insert_stmt.h"
#include "include/query_engine/analyzer/statement/delete_stmt.h"
#include "include/query_engine/analyzer/statement/update_stmt.h"
#include "include/query_engine/analyzer/statement/explain_stmt.h"
#include "include/query_engine/analyzer/statement/group_by_stmt.h"

#include "include/query_engine/structor/expression/aggregation_expression.h"
#include "include/query_engine/structor/expression/comparison_expression.h"
#include "include/query_engine/structor/expression/conjunction_expression.h"


using namespace std;

RC LogicalPlanGenerator::create(Stmt *stmt, unique_ptr<LogicalNode> &logical_node)
{
  RC rc;
  switch (stmt->type()) {

    case StmtType::SELECT: {
      SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);
      rc = plan_node(select_stmt, logical_node);
    } break;

    case StmtType::INSERT: {
      InsertStmt *insert_stmt = static_cast<InsertStmt *>(stmt);
      rc = plan_node(insert_stmt, logical_node);
    } break;

    case StmtType::DELETE: {
      DeleteStmt *delete_stmt = static_cast<DeleteStmt *>(stmt);
      rc = plan_node(delete_stmt, logical_node);
    } break;

    case StmtType::UPDATE: {
      UpdateStmt *update_stmt = static_cast<UpdateStmt *>(stmt);
      rc = plan_node(update_stmt, logical_node);
    } break;

    case StmtType::EXPLAIN: {
      ExplainStmt *explain_stmt = static_cast<ExplainStmt *>(stmt);
      rc = plan_node(explain_stmt, logical_node);
    } break;

    default: {
      rc = RC::UNIMPLENMENT;
    }
  }
  return rc;
}

unique_ptr<ConjunctionExpr> _transfer_filter_stmt_to_expr(FilterStmt *filter_stmt){
  std::vector<unique_ptr<Expression>> cmp_exprs;
  for (const FilterUnit *filter_unit : filter_stmt->filter_units()) {
    Expression *left_expr = filter_unit->left_expr()->copy();
    Expression *right_expr = nullptr;
    if (filter_unit->right_expr() != nullptr) {
      right_expr = filter_unit->right_expr()->copy();
    }
    auto *cmp_expr = new ComparisonExpr(
        filter_unit->comp(),
        unique_ptr<Expression>(left_expr),
        unique_ptr<Expression>(right_expr));
    cmp_exprs.emplace_back(cmp_expr);
  }
  if (!cmp_exprs.empty()) {
    unique_ptr<ConjunctionExpr> conjunction_expr(new ConjunctionExpr(filter_stmt->get_conjunction_type(), cmp_exprs));
    return conjunction_expr;
  }
  return nullptr;
}

RC LogicalPlanGenerator::plan_node(
    SelectStmt *select_stmt, unique_ptr<LogicalNode> &logical_node)
{
  const std::vector<Table *> &tables     = select_stmt->tables();
  const std::vector<Field *> &all_fields = select_stmt->query_fields();
  RC rc;

  std::unique_ptr<LogicalNode> root;

  // 1. Table scan node
  //TODO [Lab3] 当前只取一个表作为查询表,当引入Join后需要考虑同时查询多个表的情况
  //参考思路: 遍历tables中的所有Table，针对每个table生成TableGetLogicalNode
   Table *default_table = tables[0];
   const char *table_name = default_table->name();
   std::vector<Field> fields;
   for (auto *field : all_fields) {
     if (0 == strcmp(field->table_name(), default_table->name())) {
       fields.push_back(*field);
     }
   }

   root = std::unique_ptr<LogicalNode>(
       new TableGetLogicalNode(default_table, select_stmt->table_alias()[0], fields, true/*readonly*/));

  // 2. inner join node
  // TODO [Lab3] 完善Join节点的逻辑计划生成, 需要解析并设置Join涉及的表,以及Join使用到的连接条件
  // 如果只有一个TableGetLogicalNode,直接将其设置为root节点，跳过该阶段
  // 如果有多个TableGetLogicalNode,则需要生成JoinLogicalNode进行连接
  // 生成JoinLogicalNode可以参考下面的生成流程：
  // * 遍历TableGetLogicalNode
  // * 生成JoinLogicalNode, 通过select_stmt中的join_filter_stmts
  // ps: 需要考虑table数大于2的情况


  // 3. Table filter node
  auto *table_filter_stmt = select_stmt->filter_stmt();
  unique_ptr<LogicalNode> predicate_node;
  plan_node(table_filter_stmt, predicate_node);
  if(predicate_node){
    predicate_node->add_child(std::move(root));
    root = std::move(predicate_node);
  }

  // 4. aggregation node
  std::vector<AggrExpr *> aggr_exprs;
  for (auto *expr : select_stmt->projects()) {
    AggrExpr::getAggrExprs(expr, aggr_exprs);
  }
  if(!aggr_exprs.empty()){
    unique_ptr<LogicalNode> aggr_node = unique_ptr<LogicalNode>(new AggrLogicalNode(aggr_exprs));
    aggr_node->add_child(std::move(root));
    root = std::move(aggr_node);
  }

  // 5. Having filter node
  if (select_stmt->having_stmt() != nullptr) {
    for (auto *filter_unit : select_stmt->having_stmt()->filter_units()) {
      AggrExpr::getAggrExprs(filter_unit->left_expr(), aggr_exprs);
      AggrExpr::getAggrExprs(filter_unit->right_expr(), aggr_exprs);
    }
  }
  if (select_stmt->having_stmt() != nullptr &&
      !select_stmt->having_stmt()->filter_units().empty()) {
    unique_ptr<LogicalNode> having_node;
    plan_node(select_stmt->having_stmt(), having_node);
    having_node->add_child(std::move(root));
    root = std::move(having_node);
  }

  // 6. Sort node
  if (select_stmt->order_stmt() != nullptr && !select_stmt->order_stmt()->order_units().empty()) {
    unique_ptr<LogicalNode> order_node = unique_ptr<LogicalNode>(new OrderByLogicalNode(select_stmt->order_stmt()->order_units()));
    order_node->add_child(std::move(root));
    root = std::move(order_node);
  }

  // 7. project node
  unique_ptr<LogicalNode> project_logical_node =
      unique_ptr<LogicalNode>(new ProjectLogicalNode(select_stmt->projects()));
  project_logical_node->add_child(std::move(root));
  root = std::move(project_logical_node);

  logical_node.swap(root);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::plan_node(
    FilterStmt *filter_stmt, unique_ptr<LogicalNode> &logical_node)
{
  auto conjunction_expr = _transfer_filter_stmt_to_expr(filter_stmt);
  unique_ptr<PredicateLogicalNode> predicate_node;
  if (conjunction_expr != nullptr) {
    predicate_node = std::make_unique<PredicateLogicalNode>(std::move(conjunction_expr));
    logical_node = std::move(predicate_node);
  }
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::plan_node(
    InsertStmt *insert_stmt, unique_ptr<LogicalNode> &logical_node)
{
  Table *table = insert_stmt->table();
  vector<vector<Value>> multi_values;
  for (int i = 0; i < insert_stmt->record_amount(); i++) {
    vector<Value> values(insert_stmt->values(i), insert_stmt->values(i) + insert_stmt->value_amount());
    multi_values.push_back(values);
  }
  InsertLogicalNode *insert_node = new InsertLogicalNode(table, multi_values);
  logical_node.reset(insert_node);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::plan_node(
    DeleteStmt *delete_stmt, unique_ptr<LogicalNode> &logical_node)
{
  Table *table = delete_stmt->table();
  FilterStmt *filter_stmt = delete_stmt->filter_stmt();
  std::vector<Field> fields;
  for (int i = table->table_meta().sys_field_num(); i < table->table_meta().field_num(); i++) {
    const FieldMeta *field_meta = table->table_meta().field(i);
    fields.push_back(Field(table, field_meta));
  }
  unique_ptr<LogicalNode> table_get_node(
      new TableGetLogicalNode(table, table->name(), fields, false/*readonly*/));

  unique_ptr<LogicalNode> predicate_node;
  RC rc = plan_node(filter_stmt, predicate_node);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  unique_ptr<LogicalNode> delete_node(new DeleteLogicalNode(table));

  if (predicate_node) {
    predicate_node->add_child(std::move(table_get_node));
    delete_node->add_child(std::move(predicate_node));
  } else {
    delete_node->add_child(std::move(table_get_node));
  }

  logical_node = std::move(delete_node);
  return rc;
}

RC LogicalPlanGenerator::plan_node(UpdateStmt *update_stmt, unique_ptr<LogicalNode> &logical_node)
{
  Table *table = update_stmt->table();
  FilterStmt *filter_stmt = update_stmt->filter_stmt();
  std::vector<Field> fields;
  for (int i = table->table_meta().sys_field_num(); i < table->table_meta().field_num(); i++) {
    const FieldMeta *field_meta = table->table_meta().field(i);
    fields.push_back(Field(table, field_meta));
  }
  unique_ptr<LogicalNode> table_get_node(new TableGetLogicalNode(table, table->name(), fields, false/*readonly*/));

  unique_ptr<LogicalNode> predicate_node;
  RC rc = plan_node(filter_stmt, predicate_node);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  std::vector<UpdateUnit> update_units;
  for (const UpdateUnit update_unit :update_stmt->update_units()) {
    Expression *expr = update_unit.value->copy();
    UpdateUnit unit;
    unit.value = expr;
    unit.attribute_name = update_unit.attribute_name;
    update_units.emplace_back(unit);
  }

  unique_ptr<LogicalNode> update_node(new UpdateLogicalNode(table, update_units));

  if (predicate_node) {
    predicate_node->add_child(std::move(table_get_node));
    update_node->add_child(std::move(predicate_node));
  } else {
    update_node->add_child(std::move(table_get_node));
  }

  logical_node = std::move(update_node);
  return rc;
}

RC LogicalPlanGenerator::plan_node(
    ExplainStmt *explain_stmt, unique_ptr<LogicalNode> &logical_node)
{
  Stmt *child_stmt = explain_stmt->child();
  unique_ptr<LogicalNode> child_node;
  RC rc = create(child_stmt, child_node);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create explain's child node. rc=%s", strrc(rc));
    return rc;
  }

  logical_node = unique_ptr<LogicalNode>(new ExplainLogicalNode);
  logical_node->add_child(std::move(child_node));
  return rc;
}