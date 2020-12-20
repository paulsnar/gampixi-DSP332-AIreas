#pragma once
#include <vector>

using std::vector;
using std::pair;

class Block {
public:
	Block();
	Block(int x, int y, int w, int h);
private:
	int x, y;
	int w, h;
};

class Edge {
public:
	void calculate_valid();

	inline bool get_valid() {
		return this->valid;
	}
	inline bool operator==(const Edge& rhs) {
		// This checks if first and second are equal between pairs
		if (this->blocks == rhs.blocks) {
			return true;
		}
		// 1.first == 2.second and vice-versa is also equal since the graph isn't oriented
		else if (this->blocks.first == rhs.blocks.second && this->blocks.second == rhs.blocks.first) {
			return true;
		}
		return false;
	}
private:
	bool valid; // Cache the validity of this move so we don't have to recalculate every time we access the graph
	pair<Block*, Block*> blocks; // The order in the pair shouldn't matter
};

class Field {
public:
	Field(size_t n); // Initializes a field of size n*n
	Field(const Field& other);
	~Field();
	void merge_edge(Edge& edge); // Basically performs the move => removes edge, merges blocks, merges overlapping edges
	void calculate_valid_edges();
	vector<Edge> get_valid_edges(); // Basically returns every valid mode
private:
	// Since the field can change around and we use pointers to blocks, holding them in a vector just calls for disaster
	Block* blocks; // Dynamic array of blocks, allocated when field is created
	size_t blocks_size;
	vector<Edge> edges;
};