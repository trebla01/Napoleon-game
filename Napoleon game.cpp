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
		!cardBackTexture.loadFromFile("Napoleon game/card_back_flowers.png", gRenderer))
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
			//game state flag
			gameStateSprite gameState = BIDDING_STATE;

			//Event handler
			SDL_Event e;

			//game variables
			int bid;
			int index; //hold temporary index for bots to randomly choose card
			int turn = 0; //whose turn it is
			int turnCounter = 0; //when counter == 5, means everyone has played
			int firstSuit; //first played suit, following cards must match if possbile
			int trumpSuit = CLUBS; //trump suit for the game
			turnStateSprite turnState = FIRST_ROUND_FIRST_TURN_STATE; //initially in the first turnstate
			int round = 0;

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
			p0 = Player(Hand(yourCards, false));

			//opponent's hand is hidden
			p1 = Player(Hand(opp1Cards, true));
			p2 = Player(Hand(opp2Cards, true));
			p3 = Player(Hand(opp3Cards, true));
			p4 = Player(Hand(opp4Cards, true));

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

			player.at(0).setRole(NAPOLEON);
			player.at(1).setRole(SECETARY);
			player.at(2).setRole(ENEMY);
			player.at(3).setRole(ENEMY);
			player.at(4).setRole(ENEMY);

			//sets the position of the cards to be displayed
			//bottom is p0
			player.at(0).getHand()->setPositionOfFirstCard(SCREEN_WIDTH / 4 + cardOffSet, SCREEN_HEIGHT - CARD_HEIGHT - cardOffSet);
			
			//top left is p2
			player.at(2).getHand()->setPositionOfFirstCard(cardOffSet, cardOffSet);
			
			//top right is p3
			player.at(3).getHand()->setPositionOfFirstCard(SCREEN_WIDTH / 2 + boardOffSet / 2 + cardOffSet, cardOffSet);
			
			//bottom left is p1
			player.at(1).getHand()->setPositionOfFirstCard(cardOffSet + CARD_HEIGHT, CARD_HEIGHT + 3 * cardOffSet + boardOffSet);
			
			//bottom right is p4
			player.at(4).getHand()->setPositionOfFirstCard(SCREEN_WIDTH - cardOffSet - CARD_HEIGHT, CARD_HEIGHT + 3 * cardOffSet + boardOffSet + CARD_WIDTH + TOTAL_CARDS * cardToCardOffSet - cardToCardOffSet);
			
			//sets the position of the baggage to be displayed
			baggage[0].setPosition(SCREEN_WIDTH / 2 - CARD_WIDTH, SCREEN_HEIGHT / 2 - CARD_HEIGHT / 2);
			baggage[1].setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - CARD_HEIGHT / 2);

			///////////////////////////////  REMOVE LATER
			//random variable to get the game shown at least once before bidding
			int count = 0;

			//While application is running
			while ((gameState != QUIT_GAME))
			{
				if (gameState == BIDDING_STATE)
				{
					if (SDL_PollEvent(&e) != 0)
					{
						//User requests quit
						if (e.type == SDL_QUIT)
						{
							gameState = QUIT_GAME;
						}
					}
					if (count > 0)
					{

						cout << "What would you like to bid? (1-10)" << endl;
						cin >> bid;
						while (bid < 0 || bid > 10)
						{
							cout << "please enter a valid bid (1-10)" << endl;
							cin >> bid;
						}

						cout << "What is the trump suit (hearts, spades, diamonds, clubs)" << endl;
						
						string temp;
						cin >> temp;
						while (temp != "hearts" && temp != "spades" && temp != "diamonds" && temp != "clubs")
						{
							cout << "Please enter a valid trump suit (hearts, spades, diamonds, clubs)" << endl;
							cin >> temp;
						}
						if (temp == "hearts")
							trumpSuit = HEARTS;
						else if (temp == "spades")
							trumpSuit = SPADES;
						else if (temp == "diamonds")
							trumpSuit = DIAMONDS;
						else if (temp == "clubs")
							trumpSuit = CLUBS;

						cout << "Please start the game" << endl;

						gameState = IN_GAME;
					}
					else
						count++;


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
								for (int i = 0; i < TOTAL_CARDS; i++)
								{
									player.at(turn).getHand()->handleEvent(&e);
								}
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
											cout << "Player " << turn << ": ";
											c.at(turn).print();
											cout << endl;
											turn++;
											turnCounter++;
											//if it was p4 who last played, set p0 as next player
											if (turn == 5)
												turn = 0;
											turnState = FOLLOWING_TURN_STATE;
										}
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
							cout << "Player " << turn << ": ";
							c.at(turn).print();
							cout << endl;
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
								for (int i = 0; i < TOTAL_CARDS; i++)
								{
									player.at(turn).getHand()->handleEvent(&e);
								}
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
											cout << "Player " << turn << ": ";
											c.at(turn).print();
											cout << endl;
											turn++;
											turnCounter++;
											//if it was p4 who last played, set p0 as next player
											if (turn == 5)
												turn = 0;
											turnState = FOLLOWING_TURN_STATE;
										}
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
							cout << "Player " << turn << ": ";
							c.at(turn).print();
							cout << endl;
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
								for (int i = 0; i < TOTAL_CARDS; i++)
								{
									player.at(turn).getHand()->handleEvent(&e);
								}
								//play selected
								if (e.type == SDL_KEYDOWN)
								{
									//Adjust the velocity
									switch (e.key.keysym.sym)
									{
									case SDLK_SPACE:
										if (player.at(turn).getHand()->playSelected(c.at(turn)) == true)
										{
											cout << "Player " << turn << ": ";
											c.at(turn).print();
											cout << endl;
											turn++;
											turnCounter++;
											//if everyone has played, determine winner and move to the next round
											if (turnCounter == 5)
											{
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
													turnState = FIRST_TURN_STATE;

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
							cout << "Player " << turn << ": ";
							c.at(turn).print();
							cout << endl;
							turn++;
							turnCounter++;

							//if everyone has played, determine winner and move to the next round
							if (turnCounter == 5)
							{
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
									turnState = FIRST_TURN_STATE;

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
					}

					//Add up Napoleon and Secetary's points
					int napPoints = 0;
					for (int i = 0; i < player.size(); i++)
					{
						if (player.at(i).getRole() == NAPOLEON || player.at(i).getRole() == SECETARY)
						{
							napPoints += player.at(i).getPoints();
						}
					}
					cout << "Napoleon and Secetary got " << napPoints << " points!" << endl;
					if (napPoints >= bid)
					{
						cout << "Napoleon and Secetary wins!" << endl;
					}
					else
						cout << "Enemies win!" << endl;
					gameState = PLAY_AGAIN_SCREEN;
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
				player.at(0).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
				player.at(1).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 90);
				player.at(2).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
				player.at(3).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
				player.at(4).getHand()->render(gRenderer, &cardSheetTexture, &cardBackTexture, 270);

				//render baggage
				baggage[0].render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
				baggage[1].render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);

				//render hovered cards fully
				for (int i = 0; i < player.at(0).getHand()->getHandSize(); i++)
				{
					if (player.at(0).getHand()->at(i)->getCardSprite() == CARD_SPRITE_MOUSE_OVER_MOTION)
						player.at(0).getHand()->at(i)->render(gRenderer, &cardSheetTexture, &cardBackTexture, 0);
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