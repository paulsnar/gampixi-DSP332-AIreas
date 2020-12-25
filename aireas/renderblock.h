#pragma once

#include "aireas_defs.h"
#include "raylib.h"

using std::tuple;

class RenderBlock {
public:
	void update_position(dims new_dims);
	void update_color(Color new_color);
	void render();
private:
	int xg, yg, wg, hg;
	Rectangle rect;
	Color color;
};