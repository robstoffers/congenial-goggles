#ifndef __RENDERER_H__
#define __RENDERER_H__

class Map;
class RaycastCamera;

class RaycastRenderer {
    private:
        short* sBuffer;     // buffer of pixels where each pixel is an 16-bit ABGR color. (1-bit Alpha, 5-bit RGB)
        int sBufferLen;     // the number of pixels in the buffer.
        int sBufferSize;    // the size of the buffer.
        int sWidth;         // the screen width.
        int sHeight;        // the screen height.
        int gWidth;         // the portion of the screen buffer the game is rendered into.
        int gHeight;        // the portion of the screen buffer the game is rendered into.
        float fov;

        void putPixel(int x, int y, short pixel);
    public:
        RaycastRenderer(int width, int height, float fov);
        void render(RaycastCamera* camera, Map* map);

        short* getBuffer();
        int getBufferSize();
};

#endif //__RENDERER_H__
