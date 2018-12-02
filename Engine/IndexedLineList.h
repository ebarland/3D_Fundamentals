#pragma once
#include <vector>
#include "Vec3.h"

class IndexedLineList
{
public:
	std::vector<Vec3> vertices;
	std::vector<size_t> indeces;
};
