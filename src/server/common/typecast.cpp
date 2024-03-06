#include "include/common/typecast.h"


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