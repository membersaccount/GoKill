#pragma once

#include <cstdint>

#pragma pack(1)
namespace Packet
{
	namespace Header
	{
		struct DEFAULT
		{
			uint16_t type;
			uint16_t size;
		};
	}
}
#pragma pack()