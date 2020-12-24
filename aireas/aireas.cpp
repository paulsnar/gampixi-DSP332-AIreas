// aireas.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "field_graph.h"
#include "raylib.h"
#include <tuple>
#include <cstdlib>
#include <ctime>

constexpr unsigned int BLOCK_DRAW_SIZE = 30;
constexpr unsigned int BLOCK_PADDING = 5;

using std::tuple;

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

int main(int argc, char* argv[])
{
	Field field = Field(3);
	std::srand(std::time(NULL));

	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "AIreas | Rūdolfs Agris Stilve");

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(RAYWHITE);

		draw_field(field);
		draw_field_edges(field);

		DrawText("boo hoo this is very empty", 190, 200, 20, LIGHTGRAY);

		EndDrawing();

		if (IsKeyPressed(KEY_SPACE)) {
			auto edges = field.get_valid_edges();
			field.merge_edge(edges[rand() % edges.size()].get());
		}
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}