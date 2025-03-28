// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace MissionData {
    struct MissionInfo {
        int32 Id;
        FString Name;
        bool Completed = false;
    };
}

typedef MissionData::MissionInfo MissionInfo;