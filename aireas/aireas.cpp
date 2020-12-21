// aireas.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "field_graph.h"
#include "raylib.h"

void draw_field(const Field& field) {
	for (int i = 0; i < field.get_blocks_size(); i++) {
		const Block* b = field.get_block(i);
		//DrawRectangle(, int posY, int width, int height, Color color);
	}
}

void draw_field_edges(const Field& field) {

}

int main(int argc, char* argv[])
{
	Field field = Field(3);

	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

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

		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}