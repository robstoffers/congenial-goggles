#ifndef __GAME_H__
#define __GAME_H__

class Map;
class Input;
class RaycastRenderer;
class RaycastCamera;
class RaycastPhysics;
class PrintConsole;

class Game {
    private:
        const unsigned char* map;          // the current map we're rendering.
        int mapWidth;
        int mapHeight;

        Map* mapManager;    // the map manager.
        Input* input;
        RaycastRenderer* renderer;
        RaycastCamera* camera;
        RaycastPhysics* physics;
    public:
        void init(int sw, int sh);
        void dispose();
        bool update(float dt, PrintConsole* console);

        short* getBuffer();
        int getBufferSize();
};

#endif //__GAME_H__