#pragma once

#include <cstdint>
#include <unordered_map>
#include "GameObject.h"


class LinkingContext
{
public:
	LinkingContext() :
		mNextNetworkId(1)
	{}
	
	uint32_t GetNetworkId(GameObject* inGameObject, bool inShouldCreateIfNotFound)
	{
		auto it = mGameObjectToNetworkIdMap.find(inGameObject);
		
		if (it != mGameObjectToNetworkIdMap.end())
		{
			return it->second;
		}
		else if (inShouldCreateIfNotFound)
		{
			uint32_t newNetworkId = mNextNetworkId++;
			AddGameObject(inGameObject, newNetworkId);
			return newNetworkId;
		}
		else
		{
			return 0;
		}
	}

	void AddGameObject(GameObject* inGameObject, uint32_t inNetworkId)
	{
		mNetworkIdToGameObjectMap[inNetworkId] = inGameObject;
		mGameObjectToNetworkIdMap[inGameObject] = inNetworkId;
	}

	void RemoveGameObject(GameObject *inGameObject)
	{
		uint32_t networkId = mGameObjectToNetworkIdMap[inGameObject];
		mGameObjectToNetworkIdMap.erase(inGameObject);
		mNetworkIdToGameObjectMap.erase(networkId);
	}
	
	GameObject* GetGameObject(uint32_t inNetworkId)
	{
		auto it = mNetworkIdToGameObjectMap.find(inNetworkId);
		if (it != mNetworkIdToGameObjectMap.end())
			return it->second;
		return nullptr;
	}

private:
	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap;
	std::unordered_map<const GameObject*, uint32_t> mGameObjectToNetworkIdMap;
	uint32_t mNextNetworkId;
};




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
		std::assert(mNameToGameObjectCreationFunctionMap.find(T::kClassId)
			== mNameToGameObjectCreationFunctionMap.end());
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
	ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	//ObjectCreationRegistry::Get().RegisterCreationFunction<RoboCat>();
	//ObjectCreationRegistry::Get().RegisterCreationFunction<RoboMouse>();
}

