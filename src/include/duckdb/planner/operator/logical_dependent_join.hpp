//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/operator/logical_dependent_join.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/constants.hpp"
#include "duckdb/common/enums/joinref_type.hpp"
#include "duckdb/common/unordered_set.hpp"
#include "duckdb/planner/joinside.hpp"
#include "duckdb/planner/operator/logical_join.hpp"
#include "duckdb/planner/binder.hpp"
#include "duckdb/planner/operator/logical_comparison_join.hpp"

namespace duckdb {

class LogicalDependentJoin : public LogicalComparisonJoin {
public:
	explicit LogicalDependentJoin (unique_ptr<LogicalOperator> left, unique_ptr<LogicalOperator> right, vector<CorrelatedColumnInfo>& correlated_columns, JoinType type, unique_ptr<Expression> condition);

	//! The conditions of the join
	unique_ptr<Expression> join_condition;
	//! Used for duplicate-eliminated joins
	vector<CorrelatedColumnInfo>& correlated_columns;

public:
	static unique_ptr<LogicalOperator> Create(unique_ptr<LogicalOperator> left, unique_ptr<LogicalOperator> right, vector<CorrelatedColumnInfo>& correlated_columns, JoinType type, unique_ptr<Expression> condition);
	void Serialize(FieldWriter &writer) const override;
};
} // namespace duckdb
