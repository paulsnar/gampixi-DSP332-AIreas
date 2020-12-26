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
#include <thread>
#include <chrono>

using std::tuple;
using std::min;
using std::max;

std::vector<RenderBlock> renderblocks;
StateTreeNode root = StateTreeNode(FIELD_DIMENSION);
auto current_state = std::ref(root);

auto ui_state = UiState::PickFirst;
auto ai_player = GamePlayer::Player1;
RenderBlock* pick_first = nullptr;
RenderBlock* pick_second = nullptr;

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

void draw_debug(StateTreeNode& tree_node) {
	char extra_label[16];
	sprintf_s(extra_label, 16, "%s", tree_node.value.get_current_player() == GamePlayer::Player1 ? "MAX" : "MIN");
	DrawText(extra_label, 5, 5, 12, RED);

	unsigned int edge_index = 0;
	auto edges = tree_node.value.get_field().get_valid_edges();
	for (size_t i = 0; i < edges.size(); i++) {
		Edge& edge = edges.at(i).get();
		int x1, y1, w1, h1;
		int x2, y2, w2, h2;
		std::tie(x1, y1, w1, h1) = block_to_draw_dimensions(edge.get_first()->get_dimensions());
		std::tie(x2, y2, w2, h2) = block_to_draw_dimensions(edge.get_second()->get_dimensions());

		x1 += w1 / 2;
		y1 += h1 / 2;
		x2 += w2 / 2;
		y2 += h2 / 2;

		x1 += FIELD_OFFSET_CENTERED_X;
		x2 += FIELD_OFFSET_CENTERED_X;
		y1 += FIELD_OFFSET_CENTERED_Y;
		y2 += FIELD_OFFSET_CENTERED_Y;

		DrawLine(x1, y1, x2, y2, DARKBLUE);

		char edge_label[16];

		bool node_evaluated = tree_node.get_parsed(i);
		int path_value = 0;
		if (node_evaluated) {
			path_value = tree_node.get_child(i).node_value;
			sprintf_s(edge_label, 16, "%u/%d", edge_index, path_value);
		} else {
			sprintf_s(edge_label, 16, "%u/N", edge_index);
		}

		DrawText(edge_label, (x1+x2)/2, (y1+y2)/2, 12, RED);

		edge_index++;
	}
}

void update_renderblocks(Field& field) {
	for (size_t i = 0; i < field.get_blocks_size(); i++) {
		if (renderblocks.size() <= i) {
			renderblocks.push_back(RenderBlock());
		}
		Block* b = field.get_block(i);
		renderblocks[i].linked_highlight = false;
		renderblocks[i].assign_block(b);
		renderblocks[i].reset_linked();
		renderblocks[i].update_visibility(b->get_active());
		if (b->get_active()) {
			renderblocks[i].update_position(b->get_dimensions());
			renderblocks[i].update_color(GREEN);
		}
	}

	// Add links to newly updated renderblocks that will be used to handle move input.
	// Basically - every block gets a link to the move that can be performed from it
	// + the other renderblock this move points to. This way we can easily highlight whatever we want.
	auto edges = field.get_valid_edges();
	for (size_t i = 0; i < edges.size(); i++) {
		RenderBlock* first = nullptr;
		RenderBlock* second = nullptr;
		for (auto& rb : renderblocks) {
			if (rb.matches_block(edges.at(i).get().get_first())) {
				first = &rb;
			} else if (rb.matches_block(edges.at(i).get().get_second())) {
				second = &rb;
			}
			if (first != nullptr && second != nullptr) {
				break;
			}
		}
		if (first == nullptr || second == nullptr) {
			std::cout << "Edge couldn't find both renderblocks, pray that you won't crash." << std::endl;
			continue;
		}
		first->add_link(i, second);
		second->add_link(i, first);
	}
}

void render_renderblocks(float x_offset, float y_offset) {
	for (auto& b : renderblocks) {
		b.render(x_offset, y_offset, false);
	}
}

int total_human_wins = 0;
int total_ai_wins = 0;

void render_score(GameState& gameState) {
	static char score_text[50] = "kappa123";

	int human_score = ai_player == GamePlayer::Player1 ? gameState.get_score(GamePlayer::Player2) : gameState.get_score(GamePlayer::Player1);
	int ai_score = ai_player == GamePlayer::Player2 ? gameState.get_score(GamePlayer::Player2) : gameState.get_score(GamePlayer::Player1);

	sprintf_s(score_text, 50, "(%d) %d %s | %s %d (%d)", total_human_wins, human_score, STR_YOU, STR_AI, ai_score, total_ai_wins);
	int t_width = MeasureText(score_text, 24);
	int t_x = (SCREEN_SIZE_X - t_width) / 2;

	DrawText(score_text, t_x+1, 20+1, 24, DARKGRAY); // Shadow
	DrawText(score_text, t_x, 20, 24, LIGHTGRAY);

	if (ai_player != gameState.get_current_player()) {
		int tg_width = MeasureText(STR_YOUR_MOVE, 18);
		int tg_x = (SCREEN_SIZE_X - tg_width) / 2;
		DrawText(STR_YOUR_MOVE, tg_x + 1, 50 + 1, 18, DARKGRAY); // Shadow
		DrawText(STR_YOUR_MOVE, tg_x, 50, 18, LIGHTGRAY);
	}
	
}

void process_inputs() {
	switch (ui_state) {
	case UiState::Calculating:
		break;
	case UiState::PickFirst:
		for (auto& b : renderblocks) {
			if (!b.get_visibility()) continue;
			if (b.test_hover(GetMousePosition(), FIELD_OFFSET_CENTERED_X, FIELD_OFFSET_CENTERED_Y)) {
				b.update_color(BLOCK_COLOR_HOVER);
				if (IsMouseButtonPressed(0)) {
					pick_first = &b;
					for (auto& l : b.linked) {
						l.block->linked_highlight = true;
					}
					ui_state = UiState::PickSecond;
				}
			}
			else {
				b.update_color(BLOCK_COLOR_NORMAL);
			}
		}
		break;
	case UiState::PickSecond:
		for (auto& b : renderblocks) {
			if (!b.get_visibility()) continue;
			if (b.test_hover(GetMousePosition(), FIELD_OFFSET_CENTERED_X, FIELD_OFFSET_CENTERED_Y)) {
				if (&b == pick_first) {
					b.update_color(BLOCK_COLOR_LINKEDHOVER);
				}
				else {
					b.update_color(b.linked_highlight ? BLOCK_COLOR_LINKEDHOVER : BLOCK_COLOR_HOVER);
				}

				if (IsMouseButtonPressed(0)) {
					if (b.linked_highlight) {
						// Find the correct link and execute move.
						for (auto &other : pick_first->linked) {
							if (other.block == &b) {
								// This is the correct edge!
								pick_second = &b;
								break;
							}
						}
						if (pick_second == nullptr) {
							std::cout << "Something really bad happened, couldn't find correct edge." << std::endl;
							goto deselect;
						}
						else {
							ui_state = UiState::Calculating;
						}
					}
					else if (&b == pick_first) {
					deselect:
						// Deselect selection
						for (auto& l : pick_first->linked) {
							l.block->linked_highlight = false;
						}
						pick_first = nullptr;
						ui_state = UiState::PickFirst;
						break;
					}
					else {
						// Change first selection
						for (auto& l : pick_first->linked) {
							l.block->linked_highlight = false;
						}
						pick_first = &b;
						for (auto& l : b.linked) {
							l.block->linked_highlight = true;
						}
						ui_state = UiState::PickSecond;
					}
				}
			}
			else {
				if (&b == pick_first) {
					b.update_color(BLOCK_COLOR_LINKEDHOVER);
				}
				else {
					b.update_color(b.linked_highlight ? BLOCK_COLOR_LINKED : BLOCK_COLOR_NORMAL);
				}
			}
		}
		break;
	case UiState::Finished:
		if (IsMouseButtonPressed(0)) {
			BeginDrawing();
			DrawText(STR_CLEARING, 23, SCREEN_SIZE_Y - 35, 12, LIGHTGRAY);
			EndDrawing();
			root = StateTreeNode(FIELD_DIMENSION);
			current_state = std::ref(root);
			update_renderblocks(current_state.get().value.get_field());
			ai_player = ai_player == GamePlayer::Player1 ? GamePlayer::Player2 : GamePlayer::Player1;
			ui_state = UiState::PickFirst;
		}
		break;
	}
}

void test_after_move() {
	if (current_state.get().value.get_status() == GameStatus::Draw) {
		total_human_wins += 1;
		total_ai_wins += 1;
	}
	else if (current_state.get().value.get_status() == GameStatus::Player1Victory) {
		if (ai_player == GamePlayer::Player1) {
			total_ai_wins += 1;
		}
		else {
			total_human_wins += 1;
		}
	}
	else if (current_state.get().value.get_status() == GameStatus::Player2Victory) {
		if (ai_player == GamePlayer::Player2) {
			total_ai_wins += 1;
		}
		else {
			total_human_wins += 1;
		}
	}
}

void perform_ai_move() {
	srand(time(NULL) + std::hash<std::thread::id>{}(std::this_thread::get_id()));
	if (&current_state.get() == &root) {
		// First move!
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::vector<size_t> best_moves;
		if (FIELD_DIMENSION == 4) {
			best_moves = {3,5,7,14,11,18,17,19};
		}
		current_state = std::ref(current_state.get().get_child(best_moves[rand() % best_moves.size()]));
		test_after_move();
	}
	else {
		walk_tree_with_alphabeta(current_state.get(), INT_MIN, INT_MAX);
		// Perform one from the best moves
		current_state = std::ref(current_state.get().best_child());
		test_after_move();
	}

	update_renderblocks(current_state.get().value.get_field());
	ui_state = UiState::PickFirst;
}

int main(int argc, char* argv[])
{
	int screenWidth = SCREEN_SIZE_X;
	int screenHeight = SCREEN_SIZE_Y;
	InitWindow(screenWidth, screenHeight, "AIreas | Rūdolfs Agris Stilve");
	SetTargetFPS(60);

	renderblocks.reserve(FIELD_DIMENSION*FIELD_DIMENSION);
	update_renderblocks(current_state.get().value.get_field());

	std::srand(std::time(NULL));

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		if (current_state.get().value.get_status() != GameStatus::Playing) {
			ui_state = UiState::Finished;
		}

		if (ui_state != UiState::Calculating) {
			process_inputs();
			bool is_ai_move = current_state.get().value.get_current_player() == ai_player;
			if (!is_ai_move) {
				if (pick_first != nullptr && pick_second != nullptr) {
					// Both blocks have been picked - perform move!
					for (auto& other : pick_first->linked) {
						if (other.block == pick_second) {
							// This is the correct move to execute.
							current_state = std::ref(current_state.get().get_child(other.edge_idx));
							test_after_move();
							update_renderblocks(current_state.get().value.get_field());
							pick_first = nullptr;
							pick_second = nullptr;
							ui_state = UiState::PickFirst;
							break;
						}
					}
				}
			}
			else {
				if (current_state.get().value.get_status() == GameStatus::Playing) {
					ui_state = UiState::Calculating;
					std::cout << "Starting AI move..." << std::endl;
					pick_first = nullptr;
					pick_second = nullptr;
					auto ai_thread = std::thread (perform_ai_move);
					ai_thread.detach();
				}
			}
		}

		BeginDrawing();
		ClearBackground(BACKGROUND_COLOR);

		render_renderblocks(FIELD_OFFSET_CENTERED_X, FIELD_OFFSET_CENTERED_Y);
		render_score(current_state.get().value);

		if (ui_state == UiState::Calculating) {
			// Draw calculating UI
			Vector2 ringCenter;
			draw_debug(current_state.get());
			ringCenter.x = 30;
			ringCenter.y = SCREEN_SIZE_Y - 30;
			double angle1 = -360.0 * GetTime() * 2;
			double angle2 = -360.0 * (GetTime() + std::sin(GetTime() * 5) * 0.1) * 2 + 135;
			DrawRing(ringCenter, 8, 12, min(angle1, angle2), max(angle1, angle2), 16, LIGHTGRAY);
			DrawText(STR_CALCULATING, 52, SCREEN_SIZE_Y - 35, 12, LIGHTGRAY);
		}
		if (ui_state == UiState::Finished) {
			// Draw finish UI, let restart
			DrawRectangle(END_POPUP_X, END_POPUP_Y, END_POPUP_W, END_POPUP_H, DARKGRAY);
			auto gs = current_state.get().value.get_status();
			int final_text = 0;
			if (gs == GameStatus::Player1Victory) {
				if (ai_player == GamePlayer::Player1) {
					// AI won
					final_text = 0;
				} else {
					// Human won
					final_text = 1;
				}
			} else if(gs == GameStatus::Player2Victory) {
				if (ai_player == GamePlayer::Player2) {
					// AI won
					final_text = 0;
				} else {
					// Human won
					final_text = 1;
				}
			} else {
				// Draw
				final_text = 2;
			}
			if (final_text == 0) {
				DrawText(STR_END_LOST, END_POPUP_X+20, END_POPUP_Y + 20, 36, RAYWHITE);
				DrawText(STR_END_LOST_FLAIR, END_POPUP_X + 20, END_POPUP_Y + 60, 16, RAYWHITE);
			} else if (final_text == 1) {
				DrawText(STR_END_WON, END_POPUP_X + 20, END_POPUP_Y + 20, 36, RAYWHITE);
				DrawText(STR_END_WON_FLAIR, END_POPUP_X + 20, END_POPUP_Y + 60, 16, RAYWHITE);
			} else if (final_text == 2) {
				DrawText(STR_END_DRAW, END_POPUP_X + 20, END_POPUP_Y + 20, 36, RAYWHITE);
				DrawText(STR_END_DRAW_FLAIR, END_POPUP_X + 20, END_POPUP_Y + 60, 16, RAYWHITE);
			}
			DrawText(STR_RESTART_HINT, END_POPUP_X + 20, END_POPUP_Y + 80, 16, RAYWHITE);
		}

		EndDrawing();
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	std::quick_exit(0); // The destructors take a long time, just let the OS clean up this mess :/
	return 0;
}