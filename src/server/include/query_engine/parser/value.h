#pragma once

#include <string>

/**
 * @brief 属性的类型
 * 
 */
enum AttrType
{
  UNDEFINED,
  CHARS,          ///< 字符串类型
  INTS,           ///< 整数类型(4字节)
  DATES,          ///< 日期类型(4字节)
  NULLS,          ///< null类型(1字节)
  FLOATS,         ///< 浮点数类型(4字节)
  BOOLEANS,       ///< boolean类型，当前不是由parser解析出来的，是程序内部使用的
  TEXTS,           ///< 文本类型(最大长度为65535个字节)
};

const char *attr_type_to_string(AttrType type);
AttrType attr_type_from_string(const char *s);

/**
 * @brief 属性的值
 *
 */
class Value
{
public:
  Value() = default;

  Value(AttrType attr_type, char *data, int length = 4, bool isInit = false) : attr_type_(attr_type)
  {
    this->set_data(data, length, isInit);
  }

  explicit Value(int val);
  explicit Value(float val);
  explicit Value(bool val);
  explicit Value(const char *s, int len = 0);
  explicit Value(const char *s, int len, bool force);
  explicit Value(AttrType attrType);

  Value(const Value &other) = default;
  Value &operator=(const Value &other) = default;

  void set_type(AttrType type)
  {
    this->attr_type_ = type;
  }
  void set_data(char *data, int length, bool isInit = false);
  void set_data(const char *data, int length, bool isInit = false)
  {
    this->set_data(const_cast<char *>(data), length, isInit);
  }
  void set_int(int val);
  void set_float(float val);
  void set_boolean(bool val);
  void set_string(const char *s, int len = 0);
  void set_text(const char *s, int len = 0);
  void set_date(int val);
  void set_date_string(const char *s, int len = 0);
  void set_value(const Value &value);

  std::string to_string() const;

  int compare(const Value &other) const;

  const char *data() const;
  int length() const
  {
    return length_;
  }

  AttrType attr_type() const
  {
    return attr_type_;
  }

  bool is_null() const
  {
    return AttrType::NULLS == attr_type_;
  }
  void set_null()
  {
    this->attr_type_ = AttrType::NULLS;
  }

public:
  /**
   * 获取对应的值
   * 如果当前的类型与期望获取的类型不符，就会执行转换操作
   */
  int get_int() const;
  float get_float() const;
  std::string get_string() const;
  bool get_boolean() const;
  std::string get_text() const;

private:
  AttrType attr_type_ = UNDEFINED;
  int length_ = 0;

  union {
    int int_value_;
    float float_value_;
    bool bool_value_;
    int date_value_;
  } num_value_;
  std::string str_value_;
  std::string text_value_;
  bool check_date(int y, int m, int d);
};

/**
 * @brief 类型转换，不支持的类型转换会返回true
 */
bool type_cast_not_support(AttrType i, AttrType j);