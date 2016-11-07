#include "RPCManager.h"
#include <cassert>


void RPCManager::RegisterUnwrapFunction(uint32_t inName, RPCUnwrapFunc inFunc)
{
	assert(mNameToRPCTable.find(inName) == mNameToRPCTable.end());
	bool asd = (mNameToRPCTable.find(inName) == mNameToRPCTable.end());
	mNameToRPCTable[inName] = inFunc;
}

void RPCManager::ProcessRPC(RakNet::BitStream& inStream)
{
	uint32_t name;
	inStream.Read(name);
	mNameToRPCTable[name](inStream);
}
