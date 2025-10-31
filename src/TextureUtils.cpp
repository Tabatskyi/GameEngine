#include "TextureUtils.hpp"

std::shared_ptr<SDL_Texture> LoadTextureBMP(SDL_Renderer* renderer, const std::string& path)
{
	SDL_Surface* surface = SDL_LoadBMP(path.c_str());
	if (!surface) 
		return {};
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture) 
		return {};
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	return std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
}

std::shared_ptr<SDL_Texture> CreateCheckerTexture(SDL_Renderer* renderer, int width, int height, SDL_Color color1, SDL_Color color2, int cell)
{
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (!texture)
		return {};

	void* pixels = nullptr;
	int pitch = 0;
	if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0)
	{
		SDL_DestroyTexture(texture);
		return {};
	}
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	Uint32 col1 = SDL_MapRGBA(format, color1.r, color1.g, color1.b, color1.a);
	Uint32 col2 = SDL_MapRGBA(format, color2.r, color2.g, color2.b, color2.a);
	int stride = pitch / 4;

	Uint32* dst = static_cast<Uint32*>(pixels);
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			bool toggle = (((x / cell) + (y / cell)) % 2) == 0;
			dst[y * stride + x] = toggle ? col1 : col2;
		}
	}
	SDL_UnlockTexture(texture);
	SDL_FreeFormat(format);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	return std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
}

std::shared_ptr<SDL_Texture> LoadTexture(SDL_Renderer* renderer, const std::string& path, int width, int height, int cell, SDL_Color color1, SDL_Color color2)
{
	if (std::shared_ptr<SDL_Texture> texture = LoadTextureBMP(renderer, path))
		return texture;
	return CreateCheckerTexture(renderer, width, height, color1, color2, cell);
}