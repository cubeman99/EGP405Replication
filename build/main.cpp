



#include "ObjectCreationRegistry.h"
#include "TownHall.h"
#include "Archer.h"



int main()
{
	TownHall hallElfs(TownHallType::ELFS, Vector2i(40, 23), 100);
	TownHall hallVampires(TownHallType::VAMPIRES, Vector2i(21, 15), 100);

	Archer archerElf1(&hallElfs, Vector2i(42, 20), 100, Action::IDLE);


	

	ObjectCreationRegistry::Get().RegisterCreationFunction<Archer>();
	//ObjectCreationRegistry::Get().RegisterCreationFunction<TownHall>();

	//ObjectCreationRegistry::Get().CreateGameObject(Archer::kClassId);

	return 0;
}

