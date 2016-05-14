/*
 * game/geometry_/triangle.hpp
 *
 * A simple triangle
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_GEOMETRY__TRIANGLE_HPP
#define GRAVITY_GAME_GEOMETRY__TRIANGLE_HPP

#include <glm/glm.hpp>

#include <Box2D/Collision/Shapes/b2PolygonShape.h>

#include "gravity/game/geometry_/ishape.hpp"

namespace Gravity {

class Triangle : public IShape {
private:
    b2PolygonShape polygonShape;
public:
    explicit Triangle(glm::vec2 vertices[3]);

    virtual const b2Shape& GetPhysicsShape() const;

    virtual ~Triangle();
};

} // namespace Gravity

#endif /* GRAVITY_GAME_GEOMETRY__TRIANGLE_HPP */
