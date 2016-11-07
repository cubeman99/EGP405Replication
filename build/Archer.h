#ifndef _ARCHER_H_
#define _ARCHER_H_

#include <RakNet/BitStream.h>
#include <string>
#include "LinkingContext.h"
#include "GameObject.h"
#include "Vector2i.h"

class TownHall;


enum class Action
{
	SHOOTING,
	IDLE,
	WALKING,
};


std::ostream& operator <<(std::ostream& out, Action action);


class Archer : public GameObject
{
public:
	Archer();
	Archer(const std::string& name, TownHall* townHall, const Vector2i& location, int health, Action currentAction);

	inline const Vector2i&	GetLocation()		const { return m_location; }
	inline Action			GetCurrentAction()	const { return m_currentAction; }
	inline int				GetHealth()			const { return m_health; }
	inline TownHall*		GetTownHall()		const { return m_townHall; }
	inline const std::string GetName()		const { return m_name; }

	virtual void Serialize(RakNet::BitStream& out, LinkingContext& linkingContext) override;
	virtual void Deserialize(RakNet::BitStream& in, LinkingContext& linkingContext) override;
	virtual void PrintStateInfo(std::ostream& out) override;

	enum { kClassId = 'ARCH' };
	virtual uint32_t GetClassId() const { return kClassId; }
	static GameObject* CreateInstance() { return new Archer(); }

private:
	std::string		m_name;
	TownHall*		m_townHall;
	Vector2i		m_location;
	int				m_health;
	Action			m_currentAction;
};


#endif // _ARCHER_H_