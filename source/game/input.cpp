#include "input.h"

#include <nds.h>

void Input::update() {
    scanKeys();
}
bool Input::isAPressed() {
    return keysDown() & KEY_A;
}
bool Input::isBPressed() {
    return keysDown() & KEY_B;
}
bool Input::isXPressed() {
    return keysDown() & KEY_X;
}
bool Input::isYPressed() {
    return keysDown() & KEY_Y;
}
bool Input::isLeftPressed() {
    return keysDown() & KEY_LEFT;
}
bool Input::isRightPressed() {
    return keysDown() & KEY_RIGHT;
}
bool Input::isUpPressed() {
    return keysDown() & KEY_UP;
}
bool Input::isDownPressed() {
    return keysDown() & KEY_DOWN;
}
bool Input::isLPressed() {
    return keysDown() & KEY_L;
}
bool Input::isRPressed() {
    return keysDown() & KEY_R;
}
bool Input::isStartPressed() {
    return keysDown() & KEY_START;
}

bool Input::isAHeld() {
    return keysHeld() & KEY_A;
}
bool Input::isBHeld() {
    return keysHeld() & KEY_B;
}
bool Input::isXHeld() {
    return keysHeld() & KEY_X;
}
bool Input::isYHeld() {
    return keysHeld() & KEY_Y;
}
bool Input::isLeftHeld() {
    return keysHeld() & KEY_LEFT;
}
bool Input::isRightHeld() {
    return keysHeld() & KEY_RIGHT;
}
bool Input::isUpHeld() {
    return keysHeld() & KEY_UP;
}
bool Input::isDownHeld() {
    return keysHeld() & KEY_DOWN;
}
bool Input::isLHeld() {
    return keysHeld() & KEY_L;
}
bool Input::isRHeld() {
    return keysHeld() & KEY_R;
}
bool Input::isStartHeld() {
    return keysHeld() & KEY_START;
}