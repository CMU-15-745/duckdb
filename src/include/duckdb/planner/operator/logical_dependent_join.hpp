//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/operator/logical_dependent_join.hpp
//
// logical_dependent_join represents a logical operator for lateral joins that
// is planned buts not yet flattened
//
// This construct only exists during planning and should not exist in the plan
// once flattening is complete. Although the same information can be kept in the
// join itself, creating a new construct makes the code cleaner and easier to
// understand.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/planner/binder.hpp"
#include "duckdb/planner/operator/logical_comparison_join.hpp"

namespace duckdb {

class LogicalDependentJoin : public LogicalComparisonJoin {
public:
	explicit LogicalDependentJoin(unique_ptr<LogicalOperator> left, unique_ptr<LogicalOperator> right,
	                              vector<CorrelatedColumnInfo> correlated_columns, JoinType type,
	                              unique_ptr<Expression> condition);

	//! The conditions of the join
	unique_ptr<Expression> join_condition;
	vector<CorrelatedColumnInfo> correlated_columns;

public:
	static unique_ptr<LogicalOperator> Create(unique_ptr<LogicalOperator> left, unique_ptr<LogicalOperator> right,
	                                          vector<CorrelatedColumnInfo> correlated_columns, JoinType type,
	                                          unique_ptr<Expression> condition);
	void Serialize(FieldWriter &writer) const override;
};
} // namespace duckdb