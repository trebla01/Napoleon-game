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

const int NAPOLEON = 0;
const int SECRETARY = 1;
const int ENEMY = 2;

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

//suit values constants
const int HEARTS = 0;
const int SPADES = 1;
const int DIAMONDS = 2;
const int CLUBS = 3;
const int NOSUIT = 4;

//Card constants
const int CARD_WIDTH = 125; //165
const int CARD_HEIGHT = 175; //232
const int TOTAL_CARDS = 10;
const int cardOffSet = 10;
const int boardOffSet = 194;
const int cardToCardOffSet = 30;
const int playedCardOffSet = 125;
const int onClickOffSet = 20;

//Playing states
enum turnStateSprite
{
	FIRST_ROUND_FIRST_TURN_STATE = 0,
	FIRST_TURN_STATE = 1,
	FOLLOWING_TURN_STATE = 2,
};
//game state
enum gameStateSprite
{
	INTRO_SCREEN = 0,
	BIDDING_STATE = 1,
	NAPOLEON_CHOOSE_TRUMP = 2,
	NAPOLEON_DEAL_BAGGAGE = 3,
	NAPOLEON_CHOOSE_SEC = 4,
	IN_GAME = 5,
	GAME_OVER = 6,
	PLAY_AGAIN_SCREEN = 7,
	QUIT_GAME = 8,
	TUTORIAL = 9
};

#endif