#include "camera.h"
#include "physics.h"

#include <math.h>

RaycastCamera::RaycastCamera(PhysicsObject* physics) {
    this->physics = physics;
}

float RaycastCamera::getX() {
    return this->physics->x;
}
float RaycastCamera::getY() {
    return this->physics->y;
}
float RaycastCamera::getAngle() {
    return this->physics->a;
}
float RaycastCamera::getFacingX() {
    return this->physics->forwardX;
}
float RaycastCamera::getFacingY() {
    return this->physics->forwardY;
}
float RaycastCamera::getRightX() { 
    return this->physics->getRightX();
}
float RaycastCamera::getRightY() {
    return this->physics->getRightY();
}

void RaycastCamera::setX(float x) { 
    this->physics->x = x;
}
void RaycastCamera::setY(float y) {
    this->physics->y = y;
}
void RaycastCamera::setAngle(float a) {
    this->physics->a = a;
}
void RaycastCamera::setForwardSpeed(float speed) { 
    this->physics->vX += this->physics->forwardX * speed;
    this->physics->vY += this->physics->forwardY * speed;
}
void RaycastCamera::setStrafeSpeed(float speed) {
    this->physics->vX += this->physics->getRightX() * speed;
    this->physics->vY += this->physics->getRightY() * speed;
}
void RaycastCamera::setTurnSpeed(float speed) {
    this->physics->aSpeed = speed;
}
