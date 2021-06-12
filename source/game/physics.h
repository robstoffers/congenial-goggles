#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#define MAX_OBJECTS 20

struct PhysicsObject {
    float x;
    float y;
    float a;
    float aSpeed;
    float vX;
    float vY;
    float forwardX;
    float forwardY;
    float radius;
    bool active;

    float getRightX() { return -this->forwardY; }
    float getRightY() { return this->forwardX; }
};

class Map;

class RaycastPhysics {
    private:
        Map* map;
        PhysicsObject camera;
        PhysicsObject objects[MAX_OBJECTS];
    public:
        RaycastPhysics();               // Constructor
        void setMap(Map* map);          // Sets the currently loaded map for use with collision detection.
        PhysicsObject* getPhysics();    // Returns the next available inactive physics object for use.
        PhysicsObject* getCamera();
        void update(float dt);
};

#endif //__PHYSICS_H__
