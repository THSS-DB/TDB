#include <sstream>
#include "include/query_engine/planner/operator/explain_physical_operator.h"
#include "common/log/log.h"

using namespace std;

RC ExplainPhysicalOperator::open(Trx *trx)
{
  ASSERT(children_.size() == 1, "explain must has 1 child");
  return children_[0]->open(trx);
}

RC ExplainPhysicalOperator::close()
{
  for (std::unique_ptr<PhysicalOperator> &child_operator : children_) {
    child_operator->close();
  }
  return RC::SUCCESS;
}

RC ExplainPhysicalOperator::next()
{
  if (!physical_plan_.empty()) {
    return RC::RECORD_EOF;
  }

  stringstream ss;
  ss << "OPERATOR(NAME)\n";

  int level = 0;
  std::vector<bool> ends;
  ends.push_back(true);
  const auto children_size = static_cast<int>(children_.size());
  for (int i = 0; i < children_size - 1; i++) {
    to_string(ss, children_[i].get(), level, false /*last_child*/, ends);
  }
  if (children_size > 0) {
    to_string(ss, children_[children_size - 1].get(), level, true /*last_child*/, ends);
  }

  physical_plan_ = ss.str();

  vector<Value> cells;
  Value cell;
  cell.set_string(physical_plan_.c_str());
  cells.emplace_back(cell);
  tuple_.set_cells(cells);
  return RC::SUCCESS;
}

Tuple *ExplainPhysicalOperator::current_tuple()
{
  return &tuple_;
}

void ExplainPhysicalOperator::to_string(
    std::ostream &os, PhysicalOperator *oper, int level, bool last_child, std::vector<bool> &ends)
{
  for (int i = 0; i < level - 1; i++) {
    if (ends[i]) {
      os << "  ";
    } else {
      os << "│ ";
    }
  }
  if (level > 0) {
    if (last_child) {
      os << "└─";
      ends[level - 1] = true;
    } else {
      os << "├─";
    }
  }

  os << oper->name();
  std::string param = oper->param();
  if (!param.empty()) {
    os << "(" << param << ")";
  }
  os << '\n';

  if (static_cast<int>(ends.size()) < level + 2) {
    ends.resize(level + 2);
  }
  ends[level + 1] = false;

  vector<std::unique_ptr<PhysicalOperator>> &children = oper->children();
  const auto size = static_cast<int>(children.size());
  for (auto i = 0; i < size - 1; i++) {
    to_string(os, children[i].get(), level + 1, false /*last_child*/, ends);
  }
  if (size > 0) {
    to_string(os, children[size - 1].get(), level + 1, true /*last_child*/, ends);
  }
}
