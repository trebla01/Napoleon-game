#ifndef HELPER_H
#define HELPER_H

#include "LTexture.h"
#include "Constants.h"
#include "Cards.h"
#include "Hand.h"
#include "Player.h"
#include <vector>
using namespace std;

//determines the winner given the 5 cards, first card played, and the trump suit
Cards determineWinner(Cards c1, Cards c2, Cards c3, Cards c4, Cards c5, int firstSuitPlayed, int trumpSuit)
{
	//if there is a trump suit
	if (trumpSuit != 4)
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
			for (int i = 0; i < trumpCards.size(); i++)
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
	}

	//if no trump cards exist, find the biggest value among the cards with suit = firstSuitPlayed
	vector <Cards> firstSuitCards;
	if (c1.getSuit() == firstSuitPlayed)
	{
		firstSuitCards.push_back(c1);
	}
	if (c2.getSuit() == firstSuitPlayed)
	{
		firstSuitCards.push_back(c2);
	}
	if (c3.getSuit() == firstSuitPlayed)
	{
		firstSuitCards.push_back(c3);
	}
	if (c4.getSuit() == firstSuitPlayed)
	{
		firstSuitCards.push_back(c4);
	}
	if (c5.getSuit() == firstSuitPlayed)
	{
		firstSuitCards.push_back(c5);
	}

	//there must be at least one card that follows firstSuitPlayed
	Cards biggest = firstSuitCards.at(0);
	for (int i = 0; i < firstSuitCards.size(); i++)
	{
		//if ace in first played suit, automatically select as winner
		if (firstSuitCards.at(i).getValue() == 1)
		{
			biggest = firstSuitCards.at(i);
			return biggest;
		}
		if (firstSuitCards.at(i).getValue() > biggest.getValue())
			biggest = firstSuitCards.at(i);
	}
	return biggest;

}

void swapCards(Cards& c1, Cards& c2)
{
	int tempSuit;
	int tempValue;
	tempSuit = c1.getSuit();
	tempValue = c1.getValue();

	c1.setCard(c2.getSuit(), c2.getValue());
	c2.setCard(tempSuit, tempValue);
}

//player bids in bidding state
void playerBid (Player p, int& bid, int& previousBid, gameStateSprite& gameState, int& turnCounter, int& turn, SDL_Event* e)
{
	//User requests quit
	if (e->type == SDL_QUIT)
	{
		gameState = QUIT_GAME;
	}
	if (e->type == SDL_KEYDOWN)
	{
		//Adjust the velocity
		switch (e->key.keysym.sym)
		{
		case SDLK_RIGHT:
			if (bid < 10)
				bid++;
			break;
		case SDLK_LEFT:
			if (bid > previousBid)
				bid--;
			break;
		case SDLK_RETURN:
			if (bid > previousBid)
			{
				turnCounter = 0;
			}
			turnCounter++;
			turn++;
			previousBid = bid;
			break;
		}
	}
}

//computer bids
void cpuBid(Player p, int& bid, int& turnCounter, int& turn)
{
	float tempBid = 0;
	for (int i = 0; i < TOTAL_CARDS; i++)
	{
		switch (p.getHand()->at(i)->getValue())
		{
		case 1: tempBid = tempBid + 1; break;
		case 11: tempBid = tempBid + 0.1; break;
		case 12: tempBid = tempBid + 0.25; break;
		case 13: tempBid = tempBid + 0.5; break;
		}
	}
	int tempSuitCount[4] = { 0 };

	for (int i = 0; i < TOTAL_CARDS; i++)
	{
		switch (p.getHand()->at(i)->getSuit())
		{
		case HEARTS: tempSuitCount[0]++; break;
		case SPADES: tempSuitCount[1]++; break;
		case DIAMONDS: tempSuitCount[2]++; break;
		case CLUBS: tempSuitCount[3]++; break;
		}
	}
	//determine the most frequent suit
	int mostFrequentSuit = 0;
	for (int i = 0; i < 4; i++)
	{
		if (tempSuitCount[i] > tempSuitCount[mostFrequentSuit])
			mostFrequentSuit = i;
		if (tempSuitCount[i] == 0)
		{
			tempBid = tempBid + 0.5;
		}
		else if (tempSuitCount[i] == 1)
		{
			tempBid = tempBid + 0.25;
		}
	}
	tempBid = tempBid + 0.5 * tempSuitCount[mostFrequentSuit];
	tempBid = tempBid + 2;

	//if the calculated hand is able to beat previous bid, then bid
	if ((int)tempBid > bid)
	{
		bid = bid + 1;
		turnCounter = 0;
	}
	//otherwise, don't bid, just pass
	turnCounter++;
}


#endif