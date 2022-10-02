#include "Screen.hpp"

#include <algorithm>
#include <vector>

Screen::Screen(uint screenWidth, uint screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	uint screenSize = m_screenWidth * m_screenHeight;
	m_screen = new wchar_t[screenSize];
	SetConsole();

	uint mapWidth{ 16 }, mapHeight{ 16 };
	m_map = Map(mapWidth, mapHeight);

	// Default configuration
	m_fieldOfView = 3.14159 / 4.0;	// Field of View
	m_depth = 16.0;					// Maximum rendering distance

	// Player state
	m_xPos = 0.0;
	m_yPos = 0.0;
	m_playerAngle = 0.0;
}

Screen::~Screen()
{
	delete[] m_screen;
}

void Screen::Write(uint pos, wchar_t character)
{
	m_screen[pos] = character;
}

void Screen::SetConsole()
{
	m_console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(m_console);
}

Map Screen::GetMap()
{
	return m_map;
}

void Screen::RayCasting(double const& xPos, double const& yPos, double const& playerAngle)
{
	m_xPos = xPos;
	m_yPos = yPos;
	m_playerAngle = playerAngle;

	for (int x{ 0 }; x < m_screenWidth; x++)
	{				
		double distanceToWall = 0.0; //                                      resolution
		bool boundary = false;		// Set when ray hits boundary between two wall blocks

		IncrementalRayCasting(x, distanceToWall, boundary);

		// Calculate distance to ceiling and floor
		int ceiling = (double)(m_screenHeight / 2.0) - m_screenHeight / ((double)distanceToWall);
		int floor = m_screenHeight - ceiling;

		// Shader walls based on distance
		short shade{ WallShadeByDistance(distanceToWall) };
		if (boundary)
		{
			shade = ' '; // Black it out
		}
		else{}

		for (int y = 0; y < m_screenHeight; y++)
		{
			// Each Row
			if (y <= ceiling)
			{
				Write(y * m_screenWidth + x, ' ');
			}
			else if (y > ceiling && y <= floor)
			{
				Write(y * m_screenWidth + x, shade);
			}
			else // Floor
			{
				// Shade floor based on distance
				double b = 1.0f - (((double)y - m_screenHeight / 2.0f) / ((double)m_screenHeight / 2.0f));
				shade = std::move(FloorShade(b));

				Write(y * m_screenWidth + x, shade);
			}
		}
	}
}

void Screen::IncrementalRayCasting(double const &x, double &distanceToWall, bool &boundary) const
{
	// For each column, calculate the projected ray angle into world space
	bool hitWall = false;		// Set when ray hits wall block

	// Find distance to wall
	double stepSize = 0.1;		  // Increment size for ray casting, decrease to increase		

	// Incrementally cast ray from player, along ray angle, testing for 
	// intersection with a block
	while (!hitWall && distanceToWall < m_depth)
	{
		distanceToWall += stepSize;

		hitWall = IsWallBlock(x, distanceToWall, boundary);
	}
}

bool Screen::IsWallBlock(double const &x, double &distanceToWall, bool &boundary) const
{
	double rayAngle{ (m_playerAngle - m_fieldOfView / 2.0f) + ((double)x / (double)m_screenWidth) * m_fieldOfView };
	double eyeX{ sin(rayAngle) }; // Unit vector for ray in player space
	double eyeY{ cos(rayAngle) };

	int testX{ (int)(m_xPos + eyeX * distanceToWall) };
	int testY{ (int)(m_yPos + eyeY * distanceToWall) };
	bool hitWall{ false };


	// Test if ray is out of bounds
	if (testX < 0 || testX >= m_map.GetMapWidth() || testY < 0 || testY >= m_map.GetMapHeight())
	{
		hitWall = true;			// Just set distance to maximum depth
		distanceToWall = m_depth;
	}
	else
	{
		// Ray is inbounds so test to see if the ray cell is a wall block
		if (m_map.GetMap().c_str()[testX * m_map.GetMapWidth() + testY] == '#')
		{
			// Ray has hit wall
			hitWall = true;

			// To highlight tile boundaries, cast a ray from each corner
			// of the tile, to the player. The more coincident this ray
			// is to the rendering ray, the closer we are to a tile 
			// boundary, which we'll shade to add detail to the walls
			std::vector<std::pair<double, double>> p{};

			// Test each corner of hit tile, storing the distance from
			// the player, and the calculated dot product of the two rays
			for (int tx = 0; tx < 2; tx++)
			{
				for (int ty = 0; ty < 2; ty++)
				{
					// Angle of corner to eye
					double vy = (double)testY + ty - m_yPos;
					double vx = (double)testX + tx - m_xPos;
					double d = sqrt(vx * vx + vy * vy);
					double dot = (eyeX * vx / d) + (eyeY * vy / d);
					p.push_back(std::make_pair(d, dot));
				}
			}

			// Sort Pairs from closest to farthest
			sort(p.begin(), p.end(), [](const std::pair<double, double>& left, const std::pair<double, double>& right) {return left.first < right.first; });

			// First two/three are closest (we will never see all four)
			double bound = 0.01;
			if (acos(p.at(0).second) < bound)
			{
				boundary = true;
			}
			else{}

			if (acos(p.at(1).second) < bound)
			{
				boundary = true;
			}
			else{}

			if (acos(p.at(2).second) < bound)
			{
				boundary = true;
			}
			else{}
		}
		else {}
	}

	return hitWall;
}

short Screen::WallShadeByDistance(double const& distanceToWall) const
{
	short shade{ ' ' };

	if (distanceToWall <= m_depth / 4.0f)
	{
		shade = 0x2588;	// Very close
	}
	else if (distanceToWall < m_depth / 3.0f)
	{
		shade = 0x2593;
	}
	else if (distanceToWall < m_depth / 2.0f)
	{
		shade = 0x2592;
	}
	else if (distanceToWall < m_depth)
	{
		shade = 0x2591;
	}
	else{} // Too far away
	
	return shade;
}

short Screen::FloorShade(double const &b) const
{
	short shade{ ' ' };
	if (b < 0.25)
	{
		shade = '#';
	}
	else if (b < 0.5)
	{
		shade = 'x';
	}
	else if (b < 0.75)
	{
		shade = '.';
	}
	else if (b < 0.9)
	{
		shade = '-';
	}
	else{}

	return shade;
}

void Screen::DisplayStats(size_t bufferCount, const wchar_t *format, double const &playerXPos, double const &playerYPos, 
	double const &orientationAngle, double const &elapsedTime)
{
	swprintf_s(m_screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", playerXPos, playerYPos, orientationAngle, 1.0f / elapsedTime);
}

void Screen::WriteOutputCharacter(DWORD &bytesWritten)
{
	Write(m_screenWidth * m_screenHeight - 1, '\0');
	WriteConsoleOutputCharacter(m_console, m_screen, m_screenWidth * m_screenHeight, { 0,0 }, &bytesWritten);
}

void Screen::DisplayMap()
{
	uint mapWidth{ m_map.GetMapWidth() };
	std::wstring map{ m_map.GetMap() };
	for (uint nx = 0; nx < mapWidth; nx++)
	{
		for (uint ny = 0; ny < mapWidth; ny++)
		{
			uint positionInMap{ ny * mapWidth + nx };
			wchar_t toWrite{ map.c_str()[positionInMap] };
			uint whereToWrite{ (uint)((ny + 1) * m_screenWidth + nx) };
			Write(whereToWrite, toWrite);
		}
	}
}
