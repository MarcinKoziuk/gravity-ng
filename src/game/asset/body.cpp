/*
 * game/asset/body.cpp
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstddef>
#include <cmath>
#include <cstring>
#include <vector>
#include <utility>
#include <limits>

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include <glm/vec2.hpp>

#include <yaml-cpp/yaml.h>
#include <nanosvg.h>

#include <Box2D/Box2D.h>

#include <gravity/dev/casteljau.hpp>

#include <gravity/game/logging.hpp>
#include <gravity/game/resource/resourceloader.hpp>
#include <gravity/game/asset/body.hpp>

namespace fs = boost::filesystem;

namespace Gravity {
namespace Asset {

Body::Body(const std::string& path)
	: loaded(false)
{
	Load(path);
}

Body::Body()
	: loaded(false)
{}

Body::~Body()
{
    for (b2Shape* shape : physicsShapes) {
        delete shape;
    }
}

void Body::Load(const std::string& path)
{
	this->path = path;

	boost::optional<YAML::Node> maybeNode = ResourceLoader::OpenAsYaml(path);
	if (maybeNode) {
		const YAML::Node& root = *maybeNode;

		fs::path yamlFsPath(path);
		std::string svgFilename;

		if (root["model"]) {
			svgFilename = root["model"].as<std::string>();
		} else {
			svgFilename = yamlFsPath.filename().replace_extension("svg").string();
		}

		const std::string svgPath = (yamlFsPath.parent_path() / svgFilename).string();

		std::unique_ptr<NSVGimage, NSVGimage_deleter> nsvgImage = ResourceLoader::OpenAsSvg(svgPath);
		if (nsvgImage) {
			LoadImpl(root, *nsvgImage);
			loaded = true;
		}
	}
}

bool Body::IsLoaded() const
{
	return loaded;
}

b2Shape* Body::MakePolygonShape(const std::vector<glm::vec2>& points)
{
    b2PolygonShape* shape = new b2PolygonShape();
    std::vector<b2Vec2> bVertices;
    for (const glm::vec2 point : points) {
        bVertices.push_back(b2Vec2(point.x, point.y));
    }

    shape->Set(&bVertices[0], bVertices.size());
    return shape;
}

void Body::ConstructFixtureDef(const b2FixtureDef& standardFixtureDef, glm::vec2 pos, double radius)
{
	b2FixtureDef fixtureDef = standardFixtureDef;
	b2CircleShape* circle = new b2CircleShape();
	circle->m_radius = radius;
	circle->m_p = b2Vec2(pos.x, pos.y);

	physicsShapes.push_back(circle);
	fixtureDef.shape = circle;
	fixtureDefs.push_back(fixtureDef);
}

void Body::ConstructFixtureDef(const b2FixtureDef& standardFixtureDef, const std::vector<glm::vec2>& points)
{
    b2FixtureDef fixtureDef = standardFixtureDef;
    b2Shape* shape = MakePolygonShape(points);
    physicsShapes.push_back(shape);

    fixtureDef.shape = shape;
    fixtureDefs.push_back(fixtureDef);
}

std::pair<glm::vec2, double> Body::ShapeToCircle(const NSVGshape* shape, const TransformProps& tp)
{
	const NSVGpath* path = shape->paths;
	double radius = (path->bounds[2] - path->bounds[0]) / 2.;
	glm::vec2 pos(path->bounds[0] + radius, path->bounds[1] + radius);

	radius *= tp.scale;
	pos -= tp.origin;
	pos *= tp.scale;

	return std::pair<glm::vec2, double>(pos, radius);
}

std::vector<std::vector<glm::vec2>> Body::ShapeToLines(const NSVGshape* shape, const TransformProps& tp)
{
	std::vector<std::vector<glm::vec2>> lines;

	for (const NSVGpath* path = shape->paths; path != nullptr; path = path->next) {
		std::stringstream pathInfo;
		pathInfo << "(" << shape->id << " of " << path << ")";

		if (!path->closed) {
			LOG(warning) << "SVG body path is not closed in model " << pathInfo.str();
		}

		std::vector<glm::vec2> points;

		for (int i = 0; i < path->npts - 1; i += 3) {
			float* p = &path->pts[i * 2];
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

		for (int i = 0; i < points.size(); i++) {
			points[i] -= tp.origin;
			points[i] *= tp.scale;
		}

		lines.push_back(points);
	}

	return lines;
}

bool Body::IsCircle(const NSVGshape* shape)
{
	const NSVGpath* p = shape->paths;
	const double diff = std::abs((p->bounds[2] - p->bounds[0]) - (p->bounds[3] - p->bounds[1]));

	return p->closed
		&& p->npts == 16
		&& p->next == nullptr
		&& diff < 0.001;
}

void Body::LoadShape(const b2FixtureDef& standardFixtureDef, const NSVGshape* shape, const Body::TransformProps& tp)
{
	if (IsCircle(shape)) {
		const std::pair<glm::vec2, double> posAndRadius = ShapeToCircle(shape, tp);
		ConstructFixtureDef(standardFixtureDef, posAndRadius.first, posAndRadius.second);
	} else {
		const std::vector<std::vector<glm::vec2>> lines = ShapeToLines(shape, tp);

		for (const std::vector<glm::vec2>& line : lines) {
			ConstructFixtureDef(standardFixtureDef, line);
		}
	}
}

b2FixtureDef Body::MakeStandardFixtureDef(const YAML::Node& root)
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

Body::TransformProps Body::LoadImpl(const YAML::Node& root, const NSVGimage& image)
{
	TransformProps tp;
	b2FixtureDef standardFixtureDef = MakeStandardFixtureDef(root);

	if (root["scale"]) {
		tp.scale = root["scale"].as<double>();
	}

	// Must find origin first before parsing the rest
	for (const NSVGshape* shape = image.shapes; shape != nullptr; shape = shape->next) {
		const char* group = shape->groupLabel;
		if (std::strcmp(group, ORIGIN_GROUP_LABEL) == 0) {
			NSVGpath* path = shape->paths;
			tp.origin.x = (path->bounds[0] + path->bounds[2]) / 2.f;
			tp.origin.y = (path->bounds[1] + path->bounds[3]) / 2.f;
		}
	}

    for (const NSVGshape* shape = image.shapes; shape != nullptr; shape = shape->next) {
        const char* group = shape->groupLabel;
        if (std::strcmp(group, BODY_GROUP_LABEL) == 0) {
            LoadShape(standardFixtureDef, shape, tp);
        }
    }

	return tp;
}

const std::vector<b2FixtureDef>& Body::GetFixtureDefs() const
{
	return fixtureDefs;
}

std::size_t Body::CalculateSize() const
{
    return 0L;
}

const char* Body::GetAssetType() const
{
    return "Body";
}

} // namespace Asset
} // namespace Gravity
