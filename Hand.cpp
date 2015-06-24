#include "LTexture.h"
#include "Cards.h"
#include "Hand.h"
#include "Constants.h"


//Initializes internal variables
Hand::Hand()
{
	handSize = 0;
	selectedCardIndex = -1;
}

Hand::Hand(vector<Cards> cardsDealt, bool isFacingDown)
{
	handSize = cardsDealt.size();
	selectedCardIndex = -1;
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
int Hand::getSelectedCardIndex()
{
	return selectedCardIndex;
}
int Hand::getCardToCardOffSet()
{
	return cardToCardOffSet;
}

void Hand::setHandSize(int s)
{
	handSize = s;
}
void Hand::setSelectedCardIndex(int i)
{
	selectedCardIndex = i;
}
void Hand::setPositionOfFirstCard(int x, int y)
{
	positionOfFirstCard.x = x;
	positionOfFirstCard.y = y;
}

Cards* Hand::at(int index)
{
	return &cardsInHand.at(index);
}

//helper function for sort
bool isRightCardLessThanLeftCard(Cards left, Cards right)
{
	if (right.getSuit() < left.getSuit())
		return true;
	else if (right.getSuit() > left.getSuit())
		return false;
	else
	{
		//if the left is an ace, then left is always greater than right
		if (left.getValue() == 1)
			return true;
		else if (right.getValue() == 1)
			return false;
		else if (right.getValue() < left.getValue())
			return true;
		else
			return false;
	}
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
	
	for (int i = 0; i < handSize - 1; i++)
	{
		cardsInHand.at(i).setLast(false);
	}
	cardsInHand.at(handSize - 1).setLast(true);
}

//play selected card
void Hand::playSelected()
{
	if (selectedCardIndex != -1)
	{
		cardsInHand.erase(cardsInHand.begin() + selectedCardIndex);
		selectedCardIndex = -1;
		handSize--;
		//if hand size doesn't equal = 0, set last card as the last card in Hand
		handSize == 0 ? handSize = 0: cardsInHand.at(handSize - 1).setLast(true);
	}
}

void Hand::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		for (int i = 0; i < handSize; i++)
		{
			cardsInHand.at(i).handleEvent(e, cardToCardOffSet);
			if (cardsInHand.at(i).getIsSelected() == true)
			{
				selectedCardIndex = i;
				for (int j = 0; j < handSize; j++)
				{
					if (j != selectedCardIndex)
						cardsInHand.at(j).setSelected(false);
				}
			}
		}
	}
}

void Hand::render(SDL_Renderer* gRenderer, LTexture* cardSheetTexture, LTexture* cardBackTexture, int degrees)
{
	
	//first card at the top, last card at the bottom
	if (degrees == 90)
	{
		//set position of cards rotated 90 degrees
		for (int i = 0; i < handSize; i++)
		{
			cardsInHand.at(i).setPosition(positionOfFirstCard.x, positionOfFirstCard.y + i*cardToCardOffSet);
		}
	}

	//if the cards are rotated 270 degrees, first card at the bottom, last card at the top
	else if (degrees == 270)
	{
		//set position of cards rotated 270 degrees
		for (int i = 0; i < handSize; i++)
		{
			cardsInHand.at(i).setPosition(positionOfFirstCard.x, positionOfFirstCard.y - i*cardToCardOffSet);
		}
	}
	//otherwise, set position of cards normally
	else
	{
		for (int i = 0; i < handSize; i++)
		{
			cardsInHand.at(i).setPosition(positionOfFirstCard.x + i*cardToCardOffSet, positionOfFirstCard.y);
		}
	}
	//render the cards
	for (int i = 0; i < handSize; ++i)
	{
		cardsInHand.at(i).render(gRenderer, cardSheetTexture, cardBackTexture, degrees);
	}
}