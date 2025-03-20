#pragma once

namespace Character
{
	namespace State
	{
		enum class STATE
		{
			Dead,
			CrewMember,
			Imposter,
		};

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