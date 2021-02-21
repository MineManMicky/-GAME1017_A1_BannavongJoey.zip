#pragma once
#ifndef _BUTTON3_H_
#define _BUTTON3_H_

#include <string>
#include "SDL.h"
#include "GameObject.h"

class Button3 : public SpriteObject
{
public:
	Button3(SDL_Rect s, SDL_FRect d, const char* key);
	void Update();
	void Render();

protected:
	std::string m_key;
	enum BtnState { STATE_UP, STATE_OVER, STATE_DOWN } m_state; // enum defined locally.
	virtual void Execute() = 0; // Button3 is an abstract base class.
};

class PlayButton : public Button3
{
private:
	void Execute();
public:
	PlayButton(SDL_Rect s, SDL_FRect d, const char* k);
};

#endif

