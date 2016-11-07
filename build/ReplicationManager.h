#ifndef _REPLICATION_MANAGER_H_
#define _REPLICATION_MANAGER_H_

#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include "LinkingContext.h"
#include "ObjectCreationRegistry.h"
#include "RPCManager.h"


struct ReplicationAction
{
	enum
	{
		INVALID = 0,
		STATE = 1,
		RPC = 2,
	};
};

struct PacketType
{
	enum
	{
		UNKNOWN = ID_USER_PACKET_ENUM + 1,

		REPLICATION_DATA,
	};
};


class ObjectCreationListener
{
public:
	virtual void OnObjectCreation(GameObject* obj) = 0;
};


class ReplicationManager
{
public:
	ReplicationManager();

	inline LinkingContext*			GetLinkingContext()			{ return &m_linkingContext; }
	inline RPCManager*				GetRPCManager()				{ return &m_rpcManager; }
	inline ObjectCreationRegistry*	GetObjectCreationRegistry()	{ return &m_objectCreationRegistry; }

	void SerializeStateReplicationAction(RakNet::BitStream& outStream, GameObject* gameObject);
	void SerializeEndOfReplicationActions(RakNet::BitStream& outStream);
	void ProcessReplicationActions(RakNet::BitStream& inStream, ObjectCreationListener* listener);

private:
	LinkingContext m_linkingContext;
	RPCManager m_rpcManager;
	ObjectCreationRegistry m_objectCreationRegistry;
};


#endif // _REPLICATION_MANAGER_H_