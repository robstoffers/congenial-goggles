#ifndef __INPUT_H__
#define __INPUT_H__

class Input {
    public:
        void update();

        bool isAPressed();
        bool isBPressed();
        bool isXPressed();
        bool isYPressed();
        bool isLeftPressed();
        bool isRightPressed();
        bool isUpPressed();
        bool isDownPressed();
        bool isLPressed();
        bool isRPressed();
        bool isStartPressed();

        bool isAHeld();
        bool isBHeld();
        bool isXHeld();
        bool isYHeld();
        bool isLeftHeld();
        bool isRightHeld();
        bool isUpHeld();
        bool isDownHeld();
        bool isLHeld();
        bool isRHeld();
        bool isStartHeld();
};

#endif //__INPUT_H__