#ifndef __RENDERER_H__
#define __RENDERER_H__

struct RaycastEntity {
    float x;
    float y;
    float scale;
    int spriteId;     // the id of the sprite associated with this item.
    void reset() {
        scale = 1.0f;
        x = 0.0f;
        y = 0.0f;
    }
};

class Map;
class RaycastCamera;
class RaycastSprites;

template <typename T>
class Pool;

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

        Pool<RaycastEntity>* _entities;

        void cls();
        void putPixel(int x, int y, short pixel);
        void renderMap(RaycastCamera* camera, Map* map);
        void renderEntities(RaycastCamera* camera);
    public:
        RaycastRenderer(int width, int height);
        void render(RaycastCamera* camera, Map* map);

        RaycastEntity* addEntity();
        void removeEntity(RaycastEntity* entity);

        short* getBuffer();
        int getBufferSize();
};

#endif //__RENDERER_H__
