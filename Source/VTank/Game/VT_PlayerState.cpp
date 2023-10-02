// Fill out your copyright notice in the Description page of Project Settings.


#include "VT_PlayerState.h"
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"
#include "VTPlayerController.h"
#include "VTGameModeBase.h"

AVT_PlayerState::AVT_PlayerState()
{
	
}

void AVT_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVT_PlayerState, Username);
	DOREPLIFETIME(AVT_PlayerState, UserScore);
	DOREPLIFETIME(AVT_PlayerState, bReadiedAfterGameEnd);
}


void AVT_PlayerState::OnRep_UserScore()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVTPlayerController::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		AVTPlayerController* AVTPC = Cast< AVTPlayerController>(FoundActors[i]);
		if(AVTPC)
			AVTPC->CallUpdateUI();
	}
}

void AVT_PlayerState::OnRep_bReadiedAfterGameEnd()
{
	int ReadiedCount = 0;
	int TotalCount = 0;
	{
		TArray<AActor*> FoundActors;
		
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVT_PlayerState::StaticClass(), FoundActors);
		TotalCount = FoundActors.Num();
		for (int i = 0; i < TotalCount; i++)
		{
			AVT_PlayerState* State = Cast< AVT_PlayerState>(FoundActors[i]);
			if (State && State->HasReadied())
				ReadiedCount++;
		}
	}


	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVTPlayerController::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		AVTPlayerController* AVTPC = Cast< AVTPlayerController>(FoundActors[i]);
		if (AVTPC)
			AVTPC->UpdateReadiedCount(ReadiedCount, TotalCount);
	}
}

void AVT_PlayerState::SetReadyForNextGame()
{
	bReadiedAfterGameEnd = true;

	bool AllReadied = true;
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVT_PlayerState::StaticClass(), FoundActors);

		for (int i = 0; i < FoundActors.Num() && AllReadied; i++)
		{
			AVT_PlayerState* State = Cast< AVT_PlayerState>(FoundActors[i]);
			if (State)
				AllReadied = AllReadied && State->HasReadied();
			else AllReadied = false;
		}
	}

	if (GetNetMode() != NM_DedicatedServer)
		OnRep_bReadiedAfterGameEnd();

	if (AllReadied)
	{
		AVTGameModeBase* GM = FindGameMode();

		if (GM)
			GM->RestartTheGame();
	}
}

void AVT_PlayerState::AddScore()
{
	if (HasAuthority())
	{
		UserScore++;

		if(GetNetMode() != NM_DedicatedServer)
			OnRep_UserScore();

		if (CheckForWinningScore())
		{
			AVTGameModeBase* GM = FindGameMode();
			if (GM)
				GM->StopGame(Username);
		}
	}
}

void AVT_PlayerState::SetUsername(FString NewName)
{
	if (HasAuthority())
	{
		Username = *NewName;
	}
}

FString AVT_PlayerState::GetUsername() const
{
	return Username;
}

int AVT_PlayerState::GetUserScore() const
{
	return UserScore;
}

AVTGameModeBase* AVT_PlayerState::FindGameMode()
{
	if (StoredGameMode)
		return StoredGameMode;

	AVTGameModeBase* GameMode = Cast<AVTGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
		StoredGameMode = GameMode;
	return GameMode;
}

bool AVT_PlayerState::CheckForWinningScore()
{
	AVTGameModeBase* GM = FindGameMode();

	if (GM && UserScore >= GM->PointsToWin)
	{
		return true;
	}
	return false;
}