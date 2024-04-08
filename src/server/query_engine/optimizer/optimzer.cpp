#include "include/query_engine/optimizer/optimizer.h"

RC Optimizer::rewrite(std::unique_ptr<LogicalNode> &logical_operator)
{
    RC rc;
    bool change = false;
    rc = rewriter_.rewrite(logical_operator, change);
    if(rc != RC::SUCCESS){
      return rc;
    }
    return rc;
}