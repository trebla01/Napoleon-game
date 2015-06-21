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
	void deal(vector<Cards>* p1, vector<Cards>* p2, vector<Cards>* p3, vector<Cards>* p4, vector<Cards>* p5, vector<Cards>* baggage);

private:
	vector<int> deck; //1-13 hearts, next spades, diamonds, clubs
};

#endif