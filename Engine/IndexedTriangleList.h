#pragma once
#include "Vec3.h"
#include <vector>

struct IndexedTriangleList
{
	IndexedTriangleList(std::vector<Vec3> verticies_in, std::vector<size_t> indicies_in)
		:
		verticies(std::move(verticies_in)),
		indicies(std::move(indicies_in))
	{
		assert(verticies.size() > 2);
		assert(indicies.size() % 3 == 0);
		cullFlags.resize(indicies.size() / 3, false);
	}
	std::vector<Vec3> verticies;
	std::vector<size_t> indicies;
	std::vector<bool> cullFlags;
};