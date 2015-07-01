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

//deal the cards to the hands, deck will be empty after this
void Deck::deal(vector<Cards>* p1, vector<Cards>* p2, vector<Cards>* p3, vector<Cards>* p4, vector<Cards>* p5, vector<Cards>* baggage)
{
	//deal to the player hands
	for (int i = 0; i < TOTAL_CARDS; i++)
	{
		Cards tempCard = Cards(deck.at(0) / 13, deck.at(0) % 13 + 1, true);
		p1->push_back(tempCard);
		deck.erase(deck.begin());
		tempCard = Cards(deck.at(0) / 13, deck.at(0) % 13 + 1, true);
		p2->push_back(tempCard);
		deck.erase(deck.begin()); 
		tempCard = Cards(deck.at(0) / 13, deck.at(0) % 13 + 1, true);
		p3->push_back(tempCard);;
		deck.erase(deck.begin());
		tempCard = Cards(deck.at(0) / 13, deck.at(0) % 13 + 1, true);
		p4->push_back(tempCard);;
		deck.erase(deck.begin());
		tempCard = Cards(deck.at(0) / 13, deck.at(0) % 13 + 1, true);
		p5->push_back(tempCard);;
		deck.erase(deck.begin());
	}

	//give the last two to baggage
	Cards tempCard = Cards(deck.at(0) / 13, deck.at(0) % 13 + 1, true);
	baggage->push_back(tempCard);
	deck.erase(deck.begin());
	tempCard = Cards(deck.at(0) / 13, deck.at(0) % 13 + 1, true);
	baggage->push_back(tempCard);
}
