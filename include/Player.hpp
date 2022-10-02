#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

#include "Map.hpp"

class Player
{
public:
	Player(Map const& map);
	~Player();

	double GetXPosition() const;
	double GetYPosition() const;
	double GetOrientationAngle() const;
	void UpdatePlayerPosition(double const &elapsedTime);

private:
	double m_xPos;				// Player Start Position
	double m_yPos;
	double m_orientationAngle;	// Player Start Rotation
	double m_speed;				// Walking Speed
	Map m_map;
};

#endif // !PLAYER_HPP
