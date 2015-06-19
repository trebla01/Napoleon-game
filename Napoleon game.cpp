//Used libraries in LTexture.h
#include "LTexture.h"
#include "Cards.h"
#include "Constants.h"
#include "Deck.h"
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

//Card objects
Cards yourCards[TOTAL_CARDS];
Cards opp1Cards[TOTAL_CARDS];
Cards opp2Cards[TOTAL_CARDS];
Cards opp3Cards[TOTAL_CARDS];
Cards opp4Cards[TOTAL_CARDS];


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

			Deck d;
			d.shuffle();
			vector<int> deckShuffled = d.deal();

			//generate everyone's cards 
			
			for (int i = 0; i < TOTAL_CARDS; i++)
			{
				//your cards are revealed
				yourCards[i].setCard(deckShuffled.at(0) / 13, deckShuffled.at(0) % 13 + 1, false);
				deckShuffled.erase(deckShuffled.begin());
			}
			for (int i = 0; i < TOTAL_CARDS; i++)
			{
				//your cards are revealed
				opp1Cards[i].setCard(deckShuffled.at(0) / 13, deckShuffled.at(0) % 13 + 1, false);
				deckShuffled.erase(deckShuffled.begin());
			}
			for (int i = 0; i < TOTAL_CARDS; i++)
			{
				//your cards are revealed
				opp2Cards[i].setCard(deckShuffled.at(0) / 13, deckShuffled.at(0) % 13 + 1, false);
				deckShuffled.erase(deckShuffled.begin());
			}
			for (int i = 0; i < TOTAL_CARDS; i++)
			{
				//your cards are revealed
				opp3Cards[i].setCard(deckShuffled.at(0) / 13, deckShuffled.at(0) % 13 + 1, false);
				deckShuffled.erase(deckShuffled.begin());
			}
			for (int i = 0; i < TOTAL_CARDS; i++)
			{
				//your cards are revealed
				opp4Cards[i].setCard(deckShuffled.at(0) / 13, deckShuffled.at(0) % 13 + 1, false);
				deckShuffled.erase(deckShuffled.begin());
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
					yourCards[i].render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					opp1Cards[i].render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					opp2Cards[i].render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					opp3Cards[i].render(gRenderer, &cardSheetTexture, &cardBackTexture, 90);
					opp4Cards[i].render(gRenderer, &cardSheetTexture, &cardBackTexture, 270);
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