#pragma once

#include <vector>
#include <utility>
#include "aireas_defs.h"
#include "gamestate.h"

using std::vector;
using std::pair;
using std::make_pair;

class StateTreeNode {
public:
	StateTreeNode(size_t field_size); // Create root node
	StateTreeNode(GameState& from, size_t move); // Create next nodes, with each representing a possible mode
	int node_value;
	GameState value;
	const size_t get_child_count() const {
		return parsed.size();
	}
	const inline bool get_parsed(size_t idx) const {
		return parsed.at(idx);
	}
	StateTreeNode& get_child(size_t idx);
	StateTreeNode& best_child();
private:
	vector<bool> parsed; // True, if we have fully generated the next tree node. Might be false because of pruning.
	// Next indexes should match value->field->valid_edges indexes. That way by performing a move, we really
	// just pick an index for an already calculated path.
	vector<StateTreeNode> next;
};

void walk_tree_with_depth(StateTreeNode& from, size_t depth);

int walk_tree_with_alphabeta(StateTreeNode& from, int alpha, int beta);