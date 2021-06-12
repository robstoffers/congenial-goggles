#ifndef __CAMERA_H__
#define __CAMERA_H__

class PhysicsObject;

class RaycastCamera {
    private:
        PhysicsObject* physics;    // the camera physics object.
    public:
        RaycastCamera(PhysicsObject* physics);

        float getX();       // Returns the camera x-coordinate.
        float getY();       // Returns the camera y-coordinate.
        float getAngle();   // Returns the current angle of the camera.
        float getFacingX(); // Returns the X component of the camera facing vector.
        float getFacingY(); // Returns the Y component of the camera facing vector.
        float getRightX();  // Returns the X component of the camera right vector.
        float getRightY();  // Returns the Y component of the camera right vector.

        void setX(float x);                 // Set the camera x-coordinate.
        void setY(float y);                 // Set the camera y-coordinate.
        void setAngle(float a);             // Set the camera angle.
        void setForwardSpeed(float speed);  // Set the camera movement speed.
        void setStrafeSpeed(float speed);   // Set the camera strafe speed.
        void setTurnSpeed(float speed);     // Set the camera turn speed.
};

#endif //__CAMERA_H__
