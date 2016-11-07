#ifndef _CLIENT_H_
#define _CLIENT_H_


#include <iostream>
#include <RakNet/BitStream.h>
#include "LinkingContext.h"
#include "RPCManager.h"
#include "ReplicationManager.h"


class Client : public ObjectCreationListener
{
public:
	Client();
	~Client();

	void RunClient();

	void SerializeState(RakNet::BitStream& outStream);
	void DeserializeState(RakNet::BitStream& inStream);
	void PrintState(std::ostream& out);

	void RegisterRPCs(RPCManager* rpcManager);
	void RegisterObjectCreation(ObjectCreationRegistry* registry);

	void SpawnObjectRPC(GameObject* obj);

	void ProcessPacket(RakNet::Packet* packet);

	void OnObjectCreation(GameObject* obj) override;

	inline ReplicationManager* GetReplicationManager() { return &m_replicationManager; }

private:
	ReplicationManager m_replicationManager;
	std::vector<GameObject*> m_gameObjects;
	RakNet::RakPeerInterface* m_peerInterface;
};


#endif // _CLIENT_H_