//Hand image adapted from freepik
//Hourglass image adapted from vecteezy

#include "SDL.h"
#include "SDL_image.h"
#include "stdio.h"
#include "string"
#include "UniTexture.h"
#include "SWindow.h"

SWindow gWindow;
SDL_Renderer* gRenderer = NULL;

const std::string texturePath = "Hourglass.png";
const std::string backPath = "Hand.jpg";

//!Make sure to include all necessary flags! here by or'ing them together (e.g 'imgInitFlags = IMG_INIT_JPG | IMG_INIT_PNG' )
const int sdlInitFlags = SDL_INIT_VIDEO;
const int imgInitFlags = IMG_INIT_JPG;
const int windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
const int rendererFlags = SDL_RENDERER_PRESENTVSYNC;

//window creation values -- !See flags section for windowFlags!
const int window_w = 1000;
const int window_h = 500;
const int window_x = SDL_WINDOWPOS_UNDEFINED;
const int window_y = SDL_WINDOWPOS_UNDEFINED;
const char windowTitle[] = "Test";

float getMin(float* vals)
{
	float smallest = *vals;
	for (int i = 0; *(vals + i) != -1; i++)
	{
		if (*(vals + i) < *vals)
		{
			smallest = *(vals+i);
		}
	}

	return smallest;
}

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
		if (gWindow.init(windowTitle, window_x, window_y, window_w, window_h, windowFlags) == NULL)
		{
			printf("Failed to create window! Err:%s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = gWindow.createRenderer();
			if (gRenderer == NULL)
			{
				printf("Failed to generate renderer from window! Err:%s\n", SDL_GetError());
				success = false;
			}
		}
	}
	return success;
}

void close()
{
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();

	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Could not initialize!");
	}
	else
	{
		UniTexture timeText;
		UniTexture handText;

		timeText.setRenderTarget(gRenderer);
		handText.setRenderTarget(gRenderer);

		if (!timeText.loadFromImage(texturePath, new int[3] {0, 255, 255}))
		{
			printf("Could not generate texture from %s!", texturePath.c_str());
		}
		else if (!handText.loadFromImage(backPath))
		{
			printf("Could not generate texture from %s!", backPath.c_str());
			timeText.free();
		}
		else
		{
			//It would be much more efficient to create a "sprite" class, if this weren't a one-time deal
			int sprite_w = 179;
			int sprite_h = 470;
			int spriteBuff_x = 20;
			int spriteBuff_y = 87;
			int frameT = 8;
			int frameTop = frameT * 6;
			int spriteRender_x = 560 - sprite_w / 2;
			int spriteRender_y = 500 - sprite_h;

			float scale = getMin(new float[3] {gWindow.getScale_x(), gWindow.getScale_y(), -1});

			SDL_Event e; //event tracker
			bool quit = false;
			int frame = 0;
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
						quit = true;
					else
					{
						gWindow.handleEvent(e);

						//find the minimum of the two scales, this way I can scale things whilst maintaining asect ratio
						scale = getMin(new float[3] {gWindow.getScale_x(), gWindow.getScale_y(), -1});
					}
				}

				if (!(frame % frameT))
				{
					//Iterate the sprite forward one frame. A bit choppy, but that is not code related
					timeText.clip(sprite_w * frame / frameT + spriteBuff_x, spriteBuff_y, sprite_w, sprite_h);

					if (frame == frameTop)
						frame = 0;
				}

				//Draw an off-white backdrop semi-concealing the aspect gap
				SDL_Rect backDrop = { 0,0,gWindow.getWidth(),gWindow.getHeight() };
				SDL_SetRenderDrawColor(gRenderer, 242, 242, 240, 0xFF);
				SDL_RenderFillRect(gRenderer, &backDrop);

				//It would be better to implement scaling into the class, but I can't be bothered for this project
				handText.render(0, 0, window_w * scale, window_h * scale);
				timeText.renderClipped(spriteRender_x * scale, spriteRender_y * scale, sprite_w * scale, sprite_h * scale);

				SDL_RenderPresent(gRenderer);

				frame++;
			}

			timeText.free();
			handText.free();
		}
	}
	close();

	return 0;
}