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

	int getRole();
	int getPoints();
	Hand getHand();

	void setRole(int r);
	void setPoints(int p);
	void setHand(Hand h);

	void takeTurn();
	void playSelected();



private:
	//your hand
	Hand hand;
	
	//Napoleon, secetary, or enemy
	int role;

	//points you currently have, 0-10 for tricks, 0-16 for face cards
	int points;
};

#endif