#include <nds.h>
#include <stdlib.h>
#include <stdio.h>

#include "game.h"

u16 colorLerp16(u16 from, u16 to, float amount);

int main(void) {
	videoSetMode(MODE_5_2D);
	
	consoleDemoInit();

	Game game;
	game.init(SCREEN_WIDTH, SCREEN_HEIGHT);

	int bg = bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	bgSetScale(bg, 128, 128);

	PrintConsole* console = consoleDemoInit();
	
	cpuStartTiming(0);

	while (1) {
		u32 deltaTick = cpuEndTiming();
		float dt = (float)deltaTick / (float)BUS_CLOCK;
		cpuStartTiming(0);

		if (!game.update(dt, console)) {
			break;
		}

		dmaCopy(game.getBuffer(), BG_GFX, game.getBufferSize());

		swiWaitForVBlank();
	}

	game.dispose();

	return 0;
}

u16 colorLerp16(u16 from, u16 to, float amount) {
	u8 aTo = (to & 0x8000) >> 15;
	u8 bTo = (to & 0x7C00) >> 10;
	u8 gTo = (to & 0x3E0) >> 5;
	u8 rTo = (to & 0x1F);

	//u8 aFrom = (from & 0x8000) >> 15;
	u8 bFrom = (from & 0x7C00) >> 10;
	u8 gFrom = (from & 0x3E0) >> 5;
	u8 rFrom = (from & 0x1F);

	u8 rFinal = u8((rFrom * (1.0f - amount)) + (rTo * amount));
	u8 gFinal = u8((gFrom * (1.0f - amount)) + (gTo * amount));
	u8 bFinal = u8((bFrom * (1.0f - amount)) + (bTo * amount));

	return (aTo << 15) | (bFinal << 10) | (gFinal << 5) | rFinal;
}