#include "sprites.h"

#include <math.h>

RaycastSpriteAnimation::RaycastSpriteAnimation(const char* name, short x, short y, short width, short height, int numFrames, bool repeat, float fps) {
    this->name = name;
    this->frameX = x;
    this->frameY = y;
    this->frameWidth = width;
    this->frameHeight = height;
    this->numFrames = numFrames;
    this->repeat = repeat;
    this->fps = 1.0f / fps;
}
short RaycastSpriteAnimation::sample(const unsigned short* data, int fullWidth, float u, float v) {
    if (data) {
        u = u < 0.0f ? 0.0f : u > 1.0f ? 1.0f : u;
        v = v < 0.0f ? 0.0f : v > 1.0f ? 1.0f : v;
        int x = this->frameX + (int)(u * (float)this->frameWidth);
        int y = this->frameY + (int)(v * (float)(this->frameHeight - 1));
        return data[y * fullWidth + x];
    }
    return 0;
}
void RaycastSpriteAnimation::reset() {
    this->timer = 0;
    this->frameX = 0;
}
void RaycastSpriteAnimation::update(float dt) {
    this->timer += dt;
    if (this->timer > fps) {
        this->timer = 0;
        frameX += frameWidth;
        if (frameX / frameWidth >= numFrames) {
            frameX = repeat ? 0 : (frameWidth * numFrames) - frameWidth;
        }
    }
}

RaycastSprite::RaycastSprite(const unsigned short* data, int width, int height) {
    this->pData = data;
    this->width = width;
    this->height = height;
}

// Add an animation to the sprite.
// name:        The name of the animation.
// x:           The X pixel coordinate where the animation starts.
// y:           The Y pixel coordinate where the animation starts.
// width:       The width of each frame in the animation.
// height:      The height of each frame in the animation.
// numFrames:   The number of frames in the animation.
int RaycastSprite::addAnimation(const char* name, int x, int y, int width, int height, int numFrames, bool repeat, float fps) {
    for (int i = 0; i < 5; i++) {
        if (!animations[i]) {
            animations[i] = new RaycastSpriteAnimation(name, x, y, width, height, numFrames, repeat, fps);
            return i;
        }
    }
    return -1;
}
int RaycastSprite::addAnimation(const char* name, int x, int y, int width, int height, int numFrames, float fps) {
    return this->addAnimation(name, x, y, width, height, true, fps);
}
// Play an animation.
// id:    The ID of the animation to play.
void RaycastSprite::playAnimation(int id) {
    if (id >= 0 && id < 5 && animations[id]) {
        pCurAnimation = animations[id];
        pCurAnimation->reset();
    }
}

short RaycastSprite::sample(float u, float v) {
    if (this->pData) {
        if (this->pCurAnimation) {
            return this->pCurAnimation->sample(this->pData, this->width, u, v);
        }

        u = u < 0.0f ? 0.0f : u > 1.0f ? 1.0f : u;
        v = v < 0.0f ? 0.0f : v > 1.0f ? 1.0f : v;
        int x = u * (float)this->width;
        int y = v * (float)(this->height - 1);
        return this->pData[y * this->width + x];
    }
    return 0;
}

void RaycastSprite::update(float dt) {
    if (this->pCurAnimation) {
        this->pCurAnimation->update(dt);
    }
}

int RaycastSprite::getWidth() {
    if (pCurAnimation) {
        return pCurAnimation->getWidth();
    }
    return width;
}
int RaycastSprite::getHeight() {
    if (pCurAnimation) {
        return pCurAnimation->getHeight();
    }
    return height;
}

int RaycastSprites::addSprite(const unsigned short* sprite, int width, int height) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (!sprites[i]) {
            sprites[i] = new RaycastSprite(sprite, width, height);
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

void RaycastSprites::update(float dt) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (sprites[i]) {
            sprites[i]->update(dt);
        }
    }
}

// singleton stuff
RaycastSprites* RaycastSprites::instance = NULL;
RaycastSprites::RaycastSprites() { }
RaycastSprites* RaycastSprites::getInstance() {
    if (!instance) {
        instance = new RaycastSprites();
    }   return instance;
}