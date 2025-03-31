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
			int id;
			int index;
		};

		struct MOVEMENT		// Type 2
		{
			int id;
			int index;
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

        struct KILL      // Type 4
        {
            int idKill;
        };		

        struct MISSION_PROCESS_DATA      // Type 5
        {
            int id;
            int missionId;
        };

        //struct KILL        // Type 6
        //{
        //    int idKilled;
        //};

		struct LEAVE_GAME	// 7
		{
			int id;
		};

		struct GAME_START	// 8
		{
			bool gameStart;
		};
	}
}
#pragma pack()