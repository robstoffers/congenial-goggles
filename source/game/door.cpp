#include "door.h"

Door::Door() {
    this->time = DOOR_CLOSED;
    this->state = DOOR_CLOSED;
}

float Door::getTime() { return this->time; }
char Door::getState() { return this->state; }

void Door::toggle() {
    if (this->state == DOOR_OPEN || this->state == DOOR_OPENING) {
        this->state = DOOR_CLOSING;
    } else if (this->state == DOOR_CLOSED || this->state == DOOR_CLOSING) {
        this->state = DOOR_OPENING;
    }
}

void Door::update(float dt) {
    if (this->state == DOOR_OPENING) {
        this->time += DOOR_SPEED * dt;
        if (this->time >= 0.9f) {
            this->time = 0.9f;
            this->state = DOOR_OPEN;
        }
    } else if (this->state == DOOR_CLOSING) {
        this->time -= DOOR_SPEED * dt;
        if (this->time <= 0.0f) {
            this->time = 0.0f;
            this->state = DOOR_CLOSED;
        }
    }
}

