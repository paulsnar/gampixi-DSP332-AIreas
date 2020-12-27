#include "pch.h"
#include "state_tree.h"
#include "aireas_defs.h"
#include <cmath>
#include <iostream>

using std::max;
using std::min;

StateTreeNode::StateTreeNode(size_t field_size)
{
	value = GameState(field_size);
	node_value = value.get_current_player() == GamePlayer::Player1 ? INT_MIN : INT_MAX;
	parsed = vector<bool>(value.get_field().get_valid_edge_count(), false);
	next = vector<StateTreeNode>();
	next.reserve(parsed.size());
}

StateTreeNode::StateTreeNode(GameState& from, size_t move)
{
	value = from.perform_move(from.get_field().get_valid_edges().at(move).get());
	parsed = vector<bool>(value.get_field().get_valid_edge_count(), false);
	next = vector<StateTreeNode>();
	next.reserve(parsed.size());

	if (value.get_status() != GameStatus::Playing) {
		// This is final node and will provide the heuristic value
		if constexpr (USE_BETTER_NODE_VALUES == true) {
			// The more complex the board is, the better the node, basically, move to a victory that's harder to decipher
			/*switch (value.get_status()) {
			case GameStatus::Player1Victory:
				node_value = value.get_field().get_block_count();
				break;
			case GameStatus::Player2Victory:
				node_value = -value.get_field().get_block_count();
				break;
			case GameStatus::Playing:
			case GameStatus::Draw:
			default:
				node_value = 0;
				break;
			}*/
			//node_value = value.get_field().get_block_count() * (value.get_status == GamePlayer::Player1 ? 1 : -1);
			node_value = value.get_field().get_block_count() * 
				((int)value.get_score(GamePlayer::Player1) - (int)value.get_score(GamePlayer::Player2));
		} else {
			switch (value.get_status()) {
			case GameStatus::Player1Victory:
				node_value = 1;
				break;
			case GameStatus::Player2Victory:
				node_value = -1;
				break;
			case GameStatus::Playing:
			case GameStatus::Draw:
			default:
				node_value = 0;
				break;
			}
		}
	} else {
		node_value = value.get_current_player() == GamePlayer::Player1 ? INT_MIN : INT_MAX;
	}
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

StateTreeNode & StateTreeNode::best_child()
{
	// Pick the first node that has the same heuristic value as this node - that's the winning path.
	// If multiple nodes have the same value, DON'T PICK AT RANDOM. The next node subtrees are not
	// evaluated as thoroughly and may lead to a loss.
	for (auto& n : next) {
		if (n.node_value == this->node_value) {
			std::cout << "Found node with h-value " << n.node_value << std::endl;
			return n;
		}
	}

	std::cout << "Didn't find node - this is bad." << std::endl;
	return next.front();
}

/*
 * Some ground rules:
 * Player1 is maximizer, Player2 is minimizer.
 * Whoever gets to be the AI is arbritrary.
 * Nodes by default get initialized to huge values.
*/

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

int walk_tree_with_alphabeta(StateTreeNode & from, int alpha, int beta)
{
	if (from.value.get_status() != GameStatus::Playing) {
		//std::cout << "Reached end, score " << from.value.get_score(GamePlayer::Player1) << " | "
		//	<< from.value.get_score(GamePlayer::Player2) << std::endl;
		return from.node_value;
	}

	from.node_value = from.value.get_current_player() == GamePlayer::Player1 ? INT_MIN : INT_MAX;
	if (from.value.get_current_player() == GamePlayer::Player1) {
		// Maximizer
		for (size_t i = 0; i < from.get_child_count(); i++) {
			from.node_value = max(from.node_value, walk_tree_with_alphabeta(from.get_child(i), alpha, beta));
			alpha = max(alpha, from.node_value);
			if (alpha >= beta) {
				break; // Beta cutodff
			}
		}
		return from.node_value;
	} else {
		// Minimizer
		for (size_t i = 0; i < from.get_child_count(); i++) {
			from.node_value = min(from.node_value, walk_tree_with_alphabeta(from.get_child(i), alpha, beta));
			beta = min(beta, from.node_value);
			if (alpha >= beta) {
				break; // Alpha cutodff
			}
		}
		return from.node_value;
	}
}
