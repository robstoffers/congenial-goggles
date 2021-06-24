#ifndef __CAMERA_H__
#define __CAMERA_H__

class PhysicsObject;

class RaycastCamera {
    private:
        float fov;
        PhysicsObject* pPhysics;    // the camera physics object.
    public:
        RaycastCamera(PhysicsObject* physics, float fov);

        float getX();       // Returns the camera x-coordinate.
        float getY();       // Returns the camera y-coordinate.
        float getAngle();   // Returns the current angle of the camera.
        float getFacingX(); // Returns the X component of the camera facing vector.
        float getFacingY(); // Returns the Y component of the camera facing vector.
        float getRightX();  // Returns the X component of the camera right vector.
        float getRightY();  // Returns the Y component of the camera right vector.
        float getFOV();

        void setPosition(float x, float y);
        void setAngle(float a);             // Set the camera angle.
        void setForwardSpeed(float speed);  // Set the camera movement speed.
        void setStrafeSpeed(float speed);   // Set the camera strafe speed.
        void setTurnSpeed(float speed);     // Set the camera turn speed.
        void setFOV(float fov);
};

#endif //__CAMERA_H__
