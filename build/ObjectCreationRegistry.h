#ifndef _OBJECT_CREATION_REGISTRY_H_
#define _OBJECT_CREATION_REGISTRY_H_

#include <cassert>
#include <unordered_map>
#include "GameObject.h"


typedef GameObject* (*GameObjectCreationFunc)();


class ObjectCreationRegistry
{
public:
	ObjectCreationRegistry();

	template <class T>
	void RegisterCreationFunction()
	{
		// Ensure no duplicate class ID
		assert(mNameToGameObjectCreationFunctionMap.find(T::kClassId)
			== mNameToGameObjectCreationFunctionMap.end());
		mNameToGameObjectCreationFunctionMap[T::kClassId] = T::CreateInstance;
	}

	GameObject* CreateGameObject(uint32_t inClassId);

private:
	std::unordered_map<uint32_t, GameObjectCreationFunc> mNameToGameObjectCreationFunctionMap;
};



#endif // _OBJECT_CREATION_REGISTRY_H_