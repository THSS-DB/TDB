#pragma once

#include <vector>
#include <memory>

#include "logical_node.h"
#include "include/query_engine/structor/expression/expression.h"
#include "storage/field/field.h"

class ProjectLogicalNode : public LogicalNode
{
public:

  explicit ProjectLogicalNode(const std::vector<Expression *> &exprs) {
    for (auto expr : exprs) {
      expressions_.emplace_back(expr->copy());
    }
  }

  ~ProjectLogicalNode() override = default;

  LogicalNodeType type() const override {
    return LogicalNodeType::PROJECTION;
  }

  const std::vector<std::unique_ptr<Expression>> &expressions() const {
    return expressions_;
  }

  ProjectLogicalNode *copy() {
    std::vector<Expression *> exprs;
    exprs.reserve(expressions_.size());
    for (auto &expr : expressions_) {
      exprs.emplace_back(expr.get());
    }
    auto *res_oper = new ProjectLogicalNode(exprs);
    res_oper->add_child(std::move(children_[0]));
    return res_oper;
  }
};
