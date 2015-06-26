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
