/*
 * cgame/renderer/irenderer.hpp
 *
 * Interface for the rendering of simple primitives.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_CGAME_RENDERER_IRENDERER_HPP
#define GRAVITY_CGAME_RENDERER_IRENDERER_HPP

#include <vector>

#include <boost/noncopyable.hpp>

#include <gravity/cgame/rgb.hpp>

#include <glm/vec2.hpp>

namespace Gravity {
namespace Renderer {

class IRenderer : private boost::noncopyable {
public:
    virtual bool Init() = 0;

    virtual void Clear() = 0;

    virtual void Present() = 0;

    //virtual void DrawLine() = 0;

    virtual void DrawTriangles(const std::vector<glm::vec2>& vertices) = 0;

	virtual void DrawLines(const std::vector<glm::vec2>& lines, RGBA color, bool closed) = 0;

    virtual ~IRenderer() {}
};

} // namespace Renderer
} // namespace Gravity

#endif /* GRAVITY_CGAME_RENDERER_IRENDERER_HPP */
