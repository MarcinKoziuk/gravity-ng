/*
 * cgame/renderer/sdlrenderer.hpp
 *
 * Software SDL renderer
 *
 * Copyright (c) 2014-2016
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_CGAME_RENDERER_SDLRENDERER_HPP
#define GRAVITY_CGAME_RENDERER_SDLRENDERER_HPP

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <glm/glm.hpp>

#include <SDL2/SDL.h>

#include "gravity/game/resource/resourcemanager.hpp"
#include "gravity/cgame/renderer/irenderer.hpp"

namespace Gravity {
namespace Renderer {

class SDLRenderer : public IRenderer {
public:
    SDLRenderer(SDL_Window* window, SDL_Renderer* sdlRenderer, ResourceManager& resourceManager);

    virtual ~SDLRenderer() {}

    virtual bool Init();

    virtual void Clear();

    virtual void Present();

    virtual void DrawTriangles(const std::vector<glm::vec2>& vertices);

private:
    ResourceManager& resourceManager;
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
};

} // namespace Renderer
} // namespace Gravity

#endif /* GRAVITY_CGAME_RENDERER_GLRENDERER_HPP */
