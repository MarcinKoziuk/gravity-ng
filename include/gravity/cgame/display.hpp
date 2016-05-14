/*
 * cgame/display.hpp
 *
 * Initializes the SDL graphics context
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_CGAME_DISPLAY_HPP
#define GRAVITY_CGAME_DISPLAY_HPP

#include <vector>

#include <boost/noncopyable.hpp>

#include <SDL2/SDL.h>

#include "gravity/game/entity.hpp"
#include "gravity/game/resource/resourcemanager.hpp"
#include "gravity/cgame/renderer/irenderer.hpp"
#include "gravity/cgame/camera.hpp"

namespace Gravity {

class Display : private boost::noncopyable {
private:
    ResourceManager& resourceManager;
    bool isInitialized;
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    Renderer::IRenderer* renderer;
    Camera* camera;

public:
    Display(ResourceManager& resourceManager);

    ~Display();

    void Clear();

    void Present();

    void SetCamera(Camera& camera);

    void DrawEntity(const Entity& entity);

    bool Init();

    SDL_Window* GetWindow()
    { return window; }
};

} // namespace Gravity

#endif /* GRAVITY_CGAME_DISPLAY_HPP */
