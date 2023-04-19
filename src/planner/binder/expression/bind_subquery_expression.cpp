#include "duckdb/parser/expression/subquery_expression.hpp"
#include "duckdb/planner/binder.hpp"
#include "duckdb/planner/expression/bound_cast_expression.hpp"
#include "duckdb/planner/expression/bound_subquery_expression.hpp"
#include "duckdb/planner/expression_binder.hpp"
#include "duckdb/common/string_util.hpp"
#include <iostream>

#include "duckdb/planner/expression_binder/lateral_binder.hpp"
#include "duckdb/planner/expression_binder/relation_binder.hpp"
#include "duckdb/planner/expression_binder/returning_binder.hpp"
#include "duckdb/planner/expression_binder/alter_binder.hpp"
#include "duckdb/planner/expression_binder/group_binder.hpp"
#include "duckdb/planner/expression_binder/where_binder.hpp"
#include "duckdb/planner/expression_binder/aggregate_binder.hpp"
#include "duckdb/planner/expression_binder/constant_binder.hpp"
#include "duckdb/planner/expression_binder/table_function_binder.hpp"
#include "duckdb/planner/expression_binder/update_binder.hpp"
#include "duckdb/planner/expression_binder/check_binder.hpp"
#include "duckdb/planner/expression_binder/base_select_binder.hpp"
#include "duckdb/planner/expression_binder/insert_binder.hpp"
#include "duckdb/planner/expression_binder/index_binder.hpp"


namespace duckdb {

class BoundSubqueryNode : public QueryNode {
public:
	BoundSubqueryNode(shared_ptr<Binder> subquery_binder, unique_ptr<BoundQueryNode> bound_node,
	                  unique_ptr<SelectStatement> subquery)
	    : QueryNode(QueryNodeType::BOUND_SUBQUERY_NODE), subquery_binder(std::move(subquery_binder)),
	      bound_node(std::move(bound_node)), subquery(std::move(subquery)) {
	}

	shared_ptr<Binder> subquery_binder;
	unique_ptr<BoundQueryNode> bound_node;
	unique_ptr<SelectStatement> subquery;

	const vector<unique_ptr<ParsedExpression>> &GetSelectList() const override {
		throw InternalException("Cannot get select list of bound subquery node");
	}

	string ToString() const override {
		throw InternalException("Cannot ToString bound subquery node");
	}
	unique_ptr<QueryNode> Copy() const override {
		throw InternalException("Cannot copy bound subquery node");
	}
	void Serialize(FieldWriter &writer) const override {
		throw InternalException("Cannot serialize bound subquery node");
	}

	void FormatSerialize(FormatSerializer &serializer) const override {
		throw InternalException("Cannot serialize bound subquery node");
	}
};

static void PrintBinder(ExpressionBinder* binder)
{
    if (dynamic_cast<LateralBinder*>(binder))
    {
        std::cout << "LateralBinder" << std::endl;	
    }
    if (dynamic_cast<RelationBinder*>(binder))
    {
        std::cout << "RelationBinder" << std::endl;		
    }
    if (dynamic_cast<ReturningBinder*>(binder))
    {
        std::cout << "ReturningBinder" << std::endl;	
    }
    if (dynamic_cast<AlterBinder*>(binder))
    {
        std::cout << "AlterBinder" << std::endl;	
    }
    if (dynamic_cast<GroupBinder*>(binder))
    {
        std::cout << "GroupBinder" << std::endl;	
    }
    if (dynamic_cast<WhereBinder*>(binder))
    {
        std::cout << "WhereBinder" << std::endl;	
    }
    if (dynamic_cast<AggregateBinder*>(binder))
    {
        std::cout << "AggregateBinder" << std::endl;	
    }
    if (dynamic_cast<ConstantBinder*>(binder))
    {
        std::cout << "ConstantBinder" << std::endl;	
    }
    if (dynamic_cast<TableFunctionBinder*>(binder))
    {
        std::cout << "TableFunctionBinder" << std::endl;	
    }
    if (dynamic_cast<UpdateBinder*>(binder))
    {
        std::cout << "UpdateBinder" << std::endl;	
    }
    if (dynamic_cast<CheckBinder*>(binder))
    {
        std::cout << "CheckBinder" << std::endl;		
    }
    if (dynamic_cast<BaseSelectBinder*>(binder))
    {
        std::cout << "BaseSelectBinder" << std::endl;	
    }
}

BindResult ExpressionBinder::BindExpression(SubqueryExpression &expr, idx_t depth) {
    std::cout << "In BindExpression for SubqueryExpression!!!" << std::endl;
    std::cout << "[";
    switch (expr.subquery->node->type) {
        case (QueryNodeType::SELECT_NODE):
            std::cout << "SELECT_NODE";
            break;
        case (QueryNodeType::SET_OPERATION_NODE):
            std::cout << "SET_OPERATION_NODE";
            break;
        case (QueryNodeType::BOUND_SUBQUERY_NODE):
            std::cout << "BOUND_SUBQUERY_NODE";
            break;
        case (QueryNodeType::RECURSIVE_CTE_NODE):
            std::cout << "RECURSIVE_CTE_NODE";
            break;
    }
    std::cout << "]" << std::endl; 
    if (expr.subquery->node->type != QueryNodeType::BOUND_SUBQUERY_NODE) {
		D_ASSERT(depth == 0);
		// first bind the actual subquery in a new binder
		auto subquery_binder = Binder::CreateBinder(context, &binder);
		subquery_binder->can_contain_nulls = true;
		auto bound_node = subquery_binder->BindNode(*expr.subquery->node);
		// check the correlated columns of the subquery for correlated columns with depth > 1
		for (idx_t i = 0; i < subquery_binder->correlated_columns.size(); i++) {
			CorrelatedColumnInfo corr = subquery_binder->correlated_columns[i];
            std::cout << "Column: " << corr.name << " " << corr.depth << std::endl;
			if (corr.depth > 1) {
				// depth > 1, the column references the query ABOVE the current one
				// add to the set of correlated columns for THIS query
				corr.depth -= 1;
				binder.AddCorrelatedColumn(corr);
                std::cout << "Depth > 1 detected!" << std::endl;
                std::cout << "Updated Column: " << corr.name << " " << corr.depth << std::endl;
                std::cout << "Adding to binder: "; 
                PrintBinder(this);
			}
		}
		if (expr.subquery_type != SubqueryType::EXISTS && bound_node->types.size() > 1) {
			throw BinderException(binder.FormatError(
			    expr, StringUtil::Format("Subquery returns %zu columns - expected 1", bound_node->types.size())));
		}
		auto prior_subquery = std::move(expr.subquery);
		expr.subquery = make_uniq<SelectStatement>();
		expr.subquery->node =
		    make_uniq<BoundSubqueryNode>(std::move(subquery_binder), std::move(bound_node), std::move(prior_subquery));
	}
	// now bind the child node of the subquery
	if (expr.child) {
		// first bind the children of the subquery, if any
		string error = Bind(&expr.child, depth);
		if (!error.empty()) {
			return BindResult(error);
		}
	}
	// both binding the child and binding the subquery was successful
	D_ASSERT(expr.subquery->node->type == QueryNodeType::BOUND_SUBQUERY_NODE);
	auto bound_subquery = (BoundSubqueryNode *)expr.subquery->node.get();
	auto child = (BoundExpression *)expr.child.get();
	auto subquery_binder = std::move(bound_subquery->subquery_binder);
	auto bound_node = std::move(bound_subquery->bound_node);
	LogicalType return_type =
	    expr.subquery_type == SubqueryType::SCALAR ? bound_node->types[0] : LogicalType(LogicalTypeId::BOOLEAN);
	if (return_type.id() == LogicalTypeId::UNKNOWN) {
		return_type = LogicalType::SQLNULL;
	}

	auto result = make_uniq<BoundSubqueryExpression>(return_type);
	if (expr.subquery_type == SubqueryType::ANY) {
		// ANY comparison
		// cast child and subquery child to equivalent types
		D_ASSERT(bound_node->types.size() == 1);
		auto compare_type = LogicalType::MaxLogicalType(child->expr->return_type, bound_node->types[0]);
		child->expr = BoundCastExpression::AddCastToType(context, std::move(child->expr), compare_type);
		result->child_type = bound_node->types[0];
		result->child_target = compare_type;
	}
	result->binder = std::move(subquery_binder);
	result->subquery = std::move(bound_node);
	result->subquery_type = expr.subquery_type;
	result->child = child ? std::move(child->expr) : nullptr;
	result->comparison_type = expr.comparison_type;

	return BindResult(std::move(result));
}

} // namespace duckdb
