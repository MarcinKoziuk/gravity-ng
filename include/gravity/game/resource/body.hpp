/*
 * game/resource/body.hpp
 *
 * A body has multiple fixtures that contain shapes. The instances are reused
 * by Physics components.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_RESOURCE_BODY_HPP
#define GRAVITY_GAME_RESOURCE_BODY_HPP

#include <cstddef>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

#include <json/forwards.h>

#include <Box2D/Dynamics/b2Fixture.h>

#include "gravity/game/resource/resource.hpp"

namespace Gravity {

class Body : public Resource {
private:
    bool isLoaded;
    std::vector<b2FixtureDef> fixtureDefs;
    std::vector<b2Shape*> shapes;

    void LoadConvex(Json::Value&, b2FixtureDef&, bool isTriangle=false);
    void LoadCircle(float, b2FixtureDef&);
    void LoadFixtureData(Json::Value&, b2FixtureDef&);
    void LoadBodyData();

public:
    Body(const std::string& key, const std::string& group);

    virtual ~Body();

    virtual std::size_t GetSize() const;

    virtual bool IsLoaded() const
    { return isLoaded; }

    virtual void Load(bool inBackground = false);

    virtual void Unload();

    virtual const std::vector<b2FixtureDef>& GetFixtureDefs();
};

typedef boost::shared_ptr<Body> BodyPtr;

} // namespace Gravity

#endif /* GRAVITY_GAME_RESOURCE_BODY_HPP */
