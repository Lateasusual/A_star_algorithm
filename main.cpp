#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <set>
#include <map>
#include "Graph.h"

void print_route(GridGraph grid, std::map<GridNode*, GridNode*> cameFrom, GridNode* endNode) {
	std::vector<GridNode*> path;
	path.push_back(endNode);
	while (cameFrom.find(endNode) != cameFrom.end()) {
		endNode = cameFrom.at(endNode);
		path.push_back(endNode);
	}

	wchar_t square_empty = L' ';
	wchar_t square_filled = L'\u25AE';

	std::vector<std::vector<wchar_t>*> out;
	for (int i = 0; i < grid.size_y; i++) {
		std::vector<wchar_t>* row = new std::vector<wchar_t>;
		out.push_back(row);
		for (int j = 0; j < grid.size_x; j++) {
			if (grid.get(j, i)->blocked) {
				row->push_back(square_filled);
			}
			else {
				row->push_back(square_empty);
			}
		}
	}

	wchar_t* temp = out[path[0]->y]->data();
	temp[path[0]->x] = L'E'; // End marker
	temp = out[endNode->y]->data();
	temp[endNode->x] = L'S'; // Start marker

	for (int i = 1; i < path.size() - 1; i++) {
		GridNode* curNode = path[i];
		int dx = curNode->x - cameFrom.at(curNode)->x;
		int dy = curNode->y - cameFrom.at(curNode)->y;
		wchar_t* row = out[curNode->y]->data();

		if (dx == 0) {
			row[curNode->x] = L'|';
		}
		else if (dy == 0) {
			row[curNode->x] = L'-';
		}
		else if (dx == dy) {
			row[curNode->x] = L'\\';
		}
		else {
			row[curNode->x] = L'/';
		}

	}

	for (std::vector<wchar_t>* row : out) {
		for (wchar_t c : *row) {
			std::wcout << c;
		}
		std::wcout << std::endl;
	}
}

void A_star(GridGraph grid, int start_x, int start_y, int end_x, int end_y) {

	std::set<GridNode*> open; // Nodes we're busy exploring
	std::map<GridNode*, GridNode*> cameFrom;

	GridNode* n_start = grid.get(start_x, start_y);
	GridNode* n_end = grid.get(end_x, end_y);

	n_start->g = 0.f;
	n_start->f = 0.f;

	open.insert(n_start);

	// Set hCost for all nodes - euclidean distance
	for (int i = 0; i < grid.size_x; i++) {
		for (int j = 0; j < grid.size_y; j++) {
			// double temp_h = std::sqrt(std::pow((i - end_x),2) + std::pow((j - end_y), 2));
			double temp_h = std::abs(i - end_x) + std::abs(j - end_y);
			grid.get(i, j)->h = temp_h;
		}
	}

	while (!open.empty()) {
		GridNode* current{};

		// Find the node in the open set with the lowest fCost
		for (GridNode* n : open) {
			if (current == nullptr || n->f < current->f) {
				current = n;
			}
		}

		// If we're at the end
		if (current == n_end) {
			/* XXX TODO: Reconstruct path from end node to start */
			std::wcout << L"Path found:" << std::endl;
			print_route(grid, cameFrom, current);
			return;
		}

		// Pop the current node from the set of open nodes
		open.erase(current);
		
		// For each neighbour of our GridNode
		int x = current->x;
		int y = current->y;

		// Loop through all the neighbours of the current node
		for (int i = -1; i < 2; i++) { // Nodes left and right
			for (int j = -1; j < 2; j++) { // Nodes up and down

				// Don't test the current node
				if (i == 0 && j == 0) {
					continue;
				}

				// Node is blocked or out of bounds
				if (grid.isBlocked(x + i, y + j)) {
					continue;
				}
				GridNode* neighbour = grid.get(x + i, y + j);
				double distance;

				if (i == j) {
					distance = 1.41421f; //sqrt 2
				}
				else {
					distance = 1.f;
				}
				double temp_gScore = current->g + distance;

				if (temp_gScore <= neighbour->g) {
					cameFrom.insert_or_assign(neighbour, current);
					neighbour->g = temp_gScore;
					neighbour->f = temp_gScore + neighbour->h;

					open.insert(neighbour);
				}
			}
		}
	}

	std::wcout << L"A* Pathfinding failed" << std::endl;
	print_route(grid, cameFrom, grid.get(start_x, start_y));
}

int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT); // For consoles that have unicode fonts
	GridGraph graph(20, 20);

	for (int i = 0; i < 9; i++) {
		graph.get(4, i)->blocked = true;
	}

	for (int i = 1; i < 18; i++) {
		graph.get(8, i)->blocked = true;
	}

	for (int i = 0; i < 19; i++) {
		graph.get(i, 12)->blocked = true;
	}

	A_star(graph, 1, 1, 2, 14);
}