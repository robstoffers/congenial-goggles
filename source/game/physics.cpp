#include "physics.h"
#include "map.h"
#include "pool.h"

#include <math.h>

PhysicsObject::PhysicsObject() { }

void PhysicsObject::addVelocity(float x, float y) {
    this->vX += x;
    this->vY += y;
}
void PhysicsObject::setVelocity(float x, float y) {
    this->vX = x;
    this->vY = y;
}
void PhysicsObject::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
}
void PhysicsObject::setRotation(float a) {
    this->a = a;
}
void PhysicsObject::setRotationVelocity(float v) {
    this->aSpeed = v;
}
void PhysicsObject::rotate(float a) {
    this->a += a;
}
void PhysicsObject::reset(float x, float y, float a) {
    this->x = x;
    this->y = y;
    this->a = a;
}
void PhysicsObject::update(float dt) {
    a += (aSpeed * dt);
    forwardX = cosf(a);
    forwardY = sinf(a);
    x += (vX * dt);
    y += (vY * dt);

    vX = 0;
    vY = 0;
    aSpeed = 0;
}

RaycastPhysicsManager::RaycastPhysicsManager() {    
    this->map = NULL;
    _pool = new Pool<PhysicsObject>(20);
}
RaycastPhysicsManager::~RaycastPhysicsManager() {
    delete _pool;
}

PhysicsObject* RaycastPhysicsManager::getPhysics() {
    PhysicsObject* item = _pool->next();
    item->reset(0, 0, 0);
    return item;
}

void RaycastPhysicsManager::releasePhysics(PhysicsObject* object) {
    _pool->release(object);
}

void RaycastPhysicsManager::setMap(Map* map) {
    this->map = map;
}

void RaycastPhysicsManager::update(float dt) {
    for (int i = 0; i < _pool->size(); i++) {
        if (_pool->isActive(i)) {
            PhysicsObject* item = _pool->item(i);

            float prevX = item->getX();
            float prevY = item->getY();
            item->update(dt);

            // test collision with the map
            if (this->map) {
                if (this->map->getMap()) {
                    int tileX = (int)item->getX();
                    int tileY = (int)prevY;
                    char tile = this->map->getTile(tileX, tileY);
                    if (tile == 1) {
                        item->setX(prevX);
                    } else if (tile == 2) { // door
                        if (!map->isDoorOpen(tileX, tileY)) {
                            item->setX(prevX);
                        }
                    }

                    tileX = (int)prevX;
                    tileY = (int)item->getY();
                    tile = this->map->getTile(tileX, tileY);
                    if (tile == 1) {
                        item->setY(prevY);
                    } else if (tile == 2) { // door
                        if (!map->isDoorOpen(tileX, tileY)) {
                            item->setY(prevY);
                        }
                    }
                }
            }
        }
    }
}
