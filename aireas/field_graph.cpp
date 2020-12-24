#include "pch.h"
#include "field_graph.h"

Edge::Edge()
{
	this->set_first(nullptr);
	this->set_second(nullptr);
}

Edge::Edge(Block * first, Block * second)
{
	this->set_first(first);
	this->set_second(second);
	this->calculate_valid();
}

void Edge::calculate_valid()
{
	this->valid = this->get_first()->combine_test(*this->get_second());
}

Field::Field(size_t n)
{
	this->blocks_size = n * n;
	this->blocks_size_n = n;
	this->blocks = new Block[this->blocks_size];

	// Assign default-grid layout values to the blocks.
	for (size_t y = 0; y < n; y++) {
		for (size_t x = 0; x < n; x++) {
			this->blocks[y*n + x] = Block(x, y, 1, 1);
		}
	}

	// Generate initial edges for the grid layout.
	for (size_t y = 0; y < n; y++) {
		for (size_t x = 0; x < n; x++) {
			if (x != n - 1)
				this->edges.push_back(Edge(&this->blocks[y*n + x], &this->blocks[y*n + x + 1]));
			if (y != n - 1)
				this->edges.push_back(Edge(&this->blocks[y*n + x], &this->blocks[(y + 1)*n + x]));
		}
	}
}

Field::Field(const Field & other)
{
	Field(other.blocks_size_n);

	for (size_t i = 0; i < this->blocks_size; i++)
		this->blocks[i] = other.blocks[i];

	// Copy edges, preserving order by pointer black magic (well there isn't actually anything
	// very black magic to this, we're just certain that the block arrangement in memory is the same)
	this->edges = vector<Edge>(other.edges);
	ptrdiff_t diff = this->blocks - other.blocks;

	for (auto& e : this->edges) {
		e.set_first(e.get_first() + diff);
		e.set_second(e.get_second() + diff);
	}
}

Field::~Field()
{
	delete[] this->blocks;
}

const Block * Field::get_block(size_t at) const
{
	return &this->blocks[at];
}

const Block * Field::get_block(int x, int y) const
{
	return this->get_block(y*this->blocks_size_n + x);
}

bool Field::merge_edge(Edge& edge)
{
	// 1) Merge blocks connected by edge

	// Merge, keeping the 1st pair element, essentially making the 2nd block garbage memory
	// Assuming that the edge validity was checked beforehand!
	if (edge.get_first()->combine_to(*edge.get_second()) == false) {
		return false;
	}
	edge.get_second()->set_active(false);
	
	// 1.2) Update edge pointers to no longer point to invalidated block

	for (auto& e : this->edges) {
		if (&e == &edge) continue; // This is the edge we are merging and it will be gone either way.

		if (e.get_first() == edge.get_second()) {
			e.set_first(edge.get_first());
		} else if (e.get_second() == edge.get_second()) {
			e.set_second(edge.get_first());
		}
	}

	// 2) Remove edge which was the move done

	this->edges.erase(std::remove(this->edges.begin(), this->edges.end(), edge));

	// 3) Remove overlapping edges (duplicates)
	
	sort(this->edges.begin(), this->edges.end());
	auto new_end = unique(this->edges.begin(), this->edges.end());
	this->edges.resize(std::distance(this->edges.begin(), new_end));

	return true;
}

void Field::update_edge_validity()
{
	// Updates edge validity so get_valid_edges() returns relevant stuff

}

const vector<reference_wrapper<Edge>> Field::get_valid_edges()
{
	vector<reference_wrapper<Edge>> valid_edges;

	for (auto& e : this->edges) {
		if (e.get_valid()) {
			auto ref = std::ref(e);
			valid_edges.push_back(ref);
		}
	}

	return valid_edges;
}

Block::Block()
{
	this->x = 0;
	this->y = 0;
	this->w = 0;
	this->h = 0;
	this->active = false;
}

Block::Block(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->active = true;
}

bool Block::combine_to(const Block & other)
{
	if (this->x == other.x && this->w == other.w) {
		// Since the X components are equal, we are merging along Y axis
		// Also need to check positioning
		// Check in the axis direction
		// ------------------>
		// [ 4  ] [1] [  6    ]
		//			   5 = 4+1
		pair<const Block&, const Block&> cmp = (this->y <= other.y) ?
			make_pair(*this, other) : make_pair(other, *this);

		if (cmp.second.y == cmp.first.y + cmp.first.h) {
			// Combine and get the hell outta here
			this->y = min(this->y, other.y);
			this->h = this->h + other.h;
			return true;
		}

	} else if (this->y == other.y && this->h == other.h) {
		// Since the Y components are equal, we are merging along X axis
		pair<const Block&, const Block&> cmp = (this->x <= other.x) ?
			make_pair(*this, other) : make_pair(other, *this);

		if (cmp.second.x == cmp.first.x + cmp.first.x) {
			// Combine and get the hell outta here
			this->x = min(this->x, other.x);
			this->w = this->w + other.w;
			return true;
		}
	}

	return false;
}

const bool Block::combine_test(const Block & other) const
{
	if (this->x == other.x && this->w == other.w) {
		pair<const Block&, const Block&> cmp = (this->y <= other.y) ?
			make_pair(*this, other) : make_pair(other, *this);

		if (cmp.second.y == cmp.first.y + cmp.first.h) {
			return true;
		}

	}
	else if (this->y == other.y && this->h == other.h) {
		// Since the Y components are equal, we are merging along X axis
		pair<const Block&, const Block&> cmp = (this->x <= other.x) ?
			make_pair(*this, other) : make_pair(other, *this);

		if (cmp.second.x == cmp.first.x + cmp.first.x) {
			return true;
		}
	}

	return false;
}

const tuple<int, int, int, int> Block::get_dimensions() const
{
	return std::make_tuple(x, y, w, h);
}
