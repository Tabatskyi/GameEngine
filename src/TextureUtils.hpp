#pragma once

#include <SDL2/SDL.h>
#include <string>


SDL_Texture* LoadTextureBMP(SDL_Renderer* renderer, const std::string& path);

SDL_Texture* CreateCheckerTexture(SDL_Renderer* renderer, int width, int height, SDL_Color color1, SDL_Color color2, int cell);

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const std::string& path, int width, int height, int cell, SDL_Color color1 = SDL_Color{ 255, 0, 255, 255 }, SDL_Color color2 = SDL_Color{ 0, 0, 0, 255 });