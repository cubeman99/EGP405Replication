#ifndef _SERVER_H_
#define _SERVER_H_


#include <iostream>
#include <RakNet/BitStream.h>
#include <RakNet/RakPeerInterface.h>
#include "LinkingContext.h"
#include "RPCManager.h"
#include "ReplicationManager.h"

#define SERVER_PORT 60000
#define MAX_CLIENTS 10

class Server
{
public:
	Server();
	~Server();

	void SerializeState(RakNet::BitStream& outStream);
	void PrintState(std::ostream& out);

	void RunServer();

	void RegisterRPCs(RPCManager* rpcManager);

	void UnwrapPlaySound(RakNet::BitStream& inStream);

	void ProcessPacket(RakNet::Packet* packet);

private:
	ReplicationManager m_replicationManager;
	std::vector<GameObject*> m_gameObjects;
	RakNet::RakPeerInterface* m_peerInterface;
};


#endif // _SERVER_H_