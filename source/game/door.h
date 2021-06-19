#ifndef __DOOR_H__
#define __DOOR_H__

#define DOOR_SPEED 2.0f
#define DOOR_OPEN 0
#define DOOR_CLOSED 1
#define DOOR_OPENING 2
#define DOOR_CLOSING 3

class Door {
    private:
        float time;     // value between 0 and 1 that defines how much the door is open. (0=fully open, 1=fully closed)
        char state;     // The current state of the door. (0=open, 1=closed, 2=opening, 3=closing)
    public:
        Door();
        float getTime();
        char getState();

        void toggle();          // Toggle the door open or closed.
        void update(float dt);  // Update the door animation.
};

#endif //__DOOR_H__
