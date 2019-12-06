#pragma once
#include <vector>

typedef struct GridNode {
	// Variables needed for A*
	int x;
	int y;
	bool blocked = false;
	double f = FLT_MAX;
	double g = FLT_MAX;
	double h = 0.f;
};

class GridGraph {
private:
	std::vector<GridNode*> data;
public:
	int size_x, size_y;
	GridNode* get(int x, int y);
	bool isBlocked(int x, int y);

	void print();

	GridGraph(int sizeX, int sizeY);
};