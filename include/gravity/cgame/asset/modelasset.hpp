/*
* cgame/asset/modelasset.hpp
*
* Copyright (c) 2016
* Marcin Koziuk <marcin.koziuk@gmail.com>
*/

#ifndef GRAVITY_CGAME_ASSET_MODELASSET_HPP
#define GRAVITY_CGAME_ASSET_MODELASSET_HPP

#include <string>
#include <vector>
#include <map>

#include <boost/noncopyable.hpp>

#include <glm/vec2.hpp>

#include <gravity/game/asset/bodyasset.hpp>

#include <gravity/cgame/vectorpath.hpp>

namespace Gravity {

static const char MAIN_GROUP_LABEL[] = "model";

class ModelAsset : public BodyAsset {
private:
	std::map<std::string, std::vector<VectorPathSet>> pathGroups;

	virtual TransformProps LoadImpl(const YAML::Node& root, const NSVGimage* image);
	void LoadShape(const NSVGshape* shape, const TransformProps& tp);
	std::vector<VectorPath> ShapeToVectorPaths(const NSVGshape* shape, const TransformProps& tp);

public:
	ModelAsset(const std::string& path);

	virtual ~ModelAsset();

	virtual std::size_t CalculateSize() const;

	virtual const char* GetAssetType() const;

	const std::map<std::string, std::vector<VectorPathSet>>& GetPathGroups() const;
};

} // namespace Gravity

#endif /* GRAVITY_CGAME_ASSET_MODELASSET_HPP */
