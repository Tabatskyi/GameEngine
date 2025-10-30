#include "Engine.hpp"
#include <algorithm>
#include <iostream>
#include "Player.hpp"
#include "TextureUtils.hpp"

constexpr const unsigned int SCREEN_WIDTH = 800;
constexpr const unsigned int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	Engine engine(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!engine.Init())
	{
		return 0;
	}

	SDL_Renderer* renderer = engine.GetRenderer();

	Scene scene;

	int side = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 10;
	int startX = SCREEN_WIDTH / 2 - side / 2;
	int startY = SCREEN_HEIGHT / 2 - side / 2;

	std::unique_ptr<Player> player = std::make_unique<Player>(startX, startY, side, side, Color(0xFF, 0x00, 0x00), 0.25f);
	player->InitTexture(renderer, "assets/player.bmp", side, side, std::max(4, side / 8));
	scene.Add(std::move(player));

	std::unique_ptr<GameObject> enemy1 = std::make_unique<GameObject>(50, 50, side, side, Color(0x00, 0x80, 0xFF), 0.15f);
	enemy1->SetVelocity(0.10f, 0.07f);
	enemy1->InitTexture(renderer, "assets/enemy1.bmp", side, side, std::max(4, side / 8));
	scene.Add(std::move(enemy1));

	std::unique_ptr<GameObject> enemy2 = std::make_unique<GameObject>(700, 100, side, side, Color(0x00, 0xCC, 0x66), 0.20f);
	enemy2->SetVelocity(-0.08f, 0.12f);
	enemy2->InitTexture(renderer, "assets/enemy2.bmp", side, side, std::max(4, side / 8));
	scene.Add(std::move(enemy2));

	std::unique_ptr<GameObject> enemy3 = std::make_unique<GameObject>(100, 450, side, side, Color(0xFF, 0xA5, 0x00), 0.18f);
	enemy3->SetVelocity(0.11f, -0.06f);
	enemy3->InitTexture(renderer, "assets/enemy3.bmp", side, side, std::max(4, side / 8));
	scene.Add(std::move(enemy3));

	engine.Run(scene);

	engine.Shutdown();

	return 0;
}