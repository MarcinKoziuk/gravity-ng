/*
* cgame/component/graphics.cpp
*
* Copyright (c) 2016
* Marcin Koziuk <marcin.koziuk@gmail.com>
*/

#include <cstddef>

#include <gravity/cgame/component/graphics.hpp>
#include <gravity/cgame/asset/modelasset.hpp>

namespace Gravity {
namespace Component {

const std::size_t Graphics::family = 3243241L;

Graphics::Graphics(std::shared_ptr<ModelAsset> model)
	: model(model)
{
	for (const auto& kv : model->GetPathGroups()) {
		if (kv.first == MAIN_GROUP_LABEL) {
			for (const auto& pathSet : kv.second) {
				pathSetsToRender.push_back(&pathSet);
			}
		}
	}
}

Graphics::~Graphics()
{}

std::vector<const VectorPathSet*> Graphics::GetPathSetsToRender() const
{
	return pathSetsToRender;
}

} // namespace Component
} // namespace Gravity
