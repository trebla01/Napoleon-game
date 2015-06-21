#include "LTexture.h"
#include "Cards.h"
#include "Hand.h"
#include "Constants.h"
#include <utility>


//Initializes internal variables
Hand::Hand()
{
	handSize = 0;
}

Hand::Hand(vector<int> cardsDealt, bool isFacingDown)
{
	handSize = cardsDealt.size;
	for (int i = 0; i < cardsDealt.size; i++)
	{
		cardsInHand.push_back(Cards(cardsDealt.at(0) / 13, cardsDealt.at(0) % 13, isFacingDown));
	}
}

int Hand::getHandSize()
{
	return handSize;
}

void Hand::setHandSize(int s)
{
	handSize = s;
}

//sorted by hearts, spades, diamonds, clubs
void Hand::sort()
{
	int j;
	for (int i = 1; i < handSize; i++)
	{
		j = i;

		while (j>0 && isRightCardLessThanLeftCard(cardsInHand.at(j - 1), cardsInHand.at(j)))
		{
			swap(cardsInHand.at(j - 1), cardsInHand.at(j));
			j--;
		}
	}
}

bool isRightCardLessThanLeftCard(Cards left, Cards right)
{
	int leftv = left.getSuit() * 13 + left.getValue();
	int rightv = right.getSuit() * 13 + right.getValue();

	if (rightv < leftv)
		return true;
	else
		return false;
}