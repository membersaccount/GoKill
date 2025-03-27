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

        struct VOTE_DATA      // Type 3
        {
            int id;
            int idTarget;
        };

        struct VOTE_RESULT      // Type 4
        {
            int idKick;
        };

        struct MISSION_PROCESS_DATA      // Type 5 -> 나중에 RPC나 API 호출로 변경
        {
            int id;
            int missionId;
        };

        struct IMPOSTER_KILL        // Type 6
        {
            int id;
            int idKilled;
        };
	}
}
#pragma pack()