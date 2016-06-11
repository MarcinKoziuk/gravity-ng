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

#include <glm/vec2.hpp>

#include <SDL2/SDL.h>

#include "gravity/game/entity.hpp"
#include "gravity/cgame/renderer/irenderer.hpp"
#include "gravity/cgame/camera.hpp"

namespace Gravity {

class Display : private boost::noncopyable {
public:
    bool isInitialized;
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    Renderer::IRenderer* renderer;
    Camera* camera;

public:
    Display();

    ~Display();

    void Clear();

    void Present();

    void SetCamera(Camera& camera);

    void DrawEntity(const Entity& entity);

    bool Init();
	bool Init2();

	bool IsGL2();

	bool IsGL3();

    SDL_Window* GetWindow() { return window; }

	glm::tvec2<int> GetWindowSize();
};

} // namespace Gravity

#endif /* GRAVITY_CGAME_DISPLAY_HPP */
