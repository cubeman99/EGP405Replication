#include "TownHall.h"


static const std::string TOWN_HALL_TYPE_NAMES[] =
{
	"HUMANS",
	"ORCS",
	"ELVES",
	"UNDEAD",
};

std::ostream& operator <<(std::ostream& out, TownHallType type)
{
	out << TOWN_HALL_TYPE_NAMES[(int) type];
	return out;
}


TownHall::TownHall() :
	m_type(TownHallType::ELVES)
{
}

TownHall::TownHall(const std::string& name, TownHallType type,
				   const Vector2i& location, int health) :
	m_name(name),
	m_type(type),
	m_location(location),
	m_health(health)
{
}

void TownHall::Serialize(RakNet::BitStream& out, LinkingContext& linkingContext)
{
	out.Write(m_name.c_str());
	out.Write(m_type);
	out.Write(m_location);
	out.Write(m_health);
}

void TownHall::Deserialize(RakNet::BitStream& in, LinkingContext& linkingContext)
{
	RakNet::RakString nameStr;
		
	nameStr.Deserialize(&in);
	m_name = nameStr.C_String();
	in.Read(m_type);
	in.Read(m_location);
	in.Read(m_health);
}

void TownHall::PrintStateInfo(std::ostream& out)
{
	out << "Town Hall" << std::endl <<
		" * name           = " << m_name << std::endl <<
		" * type           = " << m_type << std::endl <<
		" * health         = " << m_health << std::endl <<
		" * location       = " << m_location << std::endl;
}
