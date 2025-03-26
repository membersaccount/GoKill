#pragma once

#include <cstdint>

#pragma pack(1)
namespace Packet
{
	namespace Payload
	{
		struct DEBUG
		{
			float x;
			float y;
			float z;
		};

		struct NEWPLAYER
		{
			uint32_t id;
		};

		struct MOVEMENT
		{
			uint32_t id;
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