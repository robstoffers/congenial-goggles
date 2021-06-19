#include "game.h"
#include "map.h"
#include "input.h"
#include "renderer.h"
#include "camera.h"
#include "physics.h"
#include "door.h"

#include <string.h>
#include <math.h>
#include <nds.h>
#include <stdio.h>

#define FOV     0.7853975

#define WALK_SPEED 1.5f
#define TURN_SPEED 2.25f

short* Game::getBuffer() {
    return this->renderer->getBuffer();
}
int Game::getBufferSize() {
    return this->renderer->getBufferSize();
}

void Game::init(int sw, int sh) {
    mapManager = new Map();
    map = mapManager->getMap();
    mapWidth = mapManager->getWidth();
    mapHeight = mapManager->getHeight();

    this->input = new Input();

    this->physics = new RaycastPhysics();
    this->physics->setMap(this->mapManager);

    this->camera = new RaycastCamera(this->physics->getCamera());
    this->camera->setX(1.5f);
    this->camera->setY(1.5f);
    this->camera->setAngle(0.0f);

    this->renderer = new RaycastRenderer(sw, sh, FOV);
}

bool Game::update(float dt, PrintConsole* console) {
    this->input->update();
    if (input->isStartPressed()) return false;
    if (input->isLeftHeld()) this->camera->setTurnSpeed(-TURN_SPEED);
    if (input->isRightHeld()) this->camera->setTurnSpeed(TURN_SPEED);
    if (input->isUpHeld()) this->camera->setForwardSpeed(WALK_SPEED);
    if (input->isDownHeld()) this->camera->setForwardSpeed(-WALK_SPEED);
    if (input->isLHeld()) this->camera->setStrafeSpeed(WALK_SPEED * -0.5f);
    if (input->isRHeld()) this->camera->setStrafeSpeed(WALK_SPEED * 0.5f);
    
    if (input->isAPressed()) {
        // check if a door is in front of the camera.
        TileInfo info = mapManager->getTileDDA(camera->getX(), camera->getY(), camera->getFacingX(), camera->getFacingY());
        if (info.id == 2) {
            if (info.distance < 2.0f) {
                Door* door = mapManager->getDoor(info.x, info.y);
                if (door) {
                    door->toggle();
                }
            }
        }
    }

    // console->cursorX = 0;
    // console->cursorY = 0;
    // printf("c: (%d, %d), f: (%d, %d)", cameraTileX, cameraTileY, tileX, tileY);

    this->physics->update(dt);
    this->mapManager->update(dt);
    this->renderer->render(this->camera, this->mapManager, dt);

    return true;
}

void Game::dispose() {
    delete physics;
    delete camera;
    delete renderer;
    delete mapManager;
    delete input;
}
