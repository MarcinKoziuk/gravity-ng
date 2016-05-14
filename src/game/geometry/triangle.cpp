/*
 * game/geometry/triangle.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <glm/glm.hpp>

#include <Box2D/Box2D.h>

#include "gravity/game/geometry_/triangle.hpp"

using glm::vec2;

namespace Gravity {

Triangle::Triangle(vec2 vertices[3])
{
    b2Vec2* bVertices = reinterpret_cast<b2Vec2*>(vertices);
    polygonShape.Set(bVertices, 3);
}

const b2Shape& Triangle::GetPhysicsShape() const
{
    return polygonShape;
}

Triangle::~Triangle()
{}


} // namespace Gravity
