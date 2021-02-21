#pragma once
#ifndef _ASTEROIDSPRITES_H_
#define _ASTEROIDSPRITES_H_

#include "GameObject.h"
#include "SDL.h"
#include <vector>

class Asteroid : public SpriteObject
{
public:
	Asteroid(SDL_Rect s, SDL_FRect d);
	void Update();
	void Render();
	const SDL_FPoint& GetCenter() { return m_center; }
	const double& GetRadius() { return m_radius; }
	void SetColMods(Uint8 r, Uint8 g, Uint8 b);
private:
	SDL_FPoint m_center;
	double m_angle,
		m_dx, m_dy,
		m_radius,
		m_rotSpeed;
	Uint8 m_rMod, m_gMod, m_bMod;
};

class AsteroidField : public GameObject
{ // Manager class for the individual asteroids.
public:
	AsteroidField(unsigned int sz = 6);
	~AsteroidField();
	void Update();
	void Render();
	vector<Asteroid*>& GetAsteroids() { return m_asteroids; }
	const unsigned int GetSize() { return m_size; }
private:
	vector<Asteroid*> m_asteroids;
	int m_size;
};

class Bullet : public SpriteObject
{
public:
	Bullet(SDL_Rect s, SDL_FRect d, const char* key, const double angle);
	void Update();
	void Render();
	const SDL_FPoint& GetCenter() { return m_center; }
	const double& GetRadius() { return m_radius; }
private:
	SDL_FPoint m_center;
	int m_ctr, m_ctrMax;
	double m_angle,
		m_dx, m_dy,
		m_radius;
	std::string m_key;
};

class ShipAsteroids : public AnimatedSpriteObject
{
public: // Methods.
	ShipAsteroids(SDL_Rect s, SDL_FRect d);
	~ShipAsteroids();
	void Update();
	void Render();
	const SDL_FPoint& GetCenter() { return m_center; }
	const double& GetRadius() { return m_radius; }
	vector<Bullet*>& GetBullets() { return m_bullets; }
private: // Properties.
	SDL_FPoint m_center;
	double m_angle,
		m_dx, m_dy,
		m_velX, m_velY,
		m_velMax,
		m_radius,
		m_rotSpeed,
		m_thrust;
	enum PlayerState { STATE_IDLING, STATE_MOVING } m_state;
	vector<Bullet*> m_bullets;
};

#endif