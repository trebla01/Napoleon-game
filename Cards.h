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
	Cards(int s, int v, bool isFacingDown);

	//Sets top left position
	void setPosition(int x, int y);

	//set suit and value
	void setCard(int s, int v);

	//sets suit, value, and whether it's revealed or not
	void setCard(int s, int v, bool isFacingDown);

	//sets selected card to be true or false
	void setSelected(bool s);

	//sets card as last card
	void setLast(bool l);

	//sets the card as viable play or not
	void setViablePlay(bool v);

	int getSuit();
	int getValue();
	bool getIsSelected();
	bool isLast();
	bool isViablePlay();
	LCardSprite getCardSprite();

	//Handles mouse event
	void handleEvent(SDL_Event* e, int cardToCardOffSet);

	//Shows card
	void render(SDL_Renderer* gRenderer, LTexture* cardSheetTexture, LTexture* cardBackTexture, int degrees);

	//prints card to output console
	void print();

private:
	//0-3, hearts, spades diamonds, clubs
	int suit;

	//1-13, ace = 1, king = 13
	int value;

	//hidden or not
	bool facedown;

	//last card or not
	bool last;
	
	//is Viable play or not given the first played suit
	bool viablePlay;

	//is Selected or not
	bool isSelected;

	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LCardSprite cardCurrentSprite;
};

#endif