/*
 * game/asset/bodyasset.cpp
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstddef>
#include <cmath>
#include <cstring>
#include <vector>
#include <utility>

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include <glm/vec2.hpp>

#include <yaml-cpp/yaml.h>
#include <nanosvg.h>

#include <Box2D/Box2D.h>

#include <gravity/dev/casteljau.hpp>

#include <gravity/game/logging.hpp>
#include <gravity/game/asset/yamlasset.hpp>
#include <gravity/game/asset/svgasset.hpp>
#include <gravity/game/asset/bodyasset.hpp>

namespace fs = boost::filesystem;

namespace Gravity {

BodyAsset::BodyAsset()
{}

BodyAsset::~BodyAsset()
{
    for (b2Shape* shape : shapes) {
        delete shape;
    }
}

b2Shape* BodyAsset::MakePolygonShape(const std::vector<glm::vec2>& points)
{
    b2PolygonShape* shape = new b2PolygonShape();
    std::vector<b2Vec2> bVertices;
    for (const glm::vec2 point : points) {
        bVertices.push_back(b2Vec2(point.x, point.y));
    }

    shape->Set(&bVertices[0], bVertices.size());
    return shape;
}

void BodyAsset::ConstructFixtureDef(const b2FixtureDef& standardFixtureDef, const std::vector<glm::vec2>& points)
{
    b2FixtureDef fixtureDef = standardFixtureDef;
    b2Shape* shape = MakePolygonShape(points);
    shapes.push_back(shape);

    fixtureDef.shape = shape;
    fixtureDefs.push_back(fixtureDef);
}

void BodyAsset::LoadShape(const b2FixtureDef& standardFixtureDef, const NSVGshape* shape)
{
    for (const NSVGpath* path = shape->paths; path != nullptr; path = path->next) {
        std::stringstream pathInfo;
        pathInfo << "(" << shape->id << " of " << svgPath << ")";

        if (path->closed) {
            LOG(warning) << "SVG body path is not closed " << pathInfo.str();
        }

        std::vector<glm::vec2> points;

        for (int i = 0; i < path->npts-1; i += 3) {
            float* p = &path->pts[i*2];
            std::vector<glm::vec2> cubicBezier;
            cubicBezier.push_back(glm::vec2(p[0], p[1]));
            cubicBezier.push_back(glm::vec2(p[2], p[3]));
            cubicBezier.push_back(glm::vec2(p[4], p[5]));
            cubicBezier.push_back(glm::vec2(p[6], p[7]));

            std::vector<glm::vec2> approximatedBezier = Casteljau(cubicBezier);

            int iz = 0;
            for (const auto& p : approximatedBezier) {
                if (iz % 4 == 0) {
                    points.push_back(p);
                }
                iz++;
            }
        }

        if (points.size() < 3) {
            LOG(error) << "SVG body path has less than 3 points " << pathInfo.str();
            continue;
        }
        if (points.size() > 8) {
            int reduction = std::ceil((double)points.size() / 8.);

            LOG(warning) << "SVG body path has more than 8 points ("
                         << points.size() << "), reducing x" << reduction << " " << pathInfo.str();


            std::vector<glm::vec2> newpoints;
            for (std::size_t i = 0; i < points.size(); i++) {
                if (i % reduction == 0) {
                    newpoints.push_back(points[i]);
                }
            }

            points.swap(newpoints);
        }

        ConstructFixtureDef(standardFixtureDef, points);
    }
}

b2FixtureDef BodyAsset::MakeStandardFixtureDef(const YAML::Node& root)
{
    b2FixtureDef fixtureDef;

    const YAML::Node& physics = root["physics"];
    if (physics) {
        if (physics["density"]) {
            fixtureDef.density = physics["density"].as<float>();
        }
        if (physics["restitution"]) {
            fixtureDef.restitution = physics["restitution"].as<float>();
        }
        if (physics["friction"]) {
            fixtureDef.friction = physics["friction"].as<float>();
        }
    }

    return fixtureDef;
}

void BodyAsset::LoadImpl(const YAML::Node& root, const NSVGimage* image)
{
    b2FixtureDef standardFixtureDef = MakeStandardFixtureDef(root);

    for (const NSVGshape* shape = image->shapes; shape != nullptr; shape = shape->next) {
        const char* group = shape->groupLabel;
        if (std::strcmp(group, "body") == 0) {
            LoadShape(standardFixtureDef, shape);
        }
    }
}

bool BodyAsset::Load(const std::string &path)
{
    { LOG(warning) << "HELLO?"; }
    YAMLAsset yamlAsset;

    yamlPath = path;

    if (yamlAsset.Load(yamlPath)) {
        fs::path yamlFsPath(yamlPath);
        YAML::Node root = yamlAsset.GetRoot();
        std::string svgFilename;

        if (root["model"]) {
            svgFilename = root["model"].as<std::string>();
        } else {
            svgFilename = yamlFsPath.filename().replace_extension("svg").string();
        }

        svgPath = (yamlFsPath.parent_path() / svgFilename).string();

        SVGAsset svgAsset;

        if (svgAsset.Load(svgPath)) {
            LoadImpl(root, svgAsset.GetImage());
            return true;
        } else {
            return false;
        }

    } else {
        return false;
    }
}

std::size_t BodyAsset::CalculateSize() const
{
    return 0L;
}

const char* BodyAsset::GetAssetType() const
{
    return "Body";
}

} // namespace Gravity
