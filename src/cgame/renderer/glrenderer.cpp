/*
 * cgame/renderer/glrenderer.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstddef>
#include <cassert>
#include <vector>

#include <boost/algorithm/string.hpp>

#include <glm/glm.hpp>

#ifdef _WIN32
    #include "gravity/windows.hpp"
#endif

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>

#include "gravity/game/logging.hpp"
#include "gravity/cgame/resource/shadersource.hpp"
#include "gravity/cgame/display.hpp"
#include "gravity/cgame/renderer/glrenderer.hpp"

namespace Gravity {
namespace Renderer {

GLRenderer::GLRenderer(SDL_Window* window, SDL_Renderer* sdlRenderer, ResourceManager& resourceManager)
    : resourceManager(resourceManager)
    , window(window)
    , sdlRenderer(sdlRenderer)
    , currentProgram(NULL)
{}

bool GLRenderer::CompileShader(const std::string& shaderKey, GLenum shaderType, GLint* dest)
{
    ShaderSourcePtr resource = resourceManager.Load<ShaderSource>(shaderKey, "gl_renderer", false);
    const char *source = &resource->GetSource()[0];

    GLint result = GL_FALSE;
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result) {
        LOG(error) << "Error while compiling shader " << shaderKey;
        return false;
    } else {
        LOG(trace) << "Compiled shader " << shaderKey;
        *dest = shader;
        return true;
    }
}

bool GLRenderer::CreateProgram(const std::string& shaderName, GLuint* dest)
{
    GLint fragmentShader;
    GLint vertexShader;

    if (!CompileShader(shaderName + ".v.glsl", GL_VERTEX_SHADER, &vertexShader)) {
        return false;
    }
    if (!CompileShader(shaderName + ".f.glsl", GL_FRAGMENT_SHADER, &fragmentShader)) {
        return false;
    }

    GLuint program;
    GLint result = GL_FALSE;
    program = glCreateProgram();

    glAttachShader(program, fragmentShader);
    glAttachShader(program, vertexShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result) {
        LOG(error) << "Shader program creation failed";
        return false;
    }

    *dest = program;

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    LOG(trace) << "Created shader program for " + shaderName;
    return true;
}

bool GLRenderer::Init()
{
    glContext = SDL_GL_CreateContext(window);
    if (glContext == NULL) {
        LOG(fatal) << "GL context initialization failed: " << SDL_GetError();
        return false;
    }

    int major, minor;

    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
    LOG(info) << "Running with OpenGL " << glGetString(GL_VERSION);

    glewExperimental = GL_TRUE;
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        LOG(fatal) << "GLEW could not be initialized: " << glewGetErrorString(glew_status);
        return false;
    }

    if (!CreateProgram("shaders/triangle", &triangleProgram)) {
        return false;
    }

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    return true;
}

void GLRenderer::Clear()
{
    //SDL_RenderClear(sdlRenderer);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLRenderer::Present()
{
    //SDL_RenderPresent(sdlRenderer);
}

void GLRenderer::LoadTriangleProgram()
{
    const char* attributeName = "position";
    attrPosition = glGetAttribLocation(triangleProgram, attributeName);
    if (attrPosition == -1) {
        LOG(fatal) << "Could not bind position attribute";
        return;
    }

    glUseProgram(triangleProgram);
    glEnableVertexAttribArray(attrPosition);

    currentProgram = &triangleProgram;

    //somewhere
    //glDisableVertexAttribArray(attrCoord2d);
}

void GLRenderer::DrawTriangles(const std::vector<glm::vec2>& vertices)
{
    if (currentProgram != &triangleProgram) {
        LoadTriangleProgram();
    }

    assert((vertices.size() % 3) == 0);

    for (std::size_t i = 0; i < vertices.size()/3; i++) {
        const glm::vec2* triangleVertices = &vertices[i * 3];

        glVertexAttribPointer(
            attrPosition,  // attribute
            2,               // number of elements per vertex, here (x,y)
            GL_FLOAT,        // the type of each element
            GL_FALSE,        // take our values as-is
            0,               // no extra data between each position
            triangleVertices // pointer to the C array
        );
    }

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

} // namespace Renderer
} // namespace Gravity
