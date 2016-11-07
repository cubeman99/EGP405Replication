#include "ObjectCreationRegistry.h"
#include "Archer.h"
#include "TownHall.h"


ObjectCreationRegistry::ObjectCreationRegistry()
{
}

GameObject* ObjectCreationRegistry::CreateGameObject(uint32_t inClassId)
{
	// add error checking if desired - for now crash if not found
	GameObjectCreationFunc creationFunc = mNameToGameObjectCreationFunctionMap[inClassId];
	GameObject* gameObject = creationFunc();
	return gameObject;
}
