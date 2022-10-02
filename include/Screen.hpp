#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "Map.hpp"

#include <utility>
#include <Windows.h>

class Screen
{
public:
	Screen() = default;
	Screen(uint screenWidth, uint screenHeight);
	virtual ~Screen();

	void Write(uint pos, wchar_t character);
	void SetConsole();
	Map GetMap();
	void RayCasting(double const &xPos, double const &yPos, double const &playerAngle);
	void IncrementalRayCasting(double const &x, double &distanceToWall, bool &boundary) const;
	bool IsWallBlock(double const &x, double &distanceToWall, bool &boundary) const;
	short WallShadeByDistance(double const& distanceToWall) const;
	short FloorShade(double const &b) const;
	void DisplayStats(size_t bufferCount, const wchar_t* format, double const& playerXPos, double const& playerYPos,
		double const& orientationAngle, double const& elapsedTime);
	void WriteOutputCharacter(DWORD &bytesWritten);
	void DisplayMap();


private:
	wchar_t *m_screen; // Screen buffer
	HANDLE m_console;
	Map m_map;
	uint m_screenWidth;
	uint m_screenHeight;
	double m_fieldOfView;	// Field of View
	double m_depth;			// Maximum rendering distance
	double m_xPos;
	double m_yPos;
	double m_playerAngle;
};

#endif //SCREEN_HPP