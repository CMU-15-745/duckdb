#include "duckdb/planner/operator/logical_dependent_join.hpp"

namespace duckdb {

LogicalDependentJoin::LogicalDependentJoin (
		unique_ptr<LogicalOperator> left,
		unique_ptr<LogicalOperator> right,
		vector<CorrelatedColumnInfo> correlated_columns,
		JoinType type,
		unique_ptr<Expression> condition):
	LogicalJoin(type, LogicalOperatorType::LOGICAL_DEPENDENT_JOIN),
	correlated_columns(correlated_columns),
	condition(std::move(condition)) {
	children.push_back(std::move(left));
	children.push_back(std::move(right));
}

unique_ptr<LogicalOperator> LogicalDependentJoin::Create(
		unique_ptr<LogicalOperator> left,
		unique_ptr<LogicalOperator> right,
		vector<CorrelatedColumnInfo> correlated_columns,
		JoinType type,
		unique_ptr<Expression> condition) {
	return make_unique<LogicalDependentJoin>(std::move(left), std::move(right), correlated_columns, type, std::move(condition));
}

void LogicalDependentJoin::Serialize(FieldWriter &writer) const {
	LogicalJoin::Serialize(writer);
	// writer.WriteRegularSerializableList(correlated_columns);
	// writer.WriteSerializable(condition);
}

} // namespace duckdb
