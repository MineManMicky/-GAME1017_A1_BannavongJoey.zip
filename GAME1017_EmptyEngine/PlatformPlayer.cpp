#include "PlatformPlayer.h"
#include "Engine.h"
#include "EventManager.h"
#include "TextureManager.h"
#include <cmath>

PlatformPlayer::PlatformPlayer(SDL_Rect s, SDL_FRect d):AnimatedSpriteObject(s, d),
m_state(STATE_JUMPING), m_grounded(false), m_facingLeft(false), m_maxVelX(10.0),
m_maxVelY(JUMPFORCE), m_grav(GRAV), m_drag(0.8)
{
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	SetAnimation(1, 8, 9); // Initialize jump animation.
}

void PlatformPlayer::Update()
{
	// Checking states.
	switch (m_state)
	{
	case STATE_IDLING:
		// Transition to run.
		if (EVMA::KeyPressed(SDL_SCANCODE_A) || EVMA::KeyPressed(SDL_SCANCODE_D))
		{
			m_state = STATE_RUNNING;
			SetAnimation(3, 0, 8, 256); // , 256
		}
		// Transition to jump.
		else if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_grounded)
		{
			m_accelY = -JUMPFORCE; // SetAccelY(-JUMPFORCE);
			m_grounded = false; // SetGrounded(false);
			m_state = STATE_JUMPING;
			SetAnimation(1, 8, 9, 256);
		}
		break;
	case STATE_RUNNING:
		// Move left and right.
		if (EVMA::KeyHeld(SDL_SCANCODE_A) && m_dst.x > 0)
		{
			m_accelX = -1.5;
			if (!m_facingLeft)
				m_facingLeft = true;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D) && m_dst.x < WIDTH - m_dst.w)
		{
			m_accelX = 1.5;
			if (m_facingLeft)
				m_facingLeft = false;
		}
		// Transition to jump.
		if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_grounded)
		{
			m_accelY = -JUMPFORCE; 
			m_grounded = false; 
			m_state = STATE_JUMPING;
			SetAnimation(1, 8, 9, 256);
		}
		// Transition to idle.
		if (!EVMA::KeyHeld(SDL_SCANCODE_A) && !EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_state = STATE_IDLING;
			SetAnimation(1, 0, 1, 256); // , 256
		}
		break;
	case STATE_JUMPING:
		// Move in mid-air is cool.
		if (EVMA::KeyHeld(SDL_SCANCODE_A) && m_dst.x > 0)
		{
			m_accelX = -1.5;
			if (!m_facingLeft)
				m_facingLeft = true;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D) && m_dst.x < WIDTH - m_dst.w)
		{
			m_accelX = 1.5;
			if (m_facingLeft)
				m_facingLeft = false;
		}
		// Hit the ground, transition to run.
		if (m_grounded)
		{
			m_state = STATE_RUNNING;
			SetAnimation(3, 0, 8, 256);
		}
		break;
	}
	// Player movement. X axis first.
	m_velX += m_accelX;
	m_velX *= (m_grounded ? m_drag : 1.0); // Cheeky deceleration.
	m_velX = std::min(std::max(m_velX, -m_maxVelX), m_maxVelX);
	m_dst.x += (float)m_velX; // May have to cast to (int)
	
	m_velY += m_accelY + m_grav;
	m_velY = std::min(std::max(m_velY, -m_maxVelY), m_maxVelY); // (m_grav * 5.0)
	m_dst.y += (float)m_velY;
	
	m_accelX = m_accelY = 0.0; // Resetting accel every frame.
	// Invoke the animation.
	Animate();
}

void PlatformPlayer::Render()
{
	SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("player"),
		&m_src, &m_dst, 0.0, NULL, (m_facingLeft?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE));
}

void PlatformPlayer::Stop()
{
	StopX();
	StopY();
}

void PlatformPlayer::StopX() { m_velX = 0.0; }

void PlatformPlayer::StopY() { m_velY = 0.0; }

void PlatformPlayer::SetAccelX(double a) { m_accelX = a; }

void PlatformPlayer::SetAccelY(double a) { m_accelY = a; }

bool PlatformPlayer::IsGrounded() { return m_grounded; }

void PlatformPlayer::SetGrounded(bool g) { m_grounded = g; }

double PlatformPlayer::GetVelX() { return m_velX; }

double PlatformPlayer::GetVelY() { return m_velY; }

void PlatformPlayer::SetX(float x) { m_dst.x = x; }

void PlatformPlayer::SetY(float y) { m_dst.y = y; }
