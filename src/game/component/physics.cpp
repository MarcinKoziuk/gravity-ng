/*
 * game/component/physics.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstddef>

#include <Box2D/Box2D.h>

#include "gravity/game/component/physics.hpp"
#include "gravity/game/resource/body.hpp"

namespace Gravity {
namespace Component {

const std::size_t Physics::family = 2122559316L;

Physics::Physics(Entity& entity, BodyPtr resource, glm::vec2 pos)
    : entity(entity)
    , bodyResource(resource)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(pos.x, pos.y);

    b2World* physicsWorld = entity.GetWorld().GetPhysics();
    physicsBody = physicsWorld->CreateBody(&bodyDef);

    std::vector<b2FixtureDef> fixtureDefs = resource->GetFixtureDefs();
    for (std::size_t i = 0; i < fixtureDefs.size(); i++) {
        b2FixtureDef* fixtureDef = &fixtureDefs[i];
        fixtureDef->density = 1.f;
        physicsBody->CreateFixture(&fixtureDefs[i]);
    }
}

Physics::~Physics()
{}

b2Body& Physics::GetPhysicsBody() const
{
    return *physicsBody;
}

} // namespace Component
} // namespace Gravity
