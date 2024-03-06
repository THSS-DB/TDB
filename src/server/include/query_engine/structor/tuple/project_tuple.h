#pragma once

#include "tuple.h"

/**
 * @brief 从一行数据中，选择部分字段组成的元组，也就是投影操作
 * @ingroup Tuple
 * @details 一般在select语句中使用。
 */
class ProjectTuple : public Tuple
{
public:
  ProjectTuple() = default;

  virtual ~ProjectTuple() {
    for (TupleCellSpec *spec : species_) {
      delete spec;
    }
    species_.clear();
  }

  const TupleType tuple_type() const override { return ProjectTuple_Type; }

  void set_tuple(Tuple *tuple) {
    this->tuple_ = tuple;
  }

  void get_record(std::vector<Record *> &records) const override
  {
    tuple_->get_record(records);
  }

  void set_record(std::vector<Record *> &records) override
  {
    tuple_->set_record(records);
  }

  std::vector<TupleCellSpec *> get_species() const {
    return species_;
  }

  void add_cell_spec(TupleCellSpec *spec) {
    species_.push_back(spec);
  }

  int cell_num() const override {
    return species_.size();
  }

  RC cell_at(int index, Value &cell) const override {
    RC rc = RC::SUCCESS;
    if (tuple_ == nullptr) {
      return RC::INTERNAL;
    }
    rc = species_[index]->expression()->get_value(*tuple_, cell);
    return rc;
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override {
    return tuple_->find_cell(spec, cell);
  }

private:
  std::vector<TupleCellSpec *> species_;
  Tuple *tuple_ = nullptr;
};