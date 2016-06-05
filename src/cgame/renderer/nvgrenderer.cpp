/*
 * cgame/renderer/nvgrenderer.cpp
 *
 * Copyright (c) 2014-2016
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstdint>

#include <GL/glew.h>

#include <SDL/SDL_gpu.h>
#include <nanovg.h>

#define NANOVG_GL3 1
#define NANOVG_GL_IMPLEMENTATION 1
#include <nanovg_gl.h>

#include <gravity/game/logging.hpp>
#include <gravity/cgame/renderer/nvgrenderer.hpp>

namespace Gravity {
namespace Renderer {

static constexpr int LOGICAL_RENDER_WIDTH = 640;
static constexpr int LOGICAL_RENDER_HEIGHT = 480;
static constexpr double SCALE_FACTOR = 48;

NvgRenderer::NvgRenderer(SDL_Window* window)
    : window(window)
{}

NvgRenderer::~NvgRenderer()
{
	nvgDeleteGL3(ctx);
}

bool NvgRenderer::Init()
{
	ctx = nvgCreateGL3(/*NVG_ANTIALIAS |*/ NVG_STENCIL_STROKES | NVG_DEBUG);

	return ctx != nullptr;
}

void NvgRenderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	int h, w;
	SDL_GetWindowSize(window, &w, &h);

	nvgBeginFrame(ctx, w, h, 1.0f);
}

void NvgRenderer::Present()
{
	nvgEndFrame(ctx);
}

void NvgRenderer::DrawLine(RGBA color, const glm::vec2& p1, const glm::vec2& p2)
{
	const double w = SCALE_FACTOR;
	const double h = SCALE_FACTOR;

	nvgBeginPath(ctx);
	nvgMoveTo(ctx, (p1.x*w) + LOGICAL_RENDER_WIDTH, (p1.y*h) + LOGICAL_RENDER_HEIGHT);
	nvgLineTo(ctx, (p2.x*w) + LOGICAL_RENDER_WIDTH, (p2.y*h) + LOGICAL_RENDER_HEIGHT);
	nvgStrokeWidth(ctx, 1.0f);
	nvgStrokeColor(ctx, color.ToNVGColor());
	nvgStroke(ctx);
}

void NvgRenderer::DrawTriangles(const std::vector<glm::vec2>& vertices)
{
    assert((vertices.size() % 3) == 0);

    for (std::size_t i = 0; i < vertices.size()/3; i++) {
        const glm::vec2* v = &vertices[i * 3];

		DrawLine(RGBA(255, 255, 255, 1.0f), v[0], v[1]);
		DrawLine(RGBA(255, 255, 255, 1.0f), v[1], v[2]);
		DrawLine(RGBA(255, 255, 255, 1.0f), v[2], v[0]);
    }
}

void NvgRenderer::DrawLines(const std::vector<glm::vec2>& lines, RGBA color, bool closed)
{
	assert(lines.size() >= 2);

	if (closed) {
		for (int i = 0; i < lines.size(); i++) {
			const glm::vec2& p1 = lines[i];
			const glm::vec2& p2 = i+1 < lines.size() ? lines[i+1] : lines[0];

			DrawLine(color, p1, p2);
		}
	} else {
		for (int i = 1; i < lines.size(); i++) {
			const glm::vec2& p1 = lines[i - 1];
			const glm::vec2& p2 = lines[i];

			DrawLine(color, p1, p2);
		}
	}
}

} // namespace Renderer
} // namespace Gravity
