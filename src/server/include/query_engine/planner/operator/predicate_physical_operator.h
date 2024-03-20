#pragma once

#include <memory>

#include "include/query_engine/structor/expression/expression.h"
#include "physical_operator.h"

class FilterStmt;

class PredicatePhysicalOperator : public PhysicalOperator {
 public:
  PredicatePhysicalOperator(std::unique_ptr<Expression> expr);

  virtual ~PredicatePhysicalOperator() = default;

  PhysicalOperatorType type() const override {
    return PhysicalOperatorType::PREDICATE;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;

 private:
  std::unique_ptr<Expression> expression_;
};
