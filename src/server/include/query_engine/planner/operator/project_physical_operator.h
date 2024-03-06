#pragma once

#include <vector>

#include "physical_operator.h"
#include "include/query_engine/planner/node/project_logical_node.h"
#include "include/query_engine/structor/tuple/project_tuple.h"

/**
 * @brief 选择/投影物理算子
 * @ingroup PhysicalOperator
 */
class ProjectPhysicalOperator : public PhysicalOperator
{
public:
  ProjectPhysicalOperator(ProjectLogicalNode * logical_operator) {
    for (int i = 0; i < logical_operator->expressions().size(); i++) {
      expressions_.emplace_back(logical_operator->expressions()[i].get()->copy());
    }
  }
  ProjectPhysicalOperator(std::vector<std::unique_ptr<Expression>> &expressions) {
    for (int i = 0; i < expressions.size(); i++) {
      expressions_.emplace_back(expressions[i].get()->copy());
    }
  }

  virtual ~ProjectPhysicalOperator() = default;

  void add_projector(const Expression *expr);

  PhysicalOperatorType type() const override {
    return PhysicalOperatorType::PROJECT;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  int cell_num() const {
    return tuple_.cell_num();
  }

  Tuple *current_tuple() override;

  ProjectPhysicalOperator *copy() {
    auto *res_oper = new ProjectPhysicalOperator(expressions_);
    res_oper->add_child(std::move(children_[0]));
    for (TupleCellSpec *spec : tuple_.get_species()) {
      res_oper->add_projector(spec->expression());
    }
    return res_oper;
  }

private:
  ProjectTuple tuple_;
  std::vector<std::unique_ptr<Expression>> expressions_;
};
