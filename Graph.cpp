#include "Graph.h"
#include <iostream>
#include <iomanip>

GridGraph::GridGraph(int x, int y){
	data.resize(x * y);
	size_x = x;
	size_y = y;
	for (int i = 0; i < x * y; i++) {
		data[i] = new GridNode;
	}
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			get(i, j)->x = i;
			get(i, j)->y = j;
		}
	}
}

GridNode* GridGraph::get(int x, int y) {
	return data[x * size_x + y];
}

bool GridGraph::isBlocked(int x, int y)
{
	// All edges are considered walls
	if (x < 0 || y < 0 || x > size_x - 1 || y > size_y - 1) {
		return true;
	}
	else {
		return get(x, y)->blocked;
	}
}

void GridGraph::print() {
	for (int i = 0; i < size_x; i++) {
		for (int j = 0; j < size_y; j++) {
			GridNode* n = get(i, j);
			if (n->blocked) {
				std::cout << "[X]";
			}
			else {
				std::cout << std::setw(10) << std::left << (n->f);
			}
		}
		std::cout << std::endl;
	}
}
