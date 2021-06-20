#ifndef __RENDERER_H__
#define __RENDERER_H__

#define MAX_ENTITIES 128

struct RaycastEntity {
    float x;
    float y;
    int spriteId;     // the id of the sprite associated with this item.
};

class Map;
class RaycastCamera;
class RaycastSprites;

class RaycastRenderer {
    private:
        short* sBuffer;     // buffer of pixels where each pixel is an 16-bit ABGR color. (1-bit Alpha, 5-bit RGB)
        int sBufferLen;     // the number of pixels in the buffer.
        int sBufferSize;    // the size of the buffer.
        int sWidth;         // the screen width.
        int sHeight;        // the screen height.
        int gWidth;         // the portion of the screen buffer the game is rendered into.
        int gHeight;        // the portion of the screen buffer the game is rendered into.

        int wallSprite;
        int doorSprite;
        RaycastSprites* pSpriteManager;

        RaycastEntity entities[MAX_ENTITIES];

        void putPixel(int x, int y, short pixel);
    public:
        RaycastRenderer(int width, int height);
        void render(RaycastCamera* camera, Map* map, float dt);

        short* getBuffer();
        int getBufferSize();
};

#endif //__RENDERER_H__
