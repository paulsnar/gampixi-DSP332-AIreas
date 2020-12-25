#pragma once
#include <tuple>

typedef std::tuple<int, int, int, int> dims;

constexpr unsigned int SCREEN_SIZE_X = 848;
constexpr unsigned int SCREEN_SIZE_Y = 480;

constexpr unsigned int FIELD_DIMENSION = 4;
constexpr unsigned int BLOCK_DRAW_SIZE = 30;
constexpr unsigned int BLOCK_PADDING = 5;
constexpr unsigned int FIELD_OFFSET_CENTERED_X =
	(SCREEN_SIZE_X - (FIELD_DIMENSION * BLOCK_DRAW_SIZE + (FIELD_DIMENSION - 1) * BLOCK_PADDING)) / 2;
constexpr unsigned int FIELD_OFFSET_CENTERED_Y =
	(SCREEN_SIZE_Y - (FIELD_DIMENSION * BLOCK_DRAW_SIZE + (FIELD_DIMENSION - 1) * BLOCK_PADDING)) / 2;