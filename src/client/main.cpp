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

#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <CEGUI/CEGUI.h>

#include "gravity/game/logging.hpp"
#include "gravity/game/world.hpp"
#include "gravity/game/entity.hpp"
#include "gravity/game/component/physics.hpp"
#include "gravity/game/resource/resourceloader.hpp"
#include "gravity/game/resource/resourcemanager.hpp"
#include "gravity/game/resource/body.hpp"
#include <gravity/game/asset/bodyasset.hpp>

#include "gravity/cgame/display.hpp"
#include "gravity/cgame/camera.hpp"
#include "gravity/cgame/resource/model.hpp"
#include <gravity/cgame/asset/modelasset.hpp>
#include <gravity/cgame/component/graphics.hpp>
#include <gravity/cgame/ui/ceguiphysfsresourceprovider.hpp>

#include <sdl_scancode_to_dinput_mappings.h>

using namespace Gravity;

CEGUI::Key::Scan toCEGUIKey(SDL_Scancode key)
{
	return static_cast<CEGUI::Key::Scan>(scanCodeToKeyNum[static_cast<int>(key)]);
}

void injectUTF8Text(const char* utf8str)
{
	static SDL_iconv_t cd = SDL_iconv_t(-1);

	if (cd == SDL_iconv_t(-1))
	{
		// note: just "UTF-32" doesn't work as toFormat, because then you get BOMs, which we don't want.
		const char* toFormat = "UTF-32LE"; // TODO: what does CEGUI expect on big endian machines?
		cd = SDL_iconv_open(toFormat, "UTF-8");
		if (cd == SDL_iconv_t(-1))
		{
			std::cerr << "Couldn't initialize SDL_iconv for UTF-8 to UTF-32!" << std::endl;
			return;
		}
	}

	// utf8str has at most SDL_TEXTINPUTEVENT_TEXT_SIZE (32) chars,
	// so we won't have have more utf32 chars than that
	Uint32 utf32buf[SDL_TEXTINPUTEVENT_TEXT_SIZE] = { 0 };

	// we'll convert utf8str to a utf32 string, saved in utf32buf.
	// the utf32 chars will be injected into cegui

	size_t len = strlen(utf8str);

	size_t inbytesleft = len;
	size_t outbytesleft = 4 * SDL_TEXTINPUTEVENT_TEXT_SIZE; // *4 because utf-32 needs 4x as much space as utf-8
	char* outbuf = (char*)utf32buf;
	size_t n = SDL_iconv(cd, &utf8str, &inbytesleft, &outbuf, &outbytesleft);

	if (n == size_t(-1)) // some error occured during iconv
	{
		std::cerr << "Converting UTF-8 string " << utf8str << " from SDL_TEXTINPUT to UTF-32 failed!" << std::endl;
	}

	for (int i = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; ++i)
	{
		if (utf32buf[i] == 0)
			break; // end of string

		CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(utf32buf[i]);
	}

	// reset cd so it can be used again
	SDL_iconv(cd, NULL, &inbytesleft, NULL, &outbytesleft);
}


void initCEGUI()
{
	using namespace CEGUI;

	// create renderer and enable extra states
	OpenGL3Renderer& cegui_renderer = OpenGL3Renderer::create(Sizef(800.f, 600.f));
	cegui_renderer.enableExtraStateSettings(true);

	// use physfs resource provider
	PhysFSResourceProvider* rp = new PhysFSResourceProvider();

	// create CEGUI system object
	System::create(cegui_renderer, static_cast<CEGUI::ResourceProvider*>(rp));

	// setup resource directories
	//DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>(System::getSingleton().getResourceProvider());
	rp->setResourceGroupDirectory("schemes", "cegui/schemes/");
	rp->setResourceGroupDirectory("imagesets", "cegui/imagesets/");
	rp->setResourceGroupDirectory("fonts", "cegui/fonts/");
	rp->setResourceGroupDirectory("layouts", "cegui/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "cegui/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "cegui/lua_scripts/");
	rp->setResourceGroupDirectory("schemas", "cegui/xml_schemas/");

	// set default resource groups
	ImageManager::setImagesetDefaultResourceGroup("imagesets");
	Font::setDefaultResourceGroup("fonts");
	Scheme::setDefaultResourceGroup("schemes");
	WidgetLookManager::setDefaultResourceGroup("looknfeels");
	WindowManager::setDefaultResourceGroup("layouts");
	ScriptModule::setDefaultResourceGroup("lua_scripts");

	//XMLParser* parser = System::getSingleton().getXMLParser();
	//if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
	//	parser->setProperty("SchemaDefaultResourceGroup", "schemas");

	// load TaharezLook scheme and DejaVuSans-10 font
	SchemeManager::getSingleton().createFromFile("TaharezLook.scheme", "schemes");
	FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

	// set default font and cursor image and tooltip type
	System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
	System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
	System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");
}


void initWindows()
{
	using namespace CEGUI;

	/////////////////////////////////////////////////////////////
	// Add your gui initialisation code in here.
	// You can use the following code as an inspiration for
	// creating your own windows.
	// But you should preferably use layout loading because you won't
	// have to recompile everytime you change the layout.
	/////////////////////////////////////////////////////////////

	// load layout
	Window* root = WindowManager::getSingleton().loadLayoutFromFile("application_templates.layout");
	System::getSingleton().getDefaultGUIContext().setRootWindow(root);
}

// convert SDL mouse button to CEGUI mouse button
CEGUI::MouseButton SDLtoCEGUIMouseButton(const Uint8& button)
{
	using namespace CEGUI;

	switch (button)
	{
	case SDL_BUTTON_LEFT:
		return LeftButton;

	case SDL_BUTTON_MIDDLE:
		return MiddleButton;

	case SDL_BUTTON_RIGHT:
		return RightButton;

	case SDL_BUTTON_X1:
		return X1Button;

	case SDL_BUTTON_X2:
		return X2Button;

	default:
		return NoButton;
	}
}

int main(int argc, char* argv[])
{
    ResourceLoader::Init();
    ResourceManager mgr;
    Display display(mgr);
    display.Init();
	initCEGUI();
	display.Init2();

	LOG(info) << "our bits is " << sizeof(void*);

    BodyPtr bodyResource = mgr.Load<Body>("bodies/ship.json");

	std::shared_ptr<ModelAsset> shippie = std::make_shared<ModelAsset>("models/ships/interceptor-0/interceptor-0.yml");

    World world;
    Entity entity(world);
    Entity entity2(world);
    std::vector<Entity*> bulletEntities;
	Component::Graphics g1(shippie);
	Component::Graphics g2(shippie);

    Component::Physics physicsComponent(entity.GetWorld(), *shippie, glm::vec2(0, 0));
    Component::Physics physicsComponent2(entity.GetWorld(), *shippie, glm::vec2(6, 6));
    entity.AddComponent<Component::Physics>(&physicsComponent);
    entity2.AddComponent<Component::Physics>(&physicsComponent2);

	entity.AddComponent<Component::Graphics>(&g1);
	entity2.AddComponent<Component::Graphics>(&g2);

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

    BodyPtr bulletResource = mgr.Load<Body>("bodies/bullet.json");


	glm::tvec2<int> windowSize = display.GetWindowSize();
	CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(windowSize.x, windowSize.y));

	initWindows();
	CEGUI::OpenGL3Renderer* renderer = static_cast<CEGUI::OpenGL3Renderer*>(CEGUI::System::getSingleton().getRenderer());
	
	LOG(info) << glGetString(GL_VERSION);

	

	float time = SDL_GetTicks() / 1000.f;


    while (running) {
        Uint32 now = SDL_GetTicks();

        display.Clear();


		//glClear(GL_COLOR_BUFFER_BIT);

		// inject time pulses
		const float newtime = SDL_GetTicks() / 1000.f;
		const float time_elapsed = newtime - time;
		CEGUI::System::getSingleton().injectTimePulse(time_elapsed);
		CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(time_elapsed);
		time = newtime;





       display.DrawEntity(entity);
        display.DrawEntity(entity2);
    //    for (std::size_t i = 0; i < bulletEntities.size(); i++) {
     //       display.DrawEntity(*bulletEntities[i]);
     //   }

        display.Present();


		// render gui
		renderer->beginRendering();
		CEGUI::System& sys = CEGUI::System::getSingleton();
		sys.renderAllGUIContexts();
		renderer->endRendering();

		// swap buffers
		SDL_GL_SwapWindow(display.window);



        /* Handle input and events: */
        SDL_Event event;
       // const Uint8* states = SDL_GetKeyboardState(NULL);


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
                    bulletPhysics->GetPhysicsBody().ApplyForceToCenter(b2Vec2(v.x, v.y), true);
                }

				CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(toCEGUIKey(event.key.keysym.scancode));

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

				CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(toCEGUIKey(event.key.keysym.scancode));
                break;
			
			case SDL_MOUSEMOTION:
				CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(static_cast<float>(event.motion.x),
					static_cast<float>(event.motion.y));
				break;

			case SDL_MOUSEBUTTONDOWN:
				CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(SDLtoCEGUIMouseButton(event.button.button));
				break;

			case SDL_MOUSEBUTTONUP:
				CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(SDLtoCEGUIMouseButton(event.button.button));
				break;

			case SDL_MOUSEWHEEL:
				CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(static_cast<float>(event.wheel.y));
				break;

			case SDL_TEXTINPUT:
				injectUTF8Text(event.text.text);
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(static_cast<float>(event.window.data1),
						static_cast<float>(event.window.data2)));
					glViewport(0, 0, event.window.data1, event.window.data2);
				}
				else if (event.window.event == SDL_WINDOWEVENT_LEAVE)
				{
					CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseLeaves();
				}
				break;
            }
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

    return 0;
}
