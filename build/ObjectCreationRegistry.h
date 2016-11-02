#ifndef _OBJECT_CREATION_REGISTRY_H_
#define _OBJECT_CREATION_REGISTRY_H_

#include <unordered_map>
#include <assert.h>
#include "GameObject.h"


typedef GameObject* (*GameObjectCreationFunc)();


class ObjectCreationRegistry
{
public:

	static ObjectCreationRegistry& Get()
	{
		static ObjectCreationRegistry sInstance;
		return sInstance;
	}

	template <class T>
	void RegisterCreationFunction()
	{
		// ensure no duplicate class id
		//std::assert(mNameToGameObjectCreationFunctionMap.find(T::kClassId)
			//== mNameToGameObjectCreationFunctionMap.end());
		mNameToGameObjectCreationFunctionMap[T::kClassId] = T::CreateInstance;
	}

	GameObject* CreateGameObject(uint32_t inClassId)
	{
		//add error checking if desired- for now crash if not found
		GameObjectCreationFunc creationFunc = mNameToGameObjectCreationFunctionMap[inClassId];
		GameObject* gameObject = creationFunc();
		return gameObject;
	}

private:
	ObjectCreationRegistry() {}

	std::unordered_map<uint32_t, GameObjectCreationFunc> mNameToGameObjectCreationFunctionMap;
};

void RegisterObjectCreation()
{
	//ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	//ObjectCreationRegistry::Get().RegisterCreationFunction<RoboCat>();
	//ObjectCreationRegistry::Get().RegisterCreationFunction<RoboMouse>();
}


#endif // _OBJECT_CREATION_REGISTRY_H_