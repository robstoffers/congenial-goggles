#include "camera.h"
#include "physics.h"

#include <math.h>

RaycastCamera::RaycastCamera(PhysicsObject* pPhysics, float fov) {
    this->pPhysics = pPhysics;
    this->fov = fov;
}

float RaycastCamera::getX() {
    return pPhysics->getX();
}
float RaycastCamera::getY() {
    return pPhysics->getY();
}
float RaycastCamera::getAngle() {
    return pPhysics->getRotation();
}
float RaycastCamera::getFacingX() {
    return pPhysics->getForwardX();
}
float RaycastCamera::getFacingY() {
    return pPhysics->getForwardY();
}
float RaycastCamera::getRightX() { 
    return pPhysics->getRightX();
}
float RaycastCamera::getRightY() {
    return pPhysics->getRightY();
}
float RaycastCamera::getFOV() {
    return fov;
}

void RaycastCamera::setPosition(float x, float y) {
    pPhysics->setPosition(x, y);
}
void RaycastCamera::setAngle(float a) {
    pPhysics->setRotation(a);
}
void RaycastCamera::setForwardSpeed(float speed) {
    float velocityX = pPhysics->getForwardX() * speed;
    float velocityY = pPhysics->getForwardY() * speed;
    pPhysics->setVelocity(velocityX, velocityY);
}
void RaycastCamera::setStrafeSpeed(float speed) {
    float velocityX = this->pPhysics->getRightX() * speed;
    float velocityY =  this->pPhysics->getRightY() * speed;
    pPhysics->setVelocity(velocityX, velocityY);
}
void RaycastCamera::setTurnSpeed(float speed) {
    pPhysics->setRotationVelocity(speed);
}
void RaycastCamera::setFOV(float fov) {
    this->fov = fov;
}
