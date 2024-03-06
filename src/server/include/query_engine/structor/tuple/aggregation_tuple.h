#pragma once

#include "tuple.h"


class AggrTuple : public Tuple
{
public:
  AggrTuple() {}

  virtual ~AggrTuple() {}

  const TupleType tuple_type() const override { return AggrTuple_Type; }

  void get_record(std::vector<Record *> &records) const override
  {}

  void set_record(std::vector<Record *> &records) override
  {}

  int cell_num() const override
  {
    return aggr_results_.size();
  }

  void set_tuple(std::vector<std::string> &alias, std::vector<Value> &aggr_results) {
    for (auto &alia : alias) {
      alias_.emplace_back(alia);
    }
    aggr_results_ = std::move(aggr_results);
  }

  RC cell_at(int index, Value &cell) const override
  {
    if (index < 0 || index >= static_cast<int>(aggr_results_.size())) {
      return RC::INTERNAL;
    }
    cell = aggr_results_[index];
    return RC::SUCCESS;
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override {
    for (int i = 0; i < alias_.size(); i++) {
      if (strcmp(alias_[i].c_str(), spec.alias()) == 0 || strcmp(alias_[i].c_str(), spec.field_name()) == 0) {
        cell = aggr_results_[i];
        return RC::SUCCESS;
      }
    }
    return RC::NOTFOUND;
  }

private:
  std::vector<std::string> alias_;
  std::vector<Value> aggr_results_;
};
