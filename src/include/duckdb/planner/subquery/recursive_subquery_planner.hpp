//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/subquery/recursive_subquery_planner.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/planner/logical_operator_visitor.hpp"

namespace duckdb {

class RecursiveSubqueryPlanner : public LogicalOperatorVisitor {
public:
	explicit RecursiveSubqueryPlanner(Binder &binder) : binder(binder) {}
	void VisitOperator(LogicalOperator &op) override;
	unique_ptr<Expression> VisitReplace(BoundSubqueryExpression &expr, unique_ptr<Expression> *expr_ptr) override;
private:
	unique_ptr<LogicalOperator> root;
	Binder &binder;
};
} // namespace duckdb

