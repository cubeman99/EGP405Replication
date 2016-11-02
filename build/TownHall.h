#ifndef _TOWN_HALL_H_
#define _TOWN_HALL_H_

#include "GameObject.h"
#include "Vector2i.h"


enum class TownHallType
{
	ELFS,
	WEREWOLFS,
	VAMPIRES,
	ORCS
};

#define REGISTER_GAME_OBJECT(type, id) \
	enum { kClassId = id }; \
	virtual uint32_t GetClassId() const { return kClassId; } \
	static type* CreateInstance() { return new type(); }


class TownHall : public GameObject
{
public:
	TownHall();
	TownHall(TownHallType type, const Vector2i& location, int health);

	inline const Vector2i&	GetLocation()	const { return m_location; }
	inline TownHallType		GetType()		const { return m_type; }
	inline int				GetHealth()		const { return m_health; }

	REGISTER_GAME_OBJECT(TownHall, 'TNHL');

private:
	TownHallType	m_type;
	Vector2i		m_location;
	int				m_health;
};


#endif // _TOWN_HALL_H_