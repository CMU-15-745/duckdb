#include "duckdb/optimizer/filter_pushdown.hpp"
#include "duckdb/planner/operator/logical_empty_result.hpp"
#include "duckdb/planner/operator/logical_filter.hpp"
#include <iostream>
#include "../filter_pushdown_constants.hpp"

namespace duckdb {

using Filter = FilterPushdown::Filter;

unique_ptr<LogicalOperator> FilterPushdown::PushdownFilter(unique_ptr<LogicalOperator> op) {
	D_ASSERT(op->type == LogicalOperatorType::LOGICAL_FILTER);
	auto &filter = op->Cast<LogicalFilter>();
	if (!filter.projection_map.empty()) {
		return FinishPushdown(std::move(op));
	}

	if (disable_filter_pushdown) {
		vector<unique_ptr<Expression>> exprs;
		for (auto &expression : filter.expressions) {
			exprs.push_back(std::move(expression));
		}
		auto child_op = Rewrite(std::move(filter.children[0]));
		for (auto &expr: exprs) {
//			std::cout  << "Got expr " << expr->ToString() << std::endl;
			vector<unique_ptr<Expression>> v;
			v.push_back(std::move(expr));
			child_op = AddLogicalFilter(std::move(child_op), std::move(v));
		}
//		child_op->Print();
		return child_op;
	} else {
		for (auto &expression : filter.expressions) {
			if (AddFilter(std::move(expression)) == FilterResult::UNSATISFIABLE) {
				// filter statically evaluates to false, strip tree
				return make_uniq<LogicalEmptyResult>(std::move(op));
			}
		}
		GenerateFilters();
		return Rewrite(std::move(filter.children[0]));
	}
	// filter: gather the filters and remove the filter from the set of operations

}

} // namespace duckdb
