/*
 * cgame/display.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstddef>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <SDL2/SDL.h>

#include <Box2D/Box2D.h>

#include <poly2tri.h>

#include "gravity/logging.hpp"
#include "gravity/game/entity.hpp"
#include "gravity/game/component/physics.hpp"
#include "gravity/cgame/display.hpp"

namespace Gravity {
namespace CGame {

using namespace Game;

Display::Display(Game::ResourceManager& resourceManager)
    : resourceManager(resourceManager)
{}

Display::~Display()
{
   if (isInitialized) SDL_Quit();
   if (renderer != NULL) delete renderer;
}

bool Display::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG(fatal) << "SDL initialization failed: " << SDL_GetError();
        return false;
    }

    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    window = SDL_CreateWindow("Window caption",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_OPENGL);

    if (window == NULL) {
        LOG(fatal) << "SDL Window creation failed: " << SDL_GetError();
        return false;
    }

    sdlRenderer = SDL_CreateRenderer(window, -1, 0);
    if (sdlRenderer == NULL) {
        LOG(fatal) << "SDL Renderer initialization failed: " << SDL_GetError();
        return false;
    }

    renderer = new Renderer::GLRenderer(window, sdlRenderer, resourceManager);

    if (!renderer->Init()) {
        LOG(fatal) << "GL Renderer initialization failed.";
        return false;
    }

    isInitialized = true;
    return true;
}

void Display::Clear()
{
    renderer->Clear();
}

void Display::SetCamera(Camera& camera_)
{
    camera = &camera_;
}

void Display::DrawEntity(const Entity& entity)
{
    if (!entity.HasComponent<Component::Physics>()) {
        LOG(error) << "Why are you trying to render entity without physics " << entity.GetId();
    }

    const Component::Physics& physics = entity.GetComponent<Component::Physics>();

    const b2Body& physicsBody = physics.GetPhysicsBody();
    const b2Fixture* fixtureList = physicsBody.GetFixtureList();

    for (const b2Fixture* fixture = fixtureList; fixture != NULL; fixture = fixture->GetNext()) {
        const b2Shape* shape = fixture->GetShape();

        if (shape->GetType() == b2Shape::e_polygon) {
            const b2PolygonShape* polyShape = reinterpret_cast<const b2PolygonShape*>(shape);

            std::vector<glm::vec2> vertices;

            for (int i = 0; i < polyShape->GetVertexCount(); i++) {
                const b2Vec2& physicsVertex = polyShape->GetVertex(i);
                glm::vec2 vertex(physicsVertex.x, physicsVertex.y);
                vertex = glm::rotate(vertex, physicsBody.GetAngle());

                b2Vec2 pos = physicsBody.GetPosition();
                vertex += glm::vec2(pos.x, pos.y) - camera->GetPos();
                vertex /= glm::vec2(6.4*3, 4.8*3);
                vertices.push_back(vertex);
            }

            if (vertices.size() == 3) {
                renderer->DrawTriangles(vertices);
            } else {
                std::vector<p2t::Point*> polyline;
                for (std::size_t i = 0; i < vertices.size(); i++) {
                    polyline.push_back(new p2t::Point(vertices[i].x, vertices[i].y));
                }

                p2t::CDT cdt(polyline);
                cdt.Triangulate();
                std::vector<p2t::Triangle*> triangles = cdt.GetTriangles();
                for (std::size_t i = 0; i < triangles.size(); i++) {
                    p2t::Triangle* triangle = triangles[i];
                    std::vector<glm::vec2> triangleVertices;
                    for (int j = 0; j < 3; j++) {
                        p2t::Point* point = triangle->GetPoint(j);
                        triangleVertices.push_back(glm::vec2(point->x, point->y));
                    }

                    renderer->DrawTriangles(triangleVertices);
                }

                for (std::size_t i = 0; i < polyline.size(); i++) {
                    delete polyline[i];
                }
            }
        }
    }
}

void Display::Present()
{
    renderer->Present();
}

} // namespace CGame
} // namespace Gravity
