#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream>
#include <map>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
using namespace std;

class Engine
{
private: // private properties.
	bool m_running = false;
	Uint32 m_start, m_startLast, m_end, m_delta, m_fps;
	double m_deltaTime;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

private: // private method prototypes.
	Engine() {} // Prevents instantiation outside class.
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags);
	void Clean();
	void Wake();
	void HandleEvents();
	// Moved KeyDown from here.
	void Update();
	void Render();
	void Sleep();

public: // public method prototypes.
	int Run();
	static Engine& Instance(); // Static method for object access.
	SDL_Renderer* GetRenderer() const { return m_pRenderer; }
	double GetDeltaTime() const { return m_deltaTime; }
	bool& Running();
	void SetRunning(const bool b);
};

#endif

// Reminder: you can ONLY have declarations in headers, not logical code
