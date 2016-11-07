#include "Archer.h"
#include "TownHall.h"


static const std::string ACTION_NAMES[] =
{
	"SHOOTING",
	"IDLE",
	"WALKING",
};

std::ostream& operator <<(std::ostream& out, Action action)
{
	out << ACTION_NAMES[(int) action];
	return out;
}


Archer::Archer() :
	m_townHall(nullptr),
	m_health(0),
	m_currentAction(Action::IDLE)
{
}

Archer::Archer(const std::string& name, TownHall* townHall,
			   const Vector2i& location, int health, Action currentAction) :
	m_name(name),
	m_townHall(townHall),
	m_location(location),
	m_health(health),
	m_currentAction(currentAction)
{
}

void Archer::Serialize(RakNet::BitStream& out, LinkingContext& linkingContext)
{
	out.Write(m_name.c_str());
	out.Write(linkingContext.GetNetworkId(m_townHall));
	out.Write(m_location);
	out.Write(m_health);
	out.Write(m_currentAction);
}

void Archer::Deserialize(RakNet::BitStream& in, LinkingContext& linkingContext)
{
	uint32_t townHallId = 0;
	RakNet::RakString nameStr;

	nameStr.Deserialize(&in);
	m_name = nameStr.C_String();
	in.Read(townHallId);
	m_townHall = dynamic_cast<TownHall*>(linkingContext.GetGameObject(townHallId));
	in.Read(m_location);
	in.Read(m_health);
	in.Read(m_currentAction);
}

void Archer::PrintStateInfo(std::ostream& out)
{
	out << "Archer" << std::endl <<
		" * name           = " << m_name << std::endl <<
		" * town hall      = " << m_townHall->GetName() << std::endl <<
		" * current action = " << m_currentAction << std::endl <<
		" * health         = " << m_health << "%" << std::endl <<
		" * location       = " << m_location << std::endl;
}
