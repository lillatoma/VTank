// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Game.generated.h"

/**
 * 
 */
UCLASS()
class VTANK_API UUW_Game : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		FString GetScoreboardText();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateScoreboard();

private:

	UPROPERTY()
		class AVTGameModeBase* StoredGameMode;

	UPROPERTY()
		class AVTGameStateBase* StoredGameState;

	bool FindGameState();
};
