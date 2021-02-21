#pragma once
#ifndef _PLATFORMPLAYER_H_
#define _PLATFORMPLAYER_H_

#include "GameObject.h"
#define GRAV 2.0
#define JUMPFORCE 25.0

class PlatformPlayer : public AnimatedSpriteObject
{
public: // Methods.
	PlatformPlayer(SDL_Rect s, SDL_FRect d);
	void Update();
	void Render();
	void Stop();
	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	bool IsGrounded();
	void SetGrounded(bool g);
	double GetVelX();
	double GetVelY();
	void SetX(float x);
	void SetY(float y);
private: // Properties.
	bool m_grounded, m_facingLeft;
	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_grav;
	enum PlayerState { STATE_IDLING, STATE_RUNNING, STATE_JUMPING } m_state;
};

#endif