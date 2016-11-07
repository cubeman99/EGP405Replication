#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <RakNet/BitStream.h>
#include <cstdint>
#include <ostream>

class LinkingContext;


class GameObject
{
public:
	GameObject() {}
	virtual ~GameObject() {}

	virtual uint32_t GetClassId() const = 0;
	virtual void Serialize(RakNet::BitStream& out, LinkingContext& linkingContext) = 0;
	virtual void Deserialize(RakNet::BitStream& in, LinkingContext& linkingContext) = 0;
	virtual void PrintStateInfo(std::ostream& out) = 0;

private:
};


#endif // _GAME_OBJECT_H_