#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>

#include "Screen.hpp"
#include "Time.hpp"
#include "Player.hpp"

int main()
{
	const uint SCREEN_WIDTH{ 120 };
	const uint SCREEN_HEIGHT{ 40 };

	// Create screen instance to display game entity
	Screen screen{ Screen(SCREEN_WIDTH, SCREEN_HEIGHT) };
	Player player{ Player(screen.GetMap()) };
	DWORD bytesWritten{ 0 };

	Time time{};
	while (true)
	{
		// Update player position
		double elapsedTime = std::move(time.ElapsedTime());
		player.UpdatePlayerPosition(elapsedTime);
		screen.RayCasting(player.GetXPosition(), player.GetYPosition(), player.GetOrientationAngle());

		// Display management
		screen.DisplayStats(40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", player.GetXPosition(), player.GetYPosition(), player.GetOrientationAngle(), 1.0f / elapsedTime);
		screen.DisplayMap();
		screen.Write(((int)player.GetXPosition() + 1) * SCREEN_WIDTH + (int)player.GetYPosition(), 'P');

		// Display Frame
		screen.WriteOutputCharacter(bytesWritten);
	}

	return 0;
}
