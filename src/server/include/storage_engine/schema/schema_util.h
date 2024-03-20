#pragma once

#include <string>
#include "common/defs.h"

static constexpr const char *TABLE_META_SUFFIX = ".table";
static constexpr const char *TABLE_META_FILE_PATTERN = ".*\\.table$";
static constexpr const char *TABLE_DATA_SUFFIX = ".data";
static constexpr const char *TABLE_INDEX_SUFFIX = ".index";

std::string table_meta_file(const char *base_dir, const char *table_name);
std::string table_data_file(const char *base_dir, const char *table_name);
std::string table_index_file(const char *base_dir, const char *table_name, const char *index_name);
