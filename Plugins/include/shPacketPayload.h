#pragma once

#include <cstdint>

#pragma pack(1)
namespace Packet
{
	namespace Payload
	{
		struct DEBUG	// Type 0
		{
			float x;
			float y;
			float z;
		};

		struct NEWPLAYER	// Type 1
		{
			uint32_t id;
		};

		struct MOVEMENT		// Type 2
		{
			int id;
			float x;
			float y;
			float z;
			float Pitch;
			float Yaw;
			float Roll;
			float vx;
			float vy;
			float vz;
		};
	}
}
#pragma pack()