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
#include "gravity/game/asset/body.hpp"
#include "gravity/game/component/icomponent.hpp"

namespace Gravity {
namespace Component {

class Physics : public IComponent {
private:
	World& world;
    b2Body* physicsBody;

public:
    static const std::size_t family;

	Physics(World& world, const Asset::Body& bodyAsset, glm::vec2 pos);

    ~Physics();

    b2Body& GetPhysicsBody() const;
};

} // namespace Component
} // namespace Gravity

#endif /* GRAVITY_GAME_COMPONENT_PHYSICS_HPP */
