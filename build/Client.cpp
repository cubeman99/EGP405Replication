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

void Client::SpawnObjectRPC(GameObject* obj)
{
	BitStream outStream;
	outStream.Write<MessageID>(PacketType::REPLICATION_DATA);
	outStream.Write<uint32_t('SPWN');
	outStream.Write(obj->GetClassId()
	outStream.Write<uint8_t>(ReplicationAction::INVALID);
}
