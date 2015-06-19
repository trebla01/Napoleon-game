#ifndef CONSTANTS_H
#define CONSTANTS_H

//Card and mouse interaction constants
enum LCardSprite
{
	CARD_SPRITE_MOUSE_OUT = 0,
	CARD_SPRITE_MOUSE_OVER_MOTION = 1,
	CARD_SPRITE_MOUSE_DOWN = 2,
	CARD_SPRITE_MOUSE_UP = 3,
	CARD_SPRITE_TOTAL = 4
};

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

//suit values constants
const int HEARTS = 0;
const int SPADES = 1;
const int DIAMONDS = 2;
const int CLUBS = 3;

//Card constants
const int CARD_WIDTH = 165; 
const int CARD_HEIGHT = 232;
const int TOTAL_CARDS = 10;
const int cardOffSet = 10;
const int boardOffSet = 4;

#endif