#ifndef __SPRITES_H__
#define __SPRITES_H__

#define MAX_SPRITES 5

class RaycastSprite {
    private:
        const unsigned short* pSprite;
        int width;
        int height;
        int offset;
    public:
        RaycastSprite(const unsigned short* sprite, int width, int height, int offset);
        short sample(float u, float v);
        int getWidth() { return width; }
        int getHeight() { return height; }
};

class RaycastSprites {
    private:
        RaycastSprite* sprites[MAX_SPRITES];
    public:
        // Add a sprite to the manager
        // sprite:  The sprite data.
        // width:   The sprite width.
        // height:  The sprite height.
        // offset:  The offset into the sprite data where the sprite starts
        int addSprite(const unsigned short* sprite, int width, int height, int offset);

        RaycastSprite* getSprite(int id);
};

#endif //__SPRITES_H__
