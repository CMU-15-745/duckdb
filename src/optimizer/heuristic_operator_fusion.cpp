#include "duckdb/optimizer/heuristic_operator_fusion.hpp"

#include "duckdb.hpp"
#include "duckdb/common/types/column/column_data_collection.hpp"
#include "duckdb/optimizer/optimizer_extension.hpp"
#include "duckdb/planner/operator/logical_column_data_get.hpp"
#include "duckdb/planner/operator/logical_get.hpp"
#include "duckdb/common/serializer/binary_serializer.hpp"
#include "duckdb/common/serializer/binary_deserializer.hpp"
#include "duckdb/common/serializer/memory_stream.hpp"
#include <iostream>

using namespace duckdb;

duckdb::unique_ptr<LogicalOperator> HeuristicOperatorFusion::Rewrite(duckdb::unique_ptr<LogicalOperator> &plan) {
		std::cout << "Called FuseOperators on plan" << std::endl;
		std::cout << "Current operator is " << LogicalOperatorToString(plan->type) << std::endl;
		for(auto& c: plan->children) {
			c = Rewrite(c);
		}
	  return std::move(plan);
}
