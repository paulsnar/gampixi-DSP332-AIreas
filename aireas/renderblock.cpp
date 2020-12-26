#include "pch.h"
#include "renderblock.h"

bool RenderBlock::matches_block(Block* other)
{
	if (block != nullptr) {
		if (!block->get_active()) {
			return false;
		}
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

void RenderBlock::add_link(size_t link_idx, RenderBlock* other)
{
	// Don't check if the link is valid, that's the caller's responsibiliy.
	RenderBlockLink rbl;
	rbl.block = other;
	rbl.edge_idx = link_idx;

	linked.push_back(rbl);
}

void RenderBlock::assign_block(Block * other)
{
	block = other;
}

void RenderBlock::update_visibility(bool new_visibility)
{
	if (new_visibility == false) {
		rect.x = 0;
		rect.y = 0;
		rect.width = 0;
		rect.height = 0;
	}
	visible = new_visibility;
}

void RenderBlock::update_position(dims new_dims)
{
	// TODO: Tweening
	std::tie(xg, yg, wg, hg) = new_dims;

	prev_rect = rect;
	prev_rect_time = GetTime();
	rect.x = xg * (BLOCK_DRAW_SIZE + BLOCK_PADDING);
	rect.y = yg * (BLOCK_DRAW_SIZE + BLOCK_PADDING);
	rect.width = wg * BLOCK_DRAW_SIZE + (wg - 1)*BLOCK_PADDING;
	rect.height = hg * BLOCK_DRAW_SIZE + (hg - 1)*BLOCK_PADDING;
}

void RenderBlock::update_color(Color new_color)
{
	// TODO: Tweening
	if (color.r == new_color.r && color.g == new_color.g && color.b == new_color.b && color.a == new_color.a) return;
	prev_color = color;
	prev_color_time = GetTime();
	color = new_color;
}

void RenderBlock::render(float xoff, float yoff, bool debug)
{
	//if (!visible) return;

	Rectangle tweened_rect;
	float rtt = (GetTime() - prev_rect_time) / BLOCK_TWEEN_LENGTH_RECT;
	if (rtt > 1.0) rtt = 1.0;
	tweened_rect.x = EaseBackOut(rtt, prev_rect.x, rect.x - prev_rect.x, 1);
	tweened_rect.y = EaseBackOut(rtt, prev_rect.y, rect.y - prev_rect.y, 1);
	tweened_rect.width = EaseBackOut(rtt, prev_rect.width, rect.width - prev_rect.width, 1);
	tweened_rect.height = EaseBackOut(rtt, prev_rect.height, rect.height - prev_rect.height, 1);

	float ctt = (GetTime() - prev_color_time) / BLOCK_TWEEN_LENGTH_COLOR;
	if (ctt > 1.0) ctt = 1.0;
	Color tweened_color;
	tweened_color.r = EaseSineInOut(ctt, (float)prev_color.r, (float)color.r - (float)prev_color.r, 1);
	tweened_color.g = EaseSineInOut(ctt, (float)prev_color.g, (float)color.g - (float)prev_color.g, 1);
	tweened_color.b = EaseSineInOut(ctt, (float)prev_color.b, (float)color.b - (float)prev_color.b, 1);
	tweened_color.a = EaseSineInOut(ctt, (float)prev_color.a, (float)color.a - (float)prev_color.a, 1);

	Rectangle final_rect = tweened_rect;
	final_rect.x += xoff;
	final_rect.y += yoff;
	Rectangle outline_rect = final_rect;
	outline_rect.x -= 1;
	outline_rect.y -= 1;
	outline_rect.height += 2;
	outline_rect.width += 2;
	DrawRectangleRec(outline_rect, BLOCK_COLOR_OUTLINE);
	DrawRectangleRec(final_rect, tweened_color);

	if (debug) {
		char debug_text[8];
		sprintf_s(debug_text, 8, "%d", linked.size());
		
		DrawText(debug_text, final_rect.x, final_rect.y, 10, DARKBLUE);
	}
}
