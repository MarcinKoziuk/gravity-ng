/*
 * game/component/physics.hpp
 *
 * Defines the interface for entity components.
 * Note that all inheriting classes should have a public static "family" member.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_COMPONENT_PHYSICS_HPP
#define GRAVITY_GAME_COMPONENT_PHYSICS_HPP

#include <cstddef>

#include <glm/glm.hpp>

#include <Box2D/Dynamics/b2Body.h>

#include "gravity/game/world.hpp"
#include "gravity/game/resource/body.hpp"
#include "gravity/game/component/icomponent.hpp"

namespace Gravity {
namespace Game {
namespace Component {

class Physics : public IComponent {
private:
    Entity& entity;
    BodyPtr bodyResource;
    b2Body* physicsBody;

public:
    static const std::size_t family;

    Physics(Entity& entity, BodyPtr resource, glm::vec2 pos);

    ~Physics();

    b2Body& GetPhysicsBody() const;
};

} // namespace Component
} // namespace Game
} // namespace Gravity

#endif /* GRAVITY_GAME_COMPONENT_PHYSICS_HPP */
