/*
* cgame/component/graphics.hpp
*
* Copyright (c) 2016
* Marcin Koziuk <marcin.koziuk@gmail.com>
*/

#ifndef GRAVITY_CGAME_COMPONENT_GRAPHICS_HPP
#define GRAVITY_CGAME_COMPONENT_GRAPHICS_HPP

#include <cstddef>
#include <memory>

#include <gravity/game/component/icomponent.hpp>

#include <gravity/cgame/asset/modelasset.hpp>

namespace Gravity {
namespace Component {

class Graphics : public IComponent {
private:
	std::shared_ptr<ModelAsset> model;
	std::vector<const VectorPathSet*> pathSetsToRender;

public:
	static const std::size_t family;

	Graphics(std::shared_ptr<ModelAsset> model);

	~Graphics();

	std::vector<const VectorPathSet*> GetPathSetsToRender() const;
};

} // namespace Component
} // namespace Gravity

#endif /* GRAVITY_CGAME_COMPONENT_GRAPHICS_HPP */
