#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>
#include <SDL_ttf.h>

//Texture wrapper class
class LTexture
{
public:
	//constructure, initializes variables
	LTexture();

	//Deallocates memory, calls free
	~LTexture();

	//loads image at specific path
	bool loadFromFile(std::string path, SDL_Renderer* gRenderer);

	//Creates image from font string
	bool loadFromRenderedText(SDL_Renderer* gRenderer, std::string textureText, SDL_Color textColor, SDL_Color bgColor, TTF_Font* gFont);

	//Dellocates texture
	void free();

	//Renders texture at given point
	void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};
