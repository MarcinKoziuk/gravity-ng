/*
 * cgame/triangulator/itriangulator.hpp
 *
 * Transforms and caches game entities into drawable triangles.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_CGAME_TRIANGULATOR_ITRIANGULATOR_HPP
#define GRAVITY_CGAME_TRIANGULATOR_ITRIANGULATOR_HPP

#include <vector>

#include <boost/noncopyable.hpp>

#include <glm/glm.hpp>

#include "gravity/game/entity.hpp"

namespace Gravity {
namespace CGame {

class ITriangulator : private boost::noncopyable {
public:
    virtual ~ITriangulator() {}

    virtual std::vector<glm::vec2>& GetTriangles(const Game::Entity* entity) = 0;

    virtual std::vector<glm::vec2>& GetTriangles(const Game::Entity *entity, const std::string& layer) = 0;
};

} // namespace CGame
} // namespace Gravity

#endif /* GRAVITY_CGAME_TRIANGULATOR_ITRIANGULATOR_HPP */
