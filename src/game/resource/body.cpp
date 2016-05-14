/*
 * game/resource/body.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <string>
#include <istream>

#include <boost/optional.hpp>

#ifdef __linux__
#include <json/value.h>
#include <json/reader.h>
#else
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#endif

#include <Box2D/Box2D.h>

#include "gravity/game/logging.hpp"
#include "gravity/game/resource/resourceloader.hpp"
#include "gravity/game/resource/body.hpp"

namespace Gravity {

Body::Body(const std::string& key, const std::string& group)
    : Resource(key, group)
    , isLoaded(false)
{}

Body::~Body() {
    Unload();
}

void Body::LoadConvex(Json::Value& vertices, b2FixtureDef& fixtureDef, bool isTriangle)
{
    b2PolygonShape* polygonShape = new b2PolygonShape;
    std::vector<b2Vec2> bVertices;

    for (Json::ArrayIndex i = 0; i < vertices.size(); i++) {
        Json::Value vertex = vertices[i];
        b2Vec2 bVertex(vertex[0].asFloat(), vertex[1].asFloat());
        bVertices.push_back(bVertex);
    }

    if (isTriangle) {
        assert(bVertices.size() == 3);
    } else {
        assert(bVertices.size() > 3);
    }

    polygonShape->Set(&bVertices[0], bVertices.size());
    fixtureDef.shape = polygonShape;
}

void Body::LoadCircle(float radius, b2FixtureDef& fixtureDef)
{
    b2CircleShape* circleShape = new b2CircleShape;
    circleShape->m_radius = radius;
    fixtureDef.shape = circleShape;
}

void Body::LoadFixtureData(Json::Value& shape, b2FixtureDef& fixtureDef)
{
    std::string shapeType = shape.get("type", "convex").asString();

    if (shapeType == "triangle") {
        Json::Value vertices = shape["vertices"];
        LoadConvex(vertices, fixtureDef, true);
    } else if (shapeType == "convex") {
        Json::Value vertices = shape["vertices"];
        LoadConvex(vertices, fixtureDef, false);
    } else if (shapeType == "circle") {
        float radius = shape.get("radius", 1.f).asFloat();
        LoadCircle(radius, fixtureDef);
    } else {
        LOG(error) << "Can't read shape of type " << shapeType;
    }
}

void Body::LoadBodyData()
{
    boost::optional<Json::Value> maybeValues = ResourceLoader::LoadJson(key);
    if (!maybeValues) {
        LOG(fatal) << "Body data does not exist: " + key;
        std::exit(1);
    }
    Json::Value values = maybeValues.get();
    Json::Value fixtures = values["fixtures"];

    for (Json::ArrayIndex i = 0; i < fixtures.size(); i++) {
        Json::Value fixture = fixtures[i];
        b2FixtureDef fixtureDef;

        fixtureDef.density = fixture.get("density", 0.f).asFloat();
        fixtureDef.friction = fixture.get("friction", 0.f).asFloat();
        fixtureDef.restitution = fixture.get("restitution", 0.f).asFloat();
        fixtureDef.isSensor = fixture.get("is-sensor", false).asBool();
        fixtureDef.userData = ((void*)123456789L);

        Json::Value shapes = fixture["shapes"];
        for (Json::ArrayIndex j = 0; j < shapes.size(); j++) {
            b2FixtureDef fixtureCopy = fixtureDef;
            LoadFixtureData(shapes[j], fixtureCopy);
            fixtureDefs.push_back(fixtureCopy);
        }

        if (fixture["shape"] != Json::nullValue) {
            LoadFixtureData(fixture["shape"], fixtureDef);
            fixtureDefs.push_back(fixtureDef);
        }

    }
}

std::size_t Body::GetSize() const
{
    std::size_t size = sizeof(b2FixtureDef) * fixtureDefs.size();
    for (std::size_t i = 0; i < fixtureDefs.size(); i++) {
        const b2Shape* shape = fixtureDefs[i].shape;
        if (shape) {
            size += sizeof(b2Shape); // approximated <insert large switch here>
        }
    }

    return sizeof(b2FixtureDef) * fixtureDefs.size()
            + sizeof(b2Body) * shapes.size();
}

void Body::Load(bool inBackground)
{
    Resource::Load(inBackground);

    if (!isLoaded) {
        LoadBodyData();
        isLoaded = true;
    }
}

void Body::Unload()
{
    Resource::Unload();

    if (isLoaded) {
        for (std::size_t i = 0; i < fixtureDefs.size(); i++) {
            const b2Shape* shape = fixtureDefs[i].shape;
            if (shape) delete shape;
        }

        fixtureDefs.clear();
        isLoaded = false;
    }
}

const std::vector<b2FixtureDef>& Body::GetFixtureDefs()
{
    if (!isLoaded) {
        Load(false);
        LOG(trace) << "Had to implicitly load " << key;
    }
    return fixtureDefs;
}

} // namespace Gravity
