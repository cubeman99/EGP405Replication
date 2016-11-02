#ifndef _ARCHER_H_
#define _ARCHER_H_

#include "GameObject.h"
#include "Vector2i.h"

class TownHall;


enum class Action
{
	SHOOTING,
	IDLE,
	WALKING,
};


class Archer : public GameObject
{
public:
	Archer();
	Archer(TownHall* townHall, const Vector2i& location, int health, Action currentAction);

	inline const Vector2i&	GetLocation()		const { return m_location; }
	inline Action			GetCurrentAction()	const { return m_currentAction; }
	inline int				GetHealth()			const { return m_health; }
	inline TownHall*		GetTownHall()		const { return m_townHall; }

	enum { kClassId = 'ARCH' };
	virtual uint32_t GetClassId() const { return kClassId; }
	static GameObject* CreateInstance() { return new Archer(); }

private:
	TownHall*		m_townHall;
	Vector2i		m_location;
	int				m_health;
	Action			m_currentAction;
};


#endif // _ARCHER_H_