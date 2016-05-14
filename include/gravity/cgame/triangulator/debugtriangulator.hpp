/*
 * cgame/triangulator/debugtriangulator.hpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_CGAME_TRIANGULATOR_DEBUGTRIANGULATOR_HPP
#define GRAVITY_CGAME_TRIANGULATOR_DEBUGTRIANGULATOR_HPP

#include <vector>

#include <glm/glm.hpp>

#include "gravity/cgame/triangulator/itriangulator.hpp"

namespace Gravity {

class DebugTriangulator : public ITriangulator {
public:
    virtual ~ITriangulator() {}

    virtual std::vector<glm::vec2>& GetTriangles(const Game::Entity* entity) = 0;

    virtual std::vector<glm::vec2>& GetTriangles(const Game::Entity *entity, const std::string& layer) = 0;
};


} // namespace Gravity

#endif /* GRAVITY_CGAME_TRIANGULATOR_DEBUGTRIANGULATOR_HPP */
