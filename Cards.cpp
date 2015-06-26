#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>
#include "LTexture.h"
#include "Cards.h"
#include "Constants.h"

Cards::Cards()
{
	suit = CLUBS;
	value = 1;
	facedown = true;
	isSelected = false;
	last = false;
	viablePlay = true;
	mPosition.x = 0;
	mPosition.y = 0;
	cardCurrentSprite = CARD_SPRITE_MOUSE_OUT;
}

Cards::Cards(int s, int v, bool isFacingDown)
{
	suit = s;
	value = v;
	facedown = isFacingDown;
	isSelected = false;
	viablePlay = true;
	last = false;
	mPosition.x = 0;
	mPosition.y = 0;

	cardCurrentSprite = CARD_SPRITE_MOUSE_OUT;
}

void Cards::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

//set suit and value
void Cards::setCard(int s, int v)
{
	suit = s;
	value = v;
}

void Cards::setCard(int s, int v, bool isFacingDown)
{
	suit = s;
	value = v;
	facedown = isFacingDown;
}
void Cards::setSelected(bool s)
{
	isSelected = s;
}
void Cards::setLast(bool l)
{
	last = l;
}
void Cards::setViablePlay(bool v)
{
	viablePlay = v;
}

int Cards::getSuit()
{
	return suit;
}
int Cards::getValue()
{
	return value;
}
bool Cards::getIsSelected()
{
	return isSelected;
}
bool Cards::isLast()
{
	return last;
}
bool Cards::isViablePlay()
{
	return viablePlay;
}
LCardSprite Cards::getCardSprite()
{
	return cardCurrentSprite;
}
void Cards::handleEvent(SDL_Event* e, int cardToCardOffSet)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in card
		bool inside = true;

		if (last == false)
		{
			//Mouse is right of the card icon
			if (x > mPosition.x + cardToCardOffSet - 1)
			{
				inside = false;
			}
			
		}
		else if (last == true)
		{
			//Mouse is right of the entire card
			if (x > mPosition.x + CARD_WIDTH)
			{
				inside = false;
			}

		}
			//Mouse is left of the card
		if (x < mPosition.x)
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
				isSelected = true;
				break;

			case SDL_MOUSEBUTTONUP:
				cardCurrentSprite = CARD_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}

void Cards::render(SDL_Renderer* gRenderer, LTexture* cardSheetTexture, LTexture* cardBackTexture, int degrees) //render and rotate degrees clockwise
{
	SDL_Point rotationPoint = { 0, 0 };
	if (facedown == true)
	{
		cardBackTexture->render(gRenderer, mPosition.x, mPosition.y, NULL, degrees, &rotationPoint);
	}
	else
	{
		if (isSelected == true)
		{
			SDL_Rect getCardRect = { CARD_WIDTH*(value - 1), CARD_HEIGHT*(suit), CARD_WIDTH, CARD_HEIGHT };
			//Show current card sprite
			cardSheetTexture->render(gRenderer, mPosition.x, mPosition.y - onClickOffSet, &getCardRect, degrees, &rotationPoint);
		}
		else
		{
			
			SDL_Rect getCardRect = { CARD_WIDTH*(value - 1), CARD_HEIGHT*(suit), CARD_WIDTH, CARD_HEIGHT };
			//Show current card sprite
			cardSheetTexture->render(gRenderer, mPosition.x, mPosition.y, &getCardRect, degrees, &rotationPoint);

		}
		
		//highlight cards that are viable plays
		if (viablePlay == true && !isSelected)
		{
			SDL_Rect highlightRect;
			if (last == true)
				highlightRect = { mPosition.x, mPosition.y, CARD_WIDTH, CARD_HEIGHT };
			else
				highlightRect = { mPosition.x, mPosition.y, cardToCardOffSet, CARD_HEIGHT };
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0, 0xFF);
			SDL_RenderDrawRect(gRenderer, &highlightRect);
		}
	}
}