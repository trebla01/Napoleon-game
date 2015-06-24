#ifndef HAND_H
#define HAND_H

#include "LTexture.h"
#include "Constants.h"
#include "Cards.h"
#include <vector>
using namespace std;

// Playing Cards
class Hand
{
public:
	//Initializes internal variables
	Hand();

	Hand(vector<Cards> cardsDealt, bool isFacingDown);

	int getHandSize();
	int getSelectedCardIndex();
	int getCardToCardOffSet();

	void setHandSize(int s);
	void setSelectedCardIndex(int i);
	void setPositionOfFirstCard(int x, int y);

	Cards* at(int index);

	//sorted by hearts, spades, diamonds, clubs
	void sort();

	//play selected card
	void playSelected();

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	void render(SDL_Renderer* gRenderer, LTexture* cardSheetTexture, LTexture* cardBackTexture, int degrees);

private:
	int handSize;
	vector<Cards> cardsInHand;
	
	//-1 means no card selected
	int selectedCardIndex;

	//position for top left card
	SDL_Point positionOfFirstCard;
};

#endif