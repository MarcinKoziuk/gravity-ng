/*
 * cgame/renderer/nvgrenderer.hpp
 *
 * NanoVG renderer
 *
 * Copyright (c) 2016
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_CGAME_RENDERER_NVGRENDERER_HPP
#define GRAVITY_CGAME_RENDERER_NVGRENDERER_HPP

#include <vector>

#include <boost/optional.hpp>

#include <glm/glm.hpp>

#include <gravity/cgame/rgb.hpp>
#include <gravity/cgame/renderer/irenderer.hpp>

struct SDL_Window;
struct NVGcontext;

namespace Gravity {
namespace Renderer {

class NvgRenderer : public IRenderer {
public:
	NvgRenderer(SDL_Window* window);

	virtual ~NvgRenderer();

    virtual bool Init();

    virtual void Clear();

    virtual void Present();

	virtual void DrawTriangles(const std::vector<glm::vec2>& vertices);

	virtual void DrawLines(const std::vector<glm::vec2>& lines, RGBA color, bool closed);

private:
	void DrawLine(RGBA color, const glm::vec2& p1, const glm::vec2& p2);

private:
	SDL_Window* window;
	NVGcontext* ctx;

};

} // namespace Renderer
} // namespace Gravity

#endif /* GRAVITY_CGAME_RENDERER_NVGRENDERER_HPP */
