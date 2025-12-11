#include "GameObject.hpp"

class Ball : public GameObject
{
public:
	Ball(int x, int y, int diameter, Color color = Color(255, 255, 0, 255), double speed = 1);

	void Update(Uint32 deltaMs, int screenWidth, int screenHeight, const Uint8* keyboard = nullptr) override;
	void OnCollision(GameObject& other) override;

private:
	double bounceFactor = 0.5;
	bool isActive = false;
	double initialSpeed = 1;
};