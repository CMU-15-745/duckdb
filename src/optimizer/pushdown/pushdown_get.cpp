#include "duckdb/optimizer/filter_pushdown.hpp"
#include "duckdb/optimizer/optimizer.hpp"
#include "duckdb/planner/expression/bound_columnref_expression.hpp"
#include "duckdb/planner/expression/bound_parameter_expression.hpp"
#include "duckdb/planner/operator/logical_filter.hpp"
#include "duckdb/planner/operator/logical_get.hpp"
#include <iostream>

namespace duckdb {

unique_ptr<LogicalOperator> FilterPushdown::PushdownGet(unique_ptr<LogicalOperator> op) {
	D_ASSERT(op->type == LogicalOperatorType::LOGICAL_GET);
	auto &get = op->Cast<LogicalGet>();

	if (get.function.pushdown_complex_filter || get.function.filter_pushdown) {
		std::cout << "Get supports filter pushdown" << std::endl;
		// this scan supports some form of filter push-down
		// check if there are any parameters
		// if there are, invalidate them to force a re-bind on execution
		for (auto &filter : filters) {
			if (filter->filter->HasParameter()) {
				std::cout << "Invalidating filter : " << filter->filter->ToString() << std::endl;
				// there is a parameter in the filters! invalidate it
				BoundParameterExpression::InvalidateRecursive(*filter->filter);
			}
		}
	}
	if (get.function.pushdown_complex_filter) {
		// for the remaining filters, check if we can push any of them into the scan as well
		vector<unique_ptr<Expression>> expressions;
		expressions.reserve(filters.size());
		for (auto &filter : filters) {
			expressions.push_back(std::move(filter->filter));
		}
		filters.clear();

		std::cout <<"Expressions before the pushdown_complex_filter" << std::endl;
		for(auto& e: expressions) {
			std::cout << e->ToString() << std::endl;
		}
		get.function.pushdown_complex_filter(optimizer.context, get, get.bind_data.get(), expressions);
		std::cout <<"Expressions after the pushdown_complex_filter" << std::endl;
		for(auto& e: expressions) {
			std::cout << e->ToString() << std::endl;
		}

		if (expressions.empty()) {
			return op;
		}
		// re-generate the filters
		for (auto &expr : expressions) {
			auto f = make_uniq<Filter>();
			f->filter = std::move(expr);
			f->ExtractBindings();
			filters.push_back(std::move(f));
		}
	}

	if (!get.table_filters.filters.empty() || !get.function.filter_pushdown) {
		std::cout << "Get does not support filter_pushdown or table_filters not empty" << std::endl;
		std::cout << "Adding filters are a LogicalFilter on top" << std::endl;
		// the table function does not support filter pushdown: push a LogicalFilter on top
		return FinishPushdown(std::move(op));
	}
	PushFilters();

	//! We generate the table filters that will be executed during the table scan
	//! Right now this only executes simple AND filters
	get.table_filters = combiner.GenerateTableScanFilters(get.column_ids);

	// //! For more complex filters if all filters to a column are constants we generate a min max boundary used to
	// check
	// //! the zonemaps.
	// auto zonemap_checks = combiner.GenerateZonemapChecks(get.column_ids, get.table_filters);

	// for (auto &f : get.table_filters) {
	// 	f.column_index = get.column_ids[f.column_index];
	// }

	// //! Use zonemap checks as table filters for pre-processing
	// for (auto &zonemap_check : zonemap_checks) {
	// 	if (zonemap_check.column_index != COLUMN_IDENTIFIER_ROW_ID) {
	// 		get.table_filters.push_back(zonemap_check);
	// 	}
	// }

	GenerateFilters();

	//! Now we try to pushdown the remaining filters to perform zonemap checking
	return FinishPushdown(std::move(op));
}

} // namespace duckdb
