#include "camera.h"
#include "physics.h"

#include <math.h>

RaycastCamera::RaycastCamera(PhysicsObject* pPhysics, float fov) {
    this->pPhysics = pPhysics;
    this->fov = fov;
}

float RaycastCamera::getX() {
    return this->pPhysics->x;
}
float RaycastCamera::getY() {
    return this->pPhysics->y;
}
float RaycastCamera::getAngle() {
    return this->pPhysics->a;
}
float RaycastCamera::getFacingX() {
    return this->pPhysics->forwardX;
}
float RaycastCamera::getFacingY() {
    return this->pPhysics->forwardY;
}
float RaycastCamera::getRightX() { 
    return this->pPhysics->getRightX();
}
float RaycastCamera::getRightY() {
    return this->pPhysics->getRightY();
}
float RaycastCamera::getFOV() {
    return this->fov;
}

void RaycastCamera::setX(float x) { 
    this->pPhysics->x = x;
}
void RaycastCamera::setY(float y) {
    this->pPhysics->y = y;
}
void RaycastCamera::setAngle(float a) {
    this->pPhysics->a = a;
}
void RaycastCamera::setForwardSpeed(float speed) { 
    this->pPhysics->vX += this->pPhysics->forwardX * speed;
    this->pPhysics->vY += this->pPhysics->forwardY * speed;
}
void RaycastCamera::setStrafeSpeed(float speed) {
    this->pPhysics->vX += this->pPhysics->getRightX() * speed;
    this->pPhysics->vY += this->pPhysics->getRightY() * speed;
}
void RaycastCamera::setTurnSpeed(float speed) {
    this->pPhysics->aSpeed = speed;
}
void RaycastCamera::setFOV(float fov) {
    this->fov = fov;
}
