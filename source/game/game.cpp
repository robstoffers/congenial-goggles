#include "game.h"
#include "map.h"
#include "input.h"

#include <string.h>
#include <math.h>

#include "textures/stone_wall.h"

#define BLACK 	0x8000
#define WHITE 	0xFFFF
#define RED 	0x801F
#define GREEN	0x83E0
#define BLUE	0xFC00

#define FOV     0.7853975
#define DRAW_DISTANCE 16.0f

#define WALK_SPEED 1.5f
#define TURN_SPEED 2.25f

void Game::putPixel(int x, int y, short pixel) {
    if (sBuffer != NULL) {
        sBuffer[y * sWidth + x] = pixel;
    }
}

void Game::init(int sw, int sh) {
    sWidth = sw;
    sHeight = sh;
    sBufferLen = sWidth * sHeight;
    sBufferSize = sBufferLen * 2;
    sBuffer = new short[sBufferLen];
    for(int i = 0; i < sBufferLen; i++){
		sBuffer[i] = 0xEEEE;
	}

    // how much of the screen buffer are we going to use for the game? on the DS
    // filling the whole screen is super slow so instead we'll use 1/4 of the screen.
    gWidth = sWidth / 2;
    gHeight = sHeight / 2;

    mapManager = new Map();
    map = mapManager->getMap();
    mapWidth = mapManager->getWidth();
    mapHeight = mapManager->getHeight();

    inputManager = new Input();

    playerA = 0;
    playerX = 1.5f;
    playerY = 1.5f;
}

bool Game::update(float dt) {
    playerDirX = cosf(playerA);
    playerDirY = sinf(playerA);
    playerRightX = -playerDirY;
    playerRightY = playerDirX;

    inputManager->update();
    if (inputManager->isStartPressed()) return false;
    if (inputManager->isLeftHeld()) rotateLeft(dt);
    if (inputManager->isRightHeld()) rotateRight(dt);
    if (inputManager->isUpHeld()) moveForward(dt);
    if (inputManager->isDownHeld()) moveBackward(dt);
    if (inputManager->isLHeld()) strafeLeft(dt);
    if (inputManager->isRHeld()) strafeRight(dt);

    float angleSweepStart = playerA - FOV / 2.0f;

    for (int x = 0; x < gWidth; x++) {
        float rayAngle = angleSweepStart + ((float)x / float(gWidth)) * FOV;

        float distanceToWall = 0.0f;
        bool hitWall = false;

        float rayDirX = cosf(rayAngle);
        float rayDirY = sinf(rayAngle);

        float sideDistX;
        float sideDistY;

        float rayUnitStepSizeX = sqrt(1 + (rayDirY / rayDirX) * (rayDirY / rayDirX));
        float rayUnitStepSizeY = sqrt(1 + (rayDirX / rayDirY) * (rayDirX / rayDirY));

        int stepX;
        int stepY;

        int mapX = (int)playerX;
        int mapY = (int)playerY;

        float sampleX = 0.0f;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (playerX - (float)mapX) * rayUnitStepSizeX;
        } else {
            stepX = 1;
            sideDistX = (float(mapX + 1) - playerX) * rayUnitStepSizeX;
        }
            
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (playerY - (float)mapY) * rayUnitStepSizeY;
        } else {
            stepY = 1;
            sideDistY = (float(mapY + 1) - playerY) * rayUnitStepSizeY;
        }

        while (!hitWall && distanceToWall < DRAW_DISTANCE) {
            if (sideDistX < sideDistY) {
                mapX += stepX;
                distanceToWall = sideDistX;
                sideDistX += rayUnitStepSizeX;
            } else {
                mapY += stepY;
                distanceToWall = sideDistY;
                sideDistY += rayUnitStepSizeY;					
            }

            if (mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight) {
                hitWall = true;
                distanceToWall = DRAW_DISTANCE;
            } else {
                if (map[mapY * mapWidth + mapX] == 1) {
                    hitWall = true;

                    float tileMidX = (float)mapX + 0.5f;
                    float tileMidY = (float)mapY + 0.5f;

                    float testPointX = playerX + rayDirX * distanceToWall;
                    float testPointY = playerY + rayDirY * distanceToWall;

                    float testAngle = atan2f((testPointY - tileMidY), (testPointX - tileMidX));
                    if (testAngle >= -3.14159f * 0.25f && testAngle < 3.14159f * 0.25f)
                        sampleX = testPointY - (float)mapY;
                    if (testAngle >= 3.14159f * 0.25f && testAngle < 3.14159f * 0.75f)
                        sampleX = testPointX - (float)mapX;
                    if (testAngle < -3.14159f * 0.25f && testAngle >= -3.14159f * 0.75f)
                        sampleX = testPointX - (float)mapX;
                    if (testAngle >= 3.14159f * 0.75f || testAngle < -3.14159f * 0.75f)
                        sampleX = testPointY - (float)mapY;
                }
            }
        }

        int ceiling = (float)(gHeight / 2.0f) - gHeight / distanceToWall;
        int floor = gHeight - ceiling;

        //float f = distanceToWall / drawDistance;
        //u16 shade = colorLerp16(0xFFFF, 0x8000, f);
        
        for (int y = 0; y < gHeight; y++) {
            if (y <= ceiling) {
                putPixel(x, y, BLACK);
            } else if (y > ceiling && y <= floor) {
                if (distanceToWall < DRAW_DISTANCE) {
                    float sampleY = ((float)y - (float)ceiling) / ((float)floor - (float)ceiling);
                    int sx = sampleX * 32.0f;
                    int sy = sampleY * 32.0f;

                    putPixel(x, y, stone_wall[sy * 32 + sx]);
                } else {
                    putPixel(x, y, BLACK);
                }
            } else {
                putPixel(x, y, 0x9CE7);
            }
        }
    }

    return true;
}

void Game::dispose() {
    delete mapManager;
    delete inputManager;
}

short* Game::getBuffer() { return sBuffer; }
int Game::getBufferSize() { return sBufferSize; }
int Game::getGameWidth() { return gWidth; }
int Game::getGameHeight() {return gHeight; }

void Game::move(float dirX, float dirY, float speed) {
	// store the position where the player will go but don't move it yet.
	float testX = playerX + dirX * speed;
	float testY = playerY + dirY * speed;

	// Only testing the X movement right now
	int tileX = (int)testX;
	int tileY = (int)playerY;
	if (map[tileY * mapWidth + tileX] == 1) {
		testX = playerX;
	}

	// Only testing the Y movement now
	tileX = (int)playerX;
	tileY = (int)testY;
	if (map[tileY * mapWidth + tileX] == 1) {
		testY = playerY;
	}

	// update the player position.
	playerX = testX;
	playerY = testY;
}
void Game::moveForward(float dt) {
    move(playerDirX, playerDirY, WALK_SPEED * dt);
}
void Game::moveBackward(float dt) {
    move(-playerDirX, -playerDirY, WALK_SPEED * dt);
}
void Game::strafeLeft(float dt) {
    move(-playerRightX, -playerRightY, WALK_SPEED * dt);
}
void Game::strafeRight(float dt) {
    move(playerRightX, playerRightY, WALK_SPEED * dt);
}
void Game::rotateLeft(float dt) {
    playerA -= TURN_SPEED * dt;
}
void Game::rotateRight(float dt) {
    playerA += TURN_SPEED * dt;
}
