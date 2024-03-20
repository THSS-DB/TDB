#pragma once

#include <cmath>
#include <cstring>
#include <memory>
#include <string>
#include <utility>

#include "common/log/log.h"
#include "include/query_engine/parser/value.h"
#include "include/storage_engine/recorder/field.h"

class Tuple;
class SelectStmt;
class ProjectLogicalNode;
class ProjectPhysicalOperator;

extern std::string transform_date_formation(int date, std::string format);

/**
 * @defgroup Expression
 * @brief 表达式
 */

/**
 * @brief 表达式类型
 * @ingroup Expression
 */
enum class ExprType {
  NONE,
  FIELD,  ///< 字段。在实际执行时，根据行数据内容提取对应字段的值
  VALUE,        ///< 常量值
  VALUES,       ///< 很多常量值
  COMPARISON,   ///< 需要做比较的表达式
  CONJUNCTION,  ///< 多个表达式使用同一种关系(AND或OR)来联结
  ARITHMETIC,   ///< 算术运算
  REL_ATTR,     ///< 属性
  AGGR,         ///< 聚合
};

/**
 * @brief 表达式的抽象描述
 * @ingroup Expression
 * @details 在SQL的元素中，任何需要得出值的元素都可以使用表达式来描述
 * 比如获取某个字段的值、比较运算、类型转换
 * 当然还有一些当前没有实现的表达式，比如算术运算。
 *
 * 通常表达式的值，是在真实的算子运算过程中，拿到具体的tuple后
 * 才能计算出来真实的值。但是有些表达式可能就表示某一个固定的
 * 值，比如ValueExpr。
 */
class Expression {
 public:
  Expression() = default;
  virtual ~Expression() = default;

  /**
   * @brief
   * 根据具体的tuple，来计算当前表达式的值。tuple有可能是一个具体某个表的行数据
   */
  virtual RC get_value(const Tuple &tuple, Value &value) const = 0;

  /**
   * @brief
   * 在没有实际运行的情况下，也就是无法获取tuple的情况下，尝试获取表达式的值
   * @details 有些表达式的值是固定的，比如ValueExpr，这种情况下可以直接获取值
   */
  virtual RC try_get_value(Value &value) const { return RC::UNIMPLENMENT; }

  /**
   * @brief 表达式的类型
   * 可以根据表达式类型来转换为具体的子类
   */
  ExprType type() const { return type_; };

  /**
   * @brief 表达式值的类型
   * @details 一个表达式运算出结果后，只有一个值
   */
  virtual AttrType value_type() const = 0;
  virtual bool nullable() { return true; }
  virtual int len() { return -1; }

  /**
   * @brief 表达式的名字，比如是字段名称，或者用户在执行SQL语句时输入的内容
   */
  std::string name() const { return name_; }
  void set_name(std::string name) { name_ = std::move(name); }

  std::string alias() const { return alias_; }
  void set_alias(std::string alias) { alias_ = std::move(alias); }

  virtual void getFields(std::vector<Field *> &query_fields) const {}

  virtual Expression *copy() const { return nullptr; }

 protected:
  ExprType type_ = ExprType::NONE;

 private:
  std::string name_;
  std::string alias_;
};
