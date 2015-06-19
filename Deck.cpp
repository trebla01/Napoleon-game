#include "LTexture.h"
#include "Cards.h"
#include "Deck.h"
#include "Constants.h"
#include <time.h>


//Initializes internal variables
Deck::Deck()
{
	for (int i = 0; i < 52; i++)
		deck.push_back(i);
}

//Shuffle the deck
void Deck::shuffle()
{
	//initialize rand
	srand(time(NULL));

	//create a temporary vector array
	vector<int> temp;

	//create a temporary index
	int index = 0;

	while (deck.size() != 0)
	{
		//pick a random card and store it in temp
		index = rand() % deck.size();
		temp.push_back(deck.at(index));
		
		//erase that value from the deck
		deck.erase(deck.begin() + index);
	}
	while (temp.size() != 0)
	{
		//add newly arrange temp into deck
		deck.push_back(temp.at(0));
		temp.erase(temp.begin());
	}
}

//deal the cards to the hands
vector<int> Deck::deal()
{
	vector<int> temp;
	//create a copy temp vector array to deal out the cards
	for (int i = 0; i < 52; i++)
	{
		//add newly arrange temp into deck
		temp.push_back(deck.at(i));
	}
	return temp;
}
