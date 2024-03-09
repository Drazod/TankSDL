#include "Projectile.h"
#include "iostream"
#include "Turret.h"

const float Projectile::speed = 10.0f;
const float Projectile::size = 0.2f;
const float Projectile::distanceTraveledMax = 20.0f;

Projectile::Projectile(SDL_Renderer* renderer, Vector2D setPos, Vector2D setDirectionNormal) :
    pos(setPos), directionNormal(setDirectionNormal) {
    texture = TextureLoader::loadTexture(renderer, "Projectile.bmp");
}

void Projectile::update(float dT, std::vector<Turret>& listUnits, Level& level, int index) {
    // Move the projectile forward.
    float distanceMove = speed * dT;
    Vector2D newPos = pos + directionNormal * distanceMove;

    // Check collision with walls
    if (!level.isTileWall(newPos.x, newPos.y)) {
        pos = newPos;
    } else {
        // Bounce off the wall at a 60-degree angle
        float angleInRadians = M_PI / 3.0f; // 60-degree angle in radians
        float cosTheta = cos(angleInRadians);
        float sinTheta = sin(angleInRadians);
        float newX = directionNormal.x * cosTheta - directionNormal.y * sinTheta;
        float newY = directionNormal.x * sinTheta + directionNormal.y * cosTheta;
        directionNormal = Vector2D(newX, newY);
        
        pos = newPos + directionNormal * distanceMove; // Move the projectile after bouncing
    }

    distanceTraveled += distanceMove;
    if (distanceTraveled >= distanceTraveledMax)
        collisionOccurred = true;

    checkCollisions(listUnits, index);
}












void Projectile::draw(SDL_Renderer* renderer, int tileSize) {
    if (renderer != nullptr) {
        // Draw the image at the projectile's position.
        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        SDL_Rect rect = {
            (int)(pos.x * tileSize) - w / 2,
            (int)(pos.y * tileSize) - h / 2,
            w,
            h
        };
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}

bool Projectile::getCollisionOccurred() {
    return collisionOccurred;
}

void Projectile::checkCollisions(std::vector<Turret>& listUnits,int index) {
    // Check for a collision with any of the units.
    if (!collisionOccurred) {
        // Check if this overlaps any of the enemy units or not.
        for (int count = 0; count < listUnits.size() && !collisionOccurred; count++) {
            auto& unitSelected = listUnits[index];
            if (unitSelected.checkOverlap(pos, size)) {
                unitSelected.setHurt();
                unitSelected.removeHealth(2);
                collisionOccurred = true;
            }
        }
    }
}
