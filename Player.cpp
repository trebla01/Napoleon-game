#include "LTexture.h"
#include "Cards.h"
#include "Hand.h"
#include "Player.h"
#include "Constants.h"
#include <time.h>


//Initializes internal variables
Player::Player()
{
	role = ENEMY;
	points = 0;
	first = false;
	CPU = false;
}

Player::Player(Hand h)
{
	role = ENEMY;
	points = 0;
	first = false;
	CPU = false;
	hand = h;
}

int Player::getRole()
{
	return role;
}
int Player::getPoints()
{
	return points;
}
bool Player::isFirst()
{
	return first;
}
bool Player::isCPU()
{
	return CPU;
}
Hand* Player::getHand()
{
	return &hand;
}

void Player::setRole(int r)
{
	role = r;
}
void Player::setPoints(int p)
{
	points = p;
}
void Player::setFirst(bool f)
{
	first = f;
}
void Player::setCPU(bool c)
{
	CPU = c;
}
void Player::setHand(Hand h)
{
	hand = h;
}

//REWRITE TAKE TURN

int Player::firstPlayerTurn()
{
	//set all the viable plays 
	hand.setAllViable();

	//if it's a CPU, select randomly
	if (CPU == true)
	{
		srand(time(NULL));
		hand.setSelectedCardIndex(rand() % hand.getHandSize());
		int suit = hand.at(hand.getSelectedCardIndex())->getSuit();
		hand.playSelected();
		return suit;
	}

	//else, let the user pick
	else
	{
		//Event handler
		SDL_Event e;
		bool finished = false;

		while (!finished)
		{
			//Handle card events
			for (int i = 0; i < TOTAL_CARDS; ++i)
			{
				hand.handleEvent(&e);
			}

			//play selected
			if (e.type == SDL_KEYDOWN)
			{
				//Adjust the velocityfalse
				switch (e.key.keysym.sym)
				{
				case SDLK_SPACE:
					if (hand.playSelected() == true)
					{
						finished = true;
					}
				}
			}
		}
	}
}

void Player::takeTurn(int firstPlayedSuit)
{
	//set the viable plays
	hand.findViablePlay(firstPlayedSuit);

	//if it's a cpu, randomly select randomly
	if (CPU == true)
	{
		srand(time(NULL));
		hand.setSelectedCardIndex(rand() % hand.getHandSize());
		hand.playSelected();
	}
	//else, let the player choose
	else
	{
		//Event handler
		SDL_Event e;
		bool finished = false;

		while (!finished)
		{
			//Handle card events
			for (int i = 0; i < TOTAL_CARDS; ++i)
			{
				hand.handleEvent(&e);
			}

			//play selected
			if (e.type == SDL_KEYDOWN)
			{
				//Adjust the velocityfalse
				switch (e.key.keysym.sym)
				{
				case SDLK_SPACE:
					if (hand.playSelected() == true)
					{
						finished = true;
					}
				}
			}
		}
	}
}