#pragma once

#include <memory>
#include <utility>
#include <vector>
#include <string>

#include "common/lang/bitmap.h"
#include "common/log/log.h"
#include "tuple_cell.h"
#include "include/query_engine/parser/value.h"
#include "include/query_engine/structor/expression/field_expression.h"
#include "include/storage_engine/recorder/record.h"

class Table;

/**
 * @defgroup Tuple
 * @brief Tuple 元组，表示一行数据，当前返回客户端时使用
 * @details 
 * tuple是一种可以嵌套的数据结构。
 * 比如select t1.a+t2.b from t1, t2;
 * 需要使用下面的结构表示：
 * @code {.cpp}
 *  Project(t1.a+t2.b)
 *        |
 *      Joined
 *      /     \
 *   Row(t1) Row(t2)
 * @endcode
 * 
 */

enum TupleType
{
  RowTuple_Type,
  ProjectTuple_Type,
  AggrTuple_Type,
  ValueListTuple_Type,
  JoinedTuple_Type,
};

/**
 * @brief The basic schema of tuple containing the field(cell) it has.
 * @ingroup Tuple
 */
class TupleSchema 
{
public:

  void append_cell(const TupleCellSpec &cell)
  {
    cells_.push_back(cell);
  }

  void append_cell(const char *alias)
  {
    append_cell(TupleCellSpec(alias));
  }

  int cell_num() const
  {
    return static_cast<int>(cells_.size());
  }

  const TupleCellSpec &cell_at(int i) const
  {
    return cells_[i];
  }

private:
  std::vector<TupleCellSpec> cells_;
};

/**
 * @brief The abstract description of tuple
 * @ingroup Tuple
 */
class Tuple 
{
public:
  Tuple() = default;
  virtual ~Tuple() = default;

  virtual const TupleType tuple_type() const = 0;

  /**
   * @brief The column number in tuple, which is the same
   * as the size of schema
   */
  virtual int cell_num() const = 0;

  /**
   * @brief Get specified cell by index
   */
  virtual RC cell_at(int index, Value &cell) const = 0;

  /**
   * @brief Get specified cell by spec
   */
  virtual RC find_cell(const TupleCellSpec &spec, Value &cell) const = 0;

  /**
   * @brief get Record
   */
  virtual void get_record(std::vector<Record *> &records) const = 0;

  /**
   * @brief set Record
   */
  virtual void set_record(std::vector<Record *> &records) = 0;

  virtual std::string to_string() const
  {
    std::string str;
    const int cell_num = this->cell_num();
    for (int i = 0; i < cell_num - 1; i++) {
      Value cell;
      cell_at(i, cell);
      str += cell.to_string();
      str += ", ";
    }

    if (cell_num > 0) {
      Value cell;
      cell_at(cell_num - 1, cell);
      str += cell.to_string();
    }
    return str;
  }
};





