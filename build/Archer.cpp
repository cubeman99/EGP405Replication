#include "Archer.h"

Archer::Archer() :
	m_townHall(nullptr),
	m_health(0),
	m_currentAction(Action::IDLE)
{
}

Archer::Archer(TownHall* townHall, const Vector2i& location, int health, Action currentAction) :
	m_townHall(townHall),
	m_location(location),
	m_health(health),
	m_currentAction(currentAction)
{
}
