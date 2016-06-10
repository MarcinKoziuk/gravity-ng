/*
* cgame/component/graphics.cpp
*
* Copyright (c) 2016
* Marcin Koziuk <marcin.koziuk@gmail.com>
*/

#include <cstddef>

#include <gravity/cgame/component/graphics.hpp>
#include <gravity/cgame/asset/model.hpp>

namespace Gravity {
namespace Component {

const std::size_t Graphics::family = 3243241L;

Graphics::Graphics(std::shared_ptr<Asset::Model> model)
	: model(model)
{
	for (const auto& kv : model->GetPathGroups()) {
		if (kv.first == Asset::MAIN_GROUP_LABEL) {
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
