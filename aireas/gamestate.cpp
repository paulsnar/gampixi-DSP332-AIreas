#include "pch.h"
#include "gamestate.h"

GameState::GameState()
{
	field = Field(3);
	current_player = GamePlayer::Player1;
	status = GameStatus::Playing;
	score_p1 = score_p2 = 0;
}

GameState::GameState(size_t field_size)
{
	field = Field(field_size);
	current_player = GamePlayer::Player1;
	status = GameStatus::Playing;
	score_p1 = score_p2 = 0;
}

GameState::GameState(const GameState& other)
{
	*this = other;
}

GameState& GameState::operator=(const GameState & other)
{
	field = other.field;
	current_player = other.current_player;
	status = other.status;
	score_p1 = other.score_p1;
	score_p2 = other.score_p2;
	return *this;
}

GameState GameState::perform_move(Edge edge)
{
	// Whenever we perform a move, don't mutate the previous gamestate, but create a new one.
	// This way we can simulate moves ahead of time, do with them as please, without affecting
	// the actual game. Therefore the frontend really just acts as a pointer to a specific gamestate,
	// and lets us walk the gamestate tree.
	std::cout << "Doing move!" << std::endl;

	auto new_state = GameState();
	perform_move_in_place(edge, new_state);

	return new_state;
}

GameState& GameState::perform_move_in_place(Edge edge, GameState& place) {
	std::cout << "Doing move!" << std::endl;

	place = *this;

	// Change the edge to point to the new blocks
	ptrdiff_t diff = (char*)place.field.get_block(0) - (char*)this->field.get_block(0);
	edge.set_first((Block*)((char*)edge.get_first() + diff));
	edge.set_second((Block*)((char*)edge.get_second() + diff));
	auto edge_score = edge.get_score();
	std::cout << "Move score: " << edge_score << std::endl;

	if (place.field.merge_edge(edge)) {
		if (place.current_player == GamePlayer::Player1) {
			place.score_p1 += edge_score;
		} else {
			place.score_p2 += edge_score;
		}
	}

	place.current_player = place.current_player == GamePlayer::Player1 ? GamePlayer::Player2 : GamePlayer::Player1;

	return place;
}