#include "pch.h"
#include "field_graph.h"

void Edge::calculate_valid()
{
}

Field::Field(size_t n)
{
	this->blocks_size = n * n;
	this->blocks = new Block[blocks_size];
}

Field::~Field()
{
	delete[] this->blocks;
}

void Field::merge_edge(Edge& edge)
{
}

void Field::calculate_valid_edges()
{
}

vector<Edge> Field::get_valid_edges()
{
	vector<Edge> valid_edges;

	for (auto e : this->edges) {
		if (e.get_valid) {
			valid_edges.push_back(e);
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
}

Block::Block(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}
