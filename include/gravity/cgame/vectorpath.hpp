/*
* cgame/vectorpath.hpp
*
* Copyright (c) 2016
* Marcin Koziuk <marcin.koziuk@gmail.com>
*/

#ifndef GRAVITY_CGAME_LINES_HPP
#define GRAVITY_CGAME_LINES_HPP

#include <vector>

#include <glm/vec2.hpp>

#include <gravity/cgame/rgb.hpp>

namespace Gravity {

struct VectorPath {
	std::vector<glm::vec2> path;
	bool closed;
};

struct VectorPathSet {
	std::vector<VectorPath> paths;
	float width;
	RGBA color;
};

} // namespace Gravity

#endif /* GRAVITY_CGAME_VECTORPATH_HPP */