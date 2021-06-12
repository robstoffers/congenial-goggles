#include "physics.h"
#include "map.h"

#include <math.h>

RaycastPhysics::RaycastPhysics() {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        this->objects[i] = PhysicsObject();
        this->objects[i].active = false;
    }
    this->camera = PhysicsObject();
    this->camera.vX = 0.0f;
    this->camera.vY = 0.0f;
    this->map = nullptr;
}

PhysicsObject* RaycastPhysics::getPhysics() {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (!this->objects[i].active) {
            return &this->objects[i];
        }
    }
    return nullptr;
}

PhysicsObject* RaycastPhysics::getCamera() {
    return &this->camera;
}

void RaycastPhysics::setMap(Map* map) {
    this->map = map;
}

void RaycastPhysics::update(float dt) {
    this->camera.a += (this->camera.aSpeed * dt);
    this->camera.forwardX = cosf(this->camera.a);
    this->camera.forwardY = sinf(this->camera.a);

    float tX = this->camera.x + (this->camera.vX * dt);
    float tY = this->camera.y + (this->camera.vY * dt);

    // test collision with the map
    if (this->map) {
        const unsigned char* mapTiles = this->map->getMap();
        if (mapTiles) {
            int mapWidth = this->map->getWidth();
            int tileX = (int)tX;
            int tileY = (int)this->camera.y;
            if (mapTiles[tileY * mapWidth + tileX] == 1) {
                tX = this->camera.x;
            }

            tileX = (int)this->camera.x;
	        tileY = (int)tY;
            if (mapTiles[tileY * mapWidth + tileX] == 1) {
                tY = this->camera.y;
            }
        }
    }

    // TODO: Move other physics objects and test camera collision against them.

    // after everything is done, set the camera position.
    this->camera.x = tX;
    this->camera.y = tY;

    // reset velocity
    this->camera.vX = 0;
    this->camera.vY = 0;
    this->camera.aSpeed = 0;
}
