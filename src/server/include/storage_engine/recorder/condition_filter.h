#pragma once

#include <stddef.h>
#include <math.h>
#include "include/common/rc.h"
#include "include/query_engine/parser/value.h"
#include "include/query_engine/parser/parse_defs.h"
#include "include/storage_engine/recorder/table.h"
#include "common/log/log.h"

struct ConDesc 
{
  bool is_attr;     // 是否属性，false 表示是值
  int attr_length;  // 如果是属性，表示属性值长度
  int attr_offset;  // 如果是属性，表示在记录中的偏移量
  Value value;      // 如果是值类型，这里记录值的数据
};

class ConditionFilter 
{
public:
  virtual ~ConditionFilter();

  /**
   * Filter one record
   * @param rec
   * @return true means match condition, false means failed to match.
   */
  virtual bool filter(const Record &rec) const = 0;
};

class DefaultConditionFilter : public ConditionFilter 
{
public:
  DefaultConditionFilter();
  virtual ~DefaultConditionFilter();

  RC init(const ConDesc &left, const ConDesc &right, AttrType attr_type, CompOp comp_op);
  RC init(Table &table, const ConditionSqlNode &condition);

  virtual bool filter(const Record &rec) const;

public:
  const ConDesc &left() const
  {
    return left_;
  }

  const ConDesc &right() const
  {
    return right_;
  }

  CompOp comp_op() const
  {
    return comp_op_;
  }

  AttrType attr_type() const
  {
    return attr_type_;
  }

private:
  ConDesc left_;
  ConDesc right_;
  AttrType attr_type_ = UNDEFINED;
  CompOp comp_op_ = NO_OP;
};

class CompositeConditionFilter : public ConditionFilter 
{
public:
  CompositeConditionFilter() = default;
  virtual ~CompositeConditionFilter();

  RC init(const ConditionFilter *filters[], int filter_num);
  RC init(Table &table, const ConditionSqlNode *conditions, int condition_num);
  virtual bool filter(const Record &rec) const;

public:
  int filter_num() const
  {
    return filter_num_;
  }
  const ConditionFilter &filter(int index) const
  {
    return *filters_[index];
  }

private:
  RC init(const ConditionFilter *filters[], int filter_num, bool own_memory);

private:
  const ConditionFilter **filters_ = nullptr;
  int filter_num_ = 0;
  bool memory_owner_ = false;  // filters_的内存是否由自己来控制
};
