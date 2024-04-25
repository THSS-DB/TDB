#pragma once

#include <vector>
#include <memory>
#include <string>

#include "include/common/rc.h"
#include "include/query_engine/structor/tuple/tuple.h"

class Record;
class TupleCellSpec;
class Trx;

enum class PhysicalOperatorType
{
  TABLE_SCAN,
  EXPLAIN,
  PREDICATE,
  PROJECT,
  STRING_LIST,
  DELETE,
  INSERT,
  UPDATE,

  AGGREGATION,
  INDEX_SCAN,
  GROUP_BY,
  ORDER_BY,
  JOIN,
};

class PhysicalOperator
{
public:
  PhysicalOperator() = default;

  virtual ~PhysicalOperator();

  /**
   * 这两个函数是为了explain在打印时使用的，如果要在explain中打印算子信息，
   * 请新建PhysicalOperatorType并修改physical_operator_type_name函数
   */
  virtual std::string name() const;
  virtual std::string param() const;

  virtual PhysicalOperatorType type() const = 0;

  virtual RC open(Trx *trx) = 0;
  virtual RC next() = 0;
  virtual RC close() = 0;

  virtual Tuple *current_tuple() = 0;

  void add_child(std::unique_ptr<PhysicalOperator> oper) {
    children_.emplace_back(std::move(oper));
  }

  std::vector<std::unique_ptr<PhysicalOperator>> &children() {
    return children_;
  }

  void set_father_tuple(const Tuple *father_tuple) {
    father_tuple_ = father_tuple;
    for (auto &child : children_) {
      child->set_father_tuple(father_tuple);
    }
  }

public:
  bool isdelete_ = false;

protected:
  const Tuple *father_tuple_ = nullptr;
  std::vector<std::unique_ptr<PhysicalOperator>> children_;
};
