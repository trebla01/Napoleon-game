#include "LTexture.h"
#include "Cards.h"
#include "Hand.h"
#include "Player.h"
#include "Constants.h"


//Initializes internal variables
Player::Player()
{
	role = ENEMY;
	points = 0;
}

int Player::getRole()
{
	return role;
}
int Player::getPoints()
{
	return points;
}
Hand Player::getHand()
{
	return hand;
}

void Player::setRole(int r)
{
	role = r;
}
void Player::setPoints(int p)
{
	points = p;
}
void Player::setHand(Hand h)
{
	hand = h;
}

void takeTurn()
{

}
void playSelected()
{

}