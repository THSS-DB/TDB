#include "include/query_engine/optimizer/optimizer.h"

RC Optimizer::rewrite(std::unique_ptr<LogicalNode> &logical_operator)
{
    //Currently we don't support optimization.
    return RC::SUCCESS;
}