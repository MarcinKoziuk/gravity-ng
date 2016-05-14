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

void SDLRenderer::DrawTriangles(const std::vector<glm::vec2>& vertices)
{
    assert((vertices.size() % 3) == 0);

    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 255, 1.0);
        //SDL_RenderDrawLine(sdlRenderer, 10, 10, 40, 40);

    //LOG(info) << "sz" <<vertices.size();

    for (std::size_t i = 0; i < vertices.size()/3; i++) {
        const glm::vec2* tv = &vertices[i * 3];
        const int w = 320;
        const int h = 240;

        SDL_RenderDrawLine(sdlRenderer, (tv[0].x*w)+w*2, (tv[0].y*h)+h*2, (tv[1].x*w)+w*2, (tv[1].y*h)+h*2);
        SDL_RenderDrawLine(sdlRenderer, (tv[1].x*w)+w*2, (tv[1].y*h)+h*2, (tv[2].x*w)+w*2, (tv[2].y*h)+h*2);
        SDL_RenderDrawLine(sdlRenderer, (tv[2].x*w)+w*2, (tv[2].y*h)+h*2, (tv[0].x*w)+w*2, (tv[0].y*h)+h*2);
    }
}

} // namespace Renderer
} // namespace Gravity
