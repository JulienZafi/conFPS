#include "Map.hpp"

Map::Map()
{
	m_mapWidth = 0;
	m_mapHeight = 0;
	m_map = std::wstring();
}

Map::Map(uint mapWidth, uint mapHeight)
{
	m_mapWidth = mapWidth;
	m_mapHeight = mapHeight;

	// Create Map of world space # = wall block, . = space
	m_map += L"#########.......";
	m_map += L"#...............";
	m_map += L"#.......########";
	m_map += L"#..............#";
	m_map += L"#......##......#";
	m_map += L"#......##......#";
	m_map += L"#..............#";
	m_map += L"###............#";
	m_map += L"##.............#";
	m_map += L"#......####..###";
	m_map += L"#......#.......#";
	m_map += L"#......#.......#";
	m_map += L"#..............#";
	m_map += L"#......#########";
	m_map += L"#..............#";
	m_map += L"################";
}

Map::~Map(){}

uint Map::GetMapWidth() const
{
	return m_mapWidth;
}

uint Map::GetMapHeight() const
{
	return m_mapHeight;
}

std::wstring Map::GetMap() const
{
	return m_map;
}