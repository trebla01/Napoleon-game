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

	//sets suit, value, and whether it's revealed or not
	void setCard(int s, int v, bool isFacingDown);

	//sets selected card to be true or false
	void setSelected(bool s);

	int getSuit();
	int getValue();
	bool getIsSelected();
	LCardSprite getCardSprite();

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Shows card
	void render(SDL_Renderer* gRenderer, LTexture* cardSheetTexture, LTexture* cardBackTexture, int degrees);

private:
	int suit;
	int value;

	//hidden or not
	bool facedown;
	
	//is Selected or not
	bool isSelected;

	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LCardSprite cardCurrentSprite;
};

#endif