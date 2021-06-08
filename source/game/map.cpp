#include "map.h"
#include "maps/test.h"

const unsigned char* Map::getMap() {
    return test_map;
}
int Map::getWidth() {
    return TEST_MAP_WIDTH;
}
int Map::getHeight() {
    return TEST_MAP_HEIGHT;
}