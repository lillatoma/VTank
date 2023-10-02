// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "VT_PlayerState.generated.h"


/**
 * 
 */
UCLASS()
class VTANK_API AVT_PlayerState : public APlayerState
{
	GENERATED_BODY()
	


public:
	AVT_PlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
		void OnRep_UserScore();

	UFUNCTION()
		void OnRep_bReadiedAfterGameEnd();

	UFUNCTION()
		bool HasReadied() const {
		return bReadiedAfterGameEnd;
	};

protected:
	UPROPERTY(Replicated, VisibleAnywhere)
		FString Username = FString("Unnamed");

	UPROPERTY(ReplicatedUsing = OnRep_UserScore, VisibleAnywhere)
		int UserScore = 0;

	UPROPERTY(ReplicatedUsing = OnRep_bReadiedAfterGameEnd)
		bool bReadiedAfterGameEnd = false;

	bool CheckForWinningScore();

public:
	void SetReadyForNextGame();
	
	void AddScore();
	void SetUsername(FString NewName);

	FString GetUsername() const;
	int GetUserScore() const;



private:
	UPROPERTY()
		class AVTGameModeBase* StoredGameMode;

	class AVTGameModeBase* FindGameMode();
};
