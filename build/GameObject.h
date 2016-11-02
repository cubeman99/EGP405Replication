#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <cstdint>


class GameObject
{
public:
	GameObject();
	virtual ~GameObject() {}

	virtual uint32_t GetClassId() const = 0;

	inline int32_t GetGameObjectId() const { return m_gameObjectId; }

	inline void SetGameObjectId(int32_t gameObjectId) { m_gameObjectId = gameObjectId; }

private:
	int32_t m_gameObjectId;
};


#endif // _GAME_OBJECT_H_