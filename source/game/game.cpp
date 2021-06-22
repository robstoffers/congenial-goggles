#include "game.h"
#include "map.h"
#include "input.h"
#include "renderer.h"
#include "camera.h"
#include "physics.h"
#include "sprites.h"
#include "door.h"

#include "skeleton.h"

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

void Game::init(int sw, int sh, PrintConsole* console) {
    mapManager = new Map();
    map = mapManager->getMap();
    mapWidth = mapManager->getWidth();
    mapHeight = mapManager->getHeight();

    this->pConsole = console;

    this->input = new Input();

    this->pPhysics = new RaycastPhysics();
    this->pPhysics->setMap(this->mapManager);

    this->camera = new RaycastCamera(this->pPhysics->getCamera(), FOV);
    this->camera->setX(2.5f);
    this->camera->setY(9.5f);
    this->camera->setAngle(1.5708f);

    this->renderer = new RaycastRenderer(sw, sh);

    RaycastSprites* sprites = RaycastSprites::getInstance();
    sprites->addSprite(skeleton, SKELETON_WIDTH, SKELETON_HEIGHT);
    RaycastSprite* sprite = sprites->getSprite(2);
    sprite->addAnimation("walk", 0, 64, 32, 32, 10, true, 8);
    sprite->playAnimation(0);
}

bool Game::update(float dt) {
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

    this->pPhysics->update(dt);
    this->mapManager->update(dt);

    RaycastSprites::getInstance()->update(dt);

    this->renderer->render(this->camera, this->mapManager);

    return true;
}

void Game::dispose() {
    delete pPhysics;
    delete camera;
    delete renderer;
    delete mapManager;
    delete input;
}
