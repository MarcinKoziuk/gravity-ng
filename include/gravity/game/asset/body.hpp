/*
 * game/asset/body.hpp
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_ASSET_BODY_HPP
#define GRAVITY_GAME_ASSET_BODY_HPP

#include <vector>
#include <utility>

#include <boost/noncopyable.hpp>

#include <glm/vec2.hpp>

#include <yaml-cpp/node/node.h>

#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2Shape.h>

#include <gravity/game/asset/iasset.hpp>

struct NSVGimage;
struct NSVGshape;

namespace Gravity {
namespace Asset {

static const char ORIGIN_GROUP_LABEL[] = "origin";
static const char BODY_GROUP_LABEL[] = "body";

class Body : public IAsset, private boost::noncopyable {
protected:
	struct TransformProps {
		glm::vec2 origin;
		double scale;

		TransformProps() : scale(1.)
		{}
	};

	Body();

private:
    b2Shape* MakePolygonShape(const std::vector<glm::vec2>& points);
	void ConstructFixtureDef(const b2FixtureDef& standardFixtureDef, glm::vec2 pos, double radius);
    void ConstructFixtureDef(const b2FixtureDef& standardFixtureDef, const std::vector<glm::vec2>& points);
    void LoadShape(const b2FixtureDef& standardFixtureDef, const NSVGshape* shape, const TransformProps& tp);
    b2FixtureDef MakeStandardFixtureDef(const YAML::Node& root);
	bool IsCircle(const NSVGshape* shape);
	std::pair<glm::vec2, double> ShapeToCircle(const NSVGshape* shape, const TransformProps& tp);
	std::vector<std::vector<glm::vec2>> ShapeToLines(const NSVGshape* shape, const TransformProps& tp);

protected:
	bool loaded;
	std::string path;
	glm::vec2 origin;
	std::vector<b2FixtureDef> fixtureDefs;
	std::vector<b2Shape*> physicsShapes;

	void Load(const std::string& path);
	virtual TransformProps LoadImpl(const YAML::Node& root, const NSVGimage& image);

public:
    Body(const std::string& path);

    virtual ~Body();

    virtual bool IsLoaded() const;

    virtual std::size_t CalculateSize() const;

    virtual const char* GetAssetType() const;

	const std::vector<b2FixtureDef>& GetFixtureDefs() const;
};

} // namespace Asset
} // namespace Gravity

#endif /* GRAVITY_GAME_ASSET_BODY_HPP */
