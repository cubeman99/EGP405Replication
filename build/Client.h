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

	void RegisterObjectCreation(ObjectCreationRegistry* registry);

	void ProcessPacket(RakNet::Packet* packet);
	void OutputStateToFile();
	void PrintState(std::ostream& out);
	void SpawnObjectRPC(GameObject* obj);
	void OnObjectCreation(GameObject* obj) override;

	inline ReplicationManager* GetReplicationManager() { return &m_replicationManager; }

private:
	RakNet::SystemAddress m_serverAddress;
	ReplicationManager m_replicationManager;
	std::vector<GameObject*> m_gameObjects;
	RakNet::RakPeerInterface* m_peerInterface;
};


#endif // _CLIENT_H_