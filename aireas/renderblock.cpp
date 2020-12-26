#include "pch.h"
#include "renderblock.h"

bool RenderBlock::matches_block(Block* other)
{
	if (block != nullptr) {
		return other == block;
	}

	int xb, yb, wb, hb;
	std::tie(xb, yb, wb, hb) = other->get_dimensions();

	if (xb != xg) return false;
	if (yb != yg) return false;
	if (wb != wg) return false;
	if (hb != hg) return false;
	return true;
}

bool RenderBlock::test_hover(Vector2 pos, float xoff, float yoff)
{
	Vector2 finalPos = pos;
	finalPos.x -= xoff;
	finalPos.y -= yoff;
	//finalPos.x -= 10;
	//finalPos.y -= 10;
	return CheckCollisionPointRec(finalPos, rect);
}

void RenderBlock::reset_linked()
{
	linked = vector<RenderBlockLink>();
}

void RenderBlock::add_link(Edge& link, RenderBlock* other)
{
	// Don't check if the link is valid, that's the caller's responsibiliy.
	RenderBlockLink rbl;
	rbl.block = other;
	rbl.edge = &link;

	linked.push_back(rbl);
}

void RenderBlock::assign_block(Block * other)
{
	block = other;
}

void RenderBlock::update_visibility(bool new_visibility)
{
	visible = new_visibility;
}

void RenderBlock::update_position(dims new_dims)
{
	// TODO: Tweening
	std::tie(xg, yg, wg, hg) = new_dims;

	rect.x = xg * (BLOCK_DRAW_SIZE + BLOCK_PADDING);
	rect.y = yg * (BLOCK_DRAW_SIZE + BLOCK_PADDING);
	rect.width = wg * BLOCK_DRAW_SIZE + (wg - 1)*BLOCK_PADDING;
	rect.height = hg * BLOCK_DRAW_SIZE + (hg - 1)*BLOCK_PADDING;
}

void RenderBlock::update_color(Color new_color)
{
	// TODO: Tweening
	if (color.r == new_color.r && color.g == new_color.g && color.b == new_color.b && color.a == new_color.a) return;
	color = new_color;
}

void RenderBlock::render(float xoff, float yoff, bool debug)
{
	if (!visible) return;
	Rectangle final_rect = rect;
	final_rect.x += xoff;
	final_rect.y += yoff;
	DrawRectangleRec(final_rect, color);

	if (debug) {
		char debug_text[8];
		sprintf_s(debug_text, 8, "%d", linked.size());
		
		DrawText(debug_text, final_rect.x, final_rect.y, 10, DARKBLUE);
	}
}
