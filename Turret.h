#pragma once
#include <memory>
#include "SDL2/SDL.h"
#include "MathAddon.h"
#include "Vector2D.h"
#include "TextureLoader.h"
#include "SoundLoader.h"
#include "Unit.h"
#include "Projectile.h"
#include "Timer.h"



class Turret
{
public:
	Turret(SDL_Renderer* renderer, Vector2D setPos);
	void update(SDL_Renderer* renderer, float dT, std::vector<std::shared_ptr<Unit>>& listUnits,
	std::vector<Projectile>& listProjectiles);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool checkIfOnTile(int x, int y);
	void setAutoShoot(bool value);
	void rotate(float angle);  // Function to rotate the turret
	bool checkOverlap(Vector2D posOther, float sizeOther) {
		return (posOther - pos).magnitude() <= (sizeOther + 0.48f) / 2.0f;
	}

    // New method to get the position of the turret
    Vector2D getPosition() const {
        return pos;
    }
	void setPosition(Vector2D newPosition) {
    	pos = newPosition;
	}
	static constexpr float TURRET_ROTATION_SPEED = 0.1f;
	float angle;
	void removeHealth(int damage);
	bool isAlive() const{
		return (health > 0);
	}
	void setHurt();
	bool isHurt = false;

private:
	bool updateAngle(float dT);
	void shootProjectile(SDL_Renderer* renderer, std::vector<Projectile>& listProjectiles);
	void drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected,
		int offset, int tileSize);
	std::weak_ptr<Unit> findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits);

 // Position of the turret
	Vector2D pos;

	static const float speedAngular, weaponRange;

	Timer timerWeapon;

	std::weak_ptr<Unit> unitTarget;

	SDL_Texture* textureMain = nullptr,
		* textureShadow = nullptr;

	Mix_Chunk* mix_ChunkShoot = nullptr;
	bool shootEnabled = false;
	int health = 20;
	Timer timerJustHurt;


};

