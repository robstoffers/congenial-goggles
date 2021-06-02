#include <nds.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "stone_wall.h"

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

int screenWidth = 256;
int screenHeight = 192;

float playerX = 4.5f;
float playerY = 2.5f;
float playerA = 0;
float fov = 3.14159f / 4.0f;
float drawDistance = 16.0f;
float walkSpeed = 1.5f;
float turnSpeed = 2.25f;

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

int main(void) {
	videoSetMode(MODE_5_2D);
	consoleDemoInit();

	int bg = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);

	PrintConsole* console = consoleDemoInit();

	u16* backBuffer = (u16*)bgGetGfxPtr(bg);

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
		printf("dir: (%.2f, %.2f)", playerDirX, playerDirY);

		float angleSweepStart = playerA - fov / 2.0f;

		for (int x = 0; x < screenWidth; x++) {
			float rayAngle = angleSweepStart + ((float)x / (float)screenWidth) * fov;

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

			int ceiling = (float)(screenHeight / 2.0f) - screenHeight / distanceToWall;
			int floor = screenHeight - ceiling;

			// float f = (distanceToWall / drawDistance);
			// float a = 31;
			// float b = 5;
			// int lerp = int((a * (1.0 - f)) + (b * f));

			// u16 shade = ARGB16(1, lerp, lerp, lerp);
			for (int y = 0; y < screenHeight; y++) {
				if (y <= ceiling) {
					backBuffer[y * screenWidth + x] = ARGB16(1, 0, 0, 0);
				} else if (y > ceiling && y <= floor) {
					float sampleY = ((float)y - (float)ceiling) / ((float)floor - (float)ceiling);
					int sx = sampleX * 32.0f;
					int sy = sampleY * 31.0f;
					backBuffer[y * screenWidth + x] = stone_wall[sy * 32 + sx];
					//backBuffer[y * screenWidth + x] = shade;
				} else {
					backBuffer[y * screenWidth + x] = ARGB16(1, 0, 0, 0);
				}
			}
		}

		swiWaitForVBlank();
	}
	return 0;
}