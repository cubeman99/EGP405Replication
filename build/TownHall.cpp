#include "TownHall.h"


TownHall::TownHall() :
	m_type(TownHallType::ELFS)
{
}

TownHall::TownHall(TownHallType type, const Vector2i& location, int health) :
	m_type(type),
	m_location(location),
	m_health(health)
{
}

