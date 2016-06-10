/*
 * game/world.hpp
 *
 * Holds entities & physics context.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_WORLD_HPP
#define GRAVITY_GAME_WORLD_HPP

#include <vector>

#include <Box2D/Dynamics/b2World.h>

#include "gravity/game/entity.hpp"

namespace Gravity {

class World {
private:
    b2World physics;
    std::vector<Entity> entities;

public:
    World() : physics(b2Vec2(0, 0)) {}

    b2World* GetPhysics()
    { return &physics; }

    const std::vector<Entity>& GetEntities() const
    { return entities; }
};

} // namespace Gravity

#endif /* GRAVITY_GAME_WORLD_HPP */
