#pragma once
#include <memory>
#include "SDL2/SDL.h"
#include "Vector2D.h"
#include "TextureLoader.h"
#include "Unit.h"
#include "Level.h" // Include Level class for collision detection

class Projectile
{
public:
	Projectile(SDL_Renderer* renderer, Vector2D setPos, Vector2D setDirectionNormal);
	void update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits, Level& level); // Include Level reference for collision detection
	void draw(SDL_Renderer* renderer, int tileSize);
	bool getCollisionOccurred();

private:
	void checkCollisions(std::vector<std::shared_ptr<Unit>>& listUnits);

	Vector2D pos, directionNormal;
	static const float speed, size, distanceTraveledMax;
	float distanceTraveled = 0.0f;

	SDL_Texture* texture = nullptr;

	bool collisionOccurred = false;
};
