/*
 * cgame/renderer/glrenderer.hpp
 *
 * OpenGL renderer
 *
 * Copyright (c) 2014-2016
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_CGAME_RENDERER_GLRENDERER_HPP
#define GRAVITY_CGAME_RENDERER_GLRENDERER_HPP

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <SDL2/SDL.h>

#include <glm/glm.hpp>

#include "gravity/game/resource/resourcemanager.hpp"
#include "gravity/cgame/resource/shadersource.hpp"
#include "gravity/cgame/renderer/irenderer.hpp"

namespace Gravity {
namespace Renderer {

class GLRenderer : public IRenderer {
public:
    GLRenderer(SDL_Window* window, SDL_Renderer* sdlRenderer, ResourceManager& resourceManager);

    virtual ~GLRenderer() {}

    virtual bool Init();

    virtual void Clear();

    virtual void Present();

    virtual void DrawTriangles(const std::vector<glm::vec2>& vertices);

private:
    bool CompileShader(const std::string&, unsigned, int*);
    bool CreateProgram(const std::string&, unsigned*);
    void LoadTriangleProgram();

private:
    ResourceManager& resourceManager;
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    SDL_GLContext glContext;

    const unsigned* currentProgram;
    unsigned triangleProgram;
    int attrPosition;
};

} // namespace Renderer
} // namespace Gravity

#endif /* GRAVITY_CGAME_RENDERER_GLRENDERER_HPP */
