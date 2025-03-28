#pragma once

#include "CoreMinimal.h"

namespace Character
{
	namespace State
	{
        UENUM()
		enum class STATE : int32
		{
			Dead,
			CrewMember,
			Imposter,
		};

        UENUM()
		enum class MISSION
		{
			NotStarted,
			InProgress,
			Completed,
		};
	}
}

typedef Character::State::STATE CharacterState;
typedef Character::State::MISSION CharacterMission;