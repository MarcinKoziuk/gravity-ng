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

#include <boost/noncopyable.hpp>

namespace Gravity {
namespace CGame {
namespace Renderer {

class IRenderer : private boost::noncopyable {
public:
    virtual void Clear();

    virtual void Present();

    virtual void DrawLine() = 0;

    virtual ~IRenderer() = 0;
};

} // namespace Renderer
} // namespace CGame
} // namespace Gravity

#endif /* GRAVITY_CGAME_RENDERER_IRENDERER_HPP */
