#include "pch.h"
#include "renderblock.h"

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
	color = new_color;
}

void RenderBlock::render()
{
	DrawRectangleRec(rect, color);
}
