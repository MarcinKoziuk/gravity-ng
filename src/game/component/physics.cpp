/*
 * game/component/physics.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstddef>

#include <Box2D/Box2D.h>

#include "gravity/game/component/physics.hpp"
#include "gravity/game/asset/body.hpp"

namespace Gravity {
namespace Component {

const std::size_t Physics::family = 2122559316L;

Physics::Physics(World& world, const Asset::Body& bodyAsset, glm::vec2 pos)
	: world(world)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(pos.x, pos.y);

	b2World* physicsWorld = world.GetPhysics();
	physicsBody = physicsWorld->CreateBody(&bodyDef);

	const std::vector<b2FixtureDef>& fixtureDefs = bodyAsset.GetFixtureDefs();
	for (std::size_t i = 0; i < fixtureDefs.size(); i++) {
		const b2FixtureDef* fixtureDef = &fixtureDefs[i];
		physicsBody->CreateFixture(&fixtureDefs[i]);
	}
}

Physics::~Physics()
{
	b2World* physicsWorld = world.GetPhysics();
	physicsWorld->DestroyBody(physicsBody);	
}

b2Body& Physics::GetPhysicsBody() const
{
    return *physicsBody;
}

} // namespace Component
} // namespace Gravity
