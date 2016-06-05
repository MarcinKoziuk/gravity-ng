/*
 * cgame/renderer/sdlrenderer.cpp
 *
 * Copyright (c) 2014-2016
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <vector>

#include "gravity/game/logging.hpp"
#include "gravity/cgame/renderer/sdlrenderer.hpp"

namespace Gravity {
namespace Renderer {

static constexpr int LOGICAL_RENDER_WIDTH = 640;
static constexpr int LOGICAL_RENDER_HEIGHT = 480;
static constexpr double SCALE_FACTOR = 28;

SDLRenderer::SDLRenderer(SDL_Window* window, SDL_Renderer* sdlRenderer, ResourceManager& resourceManager)
    : resourceManager(resourceManager)
    , window(window)
    , sdlRenderer(sdlRenderer)
{}

bool SDLRenderer::Init()
{
    return true;
}

void SDLRenderer::Clear()
{
}

void SDLRenderer::Present()
{
}

void SDLRenderer::DrawLine(const glm::vec2& p1, const glm::vec2& p2)
{
	const double w = SCALE_FACTOR;
	const double h = SCALE_FACTOR;

	SDL_RenderDrawLine(sdlRenderer,
		(p1.x*w) + LOGICAL_RENDER_WIDTH,
		(p1.y*h) + LOGICAL_RENDER_HEIGHT,
		(p2.x*w) + LOGICAL_RENDER_WIDTH,
		(p2.y*h) + LOGICAL_RENDER_HEIGHT
	);
}

void SDLRenderer::DrawTriangles(const std::vector<glm::vec2>& vertices)
{
    assert((vertices.size() % 3) == 0);

    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 255, 255);

    for (std::size_t i = 0; i < vertices.size()/3; i++) {
        const glm::vec2* v = &vertices[i * 3];

		DrawLine(v[0], v[1]);
		DrawLine(v[1], v[2]);
		DrawLine(v[2], v[0]);
    }
}

void SDLRenderer::DrawLines(const std::vector<glm::vec2>& lines, RGBA color, bool closed)
{
	assert(lines.size() >= 2);

	SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);

	if (closed) {
		for (int i = 0; i < lines.size(); i++) {
			const glm::vec2& p1 = lines[i];
			const glm::vec2& p2 = i+1 < lines.size() ? lines[i+1] : lines[0];

			DrawLine(p1, p2);
		}
	} else {
		for (int i = 1; i < lines.size(); i++) {
			const glm::vec2& p1 = lines[i - 1];
			const glm::vec2& p2 = lines[i];

			DrawLine(p1, p2);
		}
	}

}

} // namespace Renderer
} // namespace Gravity
