#ifndef DECK_H
#define DECK_H

#include "Cards.h"
#include <vector>
using namespace std;

// Playing Cards
class Deck
{
public:
	//Initializes internal variables
	Deck();

	//Shuffle the deck
	void shuffle();

	//deal the cards to the hands
	vector<int> deal();

private:
	vector<int> deck; //1-13 hearts, next spades, diamonds, clubs
};

#endif