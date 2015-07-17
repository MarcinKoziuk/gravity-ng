/*
 * game/geometry_/ishape.hpp
 *
 * Defines the interface for shapes
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_GEOMETRY__ISHAPE_HPP
#define GRAVITY_GAME_GEOMETRY__ISHAPE_HPP

#include <glm/glm.hpp>

#include <Box2D/Collision/Shapes/b2Shape.h>

namespace Gravity {
namespace Game {

class IShape {
public:
    virtual const b2Shape& GetPhysicsShape() const = 0;

    virtual ~IShape() {}
};

} // namespace Game
} // namespace Gravity

#endif /* GRAVITY_GAME_GEOMETRY__ISHAPE_HPP */
