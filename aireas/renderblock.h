#pragma once

#include "aireas_defs.h"
#include "raylib.h"
#include "field_graph.h"
#include <vector>

using std::tuple;

class RenderBlock; // Stupid forwards definition to please the C++ compiler.

struct RenderBlockLink {
	RenderBlock* block;
	size_t edge_idx;
};

class RenderBlock {
public:
	vector<RenderBlockLink> linked;
	bool linked_highlight = false;

	bool test_hover(Vector2 pos, float xoff, float yoff);

	void reset_linked();
	bool matches_block(Block* other);
	void add_link(size_t link_idx, RenderBlock* other);
	void assign_block(Block* other);
	const inline bool get_visibility() const {
		return visible;
	}
	void update_visibility(bool new_visibility);
	void update_position(dims new_dims);
	void update_color(Color new_color);
	void render(float xoff, float yoff, bool debug = false);
private:
	Block* block = nullptr;
	bool visible;
	int xg, yg, wg, hg;
	Rectangle rect;
	Color color;
};