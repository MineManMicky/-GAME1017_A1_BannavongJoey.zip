#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "SDL.h"
#include "Engine.h"
#include "TextureManager.h"

class GameObject
{
public: // Methods.
	virtual ~GameObject() {};
	virtual void Update() = 0;
	virtual void Render() = 0;
	SDL_FRect* GetDst() { return &m_dst; }
	const bool GetEnabled() const { return m_enabled; }
	void SetEnabled(const bool e) { m_enabled = e; }
protected: // Attributes.
	SDL_FRect m_dst;
	bool m_enabled;
protected: // Methods.
	GameObject() :m_dst({ 0,0,0,0 }), m_enabled(true) {}
	GameObject(const SDL_FRect d) :m_dst(d), m_enabled(true) {}
};

class SpriteObject : public GameObject
{
public: 
	virtual void Render() = 0;
	SDL_Rect* GetSrc() { return &m_src; }
protected: // Attributes.
	SDL_Rect m_src;
protected: // Methods.
	SpriteObject(const SDL_Rect s, const SDL_FRect d) :GameObject(d), m_src(s) {}
};

class AnimatedSpriteObject : public SpriteObject
{
public:
	virtual void Render() = 0;
protected: // Attributes.
	unsigned short m_frame, m_frameMax, m_sprite, m_spriteMin, m_spriteMax;
protected: // Methods.
	AnimatedSpriteObject(const SDL_Rect s, const SDL_FRect d) :SpriteObject(s, d), 
		m_frame(0), m_frameMax(0), m_sprite(0),	m_spriteMin(0), m_spriteMax(0) {}
	void SetAnimation(const unsigned short fMax, const unsigned short sMin, const unsigned short sMax, 
		const int srcY = 0)
	{
		m_frame = 0;
		m_frameMax = fMax;
		m_sprite = m_spriteMin = sMin;
		m_spriteMax = sMax;
		m_src.x = m_src.w * m_sprite;
		m_src.y = srcY;
	}
	void Animate()
	{
		if (m_frame++ == m_frameMax)
		{
			m_frame = 0;
			if (++m_sprite == m_spriteMax)
				m_sprite = m_spriteMin; // I don't use % in case m_sprite doesn't start at 0.
			m_src.x = m_src.w * m_sprite; // Moved to outside if.
		}
	}
};

#endif
