#include "ReplicationManager.h"
#include "ObjectCreationRegistry.h"


ReplicationManager::ReplicationManager()
{

}

void ReplicationManager::SerializeStateReplicationAction(RakNet::BitStream& outStream, GameObject* obj)
{
	// Order: Network ID, Class ID, Object Data
	outStream.Write<uint8_t>(ReplicationAction::STATE);
	outStream.Write(m_linkingContext.GetNetworkId(obj));
	outStream.Write(obj->GetClassId());
	obj->Serialize(outStream, m_linkingContext);
}

void ReplicationManager::SerializeEndOfReplicationActions(RakNet::BitStream& outStream)
{
	outStream.Write<uint8_t>(ReplicationAction::INVALID);
}

void ReplicationManager::ProcessReplicationActions(RakNet::BitStream& inStream, ObjectCreationListener* listener)
{
	uint8_t replicationAction;

	while (true)
	{
		inStream.Read(replicationAction);

		if (replicationAction == ReplicationAction::STATE)
		{
			ProcessStateReplicationAction(inStream, listener);
		}
		else if (replicationAction == ReplicationAction::RPC)
		{
			m_rpcManager.ProcessRPC(inStream);
		}
		else if (replicationAction == ReplicationAction::INVALID)
		{
			break; // Marks the end of the list of replication actions.
		}
		else 
		{
			// Unknown replication action.
		}
	}
}

void ReplicationManager::ProcessStateReplicationAction(RakNet::BitStream& inStream, ObjectCreationListener* listener)
{
	uint32_t networkId;
	uint32_t classId;

	// Read the network ID.
	// A value of 0 signifies the end of the list of objects.
	inStream.Read(networkId);
	if (networkId == LinkingContext::NULL_NETWORK_ID)
		return;

	// Read the class ID.
	inStream.Read(classId);

	// Get or create the object.
	GameObject* obj = m_linkingContext.GetGameObject(networkId);
	if (obj == NULL)
	{
		obj = m_objectCreationRegistry.CreateGameObject(classId);
		m_linkingContext.AddGameObject(obj, networkId);
		if (listener != NULL)
			listener->OnObjectCreation(obj);
	}

	// Read the object-specific data.
	obj->Deserialize(inStream, m_linkingContext);
}


