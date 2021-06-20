#include "sprites.h"

#include <math.h>

RaycastSprite::RaycastSprite(const unsigned short* sprite, int width, int height, int offset) {
    this->pSprite = sprite;
    this->width = width;
    this->height = height;
    this->offset = offset;
}
short RaycastSprite::sample(float u, float v) {
    if (this->pSprite) {
        const unsigned short* start = &this->pSprite[this->offset];
        int x = abs(u * this->width);
        int y = abs(v * this->height - 1.0f);
        // x = x < this->width - 1 ? x : this->width - 1;
        // y = y < this->height - 1 ? y : this->height - 1;
        return start[y * this->width + x];
    }
    return 0;
}

int RaycastSprites::addSprite(const unsigned short* sprite, int width, int height, int offset) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (!sprites[i]) {
            sprites[i] = new RaycastSprite(sprite, width, height, offset);
            return i;
        }
    }
    return -1;
}

RaycastSprite* RaycastSprites::getSprite(int id) {
    if (id >= 0 && id < MAX_SPRITES) {
        return sprites[id];
    }
    return nullptr;
}