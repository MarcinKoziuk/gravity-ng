/*
 * cgame/renderer/glrenderer.hpp
 *
 * Interface for the rendering of drawable components.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_CGAME_RENDERER_GLRENDERER_HPP
#define GRAVITY_CGAME_RENDERER_GLRENDERER_HPP

#include <string>
#include <map>

#include <boost/optional.hpp>

#include <SDL2/SDL.h>

#include <GL/gl.h>
#include <glm/glm.hpp>

#include "gravity/game/resource/resourcemanager.hpp"
#include "gravity/cgame/resource/shadersource.hpp"
#include "gravity/cgame/renderer/irenderer.hpp"

namespace Gravity {
namespace CGame {
namespace Renderer {

class GLRenderer {
public:
    GLRenderer(SDL_Window* window, SDL_Renderer* sdlRenderer, Game::ResourceManager& resourceManager);

    virtual ~GLRenderer() {}

    bool Init();

    virtual void Clear();

    virtual void Present();

    virtual void DrawTriangles(const std::vector<glm::vec2>& vertices);

private:
    bool CompileShader(const std::string&, GLenum, GLint*);
    bool CreateProgram(const std::string&, GLuint*);
    void LoadTriangleProgram();

private:
    Game::ResourceManager& resourceManager;
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    SDL_GLContext glContext;

    const GLuint* currentProgram;
    GLuint triangleProgram;
    GLint attrPosition;
};

} // namespace Renderer
} // namespace CGame
} // namespace Gravity

#endif /* GRAVITY_CGAME_RENDERER_GLRENDERER_HPP */
