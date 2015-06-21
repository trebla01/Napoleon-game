#ifndef DIALOGUEBUTTON_H
#define DIALOGUEBUTTON_H

#include "LTexture.h"
#include "Constants.h"

// Dialogue Buttons
class DialogueButton
{
public:
	//Initializes internal variables
	DialogueButton();
	DialogueButton(std::string t, SDL_Rect box, SDL_Rect button);

	//Sets top left position
	void setBoxRect(int x, int y, int w, int h);
	void setButtonRect(int x, int y, int w, int h);

	//sets text
	void setText(std::string t);

	//Handles button event
	void handleEvent(SDL_Event* e);

	//Shows dialogue box
	void render(SDL_Renderer* gRenderer);

private:
	std::string text;

	//Top left position for dialogue box
	SDL_Rect boxRect;

	//Top left position for clickable button
	SDL_Rect buttonRect;

	//Currently used global sprite
	LCardSprite buttonCurrentSprite;
};

#endif