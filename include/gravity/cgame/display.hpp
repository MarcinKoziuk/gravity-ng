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
#include "gravity/cgame/renderer/glrenderer.hpp"
#include "gravity/cgame/camera.hpp"

namespace Gravity {
namespace CGame {

class Display : private boost::noncopyable {
private:
    Game::ResourceManager& resourceManager;
    bool isInitialized;
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    Renderer::GLRenderer* renderer;
    Camera* camera;

public:
    Display(Game::ResourceManager& resourceManager);

    ~Display();

    void Clear();

    void Present();

    void SetCamera(Camera& camera);

    void DrawEntity(const Game::Entity& entity);

    bool Init();

    SDL_Window* GetWindow()
    { return window; }
};

} // namespace CGame
} // namespace Gravity

#endif /* GRAVITY_CGAME_DISPLAY_HPP */
