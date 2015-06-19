//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "LTexture.h"
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

//Card constants
const int CARD_WIDTH = 165; const int CARD_HEIGHT = 232;
const int TOTAL_CARDS = 10;
int cardOffSet = 10;
int boardOffSet = 4;

enum LCardSprite
{
	CARD_SPRITE_MOUSE_OUT = 0,
	CARD_SPRITE_MOUSE_OVER_MOTION = 1,
	CARD_SPRITE_MOUSE_DOWN = 2,
	CARD_SPRITE_MOUSE_UP = 3,
	CARD_SPRITE_TOTAL = 4
};

//suits
const int HEARTS = 0;
const int SPADES = 1;
const int DIAMONDS = 2;
const int CLUBS = 3;

//Playing Cards
class Cards
{
public:
	//Initializes internal variables
	Cards();
	Cards(int s, int v);

	//Sets top left position
	void setPosition(int x, int y);

	void setCard(int s, int v, bool isFacingDown);

	int getSuit();
	int getValue();

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Shows card
	void render(int degrees);

private:
	int suit;
	int value;
	
	bool facedown;

	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LCardSprite cardCurrentSprite;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer, everything is rendered here
SDL_Renderer* gRenderer = NULL;

LTexture cardSheetTexture;
LTexture cardBackTexture;

//Card objects
Cards yourCards[TOTAL_CARDS];
Cards opp1Cards[TOTAL_CARDS];
Cards opp2Cards[TOTAL_CARDS];
Cards opp3Cards[TOTAL_CARDS];
Cards opp4Cards[TOTAL_CARDS];

Cards::Cards()
{
	suit = CLUBS;
	value = 1;
	facedown = true;
	mPosition.x = 0;
	mPosition.y = 0;
	cardCurrentSprite = CARD_SPRITE_MOUSE_OUT;
}

Cards::Cards(int s, int v)
{
	suit = s;
	value = v;
	facedown = false;
	mPosition.x = 0;
	mPosition.y = 0;

	cardCurrentSprite = CARD_SPRITE_MOUSE_OUT;
}

void Cards::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void Cards::setCard(int s, int v, bool isFacingDown)
{
	suit = s;
	value = v;
	facedown = isFacingDown;
}
int Cards::getSuit()
{
	return suit;
}
int Cards::getValue()
{
	return value;
}

void Cards::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in card
		bool inside = true;

		//Mouse is left of the card
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the card
		else if (x > mPosition.x + CARD_WIDTH)
		{
			inside = false;
		}
		//Mouse above the card
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the card
		else if (y > mPosition.y + CARD_HEIGHT)
		{
			inside = false;
		}

		//Mouse is outside card
		if (!inside)
		{
			cardCurrentSprite = CARD_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside card
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				cardCurrentSprite = CARD_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				cardCurrentSprite = CARD_SPRITE_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				cardCurrentSprite = CARD_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}

void Cards::render(int degrees) //render and rotate degrees clockwise
{
	SDL_Point rotationPoint = { 0, 0 };
	if (facedown == true)
	{
		cardBackTexture.render(gRenderer, mPosition.x, mPosition.y, NULL, degrees, &rotationPoint);
	}
	else
	{
		SDL_Rect getCardRect = { CARD_WIDTH*(value - 1), CARD_HEIGHT*(suit), CARD_WIDTH, CARD_HEIGHT };
		//Show current card sprite
		cardSheetTexture.render(gRenderer, mPosition.x, mPosition.y, &getCardRect, degrees, &rotationPoint);


		if (cardCurrentSprite == CARD_SPRITE_MOUSE_OVER_MOTION)
		{
			SDL_Rect highlightRect = { mPosition.x, mPosition.y, CARD_WIDTH, CARD_HEIGHT };
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0, 0xFF);
			SDL_RenderDrawRect(gRenderer, &highlightRect);
		}
		if (cardCurrentSprite == CARD_SPRITE_MOUSE_DOWN)
		{
			SDL_Rect highlightRect = { mPosition.x, mPosition.y, CARD_WIDTH, CARD_HEIGHT };
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0, 0xFF);
			SDL_RenderDrawRect(gRenderer, &highlightRect);
		}
	}
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Napoleon game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if (!cardSheetTexture.loadFromFile("Napoleon game/poker.cards.png", gRenderer) ||
		!cardBackTexture.loadFromFile("Napoleon game/card_back2.jpg", gRenderer))
	{
		printf("Failed to load sprite texture!\n");
		success = false;
	}


	return success;
}

void close()
{
	//Free loaded images
	cardSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//generate playing space
			SDL_Rect yourField = { SCREEN_WIDTH / 4 + boardOffSet/4, SCREEN_HEIGHT - CARD_HEIGHT - 2*cardOffSet, SCREEN_WIDTH / 2 - boardOffSet/4, CARD_HEIGHT + 2*cardOffSet };
			SDL_Rect opp1Field = { 0, 0, SCREEN_WIDTH / 2 - boardOffSet/2, CARD_HEIGHT + 2 * cardOffSet };
			SDL_Rect opp2Field = { SCREEN_WIDTH / 2 + boardOffSet/2, 0, SCREEN_WIDTH / 2, CARD_HEIGHT + 2 * cardOffSet };
			SDL_Rect opp3Field = { 0, CARD_HEIGHT + 2 * cardOffSet + boardOffSet, CARD_HEIGHT + 2 * cardOffSet, SCREEN_WIDTH / 2 - boardOffSet / 2};
			SDL_Rect opp4Field = { SCREEN_WIDTH - (CARD_HEIGHT + 2 * cardOffSet), CARD_HEIGHT + 2 * cardOffSet + boardOffSet, CARD_HEIGHT + 2 * cardOffSet, SCREEN_WIDTH / 2 - boardOffSet / 2 };

			//generate everyone's cards 
			for (int i = 0; i < TOTAL_CARDS; i++)
			{
				//your cards are revealed
				yourCards[i].setCard(SPADES, i+1, false); 
				
				//opponent's cards are hidden
				opp1Cards[i].setCard(CLUBS, i+2, true);
				opp2Cards[i].setCard(DIAMONDS, i+3, true);
				opp3Cards[i].setCard(HEARTS, i+4, true);
				opp4Cards[i].setCard(SPADES, i+4, true);
				
			}

			////////////////////// NEEDS WORK CENTERING //////////////////

			//Set card location
			int cardToCardOffSet = (yourField.w - CARD_WIDTH * (TOTAL_CARDS - 1)/TOTAL_CARDS - 2*cardOffSet) / TOTAL_CARDS;
			for (int i = 0; i < TOTAL_CARDS; i++)
			{
				yourCards[i].setPosition(SCREEN_WIDTH / 4 + cardOffSet + i*cardToCardOffSet, SCREEN_HEIGHT - CARD_HEIGHT - cardOffSet);
				opp1Cards[i].setPosition(0 + cardOffSet + i*cardToCardOffSet, cardOffSet);
				opp2Cards[i].setPosition(SCREEN_WIDTH / 2 + boardOffSet / 2 + cardOffSet + i*cardToCardOffSet, cardOffSet);

				//placed vertically, cards rotated about topleft
				opp3Cards[i].setPosition(cardOffSet + CARD_HEIGHT, CARD_HEIGHT + 3 * cardOffSet + boardOffSet + i*cardToCardOffSet);
				opp4Cards[i].setPosition(SCREEN_WIDTH - cardOffSet - CARD_HEIGHT, CARD_HEIGHT + 3 * cardOffSet + boardOffSet + CARD_WIDTH + TOTAL_CARDS*cardToCardOffSet - (i+1) *cardToCardOffSet);

			}
			//////////////////////////////////////////////////////////////

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Handle card events
					for (int i = 0; i < TOTAL_CARDS; ++i)
					{
						yourCards[i].handleEvent(&e);
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render boards
				SDL_SetRenderDrawColor(gRenderer, 0, 0xFF, 0, 0xFF);
				SDL_RenderFillRect(gRenderer, &yourField);
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0, 0xFF);
				SDL_RenderFillRect(gRenderer, &opp1Field);
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0xFF, 0xFF);
				SDL_RenderFillRect(gRenderer, &opp2Field);
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0xFF, 0xFF);
				SDL_RenderFillRect(gRenderer, &opp3Field);
				SDL_SetRenderDrawColor(gRenderer, 0, 0xFF, 0xFF, 0xFF);
				SDL_RenderFillRect(gRenderer, &opp4Field);

				//Render cards
				for (int i = 0; i < TOTAL_CARDS; ++i)
				{
					yourCards[i].render(0);
					opp1Cards[i].render(0);
					opp2Cards[i].render(0);
					opp3Cards[i].render(90);
					opp4Cards[i].render(270);
				}

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}