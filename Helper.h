#ifndef HELPER_H
#define HELPER_H

#include "LTexture.h"
#include "Constants.h"
#include "Cards.h"
#include "Hand.h"
#include <vector>
using namespace std;

//determines the winner given the 5 cards, first card played, and the trump suit
Cards determineWinner(Cards c1, Cards c2, Cards c3, Cards c4, Cards c5, int firstSuitPlayed, int trumpSuit)
{
	//if a trump card exists, find the biggest value among the trump cards and return it
	vector <Cards> trumpCards;
	if (c1.getSuit() == trumpSuit)
	{
		trumpCards.push_back(c1);
	}
	if (c2.getSuit() == trumpSuit)
	{
		trumpCards.push_back(c2);
	}
	if (c3.getSuit() == trumpSuit)
	{
		trumpCards.push_back(c3);
	}
	if (c4.getSuit() == trumpSuit)
	{
		trumpCards.push_back(c4);
	}
	if (c5.getSuit() == trumpSuit)
	{
		trumpCards.push_back(c5);
	}
	if (trumpCards.size() > 0)
	{
		Cards biggest = trumpCards.at(0);
		for (int i = 1; i < trumpCards.size(); i++)
		{
			//if ace in trumps, automatically select as winner
			if (trumpCards.at(i).getValue() == 1)
			{
				biggest = trumpCards.at(i);
				return biggest;
			}
			else if (trumpCards.at(i).getValue() > biggest.getValue())
				biggest = trumpCards.at(i);
		}
		return biggest;
	}

	//if no trump cards exist, find the biggest value among the cards with suit = firstSuitPlayed
	vector <Cards> firstSuitPlayedCards;
	if (c1.getSuit() == firstSuitPlayed)
	{
		firstSuitPlayedCards.push_back(c1);
	}
	if (c2.getSuit() == firstSuitPlayed)
	{
		firstSuitPlayedCards.push_back(c2);
	}
	if (c3.getSuit() == firstSuitPlayed)
	{
		firstSuitPlayedCards.push_back(c3);
	}
	if (c4.getSuit() == firstSuitPlayed)
	{
		firstSuitPlayedCards.push_back(c4);
	}
	if (c5.getSuit() == firstSuitPlayed)
	{
		firstSuitPlayedCards.push_back(c5);
	}

	//there must be at least one card that follows firstSuitPlayed
	Cards biggest = firstSuitPlayedCards.at(0);
	for (int i = 1; i < firstSuitPlayedCards.size(); i++)
	{
		//if ace in first played suit, automatically select as winner
		if (firstSuitPlayedCards.at(i).getValue() == 1)
		{
			biggest = firstSuitPlayedCards.at(i);
			return biggest;
		}
		if (firstSuitPlayedCards.at(i).getValue() > biggest.getValue())
			biggest = firstSuitPlayedCards.at(i);
	}
	return biggest;

}

#endif