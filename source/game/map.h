#ifndef __MAP_H__
#define __MAP_H__

#define MAX_DOORS 32

class Door;

struct TileInfo {
    int x;
    int y;
    float dX; // the position along the tile the ray is hitting (0 - 1)
    char id;
    float distance;
};

class Map {
    private:
        Door* doors;
        int* doorLookup;
    public:
        Map();
        ~Map();

        const unsigned char* getMap();
        int getWidth();
        int getHeight();
        char getTile(int x, int y);
        Door* getDoor(int x, int y);
        bool isDoorOpen(int x, int y);

        // Get the tile intersecting with the provided location along the provided ray.
        TileInfo getTileDDA(float originX, float originY, float rayX, float rayY);

        void update(float dt);
};

#endif //__MAP_H__
