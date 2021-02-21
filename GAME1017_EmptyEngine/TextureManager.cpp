#include "TextureManager.h"
#include "Engine.h"
#include <iostream>

void TextureManager::Init()
{
	// Initialize SDL's image subsystem.
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
		std::cout << "Image init success!" << std::endl;
	else // is 0.
		std::cout << "Image init failed: ERROR - " << IMG_GetError() << std::endl;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
}

void TextureManager::Load(const char* path, const std::string key)
{
	SDL_Texture* temp = IMG_LoadTexture(Engine::Instance().GetRenderer(), path);
	if (temp == nullptr)
		std::cout << "Could not load texture: ERROR - " << IMG_GetError() << std::endl;
	else
	{
		s_textures.emplace(key, temp); // Add to the map.
		std::cout << "Successfully loaded texture '" << key << "'!" << std::endl;
	}
}

void TextureManager::Unload(const std::string key)
{
	if (s_textures.find(key) != s_textures.end())
	{
		SDL_DestroyTexture(s_textures[key]); // Deallocates texture.
		s_textures.erase(key); // Remove texture pointer from map.
	}
	else
		std::cout << "Could not unload '" << key << "' - key not found!" << std::endl;
}

SDL_Texture* TextureManager::GetTexture(const std::string key)
{
	return s_textures[key];
}

void TextureManager::Quit()
{
	for (auto const& i : s_textures)
		SDL_DestroyTexture(s_textures[i.first]);
	s_textures.clear(); // Wrangle all the dangles.
		
	// Unload(i.first); // Alternate single line in for loop to do it all
}

TextureManager::TextureManager(){}

std::map<std::string, SDL_Texture*> TextureManager::s_textures;