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
	 
	UFUNCTION(BlueprintCallable)
		void SetWinText(const FString& NewWinText);

	UFUNCTION(BlueprintCallable)
		virtual void OnReadyButtonClick();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateScoreboard();

	UFUNCTION(BlueprintImplementableEvent)
		void OpenReadyScreen();



	UFUNCTION(BlueprintImplementableEvent)
		void UpdateReadiedCount(int ReadiedCount, int TotalCount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WinText = FString("Unnamed reached point X and won");

private:

	UPROPERTY()
		class AVTGameModeBase* StoredGameMode;

	UPROPERTY()
		class AVTGameStateBase* StoredGameState;

	bool FindGameState();


};
