// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "../UI/UW_Game.h"
#include "VTGameModeBase.generated.h"




/**
 * 
 */
UCLASS()
class VTANK_API AVTGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:



public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	

protected:
	virtual void BeginPlay() override; 


};
