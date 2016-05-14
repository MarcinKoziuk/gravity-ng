/*
 * game/asset/bodyasset.hpp
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_ASSET_BODYASSET_HPP
#define GRAVITY_GAME_ASSET_BODYASSET_HPP

#include <vector>

#include <yaml-cpp/node/node.h>

#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2Shape.h>

#include <gravity/game/asset/iasset.hpp>

struct NSVGimage;
struct NSVGshape;

namespace Gravity {

class BodyAsset : public IAsset {
private:
    std::string svgPath;
    std::string yamlPath;

    std::vector<b2FixtureDef> fixtureDefs;
    std::vector<b2Shape*> shapes;

    b2Shape* MakePolygonShape(const std::vector<glm::vec2>& points);
    void ConstructFixtureDef(const b2FixtureDef& standardFixtureDef, const std::vector<glm::vec2>& points);
    void LoadShape(const b2FixtureDef& standardFixtureDef, const NSVGshape* shape);
    b2FixtureDef MakeStandardFixtureDef(const YAML::Node& root);
    void LoadImpl(const YAML::Node& root, const NSVGimage* image);

public:
    BodyAsset();

    virtual ~BodyAsset();

    virtual bool Load(const std::string& path);

    virtual std::size_t CalculateSize() const;

    virtual const char* GetAssetType() const;

    const NSVGimage* GetImage() const;
};

} // namespace Gravity

#endif /* GRAVITY_GAME_ASSET_BODYASSET_HPP */
