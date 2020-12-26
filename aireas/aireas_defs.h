#pragma once
#include <tuple>

typedef std::tuple<int, int, int, int> dims;

constexpr unsigned int SCREEN_SIZE_X = 848;
constexpr unsigned int SCREEN_SIZE_Y = 480;

// If false - heuristic values are simply -1, 0 or 1
// If true - heuristic values ar score delta between the two players. This will cause the alpha-beta
//			 algorithm to walk way more subtrees, increasing RAM usage a lot. Either way, if you play for long enough,
//			 you'll start running out of memory, sowwy.
//constexpr bool MAN_IR_DAHUJA_RAM = false;
constexpr bool MAN_IR_DAUDZ_RAM = false; // Akadēmiski-korektā versija

constexpr unsigned int FIELD_DIMENSION = 3;
constexpr unsigned int BLOCK_DRAW_SIZE = 30;
constexpr unsigned int BLOCK_PADDING = 5;
constexpr unsigned int FIELD_OFFSET_CENTERED_X =
	(SCREEN_SIZE_X - (FIELD_DIMENSION * BLOCK_DRAW_SIZE + (FIELD_DIMENSION - 1) * BLOCK_PADDING)) / 2;
constexpr unsigned int FIELD_OFFSET_CENTERED_Y =
	(SCREEN_SIZE_Y - (FIELD_DIMENSION * BLOCK_DRAW_SIZE + (FIELD_DIMENSION - 1) * BLOCK_PADDING)) / 2;