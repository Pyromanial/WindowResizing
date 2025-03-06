#ifndef SWINDOW_H
#define SWINDOW_H

#include "SDL.h"

class SWindow
{
public:
	//Intializes internals
	SWindow();

	//Creates window
	bool init(const char title[336], int x, int y, int w, int h, int flags);

	//Creates renderer from internal window
	SDL_Renderer* createRenderer();

	//Handles window events
	void handleEvent(SDL_Event& e);

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();
	float getScale_x();
	float getScale_y();

	//Window focii
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();

	//Clears renderEvent on use
	bool isRenderEvent();

private:
	//Window data
	SDL_Window* window;

	//Window dimensions
	int width;
	int height;
	int initWidth;
	int initHeight;

	//Window focus
	bool mouseFocus;
	bool keyboardFocus;
	bool fullScreen;
	bool minimized;

	//renderPresent when true -- not currently in use -- more meant for static applications
	bool renderEvent;
};

#endif // !SWINDOW_H
