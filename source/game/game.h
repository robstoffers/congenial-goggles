#ifndef __GAME_H__
#define __GAME_H__

class Map;
class Input;

class Game {
    private:
        short* sBuffer;     // buffer of pixels where each pixel is an 16-bit ABGR color. (1-bit Alpha, 5-bit RGB)
        int sBufferLen;     // the number of pixels in the buffer.
        int sBufferSize;    // the size of the buffer.
        int sWidth;         // the screen width.
        int sHeight;        // the screen height.
        int gWidth;         // the portion of the screen buffer the game is rendered into.
        int gHeight;        // the portion of the screen buffer the game is rendered into.

        float playerA;      // the players angle.
        float playerX;      // the players position within the map.
        float playerY;      // the players position within the map.
        float playerDirX;
        float playerDirY;
        float playerRightX;
        float playerRightY;
        const unsigned char* map;          // the current map we're rendering.
        int mapWidth;
        int mapHeight;

        Map* mapManager;    // the map manager.
        Input* inputManager;

        void putPixel(int x, int y, short pixel);
        void move(float dirX, float dirY, float speed);
    public:
        void init(int sw, int sh);
        void dispose();
        bool update(float dt);

        short* getBuffer();
        int getBufferSize();
        int getGameWidth();
        int getGameHeight();

        void moveForward(float dt);
        void moveBackward(float dt);
        void strafeLeft(float dt);
        void strafeRight(float dt);
        void rotateLeft(float dt);
        void rotateRight(float dt);
};

#endif //__GAME_H__