/*
* game/asset/modelasset.cpp
*
* Copyright (c) 2016
* Marcin Koziuk <marcin.koziuk@gmail.com>
*/

#include <cstring>

#include <nanosvg.h>

#include <gravity/game/logging.hpp>

#include <gravity/dev/casteljau.hpp>

#include <gravity/cgame/asset/modelasset.hpp>

namespace Gravity {

ModelAsset::ModelAsset(const std::string& path)
	: BodyAsset()
{
	Load(path);
}

ModelAsset::~ModelAsset()
{}

BodyAsset::TransformProps ModelAsset::LoadImpl(const YAML::Node& root, const NSVGimage* image)
{
	TransformProps tp = BodyAsset::LoadImpl(root, image);

	for (const NSVGshape* shape = image->shapes; shape != nullptr; shape = shape->next) {
		const char* group = shape->groupLabel;

		if (std::strcmp(group, ORIGIN_GROUP_LABEL) == 0 ||
			std::strcmp(group, BODY_GROUP_LABEL) == 0) {
			continue;
		}

		LoadShape(shape, tp);
	}

	return tp;
}

void ModelAsset::LoadShape(const NSVGshape* shape, const BodyAsset::TransformProps& tp)
{
	const std::string groupLabel = shape->groupLabel;

	VectorPathSet pathSet;
	RGBA color(255, 255, 255, shape->opacity);
	
	color.CopyHex(0xff0000);
	if (shape->stroke.type == NSVG_PAINT_COLOR) {
		if (shape->stroke.color != 0xff000000) {
			color.CopyHex(shape->stroke.color);
		}
	}

	LOG(trace) << "Color is: " << std::hex << shape->stroke.color << std::dec;
	

	pathSet.width = shape->strokeWidth;
	pathSet.color = color;
	pathSet.paths = ShapeToVectorPaths(shape, tp);

	if (pathGroups.count(groupLabel)) {
		pathGroups[groupLabel].push_back(pathSet);
	} else {
		std::vector<VectorPathSet> pathSetList;
		pathSetList.push_back(pathSet);
		pathGroups[groupLabel] = pathSetList;
	}
	
}

std::vector<VectorPath> ModelAsset::ShapeToVectorPaths(const NSVGshape* shape, const TransformProps& tp)
{
	std::vector<VectorPath> paths;

	for (const NSVGpath* svgPath = shape->paths; svgPath != nullptr; svgPath = svgPath->next) {
		VectorPath path;
		path.closed = svgPath->closed;

		std::vector<glm::vec2> points;

		for (int i = 0; i < svgPath->npts - 1; i += 3) {
			float* p = &svgPath->pts[i * 2];
			std::vector<glm::vec2> cubicBezier;
			cubicBezier.push_back(glm::vec2(p[0], p[1]));
			cubicBezier.push_back(glm::vec2(p[2], p[3]));
			cubicBezier.push_back(glm::vec2(p[4], p[5]));
			cubicBezier.push_back(glm::vec2(p[6], p[7]));

			std::vector<glm::vec2> approximatedBezier = Casteljau(cubicBezier);

			int iz = 0;
			for (const auto& p : approximatedBezier) {
				//if (iz % 4 == 0) {
					points.push_back(p);
				//}
				iz++;
			}
		}

		if (points.size() < 2) {
			LOG(error) << "Path in " << shape->groupLabel << ", " << this->path << " has less than 2 points";
			continue;
		}

		for (int i = 0; i < points.size(); i++) {
			points[i] -= tp.origin;
			points[i] *= tp.scale;
		}

		path.path = points;
		paths.push_back(path);
	}

	return paths;
}

const std::map<std::string, std::vector<VectorPathSet>>& ModelAsset::GetPathGroups() const
{
	return pathGroups;
}

std::size_t ModelAsset::CalculateSize() const
{
	return 0L;
}

const char* ModelAsset::GetAssetType() const
{
	return "Model";
}

} // namespace Gravity
