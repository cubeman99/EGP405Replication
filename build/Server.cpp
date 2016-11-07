#include "Server.h"
#include "TownHall.h"
#include "Archer.h"
#include "ObjectCreationRegistry.h"

using namespace RakNet;


Server::Server()
{
	RegisterRPCs(m_replicationManager.GetRPCManager());
	//RegisterObjectCreation(m_replicationManager.GetObjectCreationRegistry());

	// Town Halls:
	//  - 1 Elf
	// - 1 Human
	// - 1 Undead
	// - 2 Orc
	TownHall* hallElves  = new TownHall("Darnassus", TownHallType::ELVES, Vector2i(40, 23), 100);
	TownHall* hallHumans = new TownHall("Stormwind", TownHallType::HUMANS, Vector2i(21, 15), 100);
	TownHall* hallUndead = new TownHall("Undercity", TownHallType::UNDEAD, Vector2i(21, 15), 100);
	TownHall* hallOrcs1  = new TownHall("Orgrimmar", TownHallType::ORCS, Vector2i(40, 23), 100);
	TownHall* hallOrcs2  = new TownHall("Razor Hill", TownHallType::ORCS, Vector2i(40, 23), 100);
	m_gameObjects.push_back(hallElves);
	m_gameObjects.push_back(hallHumans);
	m_gameObjects.push_back(hallUndead);
	m_gameObjects.push_back(hallOrcs1);
	m_gameObjects.push_back(hallOrcs2);

	// Archers:
	//  - 2 Elves
	//  - 1 Human
	//  - 1 Undead
	//  - 3 Orcs
	m_gameObjects.push_back(new Archer("Tyrande Whisperwind", hallElves, Vector2i(38, 21), 30, Action::SHOOTING));
	m_gameObjects.push_back(new Archer("Malfurian Stormrage", hallElves, Vector2i(38, 21), 30, Action::SHOOTING));
	m_gameObjects.push_back(new Archer("Varian Wrynn", hallHumans, Vector2i(38, 21), 30, Action::SHOOTING));
	m_gameObjects.push_back(new Archer("Sylvanas Windrunner", hallUndead, Vector2i(38, 21), 30, Action::SHOOTING));
	m_gameObjects.push_back(new Archer("Thrall", hallOrcs1, Vector2i(38, 21), 30, Action::WALKING));
	m_gameObjects.push_back(new Archer("Vol'jin", hallOrcs1, Vector2i(38, 21), 30, Action::WALKING));
	m_gameObjects.push_back(new Archer("Orgnil Soulscar", hallOrcs2, Vector2i(38, 21), 30, Action::SHOOTING));

	//PrintState(std::cout);
}

Server::~Server()
{
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
		delete *it;
	m_gameObjects.clear();
}

void Server::RunServer()
{
	m_peerInterface = RakPeerInterface::GetInstance();

	// Setup the server socket.
	printf("Starting the server on port %d.\n", SERVER_PORT);
	SocketDescriptor sd(SERVER_PORT, 0);
	StartupResult result = m_peerInterface->Startup(MAX_CLIENTS, &sd, 1);
	m_peerInterface->SetMaximumIncomingConnections(MAX_CLIENTS);
	m_peerInterface->SetOccasionalPing(true);

	if (result != StartupResult::RAKNET_STARTED)
	{
		printf("Error: failure to start the RakNet peer interface.\n");
		return;
	}

	printf("Sever started successfully, press Ctrl+C to exit.\n\n");

	Packet *packet;

	while (true)
	{
		for (packet = m_peerInterface->Receive(); packet != NULL;
			m_peerInterface->DeallocatePacket(packet),
			packet = m_peerInterface->Receive())
		{
			ProcessPacket(packet);
		}
	}

}

void Server::SerializeState(BitStream& outStream)
{
	// Serialize all object states.
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		m_replicationManager.SerializeStateReplicationAction(outStream, *it);
	}

	m_replicationManager.SerializeEndOfReplicationActions(outStream);
}

void Server::PrintState(std::ostream& out)
{
	// Print the state of all objects.
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		out << "[Net ID " << m_replicationManager.GetLinkingContext()->GetNetworkId(*it) << "]: ";
		(*it)->PrintStateInfo(out);
	}
}

void Server::UnwrapPlaySound(BitStream& inStream)
{
	std::cout << "Play Sound!\n";
}



void Server::ProcessPacket(Packet* packet)
{
	BitStream inStream(packet->data, packet->length, false);
	inStream.IgnoreBytes(sizeof(MessageID));

	switch (packet->data[0])
	{
	case ID_REMOTE_NEW_INCOMING_CONNECTION: // A player has connected.
	case ID_NEW_INCOMING_CONNECTION:
	{
		printf("A new client has connected\n");

		BitStream outStream;
		outStream.Write((MessageID)PacketType::REPLICATION_DATA);
		SerializeState(outStream);
		m_peerInterface->Send(&outStream, HIGH_PRIORITY,
			RELIABLE_ORDERED, 0, packet->systemAddress, false);

		break;
	}
	case ID_REMOTE_DISCONNECTION_NOTIFICATION:
	case ID_REMOTE_CONNECTION_LOST:
	case ID_DISCONNECTION_NOTIFICATION:
	case ID_CONNECTION_LOST:
	{
		printf("A client has disconnected\n");
		break;
	}
	};
}

void Server::RegisterRPCs(RPCManager* rpcManager)
{
	//rpcManager->RegisterUnwrapFunction('PSND', &(this->UnwrapPlaySound));

	//BitStream bsOut;
	//bsOut.Write<uint32_t>('PSND');
	//rpcManager->ProcessRPC(bsOut);
}

