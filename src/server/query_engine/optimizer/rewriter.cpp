#include "include/query_engine/optimizer/rewriter.h"
#include "include/query_engine/optimizer/expression_rewriter.h"
#include "include/query_engine/optimizer/predicate_rewrite.h"
#include "include/query_engine/optimizer/predicate_pushdown_rewriter.h"
#include "include/query_engine/planner/node/logical_node.h"

Rewriter::Rewriter()
{
  rewrite_rules_.emplace_back(new PredicatePushdownRewriter);
}

RC Rewriter::rewrite(std::unique_ptr<LogicalNode> &oper, bool &change_made)
{
  RC rc = RC::SUCCESS;

  change_made = false;
  for (std::unique_ptr<RewriteRule> &rule : rewrite_rules_) {
    bool sub_change_made = false;
    rc = rule->rewrite(oper, sub_change_made);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to rewrite logical operator. rc=%s", strrc(rc));
      return rc;
    }

    if (sub_change_made && !change_made) {
      change_made = true;
    }
  }

  if (rc != RC::SUCCESS) {
    return rc;
  }

  std::vector<std::unique_ptr<LogicalNode>> &child_opers = oper->children();
  for (auto &child_oper : child_opers) {
    bool sub_change_made = false;
    rc = this->rewrite(child_oper, sub_change_made);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to rewrite child oper. rc=%s", strrc(rc));
      return rc;
    }

    if (sub_change_made && !change_made) {
      change_made = true;
    }
  }
  return rc;
}
