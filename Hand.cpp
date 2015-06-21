#include "LTexture.h"
#include "Cards.h"
#include "Hand.h"
#include "Constants.h"


//Initializes internal variables
Hand::Hand()
{
	handSize = 0;
}

Hand::Hand(vector<Cards> cardsDealt, bool isFacingDown)
{
	handSize = cardsDealt.size();
	for (int i = 0; i < cardsDealt.size(); i++)
	{
		Cards tempCard = Cards(cardsDealt.at(i).getSuit(), cardsDealt.at(i).getValue(), isFacingDown);
		cardsInHand.push_back(tempCard);
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

Cards* Hand::at(int index)
{
	return &cardsInHand.at(index);
}

//helper function for sort
bool isRightCardLessThanLeftCard(Cards left, Cards right)
{
	int leftv = left.getSuit() * 13 + left.getValue();
	int rightv = right.getSuit() * 13 + right.getValue();

	if (rightv < leftv)
		return true;
	else
		return false;
}

//sorted by hearts, spades, diamonds, clubs
void Hand::sort()
{
	int j;
	Cards temp;
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
