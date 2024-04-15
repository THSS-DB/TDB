#include <sstream>
#include "include/query_engine/parser/value.h"
#include "common/log/log.h"
#include "common/lang/comparator.h"
#include "common/lang/string.h"
#include <cmath>

const char *ATTR_TYPE_NAME[] = {"undefined", "chars", "ints", "dates","nulls","floats", "booleans", "texts"};

const char *attr_type_to_string(AttrType type)
{
  if (type >= UNDEFINED && type <= TEXTS) {
    return ATTR_TYPE_NAME[type];
  }
  return "unknown";
}
AttrType attr_type_from_string(const char *s)
{
  for (unsigned int i = 0; i < sizeof(ATTR_TYPE_NAME) / sizeof(ATTR_TYPE_NAME[0]); i++) {
    if (0 == strcmp(ATTR_TYPE_NAME[i], s)) {
      return (AttrType)i;
    }
  }
  return UNDEFINED;
}

Value::Value(int val)
{
  set_int(val);
}

Value::Value(float val)
{
  set_float(val);
}

Value::Value(bool val)
{
  set_boolean(val);
}

Value::Value(const char *s, int len /*= 0*/)
{
  set_string(s, len);
}

/**
 * force mean Force Copy from s
*/
Value::Value(const char *s, int len, bool force)
{
  if(!force) {
    set_string(s, len);
  } else {
    //为规避0带来的截断问题，我们先将其均置为1，并记录其位置以将其复原
    char *data_tmp = new char[len];
    std::vector<int> places_of_zeros;
    for(int i = 0; i < len; i++) {
      data_tmp[i] = s[i];
      if(s[i] == 0) {
        places_of_zeros.push_back(i);
        data_tmp[i] = 1;
      }
    }
    set_string(data_tmp, len);
    //复原
    for(int i = 0; i < places_of_zeros.size(); i++) {
      str_value_[places_of_zeros[i]] = 0;
    }
    delete[] data_tmp;
  }
}
Value::Value(AttrType attrType)
{
  switch (attrType) {
    case INTS:{
      set_int(0);
    } break;
    case DATES:{
      set_date(0);
    } break;
    case FLOATS:{
      set_float(0);
    } break;
    case CHARS:{
      set_string("");
    } break;
    case TEXTS:{
      set_text("");
    } break;
    case BOOLEANS:{
      set_boolean(0);
    } break;
    case NULLS:{
      set_null();
    } break;
    default:
      LOG_WARN("unknown data type: %d", attrType);
  }
}

void Value::set_data(char *data, int length, bool isInit)
{
  switch (attr_type_) {
    case TEXTS: {
      set_text(data, length);
    } break;
    case CHARS: {
      set_string(data, length);
    } break;
    case INTS: {
      num_value_.int_value_ = *(int *)data;
      length_ = length;
    } break;
    case FLOATS: {
      num_value_.float_value_ = *(float *)data;
      length_ = length;
    } break;
    case BOOLEANS: {
      num_value_.bool_value_ = *(int *)data != 0;
      length_ = length;
    } break;
    case DATES:{
      if (isInit) {
        set_date_string(data, length);
      } else {
        num_value_.date_value_ = *(int *)data;
        length_ = length;
      }
    } break;
    default: {
      LOG_WARN("unknown data type: %d", attr_type_);
    } break;
  }
}
void Value::set_int(int val)
{
  attr_type_ = INTS;
  num_value_.int_value_ = val;
  length_ = sizeof(val);
}

void Value::set_date(int val)
{
  attr_type_ = DATES;
  num_value_.date_value_ = val;
  length_ = sizeof(val);
}

void Value::set_float(float val)
{
  attr_type_ = FLOATS;
  num_value_.float_value_ = val;
  length_ = sizeof(val);
}
void Value::set_boolean(bool val)
{
  attr_type_ = BOOLEANS;
  num_value_.bool_value_ = val;
  length_ = sizeof(val);
}
void Value::set_string(const char *s, int len /*= 0*/)
{
  attr_type_ = CHARS;
  if (len > 0) {
    len = strnlen(s, len);
    str_value_.assign(s, len);
  } else {
    str_value_.assign(s);
  }
  length_ = str_value_.length();
}
void Value::set_text(const char *s, int len)
{
  attr_type_ = TEXTS;
  if (len > 0) {
    len = strnlen(s, len);
    text_value_.assign(s, len);
  } else {
    text_value_.assign(s);
  }
  length_ = text_value_.length();
}

void Value::set_value(const Value &value)
{
  switch (value.attr_type_) {
    case INTS: {
      set_int(value.get_int());
    } break;
    case FLOATS: {
      set_float(value.get_float());
    } break;
    case TEXTS: {
      set_text(value.get_string().c_str(), value.get_string().size());
    }
    case CHARS: {
      set_string(value.get_string().c_str());
    } break;
    case BOOLEANS: {
      set_boolean(value.get_boolean());
    } break;
    case DATES: {
      set_date(value.get_int());
    } break;
    case UNDEFINED: {
      ASSERT(false, "got an invalid value type");
    } break;
    default:
      LOG_WARN("unknown data type: %d", value.attr_type_);
  }
}
void Value::set_date_string(const char *s, int len) {
  str_value_ = s;
  int y, m, d;
  sscanf(s, "%d-%d-%d", &y, &m, &d);
  if(check_date(y, m, d)) {
    num_value_.date_value_ = y * 10000 + m * 100 + d;
  } else {
    num_value_.date_value_ = -1;
  }
  length_ = len;
}

const char *Value::data() const
{
  switch (attr_type_) {
    case TEXTS: {
      return text_value_.c_str();
    } break;
    case CHARS: {
      return str_value_.c_str();
    } break;
    default: {
      return (const char *)&num_value_;
    } break;
  }
}

std::string Value::to_string() const
{
  std::stringstream os;
  switch (attr_type_) {
    case INTS: {
      os << num_value_.int_value_;
    } break;
    case FLOATS: {
      os << common::double_to_str(num_value_.float_value_);
    } break;
    case BOOLEANS: {
      os << num_value_.bool_value_;
    } break;
    case TEXTS:{
      os << text_value_;
    } break;
    case CHARS: {
      os << str_value_;
    } break;
    case DATES: {
      char buf[16] = {0};
      snprintf(buf,sizeof(buf),"%04d-%02d-%02d",num_value_.date_value_/10000,(num_value_.date_value_%10000)/100,num_value_.date_value_%100);
      os << buf;
    } break;
    case NULLS: {
      os << "NULL";
    } break;
    default: {
      LOG_WARN("unsupported attr type: %d", attr_type_);
    } break;
  }
  return os.str();
}

int Value::compare(const Value &other) const
{
  if (this->attr_type_ == other.attr_type_) {
    switch (this->attr_type_) {
      case INTS: {
        return common::compare_int((void *)&this->num_value_.int_value_, (void *)&other.num_value_.int_value_);
      }
      case FLOATS: {
        return common::compare_float((void *)&this->num_value_.float_value_, (void *)&other.num_value_.float_value_);
      }
      case CHARS: {
        return common::compare_string((void *)this->str_value_.c_str(),
            this->str_value_.length(),
            (void *)other.str_value_.c_str(),
            other.str_value_.length());
      }
      case TEXTS: {
        return common::compare_string((void *)this->text_value_.c_str(),
            this->text_value_.length(),
            (void *)other.text_value_.c_str(),
            other.text_value_.length());
      }
      case BOOLEANS: {
        return common::compare_int((void *)&this->num_value_.bool_value_, (void *)&other.num_value_.bool_value_);
      }
      case DATES: {
        return common::compare_int((void *)&this->num_value_.date_value_, (void *)&other.num_value_.date_value_);
      }
      case NULLS:
      default: {
        LOG_WARN("unsupported type: %d", this->attr_type_);
      }
    }
  } else if (this->attr_type_ == INTS && other.attr_type_ == FLOATS) {
    float this_data = this->num_value_.int_value_;
    return common::compare_float((void *)&this_data, (void *)&other.num_value_.float_value_);
  } else if (this->attr_type_ == FLOATS && other.attr_type_ == INTS) {
    float other_data = other.num_value_.int_value_;
    return common::compare_float((void *)&this->num_value_.float_value_, (void *)&other_data);
  } else {
    float left = this->get_float();
    float right = other.get_float();
    return common::compare_float((void *)&left, (void *)&right);
  }
  LOG_WARN("not supported");
  return -1;  // TODO return rc?
}

int Value::get_int() const
{
  switch (attr_type_) {
    case CHARS: {
      try {
        return (int)(std::stol(str_value_));
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to number. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return 0;
      }
    }
    case INTS: {
      return num_value_.int_value_;
    }
    case FLOATS: {
      return (int)(std::round(num_value_.float_value_));
    }
    case BOOLEANS: {
      return (int)(num_value_.bool_value_);
    }
    case DATES: {
      return num_value_.date_value_;
    }
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return 0;
    }
  }
}

float Value::get_float() const
{
  switch (attr_type_) {
    case CHARS: {
      try {
        return std::stof(str_value_);
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to float. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return 0.0;
      }
    }
    case INTS: {
      return float(num_value_.int_value_);
    }
    case FLOATS: {
      return num_value_.float_value_;
    }
    case BOOLEANS: {
      return float(num_value_.bool_value_);
    }
    case DATES: {
      return float(num_value_.date_value_);
    }
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return 0;
    }
  }
  return 0;
}

std::string Value::get_string() const
{
  return this->to_string();
}

bool Value::get_boolean() const
{
  switch (attr_type_) {
    case CHARS: {
      try {
        float val = std::stof(str_value_);
        if (val >= EPSILON || val <= -EPSILON) {
          return true;
        }

        int int_val = std::stol(str_value_);
        if (int_val != 0) {
          return true;
        }

        return !str_value_.empty();
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to float or integer. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return !str_value_.empty();
      }
    }
    case INTS: {
      return num_value_.int_value_ != 0;
    }
    case FLOATS: {
      float val = num_value_.float_value_;
      return val >= EPSILON || val <= -EPSILON;
    }
    case BOOLEANS: {
      return num_value_.bool_value_;
    }
    case DATES: {
      return num_value_.date_value_ != 0;
    }
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return false;
    }
  }
}
bool Value::check_date(int y, int m, int d) {
  static int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  bool leap = (y % 400 == 0 || (y % 100 && y % 4 == 0));
  return y > 0 && (m > 0) &&(m <= 12)
         && (d > 0) && (d <= ((m ==2 && leap ) ? 1 : 0) + mon[m]);
}

////////////////////////////////////////////////////////////////////////////////

bool cast_to[AttrType::FLOATS + 1][AttrType::FLOATS + 1] = {
    {
        // UNDEFINED
        false,
        false,
        false,
        false,
        false,
        false,
    },
    {
        // CHARS
        false,
        true,
        true,
        false,
        false,
        true,
    },
    {
        // INTS
        false,
        true,
        true,
        false,
        false,
        true,
    },
    {
        // DATES
        false,
        false,
        false,
        false,
        false,
        false,
    },
    {
        // NULLS
        false,
        false,
        false,
        false,
        false,
        false,
    },
    {
        // FLOATS
        false,
        true,
        true,
        false,
        false,
        true,
    }};
bool type_cast_not_support(AttrType i, AttrType j)
{
  return ! cast_to[i][j];
}