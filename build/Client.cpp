#include "Client.h"
#include "TownHall.h"
#include "Archer.h"
#include "ObjectCreationRegistry.h"
#include "Server.h"
#include <fstream>

using namespace RakNet;


Client::Client()
{
	RegisterObjectCreation(m_replicationManager.GetObjectCreationRegistry());
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

	// Receive packets for 1 second.
	for (int i = 0; i < 10; i++)
	{
		for (packet = m_peerInterface->Receive(); packet != NULL;
			m_peerInterface->DeallocatePacket(packet),
			packet = m_peerInterface->Receive())
		{
			ProcessPacket(packet);
		}

		Sleep(100);
	}

	// Disconnect from the server.
	printf("Disconnecting...\n\n");
	m_peerInterface->CloseConnection(m_serverAddress, true, 0, IMMEDIATE_PRIORITY);
	Sleep(100);

	printf("Resulting state of the world:\n\n");
	PrintState(std::cout);
	printf("\n");
	OutputStateToFile();

	printf("\n");
}


void Client::RegisterObjectCreation(ObjectCreationRegistry* registry)
{
	registry->RegisterCreationFunction<Archer>();
	registry->RegisterCreationFunction<TownHall>();
}

void Client::ProcessPacket(Packet* packet)
{
	BitStream inStream(packet->data, packet->length, false);
	inStream.IgnoreBytes(sizeof(MessageID));

	switch (packet->data[0])
	{
	case ID_CONNECTION_REQUEST_ACCEPTED:
	{
		printf("Connected request accepted.\n");

		m_serverAddress = packet->systemAddress;

		// Prompt for the IP address to connect to.
		char str[512];
		printf("\nEnter the name of a town hall to spawn: ");
		gets_s(str, sizeof(str));
		if (str[0] == 0)
		{
			strcpy_s(str, "Thunder Bluff");
		}

		printf("Sending a Spawn-Object RPC packet with the given object:\n\n");
		TownHall newTownHall(str, TownHallType::HUMANS, Vector2i(34, 60), 100);
		newTownHall.PrintStateInfo(std::cout);
		printf("\n");

		SpawnObjectRPC(&newTownHall);

		break;
	}
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
		printf("Received replication packet from server.\n");
		m_replicationManager.ProcessReplicationActions(inStream, this);
		break;
	}
	};
}

void Client::OutputStateToFile()
{
	const std::string fileName = "state.txt";

	printf("Outputting state to the file '%s'.\n", fileName.c_str());
	//PrintState(std::cout);

	std::ofstream file(fileName);
	if (file.is_open())
	{
		PrintState(file);

		file.close();
	}
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

void Client::SpawnObjectRPC(GameObject* obj)
{
	BitStream outStream;
	outStream.Write<MessageID>(PacketType::REPLICATION_DATA);

	outStream.Write<uint8_t>(ReplicationAction::RPC);
	outStream.Write<uint32_t>('SPWN');
	outStream.Write(obj->GetClassId());
	obj->Serialize(outStream, *m_replicationManager.GetLinkingContext());

	outStream.Write<uint8_t>(ReplicationAction::INVALID);

	m_peerInterface->Send(&outStream, HIGH_PRIORITY,
		RELIABLE_ORDERED, 0, m_serverAddress, false);
}

void Client::OnObjectCreation(GameObject* obj)
{
	m_gameObjects.push_back(obj);
}


