//Used libraries in LTexture.h
#include "LTexture.h"
#include "Cards.h"
#include "Constants.h"
#include "Deck.h"
#include "Hand.h"
#include <vector>
using namespace std;

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

//pictures of the cards and the card back
LTexture cardSheetTexture;
LTexture cardBackTexture;

//cards dealt to everyone
vector<Cards> yourCards;
vector<Cards> opp1Cards;
vector<Cards> opp2Cards;
vector<Cards> opp3Cards;
vector<Cards> opp4Cards;
vector<Cards> baggage;

//everyone's hands
Hand yourHand;
Hand opp1Hand;
Hand opp2Hand;
Hand opp3Hand;
Hand opp4Hand;

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
		!cardBackTexture.loadFromFile("Napoleon game/card_back.png", gRenderer))
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

			Deck d;
			d.shuffle();
			d.deal(&yourCards, &opp1Cards, &opp2Cards, &opp3Cards, &opp4Cards, &baggage);

			//put everyone's cards in their hands
			//your hand is shown
			yourHand = Hand(yourCards, false);

			//opponent's hand is hidden
			opp1Hand = Hand(opp1Cards, true);
			opp2Hand = Hand(opp2Cards, true);
			opp3Hand = Hand(opp3Cards, true);
			opp4Hand = Hand(opp4Cards, true);

			
			//sort everyone's hands
			yourHand.sort();
			opp1Hand.sort();
			opp2Hand.sort();
			opp3Hand.sort();
			opp4Hand.sort();

			////////////////////// NEEDS WORK CENTERING //////////////////

			//Set card location
			for (int i = 0; i < TOTAL_CARDS; i++)
			{
				yourHand.at(i)->setPosition(SCREEN_WIDTH / 4 + cardOffSet + i*yourHand.getCardToCardOffSet(), SCREEN_HEIGHT - CARD_HEIGHT - cardOffSet);
				opp1Hand.at(i)->setPosition(0 + cardOffSet + i*yourHand.getCardToCardOffSet(), cardOffSet);
				opp2Hand.at(i)->setPosition(SCREEN_WIDTH / 2 + boardOffSet / 2 + cardOffSet + i*yourHand.getCardToCardOffSet(), cardOffSet);

				//placed vertically, cards rotated about topleft
				opp3Hand.at(i)->setPosition(cardOffSet + CARD_HEIGHT, CARD_HEIGHT + 3 * cardOffSet + boardOffSet + i*yourHand.getCardToCardOffSet());
				opp4Hand.at(i)->setPosition(SCREEN_WIDTH - cardOffSet - CARD_HEIGHT, CARD_HEIGHT + 3 * cardOffSet + boardOffSet + CARD_WIDTH + TOTAL_CARDS * yourHand.getCardToCardOffSet() - (i + 1) * yourHand.getCardToCardOffSet());
			}

			baggage[0].setPosition(SCREEN_WIDTH / 2 - CARD_WIDTH, SCREEN_HEIGHT / 2 - CARD_HEIGHT / 2);
			baggage[1].setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - CARD_HEIGHT / 2);
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
						yourHand.handleEvent(&e);
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
					yourHand.at(i)->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					opp1Hand.at(i)->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					opp2Hand.at(i)->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					opp3Hand.at(i)->render(gRenderer, &cardSheetTexture, &cardBackTexture, 90);
					opp4Hand.at(i)->render(gRenderer, &cardSheetTexture, &cardBackTexture, 270);
				}
				baggage[0].render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
				baggage[1].render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);

				//render hovered cards fully
				for (int i = 0; i < TOTAL_CARDS; i++)
				{
					if (yourHand.at(i)->getCardSprite() == CARD_SPRITE_MOUSE_OVER_MOTION)
						yourHand.at(i)->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
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