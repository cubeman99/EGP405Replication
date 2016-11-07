#ifndef _LINKING_CONTEXT_H_
#define _LINKING_CONTEXT_H_

#include <cstdint>
#include <unordered_map>
#include "GameObject.h"


class LinkingContext
{
public:

	static const uint32_t NULL_NETWORK_ID;

public:
	LinkingContext();

	uint32_t GetNetworkId(GameObject* inGameObject, bool inShouldCreateIfNotFound = true);
	GameObject* GetGameObject(uint32_t inNetworkId);

	void AddGameObject(GameObject* inGameObject, uint32_t inNetworkId);
	void RemoveGameObject(GameObject *inGameObject);
	void ClearGameObjects();

private:
	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap;
	std::unordered_map<const GameObject*, uint32_t> mGameObjectToNetworkIdMap;
	uint32_t mNextNetworkId;
};


#endif // _LINKING_CONTEXT_H_