#include "duckdb/parser/tableref/subqueryref.hpp"
#include "duckdb/planner/binder.hpp"
#include "duckdb/planner/tableref/bound_subqueryref.hpp"

namespace duckdb {

unique_ptr<BoundTableRef> Binder::Bind(SubqueryRef &ref, CommonTableExpressionInfo *cte) {
	my_own_debug("In Binder::Bind SubqueryRef " + ref.ToString());
	auto binder = Binder::CreateBinder(context, this);
	binder->can_contain_nulls = true;
	if (cte) {
		binder->bound_ctes.insert(cte);
	}
	binder->alias = ref.alias.empty() ? "unnamed_subquery" : ref.alias;
	auto subquery = binder->BindNode(*ref.subquery->node);
	idx_t bind_index = subquery->GetRootIndex();
	string alias;
	if (ref.alias.empty()) {
		alias = "unnamed_subquery" + to_string(bind_index);
	} else {
		alias = ref.alias;
	}
	auto result = make_unique<BoundSubqueryRef>(std::move(binder), std::move(subquery));
	bind_context.AddSubquery(bind_index, alias, ref, *result->subquery);
	auto bindings_list = bind_context.GetBindingsList();
	for(auto& p: bindings_list) {
		my_own_debug("Binder::Bind SubqueryRef Alias:" + p.first + " aka " + alias + " Table " + to_string(p.second->index));
		for(auto n: p.second->names) {
			my_own_debug("Binder::Bind SubqueryRef Bound column: " + n);
		}
	}
	MoveCorrelatedExpressions(*result->binder);
	// my_own_debug("Postmove");
	// for(auto& col : correlated_columns) {
	//   my_own_debug("CorrelatedColumns: Name: " + col.name + "[" + to_string(col.binding.table_index) + "," + to_string(col.binding.column_index) + "]" + " Depth " + to_string(col.depth));
	// }
	return std::move(result);
}

} // namespace duckdb
