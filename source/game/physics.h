#ifndef __PHYSICS_H__
#define __PHYSICS_H__

class PhysicsObject {
    private:
        friend class RaycastPhysicsManager;

        float x;
        float y;
        float a;
        float aSpeed;
        float vX;
        float vY;
        float forwardX;
        float forwardY;
        float radius;

        // Reset the physics object.
        // x: The X position of the object.
        // y: The Y position of the object.
        // a: The angle of the object.
        void reset(float x, float y, float a);

        // Update the physics object.
        // dt: How much time has elapsed since the last update.
        void update(float dt);
    public:
        // Constructor.
        PhysicsObject();

        // Set the velocity of this physics object.
        // x: The X-component of the velocity.
        // y: The Y-component of the velocity.
        void setVelocity(float x, float y);

        // Set the position of the object.
        // x: The X position.
        // y: The y position.
        void setPosition(float x, float y);

        // Set the angle of the object.
        // a: The angle.
        void setRotation(float a);

        // Sets the rotation velocity of the object.
        // v: The rotation velocity.
        void setRotationVelocity(float v);

        // Rotate the object by an amount.
        // a: The amount to rotate by.
        void rotate(float a);

        void setX(float x) { this->x = x; }
        void setY(float y) { this->y = y; }

        float getX() { return this->x; }
        float getY() { return this->y; }
        float getRotation() { return this->a; }
        float getForwardX() { return this->forwardX; }
        float getForwardY() { return this->forwardY; }
        float getRightX() { return -this->forwardY; }
        float getRightY() { return this->forwardX; }
        float getVelocityX() { return this->vX; }
        float getVelocityY() { return this->vY; }
};

class Map;

template <typename T>
class Pool;

class RaycastPhysicsManager {
    private:
        Map* map;
        Pool<PhysicsObject>* _pool;
    public:
        // Constructor
        RaycastPhysicsManager();
        ~RaycastPhysicsManager();

        void setMap(Map* map);          // Sets the currently loaded map for use with collision detection.
        
        // Returns the next available inactive physics object for use.
        PhysicsObject* getPhysics();

        void releasePhysics(PhysicsObject* object);

        void update(float dt);
};

#endif //__PHYSICS_H__
