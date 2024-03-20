#include "include/query_engine/parser/parser.h"

CalcSqlNode::~CalcSqlNode() {
  for (Expression *expr : expressions) {
    delete expr;
  }
  expressions.clear();
}

ParsedSqlNode::ParsedSqlNode() : flag(SCF_ERROR) {}

ParsedSqlNode::ParsedSqlNode(SqlCommandFlag _flag) : flag(_flag) {}

void ParsedSqlResult::add_sql_node(std::unique_ptr<ParsedSqlNode> sql_node) {
  sql_nodes_.emplace_back(std::move(sql_node));
}

int sql_parse(const char *st, ParsedSqlResult *sql_result);

RC Parser::parse(QueryInfo *query_info) {
  RC rc = RC::SUCCESS;

  SqlResult *sql_result = query_info->session_event()->sql_result();
  const std::string &sql = query_info->sql();

  ParsedSqlResult parsed_sql_result;

  sql_parse(sql.c_str(), &parsed_sql_result);
  if (parsed_sql_result.sql_nodes().empty()) {
    sql_result->set_return_code(RC::SUCCESS);
    sql_result->set_state_string("");
    return RC::INTERNAL;
  }

  if (parsed_sql_result.sql_nodes().size() > 1) {
    LOG_WARN("got multi sql commands but only 1 will be handled");
  }

  std::unique_ptr<ParsedSqlNode> sql_node =
      std::move(parsed_sql_result.sql_nodes().front());
  if (sql_node->flag == SCF_ERROR) {
    // set error information to event
    rc = RC::SQL_SYNTAX;
    sql_result->set_return_code(rc);
    sql_result->set_state_string("");
    return rc;
  }

  if (sql_node->flag == SCF_CREATE_TABLE) {
    for (int i = 0; i < sql_node->create_table.attr_infos.size(); i++) {
      if (sql_node->create_table.attr_infos[i].type == TEXTS) {
        sql_node->create_table.attr_infos[i].length =
            MAX_TEXT_VALUE_LENGTH_IN_RECORD;
      }
    }
  }

  if (sql_node->flag == SCF_INSERT) {
    for (const auto &values : sql_node->insertion.multi_values) {
      for (const auto &value : values) {
        if (value.attr_type() == DATES && value.get_int() <= 0) {
          // check invalid date data
          rc = RC::INVALID_ARGUMENT;
          sql_result->set_return_code(rc);
          sql_result->set_state_string("");
          return rc;
        }
      }
    }
  }

  if (sql_node->flag == SCF_SELECT) {
    for (const auto &conditionSqlNode :
         sql_node->selection.where_conditions.conditions) {
      if (conditionSqlNode.left_expr->type() == ExprType::VALUE) {
        auto *left_expr = dynamic_cast<ValueExpr *>(conditionSqlNode.left_expr);
        if (left_expr->value_type() == DATES) {
          Value value;
          left_expr->get_value(value);
          if (value.get_int() <= 0) {
            rc = RC::INVALID_ARGUMENT;
            sql_result->set_return_code(rc);
            sql_result->set_state_string("");
            return rc;
          }
        }
      }
      if (conditionSqlNode.right_expr != nullptr &&
          conditionSqlNode.right_expr->type() == ExprType::VALUE) {
        auto *right_expr =
            dynamic_cast<ValueExpr *>(conditionSqlNode.right_expr);
        if (right_expr->value_type() == DATES) {
          Value value;
          right_expr->get_value(value);
          if (value.get_int() <= 0) {
            rc = RC::INVALID_ARGUMENT;
            sql_result->set_return_code(rc);
            sql_result->set_state_string("");
            return rc;
          }
        }
      }
    }

    // inner join
  }

  query_info->set_sql_node(std::move(sql_node));

  return RC::SUCCESS;
}