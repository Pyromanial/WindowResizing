#include "SDL.h"
#include "SDL_image.h"

#include "stdio.h"
#include "string"

#include "UniTexture.h"

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

//Paths go here, make more vars if needed, set to "NULL" if nothing to load
const std::string unclickButt_p = "MouseEventGUI/UnclickButt.png";
const std::string clickButt_p = "MouseEventGUI/ClickButt.png";
const std::string red_p = "MouseEventGUI/Red.png";
const std::string blue_p = "MouseEventGUI/Blue.png";

//!Make sure to include all necessary flags! here by or'ing them together (e.g 'imgInitFlags = IMG_INIT_JPG | IMG_INIT_PNG' )
const int sdlInitFlags = SDL_INIT_VIDEO;
const int imgInitFlags = IMG_INIT_JPG;
const int windowFlags = 0;
const int rendererFlags = 0;

const bool renderInstead = true; //Set to true if you would like to initialize renderer instead of screen surface

//window creation values -- !See flags section for windowFlags!
const int window_w = 1000;
const int window_h = 500;
const int window_x = SDL_WINDOWPOS_UNDEFINED;
const int window_y = SDL_WINDOWPOS_UNDEFINED;
const char windowTitle[] = "Test"; //!Make sure to update size when changing titles!

bool init()
{
	bool success = true;
	if (SDL_Init(sdlInitFlags) < 0)
	{
		printf("Failed to initialize SDL subsystems! ERR:%s\n", SDL_GetError());
		success = false;
	}
	else if (!(IMG_Init(imgInitFlags) & imgInitFlags))
	{
		printf("Failed to initialize SDL_image subsystems! ERR:%s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow(windowTitle, window_x, window_y, window_w, window_h, windowFlags);
		if (gWindow == NULL)
		{
			printf("Failed to create window! Err:%s\n", SDL_GetError());
			success = false;
		}
		else
		{
			if (!renderInstead)
			{
				gScreenSurface = SDL_GetWindowSurface(gWindow);
				if (gScreenSurface == NULL)
				{
					printf("Failed to generate surface from window! Err:%s\n", SDL_GetError());
					success = false;
				}
			}
			else
			{
				gRenderer = SDL_CreateRenderer(gWindow, -1, rendererFlags);
				if (gRenderer == NULL)
				{
					printf("Failed to generate renderer from window! Err:%s\n", SDL_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

void close()
{
	SDL_FreeSurface(gScreenSurface);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	//Destroy/free all other globals

	SDL_Quit();
}

int main(int argc, char* args[])
{
	UniTexture button[2];
	UniTexture back[2];

	bool buttState = false;

	int mousePos[2];

	init();

	back[0].setRenderTarget(gRenderer);
	back[1].setRenderTarget(gRenderer);
	button[0].setRenderTarget(gRenderer);
	button[1].setRenderTarget(gRenderer);

	button[0].loadFromImage(unclickButt_p);
	button[1].loadFromImage(clickButt_p);

	back[0].loadFromImage(blue_p);
	back[1].loadFromImage(red_p);

	int buttPos[2] = { (window_w - button[0].getWidth())/2,(window_h - button[0].getHeight())/2};

	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e)!=0)
		{
			if (e.type == SDL_QUIT)
				quit = true;
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_GetMouseState(&mousePos[0], &mousePos[1]);
				buttState = ((mousePos[0] > buttPos[0]) && (mousePos[0] < (buttPos[0] + button[0].getWidth())));
				buttState *= ((mousePos[1] > buttPos[1]) && (mousePos[1] < (buttPos[1] + button[0].getHeight())));
			}
			else
				buttState = 0;
		}

		back[buttState].render(0,0);
		button[buttState].render(buttPos[0]+3*buttState, buttPos[1] + 3 * buttState);
		SDL_RenderPresent(gRenderer);
	}

	close();
	return 0;
}