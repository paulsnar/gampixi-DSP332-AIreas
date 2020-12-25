#pragma once

#include <vector>
#include "aireas_defs.h"
#include "gamestate.h"

using std::vector;

class StateTreeNode {
public:
	StateTreeNode(size_t field_size); // Create root node
	StateTreeNode(GameState& from, size_t move); // Create next nodes, with each representing a possible mode
	GameState value;
	const size_t get_child_count() const {
		return parsed.size();
	}
	StateTreeNode& get_child(size_t idx);
private:
	vector<bool> parsed; // True, if we have fully generated the next tree node. Might be false because of pruning.
	// Next indexes should match value->field->valid_edges indexes. That way by performing a move, we really
	// just pick an index for an already calculated path.
	vector<StateTreeNode> next;
};

void walk_tree_with_depth(StateTreeNode& from, size_t depth);