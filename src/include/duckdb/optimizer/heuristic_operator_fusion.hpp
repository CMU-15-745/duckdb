#pragma once

#include "duckdb.hpp"
#include "duckdb/optimizer/optimizer_extension.hpp"

namespace duckdb {

class HeuristicOperatorFusion {
public:
	HeuristicOperatorFusion() { }
	duckdb::unique_ptr<LogicalOperator> Rewrite(duckdb::unique_ptr<LogicalOperator> &plan);
};
}
