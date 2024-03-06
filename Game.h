#pragma once
#include <vector>
#include <chrono>
#include <memory>
#include "SDL2/SDL.h"
#include "Unit.h"
#include "Turret.h"
#include "Projectile.h"
#include "Level.h"
#include "Timer.h"
#include "MathAddon.h"

class Game
{
private:
    enum class PlacementMode {
        wall,
        turret
    } placementModeCurrent;

    // Additional variables for turret control
    bool turretSelected = false;
    Vector2D turretOriginalPos;

public:
    Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight);
    ~Game();

private:
    void setAutoShootForTurret(int turretIndex, bool autoShootEnabled);
    void processEvents(SDL_Renderer* renderer, bool& running);
    void update(SDL_Renderer* renderer, float dT);
    void updateUnits(float dT);
    void updateProjectiles(float dT);
    void updateSpawnUnitsIfRequired(SDL_Renderer* renderer, float dT);
    void draw(SDL_Renderer* renderer);
    void addUnit(SDL_Renderer* renderer, Vector2D posMouse);
    void addTurret(SDL_Renderer* renderer, Vector2D posMouse);
    void removeTurretsAtMousePosition(Vector2D posMouse);
    void controlTurret(SDL_Renderer* renderer); // Function declaration for turret control
    bool turretClicked(int mouseX, int mouseY); // Function declaration for turret click check
    void moveTurretAngle(SDL_Scancode key);     // Function declaration for turret angle movement
    void moveTurret(Turret& turret, Vector2D direction);

    int mouseDownStatus = 0;

    const int tileSize = 64;
    Level level;

    std::vector<std::shared_ptr<Unit>> listUnits;
    std::vector<Turret> listTurrets;
    std::vector<Projectile> listProjectiles;

    SDL_Texture* textureOverlay = nullptr;
    bool overlayVisible = true;

    Timer spawnTimer, roundTimer;
    int spawnUnitCount = 0;

    Mix_Chunk* mix_ChunkSpawnUnit = nullptr;
};
