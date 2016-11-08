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


class Server : public ObjectCreationListener
{
public:
	static Server* GetInstance()
	{
		static Server instance;
		return &instance;
	}

	void RunServer();

	void RegisterRPCs(RPCManager* rpcManager);
	void RegisterObjectCreation(ObjectCreationRegistry* registry);

	void ProcessPacket(RakNet::Packet* packet);
	void SerializeState(RakNet::BitStream& outStream);
	void PrintState(std::ostream& out);
	void OnObjectCreation(GameObject* obj) override;

	static void UnwrapSpawnUnit(RakNet::BitStream& inStream);

	inline ReplicationManager* GetReplicationManager() { return &m_replicationManager; }
	
private:
	Server();
	~Server();

	ReplicationManager m_replicationManager;
	std::vector<GameObject*> m_gameObjects;
	RakNet::RakPeerInterface* m_peerInterface;
};


#endif // _SERVER_H_