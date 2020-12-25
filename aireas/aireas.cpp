// aireas.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "aireas_defs.h"
#include "renderblock.h"
#include "field_graph.h"
#include "state_tree.h"
#include "gamestate.h"
#include "raylib.h"
#include <tuple>
#include <cstdlib>
#include <ctime>
#include <vector>

using std::tuple;

std::vector<RenderBlock> renderblocks;
StateTreeNode root = StateTreeNode(FIELD_DIMENSION);
StateTreeNode& current_state = root;

tuple<int, int, int, int> block_to_draw_dimensions(tuple<int, int, int, int> b) {
	int x, y, w, h;
	std::tie(x, y, w, h) = b;
	x *= BLOCK_DRAW_SIZE + BLOCK_PADDING;
	y *= BLOCK_DRAW_SIZE + BLOCK_PADDING;
	w = w * BLOCK_DRAW_SIZE + (w - 1)*BLOCK_PADDING;
	h = h * BLOCK_DRAW_SIZE + (h - 1)*BLOCK_PADDING;
	return std::make_tuple(x, y, w, h);
}

void draw_field(const Field& field) {
	for (size_t i = 0; i < field.get_blocks_size(); i++) {
		const Block* b = field.get_block(i);
		if (b->get_active() == false) continue;

		int x, y, w, h;
		std::tie(x, y, w, h) = block_to_draw_dimensions(b->get_dimensions());

		DrawRectangle(x, y, w, h, GREEN);

		char block_label[4];
		sprintf_s(block_label, 4, "%u", i);
		DrawText(block_label, x, y, 12, DARKGREEN);
	}
}

void draw_field_edges(Field& field) {
	unsigned int edge_index = 0;
	for (auto e : field.get_valid_edges()) {
		Edge& edge = e.get();
		int x1, y1, w1, h1;
		int x2, y2, w2, h2;
		std::tie(x1, y1, w1, h1) = block_to_draw_dimensions(edge.get_first()->get_dimensions());
		std::tie(x2, y2, w2, h2) = block_to_draw_dimensions(edge.get_second()->get_dimensions());

		x1 += w1 / 2;
		y1 += h1 / 2;
		x2 += w2 / 2;
		y2 += h2 / 2;

		DrawLine(x1, y1, x2, y2, DARKBLUE);

		char edge_label[4];
		sprintf_s(edge_label, 4, "%u", edge_index);
		DrawText(edge_label, (x1+x2)/2, (y1+y2)/2, 12, RED);

		edge_index++;
	}
}

void update_renderblocks(Field& field) {
	for (size_t i = 0; i < field.get_blocks_size(); i++) {
		if (renderblocks.size() <= i) {
			renderblocks.push_back(RenderBlock());
		}
		const Block* b = field.get_block(i);
		renderblocks[i].update_visibility(b->get_active());
		renderblocks[i].update_position(b->get_dimensions());
		renderblocks[i].update_color(GREEN);
	}
}

void render_renderblocks(float x_offset, float y_offset) {
	for (auto& b : renderblocks) {
		b.render(x_offset, y_offset);
	}
}

void render_score(GameState& gameState) {
	static char player1_text[32] = "YOU: 0";
	static char player2_text[32] = "AI: 0";

	if (gameState.get_status() == GameStatus::Playing) {
		sprintf_s(player1_text, 32, "YOU: %u", gameState.get_score(GamePlayer::Player1));
		sprintf_s(player2_text, 32, "AI: %u", gameState.get_score(GamePlayer::Player2));
	} else if (gameState.get_status() != GameStatus::Draw) {
		sprintf_s(player1_text, 32, "YOU %s: %u", 
			gameState.get_status() == GameStatus::Player1Victory ? "WIN" : "LOSE",
			gameState.get_score(GamePlayer::Player1));
		sprintf_s(player2_text, 32, "AI %s: %u",
			gameState.get_status() == GameStatus::Player2Victory ? "WIN" : "LOSE",
			gameState.get_score(GamePlayer::Player2));
	} else {
		sprintf_s(player1_text, 32, "YOU DRAW: %u", gameState.get_score(GamePlayer::Player1));
		sprintf_s(player2_text, 32, "AI DRAW: %u", gameState.get_score(GamePlayer::Player2));
	}

	DrawText(player1_text, SCREEN_SIZE_X / 2, 20, 24, DARKGRAY);
	DrawText(player2_text, SCREEN_SIZE_X / 2, 50, 24, DARKGRAY);
}

int main(int argc, char* argv[])
{
	std::cout << "Generate tree up to depth 5, because why the hell not." << std::endl;
	walk_tree_with_depth(current_state, 5);

	renderblocks.reserve(FIELD_DIMENSION*FIELD_DIMENSION);
	update_renderblocks(current_state.value.get_field());

	std::srand(std::time(NULL));

	int screenWidth = SCREEN_SIZE_X;
	int screenHeight = SCREEN_SIZE_Y;
	InitWindow(screenWidth, screenHeight, "AIreas | Rūdolfs Agris Stilve");

	SetTargetFPS(60);

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		if (IsKeyPressed(KEY_SPACE)) {
			current_state = current_state.get_child(rand() % current_state.get_child_count());
			/*auto edges = current_state.value.get_field().get_valid_edges();
			auto e = edges[rand() % edges.size()].get();
			states.push_back(states.back().perform_move(e));*/
			update_renderblocks(current_state.value.get_field());
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);

		//for (size_t i = 0; i < fields.size(); i++) {
			//draw_field(states.back().get_field());
			//draw_field_edges(states.back().get_field());
		//}
		render_renderblocks(FIELD_OFFSET_CENTERED_X, FIELD_OFFSET_CENTERED_Y);
		draw_field_edges(current_state.value.get_field());
		render_score(current_state.value);

		EndDrawing();
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}