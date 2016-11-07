#ifndef _TOWN_HALL_H_
#define _TOWN_HALL_H_

#include <RakNet/BitStream.h>
#include <string>
#include "LinkingContext.h"
#include "GameObject.h"
#include "Vector2i.h"


enum class TownHallType
{
	HUMANS,
	ORCS,
	ELVES,
	UNDEAD,
};

std::ostream& operator <<(std::ostream& out, TownHallType type);


class TownHall : public GameObject
{
public:
	TownHall();
	TownHall(const std::string& name, TownHallType type, const Vector2i& location, int health);

	inline const Vector2i&	GetLocation()	const { return m_location; }
	inline TownHallType		GetType()		const { return m_type; }
	inline int				GetHealth()		const { return m_health; }
	inline const std::string GetName()		const { return m_name; }

	virtual void Serialize(RakNet::BitStream& out, LinkingContext& linkingContext) override;
	virtual void Deserialize(RakNet::BitStream& in, LinkingContext& linkingContext) override;
	virtual void PrintStateInfo(std::ostream& out) override;

	enum { kClassId = 'TNHL' };
	virtual uint32_t GetClassId() const { return kClassId; }
	static GameObject* CreateInstance() { return new TownHall(); }

private:
	std::string		m_name;
	TownHallType	m_type;
	Vector2i		m_location;
	int				m_health;
};


#endif // _TOWN_HALL_H_