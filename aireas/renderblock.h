#pragma once

#include "aireas_defs.h"
#include "raylib.h"

using std::tuple;

class RenderBlock {
public:
	void update_visibility(bool new_visibility);
	void update_position(dims new_dims);
	void update_color(Color new_color);
	void render(float xoff, float yoff);
private:
	bool visible;
	int xg, yg, wg, hg;
	Rectangle rect;
	Color color;
};