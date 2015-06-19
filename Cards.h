#ifndef CARDS_H
#define CARDS_H

#include "LTexture.h"
#include "Constants.h"

// Playing Cards
class Cards
{
public:
	//Initializes internal variables
	Cards();
	Cards(int s, int v);

	//Sets top left position
	void setPosition(int x, int y);

	void setCard(int s, int v, bool isFacingDown);

	int getSuit();
	int getValue();

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Shows card
	void render(SDL_Renderer* gRenderer, LTexture* cardSheetTexture, LTexture* cardBackTexture, int degrees);

private:
	int suit;
	int value;

	bool facedown;

	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LCardSprite cardCurrentSprite;
};

#endif