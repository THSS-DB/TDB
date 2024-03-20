#include "include/common/rc.h"

const char *strrc(RC rc) {
#define DEFINE_RC(name) \
  case RC::name: {      \
    return #name;       \
  } break;

  switch (rc) {
    DEFINE_RCS;
    default: {
      return "unkown";
    }
  }
#undef DEFINE_RC
}

bool RC_SUCC(RC rc) { return rc == RC::SUCCESS; }

bool RC_FAIL(RC rc) { return rc != RC::SUCCESS; }
