//Used libraries in LTexture.h
#include "LTexture.h"
#include "Cards.h"
#include "Constants.h"
#include "Helper.h"
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
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

//Players
Player p1;
Player p2;
Player p3;
Player p4;
Player p5;

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

			//set p1 as human
			p1.setCPU(false);

			//set p2,p3,p4,p5 as CPUs
			p2.setCPU(true);
			p3.setCPU(true);
			p4.setCPU(true);
			p5.setCPU(true);

			//put everyone's cards in their hands
			//your hand is shown
			p1 = Player(Hand(yourCards, false));

			//opponent's hand is hidden
			p2 = Player(Hand(opp1Cards, true));
			p3 = Player(Hand(opp2Cards, true));
			p4 = Player(Hand(opp3Cards, true));
			p5 = Player(Hand(opp4Cards, true));

			p1.getHand()->sort();
			p2.getHand()->sort();
			p3.getHand()->sort();
			p4.getHand()->sort();
			p5.getHand()->sort();

			//sets the position of the cards to be displayed
			
			//bottom is p1
			p1.getHand()->setPositionOfFirstCard(SCREEN_WIDTH / 4 + cardOffSet, SCREEN_HEIGHT - CARD_HEIGHT - cardOffSet);
			
			//top left is p3
			p3.getHand()->setPositionOfFirstCard(cardOffSet, cardOffSet);
			
			//top right is p4
			p4.getHand()->setPositionOfFirstCard(SCREEN_WIDTH / 2 + boardOffSet / 2 + cardOffSet, cardOffSet);
			
			//bottom left is p2
			p2.getHand()->setPositionOfFirstCard(cardOffSet + CARD_HEIGHT, CARD_HEIGHT + 3 * cardOffSet + boardOffSet);
			
			//bottom right is p5
			p5.getHand()->setPositionOfFirstCard(SCREEN_WIDTH - cardOffSet - CARD_HEIGHT, CARD_HEIGHT + 3 * cardOffSet + boardOffSet + CARD_WIDTH + TOTAL_CARDS * cardToCardOffSet - cardToCardOffSet);
			
			//sets the position of the baggage to be displayed
			baggage[0].setPosition(SCREEN_WIDTH / 2 - CARD_WIDTH, SCREEN_HEIGHT / 2 - CARD_HEIGHT / 2);
			baggage[1].setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - CARD_HEIGHT / 2);

			int turn = 0;
			int firstSuit;

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
						//yourHand.handleEvent(&e);
						p1.getHand()->handleEvent(&e);
					}

					//play selected
					if (e.type == SDL_KEYDOWN)
					{
						//Adjust the velocity
						switch (e.key.keysym.sym)
						{
							case SDLK_SPACE:
								//yourHand.playSelected();
								p1.getHand()->playSelected();
						}
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

				//Render hands
				p1.getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
				p2.getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 90);
				p3.getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
				p4.getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
				p5.getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 270);

				//render baggage
				baggage[0].render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
				baggage[1].render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);

				//render hovered cards fully
				for (int i = 0; i < p1.getHand()->getHandSize(); i++)
				{
					if (p1.getHand()->at(i)->getCardSprite() == CARD_SPRITE_MOUSE_OVER_MOTION)
						p1.getHand()->at(i)->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
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