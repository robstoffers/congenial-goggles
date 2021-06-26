#include "renderer.h"
#include "camera.h"
#include "map.h"
#include "sprites.h"
#include "door.h"
#include "pool.h"

#include "wall_stone.h"
#include "wall_door.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

#define BLACK 	0x8000
#define WHITE 	0xFFFF
#define RED 	0x801F
#define GREEN	0x83E0
#define BLUE	0xFC00
#define FLOOR   0x9CE7

#define COLOR_R(color) (color & 0x1F)
#define COLOR_G(color) (color & 0x3E0) >> 5
#define COLOR_B(color) (color & 0x7C00) >> 10
#define COLOR(a, r, g, b) (a << 15) | (b << 10) | (g << 5) | r

#define DRAW_DISTANCE 16.0f

#define PI 3.14159f

#define MAX_ENTITIES 10

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

RaycastRenderer::RaycastRenderer(int width, int height) {
    this->sWidth = width;
    this->sHeight = height;
    this->sBufferLen = this->sWidth * this->sHeight;
    this->sBufferSize = this->sBufferLen * 2;
    this->sBuffer = new short[this->sBufferLen];
    cls();

    // how much of the screen buffer are we going to use for the game? on the DS
    // filling the whole screen is super slow so instead we'll use 1/4 of the screen.
    this->gWidth = this->sWidth / 2;
    this->gHeight = this->sHeight / 2;

    // this->pSpriteManager = new RaycastSprites();
    this->wallSprite = RaycastSprites::getInstance()->addSprite(wall_stone, WALL_STONE_WIDTH, WALL_STONE_HEIGHT);
    this->doorSprite = RaycastSprites::getInstance()->addSprite(wall_door, WALL_DOOR_WIDTH, WALL_DOOR_HEIGHT);

    this->_entities = new Pool<RaycastEntity>(MAX_ENTITIES);
}

RaycastEntity* RaycastRenderer::addEntity() {
    if (_entities) {
        RaycastEntity* pEntity = _entities->next();
        if (pEntity) pEntity->reset();
        return pEntity;
    }
    return NULL;
}
void RaycastRenderer::removeEntity(RaycastEntity* entity) {
    if (_entities) {
        _entities->release(entity);
    }
}

void RaycastRenderer::cls() {
    memset(this->sBuffer, 0xEE, this->sBufferSize);
}

void RaycastRenderer::renderMap(RaycastCamera* camera, Map* map) {
    float cameraFOV = camera->getFOV();
    float cameraHalfFOV = cameraFOV * 0.5f;

    // Rendering the map
    float angleSweepStart = camera->getAngle() - cameraHalfFOV;
    for (int x = 0; x < this->gWidth; x++) {
        float rayAngle = angleSweepStart + ((float)x / float(this->gWidth)) * cameraFOV;

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

        int side;

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

        float u = 0.0f; // the U portion of the UV coordinates for texturing.
        char tile;      // the tile that was hit by the ray.      
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
                tile = map->getTile(mapX, mapY);
                if (tile == 1) {
                    hitWall = true;
                    if (side == 0)  u = camera->getY() + rayDirY * distanceToWall;
                    else            u = camera->getX() + rayDirX * distanceToWall;
                    u -= floor(u);

                    if (side == 0 && rayDirX < 0) u = 1 - u;
                    if (side == 1 && rayDirY > 0) u = 1 - u;
                }
                if (tile == 2) { // door
                    float testPointX = camera->getX() + rayDirX * (distanceToWall + 0.5f);
                    float testPointY = camera->getY() + rayDirY * (distanceToWall + 0.5f);

                    int doorX = (int)testPointX;
                    int doorY = (int)testPointY;
                    if (map->getTile(doorX, doorY) == 2) {
                        if (side == 0)  u = testPointY;
                        else            u = testPointX;
                        u = 1 - (u - floor(u));

                        Door* door = map->getDoor(doorX, doorY);
                        if (door && u > door->getTime()) {
                            hitWall = true;
                            distanceToWall += 0.5f;
                            u -= door->getTime();
                        }
                    }
                }
            }
        }

        int ceiling = (float)(this->gHeight / 2.0f) - this->gHeight / distanceToWall;
        int floor = this->gHeight - ceiling;

        //float f = distanceToWall / DRAW_DISTANCE;
        //short shade = colorLerp16(0xFFFF, 0x8000, f);

        RaycastSprite* pSprite = NULL;
        if (distanceToWall < DRAW_DISTANCE) {
            pSprite = RaycastSprites::getInstance()->getSprite(tile - 1);
        }
        
        for (int y = 0; y < this->gHeight; y++) {
            if (y <= ceiling) {
                putPixel(x, y, BLACK);
            } else if (y > ceiling && y <= floor) {
                if (distanceToWall < DRAW_DISTANCE) {
                    int wallHeight = floor - ceiling;
                    int ceilingDelta = y - ceiling;
                    float v = (float)ceilingDelta / (float)wallHeight;
                    if (pSprite) {
                        short pixel = pSprite->sample(u, v);
                        putPixel(x, y, pixel);
                    }
                    //putPixel(x, y, colorLerp16(BLACK, RED, u));
                } else {
                    putPixel(x, y, BLACK);
                }
                //putPixel(x, y, shade);
            } else {
                putPixel(x, y, FLOOR);
            }
        }
    }
}

void RaycastRenderer::renderEntities(RaycastCamera* camera) {
    float cameraFOV = camera->getFOV();
    float cameraHalfFOV = cameraFOV * 0.5f;

    // Render entities (enemies, debris, furniture, items on floor, etc.)
    if (_entities) {
        for (int i = 0; i < _entities->size(); i++) {
            if (_entities->isActive(i)) {
                RaycastEntity* pEntity = _entities->item(i);

                float toEntityX = pEntity->x - camera->getX();
                float toEntityY = pEntity->y - camera->getY();
                float distFromCamera = sqrtf(toEntityX * toEntityX + toEntityY * toEntityY);

                float cameraDirX = camera->getFacingX();
                float cameraDirY = camera->getFacingY();

                float entityAngle = (atan2f(cameraDirY, cameraDirX) - atan2f(toEntityY, toEntityX)) * -1.0f;
                if (entityAngle < -PI) {
                    entityAngle += 2.0f * PI;
                }
                if (entityAngle > PI) {
                    entityAngle -= 2.0f * PI;
                }

                bool inView = fabs(entityAngle) < cameraHalfFOV;

                if (inView && distFromCamera >= 1.0f && distFromCamera < DRAW_DISTANCE) {
                    float entityCeiling = (float)(this->gHeight / 2.0f) - this->gHeight / distFromCamera;
                    float entityFloor = this->gHeight - entityCeiling;
                    float entityHeight = (entityFloor - entityCeiling) * pEntity->scale;

                    RaycastSprite* pSprite = RaycastSprites::getInstance()->getSprite(pEntity->spriteId);
                    if (pSprite) {
                        int spriteWidth = pSprite->getWidth();
                        int spriteHeight = pSprite->getHeight();
                        float aspectRatio = spriteHeight / spriteWidth;
                        float entityWidth = entityHeight / aspectRatio;
                        float entityMiddle = (0.5f * (entityAngle / cameraHalfFOV) + 0.5f) * (float)gWidth;

                        for (int x = 0; x < entityWidth; x++) {
                            for (int y = 0; y < entityHeight; y++) {
                                float u = x / entityWidth;
                                float v = y / entityHeight;
                                int spriteX = (int)(entityMiddle + x - (entityWidth / 2.0f));

                                if (spriteX >= 0 && spriteX < gWidth) {
                                    short spritePixel = pSprite->sample(u, v);
                                    // //The first bit in the pixel is for transparency. If it is 0 then we won't draw it.
                                    if (spritePixel >> 15)
                                        putPixel(spriteX, entityFloor - entityHeight + y, spritePixel);
                                    //putPixel(spriteX, entityFloor - entityHeight + y, RED);
                                }
                            }
                        }
                    }
                    

                    //RaycastSprite* pSprite = this->pSpriteManager->getSprite(entities[i].spriteId);
                    //int spriteWidth = pSprite->getWidth();
                    //int spriteHeight = pSprite->getHeight();
                    //float aspectRatio = spriteHeight / spriteWidth;
                    //float entityWidth = entityHeight / fAspectRatio;
                    //float entityMiddle = (0.5f * (entityAngle / cameraHalfFOV) + 0.5f) * (float)gWidth;

                    //draw the sprite.
                    //for (int x = 0; x < spriteWidth; x++) {
                        //for (int y = 0; y < spriteHeight; y++) {
                            //float u = x / spriteWidth;
                            //float v = y / spriteHeight;
                            //int spriteColumn = (int)(entityMiddle + x - (spriteWidth / 2.0f));
                            //if (spriteColumn >= 0 && spriteColumn < gWidth) {
                                //short spritePixel = pSprite->sample(u, v);
                                // The first bit in the pixel is for transparency. If it is 0 then we won't draw it.
                                //if (spritePixel >> 15)
                                    //putPixel(spriteColumn, entityCeiling + y, spritePixel);
                            //}
                        //}
                    //}
                }
            }
        }
    }

    // for (int i = 0; i < MAX_ENTITIES; i++) {
    //     float toEntityX = entities[i].x - camera->getX();
    //     float toEntityY = entities[i].y - camera->getY();
    //     float distFromCamera = sqrtf(toEntityX * toEntityX + toEntityY * toEntityY);

    //     float cameraDirX = camera->getFacingX();
    //     float cameraDirY = camera->getFacingY();
    //     float entityAngle = atan2f(cameraDirY, cameraDirX) - atan2f(toEntityY, toEntityX);
    //     if (entityAngle < -PI) {
    //         entityAngle += 2.0f * PI;
    //     }
    //     if (entityAngle > PI) {
    //         entityAngle -= 2.0f * PI;
    //     }

    //     bool inView = fabs(entityAngle) < cameraHalfFOV;

    //     if (inView && distFromCamera >= 0.5f && distFromCamera < DRAW_DISTANCE) {
    //         float entityCeiling = (float)(this->gHeight / 2.0f) - this->gHeight / distFromCamera;
    //         float entityFloor = this->gHeight - entityCeiling;
    //         float entityHeight = entityFloor - entityCeiling;
            
    //         //RaycastSprite* pSprite = this->pSpriteManager->getSprite(entities[i].spriteId);
    //         //int spriteWidth = pSprite->getWidth();
    //         //int spriteHeight = pSprite->getHeight();
    //         //float aspectRatio = spriteHeight / spriteWidth;
    //         //float entityWidth = entityHeight / fAspectRatio;
    //         //float entityMiddle = (0.5f * (entityAngle / cameraHalfFOV) + 0.5f) * (float)gWidth;

    //         //draw the sprite.
    //         //for (int x = 0; x < spriteWidth; x++) {
    //             //for (int y = 0; y < spriteHeight; y++) {
    //                 //float u = x / spriteWidth;
    //                 //float v = y / spriteHeight;
    //                 //int spriteColumn = (int)(entityMiddle + x - (spriteWidth / 2.0f));
    //                 //if (spriteColumn >= 0 && spriteColumn < gWidth) {
    //                     //short spritePixel = pSprite->sample(u, v);
    //                     // The first bit in the pixel is for transparency. If it is 0 then we won't draw it.
    //                     //if (spritePixel >> 15)
    //                         //putPixel(spriteColumn, entityCeiling + y, spritePixel);
    //                 //}
    //             //}
    //         //}
    //     }
    // }
}

void RaycastRenderer::render(RaycastCamera* camera, Map* map) {
    cls();
    this->renderMap(camera, map);
    this->renderEntities(camera);

    // RaycastSprite* skeleton = RaycastSprites::getInstance()->getSprite(2);
    // if (skeleton) {
    //     int width = skeleton->getWidth();
    //     int height = skeleton->getHeight();
    //     for (int y = 0; y < height; y++) {
    //         for (int x = 0; x < width; x++) {
    //             float u = (float)x / (float)width;
    //             float v = (float)y / ((float)height - 1);
    //             short pixel = skeleton->sample(u, v);
    //             if (pixel >> 15) {
    //                 putPixel(x, y, pixel);
    //             }
    //         }
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
