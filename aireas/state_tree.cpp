#include "pch.h"
#include "state_tree.h"

StateTreeNode::StateTreeNode(size_t field_size)
{
	value = GameState(field_size);
	parsed = vector<bool>(value.get_field().get_valid_edges().size(), false);
	next = vector<StateTreeNode>();
	next.reserve(parsed.size());
}

StateTreeNode::StateTreeNode(GameState& from, size_t move)
{
	value = from.perform_move(from.get_field().get_valid_edges()[move].get());
	parsed = vector<bool>(value.get_field().get_valid_edges().size(), false);
	next = vector<StateTreeNode>();
	next.reserve(parsed.size());
}

StateTreeNode & StateTreeNode::get_child(size_t idx)
{
	while (next.size() <= idx) {
		size_t this_idx = next.size();
		next.push_back(StateTreeNode(this->value, this_idx));
		parsed[this_idx] = true;
	}
	return next.at(idx);
}

void walk_tree_with_depth(StateTreeNode & from, size_t depth)
{
	// Generates fields up to the specified depth.
	if (depth <= 0) {
		return;
	}

	for (size_t i = 0; i < from.get_child_count(); i++) {
		walk_tree_with_depth(from.get_child(i), depth - 1);
	}
}