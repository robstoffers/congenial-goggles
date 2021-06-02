#include <nds.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(void) {
    videoSetMode(MODE_5_2D);
    PrintConsole* pConsole = consoleDemoInit();
    pConsole->cursorX = 0;
    pConsole->cursorY = 0;
    printf("Hello World!");

    while (true) {
        scanKeys();
        if (keysDown() & KEY_START) {
            break;
        }
        swiWaitForVBlank();
    }
    return 0;
}