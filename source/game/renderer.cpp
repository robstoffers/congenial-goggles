#include "renderer.h"
#include "camera.h"
#include "map.h"
#include "door.h"

#include <math.h>

#include "wall_stone.h"
#include "wall_door.h"

#define BLACK 	0x8000
#define WHITE 	0xFFFF
#define RED 	0x801F
#define GREEN	0x83E0
#define BLUE	0xFC00

#define DRAW_DISTANCE 16.0f

short colorLerp16(short from, short to, float amount) {
	char aTo = (to & 0x8000) >> 15;
	char bTo = (to & 0x7C00) >> 10;
	char gTo = (to & 0x3E0) >> 5;
	char rTo = (to & 0x1F);

	//u8 aFrom = (from & 0x8000) >> 15;
	char bFrom = (from & 0x7C00) >> 10;
	char gFrom = (from & 0x3E0) >> 5;
	char rFrom = (from & 0x1F);

	char rFinal = char((rFrom * (1.0f - amount)) + (rTo * amount));
	char gFinal = char((gFrom * (1.0f - amount)) + (gTo * amount));
	char bFinal = char((bFrom * (1.0f - amount)) + (bTo * amount));

	return (aTo << 15) | (bFinal << 10) | (gFinal << 5) | rFinal;
}

RaycastRenderer::RaycastRenderer(int width, int height, float fov) {
    this->sWidth = width;
    this->sHeight = height;
    this->sBufferLen = this->sWidth * this->sHeight;
    this->sBufferSize = this->sBufferLen * 2;
    this->sBuffer = new short[this->sBufferLen];
    for(int i = 0; i < this->sBufferLen; i++){
		this->sBuffer[i] = 0xEEEE;
	}

    // how much of the screen buffer are we going to use for the game? on the DS
    // filling the whole screen is super slow so instead we'll use 1/4 of the screen.
    this->gWidth = this->sWidth / 2;
    this->gHeight = this->sHeight / 2;

    this->fov = fov;
}

void RaycastRenderer::render(RaycastCamera* camera, Map* map, float dt) {
    float angleSweepStart = camera->getAngle() - this->fov / 2.0f;

    for (int x = 0; x < this->gWidth; x++) {
        float rayAngle = angleSweepStart + ((float)x / float(this->gWidth)) * this->fov;

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

        int mapX = (int)camera->getX();
        int mapY = (int)camera->getY();

        float sampleX = 0.0f;
        int side;
        bool isDoor = false;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (camera->getX() - (float)mapX) * rayUnitStepSizeX;
        } else {
            stepX = 1;
            sideDistX = (float(mapX + 1) - camera->getX()) * rayUnitStepSizeX;
        }
            
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (camera->getY() - (float)mapY) * rayUnitStepSizeY;
        } else {
            stepY = 1;
            sideDistY = (float(mapY + 1) - camera->getY()) * rayUnitStepSizeY;
        }

        while (!hitWall && distanceToWall < DRAW_DISTANCE) {
            if (sideDistX < sideDistY) {
                mapX += stepX;
                distanceToWall = sideDistX;
                sideDistX += rayUnitStepSizeX;
                side = 0;
            } else {
                mapY += stepY;
                distanceToWall = sideDistY;
                sideDistY += rayUnitStepSizeY;
                side = 1;				
            }

            if (mapX < 0 || mapX >= map->getWidth() || mapY < 0 || mapY >= map->getHeight()) {
                hitWall = true;
                distanceToWall = DRAW_DISTANCE;
            } else {
                char tile = map->getTile(mapX, mapY);
                if (tile == 1) {
                    hitWall = true;
                    if (side == 0)  sampleX = camera->getY() + rayDirY * distanceToWall;
                    else            sampleX = camera->getX() + rayDirX * distanceToWall;
                    sampleX -= floor(sampleX);
                }
                if (tile == 2) { // door
                    float testPointX = camera->getX() + rayDirX * (distanceToWall + 0.5f);
                    float testPointY = camera->getY() + rayDirY * (distanceToWall + 0.5f);

                    int doorX = (int)testPointX;
                    int doorY = (int)testPointY;
                    if (map->getTile(doorX, doorY) == 2) {
                        distanceToWall += 0.5f;
                        hitWall = true;

                        // figure out the texture X uv coordinate. it will be a 0 - 1 value so we can use it later
                        // for figuring out how much of the door is open.
                        if (side == 0)  sampleX = camera->getY() + rayDirY * distanceToWall;
                        else            sampleX = camera->getX() + rayDirX * distanceToWall;
                        sampleX -= floor(sampleX);

                        Door* door = map->getDoor(doorX, doorY);
                        if (door) {
                            if (sampleX > door->getTime()) {
                                hitWall = false;
                                distanceToWall -= 0.5f;
                            } else {
                                sampleX -= door->getTime();
                                isDoor = true;
                            }
                        }
                    }
                }
            }
        }

        int ceiling = (float)(this->gHeight / 2.0f) - this->gHeight / distanceToWall;
        int floor = this->gHeight - ceiling;

        //float f = distanceToWall / DRAW_DISTANCE;
        //short shade = colorLerp16(0xFFFF, 0x8000, f);
        
        for (int y = 0; y < this->gHeight; y++) {
            if (y <= ceiling) {
                putPixel(x, y, BLACK);
            } else if (y > ceiling && y <= floor) {
                if (distanceToWall < DRAW_DISTANCE) {
                    float sampleY = ((float)y - (float)ceiling) / ((float)floor - (float)ceiling);
                    int sx = sampleX * 32.0f;
                    int sy = sampleY * 32.0f;

                    if (isDoor) {
                        putPixel(x, y, wall_door[sy * 32 + sx]);
                    } else {
                        putPixel(x, y, wall_stone[sy * 32 + sx]);
                    }
                } else {
                    putPixel(x, y, BLACK);
                }
                //putPixel(x, y, shade);
            } else {
                putPixel(x, y, 0x9CE7);
            }
        }
    }

    // for (int y = 0; y < this->gHeight; y++) {
    //     for (int x = 0; x < this->gWidth; x++) {
    //         float sampleY = (float)y / (float)this->gHeight;
    //         float sampleX = (float)x / (float)this->gWidth;

    //         int sx = sampleX * 32.0f;
    //         int sy = sampleY * 32.0f;
    //         putPixel(x, y, door[sy * 32 + sx]);
    //     }
    // }
}

void RaycastRenderer::putPixel(int x, int y, short pixel) {
    if (this->sBuffer != NULL) {
        this->sBuffer[y * this->sWidth + x] = pixel;
    }
}

short* RaycastRenderer::getBuffer() {
    return this->sBuffer;
}
int RaycastRenderer::getBufferSize() {
    return this->sBufferSize;
}
