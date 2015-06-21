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

	void setHandSize(int s);

	Cards* at(int index);

	//sorted by hearts, spades, diamonds, clubs
	void sort();

private:
	int handSize;
	vector<Cards> cardsInHand;
};

#endif