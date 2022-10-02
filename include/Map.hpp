#ifndef MAP_HPP
#define MAP_HPP

#define uint unsigned int

#include <string>

class Map
{
public:
	Map();
	Map(uint mapWidth, uint mapHeight);
	~Map();

	uint GetMapWidth() const;
	uint GetMapHeight() const;
	std::wstring GetMap() const;
private:
	uint m_mapWidth;
	uint m_mapHeight;
	std::wstring m_map;
};

#endif