#include <nds.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "stone_wall.h"

#define SCREEN_BUFFER_LEN SCREEN_WIDTH * SCREEN_HEIGHT
#define SCREEN_BUFFER_SIZE SCREEN_BUFFER_LEN * 2

#define GAME_WIDTH SCREEN_WIDTH / 2
#define GAME_HEIGHT SCREEN_HEIGHT / 2

#define BLACK 	0x8000
#define WHITE 	0xFFFF
#define RED 	0xFC00
#define GREEN	0x83E0
#define BLUE	0x801F

#define PI 3.14159f

int mapWidth = 16;
int mapHeight = 16;
u8 map[] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};
// u8 map[] = {
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// };

u16 screenBuffer[SCREEN_BUFFER_LEN];

float playerX = 1.5f;
float playerY = 1.5f;
float playerA = 0;
float fov = PI / 4.0f;
float drawDistance = 16.0f;
float walkSpeed = 1.5f;
float turnSpeed = 2.25f;

void move(u8 *map, int mapWidth, float *pX, float *pY, float velX, float velY);

u16 colorLerp16(u16 from, u16 to, float amount);

int main(void) {
	videoSetMode(MODE_5_2D);
	
	consoleDemoInit();

	int bg = bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	bgSetScale(bg, 128, 128);

	PrintConsole* console = consoleDemoInit();

	for(int x = 0; x < SCREEN_BUFFER_LEN; x++){
		screenBuffer[x] = 0xEEEE;
	}
	
	cpuStartTiming(0);

	while (1) {
		u32 deltaTick = cpuEndTiming();
		float dt = (float)deltaTick / (float)BUS_CLOCK;
		cpuStartTiming(0);

		float playerDirX = cosf(playerA);
		float playerDirY = sinf(playerA);

		float playerRightX = cosf(playerA + 1.5708);
		float playerRightY = sinf(playerA + 1.5708);

		scanKeys();
		if (keysDown() & KEY_START) {
			break;
		}
		if (keysHeld() & KEY_RIGHT) {
			playerA += turnSpeed * dt;
		} else if (keysHeld() & KEY_LEFT) {
			playerA -= turnSpeed * dt;
		}
		if (keysHeld() & KEY_UP) {
			move(map, mapWidth, &playerX, &playerY, playerDirX * walkSpeed * dt, playerDirY * walkSpeed * dt);
		} else if (keysHeld() & KEY_DOWN) {
			move(map, mapWidth, &playerX, &playerY, -(playerDirX * walkSpeed * dt), -(playerDirY * walkSpeed * dt));
		}
		if (keysHeld() & KEY_L) {
			move(map, mapWidth, &playerX, &playerY, -(playerRightX * walkSpeed * dt), -(playerRightY * walkSpeed * dt));
		} else if (keysHeld() & KEY_R) {
			move(map, mapWidth, &playerX, &playerY, playerRightX * walkSpeed * dt, playerRightY * walkSpeed * dt);
		}

		console->cursorX = 0;
		console->cursorY = 1;
		printf("player: (%.2f, %.2f)", playerX, playerY);

		console->cursorX = 0;
		console->cursorY = 2;
		//printf("dir: (%.2f, %.2f)", playerDirX, playerDirY);
		//printf("dir: (%d, %d)", scaleX, scaleY);
		printf("dt: %.5f", dt);
		float angleSweepStart = playerA - fov / 2.0f;

		float rayAngle;
		for (int x = 0; x < GAME_WIDTH; x++) {
			rayAngle = angleSweepStart + ((float)x / float(GAME_WIDTH)) * fov;

			float distanceToWall = 0.0f;
			bool hitWall = false;

			float rayDirX = cosf(rayAngle);
			float rayDirY = sinf(rayAngle);

			float sideDistX;
			float sideDistY;

			float rayUnitStepSizeX = sqrt(1 + (rayDirY / rayDirX) * (rayDirY / rayDirX));
			float rayUnitStepSizeY = sqrt(1 + (rayDirX / rayDirY) * (rayDirX / rayDirY));

			int stepX;
			int stepY;

			int mapX = (int)playerX;
			int mapY = (int)playerY;

			float sampleX = 0.0f;

			if (rayDirX < 0) {
        		stepX = -1;
        		sideDistX = (playerX - (float)mapX) * rayUnitStepSizeX;
			} else {
        		stepX = 1;
        		sideDistX = (float(mapX + 1) - playerX) * rayUnitStepSizeX;
      		}
			  
			if (rayDirY < 0) {
				stepY = -1;
				sideDistY = (playerY - (float)mapY) * rayUnitStepSizeY;
			} else {
        		stepY = 1;
        		sideDistY = (float(mapY + 1) - playerY) * rayUnitStepSizeY;
			}

			while (!hitWall && distanceToWall < drawDistance) {
				if (sideDistX < sideDistY) {
					mapX += stepX;
					distanceToWall = sideDistX;
					sideDistX += rayUnitStepSizeX;
				} else {
					mapY += stepY;
					distanceToWall = sideDistY;
					sideDistY += rayUnitStepSizeY;					
				}

				if (mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight) {
					hitWall = true;
					distanceToWall = drawDistance;
				} else {
					if (map[mapY * mapWidth + mapX] == 1) {
						hitWall = true;

						float tileMidX = (float)mapX + 0.5f;
						float tileMidY = (float)mapY + 0.5f;

						float testPointX = playerX + rayDirX * distanceToWall;
						float testPointY = playerY + rayDirY * distanceToWall;

						float testAngle = atan2f((testPointY - tileMidY), (testPointX - tileMidX));
						if (testAngle >= -3.14159f * 0.25f && testAngle < 3.14159f * 0.25f)
							sampleX = testPointY - (float)mapY;
						if (testAngle >= 3.14159f * 0.25f && testAngle < 3.14159f * 0.75f)
							sampleX = testPointX - (float)mapX;
						if (testAngle < -3.14159f * 0.25f && testAngle >= -3.14159f * 0.75f)
							sampleX = testPointX - (float)mapX;
						if (testAngle >= 3.14159f * 0.75f || testAngle < -3.14159f * 0.75f)
							sampleX = testPointY - (float)mapY;
					}
				}
			}

			int ceiling = (float)(GAME_HEIGHT / 2.0f) - GAME_HEIGHT / distanceToWall;
			int floor = GAME_HEIGHT - ceiling;

			//float f = distanceToWall / drawDistance;
			//u16 shade = colorLerp16(0xFFFF, 0x8000, f);
			
			for (int y = 0; y < GAME_HEIGHT; y++) {
				if (y <= ceiling) {
					screenBuffer[y * SCREEN_WIDTH + x] = BLACK;
				} else if (y > ceiling && y <= floor) {
					if (distanceToWall < drawDistance) {
						float sampleY = ((float)y - (float)ceiling) / ((float)floor - (float)ceiling);
						int sx = sampleX * 32.0f;
						int sy = sampleY * 32.0f;

						screenBuffer[y * SCREEN_WIDTH + x] = stone_wall[sy * 32 + sx];
					} else {
						screenBuffer[y * SCREEN_WIDTH + x] = BLACK;
					}
					//screenBuffer[y * SCREEN_WIDTH + x] = shade;
				} else {
					screenBuffer[y * SCREEN_WIDTH + x] = 0x9CE7;
				}
			}
		}

		dmaCopy(screenBuffer, BG_GFX, SCREEN_BUFFER_SIZE);

		swiWaitForVBlank();
	}
	return 0;
}

void move(u8 *map, int mapWidth, float *pX, float *pY, float velX, float velY) {
	float playerX = *pX;
	float playerY = *pY;

	// store the position where the player will go but don't move it yet.
	float testX = playerX + velX;
	float testY = playerY + velY;

	// Only testing the X movement right now
	int tileX = (int)testX;
	int tileY = (int)playerY;
	if (map[tileY * mapWidth + tileX] == 1) {
		testX = playerX;
	}

	// Only testing the Y movement now
	tileX = (int)playerX;
	tileY = (int)testY;
	if (map[tileY * mapWidth + tileX] == 1) {
		testY = playerY;
	}

	// update the player position.
	*pX = testX;
	*pY = testY;
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