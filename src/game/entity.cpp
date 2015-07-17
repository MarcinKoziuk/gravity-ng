/*
 * game/entity.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include "gravity/game/world.hpp"
#include "gravity/game/entity.hpp"

namespace Gravity {
namespace Game {

std::size_t Entity::idSequence = 0;

Entity::Entity(World& world) : world(world)
{
    id = idSequence;
    idSequence += 1L;
}

} // namespace Game
} // namespace Gravity
