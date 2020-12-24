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

GameState GameState::operator=(const GameState & other)
{
	field = other.field;
	current_player = other.current_player;
	status = other.status;
	score_p1 = other.score_p1;
	score_p2 = other.score_p2;
}

GameState GameState::perform_move(Edge & edge)
{
	// Whenever we perform a move, don't mutate the previous gamestate, but create a new one.
	// This way we can simulate moves ahead of time, do with them as please, without affecting
	// the actual game. Therefore the frontend really just acts as a pointer to a specific gamestate,
	// and lets us walk the gamestate tree.
	auto new_state = *this;
	new_state.field.merge_edge(edge); //TODO: This doesn't work because copies work on a new set of edges.
	return new_state;
}
