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

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>

#include "gravity/logging.hpp"
#include "gravity/game/world.hpp"
#include "gravity/game/entity.hpp"
#include "gravity/game/component/physics.hpp"
#include "gravity/game/resource/resourceloader.hpp"
#include "gravity/game/resource/resourcemanager.hpp"
#include "gravity/game/resource/body.hpp"

#include "gravity/cgame/display.hpp"
#include "gravity/cgame/camera.hpp"
#include "gravity/cgame/resource/model.hpp"

//#include <unistd.h>
using namespace Gravity;
using namespace Game;
using namespace CGame;

int main()
{
    ResourceLoader::Init();
    ResourceManager mgr;
    Display display(mgr);
    display.Init();

    BodyPtr bodyResource = mgr.Load<Body>("bodies/ship.json");

    World world;
    Entity entity(world);
    Entity entity2(world);
    std::vector<Entity*> bulletEntities;
    Component::Physics physicsComponent(entity, bodyResource, glm::vec2(0, 0));
    Component::Physics physicsComponent2(entity, bodyResource, glm::vec2(6, 6));
    entity.AddComponent<Component::Physics>(&physicsComponent);
    entity2.AddComponent<Component::Physics>(&physicsComponent2);

    ModelPtr model = mgr.Load<Model>("models/ship.json");

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

    while (running) {
        Uint32 now = SDL_GetTicks();

        display.Clear();

        display.DrawEntity(entity);
        display.DrawEntity(entity2);
        for (std::size_t i = 0; i < bulletEntities.size(); i++) {
            display.DrawEntity(*bulletEntities[i]);
        }

        display.Present();

        /* Handle input and events: */
        SDL_Event event;
        //const Uint8* states = SDL_GetKeyboardState(NULL);


        int rotate = 0;
        int accel = 0;

        while(SDL_PollEvent(&event) > 0) {
            switch(event.type) {
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
                    BodyPtr bulletResource = mgr.Load<Body>("bodies/bullet.json");
                    Entity* bulletEntity = new Entity(world);
                    b2Vec2 pos = physicsComponent.GetPhysicsBody().GetPosition();
                    glm::vec2 v1(0.f, 1.f);
                    v1 = glm::rotate(v1, body.GetAngle());
                    v1 += glm::vec2(body.GetPosition().x, body.GetPosition().y);

                    Component::Physics* bulletPhysics = new Component::Physics(entity, bulletResource, v1);
                    bulletEntity->AddComponent<Component::Physics>(bulletPhysics);
                    bulletEntities.push_back(bulletEntity);
                    glm::vec2 v(0.f, 6.5f);
                    LOG(info) << body.GetAngle() << " and " << GLM_FORCE_RADIANS;
                    v = glm::rotate(v, body.GetAngle());
                    v += glm::vec2(body.GetLinearVelocity().x, body.GetLinearVelocity().y);

                    bulletPhysics->GetPhysicsBody().SetBullet(true);
                    bulletPhysics->GetPhysicsBody().ApplyForceToCenter(b2Vec2(v.x, v.y), true);
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
        }

        rotate = leftPressed - rightPressed;
        accel = upPressed;


        body.ApplyTorque(rotate*2 - body.GetAngularVelocity(), true);
        glm::vec2 force = glm::rotate(glm::vec2(0.f, float(accel)*2.f), body.GetAngle());
        body.ApplyForce(b2Vec2(force.x, force.y), body.GetPosition(), true);

        float32 timeStep = 1.0f / 30.f;
        int32 velocityIterations = 10;
        int32 positionIterations = 8;

        world.GetPhysics()->Step(timeStep, velocityIterations, positionIterations);


        Uint32 after = SDL_GetTicks();
        Uint32 usedTime = after - now;
        int32 remaining = 15 - usedTime;

        float fps = ( numFrames/(float)(SDL_GetTicks() - startTime) )*1000;
        numFrames++;


        if ((numFrames % 60) == 1) {
            LOG(info) << "FPS:" << fps << " ut:" << usedTime;
        }
        if (numFrames == 240) {
            numFrames = 0;
            startTime = SDL_GetTicks();
        }

        if (remaining > 0) SDL_Delay(remaining);
        b2Vec2 pos = body.GetPosition();
        camera.Update(glm::vec2(pos.x, pos.y));

    }


    ResourceLoader::Deinit();
}

#ifdef _WIN32
#include <windows.h>
int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
{
    main();
}
#endif
