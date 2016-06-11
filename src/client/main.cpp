/*
 * client/main.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#ifdef _WIN32
#	include <gravity/windows.hpp>
#endif

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#ifdef __MINGW32__
#	undef main
#endif

#include <gravity/gravity.hpp>

#include "gravity/game/logging.hpp"
#include "gravity/game/world.hpp"
#include "gravity/game/entity.hpp"
#include "gravity/game/component/physics.hpp"
#include "gravity/game/resource/resourceloader.hpp"
#include <gravity/game/asset/body.hpp>

#include "gravity/cgame/display.hpp"
#include "gravity/cgame/camera.hpp"
#include <gravity/cgame/asset/model.hpp>
#include <gravity/cgame/component/graphics.hpp>
#include <gravity/cgame/ui/ui.hpp>

using namespace Gravity;

int main(int argc, char* argv[])
{
    ResourceLoader::Init();
    Display display;
    display.Init();
	display.Init2();
	UI::Init(display);
	
    //BodyPtr bodyResource = mgr.Load<Body>("bodies/ship.json");

	std::shared_ptr<Asset::Model> shippie = std::make_shared<Asset::Model>("models/ships/interceptor-0/interceptor-0.yml");
	std::shared_ptr<Asset::Model> plannie = std::make_shared<Asset::Model>("models/planets/simple/simple.yml");

    World world;
    Entity entity(world);
    Entity entity2(world);
	Entity planet(world);
    std::vector<Entity*> bulletEntities;
	Component::Graphics g1(shippie);
	Component::Graphics g2(shippie);
	Component::Graphics g3(plannie);

    Component::Physics physicsComponent(entity.GetWorld(), *shippie, glm::vec2(0, 0));
    Component::Physics physicsComponent2(entity.GetWorld(), *shippie, glm::vec2(6, 6));
	Component::Physics physicsComponent3(planet.GetWorld(), *plannie, glm::vec2(-5, 0));
    entity.AddComponent<Component::Physics>(&physicsComponent);
    entity2.AddComponent<Component::Physics>(&physicsComponent2);
	planet.AddComponent<Component::Physics>(&physicsComponent3);

	entity.AddComponent<Component::Graphics>(&g1);
	entity2.AddComponent<Component::Graphics>(&g2);
	planet.AddComponent<Component::Graphics>(&g3);

    //ModelPtr model = mgr.Load<Model>("models/ship.json");

    bool running = true;

    long numFrames = 0;
    Uint32 startTime = SDL_GetTicks();

    bool leftPressed = false;
    bool rightPressed = false;
    bool upPressed = false;

    b2Body& body = physicsComponent.GetPhysicsBody();
    b2Vec2 pos = body.GetPosition();
    Camera camera(glm::vec2(24, 16), glm::vec2(pos.x, pos.y));
    display.SetCamera(camera);

    //BodyPtr bulletResource = mgr.Load<Body>("bodies/bullet.json");

	
	UI::InitWindows();
	
	LOG(info) << glGetString(GL_VERSION);

	
	float time = SDL_GetTicks() / 1000.f;


    while (running) {
        Uint32 now = SDL_GetTicks();

        display.Clear();


		//glClear(GL_COLOR_BUFFER_BIT);

		// inject time pulses
		const float newtime = SDL_GetTicks() / 1000.f;
		const float time_elapsed = newtime - time;
		UI::InjectTimePulse(time_elapsed);
		time = newtime;





       display.DrawEntity(entity);
        display.DrawEntity(entity2);
		display.DrawEntity(planet);
    //    for (std::size_t i = 0; i < bulletEntities.size(); i++) {
     //       display.DrawEntity(*bulletEntities[i]);
     //   }

        display.Present();


		// render gui
		UI::Render();

		// swap buffers
		SDL_GL_SwapWindow(display.window);

        /* Handle input and events: */
        SDL_Event event;
       // const Uint8* states = SDL_GetKeyboardState(NULL);


        int rotate = 0;
        int accel = 0;

        while(SDL_PollEvent(&event) > 0) {
			switch (event.type) {
			case(SDL_QUIT):
				running = false;
				break;
			case(SDL_KEYDOWN):
				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					leftPressed = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					rightPressed = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
					upPressed = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					/*
					Entity* bulletEntity = new Entity(world);
					b2Vec2 pos = physicsComponent.GetPhysicsBody().GetPosition();
					glm::vec2 v1(0.f, 1.f);
					v1 = glm::rotate(v1, body.GetAngle());
					v1 += glm::vec2(body.GetPosition().x, body.GetPosition().y);

					Component::Physics* bulletPhysics = new Component::Physics(entity.GetWorld(), bulletResource, v1);
					bulletEntity->AddComponent<Component::Physics>(bulletPhysics);
					bulletEntities.push_back(bulletEntity);
					glm::vec2 v(0.f, 6.5f);
					LOG(info) << body.GetAngle() << " and " << GLM_FORCE_RADIANS;
					v = glm::rotate(v, body.GetAngle());
					v += glm::vec2(body.GetLinearVelocity().x, body.GetLinearVelocity().y);

					bulletPhysics->GetPhysicsBody().SetBullet(true);
					bulletPhysics->GetPhysicsBody().ApplyForceToCenter(b2Vec2(v.x, v.y), true);*/
				}
				break;
			case(SDL_KEYUP):
				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					leftPressed = false;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					rightPressed = false;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
					upPressed = false;
				}

				break;
			}
			UI::HandleEvent(event);
        }

        rotate = rightPressed - leftPressed;
        accel = upPressed;


        body.ApplyTorque(rotate*2 - body.GetAngularVelocity(), true);
        glm::vec2 force = glm::rotate(glm::vec2(0.f, float(accel)*-2.f), body.GetAngle());
        body.ApplyForce(b2Vec2(force.x, force.y), body.GetPosition(), true);

        float32 timeStep = 1.0f / 30.f;
        int32 velocityIterations = 10;
        int32 positionIterations = 8;

        world.GetPhysics()->Step(timeStep, velocityIterations, positionIterations);

        b2Vec2 pos = body.GetPosition();
        camera.Update(glm::vec2(pos.x, pos.y));

    }

	UI::Deinit();

    ResourceLoader::Deinit();



    return 0;
}
