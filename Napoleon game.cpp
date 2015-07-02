//Used libraries in LTexture.h
#include "LTexture.h"
#include "Cards.h"
#include "Constants.h"
#include "Helper.h"
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
#include <vector>
#include "time.h"
#include <sstream>

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
LTexture gTextTexture;

//Globally used font
TTF_Font *gFont = NULL;

//cards dealt to everyone
vector<Cards> yourCards;
vector<Cards> opp1Cards;
vector<Cards> opp2Cards;
vector<Cards> opp3Cards;
vector<Cards> opp4Cards;
vector<Cards> baggage;

//Players
vector<Player> player;

Player p0;
Player p1;
Player p2;
Player p3;
Player p4;

//the cards each player plays each turn;
vector<Cards> c(5);

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
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
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
	
	//Open the font
	gFont = TTF_OpenFont("Napoleon game/OpenSans-Semibold.ttf", 24);
	if (gFont == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	//Load sprites
	if (!cardSheetTexture.loadFromFile("Napoleon game/poker.cards.smaller.png", gRenderer) ||
		!cardBackTexture.loadFromFile("Napoleon game/card_back_flowers_smaller.png", gRenderer))
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
			//Event handler
			SDL_Event e;

			//game state flag
			gameStateSprite gameState = BIDDING_STATE;

			//game variables
			int bid = 4; 
			int previousBid = 4;
			int index; //hold temporary index for bots to randomly choose card
			int turn = 0; //whose turn it is
			int turnCounter = 0; //when counter == 5, means everyone has played
			int firstSuit; //first played suit, following cards must match if possbile
			int trumpSuit = NOSUIT; //trump suit for the game
			turnStateSprite turnState = FIRST_ROUND_FIRST_TURN_STATE; //initially in the first turnstate
			int round = 0; //what round it is
			bool wait = false; //waiting variable
			SDL_Color textColor = { 0, 0, 0 }; //text color

			//the secretary card
			Cards secCard = Cards(0, 1, false);
			secCard.setPosition(SCREEN_WIDTH / 2 - CARD_WIDTH / 2, SCREEN_HEIGHT / 2 - CARD_HEIGHT / 2);

			//text stream
			stringstream text;

			//generate playing space
			SDL_Rect yourField = { SCREEN_WIDTH / 4 + boardOffSet/4, SCREEN_HEIGHT - CARD_HEIGHT - 2*cardOffSet, SCREEN_WIDTH / 2 - boardOffSet/2, CARD_HEIGHT + 2*cardOffSet };
			SDL_Rect opp2Field = { boardOffSet / 4, 0, SCREEN_WIDTH / 2 - boardOffSet/2, CARD_HEIGHT + 2 * cardOffSet };
			SDL_Rect opp3Field = { SCREEN_WIDTH / 2 + boardOffSet / 4, 0, SCREEN_WIDTH / 2 - boardOffSet / 2, CARD_HEIGHT + 2 * cardOffSet };
			SDL_Rect opp1Field = { 0, CARD_HEIGHT + 2 * cardOffSet + boardOffSet / 4, CARD_HEIGHT + 2 * cardOffSet, SCREEN_WIDTH / 2 - boardOffSet / 2};
			SDL_Rect opp4Field = { SCREEN_WIDTH - (CARD_HEIGHT + 2 * cardOffSet), CARD_HEIGHT + 2 * cardOffSet + boardOffSet / 4, CARD_HEIGHT + 2 * cardOffSet, SCREEN_WIDTH / 2 - boardOffSet / 2 };

			Deck d;
			d.shuffle();
			d.deal(&yourCards, &opp1Cards, &opp2Cards, &opp3Cards, &opp4Cards, &baggage);

			//put everyone's cards in their hands
			//your hand is face up
			p0 = Player(Hand(yourCards, false));

			//opponent's hand is face down
			p1 = Player(Hand(opp1Cards, false));
			p2 = Player(Hand(opp2Cards, false));
			p3 = Player(Hand(opp3Cards, false));
			p4 = Player(Hand(opp4Cards, false));

			Player playerBaggage = Player(Hand(baggage, false));

			//add the players into a player vector array
			player.push_back(p0);
			player.push_back(p1);
			player.push_back(p2);
			player.push_back(p3);
			player.push_back(p4);

			player.at(0).getHand()->sort();
			player.at(1).getHand()->sort();
			player.at(2).getHand()->sort();
			player.at(3).getHand()->sort();
			player.at(4).getHand()->sort();

			//set p0 as human
			player.at(0).setCPU(false);

			//set p1,p2,p3,p4 as CPUs
			player.at(1).setCPU(true);
			player.at(2).setCPU(true);
			player.at(3).setCPU(true);
			player.at(4).setCPU(true);

			player.at(0).setRole(ENEMY);
			player.at(1).setRole(ENEMY);
			player.at(2).setRole(ENEMY);
			player.at(3).setRole(ENEMY);
			player.at(4).setRole(ENEMY);

			//sets the position of the cards to be displayed
			//bottom is p0
			player.at(0).getHand()->setPositionOfFirstCard(yourField.x + cardOffSet, SCREEN_HEIGHT - CARD_HEIGHT - cardOffSet);
			
			//top left is p2
			player.at(2).getHand()->setPositionOfFirstCard(opp2Field.x + cardOffSet, cardOffSet);
			
			//top right is p3
			player.at(3).getHand()->setPositionOfFirstCard(opp3Field.x + cardOffSet, cardOffSet);
			
			//bottom left is p1
			player.at(1).getHand()->setPositionOfFirstCard(cardOffSet + CARD_HEIGHT, opp1Field.y + cardOffSet);
			
			//bottom right is p4
			player.at(4).getHand()->setPositionOfFirstCard(SCREEN_WIDTH - cardOffSet - CARD_HEIGHT, opp4Field.y + opp4Field.h - cardOffSet);
			
			//sets the position of the baggage to be displayed
			playerBaggage.getHand()->setPositionOfFirstCard(SCREEN_WIDTH / 2 - CARD_WIDTH / 2 - cardToCardOffSet / 2, SCREEN_HEIGHT / 2 - CARD_HEIGHT / 2);
			playerBaggage.getHand()->at(1)->setLast(true); // set the last card as last
			
			//sets the position of the played cards
			c.at(0).setPosition(yourField.x + yourField.w / 2 - CARD_WIDTH / 2 , SCREEN_HEIGHT - CARD_HEIGHT - playedCardOffSet);
			c.at(2).setPosition(opp2Field.x + opp2Field.w - CARD_WIDTH - cardOffSet, playedCardOffSet);
			c.at(3).setPosition(opp3Field.x + cardOffSet, playedCardOffSet);
			c.at(1).setPosition(opp1Field.w + CARD_HEIGHT + cardOffSet, opp1Field.y + opp1Field.h / 2 - CARD_WIDTH / 2);
			c.at(4).setPosition(opp4Field.x - CARD_HEIGHT - cardOffSet, opp4Field. y + opp4Field.h / 2 + CARD_WIDTH / 2);
			//set them all to be hidden initially
			for (int i = 0; i < c.size(); i++)
			{
				c.at(i).setFacedown(false);
				c.at(i).setHidden(true);
			}

			//While application is running
			while ((gameState != QUIT_GAME))
			{
				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render boards
				SDL_SetRenderDrawColor(gRenderer, 0, 0xFF, 0, 0xFF);
				SDL_RenderFillRect(gRenderer, &yourField);
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0, 0xFF);
				SDL_RenderFillRect(gRenderer, &opp2Field);
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0xFF, 0xFF);
				SDL_RenderFillRect(gRenderer, &opp3Field);
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0xFF, 0xFF);
				SDL_RenderFillRect(gRenderer, &opp1Field);
				SDL_SetRenderDrawColor(gRenderer, 0, 0xFF, 0xFF, 0xFF);
				SDL_RenderFillRect(gRenderer, &opp4Field);

				if (gameState == BIDDING_STATE)
				{
					//Render hands
					player.at(0).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(1).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 90);
					player.at(2).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(3).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(4).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 270);

					text.str("");
					if (previousBid == 4)
						text << "Curent Highest Bid: NO BID";
					else
						text << "Curent Highest Bid: " << previousBid;
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - 3 * gTextTexture.getHeight() / 2);

					if (player.at(turn).isCPU() == false)
					{
						if (SDL_PollEvent(&e) != 0)
							playerBid(player.at(turn), bid, previousBid, gameState, turnCounter, turn, &e);

						text.str("");
						if (previousBid == bid)
							text << "Your Bid: Pass";
						else
							text << "Your Bid: " << bid;
						gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
						gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTextTexture.getHeight()) / 2);
						//Update screen
						SDL_RenderPresent(gRenderer);
					}
					//else, let the AI bid
					else
					{
						cpuBid(player.at(turn), bid, turnCounter, turn);

						text.str("");
						if (previousBid == 4)
							text << "Curent Highest Bid: NO BID";
						else
							text << "Curent Highest Bid: " << previousBid;
						gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
						gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - 3 * gTextTexture.getHeight() / 2);

						text.str("");
						if (previousBid == bid)
							text << "Opponent " << turn << " Bid: Pass";
						else
							text << "Opponent " << turn << " Bids " << bid;
						gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
						gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTextTexture.getHeight()) / 2);
						//Update screen
						SDL_RenderPresent(gRenderer);
						SDL_Delay(1000);
						previousBid = bid;
						turn++;
						if (turn == 5)
							turn = 0;
					}

					if (turnCounter == 5)
					{
						if (bid != 4)
						{
							player.at(turn).setRole(NAPOLEON);
							gameState = NAPOLEON_CHOOSE_TRUMP;
							turnCounter = 0;
						}
						else
							turnCounter = 0;
					}

				}
				else if (gameState == NAPOLEON_CHOOSE_TRUMP)
				{
					//Render hands
					player.at(0).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(1).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 90);
					player.at(2).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(3).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(4).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 270);

					if (player.at(turn).isCPU() == false)
					{
						if (SDL_PollEvent(&e) != 0)
						{
							//User requests quit
							if (e.type == SDL_QUIT)
							{
								gameState = QUIT_GAME;
							}
							if (e.type == SDL_KEYDOWN)
							{
								//Adjust the velocity
								switch (e.key.keysym.sym)
								{
								case SDLK_RIGHT:
									trumpSuit++;
									if (trumpSuit > NOSUIT)
										trumpSuit = HEARTS;
									break;
								case SDLK_LEFT:
									trumpSuit--;
									if (trumpSuit < HEARTS)
										trumpSuit = NOSUIT;
									break;
								case SDLK_SPACE:
									gameState = NAPOLEON_DEAL_BAGGAGE;
									break;
								}
							}
						}
					}
					//if it is a CPU
					else
					{
						
					}

					text.str("");
					switch (player.at(0).getRole())
					{
					case NAPOLEON:
						text << "Napoleon";
						break;
					case SECRETARY:
						text << "Secretary";
						break;
					case ENEMY:
						text << "Enemy";
						break;
					}
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2 - 100, yourField.y - gTextTexture.getHeight());

					text.str("");
					text << "Choose trump suit";
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - 3 * gTextTexture.getHeight() / 2);

					text.str("");
					switch (trumpSuit)
					{
					case HEARTS:
						text << "Hearts";
						break;
					case SPADES:
						text << "Spades";
						break;
					case DIAMONDS:
						text << "Diamonds";
						break;
					case CLUBS:
						text << "Clubs";
						break;
					case NOSUIT:
						text << "No trump";
						break;
					}
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTextTexture.getHeight()) / 2);

					//Update screen
					SDL_RenderPresent(gRenderer);

				}
				else if (gameState == NAPOLEON_DEAL_BAGGAGE)
				{
					//Render hands
					player.at(0).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(1).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 90);
					player.at(2).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(3).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(4).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 270);

					if (player.at(turn).isCPU() == false)
					{
						if (SDL_PollEvent(&e) != 0)
						{
							//User requests quit
							if (e.type == SDL_QUIT)
							{
								gameState = QUIT_GAME;
							}
							player.at(turn).getHand()->handleEvent(&e);
							playerBaggage.getHand()->handleEvent(&e);
							if (e.type == SDL_KEYDOWN)
							{
								//Adjust the velocity
								switch (e.key.keysym.sym)
								{
								case SDLK_SPACE:
									if (playerBaggage.getHand()->getSelectedCardIndex() != -1 && player.at(turn).getHand()->getSelectedCardIndex() != -1)
									{
										swapCards(*(playerBaggage.getHand()->at(playerBaggage.getHand()->getSelectedCardIndex())),
											*(player.at(turn).getHand()->at(player.at(turn).getHand()->getSelectedCardIndex())));
										player.at(turn).getHand()->sort();
									}
									break;
								case SDLK_ESCAPE:
									playerBaggage.getHand()->deselectAll();
									player.at(turn).getHand()->deselectAll();
									break;
								case SDLK_RETURN:
									player.at(turn).getHand()->deselectAll();
									gameState = NAPOLEON_CHOOSE_SEC;
								}
							}
						}
					}
					//if it is a CPU
					else
					{

					}

					//render baggage
					playerBaggage.getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);

					text.str("");
					switch (trumpSuit)
					{
					case HEARTS:
						text << "Trump Suit: Hearts";
						break;
					case SPADES:
						text << "Trump Suit: Spades";
						break;
					case DIAMONDS:
						text << "Trump Suit: Diamonds";
						break;
					case CLUBS:
						text << "Trump Suit: Clubs";
						break;
					case NOSUIT:
						text << "Trump Suit: No trump";
						break;
					}
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 + CARD_HEIGHT / 2);

					//Update screen
					SDL_RenderPresent(gRenderer);
				}
				else if (gameState == NAPOLEON_CHOOSE_SEC)
				{
					//Render hands
					player.at(0).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(1).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 90);
					player.at(2).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(3).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(4).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 270);

					if (player.at(turn).isCPU() == false)
					{
						if (SDL_PollEvent(&e) != 0)
						{
							//User requests quit
							if (e.type == SDL_QUIT)
							{
								gameState = QUIT_GAME;
							}
							if (e.type == SDL_KEYDOWN)
							{
								//Adjust the velocity
								switch (e.key.keysym.sym)
								{
								case SDLK_UP:
									if (secCard.getSuit() == CLUBS)
										secCard.setCard(HEARTS, secCard.getValue());
									else
										secCard.setCard(secCard.getSuit() + 1, secCard.getValue());
									break;
								case SDLK_DOWN:
									if (secCard.getSuit() == HEARTS)
										secCard.setCard(CLUBS, secCard.getValue());
									else
										secCard.setCard(secCard.getSuit() - 1, secCard.getValue());
									break;
								case SDLK_RIGHT:
									if (secCard.getValue() == 13)
										secCard.setCard(secCard.getSuit(), 1);
									else
										secCard.setCard(secCard.getSuit(), secCard.getValue() + 1);
									break;
								case SDLK_LEFT:
									if (secCard.getValue() == 1)
										secCard.setCard(secCard.getSuit(), 13);
									else
										secCard.setCard(secCard.getSuit(), secCard.getValue() - 1);
									break;
								case SDLK_SPACE:
									int flag = true;
									//find the person who has the secretary card and set them as secretary
									for (int i = 0; i < TOTAL_CARDS; i++)
									{
										//make sure selected card is not within your hand or your baggage
										if (player.at(turn).getHand()->at(i)->getSuit() == secCard.getSuit() &&
											player.at(turn).getHand()->at(i)->getValue() == secCard.getValue())
										{
											cout << "Please choose a valid secretary";
											flag = false;
											break;
										}
									}
									for (int i = 0; i < 2; i++)
									{
										//make sure selected card is not within your hand or your baggage
										if (playerBaggage.getHand()->at(i)->getSuit() == secCard.getSuit() &&
											playerBaggage.getHand()->at(i)->getValue() == secCard.getValue())
										{
											cout << "Please choose a valid secretary";
											flag = false;
											break;
										}
									}
									if (flag == true)
									{
									//otherwise, put selected person as sec
										for (int i = 0; i < TOTAL_CARDS; i++)
										{
											for (int j = 0; j < 5; j++)
											if (player.at(j).getHand()->at(i)->getSuit() == secCard.getSuit() &&
												player.at(j).getHand()->at(i)->getValue() == secCard.getValue())
											{
												player.at(j).setRole(SECRETARY);
												gameState = IN_GAME;
												break;
											}
										}
									}
									break;
								}
							}
						}

					}
					//if it's a cpu, do something
					else
					{

					}

					text.str("");
					text << "Choose secretary";
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - 3 * gTextTexture.getHeight() / 2 - CARD_HEIGHT / 2);

					text.str("");
					switch (trumpSuit)
					{
					case HEARTS:
						text << "Trump Suit: Hearts";
						break;
					case SPADES:
						text << "Trump Suit: Spades";
						break;
					case DIAMONDS:
						text << "Trump Suit: Diamonds";
						break;
					case CLUBS:
						text << "Trump Suit: Clubs";
						break;
					case NOSUIT:
						text << "Trump Suit: No trump";
						break;
					}
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 + CARD_HEIGHT / 2);

					secCard.render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);

					//Update screen
					SDL_RenderPresent(gRenderer);
				}
				else if (gameState == IN_GAME)
				{
					if (turnState == FIRST_ROUND_FIRST_TURN_STATE)
					{
						//set Napoleon as the player who goes first
						for (int i = 0; i < player.size(); i++)
						{
							if (player.at(i).getRole() == NAPOLEON)
							{
								turn = i;
								break;
							}
						}

						turnCounter = 0;
						player.at(turn).getHand()->setAllViable();

						//if it is a human player, get their inputs
						if (player.at(turn).isCPU() == false)
						{
							if (SDL_PollEvent(&e) != 0)
							{
								//User requests quit
								if (e.type == SDL_QUIT)
								{
									gameState = QUIT_GAME;
								}

								player.at(turn).getHand()->handleEvent(&e);
								
								//play selected
								if (e.type == SDL_KEYDOWN)
								{
									//Adjust the velocity
									switch (e.key.keysym.sym)
									{
									case SDLK_SPACE:
										if (player.at(turn).getHand()->playSelected(c.at(turn)) == true)
										{
											firstSuit = c.at(turn).getSuit();
											c.at(turn).setHidden(false);
											turn++;
											turnCounter++;
											//if it was p4 who last played, set p0 as next player
											if (turn == 5)
												turn = 0;
											turnState = FOLLOWING_TURN_STATE;
										}
										break;
									case SDLK_ESCAPE:
										player.at(turn).getHand()->deselectAll();
										break;
									}
								}

							}
						}

						//else, let AI play cards
						else
						{
							index = rand() % player.at(turn).getHand()->getHandSize();
							while (player.at(turn).getHand()->at(index)->isViablePlay() == false)
							{
								index = rand() % player.at(turn).getHand()->getHandSize();
							}
							player.at(turn).getHand()->setSelectedCardIndex(index);
							player.at(turn).getHand()->playSelected(c.at(turn));
							firstSuit = c.at(turn).getSuit();
							c.at(turn).setHidden(false);
							SDL_Delay(1000);
							turn++;
							turnCounter++;
							//if it was p4 who last played, set p0 as next player
							if (turn == 5)
								turn = 0;
							turnState = FOLLOWING_TURN_STATE;
						}
					}
					else if (turnState == FIRST_TURN_STATE)
					{

						turnCounter = 0;
						player.at(turn).getHand()->setAllViable();

						//if it is a human player, get their inputs
						if (player.at(turn).isCPU() == false)
						{
							if (SDL_PollEvent(&e) != 0)
							{
								//User requests quit
								if (e.type == SDL_QUIT)
								{
									gameState = QUIT_GAME;
								}
								player.at(turn).getHand()->handleEvent(&e);
								//play selected
								if (e.type == SDL_KEYDOWN)
								{
									//Adjust the velocity
									switch (e.key.keysym.sym)
									{
									case SDLK_SPACE:
										if (player.at(turn).getHand()->playSelected(c.at(turn)) == true)
										{
											firstSuit = c.at(turn).getSuit();
											c.at(turn).setHidden(false);
											turn++;
											turnCounter++;
											//if it was p4 who last played, set p0 as next player
											if (turn == 5)
												turn = 0;
											turnState = FOLLOWING_TURN_STATE;
										}
										break;
									case SDLK_ESCAPE:
										player.at(turn).getHand()->deselectAll();
										break;
									}
								}

							}
						}

						//else, let AI play cards
						else
						{
							index = rand() % player.at(turn).getHand()->getHandSize();
							while (player.at(turn).getHand()->at(index)->isViablePlay() == false)
							{
								index = rand() % player.at(turn).getHand()->getHandSize();
							}
							player.at(turn).getHand()->setSelectedCardIndex(index);
							player.at(turn).getHand()->playSelected(c.at(turn));
							firstSuit = c.at(turn).getSuit();
							c.at(turn).setHidden(false);
							SDL_Delay(1000);
							turn++;
							turnCounter++;
							//if it was p4 who last played, set p0 as next player
							if (turn == 5)
								turn = 0;
							turnState = FOLLOWING_TURN_STATE;
						}
					}
					else if (turnState == FOLLOWING_TURN_STATE)
					{
						player.at(turn).getHand()->findViablePlay(firstSuit);

						//if it is a human player, get their inputs
						if (player.at(turn).isCPU() == false)
						{
							if (SDL_PollEvent(&e) != 0)
							{
								//User requests quit
								if (e.type == SDL_QUIT)
								{
									gameState = QUIT_GAME;
								}
								player.at(turn).getHand()->handleEvent(&e);
								//play selected
								if (e.type == SDL_KEYDOWN)
								{
									//Adjust the velocity
									switch (e.key.keysym.sym)
									{
									case SDLK_SPACE:
										if (player.at(turn).getHand()->playSelected(c.at(turn)) == true)
										{
											c.at(turn).setHidden(false);
											turn++;
											turnCounter++;
											//if everyone has played, determine winner and move to the next round
											if (turnCounter == 5)
											{
												wait = true;
												round++;
												Cards roundWinner = determineWinner(c.at(0), c.at(1), c.at(2), c.at(3), c.at(4), firstSuit, trumpSuit);

												//give the player with the winning card a point and let them go first in the next round
												for (int i = 0; i < player.size(); i++)
												{
													if (roundWinner.getSuit() == c.at(i).getSuit() && roundWinner.getValue() == c.at(i).getValue())
													{
														player.at(i).setPoints(player.at(i).getPoints() + 1);
														turn = i;
													}
												}

												if (round == 10)
													gameState = GAME_OVER;
												else
												{
													turnState = FIRST_TURN_STATE;
												}

											}

											//if not everyone has played yet, let everyone play
											else
											{
												//if it was p4 who last played, set p0 as next player
												if (turn == 5)
													turn = 0;
												turnState = FOLLOWING_TURN_STATE;
											}
										}
										break;
									case SDLK_ESCAPE:
										player.at(turn).getHand()->deselectAll();
										break;
									}
								}

							}
						}

						//else, let AI play cards
						else
						{
							index = rand() % player.at(turn).getHand()->getHandSize();
							while (player.at(turn).getHand()->at(index)->isViablePlay() == false)
							{
								index = rand() % player.at(turn).getHand()->getHandSize();
							}
							player.at(turn).getHand()->setSelectedCardIndex(index);
							player.at(turn).getHand()->playSelected(c.at(turn));
							c.at(turn).setHidden(false);
							SDL_Delay(1000);
							turn++;
							turnCounter++;

							//if everyone has played, determine winner and move to the next round
							if (turnCounter == 5)
							{
								wait = true;
								round++;
								Cards roundWinner = determineWinner(c.at(0), c.at(1), c.at(2), c.at(3), c.at(4), firstSuit, trumpSuit);

								//give the player with the winning card a point and let them go first in the next round
								for (int i = 0; i < player.size(); i++)
								{
									if (roundWinner.getSuit() == c.at(i).getSuit() && roundWinner.getValue() == c.at(i).getValue())
									{
										player.at(i).setPoints(player.at(i).getPoints() + 1);
										turn = i;
									}
								}

								cout << "Round " << round << ": " << endl;
								cout << "Player 0: " << player.at(0).getPoints() << endl;
								cout << "Player 1: " << player.at(1).getPoints() << endl;
								cout << "Player 2: " << player.at(2).getPoints() << endl;
								cout << "Player 3: " << player.at(3).getPoints() << endl;
								cout << "Player 4: " << player.at(4).getPoints() << endl;

								if (round == 10)
									gameState = GAME_OVER;
								else
								{
									turnState = FIRST_TURN_STATE;
								}

							}

							//if not everyone has played yet, let everyone play 
							else
							{
								//if it was p4 who last played, set p0 as next player
								if (turn == 5)
									turn = 0;
								turnState = FOLLOWING_TURN_STATE;
							}
						}
					}

					//render role text
					text.str("");
					switch (player.at(0).getRole())
					{
					case NAPOLEON:
						text << "Napoleon";
						break;
					case SECRETARY:
						text << "secretary";
						break;
					case ENEMY:
						text << "Enemy";
						break;
					}
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2 - 100, yourField.y - gTextTexture.getHeight());

					//render trump suit
					text.str("");
					switch (trumpSuit)
					{
					case HEARTS:
						text << "Trump Suit: Hearts";
						break;
					case SPADES:
						text << "Trump Suit: Spades";
						break;
					case DIAMONDS:
						text << "Trump Suit: Diamonds";
						break;
					case CLUBS:
						text << "Trump Suit: Clubs";
						break;
					case NOSUIT:
						text << "Trump Suit: No trump";
						break;
					}
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTextTexture.getHeight()) / 2);

					//render points
					text.str("");
					text << "Points: " << player.at(0).getPoints();
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2 + 100, yourField.y - gTextTexture.getHeight());

					text.str("");
					text << "Points: " << player.at(1).getPoints();
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (opp1Field.x + opp1Field.w) / 2 - gTextTexture.getWidth() / 2, opp1Field.y + opp1Field.h);

					text.str("");
					text << "Points: " << player.at(2).getPoints();
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (opp2Field.x + opp2Field.w) / 2 - gTextTexture.getWidth() / 2, opp2Field.y + opp2Field.h);

					text.str("");
					text << "Points: " << player.at(3).getPoints();
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, opp3Field.x + opp3Field.w/2- gTextTexture.getWidth() / 2, opp3Field.y + opp3Field.h);

					text.str("");
					text << "Points: " << player.at(4).getPoints();
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, opp4Field.x + opp4Field.w/ 2 - gTextTexture.getWidth() / 2, opp4Field.y + opp4Field.h);

					//Render hands
					player.at(0).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(1).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 90);
					player.at(2).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(3).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					player.at(4).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 270);

					for (int i = 0; i < player.at(0).getHand()->getHandSize(); i++)
					{
						if (player.at(0).getHand()->at(i)->getCardSprite() == CARD_SPRITE_MOUSE_OVER_MOTION)
							player.at(0).getHand()->at(i)->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					}

					//render all the cards played so far
					if (c.at(0).getHidden() == false)
						c.at(0).render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					if (c.at(1).getHidden() == false)
						c.at(1).render(gRenderer, &cardSheetTexture, &cardBackTexture, 90);
					if (c.at(2).getHidden() == false)
						c.at(2).render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					if (c.at(3).getHidden() == false)
						c.at(3).render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
					if (c.at(4).getHidden() == false)
						c.at(4).render(gRenderer, &cardSheetTexture, &cardBackTexture, 270);

					//Update screen
					SDL_RenderPresent(gRenderer);

					//wait as the winner of the round is determined, add a cool animation here
					if (wait == true)
					{
						SDL_Delay(2000);

						//set all played cards to be hidden
						for (int i = 0; i < c.size(); i++)
							c.at(i).setHidden(true);

						wait = false;
					}
				}
				else if (gameState == GAME_OVER)
				{
					if (SDL_PollEvent(&e) != 0)
					{
						//User requests quit
						if (e.type == SDL_QUIT)
						{
							gameState = QUIT_GAME;
						}
						if (e.type == SDL_KEYDOWN)
						{
							//Adjust the velocity
							switch (e.key.keysym.sym)
							{
							case SDLK_SPACE:
								gameState = PLAY_AGAIN_SCREEN;
							}
						}
					}

					//Add up Napoleon and secretary's points
					int napPoints = 0;
					for (int i = 0; i < player.size(); i++)
					{
						if (player.at(i).getRole() == NAPOLEON || player.at(i).getRole() == SECRETARY)
						{
							napPoints += player.at(i).getPoints();
						}
					}
					text.str("");
					text << "Napoleon and secretary got " << napPoints << " points!";
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - gTextTexture.getHeight() / 2);

					text.str("");
					if (napPoints >= bid)
						text << "Napoleon and secretary wins!";
					else
						text << "Enemies win!";
					gTextTexture.loadFromRenderedText(gRenderer, text.str().c_str(), textColor, gFont);
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 + gTextTexture.getHeight() / 2);

					//Update screen
					SDL_RenderPresent(gRenderer);
				}

				else if (gameState == PLAY_AGAIN_SCREEN)
				{
					if (SDL_PollEvent(&e) != 0)
					{
						//User requests quit
						if (e.type == SDL_QUIT)
						{
							gameState = QUIT_GAME;
						}
					}

					//Update screen
					SDL_RenderPresent(gRenderer);
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}