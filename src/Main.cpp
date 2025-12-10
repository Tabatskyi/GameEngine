#include <iostream>
#include "Engine.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Wall.hpp"
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
	player->InitTexture(renderer, "../assets/cat.bmp", side, side, std::max(4, side / 8));
	Player* playerPtr = player.get();
	scene.Add(std::move(player));
	scene.SetPlayer(playerPtr);

	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(50, 50, side, side, Color(0x00, 0x80, 0xFF), 0.15f);
	enemy->SetTarget(playerPtr);
	enemy->InitTexture(renderer, "../assets/scp-67.bmp", side, side, std::max(4, side / 8));
	scene.Add(std::move(enemy));

	std::unique_ptr<Wall> wall1 = std::make_unique<Wall>(300, 200, 200, 30, Color(0x80, 0x80, 0x80));
	scene.Add(std::move(wall1));

	std::unique_ptr<Wall> wall2 = std::make_unique<Wall>(350, 400, 30, 150, Color(0x60, 0x60, 0x60));
	scene.Add(std::move(wall2));

	engine.Run(scene);

	engine.Shutdown();

	return 0;
}