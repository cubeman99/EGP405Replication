#include "Client.h"
#include "TownHall.h"
#include "Archer.h"
#include "ObjectCreationRegistry.h"
#include "Server.h"

using namespace RakNet;


Client::Client()
{
	RegisterRPCs(m_replicationManager.GetRPCManager());
	RegisterObjectCreation(m_replicationManager.GetObjectCreationRegistry());

	/*
	TownHall* hallHumans = new TownHall("Stormwind", TownHallType::HUMANS, Vector2i(21, 15), 100);
	TownHall* hallOrcs   = new TownHall("Orgrimmar", TownHallType::ORCS, Vector2i(40, 23), 100);
	TownHall* hallElves  = new TownHall("Darnassus", TownHallType::ELVES, Vector2i(40, 23), 100);
	TownHall* hallUndead = new TownHall("Undercity", TownHallType::UNDEAD, Vector2i(21, 15), 100);

	m_gameObjects.push_back(hallHumans);
	m_gameObjects.push_back(hallOrcs);
	m_gameObjects.push_back(hallElves);
	m_gameObjects.push_back(hallUndead);

	m_gameObjects.push_back(new Archer("Tyrande Whisperwind", hallElves, Vector2i(38, 21), 30, Action::SHOOTING));
	m_gameObjects.push_back(new Archer("Malfurian Stormrage", hallElves, Vector2i(38, 21), 30, Action::SHOOTING));
	m_gameObjects.push_back(new Archer("Varian Wrynn", hallHumans, Vector2i(38, 21), 30, Action::SHOOTING));
	m_gameObjects.push_back(new Archer("Thrall", hallOrcs, Vector2i(38, 21), 30, Action::SHOOTING));
	m_gameObjects.push_back(new Archer("Sylvanas Windrunner", hallUndead, Vector2i(38, 21), 30, Action::SHOOTING));

	PrintState(std::cout);

	BitStream bsOut;
	SerializeState(bsOut);

	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
		delete *it;
	m_gameObjects.clear();
	m_replicationManager.GetLinkingContext()->ClearGameObjects();

	m_replicationManager.ProcessReplicationActions(bsOut, this);
	//DeserializeState(bsOut);

	std::cout << "\n------------------------------------\n" << std::endl;
	PrintState(std::cout);
	*/
}

Client::~Client()
{
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
		delete *it;
	m_gameObjects.clear();
}

void Client::RunClient()
{
	m_peerInterface = RakPeerInterface::GetInstance();

	// Setup the client socket.
	SocketDescriptor sd;
	m_peerInterface->Startup(1, &sd, 1);
	m_peerInterface->SetOccasionalPing(true);

	// Prompt for the IP address to connect to.
	char str[512];
	printf("Enter server IP or hit enter for 127.0.0.1\n");
	gets_s(str, sizeof(str));
	if (str[0] == 0)
	{
		strcpy_s(str, "127.0.0.1");
	}

	// Connect to the server.
	printf("Starting the client.\n");
	m_peerInterface->Connect(str, SERVER_PORT, 0, 0);

	SystemAddress serverAddress;
	Packet* packet;

	while (true)
	{
		for (packet = m_peerInterface->Receive(); packet != NULL;
			m_peerInterface->DeallocatePacket(packet),
			packet = m_peerInterface->Receive())
		{
			ProcessPacket(packet);
		}

		Sleep(100);
	}
}

void Client::SerializeState(BitStream& outStream)
{
	// Serialize all object states.
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		m_replicationManager.SerializeStateReplicationAction(outStream, *it);
	}

	m_replicationManager.SerializeEndOfReplicationActions(outStream);
}

void Client::DeserializeState(BitStream& inStream)
{
	m_replicationManager.ProcessReplicationActions(inStream, this);
}

void Client::PrintState(std::ostream& out)
{
	// Print the state of all objects.
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		out << "[Net ID " << m_replicationManager.GetLinkingContext()->GetNetworkId(*it) << "]: ";
		(*it)->PrintStateInfo(out);
	}
}

void Client::UnwrapPlaySound(BitStream& inStream)
{
	std::cout << "Play Sound!\n";
}



void Client::ProcessPacket(Packet* packet)
{
	BitStream inStream(packet->data, packet->length, false);
	inStream.IgnoreBytes(sizeof(MessageID));

	switch (packet->data[0])
	{
	case ID_DISCONNECTION_NOTIFICATION:
	{
		printf("We have been disconnected.\n");
		break;
	}
	case ID_CONNECTION_LOST:
	{
		printf("Connection lost.\n");
		break;
	}
	case PacketType::REPLICATION_DATA:
	{
		m_replicationManager.ProcessReplicationActions(inStream, this);
		std::cout << "Received replication packet from server:" << std::endl << std::endl;
		PrintState(std::cout);
		break;
	}
	};
}

void Client::OnObjectCreation(GameObject* obj)
{
	m_gameObjects.push_back(obj);
}



void Client::RegisterRPCs(RPCManager* rpcManager)
{
	//rpcManager->RegisterUnwrapFunction('PSND', &(this->UnwrapPlaySound));

	//BitStream bsOut;
	//bsOut.Write<uint32_t>('PSND');
	//rpcManager->ProcessRPC(bsOut);
}

void Client::RegisterObjectCreation(ObjectCreationRegistry* registry)
{
	registry->RegisterCreationFunction<Archer>();
	registry->RegisterCreationFunction<TownHall>();
}

