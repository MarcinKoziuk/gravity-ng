/*
 * cgame/ui/ui.cpp
 *
 * Copyright (c) 2016
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <SDL2/SDL.h>

#include <sdl_scancode_to_dinput_mappings.h>

#include <gravity/game/logging.hpp>

#include <gravity/cgame/ui/ceguiphysfsresourceprovider.hpp>
#include <gravity/cgame/ui/ui.hpp>

using namespace CEGUI;

namespace Gravity {

PhysFSResourceProvider* UI::resourceProvider;
CEGUI::OpenGLRendererBase* UI::renderer;
bool UI::isGL3Renderer;

void UI::CreateRenderer(Display& display)
{
	glm::vec2 windowSize = display.GetWindowSize();

	// create renderer and enable extra states

	if (display.IsGL3()) {
		UI::renderer = &OpenGL3Renderer::create(Sizef(windowSize.x, windowSize.y));
		UI::isGL3Renderer = true;
	} else {
		UI::renderer = &OpenGLRenderer::create(Sizef(windowSize.x, windowSize.y));
		UI::isGL3Renderer = false;
	}

	UI::renderer->enableExtraStateSettings(true);
}

void UI::Init(Display& display)
{
	using namespace CEGUI;

	// use physfs resource provider
	UI::resourceProvider = new PhysFSResourceProvider();

	// create GL renderer depending on version
	CreateRenderer(display);

	// create CEGUI system object
	System::create(*UI::renderer, static_cast<CEGUI::ResourceProvider*>(UI::resourceProvider));

	// setup resource directories
	//DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>(System::getSingleton().getResourceProvider());
	resourceProvider->setResourceGroupDirectory("schemes", "cegui/schemes/");
	resourceProvider->setResourceGroupDirectory("imagesets", "cegui/imagesets/");
	resourceProvider->setResourceGroupDirectory("fonts", "cegui/fonts/");
	resourceProvider->setResourceGroupDirectory("layouts", "cegui/layouts/");
	resourceProvider->setResourceGroupDirectory("looknfeels", "cegui/looknfeel/");
	resourceProvider->setResourceGroupDirectory("lua_scripts", "cegui/lua_scripts/");
	resourceProvider->setResourceGroupDirectory("schemas", "cegui/xml_schemas/");

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

void UI::Deinit()
{
	CEGUI::System::destroy();

	if (UI::isGL3Renderer) {
		CEGUI::OpenGL3Renderer::destroy(*((CEGUI::OpenGL3Renderer*)renderer));
	} else {
		CEGUI::OpenGLRenderer::destroy(*((CEGUI::OpenGLRenderer*)renderer));
	}

	delete UI::resourceProvider;
}

void UI::Render()
{
	UI::renderer->beginRendering();
	CEGUI::System::getSingleton().renderAllGUIContexts();
	UI::renderer->endRendering();
}

void UI::InjectTimePulse(float timeElapsed)
{
	CEGUI::System::getSingleton().injectTimePulse(timeElapsed);
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(timeElapsed);
}

void UI::InitWindows()
{
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

// convert SDL scancode to CEGUI scancode
static CEGUI::Key::Scan ToCEGUIKey(SDL_Scancode key)
{
	return static_cast<CEGUI::Key::Scan>(scanCodeToKeyNum[static_cast<int>(key)]);
}

// convert SDL mouse button to CEGUI mouse button
static CEGUI::MouseButton ToCEGUIMouseButton(const Uint8& button)
{
	using namespace CEGUI;

	switch (button) {
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

static void InjectUTF8Text(const char* utf8str)
{
	static SDL_iconv_t cd = SDL_iconv_t(-1);

	if (cd == SDL_iconv_t(-1)) {
		// note: just "UTF-32" doesn't work as toFormat, because then you get BOMs, which we don't want.
		const char* toFormat = "UTF-32LE"; // TODO: what does CEGUI expect on big endian machines?
		cd = SDL_iconv_open(toFormat, "UTF-8");
		if (cd == SDL_iconv_t(-1)) {
			LOG(fatal) << "Couldn't initialize SDL_iconv for UTF-8 to UTF-32!";
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

	if (n == size_t(-1)) { // some error occured during iconv
		LOG(fatal) << "Converting UTF-8 string " << utf8str << " from SDL_TEXTINPUT to UTF-32 failed!";
	}

	for (int i = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; ++i) {
		if (utf32buf[i] == 0) {
			break; // end of string
		}

		CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(utf32buf[i]);
	}

	// reset cd so it can be used again
	SDL_iconv(cd, NULL, &inbytesleft, NULL, &outbytesleft);
}

void UI::HandleEvent(SDL_Event& event)
{
	switch (event.type) {
	case(SDL_KEYDOWN):
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(ToCEGUIKey(event.key.keysym.scancode));
		break;

	case(SDL_KEYUP):
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(ToCEGUIKey(event.key.keysym.scancode));
		break;

	case SDL_MOUSEMOTION:
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(static_cast<float>(event.motion.x),
			static_cast<float>(event.motion.y));
		break;

	case SDL_MOUSEBUTTONDOWN:
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(ToCEGUIMouseButton(event.button.button));
		break;

	case SDL_MOUSEBUTTONUP:
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(ToCEGUIMouseButton(event.button.button));
		break;

	case SDL_MOUSEWHEEL:
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(static_cast<float>(event.wheel.y));
		break;

	case SDL_TEXTINPUT:
		InjectUTF8Text(event.text.text);
		break;

	case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
			CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(static_cast<float>(event.window.data1),
				static_cast<float>(event.window.data2)));
			glViewport(0, 0, event.window.data1, event.window.data2);
		}
		else if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseLeaves();
		}
		break;
	}
}

}