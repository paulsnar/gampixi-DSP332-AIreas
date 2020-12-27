#pragma once
#include <tuple>
#include "raylib.h"

typedef std::tuple<int, int, int, int> dims;

constexpr unsigned int SCREEN_SIZE_X = 848;
constexpr unsigned int SCREEN_SIZE_Y = 480;

// If false - heuristic values are simply -1, 0 or 1
// If true - heuristic values are score delta * board complexity between the two players. This will cause the alpha-beta
//			 algorithm to walk way more subtrees, increasing RAM usage a lot.
constexpr bool USE_BETTER_NODE_VALUES = true;

constexpr unsigned int FIELD_DIMENSION = 4;
constexpr unsigned int BLOCK_DRAW_SIZE = 50;
constexpr unsigned int BLOCK_PADDING = 10;
constexpr unsigned int FIELD_OFFSET_CENTERED_X =
	(SCREEN_SIZE_X - (FIELD_DIMENSION * BLOCK_DRAW_SIZE + (FIELD_DIMENSION - 1) * BLOCK_PADDING)) / 2;
constexpr unsigned int FIELD_OFFSET_CENTERED_Y =
	(SCREEN_SIZE_Y - (FIELD_DIMENSION * BLOCK_DRAW_SIZE + (FIELD_DIMENSION - 1) * BLOCK_PADDING)) / 2;

constexpr unsigned int END_POPUP_W = 500;
constexpr unsigned int END_POPUP_H = 120;

constexpr unsigned int END_POPUP_X = (SCREEN_SIZE_X - END_POPUP_W) / 2;
constexpr unsigned int END_POPUP_Y = SCREEN_SIZE_Y - END_POPUP_H - 50;

constexpr Color BACKGROUND_COLOR = Color { 36, 36, 36, 255 };

constexpr double BLOCK_TWEEN_LENGTH_RECT = 0.25;
constexpr double BLOCK_TWEEN_LENGTH_COLOR = 0.15;
constexpr Color BLOCK_COLOR_OUTLINE = Color {134, 207, 154, 255};
constexpr Color BLOCK_COLOR_NORMAL = GREEN;
constexpr Color BLOCK_COLOR_HOVER = DARKGREEN;
constexpr Color BLOCK_COLOR_LINKED = BLUE;
constexpr Color BLOCK_COLOR_LINKEDHOVER = DARKBLUE;

enum UiState {
	PickFirst,
	PickSecond,
	Calculating,
	Finished
};

enum Language {
	English,
	Latvian
};

constexpr Language LANGUAGE = Language::Latvian;
constexpr auto STR_CALCULATING = LANGUAGE == Language::English ? "Calculating..." : "Aprekina...";
constexpr auto STR_CLEARING = LANGUAGE == Language::English ? "CLEARING MEMORY..." : "NOTIRA ATMINU...";
constexpr auto STR_YOU = LANGUAGE == Language::English ? "You" : "Tu";
constexpr auto STR_AI = LANGUAGE == Language::English ? "AI" : "Dators";
constexpr auto STR_YOUR_MOVE = LANGUAGE == Language::English ? "Your move" : "Tavs gajiens";
constexpr auto STR_END_LOST = LANGUAGE == Language::English ? "You lost!" : "Dators uzvareja!";
constexpr auto STR_END_DRAW = LANGUAGE == Language::English ? "Draw!" : "Neizskirts!";
constexpr auto STR_END_WON = LANGUAGE == Language::English ? "You won!" : "Tu uzvareji!";
constexpr auto STR_END_LOST_FLAIR = LANGUAGE == Language::English
? "Better luck next time" : "Nakamreiz paveiksies!";
constexpr auto STR_END_DRAW_FLAIR = LANGUAGE == Language::English
? "You were so close to winning" : "Tik tuvu, un tomer tik talu";
constexpr auto STR_END_WON_FLAIR = LANGUAGE == Language::English
? "I'll just mention that the first player has an advantage" : "Starp citu, but pirmajam ir vieglak :)";
constexpr auto STR_RESTART_HINT = LANGUAGE == Language::English
? "Click anywhere to continue" : "Noklikskini, lai turpinatu";
constexpr auto STR_ATTRIBUTION = LANGUAGE == Language::English
? "AIreas by Rudolfs Agris Stilve | 2020" : "AIreas, Rudolfs Agris Stilve | 2020.";