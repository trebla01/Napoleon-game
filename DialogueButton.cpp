#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>
#include "LTexture.h"
#include "DialogueButton.h"
#include "Constants.h"

DialogueButton::DialogueButton()
{
	text = "";
	boxRect = { 0, 0, 0, 0 };
	buttonRect = { 0, 0, 0, 0 };
	buttonCurrentSprite = CARD_SPRITE_MOUSE_OUT;
}
DialogueButton::DialogueButton(std::string t, SDL_Rect box, SDL_Rect button)
{
	text = t;
	boxRect = box;
	buttonRect = button;
	buttonCurrentSprite = CARD_SPRITE_MOUSE_OUT;
}

//Sets top left position
void DialogueButton::setBoxRect(int x, int y, int w, int h)
{
	boxRect = { x, y, w, h };
}
void DialogueButton::setButtonRect(int x, int y, int w, int h)
{
	buttonRect = { x, y, w, h };
}

void DialogueButton::setText(std::string t)
{
	text = t;
}

//Handles button event
void DialogueButton::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < buttonRect.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > buttonRect.x + buttonRect.w)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < buttonRect.y)
		{
			inside = false;
		}
		//Mouse below the card
		else if (y > buttonRect.y + buttonRect.h)
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside)
		{
			buttonCurrentSprite = CARD_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				buttonCurrentSprite = CARD_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				buttonCurrentSprite = CARD_SPRITE_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				buttonCurrentSprite = CARD_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}

//Shows dialogue box
void DialogueButton::render(SDL_Renderer* gRenderer)
{
	//render box 

	//render text

	//render button
}
