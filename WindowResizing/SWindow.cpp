#include "SWindow.h"
#include <sstream>

SWindow::SWindow()
{
	//Initialize non-existant window
	window = NULL;
	mouseFocus = false;
	keyboardFocus = false;
	fullScreen = false;
	minimized = false;
	renderEvent = false;
	width = 0;
	height = 0;
	initWidth = 0;
	initHeight = 0;
}

bool SWindow::init(const char title[336], int x, int y, int w, int h, int flags)
{
	//Create window
	window = SDL_CreateWindow(title, x, y, w, h, flags);
	if (window != NULL)
	{
		mouseFocus = true;
		keyboardFocus = true;
		width = w;
		height = h;
		initWidth = w;
		initHeight = h;
	}

	return window != NULL;
}

SDL_Renderer* SWindow::createRenderer()
{
	return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void SWindow::handleEvent(SDL_Event& e)
{
	//Window event occured
	if (e.type == SDL_WINDOWEVENT)
	{
		//Caption update flag
		bool updateCaption = false;

		switch (e.window.event)
		{
			//Get new dimensions and repaint on window size change
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			width = e.window.data1;
			height = e.window.data2;
			renderEvent = true;

			break;

			//Repaint on exposure
		case SDL_WINDOWEVENT_EXPOSED:
			renderEvent = true;
			break;

			//Mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			mouseFocus = true;
			updateCaption = true;
			break;

			//Mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			mouseFocus = false;
			updateCaption = true;
			break;

			//Window has keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			keyboardFocus = true;
			updateCaption = true;
			break;

			//Window lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			keyboardFocus = false;
			updateCaption = true;
			break;

			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			minimized = true;
			break;

			//Window maximized
		case SDL_WINDOWEVENT_MAXIMIZED:

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			minimized = false;
			break;
		}

		//Update window caption with new data
		if (updateCaption)
		{
			std::stringstream caption;
			caption << "SDL Tutorial - MouseFocus:" << ((mouseFocus) ? "On" : "Off") << " KeyboardFocus:" << ((keyboardFocus) ? "On" : "Off");
			SDL_SetWindowTitle(window, caption.str().c_str());
		}
	}
	//Enter exit full screen on return key
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
	{
		if (fullScreen)
		{
			SDL_SetWindowFullscreen(window, 0);
			fullScreen = false;
		}
		else
		{
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			fullScreen = true;
			minimized = false;
		}
	}
}

void SWindow::free()
{
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	mouseFocus = false;
	keyboardFocus = false;
	width = 0;
	height = 0;
}

int SWindow::getWidth()
{
	return width;
}

int SWindow::getHeight()
{
	return height;
}

bool SWindow::hasMouseFocus()
{
	return mouseFocus;
}

bool SWindow::hasKeyboardFocus()
{
	return keyboardFocus;
}

bool SWindow::isMinimized()
{
	return minimized;
}

bool SWindow::isRenderEvent()
{
	bool cache = renderEvent;
	renderEvent = 0;

	return cache;
}

float SWindow::getScale_x()
{
	return (width + 0.0) / initWidth;
}

float SWindow::getScale_y()
{
	return (height + 0.0) / initHeight;
}