#include "Game.h"
#include <iostream>

Game::Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight) :
    placementModeCurrent(PlacementMode::wall), 
    level(renderer, windowWidth / tileSize, windowHeight / tileSize),
    spawnTimer(0.25f), roundTimer(5.0f) {

    //Run the game.
    if (window != nullptr && renderer != nullptr) {
        //Load the overlay texture.
        textureOverlay = TextureLoader::loadTexture(renderer, "Overlay.bmp");

        //Load the spawn unit sound.
        // mix_ChunkSpawnUnit = SoundLoader::loadSound("Spawn Unit.ogg");

        //Store the current times for the clock.
        auto time1 = std::chrono::system_clock::now();
        auto time2 = std::chrono::system_clock::now();

        //The amount of time for each frame (60 fps).
        const float dT = 1.0f / 60.0f;
        level.setFixedObstacle(2, 2);
        level.setFixedObstacle(3, 3);
        level.setFixedObstacle(4, 4);
        level.setFixedObstacle(3, 5);
        level.setFixedObstacle(2, 6);


        level.setFixedObstacle(10, 4);
        level.setFixedObstacle(11, 3);
        level.setFixedObstacle(12, 2);
        level.setFixedObstacle(11, 5);
        level.setFixedObstacle(12, 6);
        
        Vector2D posMouse((float)1 / tileSize, (float)200/ tileSize);
        addTurret(renderer, posMouse);
        Vector2D posMouse2((float)700 / tileSize, (float)200/ tileSize);
        addTurret(renderer, posMouse2);
        //Start the game loop and run until it's time to stop.
        bool running = true;
        while (running) {
            //Determine how much time has elapsed since the last frame.
            time2 = std::chrono::system_clock::now();
            std::chrono::duration<float> timeDelta = time2 - time1;
            float timeDeltaFloat = timeDelta.count();

            //If enough time has passed then do everything required to generate the next frame.
            if (timeDeltaFloat >= dT) {
                //Store the new time for the next frame.
                time1 = time2;

                processEvents(renderer, running);
                update(renderer, dT);
                draw(renderer);
            }
        }
    }
}

Game::~Game() {
    //Clean up.
    TextureLoader::deallocateTextures();
    SoundLoader::deallocateSounds();
}

void Game::processEvents(SDL_Renderer* renderer, bool& running) {
    bool mouseDownThisFrame = false;

    //Process events.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            mouseDownThisFrame = (mouseDownStatus == 0);
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouseDownStatus = SDL_BUTTON_LEFT;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
                mouseDownStatus = SDL_BUTTON_RIGHT;
            break;
        case SDL_MOUSEBUTTONUP:
            mouseDownStatus = 0;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                running = false;
                break;
            case SDL_SCANCODE_1:
                placementModeCurrent = PlacementMode::wall;
                break;
            case SDL_SCANCODE_2:
                placementModeCurrent = PlacementMode::turret;
                break;
            case SDL_SCANCODE_H:
                overlayVisible = !overlayVisible;
                break;
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_I:
            case SDL_SCANCODE_J:
            case SDL_SCANCODE_K:
            case SDL_SCANCODE_L:
                if (true) {
                    moveTurretAngle(event.key.keysym.scancode);
                }
                break;
            case SDL_SCANCODE_LSHIFT:
            {
                index = 1;
                setAutoShootForTurret(0, true); // Player 1 turret
                break;
            }
            case SDL_SCANCODE_RSHIFT:
            {
                index = 0;
                setAutoShootForTurret(1, true); // Player 1 turret
                break;
            }
            case SDL_SCANCODE_Q:
                rotateTurret(0, -Turret::TURRET_ROTATION_SPEED); // Rotate left for turret 0
                break;

            case SDL_SCANCODE_E:
                rotateTurret(0, Turret::TURRET_ROTATION_SPEED); // Rotate right for turret 0
                break;
            case SDL_SCANCODE_U:
                rotateTurret(1, -Turret::TURRET_ROTATION_SPEED); // Rotate left for turret 0
                break;

            case SDL_SCANCODE_O:
                rotateTurret(1, Turret::TURRET_ROTATION_SPEED); // Rotate right for turret 0
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_LSHIFT:
                setAutoShootForTurret(0, false); // Player 1 turret
                break;
            case SDL_SCANCODE_RSHIFT:
                setAutoShootForTurret(1, false); // Player 2 turret
                break;
            }
            break;
        }
    }
}

void Game::rotateTurret(int turretIndex, float angleChange) {
    if (turretIndex >= 0 && turretIndex < listTurrets.size()) {
        listTurrets[turretIndex].rotate(angleChange);
    }
}

void Game::setAutoShootForTurret(int turretIndex, bool autoShootEnabled) {
    if (turretIndex >= 0 && turretIndex < listTurrets.size()) {
        listTurrets[turretIndex].setAutoShoot(autoShootEnabled);
    }
}

bool Game::turretClicked(int mouseX, int mouseY) {
    for (auto& turret : listTurrets) {
        Vector2D turretPos = turret.getPosition();
        if (mouseX >= turretPos.x - tileSize && mouseX <= turretPos.x + tileSize ||
            mouseY >= turretPos.y -tileSize && mouseY <= turretPos.y + tileSize) {
            return true;
        }
    }
    return false;
}

void Game::moveTurretAngle(SDL_Scancode key) {
    if (!listTurrets.empty()) {
        Turret& selectedTurret = listTurrets[0];
        if (listTurrets.size() > 1) {
            Turret& selectedTurret2 = listTurrets.back();

            // Define the movement step
            const float movementStep = 0.1f; // Adjust this value for desired smoothness

            switch (key) {
            case SDL_SCANCODE_W:
                moveTurret(selectedTurret, Vector2D(0, -movementStep));
                break;
            case SDL_SCANCODE_A:
                moveTurret(selectedTurret, Vector2D(-movementStep, 0));
                break;
            case SDL_SCANCODE_S:
                moveTurret(selectedTurret, Vector2D(0, movementStep));
                break;
            case SDL_SCANCODE_D:
                moveTurret(selectedTurret, Vector2D(movementStep, 0));
                break;
            case SDL_SCANCODE_I:
                moveTurret(selectedTurret2, Vector2D(0, -movementStep));
                break;
            case SDL_SCANCODE_J:
                moveTurret(selectedTurret2, Vector2D(-movementStep, 0));
                break;
            case SDL_SCANCODE_K:
                moveTurret(selectedTurret2, Vector2D(0, movementStep));
                break;
            case SDL_SCANCODE_L:
                moveTurret(selectedTurret2, Vector2D(movementStep, 0));
                break;
            }
        }
    }
}


void Game::moveTurret(Turret& turret, Vector2D direction) {
    Vector2D oldPos = turret.getPosition();
    Vector2D newPos = oldPos + direction;

    if (!level.isTileWall(newPos.x, newPos.y)) {
        turret.setPosition(newPos);
    }
}


void Game::update(SDL_Renderer* renderer, float dT) {
    // updateUnits(dT);
    // for (auto it = listTurrets.begin(); it != listTurrets.end();) {
    //     if (!it->isAlive()) {
    //         it = listTurrets.erase(it);
    //     } else {
    //         ++it;
    //     }
    // }
    for (auto& turretSelected : listTurrets)
        turretSelected.update(renderer, dT, listUnits, listProjectiles);


    updateProjectiles(dT);
    // updateSpawnUnitsIfRequired(renderer, dT);
}

// void Game::updateUnits(float dT) {
//     auto it = listUnits.begin();
//     while (it != listUnits.end()) {
//         bool increment = true;

//         if ((*it) != nullptr) {
//             (*it)->update(dT, level, listUnits);

//             if ((*it)->isAlive() == false) {
//                 it = listUnits.erase(it);
//                 increment = false;
//             }
//         }

//         if (increment)
//             it++;
//     }
// }

void Game::updateProjectiles(float dT) {
    auto it = listProjectiles.begin();
    while (it != listProjectiles.end()) {
        (*it).update(dT, listTurrets, level, index);


        if ((*it).getCollisionOccurred())
            it = listProjectiles.erase(it);
        else
            it++;
    }
}

// void Game::updateSpawnUnitsIfRequired(SDL_Renderer* renderer, float dT) {
//     spawnTimer.countDown(dT);

//     if (listUnits.empty() && spawnUnitCount == 0) {
//         roundTimer.countDown(dT);
//         if (roundTimer.timeSIsZero()) {
//             spawnUnitCount = 15;
//             roundTimer.resetToMax();
//         }
//     }

//     if (spawnUnitCount > 0 && spawnTimer.timeSIsZero()) {
//         addUnit(renderer, level.getRandomEnemySpawnerLocation());

//         if (mix_ChunkSpawnUnit != nullptr)
//             Mix_PlayChannel(-1, mix_ChunkSpawnUnit, 0);

//         spawnUnitCount--;
//         spawnTimer.resetToMax();
//     }
// }

void Game::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    level.draw(renderer, tileSize);

    // for (auto& unitSelected : listUnits)
    //     if (unitSelected != nullptr)
    //         unitSelected->draw(renderer, tileSize);

    for (auto& turretSelected : listTurrets)
        turretSelected.draw(renderer, tileSize);


    for (auto& projectileSelected : listProjectiles)
        projectileSelected.draw(renderer, tileSize);
    
    if (textureOverlay != nullptr && overlayVisible) {
        int w = 0, h = 0;
        SDL_QueryTexture(textureOverlay, NULL, NULL, &w, &h);
        SDL_Rect rect = { 40, 40, w, h };
        SDL_RenderCopy(renderer, textureOverlay, NULL, &rect);
    }

    SDL_RenderPresent(renderer);
}

// void Game::addUnit(SDL_Renderer* renderer, Vector2D posMouse) {
//     listUnits.push_back(std::make_shared<Unit>(renderer, posMouse));
// }

void Game::addTurret(SDL_Renderer* renderer, Vector2D posMouse) {
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f);
    listTurrets.push_back(Turret(renderer, pos));
}

void Game::removeTurretsAtMousePosition(Vector2D posMouse) {
    for (auto it = listTurrets.begin(); it != listTurrets.end();) {
        if ((*it).checkIfOnTile((int)posMouse.x, (int)posMouse.y))
            it = listTurrets.erase(it);
        else
            it++;
    }
}
