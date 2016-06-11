/*
 * cgame/ui/ui.hpp
 *
 * Copyright (c) 2016
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <gravity/cgame/display.hpp>

#include <CEGUI/RendererModules/OpenGL/RendererBase.h>

union SDL_Event;

namespace Gravity {
class PhysFSResourceProvider;

class UI {
private:
	static PhysFSResourceProvider* resourceProvider;
	static CEGUI::OpenGLRendererBase* renderer;
	static bool isGL3Renderer;

	static void CreateRenderer(Display& display);
public:
	static void Init(Display& display);

	static void InitWindows();

	static void Render();

	static void InjectTimePulse(float timeElapsed);

	static void HandleEvent(SDL_Event& e);

	static void Deinit();
};

} // namespace Gravity