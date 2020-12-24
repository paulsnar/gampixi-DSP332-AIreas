#pragma once

#include "field_graph.h"

enum GamePlayer {
	Player1,
	Player2
};

enum GameStatus {
	Playing,
	Player1Victory,
	Player2Victory,
	Draw
};

class GameState {
public:
	GameState();
	GameState(size_t field_size);
	GameState(const GameState& other);
	GameState& operator=(const GameState& other);
	GameState& perform_move(Edge& edge);
	inline const GamePlayer get_current_player() const {
		return current_player;
	}
	inline const GameStatus get_status() const {
		return status;
	}
	inline const unsigned int get_score(GamePlayer for_player) const {
		return for_player == GamePlayer::Player1 ? score_p1 : score_p2;
	}
	inline const Field& get_field() {
		return field;
	}
private:
	Field field;
	GamePlayer current_player;
	GameStatus status;
	unsigned int score_p1;
	unsigned int score_p2;
};