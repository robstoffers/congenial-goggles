#include "map.h"
#include "maps/test.h"

#include "door.h"
#include <math.h>

Map::Map() {
    this->doors = new Door[MAX_DOORS];
    this->doorLookup = new int[MAX_DOORS];
    for (int i = 0; i < MAX_DOORS; i++) {
        this->doorLookup[i] = -1;
    }

    // TODO: remove when actually generating a map
    int doorCounter = 0;
    int testMapSize = TEST_MAP_WIDTH * TEST_MAP_HEIGHT;
    for (int i = 0; i < testMapSize; i++) {
        if (test_map[i] == 2) {
            this->doorLookup[doorCounter] = i;
            doorCounter++;
        }
    }
}
Map::~Map() {
    delete this->doors;
}

const unsigned char* Map::getMap() {
    return test_map;
}
int Map::getWidth() {
    return TEST_MAP_WIDTH;
}
int Map::getHeight() {
    return TEST_MAP_HEIGHT;
}
char Map::getTile(int x, int y) {
    return this->getMap()[y * this->getWidth() + x];
}
TileInfo Map::getTileDDA(float originX, float originY, float rayX, float rayY) {
    int stepX;
    int stepY;

    float rayUnitStepSizeX = sqrt(1 + (rayY / rayX) * (rayY / rayX));
    float rayUnitStepSizeY = sqrt(1 + (rayX / rayY) * (rayX / rayY));   

    int mapX = (int)originX;
    int mapY = (int)originY;

    float sideDistX;
    float sideDistY;

    if (rayX < 0) {
        stepX = -1;
        sideDistX = (originX - (float)mapX) * rayUnitStepSizeX;
    } else {
        stepX = 1;
        sideDistX = (float(mapX + 1) - originX) * rayUnitStepSizeX;
    }
        
    if (rayY < 0) {
        stepY = -1;
        sideDistY = (originY - (float)mapY) * rayUnitStepSizeY;
    } else {
        stepY = 1;
        sideDistY = (float(mapY + 1) - originY) * rayUnitStepSizeY;
    }

    int side;
    bool hitWall = false;
    float distanceToWall = 0.0f;

    while (!hitWall && distanceToWall < 16.0f) {
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

        if (mapX < 0 || mapX >= getWidth() || mapY < 0 || mapY >= getHeight()) {
            TileInfo info;
            info.x = mapX;
            info.y = mapY;
            info.id = 1;
            info.distance = 16.0f;
            return info;
        } else {
            char tile = getTile(mapX, mapY);
            if (tile != 0) {
                float dX = 0.0f;
                if (side == 0)  dX = originY + rayY * distanceToWall;
                else            dX = originX + rayX * distanceToWall;
                dX -= floor(dX);
                
                TileInfo info;
                info.x = mapX;
                info.y = mapY;
                info.dX = dX;
                info.id = tile;
                info.distance = distanceToWall;
                return info;
            }
        }
    }
    TileInfo info;
    info.distance = 16;
    info.id = 1;
    return info;
}
Door* Map::getDoor(int x, int y) {
    int tile = y * this->getWidth() + x;
    for (int i = 0; i < MAX_DOORS; i++) {
        if (this->doorLookup[i] == tile) {
            return &this->doors[i];
        }
    }
    return nullptr;
}
bool Map::isDoorOpen(int x, int y) {
    Door* door = getDoor(x, y);
    if (door != nullptr) {
        return door->getState() == DOOR_OPEN;
    }
    return false;
}

void Map::update(float dt) {
    for (int i = 0; i < MAX_DOORS; i++) {
        if (this->doorLookup[i] == -1) {
            continue;
        }

        this->doors[i].update(dt);
    }
}