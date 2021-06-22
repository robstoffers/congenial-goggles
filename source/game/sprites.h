#ifndef __SPRITES_H__
#define __SPRITES_H__

#define MAX_SPRITES 5

class RaycastSpriteAnimation {
    private:
        const char* name;
        short frameX;
        short frameY;
        short frameWidth;
        short frameHeight;
        float timer;
        float fps;
        char numFrames;
        bool repeat;
    public:
        RaycastSpriteAnimation(const char* name, short x, short y, short width, short height, int numFrames, bool repeat, float fps);
        short sample(const unsigned short* data, int fullWidth, float u, float v);
        void reset();

        void update(float dt);

        short getWidth() { return frameWidth; }
        short getHeight() { return frameHeight; }
};

class RaycastSprite {
    private:
        const unsigned short* pData;    // The data for the sprite
        int width;                      // The total width of the sprite
        int height;                     // The total height of the sprite

        RaycastSpriteAnimation* animations[5];  // List of animations associated with this sprite, if any.
        RaycastSpriteAnimation* pCurAnimation;  // The current animation playing, if any.
    public:
        RaycastSprite(const unsigned short* data, int width, int height);
        // Add an animation to the sprite.
        // name:        The name of the animation.
        // x:           The X pixel coordinate where the animation starts.
        // y:           The Y pixel coordinate where the animation starts.
        // width:       The width of each frame in the animation.
        // height:      The height of each frame in the animation.
        // numFrames:   The number of frames in the animation.
        int addAnimation(const char* name, int x, int y, int width, int height, int numFrames, bool repeat, float fps);
        int addAnimation(const char* name, int x, int y, int width, int height, int numFrames, float fps);
        // Play an animation.
        // name:    The name of the animation.
        void playAnimation(int id);

        void update(float dt);

        short sample(float u, float v);
        int getWidth();
        int getHeight();
};

class RaycastSprites {
    private:
        RaycastSprite* sprites[MAX_SPRITES];

        static RaycastSprites* instance;
        RaycastSprites();
    public:
        static RaycastSprites* getInstance();

        // Add a sprite to the manager
        // sprite:  The sprite data.
        // width:   The sprite width.
        // height:  The sprite height.
        // offset:  The offset into the sprite data where the sprite starts
        int addSprite(const unsigned short* sprite, int width, int height);
        RaycastSprite* getSprite(int id);

        void update(float dt);
};

#endif //__SPRITES_H__
