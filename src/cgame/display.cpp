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

#include "gravity/game/logging.hpp"
#include "gravity/game/entity.hpp"
#include "gravity/game/resource/resourcemanager.hpp"
#include "gravity/game/component/physics.hpp"
#include "gravity/cgame/display.hpp"
#include "gravity/cgame/renderer/glrenderer.hpp"
#include "gravity/cgame/renderer/sdlrenderer.hpp"
#include "gravity/cgame/component/graphics.hpp"

namespace Gravity {

Display::Display(ResourceManager& resourceManager)
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
        1280, 800,
		SDL_WINDOW_RESIZABLE);


    if (window == NULL) {
        LOG(fatal) << "SDL Window creation failed: " << SDL_GetError();
        return false;
    }

    sdlRenderer = SDL_CreateRenderer(window, -1, 0);
    if (sdlRenderer == NULL) {
        LOG(fatal) << "SDL Renderer initialization failed: " << SDL_GetError();
        return false;
    }

    renderer = new Renderer::SDLRenderer(window, sdlRenderer, resourceManager);

    if (!renderer->Init()) {
        LOG(fatal) << "Renderer initialization failed.";
        return false;
    }

    isInitialized = true;
    return true;
}

void Display::Clear()
{
    SDL_RenderClear(sdlRenderer);
    renderer->Clear();
}

void Display::SetCamera(Camera& camera_)
{
    camera = &camera_;
}

void Display::DrawEntity(const Entity& entity)
{
    if (!entity.HasComponent<Component::Graphics>()) {
        LOG(error) << "Why are you trying to render entity without graphics " << entity.GetId();
    }

	const Component::Graphics& graphics = entity.GetComponent<Component::Graphics>();
	for (const VectorPathSet* pathSet : graphics.GetPathSetsToRender()) {
		
		for (const VectorPath& path : pathSet->paths) {
			std::vector<glm::vec2> vertices;

			for (glm::vec2 vertex : path.path) {
				if (entity.HasComponent<Component::Physics>()) {
					const Component::Physics& physics = entity.GetComponent<Component::Physics>();
					vertex = glm::rotate(vertex, physics.GetPhysicsBody().GetAngle());
					b2Vec2 pos = physics.GetPhysicsBody().GetPosition();
					vertex += glm::vec2(pos.x, pos.y) - camera->GetPos();
				}

				vertices.push_back(vertex);
			}

			renderer->DrawLines(vertices, pathSet->color, path.closed);
		}
	}

	if (entity.HasComponent<Component::Physics>() && false) {
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
					//vertex /= glm::vec2(6.4 * 3, 4.8 * 3);
					vertices.push_back(vertex);
				}

				if (vertices.size() == 3) {
					renderer->DrawTriangles(vertices);
				}
				else {
					renderer->DrawLines(vertices, RGBA(255, 0, 255, 0.5), true);

					/*std::vector<p2t::Point*> polyline;
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
					}*/
				}
			}
		}
	}
    
}

void Display::Present()
{
    renderer->Present();

    /*SDL_SetRenderDrawColor(sdlRenderer, 255, 0, 0, 1.0);
    SDL_RenderDrawLine(sdlRenderer, 10, 10, 40, 40);*/

    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);

    SDL_RenderPresent(sdlRenderer);
}

} // namespace Gravity
