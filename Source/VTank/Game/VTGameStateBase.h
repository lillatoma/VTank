// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "VTGameModeBase.h"
#include "VTGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class VTANK_API AVTGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual TArray<FPlayerScoreboardInfo> GetScoreboardInfo();

};
