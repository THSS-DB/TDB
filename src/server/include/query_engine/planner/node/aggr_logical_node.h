#pragma once

#include <memory>
#include <vector>

#include "include/storage_engine/recorder/field.h"
#include "logical_node.h"

class AggrExpr;

class AggrLogicalNode : public LogicalNode {
 public:
  explicit AggrLogicalNode(const std::vector<AggrExpr *> &aggr_expressions);

  ~AggrLogicalNode() override = default;

  LogicalNodeType type() const override { return LogicalNodeType::AGGR; }

  const std::vector<std::unique_ptr<Expression>> &expressions() const {
    return expressions_;
  }

  const std::vector<std::string> _alias_() const { return alias_; }
  const std::vector<AggrType> _aggr_types_() const { return aggr_types_; }
  const std::vector<Field> _aggr_fields_() const { return aggr_fields_; }

 protected:
  std::vector<std::string> alias_;
  std::vector<AggrType> aggr_types_;
  std::vector<Field> aggr_fields_;
};
