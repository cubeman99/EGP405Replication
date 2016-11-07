#ifndef _RCP_MANAGER_H_
#define _RCP_MANAGER_H_

#include <cstdint>
#include <RakNet/BitStream.h>
#include <unordered_map>


typedef void (*RPCUnwrapFunc)(RakNet::BitStream&);


class RPCManager
{
public:
	RPCManager() {}

	void RegisterUnwrapFunction(uint32_t inName, RPCUnwrapFunc inFunc);
	void ProcessRPC(RakNet::BitStream& inStream);

private:
	std::unordered_map<uint32_t, RPCUnwrapFunc> mNameToRPCTable;
};


#endif // _RCP_MANAGER_H_
