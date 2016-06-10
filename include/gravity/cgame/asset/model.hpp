/*
* cgame/asset/model.hpp
*
* Copyright (c) 2016
* Marcin Koziuk <marcin.koziuk@gmail.com>
*/

#ifndef GRAVITY_CGAME_ASSET_MODEL_HPP
#define GRAVITY_CGAME_ASSET_MODEL_HPP

#include <string>
#include <vector>
#include <map>

#include <boost/noncopyable.hpp>

#include <glm/vec2.hpp>

#include <gravity/game/asset/body.hpp>

#include <gravity/cgame/vectorpath.hpp>

namespace Gravity {
namespace Asset {

static const char MAIN_GROUP_LABEL[] = "model";

class Model : public Body {
private:
	std::map<std::string, std::vector<VectorPathSet>> pathGroups;

	virtual TransformProps LoadImpl(const YAML::Node& root, const NSVGimage& image);
	void LoadShape(const NSVGshape* shape, const TransformProps& tp);
	std::vector<VectorPath> ShapeToVectorPaths(const NSVGshape* shape, const TransformProps& tp);

public:
	Model(const std::string& path);

	virtual ~Model();

	virtual std::size_t CalculateSize() const;

	virtual const char* GetAssetType() const;

	const std::map<std::string, std::vector<VectorPathSet>>& GetPathGroups() const;
};

} // namespace Asset
} // namespace Gravity

#endif /* GRAVITY_CGAME_ASSET_MODEL_HPP */
