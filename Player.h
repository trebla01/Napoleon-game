#ifndef PLAYER_H
#define PLAYER_H

#include "LTexture.h"
#include "Constants.h"
#include "Cards.h"
#include "Hand.h"
#include <vector>
using namespace std;

// Player class
class Player
{
public:
	//Initializes internal variables
	Player();
	Player(Hand h);

	int getRole();
	int getPoints();
	bool isFirst();
	bool isCPU();
	Hand* getHand();

	void setRole(int r);
	void setPoints(int p);
	void setFirst(bool f);
	void setCPU(bool c);
	void setHand(Hand h);

	//first player's turn, return the suit of the played card
	int firstPlayerTurn();
	
	//all turns but the first player's
	void takeTurn( int firstPlayedSuit);



private:
	//your hand
	Hand hand;
	
	//Napoleon, secetary, or enemy
	int role;

	//points you currently have, 0-10 for tricks, 0-16 for face cards
	int points;

	//is this player going first
	bool first;

	//is computer player or human
	bool CPU;
};

#endif