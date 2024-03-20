#pragma once

/**
 * @brief 常量值组成的Tuple，主要用于在explain中传递一行字符串
 * @ingroup Tuple
 */
class ValueListTuple : public Tuple {
 public:
  ValueListTuple() = default;
  virtual ~ValueListTuple() = default;

  const TupleType tuple_type() const override { return ValueListTuple_Type; }

  void set_cells(const std::vector<Value> &cells) { cells_ = cells; }

  void get_record(std::vector<Record *> &records) const override {
    // unimplemented
  }

  void set_record(std::vector<Record *> &records) override {
    // unimplemented
  }

  virtual int cell_num() const override {
    return static_cast<int>(cells_.size());
  }

  virtual RC cell_at(int index, Value &cell) const override {
    if (index < 0 || index >= cell_num()) {
      return RC::NOTFOUND;
    }

    cell = cells_[index];
    return RC::SUCCESS;
  }

  virtual RC find_cell(const TupleCellSpec &spec, Value &cell) const override {
    return RC::INTERNAL;
  }

 private:
  std::vector<Value> cells_;
};
