#include "duckdb/planner/subquery/has_correlated_expressions.hpp"

#include "duckdb/planner/expression/bound_columnref_expression.hpp"
#include "duckdb/planner/expression/bound_subquery_expression.hpp"

#include <algorithm>

namespace duckdb {

HasCorrelatedExpressions::HasCorrelatedExpressions(const vector<CorrelatedColumnInfo> &correlated, bool lateral, int depth)
    : has_correlated_expressions(false), lateral(lateral), correlated_columns(correlated), depth(depth) {
}

void HasCorrelatedExpressions::VisitOperator(LogicalOperator &op) {
	VisitOperatorExpressions(op);
}

unique_ptr<Expression> HasCorrelatedExpressions::VisitReplace(BoundColumnRefExpression &expr,
                                                              unique_ptr<Expression> *expr_ptr) {
  my_own_debug("HasCorrelatedExpressions::VisitReplace Got ColumnRef " + expr.ToString() + " at Depth " + to_string(expr.depth) + " VisitorDepth " + to_string(depth));
	if (expr.depth == 0) {
		return nullptr;
	}
	if (expr.depth > 1) {
		if (lateral) {
			if (expr.depth == depth) {
				has_correlated_expressions = true;
			}
			// throw BinderException("Nested lateral joins are not (yet) supported");
			return nullptr;
		}
		throw InternalException("Expression with depth > 1 detected in non-lateral join");
	}
	// correlated column reference
	D_ASSERT(expr.depth == 1);
	has_correlated_expressions = true;
	return nullptr;
}

unique_ptr<Expression> HasCorrelatedExpressions::VisitReplace(BoundSubqueryExpression &expr,
                                                              unique_ptr<Expression> *expr_ptr) {
	my_own_debug("HasCorrelatedExpressions::VisitReplace Got Expr " + expr.ToString());
	if (!expr.IsCorrelated()) {
		my_own_debug("HasCorrelatedExpressions::VisitReplace Not correlated");
		return nullptr;
	}
	// check if the subquery contains any of the correlated expressions that we are concerned about in this node
	for (idx_t i = 0; i < correlated_columns.size(); i++) {
		if (std::find(expr.binder->correlated_columns.begin(), expr.binder->correlated_columns.end(),
		              correlated_columns[i]) != expr.binder->correlated_columns.end()) {
			my_own_debug("HasCorrelatedExpressions::VisitReplace Has BoundSubqueryExpression");
			has_correlated_expressions = true;
			break;
		}
	}
	return nullptr;
}

} // namespace duckdb
